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

static sa_nracanaa obstack format_obstack;


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


#define GENERATE_MUSCLE_INSERT_TABLE(Name, Type)                        \
                                                                        \
static void                                                             \
Name (var_na const *name, Type *table_data, Type first,                   \
      puur_naa_nka begin, puur_naa_nka end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  puur_naa_nka j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      ꦪꦼꦤ꧀ (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      ꦭꦶꦪ                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      ꦪꦼꦤ꧀ (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      ꦪꦼꦤ꧀ (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_insert (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_INSERT_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_INSERT_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_INSERT_TABLE (muscle_insert_int_table, puur_naa_nka)
GENERATE_MUSCLE_INSERT_TABLE (muscle_insert_base_table, base_number)
GENERATE_MUSCLE_INSERT_TABLE (muscle_insert_rule_number_table, rule_number)
GENERATE_MUSCLE_INSERT_TABLE (muscle_insert_symbol_number_table, symbol_number)
GENERATE_MUSCLE_INSERT_TABLE (muscle_insert_item_number_table, item_number)
GENERATE_MUSCLE_INSERT_TABLE (muscle_insert_state_number_table, state_number)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

static void
output_escaped (FILE *out, const var_na *cp)
{
  ꦏꦁꦒꦺꦴ (; *cp; ++cp)
    switch (*cp)
      {
      pak_sa '$': fputs ("$][", out); bha_nga;
      pak_sa '@': fputs ("@@",  out); bha_nga;
      pak_sa '[': fputs ("@{",  out); bha_nga;
      pak_sa ']': fputs ("@}",  out); bha_nga;

      pak_sa 'b':
        ꦪꦼꦤ꧀ (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            bha_nga;
          }
        ꦭꦶꦪ
          goto append;

      pak_sa 'm':
        ꦪꦼꦤ꧀ (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            bha_nga;
          }
        ꦭꦶꦪ
          goto append;

      append:
      default:  fputc (*cp,   out); bha_nga;
      }
}

static void
output_quoted (FILE *out, var_na const *cp)
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
string_output (FILE *out, var_na const *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
static ptrdiff_t
escape_trigraphs (var_na *buffer, ptrdiff_t buffersize, const var_na *src)
{
#define STORE(c)                                \
  do                                            \
    {                                           \
      ꦪꦼꦤ꧀ (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  ꦱꦼꦢꦼꦭ (0)
  ptrdiff_t res = 0;
  ꦏꦁꦒꦺꦴ (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      ꦪꦼꦤ꧀ (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          switch (src[i+2])
            {
            pak_sa '!': pak_sa '\'':
            pak_sa '(': pak_sa ')': pak_sa '-': pak_sa '/':
            pak_sa '<': pak_sa '=': pak_sa '>':
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
  ꦮꦭꦶ res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
static var_na *
xescape_trigraphs (const var_na *src)
{
  ptrdiff_t bufsize = escape_trigraphs (NULL, 0, src);
  var_na *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  ꦮꦭꦶ buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
static const var_na *
symbol_tag (const symbol *sym)
{
  const bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  ꦪꦼꦤ꧀ (!eof_is_user_defined && sym->content == eoftoken->content)
    ꦮꦭꦶ "\"end of file\"";
  ꦭꦶꦪ ꦪꦼꦤ꧀ (sym->content == undeftoken->content)
    ꦮꦭꦶ "\"invalid token\"";
  ꦭꦶꦪ
    ꦮꦭꦶ sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

static void
prepare_symbol_names (var_na const *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  const bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  puur_naa_nka col = 2;
  sa_nracanaa quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < nsyms; i++)
    {
      const var_na *tag = symbol_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      ꦪꦼꦤ꧀ (translatable)
        has_translations = true;

      var_na *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      puur_naa_nka width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      ꦪꦼꦤ꧀ (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      ꦪꦼꦤ꧀ (i)
        obstack_1grow (&format_obstack, ' ');
      ꦪꦼꦤ꧀ (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      ꦪꦼꦤ꧀ (translatable)
        obstack_sgrow (&format_obstack, "])[");
      free (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  free (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_insert (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_INSERT_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

static void
prepare_symbols (void)
{
  MUSCLE_INSERT_INT ("tokens_number", ntokens);
  MUSCLE_INSERT_INT ("nterms_number", nnterms);
  MUSCLE_INSERT_INT ("symbols_number", nsyms);
  MUSCLE_INSERT_INT ("code_max", max_code);

  muscle_insert_symbol_number_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_symbol_names ("tname");
  prepare_symbol_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < ntokens; ++i)
      ꦪꦼꦤ꧀ (symbols[i]->translatable)
        {
          translatable = true;
          bha_nga;
        }
    ꦪꦼꦤ꧀ (translatable)
      {
        puur_naa_nka *values = xnmalloc (nsyms, parimaa_na *values);
        ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < ntokens; ++i)
          values[i] = symbols[i]->translatable;
        muscle_insert_int_table ("translatable", values,
                                 values[0], 1, ntokens);
        free (values);
      }
  }

  /* Output YYTOKNUM. */
  {
    puur_naa_nka *values = xnmalloc (ntokens, parimaa_na *values);
    ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < ntokens; ++i)
      values[i] = symbols[i]->content->code;
    muscle_insert_int_table ("toknum", values,
                             values[0], 1, ntokens);
    free (values);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

static void
prepare_rules (void)
{
  puur_naa_nka *prhs = xnmalloc (nrules, parimaa_na *prhs);
  item_number *rhs = xnmalloc (nritems, parimaa_na *rhs);
  puur_naa_nka *rline = xnmalloc (nrules, parimaa_na *rline);
  symbol_number *r1 = xnmalloc (nrules, parimaa_na *r1);
  puur_naa_nka *r2 = xnmalloc (nrules, parimaa_na *r2);
  puur_naa_nka *dprec = xnmalloc (nrules, parimaa_na *dprec);
  puur_naa_nka *merger = xnmalloc (nrules, parimaa_na *merger);
  puur_naa_nka *immediate = xnmalloc (nrules, parimaa_na *immediate);

  /* Index in RHS.  */
  puur_naa_nka i = 0;
  ꦏꦁꦒꦺꦴ (rule_number r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      ꦏꦁꦒꦺꦴ (item_number *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.line;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->number;
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

  muscle_insert_item_number_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_insert_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_insert_int_table ("rline", rline, 0, 0, nrules);
  muscle_insert_symbol_number_table ("r1", r1, 0, 0, nrules);
  muscle_insert_int_table ("r2", r2, 0, 0, nrules);
  muscle_insert_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_insert_int_table ("merger", merger, 0, 0, nrules);
  muscle_insert_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_INSERT_INT ("rules_number", nrules);
  MUSCLE_INSERT_INT ("max_left_semantic_context", max_left_semantic_context);

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
  symbol_number *values = xnmalloc (nstates, parimaa_na *values);
  ꦏꦁꦒꦺꦴ (state_number i = 0; i < nstates; ++i)
    values[i] = states[i]->accessing_symbol;
  muscle_insert_symbol_number_table ("stos", values,
                                     0, 1, nstates);
  free (values);

  MUSCLE_INSERT_INT ("last", high);
  MUSCLE_INSERT_INT ("final_state_number", final_state->number);
  MUSCLE_INSERT_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

static puur_naa_nka
symbol_type_name_cmp (const symbol **lhs, const symbol **rhs)
{
  puur_naa_nka res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  ꦪꦼꦤ꧀ (!res)
    res = (*lhs)->content->number - (*rhs)->content->number;
  ꦮꦭꦶ res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

static symbol **
symbols_by_type_name (void)
{
  typedef puur_naa_nka (*qcmp_type) (const void *, const void *);
  symbol **res = xmemdup (symbols, nsyms * parimaa_na *res);
  qsort (res, nsyms, parimaa_na *res, (qcmp_type) &symbol_type_name_cmp);
  ꦮꦭꦶ res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

static void
type_names_output (FILE *out)
{
  symbol **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      puur_naa_nka i0 = i;
      fputs (i ? ",\n[" : "[", out);
      ꦏꦁꦒꦺꦴ (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->number);
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
  ꦪꦼꦤ꧀ (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      ꦏꦁꦒꦺꦴ (symbol_list *list = start_symbols; list; list = list->next)
        {
          const symbol *start = list->content.sym;
          const symbol *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->number, swtok->content->number);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

static void
symbol_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

static void
rule_output (const rule *r, FILE *out)
{
  output_escaped (out, r->lhs->symbol->tag);
  fputc (':', out);
  ꦪꦼꦤ꧀ (0 <= *r->rhs)
    ꦏꦁꦒꦺꦴ (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  ꦭꦶꦪ
    fputs (" %empty", out);
}

static void
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  ꦏꦁꦒꦺꦴ (rule_number r = 0; r < nrules; ++r)
    ꦪꦼꦤ꧀ (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        ꦪꦼꦤ꧀ (!no_lines_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.line);
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
  puur_naa_nka n;
  merger_list* p;
  ꦏꦁꦒꦺꦴ (n = 1, p = merge_functions; p != NULL; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->number);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

static void
prepare_symbol_definitions (void)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  ꦏꦁꦒꦺꦴ (symbol_number i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      const var_na *key = obstack_finish0 (&format_obstack);
      MUSCLE_INSERT_INT (key, nterm_map ? nterm_map[i - ntokens] : i);
    }

  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < nsyms; ++i)
    {
      symbol *sym = symbols[i];
      const var_na *key;

#define SET_KEY(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      key = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      key = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      const var_na *id = symbol_id_get (sym);
      SET_KEY ("has_id");
      MUSCLE_INSERT_INT (key, !!id);

      /* Its identifier.  */
      SET_KEY ("id");
      MUSCLE_INSERT_STRING (key, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_KEY ("tag");
      MUSCLE_INSERT_STRING (key, symbol_tag (sym));

      SET_KEY ("code");
      MUSCLE_INSERT_INT (key, sym->content->code);

      SET_KEY ("is_token");
      MUSCLE_INSERT_INT (key, i < ntokens);

      SET_KEY ("number");
      MUSCLE_INSERT_INT (key, sym->content->number);

      SET_KEY ("has_type");
      MUSCLE_INSERT_INT (key, !!sym->content->type_name);

      SET_KEY ("type");
      MUSCLE_INSERT_STRING (key, sym->content->type_name
                            ? sym->content->type_name : "");

      ꦏꦁꦒꦺꦴ (puur_naa_nka j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          var_na const *pname = code_props_type_string (j) + 1;
          code_props const *p = symbol_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_INSERT_INT (key, !!p->code);

          ꦪꦼꦤ꧀ (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_INSERT_C_STRING (key, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_INSERT_INT (key, p->location.start.line);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (key, p->location);

              SET_KEY (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_insert (key, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_KEY
    }
}


static void
prepare_actions (void)
{
  /* Figure out the actions for the specified state.  */
  muscle_insert_rule_number_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_insert_state_number_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_insert_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_INSERT_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_insert_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_insert_base_table ("table", table,
                            table[0], 1, high + 1);
  MUSCLE_INSERT_INT ("table_ninf", table_ninf);

  muscle_insert_base_table ("check", check,
                            check[0], 1, high + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_insert_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, high + 1);
  muscle_insert_int_table ("conflicting_rules", conflict_list,
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
  symbol_numbers_output (out);
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
  var_na const *m4 = m4path ();
  var_na const *datadir = pkgdatadir ();
  var_na *skeldir = xpath_join (datadir, "skeletons");
  var_na *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  var_na *m4bison = xpath_join (skeldir, "bison.m4");
  var_na *traceon = xpath_join (skeldir, "traceon.m4");
  var_na *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  puur_naa_nka filter_fd[2];
  pid_t pid;
  {
    var_na const *argv[11];
    puur_naa_nka i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ꦪꦼꦤ꧀ (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ꦪꦼꦤ꧀ (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    ꦪꦼꦤ꧀ (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = NULL;
    aver (i <= ARRAY_CARDINALITY (argv));

    ꦪꦼꦤ꧀ (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        ꦏꦁꦒꦺꦴ (puur_naa_nka j = 0; argv[j]; ++j)
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

  ꦪꦼꦤ꧀ (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_push (tv_m4);
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
  timevar_pop (tv_m4);
}

static void
prepare (void)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  var_na const *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_push_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_INSERT_STRING ("version_string", VERSION);
  MUSCLE_INSERT_INT ("version", strversion_to_int (VERSION));
  MUSCLE_INSERT_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_INSERT_BOOL ("header_flag", header_flag);
  MUSCLE_INSERT_BOOL ("glr_flag", glr_parser);
  MUSCLE_INSERT_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_INSERT_BOOL ("synclines_flag", !no_lines_flag);
  MUSCLE_INSERT_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_INSERT_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_INSERT_BOOL ("use_push_for_pull_flag", use_push_for_pull_flag);
  MUSCLE_INSERT_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  ꦪꦼꦤ꧀ (spec_name_prefix)
    MUSCLE_INSERT_STRING ("prefix", spec_name_prefix);

  MUSCLE_INSERT_STRING ("file_name_all_but_ext", all_but_ext);

  const var_na *spec_mapped_header_file = map_file_name (spec_header_file);
  const var_na *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_INSERT_STRING (#Name, Name ? Name : "")
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
  ꦪꦼꦤ꧀ (skeleton)
    MUSCLE_INSERT_C_STRING ("skeleton", skeleton);
  ꦭꦶꦪ
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    var_na *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_INSERT_STRING_RAW ("skeletonsdir", skeldir);
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
  prepare_symbol_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  ꦪꦼꦤ꧀ (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, NULL);
}
