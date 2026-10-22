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


#define GENERATE_MUSCLE_घुसाउ_TABLE(Name, Type)                        \
                                                                        \
स्थिर शून्य                                                             \
Name (वर्ण अचर *name, Type *table_data, Type first,                   \
      पूर्णाङ्क begin, पूर्णाङ्क end)                                               \
{                                                                       \
  Type min = first;                                                     \
  Type max = first;                                                     \
  पूर्णाङ्क j = 1;                                                            \
                                                                        \
  obstack_printf (&format_obstack, "%6d", first);                       \
  लागि (पूर्णाङ्क i = begin; i < end; ++i)                                     \
    {                                                                   \
      obstack_1grow (&format_obstack, ',');                             \
      यदि (j >= 10)                                                      \
        {                                                               \
          obstack_sgrow (&format_obstack, "\n  ");                      \
          j = 1;                                                        \
        }                                                               \
      अन्यथा                                                              \
        ++j;                                                            \
      obstack_printf (&format_obstack, "%6d", table_data[i]);           \
      यदि (table_data[i] < min)                                          \
        min = table_data[i];                                            \
      यदि (max < table_data[i])                                          \
        max = table_data[i];                                            \
    }                                                                   \
  muscle_घुसाउ (name, obstack_finish0 (&format_obstack));              \
                                                                        \
  दीर्घ lmin = min;                                                      \
  दीर्घ lmax = max;                                                      \
  /* Build 'NAME_min' and 'NAME_max' in the obstack. */                 \
  obstack_printf (&format_obstack, "%s_min", name);                     \
  MUSCLE_घुसाउ_LONG_INT (obstack_finish0 (&format_obstack), lmin);     \
  obstack_printf (&format_obstack, "%s_max", name);                     \
  MUSCLE_घुसाउ_LONG_INT (obstack_finish0 (&format_obstack), lmax);     \
}

GENERATE_MUSCLE_घुसाउ_TABLE (muscle_घुसाउ_int_table, पूर्णाङ्क)
GENERATE_MUSCLE_घुसाउ_TABLE (muscle_घुसाउ_base_table, base_सङ्ख्या)
GENERATE_MUSCLE_घुसाउ_TABLE (muscle_घुसाउ_rule_सङ्ख्या_table, rule_सङ्ख्या)
GENERATE_MUSCLE_घुसाउ_TABLE (muscle_घुसाउ_चिह्न_सङ्ख्या_table, चिह्न_सङ्ख्या)
GENERATE_MUSCLE_घुसाउ_TABLE (muscle_घुसाउ_item_सङ्ख्या_table, item_सङ्ख्या)
GENERATE_MUSCLE_घुसाउ_TABLE (muscle_घुसाउ_state_सङ्ख्या_table, state_सङ्ख्या)


/*----------------------------------------------------------------.
| Print to OUT a representation of CP quoted and escaped for M4.  |
|                                                                 |
| Keep sync'ed with obstack_escape.                               |
`----------------------------------------------------------------*/

स्थिर शून्य
output_escaped (खाता *out, अचर वर्ण *cp)
{
  लागि (; *cp; ++cp)
    विकल्प (*cp)
      {
      पक्ष '$': ख_माला_दो ("$][", out); रोक;
      पक्ष '@': ख_माला_दो ("@@",  out); रोक;
      पक्ष '[': ख_माला_दो ("@{",  out); रोक;
      पक्ष ']': ख_माला_दो ("@}",  out); रोक;

      पक्ष 'b':
        यदि (STRPREFIX_LIT ("b4_", cp))
          {
            ख_माला_दो ("b4@'_", out);
            cp += म_माप ("b4_") - 1;
            रोक;
          }
        अन्यथा
          गच्छ append;

      पक्ष 'm':
        यदि (STRPREFIX_LIT ("m4_", cp))
          {
            ख_माला_दो ("m4@'_", out);
            cp += म_माप ("m4_") - 1;
            रोक;
          }
        अन्यथा
          गच्छ append;

      append:
      स्वभाव:  ख_अक्षर_दो (*cp,   out); रोक;
      }
}

स्थिर शून्य
output_quoted (खाता *out, वर्ण अचर *cp)
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
string_output (खाता *out, वर्ण अचर *string)
{
  output_quoted (out, quotearg_style (c_quoting_style, string));
}


/* Store in BUFFER a copy of SRC where trigraphs are escaped, return
   the size of the result (including the final NUL).  If called with
   BUFFERSIZE = 0, returns the needed size for BUFFER.  */
