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

#شامل <config.h>
#شامل "print-xml.h"

#شامل "system.h"

#شامل <bitset.h>
#شامل <stdarg.h>

#شامل "closure.h"
#شامل "complain.h"
#شامل "conflicts.h"
#شامل "execute.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "muscle-tab.h"
#شامل "path-join.h"
#شامل "print.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "state.h"
#شامل "symtab.h"
#شامل "tables.h"

ایستا bitset no_reduce_set;
ساختار escape_buf
{
  نویسه *ptr;
  size_t size;
};
شمارش { رقم_escape_bufs = 3 };
ایستا ساختار escape_buf escape_bufs[رقم_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ایستا تهی
print_core (FILE *out, صحیح level, state *s)
{
  item_index *sitems = s->اقلام;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  اگر (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      بازگشت;
    }

  xml_puts (out, level, "<itemset>");

  برای (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_عدد *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);
      item_عدد *sp = r->rhs;

      /* Display the lookahead tokens?  */
      اگر (item_عدد_is_rule_عدد (*sp1))
        {
          reductions *reds = s->reductions;
          صحیح red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          اگر (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->عدد, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      اگر (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->عدد,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ایستا تهی
print_transitions (state *s, FILE *out, صحیح level)
{
  transitions *trans = s->transitions;
  صحیح n = 0;
  صحیح i;

  برای (i = 0; i < trans->رقم; i++)
    اگر (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  اگر (!n)
    {
      xml_puts (out, level, "<transitions/>");
      بازگشت;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  برای (i = 0; i < trans->رقم; i++)
    اگر (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        نماد *sym = symbols[TRANSITION_نماد (trans, i)];
        نویسه ثابت *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->عدد);
      }

  برای (i = 0; i < trans->رقم; i++)
    اگر (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        نماد *sym = symbols[TRANSITION_نماد (trans, i)];
        نویسه ثابت *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->عدد);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ایستا تهی
print_errs (FILE *out, صحیح level, state *s)
{
  errs *errp = s->errs;
  bool شمار = false;
  صحیح i;

  برای (i = 0; i < errp->رقم; ++i)
    اگر (errp->symbols[i])
      شمار = true;

  /* Nothing to report. */
  اگر (!شمار)
    {
      xml_puts (out, level, "<errors/>");
      بازگشت;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  برای (i = 0; i < errp->رقم; ++i)
    اگر (errp->symbols[i])
      {
        نویسه ثابت *tag = errp->symbols[i]->tag;
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

ایستا تهی
print_reduction (FILE *out, صحیح level, نویسه ثابت *lookahead,
                 rule *r, bool enabled)
{
  اگر (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  وگرنه
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->عدد,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ایستا تهی
print_reductions (FILE *out, صحیح level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = پوچ;
  صحیح report = false;
  صحیح i, j;

  اگر (reds->رقم == 0)
    {
      xml_puts (out, level, "<reductions/>");
      بازگشت;
    }

  اگر (yydefact[s->عدد] != 0)
    default_reduction = &rules[yydefact[s->عدد] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_نماد (trans, i));
  برای (i = 0; i < s->errs->رقم; ++i)
    اگر (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->عدد);

  اگر (default_reduction)
    report = true;

  اگر (reds->lookaheads)
    برای (i = 0; i < ntokens; i++)
      {
        bool شمار = bitset_test (no_reduce_set, i);

        برای (j = 0; j < reds->رقم; ++j)
          اگر (bitset_test (reds->lookaheads[j], i))
            {
              اگر (! شمار)
                {
                  اگر (reds->rules[j] != default_reduction)
                    report = true;
                  شمار = true;
                }
              وگرنه
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  اگر (!report)
    {
      xml_puts (out, level, "<reductions/>");
      بازگشت;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  اگر (reds->lookaheads)
    برای (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool شمار = bitset_test (no_reduce_set, i);

        برای (j = 0; j < reds->رقم; ++j)
          اگر (bitset_test (reds->lookaheads[j], i))
            {
              اگر (! شمار)
                {
                  اگر (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  وگرنه
                    defaulted = true;
                  شمار = true;
                }
              وگرنه
                {
                  اگر (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  اگر (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ایستا تهی
print_actions (FILE *out, صحیح level, state *s)
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

ایستا تهی
print_state (FILE *out, صحیح level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->عدد);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  اگر (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  وگرنه
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ایستا تهی
print_grammar (FILE *out, صحیح level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  برای (صحیح i = 0; i < max_code + 1; i++)
    اگر (token_translations[i] != undeftoken->content->عدد)
      {
        نماد ثابت *sym = symbols[token_translations[i]];
        نویسه ثابت *tag = sym->tag;
        نویسه ثابت *type = sym->content->type_name;
        صحیح precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        اگر (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        اگر (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  برای (نماد_عدد i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      نماد ثابت *sym = symbols[i];
      نویسه ثابت *tag = sym->tag;
      نویسه ثابت *type = sym->content->type_name;
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

تهی
xml_indent (FILE *out, صحیح level)
{
  برای (صحیح i = 0; i < level; i++)
    fputs ("  ", out);
}

تهی
xml_puts (FILE *out, صحیح level, نویسه ثابت *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

تهی
xml_printf (FILE *out, صحیح level, نویسه ثابت *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

ایستا نویسه ثابت *
xml_escape_string (ساختار escape_buf *buf, نویسه ثابت *str)
{
  size_t len = strlen (str);
  size_t max_expansion = اندازه "&quot;" - 1;

  اگر (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  نویسه *p = buf->ptr;

  برای (; *str; str++)
    انتخاب (*str)
      {
      پیشفرض: *p++ = *str; بشکن;
      حالت '&': p = stpcpy (p, "&amp;" ); بشکن;
      حالت '<': p = stpcpy (p, "&lt;"  ); بشکن;
      حالت '>': p = stpcpy (p, "&gt;"  ); بشکن;
      حالت '"': p = stpcpy (p, "&quot;"); بشکن;
      }

  *p = '\0';
  بازگشت buf->ptr;
}

نویسه ثابت *
xml_escape_n (صحیح n, نویسه ثابت *str)
{
  بازگشت xml_escape_string (escape_bufs + n, str);
}

نویسه ثابت *
xml_escape (نویسه ثابت *str)
{
  بازگشت xml_escape_n (0, str);
}

تهی
print_xml (تهی)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  صحیح level = 0;
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
  برای (state_عدد i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  برای (صحیح i = 0; i < رقم_escape_bufs; ++i)
    آزاد (escape_bufs[i].ptr);

  xfclose (out);
}


تهی
print_html (تهی)
{
  أكد (xml_flag);

  نویسه *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  نویسه *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  نویسه ثابت *argv[11];
  صحیح i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = پوچ;
  aver (i <= ARRAY_CARDINALITY (argv));

  اگر (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      برای (صحیح j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  صحیح status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ پوچ,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ پوچ);
  اگر (status)
    complain (پوچ, complaint, _("%s failed with status %d"), argv[0], status);
  آزاد (xsltproc);
  آزاد (xml2html);
}
