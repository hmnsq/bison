/* Allocate input grammar variables for Bison.

   Copyright (C) 1984, 1986, 1989, 2001-2003, 2005-2015, 2018-2022 Free
   Software Foundation, Inc.

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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "glyphs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "print-xml.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "symtab.h"

/* Comments for these variables are in gram.h.  */

item_সংখ্যা *ritem = ফাঁকা;
পূর্ণসংখ্যা nritems = 0;

rule *rules = ফাঁকা;
rule_সংখ্যা nrules = 0;

প্রতীক **symbols = ফাঁকা;
পূর্ণসংখ্যা nsyms = 0;
পূর্ণসংখ্যা ntokens = 1;
পূর্ণসংখ্যা nnterms = 0;

প্রতীক_সংখ্যা *token_translations = ফাঁকা;

পূর্ণসংখ্যা max_code = 256;

পূর্ণসংখ্যা required_version = 0;

শূন্য
item_print (item_সংখ্যা *item, rule ধ্রুবক *previous_rule, FILE *out)
{
  rule ধ্রুবক *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : ফাঁকা, out);

  যদি (0 <= *r->rhs)
    {
      // Non-empty rhs.
      জন্য (item_সংখ্যা *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      জন্য (item_সংখ্যা *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  অন্যথা
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule ধ্রুবক *r)
{
  ফেরত r->সংখ্যা < nrules;
}

bool
rule_useless_in_grammar_p (rule ধ্রুবক *r)
{
  ফেরত !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule ধ্রুবক *r)
{
  ফেরত !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule ধ্রুবক *r)
{
  ফেরত rule_rhs_দৈর্ঘ্য (r) == 1 && !r->action;
}

শূন্য
rule_lhs_print (rule ধ্রুবক *r, sym_content ধ্রুবক *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->সংখ্যা);
  যদি (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->প্রতীক->tag);
  অন্যথা
    fprintf (out, "%*s|", (পূর্ণসংখ্যা) strlen (previous_lhs->প্রতীক->tag), "");
}

শূন্য
rule_lhs_print_xml (rule ধ্রুবক *r, FILE *out, পূর্ণসংখ্যা level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->প্রতীক->tag);
}

size_t
rule_rhs_দৈর্ঘ্য (rule ধ্রুবক *r)
{
  size_t res = 0;
  জন্য (item_সংখ্যা *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  ফেরত res;
}

শূন্য
rule_rhs_print (rule ধ্রুবক *r, FILE *out)
{
  যদি (0 <= *r->rhs)
    জন্য (item_সংখ্যা *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  অন্যথা
    fprintf (out, " %s", empty);
}

স্থির শূন্য
rule_rhs_print_xml (rule ধ্রুবক *r, FILE *out, পূর্ণসংখ্যা level)
{
  যদি (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      জন্য (item_সংখ্যা *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  অন্যথা
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

শূন্য
rule_print (rule ধ্রুবক *r, rule ধ্রুবক *পূর্ব_rule, FILE *out)
{
  rule_lhs_print (r, পূর্ব_rule ? পূর্ব_rule->lhs : ফাঁকা, out);
  rule_rhs_print (r, out);
}

শূন্য
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  জন্য (পূর্ণসংখ্যা i = 0; i < nritems; ++i)
    {
      যদি (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      যদি (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      অন্যথা
        {
          fprintf (out, "  (rule %d)\n", item_সংখ্যা_as_rule_সংখ্যা (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (শূন্য)
{
  পূর্ণসংখ্যা max = 0;
  জন্য (rule_সংখ্যা r = 0; r < nrules; ++r)
    {
      size_t দৈর্ঘ্য = rule_rhs_দৈর্ঘ্য (&rules[r]);
      যদি (দৈর্ঘ্য > max)
        max = দৈর্ঘ্য;
    }

  ফেরত max;
}

শূন্য
grammar_rules_partial_print (FILE *out, ধ্রুবক অক্ষর *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = ফাঁকা;

  /* rule # : LHS -> RHS */
  জন্য (rule_সংখ্যা r = 0; r < nrules + nuseless_productions; r++)
    {
      যদি (filter && !filter (&rules[r]))
        চালু;
      যদি (first)
        fprintf (out, "%s\n\n", title);
      অন্যথা যদি (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  যদি (!first)
    fputs ("\n\n", out);
}

শূন্য
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

শূন্য
grammar_rules_print_xml (FILE *out, পূর্ণসংখ্যা level)
{
  bool first = true;

  জন্য (rule_সংখ্যা r = 0; r < nrules + nuseless_productions; r++)
    {
      যদি (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        অক্ষর ধ্রুবক *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].সংখ্যা, usefulness);
        যদি (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->প্রতীক->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  যদি (!first)
    xml_puts (out, level + 1, "</rules>");
  অন্যথা
   xml_puts (out, level + 1, "<rules/>");
}

স্থির শূন্য
section (FILE *out, ধ্রুবক অক্ষর *s)
{
  fprintf (out, "%s\n", s);
  জন্য (পূর্ণসংখ্যা i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

শূন্য
grammar_dump (FILE *out, ধ্রুবক অক্ষর *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    জন্য (প্রতীক_সংখ্যা i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    জন্য (প্রতীক_সংখ্যা i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    জন্য (rule_সংখ্যা i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule ধ্রুবক *rule_i = &rules[i];
        পূর্ণসংখ্যা ধ্রুবক rhs_itemno = rule_i->rhs - ritem;
        পূর্ণসংখ্যা দৈর্ঘ্য = rule_rhs_দৈর্ঘ্য (rule_i);
        aver (item_সংখ্যা_as_rule_সংখ্যা (rule_i->rhs[দৈর্ঘ্য]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->সংখ্যা,
                 rhs_itemno, rhs_itemno + দৈর্ঘ্য - 1);
        /* Dumped the RHS. */
        জন্য (item_সংখ্যা *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  জন্য (rule_সংখ্যা r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->প্রতীক->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

শূন্য
grammar_rules_useless_report (ধ্রুবক অক্ষর *message)
{
  জন্য (rule_সংখ্যা r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    যদি (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

শূন্য
grammar_free (শূন্য)
{
  যদি (ritem)
    মুক্ত (ritem - 1);
  মুক্ত (rules);
  মুক্ত (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