स्थिर सूचक_भेद_प्रकार
escape_trigraphs (वर्ण *buffer, सूचक_भेद_प्रकार buffersize, अचर वर्ण *src)
{
#define STORE(c)                                \
  गर                                            \
    {                                           \
      यदि (res < buffersize)                     \
        buffer[res] = (c);                      \
      ++res;                                    \
    }                                           \
  जबसम्म (0)
  सूचक_भेद_प्रकार res = 0;
  लागि (सूचक_भेद_प्रकार i = 0, len = म_माप (src); i < len; ++i)
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
              जारी;
            }
        }
      STORE (src[i]);
    }
  STORE ('\0');
#undef STORE
  फर्काऊ res;
}

/* Same as xstrdup, except that trigraphs are escaped.  */
स्थिर वर्ण *
xescape_trigraphs (अचर वर्ण *src)
{
  सूचक_भेद_प्रकार bufsize = escape_trigraphs (रिक्त, 0, src);
  वर्ण *buf = xcharalloc (bufsize);
  escape_trigraphs (buf, bufsize, src);
  फर्काऊ buf;
}

/* The tag to show in the generated parsers.  Use "end of file" rather
   than "$end".  But keep "$end" in the reports, it's shorter and more
   consistent.  Support i18n if the user already uses it.  */
स्थिर अचर वर्ण *
चिह्न_tag (अचर चिह्न *sym)
{
  अचर bool eof_is_user_defined
    = !eoftoken->alias || STRNEQ (eoftoken->alias->tag, "$end");

  यदि (!eof_is_user_defined && sym->content == eoftoken->content)
    फर्काऊ "\"end of file\"";
  अन्यथा यदि (sym->content == undeftoken->content)
    फर्काऊ "\"invalid token\"";
  अन्यथा
    फर्काऊ sym->tag;
}

/* Generate the b4_<MUSCLE_NAME> (e.g., b4_tname) table with the
   symbol names (aka tags). */

स्थिर शून्य
prepare_चिह्न_names (वर्ण अचर *muscle_name)
{
  // Whether to add a pair of quotes around the name.
  अचर bool quote = STREQ (muscle_name, "tname");
  bool has_translations = false;

  /* We assume that the table will be output starting at column 2. */
  पूर्णाङ्क col = 2;
  संरचना quoting_options *qo = clone_quoting_options (0);
  set_quoting_style (qo, c_quoting_style);
  set_quoting_flags (qo, QA_SPLIT_TRIGRAPHS);
  लागि (पूर्णाङ्क i = 0; i < nsyms; i++)
    {
      अचर वर्ण *tag = चिह्न_tag (symbols[i]);
      bool translatable = !quote && symbols[i]->translatable;
      यदि (translatable)
        has_translations = true;

      वर्ण *cp
        = tag[0] == '"' && !quote
        ? xescape_trigraphs (tag)
        : quotearg_alloc (tag, -1, qo);
      /* Width of the next token, including the two quotes, the
         comma and the space.  */
      पूर्णाङ्क width
        = mbswidth (cp, 0) + 2
        + (translatable ? म_माप ("N_()") : 0);

      यदि (col + width > 75)
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
      col += width;
    }
  मुक्त (qo);
  obstack_sgrow (&format_obstack, " ]b4_null[");

  /* Finish table and store. */
  muscle_घुसाउ (muscle_name, obstack_finish0 (&format_obstack));

  /* Announce whether translation support is needed.  */
  MUSCLE_घुसाउ_BOOL ("has_translations_flag", has_translations);
}


/*------------------------------------------------------------------.
| Prepare the muscles related to the symbols: translate, tname, and |
| toknum.                                                           |
`------------------------------------------------------------------*/

