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

स्थिर bitset no_reduce_समुच्चयः;
संरचना escape_buf
{
  वर्ण *ptr;
  size_t आकार;
};
गणना { अङ्कः_escape_bufs = 3 };
स्थिर संरचना escape_buf escape_bufs[अङ्कः_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (FILE *बहिर्गमनम्, पूर्णाङ्क level, state *s)
{
  item_सूचकांक *sitems = s->वस्तूनि;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  संवृतिः (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  यदि (!snritems)
    {
      xml_puts (बहिर्गमनम्, level, "<itemset/>");
      प्रत्यागम;
    }

  xml_puts (बहिर्गमनम्, level, "<itemset>");

  प्रति (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_सङ्ख्या *sp1 = ritem + sitems[i];
      नियमः ध्रुव *r = item_नियमः (sp1);
      item_सङ्ख्या *sp = r->rhs;

      /* Display the lookahead tokens?  */
      यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (*sp1))
        {
          reductions *reds = s->reductions;
          पूर्णाङ्क red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          यदि (reds->lookaheads && red != -1)
            {
              xml_printf (बहिर्गमनम्, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->सङ्ख्या, sp1 - sp);
              state_नियमः_lookaheads_print_xml (s, r,
                                                     बहिर्गमनम्, level + 2);
              xml_puts (बहिर्गमनम्, level + 1, "</item>");
              printed = true;
            }
        }

      यदि (!printed)
        xml_printf (बहिर्गमनम्, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->सङ्ख्या,
                    sp1 - sp);
    }
  xml_puts (बहिर्गमनम्, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

स्थिर शून्य
print_transitions (state *s, FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  transitions *trans = s->transitions;
  पूर्णाङ्क n = 0;
  पूर्णाङ्क i;

  प्रति (i = 0; i < trans->अङ्कः; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  यदि (!n)
    {
      xml_puts (बहिर्गमनम्, level, "<transitions/>");
      प्रत्यागम;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (बहिर्गमनम्, level, "<transitions>");

  प्रति (i = 0; i < trans->अङ्कः; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_सरणम् (trans, i))
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        वर्ण ध्रुव *चिह्नकम् = sym->चिह्नकम्;
        state *s1 = trans->अवस्थाः[i];

        xml_printf (बहिर्गमनम्, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (चिह्नकम्), s1->सङ्ख्या);
      }

  प्रति (i = 0; i < trans->अङ्कः; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_सरणम् (trans, i))
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        वर्ण ध्रुव *चिह्नकम् = sym->चिह्नकम्;
        state *s1 = trans->अवस्थाः[i];

        xml_printf (बहिर्गमनम्, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (चिह्नकम्), s1->सङ्ख्या);
      }

  xml_puts (बहिर्गमनम्, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

स्थिर शून्य
print_errs (FILE *बहिर्गमनम्, पूर्णाङ्क level, state *s)
{
  errs *errp = s->errs;
  bool गणनम् = false;
  पूर्णाङ्क i;

  प्रति (i = 0; i < errp->अङ्कः; ++i)
    यदि (errp->symbols[i])
      गणनम् = true;

  /* Nothing to report. */
  यदि (!गणनम्)
    {
      xml_puts (बहिर्गमनम्, level, "<errors/>");
      प्रत्यागम;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (बहिर्गमनम्, level, "<errors>");
  प्रति (i = 0; i < errp->अङ्कः; ++i)
    यदि (errp->symbols[i])
      {
        वर्ण ध्रुव *चिह्नकम् = errp->symbols[i]->चिह्नकम्;
        xml_printf (बहिर्गमनम्, level + 1,
                    "<error symbol=\"%s\">nonassociative</error>",
                    xml_escape (चिह्नकम्));
      }
  xml_puts (बहिर्गमनम्, level, "</errors>");
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

स्थिर शून्य
print_reduction (FILE *बहिर्गमनम्, पूर्णाङ्क level, वर्ण ध्रुव *lookahead,
                 नियमः *r, bool enabled)
{
  यदि (नियमः_is_initial (r))
    xml_printf (बहिर्गमनम्, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  अन्यथा
    xml_printf (बहिर्गमनम्, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->सङ्ख्या,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

स्थिर शून्य
print_reductions (FILE *बहिर्गमनम्, पूर्णाङ्क level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  नियमः *default_reduction = रिक्त;
  पूर्णाङ्क report = false;
  पूर्णाङ्क i, j;

  यदि (reds->अङ्कः == 0)
    {
      xml_puts (बहिर्गमनम्, level, "<reductions/>");
      प्रत्यागम;
    }

  यदि (yydefact[s->सङ्ख्या] != 0)
    default_reduction = &नियमाः[yydefact[s->सङ्ख्या] - 1];

  bitset_zero (no_reduce_समुच्चयः);
  FOR_EACH_सरणम् (trans, i)
    bitset_समुच्चयः (no_reduce_समुच्चयः, TRANSITION_चिह्नम् (trans, i));
  प्रति (i = 0; i < s->errs->अङ्कः; ++i)
    यदि (s->errs->symbols[i])
      bitset_समुच्चयः (no_reduce_समुच्चयः, s->errs->symbols[i]->content->सङ्ख्या);

  यदि (default_reduction)
    report = true;

  यदि (reds->lookaheads)
    प्रति (i = 0; i < ntokens; i++)
      {
        bool गणनम् = bitset_test (no_reduce_समुच्चयः, i);

        प्रति (j = 0; j < reds->अङ्कः; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गणनम्)
                {
                  यदि (reds->नियमाः[j] != default_reduction)
                    report = true;
                  गणनम् = true;
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
      xml_puts (बहिर्गमनम्, level, "<reductions/>");
      प्रत्यागम;
    }

  xml_puts (बहिर्गमनम्, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  यदि (reds->lookaheads)
    प्रति (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गणनम् = bitset_test (no_reduce_समुच्चयः, i);

        प्रति (j = 0; j < reds->अङ्कः; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गणनम्)
                {
                  यदि (reds->नियमाः[j] != default_reduction)
                    print_reduction (बहिर्गमनम्, level + 1, symbols[i]->चिह्नकम्,
                                     reds->नियमाः[j], true);
                  अन्यथा
                    defaulted = true;
                  गणनम् = true;
                }
              अन्यथा
                {
                  यदि (defaulted)
                    print_reduction (बहिर्गमनम्, level + 1, symbols[i]->चिह्नकम्,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (बहिर्गमनम्, level + 1, symbols[i]->चिह्नकम्,
                                   reds->नियमाः[j], false);
                }
            }
      }

  यदि (default_reduction)
    print_reduction (बहिर्गमनम्, level + 1,
                     "$default", default_reduction, true);

  xml_puts (बहिर्गमनम्, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
print_actions (FILE *बहिर्गमनम्, पूर्णाङ्क level, state *s)
{
  xml_puts (बहिर्गमनम्, level, "<actions>");
  print_transitions (s, बहिर्गमनम्, level + 1);
  print_errs (बहिर्गमनम्, level + 1, s);
  print_reductions (बहिर्गमनम्, level + 1, s);
  xml_puts (बहिर्गमनम्, level, "</actions>");
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

स्थिर शून्य
print_state (FILE *बहिर्गमनम्, पूर्णाङ्क level, state *s)
{
  fputc ('\n', बहिर्गमनम्);
  xml_printf (बहिर्गमनम्, level, "<state number=\"%d\">", s->सङ्ख्या);
  print_core (बहिर्गमनम्, level + 1, s);
  print_actions (बहिर्गमनम्, level + 1, s);
  यदि (s->solved_conflicts_xml)
    {
      xml_puts (बहिर्गमनम्, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, बहिर्गमनम्);
      xml_puts (बहिर्गमनम्, level + 1, "</solved-conflicts>");
    }
  अन्यथा
    xml_puts (बहिर्गमनम्, level + 1, "<solved-conflicts/>");
  xml_puts (बहिर्गमनम्, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

स्थिर शून्य
print_grammar (FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  fputc ('\n', बहिर्गमनम्);
  xml_puts (बहिर्गमनम्, level, "<grammar>");
  grammar_नियमाः_print_xml (बहिर्गमनम्, level);

  /* Terminals */
  xml_puts (बहिर्गमनम्, level + 1, "<terminals>");
  प्रति (पूर्णाङ्क i = 0; i < max_संहिता + 1; i++)
    यदि (token_translations[i] != undeftoken->content->सङ्ख्या)
      {
        चिह्नम् ध्रुव *sym = symbols[token_translations[i]];
        वर्ण ध्रुव *चिह्नकम् = sym->चिह्नकम्;
        वर्ण ध्रुव *type = sym->content->type_name;
        पूर्णाङ्क precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (बहिर्गमनम्, level + 2);
        fprintf (बहिर्गमनम्,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, चिह्नकम्),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_अप्रयुक्तम्_अन्तर्_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        यदि (precedence)
          fprintf (बहिर्गमनम्, " prec=\"%d\"", precedence);
        यदि (associativity != undef_assoc)
          fprintf (बहिर्गमनम्, " assoc=\"%s\"", assoc_गन्तव्यम्_string (associativity) + 1);
        fputs ("/>\n", बहिर्गमनम्);
      }
  xml_puts (बहिर्गमनम्, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (बहिर्गमनम्, level + 1, "<nonterminals>");
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      चिह्नम् ध्रुव *sym = symbols[i];
      वर्ण ध्रुव *चिह्नकम् = sym->चिह्नकम्;
      वर्ण ध्रुव *type = sym->content->type_name;
      xml_printf (बहिर्गमनम्, level + 2,
                  "<nonterminal symbol-number=\"%d\" name=\"%s\""
                  " type=\"%s\""
                  " usefulness=\"%s\"/>",
                  i, xml_escape_n (0, चिह्नकम्),
                  type ? xml_escape_n (1, type) : "",
                  reduce_nonterminal_useless_अन्तर्_grammar (sym->content)
                    ? "useless-in-grammar" : "useful");
    }
  xml_puts (बहिर्गमनम्, level + 1, "</nonterminals>");
  xml_puts (बहिर्गमनम्, level, "</grammar>");
}

शून्य
xml_indent (FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  प्रति (पूर्णाङ्क i = 0; i < level; i++)
    fputs ("  ", बहिर्गमनम्);
}

शून्य
xml_puts (FILE *बहिर्गमनम्, पूर्णाङ्क level, वर्ण ध्रुव *s)
{
  xml_indent (बहिर्गमनम्, level);
  fputs (s, बहिर्गमनम्);
  fputc ('\n', बहिर्गमनम्);
}

शून्य
xml_printf (FILE *बहिर्गमनम्, पूर्णाङ्क level, वर्ण ध्रुव *fmt, ...)
{
  va_list arglist;

  xml_indent (बहिर्गमनम्, level);

  va_start (arglist, fmt);
  vfprintf (बहिर्गमनम्, fmt, arglist);
  va_अन्तः (arglist);

  fputc ('\n', बहिर्गमनम्);
}

स्थिर वर्ण ध्रुव *
xml_escape_string (संरचना escape_buf *buf, वर्ण ध्रुव *str)
{
  size_t len = वाक्यदैर्घ्यम् (str);
  size_t max_expansion = परिमाण "&quot;" - 1;

  यदि (buf->आकार <= max_expansion * len)
    {
      buf->आकार = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->आकार);
    }
  वर्ण *p = buf->ptr;

  प्रति (; *str; str++)
    विकल्प (*str)
      {
      स्वभाव: *p++ = *str; भङ्ग;
      पक्ष '&': p = stpcpy (p, "&amp;" ); भङ्ग;
      पक्ष '<': p = stpcpy (p, "&lt;"  ); भङ्ग;
      पक्ष '>': p = stpcpy (p, "&gt;"  ); भङ्ग;
      पक्ष '"': p = stpcpy (p, "&quot;"); भङ्ग;
      }

  *p = '\0';
  प्रत्यागम buf->ptr;
}

वर्ण ध्रुव *
xml_escape_n (पूर्णाङ्क n, वर्ण ध्रुव *str)
{
  प्रत्यागम xml_escape_string (escape_bufs + n, str);
}

वर्ण ध्रुव *
xml_escape (वर्ण ध्रुव *str)
{
  प्रत्यागम xml_escape_n (0, str);
}

शून्य
print_xml (शून्य)
{
  FILE *बहिर्गमनम् = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", बहिर्गमनम्);

  पूर्णाङ्क level = 0;
  xml_printf (बहिर्गमनम्, level,
              "<bison-xml-report version=\"%s\" bug-report=\"%s\""
              " url=\"%s\">",
              xml_escape_n (0, संस्करणम्_2),
              xml_escape_n (1, PACKAGE_BUGREPORT),
              xml_escape_n (2, PACKAGE_URL));

  fputc ('\n', बहिर्गमनम्);
  xml_printf (बहिर्गमनम्, level + 1, "<filename>%s</filename>",
              xml_escape (grammar_file));

  /* print grammar */
  print_grammar (बहिर्गमनम्, level + 1);

  no_reduce_समुच्चयः = bitset_create (ntokens, BITSET_FIXED);

  /* print automaton */
  fputc ('\n', बहिर्गमनम्);
  xml_puts (बहिर्गमनम्, level + 1, "<automaton>");
  प्रति (state_सङ्ख्या i = 0; i < nstates; i++)
    print_state (बहिर्गमनम्, level + 2, अवस्थाः[i]);
  xml_puts (बहिर्गमनम्, level + 1, "</automaton>");

  bitset_free (no_reduce_समुच्चयः);

  xml_puts (बहिर्गमनम्, 0, "</bison-xml-report>");

  प्रति (पूर्णाङ्क i = 0; i < अङ्कः_escape_bufs; ++i)
    मुक्त (escape_bufs[i].ptr);

  xfclose (बहिर्गमनम्);
}


शून्य
print_html (शून्य)
{
  प्रतिज्ञा (xml_flag);

  वर्ण *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  वर्ण *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  वर्ण ध्रुव *argv[11];
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
      fputs ("running:", stderr);
      प्रति (पूर्णाङ्क j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  पूर्णाङ्क स्थितिः
    = execute (argv[0],
               argv[0], argv,
               /* directory */ रिक्त,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ रिक्त);
  यदि (स्थितिः)
    complain (रिक्त, complaint, _("%s failed with status %d"), argv[0], स्थितिः);
  मुक्त (xsltproc);
  मुक्त (xml2html);
}
