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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ <filename.h> /* IS_PATH_WITH_DIR */
#অন্তর্ভাৱ <get-errno.h>
#অন্তর্ভাৱ <mbswidth.h>
#অন্তর্ভাৱ <path-join.h>
#অন্তর্ভাৱ <quotearg.h>
#অন্তর্ভাৱ <spawn-pipe.h>
#অন্তর্ভাৱ <timevar.h>
#অন্তর্ভাৱ <wait-process.h>

#অন্তর্ভাৱ "complain.h"
#অন্তর্ভাৱ "files.h"
#অন্তর্ভাৱ "getargs.h"
#অন্তর্ভাৱ "gram.h"
#অন্তর্ভাৱ "muscle-tab.h"
#অন্তর্ভাৱ "output.h"
#অন্তর্ভাৱ "reader.h"
#অন্তর্ভাৱ "reduce.h"
#অন্তর্ভাৱ "scan-code.h"    /* max_left_semantic_context */
#অন্তর্ভাৱ "scan-skel.h"
#অন্তর্ভাৱ "symtab.h"
#অন্তর্ভাৱ "tables.h"
#অন্তর্ভাৱ "strversion.h"

স্থির সংরচনা obstack format_obstack;


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


#define GENERATE_MUSCLE_ঢোকাও_TABLE(Name, Type)                        \
                                                                        \
