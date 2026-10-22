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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "print-xml.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "conflicts.h"
#ଅନ୍ତର୍ଭାଵ "execute.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "path-join.h"
#ଅନ୍ତର୍ଭାଵ "print.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "state.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"
#ଅନ୍ତର୍ଭାଵ "tables.h"

ସ୍ଥିର bitset no_reduce_set;
ସଂରଚନା escape_buf
{
  ଅକ୍ଷର *ptr;
  size_t size;
};
ଗଣନା { ଅଙ୍କ_escape_bufs = 3 };
ସ୍ଥିର ସଂରଚନା escape_buf escape_bufs[ଅଙ୍କ_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_core (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, state *s)
{
  item_index *sitems = s->ବସ୍ତୁଗୁଡ଼ିକ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  ଯଦି (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      ଫେରନ୍ତୁ;
    }

  xml_puts (out, level, "<itemset>");

  ପାଇଁ (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_ସଂଖ୍ୟା *sp1 = ritem + sitems[i];
      rule ଅଚଳ *r = item_rule (sp1);
      item_ସଂଖ୍ୟା *sp = r->rhs;

      /* Display the lookahead tokens?  */
      ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*sp1))
        {
          reductions *reds = s->reductions;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          ଯଦି (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->ସଂଖ୍ୟା, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      ଯଦି (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->ସଂଖ୍ୟା,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_transitions (state *s, FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  transitions *trans = s->transitions;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ପାଇଁ (i = 0; i < trans->ଅଙ୍କ; i++)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  ଯଦି (!n)
    {
      xml_puts (out, level, "<transitions/>");
      ଫେରନ୍ତୁ;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  ପାଇଁ (i = 0; i < trans->ଅଙ୍କ; i++)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        ପ୍ରତୀକ *sym = symbols[TRANSITION_ପ୍ରତୀକ (trans, i)];
        ଅକ୍ଷର ଅଚଳ *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->ସଂଖ୍ୟା);
      }

  ପାଇଁ (i = 0; i < trans->ଅଙ୍କ; i++)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        ପ୍ରତୀକ *sym = symbols[TRANSITION_ପ୍ରତୀକ (trans, i)];
        ଅକ୍ଷର ଅଚଳ *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->ସଂଖ୍ୟା);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_errs (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, state *s)
{
  errs *errp = s->errs;
  bool ଗଣନା_2 = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ପାଇଁ (i = 0; i < errp->ଅଙ୍କ; ++i)
    ଯଦି (errp->symbols[i])
      ଗଣନା_2 = true;

  /* Nothing to report. */
  ଯଦି (!ଗଣନା_2)
    {
      xml_puts (out, level, "<errors/>");
      ଫେରନ୍ତୁ;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  ପାଇଁ (i = 0; i < errp->ଅଙ୍କ; ++i)
    ଯଦି (errp->symbols[i])
      {
        ଅକ୍ଷର ଅଚଳ *tag = errp->symbols[i]->tag;
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

ସ୍ଥିର ଶୂନ୍ୟ
print_reduction (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, ଅକ୍ଷର ଅଚଳ *lookahead,
                 rule *r, bool enabled)
{
  ଯଦି (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  ଅନ୍ୟଥା
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->ସଂଖ୍ୟା,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_reductions (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା report = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

  ଯଦି (reds->ଅଙ୍କ == 0)
    {
      xml_puts (out, level, "<reductions/>");
      ଫେରନ୍ତୁ;
    }

  ଯଦି (yydefact[s->ସଂଖ୍ୟା] != 0)
    default_reduction = &rules[yydefact[s->ସଂଖ୍ୟା] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_ପ୍ରତୀକ (trans, i));
  ପାଇଁ (i = 0; i < s->errs->ଅଙ୍କ; ++i)
    ଯଦି (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->ସଂଖ୍ୟା);

  ଯଦି (default_reduction)
    report = true;

  ଯଦି (reds->lookaheads)
    ପାଇଁ (i = 0; i < ntokens; i++)
      {
        bool ଗଣନା_2 = bitset_test (no_reduce_set, i);

        ପାଇଁ (j = 0; j < reds->ଅଙ୍କ; ++j)
          ଯଦି (bitset_test (reds->lookaheads[j], i))
            {
              ଯଦି (! ଗଣନା_2)
                {
                  ଯଦି (reds->rules[j] != default_reduction)
                    report = true;
                  ଗଣନା_2 = true;
                }
              ଅନ୍ୟଥା
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  ଯଦି (!report)
    {
      xml_puts (out, level, "<reductions/>");
      ଫେରନ୍ତୁ;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  ଯଦି (reds->lookaheads)
    ପାଇଁ (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ଗଣନା_2 = bitset_test (no_reduce_set, i);

        ପାଇଁ (j = 0; j < reds->ଅଙ୍କ; ++j)
          ଯଦି (bitset_test (reds->lookaheads[j], i))
            {
              ଯଦି (! ଗଣନା_2)
                {
                  ଯଦି (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  ଅନ୍ୟଥା
                    defaulted = true;
                  ଗଣନା_2 = true;
                }
              ଅନ୍ୟଥା
                {
                  ଯଦି (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  ଯଦି (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_actions (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, state *s)
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

ସ୍ଥିର ଶୂନ୍ୟ
print_state (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->ସଂଖ୍ୟା);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  ଯଦି (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  ଅନ୍ୟଥା
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_grammar (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < max_code + 1; i++)
    ଯଦି (token_translations[i] != undeftoken->content->ସଂଖ୍ୟା)
      {
        ପ୍ରତୀକ ଅଚଳ *sym = symbols[token_translations[i]];
        ଅକ୍ଷର ଅଚଳ *tag = sym->tag;
        ଅକ୍ଷର ଅଚଳ *type = sym->content->type_name;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        ଯଦି (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        ଯଦି (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      ପ୍ରତୀକ ଅଚଳ *sym = symbols[i];
      ଅକ୍ଷର ଅଚଳ *tag = sym->tag;
      ଅକ୍ଷର ଅଚଳ *type = sym->content->type_name;
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

ଶୂନ୍ୟ
xml_indent (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < level; i++)
    fputs ("  ", out);
}

ଶୂନ୍ୟ
xml_puts (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, ଅକ୍ଷର ଅଚଳ *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

ଶୂନ୍ୟ
xml_printf (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level, ଅକ୍ଷର ଅଚଳ *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
xml_escape_string (ସଂରଚନା escape_buf *buf, ଅକ୍ଷର ଅଚଳ *str)
{
  size_t len = strlen (str);
  size_t max_expansion = ଆକାର "&quot;" - 1;

  ଯଦି (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  ଅକ୍ଷର *p = buf->ptr;

  ପାଇଁ (; *str; str++)
    ବାଛ (*str)
      {
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: *p++ = *str; ଭାଙ୍ଗ;
      ସ୍ଥିତି '&': p = stpcpy (p, "&amp;" ); ଭାଙ୍ଗ;
      ସ୍ଥିତି '<': p = stpcpy (p, "&lt;"  ); ଭାଙ୍ଗ;
      ସ୍ଥିତି '>': p = stpcpy (p, "&gt;"  ); ଭାଙ୍ଗ;
      ସ୍ଥିତି '"': p = stpcpy (p, "&quot;"); ଭାଙ୍ଗ;
      }

  *p = '\0';
  ଫେରନ୍ତୁ buf->ptr;
}

ଅକ୍ଷର ଅଚଳ *
xml_escape_n (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର ଅଚଳ *str)
{
  ଫେରନ୍ତୁ xml_escape_string (escape_bufs + n, str);
}

ଅକ୍ଷର ଅଚଳ *
xml_escape (ଅକ୍ଷର ଅଚଳ *str)
{
  ଫେରନ୍ତୁ xml_escape_n (0, str);
}

ଶୂନ୍ୟ
print_xml (ଶୂନ୍ୟ)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା level = 0;
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
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_escape_bufs; ++i)
    ମୁକ୍ତ (escape_bufs[i].ptr);

  xfclose (out);
}


ଶୂନ୍ୟ
print_html (ଶୂନ୍ୟ)
{
  ପ୍ରତିଜ୍ଞା (xml_flag);

  ଅକ୍ଷର *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  ଅକ୍ଷର *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  ଅକ୍ଷର ଅଚଳ *argv[11];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = ଖାଲି;
  aver (i <= ARRAY_CARDINALITY (argv));

  ଯଦି (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ ଖାଲି,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ ଖାଲି);
  ଯଦି (status)
    complain (ଖାଲି, complaint, _("%s failed with status %d"), argv[0], status);
  ମୁକ୍ତ (xsltproc);
  ମୁକ୍ତ (xml2html);
}
