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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "print-xml.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>
#അന്തര്ഭാവ <stdarg.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "execute.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "path-join.h"
#അന്തര്ഭാവ "print.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "state.h"
#അന്തര്ഭാവ "symtab.h"
#അന്തര്ഭാവ "tables.h"

സ്ഥിതം bitset no_reduce_set;
ഘടന escape_buf
{
  അക്ഷരം *ptr;
  size_t size;
};
എണ്ണൽ { അക്കം_escape_bufs = 3 };
സ്ഥിതം ഘടന escape_buf escape_bufs[അക്കം_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

സ്ഥിതം ശൂന്യം
print_core (FILE *out, പൂർണ്ണസംഖ്യ level, state *s)
{
  item_index *sitems = s->ഇനങ്ങൾ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  എങ്കിൽ (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      തിരികെ;
    }

  xml_puts (out, level, "<itemset>");

  വേണ്ടി (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_സംഖ്യ *sp1 = ritem + sitems[i];
      rule സ്ഥിരം *r = item_rule (sp1);
      item_സംഖ്യ *sp = r->rhs;

      /* Display the lookahead tokens?  */
      എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (*sp1))
        {
          reductions *reds = s->reductions;
          പൂർണ്ണസംഖ്യ red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          എങ്കിൽ (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->സംഖ്യ, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      എങ്കിൽ (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->സംഖ്യ,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_transitions (state *s, FILE *out, പൂർണ്ണസംഖ്യ level)
{
  transitions *trans = s->transitions;
  പൂർണ്ണസംഖ്യ n = 0;
  പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < trans->അക്കം; i++)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  എങ്കിൽ (!n)
    {
      xml_puts (out, level, "<transitions/>");
      തിരികെ;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  വേണ്ടി (i = 0; i < trans->അക്കം; i++)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        ചിഹ്നം *sym = symbols[TRANSITION_ചിഹ്നം (trans, i)];
        അക്ഷരം സ്ഥിരം *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->സംഖ്യ);
      }

  വേണ്ടി (i = 0; i < trans->അക്കം; i++)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        ചിഹ്നം *sym = symbols[TRANSITION_ചിഹ്നം (trans, i)];
        അക്ഷരം സ്ഥിരം *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->സംഖ്യ);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_errs (FILE *out, പൂർണ്ണസംഖ്യ level, state *s)
{
  errs *errp = s->errs;
  bool എണ്ണം = false;
  പൂർണ്ണസംഖ്യ i;

  വേണ്ടി (i = 0; i < errp->അക്കം; ++i)
    എങ്കിൽ (errp->symbols[i])
      എണ്ണം = true;

  /* Nothing to report. */
  എങ്കിൽ (!എണ്ണം)
    {
      xml_puts (out, level, "<errors/>");
      തിരികെ;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  വേണ്ടി (i = 0; i < errp->അക്കം; ++i)
    എങ്കിൽ (errp->symbols[i])
      {
        അക്ഷരം സ്ഥിരം *tag = errp->symbols[i]->tag;
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

സ്ഥിതം ശൂന്യം
print_reduction (FILE *out, പൂർണ്ണസംഖ്യ level, അക്ഷരം സ്ഥിരം *lookahead,
                 rule *r, bool enabled)
{
  എങ്കിൽ (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  അല്ലെങ്കിൽ
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->സംഖ്യ,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_reductions (FILE *out, പൂർണ്ണസംഖ്യ level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = ശൂന്യ;
  പൂർണ്ണസംഖ്യ report = false;
  പൂർണ്ണസംഖ്യ i, j;

  എങ്കിൽ (reds->അക്കം == 0)
    {
      xml_puts (out, level, "<reductions/>");
      തിരികെ;
    }

  എങ്കിൽ (yydefact[s->സംഖ്യ] != 0)
    default_reduction = &rules[yydefact[s->സംഖ്യ] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_ചിഹ്നം (trans, i));
  വേണ്ടി (i = 0; i < s->errs->അക്കം; ++i)
    എങ്കിൽ (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->സംഖ്യ);

  എങ്കിൽ (default_reduction)
    report = true;

  എങ്കിൽ (reds->lookaheads)
    വേണ്ടി (i = 0; i < ntokens; i++)
      {
        bool എണ്ണം = bitset_test (no_reduce_set, i);

        വേണ്ടി (j = 0; j < reds->അക്കം; ++j)
          എങ്കിൽ (bitset_test (reds->lookaheads[j], i))
            {
              എങ്കിൽ (! എണ്ണം)
                {
                  എങ്കിൽ (reds->rules[j] != default_reduction)
                    report = true;
                  എണ്ണം = true;
                }
              അല്ലെങ്കിൽ
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  എങ്കിൽ (!report)
    {
      xml_puts (out, level, "<reductions/>");
      തിരികെ;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  എങ്കിൽ (reds->lookaheads)
    വേണ്ടി (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool എണ്ണം = bitset_test (no_reduce_set, i);

        വേണ്ടി (j = 0; j < reds->അക്കം; ++j)
          എങ്കിൽ (bitset_test (reds->lookaheads[j], i))
            {
              എങ്കിൽ (! എണ്ണം)
                {
                  എങ്കിൽ (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  അല്ലെങ്കിൽ
                    defaulted = true;
                  എണ്ണം = true;
                }
              അല്ലെങ്കിൽ
                {
                  എങ്കിൽ (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  എങ്കിൽ (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_actions (FILE *out, പൂർണ്ണസംഖ്യ level, state *s)
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

സ്ഥിതം ശൂന്യം
print_state (FILE *out, പൂർണ്ണസംഖ്യ level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->സംഖ്യ);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  എങ്കിൽ (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  അല്ലെങ്കിൽ
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

സ്ഥിതം ശൂന്യം
print_grammar (FILE *out, പൂർണ്ണസംഖ്യ level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < max_code + 1; i++)
    എങ്കിൽ (token_translations[i] != undeftoken->content->സംഖ്യ)
      {
        ചിഹ്നം സ്ഥിരം *sym = symbols[token_translations[i]];
        അക്ഷരം സ്ഥിരം *tag = sym->tag;
        അക്ഷരം സ്ഥിരം *type = sym->content->type_name;
        പൂർണ്ണസംഖ്യ precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        എങ്കിൽ (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        എങ്കിൽ (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      ചിഹ്നം സ്ഥിരം *sym = symbols[i];
      അക്ഷരം സ്ഥിരം *tag = sym->tag;
      അക്ഷരം സ്ഥിരം *type = sym->content->type_name;
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

ശൂന്യം
xml_indent (FILE *out, പൂർണ്ണസംഖ്യ level)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < level; i++)
    fputs ("  ", out);
}

ശൂന്യം
xml_puts (FILE *out, പൂർണ്ണസംഖ്യ level, അക്ഷരം സ്ഥിരം *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

ശൂന്യം
xml_printf (FILE *out, പൂർണ്ണസംഖ്യ level, അക്ഷരം സ്ഥിരം *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

സ്ഥിതം അക്ഷരം സ്ഥിരം *
xml_escape_string (ഘടന escape_buf *buf, അക്ഷരം സ്ഥിരം *str)
{
  size_t len = strlen (str);
  size_t max_expansion = വലുപ്പം "&quot;" - 1;

  എങ്കിൽ (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  അക്ഷരം *p = buf->ptr;

  വേണ്ടി (; *str; str++)
    തിരഞ്ഞെടുപ്പ് (*str)
      {
      സ്ഥിരസ്ഥിതി: *p++ = *str; നിർത്തുക;
      അവസ്ഥ '&': p = stpcpy (p, "&amp;" ); നിർത്തുക;
      അവസ്ഥ '<': p = stpcpy (p, "&lt;"  ); നിർത്തുക;
      അവസ്ഥ '>': p = stpcpy (p, "&gt;"  ); നിർത്തുക;
      അവസ്ഥ '"': p = stpcpy (p, "&quot;"); നിർത്തുക;
      }

  *p = '\0';
  തിരികെ buf->ptr;
}

അക്ഷരം സ്ഥിരം *
xml_escape_n (പൂർണ്ണസംഖ്യ n, അക്ഷരം സ്ഥിരം *str)
{
  തിരികെ xml_escape_string (escape_bufs + n, str);
}

അക്ഷരം സ്ഥിരം *
xml_escape (അക്ഷരം സ്ഥിരം *str)
{
  തിരികെ xml_escape_n (0, str);
}

ശൂന്യം
print_xml (ശൂന്യം)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  പൂർണ്ണസംഖ്യ level = 0;
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
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_escape_bufs; ++i)
    സ്വതന്ത്രം (escape_bufs[i].ptr);

  xfclose (out);
}


ശൂന്യം
print_html (ശൂന്യം)
{
  പ്രതിജ്ഞാ (xml_flag);

  അക്ഷരം *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  അക്ഷരം *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  അക്ഷരം സ്ഥിരം *argv[11];
  പൂർണ്ണസംഖ്യ i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = ശൂന്യ;
  aver (i <= ARRAY_CARDINALITY (argv));

  എങ്കിൽ (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  പൂർണ്ണസംഖ്യ status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ ശൂന്യ,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ ശൂന്യ);
  എങ്കിൽ (status)
    complain (ശൂന്യ, complaint, _("%s failed with status %d"), argv[0], status);
  സ്വതന്ത്രം (xsltproc);
  സ്വതന്ത്രം (xml2html);
}
