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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <filename.h> /* IS_PATH_WITH_DIR */
#ಅನ್ತರ್ಭಾವ <get-errno.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <path-join.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <spawn-pipe.h>
#ಅನ್ತರ್ಭಾವ <timevar.h>
#ಅನ್ತರ್ಭಾವ <wait-process.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "output.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "scan-code.h"    /* max_left_semantic_context */
#ಅನ್ತರ್ಭಾವ "scan-skel.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "tables.h"
#ಅನ್ತರ್ಭಾವ "strversion.h"

ಸ್ಥಿರ ಸಂರಚನಾ obstack format_obstack;


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


#define GENERATE_MUSCLE_ಸೇರಿಸು_TABLE(Name, Type)                        \
                                                                        \
ಸ್ಥಿರ ಶೂನ್ಯ                                                             \
Name (ವರ್ಣ ಧ್ರುವ *name, Type *table_data, Type first,                   \
      ಪೂರ್ಣಾಙ್ಕ begin, ಪೂರ್ಣಾಙ್ಕ end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  ಪೂರ್ಣಾಙ್ಕ j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      ಯದಿ (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      ಅನ್ಯಥಾ                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      ಯದಿ (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      ಯದಿ (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_ಸೇರಿಸು (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_ಸೇರಿಸು_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_ಸೇರಿಸು_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_ಸೇರಿಸು_TABLE (muscle_ಸೇರಿಸು_int_table, ಪೂರ್ಣಾಙ್ಕ)
GENERATE_MUSCLE_ಸೇರಿಸು_TABLE (muscle_ಸೇರಿಸು_base_table, base_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_TABLE (muscle_ಸೇರಿಸು_rule_ಸಂಖ್ಯೆ_table, rule_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_TABLE (muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_table, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_TABLE (muscle_ಸೇರಿಸು_item_ಸಂಖ್ಯೆ_table, item_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_TABLE (muscle_ಸೇರಿಸು_state_ಸಂಖ್ಯೆ_table, state_ಸಂಖ್ಯೆ)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_escaped (FILE *out, ಧ್ರುವ ವರ್ಣ *cp)
{
  ಪ್ರತಿ (; *cp; ++cp)
    ವಿಕಲ್ಪ (*cp)
      {
      ಪಕ್ಷ '$': fputs ("$][", out); ಭಙ್ಗ;
      ಪಕ್ಷ '@': fputs ("@@",  out); ಭಙ್ಗ;
      ಪಕ್ಷ '[': fputs ("@{",  out); ಭಙ್ಗ;
      ಪಕ್ಷ ']': fputs ("@}",  out); ಭಙ್ಗ;

      ಪಕ್ಷ 'b':
        ಯದಿ (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            ಭಙ್ಗ;
          }
        ಅನ್ಯಥಾ
          ಗಚ್ಛ append;

      ಪಕ್ಷ 'm':
        ಯದಿ (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            ಭಙ್ಗ;
          }
        ಅನ್ಯಥಾ
          ಗಚ್ಛ append;

      append:
      ಸ್ವಭಾವ:  fputc (*cp,   out); ಭಙ್ಗ;
      }
}

ಸ್ಥಿರ ಶೂನ್ಯ
output_quoted (FILE *out, ವರ್ಣ ಧ್ರುವ *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
string_output (FILE *out, ವರ್ಣ ಧ್ರುವ *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ಸ್ಥಿರ ptrdiff_t
escape_trigraphs (ವರ್ಣ *buffer, ptrdiff_t buffersize, ಧ್ರುವ ವರ್ಣ *src)
{
#define STORE(c)                                \
  ಕುರು                                            \
    {                                           \
      ಯದಿ (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  ಯಾವತ್ (0)
  ptrdiff_t res = 0;
  ಪ್ರತಿ (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      ಯದಿ (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          ವಿಕಲ್ಪ (src[i+2])
            {
            ಪಕ್ಷ '!': ಪಕ್ಷ '\'':
            ಪಕ್ಷ '(': ಪಕ್ಷ ')': ಪಕ್ಷ '-': ಪಕ್ಷ '/':
            ಪಕ್ಷ '<': ಪಕ್ಷ '=': ಪಕ್ಷ '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              ಅನುವರ್ತ;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
ಸ್ಥಿರ ವರ್ಣ *
xescape_trigraphs (ಧ್ರುವ ವರ್ಣ *src)
{
  ptrdiff_t bufsize = escape_trigraphs (ರಿಕ್ತ, 0, src);
  ವರ್ಣ *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  ಪ್ರತ್ಯಾಗಮ buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
ಚಿಹ್ನೆ_tag (ಧ್ರುವ ಚಿಹ್ನೆ *sym)
{
  ಧ್ರುವ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  ಯದಿ (!eof_is_user_defined && sym->content == eoftoken->content)
    ಪ್ರತ್ಯಾಗಮ "\"end of file\"";
  ಅನ್ಯಥಾ ಯದಿ (sym->content == undeftoken->content)
    ಪ್ರತ್ಯಾಗಮ "\"invalid token\"";
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_ಚಿಹ್ನೆ_names (ವರ್ಣ ಧ್ರುವ *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ಧ್ರುವ bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  ಪೂರ್ಣಾಙ್ಕ col = 2;
  ಸಂರಚನಾ quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; i++)
    {
      ಧ್ರುವ ವರ್ಣ *tag = ಚಿಹ್ನೆ_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      ಯದಿ (translatable)
        has_translations = true;

      ವರ್ಣ *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      ಪೂರ್ಣಾಙ್ಕ width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      ಯದಿ (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      ಯದಿ (i)
        obstack_1grow (&format_obstack, ' ');
      ಯದಿ (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      ಯದಿ (translatable)
        obstack_sgrow (&format_obstack, "])[");
      ಮುಕ್ತ (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  ಮುಕ್ತ (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_ಸೇರಿಸು (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_ಸೇರಿಸು_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_symbols (ಶೂನ್ಯ)
{
  MUSCLE_ಸೇರಿಸು_INT ("tokens_number", ntokens);
  MUSCLE_ಸೇರಿಸು_INT ("nterms_number", nnterms);
  MUSCLE_ಸೇರಿಸು_INT ("symbols_number", nsyms);
  MUSCLE_ಸೇರಿಸು_INT ("code_max", max_code);

  muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_ಚಿಹ್ನೆ_names ("tname");
  prepare_ಚಿಹ್ನೆ_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; ++i)
      ಯದಿ (symbols[i]->translatable)
        {
          translatable = true;
          ಭಙ್ಗ;
        }
    ಯದಿ (translatable)
      {
        ಪೂರ್ಣಾಙ್ಕ *ಮೌಲ್ಯಗಳು = xnmalloc (nsyms, ಪರಿಮಾಣ *ಮೌಲ್ಯಗಳು);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; ++i)
          ಮೌಲ್ಯಗಳು[i] = symbols[i]->translatable;
        muscle_ಸೇರಿಸು_int_table ("translatable", ಮೌಲ್ಯಗಳು,
                                 ಮೌಲ್ಯಗಳು[0], 1, ntokens);
        ಮುಕ್ತ (ಮೌಲ್ಯಗಳು);
      }
  }

  /* Output YYTOKNUM. */
  {
    ಪೂರ್ಣಾಙ್ಕ *ಮೌಲ್ಯಗಳು = xnmalloc (ntokens, ಪರಿಮಾಣ *ಮೌಲ್ಯಗಳು);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; ++i)
      ಮೌಲ್ಯಗಳು[i] = symbols[i]->content->code;
    muscle_ಸೇರಿಸು_int_table ("toknum", ಮೌಲ್ಯಗಳು,
                             ಮೌಲ್ಯಗಳು[0], 1, ntokens);
    ಮುಕ್ತ (ಮೌಲ್ಯಗಳು);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_rules (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ *prhs = xnmalloc (nrules, ಪರಿಮಾಣ *prhs);
  item_ಸಂಖ್ಯೆ *rhs = xnmalloc (nritems, ಪರಿಮಾಣ *rhs);
  ಪೂರ್ಣಾಙ್ಕ *rline = xnmalloc (nrules, ಪರಿಮಾಣ *rline);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *r1 = xnmalloc (nrules, ಪರಿಮಾಣ *r1);
  ಪೂರ್ಣಾಙ್ಕ *r2 = xnmalloc (nrules, ಪರಿಮಾಣ *r2);
  ಪೂರ್ಣಾಙ್ಕ *dprec = xnmalloc (nrules, ಪರಿಮಾಣ *dprec);
  ಪೂರ್ಣಾಙ್ಕ *merger = xnmalloc (nrules, ಪರಿಮಾಣ *merger);
  ಪೂರ್ಣಾಙ್ಕ *immediate = xnmalloc (nrules, ಪರಿಮಾಣ *immediate);

  /* Index in RHS.  */
  ಪೂರ್ಣಾಙ್ಕ i = 0;
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.ಸಾಲು;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->ಸಂಖ್ಯೆ;
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

  muscle_ಸೇರಿಸು_item_ಸಂಖ್ಯೆ_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_ಸೇರಿಸು_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_table ("rline", rline, 0, 0, nrules);
  muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_table ("r1", r1, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_table ("r2", r2, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_table ("merger", merger, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_ಸೇರಿಸು_INT ("rules_number", nrules);
  MUSCLE_ಸೇರಿಸು_INT ("max_left_semantic_context", max_ಎಡ_semantic_context);

  ಮುಕ್ತ (prhs);
  ಮುಕ್ತ (rhs);
  ಮುಕ್ತ (rline);
  ಮುಕ್ತ (r1);
  ಮುಕ್ತ (r2);
  ಮುಕ್ತ (dprec);
  ಮುಕ್ತ (merger);
  ಮುಕ್ತ (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_states (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *ಮೌಲ್ಯಗಳು = xnmalloc (nstates, ಪರಿಮಾಣ *ಮೌಲ್ಯಗಳು);
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಮೌಲ್ಯಗಳು[i] = states[i]->accessing_ಚಿಹ್ನೆ;
  muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_table ("stos", ಮೌಲ್ಯಗಳು,
                                     0, 1, nstates);
  ಮುಕ್ತ (ಮೌಲ್ಯಗಳು);

  MUSCLE_ಸೇರಿಸು_INT ("last", ಮೇಲಿನ);
  MUSCLE_ಸೇರಿಸು_INT ("final_state_number", final_state->ಸಂಖ್ಯೆ);
  MUSCLE_ಸೇರಿಸು_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ಚಿಹ್ನೆ_type_name_cmp (ಧ್ರುವ ಚಿಹ್ನೆ **lhs, ಧ್ರುವ ಚಿಹ್ನೆ **rhs)
{
  ಪೂರ್ಣಾಙ್ಕ res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  ಯದಿ (!res)
    res = (*lhs)->content->ಸಂಖ್ಯೆ - (*rhs)->content->ಸಂಖ್ಯೆ;
  ಪ್ರತ್ಯಾಗಮ res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಚಿಹ್ನೆ **
symbols_by_type_name (ಶೂನ್ಯ)
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*qcmp_type) (ಧ್ರುವ ಶೂನ್ಯ *, ಧ್ರುವ ಶೂನ್ಯ *);
  ಚಿಹ್ನೆ **res = xmemdup (symbols, nsyms * ಪರಿಮಾಣ *res);
  qsort (res, nsyms, ಪರಿಮಾಣ *res, (qcmp_type) &ಚಿಹ್ನೆ_type_name_cmp);
  ಪ್ರತ್ಯಾಗಮ res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
type_names_output (FILE *out)
{
  ಚಿಹ್ನೆ **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      ಪೂರ್ಣಾಙ್ಕ i0 = i;
      fputs (i ? ",\n[" : "[", out);
      ಪ್ರತಿ (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->ಸಂಖ್ಯೆ);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  ಮುಕ್ತ (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ಸ್ಥಿರ ಶೂನ್ಯ
start_symbols_output (FILE *out)
{
  ಯದಿ (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = start_symbols; list; list = list->next)
        {
          ಧ್ರುವ ಚಿಹ್ನೆ *start = list->content.sym;
          ಧ್ರುವ ಚಿಹ್ನೆ *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->ಸಂಖ್ಯೆ, swtok->content->ಸಂಖ್ಯೆ);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
rule_output (ಧ್ರುವ rule *r, FILE *out)
{
  output_escaped (out, r->lhs->ಚಿಹ್ನೆ->tag);
  fputc (':', out);
  ಯದಿ (0 <= *r->rhs)
    ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  ಅನ್ಯಥಾ
    fputs (" %empty", out);
}

ಸ್ಥಿರ ಶೂನ್ಯ
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    ಯದಿ (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        ಯದಿ (!no_ಸಾಲುಗಳು_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.ಸಾಲು);
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

ಸ್ಥಿರ ಶೂನ್ಯ
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  ಪೂರ್ಣಾಙ್ಕ n;
  merger_list* p;
  ಪ್ರತಿ (n = 1, p = merge_functions; p != ರಿಕ್ತ; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->ಸಂಖ್ಯೆ);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_ಚಿಹ್ನೆ_definitions (ಶೂನ್ಯ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ = obstack_finish0 (&format_obstack);
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, nterm_map ? nterm_map[i - ntokens] : i);
    }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; ++i)
    {
      ಚಿಹ್ನೆ *sym = symbols[i];
      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ;

#define SET_ಕೀಲಿ(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      ಕೀಲಿ = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      ಕೀಲಿ = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ಧ್ರುವ ವರ್ಣ *id = ಚಿಹ್ನೆ_id_get (sym);
      SET_ಕೀಲಿ ("has_id");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, !!id);

      /* Its identifier.  */
      SET_ಕೀಲಿ ("id");
      MUSCLE_ಸೇರಿಸು_STRING (ಕೀಲಿ, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_ಕೀಲಿ ("tag");
      MUSCLE_ಸೇರಿಸು_STRING (ಕೀಲಿ, ಚಿಹ್ನೆ_tag (sym));

      SET_ಕೀಲಿ ("code");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, sym->content->code);

      SET_ಕೀಲಿ ("is_token");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, i < ntokens);

      SET_ಕೀಲಿ ("number");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, sym->content->ಸಂಖ್ಯೆ);

      SET_ಕೀಲಿ ("has_type");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, !!sym->content->type_name);

      SET_ಕೀಲಿ ("type");
      MUSCLE_ಸೇರಿಸು_STRING (ಕೀಲಿ, sym->content->type_name
                            ? sym->content->type_name : "");

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          ವರ್ಣ ಧ್ರುವ *pname = code_props_type_string (j) + 1;
          code_props ಧ್ರುವ *p = ಚಿಹ್ನೆ_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, !!p->code);

          ಯದಿ (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_ಸೇರಿಸು_C_STRING (ಕೀಲಿ, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, p->location.start.ಸಾಲು);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (ಕೀಲಿ, p->location);

              SET_ಕೀಲಿ (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_ಸೇರಿಸು (ಕೀಲಿ, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_ಕೀಲಿ
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
prepare_actions (ಶೂನ್ಯ)
{
  /* Figure out the actions for the specified state.  */
  muscle_ಸೇರಿಸು_rule_ಸಂಖ್ಯೆ_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_ಸೇರಿಸು_state_ಸಂಖ್ಯೆ_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_ಸೇರಿಸು_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_ಸೇರಿಸು_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_ಸೇರಿಸು_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_ಸೇರಿಸು_base_table ("table", table,
                            table[0], 1, ಮೇಲಿನ + 1);
  MUSCLE_ಸೇರಿಸು_INT ("table_ninf", table_ninf);

  muscle_ಸೇರಿಸು_base_table ("check", check,
                            check[0], 1, ಮೇಲಿನ + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_ಸೇರಿಸು_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, ಮೇಲಿನ + 1);
  muscle_ಸೇರಿಸು_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  ಚಿಹ್ನೆ_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_skeleton (ಶೂನ್ಯ)
{
  /* Compute the names of the package data dir and skeleton files.  */
  ವರ್ಣ ಧ್ರುವ *m4 = m4path ();
  ವರ್ಣ ಧ್ರುವ *datadir = pkgdatadir ();
  ವರ್ಣ *skeldir = xpath_join (datadir, "skeletons");
  ವರ್ಣ *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  ವರ್ಣ *m4bison = xpath_join (skeldir, "bison.m4");
  ವರ್ಣ *traceon = xpath_join (skeldir, "traceon.m4");
  ವರ್ಣ *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  ಪೂರ್ಣಾಙ್ಕ filter_fd[2];
  pid_t pid;
  {
    ವರ್ಣ ಧ್ರುವ *argv[11];
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ಯದಿ (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ಯದಿ (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    ಯದಿ (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = ರಿಕ್ತ;
    aver (i <= ARRAY_CARDINALITY (argv));

    ಯದಿ (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ ರಿಕ್ತ,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  ಮುಕ್ತ (skeldir);
  ಮುಕ್ತ (m4sugar);
  ಮುಕ್ತ (m4bison);
  ಮುಕ್ತ (traceon);
  ಮುಕ್ತ (skel);

  ಯದಿ (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_ತಳ್ಳು (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, ರಿಕ್ತ);
  timevar_ತೆಗೆ (tv_m4);
}

ಸ್ಥಿರ ಶೂನ್ಯ
prepare (ಶೂನ್ಯ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  ವರ್ಣ ಧ್ರುವ *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ತಳ್ಳು_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_ಸೇರಿಸು_STRING ("version_string", VERSION);
  MUSCLE_ಸೇರಿಸು_INT ("version", strversion_to_int (VERSION));
  MUSCLE_ಸೇರಿಸು_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_ಸೇರಿಸು_BOOL ("header_flag", header_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("glr_flag", glr_parser);
  MUSCLE_ಸೇರಿಸು_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_ಸೇರಿಸು_BOOL ("synclines_flag", !no_ಸಾಲುಗಳು_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_ಸೇರಿಸು_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("use_push_for_pull_flag", use_ತಳ್ಳು_for_pull_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  ಯದಿ (spec_name_prefix)
    MUSCLE_ಸೇರಿಸು_STRING ("prefix", spec_name_prefix);

  MUSCLE_ಸೇರಿಸು_STRING ("file_name_all_but_ext", all_but_ext);

  ಧ್ರುವ ವರ್ಣ *spec_mapped_header_file = map_file_name (spec_header_file);
  ಧ್ರುವ ವರ್ಣ *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_ಸೇರಿಸು_STRING (#Name, Name ? Name : "")
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
  ಯದಿ (skeleton)
    MUSCLE_ಸೇರಿಸು_C_STRING ("skeleton", skeleton);
  ಅನ್ಯಥಾ
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    ವರ್ಣ *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_ಸೇರಿಸು_STRING_RAW ("skeletonsdir", skeldir);
    ಮುಕ್ತ (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

ಶೂನ್ಯ
output (ಶೂನ್ಯ)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_ಚಿಹ್ನೆ_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  ಯದಿ (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, ರಿಕ್ತ);
}
