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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "glyphs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "print-xml.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

/* Comments for these variables are in gram.h.  */

item_ସଂଖ୍ୟା *ritem = ଖାଲି;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା nritems = 0;

rule *rules = ଖାଲି;
rule_ସଂଖ୍ୟା nrules = 0;

ପ୍ରତୀକ **symbols = ଖାଲି;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsyms = 0;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ntokens = 1;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା nnterms = 0;

ପ୍ରତୀକ_ସଂଖ୍ୟା *token_translations = ଖାଲି;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_code = 256;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା required_version = 0;

ଶୂନ୍ୟ
item_print (item_ସଂଖ୍ୟା *item, rule ଅଚଳ *previous_rule, FILE *out)
{
  rule ଅଚଳ *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : ଖାଲି, out);

  ଯଦି (0 <= *r->rhs)
    {
      // Non-empty rhs.
      ପାଇଁ (item_ସଂଖ୍ୟା *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      ପାଇଁ (item_ସଂଖ୍ୟା *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  ଅନ୍ୟଥା
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule ଅଚଳ *r)
{
  ଫେରନ୍ତୁ r->ସଂଖ୍ୟା < nrules;
}

bool
rule_useless_in_grammar_p (rule ଅଚଳ *r)
{
  ଫେରନ୍ତୁ !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule ଅଚଳ *r)
{
  ଫେରନ୍ତୁ !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule ଅଚଳ *r)
{
  ଫେରନ୍ତୁ rule_rhs_length (r) == 1 && !r->action;
}

ଶୂନ୍ୟ
rule_lhs_print (rule ଅଚଳ *r, sym_content ଅଚଳ *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->ସଂଖ୍ୟା);
  ଯଦି (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->ପ୍ରତୀକ->tag);
  ଅନ୍ୟଥା
    fprintf (out, "%*s|", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) strlen (previous_lhs->ପ୍ରତୀକ->tag), "");
}

ଶୂନ୍ୟ
rule_lhs_print_xml (rule ଅଚଳ *r, FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->ପ୍ରତୀକ->tag);
}

size_t
rule_rhs_length (rule ଅଚଳ *r)
{
  size_t res = 0;
  ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ
rule_rhs_print (rule ଅଚଳ *r, FILE *out)
{
  ଯଦି (0 <= *r->rhs)
    ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  ଅନ୍ୟଥା
    fprintf (out, " %s", empty);
}

ସ୍ଥିର ଶୂନ୍ୟ
rule_rhs_print_xml (rule ଅଚଳ *r, FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  ଯଦି (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  ଅନ୍ୟଥା
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

ଶୂନ୍ୟ
rule_print (rule ଅଚଳ *r, rule ଅଚଳ *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : ଖାଲି, out);
  rule_rhs_print (r, out);
}

ଶୂନ୍ୟ
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nritems; ++i)
    {
      ଯଦି (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      ଯଦି (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      ଅନ୍ୟଥା
        {
          fprintf (out, "  (rule %d)\n", item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା max = 0;
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      ଯଦି (length > max)
        max = length;
    }

  ଫେରନ୍ତୁ max;
}

ଶୂନ୍ୟ
grammar_rules_partial_print (FILE *out, ଅଚଳ ଅକ୍ଷର *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = ଖାଲି;

  /* rule # : LHS -> RHS */
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules + nuseless_productions; r++)
    {
      ଯଦି (filter && !filter (&rules[r]))
        ଜାରି;
      ଯଦି (first)
        fprintf (out, "%s\n\n", title);
      ଅନ୍ୟଥା ଯଦି (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  ଯଦି (!first)
    fputs ("\n\n", out);
}

ଶୂନ୍ୟ
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

ଶୂନ୍ୟ
grammar_rules_print_xml (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  bool first = true;

  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules + nuseless_productions; r++)
    {
      ଯଦି (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        ଅକ୍ଷର ଅଚଳ *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].ସଂଖ୍ୟା, usefulness);
        ଯଦି (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->ପ୍ରତୀକ->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  ଯଦି (!first)
    xml_puts (out, level + 1, "</rules>");
  ଅନ୍ୟଥା
   xml_puts (out, level + 1, "<rules/>");
}

ସ୍ଥିର ଶୂନ୍ୟ
section (FILE *out, ଅଚଳ ଅକ୍ଷର *s)
{
  fprintf (out, "%s\n", s);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

ଶୂନ୍ୟ
grammar_dump (FILE *out, ଅଚଳ ଅକ୍ଷର *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    ପାଇଁ (rule_ସଂଖ୍ୟା i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule ଅଚଳ *rule_i = &rules[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଚଳ rhs_itemno = rule_i->rhs - ritem;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = rule_rhs_length (rule_i);
        aver (item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->ସଂଖ୍ୟା,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->ପ୍ରତୀକ->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

ଶୂନ୍ୟ
grammar_rules_useless_report (ଅଚଳ ଅକ୍ଷର *message)
{
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    ଯଦି (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

ଶୂନ୍ୟ
grammar_free (ଶୂନ୍ୟ)
{
  ଯଦି (ritem)
    ମୁକ୍ତ (ritem - 1);
  ମୁକ୍ତ (rules);
  ମୁକ୍ତ (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