स्थिर शून्य
prepare_symbols (शून्य)
{
  MUSCLE_घुसाउ_INT ("tokens_number", ntokens);
  MUSCLE_घुसाउ_INT ("nterms_number", nnterms);
  MUSCLE_घुसाउ_INT ("symbols_number", nsyms);
  MUSCLE_घुसाउ_INT ("code_max", max_code);

  muscle_घुसाउ_चिह्न_सङ्ख्या_table ("translate",
                                     token_translations,
                                     token_translations[0],
                                     1, max_code + 1);

  /* tname -- token names.  */
  prepare_चिह्न_names ("tname");
  prepare_चिह्न_names ("symbol_names");

  /* translatable -- whether a token is translatable. */
  {
    bool translatable = false;
    लागि (पूर्णाङ्क i = 0; i < ntokens; ++i)
      यदि (symbols[i]->translatable)
        {
          translatable = true;
          रोक;
        }
    यदि (translatable)
      {
        पूर्णाङ्क *मानहरू = xnmalloc (nsyms, परिमाण *मानहरू);
        लागि (पूर्णाङ्क i = 0; i < ntokens; ++i)
          मानहरू[i] = symbols[i]->translatable;
        muscle_घुसाउ_int_table ("translatable", मानहरू,
                                 मानहरू[0], 1, ntokens);
        मुक्त (मानहरू);
      }
  }

  /* Output YYTOKNUM. */
  {
    पूर्णाङ्क *मानहरू = xnmalloc (ntokens, परिमाण *मानहरू);
    लागि (पूर्णाङ्क i = 0; i < ntokens; ++i)
      मानहरू[i] = symbols[i]->content->code;
    muscle_घुसाउ_int_table ("toknum", मानहरू,
                             मानहरू[0], 1, ntokens);
    मुक्त (मानहरू);
  }
}


/*-------------------------------------------------------------.
| Prepare the muscles related to the rules: rhs, prhs, r1, r2, |
| rline, dprec, merger, immediate.                             |
`-------------------------------------------------------------*/

