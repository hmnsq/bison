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

ساكن bitset no_reduce_سېټ;
بنية escape_buf
{
  حرف *ptr;
  size_t اندازه;
};
تعداد { عدد_escape_bufs = 3 };
ساكن بنية escape_buf escape_bufs[عدد_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *بهر, صحيح level, state *s)
{
  توکی_شاخص *sitems = s->توکي;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  تړون (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  إذا (!snritems)
    {
      xml_puts (بهر, level, "<itemset/>");
      أرجع;
    }

  xml_puts (بهر, level, "<itemset>");

  لكل (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      توکی_شمېره *sp1 = ritem + sitems[i];
      قاعده ثابت *r = توکی_قاعده (sp1);
      توکی_شمېره *sp = r->rhs;

      /* Display the lookahead tokens?  */
      إذا (توکی_شمېره_is_قاعده_شمېره (*sp1))
        {
          reductions *reds = s->reductions;
          صحيح red = state_reduction_پیدا_کړه (s, r);
          /* Print item with lookaheads if there are. */
          إذا (reds->lookaheads && red != -1)
            {
              xml_printf (بهر, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->شمېره, sp1 - sp);
              state_قاعده_lookaheads_print_xml (s, r,
                                                     بهر, level + 2);
              xml_puts (بهر, level + 1, "</item>");
              printed = true;
            }
        }

      إذا (!printed)
        xml_printf (بهر, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->شمېره,
                    sp1 - sp);
    }
  xml_puts (بهر, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ساكن فراغ
print_transitions (state *s, FILE *بهر, صحيح level)
{
  transitions *trans = s->transitions;
  صحيح n = 0;
  صحيح i;

  لكل (i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  إذا (!n)
    {
      xml_puts (بهر, level, "<transitions/>");
      أرجع;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (بهر, level, "<transitions>");

  لكل (i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_ښويدنه (trans, i))
      {
        نښه *sym = symbols[TRANSITION_نښه (trans, i)];
        حرف ثابت *نښان = sym->نښان;
        state *s1 = trans->حالتونه[i];

        xml_printf (بهر, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (نښان), s1->شمېره);
      }

  لكل (i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_ښويدنه (trans, i))
      {
        نښه *sym = symbols[TRANSITION_نښه (trans, i)];
        حرف ثابت *نښان = sym->نښان;
        state *s1 = trans->حالتونه[i];

        xml_printf (بهر, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (نښان), s1->شمېره);
      }

  xml_puts (بهر, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *بهر, صحيح level, state *s)
{
  errs *errp = s->errs;
  bool شمېر = false;
  صحيح i;

  لكل (i = 0; i < errp->عدد; ++i)
    إذا (errp->symbols[i])
      شمېر = true;

  /* Nothing to report. */
  إذا (!شمېر)
    {
      xml_puts (بهر, level, "<errors/>");
      أرجع;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (بهر, level, "<errors>");
  لكل (i = 0; i < errp->عدد; ++i)
    إذا (errp->symbols[i])
      {
        حرف ثابت *نښان = errp->symbols[i]->نښان;
        xml_printf (بهر, level + 1,
                    "<error symbol=\"%s\">nonassociative</error>",
                    xml_escape (نښان));
      }
  xml_puts (بهر, level, "</errors>");
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
print_reduction (FILE *بهر, صحيح level, حرف ثابت *lookahead,
                 قاعده *r, bool enabled)
{
  إذا (قاعده_is_initial (r))
    xml_printf (بهر, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  وإلا
    xml_printf (بهر, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->شمېره,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *بهر, صحيح level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  قاعده *default_reduction = عدم;
  صحيح report = false;
  صحيح i, j;

  إذا (reds->عدد == 0)
    {
      xml_puts (بهر, level, "<reductions/>");
      أرجع;
    }

  إذا (yydefact[s->شمېره] != 0)
    default_reduction = &قواعد[yydefact[s->شمېره] - 1];

  bitset_zero (no_reduce_سېټ);
  FOR_EACH_ښويدنه (trans, i)
    bitset_سېټ (no_reduce_سېټ, TRANSITION_نښه (trans, i));
  لكل (i = 0; i < s->errs->عدد; ++i)
    إذا (s->errs->symbols[i])
      bitset_سېټ (no_reduce_سېټ, s->errs->symbols[i]->content->شمېره);

  إذا (default_reduction)
    report = true;

  إذا (reds->lookaheads)
    لكل (i = 0; i < ntokens; i++)
      {
        bool شمېر = bitset_test (no_reduce_سېټ, i);

        لكل (j = 0; j < reds->عدد; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! شمېر)
                {
                  إذا (reds->قواعد[j] != default_reduction)
                    report = true;
                  شمېر = true;
                }
              وإلا
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  إذا (!report)
    {
      xml_puts (بهر, level, "<reductions/>");
      أرجع;
    }

  xml_puts (بهر, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  إذا (reds->lookaheads)
    لكل (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool شمېر = bitset_test (no_reduce_سېټ, i);

        لكل (j = 0; j < reds->عدد; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! شمېر)
                {
                  إذا (reds->قواعد[j] != default_reduction)
                    print_reduction (بهر, level + 1, symbols[i]->نښان,
                                     reds->قواعد[j], true);
                  وإلا
                    defaulted = true;
                  شمېر = true;
                }
              وإلا
                {
                  إذا (defaulted)
                    print_reduction (بهر, level + 1, symbols[i]->نښان,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (بهر, level + 1, symbols[i]->نښان,
                                   reds->قواعد[j], false);
                }
            }
      }

  إذا (default_reduction)
    print_reduction (بهر, level + 1,
                     "$default", default_reduction, true);

  xml_puts (بهر, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *بهر, صحيح level, state *s)
{
  xml_puts (بهر, level, "<actions>");
  print_transitions (s, بهر, level + 1);
  print_errs (بهر, level + 1, s);
  print_reductions (بهر, level + 1, s);
  xml_puts (بهر, level, "</actions>");
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ساكن فراغ
print_state (FILE *بهر, صحيح level, state *s)
{
  fputc ('\n', بهر);
  xml_printf (بهر, level, "<state number=\"%d\">", s->شمېره);
  print_core (بهر, level + 1, s);
  print_actions (بهر, level + 1, s);
  إذا (s->solved_conflicts_xml)
    {
      xml_puts (بهر, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, بهر);
      xml_puts (بهر, level + 1, "</solved-conflicts>");
    }
  وإلا
    xml_puts (بهر, level + 1, "<solved-conflicts/>");
  xml_puts (بهر, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ساكن فراغ
print_grammar (FILE *بهر, صحيح level)
{
  fputc ('\n', بهر);
  xml_puts (بهر, level, "<grammar>");
  grammar_قواعد_print_xml (بهر, level);

  /* Terminals */
  xml_puts (بهر, level + 1, "<terminals>");
  لكل (صحيح i = 0; i < max_کوډ + 1; i++)
    إذا (token_translations[i] != undeftoken->content->شمېره)
      {
        نښه ثابت *sym = symbols[token_translations[i]];
        حرف ثابت *نښان = sym->نښان;
        حرف ثابت *type = sym->content->type_name;
        صحيح precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (بهر, level + 2);
        fprintf (بهر,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, نښان),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_ناکارول_شوی_دننه_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        إذا (precedence)
          fprintf (بهر, " prec=\"%d\"", precedence);
        إذا (associativity != undef_assoc)
          fprintf (بهر, " assoc=\"%s\"", assoc_منزل_string (associativity) + 1);
        fputs ("/>\n", بهر);
      }
  xml_puts (بهر, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (بهر, level + 1, "<nonterminals>");
  لكل (نښه_شمېره i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      نښه ثابت *sym = symbols[i];
      حرف ثابت *نښان = sym->نښان;
      حرف ثابت *type = sym->content->type_name;
      xml_printf (بهر, level + 2,
                  "<nonterminal symbol-number=\"%d\" name=\"%s\""
                  " type=\"%s\""
                  " usefulness=\"%s\"/>",
                  i, xml_escape_n (0, نښان),
                  type ? xml_escape_n (1, type) : "",
                  reduce_nonterminal_useless_دننه_grammar (sym->content)
                    ? "useless-in-grammar" : "useful");
    }
  xml_puts (بهر, level + 1, "</nonterminals>");
  xml_puts (بهر, level, "</grammar>");
}

فراغ
xml_indent (FILE *بهر, صحيح level)
{
  لكل (صحيح i = 0; i < level; i++)
    fputs ("  ", بهر);
}

فراغ
xml_puts (FILE *بهر, صحيح level, حرف ثابت *s)
{
  xml_indent (بهر, level);
  fputs (s, بهر);
  fputc ('\n', بهر);
}

فراغ
xml_printf (FILE *بهر, صحيح level, حرف ثابت *fmt, ...)
{
  va_list arglist;

  xml_indent (بهر, level);

  va_پیل (arglist, fmt);
  vfprintf (بهر, fmt, arglist);
  va_پای_ټکی (arglist);

  fputc ('\n', بهر);
}

ساكن حرف ثابت *
xml_escape_string (بنية escape_buf *buf, حرف ثابت *str)
{
  size_t len = مزي_اوږدوالی (str);
  size_t max_expansion = حجم "&quot;" - 1;

  إذا (buf->اندازه <= max_expansion * len)
    {
      buf->اندازه = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->اندازه);
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
  FILE *بهر = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", بهر);

  صحيح level = 0;
  xml_printf (بهر, level,
              "<bison-xml-report version=\"%s\" bug-report=\"%s\""
              " url=\"%s\">",
              xml_escape_n (0, نسخه_2),
              xml_escape_n (1, PACKAGE_BUGREPORT),
              xml_escape_n (2, PACKAGE_URL));

  fputc ('\n', بهر);
  xml_printf (بهر, level + 1, "<filename>%s</filename>",
              xml_escape (grammar_file));

  /* print grammar */
  print_grammar (بهر, level + 1);

  no_reduce_سېټ = bitset_create (ntokens, BITSET_FIXED);

  /* print automaton */
  fputc ('\n', بهر);
  xml_puts (بهر, level + 1, "<automaton>");
  لكل (state_شمېره i = 0; i < nstates; i++)
    print_state (بهر, level + 2, حالتونه[i]);
  xml_puts (بهر, level + 1, "</automaton>");

  bitset_free (no_reduce_سېټ);

  xml_puts (بهر, 0, "</bison-xml-report>");

  لكل (صحيح i = 0; i < عدد_escape_bufs; ++i)
    حرر (escape_bufs[i].ptr);

  xfclose (بهر);
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

  إذا (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      لكل (صحيح j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  صحيح حالت
    = execute (argv[0],
               argv[0], argv,
               /* directory */ عدم,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ عدم);
  إذا (حالت)
    complain (عدم, complaint, _("%s failed with status %d"), argv[0], حالت);
  حرر (xsltproc);
  حرر (xml2html);
}
