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
  वर्ण *ptr;
  माप_प्रकार size;
};
गणना { अङ्क_escape_bufs = 3 };
स्थिर संरचना escape_buf escape_bufs[अङ्क_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (खाता *out, पूर्णाङ्क level, state *s)
{
  item_index *sitems = s->वस्तुहरू;
  माप_प्रकार snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  यदि (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      फर्काऊ;
    }

  xml_puts (out, level, "<itemset>");

  लागि (माप_प्रकार i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_सङ्ख्या *sp1 = ritem + sitems[i];
      rule अचर *r = item_rule (sp1);
      item_सङ्ख्या *sp = r->rhs;

      /* Display the lookahead tokens?  */
      यदि (item_सङ्ख्या_is_rule_सङ्ख्या (*sp1))
        {
          reductions *reds = s->reductions;
          पूर्णाङ्क red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          यदि (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->सङ्ख्या, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      यदि (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->सङ्ख्या,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

स्थिर शून्य
print_transitions (state *s, खाता *out, पूर्णाङ्क level)
{
  transitions *trans = s->transitions;
  पूर्णाङ्क n = 0;
  पूर्णाङ्क i;

  लागि (i = 0; i < trans->अङ्क; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  यदि (!n)
    {
      xml_puts (out, level, "<transitions/>");
      फर्काऊ;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  लागि (i = 0; i < trans->अङ्क; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        चिह्न *sym = symbols[TRANSITION_चिह्न (trans, i)];
        वर्ण अचर *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->सङ्ख्या);
      }

  लागि (i = 0; i < trans->अङ्क; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        चिह्न *sym = symbols[TRANSITION_चिह्न (trans, i)];
        वर्ण अचर *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->सङ्ख्या);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

स्थिर शून्य
print_errs (खाता *out, पूर्णाङ्क level, state *s)
{
  errs *errp = s->errs;
  bool गन्ती = false;
  पूर्णाङ्क i;

  लागि (i = 0; i < errp->अङ्क; ++i)
    यदि (errp->symbols[i])
      गन्ती = true;

  /* Nothing to report. */
  यदि (!गन्ती)
    {
      xml_puts (out, level, "<errors/>");
      फर्काऊ;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  लागि (i = 0; i < errp->अङ्क; ++i)
    यदि (errp->symbols[i])
      {
        वर्ण अचर *tag = errp->symbols[i]->tag;
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
print_reduction (खाता *out, पूर्णाङ्क level, वर्ण अचर *lookahead,
                 rule *r, bool enabled)
{
  यदि (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  अन्यथा
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->सङ्ख्या,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

स्थिर शून्य
print_reductions (खाता *out, पूर्णाङ्क level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = रिक्त;
  पूर्णाङ्क report = false;
  पूर्णाङ्क i, j;

  यदि (reds->अङ्क == 0)
    {
      xml_puts (out, level, "<reductions/>");
      फर्काऊ;
    }

  यदि (yydefact[s->सङ्ख्या] != 0)
    default_reduction = &rules[yydefact[s->सङ्ख्या] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_चिह्न (trans, i));
  लागि (i = 0; i < s->errs->अङ्क; ++i)
    यदि (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->सङ्ख्या);

  यदि (default_reduction)
    report = true;

  यदि (reds->lookaheads)
    लागि (i = 0; i < ntokens; i++)
      {
        bool गन्ती = bitset_test (no_reduce_set, i);

        लागि (j = 0; j < reds->अङ्क; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गन्ती)
                {
                  यदि (reds->rules[j] != default_reduction)
                    report = true;
                  गन्ती = true;
                }
              अन्यथा
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  यदि (!report)
    {
      xml_puts (out, level, "<reductions/>");
      फर्काऊ;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  यदि (reds->lookaheads)
    लागि (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गन्ती = bitset_test (no_reduce_set, i);

        लागि (j = 0; j < reds->अङ्क; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गन्ती)
                {
                  यदि (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  अन्यथा
                    defaulted = true;
                  गन्ती = true;
                }
              अन्यथा
                {
                  यदि (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  यदि (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
print_actions (खाता *out, पूर्णाङ्क level, state *s)
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
print_state (खाता *out, पूर्णाङ्क level, state *s)
{
  ख_अक्षर_दो ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->सङ्ख्या);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  यदि (s->solved_conflicts_xml)
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
print_grammar (खाता *out, पूर्णाङ्क level)
{
  ख_अक्षर_दो ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  लागि (पूर्णाङ्क i = 0; i < max_code + 1; i++)
    यदि (token_translations[i] != undeftoken->content->सङ्ख्या)
      {
        चिह्न अचर *sym = symbols[token_translations[i]];
        वर्ण अचर *tag = sym->tag;
        वर्ण अचर *type = sym->content->type_name;
        पूर्णाङ्क precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        ख_लिखो (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        यदि (precedence)
          ख_लिखो (out, " prec=\"%d\"", precedence);
        यदि (associativity != undef_assoc)
          ख_लिखो (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        ख_माला_दो ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      चिह्न अचर *sym = symbols[i];
      वर्ण अचर *tag = sym->tag;
      वर्ण अचर *type = sym->content->type_name;
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
xml_indent (खाता *out, पूर्णाङ्क level)
{
  लागि (पूर्णाङ्क i = 0; i < level; i++)
    ख_माला_दो ("  ", out);
}

शून्य
xml_puts (खाता *out, पूर्णाङ्क level, वर्ण अचर *s)
{
  xml_indent (out, level);
  ख_माला_दो (s, out);
  ख_अक्षर_दो ('\n', out);
}

शून्य
xml_printf (खाता *out, पूर्णाङ्क level, वर्ण अचर *fmt, ...)
{
  बहु_सूची arglist;

  xml_indent (out, level);

  बहु_शुरू (arglist, fmt);
  भख_लिखो (out, fmt, arglist);
  बहु_पूर्ण (arglist);

  ख_अक्षर_दो ('\n', out);
}

स्थिर वर्ण अचर *
xml_escape_string (संरचना escape_buf *buf, वर्ण अचर *str)
{
  माप_प्रकार len = म_माप (str);
  माप_प्रकार max_expansion = परिमाण "&quot;" - 1;

  यदि (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  वर्ण *p = buf->ptr;

  लागि (; *str; str++)
    विकल्प (*str)
      {
      स्वभाव: *p++ = *str; रोक;
      पक्ष '&': p = stpcpy (p, "&amp;" ); रोक;
      पक्ष '<': p = stpcpy (p, "&lt;"  ); रोक;
      पक्ष '>': p = stpcpy (p, "&gt;"  ); रोक;
      पक्ष '"': p = stpcpy (p, "&quot;"); रोक;
      }

  *p = '\0';
  फर्काऊ buf->ptr;
}

वर्ण अचर *
xml_escape_n (पूर्णाङ्क n, वर्ण अचर *str)
{
  फर्काऊ xml_escape_string (escape_bufs + n, str);
}

वर्ण अचर *
xml_escape (वर्ण अचर *str)
{
  फर्काऊ xml_escape_n (0, str);
}

शून्य
print_xml (शून्य)
{
  खाता *out = xfopen (spec_xml_file, "w");

  ख_माला_दो ("<?xml version=\"1.0\"?>\n\n", out);

  पूर्णाङ्क level = 0;
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
  लागि (state_सङ्ख्या i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  लागि (पूर्णाङ्क i = 0; i < अङ्क_escape_bufs; ++i)
    मुक्त (escape_bufs[i].ptr);

  xfclose (out);
}


शून्य
print_html (शून्य)
{
  प्रतिज्ञा (xml_flag);

  वर्ण *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  वर्ण *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  वर्ण अचर *argv[11];
  पूर्णाङ्क i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = रिक्त;
  aver (i <= ARRAY_CARDINALITY (argv));

  यदि (trace_flag & trace_tools)
    {
      ख_माला_दो ("running:", मानक_त्रुटि);
      लागि (पूर्णाङ्क j = 0; argv[j]; ++j)
        ख_लिखो (मानक_त्रुटि, " %s", argv[j]);
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
    }

  पूर्णाङ्क status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ रिक्त,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ रिक्त);
  यदि (status)
    complain (रिक्त, complaint, _("%s failed with status %d"), argv[0], status);
  मुक्त (xsltproc);
  मुक्त (xml2html);
}
