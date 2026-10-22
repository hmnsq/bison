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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "print-xml.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>
#અન્તર્ભાવ <stdarg.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "execute.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "path-join.h"
#અન્તર્ભાવ "print.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "state.h"
#અન્તર્ભાવ "symtab.h"
#અન્તર્ભાવ "tables.h"

સ્થિર bitset no_reduce_set;
સંરચના escape_buf
{
  અક્ષર *ptr;
  size_t size;
};
ગણના { અંક_escape_bufs = 3 };
સ્થિર સંરચના escape_buf escape_bufs[અંક_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

સ્થિર શૂન્ય
print_core (FILE *out, પૂર્ણાંક level, state *s)
{
  item_index *sitems = s->વસ્તુઓ;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  જો (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      પાછા;
    }

  xml_puts (out, level, "<itemset>");

  માટે (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_સંખ્યા *sp1 = ritem + sitems[i];
      rule અચલ *r = item_rule (sp1);
      item_સંખ્યા *sp = r->rhs;

      /* Display the lookahead tokens?  */
      જો (item_સંખ્યા_is_rule_સંખ્યા (*sp1))
        {
          reductions *reds = s->reductions;
          પૂર્ણાંક red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          જો (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->સંખ્યા, sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      જો (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->સંખ્યા,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

સ્થિર શૂન્ય
print_transitions (state *s, FILE *out, પૂર્ણાંક level)
{
  transitions *trans = s->transitions;
  પૂર્ણાંક n = 0;
  પૂર્ણાંક i;

  માટે (i = 0; i < trans->અંક; i++)
    જો (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  જો (!n)
    {
      xml_puts (out, level, "<transitions/>");
      પાછા;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  માટે (i = 0; i < trans->અંક; i++)
    જો (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        પ્રતીક *sym = symbols[TRANSITION_પ્રતીક (trans, i)];
        અક્ષર અચલ *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->સંખ્યા);
      }

  માટે (i = 0; i < trans->અંક; i++)
    જો (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        પ્રતીક *sym = symbols[TRANSITION_પ્રતીક (trans, i)];
        અક્ષર અચલ *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->સંખ્યા);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

સ્થિર શૂન્ય
print_errs (FILE *out, પૂર્ણાંક level, state *s)
{
  errs *errp = s->errs;
  bool ગણતરી = false;
  પૂર્ણાંક i;

  માટે (i = 0; i < errp->અંક; ++i)
    જો (errp->symbols[i])
      ગણતરી = true;

  /* Nothing to report. */
  જો (!ગણતરી)
    {
      xml_puts (out, level, "<errors/>");
      પાછા;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  માટે (i = 0; i < errp->અંક; ++i)
    જો (errp->symbols[i])
      {
        અક્ષર અચલ *tag = errp->symbols[i]->tag;
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

સ્થિર શૂન્ય
print_reduction (FILE *out, પૂર્ણાંક level, અક્ષર અચલ *lookahead,
                 rule *r, bool enabled)
{
  જો (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  નહીંતર
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->સંખ્યા,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

સ્થિર શૂન્ય
print_reductions (FILE *out, પૂર્ણાંક level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = ખાલી;
  પૂર્ણાંક report = false;
  પૂર્ણાંક i, j;

  જો (reds->અંક == 0)
    {
      xml_puts (out, level, "<reductions/>");
      પાછા;
    }

  જો (yydefact[s->સંખ્યા] != 0)
    default_reduction = &rules[yydefact[s->સંખ્યા] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_પ્રતીક (trans, i));
  માટે (i = 0; i < s->errs->અંક; ++i)
    જો (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->સંખ્યા);

  જો (default_reduction)
    report = true;

  જો (reds->lookaheads)
    માટે (i = 0; i < ntokens; i++)
      {
        bool ગણતરી = bitset_test (no_reduce_set, i);

        માટે (j = 0; j < reds->અંક; ++j)
          જો (bitset_test (reds->lookaheads[j], i))
            {
              જો (! ગણતરી)
                {
                  જો (reds->rules[j] != default_reduction)
                    report = true;
                  ગણતરી = true;
                }
              નહીંતર
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  જો (!report)
    {
      xml_puts (out, level, "<reductions/>");
      પાછા;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  જો (reds->lookaheads)
    માટે (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ગણતરી = bitset_test (no_reduce_set, i);

        માટે (j = 0; j < reds->અંક; ++j)
          જો (bitset_test (reds->lookaheads[j], i))
            {
              જો (! ગણતરી)
                {
                  જો (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  નહીંતર
                    defaulted = true;
                  ગણતરી = true;
                }
              નહીંતર
                {
                  જો (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  જો (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

સ્થિર શૂન્ય
print_actions (FILE *out, પૂર્ણાંક level, state *s)
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

સ્થિર શૂન્ય
print_state (FILE *out, પૂર્ણાંક level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->સંખ્યા);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  જો (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  નહીંતર
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

સ્થિર શૂન્ય
print_grammar (FILE *out, પૂર્ણાંક level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  માટે (પૂર્ણાંક i = 0; i < max_code + 1; i++)
    જો (token_translations[i] != undeftoken->content->સંખ્યા)
      {
        પ્રતીક અચલ *sym = symbols[token_translations[i]];
        અક્ષર અચલ *tag = sym->tag;
        અક્ષર અચલ *type = sym->content->type_name;
        પૂર્ણાંક precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        જો (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        જો (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      પ્રતીક અચલ *sym = symbols[i];
      અક્ષર અચલ *tag = sym->tag;
      અક્ષર અચલ *type = sym->content->type_name;
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

શૂન્ય
xml_indent (FILE *out, પૂર્ણાંક level)
{
  માટે (પૂર્ણાંક i = 0; i < level; i++)
    fputs ("  ", out);
}

શૂન્ય
xml_puts (FILE *out, પૂર્ણાંક level, અક્ષર અચલ *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

શૂન્ય
xml_printf (FILE *out, પૂર્ણાંક level, અક્ષર અચલ *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

સ્થિર અક્ષર અચલ *
xml_escape_string (સંરચના escape_buf *buf, અક્ષર અચલ *str)
{
  size_t len = strlen (str);
  size_t max_expansion = કદ "&quot;" - 1;

  જો (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  અક્ષર *p = buf->ptr;

  માટે (; *str; str++)
    પસંદગી (*str)
      {
      મૂળભૂત: *p++ = *str; તોડો;
      સ્થિતિ '&': p = stpcpy (p, "&amp;" ); તોડો;
      સ્થિતિ '<': p = stpcpy (p, "&lt;"  ); તોડો;
      સ્થિતિ '>': p = stpcpy (p, "&gt;"  ); તોડો;
      સ્થિતિ '"': p = stpcpy (p, "&quot;"); તોડો;
      }

  *p = '\0';
  પાછા buf->ptr;
}

અક્ષર અચલ *
xml_escape_n (પૂર્ણાંક n, અક્ષર અચલ *str)
{
  પાછા xml_escape_string (escape_bufs + n, str);
}

અક્ષર અચલ *
xml_escape (અક્ષર અચલ *str)
{
  પાછા xml_escape_n (0, str);
}

શૂન્ય
print_xml (શૂન્ય)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  પૂર્ણાંક level = 0;
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
  માટે (state_સંખ્યા i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  માટે (પૂર્ણાંક i = 0; i < અંક_escape_bufs; ++i)
    મુક્ત (escape_bufs[i].ptr);

  xfclose (out);
}


શૂન્ય
print_html (શૂન્ય)
{
  પ્રતિજ્ઞા (xml_flag);

  અક્ષર *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  અક્ષર *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  અક્ષર અચલ *argv[11];
  પૂર્ણાંક i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = ખાલી;
  aver (i <= ARRAY_CARDINALITY (argv));

  જો (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      માટે (પૂર્ણાંક j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  પૂર્ણાંક status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ ખાલી,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ ખાલી);
  જો (status)
    complain (ખાલી, complaint, _("%s failed with status %d"), argv[0], status);
  મુક્ત (xsltproc);
  મુક્ત (xml2html);
}
