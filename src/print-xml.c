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
  size_t size;
};
गणना { अङ्कः_escape_bufs = 3 };
स्थिर संरचना escape_buf escape_bufs[अङ्कः_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (FILE *out, पूर्णाङ्क level, state *s)
{
  item_index *sitems = s->वस्तूनि;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  सचे (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      पटिनिवत्त;
    }

  xml_puts (out, level, "<itemset>");

  अत्थाय (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_सङ्ख्या *sp1 = ritem + sitems[i];
      rule ध्रुव *r = item_rule (sp1);
      item_सङ्ख्या *sp = r->rhs;

      /* Display the lookahead tokens?  */
      सचे (item_सङ्ख्या_is_rule_सङ्ख्या (*sp1))
        {
          reductions *reds = s->reductions;
          पूर्णाङ्क red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          सचे (reds->lookaheads && red != -1)
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

      सचे (!printed)
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
print_transitions (state *s, FILE *out, पूर्णाङ्क level)
{
  transitions *trans = s->transitions;
  पूर्णाङ्क n = 0;
  पूर्णाङ्क i;

  अत्थाय (i = 0; i < trans->अङ्कः; i++)
    सचे (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  सचे (!n)
    {
      xml_puts (out, level, "<transitions/>");
      पटिनिवत्त;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  अत्थाय (i = 0; i < trans->अङ्कः; i++)
    सचे (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        वर्ण ध्रुव *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->सङ्ख्या);
      }

  अत्थाय (i = 0; i < trans->अङ्कः; i++)
    सचे (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        वर्ण ध्रुव *tag = sym->tag;
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
print_errs (FILE *out, पूर्णाङ्क level, state *s)
{
  errs *errp = s->errs;
  bool गणनम् = false;
  पूर्णाङ्क i;

  अत्थाय (i = 0; i < errp->अङ्कः; ++i)
    सचे (errp->symbols[i])
      गणनम् = true;

  /* Nothing to report. */
  सचे (!गणनम्)
    {
      xml_puts (out, level, "<errors/>");
      पटिनिवत्त;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  अत्थाय (i = 0; i < errp->अङ्कः; ++i)
    सचे (errp->symbols[i])
      {
        वर्ण ध्रुव *tag = errp->symbols[i]->tag;
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
print_reduction (FILE *out, पूर्णाङ्क level, वर्ण ध्रुव *lookahead,
                 rule *r, bool enabled)
{
  सचे (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  अञ्ञथा
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
print_reductions (FILE *out, पूर्णाङ्क level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = रिक्त;
  पूर्णाङ्क report = false;
  पूर्णाङ्क i, j;

  सचे (reds->अङ्कः == 0)
    {
      xml_puts (out, level, "<reductions/>");
      पटिनिवत्त;
    }

  सचे (yydefact[s->सङ्ख्या] != 0)
    default_reduction = &rules[yydefact[s->सङ्ख्या] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_चिह्नम् (trans, i));
  अत्थाय (i = 0; i < s->errs->अङ्कः; ++i)
    सचे (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->सङ्ख्या);

  सचे (default_reduction)
    report = true;

  सचे (reds->lookaheads)
    अत्थाय (i = 0; i < ntokens; i++)
      {
        bool गणनम् = bitset_test (no_reduce_set, i);

        अत्थाय (j = 0; j < reds->अङ्कः; ++j)
          सचे (bitset_test (reds->lookaheads[j], i))
            {
              सचे (! गणनम्)
                {
                  सचे (reds->rules[j] != default_reduction)
                    report = true;
                  गणनम् = true;
                }
              अञ्ञथा
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  सचे (!report)
    {
      xml_puts (out, level, "<reductions/>");
      पटिनिवत्त;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  सचे (reds->lookaheads)
    अत्थाय (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गणनम् = bitset_test (no_reduce_set, i);

        अत्थाय (j = 0; j < reds->अङ्कः; ++j)
          सचे (bitset_test (reds->lookaheads[j], i))
            {
              सचे (! गणनम्)
                {
                  सचे (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  अञ्ञथा
                    defaulted = true;
                  गणनम् = true;
                }
              अञ्ञथा
                {
                  सचे (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  सचे (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
print_actions (FILE *out, पूर्णाङ्क level, state *s)
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
print_state (FILE *out, पूर्णाङ्क level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->सङ्ख्या);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  सचे (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  अञ्ञथा
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

स्थिर शून्य
print_grammar (FILE *out, पूर्णाङ्क level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  अत्थाय (पूर्णाङ्क i = 0; i < max_code + 1; i++)
    सचे (token_translations[i] != undeftoken->content->सङ्ख्या)
      {
        चिह्नम् ध्रुव *sym = symbols[token_translations[i]];
        वर्ण ध्रुव *tag = sym->tag;
        वर्ण ध्रुव *type = sym->content->type_name;
        पूर्णाङ्क precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        सचे (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        सचे (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  अत्थाय (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      चिह्नम् ध्रुव *sym = symbols[i];
      वर्ण ध्रुव *tag = sym->tag;
      वर्ण ध्रुव *type = sym->content->type_name;
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
xml_indent (FILE *out, पूर्णाङ्क level)
{
  अत्थाय (पूर्णाङ्क i = 0; i < level; i++)
    fputs ("  ", out);
}

शून्य
xml_puts (FILE *out, पूर्णाङ्क level, वर्ण ध्रुव *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

शून्य
xml_printf (FILE *out, पूर्णाङ्क level, वर्ण ध्रुव *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

स्थिर वर्ण ध्रुव *
xml_escape_string (संरचना escape_buf *buf, वर्ण ध्रुव *str)
{
  size_t len = strlen (str);
  size_t max_expansion = परिमाण "&quot;" - 1;

  सचे (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  वर्ण *p = buf->ptr;

  अत्थाय (; *str; str++)
    विकल्प (*str)
      {
      स्वभाव: *p++ = *str; भङ्ग;
      पक्ष '&': p = stpcpy (p, "&amp;" ); भङ्ग;
      पक्ष '<': p = stpcpy (p, "&lt;"  ); भङ्ग;
      पक्ष '>': p = stpcpy (p, "&gt;"  ); भङ्ग;
      पक्ष '"': p = stpcpy (p, "&quot;"); भङ्ग;
      }

  *p = '\0';
  पटिनिवत्त buf->ptr;
}

वर्ण ध्रुव *
xml_escape_n (पूर्णाङ्क n, वर्ण ध्रुव *str)
{
  पटिनिवत्त xml_escape_string (escape_bufs + n, str);
}

वर्ण ध्रुव *
xml_escape (वर्ण ध्रुव *str)
{
  पटिनिवत्त xml_escape_n (0, str);
}

शून्य
print_xml (शून्य)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  पूर्णाङ्क level = 0;
  xml_printf (out, level,
              "<bison-xml-report version=\"%s\" bug-report=\"%s\""
              " url=\"%s\">",
              xml_escape_n (0, VERSION),
              xml_escape_n (1, PACKAGE_BUGREPORT),
              xml_escape_n (2, PACKAGE_URL));

  fputc ('\n', out);
  xml_printf (out, level + 1, "<filename>%s</filename>",
              xml_escape (grammar_file));

  /* print grammar */
  print_grammar (out, level + 1);

  no_reduce_set = bitset_create (ntokens, BITSET_FIXED);

  /* print automaton */
  fputc ('\n', out);
  xml_puts (out, level + 1, "<automaton>");
  अत्थाय (state_सङ्ख्या i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  अत्थाय (पूर्णाङ्क i = 0; i < अङ्कः_escape_bufs; ++i)
    मुक्त (escape_bufs[i].ptr);

  xfclose (out);
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

  सचे (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      अत्थाय (पूर्णाङ्क j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  पूर्णाङ्क status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ रिक्त,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ रिक्त);
  सचे (status)
    complain (रिक्त, complaint, _("%s failed with status %d"), argv[0], status);
  मुक्त (xsltproc);
  मुक्त (xml2html);
}
