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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "print-xml.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <stdarg.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "execute.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "path-join.h"
#அந்தர்பாவ "print.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "state.h"
#அந்தர்பாவ "symtab.h"
#அந்தர்பாவ "tables.h"

நிலையான bitset no_reduce_set;
கட்டமைப்பு escape_buf
{
  எழுத்து *ptr;
  size_t size;
};
எண்ணிக்கை { இலக்கம்_escape_bufs = 3 };
நிலையான கட்டமைப்பு escape_buf escape_bufs[இலக்கம்_escape_bufs];


/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

நிலையான வெறுமை
print_core (FILE *out, முழு level, state *s)
{
  item_index *sitems = s->பொருட்கள்;
  size_t snritems = s->nitems;

  /* Output all the items of a state, not only its kernel.  */
  closure (sitems, snritems);
  sitems = itemset;
  snritems = nitemset;

  எனில் (!snritems)
    {
      xml_puts (out, level, "<itemset/>");
      திரும்பு;
    }

  xml_puts (out, level, "<itemset>");

  க்கு (size_t i = 0; i < snritems; i++)
    {
      bool printed = false;
      item_எண் *sp1 = ritem + sitems[i];
      rule மாறிலி *r = item_rule (sp1);
      item_எண் *sp = r->rhs;

      /* Display the lookahead tokens?  */
      எனில் (item_எண்_is_rule_எண் (*sp1))
        {
          reductions *reds = s->reductions;
          முழு red = state_reduction_find (s, r);
          /* Print item with lookaheads if there are. */
          எனில் (reds->lookaheads && red != -1)
            {
              xml_printf (out, level + 1,
                          "<item rule-number=\"%d\" dot=\"%d\">",
                          r->எண், sp1 - sp);
              state_rule_lookaheads_print_xml (s, r,
                                                     out, level + 2);
              xml_puts (out, level + 1, "</item>");
              printed = true;
            }
        }

      எனில் (!printed)
        xml_printf (out, level + 1,
                    "<item rule-number=\"%d\" dot=\"%d\"/>",
                    r->எண்,
                    sp1 - sp);
    }
  xml_puts (out, level, "</itemset>");
}


/*-----------------------------------------------------------.
| Report the shifts if DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                       |
`-----------------------------------------------------------*/

நிலையான வெறுமை
print_transitions (state *s, FILE *out, முழு level)
{
  transitions *trans = s->transitions;
  முழு n = 0;
  முழு i;

  க்கு (i = 0; i < trans->இலக்கம்; i++)
    எனில் (!TRANSITION_IS_DISABLED (trans, i))
      {
        n++;
      }

  /* Nothing to report. */
  எனில் (!n)
    {
      xml_puts (out, level, "<transitions/>");
      திரும்பு;
    }

  /* Report lookahead tokens and shifts.  */
  xml_puts (out, level, "<transitions>");

  க்கு (i = 0; i < trans->இலக்கம்; i++)
    எனில் (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i))
      {
        குறியீடு *sym = symbols[TRANSITION_குறியீடு (trans, i)];
        எழுத்து மாறிலி *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"shift\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->எண்);
      }

  க்கு (i = 0; i < trans->இலக்கம்; i++)
    எனில் (!TRANSITION_IS_DISABLED (trans, i)
        && !TRANSITION_IS_SHIFT (trans, i))
      {
        குறியீடு *sym = symbols[TRANSITION_குறியீடு (trans, i)];
        எழுத்து மாறிலி *tag = sym->tag;
        state *s1 = trans->states[i];

        xml_printf (out, level + 1,
                    "<transition type=\"goto\" symbol=\"%s\" state=\"%d\"/>",
                    xml_escape (tag), s1->எண்);
      }

  xml_puts (out, level, "</transitions>");
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

