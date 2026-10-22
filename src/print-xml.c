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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "print-xml.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "execute.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "path-join.h"
#ಅನ್ತರ್ಭಾವ "print.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "tables.h"

ಸ್ಥಿರ bitset no_reduce_set;
ಸಂರಚನಾ escape_buf
{
  ವರ್ಣ *ptr;
  size_t size;
};
ಗಣನಾ { ಅಂಕೆ_escape_bufs = 3 };
ಸ್ಥಿರ ಸಂರಚನಾ escape_buf escape_bufs[ಅಂಕೆ_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_core (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, state *s)
{
  item_index *sitems = s->ವಸ್ತುಗಳು;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  ಯದಿ (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      ಪ್ರತ್ಯಾಗಮ;
    }

  xml_puts (out, level, "<itemset>");

  ಪ್ರತಿ (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_ಸಂಖ್ಯೆ *sp1 = ritem + sitems[i];
      rule ಧ್ರುವ *r = item_rule (sp1);
      item_ಸಂಖ್ಯೆ *sp = r->rhs;

      /* Display the lookahead tokens?  */
      ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*sp1))
        {
          reductions *reds = s->reductions;
          ಪೂರ್ಣಾಙ್ಕ red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          ಯದಿ (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->ಸಂಖ್ಯೆ, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      ಯದಿ (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->ಸಂಖ್ಯೆ,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_transitions (state *s, FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  transitions *trans = s->transitions;
  ಪೂರ್ಣಾಙ್ಕ n = 0;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಪ್ರತಿ (i = 0; i < trans->ಅಂಕೆ; i++)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  ಯದಿ (!n)
    {
      xml_puts (out, level, "<transitions/>");
      ಪ್ರತ್ಯಾಗಮ;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  ಪ್ರತಿ (i = 0; i < trans->ಅಂಕೆ; i++)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        ವರ್ಣ ಧ್ರುವ *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->ಸಂಖ್ಯೆ);
      }

  ಪ್ರತಿ (i = 0; i < trans->ಅಂಕೆ; i++)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        ವರ್ಣ ಧ್ರುವ *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->ಸಂಖ್ಯೆ);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_errs (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, state *s)
{
  errs *errp = s->errs;
  bool ಎಣಿಕೆ = false;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಪ್ರತಿ (i = 0; i < errp->ಅಂಕೆ; ++i)
    ಯದಿ (errp->symbols[i])
      ಎಣಿಕೆ = true;

  /* Nothing to report. */
  ಯದಿ (!ಎಣಿಕೆ)
    {
      xml_puts (out, level, "<errors/>");
      ಪ್ರತ್ಯಾಗಮ;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  ಪ್ರತಿ (i = 0; i < errp->ಅಂಕೆ; ++i)
    ಯದಿ (errp->symbols[i])
      {
        ವರ್ಣ ಧ್ರುವ *tag = errp->symbols[i]->tag;
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

ಸ್ಥಿರ ಶೂನ್ಯ
print_reduction (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, ವರ್ಣ ಧ್ರುವ *lookahead,
                 rule *r, bool enabled)
{
  ಯದಿ (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  ಅನ್ಯಥಾ
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->ಸಂಖ್ಯೆ,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reductions (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ report = false;
  ಪೂರ್ಣಾಙ್ಕ i, j;

  ಯದಿ (reds->ಅಂಕೆ == 0)
    {
      xml_puts (out, level, "<reductions/>");
      ಪ್ರತ್ಯಾಗಮ;
    }

  ಯದಿ (yydefact[s->ಸಂಖ್ಯೆ] != 0)
    default_reduction = &rules[yydefact[s->ಸಂಖ್ಯೆ] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  ಪ್ರತಿ (i = 0; i < s->errs->ಅಂಕೆ; ++i)
    ಯದಿ (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->ಸಂಖ್ಯೆ);

  ಯದಿ (default_reduction)
    report = true;

  ಯದಿ (reds->lookaheads)
    ಪ್ರತಿ (i = 0; i < ntokens; i++)
      {
        bool ಎಣಿಕೆ = bitset_test (no_reduce_set, i);

        ಪ್ರತಿ (j = 0; j < reds->ಅಂಕೆ; ++j)
          ಯದಿ (bitset_test (reds->lookaheads[j], i))
            {
              ಯದಿ (! ಎಣಿಕೆ)
                {
                  ಯದಿ (reds->rules[j] != default_reduction)
                    report = true;
                  ಎಣಿಕೆ = true;
                }
              ಅನ್ಯಥಾ
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  ಯದಿ (!report)
    {
      xml_puts (out, level, "<reductions/>");
      ಪ್ರತ್ಯಾಗಮ;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  ಯದಿ (reds->lookaheads)
    ಪ್ರತಿ (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ಎಣಿಕೆ = bitset_test (no_reduce_set, i);

        ಪ್ರತಿ (j = 0; j < reds->ಅಂಕೆ; ++j)
          ಯದಿ (bitset_test (reds->lookaheads[j], i))
            {
              ಯದಿ (! ಎಣಿಕೆ)
                {
                  ಯದಿ (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  ಅನ್ಯಥಾ
                    defaulted = true;
                  ಎಣಿಕೆ = true;
                }
              ಅನ್ಯಥಾ
                {
                  ಯದಿ (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  ಯದಿ (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_actions (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, state *s)
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

ಸ್ಥಿರ ಶೂನ್ಯ
print_state (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->ಸಂಖ್ಯೆ);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  ಯದಿ (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  ಅನ್ಯಥಾ
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_grammar (FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < max_code + 1; i++)
    ಯದಿ (token_translations[i] != undeftoken->content->ಸಂಖ್ಯೆ)
      {
        ಚಿಹ್ನೆ ಧ್ರುವ *sym = symbols[token_translations[i]];
        ವರ್ಣ ಧ್ರುವ *tag = sym->tag;
        ವರ್ಣ ಧ್ರುವ *type = sym->content->type_name;
        ಪೂರ್ಣಾಙ್ಕ precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        ಯದಿ (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        ಯದಿ (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      ಚಿಹ್ನೆ ಧ್ರುವ *sym = symbols[i];
      ವರ್ಣ ಧ್ರುವ *tag = sym->tag;
      ವರ್ಣ ಧ್ರುವ *type = sym->content->type_name;
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

ಶೂನ್ಯ
xml_indent (FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < level; i++)
    fputs ("  ", out);
}

ಶೂನ್ಯ
xml_puts (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, ವರ್ಣ ಧ್ರುವ *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

ಶೂನ್ಯ
xml_printf (FILE *out, ಪೂರ್ಣಾಙ್ಕ level, ವರ್ಣ ಧ್ರುವ *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
xml_escape_string (ಸಂರಚನಾ escape_buf *buf, ವರ್ಣ ಧ್ರುವ *str)
{
  size_t len = strlen (str);
  size_t max_expansion = ಪರಿಮಾಣ "&quot;" - 1;

  ಯದಿ (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  ವರ್ಣ *p = buf->ptr;

  ಪ್ರತಿ (; *str; str++)
    ವಿಕಲ್ಪ (*str)
      {
      ಸ್ವಭಾವ: *p++ = *str; ಭಙ್ಗ;
      ಪಕ್ಷ '&': p = stpcpy (p, "&amp;" ); ಭಙ್ಗ;
      ಪಕ್ಷ '<': p = stpcpy (p, "&lt;"  ); ಭಙ್ಗ;
      ಪಕ್ಷ '>': p = stpcpy (p, "&gt;"  ); ಭಙ್ಗ;
      ಪಕ್ಷ '"': p = stpcpy (p, "&quot;"); ಭಙ್ಗ;
      }

  *p = '\0';
  ಪ್ರತ್ಯಾಗಮ buf->ptr;
}

ವರ್ಣ ಧ್ರುವ *
xml_escape_n (ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ ಧ್ರುವ *str)
{
  ಪ್ರತ್ಯಾಗಮ xml_escape_string (escape_bufs + n, str);
}

ವರ್ಣ ಧ್ರುವ *
xml_escape (ವರ್ಣ ಧ್ರುವ *str)
{
  ಪ್ರತ್ಯಾಗಮ xml_escape_n (0, str);
}

ಶೂನ್ಯ
print_xml (ಶೂನ್ಯ)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  ಪೂರ್ಣಾಙ್ಕ level = 0;
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
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_escape_bufs; ++i)
    ಮುಕ್ತ (escape_bufs[i].ptr);

  xfclose (out);
}


ಶೂನ್ಯ
print_html (ಶೂನ್ಯ)
{
  ಪ್ರತಿಜ್ಞಾ (xml_flag);

  ವರ್ಣ *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  ವರ್ಣ *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  ವರ್ಣ ಧ್ರುವ *argv[11];
  ಪೂರ್ಣಾಙ್ಕ i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = ರಿಕ್ತ;
  aver (i <= ARRAY_CARDINALITY (argv));

  ಯದಿ (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  ಪೂರ್ಣಾಙ್ಕ status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ ರಿಕ್ತ,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ ರಿಕ್ತ);
  ಯದಿ (status)
    complain (ರಿಕ್ತ, complaint, _("%s failed with status %d"), argv[0], status);
  ಮುಕ್ತ (xsltproc);
  ಮುಕ್ತ (xml2html);
}
