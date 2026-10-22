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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <filename.h> /* IS_PATH_WITH_DIR */
#ضمن <get-errno.h>
#ضمن <mbswidth.h>
#ضمن <path-join.h>
#ضمن <quotearg.h>
#ضمن <spawn-pipe.h>
#ضمن <timevar.h>
#ضمن <wait-process.h>

#ضمن "complain.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "muscle-tab.h"
#ضمن "output.h"
#ضمن "reader.h"
#ضمن "reduce.h"
#ضمن "scan-code.h"    /* max_left_semantic_context */
#ضمن "scan-skel.h"
#ضمن "symtab.h"
#ضمن "tables.h"
#ضمن "strversion.h"

ساكن بنية obstack format_obstack;


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


#تعریف_کرو GENERATE_MUSCLE_داخل_کرو_جدول(Name, Type)                        \
                                                                        \
ساكن فراغ                                                             \
Name (حرف ثابت *name, Type *جدول_انکڑے, Type پہلا,                   \
      صحيح begin, صحيح سرا)                                               \
{                                                                       \
  Type min = پہلا;                                                     \
  Type max = پہلا;                                                     \
  صحيح j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", پہلا);                       \
  لكل (صحيح i = begin; i < سرا; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      إذا (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      وإلا                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", جدول_انکڑے[i]);           \
      إذا (جدول_انکڑے[i] < min)                                          \
        min = جدول_انکڑے[i];                                            \
      إذا (max < جدول_انکڑے[i])                                          \
        max = جدول_انکڑے[i];                                            \
    }                                                                   \
  muscle_داخل_کرو (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  لما lmin = min;                                                      \
  لما lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_داخل_کرو_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_داخل_کرو_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_داخل_کرو_جدول (muscle_داخل_کرو_int_جدول, صحيح)
GENERATE_MUSCLE_داخل_کرو_جدول (muscle_داخل_کرو_ادھار_جدول, ادھار_نمبر)
GENERATE_MUSCLE_داخل_کرو_جدول (muscle_داخل_کرو_قاعدہ_نمبر_جدول, قاعدہ_نمبر)
GENERATE_MUSCLE_داخل_کرو_جدول (muscle_داخل_کرو_علامت_نمبر_جدول, علامت_نمبر)
GENERATE_MUSCLE_داخل_کرو_جدول (muscle_داخل_کرو_شے_نمبر_جدول, شے_نمبر)
GENERATE_MUSCLE_داخل_کرو_جدول (muscle_داخل_کرو_state_نمبر_جدول, state_نمبر)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ساكن فراغ
output_escaped (FILE *باہرلا, ثابت حرف *cp)
{
  لكل (; *cp; ++cp)
    بدل (*cp)
      {
      حالة '$': fputs ("$][", باہرلا); اكسر;
      حالة '@': fputs ("@@",  باہرلا); اكسر;
      حالة '[': fputs ("@{",  باہرلا); اكسر;
      حالة ']': fputs ("@}",  باہرلا); اكسر;

      حالة 'b':
        إذا (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", باہرلا);
            cp += لڑی_لمبائی ("b4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      حالة 'm':
        إذا (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", باہرلا);
            cp += لڑی_لمبائی ("m4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      append:
      افتراضي:  fputc (*cp,   باہرلا); اكسر;
      }
}

ساكن فراغ
output_quoted (FILE *باہرلا, حرف ثابت *cp)
{
  fprintf (باہرلا, "[[");
  output_escaped (باہرلا, cp);
  fprintf (باہرلا, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ساكن فراغ
string_output (FILE *باہرلا, حرف ثابت *string)
{
  output_quoted (باہرلا, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ساكن ptrdiff_t
escape_trigraphs (حرف *بفر, ptrdiff_t buffersize, ثابت حرف *src)
{
#تعریف_کرو STORE(c)                                \
  افعل                                            \
    {                                           \
      إذا (res < buffersize)                     \
        بفر[res] = (c);                      \
      ++res;                                    \
    }                                           \
  طالما (0)
  ptrdiff_t res = 0;
  لكل (ptrdiff_t i = 0, len = لڑی_لمبائی (src); i < len; ++i)
    {
      إذا (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          بدل (src[i+2])
            {
            حالة '!': حالة '\'':
            حالة '(': حالة ')': حالة '-': حالة '/':
            حالة '<': حالة '=': حالة '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              استمر;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#تعریف_رد STORE
  أرجع res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
ساكن حرف *
xescape_trigraphs (ثابت حرف *src)
{
  ptrdiff_t bufsize = escape_trigraphs (عدم, 0, src);
  حرف *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  أرجع buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
ساكن ثابت حرف *
علامت_ٹیگ (ثابت علامت *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->ٹیگ, "$end");

  إذا (!eof_is_user_defined && sym->content == eoftoken->content)
    أرجع "\"end of file\"";
  وإلا إذا (sym->content == undeftoken->content)
    أرجع "\"invalid token\"";
  وإلا
    أرجع sym->ٹیگ;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ساكن فراغ
prepare_علامت_ناں (حرف ثابت *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ثابت bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  صحيح خانہ = 2;
  بنية quoting_options *qo = clone_quoting_options (0);
  سیٹ_quoting_style (qo, c_quoting_style);
  سیٹ_quoting_flags (qo, QA_ونڈو_TRIGRAPHS);
  لكل (صحيح i = 0; i < nsyms; i++)
    {
      ثابت حرف *ٹیگ = علامت_ٹیگ (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      إذا (translatable)
        has_translations = true;

      حرف *cp
        = ٹیگ[0] == '"' && !quote
        ? xescape_trigraphs (ٹیگ)
        : quotearg_alloc (ٹیگ, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      صحيح چوڑائی
        = mbswidth (cp, 0) + 2
        + (translatable ? لڑی_لمبائی ("N_()") : 0);

      إذا (خانہ + چوڑائی > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          خانہ = 1;
        }

      إذا (i)
        obstack_1grow (&format_obstack, ' ');
      إذا (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      إذا (translatable)
        obstack_sgrow (&format_obstack, "])[");
      حرر (cp);
      obstack_1grow (&format_obstack, ',');
      خانہ += چوڑائی;
    }
  حرر (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_داخل_کرو (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_داخل_کرو_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ساكن فراغ
prepare_symbols (فراغ)
{
  MUSCLE_داخل_کرو_INT ("tokens_number", ntokens);
  MUSCLE_داخل_کرو_INT ("nterms_number", nnterms);
  MUSCLE_داخل_کرو_INT ("symbols_number", nsyms);
  MUSCLE_داخل_کرو_INT ("code_max", max_کوڈ);

  muscle_داخل_کرو_علامت_نمبر_جدول ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_کوڈ + 1);

  /* tname -- token names.  */
  prepare_علامت_ناں ("tname");
  prepare_علامت_ناں ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    لكل (صحيح i = 0; i < ntokens; ++i)
      إذا (symbols[i]->translatable)
        {
          translatable = true;
          اكسر;
        }
    إذا (translatable)
      {
        صحيح *اقدار = xnmalloc (nsyms, حجم *اقدار);
        لكل (صحيح i = 0; i < ntokens; ++i)
          اقدار[i] = symbols[i]->translatable;
        muscle_داخل_کرو_int_جدول ("translatable", اقدار,
                                 اقدار[0], 1, ntokens);
        حرر (اقدار);
      }
  }

  /* Output YYTOKNUM. */
  {
    صحيح *اقدار = xnmalloc (ntokens, حجم *اقدار);
    لكل (صحيح i = 0; i < ntokens; ++i)
      اقدار[i] = symbols[i]->content->کوڈ;
    muscle_داخل_کرو_int_جدول ("toknum", اقدار,
                             اقدار[0], 1, ntokens);
    حرر (اقدار);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ساكن فراغ
prepare_قاعدے (فراغ)
{
  صحيح *prhs = xnmalloc (nrules, حجم *prhs);
  شے_نمبر *rhs = xnmalloc (nritems, حجم *rhs);
  صحيح *rline = xnmalloc (nrules, حجم *rline);
  علامت_نمبر *r1 = xnmalloc (nrules, حجم *r1);
  صحيح *r2 = xnmalloc (nrules, حجم *r2);
  صحيح *dprec = xnmalloc (nrules, حجم *dprec);
  صحيح *merger = xnmalloc (nrules, حجم *merger);
  صحيح *فوری = xnmalloc (nrules, حجم *فوری);

  /* Index in RHS.  */
  صحيح i = 0;
  لكل (قاعدہ_نمبر r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      لكل (شے_نمبر *rhsp = قاعدے[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = قاعدے[r].location.شروع.سطر;
      /* LHS of the rule R. */
      r1[r] = قاعدے[r].lhs->نمبر;
      /* Length of rule R's RHS. */
      r2[r] = قاعدہ_rhs_لمائی (&قاعدے[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = قاعدے[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = قاعدے[r].merger;
      /* Immediate reduction flags (GLR).  */
      فوری[r] = قاعدے[r].is_predicate;
    }
  aver (i == nritems);

  muscle_داخل_کرو_شے_نمبر_جدول ("rhs", rhs, ritem[0], 1, nritems);
  muscle_داخل_کرو_int_جدول ("prhs", prhs, 0, 0, nrules);
  muscle_داخل_کرو_int_جدول ("rline", rline, 0, 0, nrules);
  muscle_داخل_کرو_علامت_نمبر_جدول ("r1", r1, 0, 0, nrules);
  muscle_داخل_کرو_int_جدول ("r2", r2, 0, 0, nrules);
  muscle_داخل_کرو_int_جدول ("dprec", dprec, 0, 0, nrules);
  muscle_داخل_کرو_int_جدول ("merger", merger, 0, 0, nrules);
  muscle_داخل_کرو_int_جدول ("immediate", فوری, 0, 0, nrules);

  MUSCLE_داخل_کرو_INT ("rules_number", nrules);
  MUSCLE_داخل_کرو_INT ("max_left_semantic_context", max_بایاں_semantic_context);

  حرر (prhs);
  حرر (rhs);
  حرر (rline);
  حرر (r1);
  حرر (r2);
  حرر (dprec);
  حرر (merger);
  حرر (فوری);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ساكن فراغ
prepare_حالتاں (فراغ)
{
  علامت_نمبر *اقدار = xnmalloc (nstates, حجم *اقدار);
  لكل (state_نمبر i = 0; i < nstates; ++i)
    اقدار[i] = حالتاں[i]->accessing_علامت;
  muscle_داخل_کرو_علامت_نمبر_جدول ("stos", اقدار,
                                     0, 1, nstates);
  حرر (اقدار);

  MUSCLE_داخل_کرو_INT ("last", بلند);
  MUSCLE_داخل_کرو_INT ("final_state_number", final_state->نمبر);
  MUSCLE_داخل_کرو_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ساكن صحيح
علامت_type_name_cmp (ثابت علامت **lhs, ثابت علامت **rhs)
{
  صحيح res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  إذا (!res)
    res = (*lhs)->content->نمبر - (*rhs)->content->نمبر;
  أرجع res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ساكن علامت **
symbols_by_type_name (فراغ)
{
  تعريفنوع صحيح (*qcmp_type) (ثابت فراغ *, ثابت فراغ *);
  علامت **res = xmemdup (symbols, nsyms * حجم *res);
  qsort (res, nsyms, حجم *res, (qcmp_type) &علامت_type_name_cmp);
  أرجع res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ساكن فراغ
type_ناں_output (FILE *باہرلا)
{
  علامت **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", باہرلا);
  لكل (صحيح i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      صحيح i0 = i;
      fputs (i ? ",\n[" : "[", باہرلا);
      لكل (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (باہرلا, "%s%d", i != i0 ? ", " : "", syms[i]->content->نمبر);
      fputs ("]", باہرلا);
    }
  fputs ("])\n\n", باہرلا);
  حرر (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ساكن فراغ
شروع_symbols_output (FILE *باہرلا)
{
  إذا (شروع_symbols && شروع_symbols->اگلا)
    {
      fputs ("m4_define([b4_start_symbols],\n[", باہرلا);
      لكل (علامت_list *list = شروع_symbols; list; list = list->اگلا)
        {
          ثابت علامت *شروع = list->content.sym;
          ثابت علامت *swtok = switching_token (شروع);
          fprintf (باہرلا, "%s[%d, %d]",
                   list == شروع_symbols ? "" : ", ",
                   شروع->content->نمبر, swtok->content->نمبر);
        }
      fputs ("])\n\n", باہرلا);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ساكن فراغ
علامت_numbers_output (FILE *باہرلا)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", باہرلا);
  لكل (صحيح i = 0; i < nsyms; ++i)
    fprintf (باہرلا, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", باہرلا);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ساكن فراغ
قاعدہ_output (ثابت قاعدہ *r, FILE *باہرلا)
{
  output_escaped (باہرلا, r->lhs->علامت->ٹیگ);
  fputc (':', باہرلا);
  إذا (0 <= *r->rhs)
    لكل (شے_نمبر *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', باہرلا);
        output_escaped (باہرلا, symbols[*rhsp]->ٹیگ);
      }
  وإلا
    fputs (" %empty", باہرلا);
}

ساكن فراغ
user_actions_output (FILE *باہرلا)
{
  fputs ("m4_define([b4_actions], \n[", باہرلا);
  لكل (قاعدہ_نمبر r = 0; r < nrules; ++r)
    إذا (قاعدے[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (باہرلا, "%s""(%d, [",
                 قاعدے[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        إذا (!no_سطریں_flag)
          {
            fprintf (باہرلا, "b4_syncline(%d, ",
                     قاعدے[r].action_loc.شروع.سطر);
            string_output (باہرلا, map_file_name (قاعدے[r].action_loc.شروع.file));
            fprintf (باہرلا, ")dnl\n");
          }
        fprintf (باہرلا, "[%*s%s]],\n[[",
                 قاعدے[r].action_loc.شروع.کالم - 1, "",
                 قاعدے[r].action);
        قاعدہ_output (&قاعدے[r], باہرلا);
        fprintf (باہرلا, "]])\n\n");
      }
  fputs ("])\n\n", باہرلا);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

ساكن فراغ
merger_output (FILE *باہرلا)
{
  fputs ("m4_define([b4_mergers], \n[[", باہرلا);
  صحيح n;
  merger_list* p;
  لكل (n = 1, p = ملاؤ_functions; p != عدم; n += 1, p = p->اگلا)
    fprintf (باہرلا, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->نمبر);
  fputs ("]])\n\n", باہرلا);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ساكن فراغ
prepare_علامت_definitions (فراغ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  لكل (علامت_نمبر i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ثابت حرف *کلید = obstack_finish0 (&format_obstack);
      MUSCLE_داخل_کرو_INT (کلید, nterm_map ? nterm_map[i - ntokens] : i);
    }

  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      علامت *sym = symbols[i];
      ثابت حرف *کلید;

#تعریف_کرو سیٹ_کلید(اندراج_2)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, اندراج_2);                                \
      کلید = obstack_finish0 (&format_obstack);

#تعریف_کرو سیٹ_KEY2(اندراج_2, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, اندراج_2, Suffix);                        \
      کلید = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ثابت حرف *id = علامت_id_get (sym);
      سیٹ_کلید ("has_id");
      MUSCLE_داخل_کرو_INT (کلید, !!id);

      /* Its identifier.  */
      سیٹ_کلید ("id");
      MUSCLE_داخل_کرو_STRING (کلید, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      سیٹ_کلید ("tag");
      MUSCLE_داخل_کرو_STRING (کلید, علامت_ٹیگ (sym));

      سیٹ_کلید ("code");
      MUSCLE_داخل_کرو_INT (کلید, sym->content->کوڈ);

      سیٹ_کلید ("is_token");
      MUSCLE_داخل_کرو_INT (کلید, i < ntokens);

      سیٹ_کلید ("number");
      MUSCLE_داخل_کرو_INT (کلید, sym->content->نمبر);

      سیٹ_کلید ("has_type");
      MUSCLE_داخل_کرو_INT (کلید, !!sym->content->type_name);

      سیٹ_کلید ("type");
      MUSCLE_داخل_کرو_STRING (کلید, sym->content->type_name
                            ? sym->content->type_name : "");

      لكل (صحيح j = 0; j < کوڈ_PROPS_ناپ; ++j)
        {
          /* "printer", not "%printer".  */
          حرف ثابت *pname = کوڈ_props_type_string (j) + 1;
          کوڈ_props ثابت *p = علامت_کوڈ_props_get (sym, j);
          سیٹ_KEY2 ("has", pname);
          MUSCLE_داخل_کرو_INT (کلید, !!p->کوڈ);

          إذا (p->کوڈ)
            {
              سیٹ_KEY2 (pname, "file");
              MUSCLE_داخل_کرو_C_STRING (کلید, map_file_name (p->location.شروع.file));

              سیٹ_KEY2 (pname, "line");
              MUSCLE_داخل_کرو_INT (کلید, p->location.شروع.سطر);

              سیٹ_KEY2 (pname, "loc");
              muscle_location_grow (کلید, p->location);

              سیٹ_کلید (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.شروع.کالم - 1, "", p->کوڈ);
              muscle_داخل_کرو (کلید, obstack_finish0 (&muscle_obstack));
            }
        }
#تعریف_رد سیٹ_KEY2
#تعریف_رد سیٹ_کلید
    }
}


ساكن فراغ
prepare_actions (فراغ)
{
  /* Figure out the actions for the specified state.  */
  muscle_داخل_کرو_قاعدہ_نمبر_جدول ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_داخل_کرو_state_نمبر_جدول ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_داخل_کرو_ادھار_جدول ("pact", ادھار,
                             ادھار[0], 1, nstates);
  MUSCLE_داخل_کرو_INT ("pact_ninf", ادھار_ninf);

  /* Output PGOTO. */
  muscle_داخل_کرو_ادھار_جدول ("pgoto", ادھار,
                             ادھار[nstates], nstates + 1, nvectors);

  muscle_داخل_کرو_ادھار_جدول ("table", جدول,
                            جدول[0], 1, بلند + 1);
  MUSCLE_داخل_کرو_INT ("table_ninf", جدول_ninf);

  muscle_داخل_کرو_ادھار_جدول ("check", check,
                            check[0], 1, بلند + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_داخل_کرو_int_جدول ("conflict_list_heads", conflict_جدول,
                           conflict_جدول[0], 1, بلند + 1);
  muscle_داخل_کرو_int_جدول ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ساكن فراغ
muscles_output (FILE *باہرلا)
{
  fputs ("m4_init()\n", باہرلا);
  merger_output (باہرلا);
  علامت_numbers_output (باہرلا);
  type_ناں_output (باہرلا);
  شروع_symbols_output (باہرلا);
  user_actions_output (باہرلا);
  /* Must be last.  */
  muscles_m4_output (باہرلا);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

ساكن فراغ
output_skeleton (فراغ)
{
  /* Compute the names of the package data dir and skeleton files.  */
  حرف ثابت *m4 = m4path ();
  حرف ثابت *datadir = pkgdatadir ();
  حرف *skeldir = xpath_join (datadir, "skeletons");
  حرف *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  حرف *m4bison = xpath_join (skeldir, "bison.m4");
  حرف *traceon = xpath_join (skeldir, "traceon.m4");
  حرف *skel = (IS_رستہ_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  صحيح filter_fd[2];
  pid_t pid;
  {
    حرف ثابت *argv[11];
    صحيح i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    إذا (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    إذا (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    إذا (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = عدم;
    aver (i <= ARRAY_CARDINALITY (argv));

    إذا (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        لكل (صحيح j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ عدم,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  حرر (skeldir);
  حرر (m4sugar);
  حرر (m4bison);
  حرر (traceon);
  حرر (skel);

  إذا (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *باہرلا = xfdopen (filter_fd[1], "w");
    muscles_output (باہرلا);
    xfclose (باہرلا);
  }

  /* Read and process m4's output.  */
  timevar_ڈالو (tv_m4);
  {
    FILE *وچ = xfdopen (filter_fd[0], "r");
    جانچ_skel (وچ);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (وچ));
    xfclose (وچ);
  }
  wait_subprocess (pid, "m4", false, false, true, true, عدم);
  timevar_نکالو (tv_m4);
}

ساكن فراغ
prepare (فراغ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  حرف ثابت *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ڈالو_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_داخل_کرو_STRING ("version_string", نسخہ_2);
  MUSCLE_داخل_کرو_INT ("version", strversion_منزل_int (نسخہ_2));
  MUSCLE_داخل_کرو_INT ("required_version", required_نسخہ);

  /* Flags. */
  MUSCLE_داخل_کرو_BOOL ("header_flag", سرخی_flag);
  MUSCLE_داخل_کرو_BOOL ("glr_flag", glr_parser);
  MUSCLE_داخل_کرو_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_داخل_کرو_BOOL ("synclines_flag", !no_سطریں_flag);
  MUSCLE_داخل_کرو_BOOL ("tag_seen_flag", ٹیگ_تکیا);
  MUSCLE_داخل_کرو_BOOL ("token_table_flag", token_جدول_flag);
  MUSCLE_داخل_کرو_BOOL ("use_push_for_pull_flag", use_ڈالو_for_pull_flag);
  MUSCLE_داخل_کرو_BOOL ("yacc_flag", !location_خالی_تھاواں (yacc_loc));

  /* File names.  */
  إذا (spec_name_اگیتر)
    MUSCLE_داخل_کرو_STRING ("prefix", spec_name_اگیتر);

  MUSCLE_داخل_کرو_STRING ("file_name_all_but_ext", all_but_ext);

  ثابت حرف *spec_mapped_سرخی_file = map_file_name (spec_سرخی_file);
  ثابت حرف *mapped_dir_اگیتر = map_file_name (dir_اگیتر);

#تعریف_کرو DEFINE(Name) MUSCLE_داخل_کرو_STRING (#Name, Name ? Name : "")
  DEFINE (dir_اگیتر);
  DEFINE (mapped_dir_اگیتر);
  DEFINE (parser_file_name);
  DEFINE (spec_سرخی_file);
  DEFINE (spec_mapped_سرخی_file);
  DEFINE (spec_file_اگیتر);
  DEFINE (spec_گراف_file);
  DEFINE (spec_name_اگیتر);
  DEFINE (spec_outfile);
  DEFINE (spec_verbose_file);
#تعریف_رد DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  إذا (skeleton)
    MUSCLE_داخل_کرو_C_STRING ("skeleton", skeleton);
  وإلا
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    حرف *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_داخل_کرو_STRING_RAW ("skeletonsdir", skeldir);
    حرر (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

فراغ
output (فراغ)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_قاعدے ();
  prepare_حالتاں ();
  prepare_actions ();
  prepare_علامت_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  إذا (complaint_حالت)
    unlink_generated_sources ();

  obstack_free (&format_obstack, عدم);
}
