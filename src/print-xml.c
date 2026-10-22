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

#अन्तर्भाव <config.h>
#अन्तर्भाव "print-xml.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <stdarg.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "execute.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "path-join.h"
#अन्तर्भाव "print.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "symtab.h"
#अन्तर्भाव "tables.h"

स्थिर bitset no_reduce_set;
संरचना escape_buf
{
  अक्षर *ptr;
  माप_प्रकार size;
};
गणना { अंक_escape_bufs = 3 };
स्थिर संरचना escape_buf escape_bufs[अंक_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (खाता *out, पूर्णांक level, state *s)
{
  item_index *sitems = s->वस्तुसंच;
  माप_प्रकार snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  जर (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      परत;
    }

  xml_puts (out, level, "<itemset>");

  साठी (माप_प्रकार i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_संख्या *sp1 = ritem + sitems[i];
      rule स्थिरांक *r = item_rule (sp1);
      item_संख्या *sp = r->rhs;

      /* Display the lookahead tokens?  */
      जर (item_संख्या_is_rule_संख्या (*sp1))
        {
          reductions *reds = s->reductions;
          पूर्णांक red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          जर (reds->lookaheads && red != -1)
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

      जर (!printed)
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

स्थिर शून्य
print_transitions (state *s, खाता *out, पूर्णांक level)
{
  transitions *trans = s->transitions;
  पूर्णांक n = 0;
  पूर्णांक i;

  साठी (i = 0; i < trans->अंक; i++)
    जर (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  जर (!n)
    {
      xml_puts (out, level, "<transitions/>");
      परत;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  साठी (i = 0; i < trans->अंक; i++)
    जर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        चिन्ह *sym = symbols[TRANSITION_चिन्ह (trans, i)];
        अक्षर स्थिरांक *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->संख्या);
      }

  साठी (i = 0; i < trans->अंक; i++)
    जर (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        चिन्ह *sym = symbols[TRANSITION_चिन्ह (trans, i)];
        अक्षर स्थिरांक *tag = sym->tag;
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

स्थिर शून्य
print_errs (खाता *out, पूर्णांक level, state *s)
{
  errs *errp = s->errs;
  bool मोजणी = false;
  पूर्णांक i;

  साठी (i = 0; i < errp->अंक; ++i)
    जर (errp->symbols[i])
      मोजणी = true;

  /* Nothing to report. */
  जर (!मोजणी)
    {
      xml_puts (out, level, "<errors/>");
      परत;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  साठी (i = 0; i < errp->अंक; ++i)
    जर (errp->symbols[i])
      {
        अक्षर स्थिरांक *tag = errp->symbols[i]->tag;
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

स्थिर शून्य
print_reduction (खाता *out, पूर्णांक level, अक्षर स्थिरांक *lookahead,
                 rule *r, bool enabled)
{
  जर (rule_is_initial (r))
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

स्थिर शून्य
print_reductions (खाता *out, पूर्णांक level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = रिक्त;
  पूर्णांक report = false;
  पूर्णांक i, j;

  जर (reds->अंक == 0)
    {
      xml_puts (out, level, "<reductions/>");
      परत;
    }

  जर (yydefact[s->संख्या] != 0)
    default_reduction = &rules[yydefact[s->संख्या] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_चिन्ह (trans, i));
  साठी (i = 0; i < s->errs->अंक; ++i)
    जर (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->संख्या);

  जर (default_reduction)
    report = true;

  जर (reds->lookaheads)
    साठी (i = 0; i < ntokens; i++)
      {
        bool मोजणी = bitset_test (no_reduce_set, i);

        साठी (j = 0; j < reds->अंक; ++j)
          जर (bitset_test (reds->lookaheads[j], i))
            {
              जर (! मोजणी)
                {
                  जर (reds->rules[j] != default_reduction)
                    report = true;
                  मोजणी = true;
                }
              अन्यथा
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  जर (!report)
    {
      xml_puts (out, level, "<reductions/>");
      परत;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  जर (reds->lookaheads)
    साठी (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool मोजणी = bitset_test (no_reduce_set, i);

        साठी (j = 0; j < reds->अंक; ++j)
          जर (bitset_test (reds->lookaheads[j], i))
            {
              जर (! मोजणी)
                {
                  जर (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  अन्यथा
                    defaulted = true;
                  मोजणी = true;
                }
              अन्यथा
                {
                  जर (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  जर (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
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

स्थिर शून्य
print_state (खाता *out, पूर्णांक level, state *s)
{
  ख_अक्षर_दो ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->संख्या);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  जर (s->solved_conflicts_xml)
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

स्थिर शून्य
print_grammar (खाता *out, पूर्णांक level)
{
  ख_अक्षर_दो ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  साठी (पूर्णांक i = 0; i < max_code + 1; i++)
    जर (token_translations[i] != undeftoken->content->संख्या)
      {
        चिन्ह स्थिरांक *sym = symbols[token_translations[i]];
        अक्षर स्थिरांक *tag = sym->tag;
        अक्षर स्थिरांक *type = sym->content->type_name;
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
        जर (precedence)
          ख_लिखो (out, " prec=\"%d\"", precedence);
        जर (associativity != undef_assoc)
          ख_लिखो (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        ख_माला_दो ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  साठी (चिन्ह_संख्या i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      चिन्ह स्थिरांक *sym = symbols[i];
      अक्षर स्थिरांक *tag = sym->tag;
      अक्षर स्थिरांक *type = sym->content->type_name;
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

शून्य
xml_indent (खाता *out, पूर्णांक level)
{
  साठी (पूर्णांक i = 0; i < level; i++)
    ख_माला_दो ("  ", out);
}

शून्य
xml_puts (खाता *out, पूर्णांक level, अक्षर स्थिरांक *s)
{
  xml_indent (out, level);
  ख_माला_दो (s, out);
  ख_अक्षर_दो ('\n', out);
}

शून्य
xml_printf (खाता *out, पूर्णांक level, अक्षर स्थिरांक *fmt, ...)
{
  बहु_सूची arglist;

  xml_indent (out, level);

  बहु_शुरू (arglist, fmt);
  भख_लिखो (out, fmt, arglist);
  बहु_पूर्ण (arglist);

  ख_अक्षर_दो ('\n', out);
}

स्थिर अक्षर स्थिरांक *
xml_escape_string (संरचना escape_buf *buf, अक्षर स्थिरांक *str)
{
  माप_प्रकार len = म_माप (str);
  माप_प्रकार max_expansion = आकार "&quot;" - 1;

  जर (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  अक्षर *p = buf->ptr;

  साठी (; *str; str++)
    निवड (*str)
      {
      पूर्वनिर्धारित: *p++ = *str; थांबा;
      स्थिती '&': p = stpcpy (p, "&amp;" ); थांबा;
      स्थिती '<': p = stpcpy (p, "&lt;"  ); थांबा;
      स्थिती '>': p = stpcpy (p, "&gt;"  ); थांबा;
      स्थिती '"': p = stpcpy (p, "&quot;"); थांबा;
      }

  *p = '\0';
  परत buf->ptr;
}

अक्षर स्थिरांक *
xml_escape_n (पूर्णांक n, अक्षर स्थिरांक *str)
{
  परत xml_escape_string (escape_bufs + n, str);
}

अक्षर स्थिरांक *
xml_escape (अक्षर स्थिरांक *str)
{
  परत xml_escape_n (0, str);
}

शून्य
print_xml (शून्य)
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
  साठी (state_संख्या i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  साठी (पूर्णांक i = 0; i < अंक_escape_bufs; ++i)
    मुक्त (escape_bufs[i].ptr);

  xfclose (out);
}


शून्य
print_html (शून्य)
{
  प्रतिज्ञा (xml_flag);

  अक्षर *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  अक्षर *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  अक्षर स्थिरांक *argv[11];
  पूर्णांक i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = रिक्त;
  aver (i <= ARRAY_CARDINALITY (argv));

  जर (trace_flag & trace_tools)
    {
      ख_माला_दो ("running:", मानक_त्रुटि);
      साठी (पूर्णांक j = 0; argv[j]; ++j)
        ख_लिखो (मानक_त्रुटि, " %s", argv[j]);
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
    }

  पूर्णांक status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ रिक्त,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ रिक्त);
  जर (status)
    complain (रिक्त, complaint, _("%s failed with status %d"), argv[0], status);
  मुक्त (xsltproc);
  मुक्त (xml2html);
}
