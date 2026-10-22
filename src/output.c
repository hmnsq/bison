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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <filename.h> /* IS_PATH_WITH_DIR */
#അന്തര്ഭാവ <get-errno.h>
#അന്തര്ഭാവ <mbswidth.h>
#അന്തര്ഭാവ <path-join.h>
#അന്തര്ഭാവ <quotearg.h>
#അന്തര്ഭാവ <spawn-pipe.h>
#അന്തര്ഭാവ <timevar.h>
#അന്തര്ഭാവ <wait-process.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "output.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "scan-code.h"    /* max_left_semantic_context */
#അന്തര്ഭാവ "scan-skel.h"
#അന്തര്ഭാവ "symtab.h"
#അന്തര്ഭാവ "tables.h"
#അന്തര്ഭാവ "strversion.h"

സ്ഥിതം ഘടന obstack format_obstack;


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


#define GENERATE_MUSCLE_തിരുകുക_TABLE(Name, Type)                        \
                                                                        \
സ്ഥിതം ശൂന്യം                                                             \
Name (അക്ഷരം സ്ഥിരം *name, Type *table_data, Type first,                   \
      പൂർണ്ണസംഖ്യ begin, പൂർണ്ണസംഖ്യ end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  പൂർണ്ണസംഖ്യ j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      എങ്കിൽ (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      അല്ലെങ്കിൽ                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      എങ്കിൽ (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      എങ്കിൽ (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_തിരുകുക (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_തിരുകുക_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_തിരുകുക_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_തിരുകുക_TABLE (muscle_തിരുകുക_int_table, പൂർണ്ണസംഖ്യ)
GENERATE_MUSCLE_തിരുകുക_TABLE (muscle_തിരുകുക_base_table, base_സംഖ്യ)
GENERATE_MUSCLE_തിരുകുക_TABLE (muscle_തിരുകുക_rule_സംഖ്യ_table, rule_സംഖ്യ)
GENERATE_MUSCLE_തിരുകുക_TABLE (muscle_തിരുകുക_ചിഹ്നം_സംഖ്യ_table, ചിഹ്നം_സംഖ്യ)
GENERATE_MUSCLE_തിരുകുക_TABLE (muscle_തിരുകുക_item_സംഖ്യ_table, item_സംഖ്യ)
GENERATE_MUSCLE_തിരുകുക_TABLE (muscle_തിരുകുക_state_സംഖ്യ_table, state_സംഖ്യ)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
output_escaped (FILE *out, സ്ഥിരം അക്ഷരം *cp)
{
  വേണ്ടി (; *cp; ++cp)
    തിരഞ്ഞെടുപ്പ് (*cp)
      {
      അവസ്ഥ '$': fputs ("$][", out); നിർത്തുക;
      അവസ്ഥ '@': fputs ("@@",  out); നിർത്തുക;
      അവസ്ഥ '[': fputs ("@{",  out); നിർത്തുക;
      അവസ്ഥ ']': fputs ("@}",  out); നിർത്തുക;

      അവസ്ഥ 'b':
        എങ്കിൽ (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            നിർത്തുക;
          }
        അല്ലെങ്കിൽ
          പോകുക append;

      അവസ്ഥ 'm':
        എങ്കിൽ (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            നിർത്തുക;
          }
        അല്ലെങ്കിൽ
          പോകുക append;

      append:
      സ്ഥിരസ്ഥിതി:  fputc (*cp,   out); നിർത്തുക;
      }
}

സ്ഥിതം ശൂന്യം
output_quoted (FILE *out, അക്ഷരം സ്ഥിരം *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
string_output (FILE *out, അക്ഷരം സ്ഥിരം *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
സ്ഥിതം ptrdiff_t
escape_trigraphs (അക്ഷരം *buffer, ptrdiff_t buffersize, സ്ഥിരം അക്ഷരം *src)
{
#define STORE(c)                                \
  ചെയ്യുക                                            \
    {                                           \
      എങ്കിൽ (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  ആകുമ്പോൾ (0)
  ptrdiff_t res = 0;
  വേണ്ടി (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      എങ്കിൽ (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          തിരഞ്ഞെടുപ്പ് (src[i+2])
            {
            അവസ്ഥ '!': അവസ്ഥ '\'':
            അവസ്ഥ '(': അവസ്ഥ ')': അവസ്ഥ '-': അവസ്ഥ '/':
            അവസ്ഥ '<': അവസ്ഥ '=': അവസ്ഥ '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              തുടരുക;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  തിരികെ res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
സ്ഥിതം അക്ഷരം *
xescape_trigraphs (സ്ഥിരം അക്ഷരം *src)
{
  ptrdiff_t bufsize = escape_trigraphs (ശൂന്യ, 0, src);
  അക്ഷരം *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  തിരികെ buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
ചിഹ്നം_tag (സ്ഥിരം ചിഹ്നം *sym)
{
  സ്ഥിരം bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  എങ്കിൽ (!eof_is_user_defined && sym->content == eoftoken->content)
    തിരികെ "\"end of file\"";
  അല്ലെങ്കിൽ എങ്കിൽ (sym->content == undeftoken->content)
    തിരികെ "\"invalid token\"";
  അല്ലെങ്കിൽ
    തിരികെ sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

സ്ഥിതം ശൂന്യം
prepare_ചിഹ്നം_names (അക്ഷരം സ്ഥിരം *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  സ്ഥിരം bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  പൂർണ്ണസംഖ്യ col = 2;
  ഘടന quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; i++)
    {
      സ്ഥിരം അക്ഷരം *tag = ചിഹ്നം_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      എങ്കിൽ (translatable)
        has_translations = true;

      അക്ഷരം *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      പൂർണ്ണസംഖ്യ width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      എങ്കിൽ (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      എങ്കിൽ (i)
        obstack_1grow (&format_obstack, ' ');
      എങ്കിൽ (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      എങ്കിൽ (translatable)
        obstack_sgrow (&format_obstack, "])[");
      സ്വതന്ത്രം (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  സ്വതന്ത്രം (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_തിരുകുക (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_തിരുകുക_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
prepare_symbols (ശൂന്യം)
{
  MUSCLE_തിരുകുക_INT ("tokens_number", ntokens);
  MUSCLE_തിരുകുക_INT ("nterms_number", nnterms);
  MUSCLE_തിരുകുക_INT ("symbols_number", nsyms);
  MUSCLE_തിരുകുക_INT ("code_max", max_code);

  muscle_തിരുകുക_ചിഹ്നം_സംഖ്യ_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_ചിഹ്നം_names ("tname");
  prepare_ചിഹ്നം_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; ++i)
      എങ്കിൽ (symbols[i]->translatable)
        {
          translatable = true;
          നിർത്തുക;
        }
    എങ്കിൽ (translatable)
      {
        പൂർണ്ണസംഖ്യ *മൂല്യങ്ങൾ = xnmalloc (nsyms, വലുപ്പം *മൂല്യങ്ങൾ);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; ++i)
          മൂല്യങ്ങൾ[i] = symbols[i]->translatable;
        muscle_തിരുകുക_int_table ("translatable", മൂല്യങ്ങൾ,
                                 മൂല്യങ്ങൾ[0], 1, ntokens);
        സ്വതന്ത്രം (മൂല്യങ്ങൾ);
      }
  }

  /* Output YYTOKNUM. */
  {
    പൂർണ്ണസംഖ്യ *മൂല്യങ്ങൾ = xnmalloc (ntokens, വലുപ്പം *മൂല്യങ്ങൾ);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; ++i)
      മൂല്യങ്ങൾ[i] = symbols[i]->content->code;
    muscle_തിരുകുക_int_table ("toknum", മൂല്യങ്ങൾ,
                             മൂല്യങ്ങൾ[0], 1, ntokens);
    സ്വതന്ത്രം (മൂല്യങ്ങൾ);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
prepare_rules (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ *prhs = xnmalloc (nrules, വലുപ്പം *prhs);
  item_സംഖ്യ *rhs = xnmalloc (nritems, വലുപ്പം *rhs);
  പൂർണ്ണസംഖ്യ *rline = xnmalloc (nrules, വലുപ്പം *rline);
  ചിഹ്നം_സംഖ്യ *r1 = xnmalloc (nrules, വലുപ്പം *r1);
  പൂർണ്ണസംഖ്യ *r2 = xnmalloc (nrules, വലുപ്പം *r2);
  പൂർണ്ണസംഖ്യ *dprec = xnmalloc (nrules, വലുപ്പം *dprec);
  പൂർണ്ണസംഖ്യ *merger = xnmalloc (nrules, വലുപ്പം *merger);
  പൂർണ്ണസംഖ്യ *immediate = xnmalloc (nrules, വലുപ്പം *immediate);

  /* Index in RHS.  */
  പൂർണ്ണസംഖ്യ i = 0;
  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      വേണ്ടി (item_സംഖ്യ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.വരി;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->സംഖ്യ;
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

  muscle_തിരുകുക_item_സംഖ്യ_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_തിരുകുക_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_തിരുകുക_int_table ("rline", rline, 0, 0, nrules);
  muscle_തിരുകുക_ചിഹ്നം_സംഖ്യ_table ("r1", r1, 0, 0, nrules);
  muscle_തിരുകുക_int_table ("r2", r2, 0, 0, nrules);
  muscle_തിരുകുക_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_തിരുകുക_int_table ("merger", merger, 0, 0, nrules);
  muscle_തിരുകുക_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_തിരുകുക_INT ("rules_number", nrules);
  MUSCLE_തിരുകുക_INT ("max_left_semantic_context", max_ഇടത്_semantic_context);

  സ്വതന്ത്രം (prhs);
  സ്വതന്ത്രം (rhs);
  സ്വതന്ത്രം (rline);
  സ്വതന്ത്രം (r1);
  സ്വതന്ത്രം (r2);
  സ്വതന്ത്രം (dprec);
  സ്വതന്ത്രം (merger);
  സ്വതന്ത്രം (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

സ്ഥിതം ശൂന്യം
prepare_states (ശൂന്യം)
{
  ചിഹ്നം_സംഖ്യ *മൂല്യങ്ങൾ = xnmalloc (nstates, വലുപ്പം *മൂല്യങ്ങൾ);
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    മൂല്യങ്ങൾ[i] = states[i]->accessing_ചിഹ്നം;
  muscle_തിരുകുക_ചിഹ്നം_സംഖ്യ_table ("stos", മൂല്യങ്ങൾ,
                                     0, 1, nstates);
  സ്വതന്ത്രം (മൂല്യങ്ങൾ);

  MUSCLE_തിരുകുക_INT ("last", മുകളിലെ);
  MUSCLE_തിരുകുക_INT ("final_state_number", final_state->സംഖ്യ);
  MUSCLE_തിരുകുക_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

സ്ഥിതം പൂർണ്ണസംഖ്യ
ചിഹ്നം_type_name_cmp (സ്ഥിരം ചിഹ്നം **lhs, സ്ഥിരം ചിഹ്നം **rhs)
{
  പൂർണ്ണസംഖ്യ res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  എങ്കിൽ (!res)
    res = (*lhs)->content->സംഖ്യ - (*rhs)->content->സംഖ്യ;
  തിരികെ res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

സ്ഥിതം ചിഹ്നം **
symbols_by_type_name (ശൂന്യം)
{
  തരനിർവചനം പൂർണ്ണസംഖ്യ (*qcmp_type) (സ്ഥിരം ശൂന്യം *, സ്ഥിരം ശൂന്യം *);
  ചിഹ്നം **res = xmemdup (symbols, nsyms * വലുപ്പം *res);
  qsort (res, nsyms, വലുപ്പം *res, (qcmp_type) &ചിഹ്നം_type_name_cmp);
  തിരികെ res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
type_names_output (FILE *out)
{
  ചിഹ്നം **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      പൂർണ്ണസംഖ്യ i0 = i;
      fputs (i ? ",\n[" : "[", out);
      വേണ്ടി (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->സംഖ്യ);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  സ്വതന്ത്രം (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
സ്ഥിതം ശൂന്യം
start_symbols_output (FILE *out)
{
  എങ്കിൽ (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      വേണ്ടി (ചിഹ്നം_list *list = start_symbols; list; list = list->next)
        {
          സ്ഥിരം ചിഹ്നം *start = list->content.sym;
          സ്ഥിരം ചിഹ്നം *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->സംഖ്യ, swtok->content->സംഖ്യ);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

സ്ഥിതം ശൂന്യം
ചിഹ്നം_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

സ്ഥിതം ശൂന്യം
rule_output (സ്ഥിരം rule *r, FILE *out)
{
  output_escaped (out, r->lhs->ചിഹ്നം->tag);
  fputc (':', out);
  എങ്കിൽ (0 <= *r->rhs)
    വേണ്ടി (item_സംഖ്യ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  അല്ലെങ്കിൽ
    fputs (" %empty", out);
}

സ്ഥിതം ശൂന്യം
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; ++r)
    എങ്കിൽ (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        എങ്കിൽ (!no_വരികൾ_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.വരി);
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

സ്ഥിതം ശൂന്യം
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  പൂർണ്ണസംഖ്യ n;
  merger_list* p;
  വേണ്ടി (n = 1, p = merge_functions; p != ശൂന്യ; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->സംഖ്യ);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

സ്ഥിതം ശൂന്യം
prepare_ചിഹ്നം_definitions (ശൂന്യം)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      സ്ഥിരം അക്ഷരം *താക്കോൽ = obstack_finish0 (&format_obstack);
      MUSCLE_തിരുകുക_INT (താക്കോൽ, nterm_map ? nterm_map[i - ntokens] : i);
    }

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; ++i)
    {
      ചിഹ്നം *sym = symbols[i];
      സ്ഥിരം അക്ഷരം *താക്കോൽ;

#define SET_താക്കോൽ(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      താക്കോൽ = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      താക്കോൽ = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      സ്ഥിരം അക്ഷരം *id = ചിഹ്നം_id_get (sym);
      SET_താക്കോൽ ("has_id");
      MUSCLE_തിരുകുക_INT (താക്കോൽ, !!id);

      /* Its identifier.  */
      SET_താക്കോൽ ("id");
      MUSCLE_തിരുകുക_STRING (താക്കോൽ, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_താക്കോൽ ("tag");
      MUSCLE_തിരുകുക_STRING (താക്കോൽ, ചിഹ്നം_tag (sym));

      SET_താക്കോൽ ("code");
      MUSCLE_തിരുകുക_INT (താക്കോൽ, sym->content->code);

      SET_താക്കോൽ ("is_token");
      MUSCLE_തിരുകുക_INT (താക്കോൽ, i < ntokens);

      SET_താക്കോൽ ("number");
      MUSCLE_തിരുകുക_INT (താക്കോൽ, sym->content->സംഖ്യ);

      SET_താക്കോൽ ("has_type");
      MUSCLE_തിരുകുക_INT (താക്കോൽ, !!sym->content->type_name);

      SET_താക്കോൽ ("type");
      MUSCLE_തിരുകുക_STRING (താക്കോൽ, sym->content->type_name
                            ? sym->content->type_name : "");

      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          അക്ഷരം സ്ഥിരം *pname = code_props_type_string (j) + 1;
          code_props സ്ഥിരം *p = ചിഹ്നം_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_തിരുകുക_INT (താക്കോൽ, !!p->code);

          എങ്കിൽ (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_തിരുകുക_C_STRING (താക്കോൽ, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_തിരുകുക_INT (താക്കോൽ, p->location.start.വരി);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (താക്കോൽ, p->location);

              SET_താക്കോൽ (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_തിരുകുക (താക്കോൽ, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_താക്കോൽ
    }
}


സ്ഥിതം ശൂന്യം
prepare_actions (ശൂന്യം)
{
  /* Figure out the actions for the specified state.  */
  muscle_തിരുകുക_rule_സംഖ്യ_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_തിരുകുക_state_സംഖ്യ_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_തിരുകുക_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_തിരുകുക_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_തിരുകുക_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_തിരുകുക_base_table ("table", table,
                            table[0], 1, മുകളിലെ + 1);
  MUSCLE_തിരുകുക_INT ("table_ninf", table_ninf);

  muscle_തിരുകുക_base_table ("check", check,
                            check[0], 1, മുകളിലെ + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_തിരുകുക_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, മുകളിലെ + 1);
  muscle_തിരുകുക_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

സ്ഥിതം ശൂന്യം
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  ചിഹ്നം_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

സ്ഥിതം ശൂന്യം
output_skeleton (ശൂന്യം)
{
  /* Compute the names of the package data dir and skeleton files.  */
  അക്ഷരം സ്ഥിരം *m4 = m4path ();
  അക്ഷരം സ്ഥിരം *datadir = pkgdatadir ();
  അക്ഷരം *skeldir = xpath_join (datadir, "skeletons");
  അക്ഷരം *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  അക്ഷരം *m4bison = xpath_join (skeldir, "bison.m4");
  അക്ഷരം *traceon = xpath_join (skeldir, "traceon.m4");
  അക്ഷരം *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  പൂർണ്ണസംഖ്യ filter_fd[2];
  pid_t pid;
  {
    അക്ഷരം സ്ഥിരം *argv[11];
    പൂർണ്ണസംഖ്യ i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    എങ്കിൽ (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    എങ്കിൽ (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    എങ്കിൽ (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = ശൂന്യ;
    aver (i <= ARRAY_CARDINALITY (argv));

    എങ്കിൽ (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ ശൂന്യ,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  സ്വതന്ത്രം (skeldir);
  സ്വതന്ത്രം (m4sugar);
  സ്വതന്ത്രം (m4bison);
  സ്വതന്ത്രം (traceon);
  സ്വതന്ത്രം (skel);

  എങ്കിൽ (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_തള്ളുക (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, ശൂന്യ);
  timevar_എടുക്കുക (tv_m4);
}

സ്ഥിതം ശൂന്യം
prepare (ശൂന്യം)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  അക്ഷരം സ്ഥിരം *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_തള്ളുക_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_തിരുകുക_STRING ("version_string", VERSION);
  MUSCLE_തിരുകുക_INT ("version", strversion_to_int (VERSION));
  MUSCLE_തിരുകുക_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_തിരുകുക_BOOL ("header_flag", header_flag);
  MUSCLE_തിരുകുക_BOOL ("glr_flag", glr_parser);
  MUSCLE_തിരുകുക_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_തിരുകുക_BOOL ("synclines_flag", !no_വരികൾ_flag);
  MUSCLE_തിരുകുക_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_തിരുകുക_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_തിരുകുക_BOOL ("use_push_for_pull_flag", use_തള്ളുക_for_pull_flag);
  MUSCLE_തിരുകുക_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  എങ്കിൽ (spec_name_prefix)
    MUSCLE_തിരുകുക_STRING ("prefix", spec_name_prefix);

  MUSCLE_തിരുകുക_STRING ("file_name_all_but_ext", all_but_ext);

  സ്ഥിരം അക്ഷരം *spec_mapped_header_file = map_file_name (spec_header_file);
  സ്ഥിരം അക്ഷരം *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_തിരുകുക_STRING (#Name, Name ? Name : "")
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
  എങ്കിൽ (skeleton)
    MUSCLE_തിരുകുക_C_STRING ("skeleton", skeleton);
  അല്ലെങ്കിൽ
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    അക്ഷരം *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_തിരുകുക_STRING_RAW ("skeletonsdir", skeldir);
    സ്വതന്ത്രം (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

ശൂന്യം
output (ശൂന്യം)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_ചിഹ്നം_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  എങ്കിൽ (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, ശൂന്യ);
}
