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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <filename.h> /* IS_PATH_WITH_DIR */
#समावेश <get-errno.h>
#समावेश <mbswidth.h>
#समावेश <path-join.h>
#समावेश <quotearg.h>
#समावेश <spawn-pipe.h>
#समावेश <timevar.h>
#समावेश <wait-process.h>

#समावेश "complain.h"
#समावेश "files.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "muscle-tab.h"
#समावेश "output.h"
#समावेश "reader.h"
#समावेश "reduce.h"
#समावेश "scan-code.h"    /* max_left_semantic_context */
#समावेश "scan-skel.h"
#समावेश "symtab.h"
#समावेश "tables.h"
#समावेश "strversion.h"

अटल काष्ठा obstack format_obstack;


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


#घोषणा GENERATE_MUSCLE_जोड़ो_TABLE(Name, Type)                        \
                                                                        \
अटल व्योम                                                             \
Name (अक्षर स्थिर *name, Type *table_तथ्य, Type first,                   \
      पूर्णांक begin, पूर्णांक end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  पूर्णांक j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  क्रम (पूर्णांक i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      अगर (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      अन्यथा                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_तथ्य[i]);           \
      अगर (table_तथ्य[i] < min)                                          \
        min = table_तथ्य[i];                                            \
      अगर (max < table_तथ्य[i])                                          \
        max = table_तथ्य[i];                                            \
    }                                                                   \
  muscle_जोड़ो (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  दीर्घ lmin = min;                                                      \
  दीर्घ lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_जोड़ो_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_जोड़ो_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_जोड़ो_TABLE (muscle_जोड़ो_int_table, पूर्णांक)
GENERATE_MUSCLE_जोड़ो_TABLE (muscle_जोड़ो_base_table, base_संख्या)
GENERATE_MUSCLE_जोड़ो_TABLE (muscle_जोड़ो_rule_संख्या_table, rule_संख्या)
GENERATE_MUSCLE_जोड़ो_TABLE (muscle_जोड़ो_प्रतीक_संख्या_table, प्रतीक_संख्या)
GENERATE_MUSCLE_जोड़ो_TABLE (muscle_जोड़ो_item_संख्या_table, item_संख्या)
GENERATE_MUSCLE_जोड़ो_TABLE (muscle_जोड़ो_state_संख्या_table, state_संख्या)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

अटल व्योम
output_escaped (खाता *out, स्थिर अक्षर *cp)
{
  क्रम (; *cp; ++cp)
    चयन (*cp)
      {
      हाल '$': ख_माला_दो ("$][", out); अवरोध;
      हाल '@': ख_माला_दो ("@@",  out); अवरोध;
      हाल '[': ख_माला_दो ("@{",  out); अवरोध;
      हाल ']': ख_माला_दो ("@}",  out); अवरोध;

      हाल 'b':
        अगर (STRPREFIX_LIT ("b4_", cp))
          {
            ख_माला_दो ("b4@'_", out);
            cp += म_माप ("b4_") - 1;
            अवरोध;
          }
        अन्यथा
          जाओ append;

      हाल 'm':
        अगर (STRPREFIX_LIT ("m4_", cp))
          {
            ख_माला_दो ("m4@'_", out);
            cp += म_माप ("m4_") - 1;
            अवरोध;
          }
        अन्यथा
          जाओ append;

      append:
      शेष:  ख_अक्षर_दो (*cp,   out); अवरोध;
      }
}

अटल व्योम
output_quoted (खाता *out, अक्षर स्थिर *cp)
{
  ख_लिखो (out, "[[");
  output_escaped (out, cp);
  ख_लिखो (out, "]]");
}

/*----------------------------------------------------------------.
| Print to OUT a representation of STRING quoted and escaped both |
| for C and M4.                                                   |
`----------------------------------------------------------------*/

अटल व्योम
string_output (खाता *out, अक्षर स्थिर *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
अटल सूचक_भेद_प्रकार
escape_trigraphs (अक्षर *बफर, सूचक_भेद_प्रकार buffersize, स्थिर अक्षर *src)
{
#घोषणा STORE(c)                                \
  करो                                            \
    {                                           \
      अगर (res < buffersize)                     \
        बफर[res] = (c);                      \
      ++res;                                    \
    }                                           \
  जबतक (0)
  सूचक_भेद_प्रकार res = 0;
  क्रम (सूचक_भेद_प्रकार i = 0, len = म_माप (src); i < len; ++i)
    {
      अगर (i + 2 < len
          && src[i] == '?' && src[i+1] == '?')
        {
          चयन (src[i+2])
            {
            हाल '!': हाल '\'':
            हाल '(': हाल ')': हाल '-': हाल '/':
            हाल '<': हाल '=': हाल '>':
              i += 1;
              STORE ('?');
              STORE ('"');
              STORE ('"');
              STORE ('?');
              जारी;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#अघोषित STORE
  वापस res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
अटल अक्षर *
xescape_trigraphs (स्थिर अक्षर *src)
{
  सूचक_भेद_प्रकार bufsize = escape_trigraphs (शून्य, 0, src);
  अक्षर *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  वापस buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
अटल स्थिर अक्षर *
प्रतीक_tag (स्थिर प्रतीक *sym)
{
  स्थिर bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  अगर (!eof_is_user_defined && sym->content == eoftoken->content)
    वापस "\"end of file\"";
  अन्यथा अगर (sym->content == undeftoken->content)
    वापस "\"invalid token\"";
  अन्यथा
    वापस sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

अटल व्योम
prepare_प्रतीक_names (अक्षर स्थिर *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  स्थिर bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  पूर्णांक col = 2;
  काष्ठा quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  क्रम (पूर्णांक i = 0; i < nsyms; i++)
    {
      स्थिर अक्षर *tag = प्रतीक_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      अगर (translatable)
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

      अगर (col + width > 75)
        {
          obstack_sgrow (&format_obstack, "\n ");
          col = 1;
        }

      अगर (i)
        obstack_1grow (&format_obstack, ' ');
      अगर (translatable)
        obstack_sgrow (&format_obstack, "]b4_symbol_translate""([");
      obstack_escape (&format_obstack, cp);
      अगर (translatable)
        obstack_sgrow (&format_obstack, "])[");
      मुक्त (cp);
      obstack_1grow (&format_obstack, ',');
      col += width;
    }
  मुक्त (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_जोड़ो (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_जोड़ो_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

अटल व्योम
prepare_symbols (व्योम)
{
  MUSCLE_जोड़ो_INT ("tokens_number", ntokens);
  MUSCLE_जोड़ो_INT ("nterms_number", nnterms);
  MUSCLE_जोड़ो_INT ("symbols_number", nsyms);
  MUSCLE_जोड़ो_INT ("code_max", max_code);

  muscle_जोड़ो_प्रतीक_संख्या_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_प्रतीक_names ("tname");
  prepare_प्रतीक_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    क्रम (पूर्णांक i = 0; i < ntokens; ++i)
      अगर (symbols[i]->translatable)
        {
          translatable = true;
          अवरोध;
        }
    अगर (translatable)
      {
        पूर्णांक *मूल्यावली = xnmalloc (nsyms, माप *मूल्यावली);
        क्रम (पूर्णांक i = 0; i < ntokens; ++i)
          मूल्यावली[i] = symbols[i]->translatable;
        muscle_जोड़ो_int_table ("translatable", मूल्यावली,
                                 मूल्यावली[0], 1, ntokens);
        मुक्त (मूल्यावली);
      }
  }

  /* Output YYTOKNUM. */
  {
    पूर्णांक *मूल्यावली = xnmalloc (ntokens, माप *मूल्यावली);
    क्रम (पूर्णांक i = 0; i < ntokens; ++i)
      मूल्यावली[i] = symbols[i]->content->code;
    muscle_जोड़ो_int_table ("toknum", मूल्यावली,
                             मूल्यावली[0], 1, ntokens);
    मुक्त (मूल्यावली);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

अटल व्योम
prepare_rules (व्योम)
{
  पूर्णांक *prhs = xnmalloc (nrules, माप *prhs);
  item_संख्या *rhs = xnmalloc (nritems, माप *rhs);
  पूर्णांक *rline = xnmalloc (nrules, माप *rline);
  प्रतीक_संख्या *r1 = xnmalloc (nrules, माप *r1);
  पूर्णांक *r2 = xnmalloc (nrules, माप *r2);
  पूर्णांक *dprec = xnmalloc (nrules, माप *dprec);
  पूर्णांक *merger = xnmalloc (nrules, माप *merger);
  पूर्णांक *immediate = xnmalloc (nrules, माप *immediate);

  /* Index in RHS.  */
  पूर्णांक i = 0;
  क्रम (rule_संख्या r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      क्रम (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.पंक्ति;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->संख्या;
      /* Length of rule R's RHS. */
      r2[r] = rule_rhs_लंबाई (&rules[r]);
      /* Dynamic precedence (GLR).  */
      dprec[r] = rules[r].dprec;
      /* Merger-function index (GLR).  */
      merger[r] = rules[r].merger;
      /* Immediate reduction flags (GLR).  */
      immediate[r] = rules[r].is_predicate;
    }
  aver (i == nritems);

  muscle_जोड़ो_item_संख्या_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_जोड़ो_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_जोड़ो_int_table ("rline", rline, 0, 0, nrules);
  muscle_जोड़ो_प्रतीक_संख्या_table ("r1", r1, 0, 0, nrules);
  muscle_जोड़ो_int_table ("r2", r2, 0, 0, nrules);
  muscle_जोड़ो_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_जोड़ो_int_table ("merger", merger, 0, 0, nrules);
  muscle_जोड़ो_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_जोड़ो_INT ("rules_number", nrules);
  MUSCLE_जोड़ो_INT ("max_left_semantic_context", max_बायाँ_semantic_context);

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

अटल व्योम
prepare_states (व्योम)
{
  प्रतीक_संख्या *मूल्यावली = xnmalloc (nstates, माप *मूल्यावली);
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    मूल्यावली[i] = states[i]->accessing_प्रतीक;
  muscle_जोड़ो_प्रतीक_संख्या_table ("stos", मूल्यावली,
                                     0, 1, nstates);
  मुक्त (मूल्यावली);

  MUSCLE_जोड़ो_INT ("last", ऊपरी);
  MUSCLE_जोड़ो_INT ("final_state_number", final_state->संख्या);
  MUSCLE_जोड़ो_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

अटल पूर्णांक
प्रतीक_type_name_cmp (स्थिर प्रतीक **lhs, स्थिर प्रतीक **rhs)
{
  पूर्णांक res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  अगर (!res)
    res = (*lhs)->content->संख्या - (*rhs)->content->संख्या;
  वापस res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

अटल प्रतीक **
symbols_by_type_name (व्योम)
{
  प्रकार पूर्णांक (*qcmp_type) (स्थिर व्योम *, स्थिर व्योम *);
  प्रतीक **res = xmemdup (symbols, nsyms * माप *res);
  क्विक (res, nsyms, माप *res, (qcmp_type) &प्रतीक_type_name_cmp);
  वापस res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

अटल व्योम
type_names_output (खाता *out)
{
  प्रतीक **syms = symbols_by_type_name ();
  ख_माला_दो ("m4_define([b4_type_names],\n[", out);
  क्रम (पूर्णांक i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      पूर्णांक i0 = i;
      ख_माला_दो (i ? ",\n[" : "[", out);
      क्रम (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        ख_लिखो (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->संख्या);
      ख_माला_दो ("]", out);
    }
  ख_माला_दो ("])\n\n", out);
  मुक्त (syms);
}


/* Define the list of start symbols *if* there are several.  Define
   them by pairs: [START-SYMBOL-NUM, SWITCHING-TOKEN-SYMBOL-NUM]. */
अटल व्योम
start_symbols_output (खाता *out)
{
  अगर (start_symbols && start_symbols->अग्र)
    {
      ख_माला_दो ("m4_define([b4_start_symbols],\n[", out);
      क्रम (प्रतीक_list *list = start_symbols; list; list = list->अग्र)
        {
          स्थिर प्रतीक *start = list->content.sym;
          स्थिर प्रतीक *swtok = switching_token (start);
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

अटल व्योम
प्रतीक_numbers_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_symbol_numbers],\n[", out);
  क्रम (पूर्णांक i = 0; i < nsyms; ++i)
    ख_लिखो (out, "%s[%d]", i ? ", " : "", i);
  ख_माला_दो ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

अटल व्योम
rule_output (स्थिर rule *r, खाता *out)
{
  output_escaped (out, r->lhs->प्रतीक->tag);
  ख_अक्षर_दो (':', out);
  अगर (0 <= *r->rhs)
    क्रम (item_संख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      {
        ख_अक्षर_दो (' ', out);
        output_escaped (out, symbols[*rhsp]->tag);
      }
  अन्यथा
    ख_माला_दो (" %empty", out);
}

अटल व्योम
user_actions_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_actions], \n[", out);
  क्रम (rule_संख्या r = 0; r < nrules; ++r)
    अगर (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        ख_लिखो (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        अगर (!no_पंक्तियाँ_flag)
          {
            ख_लिखो (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.पंक्ति);
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

अटल व्योम
merger_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_mergers], \n[[", out);
  पूर्णांक n;
  merger_list* p;
  क्रम (n = 1, p = merge_functions; p != शून्य; n += 1, p = p->अग्र)
    ख_लिखो (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->संख्या);
  ख_माला_दो ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

अटल व्योम
prepare_प्रतीक_definitions (व्योम)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      स्थिर अक्षर *कुंजी = obstack_finish0 (&format_obstack);
      MUSCLE_जोड़ो_INT (कुंजी, nterm_map ? nterm_map[i - ntokens] : i);
    }

  क्रम (पूर्णांक i = 0; i < nsyms; ++i)
    {
      प्रतीक *sym = symbols[i];
      स्थिर अक्षर *कुंजी;

#घोषणा SET_कुंजी(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      कुंजी = obstack_finish0 (&format_obstack);

#घोषणा SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      कुंजी = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      स्थिर अक्षर *id = प्रतीक_id_get (sym);
      SET_कुंजी ("has_id");
      MUSCLE_जोड़ो_INT (कुंजी, !!id);

      /* Its identifier.  */
      SET_कुंजी ("id");
      MUSCLE_जोड़ो_STRING (कुंजी, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_कुंजी ("tag");
      MUSCLE_जोड़ो_STRING (कुंजी, प्रतीक_tag (sym));

      SET_कुंजी ("code");
      MUSCLE_जोड़ो_INT (कुंजी, sym->content->code);

      SET_कुंजी ("is_token");
      MUSCLE_जोड़ो_INT (कुंजी, i < ntokens);

      SET_कुंजी ("number");
      MUSCLE_जोड़ो_INT (कुंजी, sym->content->संख्या);

      SET_कुंजी ("has_type");
      MUSCLE_जोड़ो_INT (कुंजी, !!sym->content->type_name);

      SET_कुंजी ("type");
      MUSCLE_जोड़ो_STRING (कुंजी, sym->content->type_name
                            ? sym->content->type_name : "");

      क्रम (पूर्णांक j = 0; j < CODE_PROPS_आकार; ++j)
        {
          /* "printer", not "%printer".  */
          अक्षर स्थिर *pname = code_props_type_string (j) + 1;
          code_props स्थिर *p = प्रतीक_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_जोड़ो_INT (कुंजी, !!p->code);

          अगर (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_जोड़ो_C_STRING (कुंजी, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_जोड़ो_INT (कुंजी, p->location.start.पंक्ति);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (कुंजी, p->location);

              SET_कुंजी (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_जोड़ो (कुंजी, obstack_finish0 (&muscle_obstack));
            }
        }
#अघोषित SET_KEY2
#अघोषित SET_कुंजी
    }
}


अटल व्योम
prepare_actions (व्योम)
{
  /* Figure out the actions for the specified state.  */
  muscle_जोड़ो_rule_संख्या_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_जोड़ो_state_संख्या_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_जोड़ो_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_जोड़ो_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_जोड़ो_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_जोड़ो_base_table ("table", table,
                            table[0], 1, ऊपरी + 1);
  MUSCLE_जोड़ो_INT ("table_ninf", table_ninf);

  muscle_जोड़ो_base_table ("check", check,
                            check[0], 1, ऊपरी + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_जोड़ो_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, ऊपरी + 1);
  muscle_जोड़ो_int_table ("conflicting_rules", conflict_list,
                           0, 1, conflict_list_cnt);
}


/*--------------------------------------------.
| Output the definitions of all the muscles.  |
`--------------------------------------------*/

अटल व्योम
muscles_output (खाता *out)
{
  ख_माला_दो ("m4_init()\n", out);
  merger_output (out);
  प्रतीक_numbers_output (out);
  type_names_output (out);
  start_symbols_output (out);
  user_actions_output (out);
  /* Must be last.  */
  muscles_m4_output (out);
}

/*---------------------------.
| Call the skeleton parser.  |
`---------------------------*/

अटल व्योम
output_skeleton (व्योम)
{
  /* Compute the names of the package data dir and skeleton files.  */
  अक्षर स्थिर *m4 = m4path ();
  अक्षर स्थिर *datadir = pkgdatadir ();
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
    अक्षर स्थिर *argv[11];
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
    अगर (*M4_GNU_OPTION)
      argv[i++] = M4_GNU_OPTION;

    argv[i++] = "-I";
    argv[i++] = datadir;
    /* Some future version of GNU M4 (most likely 1.6) may treat the
       -dV in a position-dependent manner.  See the thread starting at
       <https://lists.gnu.org/r/bug-bison/2008-07/msg00000.html>
       for details.  */
    अगर (trace_flag & trace_m4_early)
      argv[i++] = "-dV";
    argv[i++] = m4sugar;
    argv[i++] = "-";
    argv[i++] = m4bison;
    अगर (trace_flag & trace_m4)
      argv[i++] = traceon;
    argv[i++] = skel;
    argv[i++] = शून्य;
    aver (i <= ARRAY_CARDINALITY (argv));

    अगर (trace_flag & trace_tools)
      {
        ख_माला_दो ("running:", मानक_त्रुटि);
        क्रम (पूर्णांक j = 0; argv[j]; ++j)
          ख_लिखो (मानक_त्रुटि, " %s", argv[j]);
        ख_अक्षर_दो ('\n', मानक_त्रुटि);
      }

    pid = create_pipe_bidi ("m4", m4, argv,
                            /* directory */ शून्य,
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

  अगर (trace_flag & trace_muscles)
    muscles_output (मानक_त्रुटि);
  {
    खाता *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_धकेलो (tv_m4);
  {
    खाता *in = xfdopen (filter_fd[0], "r");
    scan_skel (in);
    /* scan_skel should have read all of M4's output.  Otherwise, when we
       close the pipe, we risk letting M4 report a broken-pipe to the
       Bison user.  */
    aver (ख_पूर्ण (in));
    xfclose (in);
  }
  wait_subprocess (pid, "m4", false, false, true, true, शून्य);
  timevar_निकालो (tv_m4);
}

अटल व्योम
prepare (व्योम)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  अक्षर स्थिर *cp = दो_पर्या ("BISON_USE_PUSH_FOR_PULL");
  bool use_धकेलो_for_pull_flag = cp && *cp && म_से_दीर्घ (cp, 0, 10);

  /* Versions.  */
  MUSCLE_जोड़ो_STRING ("version_string", VERSION);
  MUSCLE_जोड़ो_INT ("version", strversion_to_int (VERSION));
  MUSCLE_जोड़ो_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_जोड़ो_BOOL ("header_flag", header_flag);
  MUSCLE_जोड़ो_BOOL ("glr_flag", glr_parser);
  MUSCLE_जोड़ो_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_जोड़ो_BOOL ("synclines_flag", !no_पंक्तियाँ_flag);
  MUSCLE_जोड़ो_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_जोड़ो_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_जोड़ो_BOOL ("use_push_for_pull_flag", use_धकेलो_for_pull_flag);
  MUSCLE_जोड़ो_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  अगर (spec_name_prefix)
    MUSCLE_जोड़ो_STRING ("prefix", spec_name_prefix);

  MUSCLE_जोड़ो_STRING ("file_name_all_but_ext", all_but_ext);

  स्थिर अक्षर *spec_mapped_header_file = map_file_name (spec_header_file);
  स्थिर अक्षर *mapped_dir_prefix = map_file_name (dir_prefix);

#घोषणा DEFINE(Name) MUSCLE_जोड़ो_STRING (#Name, Name ? Name : "")
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
#अघोषित DEFINE

  /* Find the right skeleton file, and add muscles about the skeletons.  */
  अगर (skeleton)
    MUSCLE_जोड़ो_C_STRING ("skeleton", skeleton);
  अन्यथा
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    अक्षर *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_जोड़ो_STRING_RAW ("skeletonsdir", skeldir);
    मुक्त (skeldir);
  }
}


/*----------------------------------------------------------.
| Output the parsing tables and the parser code to ftable.  |
`----------------------------------------------------------*/

व्योम
output (व्योम)
{
  obstack_init (&format_obstack);

  prepare_symbols ();
  prepare_rules ();
  prepare_states ();
  prepare_actions ();
  prepare_प्रतीक_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  अगर (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, शून्य);
}