நிலையான வெறுமை
print_errs (FILE *out, முழு level, state *s)
{
  errs *errp = s->errs;
  bool மொத்தம் = false;
  முழு i;

  க்கு (i = 0; i < errp->இலக்கம்; ++i)
    எனில் (errp->symbols[i])
      மொத்தம் = true;

  /* Nothing to report. */
  எனில் (!மொத்தம்)
    {
      xml_puts (out, level, "<errors/>");
      திரும்பு;
    }

  /* Report lookahead tokens and errors.  */
  xml_puts (out, level, "<errors>");
  க்கு (i = 0; i < errp->இலக்கம்; ++i)
    எனில் (errp->symbols[i])
      {
        எழுத்து மாறிலி *tag = errp->symbols[i]->tag;
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

நிலையான வெறுமை
print_reduction (FILE *out, முழு level, எழுத்து மாறிலி *lookahead,
                 rule *r, bool enabled)
{
  எனில் (rule_is_initial (r))
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"accept\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                enabled ? "true" : "false");
  இல்லையெனில்
    xml_printf (out, level,
                "<reduction symbol=\"%s\" rule=\"%d\" enabled=\"%s\"/>",
                xml_escape (lookahead),
                r->எண்,
                enabled ? "true" : "false");
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

நிலையான வெறுமை
print_reductions (FILE *out, முழு level, state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;
  rule *default_reduction = வெற்று;
  முழு report = false;
  முழு i, j;

  எனில் (reds->இலக்கம் == 0)
    {
      xml_puts (out, level, "<reductions/>");
      திரும்பு;
    }

  எனில் (yydefact[s->எண்] != 0)
    default_reduction = &rules[yydefact[s->எண்] - 1];

  bitset_zero (no_reduce_set);
  FOR_EACH_SHIFT (trans, i)
    bitset_set (no_reduce_set, TRANSITION_குறியீடு (trans, i));
  க்கு (i = 0; i < s->errs->இலக்கம்; ++i)
    எனில் (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->எண்);

  எனில் (default_reduction)
    report = true;

  எனில் (reds->lookaheads)
    க்கு (i = 0; i < ntokens; i++)
      {
        bool மொத்தம் = bitset_test (no_reduce_set, i);

        க்கு (j = 0; j < reds->இலக்கம்; ++j)
          எனில் (bitset_test (reds->lookaheads[j], i))
            {
              எனில் (! மொத்தம்)
                {
                  எனில் (reds->rules[j] != default_reduction)
                    report = true;
                  மொத்தம் = true;
                }
              இல்லையெனில்
                {
                  report = true;
                }
            }
      }

  /* Nothing to report. */
  எனில் (!report)
    {
      xml_puts (out, level, "<reductions/>");
      திரும்பு;
    }

  xml_puts (out, level, "<reductions>");

  /* Report lookahead tokens (or $default) and reductions.  */
  எனில் (reds->lookaheads)
    க்கு (i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool மொத்தம் = bitset_test (no_reduce_set, i);

        க்கு (j = 0; j < reds->இலக்கம்; ++j)
          எனில் (bitset_test (reds->lookaheads[j], i))
            {
              எனில் (! மொத்தம்)
                {
                  எனில் (reds->rules[j] != default_reduction)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     reds->rules[j], true);
                  இல்லையெனில்
                    defaulted = true;
                  மொத்தம் = true;
                }
              இல்லையெனில்
                {
                  எனில் (defaulted)
                    print_reduction (out, level + 1, symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, level + 1, symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  எனில் (default_reduction)
    print_reduction (out, level + 1,
                     "$default", default_reduction, true);

  xml_puts (out, level, "</reductions>");
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

நிலையான வெறுமை
print_actions (FILE *out, முழு level, state *s)
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

நிலையான வெறுமை
print_state (FILE *out, முழு level, state *s)
{
  fputc ('\n', out);
  xml_printf (out, level, "<state number=\"%d\">", s->எண்);
  print_core (out, level + 1, s);
  print_actions (out, level + 1, s);
  எனில் (s->solved_conflicts_xml)
    {
      xml_puts (out, level + 1, "<solved-conflicts>");
      fputs (s->solved_conflicts_xml, out);
      xml_puts (out, level + 1, "</solved-conflicts>");
    }
  இல்லையெனில்
    xml_puts (out, level + 1, "<solved-conflicts/>");
  xml_puts (out, level, "</state>");
}


/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

நிலையான வெறுமை
print_grammar (FILE *out, முழு level)
{
  fputc ('\n', out);
  xml_puts (out, level, "<grammar>");
  grammar_rules_print_xml (out, level);

  /* Terminals */
  xml_puts (out, level + 1, "<terminals>");
  க்கு (முழு i = 0; i < max_code + 1; i++)
    எனில் (token_translations[i] != undeftoken->content->எண்)
      {
        குறியீடு மாறிலி *sym = symbols[token_translations[i]];
        எழுத்து மாறிலி *tag = sym->tag;
        எழுத்து மாறிலி *type = sym->content->type_name;
        முழு precedence = sym->content->prec;
        assoc associativity = sym->content->assoc;
        xml_indent (out, level + 2);
        fprintf (out,
                 "<terminal symbol-number=\"%d\" token-number=\"%d\""
                 " name=\"%s\" type=\"%s\" usefulness=\"%s\"",
                 token_translations[i], i, xml_escape_n (0, tag),
                 type ? xml_escape_n (1, type) : "",
                 reduce_token_unused_in_grammar (token_translations[i])
                   ? "unused-in-grammar" : "useful");
        எனில் (precedence)
          fprintf (out, " prec=\"%d\"", precedence);
        எனில் (associativity != undef_assoc)
          fprintf (out, " assoc=\"%s\"", assoc_to_string (associativity) + 1);
        fputs ("/>\n", out);
      }
  xml_puts (out, level + 1, "</terminals>");

  /* Nonterminals */
  xml_puts (out, level + 1, "<nonterminals>");
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms + nuseless_nonterminals; i++)
    {
      குறியீடு மாறிலி *sym = symbols[i];
      எழுத்து மாறிலி *tag = sym->tag;
      எழுத்து மாறிலி *type = sym->content->type_name;
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

வெறுமை
xml_indent (FILE *out, முழு level)
{
  க்கு (முழு i = 0; i < level; i++)
    fputs ("  ", out);
}

வெறுமை
xml_puts (FILE *out, முழு level, எழுத்து மாறிலி *s)
{
  xml_indent (out, level);
  fputs (s, out);
  fputc ('\n', out);
}

வெறுமை
xml_printf (FILE *out, முழு level, எழுத்து மாறிலி *fmt, ...)
{
  va_list arglist;

  xml_indent (out, level);

  va_start (arglist, fmt);
  vfprintf (out, fmt, arglist);
  va_end (arglist);

  fputc ('\n', out);
}

நிலையான எழுத்து மாறிலி *
xml_escape_string (கட்டமைப்பு escape_buf *buf, எழுத்து மாறிலி *str)
{
  size_t len = strlen (str);
  size_t max_expansion = அளவு "&quot;" - 1;

  எனில் (buf->size <= max_expansion * len)
    {
      buf->size = max_expansion * len + 1;
      buf->ptr = x2realloc (buf->ptr, &buf->size);
    }
  எழுத்து *p = buf->ptr;

  க்கு (; *str; str++)
    தேர்வு (*str)
      {
      இயல்பு: *p++ = *str; நிறுத்து;
      நிலை '&': p = stpcpy (p, "&amp;" ); நிறுத்து;
      நிலை '<': p = stpcpy (p, "&lt;"  ); நிறுத்து;
      நிலை '>': p = stpcpy (p, "&gt;"  ); நிறுத்து;
      நிலை '"': p = stpcpy (p, "&quot;"); நிறுத்து;
      }

  *p = '\0';
  திரும்பு buf->ptr;
}

எழுத்து மாறிலி *
xml_escape_n (முழு n, எழுத்து மாறிலி *str)
{
  திரும்பு xml_escape_string (escape_bufs + n, str);
}

எழுத்து மாறிலி *
xml_escape (எழுத்து மாறிலி *str)
{
  திரும்பு xml_escape_n (0, str);
}

வெறுமை
print_xml (வெறுமை)
{
  FILE *out = xfopen (spec_xml_file, "w");

  fputs ("<?xml version=\"1.0\"?>\n\n", out);

  முழு level = 0;
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
  க்கு (state_எண் i = 0; i < nstates; i++)
    print_state (out, level + 2, states[i]);
  xml_puts (out, level + 1, "</automaton>");

  bitset_free (no_reduce_set);

  xml_puts (out, 0, "</bison-xml-report>");

  க்கு (முழு i = 0; i < இலக்கம்_escape_bufs; ++i)
    விடுவி (escape_bufs[i].ptr);

  xfclose (out);
}


வெறுமை
print_html (வெறுமை)
{
  ப்ரதிஜ்ஞா (xml_flag);

  எழுத்து *xml2html = xpath_join (pkgdatadir (), "xslt/xml2xhtml.xsl");
  எழுத்து *xsltproc = muscle_percent_define_get ("tool.xsltproc");
  எழுத்து மாறிலி *argv[11];
  முழு i = 0;
  argv[i++] = xsltproc;
  argv[i++] = "-o";
  argv[i++] = spec_html_file;
  argv[i++] = xml2html;
  argv[i++] = spec_xml_file;
  argv[i++] = வெற்று;
  aver (i <= ARRAY_CARDINALITY (argv));

  எனில் (trace_flag & trace_tools)
    {
      fputs ("running:", stderr);
      க்கு (முழு j = 0; argv[j]; ++j)
        fprintf (stderr, " %s", argv[j]);
      fputc ('\n', stderr);
    }

  முழு status
    = execute (argv[0],
               argv[0], argv,
               /* directory */ வெற்று,
               /* ignore_sigpipe */ false,
               /* null_stdin, null_stdout, null_stderr */ true, true, true,
               /* slave_process */ true, /* exit_on_error */ false,
               /* termsigp */ வெற்று);
  எனில் (status)
    complain (வெற்று, complaint, _("%s failed with status %d"), argv[0], status);
  விடுவி (xsltproc);
  விடுவி (xml2html);
}
