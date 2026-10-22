/* Output the generated parsing program for Bison.

   Copyright (C) 1984, 1986, 1989, 1992, 2000-2015, 2018-2022 Free
   Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include <config.h>
#include "system.h"

#include <filename.h> /* IS_PATH_WITH_DIR */
#include <get-errno.h>
#include <mbswidth.h>
#include <path-join.h>
#include <quotearg.h>
#include <spawn-pipe.h>
#include <timevar.h>
#include <wait-process.h>

#include "complain.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "muscle-tab.h"
#include "output.h"
#include "reader.h"
#include "reduce.h"
#include "scan-code.h"    /* max_left_semantic_context */
#include "scan-skel.h"
#include "symtab.h"
#include "tables.h"
#include "strversion.h"

static struct obstack format_obstack;


/*-------------------------------------------------------------------.
| Create a function NAME which associates to the muscle NAME the     |
| result of formatting the FIRST and then TABLE_DATA[BEGIN..END[ (of |
| TYPE), and to the muscle NAME_max, the max value of the            |
| TABLE_DATA.                                                        |
|                                                                    |
| For the typical case of outputting a complete table from 0, pass   |
| TABLE[0] as FIRST, and 1 as BEGIN.  For instance                   |
| muscle_insert_base_table ("pact", base, base[0], 1, nstates);      |
`-------------------------------------------------------------------*/


#define GENERATE_MUSCLE_इसिंगित_TABLE(Name, Type)                        \
                                                                        \
