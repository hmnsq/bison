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


#وصف_ڪريو GENERATE_MUSCLE_وجهو_جدول(Name, Type)                        \
                                                                        \
ساكن فراغ                                                             \
Name (حرف ثابت *name, Type *جدول_معلومات, Type پهريون,                   \
      صحيح begin, صحيح پڇاڙي)                                               \
{                                                                       \
  Type min = پهريون;                                                     \
  Type max = پهريون;                                                     \
  صحيح j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", پهريون);                       \
  لكل (صحيح i = begin; i < پڇاڙي; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      إذا (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      وإلا                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", جدول_معلومات[i]);           \
      إذا (جدول_معلومات[i] < min)                                          \
        min = جدول_معلومات[i];                                            \
      إذا (max < جدول_معلومات[i])                                          \
        max = جدول_معلومات[i];                                            \
    }                                                                   \
  muscle_وجهو (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  ڊگهو lmin = min;                                                      \
  ڊگهو lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_وجهو_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_وجهو_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_وجهو_جدول (muscle_وجهو_int_جدول, صحيح)
GENERATE_MUSCLE_وجهو_جدول (muscle_وجهو_بنياد_جدول, بنياد_عدد)
GENERATE_MUSCLE_وجهو_جدول (muscle_وجهو_قاعدو_عدد_جدول, قاعدو_عدد)
GENERATE_MUSCLE_وجهو_جدول (muscle_وجهو_نشان_عدد_جدول, نشان_عدد)
GENERATE_MUSCLE_وجهو_جدول (muscle_وجهو_شي_عدد_جدول, شي_عدد)
GENERATE_MUSCLE_وجهو_جدول (muscle_وجهو_state_عدد_جدول, state_عدد)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ساكن فراغ
output_escaped (FILE *ٻاهريون, ثابت حرف *cp)
{
  لكل (; *cp; ++cp)
    بدل (*cp)
      {
      حالة '$': fputs ("$][", ٻاهريون); اكسر;
      حالة '@': fputs ("@@",  ٻاهريون); اكسر;
      حالة '[': fputs ("@{",  ٻاهريون); اكسر;
      حالة ']': fputs ("@}",  ٻاهريون); اكسر;

      حالة 'b':
        إذا (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", ٻاهريون);
            cp += لڙي_ڊيگهه ("b4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      حالة 'm':
        إذا (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", ٻاهريون);
            cp += لڙي_ڊيگهه ("m4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      append:
      افتراضي:  fputc (*cp,   ٻاهريون); اكسر;
      }
}

ساكن فراغ
output_quoted (FILE *ٻاهريون, حرف ثابت *cp)
{
  fprintf (ٻاهريون, "[[");
  output_escaped (ٻاهريون, cp);
  fprintf (ٻاهريون, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ساكن فراغ
string_output (FILE *ٻاهريون, حرف ثابت *string)
{
  output_quoted (ٻاهريون, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ساكن ptrdiff_t
escape_trigraphs (حرف *بفر, ptrdiff_t buffersize, ثابت حرف *src)
{
#وصف_ڪريو STORE(c)                                \
  افعل                                            \
    {                                           \
      إذا (res < buffersize)                     \
        بفر[res] = (c);                      \
      ++res;                                    \
    }                                           \
  طالما (0)
  ptrdiff_t res = 0;
  لكل (ptrdiff_t i = 0, len = لڙي_ڊيگهه (src); i < len; ++i)
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
#وصف_رد STORE
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
نشان_ٽيگ (ثابت نشان *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->ٽيگ, "$end");

  إذا (!eof_is_user_defined && sym->content == eoftoken->content)
    أرجع "\"end of file\"";
  وإلا إذا (sym->content == undeftoken->content)
    أرجع "\"invalid token\"";
  وإلا
    أرجع sym->ٽيگ;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ساكن فراغ
prepare_نشان_نالا (حرف ثابت *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ثابت bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  صحيح کانو = 2;
  بنية quoting_options *qo = clone_quoting_options (0);
  سيٽ_quoting_style (qo, c_quoting_style);
  سيٽ_quoting_flags (qo, QA_ورهايو_TRIGRAPHS);
  لكل (صحيح i = 0; i < nsyms; i++)
    {
      ثابت حرف *ٽيگ = نشان_ٽيگ (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      إذا (translatable)
        has_translations = true;

      حرف *cp
        = ٽيگ[0] == '"' && !quote
        ? xescape_trigraphs (ٽيگ)
        : quotearg_alloc (ٽيگ, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      صحيح ويڪر
        = mbswidth (cp, 0) + 2
        + (translatable ? لڙي_ڊيگهه ("N_()") : 0);

      إذا (کانو + ويڪر > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          کانو = 1;
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
      کانو += ويڪر;
    }
  حرر (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_وجهو (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_وجهو_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ساكن فراغ
prepare_symbols (فراغ)
{
  MUSCLE_وجهو_INT ("tokens_number", ntokens);
  MUSCLE_وجهو_INT ("nterms_number", nnterms);
  MUSCLE_وجهو_INT ("symbols_number", nsyms);
  MUSCLE_وجهو_INT ("code_max", max_ڪوڊ);

  muscle_وجهو_نشان_عدد_جدول ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_ڪوڊ + 1);

  /* tname -- token names.  */
  prepare_نشان_نالا ("tname");
  prepare_نشان_نالا ("symbol_names");

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
        صحيح *قدرون = xnmalloc (nsyms, حجم *قدرون);
        لكل (صحيح i = 0; i < ntokens; ++i)
          قدرون[i] = symbols[i]->translatable;
        muscle_وجهو_int_جدول ("translatable", قدرون,
                                 قدرون[0], 1, ntokens);
        حرر (قدرون);
      }
  }

  /* Output YYTOKNUM. */
  {
    صحيح *قدرون = xnmalloc (ntokens, حجم *قدرون);
    لكل (صحيح i = 0; i < ntokens; ++i)
      قدرون[i] = symbols[i]->content->ڪوڊ;
    muscle_وجهو_int_جدول ("toknum", قدرون,
                             قدرون[0], 1, ntokens);
    حرر (قدرون);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ساكن فراغ
prepare_قاعدا (فراغ)
{
  صحيح *prhs = xnmalloc (nrules, حجم *prhs);
  شي_عدد *rhs = xnmalloc (nritems, حجم *rhs);
  صحيح *rline = xnmalloc (nrules, حجم *rline);
  نشان_عدد *r1 = xnmalloc (nrules, حجم *r1);
  صحيح *r2 = xnmalloc (nrules, حجم *r2);
  صحيح *dprec = xnmalloc (nrules, حجم *dprec);
  صحيح *merger = xnmalloc (nrules, حجم *merger);
  صحيح *فوري = xnmalloc (nrules, حجم *فوري);

  /* Index in RHS.  */
  صحيح i = 0;
  لكل (قاعدو_عدد r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      لكل (شي_عدد *rhsp = قاعدا[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = قاعدا[r].location.شروعات.سٽ;
      /* LHS of the rule R. */
      r1[r] = قاعدا[r].lhs->عدد;
      /* Length of rule R's RHS. */
      r2[r] = قاعدو_rhs_ڊيگهائي (&قاعدا[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = قاعدا[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = قاعدا[r].merger;
      /* Immediate reduction flags (GLR).  */
      فوري[r] = قاعدا[r].is_predicate;
    }
  aver (i == nritems);

  muscle_وجهو_شي_عدد_جدول ("rhs", rhs, ritem[0], 1, nritems);
  muscle_وجهو_int_جدول ("prhs", prhs, 0, 0, nrules);
  muscle_وجهو_int_جدول ("rline", rline, 0, 0, nrules);
  muscle_وجهو_نشان_عدد_جدول ("r1", r1, 0, 0, nrules);
  muscle_وجهو_int_جدول ("r2", r2, 0, 0, nrules);
  muscle_وجهو_int_جدول ("dprec", dprec, 0, 0, nrules);
  muscle_وجهو_int_جدول ("merger", merger, 0, 0, nrules);
  muscle_وجهو_int_جدول ("immediate", فوري, 0, 0, nrules);

  MUSCLE_وجهو_INT ("rules_number", nrules);
  MUSCLE_وجهو_INT ("max_left_semantic_context", max_کاٻو_semantic_context);

  حرر (prhs);
  حرر (rhs);
  حرر (rline);
  حرر (r1);
  حرر (r2);
  حرر (dprec);
  حرر (merger);
  حرر (فوري);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ساكن فراغ
prepare_حالتون (فراغ)
{
  نشان_عدد *قدرون = xnmalloc (nstates, حجم *قدرون);
  لكل (state_عدد i = 0; i < nstates; ++i)
    قدرون[i] = حالتون[i]->accessing_نشان;
  muscle_وجهو_نشان_عدد_جدول ("stos", قدرون,
                                     0, 1, nstates);
  حرر (قدرون);

  MUSCLE_وجهو_INT ("last", مٿاهون);
  MUSCLE_وجهو_INT ("final_state_number", final_state->عدد);
  MUSCLE_وجهو_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ساكن صحيح
نشان_type_name_cmp (ثابت نشان **lhs, ثابت نشان **rhs)
{
  صحيح res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  إذا (!res)
    res = (*lhs)->content->عدد - (*rhs)->content->عدد;
  أرجع res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ساكن نشان **
symbols_by_type_name (فراغ)
{
  تعريفنوع صحيح (*qcmp_type) (ثابت فراغ *, ثابت فراغ *);
  نشان **res = xmemdup (symbols, nsyms * حجم *res);
  qsort (res, nsyms, حجم *res, (qcmp_type) &نشان_type_name_cmp);
  أرجع res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ساكن فراغ
type_نالا_output (FILE *ٻاهريون)
{
  نشان **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", ٻاهريون);
  لكل (صحيح i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      صحيح i0 = i;
      fputs (i ? ",\n[" : "[", ٻاهريون);
      لكل (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (ٻاهريون, "%s%d", i != i0 ? ", " : "", syms[i]->content->عدد);
      fputs ("]", ٻاهريون);
    }
  fputs ("])\n\n", ٻاهريون);
  حرر (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ساكن فراغ
شروعات_symbols_output (FILE *ٻاهريون)
{
  إذا (شروعات_symbols && شروعات_symbols->ايندڙ)
    {
      fputs ("m4_define([b4_start_symbols],\n[", ٻاهريون);
      لكل (نشان_list *list = شروعات_symbols; list; list = list->ايندڙ)
        {
          ثابت نشان *شروعات = list->content.sym;
          ثابت نشان *swtok = switching_token (شروعات);
          fprintf (ٻاهريون, "%s[%d, %d]",
                   list == شروعات_symbols ? "" : ", ",
                   شروعات->content->عدد, swtok->content->عدد);
        }
      fputs ("])\n\n", ٻاهريون);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ساكن فراغ
نشان_numbers_output (FILE *ٻاهريون)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", ٻاهريون);
  لكل (صحيح i = 0; i < nsyms; ++i)
    fprintf (ٻاهريون, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", ٻاهريون);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ساكن فراغ
قاعدو_output (ثابت قاعدو *r, FILE *ٻاهريون)
{
  output_escaped (ٻاهريون, r->lhs->نشان->ٽيگ);
  fputc (':', ٻاهريون);
  إذا (0 <= *r->rhs)
    لكل (شي_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', ٻاهريون);
        output_escaped (ٻاهريون, symbols[*rhsp]->ٽيگ);
      }
  وإلا
    fputs (" %empty", ٻاهريون);
}

ساكن فراغ
user_actions_output (FILE *ٻاهريون)
{
  fputs ("m4_define([b4_actions], \n[", ٻاهريون);
  لكل (قاعدو_عدد r = 0; r < nrules; ++r)
    إذا (قاعدا[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (ٻاهريون, "%s""(%d, [",
                 قاعدا[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        إذا (!no_سٽون_flag)
          {
            fprintf (ٻاهريون, "b4_syncline(%d, ",
                     قاعدا[r].action_loc.شروعات.سٽ);
            string_output (ٻاهريون, map_file_name (قاعدا[r].action_loc.شروعات.file));
            fprintf (ٻاهريون, ")dnl\n");
          }
        fprintf (ٻاهريون, "[%*s%s]],\n[[",
                 قاعدا[r].action_loc.شروعات.ڪالم - 1, "",
                 قاعدا[r].action);
        قاعدو_output (&قاعدا[r], ٻاهريون);
        fprintf (ٻاهريون, "]])\n\n");
      }
  fputs ("])\n\n", ٻاهريون);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

ساكن فراغ
merger_output (FILE *ٻاهريون)
{
  fputs ("m4_define([b4_mergers], \n[[", ٻاهريون);
  صحيح n;
  merger_list* p;
  لكل (n = 1, p = ملايو_functions; p != عدم; n += 1, p = p->ايندڙ)
    fprintf (ٻاهريون, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->عدد);
  fputs ("]])\n\n", ٻاهريون);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ساكن فراغ
prepare_نشان_definitions (فراغ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  لكل (نشان_عدد i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ثابت حرف *ڪنجي = obstack_finish0 (&format_obstack);
      MUSCLE_وجهو_INT (ڪنجي, nterm_map ? nterm_map[i - ntokens] : i);
    }

  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      نشان *sym = symbols[i];
      ثابت حرف *ڪنجي;

#وصف_ڪريو سيٽ_ڪنجي(داخلا_3)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, داخلا_3);                                \
      ڪنجي = obstack_finish0 (&format_obstack);

#وصف_ڪريو سيٽ_KEY2(داخلا_3, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, داخلا_3, Suffix);                        \
      ڪنجي = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ثابت حرف *id = نشان_id_get (sym);
      سيٽ_ڪنجي ("has_id");
      MUSCLE_وجهو_INT (ڪنجي, !!id);

      /* Its identifier.  */
      سيٽ_ڪنجي ("id");
      MUSCLE_وجهو_STRING (ڪنجي, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      سيٽ_ڪنجي ("tag");
      MUSCLE_وجهو_STRING (ڪنجي, نشان_ٽيگ (sym));

      سيٽ_ڪنجي ("code");
      MUSCLE_وجهو_INT (ڪنجي, sym->content->ڪوڊ);

      سيٽ_ڪنجي ("is_token");
      MUSCLE_وجهو_INT (ڪنجي, i < ntokens);

      سيٽ_ڪنجي ("number");
      MUSCLE_وجهو_INT (ڪنجي, sym->content->عدد);

      سيٽ_ڪنجي ("has_type");
      MUSCLE_وجهو_INT (ڪنجي, !!sym->content->type_name);

      سيٽ_ڪنجي ("type");
      MUSCLE_وجهو_STRING (ڪنجي, sym->content->type_name
                            ? sym->content->type_name : "");

      لكل (صحيح j = 0; j < ڪوڊ_PROPS_سائيز; ++j)
        {
          /* "printer", not "%printer".  */
          حرف ثابت *pname = ڪوڊ_props_type_string (j) + 1;
          ڪوڊ_props ثابت *p = نشان_ڪوڊ_props_get (sym, j);
          سيٽ_KEY2 ("has", pname);
          MUSCLE_وجهو_INT (ڪنجي, !!p->ڪوڊ);

          إذا (p->ڪوڊ)
            {
              سيٽ_KEY2 (pname, "file");
              MUSCLE_وجهو_C_STRING (ڪنجي, map_file_name (p->location.شروعات.file));

              سيٽ_KEY2 (pname, "line");
              MUSCLE_وجهو_INT (ڪنجي, p->location.شروعات.سٽ);

              سيٽ_KEY2 (pname, "loc");
              muscle_location_grow (ڪنجي, p->location);

              سيٽ_ڪنجي (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.شروعات.ڪالم - 1, "", p->ڪوڊ);
              muscle_وجهو (ڪنجي, obstack_finish0 (&muscle_obstack));
            }
        }
#وصف_رد سيٽ_KEY2
#وصف_رد سيٽ_ڪنجي
    }
}


ساكن فراغ
prepare_actions (فراغ)
{
  /* Figure out the actions for the specified state.  */
  muscle_وجهو_قاعدو_عدد_جدول ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_وجهو_state_عدد_جدول ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_وجهو_بنياد_جدول ("pact", بنياد,
                             بنياد[0], 1, nstates);
  MUSCLE_وجهو_INT ("pact_ninf", بنياد_ninf);

  /* Output PGOTO. */
  muscle_وجهو_بنياد_جدول ("pgoto", بنياد,
                             بنياد[nstates], nstates + 1, nvectors);

  muscle_وجهو_بنياد_جدول ("table", جدول,
                            جدول[0], 1, مٿاهون + 1);
  MUSCLE_وجهو_INT ("table_ninf", جدول_ninf);

  muscle_وجهو_بنياد_جدول ("check", check,
                            check[0], 1, مٿاهون + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_وجهو_int_جدول ("conflict_list_heads", conflict_جدول,
                           conflict_جدول[0], 1, مٿاهون + 1);
  muscle_وجهو_int_جدول ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ساكن فراغ
muscles_output (FILE *ٻاهريون)
{
  fputs ("m4_init()\n", ٻاهريون);
  merger_output (ٻاهريون);
  نشان_numbers_output (ٻاهريون);
  type_نالا_output (ٻاهريون);
  شروعات_symbols_output (ٻاهريون);
  user_actions_output (ٻاهريون);
  /* Must be last.  */
  muscles_m4_output (ٻاهريون);
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
  حرف *skel = (IS_رستو_WITH_DIR (skeleton)
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
    FILE *ٻاهريون = xfdopen (filter_fd[1], "w");
    muscles_output (ٻاهريون);
    xfclose (ٻاهريون);
  }

  /* Read and process m4's output.  */
  timevar_ڌڪو (tv_m4);
  {
    FILE *اندريون = xfdopen (filter_fd[0], "r");
    جاچ_skel (اندريون);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (اندريون));
    xfclose (اندريون);
  }
  wait_subprocess (pid, "m4", false, false, true, true, عدم);
  timevar_ڪڍو (tv_m4);
}

ساكن فراغ
prepare (فراغ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  حرف ثابت *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ڌڪو_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_وجهو_STRING ("version_string", نسخو_2);
  MUSCLE_وجهو_INT ("version", strversion_منزل_int (نسخو_2));
  MUSCLE_وجهو_INT ("required_version", required_نسخو);

  /* Flags. */
  MUSCLE_وجهو_BOOL ("header_flag", سرو_flag);
  MUSCLE_وجهو_BOOL ("glr_flag", glr_parser);
  MUSCLE_وجهو_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_وجهو_BOOL ("synclines_flag", !no_سٽون_flag);
  MUSCLE_وجهو_BOOL ("tag_seen_flag", ٽيگ_ڏٺل);
  MUSCLE_وجهو_BOOL ("token_table_flag", token_جدول_flag);
  MUSCLE_وجهو_BOOL ("use_push_for_pull_flag", use_ڌڪو_for_pull_flag);
  MUSCLE_وجهو_BOOL ("yacc_flag", !location_خالي_جايون (yacc_loc));

  /* File names.  */
  إذا (spec_name_اڳياڙي)
    MUSCLE_وجهو_STRING ("prefix", spec_name_اڳياڙي);

  MUSCLE_وجهو_STRING ("file_name_all_but_ext", all_but_ext);

  ثابت حرف *spec_mapped_سرو_file = map_file_name (spec_سرو_file);
  ثابت حرف *mapped_dir_اڳياڙي = map_file_name (dir_اڳياڙي);

#وصف_ڪريو DEFINE(Name) MUSCLE_وجهو_STRING (#Name, Name ? Name : "")
  DEFINE (dir_اڳياڙي);
  DEFINE (mapped_dir_اڳياڙي);
  DEFINE (parser_file_name);
  DEFINE (spec_سرو_file);
  DEFINE (spec_mapped_سرو_file);
  DEFINE (spec_file_اڳياڙي);
  DEFINE (spec_گراف_file);
  DEFINE (spec_name_اڳياڙي);
  DEFINE (spec_outfile);
  DEFINE (spec_verbose_file);
#وصف_رد DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  إذا (skeleton)
    MUSCLE_وجهو_C_STRING ("skeleton", skeleton);
  وإلا
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    حرف *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_وجهو_STRING_RAW ("skeletonsdir", skeldir);
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
  prepare_قاعدا ();
  prepare_حالتون ();
  prepare_actions ();
  prepare_نشان_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  إذا (complaint_حالت)
    unlink_generated_sources ();

  obstack_free (&format_obstack, عدم);
}
