/* Print an xml on generated parser, for Bison,

   Copyright (C) 2007, 2009-2015, 2018-2022 Free Software Foundation,
   Inc.

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
#समावेश "print-xml.h"

#समावेश "system.h"

#समावेश <bitset.h>
#समावेश <stdarg.h>

#समावेश "closure.h"
#समावेश "complain.h"
#समावेश "conflicts.h"
#समावेश "execute.h"
#समावेश "files.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "lalr.h"
#समावेश "lr0.h"
#समावेश "muscle-tab.h"
#समावेश "path-join.h"
#समावेश "print.h"
#समावेश "reader.h"
#समावेश "reduce.h"
#समावेश "state.h"
#समावेश "symtab.h"
#समावेश "tables.h"

अटल bitset no_reduce_set;
काष्ठा escape_buf
{
  अक्षर *ptr;
  माप_प्रकार आकार;
};
क्रमागत { अंक_escape_bufs = 3 };
अटल काष्ठा escape_buf escape_bufs[अंक_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

अटल व्योम
print_core (खाता *out, पूर्णांक level, state *s)
{
  item_सूचकांक *sitems = s->वस्तुएँ;
  माप_प्रकार snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  अगर (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      वापस;
    }

  xml_puts (out, level, "<itemset>");

  क्रम (माप_प्रकार i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_संख्या *sp1 = ritem + sitems[i];
      rule स्थिर *r = item_rule (sp1);
      item_संख्या *sp = r->rhs;

      /* Display the lookahead tokens?  */
      अगर (item_संख्या_is_rule_संख्या (*sp1))
        {
          reductions *reds = s->reductions;
          पूर्णांक red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          अगर (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->संख्या, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      अगर (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->संख्या,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

अटल व्योम
print_transitions (state *s, खाता *out, पूर्णांक level)
{
  transitions *trans = s->transitions;
  पूर्णांक n = 0;
  पूर्णांक i;

  क्रम (i = 0; i < trans->अंक; i++)
    अगर (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  अगर (!n)
    {
      xml_puts (out, level, "<transitions/>");
      वापस;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  क्रम (i = 0; i < trans->अंक; i++)
    अगर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        अक्षर स्थिर *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->संख्या);
      }

  क्रम (i = 0; i < trans->अंक; i++)
    अगर (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        अक्षर स्थिर *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->संख्या);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

अटल व्योम
print_errs (खाता *out, पूर्णांक level, state *s)
{
  errs *errp = s->errs;
  bool गिनती = false;
  पूर्णांक i;

  क्रम (i = 0; i < errp->अंक; ++i)
    अगर (errp->symbols[i])
      गिनती = true;

  /* Nothing to report. */
  अगर (!गिनती)
    {
      xml_puts (out, level, "<errors/>");
      वापस;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  क्रम (i = 0; i < errp->अंक; ++i)
    अगर (errp->symbols[i])
      {
        अक्षर स्थिर *tag = errp->symbols[i]->tag;
        xml_printf (out, level + 1,
                    "<error symbol=\"%s\">nonassociative</error>",
                    xml_escape (tag));
      }
  xml_puts (out, level, "</errors>");
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

अटल व्योम
print_reduction (खाता *out, पूर्णांक level, अक्षर स्थिर *lookahead,
                 rule *r, bool enabled)
{
  अगर (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  अन्यथा
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->संख्या,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

अटल व्योम
print_reductions (खाता *out, पूर्णांक level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = शून्य;
  पूर्णांक report = false;
  पूर्णांक i, j;

  अगर (reds->अंक == 0)
    {
      xml_puts (out, level, "<reductions/>");
      वापस;
    }

  अगर (yydefact[s->संख्या] != 0)
    default_reduction = &rules[yydefact[s->संख्या] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_प्रतीक (trans, i));
  क्रम (i = 0; i < s->errs->अंक; ++i)
    अगर (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->संख्या);

  अगर (default_reduction)
    report = true;

  अगर (reds->lookaheads)
    क्रम (i = 0; i < ntokens; i++)
      {
        bool गिनती = bitset_test (no_reduce_set, i);

        क्रम (j = 0; j < reds->अंक; ++j)
          अगर (bitset_test (reds->lookaheads[j], i))
            {
              अगर (! गिनती)
                {
                  अगर (reds->rules[j] != default_reduction)
                    report = true;
                  गिनती = true;
                }
              अन्यथा
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  अगर (!report)
    {
      xml_puts (out, level, "<reductions/>");
      वापस;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  अगर (reds->lookaheads)
    क्रम (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गिनती = bitset_test (no_reduce_set, i);

        क्रम (j = 0; j < reds->अंक; ++j)
          अगर (bitset_test (reds->lookaheads[j], i))
            {
              अगर (! गिनती)
                {
                  अगर (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  अन्यथा
                    defaulted = true;
                  गिनती = true;
                }
              अन्यथा
                {
                  अगर (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  अगर (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

अटल व्योम
print_actions (खाता *out, पूर्णांक level, state *s)
{
  xml_puts (out, level, "<actions>");
  print_transitions (s, out, level + 1);
  print_errs (out, level + 1, s);
  print_reductions (out, level + 1, s);
  xml_puts (out, level, "</actions>");
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

अटल व्योम
print_state (खाता *out, पूर्णांक level, state *s)
{
  ख_अक्षर_दो ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->संख्या);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  अगर (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      ख_माला_दो (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  अन्यथा
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

अटल व्योम
print_grammar (खाता *out, पूर्णांक level)
{
  ख_अक्षर_दो ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  क्रम (पूर्णांक i = 0; i < max_code + 1; i++)
    अगर (token_translations[i] != undeftoken->content->संख्या)
      {
        प्रतीक स्थिर *sym = symbols[token_translations[i]];
        अक्षर स्थिर *tag = sym->tag;
        अक्षर स्थिर *type = sym->content->type_name;
        पूर्णांक precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        ख_लिखो (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        अगर (precedence)
          ख_लिखो (out, " prec=\"%d\"", precedence);
        अगर (associativity != undef_assoc)
          ख_लिखो (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        ख_माला_दो ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      प्रतीक स्थिर *sym = symbols[i];
      अक्षर स्थिर *tag = sym->tag;
      अक्षर स्थिर *type = sym->content->type_name;
      xml_printf (out, level + 2,
                  "<nonterminal symbol-number=\"%d\" name=\"%s\""
                  " type=\"%s\""
                  " usefulness=\"%s\"/>",
                  i, xml_escape_n (0, tag),
                  type ? xml_escape_n (1, type) : "",
                  reduce_nonterminal_useless_in_grammar (sym->content)
                    ? "useless-in-grammar" : "useful");
    }
  xml_puts (out, level + 1, "</nonterminals>");
  xml_puts (out, level, "</grammar>");
}

व्योम
xml_indent (खाता *out, पूर्णांक level)
{
  क्रम (पूर्णांक i = 0; i < level; i++)
    ख_माला_दो ("  ", out);
}

व्योम
xml_puts (खाता *out, पूर्णांक level, अक्षर स्थिर *s)
{
  xml_indent (out, level);
  ख_माला_दो (s, out);
  ख_अक्षर_दो ('\n', out);
}

व्योम
xml_printf (खाता *out, पूर्णांक level, अक्षर स्थिर *fmt, ...)
{
  बहु_सूची arglist;

  xml_indent (out, level);

  बहु_शुरू (arglist, fmt);
  भख_लिखो (out, fmt, arglist);
  बहु_पूर्ण (arglist);

  ख_अक्षर_दो ('\n', out);
}

अटल अक्षर स्थिर *
xml_escape_string (काष्ठा escape_buf *buf, अक्षर स्थिर *str)
{
  माप_प्रकार len = म_माप (str);
  माप_प्रकार max_expansion = माप "&quot;" - 1;

  अगर (buf->आकार <= max_expansion * len)
    {
      buf->आकार = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->आकार);
    }
  अक्षर *p = buf->ptr;

  क्रम (; *str; str++)
    चयन (*str)
      {
      शेष: *p++ = *str; अवरोध;
      हाल '&': p = stpcpy (p, "&amp;" ); अवरोध;
      हाल '<': p = stpcpy (p, "&lt;"  ); अवरोध;
      हाल '>': p = stpcpy (p, "&gt;"  ); अवरोध;
      हाल '"': p = stpcpy (p, "&quot;"); अवरोध;
      }

  *p = '\0';
  वापस buf->ptr;
}

अक्षर स्थिर *
xml_escape_n (पूर्णांक n, अक्षर स्थिर *str)
{
  वापस xml_escape_string (escape_bufs + n, str);
}

अक्षर स्थिर *
xml_escape (अक्षर स्थिर *str)
{
  वापस xml_escape_n (0, str);
}

व्योम
print_xml (व्योम)
{
  खाता *out = xfopen (spec_xml_file, "w");

  ख_माला_दो ("<?xml version=\"1.0\"?>\n\n", out);

  पूर्णांक level = 0;
  xml_printf (out, level,
              "<bison-xml-report version=\"%s\" bug-report=\"%s\""
              " url=\"%s\">",
              xml_escape_n (0, VERSION),
              xml_escape_n (1, PACKAGE_BUGREPORT),
              xml_escape_n (2, PACKAGE_URL));

  ख_अक्षर_दो ('\n', out);
  xml_printf (out, level + 1, "<filename>%s</filename>",
              xml_escape (grammar_file));

  /* print grammar */
  print_grammar (out, level + 1);

  no_reduce_set = bitset_create (ntokens, BITSET_FIXED);

  /* print automaton */
  ख_अक्षर_दो ('\n', out);
  xml_puts (out, level + 1, "<automaton>");
  क्रम (state_संख्या i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  क्रम (पूर्णांक i = 0; i < अंक_escape_bufs; ++i)
    मुक्त (escape_bufs[i].ptr);

  xfclose (out);
}


व्योम
print_html (व्योम)
{
  निश्चित (xml_flag);

  अक्षर *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  अक्षर *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  अक्षर स्थिर *argv[11];
  पूर्णांक i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = शून्य;
  aver (i <= ARRAY_CARDINALITY (argv));

  अगर (trace_flag & trace_tools)
    {
      ख_माला_दो ("running:", मानक_त्रुटि);
      क्रम (पूर्णांक j = 0; argv[j]; ++j)
        ख_लिखो (मानक_त्रुटि, " %s", argv[j]);
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
    }

  पूर्णांक status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ शून्य,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ शून्य);
  अगर (status)
    complain (शून्य, complaint, _("%s failed with status %d"), argv[0], status);
  मुक्त (xsltproc);
  मुक्त (xml2html);
}