स्थिर शून्य
prepare_rules (शून्य)
{
  पूर्णाङ्क *prhs = xnmalloc (nrules, परिमाण *prhs);
  item_सङ्ख्या *rhs = xnmalloc (nritems, परिमाण *rhs);
  पूर्णाङ्क *rline = xnmalloc (nrules, परिमाण *rline);
  चिह्न_सङ्ख्या *r1 = xnmalloc (nrules, परिमाण *r1);
  पूर्णाङ्क *r2 = xnmalloc (nrules, परिमाण *r2);
  पूर्णाङ्क *dprec = xnmalloc (nrules, परिमाण *dprec);
  पूर्णाङ्क *merger = xnmalloc (nrules, परिमाण *merger);
  पूर्णाङ्क *immediate = xnmalloc (nrules, परिमाण *immediate);

  /* Index in RHS.  */
  पूर्णाङ्क i = 0;
  लागि (rule_सङ्ख्या r = 0; r < nrules; ++r)
    {
      /* Index of rule R in RHS. */
      prhs[r] = i;
      /* RHS of the rule R. */
      लागि (item_सङ्ख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        rhs[i++] = *rhsp;
      /* Separator in RHS. */
      rhs[i++] = -1;

      /* Line where rule was defined. */
      rline[r] = rules[r].location.start.पङ्क्ति;
      /* LHS of the rule R. */
      r1[r] = rules[r].lhs->सङ्ख्या;
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

  muscle_घुसाउ_item_सङ्ख्या_table ("rhs", rhs, ritem[0], 1, nritems);
  muscle_घुसाउ_int_table ("prhs", prhs, 0, 0, nrules);
  muscle_घुसाउ_int_table ("rline", rline, 0, 0, nrules);
  muscle_घुसाउ_चिह्न_सङ्ख्या_table ("r1", r1, 0, 0, nrules);
  muscle_घुसाउ_int_table ("r2", r2, 0, 0, nrules);
  muscle_घुसाउ_int_table ("dprec", dprec, 0, 0, nrules);
  muscle_घुसाउ_int_table ("merger", merger, 0, 0, nrules);
  muscle_घुसाउ_int_table ("immediate", immediate, 0, 0, nrules);

  MUSCLE_घुसाउ_INT ("rules_number", nrules);
  MUSCLE_घुसाउ_INT ("max_left_semantic_context", max_बायाँ_semantic_context);

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
  चिह्न_सङ्ख्या *मानहरू = xnmalloc (nstates, परिमाण *मानहरू);
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    मानहरू[i] = states[i]->accessing_चिह्न;
  muscle_घुसाउ_चिह्न_सङ्ख्या_table ("stos", मानहरू,
                                     0, 1, nstates);
  मुक्त (मानहरू);

  MUSCLE_घुसाउ_INT ("last", माथिल्लो);
  MUSCLE_घुसाउ_INT ("final_state_number", final_state->सङ्ख्या);
  MUSCLE_घुसाउ_INT ("states_number", nstates);
}


/*-------------------------------------------------------.
| Compare two symbols by type-name, and then by number.  |
`-------------------------------------------------------*/

स्थिर पूर्णाङ्क
चिह्न_type_name_cmp (अचर चिह्न **lhs, अचर चिह्न **rhs)
{
  पूर्णाङ्क res = uniqstr_cmp ((*lhs)->content->type_name, (*rhs)->content->type_name);
  यदि (!res)
    res = (*lhs)->content->सङ्ख्या - (*rhs)->content->सङ्ख्या;
  फर्काऊ res;
}


/*----------------------------------------------------------------.
| Return a (malloc'ed) table of the symbols sorted by type-name.  |
`----------------------------------------------------------------*/

स्थिर चिह्न **
symbols_by_type_name (शून्य)
{
  प्रकारनिर्देश पूर्णाङ्क (*qcmp_type) (अचर शून्य *, अचर शून्य *);
  चिह्न **res = xmemdup (symbols, nsyms * परिमाण *res);
  क्विक (res, nsyms, परिमाण *res, (qcmp_type) &चिह्न_type_name_cmp);
  फर्काऊ res;
}


/*------------------------------------------------------------------.
| Define b4_type_names, which is a list of (lists of the numbers of |
| symbols with same type-name).                                     |
`------------------------------------------------------------------*/

स्थिर शून्य
type_names_output (खाता *out)
{
  चिह्न **syms = symbols_by_type_name ();
  ख_माला_दो ("m4_define([b4_type_names],\n[", out);
  लागि (पूर्णाङ्क i = 0; i < nsyms; /* nothing */)
    {
      /* The index of the first symbol of the current type-name.  */
      पूर्णाङ्क i0 = i;
      ख_माला_दो (i ? ",\n[" : "[", out);
      लागि (; i < nsyms
           && syms[i]->content->type_name == syms[i0]->content->type_name; ++i)
        ख_लिखो (out, "%s%d", i != i0 ? ", " : "", syms[i]->content->सङ्ख्या);
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
  यदि (start_symbols && start_symbols->next)
    {
      ख_माला_दो ("m4_define([b4_start_symbols],\n[", out);
      लागि (चिह्न_list *list = start_symbols; list; list = list->next)
        {
          अचर चिह्न *start = list->content.sym;
          अचर चिह्न *swtok = switching_token (start);
          ख_लिखो (out, "%s[%d, %d]",
                   list == start_symbols ? "" : ", ",
                   start->content->सङ्ख्या, swtok->content->सङ्ख्या);
        }
      ख_माला_दो ("])\n\n", out);
    }
}


/*-------------------------------------.
| The list of all the symbol numbers.  |
`-------------------------------------*/

स्थिर शून्य
चिह्न_numbers_output (खाता *out)
{
  ख_माला_दो ("m4_define([b4_symbol_numbers],\n[", out);
  लागि (पूर्णाङ्क i = 0; i < nsyms; ++i)
    ख_लिखो (out, "%s[%d]", i ? ", " : "", i);
  ख_माला_दो ("])\n\n", out);
}


/*-------------------------------------------.
| Output the user reduction actions to OUT.  |
`-------------------------------------------*/

स्थिर शून्य
rule_output (अचर rule *r, खाता *out)
{
  output_escaped (out, r->lhs->चिह्न->tag);
  ख_अक्षर_दो (':', out);
  यदि (0 <= *r->rhs)
    लागि (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
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
  लागि (rule_सङ्ख्या r = 0; r < nrules; ++r)
    यदि (rules[r].action)
      {
        /* The useless "" is there to pacify syntax-check.  */
        ख_लिखो (out, "%s""(%d, [",
                 rules[r].is_predicate ? "b4_predicate_case" : "b4_case",
                 r + 1);
        यदि (!no_पङ्क्तिहरू_flag)
          {
            ख_लिखो (out, "b4_syncline(%d, ",
                     rules[r].action_loc.start.पङ्क्ति);
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
  पूर्णाङ्क n;
  merger_list* p;
  लागि (n = 1, p = merge_functions; p != रिक्त; n += 1, p = p->next)
    ख_लिखो (out, "]b4_call_merger""([%d], [%s], [%d])[\n",
             n, p->name, p->sym->content->सङ्ख्या);
  ख_माला_दो ("]])\n\n", out);
}


/*---------------------------------------------.
| Prepare the muscles for symbol definitions.  |
`---------------------------------------------*/

स्थिर शून्य
prepare_चिह्न_definitions (शून्य)
{
  /* Map "orig NUM" to new numbers.  See data/README.  */
  लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms + nuseless_nonterminals; ++i)
    {
      obstack_printf (&format_obstack, "symbol""(orig %d, number)", i);
      अचर वर्ण *साँचो = obstack_finish0 (&format_obstack);
      MUSCLE_घुसाउ_INT (साँचो, nterm_map ? nterm_map[i - ntokens] : i);
    }

  लागि (पूर्णाङ्क i = 0; i < nsyms; ++i)
    {
      चिह्न *sym = symbols[i];
      अचर वर्ण *साँचो;

#define SET_साँचो(Entry)                                          \
      obstack_printf (&format_obstack, "symbol""(%d, %s)",        \
                      i, Entry);                                \
      साँचो = obstack_finish0 (&format_obstack);

#define SET_KEY2(Entry, Suffix)                                 \
      obstack_printf (&format_obstack, "symbol""(%d, %s_%s)",     \
                      i, Entry, Suffix);                        \
      साँचो = obstack_finish0 (&format_obstack);

      /* Whether the symbol has an identifier.  */
      अचर वर्ण *id = चिह्न_id_get (sym);
      SET_साँचो ("has_id");
      MUSCLE_घुसाउ_INT (साँचो, !!id);

      /* Its identifier.  */
      SET_साँचो ("id");
      MUSCLE_घुसाउ_STRING (साँचो, id ? id : "");

      /* Its tag.  Typically for documentation purpose.  */
      SET_साँचो ("tag");
      MUSCLE_घुसाउ_STRING (साँचो, चिह्न_tag (sym));

      SET_साँचो ("code");
      MUSCLE_घुसाउ_INT (साँचो, sym->content->code);

      SET_साँचो ("is_token");
      MUSCLE_घुसाउ_INT (साँचो, i < ntokens);

      SET_साँचो ("number");
      MUSCLE_घुसाउ_INT (साँचो, sym->content->सङ्ख्या);

      SET_साँचो ("has_type");
      MUSCLE_घुसाउ_INT (साँचो, !!sym->content->type_name);

      SET_साँचो ("type");
      MUSCLE_घुसाउ_STRING (साँचो, sym->content->type_name
                            ? sym->content->type_name : "");

      लागि (पूर्णाङ्क j = 0; j < CODE_PROPS_SIZE; ++j)
        {
          /* "printer", not "%printer".  */
          वर्ण अचर *pname = code_props_type_string (j) + 1;
          code_props अचर *p = चिह्न_code_props_get (sym, j);
          SET_KEY2 ("has", pname);
          MUSCLE_घुसाउ_INT (साँचो, !!p->code);

          यदि (p->code)
            {
              SET_KEY2 (pname, "file");
              MUSCLE_घुसाउ_C_STRING (साँचो, map_file_name (p->location.start.file));

              SET_KEY2 (pname, "line");
              MUSCLE_घुसाउ_INT (साँचो, p->location.start.पङ्क्ति);

              SET_KEY2 (pname, "loc");
              muscle_location_grow (साँचो, p->location);

              SET_साँचो (pname);
              obstack_printf (&muscle_obstack,
                              "%*s%s", p->location.start.column - 1, "", p->code);
              muscle_घुसाउ (साँचो, obstack_finish0 (&muscle_obstack));
            }
        }
#undef SET_KEY2
#undef SET_साँचो
    }
}


स्थिर शून्य
prepare_actions (शून्य)
{
  /* Figure out the actions for the specified state.  */
  muscle_घुसाउ_rule_सङ्ख्या_table ("defact", yydefact,
                                   yydefact[0], 1, nstates);

  /* Figure out what to do after reducing with each rule, depending on
     the saved state from before the beginning of parsing the data
     that matched this rule.  */
  muscle_घुसाउ_state_सङ्ख्या_table ("defgoto", yydefgoto,
                                    yydefgoto[0], 1, nsyms - ntokens);


  /* Output PACT. */
  muscle_घुसाउ_base_table ("pact", base,
                             base[0], 1, nstates);
  MUSCLE_घुसाउ_INT ("pact_ninf", base_ninf);

  /* Output PGOTO. */
  muscle_घुसाउ_base_table ("pgoto", base,
                             base[nstates], nstates + 1, nvectors);

  muscle_घुसाउ_base_table ("table", table,
                            table[0], 1, माथिल्लो + 1);
  MUSCLE_घुसाउ_INT ("table_ninf", table_ninf);

  muscle_घुसाउ_base_table ("check", check,
                            check[0], 1, माथिल्लो + 1);

  /* GLR parsing slightly modifies YYTABLE and YYCHECK (and thus
     YYPACT) so that in states with unresolved conflicts, the default
     reduction is not used in the conflicted entries, so that there is
     a place to put a conflict pointer.

     This means that YYCONFLP and YYCONFL are nonsense for a non-GLR
     parser, so we could avoid accidents by not writing them out in
     that case.  Nevertheless, it seems even better to be able to use
     the GLR skeletons even without the non-deterministic tables.  */
  muscle_घुसाउ_int_table ("conflict_list_heads", conflict_table,
                           conflict_table[0], 1, माथिल्लो + 1);
  muscle_घुसाउ_int_table ("conflicting_rules", conflict_list,
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
  चिह्न_numbers_output (out);
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
  वर्ण अचर *m4 = m4path ();
  वर्ण अचर *datadir = pkgdatadir ();
  वर्ण *skeldir = xpath_join (datadir, "skeletons");
  वर्ण *m4sugar = xpath_join (datadir, "m4sugar/m4sugar.m4");
  वर्ण *m4bison = xpath_join (skeldir, "bison.m4");
  वर्ण *traceon = xpath_join (skeldir, "traceon.m4");
  वर्ण *skel = (IS_PATH_WITH_DIR (skeleton)
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
    वर्ण अचर *argv[11];
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
        ख_माला_दो ("running:", मानक_त्रुटि);
        लागि (पूर्णाङ्क j = 0; argv[j]; ++j)
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

  यदि (trace_flag & trace_muscles)
    muscles_output (मानक_त्रुटि);
  {
    खाता *out = xfdopen (filter_fd[1], "w");
    muscles_output (out);
    xfclose (out);
  }

  /* Read and process m4's output.  */
  timevar_थप (tv_m4);
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
  timevar_झिक (tv_m4);
}

स्थिर शून्य
prepare (शून्य)
{
  /* BISON_USE_PUSH_FOR_PULL is for the test suite and should not be
     documented for the user.  */
  वर्ण अचर *cp = दो_पर्या ("BISON_USE_PUSH_FOR_PULL");
  bool use_थप_for_pull_flag = cp && *cp && म_से_दीर्घ (cp, 0, 10);

  /* Versions.  */
  MUSCLE_घुसाउ_STRING ("version_string", VERSION);
  MUSCLE_घुसाउ_INT ("version", strversion_to_int (VERSION));
  MUSCLE_घुसाउ_INT ("required_version", required_version);

  /* Flags. */
  MUSCLE_घुसाउ_BOOL ("header_flag", header_flag);
  MUSCLE_घुसाउ_BOOL ("glr_flag", glr_parser);
  MUSCLE_घुसाउ_BOOL ("nondeterministic_flag", nondeterministic_parser);
  MUSCLE_घुसाउ_BOOL ("synclines_flag", !no_पङ्क्तिहरू_flag);
  MUSCLE_घुसाउ_BOOL ("tag_seen_flag", tag_seen);
  MUSCLE_घुसाउ_BOOL ("token_table_flag", token_table_flag);
  MUSCLE_घुसाउ_BOOL ("use_push_for_pull_flag", use_थप_for_pull_flag);
  MUSCLE_घुसाउ_BOOL ("yacc_flag", !location_empty (yacc_loc));

  /* File names.  */
  यदि (spec_name_prefix)
    MUSCLE_घुसाउ_STRING ("prefix", spec_name_prefix);

  MUSCLE_घुसाउ_STRING ("file_name_all_but_ext", all_but_ext);

  अचर वर्ण *spec_mapped_header_file = map_file_name (spec_header_file);
  अचर वर्ण *mapped_dir_prefix = map_file_name (dir_prefix);

#define DEFINE(Name) MUSCLE_घुसाउ_STRING (#Name, Name ? Name : "")
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
  यदि (skeleton)
    MUSCLE_घुसाउ_C_STRING ("skeleton", skeleton);
  अन्यथा
    skeleton = language->skeleton;

  /* About the skeletons.  */
  {
    /* b4_skeletonsdir is used inside m4_include in the skeletons, so digraphs
       would never be expanded.  Hopefully no one has M4-special characters in
       his Bison installation path.  */
    वर्ण *skeldir = xpath_join (pkgdatadir (), "skeletons");
    MUSCLE_घुसाउ_STRING_RAW ("skeletonsdir", skeldir);
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
  prepare_चिह्न_definitions ();

  prepare ();

  /* Process the selected skeleton file.  */
  output_skeleton ();

  /* If late errors were generated, destroy the generated source
     files. */
  यदि (complaint_status)
    unlink_generated_sources ();

  obstack_free (&format_obstack, रिक्त);
}
