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
गणना { अंक_escape_bufs = 3 };
स्थिर संरचना escape_buf escape_bufs[अंक_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (FILE *out, पूर्णाङ्क level, state *s)
{
  item_index *sitems = s->वस्तुएँ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  यदि (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      प्रत्यागम;
    }

  xml_puts (out, level, "<itemset>");

  प्रति (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_संख्या *sp1 = ritem + sitems[i];
      rule ध्रुव *r = item_rule (sp1);
      item_संख्या *sp = r->rhs;

      /* Display the lookahead tokens?  */
      यदि (item_संख्या_is_rule_संख्या (*sp1))
        {
          reductions *reds = s->reductions;
          पूर्णाङ्क red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          यदि (reds->lookaheads && red != -1)
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

      यदि (!printed)
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
print_transitions (state *s, FILE *out, पूर्णाङ्क level)
{
  transitions *trans = s->transitions;
  पूर्णाङ्क n = 0;
  पूर्णाङ्क i;

  प्रति (i = 0; i < trans->अंक; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  यदि (!n)
    {
      xml_puts (out, level, "<transitions/>");
      प्रत्यागम;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  प्रति (i = 0; i < trans->अंक; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        वर्ण ध्रुव *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->संख्या);
      }

  प्रति (i = 0; i < trans->अंक; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        वर्ण ध्रुव *tag = sym->tag;
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
print_errs (FILE *out, पूर्णाङ्क level, state *s)
{
  errs *errp = s->errs;
  bool गिनती = false;
  पूर्णाङ्क i;

  प्रति (i = 0; i < errp->अंक; ++i)
    यदि (errp->symbols[i])
      गिनती = true;

  /* Nothing to report. */
  यदि (!गिनती)
    {
      xml_puts (out, level, "<errors/>");
      प्रत्यागम;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  प्रति (i = 0; i < errp->अंक; ++i)
    यदि (errp->symbols[i])
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
  यदि (rule_is_initial (r))
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
print_reductions (FILE *out, पूर्णाङ्क level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = रिक्त;
  पूर्णाङ्क report = false;
  पूर्णाङ्क i, j;

  यदि (reds->अंक == 0)
    {
      xml_puts (out, level, "<reductions/>");
      प्रत्यागम;
    }

  यदि (yydefact[s->संख्या] != 0)
    default_reduction = &rules[yydefact[s->संख्या] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_प्रतीक (trans, i));
  प्रति (i = 0; i < s->errs->अंक; ++i)
    यदि (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->संख्या);

  यदि (default_reduction)
    report = true;

  यदि (reds->lookaheads)
    प्रति (i = 0; i < ntokens; i++)
      {
        bool गिनती = bitset_test (no_reduce_set, i);

        प्रति (j = 0; j < reds->अंक; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गिनती)
                {
                  यदि (reds->rules[j] != default_reduction)
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
  यदि (!report)
    {
      xml_puts (out, level, "<reductions/>");
      प्रत्यागम;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  यदि (reds->lookaheads)
    प्रति (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गिनती = bitset_test (no_reduce_set, i);

        प्रति (j = 0; j < reds->अंक; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गिनती)
                {
                  यदि (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  अन्यथा
                    defaulted = true;
                  गिनती = true;
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
  xml_printf (out, level, "<state number=\"%d\">", s->संख्या);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  यदि (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
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
print_grammar (FILE *out, पूर्णाङ्क level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  प्रति (पूर्णाङ्क i = 0; i < max_code + 1; i++)
    यदि (token_translations[i] != undeftoken->content->संख्या)
      {
        प्रतीक ध्रुव *sym = symbols[token_translations[i]];
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
        यदि (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        यदि (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  प्रति (प्रतीक_संख्या i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      प्रतीक ध्रुव *sym = symbols[i];
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
  प्रति (पूर्णाङ्क i = 0; i < level; i++)
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

  यदि (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
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
  प्रति (state_संख्या i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  प्रति (पूर्णाङ्क i = 0; i < अंक_escape_bufs; ++i)
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

  यदि (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      प्रति (पूर्णाङ्क j = 0; argv[j]; ++j)
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
  यदि (status)
    complain (रिक्त, complaint, _("%s failed with status %d"), argv[0], status);
  मुक्त (xsltproc);
  मुक्त (xml2html);
}