স্থির শূন্য                                                             \
Name (ৱর্ণ ধ্রুৱ *name, Type *table_data, Type first,                   \
      পূর্ণাঙ্ক begin, পূর্ণাঙ্ক end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  পূর্ণাঙ্ক j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  বাবে (পূর্ণাঙ্ক i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      যদি (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      অন্যথা                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      যদি (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      যদি (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_ঢোকাও (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_ঢোকাও_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_ঢোকাও_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_ঢোকাও_TABLE (muscle_ঢোকাও_int_table, পূর্ণাঙ্ক)
GENERATE_MUSCLE_ঢোকাও_TABLE (muscle_ঢোকাও_base_table, base_সংখ্যা)
GENERATE_MUSCLE_ঢোকাও_TABLE (muscle_ঢোকাও_rule_সংখ্যা_table, rule_সংখ্যা)
GENERATE_MUSCLE_ঢোকাও_TABLE (muscle_ঢোকাও_প্ৰতীক_সংখ্যা_table, প্ৰতীক_সংখ্যা)
GENERATE_MUSCLE_ঢোকাও_TABLE (muscle_ঢোকাও_item_সংখ্যা_table, item_সংখ্যা)
GENERATE_MUSCLE_ঢোকাও_TABLE (muscle_ঢোকাও_state_সংখ্যা_table, state_সংখ্যা)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

স্থির শূন্য
output_escaped (FILE *out, ধ্রুৱ ৱর্ণ *cp)
{
  বাবে (; *cp; ++cp)
    ৱিকল্প (*cp)
      {
      পক্ষ '$': fputs ("$][", out); ভঙ্গ;
      পক্ষ '@': fputs ("@@",  out); ভঙ্গ;
      পক্ষ '[': fputs ("@{",  out); ভঙ্গ;
      পক্ষ ']': fputs ("@}",  out); ভঙ্গ;

      পক্ষ 'b':
        যদি (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            ভঙ্গ;
          }
        অন্যথা
          গচ্ছ append;

      পক্ষ 'm':
        যদি (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            ভঙ্গ;
          }
        অন্যথা
          গচ্ছ append;

      append:
      স্ৱভাৱ:  fputc (*cp,   out); ভঙ্গ;
      }
}

স্থির শূন্য
output_quoted (FILE *out, ৱর্ণ ধ্রুৱ *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

স্থির শূন্য
string_output (FILE *out, ৱর্ণ ধ্রুৱ *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
স্থির ptrdiff_t
escape_trigraphs (ৱর্ণ *buffer, ptrdiff_t buffersize, ধ্রুৱ ৱর্ণ *src)
{
#define STORE(c)                                \
  কৰা                                            \
    {                                           \
      যদি (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  যেতিয়ালৈ (0)
  ptrdiff_t res = 0;
  বাবে (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      যদি (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          ৱিকল্প (src[i+2])
            {
            পক্ষ '!': পক্ষ '\'':
            পক্ষ '(': পক্ষ ')': পক্ষ '-': পক্ষ '/':
            পক্ষ '<': পক্ষ '=': পক্ষ '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              অনুৱর্ত;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  ঘূৰাই res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
স্থির ৱর্ণ *
xescape_trigraphs (ধ্রুৱ ৱর্ণ *src)
{
  ptrdiff_t bufsize = escape_trigraphs (রিক্ত, 0, src);
  ৱর্ণ *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  ঘূৰাই buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
স্থির ধ্রুৱ ৱর্ণ *
প্ৰতীক_tag (ধ্রুৱ প্ৰতীক *sym)
{
  ধ্রুৱ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  যদি (!eof_is_user_defined && sym->content == eoftoken->content)
    ঘূৰাই "\"end of file\"";
  অন্যথা যদি (sym->content == undeftoken->content)
    ঘূৰাই "\"invalid token\"";
  অন্যথা
    ঘূৰাই sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

স্থির শূন্য
prepare_প্ৰতীক_names (ৱর্ণ ধ্রুৱ *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ধ্রুৱ bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  পূর্ণাঙ্ক col = 2;
  সংরচনা quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  বাবে (পূর্ণাঙ্ক i = 0; i < nsyms; i++)
    {
      ধ্রুৱ ৱর্ণ *tag = প্ৰতীক_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      যদি (translatable)
        has_translations = true;

      ৱর্ণ *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      পূর্ণাঙ্ক width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      যদি (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      যদি (i)
        obstack_1grow (&format_obstack, ' ');
      যদি (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      যদি (translatable)
        obstack_sgrow (&format_obstack, "])[");
      মুক্ত (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  মুক্ত (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_ঢোকাও (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_ঢোকাও_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

স্থির শূন্য
prepare_symbols (শূন্য)
{
  MUSCLE_ঢোকাও_INT ("tokens_number", ntokens);
  MUSCLE_ঢোকাও_INT ("nterms_number", nnterms);
  MUSCLE_ঢোকাও_INT ("symbols_number", nsyms);
  MUSCLE_ঢোকাও_INT ("code_max", max_code);

  muscle_ঢোকাও_প্ৰতীক_সংখ্যা_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_প্ৰতীক_names ("tname");
  prepare_প্ৰতীক_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    বাবে (পূর্ণাঙ্ক i = 0; i < ntokens; ++i)
      যদি (symbols[i]->translatable)
        {
          translatable = true;
          ভঙ্গ;
        }
    যদি (translatable)
      {
        পূর্ণাঙ্ক *মানসমূহ = xnmalloc (nsyms, পরিমাণ *মানসমূহ);
        বাবে (পূর্ণাঙ্ক i = 0; i < ntokens; ++i)
          মানসমূহ[i] = symbols[i]->translatable;
        muscle_ঢোকাও_int_table ("translatable", মানসমূহ,
                                 মানসমূহ[0], 1, ntokens);
        মুক্ত (মানসমূহ);
      }
  }

  /* Output YYTOKNUM. */
  {
    পূর্ণাঙ্ক *মানসমূহ = xnmalloc (ntokens, পরিমাণ *মানসমূহ);
    বাবে (পূর্ণাঙ্ক i = 0; i < ntokens; ++i)
      মানসমূহ[i] = symbols[i]->content->code;
    muscle_ঢোকাও_int_table ("toknum", মানসমূহ,
                             মানসমূহ[0], 1, ntokens);
    মুক্ত (মানসমূহ);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

স্থির শূন্য
prepare_rules (শূন্য)
{
  পূর্ণাঙ্ক *prhs = xnmalloc (nrules, পরিমাণ *prhs);
  item_সংখ্যা *rhs = xnmalloc (nritems, পরিমাণ *rhs);
  পূর্ণাঙ্ক *rline = xnmalloc (nrules, পরিমাণ *rline);
  প্ৰতীক_সংখ্যা *r1 = xnmalloc (nrules, পরিমাণ *r1);
  পূর্ণাঙ্ক *r2 = xnmalloc (nrules, পরিমাণ *r2);
  পূর্ণাঙ্ক *dprec = xnmalloc (nrules, পরিমাণ *dprec);
  পূর্ণাঙ্ক *merger = xnmalloc (nrules, পরিমাণ *merger);
  পূর্ণাঙ্ক *immediate = xnmalloc (nrules, পরিমাণ *immediate);

  /* Index in RHS.  */
  পূর্ণাঙ্ক i = 0;
  বাবে (rule_সংখ্যা r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      বাবে (item_সংখ্যা *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.সাৰি;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->সংখ্যা;
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

  muscle_ঢোকাও_item_সংখ্যা_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_ঢোকাও_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_ঢোকাও_int_table ("rline", rline, 0, 0, nrules);
  muscle_ঢোকাও_প্ৰতীক_সংখ্যা_table ("r1", r1, 0, 0, nrules);
  muscle_ঢোকাও_int_table ("r2", r2, 0, 0, nrules);
  muscle_ঢোকাও_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_ঢোকাও_int_table ("merger", merger, 0, 0, nrules);
  muscle_ঢোকাও_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_ঢোকাও_INT ("rules_number", nrules);
  MUSCLE_ঢোকাও_INT ("max_left_semantic_context", max_বাম_semantic_context);

  মুক্ত (prhs);
  মুক্ত (rhs);
  মুক্ত (rline);
  মুক্ত (r1);
  মুক্ত (r2);
  মুক্ত (dprec);
  মুক্ত (merger);
  মুক্ত (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

স্থির শূন্য
prepare_states (শূন্য)
{
  প্ৰতীক_সংখ্যা *মানসমূহ = xnmalloc (nstates, পরিমাণ *মানসমূহ);
  বাবে (state_সংখ্যা i = 0; i < nstates; ++i)
    মানসমূহ[i] = states[i]->accessing_প্ৰতীক;
  muscle_ঢোকাও_প্ৰতীক_সংখ্যা_table ("stos", মানসমূহ,
                                     0, 1, nstates);
  মুক্ত (মানসমূহ);

  MUSCLE_ঢোকাও_INT ("last", উপৰেৰ);
  MUSCLE_ঢোকাও_INT ("final_state_number", final_state->সংখ্যা);
  MUSCLE_ঢোকাও_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

স্থির পূর্ণাঙ্ক
প্ৰতীক_type_name_cmp (ধ্রুৱ প্ৰতীক **lhs, ধ্রুৱ প্ৰতীক **rhs)
{
  পূর্ণাঙ্ক res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  যদি (!res)
    res = (*lhs)->content->সংখ্যা - (*rhs)->content->সংখ্যা;
  ঘূৰাই res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

স্থির প্ৰতীক **
symbols_by_type_name (শূন্য)
{
  প্রকারনির্দেশ পূর্ণাঙ্ক (*qcmp_type) (ধ্রুৱ শূন্য *, ধ্রুৱ শূন্য *);
  প্ৰতীক **res = xmemdup (symbols, nsyms * পরিমাণ *res);
  qsort (res, nsyms, পরিমাণ *res, (qcmp_type) &প্ৰতীক_type_name_cmp);
  ঘূৰাই res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

স্থির শূন্য
type_names_output (FILE *out)
{
  প্ৰতীক **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  বাবে (পূর্ণাঙ্ক i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      পূর্ণাঙ্ক i0 = i;
      fputs (i ? ",\n[" : "[", out);
      বাবে (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->সংখ্যা);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  মুক্ত (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
স্থির শূন্য
start_symbols_output (FILE *out)
{
  যদি (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      বাবে (প্ৰতীক_list *list = start_symbols; list; list = list->next)
        {
          ধ্রুৱ প্ৰতীক *start = list->content.sym;
          ধ্রুৱ প্ৰতীক *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->সংখ্যা, swtok->content->সংখ্যা);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

স্থির শূন্য
প্ৰতীক_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  বাবে (পূর্ণাঙ্ক i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

স্থির শূন্য
rule_output (ধ্রুৱ rule *r, FILE *out)
{
  output_escaped (out, r->lhs->প্ৰতীক->tag);
  fputc (':', out);
  যদি (0 <= *r->rhs)
    বাবে (item_সংখ্যা *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  অন্যথা
    fputs (" %empty", out);
}

স্থির শূন্য
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  বাবে (rule_সংখ্যা r = 0; r < nrules; ++r)
    যদি (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        যদি (!no_সাৰিগুলি_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.সাৰি);
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

স্থির শূন্য
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  পূর্ণাঙ্ক n;
  merger_list* p;
  বাবে (n = 1, p = merge_functions; p != রিক্ত; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->সংখ্যা);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

স্থির শূন্য
prepare_প্ৰতীক_definitions (শূন্য)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  বাবে (প্ৰতীক_সংখ্যা i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ধ্রুৱ ৱর্ণ *চাবি = obstack_finish0 (&format_obstack);
      MUSCLE_ঢোকাও_INT (চাবি, nterm_map ? nterm_map[i - ntokens] : i);
    }

  বাবে (পূর্ণাঙ্ক i = 0; i < nsyms; ++i)
    {
      প্ৰতীক *sym = symbols[i];
      ধ্রুৱ ৱর্ণ *চাবি;

#define SET_চাবি(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      চাবি = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      চাবি = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ধ্রুৱ ৱর্ণ *id = প্ৰতীক_id_get (sym);
      SET_চাবি ("has_id");
      MUSCLE_ঢোকাও_INT (চাবি, !!id);

      /* Its identifier.  */
      SET_চাবি ("id");
      MUSCLE_ঢোকাও_STRING (চাবি, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_চাবি ("tag");
      MUSCLE_ঢোকাও_STRING (চাবি, প্ৰতীক_tag (sym));

      SET_চাবি ("code");
      MUSCLE_ঢোকাও_INT (চাবি, sym->content->code);

      SET_চাবি ("is_token");
      MUSCLE_ঢোকাও_INT (চাবি, i < ntokens);

      SET_চাবি ("number");
      MUSCLE_ঢোকাও_INT (চাবি, sym->content->সংখ্যা);

      SET_চাবি ("has_type");
      MUSCLE_ঢোকাও_INT (চাবি, !!sym->content->type_name);

      SET_চাবি ("type");
      MUSCLE_ঢোকাও_STRING (চাবি, sym->content->type_name
                            ? sym->content->type_name : "");

      বাবে (পূর্ণাঙ্ক j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          ৱর্ণ ধ্রুৱ *pname = code_props_type_string (j) + 1;
          code_props ধ্রুৱ *p = প্ৰতীক_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_ঢোকাও_INT (চাবি, !!p->code);

          যদি (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_ঢোকাও_C_STRING (চাবি, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_ঢোকাও_INT (চাবি, p->location.start.সাৰি);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (চাবি, p->location);

              SET_চাবি (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_ঢোকাও (চাবি, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_চাবি
    }
}


স্থির শূন্য
prepare_actions (শূন্য)
{
  /* Figure out the actions for the specified state.  */
  muscle_ঢোকাও_rule_সংখ্যা_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_ঢোকাও_state_সংখ্যা_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_ঢোকাও_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_ঢোকাও_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_ঢোকাও_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_ঢোকাও_base_table ("table", table,
                            table[0], 1, উপৰেৰ + 1);
  MUSCLE_ঢোকাও_INT ("table_ninf", table_ninf);

  muscle_ঢোকাও_base_table ("check", check,
                            check[0], 1, উপৰেৰ + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_ঢোকাও_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, উপৰেৰ + 1);
  muscle_ঢোকাও_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

স্থির শূন্য
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  প্ৰতীক_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

স্থির শূন্য
output_skeleton (শূন্য)
{
  /* Compute the names of the package data dir and skeleton files.  */
  ৱর্ণ ধ্রুৱ *m4 = m4path ();
  ৱর্ণ ধ্রুৱ *datadir = pkgdatadir ();
  ৱর্ণ *skeldir = xpath_join (datadir, "skeletons");
  ৱর্ণ *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  ৱর্ণ *m4bison = xpath_join (skeldir, "bison.m4");
  ৱর্ণ *traceon = xpath_join (skeldir, "traceon.m4");
  ৱর্ণ *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  পূর্ণাঙ্ক filter_fd[2];
  pid_t pid;
  {
    ৱর্ণ ধ্রুৱ *argv[11];
    পূর্ণাঙ্ক i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    যদি (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    যদি (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    যদি (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = রিক্ত;
    aver (i <= ARRAY_CARDINALITY (argv));

    যদি (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        বাবে (পূর্ণাঙ্ক j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ রিক্ত,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  মুক্ত (skeldir);
  মুক্ত (m4sugar);
  মুক্ত (m4bison);
  মুক্ত (traceon);
  মুক্ত (skel);

  যদি (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_ঠেলো (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, রিক্ত);
  timevar_বেৰ_কৰো (tv_m4);
}

স্থির শূন্য
prepare (শূন্য)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  ৱর্ণ ধ্রুৱ *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ঠেলো_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_ঢোকাও_STRING ("version_string", VERSION);
  MUSCLE_ঢোকাও_INT ("version", strversion_to_int (VERSION));
  MUSCLE_ঢোকাও_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_ঢোকাও_BOOL ("header_flag", header_flag);
  MUSCLE_ঢোকাও_BOOL ("glr_flag", glr_parser);
  MUSCLE_ঢোকাও_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_ঢোকাও_BOOL ("synclines_flag", !no_সাৰিগুলি_flag);
  MUSCLE_ঢোকাও_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_ঢোকাও_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_ঢোকাও_BOOL ("use_push_for_pull_flag", use_ঠেলো_for_pull_flag);
  MUSCLE_ঢোকাও_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  যদি (spec_name_prefix)
    MUSCLE_ঢোকাও_STRING ("prefix", spec_name_prefix);

  MUSCLE_ঢোকাও_STRING ("file_name_all_but_ext", all_but_ext);

  ধ্রুৱ ৱর্ণ *spec_mapped_header_file = map_file_name (spec_header_file);
  ধ্রুৱ ৱর্ণ *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_ঢোকাও_STRING (#Name, Name ? Name : "")
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
  যদি (skeleton)
    MUSCLE_ঢোকাও_C_STRING ("skeleton", skeleton);
  অন্যথা
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    ৱর্ণ *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_ঢোকাও_STRING_RAW ("skeletonsdir", skeldir);
    মুক্ত (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

শূন্য
output (শূন্য)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_প্ৰতীক_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  যদি (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, রিক্ত);
}
