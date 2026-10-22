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


#تعریف_کړه GENERATE_MUSCLE_ورننباسه_جدول(Name, Type)                        \
                                                                        \
ساكن فراغ                                                             \
Name (حرف ثابت *name, Type *جدول_معلومات, Type لومړی,                   \
      صحيح begin, صحيح پای_ټکی)                                               \
{                                                                       \
  Type min = لومړی;                                                     \
  Type max = لومړی;                                                     \
  صحيح j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", لومړی);                       \
  لكل (صحيح i = begin; i < پای_ټکی; ++i)                                     \
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
  muscle_ورننباسه (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  اوږد lmin = min;                                                      \
  اوږد lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_ورننباسه_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_ورننباسه_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_ورننباسه_جدول (muscle_ورننباسه_int_جدول, صحيح)
GENERATE_MUSCLE_ورننباسه_جدول (muscle_ورننباسه_بنسټ_جدول, بنسټ_شمېره)
GENERATE_MUSCLE_ورننباسه_جدول (muscle_ورننباسه_قاعده_شمېره_جدول, قاعده_شمېره)
GENERATE_MUSCLE_ورننباسه_جدول (muscle_ورننباسه_نښه_شمېره_جدول, نښه_شمېره)
GENERATE_MUSCLE_ورننباسه_جدول (muscle_ورننباسه_توکی_شمېره_جدول, توکی_شمېره)
GENERATE_MUSCLE_ورننباسه_جدول (muscle_ورننباسه_state_شمېره_جدول, state_شمېره)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ساكن فراغ
output_escaped (FILE *بهر, ثابت حرف *cp)
{
  لكل (; *cp; ++cp)
    بدل (*cp)
      {
      حالة '$': fputs ("$][", بهر); اكسر;
      حالة '@': fputs ("@@",  بهر); اكسر;
      حالة '[': fputs ("@{",  بهر); اكسر;
      حالة ']': fputs ("@}",  بهر); اكسر;

      حالة 'b':
        إذا (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", بهر);
            cp += مزي_اوږدوالی ("b4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      حالة 'm':
        إذا (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", بهر);
            cp += مزي_اوږدوالی ("m4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      append:
      افتراضي:  fputc (*cp,   بهر); اكسر;
      }
}

ساكن فراغ
output_quoted (FILE *بهر, حرف ثابت *cp)
{
  fprintf (بهر, "[[");
  output_escaped (بهر, cp);
  fprintf (بهر, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ساكن فراغ
string_output (FILE *بهر, حرف ثابت *string)
{
  output_quoted (بهر, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ساكن ptrdiff_t
escape_trigraphs (حرف *بفر, ptrdiff_t buffersize, ثابت حرف *src)
{
#تعریف_کړه STORE(c)                                \
  افعل                                            \
    {                                           \
      إذا (res < buffersize)                     \
        بفر[res] = (c);                      \
      ++res;                                    \
    }                                           \
  طالما (0)
  ptrdiff_t res = 0;
  لكل (ptrdiff_t i = 0, len = مزي_اوږدوالی (src); i < len; ++i)
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
#تعریف_لغوه STORE
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
نښه_نښان (ثابت نښه *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->نښان, "$end");

  إذا (!eof_is_user_defined && sym->content == eoftoken->content)
    أرجع "\"end of file\"";
  وإلا إذا (sym->content == undeftoken->content)
    أرجع "\"invalid token\"";
  وإلا
    أرجع sym->نښان;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ساكن فراغ
prepare_نښه_نومونه (حرف ثابت *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ثابت bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  صحيح ستون = 2;
  بنية quoting_options *qo = clone_quoting_options (0);
  سېټ_quoting_style (qo, c_quoting_style);
  سېټ_quoting_flags (qo, QA_وېشل_TRIGRAPHS);
  لكل (صحيح i = 0; i < nsyms; i++)
    {
      ثابت حرف *نښان = نښه_نښان (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      إذا (translatable)
        has_translations = true;

      حرف *cp
        = نښان[0] == '"' && !quote
        ? xescape_trigraphs (نښان)
        : quotearg_alloc (نښان, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      صحيح پلنوالی
        = mbswidth (cp, 0) + 2
        + (translatable ? مزي_اوږدوالی ("N_()") : 0);

      إذا (ستون + پلنوالی > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          ستون = 1;
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
      ستون += پلنوالی;
    }
  حرر (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_ورننباسه (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_ورننباسه_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ساكن فراغ
prepare_symbols (فراغ)
{
  MUSCLE_ورننباسه_INT ("tokens_number", ntokens);
  MUSCLE_ورننباسه_INT ("nterms_number", nnterms);
  MUSCLE_ورننباسه_INT ("symbols_number", nsyms);
  MUSCLE_ورننباسه_INT ("code_max", max_کوډ);

  muscle_ورننباسه_نښه_شمېره_جدول ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_کوډ + 1);

  /* tname -- token names.  */
  prepare_نښه_نومونه ("tname");
  prepare_نښه_نومونه ("symbol_names");

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
        صحيح *ارزښتونه = xnmalloc (nsyms, حجم *ارزښتونه);
        لكل (صحيح i = 0; i < ntokens; ++i)
          ارزښتونه[i] = symbols[i]->translatable;
        muscle_ورننباسه_int_جدول ("translatable", ارزښتونه,
                                 ارزښتونه[0], 1, ntokens);
        حرر (ارزښتونه);
      }
  }

  /* Output YYTOKNUM. */
  {
    صحيح *ارزښتونه = xnmalloc (ntokens, حجم *ارزښتونه);
    لكل (صحيح i = 0; i < ntokens; ++i)
      ارزښتونه[i] = symbols[i]->content->کوډ;
    muscle_ورننباسه_int_جدول ("toknum", ارزښتونه,
                             ارزښتونه[0], 1, ntokens);
    حرر (ارزښتونه);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ساكن فراغ
prepare_قواعد (فراغ)
{
  صحيح *prhs = xnmalloc (nrules, حجم *prhs);
  توکی_شمېره *rhs = xnmalloc (nritems, حجم *rhs);
  صحيح *rline = xnmalloc (nrules, حجم *rline);
  نښه_شمېره *r1 = xnmalloc (nrules, حجم *r1);
  صحيح *r2 = xnmalloc (nrules, حجم *r2);
  صحيح *dprec = xnmalloc (nrules, حجم *dprec);
  صحيح *merger = xnmalloc (nrules, حجم *merger);
  صحيح *سمدستي = xnmalloc (nrules, حجم *سمدستي);

  /* Index in RHS.  */
  صحيح i = 0;
  لكل (قاعده_شمېره r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      لكل (توکی_شمېره *rhsp = قواعد[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = قواعد[r].location.پیل.کرښه;
      /* LHS of the rule R. */
      r1[r] = قواعد[r].lhs->شمېره;
      /* Length of rule R's RHS. */
      r2[r] = قاعده_rhs_اوږدتیا (&قواعد[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = قواعد[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = قواعد[r].merger;
      /* Immediate reduction flags (GLR).  */
      سمدستي[r] = قواعد[r].is_predicate;
    }
  aver (i == nritems);

  muscle_ورننباسه_توکی_شمېره_جدول ("rhs", rhs, ritem[0], 1, nritems);
  muscle_ورننباسه_int_جدول ("prhs", prhs, 0, 0, nrules);
  muscle_ورننباسه_int_جدول ("rline", rline, 0, 0, nrules);
  muscle_ورننباسه_نښه_شمېره_جدول ("r1", r1, 0, 0, nrules);
  muscle_ورننباسه_int_جدول ("r2", r2, 0, 0, nrules);
  muscle_ورننباسه_int_جدول ("dprec", dprec, 0, 0, nrules);
  muscle_ورننباسه_int_جدول ("merger", merger, 0, 0, nrules);
  muscle_ورننباسه_int_جدول ("immediate", سمدستي, 0, 0, nrules);

  MUSCLE_ورننباسه_INT ("rules_number", nrules);
  MUSCLE_ورننباسه_INT ("max_left_semantic_context", max_کیڼ_semantic_context);

  حرر (prhs);
  حرر (rhs);
  حرر (rline);
  حرر (r1);
  حرر (r2);
  حرر (dprec);
  حرر (merger);
  حرر (سمدستي);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ساكن فراغ
prepare_حالتونه (فراغ)
{
  نښه_شمېره *ارزښتونه = xnmalloc (nstates, حجم *ارزښتونه);
  لكل (state_شمېره i = 0; i < nstates; ++i)
    ارزښتونه[i] = حالتونه[i]->accessing_نښه;
  muscle_ورننباسه_نښه_شمېره_جدول ("stos", ارزښتونه,
                                     0, 1, nstates);
  حرر (ارزښتونه);

  MUSCLE_ورننباسه_INT ("last", لوړ);
  MUSCLE_ورننباسه_INT ("final_state_number", final_state->شمېره);
  MUSCLE_ورننباسه_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ساكن صحيح
نښه_type_name_cmp (ثابت نښه **lhs, ثابت نښه **rhs)
{
  صحيح res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  إذا (!res)
    res = (*lhs)->content->شمېره - (*rhs)->content->شمېره;
  أرجع res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ساكن نښه **
symbols_by_type_name (فراغ)
{
  تعريفنوع صحيح (*qcmp_type) (ثابت فراغ *, ثابت فراغ *);
  نښه **res = xmemdup (symbols, nsyms * حجم *res);
  qsort (res, nsyms, حجم *res, (qcmp_type) &نښه_type_name_cmp);
  أرجع res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ساكن فراغ
type_نومونه_output (FILE *بهر)
{
  نښه **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", بهر);
  لكل (صحيح i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      صحيح i0 = i;
      fputs (i ? ",\n[" : "[", بهر);
      لكل (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (بهر, "%s%d", i != i0 ? ", " : "", syms[i]->content->شمېره);
      fputs ("]", بهر);
    }
  fputs ("])\n\n", بهر);
  حرر (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ساكن فراغ
پیل_symbols_output (FILE *بهر)
{
  إذا (پیل_symbols && پیل_symbols->بل)
    {
      fputs ("m4_define([b4_start_symbols],\n[", بهر);
      لكل (نښه_list *list = پیل_symbols; list; list = list->بل)
        {
          ثابت نښه *پیل = list->content.sym;
          ثابت نښه *swtok = switching_token (پیل);
          fprintf (بهر, "%s[%d, %d]",
                   list == پیل_symbols ? "" : ", ",
                   پیل->content->شمېره, swtok->content->شمېره);
        }
      fputs ("])\n\n", بهر);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ساكن فراغ
نښه_numbers_output (FILE *بهر)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", بهر);
  لكل (صحيح i = 0; i < nsyms; ++i)
    fprintf (بهر, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", بهر);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ساكن فراغ
قاعده_output (ثابت قاعده *r, FILE *بهر)
{
  output_escaped (بهر, r->lhs->نښه->نښان);
  fputc (':', بهر);
  إذا (0 <= *r->rhs)
    لكل (توکی_شمېره *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', بهر);
        output_escaped (بهر, symbols[*rhsp]->نښان);
      }
  وإلا
    fputs (" %empty", بهر);
}

ساكن فراغ
user_actions_output (FILE *بهر)
{
  fputs ("m4_define([b4_actions], \n[", بهر);
  لكل (قاعده_شمېره r = 0; r < nrules; ++r)
    إذا (قواعد[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (بهر, "%s""(%d, [",
                 قواعد[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        إذا (!no_کرښې_flag)
          {
            fprintf (بهر, "b4_syncline(%d, ",
                     قواعد[r].action_loc.پیل.کرښه);
            string_output (بهر, map_file_name (قواعد[r].action_loc.پیل.file));
            fprintf (بهر, ")dnl\n");
          }
        fprintf (بهر, "[%*s%s]],\n[[",
                 قواعد[r].action_loc.پیل.ستنه - 1, "",
                 قواعد[r].action);
        قاعده_output (&قواعد[r], بهر);
        fprintf (بهر, "]])\n\n");
      }
  fputs ("])\n\n", بهر);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

ساكن فراغ
merger_output (FILE *بهر)
{
  fputs ("m4_define([b4_mergers], \n[[", بهر);
  صحيح n;
  merger_list* p;
  لكل (n = 1, p = یوځای_functions; p != عدم; n += 1, p = p->بل)
    fprintf (بهر, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->شمېره);
  fputs ("]])\n\n", بهر);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ساكن فراغ
prepare_نښه_definitions (فراغ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  لكل (نښه_شمېره i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ثابت حرف *کلی = obstack_finish0 (&format_obstack);
      MUSCLE_ورننباسه_INT (کلی, nterm_map ? nterm_map[i - ntokens] : i);
    }

  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      نښه *sym = symbols[i];
      ثابت حرف *کلی;

#تعریف_کړه سېټ_کلی(ثبت_2)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, ثبت_2);                                \
      کلی = obstack_finish0 (&format_obstack);

#تعریف_کړه سېټ_KEY2(ثبت_2, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, ثبت_2, Suffix);                        \
      کلی = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ثابت حرف *id = نښه_id_get (sym);
      سېټ_کلی ("has_id");
      MUSCLE_ورننباسه_INT (کلی, !!id);

      /* Its identifier.  */
      سېټ_کلی ("id");
      MUSCLE_ورننباسه_STRING (کلی, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      سېټ_کلی ("tag");
      MUSCLE_ورننباسه_STRING (کلی, نښه_نښان (sym));

      سېټ_کلی ("code");
      MUSCLE_ورننباسه_INT (کلی, sym->content->کوډ);

      سېټ_کلی ("is_token");
      MUSCLE_ورننباسه_INT (کلی, i < ntokens);

      سېټ_کلی ("number");
      MUSCLE_ورننباسه_INT (کلی, sym->content->شمېره);

      سېټ_کلی ("has_type");
      MUSCLE_ورننباسه_INT (کلی, !!sym->content->type_name);

      سېټ_کلی ("type");
      MUSCLE_ورننباسه_STRING (کلی, sym->content->type_name
                            ? sym->content->type_name : "");

      لكل (صحيح j = 0; j < کوډ_PROPS_اندازه; ++j)
        {
          /* "printer", not "%printer".  */
          حرف ثابت *pname = کوډ_props_type_string (j) + 1;
          کوډ_props ثابت *p = نښه_کوډ_props_get (sym, j);
          سېټ_KEY2 ("has", pname);
          MUSCLE_ورننباسه_INT (کلی, !!p->کوډ);

          إذا (p->کوډ)
            {
              سېټ_KEY2 (pname, "file");
              MUSCLE_ورننباسه_C_STRING (کلی, map_file_name (p->location.پیل.file));

              سېټ_KEY2 (pname, "line");
              MUSCLE_ورننباسه_INT (کلی, p->location.پیل.کرښه);

              سېټ_KEY2 (pname, "loc");
              muscle_location_grow (کلی, p->location);

              سېټ_کلی (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.پیل.ستنه - 1, "", p->کوډ);
              muscle_ورننباسه (کلی, obstack_finish0 (&muscle_obstack));
            }
        }
#تعریف_لغوه سېټ_KEY2
#تعریف_لغوه سېټ_کلی
    }
}


ساكن فراغ
prepare_actions (فراغ)
{
  /* Figure out the actions for the specified state.  */
  muscle_ورننباسه_قاعده_شمېره_جدول ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_ورننباسه_state_شمېره_جدول ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_ورننباسه_بنسټ_جدول ("pact", بنسټ,
                             بنسټ[0], 1, nstates);
  MUSCLE_ورننباسه_INT ("pact_ninf", بنسټ_ninf);

  /* Output PGOTO. */
  muscle_ورننباسه_بنسټ_جدول ("pgoto", بنسټ,
                             بنسټ[nstates], nstates + 1, nvectors);

  muscle_ورننباسه_بنسټ_جدول ("table", جدول,
                            جدول[0], 1, لوړ + 1);
  MUSCLE_ورننباسه_INT ("table_ninf", جدول_ninf);

  muscle_ورننباسه_بنسټ_جدول ("check", check,
                            check[0], 1, لوړ + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_ورننباسه_int_جدول ("conflict_list_heads", conflict_جدول,
                           conflict_جدول[0], 1, لوړ + 1);
  muscle_ورننباسه_int_جدول ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ساكن فراغ
muscles_output (FILE *بهر)
{
  fputs ("m4_init()\n", بهر);
  merger_output (بهر);
  نښه_numbers_output (بهر);
  type_نومونه_output (بهر);
  پیل_symbols_output (بهر);
  user_actions_output (بهر);
  /* Must be last.  */
  muscles_m4_output (بهر);
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
  حرف *skel = (IS_لار_WITH_DIR (skeleton)
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
    FILE *بهر = xfdopen (filter_fd[1], "w");
    muscles_output (بهر);
    xfclose (بهر);
  }

  /* Read and process m4's output.  */
  timevar_ټېل (tv_m4);
  {
    FILE *دننه = xfdopen (filter_fd[0], "r");
    لټون_skel (دننه);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (دننه));
    xfclose (دننه);
  }
  wait_subprocess (pid, "m4", false, false, true, true, عدم);
  timevar_راوباسه (tv_m4);
}

ساكن فراغ
prepare (فراغ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  حرف ثابت *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ټېل_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_ورننباسه_STRING ("version_string", نسخه_2);
  MUSCLE_ورننباسه_INT ("version", strversion_منزل_int (نسخه_2));
  MUSCLE_ورننباسه_INT ("required_version", required_نسخه);

  /* Flags. */
  MUSCLE_ورننباسه_BOOL ("header_flag", سرلیک_flag);
  MUSCLE_ورننباسه_BOOL ("glr_flag", glr_parser);
  MUSCLE_ورننباسه_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_ورننباسه_BOOL ("synclines_flag", !no_کرښې_flag);
  MUSCLE_ورننباسه_BOOL ("tag_seen_flag", نښان_کتل_شوی);
  MUSCLE_ورننباسه_BOOL ("token_table_flag", token_جدول_flag);
  MUSCLE_ورننباسه_BOOL ("use_push_for_pull_flag", use_ټېل_for_pull_flag);
  MUSCLE_ورننباسه_BOOL ("yacc_flag", !location_تشه (yacc_loc));

  /* File names.  */
  إذا (spec_name_مختاړی)
    MUSCLE_ورننباسه_STRING ("prefix", spec_name_مختاړی);

  MUSCLE_ورننباسه_STRING ("file_name_all_but_ext", all_but_ext);

  ثابت حرف *spec_mapped_سرلیک_file = map_file_name (spec_سرلیک_file);
  ثابت حرف *mapped_dir_مختاړی = map_file_name (dir_مختاړی);

#تعریف_کړه DEFINE(Name) MUSCLE_ورننباسه_STRING (#Name, Name ? Name : "")
  DEFINE (dir_مختاړی);
  DEFINE (mapped_dir_مختاړی);
  DEFINE (parser_file_name);
  DEFINE (spec_سرلیک_file);
  DEFINE (spec_mapped_سرلیک_file);
  DEFINE (spec_file_مختاړی);
  DEFINE (spec_ګراف_file);
  DEFINE (spec_name_مختاړی);
  DEFINE (spec_outfile);
  DEFINE (spec_verbose_file);
#تعریف_لغوه DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  إذا (skeleton)
    MUSCLE_ورننباسه_C_STRING ("skeleton", skeleton);
  وإلا
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    حرف *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_ورننباسه_STRING_RAW ("skeletonsdir", skeldir);
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
  prepare_قواعد ();
  prepare_حالتونه ();
  prepare_actions ();
  prepare_نښه_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  إذا (complaint_حالت)
    unlink_generated_sources ();

  obstack_free (&format_obstack, عدم);
}
