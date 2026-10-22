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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <filename.h> /* IS_PATH_WITH_DIR */
#ଅନ୍ତର୍ଭାଵ <get-errno.h>
#ଅନ୍ତର୍ଭାଵ <mbswidth.h>
#ଅନ୍ତର୍ଭାଵ <path-join.h>
#ଅନ୍ତର୍ଭାଵ <quotearg.h>
#ଅନ୍ତର୍ଭାଵ <spawn-pipe.h>
#ଅନ୍ତର୍ଭାଵ <timevar.h>
#ଅନ୍ତର୍ଭାଵ <wait-process.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "output.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "scan-code.h"    /* max_left_semantic_context */
#ଅନ୍ତର୍ଭାଵ "scan-skel.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"
#ଅନ୍ତର୍ଭାଵ "tables.h"
#ଅନ୍ତର୍ଭାଵ "strversion.h"

ସ୍ଥିର ସଂରଚନା obstack format_obstack;


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


#define GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE(Name, Type)                        \
                                                                        \
ସ୍ଥିର ଶୂନ୍ୟ                                                             \
Name (ଅକ୍ଷର ଅଚଳ *name, Type *table_data, Type first,                   \
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା begin, ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      ଯଦି (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      ଅନ୍ୟଥା                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      ଯଦି (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      ଯଦି (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_ଭର୍ତ୍ତି (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_ଭର୍ତ୍ତି_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_ଭର୍ତ୍ତି_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE (muscle_ଭର୍ତ୍ତି_int_table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE (muscle_ଭର୍ତ୍ତି_base_table, base_ସଂଖ୍ୟା)
GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE (muscle_ଭର୍ତ୍ତି_rule_ସଂଖ୍ୟା_table, rule_ସଂଖ୍ୟା)
GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE (muscle_ଭର୍ତ୍ତି_ପ୍ରତୀକ_ସଂଖ୍ୟା_table, ପ୍ରତୀକ_ସଂଖ୍ୟା)
GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE (muscle_ଭର୍ତ୍ତି_item_ସଂଖ୍ୟା_table, item_ସଂଖ୍ୟା)
GENERATE_MUSCLE_ଭର୍ତ୍ତି_TABLE (muscle_ଭର୍ତ୍ତି_state_ସଂଖ୍ୟା_table, state_ସଂଖ୍ୟା)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
output_escaped (FILE *out, ଅଚଳ ଅକ୍ଷର *cp)
{
  ପାଇଁ (; *cp; ++cp)
    ବାଛ (*cp)
      {
      ସ୍ଥିତି '$': fputs ("$][", out); ଭାଙ୍ଗ;
      ସ୍ଥିତି '@': fputs ("@@",  out); ଭାଙ୍ଗ;
      ସ୍ଥିତି '[': fputs ("@{",  out); ଭାଙ୍ଗ;
      ସ୍ଥିତି ']': fputs ("@}",  out); ଭାଙ୍ଗ;

      ସ୍ଥିତି 'b':
        ଯଦି (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            ଭାଙ୍ଗ;
          }
        ଅନ୍ୟଥା
          ଯାଅ append;

      ସ୍ଥିତି 'm':
        ଯଦି (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            ଭାଙ୍ଗ;
          }
        ଅନ୍ୟଥା
          ଯାଅ append;

      append:
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:  fputc (*cp,   out); ଭାଙ୍ଗ;
      }
}

ସ୍ଥିର ଶୂନ୍ୟ
output_quoted (FILE *out, ଅକ୍ଷର ଅଚଳ *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
string_output (FILE *out, ଅକ୍ଷର ଅଚଳ *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ସ୍ଥିର ptrdiff_t
escape_trigraphs (ଅକ୍ଷର *buffer, ptrdiff_t buffersize, ଅଚଳ ଅକ୍ଷର *src)
{
#define STORE(c)                                \
  କର                                            \
    {                                           \
      ଯଦି (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  ଯେତେବେଳେ (0)
  ptrdiff_t res = 0;
  ପାଇଁ (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      ଯଦି (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          ବାଛ (src[i+2])
            {
            ସ୍ଥିତି '!': ସ୍ଥିତି '\'':
            ସ୍ଥିତି '(': ସ୍ଥିତି ')': ସ୍ଥିତି '-': ସ୍ଥିତି '/':
            ସ୍ଥିତି '<': ସ୍ଥିତି '=': ସ୍ଥିତି '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              ଜାରି;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  ଫେରନ୍ତୁ res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
ସ୍ଥିର ଅକ୍ଷର *
xescape_trigraphs (ଅଚଳ ଅକ୍ଷର *src)
{
  ptrdiff_t bufsize = escape_trigraphs (ଖାଲି, 0, src);
  ଅକ୍ଷର *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  ଫେରନ୍ତୁ buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
ପ୍ରତୀକ_tag (ଅଚଳ ପ୍ରତୀକ *sym)
{
  ଅଚଳ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  ଯଦି (!eof_is_user_defined && sym->content == eoftoken->content)
    ଫେରନ୍ତୁ "\"end of file\"";
  ଅନ୍ୟଥା ଯଦି (sym->content == undeftoken->content)
    ଫେରନ୍ତୁ "\"invalid token\"";
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ସ୍ଥିର ଶୂନ୍ୟ
prepare_ପ୍ରତୀକ_names (ଅକ୍ଷର ଅଚଳ *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ଅଚଳ bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା col = 2;
  ସଂରଚନା quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; i++)
    {
      ଅଚଳ ଅକ୍ଷର *tag = ପ୍ରତୀକ_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      ଯଦି (translatable)
        has_translations = true;

      ଅକ୍ଷର *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      ଯଦି (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      ଯଦି (i)
        obstack_1grow (&format_obstack, ' ');
      ଯଦି (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      ଯଦି (translatable)
        obstack_sgrow (&format_obstack, "])[");
      ମୁକ୍ତ (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  ମୁକ୍ତ (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_ଭର୍ତ୍ତି (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
prepare_symbols (ଶୂନ୍ୟ)
{
  MUSCLE_ଭର୍ତ୍ତି_INT ("tokens_number", ntokens);
  MUSCLE_ଭର୍ତ୍ତି_INT ("nterms_number", nnterms);
  MUSCLE_ଭର୍ତ୍ତି_INT ("symbols_number", nsyms);
  MUSCLE_ଭର୍ତ୍ତି_INT ("code_max", max_code);

  muscle_ଭର୍ତ୍ତି_ପ୍ରତୀକ_ସଂଖ୍ୟା_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_ପ୍ରତୀକ_names ("tname");
  prepare_ପ୍ରତୀକ_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; ++i)
      ଯଦି (symbols[i]->translatable)
        {
          translatable = true;
          ଭାଙ୍ଗ;
        }
    ଯଦି (translatable)
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ମୂଲ୍ୟଗୁଡ଼ିକ = xnmalloc (nsyms, ଆକାର *ମୂଲ୍ୟଗୁଡ଼ିକ);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; ++i)
          ମୂଲ୍ୟଗୁଡ଼ିକ[i] = symbols[i]->translatable;
        muscle_ଭର୍ତ୍ତି_int_table ("translatable", ମୂଲ୍ୟଗୁଡ଼ିକ,
                                 ମୂଲ୍ୟଗୁଡ଼ିକ[0], 1, ntokens);
        ମୁକ୍ତ (ମୂଲ୍ୟଗୁଡ଼ିକ);
      }
  }

  /* Output YYTOKNUM. */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ମୂଲ୍ୟଗୁଡ଼ିକ = xnmalloc (ntokens, ଆକାର *ମୂଲ୍ୟଗୁଡ଼ିକ);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; ++i)
      ମୂଲ୍ୟଗୁଡ଼ିକ[i] = symbols[i]->content->code;
    muscle_ଭର୍ତ୍ତି_int_table ("toknum", ମୂଲ୍ୟଗୁଡ଼ିକ,
                             ମୂଲ୍ୟଗୁଡ଼ିକ[0], 1, ntokens);
    ମୁକ୍ତ (ମୂଲ୍ୟଗୁଡ଼ିକ);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
prepare_rules (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *prhs = xnmalloc (nrules, ଆକାର *prhs);
  item_ସଂଖ୍ୟା *rhs = xnmalloc (nritems, ଆକାର *rhs);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rline = xnmalloc (nrules, ଆକାର *rline);
  ପ୍ରତୀକ_ସଂଖ୍ୟା *r1 = xnmalloc (nrules, ଆକାର *r1);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *r2 = xnmalloc (nrules, ଆକାର *r2);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dprec = xnmalloc (nrules, ଆକାର *dprec);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *merger = xnmalloc (nrules, ଆକାର *merger);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *immediate = xnmalloc (nrules, ଆକାର *immediate);

  /* Index in RHS.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.ଧାଡ଼ି;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->ସଂଖ୍ୟା;
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

  muscle_ଭର୍ତ୍ତି_item_ସଂଖ୍ୟା_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_ଭର୍ତ୍ତି_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_ଭର୍ତ୍ତି_int_table ("rline", rline, 0, 0, nrules);
  muscle_ଭର୍ତ୍ତି_ପ୍ରତୀକ_ସଂଖ୍ୟା_table ("r1", r1, 0, 0, nrules);
  muscle_ଭର୍ତ୍ତି_int_table ("r2", r2, 0, 0, nrules);
  muscle_ଭର୍ତ୍ତି_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_ଭର୍ତ୍ତି_int_table ("merger", merger, 0, 0, nrules);
  muscle_ଭର୍ତ୍ତି_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_ଭର୍ତ୍ତି_INT ("rules_number", nrules);
  MUSCLE_ଭର୍ତ୍ତି_INT ("max_left_semantic_context", max_ବାମ_semantic_context);

  ମୁକ୍ତ (prhs);
  ମୁକ୍ତ (rhs);
  ମୁକ୍ତ (rline);
  ମୁକ୍ତ (r1);
  ମୁକ୍ତ (r2);
  ମୁକ୍ତ (dprec);
  ମୁକ୍ତ (merger);
  ମୁକ୍ତ (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
prepare_states (ଶୂନ୍ୟ)
{
  ପ୍ରତୀକ_ସଂଖ୍ୟା *ମୂଲ୍ୟଗୁଡ଼ିକ = xnmalloc (nstates, ଆକାର *ମୂଲ୍ୟଗୁଡ଼ିକ);
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    ମୂଲ୍ୟଗୁଡ଼ିକ[i] = states[i]->accessing_ପ୍ରତୀକ;
  muscle_ଭର୍ତ୍ତି_ପ୍ରତୀକ_ସଂଖ୍ୟା_table ("stos", ମୂଲ୍ୟଗୁଡ଼ିକ,
                                     0, 1, nstates);
  ମୁକ୍ତ (ମୂଲ୍ୟଗୁଡ଼ିକ);

  MUSCLE_ଭର୍ତ୍ତି_INT ("last", ଉପର);
  MUSCLE_ଭର୍ତ୍ତି_INT ("final_state_number", final_state->ସଂଖ୍ୟା);
  MUSCLE_ଭର୍ତ୍ତି_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ପ୍ରତୀକ_type_name_cmp (ଅଚଳ ପ୍ରତୀକ **lhs, ଅଚଳ ପ୍ରତୀକ **rhs)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  ଯଦି (!res)
    res = (*lhs)->content->ସଂଖ୍ୟା - (*rhs)->content->ସଂଖ୍ୟା;
  ଫେରନ୍ତୁ res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ସ୍ଥିର ପ୍ରତୀକ **
symbols_by_type_name (ଶୂନ୍ୟ)
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*qcmp_type) (ଅଚଳ ଶୂନ୍ୟ *, ଅଚଳ ଶୂନ୍ୟ *);
  ପ୍ରତୀକ **res = xmemdup (symbols, nsyms * ଆକାର *res);
  qsort (res, nsyms, ଆକାର *res, (qcmp_type) &ପ୍ରତୀକ_type_name_cmp);
  ଫେରନ୍ତୁ res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
type_names_output (FILE *out)
{
  ପ୍ରତୀକ **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0 = i;
      fputs (i ? ",\n[" : "[", out);
      ପାଇଁ (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->ସଂଖ୍ୟା);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  ମୁକ୍ତ (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ସ୍ଥିର ଶୂନ୍ୟ
start_symbols_output (FILE *out)
{
  ଯଦି (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      ପାଇଁ (ପ୍ରତୀକ_list *list = start_symbols; list; list = list->next)
        {
          ଅଚଳ ପ୍ରତୀକ *start = list->content.sym;
          ଅଚଳ ପ୍ରତୀକ *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->ସଂଖ୍ୟା, swtok->content->ସଂଖ୍ୟା);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
ପ୍ରତୀକ_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
rule_output (ଅଚଳ rule *r, FILE *out)
{
  output_escaped (out, r->lhs->ପ୍ରତୀକ->tag);
  fputc (':', out);
  ଯଦି (0 <= *r->rhs)
    ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  ଅନ୍ୟଥା
    fputs (" %empty", out);
}

ସ୍ଥିର ଶୂନ୍ୟ
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
    ଯଦି (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        ଯଦି (!no_ଧାଡ଼ିଗୁଡ଼ିକ_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.ଧାଡ଼ି);
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

ସ୍ଥିର ଶୂନ୍ୟ
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;
  merger_list* p;
  ପାଇଁ (n = 1, p = merge_functions; p != ଖାଲି; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->ସଂଖ୍ୟା);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
prepare_ପ୍ରତୀକ_definitions (ଶୂନ୍ୟ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ଅଚଳ ଅକ୍ଷର *ଚାବି = obstack_finish0 (&format_obstack);
      MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, nterm_map ? nterm_map[i - ntokens] : i);
    }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; ++i)
    {
      ପ୍ରତୀକ *sym = symbols[i];
      ଅଚଳ ଅକ୍ଷର *ଚାବି;

#define SET_ଚାବି(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      ଚାବି = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      ଚାବି = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ଅଚଳ ଅକ୍ଷର *id = ପ୍ରତୀକ_id_get (sym);
      SET_ଚାବି ("has_id");
      MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, !!id);

      /* Its identifier.  */
      SET_ଚାବି ("id");
      MUSCLE_ଭର୍ତ୍ତି_STRING (ଚାବି, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_ଚାବି ("tag");
      MUSCLE_ଭର୍ତ୍ତି_STRING (ଚାବି, ପ୍ରତୀକ_tag (sym));

      SET_ଚାବି ("code");
      MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, sym->content->code);

      SET_ଚାବି ("is_token");
      MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, i < ntokens);

      SET_ଚାବି ("number");
      MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, sym->content->ସଂଖ୍ୟା);

      SET_ଚାବି ("has_type");
      MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, !!sym->content->type_name);

      SET_ଚାବି ("type");
      MUSCLE_ଭର୍ତ୍ତି_STRING (ଚାବି, sym->content->type_name
                            ? sym->content->type_name : "");

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          ଅକ୍ଷର ଅଚଳ *pname = code_props_type_string (j) + 1;
          code_props ଅଚଳ *p = ପ୍ରତୀକ_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, !!p->code);

          ଯଦି (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_ଭର୍ତ୍ତି_C_STRING (ଚାବି, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_ଭର୍ତ୍ତି_INT (ଚାବି, p->location.start.ଧାଡ଼ି);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (ଚାବି, p->location);

              SET_ଚାବି (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_ଭର୍ତ୍ତି (ଚାବି, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_ଚାବି
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
prepare_actions (ଶୂନ୍ୟ)
{
  /* Figure out the actions for the specified state.  */
  muscle_ଭର୍ତ୍ତି_rule_ସଂଖ୍ୟା_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_ଭର୍ତ୍ତି_state_ସଂଖ୍ୟା_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_ଭର୍ତ୍ତି_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_ଭର୍ତ୍ତି_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_ଭର୍ତ୍ତି_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_ଭର୍ତ୍ତି_base_table ("table", table,
                            table[0], 1, ଉପର + 1);
  MUSCLE_ଭର୍ତ୍ତି_INT ("table_ninf", table_ninf);

  muscle_ଭର୍ତ୍ତି_base_table ("check", check,
                            check[0], 1, ଉପର + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_ଭର୍ତ୍ତି_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, ଉପର + 1);
  muscle_ଭର୍ତ୍ତି_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  ପ୍ରତୀକ_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
output_skeleton (ଶୂନ୍ୟ)
{
  /* Compute the names of the package data dir and skeleton files.  */
  ଅକ୍ଷର ଅଚଳ *m4 = m4path ();
  ଅକ୍ଷର ଅଚଳ *datadir = pkgdatadir ();
  ଅକ୍ଷର *skeldir = xpath_join (datadir, "skeletons");
  ଅକ୍ଷର *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  ଅକ୍ଷର *m4bison = xpath_join (skeldir, "bison.m4");
  ଅକ୍ଷର *traceon = xpath_join (skeldir, "traceon.m4");
  ଅକ୍ଷର *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_fd[2];
  pid_t pid;
  {
    ଅକ୍ଷର ଅଚଳ *argv[11];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ଯଦି (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ଯଦି (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    ଯଦି (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = ଖାଲି;
    aver (i <= ARRAY_CARDINALITY (argv));

    ଯଦି (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ ଖାଲି,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  ମୁକ୍ତ (skeldir);
  ମୁକ୍ତ (m4sugar);
  ମୁକ୍ତ (m4bison);
  ମୁକ୍ତ (traceon);
  ମୁକ୍ତ (skel);

  ଯଦି (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_ଠେଲ (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, ଖାଲି);
  timevar_କାଢ଼ (tv_m4);
}

ସ୍ଥିର ଶୂନ୍ୟ
prepare (ଶୂନ୍ୟ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  ଅକ୍ଷର ଅଚଳ *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ଠେଲ_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_ଭର୍ତ୍ତି_STRING ("version_string", VERSION);
  MUSCLE_ଭର୍ତ୍ତି_INT ("version", strversion_to_int (VERSION));
  MUSCLE_ଭର୍ତ୍ତି_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("header_flag", header_flag);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("glr_flag", glr_parser);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("synclines_flag", !no_ଧାଡ଼ିଗୁଡ଼ିକ_flag);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("use_push_for_pull_flag", use_ଠେଲ_for_pull_flag);
  MUSCLE_ଭର୍ତ୍ତି_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  ଯଦି (spec_name_prefix)
    MUSCLE_ଭର୍ତ୍ତି_STRING ("prefix", spec_name_prefix);

  MUSCLE_ଭର୍ତ୍ତି_STRING ("file_name_all_but_ext", all_but_ext);

  ଅଚଳ ଅକ୍ଷର *spec_mapped_header_file = map_file_name (spec_header_file);
  ଅଚଳ ଅକ୍ଷର *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_ଭର୍ତ୍ତି_STRING (#Name, Name ? Name : "")
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
  ଯଦି (skeleton)
    MUSCLE_ଭର୍ତ୍ତି_C_STRING ("skeleton", skeleton);
  ଅନ୍ୟଥା
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    ଅକ୍ଷର *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_ଭର୍ତ୍ତି_STRING_RAW ("skeletonsdir", skeldir);
    ମୁକ୍ତ (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

ଶୂନ୍ୟ
output (ଶୂନ୍ୟ)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_ପ୍ରତୀକ_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  ଯଦି (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, ଖାଲି);
}
