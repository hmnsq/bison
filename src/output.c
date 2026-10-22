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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <filename.h> /* IS_PATH_WITH_DIR */
#અન્તર્ભાવ <get-errno.h>
#અન્તર્ભાવ <mbswidth.h>
#અન્તર્ભાવ <path-join.h>
#અન્તર્ભાવ <quotearg.h>
#અન્તર્ભાવ <spawn-pipe.h>
#અન્તર્ભાવ <timevar.h>
#અન્તર્ભાવ <wait-process.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "output.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "scan-code.h"    /* max_left_semantic_context */
#અન્તર્ભાવ "scan-skel.h"
#અન્તર્ભાવ "symtab.h"
#અન્તર્ભાવ "tables.h"
#અન્તર્ભાવ "strversion.h"

સ્થિર સંરચના obstack format_obstack;


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


#define GENERATE_MUSCLE_ઉમેરો_TABLE(Name, Type)                        \
                                                                        \
સ્થિર શૂન્ય                                                             \
Name (અક્ષર અચલ *name, Type *table_data, Type first,                   \
      પૂર્ણાંક begin, પૂર્ણાંક end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  પૂર્ણાંક j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  માટે (પૂર્ણાંક i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      જો (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      નહીંતર                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      જો (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      જો (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_ઉમેરો (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_ઉમેરો_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_ઉમેરો_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_ઉમેરો_TABLE (muscle_ઉમેરો_int_table, પૂર્ણાંક)
GENERATE_MUSCLE_ઉમેરો_TABLE (muscle_ઉમેરો_base_table, base_સંખ્યા)
GENERATE_MUSCLE_ઉમેરો_TABLE (muscle_ઉમેરો_rule_સંખ્યા_table, rule_સંખ્યા)
GENERATE_MUSCLE_ઉમેરો_TABLE (muscle_ઉમેરો_પ્રતીક_સંખ્યા_table, પ્રતીક_સંખ્યા)
GENERATE_MUSCLE_ઉમેરો_TABLE (muscle_ઉમેરો_item_સંખ્યા_table, item_સંખ્યા)
GENERATE_MUSCLE_ઉમેરો_TABLE (muscle_ઉમેરો_state_સંખ્યા_table, state_સંખ્યા)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

સ્થિર શૂન્ય
output_escaped (FILE *out, અચલ અક્ષર *cp)
{
  માટે (; *cp; ++cp)
    પસંદગી (*cp)
      {
      સ્થિતિ '$': fputs ("$][", out); તોડો;
      સ્થિતિ '@': fputs ("@@",  out); તોડો;
      સ્થિતિ '[': fputs ("@{",  out); તોડો;
      સ્થિતિ ']': fputs ("@}",  out); તોડો;

      સ્થિતિ 'b':
        જો (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            તોડો;
          }
        નહીંતર
          જાઓ append;

      સ્થિતિ 'm':
        જો (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            તોડો;
          }
        નહીંતર
          જાઓ append;

      append:
      મૂળભૂત:  fputc (*cp,   out); તોડો;
      }
}

સ્થિર શૂન્ય
output_quoted (FILE *out, અક્ષર અચલ *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

સ્થિર શૂન્ય
string_output (FILE *out, અક્ષર અચલ *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
સ્થિર ptrdiff_t
escape_trigraphs (અક્ષર *buffer, ptrdiff_t buffersize, અચલ અક્ષર *src)
{
#define STORE(c)                                \
  કરો                                            \
    {                                           \
      જો (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  જ્યારે (0)
  ptrdiff_t res = 0;
  માટે (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      જો (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          પસંદગી (src[i+2])
            {
            સ્થિતિ '!': સ્થિતિ '\'':
            સ્થિતિ '(': સ્થિતિ ')': સ્થિતિ '-': સ્થિતિ '/':
            સ્થિતિ '<': સ્થિતિ '=': સ્થિતિ '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              ચાલુ;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  પાછા res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
સ્થિર અક્ષર *
xescape_trigraphs (અચલ અક્ષર *src)
{
  ptrdiff_t bufsize = escape_trigraphs (ખાલી, 0, src);
  અક્ષર *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  પાછા buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
સ્થિર અચલ અક્ષર *
પ્રતીક_tag (અચલ પ્રતીક *sym)
{
  અચલ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  જો (!eof_is_user_defined && sym->content == eoftoken->content)
    પાછા "\"end of file\"";
  નહીંતર જો (sym->content == undeftoken->content)
    પાછા "\"invalid token\"";
  નહીંતર
    પાછા sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

સ્થિર શૂન્ય
prepare_પ્રતીક_names (અક્ષર અચલ *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  અચલ bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  પૂર્ણાંક col = 2;
  સંરચના quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  માટે (પૂર્ણાંક i = 0; i < nsyms; i++)
    {
      અચલ અક્ષર *tag = પ્રતીક_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      જો (translatable)
        has_translations = true;

      અક્ષર *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      પૂર્ણાંક width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      જો (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      જો (i)
        obstack_1grow (&format_obstack, ' ');
      જો (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      જો (translatable)
        obstack_sgrow (&format_obstack, "])[");
      મુક્ત (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  મુક્ત (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_ઉમેરો (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_ઉમેરો_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
prepare_symbols (શૂન્ય)
{
  MUSCLE_ઉમેરો_INT ("tokens_number", ntokens);
  MUSCLE_ઉમેરો_INT ("nterms_number", nnterms);
  MUSCLE_ઉમેરો_INT ("symbols_number", nsyms);
  MUSCLE_ઉમેરો_INT ("code_max", max_code);

  muscle_ઉમેરો_પ્રતીક_સંખ્યા_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_પ્રતીક_names ("tname");
  prepare_પ્રતીક_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    માટે (પૂર્ણાંક i = 0; i < ntokens; ++i)
      જો (symbols[i]->translatable)
        {
          translatable = true;
          તોડો;
        }
    જો (translatable)
      {
        પૂર્ણાંક *કિંમતો = xnmalloc (nsyms, કદ *કિંમતો);
        માટે (પૂર્ણાંક i = 0; i < ntokens; ++i)
          કિંમતો[i] = symbols[i]->translatable;
        muscle_ઉમેરો_int_table ("translatable", કિંમતો,
                                 કિંમતો[0], 1, ntokens);
        મુક્ત (કિંમતો);
      }
  }

  /* Output YYTOKNUM. */
  {
    પૂર્ણાંક *કિંમતો = xnmalloc (ntokens, કદ *કિંમતો);
    માટે (પૂર્ણાંક i = 0; i < ntokens; ++i)
      કિંમતો[i] = symbols[i]->content->code;
    muscle_ઉમેરો_int_table ("toknum", કિંમતો,
                             કિંમતો[0], 1, ntokens);
    મુક્ત (કિંમતો);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

સ્થિર શૂન્ય
prepare_rules (શૂન્ય)
{
  પૂર્ણાંક *prhs = xnmalloc (nrules, કદ *prhs);
  item_સંખ્યા *rhs = xnmalloc (nritems, કદ *rhs);
  પૂર્ણાંક *rline = xnmalloc (nrules, કદ *rline);
  પ્રતીક_સંખ્યા *r1 = xnmalloc (nrules, કદ *r1);
  પૂર્ણાંક *r2 = xnmalloc (nrules, કદ *r2);
  પૂર્ણાંક *dprec = xnmalloc (nrules, કદ *dprec);
  પૂર્ણાંક *merger = xnmalloc (nrules, કદ *merger);
  પૂર્ણાંક *immediate = xnmalloc (nrules, કદ *immediate);

  /* Index in RHS.  */
  પૂર્ણાંક i = 0;
  માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      માટે (item_સંખ્યા *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.લીટી;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->સંખ્યા;
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

  muscle_ઉમેરો_item_સંખ્યા_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_ઉમેરો_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_ઉમેરો_int_table ("rline", rline, 0, 0, nrules);
  muscle_ઉમેરો_પ્રતીક_સંખ્યા_table ("r1", r1, 0, 0, nrules);
  muscle_ઉમેરો_int_table ("r2", r2, 0, 0, nrules);
  muscle_ઉમેરો_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_ઉમેરો_int_table ("merger", merger, 0, 0, nrules);
  muscle_ઉમેરો_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_ઉમેરો_INT ("rules_number", nrules);
  MUSCLE_ઉમેરો_INT ("max_left_semantic_context", max_ડાબું_semantic_context);

  મુક્ત (prhs);
  મુક્ત (rhs);
  મુક્ત (rline);
  મુક્ત (r1);
  મુક્ત (r2);
  મુક્ત (dprec);
  મુક્ત (merger);
  મુક્ત (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

સ્થિર શૂન્ય
prepare_states (શૂન્ય)
{
  પ્રતીક_સંખ્યા *કિંમતો = xnmalloc (nstates, કદ *કિંમતો);
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    કિંમતો[i] = states[i]->accessing_પ્રતીક;
  muscle_ઉમેરો_પ્રતીક_સંખ્યા_table ("stos", કિંમતો,
                                     0, 1, nstates);
  મુક્ત (કિંમતો);

  MUSCLE_ઉમેરો_INT ("last", ઉપલું);
  MUSCLE_ઉમેરો_INT ("final_state_number", final_state->સંખ્યા);
  MUSCLE_ઉમેરો_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

સ્થિર પૂર્ણાંક
પ્રતીક_type_name_cmp (અચલ પ્રતીક **lhs, અચલ પ્રતીક **rhs)
{
  પૂર્ણાંક res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  જો (!res)
    res = (*lhs)->content->સંખ્યા - (*rhs)->content->સંખ્યા;
  પાછા res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

સ્થિર પ્રતીક **
symbols_by_type_name (શૂન્ય)
{
  પ્રકારવ્યાખ્યા પૂર્ણાંક (*qcmp_type) (અચલ શૂન્ય *, અચલ શૂન્ય *);
  પ્રતીક **res = xmemdup (symbols, nsyms * કદ *res);
  qsort (res, nsyms, કદ *res, (qcmp_type) &પ્રતીક_type_name_cmp);
  પાછા res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
type_names_output (FILE *out)
{
  પ્રતીક **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  માટે (પૂર્ણાંક i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      પૂર્ણાંક i0 = i;
      fputs (i ? ",\n[" : "[", out);
      માટે (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->સંખ્યા);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  મુક્ત (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
સ્થિર શૂન્ય
start_symbols_output (FILE *out)
{
  જો (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      માટે (પ્રતીક_list *list = start_symbols; list; list = list->next)
        {
          અચલ પ્રતીક *start = list->content.sym;
          અચલ પ્રતીક *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->સંખ્યા, swtok->content->સંખ્યા);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

સ્થિર શૂન્ય
પ્રતીક_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  માટે (પૂર્ણાંક i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

સ્થિર શૂન્ય
rule_output (અચલ rule *r, FILE *out)
{
  output_escaped (out, r->lhs->પ્રતીક->tag);
  fputc (':', out);
  જો (0 <= *r->rhs)
    માટે (item_સંખ્યા *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  નહીંતર
    fputs (" %empty", out);
}

સ્થિર શૂન્ય
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
    જો (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        જો (!no_લીટીઓ_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.લીટી);
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

સ્થિર શૂન્ય
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  પૂર્ણાંક n;
  merger_list* p;
  માટે (n = 1, p = merge_functions; p != ખાલી; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->સંખ્યા);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

સ્થિર શૂન્ય
prepare_પ્રતીક_definitions (શૂન્ય)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      અચલ અક્ષર *ચાવી = obstack_finish0 (&format_obstack);
      MUSCLE_ઉમેરો_INT (ચાવી, nterm_map ? nterm_map[i - ntokens] : i);
    }

  માટે (પૂર્ણાંક i = 0; i < nsyms; ++i)
    {
      પ્રતીક *sym = symbols[i];
      અચલ અક્ષર *ચાવી;

#define SET_ચાવી(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      ચાવી = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      ચાવી = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      અચલ અક્ષર *id = પ્રતીક_id_get (sym);
      SET_ચાવી ("has_id");
      MUSCLE_ઉમેરો_INT (ચાવી, !!id);

      /* Its identifier.  */
      SET_ચાવી ("id");
      MUSCLE_ઉમેરો_STRING (ચાવી, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_ચાવી ("tag");
      MUSCLE_ઉમેરો_STRING (ચાવી, પ્રતીક_tag (sym));

      SET_ચાવી ("code");
      MUSCLE_ઉમેરો_INT (ચાવી, sym->content->code);

      SET_ચાવી ("is_token");
      MUSCLE_ઉમેરો_INT (ચાવી, i < ntokens);

      SET_ચાવી ("number");
      MUSCLE_ઉમેરો_INT (ચાવી, sym->content->સંખ્યા);

      SET_ચાવી ("has_type");
      MUSCLE_ઉમેરો_INT (ચાવી, !!sym->content->type_name);

      SET_ચાવી ("type");
      MUSCLE_ઉમેરો_STRING (ચાવી, sym->content->type_name
                            ? sym->content->type_name : "");

      માટે (પૂર્ણાંક j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          અક્ષર અચલ *pname = code_props_type_string (j) + 1;
          code_props અચલ *p = પ્રતીક_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_ઉમેરો_INT (ચાવી, !!p->code);

          જો (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_ઉમેરો_C_STRING (ચાવી, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_ઉમેરો_INT (ચાવી, p->location.start.લીટી);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (ચાવી, p->location);

              SET_ચાવી (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_ઉમેરો (ચાવી, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_ચાવી
    }
}


સ્થિર શૂન્ય
prepare_actions (શૂન્ય)
{
  /* Figure out the actions for the specified state.  */
  muscle_ઉમેરો_rule_સંખ્યા_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_ઉમેરો_state_સંખ્યા_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_ઉમેરો_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_ઉમેરો_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_ઉમેરો_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_ઉમેરો_base_table ("table", table,
                            table[0], 1, ઉપલું + 1);
  MUSCLE_ઉમેરો_INT ("table_ninf", table_ninf);

  muscle_ઉમેરો_base_table ("check", check,
                            check[0], 1, ઉપલું + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_ઉમેરો_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, ઉપલું + 1);
  muscle_ઉમેરો_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

સ્થિર શૂન્ય
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  પ્રતીક_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

સ્થિર શૂન્ય
output_skeleton (શૂન્ય)
{
  /* Compute the names of the package data dir and skeleton files.  */
  અક્ષર અચલ *m4 = m4path ();
  અક્ષર અચલ *datadir = pkgdatadir ();
  અક્ષર *skeldir = xpath_join (datadir, "skeletons");
  અક્ષર *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  અક્ષર *m4bison = xpath_join (skeldir, "bison.m4");
  અક્ષર *traceon = xpath_join (skeldir, "traceon.m4");
  અક્ષર *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  પૂર્ણાંક filter_fd[2];
  pid_t pid;
  {
    અક્ષર અચલ *argv[11];
    પૂર્ણાંક i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    જો (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    જો (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    જો (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = ખાલી;
    aver (i <= ARRAY_CARDINALITY (argv));

    જો (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        માટે (પૂર્ણાંક j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ ખાલી,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  મુક્ત (skeldir);
  મુક્ત (m4sugar);
  મુક્ત (m4bison);
  મુક્ત (traceon);
  મુક્ત (skel);

  જો (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_ધકેલો (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, ખાલી);
  timevar_કાઢો (tv_m4);
}

સ્થિર શૂન્ય
prepare (શૂન્ય)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  અક્ષર અચલ *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ધકેલો_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_ઉમેરો_STRING ("version_string", VERSION);
  MUSCLE_ઉમેરો_INT ("version", strversion_to_int (VERSION));
  MUSCLE_ઉમેરો_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_ઉમેરો_BOOL ("header_flag", header_flag);
  MUSCLE_ઉમેરો_BOOL ("glr_flag", glr_parser);
  MUSCLE_ઉમેરો_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_ઉમેરો_BOOL ("synclines_flag", !no_લીટીઓ_flag);
  MUSCLE_ઉમેરો_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_ઉમેરો_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_ઉમેરો_BOOL ("use_push_for_pull_flag", use_ધકેલો_for_pull_flag);
  MUSCLE_ઉમેરો_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  જો (spec_name_prefix)
    MUSCLE_ઉમેરો_STRING ("prefix", spec_name_prefix);

  MUSCLE_ઉમેરો_STRING ("file_name_all_but_ext", all_but_ext);

  અચલ અક્ષર *spec_mapped_header_file = map_file_name (spec_header_file);
  અચલ અક્ષર *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_ઉમેરો_STRING (#Name, Name ? Name : "")
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
  જો (skeleton)
    MUSCLE_ઉમેરો_C_STRING ("skeleton", skeleton);
  નહીંતર
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    અક્ષર *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_ઉમેરો_STRING_RAW ("skeletonsdir", skeldir);
    મુક્ત (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

શૂન્ય
output (શૂન્ય)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_પ્રતીક_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  જો (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, ખાલી);
}
