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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "glyphs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "print-xml.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* Comments for these variables are in gram.h.  */

item_ಸಂಖ್ಯೆ *ritem = ರಿಕ್ತ;
ಪೂರ್ಣಾಙ್ಕ nritems = 0;

rule *rules = ರಿಕ್ತ;
rule_ಸಂಖ್ಯೆ nrules = 0;

ಚಿಹ್ನೆ **symbols = ರಿಕ್ತ;
ಪೂರ್ಣಾಙ್ಕ nsyms = 0;
ಪೂರ್ಣಾಙ್ಕ ntokens = 1;
ಪೂರ್ಣಾಙ್ಕ nnterms = 0;

ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *token_translations = ರಿಕ್ತ;

ಪೂರ್ಣಾಙ್ಕ max_code = 256;

ಪೂರ್ಣಾಙ್ಕ required_version = 0;

ಶೂನ್ಯ
item_print (item_ಸಂಖ್ಯೆ *item, rule ಧ್ರುವ *previous_rule, FILE *out)
{
  rule ಧ್ರುವ *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : ರಿಕ್ತ, out);

  ಯದಿ (0 <= *r->rhs)
    {
      // Non-empty rhs.
      ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  ಅನ್ಯಥಾ
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule ಧ್ರುವ *r)
{
  ಪ್ರತ್ಯಾಗಮ r->ಸಂಖ್ಯೆ < nrules;
}

bool
rule_useless_in_grammar_p (rule ಧ್ರುವ *r)
{
  ಪ್ರತ್ಯಾಗಮ !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule ಧ್ರುವ *r)
{
  ಪ್ರತ್ಯಾಗಮ !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule ಧ್ರುವ *r)
{
  ಪ್ರತ್ಯಾಗಮ rule_rhs_length (r) == 1 && !r->action;
}

ಶೂನ್ಯ
rule_lhs_print (rule ಧ್ರುವ *r, sym_content ಧ್ರುವ *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->ಸಂಖ್ಯೆ);
  ಯದಿ (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->ಚಿಹ್ನೆ->tag);
  ಅನ್ಯಥಾ
    fprintf (out, "%*s|", (ಪೂರ್ಣಾಙ್ಕ) strlen (previous_lhs->ಚಿಹ್ನೆ->tag), "");
}

ಶೂನ್ಯ
rule_lhs_print_xml (rule ಧ್ರುವ *r, FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->ಚಿಹ್ನೆ->tag);
}

size_t
rule_rhs_length (rule ಧ್ರುವ *r)
{
  size_t res = 0;
  ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ
rule_rhs_print (rule ಧ್ರುವ *r, FILE *out)
{
  ಯದಿ (0 <= *r->rhs)
    ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  ಅನ್ಯಥಾ
    fprintf (out, " %s", empty);
}

ಸ್ಥಿರ ಶೂನ್ಯ
rule_rhs_print_xml (rule ಧ್ರುವ *r, FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  ಯದಿ (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  ಅನ್ಯಥಾ
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

ಶೂನ್ಯ
rule_print (rule ಧ್ರುವ *r, rule ಧ್ರುವ *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : ರಿಕ್ತ, out);
  rule_rhs_print (r, out);
}

ಶೂನ್ಯ
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nritems; ++i)
    {
      ಯದಿ (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      ಯದಿ (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      ಅನ್ಯಥಾ
        {
          fprintf (out, "  (rule %d)\n", item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ max = 0;
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      ಯದಿ (length > max)
        max = length;
    }

  ಪ್ರತ್ಯಾಗಮ max;
}

ಶೂನ್ಯ
grammar_rules_partial_print (FILE *out, ಧ್ರುವ ವರ್ಣ *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = ರಿಕ್ತ;

  /* rule # : LHS -> RHS */
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules + nuseless_productions; r++)
    {
      ಯದಿ (filter && !filter (&rules[r]))
        ಅನುವರ್ತ;
      ಯದಿ (first)
        fprintf (out, "%s\n\n", title);
      ಅನ್ಯಥಾ ಯದಿ (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  ಯದಿ (!first)
    fputs ("\n\n", out);
}

ಶೂನ್ಯ
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

ಶೂನ್ಯ
grammar_rules_print_xml (FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  bool first = true;

  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules + nuseless_productions; r++)
    {
      ಯದಿ (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        ವರ್ಣ ಧ್ರುವ *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].ಸಂಖ್ಯೆ, usefulness);
        ಯದಿ (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->ಚಿಹ್ನೆ->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  ಯದಿ (!first)
    xml_puts (out, level + 1, "</rules>");
  ಅನ್ಯಥಾ
   xml_puts (out, level + 1, "<rules/>");
}

ಸ್ಥಿರ ಶೂನ್ಯ
section (FILE *out, ಧ್ರುವ ವರ್ಣ *s)
{
  fprintf (out, "%s\n", s);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

ಶೂನ್ಯ
grammar_dump (FILE *out, ಧ್ರುವ ವರ್ಣ *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule ಧ್ರುವ *rule_i = &rules[i];
        ಪೂರ್ಣಾಙ್ಕ ಧ್ರುವ rhs_itemno = rule_i->rhs - ritem;
        ಪೂರ್ಣಾಙ್ಕ length = rule_rhs_length (rule_i);
        aver (item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->ಸಂಖ್ಯೆ,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->ಚಿಹ್ನೆ->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

ಶೂನ್ಯ
grammar_rules_useless_report (ಧ್ರುವ ವರ್ಣ *message)
{
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    ಯದಿ (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

ಶೂನ್ಯ
grammar_free (ಶೂನ್ಯ)
{
  ಯದಿ (ritem)
    ಮುಕ್ತ (ritem - 1);
  ಮುಕ್ತ (rules);
  ಮುಕ್ತ (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
