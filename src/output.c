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


#define GENERATE_MUSCLE_घाला_TABLE(Name, Type)                        \
                                                                        \
स्थिर शून्य                                                             \
Name (अक्षर स्थिरांक *name, Type *table_data, Type first,                   \
      पूर्णांक begin, पूर्णांक end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  पूर्णांक j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  साठी (पूर्णांक i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      जर (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      अन्यथा                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      जर (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      जर (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_घाला (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  दीर्घ lmin = min;                                                      \
  दीर्घ lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_घाला_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_घाला_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_घाला_TABLE (muscle_घाला_int_table, पूर्णांक)
GENERATE_MUSCLE_घाला_TABLE (muscle_घाला_base_table, base_संख्या)
GENERATE_MUSCLE_घाला_TABLE (muscle_घाला_rule_संख्या_table, rule_संख्या)
GENERATE_MUSCLE_घाला_TABLE (muscle_घाला_चिन्ह_संख्या_table, चिन्ह_संख्या)
GENERATE_MUSCLE_घाला_TABLE (muscle_घाला_item_संख्या_table, item_संख्या)
GENERATE_MUSCLE_घाला_TABLE (muscle_घाला_state_संख्या_table, state_संख्या)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

स्थिर शून्य
output_escaped (खाता *out, स्थिरांक अक्षर *cp)
{
  साठी (; *cp; ++cp)
    निवड (*cp)
      {
      स्थिती '$': ख_माला_दो ("$][", out); थांबा;
      स्थिती '@': ख_माला_दो ("@@",  out); थांबा;
      स्थिती '[': ख_माला_दो ("@{",  out); थांबा;
      स्थिती ']': ख_माला_दो ("@}",  out); थांबा;

      स्थिती 'b':
        जर (STRPREFIX_LIT ("b4_", cp))
          {
            ख_माला_दो ("b4@'_", out);
            cp += म_माप ("b4_") - 1;
            थांबा;
          }
        अन्यथा
          जा append;

      स्थिती 'm':
        जर (STRPREFIX_LIT ("m4_", cp))
          {
            ख_माला_दो ("m4@'_", out);
            cp += म_माप ("m4_") - 1;
            थांबा;
          }
        अन्यथा
          जा append;

      append:
      पूर्वनिर्धारित:  ख_अक्षर_दो (*cp,   out); थांबा;
      }
}

स्थिर शून्य
output_quoted (खाता *out, अक्षर स्थिरांक *cp)
{
  ख_लिखो (out, "[[");
  output_escaped (out, cp);
  ख_लिखो (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

स्थिर शून्य
string_output (खाता *out, अक्षर स्थिरांक *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
स्थिर सूचक_भेद_प्रकार
escape_trigraphs (अक्षर *buffer, सूचक_भेद_प्रकार buffersize, स्थिरांक अक्षर *src)
{
#define STORE(c)                                \
  करा                                            \
    {                                           \
      जर (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  पर्यंत (0)
  सूचक_भेद_प्रकार res = 0;
  साठी (सूचक_भेद_प्रकार i = 0, len = म_माप (src); i < len; ++i)
    {
      जर (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          निवड (src[i+2])
            {
            स्थिती '!': स्थिती '\'':
            स्थिती '(': स्थिती ')': स्थिती '-': स्थिती '/':
            स्थिती '<': स्थिती '=': स्थिती '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              सुरूठेवा;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  परत res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
स्थिर अक्षर *
xescape_trigraphs (स्थिरांक अक्षर *src)
{
  सूचक_भेद_प्रकार bufsize = escape_trigraphs (रिक्त, 0, src);
  अक्षर *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  परत buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
स्थिर स्थिरांक अक्षर *
चिन्ह_tag (स्थिरांक चिन्ह *sym)
{
  स्थिरांक bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  जर (!eof_is_user_defined && sym->content == eoftoken->content)
    परत "\"end of file\"";
  अन्यथा जर (sym->content == undeftoken->content)
    परत "\"invalid token\"";
  अन्यथा
    परत sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

स्थिर शून्य
prepare_चिन्ह_names (अक्षर स्थिरांक *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  स्थिरांक bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  पूर्णांक col = 2;
  संरचना quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  साठी (पूर्णांक i = 0; i < nsyms; i++)
    {
      स्थिरांक अक्षर *tag = चिन्ह_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      जर (translatable)
        has_translations = true;

      अक्षर *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      पूर्णांक width
        = mbswidth (cp, 0) + 2
        + (translatable ? म_माप ("N_()") : 0);

      जर (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      जर (i)
        obstack_1grow (&format_obstack, ' ');
      जर (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      जर (translatable)
        obstack_sgrow (&format_obstack, "])[");
      मुक्त (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  मुक्त (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_घाला (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_घाला_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

स्थिर शून्य
prepare_symbols (शून्य)
{
  MUSCLE_घाला_INT ("tokens_number", ntokens);
  MUSCLE_घाला_INT ("nterms_number", nnterms);
  MUSCLE_घाला_INT ("symbols_number", nsyms);
  MUSCLE_घाला_INT ("code_max", max_code);

  muscle_घाला_चिन्ह_संख्या_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_चिन्ह_names ("tname");
  prepare_चिन्ह_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    साठी (पूर्णांक i = 0; i < ntokens; ++i)
      जर (symbols[i]->translatable)
        {
          translatable = true;
          थांबा;
        }
    जर (translatable)
      {
        पूर्णांक *मूल्ये = xnmalloc (nsyms, आकार *मूल्ये);
        साठी (पूर्णांक i = 0; i < ntokens; ++i)
          मूल्ये[i] = symbols[i]->translatable;
        muscle_घाला_int_table ("translatable", मूल्ये,
                                 मूल्ये[0], 1, ntokens);
        मुक्त (मूल्ये);
      }
  }

  /* Output YYTOKNUM. */
  {
    पूर्णांक *मूल्ये = xnmalloc (ntokens, आकार *मूल्ये);
    साठी (पूर्णांक i = 0; i < ntokens; ++i)
      मूल्ये[i] = symbols[i]->content->code;
    muscle_घाला_int_table ("toknum", मूल्ये,
                             मूल्ये[0], 1, ntokens);
    मुक्त (मूल्ये);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

स्थिर शून्य
prepare_rules (शून्य)
{
  पूर्णांक *prhs = xnmalloc (nrules, आकार *prhs);
  item_संख्या *rhs = xnmalloc (nritems, आकार *rhs);
  पूर्णांक *rline = xnmalloc (nrules, आकार *rline);
  चिन्ह_संख्या *r1 = xnmalloc (nrules, आकार *r1);
  पूर्णांक *r2 = xnmalloc (nrules, आकार *r2);
  पूर्णांक *dprec = xnmalloc (nrules, आकार *dprec);
  पूर्णांक *merger = xnmalloc (nrules, आकार *merger);
  पूर्णांक *immediate = xnmalloc (nrules, आकार *immediate);

  /* Index in RHS.  */
  पूर्णांक i = 0;
  साठी (rule_संख्या r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      साठी (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.ओळ;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->संख्या;
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

  muscle_घाला_item_संख्या_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_घाला_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_घाला_int_table ("rline", rline, 0, 0, nrules);
  muscle_घाला_चिन्ह_संख्या_table ("r1", r1, 0, 0, nrules);
  muscle_घाला_int_table ("r2", r2, 0, 0, nrules);
  muscle_घाला_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_घाला_int_table ("merger", merger, 0, 0, nrules);
  muscle_घाला_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_घाला_INT ("rules_number", nrules);
  MUSCLE_घाला_INT ("max_left_semantic_context", max_डावा_semantic_context);

  मुक्त (prhs);
  मुक्त (rhs);
  मुक्त (rline);
  मुक्त (r1);
  मुक्त (r2);
  मुक्त (dprec);
  मुक्त (merger);
  मुक्त (immediate);
}

/*--------------------------------------------.
| Prepare the muscles related to the states.  |
`--------------------------------------------*/

स्थिर शून्य
prepare_states (शून्य)
{
  चिन्ह_संख्या *मूल्ये = xnmalloc (nstates, आकार *मूल्ये);
  साठी (state_संख्या i = 0; i < nstates; ++i)
    मूल्ये[i] = states[i]->accessing_चिन्ह;
  muscle_घाला_चिन्ह_संख्या_table ("stos", मूल्ये,
                                     0, 1, nstates);
  मुक्त (मूल्ये);

  MUSCLE_घाला_INT ("last", वरचा);
  MUSCLE_घाला_INT ("final_state_number", final_state->संख्या);
  MUSCLE_घाला_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

स्थिर पूर्णांक
चिन्ह_type_name_cmp (स्थिरांक चिन्ह **lhs, स्थिरांक चिन्ह **rhs)
{
  पूर्णांक res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  जर (!res)
    res = (*lhs)->content->संख्या - (*rhs)->content->संख्या;
  परत res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

स्थिर चिन्ह **
symbols_by_type_name (शून्य)
{
  प्रकारव्याख्या पूर्णांक (*qcmp_type) (स्थिरांक शून्य *, स्थिरांक शून्य *);
  चिन्ह **res = xmemdup (symbols, nsyms * आकार *res);
  क्विक (res, nsyms, आकार *res, (qcmp_type) &चिन्ह_type_name_cmp);
  परत res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

स्थिर शून्य
type_names_output (खाता *out)
{
  चिन्ह **syms = symbols_by_type_name ();
  ख_माला_दो ("m4_define([b4_type_names],\n[", out);
  साठी (पूर्णांक i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      पूर्णांक i0 = i;
      ख_माला_दो (i ? ",\n[" : "[", out);
      साठी (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        ख_लिखो (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->संख्या);
      ख_माला_दो ("]", out);
    }
  ख_माला_दो ("])\n\n", out);
  मुक्त (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
स्थिर शून्य
start_symbols_output (खाता *out)
{
  जर (start_symbols && start_symbols->next)
    {
      ख_माला_दो ("m4_define([b4_start_symbols],\n[", out);
      साठी (चिन्ह_list *list = start_symbols; list; list = list->next)
        {
          स्थिरांक चिन्ह *start = list->content.sym;
          स्थिरांक चिन्ह *swtok = switching_token (start);
          ख_लिखो (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->संख्या, swtok->content->संख्या);
        }
      ख_माला_दो ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

स्थिर शून्य
चिन्ह_numbers_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_symbol_numbers],\n[", out);
  साठी (पूर्णांक i = 0; i < nsyms; ++i)
    ख_लिखो (out, "%s[%d]", i ? ", " : "", i);
  ख_माला_दो ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

स्थिर शून्य
rule_output (स्थिरांक rule *r, खाता *out)
{
  output_escaped (out, r->lhs->चिन्ह->tag);
  ख_अक्षर_दो (':', out);
  जर (0 <= *r->rhs)
    साठी (item_संख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        ख_अक्षर_दो (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  अन्यथा
    ख_माला_दो (" %empty", out);
}

स्थिर शून्य
user_actions_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_actions], \n[", out);
  साठी (rule_संख्या r = 0; r < nrules; ++r)
    जर (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        ख_लिखो (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        जर (!no_ओळी_flag)
          {
            ख_लिखो (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.ओळ);
            string_output (out, map_file_name (rules[r].action_loc.start.file));
            ख_लिखो (out, ")dnl\n");
          }
        ख_लिखो (out, "[%*s%s]],\n[[",
                 rules[r].action_loc.start.column - 1, "",
                 rules[r].action);
        rule_output (&rules[r], out);
        ख_लिखो (out, "]])\n\n");
      }
  ख_माला_दो ("])\n\n", out);
}

/*------------------------------------.
| Output the merge functions to OUT.  |
`------------------------------------*/

स्थिर शून्य
merger_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_mergers], \n[[", out);
  पूर्णांक n;
  merger_list* p;
  साठी (n = 1, p = merge_functions; p != रिक्त; n += 1, p = p->next)
    ख_लिखो (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->संख्या);
  ख_माला_दो ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

स्थिर शून्य
prepare_चिन्ह_definitions (शून्य)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  साठी (चिन्ह_संख्या i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      स्थिरांक अक्षर *किल्ली = obstack_finish0 (&format_obstack);
      MUSCLE_घाला_INT (किल्ली, nterm_map ? nterm_map[i - ntokens] : i);
    }

  साठी (पूर्णांक i = 0; i < nsyms; ++i)
    {
      चिन्ह *sym = symbols[i];
      स्थिरांक अक्षर *किल्ली;

#define SET_किल्ली(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      किल्ली = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      किल्ली = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      स्थिरांक अक्षर *id = चिन्ह_id_get (sym);
      SET_किल्ली ("has_id");
      MUSCLE_घाला_INT (किल्ली, !!id);

      /* Its identifier.  */
      SET_किल्ली ("id");
      MUSCLE_घाला_STRING (किल्ली, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_किल्ली ("tag");
      MUSCLE_घाला_STRING (किल्ली, चिन्ह_tag (sym));

      SET_किल्ली ("code");
      MUSCLE_घाला_INT (किल्ली, sym->content->code);

      SET_किल्ली ("is_token");
      MUSCLE_घाला_INT (किल्ली, i < ntokens);

      SET_किल्ली ("number");
      MUSCLE_घाला_INT (किल्ली, sym->content->संख्या);

      SET_किल्ली ("has_type");
      MUSCLE_घाला_INT (किल्ली, !!sym->content->type_name);

      SET_किल्ली ("type");
      MUSCLE_घाला_STRING (किल्ली, sym->content->type_name
                            ? sym->content->type_name : "");

      साठी (पूर्णांक j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          अक्षर स्थिरांक *pname = code_props_type_string (j) + 1;
          code_props स्थिरांक *p = चिन्ह_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_घाला_INT (किल्ली, !!p->code);

          जर (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_घाला_C_STRING (किल्ली, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_घाला_INT (किल्ली, p->location.start.ओळ);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (किल्ली, p->location);

              SET_किल्ली (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_घाला (किल्ली, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_किल्ली
    }
}


स्थिर शून्य
prepare_actions (शून्य)
{
  /* Figure out the actions for the specified state.  */
  muscle_घाला_rule_संख्या_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_घाला_state_संख्या_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_घाला_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_घाला_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_घाला_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_घाला_base_table ("table", table,
                            table[0], 1, वरचा + 1);
  MUSCLE_घाला_INT ("table_ninf", table_ninf);

  muscle_घाला_base_table ("check", check,
                            check[0], 1, वरचा + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_घाला_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, वरचा + 1);
  muscle_घाला_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

स्थिर शून्य
muscles_output (खाता *out)
{
  ख_माला_दो ("m4_init()\n", out);
  merger_output (out);
  चिन्ह_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

स्थिर शून्य
output_skeleton (शून्य)
{
  /* Compute the names of the package data dir and skeleton files.  */
  अक्षर स्थिरांक *m4 = m4path ();
  अक्षर स्थिरांक *datadir = pkgdatadir ();
  अक्षर *skeldir = xpath_join (datadir, "skeletons");
  अक्षर *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  अक्षर *m4bison = xpath_join (skeldir, "bison.m4");
  अक्षर *traceon = xpath_join (skeldir, "traceon.m4");
  अक्षर *skel = (IS_PATH_WITH_DIR (skeleton)
                ? xstrdup (skeleton)
                : xpath_join (skeldir, skeleton));

  /* Test whether m4sugar.m4 is readable, to check for proper
     installation.  A faulty installation can cause deadlock, so a
     cheap sanity check is worthwhile.  */
  xfclose (xfopen (m4sugar, "r"));

  /* Create an m4 subprocess connected to us via two pipes.  */

  पूर्णांक filter_fd[2];
  pid_t pid;
  {
    अक्षर स्थिरांक *argv[11];
    पूर्णांक i = 0;
    argv[i++] = m4;

    /* When POSIXLY_CORRECT is set, GNU M4 1.6 and later disable GNU
       extensions, which Bison's skeletons depend on.  With older M4,
       it has no effect.  M4 1.4.12 added a -g/--gnu command-line
       option to make it explicit that a program wants GNU M4
       extensions even when POSIXLY_CORRECT is set.

       See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    जर (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    जर (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    जर (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = रिक्त;
    aver (i <= ARRAY_CARDINALITY (argv));

    जर (trace_flag & trace_tools)
      {
        ख_माला_दो ("running:", मानक_त्रुटि);
        साठी (पूर्णांक j = 0; argv[j]; ++j)
          ख_लिखो (मानक_त्रुटि, " %s", argv[j]);
        ख_अक्षर_दो ('\n', मानक_त्रुटि);
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

  जर (trace_flag & trace_muscles)
    muscles_output (मानक_त्रुटि);
  {
    खाता *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_ढकला (tv_m4);
  {
    खाता *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (ख_पूर्ण (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, रिक्त);
  timevar_काढा (tv_m4);
}

स्थिर शून्य
prepare (शून्य)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  अक्षर स्थिरांक *cp = दो_पर्या ("BISON_USE_PUSH_FOR_PULL");
  bool use_ढकला_for_pull_flag = cp && *cp && म_से_दीर्घ (cp, 0, 10);

  /* Versions.  */
  MUSCLE_घाला_STRING ("version_string", VERSION);
  MUSCLE_घाला_INT ("version", strversion_to_int (VERSION));
  MUSCLE_घाला_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_घाला_BOOL ("header_flag", header_flag);
  MUSCLE_घाला_BOOL ("glr_flag", glr_parser);
  MUSCLE_घाला_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_घाला_BOOL ("synclines_flag", !no_ओळी_flag);
  MUSCLE_घाला_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_घाला_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_घाला_BOOL ("use_push_for_pull_flag", use_ढकला_for_pull_flag);
  MUSCLE_घाला_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  जर (spec_name_prefix)
    MUSCLE_घाला_STRING ("prefix", spec_name_prefix);

  MUSCLE_घाला_STRING ("file_name_all_but_ext", all_but_ext);

  स्थिरांक अक्षर *spec_mapped_header_file = map_file_name (spec_header_file);
  स्थिरांक अक्षर *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_घाला_STRING (#Name, Name ? Name : "")
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
  जर (skeleton)
    MUSCLE_घाला_C_STRING ("skeleton", skeleton);
  अन्यथा
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    अक्षर *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_घाला_STRING_RAW ("skeletonsdir", skeldir);
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
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_चिन्ह_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  जर (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, रिक्त);
}
