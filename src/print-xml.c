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

ساكن bitset no_reduce_سيٽ;
بنية escape_buf
{
  حرف *ptr;
  size_t سائيز;
};
تعداد { انگ_escape_bufs = 3 };
ساكن بنية escape_buf escape_bufs[انگ_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *ٻاهريون, صحيح level, state *s)
{
  شي_اشاريو *sitems = s->شيون;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  گهيرو (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  إذا (!snritems)
    {
      xml_puts (ٻاهريون, level, "<itemset/>");
      أرجع;
    }

  xml_puts (ٻاهريون, level, "<itemset>");

  لكل (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      شي_عدد *sp1 = ritem + sitems[i];
      قاعدو ثابت *r = شي_قاعدو (sp1);
      شي_عدد *sp = r->rhs;

      /* Display the lookahead tokens?  */
      إذا (شي_عدد_is_قاعدو_عدد (*sp1))
        {
          reductions *reds = s->reductions;
          صحيح red = state_reduction_ڳوليو (s, r);
          /* Print item with lookaheads if there are. */
          إذا (reds->lookaheads && red != -1)
            {
              xml_printf (ٻاهريون, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->عدد, sp1 - sp);
              state_قاعدو_lookaheads_print_xml (s, r,
                                                     ٻاهريون, level + 2);
              xml_puts (ٻاهريون, level + 1, "</item>");
              printed = true;
            }
        }

      إذا (!printed)
        xml_printf (ٻاهريون, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->عدد,
                    sp1 - sp);
    }
  xml_puts (ٻاهريون, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

ساكن فراغ
print_transitions (state *s, FILE *ٻاهريون, صحيح level)
{
  transitions *trans = s->transitions;
  صحيح n = 0;
  صحيح i;

  لكل (i = 0; i < trans->انگ; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  إذا (!n)
    {
      xml_puts (ٻاهريون, level, "<transitions/>");
      أرجع;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (ٻاهريون, level, "<transitions>");

  لكل (i = 0; i < trans->انگ; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_سرڪايو (trans, i))
      {
        نشان *sym = symbols[TRANSITION_نشان (trans, i)];
        حرف ثابت *ٽيگ = sym->ٽيگ;
        state *s1 = trans->حالتون[i];

        xml_printf (ٻاهريون, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (ٽيگ), s1->عدد);
      }

  لكل (i = 0; i < trans->انگ; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_سرڪايو (trans, i))
      {
        نشان *sym = symbols[TRANSITION_نشان (trans, i)];
        حرف ثابت *ٽيگ = sym->ٽيگ;
        state *s1 = trans->حالتون[i];

        xml_printf (ٻاهريون, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (ٽيگ), s1->عدد);
      }

  xml_puts (ٻاهريون, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *ٻاهريون, صحيح level, state *s)
{
  errs *errp = s->errs;
  bool ڳڻپ = false;
  صحيح i;

  لكل (i = 0; i < errp->انگ; ++i)
    إذا (errp->symbols[i])
      ڳڻپ = true;

  /* Nothing to report. */
  إذا (!ڳڻپ)
    {
      xml_puts (ٻاهريون, level, "<errors/>");
      أرجع;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (ٻاهريون, level, "<errors>");
  لكل (i = 0; i < errp->انگ; ++i)
    إذا (errp->symbols[i])
      {
        حرف ثابت *ٽيگ = errp->symbols[i]->ٽيگ;
        xml_printf (ٻاهريون, level + 1,
                    "<error symbol=\"%s\">nonassociative</error>",
                    xml_escape (ٽيگ));
      }
  xml_puts (ٻاهريون, level, "</errors>");
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
print_reduction (FILE *ٻاهريون, صحيح level, حرف ثابت *lookahead,
                 قاعدو *r, bool enabled)
{
  إذا (قاعدو_is_initial (r))
    xml_printf (ٻاهريون, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  وإلا
    xml_printf (ٻاهريون, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->عدد,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *ٻاهريون, صحيح level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  قاعدو *default_reduction = عدم;
  صحيح report = false;
  صحيح i, j;

  إذا (reds->انگ == 0)
    {
      xml_puts (ٻاهريون, level, "<reductions/>");
      أرجع;
    }

  إذا (yydefact[s->عدد] != 0)
    default_reduction = &قاعدا[yydefact[s->عدد] - 1];

  bitset_zero (no_reduce_سيٽ);
  FOR_EACH_سرڪايو (trans, i)
    bitset_سيٽ (no_reduce_سيٽ, TRANSITION_نشان (trans, i));
  لكل (i = 0; i < s->errs->انگ; ++i)
    إذا (s->errs->symbols[i])
      bitset_سيٽ (no_reduce_سيٽ, s->errs->symbols[i]->content->عدد);

  إذا (default_reduction)
    report = true;

  إذا (reds->lookaheads)
    لكل (i = 0; i < ntokens; i++)
      {
        bool ڳڻپ = bitset_test (no_reduce_سيٽ, i);

        لكل (j = 0; j < reds->انگ; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! ڳڻپ)
                {
                  إذا (reds->قاعدا[j] != default_reduction)
                    report = true;
                  ڳڻپ = true;
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
      xml_puts (ٻاهريون, level, "<reductions/>");
      أرجع;
    }

  xml_puts (ٻاهريون, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  إذا (reds->lookaheads)
    لكل (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ڳڻپ = bitset_test (no_reduce_سيٽ, i);

        لكل (j = 0; j < reds->انگ; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! ڳڻپ)
                {
                  إذا (reds->قاعدا[j] != default_reduction)
                    print_reduction (ٻاهريون, level + 1, symbols[i]->ٽيگ,
                                     reds->قاعدا[j], true);
                  وإلا
                    defaulted = true;
                  ڳڻپ = true;
                }
              وإلا
                {
                  إذا (defaulted)
                    print_reduction (ٻاهريون, level + 1, symbols[i]->ٽيگ,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (ٻاهريون, level + 1, symbols[i]->ٽيگ,
                                   reds->قاعدا[j], false);
                }
            }
      }

  إذا (default_reduction)
    print_reduction (ٻاهريون, level + 1,
                     "$default", default_reduction, true);

  xml_puts (ٻاهريون, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *ٻاهريون, صحيح level, state *s)
{
  xml_puts (ٻاهريون, level, "<actions>");
  print_transitions (s, ٻاهريون, level + 1);
  print_errs (ٻاهريون, level + 1, s);
  print_reductions (ٻاهريون, level + 1, s);
  xml_puts (ٻاهريون, level, "</actions>");
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ساكن فراغ
print_state (FILE *ٻاهريون, صحيح level, state *s)
{
  fputc ('\n', ٻاهريون);
  xml_printf (ٻاهريون, level, "<state number=\"%d\">", s->عدد);
  print_core (ٻاهريون, level + 1, s);
  print_actions (ٻاهريون, level + 1, s);
  إذا (s->solved_conflicts_xml)
    {
      xml_puts (ٻاهريون, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, ٻاهريون);
      xml_puts (ٻاهريون, level + 1, "</solved-conflicts>");
    }
  وإلا
    xml_puts (ٻاهريون, level + 1, "<solved-conflicts/>");
  xml_puts (ٻاهريون, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ساكن فراغ
print_grammar (FILE *ٻاهريون, صحيح level)
{
  fputc ('\n', ٻاهريون);
  xml_puts (ٻاهريون, level, "<grammar>");
  grammar_قاعدا_print_xml (ٻاهريون, level);

  /* Terminals */
  xml_puts (ٻاهريون, level + 1, "<terminals>");
  لكل (صحيح i = 0; i < max_ڪوڊ + 1; i++)
    إذا (token_translations[i] != undeftoken->content->عدد)
      {
        نشان ثابت *sym = symbols[token_translations[i]];
        حرف ثابت *ٽيگ = sym->ٽيگ;
        حرف ثابت *type = sym->content->type_name;
        صحيح precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (ٻاهريون, level + 2);
        fprintf (ٻاهريون,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, ٽيگ),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_اڻ_استعمال_ٿيل_اندريون_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        إذا (precedence)
          fprintf (ٻاهريون, " prec=\"%d\"", precedence);
        إذا (associativity != undef_assoc)
          fprintf (ٻاهريون, " assoc=\"%s\"", assoc_منزل_string (associativity) + 1);
        fputs ("/>\n", ٻاهريون);
      }
  xml_puts (ٻاهريون, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (ٻاهريون, level + 1, "<nonterminals>");
  لكل (نشان_عدد i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      نشان ثابت *sym = symbols[i];
      حرف ثابت *ٽيگ = sym->ٽيگ;
      حرف ثابت *type = sym->content->type_name;
      xml_printf (ٻاهريون, level + 2,
                  "<nonterminal symbol-number=\"%d\" name=\"%s\""
                  " type=\"%s\""
                  " usefulness=\"%s\"/>",
                  i, xml_escape_n (0, ٽيگ),
                  type ? xml_escape_n (1, type) : "",
                  reduce_nonterminal_useless_اندريون_grammar (sym->content)
                    ? "useless-in-grammar" : "useful");
    }
  xml_puts (ٻاهريون, level + 1, "</nonterminals>");
  xml_puts (ٻاهريون, level, "</grammar>");
}

فراغ
xml_indent (FILE *ٻاهريون, صحيح level)
{
  لكل (صحيح i = 0; i < level; i++)
    fputs ("  ", ٻاهريون);
}

فراغ
xml_puts (FILE *ٻاهريون, صحيح level, حرف ثابت *s)
{
  xml_indent (ٻاهريون, level);
  fputs (s, ٻاهريون);
  fputc ('\n', ٻاهريون);
}

فراغ
xml_printf (FILE *ٻاهريون, صحيح level, حرف ثابت *fmt, ...)
{
  va_list arglist;

  xml_indent (ٻاهريون, level);

  va_شروعات (arglist, fmt);
  vfprintf (ٻاهريون, fmt, arglist);
  va_پڇاڙي (arglist);

  fputc ('\n', ٻاهريون);
}

ساكن حرف ثابت *
xml_escape_string (بنية escape_buf *buf, حرف ثابت *str)
{
  size_t len = لڙي_ڊيگهه (str);
  size_t max_expansion = حجم "&quot;" - 1;

  إذا (buf->سائيز <= max_expansion * len)
    {
      buf->سائيز = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->سائيز);
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
  FILE *ٻاهريون = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", ٻاهريون);

  صحيح level = 0;
  xml_printf (ٻاهريون, level,
              "<bison-xml-report version=\"%s\" bug-report=\"%s\""
              " url=\"%s\">",
              xml_escape_n (0, نسخو_2),
              xml_escape_n (1, PACKAGE_BUGREPORT),
              xml_escape_n (2, PACKAGE_URL));

  fputc ('\n', ٻاهريون);
  xml_printf (ٻاهريون, level + 1, "<filename>%s</filename>",
              xml_escape (grammar_file));

  /* print grammar */
  print_grammar (ٻاهريون, level + 1);

  no_reduce_سيٽ = bitset_create (ntokens, BITSET_FIXED);

  /* print automaton */
  fputc ('\n', ٻاهريون);
  xml_puts (ٻاهريون, level + 1, "<automaton>");
  لكل (state_عدد i = 0; i < nstates; i++)
    print_state (ٻاهريون, level + 2, حالتون[i]);
  xml_puts (ٻاهريون, level + 1, "</automaton>");

  bitset_free (no_reduce_سيٽ);

  xml_puts (ٻاهريون, 0, "</bison-xml-report>");

  لكل (صحيح i = 0; i < انگ_escape_bufs; ++i)
    حرر (escape_bufs[i].ptr);

  xfclose (ٻاهريون);
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
