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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <filename.h> /* IS_PATH_WITH_DIR */
#अन्तर्भाव <get-errno.h>
#अन्तर्भाव <mbswidth.h>
#अन्तर्भाव <path-join.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <spawn-pipe.h>
#अन्तर्भाव <timevar.h>
#अन्तर्भाव <wait-process.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "output.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "scan-code.h"    /* max_left_semantic_context */
#अन्तर्भाव "scan-skel.h"
#अन्तर्भाव "symtab.h"
#अन्तर्भाव "tables.h"
#अन्तर्भाव "strversion.h"

स्थिर संरचना obstack format_obstack;


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


#परिभाषय GENERATE_MUSCLE_निवेशय_सारणी(Name, Type)                        \
                                                                        \
स्थिर शून्य                                                             \
Name (वर्ण ध्रुव *name, Type *सारणी_दत्त, Type first,                   \
      पूर्णाङ्क begin, पूर्णाङ्क अन्तः)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  पूर्णाङ्क j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  प्रति (पूर्णाङ्क i = begin; i < अन्तः; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      यदि (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      अन्यथा                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", सारणी_दत्त[i]);           \
      यदि (सारणी_दत्त[i] < min)                                          \
        min = सारणी_दत्त[i];                                            \
      यदि (max < सारणी_दत्त[i])                                          \
        max = सारणी_दत्त[i];                                            \
    }                                                                   \
  muscle_निवेशय (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  दीर्घ lmin = min;                                                      \
  दीर्घ lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_निवेशय_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_निवेशय_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_निवेशय_सारणी (muscle_निवेशय_int_सारणी, पूर्णाङ्क)
GENERATE_MUSCLE_निवेशय_सारणी (muscle_निवेशय_आधारः_सारणी, आधारः_सङ्ख्या)
GENERATE_MUSCLE_निवेशय_सारणी (muscle_निवेशय_नियमः_सङ्ख्या_सारणी, नियमः_सङ्ख्या)
GENERATE_MUSCLE_निवेशय_सारणी (muscle_निवेशय_चिह्नम्_सङ्ख्या_सारणी, चिह्नम्_सङ्ख्या)
GENERATE_MUSCLE_निवेशय_सारणी (muscle_निवेशय_item_सङ्ख्या_सारणी, item_सङ्ख्या)
GENERATE_MUSCLE_निवेशय_सारणी (muscle_निवेशय_state_सङ्ख्या_सारणी, state_सङ्ख्या)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

स्थिर शून्य
output_escaped (FILE *बहिर्गमनम्, ध्रुव वर्ण *cp)
{
  प्रति (; *cp; ++cp)
    विकल्प (*cp)
      {
      पक्ष '$': fputs ("$][", बहिर्गमनम्); भङ्ग;
      पक्ष '@': fputs ("@@",  बहिर्गमनम्); भङ्ग;
      पक्ष '[': fputs ("@{",  बहिर्गमनम्); भङ्ग;
      पक्ष ']': fputs ("@}",  बहिर्गमनम्); भङ्ग;

      पक्ष 'b':
        यदि (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", बहिर्गमनम्);
            cp += वाक्यदैर्घ्यम् ("b4_") - 1;
            भङ्ग;
          }
        अन्यथा
          गच्छ append;

      पक्ष 'm':
        यदि (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", बहिर्गमनम्);
            cp += वाक्यदैर्घ्यम् ("m4_") - 1;
            भङ्ग;
          }
        अन्यथा
          गच्छ append;

      append:
      स्वभाव:  fputc (*cp,   बहिर्गमनम्); भङ्ग;
      }
}

स्थिर शून्य
output_quoted (FILE *बहिर्गमनम्, वर्ण ध्रुव *cp)
{
  fprintf (बहिर्गमनम्, "[[");
  output_escaped (बहिर्गमनम्, cp);
  fprintf (बहिर्गमनम्, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

स्थिर शून्य
string_output (FILE *बहिर्गमनम्, वर्ण ध्रुव *string)
{
  output_quoted (बहिर्गमनम्, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
स्थिर ptrdiff_t
escape_trigraphs (वर्ण *बफर, ptrdiff_t buffersize, ध्रुव वर्ण *src)
{
#परिभाषय STORE(c)                                \
  कुरु                                            \
    {                                           \
      यदि (res < buffersize)                     \
        बफर[res] = (c);                      \
      ++res;                                    \
    }                                           \
  यावत् (0)
  ptrdiff_t res = 0;
  प्रति (ptrdiff_t i = 0, len = वाक्यदैर्घ्यम् (src); i < len; ++i)
    {
      यदि (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          विकल्प (src[i+2])
            {
            पक्ष '!': पक्ष '\'':
            पक्ष '(': पक्ष ')': पक्ष '-': पक्ष '/':
            पक्ष '<': पक्ष '=': पक्ष '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              अनुवर्त;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#अपरिभाषय STORE
  प्रत्यागम res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
स्थिर वर्ण *
xescape_trigraphs (ध्रुव वर्ण *src)
{
  ptrdiff_t bufsize = escape_trigraphs (रिक्त, 0, src);
  वर्ण *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  प्रत्यागम buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
स्थिर ध्रुव वर्ण *
चिह्नम्_चिह्नकम् (ध्रुव चिह्नम् *sym)
{
  ध्रुव bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->चिह्नकम्, "$end");

  यदि (!eof_is_user_defined && sym->content == eoftoken->content)
    प्रत्यागम "\"end of file\"";
  अन्यथा यदि (sym->content == undeftoken->content)
    प्रत्यागम "\"invalid token\"";
  अन्यथा
    प्रत्यागम sym->चिह्नकम्;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

स्थिर शून्य
prepare_चिह्नम्_नामानि (वर्ण ध्रुव *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ध्रुव bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  पूर्णाङ्क col = 2;
  संरचना quoting_options *qo = clone_quoting_options (0);
  समुच्चयः_quoting_style (qo, c_quoting_style);
  समुच्चयः_quoting_flags (qo, QA_विभज_TRIGRAPHS);
  प्रति (पूर्णाङ्क i = 0; i < nsyms; i++)
    {
      ध्रुव वर्ण *चिह्नकम् = चिह्नम्_चिह्नकम् (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      यदि (translatable)
        has_translations = true;

      वर्ण *cp
        = चिह्नकम्[0] == '"' && !quote
        ? xescape_trigraphs (चिह्नकम्)
        : quotearg_alloc (चिह्नकम्, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      पूर्णाङ्क विस्तारः
        = mbswidth (cp, 0) + 2
        + (translatable ? वाक्यदैर्घ्यम् ("N_()") : 0);

      यदि (col + विस्तारः > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      यदि (i)
        obstack_1grow (&format_obstack, ' ');
      यदि (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      यदि (translatable)
        obstack_sgrow (&format_obstack, "])[");
      मुक्त (cp);
      obstack_1grow (&format_obstack, ',');
      col += विस्तारः;
    }
  मुक्त (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_निवेशय (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_निवेशय_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

स्थिर शून्य
prepare_symbols (शून्य)
{
  MUSCLE_निवेशय_INT ("tokens_number", ntokens);
  MUSCLE_निवेशय_INT ("nterms_number", nnterms);
  MUSCLE_निवेशय_INT ("symbols_number", nsyms);
  MUSCLE_निवेशय_INT ("code_max", max_संहिता);

  muscle_निवेशय_चिह्नम्_सङ्ख्या_सारणी ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_संहिता + 1);

  /* tname -- token names.  */
  prepare_चिह्नम्_नामानि ("tname");
  prepare_चिह्नम्_नामानि ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
      यदि (symbols[i]->translatable)
        {
          translatable = true;
          भङ्ग;
        }
    यदि (translatable)
      {
        पूर्णाङ्क *मानानि = xnmalloc (nsyms, परिमाण *मानानि);
        प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
          मानानि[i] = symbols[i]->translatable;
        muscle_निवेशय_int_सारणी ("translatable", मानानि,
                                 मानानि[0], 1, ntokens);
        मुक्त (मानानि);
      }
  }

  /* Output YYTOKNUM. */
  {
    पूर्णाङ्क *मानानि = xnmalloc (ntokens, परिमाण *मानानि);
    प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
      मानानि[i] = symbols[i]->content->संहिता;
    muscle_निवेशय_int_सारणी ("toknum", मानानि,
                             मानानि[0], 1, ntokens);
    मुक्त (मानानि);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

स्थिर शून्य
prepare_नियमाः (शून्य)
{
  पूर्णाङ्क *prhs = xnmalloc (nrules, परिमाण *prhs);
  item_सङ्ख्या *rhs = xnmalloc (nritems, परिमाण *rhs);
  पूर्णाङ्क *rline = xnmalloc (nrules, परिमाण *rline);
  चिह्नम्_सङ्ख्या *r1 = xnmalloc (nrules, परिमाण *r1);
  पूर्णाङ्क *r2 = xnmalloc (nrules, परिमाण *r2);
  पूर्णाङ्क *dprec = xnmalloc (nrules, परिमाण *dprec);
  पूर्णाङ्क *merger = xnmalloc (nrules, परिमाण *merger);
  पूर्णाङ्क *तात्कालिकम् = xnmalloc (nrules, परिमाण *तात्कालिकम्);

  /* Index in RHS.  */
  पूर्णाङ्क i = 0;
  प्रति (नियमः_सङ्ख्या r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      प्रति (item_सङ्ख्या *rhsp = नियमाः[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = नियमाः[r].location.start.पङ्क्तिः;
      /* LHS of the rule R. */
      r1[r] = नियमाः[r].lhs->सङ्ख्या;
      /* Length of rule R's RHS. */
      r2[r] = नियमः_rhs_दीर्घता (&नियमाः[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = नियमाः[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = नियमाः[r].merger;
      /* Immediate reduction flags (GLR).  */
      तात्कालिकम्[r] = नियमाः[r].is_predicate;
    }
  aver (i == nritems);

  muscle_निवेशय_item_सङ्ख्या_सारणी ("rhs", rhs, ritem[0], 1, nritems);
  muscle_निवेशय_int_सारणी ("prhs", prhs, 0, 0, nrules);
  muscle_निवेशय_int_सारणी ("rline", rline, 0, 0, nrules);
  muscle_निवेशय_चिह्नम्_सङ्ख्या_सारणी ("r1", r1, 0, 0, nrules);
  muscle_निवेशय_int_सारणी ("r2", r2, 0, 0, nrules);
  muscle_निवेशय_int_सारणी ("dprec", dprec, 0, 0, nrules);
  muscle_निवेशय_int_सारणी ("merger", merger, 0, 0, nrules);
  muscle_निवेशय_int_सारणी ("immediate", तात्कालिकम्, 0, 0, nrules);

  MUSCLE_निवेशय_INT ("rules_number", nrules);
  MUSCLE_निवेशय_INT ("max_left_semantic_context", max_वामः_semantic_context);

  मुक्त (prhs);
  मुक्त (rhs);
  मुक्त (rline);
  मुक्त (r1);
  मुक्त (r2);
  मुक्त (dprec);
  मुक्त (merger);
  मुक्त (तात्कालिकम्);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

स्थिर शून्य
prepare_अवस्थाः (शून्य)
{
  चिह्नम्_सङ्ख्या *मानानि = xnmalloc (nstates, परिमाण *मानानि);
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    मानानि[i] = अवस्थाः[i]->accessing_चिह्नम्;
  muscle_निवेशय_चिह्नम्_सङ्ख्या_सारणी ("stos", मानानि,
                                     0, 1, nstates);
  मुक्त (मानानि);

  MUSCLE_निवेशय_INT ("last", उच्चः);
  MUSCLE_निवेशय_INT ("final_state_number", final_state->सङ्ख्या);
  MUSCLE_निवेशय_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

स्थिर पूर्णाङ्क
चिह्नम्_type_name_cmp (ध्रुव चिह्नम् **lhs, ध्रुव चिह्नम् **rhs)
{
  पूर्णाङ्क res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  यदि (!res)
    res = (*lhs)->content->सङ्ख्या - (*rhs)->content->सङ्ख्या;
  प्रत्यागम res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

स्थिर चिह्नम् **
symbols_by_type_name (शून्य)
{
  प्रकारनिर्देश पूर्णाङ्क (*qcmp_type) (ध्रुव शून्य *, ध्रुव शून्य *);
  चिह्नम् **res = xmemdup (symbols, nsyms * परिमाण *res);
  शीघ्रं_क्रमय (res, nsyms, परिमाण *res, (qcmp_type) &चिह्नम्_type_name_cmp);
  प्रत्यागम res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

स्थिर शून्य
type_नामानि_output (FILE *बहिर्गमनम्)
{
  चिह्नम् **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", बहिर्गमनम्);
  प्रति (पूर्णाङ्क i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      पूर्णाङ्क i0 = i;
      fputs (i ? ",\n[" : "[", बहिर्गमनम्);
      प्रति (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (बहिर्गमनम्, "%s%d", i != i0 ? ", " : "", syms[i]->content->सङ्ख्या);
      fputs ("]", बहिर्गमनम्);
    }
  fputs ("])\n\n", बहिर्गमनम्);
  मुक्त (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
स्थिर शून्य
start_symbols_output (FILE *बहिर्गमनम्)
{
  यदि (start_symbols && start_symbols->अग्र)
    {
      fputs ("m4_define([b4_start_symbols],\n[", बहिर्गमनम्);
      प्रति (चिह्नम्_list *list = start_symbols; list; list = list->अग्र)
        {
          ध्रुव चिह्नम् *start = list->content.sym;
          ध्रुव चिह्नम् *swtok = switching_token (start);
          fprintf (बहिर्गमनम्, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->सङ्ख्या, swtok->content->सङ्ख्या);
        }
      fputs ("])\n\n", बहिर्गमनम्);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

स्थिर शून्य
चिह्नम्_numbers_output (FILE *बहिर्गमनम्)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", बहिर्गमनम्);
  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
    fprintf (बहिर्गमनम्, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", बहिर्गमनम्);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

स्थिर शून्य
नियमः_output (ध्रुव नियमः *r, FILE *बहिर्गमनम्)
{
  output_escaped (बहिर्गमनम्, r->lhs->चिह्नम्->चिह्नकम्);
  fputc (':', बहिर्गमनम्);
  यदि (0 <= *r->rhs)
    प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', बहिर्गमनम्);
        output_escaped (बहिर्गमनम्, symbols[*rhsp]->चिह्नकम्);
      }
  अन्यथा
    fputs (" %empty", बहिर्गमनम्);
}

स्थिर शून्य
user_actions_output (FILE *बहिर्गमनम्)
{
  fputs ("m4_define([b4_actions], \n[", बहिर्गमनम्);
  प्रति (नियमः_सङ्ख्या r = 0; r < nrules; ++r)
    यदि (नियमाः[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (बहिर्गमनम्, "%s""(%d, [",
                 नियमाः[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        यदि (!no_पङ्क्तयः_flag)
          {
            fprintf (बहिर्गमनम्, "b4_syncline(%d, ",
                     नियमाः[r].action_loc.start.पङ्क्तिः);
            string_output (बहिर्गमनम्, map_file_name (नियमाः[r].action_loc.start.file));
            fprintf (बहिर्गमनम्, ")dnl\n");
          }
        fprintf (बहिर्गमनम्, "[%*s%s]],\n[[",
                 नियमाः[r].action_loc.start.column - 1, "",
                 नियमाः[r].action);
        नियमः_output (&नियमाः[r], बहिर्गमनम्);
        fprintf (बहिर्गमनम्, "]])\n\n");
      }
  fputs ("])\n\n", बहिर्गमनम्);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

स्थिर शून्य
merger_output (FILE *बहिर्गमनम्)
{
  fputs ("m4_define([b4_mergers], \n[[", बहिर्गमनम्);
  पूर्णाङ्क n;
  merger_list* p;
  प्रति (n = 1, p = merge_functions; p != रिक्त; n += 1, p = p->अग्र)
    fprintf (बहिर्गमनम्, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->सङ्ख्या);
  fputs ("]])\n\n", बहिर्गमनम्);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

स्थिर शून्य
prepare_चिह्नम्_definitions (शून्य)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ध्रुव वर्ण *कुञ्चिका = obstack_finish0 (&format_obstack);
      MUSCLE_निवेशय_INT (कुञ्चिका, nterm_map ? nterm_map[i - ntokens] : i);
    }

  प्रति (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      चिह्नम् *sym = symbols[i];
      ध्रुव वर्ण *कुञ्चिका;

#परिभाषय समुच्चयः_कुञ्चिका(प्रविष्टिः_2)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, प्रविष्टिः_2);                                \
      कुञ्चिका = obstack_finish0 (&format_obstack);

#परिभाषय समुच्चयः_KEY2(प्रविष्टिः_2, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, प्रविष्टिः_2, Suffix);                        \
      कुञ्चिका = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ध्रुव वर्ण *id = चिह्नम्_id_get (sym);
      समुच्चयः_कुञ्चिका ("has_id");
      MUSCLE_निवेशय_INT (कुञ्चिका, !!id);

      /* Its identifier.  */
      समुच्चयः_कुञ्चिका ("id");
      MUSCLE_निवेशय_STRING (कुञ्चिका, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      समुच्चयः_कुञ्चिका ("tag");
      MUSCLE_निवेशय_STRING (कुञ्चिका, चिह्नम्_चिह्नकम् (sym));

      समुच्चयः_कुञ्चिका ("code");
      MUSCLE_निवेशय_INT (कुञ्चिका, sym->content->संहिता);

      समुच्चयः_कुञ्चिका ("is_token");
      MUSCLE_निवेशय_INT (कुञ्चिका, i < ntokens);

      समुच्चयः_कुञ्चिका ("number");
      MUSCLE_निवेशय_INT (कुञ्चिका, sym->content->सङ्ख्या);

      समुच्चयः_कुञ्चिका ("has_type");
      MUSCLE_निवेशय_INT (कुञ्चिका, !!sym->content->type_name);

      समुच्चयः_कुञ्चिका ("type");
      MUSCLE_निवेशय_STRING (कुञ्चिका, sym->content->type_name
                            ? sym->content->type_name : "");

      प्रति (पूर्णाङ्क j = 0; j < संहिता_PROPS_आकार; ++j)
        {
          /* "printer", not "%printer".  */
          वर्ण ध्रुव *pname = संहिता_props_type_string (j) + 1;
          संहिता_props ध्रुव *p = चिह्नम्_संहिता_props_get (sym, j);
          समुच्चयः_KEY2 ("has", pname);
          MUSCLE_निवेशय_INT (कुञ्चिका, !!p->संहिता);

          यदि (p->संहिता)
            {
              समुच्चयः_KEY2 (pname, "file");
              MUSCLE_निवेशय_C_STRING (कुञ्चिका, map_file_name (p->location.start.file));

              समुच्चयः_KEY2 (pname, "line");
              MUSCLE_निवेशय_INT (कुञ्चिका, p->location.start.पङ्क्तिः);

              समुच्चयः_KEY2 (pname, "loc");
              muscle_location_grow (कुञ्चिका, p->location);

              समुच्चयः_कुञ्चिका (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->संहिता);
              muscle_निवेशय (कुञ्चिका, obstack_finish0 (&muscle_obstack));
            }
        }
#अपरिभाषय समुच्चयः_KEY2
#अपरिभाषय समुच्चयः_कुञ्चिका
    }
}


स्थिर शून्य
prepare_actions (शून्य)
{
  /* Figure out the actions for the specified state.  */
  muscle_निवेशय_नियमः_सङ्ख्या_सारणी ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_निवेशय_state_सङ्ख्या_सारणी ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_निवेशय_आधारः_सारणी ("pact", आधारः,
                             आधारः[0], 1, nstates);
  MUSCLE_निवेशय_INT ("pact_ninf", आधारः_ninf);

  /* Output PGOTO. */
  muscle_निवेशय_आधारः_सारणी ("pgoto", आधारः,
                             आधारः[nstates], nstates + 1, nvectors);

  muscle_निवेशय_आधारः_सारणी ("table", सारणी,
                            सारणी[0], 1, उच्चः + 1);
  MUSCLE_निवेशय_INT ("table_ninf", सारणी_ninf);

  muscle_निवेशय_आधारः_सारणी ("check", check,
                            check[0], 1, उच्चः + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_निवेशय_int_सारणी ("conflict_list_heads", conflict_सारणी,
                           conflict_सारणी[0], 1, उच्चः + 1);
  muscle_निवेशय_int_सारणी ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

स्थिर शून्य
muscles_output (FILE *बहिर्गमनम्)
{
  fputs ("m4_init()\n", बहिर्गमनम्);
  merger_output (बहिर्गमनम्);
  चिह्नम्_numbers_output (बहिर्गमनम्);
  type_नामानि_output (बहिर्गमनम्);
  start_symbols_output (बहिर्गमनम्);
  user_actions_output (बहिर्गमनम्);
  /* Must be last.  */
  muscles_m4_output (बहिर्गमनम्);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

स्थिर शून्य
output_skeleton (शून्य)
{
  /* Compute the names of the package data dir and skeleton files.  */
  वर्ण ध्रुव *m4 = m4path ();
  वर्ण ध्रुव *datadir = pkgdatadir ();
  वर्ण *skeldir = xpath_join (datadir, "skeletons");
  वर्ण *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  वर्ण *m4bison = xpath_join (skeldir, "bison.m4");
  वर्ण *traceon = xpath_join (skeldir, "traceon.m4");
  वर्ण *skel = (IS_मार्गः_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  पूर्णाङ्क filter_fd[2];
  pid_t pid;
  {
    वर्ण ध्रुव *argv[11];
    पूर्णाङ्क i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    यदि (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    यदि (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    यदि (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = रिक्त;
    aver (i <= ARRAY_CARDINALITY (argv));

    यदि (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        प्रति (पूर्णाङ्क j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ रिक्त,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  मुक्त (skeldir);
  मुक्त (m4sugar);
  मुक्त (m4bison);
  मुक्त (traceon);
  मुक्त (skel);

  यदि (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *बहिर्गमनम् = xfdopen (filter_fd[1], "w");
    muscles_output (बहिर्गमनम्);
    xfclose (बहिर्गमनम्);
  }

  /* Read and process m4's output.  */
  timevar_प्रक्षिप (tv_m4);
  {
    FILE *अन्तर् = xfdopen (filter_fd[0], "r");
    अवलोकनम्_skel (अन्तर्);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (अन्तर्));
    xfclose (अन्तर्);
  }
  wait_subprocess (pid, "m4", false, false, true, true, रिक्त);
  timevar_निष्कासय (tv_m4);
}

स्थिर शून्य
prepare (शून्य)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  वर्ण ध्रुव *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_प्रक्षिप_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_निवेशय_STRING ("version_string", संस्करणम्_2);
  MUSCLE_निवेशय_INT ("version", strversion_गन्तव्यम्_int (संस्करणम्_2));
  MUSCLE_निवेशय_INT ("required_version", required_संस्करणम्);

  /* Flags. */
  MUSCLE_निवेशय_BOOL ("header_flag", शीर्षकम्_flag);
  MUSCLE_निवेशय_BOOL ("glr_flag", glr_parser);
  MUSCLE_निवेशय_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_निवेशय_BOOL ("synclines_flag", !no_पङ्क्तयः_flag);
  MUSCLE_निवेशय_BOOL ("tag_seen_flag", चिह्नकम्_आलोकितम्);
  MUSCLE_निवेशय_BOOL ("token_table_flag", token_सारणी_flag);
  MUSCLE_निवेशय_BOOL ("use_push_for_pull_flag", use_प्रक्षिप_for_pull_flag);
  MUSCLE_निवेशय_BOOL ("yacc_flag", !location_रिक्तानि (yacc_loc));

  /* File names.  */
  यदि (spec_name_उपसर्गः)
    MUSCLE_निवेशय_STRING ("prefix", spec_name_उपसर्गः);

  MUSCLE_निवेशय_STRING ("file_name_all_but_ext", all_but_ext);

  ध्रुव वर्ण *spec_mapped_शीर्षकम्_file = map_file_name (spec_शीर्षकम्_file);
  ध्रुव वर्ण *mapped_dir_उपसर्गः = map_file_name (dir_उपसर्गः);

#परिभाषय DEFINE(Name) MUSCLE_निवेशय_STRING (#Name, Name ? Name : "")
  DEFINE (dir_उपसर्गः);
  DEFINE (mapped_dir_उपसर्गः);
  DEFINE (parser_file_name);
  DEFINE (spec_शीर्षकम्_file);
  DEFINE (spec_mapped_शीर्षकम्_file);
  DEFINE (spec_file_उपसर्गः);
  DEFINE (spec_graph_file);
  DEFINE (spec_name_उपसर्गः);
  DEFINE (spec_outfile);
  DEFINE (spec_verbose_file);
#अपरिभाषय DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  यदि (skeleton)
    MUSCLE_निवेशय_C_STRING ("skeleton", skeleton);
  अन्यथा
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    वर्ण *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_निवेशय_STRING_RAW ("skeletonsdir", skeldir);
    मुक्त (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

शून्य
output (शून्य)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_नियमाः ();
  prepare_अवस्थाः ();
  prepare_actions ();
  prepare_चिह्नम्_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  यदि (complaint_स्थितिः)
    unlink_generated_sources ();

  obstack_free (&format_obstack, रिक्त);
}