static void                                                             \
Name (char const *name, Type *table_data, Type first,                   \
      int begin, int end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  int j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  for (int i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      if (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      else                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      if (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      if (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_इसिंगित (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_इसिंगित_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_इसिंगित_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_इसिंगित_TABLE (muscle_इसिंगित_int_table, int)
GENERATE_MUSCLE_इसिंगित_TABLE (muscle_इसिंगित_base_table, base_नुमेरो)
GENERATE_MUSCLE_इसिंगित_TABLE (muscle_इसिंगित_rule_नुमेरो_table, rule_नुमेरो)
GENERATE_MUSCLE_इसिंगित_TABLE (muscle_इसिंगित_सिम्बोलो_नुमेरो_table, सिम्बोलो_नुमेरो)
GENERATE_MUSCLE_इसिंगित_TABLE (muscle_इसिंगित_item_नुमेरो_table, item_नुमेरो)
GENERATE_MUSCLE_इसिंगित_TABLE (muscle_इसिंगित_state_नुमेरो_table, state_नुमेरो)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

static void
output_escaped (FILE *out, const char *cp)
{
  for (; *cp; ++cp)
    switch (*cp)
      {
      case '$': fputs ("$][", out); break;
      case '@': fputs ("@@",  out); break;
      case '[': fputs ("@{",  out); break;
      case ']': fputs ("@}",  out); break;

      case 'b':
        if (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            break;
          }
        else
          goto append;

      case 'm':
        if (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            break;
          }
        else
          goto append;

      append:
      default:  fputc (*cp,   out); break;
      }
}

static void
output_quoted (FILE *out, char const *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

static void
string_output (FILE *out, char const *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
static ptrdiff_t
escape_trigraphs (char *buffer, ptrdiff_t buffersize, const char *src)
{
#define STORE(c)                                \
  do                                            \
    {                                           \
      if (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  while (0)
  ptrdiff_t res = 0;
  for (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      if (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          switch (src[i+2])
            {
            case '!': case '\'':
            case '(': case ')': case '-': case '/':
            case '<': case '=': case '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              continue;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  return res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
static char *
xescape_trigraphs (const char *src)
{
  ptrdiff_t bufsize = escape_trigraphs (NULL, 0, src);
  char *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  return buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
static const char *
सिम्बोलो_tag (const सिम्बोलो *sym)
{
  const bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  if (!eof_is_user_defined && sym->content == eoftoken->content)
    return "\"end of file\"";
  else if (sym->content == undeftoken->content)
    return "\"invalid token\"";
  else
    return sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

static void
prepare_सिम्बोलो_names (char const *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  const bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  int col = 2;
  struct quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  for (int i = 0; i < nsyms; i++)
    {
      const char *tag = सिम्बोलो_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      if (translatable)
        has_translations = true;

      char *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      int width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      if (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      if (i)
        obstack_1grow (&format_obstack, ' ');
      if (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      if (translatable)
        obstack_sgrow (&format_obstack, "])[");
      free (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  free (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_इसिंगित (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_इसिंगित_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

static void
prepare_symbols (void)
{
  MUSCLE_इसिंगित_INT ("tokens_number", ntokens);
  MUSCLE_इसिंगित_INT ("nterms_number", nnterms);
  MUSCLE_इसिंगित_INT ("symbols_number", nsyms);
  MUSCLE_इसिंगित_INT ("code_max", max_code);

  muscle_इसिंगित_सिम्बोलो_नुमेरो_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_सिम्बोलो_names ("tname");
  prepare_सिम्बोलो_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    for (int i = 0; i < ntokens; ++i)
      if (symbols[i]->translatable)
        {
          translatable = true;
          break;
        }
    if (translatable)
      {
        int *मगा_हालागा = xnmalloc (nsyms, sizeof *मगा_हालागा);
        for (int i = 0; i < ntokens; ++i)
          मगा_हालागा[i] = symbols[i]->translatable;
        muscle_इसिंगित_int_table ("translatable", मगा_हालागा,
                                 मगा_हालागा[0], 1, ntokens);
        free (मगा_हालागा);
      }
  }

  /* Output YYTOKNUM. */
  {
    int *मगा_हालागा = xnmalloc (ntokens, sizeof *मगा_हालागा);
    for (int i = 0; i < ntokens; ++i)
      मगा_हालागा[i] = symbols[i]->content->code;
    muscle_इसिंगित_int_table ("toknum", मगा_हालागा,
                             मगा_हालागा[0], 1, ntokens);
    free (मगा_हालागा);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

static void
prepare_rules (void)
{
  int *prhs = xnmalloc (nrules, sizeof *prhs);
  item_नुमेरो *rhs = xnmalloc (nritems, sizeof *rhs);
  int *rline = xnmalloc (nrules, sizeof *rline);
  सिम्बोलो_नुमेरो *r1 = xnmalloc (nrules, sizeof *r1);
  int *r2 = xnmalloc (nrules, sizeof *r2);
  int *dprec = xnmalloc (nrules, sizeof *dprec);
  int *merger = xnmalloc (nrules, sizeof *merger);
  int *immediate = xnmalloc (nrules, sizeof *immediate);

  /* Index in RHS.  */
  int i = 0;
  for (rule_नुमेरो r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      for (item_नुमेरो *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.लिन्या;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->नुमेरो_2;
      /* Length of rule R's RHS. */
      r2[r] = rule_rhs_length (&rules[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = rules[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = rules[r].merger;
      /* Immediate reduction flags (GLR).  */
      immediate[r] = rules[r].is_predicate;
    }
  aver (i == nritems);

  muscle_इसिंगित_item_नुमेरो_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_इसिंगित_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_इसिंगित_int_table ("rline", rline, 0, 0, nrules);
  muscle_इसिंगित_सिम्बोलो_नुमेरो_table ("r1", r1, 0, 0, nrules);
  muscle_इसिंगित_int_table ("r2", r2, 0, 0, nrules);
  muscle_इसिंगित_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_इसिंगित_int_table ("merger", merger, 0, 0, nrules);
  muscle_इसिंगित_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_इसिंगित_INT ("rules_number", nrules);
  MUSCLE_इसिंगित_INT ("max_left_semantic_context", max_कालिवा_semantic_context);

  free (prhs);
  free (rhs);
  free (rline);
  free (r1);
  free (r2);
  free (dprec);
  free (merger);
  free (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

static void
prepare_states (void)
{
  सिम्बोलो_नुमेरो *मगा_हालागा = xnmalloc (nstates, sizeof *मगा_हालागा);
  for (state_नुमेरो i = 0; i < nstates; ++i)
    मगा_हालागा[i] = states[i]->accessing_सिम्बोलो;
  muscle_इसिंगित_सिम्बोलो_नुमेरो_table ("stos", मगा_हालागा,
                                     0, 1, nstates);
  free (मगा_हालागा);

  MUSCLE_इसिंगित_INT ("last", माताअस);
  MUSCLE_इसिंगित_INT ("final_state_number", final_state->नुमेरो_2);
  MUSCLE_इसिंगित_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

static int
सिम्बोलो_type_name_cmp (const सिम्बोलो **lhs, const सिम्बोलो **rhs)
{
  int res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  if (!res)
    res = (*lhs)->content->नुमेरो_2 - (*rhs)->content->नुमेरो_2;
  return res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

static सिम्बोलो **
symbols_by_type_name (void)
{
  typedef int (*qcmp_type) (const void *, const void *);
  सिम्बोलो **res = xmemdup (symbols, nsyms * sizeof *res);
  qsort (res, nsyms, sizeof *res, (qcmp_type) &सिम्बोलो_type_name_cmp);
  return res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

static void
type_names_output (FILE *out)
{
  सिम्बोलो **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  for (int i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      int i0 = i;
      fputs (i ? ",\n[" : "[", out);
      for (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->नुमेरो_2);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  free (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
static void
start_symbols_output (FILE *out)
{
  if (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      for (सिम्बोलो_list *list = start_symbols; list; list = list->next)
        {
          const सिम्बोलो *start = list->content.sym;
          const सिम्बोलो *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->नुमेरो_2, swtok->content->नुमेरो_2);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

static void
सिम्बोलो_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  for (int i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

static void
rule_output (const rule *r, FILE *out)
{
  output_escaped (out, r->lhs->सिम्बोलो->tag);
  fputc (':', out);
  if (0 <= *r->rhs)
    for (item_नुमेरो *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  else
    fputs (" %empty", out);
}

static void
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  for (rule_नुमेरो r = 0; r < nrules; ++r)
    if (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        if (!no_मगा_लिन्या_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.लिन्या);
            string_output (out, map_file_name (rules[r].action_loc.start.file));
            fprintf (out, ")dnl\n");
          }
        fprintf (out, "[%*s%s]],\n[[",
                 rules[r].action_loc.start.column - 1, "",
                 rules[r].action);
        rule_output (&rules[r], out);
        fprintf (out, "]])\n\n");
      }
  fputs ("])\n\n", out);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

static void
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  int n;
  merger_list* p;
  for (n = 1, p = merge_functions; p != NULL; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->नुमेरो_2);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

static void
prepare_सिम्बोलो_definitions (void)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  for (सिम्बोलो_नुमेरो i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      const char *सुसि = obstack_finish0 (&format_obstack);
      MUSCLE_इसिंगित_INT (सुसि, nterm_map ? nterm_map[i - ntokens] : i);
    }

  for (int i = 0; i < nsyms; ++i)
    {
      सिम्बोलो *sym = symbols[i];
      const char *सुसि;

#define SET_सुसि(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      सुसि = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      सुसि = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      const char *id = सिम्बोलो_id_get (sym);
      SET_सुसि ("has_id");
      MUSCLE_इसिंगित_INT (सुसि, !!id);

      /* Its identifier.  */
      SET_सुसि ("id");
      MUSCLE_इसिंगित_STRING (सुसि, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_सुसि ("tag");
      MUSCLE_इसिंगित_STRING (सुसि, सिम्बोलो_tag (sym));

      SET_सुसि ("code");
      MUSCLE_इसिंगित_INT (सुसि, sym->content->code);

      SET_सुसि ("is_token");
      MUSCLE_इसिंगित_INT (सुसि, i < ntokens);

      SET_सुसि ("number");
      MUSCLE_इसिंगित_INT (सुसि, sym->content->नुमेरो_2);

      SET_सुसि ("has_type");
      MUSCLE_इसिंगित_INT (सुसि, !!sym->content->type_name);

      SET_सुसि ("type");
      MUSCLE_इसिंगित_STRING (सुसि, sym->content->type_name
                            ? sym->content->type_name : "");

      for (int j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          char const *pname = code_props_type_string (j) + 1;
          code_props const *p = सिम्बोलो_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_इसिंगित_INT (सुसि, !!p->code);

          if (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_इसिंगित_C_STRING (सुसि, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_इसिंगित_INT (सुसि, p->location.start.लिन्या);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (सुसि, p->location);

              SET_सुसि (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_इसिंगित (सुसि, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_सुसि
    }
}


static void
prepare_actions (void)
{
  /* Figure out the actions for the specified state.  */
  muscle_इसिंगित_rule_नुमेरो_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_इसिंगित_state_नुमेरो_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_इसिंगित_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_इसिंगित_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_इसिंगित_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_इसिंगित_base_table ("table", table,
                            table[0], 1, माताअस + 1);
  MUSCLE_इसिंगित_INT ("table_ninf", table_ninf);

  muscle_इसिंगित_base_table ("check", check,
                            check[0], 1, माताअस + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_इसिंगित_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, माताअस + 1);
  muscle_इसिंगित_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

static void
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  सिम्बोलो_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

static void
output_skeleton (void)
{
  /* Compute the names of the package data dir and skeleton files.  */
  char const *m4 = m4path ();
  char const *datadir = pkgdatadir ();
  char *skeldir = xpath_join (datadir, "skeletons");
  char *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  char *m4bison = xpath_join (skeldir, "bison.m4");
  char *traceon = xpath_join (skeldir, "traceon.m4");
  char *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  int filter_fd[2];
  pid_t pid;
  {
    char const *argv[11];
    int i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    if (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    if (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    if (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = NULL;
    aver (i <= ARRAY_CARDINALITY (argv));

    if (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        for (int j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ NULL,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  free (skeldir);
  free (m4sugar);
  free (m4bison);
  free (traceon);
  free (skel);

  if (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_इतुलाक (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, NULL);
  timevar_आलिसिन (tv_m4);
}

static void
prepare (void)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  char const *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_इतुलाक_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_इसिंगित_STRING ("version_string", VERSION);
  MUSCLE_इसिंगित_INT ("version", strversion_to_int (VERSION));
  MUSCLE_इसिंगित_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_इसिंगित_BOOL ("header_flag", header_flag);
  MUSCLE_इसिंगित_BOOL ("glr_flag", glr_parser);
  MUSCLE_इसिंगित_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_इसिंगित_BOOL ("synclines_flag", !no_मगा_लिन्या_flag);
  MUSCLE_इसिंगित_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_इसिंगित_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_इसिंगित_BOOL ("use_push_for_pull_flag", use_इतुलाक_for_pull_flag);
  MUSCLE_इसिंगित_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  if (spec_name_prefix)
    MUSCLE_इसिंगित_STRING ("prefix", spec_name_prefix);

  MUSCLE_इसिंगित_STRING ("file_name_all_but_ext", all_but_ext);

  const char *spec_mapped_header_file = map_file_name (spec_header_file);
  const char *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_इसिंगित_STRING (#Name, Name ? Name : "")
  DEFINE (dir_prefix);
  DEFINE (mapped_dir_prefix);
  DEFINE (parser_file_name);
  DEFINE (spec_header_file);
  DEFINE (spec_mapped_header_file);
  DEFINE (spec_file_prefix);
  DEFINE (spec_graph_file);
  DEFINE (spec_name_prefix);
  DEFINE (spec_outfile);
  DEFINE (spec_verbose_file);
#undef DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  if (skeleton)
    MUSCLE_इसिंगित_C_STRING ("skeleton", skeleton);
  else
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    char *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_इसिंगित_STRING_RAW ("skeletonsdir", skeldir);
    free (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

void
output (void)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_सिम्बोलो_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  if (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, NULL);
}
