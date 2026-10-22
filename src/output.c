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

ಸ್ಥಿರ ರಚನೆ obstack format_obstack;


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


#ವ್ಯಾಖ್ಯಾನಿಸು GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ(Name, Type)                        \
                                                                        \
ಸ್ಥಿರ ಶೂನ್ಯ                                                             \
Name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, Type *ಕೋಷ್ಟಕ_ದತ್ತಾಂಶ, Type ಮೊದಲ,                   \
      ಪೂರ್ಣಾಂಕ begin, ಪೂರ್ಣಾಂಕ ಕೊನೆ)                                               \
{                                                                       \
  Type min = ಮೊದಲ;                                                     \
  Type max = ಮೊದಲ;                                                     \
  ಪೂರ್ಣಾಂಕ j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", ಮೊದಲ);                       \
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = begin; i < ಕೊನೆ; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      ಆದರೆ (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      ಇಲ್ಲದಿದ್ದರೆ                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", ಕೋಷ್ಟಕ_ದತ್ತಾಂಶ[i]);           \
      ಆದರೆ (ಕೋಷ್ಟಕ_ದತ್ತಾಂಶ[i] < min)                                          \
        min = ಕೋಷ್ಟಕ_ದತ್ತಾಂಶ[i];                                            \
      ಆದರೆ (max < ಕೋಷ್ಟಕ_ದತ್ತಾಂಶ[i])                                          \
        max = ಕೋಷ್ಟಕ_ದತ್ತಾಂಶ[i];                                            \
    }                                                                   \
  muscle_ಸೇರಿಸು (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  ದೀರ್ಘ lmin = min;                                                      \
  ದೀರ್ಘ lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_ಸೇರಿಸು_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_ಸೇರಿಸು_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ (muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ)
GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ (muscle_ಸೇರಿಸು_ತಳ_ಕೋಷ್ಟಕ, ತಳ_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ (muscle_ಸೇರಿಸು_ನಿಯಮ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ, ನಿಯಮ_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ (muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ (muscle_ಸೇರಿಸು_ಅಂಶ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ, ಅಂಶ_ಸಂಖ್ಯೆ)
GENERATE_MUSCLE_ಸೇರಿಸು_ಕೋಷ್ಟಕ (muscle_ಸೇರಿಸು_state_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ, state_ಸಂಖ್ಯೆ)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_escaped (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp)
{
  ಗಾಗಿ (; *cp; ++cp)
    ಆಯ್ಕೆ (*cp)
      {
      ಸ್ಥಿತಿ '$': fputs ("$][", ಹೊರಹೋಗು); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '@': fputs ("@@",  ಹೊರಹೋಗು); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '[': fputs ("@{",  ಹೊರಹೋಗು); ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ ']': fputs ("@}",  ಹೊರಹೋಗು); ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'b':
        ಆದರೆ (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", ಹೊರಹೋಗು);
            cp += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("b4_") - 1;
            ನಿಲ್ಲಿಸು;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          ಹೋಗು append;

      ಸ್ಥಿತಿ 'm':
        ಆದರೆ (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", ಹೊರಹೋಗು);
            cp += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("m4_") - 1;
            ನಿಲ್ಲಿಸು;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          ಹೋಗು append;

      append:
      ಪೂರ್ವನಿಯೋಜಿತ:  fputc (*cp,   ಹೊರಹೋಗು); ನಿಲ್ಲಿಸು;
      }
}

ಸ್ಥಿರ ಶೂನ್ಯ
output_quoted (FILE *ಹೊರಹೋಗು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp)
{
  fprintf (ಹೊರಹೋಗು, "[[");
  output_escaped (ಹೊರಹೋಗು, cp);
  fprintf (ಹೊರಹೋಗು, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
string_output (FILE *ಹೊರಹೋಗು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string)
{
  output_quoted (ಹೊರಹೋಗು, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ಸ್ಥಿರ ptrdiff_t
escape_trigraphs (ಅಕ್ಷರ *ಬಫರ್, ptrdiff_t buffersize, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src)
{
#ವ್ಯಾಖ್ಯಾನಿಸು STORE(c)                                \
  ಮಾಡು                                            \
    {                                           \
      ಆದರೆ (res < buffersize)                     \
        ಬಫರ್[res] = (c);                      \
      ++res;                                    \
    }                                           \
  ಆಗ (0)
  ptrdiff_t res = 0;
  ಗಾಗಿ (ptrdiff_t i = 0, len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (src); i < len; ++i)
    {
      ಆದರೆ (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          ಆಯ್ಕೆ (src[i+2])
            {
            ಸ್ಥಿತಿ '!': ಸ್ಥಿತಿ '\'':
            ಸ್ಥಿತಿ '(': ಸ್ಥಿತಿ ')': ಸ್ಥಿತಿ '-': ಸ್ಥಿತಿ '/':
            ಸ್ಥಿತಿ '<': ಸ್ಥಿತಿ '=': ಸ್ಥಿತಿ '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              ಮುಂದುವರಿ;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STORE
  ಹಿಂತಿರುಗಿ res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
xescape_trigraphs (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src)
{
  ptrdiff_t bufsize = escape_trigraphs (ಖಾಲಿ, 0, src);
  ಅಕ್ಷರ *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  ಹಿಂತಿರುಗಿ buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಚಿಹ್ನೆ_ಟ್ಯಾಗ್ (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym)
{
  ಸ್ಥಿರಾಂಕ bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->ಟ್ಯಾಗ್, "$end");

  ಆದರೆ (!eof_is_user_defined && sym->content == eoftoken->content)
    ಹಿಂತಿರುಗಿ "\"end of file\"";
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sym->content == undeftoken->content)
    ಹಿಂತಿರುಗಿ "\"invalid token\"";
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ sym->ಟ್ಯಾಗ್;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_ಚಿಹ್ನೆ_ಹೆಸರುಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ಸ್ಥಿರಾಂಕ bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು = 2;
  ರಚನೆ quoting_options *qo = clone_quoting_options (0);
  ಗಣ_quoting_style (qo, c_quoting_style);
  ಗಣ_quoting_flags (qo, QA_ವಿಭಜಿಸು_TRIGRAPHS);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; i++)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = ಚಿಹ್ನೆ_ಟ್ಯಾಗ್ (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      ಆದರೆ (translatable)
        has_translations = true;

      ಅಕ್ಷರ *cp
        = ಟ್ಯಾಗ್[0] == '"' && !quote
        ? xescape_trigraphs (ಟ್ಯಾಗ್)
        : quotearg_alloc (ಟ್ಯಾಗ್, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      ಪೂರ್ಣಾಂಕ ಅಗಲ
        = mbswidth (cp, 0) + 2
        + (translatable ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("N_()") : 0);

      ಆದರೆ (ಲಂಬಸಾಲು + ಅಗಲ > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          ಲಂಬಸಾಲು = 1;
        }

      ಆದರೆ (i)
        obstack_1grow (&format_obstack, ' ');
      ಆದರೆ (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      ಆದರೆ (translatable)
        obstack_sgrow (&format_obstack, "])[");
      ಬಿಡುಗಡೆ (cp);
      obstack_1grow (&format_obstack, ',');
      ಲಂಬಸಾಲು += ಅಗಲ;
    }
  ಬಿಡುಗಡೆ (qo);
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
  MUSCLE_ಸೇರಿಸು_INT ("code_max", max_ಸಂಕೇತ);

  muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_ಸಂಕೇತ + 1);

  /* tname -- token names.  */
  prepare_ಚಿಹ್ನೆ_ಹೆಸರುಗಳು ("tname");
  prepare_ಚಿಹ್ನೆ_ಹೆಸರುಗಳು ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; ++i)
      ಆದರೆ (symbols[i]->translatable)
        {
          translatable = true;
          ನಿಲ್ಲಿಸು;
        }
    ಆದರೆ (translatable)
      {
        ಪೂರ್ಣಾಂಕ *ಮೌಲ್ಯಗಳು = xnmalloc (nsyms, ಗಾತ್ರ *ಮೌಲ್ಯಗಳು);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; ++i)
          ಮೌಲ್ಯಗಳು[i] = symbols[i]->translatable;
        muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("translatable", ಮೌಲ್ಯಗಳು,
                                 ಮೌಲ್ಯಗಳು[0], 1, ntokens);
        ಬಿಡುಗಡೆ (ಮೌಲ್ಯಗಳು);
      }
  }

  /* Output YYTOKNUM. */
  {
    ಪೂರ್ಣಾಂಕ *ಮೌಲ್ಯಗಳು = xnmalloc (ntokens, ಗಾತ್ರ *ಮೌಲ್ಯಗಳು);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; ++i)
      ಮೌಲ್ಯಗಳು[i] = symbols[i]->content->ಸಂಕೇತ_2;
    muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("toknum", ಮೌಲ್ಯಗಳು,
                             ಮೌಲ್ಯಗಳು[0], 1, ntokens);
    ಬಿಡುಗಡೆ (ಮೌಲ್ಯಗಳು);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_ನಿಯಮಗಳು (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ *prhs = xnmalloc (nrules, ಗಾತ್ರ *prhs);
  ಅಂಶ_ಸಂಖ್ಯೆ *rhs = xnmalloc (nritems, ಗಾತ್ರ *rhs);
  ಪೂರ್ಣಾಂಕ *rline = xnmalloc (nrules, ಗಾತ್ರ *rline);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *r1 = xnmalloc (nrules, ಗಾತ್ರ *r1);
  ಪೂರ್ಣಾಂಕ *r2 = xnmalloc (nrules, ಗಾತ್ರ *r2);
  ಪೂರ್ಣಾಂಕ *dprec = xnmalloc (nrules, ಗಾತ್ರ *dprec);
  ಪೂರ್ಣಾಂಕ *merger = xnmalloc (nrules, ಗಾತ್ರ *merger);
  ಪೂರ್ಣಾಂಕ *ತಕ್ಷಣ = xnmalloc (nrules, ಗಾತ್ರ *ತಕ್ಷಣ);

  /* Index in RHS.  */
  ಪೂರ್ಣಾಂಕ i = 0;
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = ನಿಯಮಗಳು[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = ನಿಯಮಗಳು[r].location.ಆರಂಭ.ಸಾಲು;
      /* LHS of the rule R. */
      r1[r] = ನಿಯಮಗಳು[r].lhs->ಸಂಖ್ಯೆ;
      /* Length of rule R's RHS. */
      r2[r] = ನಿಯಮ_rhs_ಉದ್ದಳತೆ (&ನಿಯಮಗಳು[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = ನಿಯಮಗಳು[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = ನಿಯಮಗಳು[r].merger;
      /* Immediate reduction flags (GLR).  */
      ತಕ್ಷಣ[r] = ನಿಯಮಗಳು[r].is_predicate;
    }
  aver (i == nritems);

  muscle_ಸೇರಿಸು_ಅಂಶ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ ("rhs", rhs, ritem[0], 1, nritems);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("prhs", prhs, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("rline", rline, 0, 0, nrules);
  muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ ("r1", r1, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("r2", r2, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("dprec", dprec, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("merger", merger, 0, 0, nrules);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("immediate", ತಕ್ಷಣ, 0, 0, nrules);

  MUSCLE_ಸೇರಿಸು_INT ("rules_number", nrules);
  MUSCLE_ಸೇರಿಸು_INT ("max_left_semantic_context", max_ಎಡ_semantic_context);

  ಬಿಡುಗಡೆ (prhs);
  ಬಿಡುಗಡೆ (rhs);
  ಬಿಡುಗಡೆ (rline);
  ಬಿಡುಗಡೆ (r1);
  ಬಿಡುಗಡೆ (r2);
  ಬಿಡುಗಡೆ (dprec);
  ಬಿಡುಗಡೆ (merger);
  ಬಿಡುಗಡೆ (ತಕ್ಷಣ);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_ಸ್ಥಿತಿಗಳು (ಶೂನ್ಯ)
{
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *ಮೌಲ್ಯಗಳು = xnmalloc (nstates, ಗಾತ್ರ *ಮೌಲ್ಯಗಳು);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಮೌಲ್ಯಗಳು[i] = ಸ್ಥಿತಿಗಳು[i]->accessing_ಚಿಹ್ನೆ;
  muscle_ಸೇರಿಸು_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ ("stos", ಮೌಲ್ಯಗಳು,
                                     0, 1, nstates);
  ಬಿಡುಗಡೆ (ಮೌಲ್ಯಗಳು);

  MUSCLE_ಸೇರಿಸು_INT ("last", ಮೇಲಿನ);
  MUSCLE_ಸೇರಿಸು_INT ("final_state_number", final_state->ಸಂಖ್ಯೆ);
  MUSCLE_ಸೇರಿಸು_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಚಿಹ್ನೆ_type_name_cmp (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ **lhs, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ **rhs)
{
  ಪೂರ್ಣಾಂಕ res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  ಆದರೆ (!res)
    res = (*lhs)->content->ಸಂಖ್ಯೆ - (*rhs)->content->ಸಂಖ್ಯೆ;
  ಹಿಂತಿರುಗಿ res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಚಿಹ್ನೆ **
symbols_by_type_name (ಶೂನ್ಯ)
{
  ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*qcmp_type) (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *);
  ಚಿಹ್ನೆ **res = xmemdup (symbols, nsyms * ಗಾತ್ರ *res);
  qsort (res, nsyms, ಗಾತ್ರ *res, (qcmp_type) &ಚಿಹ್ನೆ_type_name_cmp);
  ಹಿಂತಿರುಗಿ res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
type_ಹೆಸರುಗಳು_output (FILE *ಹೊರಹೋಗು)
{
  ಚಿಹ್ನೆ **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", ಹೊರಹೋಗು);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      ಪೂರ್ಣಾಂಕ i0 = i;
      fputs (i ? ",\n[" : "[", ಹೊರಹೋಗು);
      ಗಾಗಿ (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (ಹೊರಹೋಗು, "%s%d", i != i0 ? ", " : "", syms[i]->content->ಸಂಖ್ಯೆ);
      fputs ("]", ಹೊರಹೋಗು);
    }
  fputs ("])\n\n", ಹೊರಹೋಗು);
  ಬಿಡುಗಡೆ (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ಸ್ಥಿರ ಶೂನ್ಯ
ಆರಂಭ_symbols_output (FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (ಆರಂಭ_symbols && ಆರಂಭ_symbols->ಮುಂದಿನ)
    {
      fputs ("m4_define([b4_start_symbols],\n[", ಹೊರಹೋಗು);
      ಗಾಗಿ (ಚಿಹ್ನೆ_list *list = ಆರಂಭ_symbols; list; list = list->ಮುಂದಿನ)
        {
          ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಆರಂಭ = list->content.sym;
          ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *swtok = switching_token (ಆರಂಭ);
          fprintf (ಹೊರಹೋಗು, "%s[%d, %d]",
                   list == ಆರಂಭ_symbols ? "" : ", ",
                   ಆರಂಭ->content->ಸಂಖ್ಯೆ, swtok->content->ಸಂಖ್ಯೆ);
        }
      fputs ("])\n\n", ಹೊರಹೋಗು);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಚಿಹ್ನೆ_numbers_output (FILE *ಹೊರಹೋಗು)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", ಹೊರಹೋಗು);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; ++i)
    fprintf (ಹೊರಹೋಗು, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", ಹೊರಹೋಗು);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ನಿಯಮ_output (ಸ್ಥಿರಾಂಕ ನಿಯಮ *r, FILE *ಹೊರಹೋಗು)
{
  output_escaped (ಹೊರಹೋಗು, r->lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
  fputc (':', ಹೊರಹೋಗು);
  ಆದರೆ (0 <= *r->rhs)
    ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', ಹೊರಹೋಗು);
        output_escaped (ಹೊರಹೋಗು, symbols[*rhsp]->ಟ್ಯಾಗ್);
      }
  ಇಲ್ಲದಿದ್ದರೆ
    fputs (" %empty", ಹೊರಹೋಗು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
user_actions_output (FILE *ಹೊರಹೋಗು)
{
  fputs ("m4_define([b4_actions], \n[", ಹೊರಹೋಗು);
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    ಆದರೆ (ನಿಯಮಗಳು[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (ಹೊರಹೋಗು, "%s""(%d, [",
                 ನಿಯಮಗಳು[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        ಆದರೆ (!no_ಸಾಲುಗಳು_flag)
          {
            fprintf (ಹೊರಹೋಗು, "b4_syncline(%d, ",
                     ನಿಯಮಗಳು[r].action_loc.ಆರಂಭ.ಸಾಲು);
            string_output (ಹೊರಹೋಗು, map_file_name (ನಿಯಮಗಳು[r].action_loc.ಆರಂಭ.file));
            fprintf (ಹೊರಹೋಗು, ")dnl\n");
          }
        fprintf (ಹೊರಹೋಗು, "[%*s%s]],\n[[",
                 ನಿಯಮಗಳು[r].action_loc.ಆರಂಭ.ಸ್ತಂಭ - 1, "",
                 ನಿಯಮಗಳು[r].action);
        ನಿಯಮ_output (&ನಿಯಮಗಳು[r], ಹೊರಹೋಗು);
        fprintf (ಹೊರಹೋಗು, "]])\n\n");
      }
  fputs ("])\n\n", ಹೊರಹೋಗು);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
merger_output (FILE *ಹೊರಹೋಗು)
{
  fputs ("m4_define([b4_mergers], \n[[", ಹೊರಹೋಗು);
  ಪೂರ್ಣಾಂಕ n;
  merger_list* p;
  ಗಾಗಿ (n = 1, p = ವಿಲೀನ_functions; p != ಖಾಲಿ; n += 1, p = p->ಮುಂದಿನ)
    fprintf (ಹೊರಹೋಗು, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->ಸಂಖ್ಯೆ);
  fputs ("]])\n\n", ಹೊರಹೋಗು);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
prepare_ಚಿಹ್ನೆ_definitions (ಶೂನ್ಯ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ = obstack_finish0 (&format_obstack);
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, nterm_map ? nterm_map[i - ntokens] : i);
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; ++i)
    {
      ಚಿಹ್ನೆ *sym = symbols[i];
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ;

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_ಕೀಲಿ(ನಮೂದು_2)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, ನಮೂದು_2);                                \
      ಕೀಲಿ = obstack_finish0 (&format_obstack);

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_KEY2(ನಮೂದು_2, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, ನಮೂದು_2, Suffix);                        \
      ಕೀಲಿ = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id = ಚಿಹ್ನೆ_id_get (sym);
      ಗಣ_ಕೀಲಿ ("has_id");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, !!id);

      /* Its identifier.  */
      ಗಣ_ಕೀಲಿ ("id");
      MUSCLE_ಸೇರಿಸು_STRING (ಕೀಲಿ, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      ಗಣ_ಕೀಲಿ ("tag");
      MUSCLE_ಸೇರಿಸು_STRING (ಕೀಲಿ, ಚಿಹ್ನೆ_ಟ್ಯಾಗ್ (sym));

      ಗಣ_ಕೀಲಿ ("code");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, sym->content->ಸಂಕೇತ_2);

      ಗಣ_ಕೀಲಿ ("is_token");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, i < ntokens);

      ಗಣ_ಕೀಲಿ ("number");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, sym->content->ಸಂಖ್ಯೆ);

      ಗಣ_ಕೀಲಿ ("has_type");
      MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, !!sym->content->type_name);

      ಗಣ_ಕೀಲಿ ("type");
      MUSCLE_ಸೇರಿಸು_STRING (ಕೀಲಿ, sym->content->type_name
                            ? sym->content->type_name : "");

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಸಂಕೇತ_PROPS_ಗಾತ್ರ; ++j)
        {
          /* "printer", not "%printer".  */
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pname = ಸಂಕೇತ_props_type_string (j) + 1;
          ಸಂಕೇತ_props ಸ್ಥಿರಾಂಕ *p = ಚಿಹ್ನೆ_ಸಂಕೇತ_props_get (sym, j);
          ಗಣ_KEY2 ("has", pname);
          MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, !!p->ಸಂಕೇತ_2);

          ಆದರೆ (p->ಸಂಕೇತ_2)
            {
              ಗಣ_KEY2 (pname, "file");
              MUSCLE_ಸೇರಿಸು_C_STRING (ಕೀಲಿ, map_file_name (p->location.ಆರಂಭ.file));

              ಗಣ_KEY2 (pname, "line");
              MUSCLE_ಸೇರಿಸು_INT (ಕೀಲಿ, p->location.ಆರಂಭ.ಸಾಲು);

              ಗಣ_KEY2 (pname, "loc");
              muscle_location_grow (ಕೀಲಿ, p->location);

              ಗಣ_ಕೀಲಿ (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.ಆರಂಭ.ಸ್ತಂಭ - 1, "", p->ಸಂಕೇತ_2);
              muscle_ಸೇರಿಸು (ಕೀಲಿ, obstack_finish0 (&muscle_obstack));
            }
        }
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಗಣ_KEY2
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಗಣ_ಕೀಲಿ
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
prepare_actions (ಶೂನ್ಯ)
{
  /* Figure out the actions for the specified state.  */
  muscle_ಸೇರಿಸು_ನಿಯಮ_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_ಸೇರಿಸು_state_ಸಂಖ್ಯೆ_ಕೋಷ್ಟಕ ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_ಸೇರಿಸು_ತಳ_ಕೋಷ್ಟಕ ("pact", ತಳ,
                             ತಳ[0], 1, nstates);
  MUSCLE_ಸೇರಿಸು_INT ("pact_ninf", ತಳ_ninf);

  /* Output PGOTO. */
  muscle_ಸೇರಿಸು_ತಳ_ಕೋಷ್ಟಕ ("pgoto", ತಳ,
                             ತಳ[nstates], nstates + 1, nvectors);

  muscle_ಸೇರಿಸು_ತಳ_ಕೋಷ್ಟಕ ("table", ಕೋಷ್ಟಕ,
                            ಕೋಷ್ಟಕ[0], 1, ಮೇಲಿನ + 1);
  MUSCLE_ಸೇರಿಸು_INT ("table_ninf", ಕೋಷ್ಟಕ_ninf);

  muscle_ಸೇರಿಸು_ತಳ_ಕೋಷ್ಟಕ ("check", check,
                            check[0], 1, ಮೇಲಿನ + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("conflict_list_heads", conflict_ಕೋಷ್ಟಕ,
                           conflict_ಕೋಷ್ಟಕ[0], 1, ಮೇಲಿನ + 1);
  muscle_ಸೇರಿಸು_int_ಕೋಷ್ಟಕ ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
muscles_output (FILE *ಹೊರಹೋಗು)
{
  fputs ("m4_init()\n", ಹೊರಹೋಗು);
  merger_output (ಹೊರಹೋಗು);
  ಚಿಹ್ನೆ_numbers_output (ಹೊರಹೋಗು);
  type_ಹೆಸರುಗಳು_output (ಹೊರಹೋಗು);
  ಆರಂಭ_symbols_output (ಹೊರಹೋಗು);
  user_actions_output (ಹೊರಹೋಗು);
  /* Must be last.  */
  muscles_m4_output (ಹೊರಹೋಗು);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
output_skeleton (ಶೂನ್ಯ)
{
  /* Compute the names of the package data dir and skeleton files.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *m4 = m4path ();
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *datadir = pkgdatadir ();
  ಅಕ್ಷರ *skeldir = xpath_join (datadir, "skeletons");
  ಅಕ್ಷರ *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  ಅಕ್ಷರ *m4bison = xpath_join (skeldir, "bison.m4");
  ಅಕ್ಷರ *traceon = xpath_join (skeldir, "traceon.m4");
  ಅಕ್ಷರ *skel = (IS_ಮಾರ್ಗ_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  ಪೂರ್ಣಾಂಕ filter_fd[2];
  pid_t pid;
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *argv[11];
    ಪೂರ್ಣಾಂಕ i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ಆದರೆ (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    ಆದರೆ (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    ಆದರೆ (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = ಖಾಲಿ;
    aver (i <= ARRAY_CARDINALITY (argv));

    ಆದರೆ (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ ಖಾಲಿ,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  ಬಿಡುಗಡೆ (skeldir);
  ಬಿಡುಗಡೆ (m4sugar);
  ಬಿಡುಗಡೆ (m4bison);
  ಬಿಡುಗಡೆ (traceon);
  ಬಿಡುಗಡೆ (skel);

  ಆದರೆ (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *ಹೊರಹೋಗು = xfdopen (filter_fd[1], "w");
    muscles_output (ಹೊರಹೋಗು);
    xfclose (ಹೊರಹೋಗು);
  }

  /* Read and process m4's output.  */
  timevar_ತಳ್ಳು (tv_m4);
  {
    FILE *ಒಳ = xfdopen (filter_fd[0], "r");
    ಪರಿಶೀಲನೆ_skel (ಒಳ);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (ಒಳ));
    xfclose (ಒಳ);
  }
  wait_subprocess (pid, "m4", false, false, true, true, ಖಾಲಿ);
  timevar_ತೆಗೆ (tv_m4);
}

ಸ್ಥಿರ ಶೂನ್ಯ
prepare (ಶೂನ್ಯ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ತಳ್ಳು_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_ಸೇರಿಸು_STRING ("version_string", ಆವೃತ್ತಿ_2);
  MUSCLE_ಸೇರಿಸು_INT ("version", strversion_ಗಮ್ಯ_int (ಆವೃತ್ತಿ_2));
  MUSCLE_ಸೇರಿಸು_INT ("required_version", required_ಆವೃತ್ತಿ);

  /* Flags. */
  MUSCLE_ಸೇರಿಸು_BOOL ("header_flag", ಶೀರ್ಷಿಕೆ_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("glr_flag", glr_parser);
  MUSCLE_ಸೇರಿಸು_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_ಸೇರಿಸು_BOOL ("synclines_flag", !no_ಸಾಲುಗಳು_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("tag_seen_flag", ಟ್ಯಾಗ್_ನೋಡಿದ);
  MUSCLE_ಸೇರಿಸು_BOOL ("token_table_flag", token_ಕೋಷ್ಟಕ_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("use_push_for_pull_flag", use_ತಳ್ಳು_for_pull_flag);
  MUSCLE_ಸೇರಿಸು_BOOL ("yacc_flag", !location_ಖಾಲಿಗಳು (yacc_loc));

  /* File names.  */
  ಆದರೆ (spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ)
    MUSCLE_ಸೇರಿಸು_STRING ("prefix", spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ);

  MUSCLE_ಸೇರಿಸು_STRING ("file_name_all_but_ext", all_but_ext);

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec_mapped_ಶೀರ್ಷಿಕೆ_file = map_file_name (spec_ಶೀರ್ಷಿಕೆ_file);
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mapped_dir_ಪೂರ್ವಪ್ರತ್ಯಯ = map_file_name (dir_ಪೂರ್ವಪ್ರತ್ಯಯ);

#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE(Name) MUSCLE_ಸೇರಿಸು_STRING (#Name, Name ? Name : "")
  DEFINE (dir_ಪೂರ್ವಪ್ರತ್ಯಯ);
  DEFINE (mapped_dir_ಪೂರ್ವಪ್ರತ್ಯಯ);
  DEFINE (parser_file_name);
  DEFINE (spec_ಶೀರ್ಷಿಕೆ_file);
  DEFINE (spec_mapped_ಶೀರ್ಷಿಕೆ_file);
  DEFINE (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ);
  DEFINE (spec_ನಕ್ಷೆ_file);
  DEFINE (spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ);
  DEFINE (spec_outfile);
  DEFINE (spec_verbose_file);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  ಆದರೆ (skeleton)
    MUSCLE_ಸೇರಿಸು_C_STRING ("skeleton", skeleton);
  ಇಲ್ಲದಿದ್ದರೆ
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    ಅಕ್ಷರ *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_ಸೇರಿಸು_STRING_RAW ("skeletonsdir", skeldir);
    ಬಿಡುಗಡೆ (skeldir);
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
  prepare_ನಿಯಮಗಳು ();
  prepare_ಸ್ಥಿತಿಗಳು ();
  prepare_actions ();
  prepare_ಚಿಹ್ನೆ_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  ಆದರೆ (complaint_ಸ್ಥಿತಿ)
    unlink_generated_sources ();

  obstack_free (&format_obstack, ಖಾಲಿ);
}
