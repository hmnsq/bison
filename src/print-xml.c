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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "print-xml.h"

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>
#অন্তর্ভাব <stdarg.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "conflicts.h"
#অন্তর্ভাব "execute.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lr0.h"
#অন্তর্ভাব "muscle-tab.h"
#অন্তর্ভাব "path-join.h"
#অন্তর্ভাব "print.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "state.h"
#অন্তর্ভাব "symtab.h"
#অন্তর্ভাব "tables.h"

স্থির bitset no_reduce_set;
সংরচনা escape_buf
{
  বর্ণ *ptr;
  size_t size;
};
গণনা { অঙ্ক_escape_bufs = 3 };
স্থির সংরচনা escape_buf escape_bufs[অঙ্ক_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

স্থির শূন্য
print_core (FILE *out, পূর্ণাঙ্ক level, state *s)
{
  item_index *sitems = s->বস্তুসমূহ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  যদি (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      প্রত্যাগম;
    }

  xml_puts (out, level, "<itemset>");

  প্রতি (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_সংখ্যা *sp1 = ritem + sitems[i];
      rule ধ্রুব *r = item_rule (sp1);
      item_সংখ্যা *sp = r->rhs;

      /* Display the lookahead tokens?  */
      যদি (item_সংখ্যা_is_rule_সংখ্যা (*sp1))
        {
          reductions *reds = s->reductions;
          পূর্ণাঙ্ক red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          যদি (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->সংখ্যা, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      যদি (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->সংখ্যা,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

স্থির শূন্য
print_transitions (state *s, FILE *out, পূর্ণাঙ্ক level)
{
  transitions *trans = s->transitions;
  পূর্ণাঙ্ক n = 0;
  পূর্ণাঙ্ক i;

  প্রতি (i = 0; i < trans->অঙ্ক; i++)
    যদি (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  যদি (!n)
    {
      xml_puts (out, level, "<transitions/>");
      প্রত্যাগম;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  প্রতি (i = 0; i < trans->অঙ্ক; i++)
    যদি (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        প্রতীক *sym = symbols[TRANSITION_প্রতীক (trans, i)];
        বর্ণ ধ্রুব *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->সংখ্যা);
      }

  প্রতি (i = 0; i < trans->অঙ্ক; i++)
    যদি (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        প্রতীক *sym = symbols[TRANSITION_প্রতীক (trans, i)];
        বর্ণ ধ্রুব *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->সংখ্যা);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

স্থির শূন্য
print_errs (FILE *out, পূর্ণাঙ্ক level, state *s)
{
  errs *errp = s->errs;
  bool গুনতি = false;
  পূর্ণাঙ্ক i;

  প্রতি (i = 0; i < errp->অঙ্ক; ++i)
    যদি (errp->symbols[i])
      গুনতি = true;

  /* Nothing to report. */
  যদি (!গুনতি)
    {
      xml_puts (out, level, "<errors/>");
      প্রত্যাগম;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  প্রতি (i = 0; i < errp->অঙ্ক; ++i)
    যদি (errp->symbols[i])
      {
        বর্ণ ধ্রুব *tag = errp->symbols[i]->tag;
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

স্থির শূন্য
print_reduction (FILE *out, পূর্ণাঙ্ক level, বর্ণ ধ্রুব *lookahead,
                 rule *r, bool enabled)
{
  যদি (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  অন্যথা
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->সংখ্যা,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

স্থির শূন্য
print_reductions (FILE *out, পূর্ণাঙ্ক level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = রিক্ত;
  পূর্ণাঙ্ক report = false;
  পূর্ণাঙ্ক i, j;

  যদি (reds->অঙ্ক == 0)
    {
      xml_puts (out, level, "<reductions/>");
      প্রত্যাগম;
    }

  যদি (yydefact[s->সংখ্যা] != 0)
    default_reduction = &rules[yydefact[s->সংখ্যা] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_প্রতীক (trans, i));
  প্রতি (i = 0; i < s->errs->অঙ্ক; ++i)
    যদি (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->সংখ্যা);

  যদি (default_reduction)
    report = true;

  যদি (reds->lookaheads)
    প্রতি (i = 0; i < ntokens; i++)
      {
        bool গুনতি = bitset_test (no_reduce_set, i);

        প্রতি (j = 0; j < reds->অঙ্ক; ++j)
          যদি (bitset_test (reds->lookaheads[j], i))
            {
              যদি (! গুনতি)
                {
                  যদি (reds->rules[j] != default_reduction)
                    report = true;
                  গুনতি = true;
                }
              অন্যথা
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  যদি (!report)
    {
      xml_puts (out, level, "<reductions/>");
      প্রত্যাগম;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  যদি (reds->lookaheads)
    প্রতি (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool গুনতি = bitset_test (no_reduce_set, i);

        প্রতি (j = 0; j < reds->অঙ্ক; ++j)
          যদি (bitset_test (reds->lookaheads[j], i))
            {
              যদি (! গুনতি)
                {
                  যদি (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  অন্যথা
                    defaulted = true;
                  গুনতি = true;
                }
              অন্যথা
                {
                  যদি (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  যদি (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

স্থির শূন্য
print_actions (FILE *out, পূর্ণাঙ্ক level, state *s)
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

স্থির শূন্য
print_state (FILE *out, পূর্ণাঙ্ক level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->সংখ্যা);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  যদি (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  অন্যথা
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

স্থির শূন্য
print_grammar (FILE *out, পূর্ণাঙ্ক level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  প্রতি (পূর্ণাঙ্ক i = 0; i < max_code + 1; i++)
    যদি (token_translations[i] != undeftoken->content->সংখ্যা)
      {
        প্রতীক ধ্রুব *sym = symbols[token_translations[i]];
        বর্ণ ধ্রুব *tag = sym->tag;
        বর্ণ ধ্রুব *type = sym->content->type_name;
        পূর্ণাঙ্ক precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        যদি (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        যদি (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      প্রতীক ধ্রুব *sym = symbols[i];
      বর্ণ ধ্রুব *tag = sym->tag;
      বর্ণ ধ্রুব *type = sym->content->type_name;
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

শূন্য
xml_indent (FILE *out, পূর্ণাঙ্ক level)
{
  প্রতি (পূর্ণাঙ্ক i = 0; i < level; i++)
    fputs ("  ", out);
}

শূন্য
xml_puts (FILE *out, পূর্ণাঙ্ক level, বর্ণ ধ্রুব *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

শূন্য
xml_printf (FILE *out, পূর্ণাঙ্ক level, বর্ণ ধ্রুব *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

স্থির বর্ণ ধ্রুব *
xml_escape_string (সংরচনা escape_buf *buf, বর্ণ ধ্রুব *str)
{
  size_t len = strlen (str);
  size_t max_expansion = পরিমাণ "&quot;" - 1;

  যদি (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  বর্ণ *p = buf->ptr;

  প্রতি (; *str; str++)
    বিকল্প (*str)
      {
      স্বভাব: *p++ = *str; ভঙ্গ;
      পক্ষ '&': p = stpcpy (p, "&amp;" ); ভঙ্গ;
      পক্ষ '<': p = stpcpy (p, "&lt;"  ); ভঙ্গ;
      পক্ষ '>': p = stpcpy (p, "&gt;"  ); ভঙ্গ;
      পক্ষ '"': p = stpcpy (p, "&quot;"); ভঙ্গ;
      }

  *p = '\0';
  প্রত্যাগম buf->ptr;
}

বর্ণ ধ্রুব *
xml_escape_n (পূর্ণাঙ্ক n, বর্ণ ধ্রুব *str)
{
  প্রত্যাগম xml_escape_string (escape_bufs + n, str);
}

বর্ণ ধ্রুব *
xml_escape (বর্ণ ধ্রুব *str)
{
  প্রত্যাগম xml_escape_n (0, str);
}

শূন্য
print_xml (শূন্য)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  পূর্ণাঙ্ক level = 0;
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
  প্রতি (state_সংখ্যা i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_escape_bufs; ++i)
    মুক্ত (escape_bufs[i].ptr);

  xfclose (out);
}


শূন্য
print_html (শূন্য)
{
  প্রতিজ্ঞা (xml_flag);

  বর্ণ *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  বর্ণ *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  বর্ণ ধ্রুব *argv[11];
  পূর্ণাঙ্ক i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = রিক্ত;
  aver (i <= ARRAY_CARDINALITY (argv));

  যদি (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      প্রতি (পূর্ণাঙ্ক j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  পূর্ণাঙ্ক status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ রিক্ত,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ রিক্ত);
  যদি (status)
    complain (রিক্ত, complaint, _("%s failed with status %d"), argv[0], status);
  মুক্ত (xsltproc);
  মুক্ত (xml2html);
}
