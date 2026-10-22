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

#include <config.h>
#include "print-xml.h"

#include "system.h"

#include <bitset.h>
#include <stdarg.h>

#include "closure.h"
#include "complain.h"
#include "conflicts.h"
#include "execute.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "lalr.h"
#include "lr0.h"
#include "muscle-tab.h"
#include "path-join.h"
#include "print.h"
#include "reader.h"
#include "reduce.h"
#include "state.h"
#include "symtab.h"
#include "tables.h"

static bitset no_reduce_set;
sa_nracanaa escape_buf
{
  var_na *ptr;
  size_t size;
};
ga_nanaa { num_escape_bufs = 3 };
static sa_nracanaa escape_buf escape_bufs[num_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

static void
print_core (FILE *out, ཧྲིལ level, state *s)
{
  item_index *sitems = s->items;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  གལ་སྲིད (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      ལོག;
    }

  xml_puts (out, level, "<itemset>");

  དོན་ལུ (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_number *sp1 = ritem + sitems[i];
      rule const *r = item_rule (sp1);
      item_number *sp = r->rhs;

      /* Display the lookahead tokens?  */
      གལ་སྲིད (item_number_is_rule_number (*sp1))
        {
          reductions *reds = s->reductions;
          ཧྲིལ red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          གལ་སྲིད (reds->lookaheads && red != -1)
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

      གལ་སྲིད (!printed)
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

static void
print_transitions (state *s, FILE *out, ཧྲིལ level)
{
  transitions *trans = s->transitions;
  ཧྲིལ n = 0;
  ཧྲིལ i;

  དོན་ལུ (i = 0; i < trans->num; i++)
    གལ་སྲིད (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  གལ་སྲིད (!n)
    {
      xml_puts (out, level, "<transitions/>");
      ལོག;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  དོན་ལུ (i = 0; i < trans->num; i++)
    གལ་སྲིད (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        symbol *sym = symbols[TRANSITION_SYMBOL (trans, i)];
        var_na const *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->number);
      }

  དོན་ལུ (i = 0; i < trans->num; i++)
    གལ་སྲིད (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        symbol *sym = symbols[TRANSITION_SYMBOL (trans, i)];
        var_na const *tag = sym->tag;
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

static void
print_errs (FILE *out, ཧྲིལ level, state *s)
{
  errs *errp = s->errs;
  bool count = false;
  ཧྲིལ i;

  དོན་ལུ (i = 0; i < errp->num; ++i)
    གལ་སྲིད (errp->symbols[i])
      count = true;

  /* Nothing to report. */
  གལ་སྲིད (!count)
    {
      xml_puts (out, level, "<errors/>");
      ལོག;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  དོན་ལུ (i = 0; i < errp->num; ++i)
    གལ་སྲིད (errp->symbols[i])
      {
        var_na const *tag = errp->symbols[i]->tag;
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

static void
print_reduction (FILE *out, ཧྲིལ level, var_na const *lookahead,
                 rule *r, bool enabled)
{
  གལ་སྲིད (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  དེ་མིན
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->number,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

static void
print_reductions (FILE *out, ཧྲིལ level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = NULL;
  ཧྲིལ report = false;
  ཧྲིལ i, j;

  གལ་སྲིད (reds->num == 0)
    {
      xml_puts (out, level, "<reductions/>");
      ལོག;
    }

  གལ་སྲིད (yydefact[s->number] != 0)
    default_reduction = &rules[yydefact[s->number] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_SYMBOL (trans, i));
  དོན་ལུ (i = 0; i < s->errs->num; ++i)
    གལ་སྲིད (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->number);

  གལ་སྲིད (default_reduction)
    report = true;

  གལ་སྲིད (reds->lookaheads)
    དོན་ལུ (i = 0; i < ntokens; i++)
      {
        bool count = bitset_test (no_reduce_set, i);

        དོན་ལུ (j = 0; j < reds->num; ++j)
          གལ་སྲིད (bitset_test (reds->lookaheads[j], i))
            {
              གལ་སྲིད (! count)
                {
                  གལ་སྲིད (reds->rules[j] != default_reduction)
                    report = true;
                  count = true;
                }
              དེ་མིན
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  གལ་སྲིད (!report)
    {
      xml_puts (out, level, "<reductions/>");
      ལོག;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  གལ་སྲིད (reds->lookaheads)
    དོན་ལུ (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool count = bitset_test (no_reduce_set, i);

        དོན་ལུ (j = 0; j < reds->num; ++j)
          གལ་སྲིད (bitset_test (reds->lookaheads[j], i))
            {
              གལ་སྲིད (! count)
                {
                  གལ་སྲིད (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  དེ་མིན
                    defaulted = true;
                  count = true;
                }
              དེ་མིན
                {
                  གལ་སྲིད (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  གལ་སྲིད (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

static void
print_actions (FILE *out, ཧྲིལ level, state *s)
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

static void
print_state (FILE *out, ཧྲིལ level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->number);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  གལ་སྲིད (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  དེ་མིན
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

static void
print_grammar (FILE *out, ཧྲིལ level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  དོན་ལུ (ཧྲིལ i = 0; i < max_code + 1; i++)
    གལ་སྲིད (token_translations[i] != undeftoken->content->number)
      {
        symbol const *sym = symbols[token_translations[i]];
        var_na const *tag = sym->tag;
        var_na const *type = sym->content->type_name;
        ཧྲིལ precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        གལ་སྲིད (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        གལ་སྲིད (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  དོན་ལུ (symbol_number i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      symbol const *sym = symbols[i];
      var_na const *tag = sym->tag;
      var_na const *type = sym->content->type_name;
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

void
xml_indent (FILE *out, ཧྲིལ level)
{
  དོན་ལུ (ཧྲིལ i = 0; i < level; i++)
    fputs ("  ", out);
}

void
xml_puts (FILE *out, ཧྲིལ level, var_na const *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

void
xml_printf (FILE *out, ཧྲིལ level, var_na const *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

static var_na const *
xml_escape_string (sa_nracanaa escape_buf *buf, var_na const *str)
{
  size_t len = strlen (str);
  size_t max_expansion = parimaa_na "&quot;" - 1;

  གལ་སྲིད (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  var_na *p = buf->ptr;

  དོན་ལུ (; *str; str++)
    switch (*str)
      {
      default: *p++ = *str; bha_nga;
      pak_sa '&': p = stpcpy (p, "&amp;" ); bha_nga;
      pak_sa '<': p = stpcpy (p, "&lt;"  ); bha_nga;
      pak_sa '>': p = stpcpy (p, "&gt;"  ); bha_nga;
      pak_sa '"': p = stpcpy (p, "&quot;"); bha_nga;
      }

  *p = '\0';
  ལོག buf->ptr;
}

var_na const *
xml_escape_n (ཧྲིལ n, var_na const *str)
{
  ལོག xml_escape_string (escape_bufs + n, str);
}

var_na const *
xml_escape (var_na const *str)
{
  ལོག xml_escape_n (0, str);
}

void
print_xml (void)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  ཧྲིལ level = 0;
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
  དོན་ལུ (state_number i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  དོན་ལུ (ཧྲིལ i = 0; i < num_escape_bufs; ++i)
    free (escape_bufs[i].ptr);

  xfclose (out);
}


void
print_html (void)
{
  assert (xml_flag);

  var_na *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  var_na *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  var_na const *argv[11];
  ཧྲིལ i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = NULL;
  aver (i <= ARRAY_CARDINALITY (argv));

  གལ་སྲིད (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      དོན་ལུ (ཧྲིལ j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  ཧྲིལ status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ NULL,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ NULL);
  གལ་སྲིད (status)
    complain (NULL, complaint, _("%s failed with status %d"), argv[0], status);
  free (xsltproc);
  free (xml2html);
}
