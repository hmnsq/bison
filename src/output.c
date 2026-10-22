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


#define GENERATE_MUSCLE_قىستۇرۇش_TABLE(Name, Type)                        \
                                                                        \
ساكن فراغ                                                             \
Name (حرف ثابت *name, Type *table_data, Type first,                   \
      صحيح begin, صحيح end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  صحيح j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  لكل (صحيح i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      إذا (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      وإلا                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      إذا (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      إذا (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_قىستۇرۇش (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_قىستۇرۇش_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_قىستۇرۇش_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_قىستۇرۇش_TABLE (muscle_قىستۇرۇش_int_table, صحيح)
GENERATE_MUSCLE_قىستۇرۇش_TABLE (muscle_قىستۇرۇش_base_table, base_سان)
GENERATE_MUSCLE_قىستۇرۇش_TABLE (muscle_قىستۇرۇش_rule_سان_table, rule_سان)
GENERATE_MUSCLE_قىستۇرۇش_TABLE (muscle_قىستۇرۇش_بەلگە_سان_table, بەلگە_سان)
GENERATE_MUSCLE_قىستۇرۇش_TABLE (muscle_قىستۇرۇش_item_سان_table, item_سان)
GENERATE_MUSCLE_قىستۇرۇش_TABLE (muscle_قىستۇرۇش_state_سان_table, state_سان)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ساكن فراغ
output_escaped (FILE *out, ثابت حرف *cp)
{
  لكل (; *cp; ++cp)
    بدل (*cp)
      {
      حالة '$': fputs ("$][", out); اكسر;
      حالة '@': fputs ("@@",  out); اكسر;
      حالة '[': fputs ("@{",  out); اكسر;
      حالة ']': fputs ("@}",  out); اكسر;

      حالة 'b':
        إذا (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      حالة 'm':
        إذا (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            اكسر;
          }
        وإلا
          اذهب append;

      append:
      افتراضي:  fputc (*cp,   out); اكسر;
      }
}

ساكن فراغ
output_quoted (FILE *out, حرف ثابت *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ساكن فراغ
string_output (FILE *out, حرف ثابت *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ساكن ptrdiff_t
escape_trigraphs (حرف *buffer, ptrdiff_t buffersize, ثابت حرف *src)
{
#define STORE(c)                                \
  افعل                                            \
    {                                           \
      إذا (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  طالما (0)
  ptrdiff_t res = 0;
  لكل (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
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
#undef STORE
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
بەلگە_tag (ثابت بەلگە *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  إذا (!eof_is_user_defined && sym->content == eoftoken->content)
    أرجع "\"end of file\"";
  وإلا إذا (sym->content == undeftoken->content)
    أرجع "\"invalid token\"";
  وإلا
    أرجع sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ساكن فراغ
prepare_بەلگە_names (حرف ثابت *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ثابت bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  صحيح col = 2;
  بنية quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  لكل (صحيح i = 0; i < nsyms; i++)
    {
      ثابت حرف *tag = بەلگە_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      إذا (translatable)
        has_translations = true;

      حرف *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      صحيح width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      إذا (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
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
      col += width;
    }
  حرر (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_قىستۇرۇش (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_قىستۇرۇش_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ساكن فراغ
prepare_symbols (فراغ)
{
  MUSCLE_قىستۇرۇش_INT ("tokens_number", ntokens);
  MUSCLE_قىستۇرۇش_INT ("nterms_number", nnterms);
  MUSCLE_قىستۇرۇش_INT ("symbols_number", nsyms);
  MUSCLE_قىستۇرۇش_INT ("code_max", max_code);

  muscle_قىستۇرۇش_بەلگە_سان_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_بەلگە_names ("tname");
  prepare_بەلگە_names ("symbol_names");

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
        صحيح *قىممەتلەر = xnmalloc (nsyms, حجم *قىممەتلەر);
        لكل (صحيح i = 0; i < ntokens; ++i)
          قىممەتلەر[i] = symbols[i]->translatable;
        muscle_قىستۇرۇش_int_table ("translatable", قىممەتلەر,
                                 قىممەتلەر[0], 1, ntokens);
        حرر (قىممەتلەر);
      }
  }

  /* Output YYTOKNUM. */
  {
    صحيح *قىممەتلەر = xnmalloc (ntokens, حجم *قىممەتلەر);
    لكل (صحيح i = 0; i < ntokens; ++i)
      قىممەتلەر[i] = symbols[i]->content->code;
    muscle_قىستۇرۇش_int_table ("toknum", قىممەتلەر,
                             قىممەتلەر[0], 1, ntokens);
    حرر (قىممەتلەر);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ساكن فراغ
prepare_rules (فراغ)
{
  صحيح *prhs = xnmalloc (nrules, حجم *prhs);
  item_سان *rhs = xnmalloc (nritems, حجم *rhs);
  صحيح *rline = xnmalloc (nrules, حجم *rline);
  بەلگە_سان *r1 = xnmalloc (nrules, حجم *r1);
  صحيح *r2 = xnmalloc (nrules, حجم *r2);
  صحيح *dprec = xnmalloc (nrules, حجم *dprec);
  صحيح *merger = xnmalloc (nrules, حجم *merger);
  صحيح *immediate = xnmalloc (nrules, حجم *immediate);

  /* Index in RHS.  */
  صحيح i = 0;
  لكل (rule_سان r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      لكل (item_سان *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.قۇر;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->سان;
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

  muscle_قىستۇرۇش_item_سان_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_قىستۇرۇش_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_قىستۇرۇش_int_table ("rline", rline, 0, 0, nrules);
  muscle_قىستۇرۇش_بەلگە_سان_table ("r1", r1, 0, 0, nrules);
  muscle_قىستۇرۇش_int_table ("r2", r2, 0, 0, nrules);
  muscle_قىستۇرۇش_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_قىستۇرۇش_int_table ("merger", merger, 0, 0, nrules);
  muscle_قىستۇرۇش_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_قىستۇرۇش_INT ("rules_number", nrules);
  MUSCLE_قىستۇرۇش_INT ("max_left_semantic_context", max_سول_semantic_context);

  حرر (prhs);
  حرر (rhs);
  حرر (rline);
  حرر (r1);
  حرر (r2);
  حرر (dprec);
  حرر (merger);
  حرر (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ساكن فراغ
prepare_states (فراغ)
{
  بەلگە_سان *قىممەتلەر = xnmalloc (nstates, حجم *قىممەتلەر);
  لكل (state_سان i = 0; i < nstates; ++i)
    قىممەتلەر[i] = states[i]->accessing_بەلگە;
  muscle_قىستۇرۇش_بەلگە_سان_table ("stos", قىممەتلەر,
                                     0, 1, nstates);
  حرر (قىممەتلەر);

  MUSCLE_قىستۇرۇش_INT ("last", يۇقىرى);
  MUSCLE_قىستۇرۇش_INT ("final_state_number", final_state->سان);
  MUSCLE_قىستۇرۇش_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ساكن صحيح
بەلگە_type_name_cmp (ثابت بەلگە **lhs, ثابت بەلگە **rhs)
{
  صحيح res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  إذا (!res)
    res = (*lhs)->content->سان - (*rhs)->content->سان;
  أرجع res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ساكن بەلگە **
symbols_by_type_name (فراغ)
{
  تعريفنوع صحيح (*qcmp_type) (ثابت فراغ *, ثابت فراغ *);
  بەلگە **res = xmemdup (symbols, nsyms * حجم *res);
  qsort (res, nsyms, حجم *res, (qcmp_type) &بەلگە_type_name_cmp);
  أرجع res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ساكن فراغ
type_names_output (FILE *out)
{
  بەلگە **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  لكل (صحيح i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      صحيح i0 = i;
      fputs (i ? ",\n[" : "[", out);
      لكل (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->سان);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  حرر (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ساكن فراغ
start_symbols_output (FILE *out)
{
  إذا (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      لكل (بەلگە_list *list = start_symbols; list; list = list->next)
        {
          ثابت بەلگە *start = list->content.sym;
          ثابت بەلگە *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->سان, swtok->content->سان);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ساكن فراغ
بەلگە_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  لكل (صحيح i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ساكن فراغ
rule_output (ثابت rule *r, FILE *out)
{
  output_escaped (out, r->lhs->بەلگە->tag);
  fputc (':', out);
  إذا (0 <= *r->rhs)
    لكل (item_سان *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  وإلا
    fputs (" %empty", out);
}

ساكن فراغ
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  لكل (rule_سان r = 0; r < nrules; ++r)
    إذا (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        إذا (!no_قۇرلار_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.قۇر);
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

ساكن فراغ
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  صحيح n;
  merger_list* p;
  لكل (n = 1, p = merge_functions; p != عدم; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->سان);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ساكن فراغ
prepare_بەلگە_definitions (فراغ)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  لكل (بەلگە_سان i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ثابت حرف *ئاچقۇچ = obstack_finish0 (&format_obstack);
      MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, nterm_map ? nterm_map[i - ntokens] : i);
    }

  لكل (صحيح i = 0; i < nsyms; ++i)
    {
      بەلگە *sym = symbols[i];
      ثابت حرف *ئاچقۇچ;

#define SET_ئاچقۇچ(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      ئاچقۇچ = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      ئاچقۇچ = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ثابت حرف *id = بەلگە_id_get (sym);
      SET_ئاچقۇچ ("has_id");
      MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, !!id);

      /* Its identifier.  */
      SET_ئاچقۇچ ("id");
      MUSCLE_قىستۇرۇش_STRING (ئاچقۇچ, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_ئاچقۇچ ("tag");
      MUSCLE_قىستۇرۇش_STRING (ئاچقۇچ, بەلگە_tag (sym));

      SET_ئاچقۇچ ("code");
      MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, sym->content->code);

      SET_ئاچقۇچ ("is_token");
      MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, i < ntokens);

      SET_ئاچقۇچ ("number");
      MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, sym->content->سان);

      SET_ئاچقۇچ ("has_type");
      MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, !!sym->content->type_name);

      SET_ئاچقۇچ ("type");
      MUSCLE_قىستۇرۇش_STRING (ئاچقۇچ, sym->content->type_name
                            ? sym->content->type_name : "");

      لكل (صحيح j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          حرف ثابت *pname = code_props_type_string (j) + 1;
          code_props ثابت *p = بەلگە_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, !!p->code);

          إذا (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_قىستۇرۇش_C_STRING (ئاچقۇچ, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_قىستۇرۇش_INT (ئاچقۇچ, p->location.start.قۇر);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (ئاچقۇچ, p->location);

              SET_ئاچقۇچ (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_قىستۇرۇش (ئاچقۇچ, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_ئاچقۇچ
    }
}


ساكن فراغ
prepare_actions (فراغ)
{
  /* Figure out the actions for the specified state.  */
  muscle_قىستۇرۇش_rule_سان_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_قىستۇرۇش_state_سان_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_قىستۇرۇش_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_قىستۇرۇش_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_قىستۇرۇش_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_قىستۇرۇش_base_table ("table", table,
                            table[0], 1, يۇقىرى + 1);
  MUSCLE_قىستۇرۇش_INT ("table_ninf", table_ninf);

  muscle_قىستۇرۇش_base_table ("check", check,
                            check[0], 1, يۇقىرى + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_قىستۇرۇش_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, يۇقىرى + 1);
  muscle_قىستۇرۇش_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ساكن فراغ
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  بەلگە_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
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
  حرف *skel = (IS_PATH_WITH_DIR (skeleton)
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
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_ئىتتىرىش (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, عدم);
  timevar_چىقىرىش (tv_m4);
}

ساكن فراغ
prepare (فراغ)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  حرف ثابت *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_ئىتتىرىش_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_قىستۇرۇش_STRING ("version_string", VERSION);
  MUSCLE_قىستۇرۇش_INT ("version", strversion_to_int (VERSION));
  MUSCLE_قىستۇرۇش_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_قىستۇرۇش_BOOL ("header_flag", header_flag);
  MUSCLE_قىستۇرۇش_BOOL ("glr_flag", glr_parser);
  MUSCLE_قىستۇرۇش_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_قىستۇرۇش_BOOL ("synclines_flag", !no_قۇرلار_flag);
  MUSCLE_قىستۇرۇش_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_قىستۇرۇش_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_قىستۇرۇش_BOOL ("use_push_for_pull_flag", use_ئىتتىرىش_for_pull_flag);
  MUSCLE_قىستۇرۇش_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  إذا (spec_name_prefix)
    MUSCLE_قىستۇرۇش_STRING ("prefix", spec_name_prefix);

  MUSCLE_قىستۇرۇش_STRING ("file_name_all_but_ext", all_but_ext);

  ثابت حرف *spec_mapped_header_file = map_file_name (spec_header_file);
  ثابت حرف *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_قىستۇرۇش_STRING (#Name, Name ? Name : "")
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
  إذا (skeleton)
    MUSCLE_قىستۇرۇش_C_STRING ("skeleton", skeleton);
  وإلا
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    حرف *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_قىستۇرۇش_STRING_RAW ("skeletonsdir", skeldir);
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
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_بەلگە_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  إذا (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, عدم);
}
