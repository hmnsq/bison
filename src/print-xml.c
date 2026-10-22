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

#ضمن <config.h>
#ضمن "print-xml.h"

#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <stdarg.h>

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "execute.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "muscle-tab.h"
#ضمن "path-join.h"
#ضمن "print.h"
#ضمن "reader.h"
#ضمن "reduce.h"
#ضمن "state.h"
#ضمن "symtab.h"
#ضمن "tables.h"

ساكن bitset no_reduce_set;
بنية escape_buf
{
  حرف *ptr;
  size_t size;
};
تعداد { num_escape_bufs = 3 };
ساكن بنية escape_buf escape_bufs[num_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *out, صحيح level, state *s)
{
  item_index *sitems = s->items;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  𐡄𐡍 (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      أرجع;
    }

  xml_puts (out, level, "<itemset>");

  لكل (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_number *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);
      item_number *sp = r->rhs;

      /* Display the lookahead tokens?  */
      𐡄𐡍 (item_number_is_rule_number (*sp1))
        {
          reductions *reds = s->reductions;
          صحيح red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          𐡄𐡍 (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->number, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      𐡄𐡍 (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->number,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ساكن فراغ
print_transitions (state *s, FILE *out, صحيح level)
{
  transitions *trans = s->transitions;
  صحيح n = 0;
  صحيح i;

  لكل (i = 0; i < trans->num; i++)
    𐡄𐡍 (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  𐡄𐡍 (!n)
    {
      xml_puts (out, level, "<transitions/>");
      أرجع;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  لكل (i = 0; i < trans->num; i++)
    𐡄𐡍 (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        symbol *sym = symbols[TRANSITION_SYMBOL (trans, i)];
        حرف ثابت *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->number);
      }

  لكل (i = 0; i < trans->num; i++)
    𐡄𐡍 (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        symbol *sym = symbols[TRANSITION_SYMBOL (trans, i)];
        حرف ثابت *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->number);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *out, صحيح level, state *s)
{
  errs *errp = s->errs;
  bool count = false;
  صحيح i;

  لكل (i = 0; i < errp->num; ++i)
    𐡄𐡍 (errp->symbols[i])
      count = true;

  /* Nothing to report. */
  𐡄𐡍 (!count)
    {
      xml_puts (out, level, "<errors/>");
      أرجع;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  لكل (i = 0; i < errp->num; ++i)
    𐡄𐡍 (errp->symbols[i])
      {
        حرف ثابت *tag = errp->symbols[i]->tag;
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

ساكن فراغ
print_reduction (FILE *out, صحيح level, حرف ثابت *lookahead,
                 rule *r, bool enabled)
{
  𐡄𐡍 (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  وإلا
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->number,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *out, صحيح level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = عدم;
  صحيح report = false;
  صحيح i, j;

  𐡄𐡍 (reds->num == 0)
    {
      xml_puts (out, level, "<reductions/>");
      أرجع;
    }

  𐡄𐡍 (yydefact[s->number] != 0)
    default_reduction = &rules[yydefact[s->number] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_SYMBOL (trans, i));
  لكل (i = 0; i < s->errs->num; ++i)
    𐡄𐡍 (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->number);

  𐡄𐡍 (default_reduction)
    report = true;

  𐡄𐡍 (reds->lookaheads)
    لكل (i = 0; i < ntokens; i++)
      {
        bool count = bitset_test (no_reduce_set, i);

        لكل (j = 0; j < reds->num; ++j)
          𐡄𐡍 (bitset_test (reds->lookaheads[j], i))
            {
              𐡄𐡍 (! count)
                {
                  𐡄𐡍 (reds->rules[j] != default_reduction)
                    report = true;
                  count = true;
                }
              وإلا
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  𐡄𐡍 (!report)
    {
      xml_puts (out, level, "<reductions/>");
      أرجع;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  𐡄𐡍 (reds->lookaheads)
    لكل (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool count = bitset_test (no_reduce_set, i);

        لكل (j = 0; j < reds->num; ++j)
          𐡄𐡍 (bitset_test (reds->lookaheads[j], i))
            {
              𐡄𐡍 (! count)
                {
                  𐡄𐡍 (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  وإلا
                    defaulted = true;
                  count = true;
                }
              وإلا
                {
                  𐡄𐡍 (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  𐡄𐡍 (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *out, صحيح level, state *s)
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

ساكن فراغ
print_state (FILE *out, صحيح level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->number);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  𐡄𐡍 (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  وإلا
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ساكن فراغ
print_grammar (FILE *out, صحيح level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  لكل (صحيح i = 0; i < max_code + 1; i++)
    𐡄𐡍 (token_translations[i] != undeftoken->content->number)
      {
        symbol ثابت *sym = symbols[token_translations[i]];
        حرف ثابت *tag = sym->tag;
        حرف ثابت *type = sym->content->type_name;
        صحيح precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        𐡄𐡍 (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        𐡄𐡍 (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  لكل (symbol_number i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      symbol ثابت *sym = symbols[i];
      حرف ثابت *tag = sym->tag;
      حرف ثابت *type = sym->content->type_name;
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

فراغ
xml_indent (FILE *out, صحيح level)
{
  لكل (صحيح i = 0; i < level; i++)
    fputs ("  ", out);
}

فراغ
xml_puts (FILE *out, صحيح level, حرف ثابت *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

فراغ
xml_printf (FILE *out, صحيح level, حرف ثابت *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

ساكن حرف ثابت *
xml_escape_string (بنية escape_buf *buf, حرف ثابت *str)
{
  size_t len = strlen (str);
  size_t max_expansion = حجم "&quot;" - 1;

  𐡄𐡍 (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  حرف *p = buf->ptr;

  لكل (; *str; str++)
    بدل (*str)
      {
      افتراضي: *p++ = *str; اكسر;
      حالة '&': p = stpcpy (p, "&amp;" ); اكسر;
      حالة '<': p = stpcpy (p, "&lt;"  ); اكسر;
      حالة '>': p = stpcpy (p, "&gt;"  ); اكسر;
      حالة '"': p = stpcpy (p, "&quot;"); اكسر;
      }

  *p = '\0';
  أرجع buf->ptr;
}

حرف ثابت *
xml_escape_n (صحيح n, حرف ثابت *str)
{
  أرجع xml_escape_string (escape_bufs + n, str);
}

حرف ثابت *
xml_escape (حرف ثابت *str)
{
  أرجع xml_escape_n (0, str);
}

فراغ
print_xml (فراغ)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  صحيح level = 0;
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
  لكل (state_number i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  لكل (صحيح i = 0; i < num_escape_bufs; ++i)
    حرر (escape_bufs[i].ptr);

  xfclose (out);
}


فراغ
print_html (فراغ)
{
  أكد (xml_flag);

  حرف *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  حرف *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  حرف ثابت *argv[11];
  صحيح i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = عدم;
  aver (i <= ARRAY_CARDINALITY (argv));

  𐡄𐡍 (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      لكل (صحيح j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  صحيح status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ عدم,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ عدم);
  𐡄𐡍 (status)
    complain (عدم, complaint, _("%s failed with status %d"), argv[0], status);
  حرر (xsltproc);
  حرر (xml2html);
}
