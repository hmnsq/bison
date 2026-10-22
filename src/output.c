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

#شامل <config.h>
#شامل "system.h"

#شامل <filename.h> /* IS_PATH_WITH_DIR */
#شامل <get-errno.h>
#شامل <mbswidth.h>
#شامل <path-join.h>
#شامل <quotearg.h>
#شامل <spawn-pipe.h>
#شامل <timevar.h>
#شامل <wait-process.h>

#شامل "complain.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "muscle-tab.h"
#شامل "output.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "scan-code.h"    /* max_left_semantic_context */
#شامل "scan-skel.h"
#شامل "symtab.h"
#شامل "tables.h"
#شامل "strversion.h"

ایستا ساختار obstack format_obstack;


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


#define GENERATE_MUSCLE_درج_TABLE(Name, Type)                        \
                                                                        \
ایستا تهی                                                             \
Name (نویسه ثابت *name, Type *table_data, Type first,                   \
      صحیح begin, صحیح end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  صحیح j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  برای (صحیح i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      اگر (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      وگرنه                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      اگر (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      اگر (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_درج (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  long lmin = min;                                                      \
  long lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_درج_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_درج_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_درج_TABLE (muscle_درج_int_table, صحیح)
GENERATE_MUSCLE_درج_TABLE (muscle_درج_base_table, base_عدد)
GENERATE_MUSCLE_درج_TABLE (muscle_درج_rule_عدد_table, rule_عدد)
GENERATE_MUSCLE_درج_TABLE (muscle_درج_نماد_عدد_table, نماد_عدد)
GENERATE_MUSCLE_درج_TABLE (muscle_درج_item_عدد_table, item_عدد)
GENERATE_MUSCLE_درج_TABLE (muscle_درج_state_عدد_table, state_عدد)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

ایستا تهی
output_escaped (FILE *out, ثابت نویسه *cp)
{
  برای (; *cp; ++cp)
    انتخاب (*cp)
      {
      حالت '$': fputs ("$][", out); بشکن;
      حالت '@': fputs ("@@",  out); بشکن;
      حالت '[': fputs ("@{",  out); بشکن;
      حالت ']': fputs ("@}",  out); بشکن;

      حالت 'b':
        اگر (STRPREFIX_LIT ("b4_", cp))
          {
            fputs ("b4@'_", out);
            cp += strlen ("b4_") - 1;
            بشکن;
          }
        وگرنه
          برو append;

      حالت 'm':
        اگر (STRPREFIX_LIT ("m4_", cp))
          {
            fputs ("m4@'_", out);
            cp += strlen ("m4_") - 1;
            بشکن;
          }
        وگرنه
          برو append;

      append:
      پیشفرض:  fputc (*cp,   out); بشکن;
      }
}

ایستا تهی
output_quoted (FILE *out, نویسه ثابت *cp)
{
  fprintf (out, "[[");
  output_escaped (out, cp);
  fprintf (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

ایستا تهی
string_output (FILE *out, نویسه ثابت *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
ایستا ptrdiff_t
escape_trigraphs (نویسه *buffer, ptrdiff_t buffersize, ثابت نویسه *src)
{
#define STORE(c)                                \
  انجامبده                                            \
    {                                           \
      اگر (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  وقتی (0)
  ptrdiff_t res = 0;
  برای (ptrdiff_t i = 0, len = strlen (src); i < len; ++i)
    {
      اگر (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          انتخاب (src[i+2])
            {
            حالت '!': حالت '\'':
            حالت '(': حالت ')': حالت '-': حالت '/':
            حالت '<': حالت '=': حالت '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              ادامه;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  بازگشت res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
ایستا نویسه *
xescape_trigraphs (ثابت نویسه *src)
{
  ptrdiff_t bufsize = escape_trigraphs (پوچ, 0, src);
  نویسه *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  بازگشت buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
ایستا ثابت نویسه *
نماد_tag (ثابت نماد *sym)
{
  ثابت bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  اگر (!eof_is_user_defined && sym->content == eoftoken->content)
    بازگشت "\"end of file\"";
  وگرنه اگر (sym->content == undeftoken->content)
    بازگشت "\"invalid token\"";
  وگرنه
    بازگشت sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

ایستا تهی
prepare_نماد_names (نویسه ثابت *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  ثابت bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  صحیح col = 2;
  ساختار quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  برای (صحیح i = 0; i < nsyms; i++)
    {
      ثابت نویسه *tag = نماد_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      اگر (translatable)
        has_translations = true;

      نویسه *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      صحیح width
        = mbswidth (cp, 0) + 2
        + (translatable ? strlen ("N_()") : 0);

      اگر (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      اگر (i)
        obstack_1grow (&format_obstack, ' ');
      اگر (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      اگر (translatable)
        obstack_sgrow (&format_obstack, "])[");
      آزاد (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  آزاد (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_درج (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_درج_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

ایستا تهی
prepare_symbols (تهی)
{
  MUSCLE_درج_INT ("tokens_number", ntokens);
  MUSCLE_درج_INT ("nterms_number", nnterms);
  MUSCLE_درج_INT ("symbols_number", nsyms);
  MUSCLE_درج_INT ("code_max", max_code);

  muscle_درج_نماد_عدد_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_نماد_names ("tname");
  prepare_نماد_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    برای (صحیح i = 0; i < ntokens; ++i)
      اگر (symbols[i]->translatable)
        {
          translatable = true;
          بشکن;
        }
    اگر (translatable)
      {
        صحیح *مقادیر = xnmalloc (nsyms, اندازه *مقادیر);
        برای (صحیح i = 0; i < ntokens; ++i)
          مقادیر[i] = symbols[i]->translatable;
        muscle_درج_int_table ("translatable", مقادیر,
                                 مقادیر[0], 1, ntokens);
        آزاد (مقادیر);
      }
  }

  /* Output YYTOKNUM. */
  {
    صحیح *مقادیر = xnmalloc (ntokens, اندازه *مقادیر);
    برای (صحیح i = 0; i < ntokens; ++i)
      مقادیر[i] = symbols[i]->content->code;
    muscle_درج_int_table ("toknum", مقادیر,
                             مقادیر[0], 1, ntokens);
    آزاد (مقادیر);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

ایستا تهی
prepare_rules (تهی)
{
  صحیح *prhs = xnmalloc (nrules, اندازه *prhs);
  item_عدد *rhs = xnmalloc (nritems, اندازه *rhs);
  صحیح *rline = xnmalloc (nrules, اندازه *rline);
  نماد_عدد *r1 = xnmalloc (nrules, اندازه *r1);
  صحیح *r2 = xnmalloc (nrules, اندازه *r2);
  صحیح *dprec = xnmalloc (nrules, اندازه *dprec);
  صحیح *merger = xnmalloc (nrules, اندازه *merger);
  صحیح *immediate = xnmalloc (nrules, اندازه *immediate);

  /* Index in RHS.  */
  صحیح i = 0;
  برای (rule_عدد r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      برای (item_عدد *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.سطر;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->عدد;
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

  muscle_درج_item_عدد_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_درج_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_درج_int_table ("rline", rline, 0, 0, nrules);
  muscle_درج_نماد_عدد_table ("r1", r1, 0, 0, nrules);
  muscle_درج_int_table ("r2", r2, 0, 0, nrules);
  muscle_درج_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_درج_int_table ("merger", merger, 0, 0, nrules);
  muscle_درج_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_درج_INT ("rules_number", nrules);
  MUSCLE_درج_INT ("max_left_semantic_context", max_چپ_semantic_context);

  آزاد (prhs);
  آزاد (rhs);
  آزاد (rline);
  آزاد (r1);
  آزاد (r2);
  آزاد (dprec);
  آزاد (merger);
  آزاد (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

ایستا تهی
prepare_states (تهی)
{
  نماد_عدد *مقادیر = xnmalloc (nstates, اندازه *مقادیر);
  برای (state_عدد i = 0; i < nstates; ++i)
    مقادیر[i] = states[i]->accessing_نماد;
  muscle_درج_نماد_عدد_table ("stos", مقادیر,
                                     0, 1, nstates);
  آزاد (مقادیر);

  MUSCLE_درج_INT ("last", بالا);
  MUSCLE_درج_INT ("final_state_number", final_state->عدد);
  MUSCLE_درج_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

ایستا صحیح
نماد_type_name_cmp (ثابت نماد **lhs, ثابت نماد **rhs)
{
  صحیح res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  اگر (!res)
    res = (*lhs)->content->عدد - (*rhs)->content->عدد;
  بازگشت res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

ایستا نماد **
symbols_by_type_name (تهی)
{
  تعریفنوع صحیح (*qcmp_type) (ثابت تهی *, ثابت تهی *);
  نماد **res = xmemdup (symbols, nsyms * اندازه *res);
  qsort (res, nsyms, اندازه *res, (qcmp_type) &نماد_type_name_cmp);
  بازگشت res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

ایستا تهی
type_names_output (FILE *out)
{
  نماد **syms = symbols_by_type_name ();
  fputs ("m4_define([b4_type_names],\n[", out);
  برای (صحیح i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      صحیح i0 = i;
      fputs (i ? ",\n[" : "[", out);
      برای (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        fprintf (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->عدد);
      fputs ("]", out);
    }
  fputs ("])\n\n", out);
  آزاد (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
ایستا تهی
start_symbols_output (FILE *out)
{
  اگر (start_symbols && start_symbols->next)
    {
      fputs ("m4_define([b4_start_symbols],\n[", out);
      برای (نماد_list *list = start_symbols; list; list = list->next)
        {
          ثابت نماد *start = list->content.sym;
          ثابت نماد *swtok = switching_token (start);
          fprintf (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->عدد, swtok->content->عدد);
        }
      fputs ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

ایستا تهی
نماد_numbers_output (FILE *out)
{
  fputs ("m4_define([b4_symbol_numbers],\n[", out);
  برای (صحیح i = 0; i < nsyms; ++i)
    fprintf (out, "%s[%d]", i ? ", " : "", i);
  fputs ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

ایستا تهی
rule_output (ثابت rule *r, FILE *out)
{
  output_escaped (out, r->lhs->نماد->tag);
  fputc (':', out);
  اگر (0 <= *r->rhs)
    برای (item_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        fputc (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  وگرنه
    fputs (" %empty", out);
}

ایستا تهی
user_actions_output (FILE *out)
{
  fputs ("m4_define([b4_actions], \n[", out);
  برای (rule_عدد r = 0; r < nrules; ++r)
    اگر (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        fprintf (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        اگر (!no_سطرها_flag)
          {
            fprintf (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.سطر);
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

ایستا تهی
merger_output (FILE *out)
{
  fputs ("m4_define([b4_mergers], \n[[", out);
  صحیح n;
  merger_list* p;
  برای (n = 1, p = merge_functions; p != پوچ; n += 1, p = p->next)
    fprintf (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->عدد);
  fputs ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

ایستا تهی
prepare_نماد_definitions (تهی)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  برای (نماد_عدد i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      ثابت نویسه *کلید = obstack_finish0 (&format_obstack);
      MUSCLE_درج_INT (کلید, nterm_map ? nterm_map[i - ntokens] : i);
    }

  برای (صحیح i = 0; i < nsyms; ++i)
    {
      نماد *sym = symbols[i];
      ثابت نویسه *کلید;

#define SET_کلید(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      کلید = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      کلید = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      ثابت نویسه *id = نماد_id_get (sym);
      SET_کلید ("has_id");
      MUSCLE_درج_INT (کلید, !!id);

      /* Its identifier.  */
      SET_کلید ("id");
      MUSCLE_درج_STRING (کلید, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_کلید ("tag");
      MUSCLE_درج_STRING (کلید, نماد_tag (sym));

      SET_کلید ("code");
      MUSCLE_درج_INT (کلید, sym->content->code);

      SET_کلید ("is_token");
      MUSCLE_درج_INT (کلید, i < ntokens);

      SET_کلید ("number");
      MUSCLE_درج_INT (کلید, sym->content->عدد);

      SET_کلید ("has_type");
      MUSCLE_درج_INT (کلید, !!sym->content->type_name);

      SET_کلید ("type");
      MUSCLE_درج_STRING (کلید, sym->content->type_name
                            ? sym->content->type_name : "");

      برای (صحیح j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          نویسه ثابت *pname = code_props_type_string (j) + 1;
          code_props ثابت *p = نماد_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_درج_INT (کلید, !!p->code);

          اگر (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_درج_C_STRING (کلید, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_درج_INT (کلید, p->location.start.سطر);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (کلید, p->location);

              SET_کلید (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_درج (کلید, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_کلید
    }
}


ایستا تهی
prepare_actions (تهی)
{
  /* Figure out the actions for the specified state.  */
  muscle_درج_rule_عدد_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_درج_state_عدد_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_درج_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_درج_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_درج_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_درج_base_table ("table", table,
                            table[0], 1, بالا + 1);
  MUSCLE_درج_INT ("table_ninf", table_ninf);

  muscle_درج_base_table ("check", check,
                            check[0], 1, بالا + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_درج_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, بالا + 1);
  muscle_درج_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

ایستا تهی
muscles_output (FILE *out)
{
  fputs ("m4_init()\n", out);
  merger_output (out);
  نماد_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

ایستا تهی
output_skeleton (تهی)
{
  /* Compute the names of the package data dir and skeleton files.  */
  نویسه ثابت *m4 = m4path ();
  نویسه ثابت *datadir = pkgdatadir ();
  نویسه *skeldir = xpath_join (datadir, "skeletons");
  نویسه *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  نویسه *m4bison = xpath_join (skeldir, "bison.m4");
  نویسه *traceon = xpath_join (skeldir, "traceon.m4");
  نویسه *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  صحیح filter_fd[2];
  pid_t pid;
  {
    نویسه ثابت *argv[11];
    صحیح i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    اگر (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    اگر (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    اگر (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = پوچ;
    aver (i <= ARRAY_CARDINALITY (argv));

    اگر (trace_flag & trace_tools)
      {
        fputs ("running:", stderr);
        برای (صحیح j = 0; argv[j]; ++j)
          fprintf (stderr, " %s", argv[j]);
        fputc ('\n', stderr);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ پوچ,
                            /* null_stderr */ false,
                            /* slave_process */ true,
                            /* exit_on_error */ true,
                            filter_fd);
  }

  آزاد (skeldir);
  آزاد (m4sugar);
  آزاد (m4bison);
  آزاد (traceon);
  آزاد (skel);

  اگر (trace_flag & trace_muscles)
    muscles_output (stderr);
  {
    FILE *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_افزودن (tv_m4);
  {
    FILE *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (feof (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, پوچ);
  timevar_برداشت (tv_m4);
}

ایستا تهی
prepare (تهی)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  نویسه ثابت *cp = getenv ("BISON_USE_PUSH_FOR_PULL");
  bool use_افزودن_for_pull_flag = cp && *cp && strtol (cp, 0, 10);

  /* Versions.  */
  MUSCLE_درج_STRING ("version_string", VERSION);
  MUSCLE_درج_INT ("version", strversion_to_int (VERSION));
  MUSCLE_درج_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_درج_BOOL ("header_flag", header_flag);
  MUSCLE_درج_BOOL ("glr_flag", glr_parser);
  MUSCLE_درج_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_درج_BOOL ("synclines_flag", !no_سطرها_flag);
  MUSCLE_درج_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_درج_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_درج_BOOL ("use_push_for_pull_flag", use_افزودن_for_pull_flag);
  MUSCLE_درج_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  اگر (spec_name_prefix)
    MUSCLE_درج_STRING ("prefix", spec_name_prefix);

  MUSCLE_درج_STRING ("file_name_all_but_ext", all_but_ext);

  ثابت نویسه *spec_mapped_header_file = map_file_name (spec_header_file);
  ثابت نویسه *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_درج_STRING (#Name, Name ? Name : "")
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
  اگر (skeleton)
    MUSCLE_درج_C_STRING ("skeleton", skeleton);
  وگرنه
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    نویسه *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_درج_STRING_RAW ("skeletonsdir", skeldir);
    آزاد (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

تهی
output (تهی)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_نماد_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  اگر (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, پوچ);
}
