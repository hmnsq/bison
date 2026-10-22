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

#include <config.h>
#include "system.h"

#include "complain.h"
#include "getargs.h"
#include "glyphs.h"
#include "gram.h"
#include "print-xml.h"
#include "reader.h"
#include "reduce.h"
#include "symtab.h"

/* Comments for these variables are in gram.h.  */

item_number *ritem = NULL;
số_nguyên nritems = 0;

rule *rules = NULL;
rule_number nrules = 0;

symbol **symbols = NULL;
số_nguyên nsyms = 0;
số_nguyên ntokens = 1;
số_nguyên nnterms = 0;

symbol_number *token_translations = NULL;

số_nguyên max_code = 256;

số_nguyên required_version = 0;

void
item_print (item_number *item, rule const *previous_rule, FILE *out)
{
  rule const *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : NULL, out);

  nếu (0 <= *r->rhs)
    {
      // Non-empty rhs.
      cho (item_number *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      cho (item_number *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  không_thì
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule const *r)
{
  trả_về r->number < nrules;
}

bool
rule_useless_in_grammar_p (rule const *r)
{
  trả_về !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule const *r)
{
  trả_về !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule const *r)
{
  trả_về rule_rhs_length (r) == 1 && !r->action;
}

void
rule_lhs_print (rule const *r, sym_content const *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->number);
  nếu (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->symbol->tag);
  không_thì
    fprintf (out, "%*s|", (số_nguyên) strlen (previous_lhs->symbol->tag), "");
}

void
rule_lhs_print_xml (rule const *r, FILE *out, số_nguyên level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->symbol->tag);
}

size_t
rule_rhs_length (rule const *r)
{
  size_t res = 0;
  cho (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  trả_về res;
}

void
rule_rhs_print (rule const *r, FILE *out)
{
  nếu (0 <= *r->rhs)
    cho (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  không_thì
    fprintf (out, " %s", empty);
}

static void
rule_rhs_print_xml (rule const *r, FILE *out, số_nguyên level)
{
  nếu (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      cho (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  không_thì
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

void
rule_print (rule const *r, rule const *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : NULL, out);
  rule_rhs_print (r, out);
}

void
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  cho (số_nguyên i = 0; i < nritems; ++i)
    {
      nếu (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      nếu (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      không_thì
        {
          fprintf (out, "  (rule %d)\n", item_number_as_rule_number (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (void)
{
  số_nguyên max = 0;
  cho (rule_number r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      nếu (length > max)
        max = length;
    }

  trả_về max;
}

void
grammar_rules_partial_print (FILE *out, const char *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = NULL;

  /* rule # : LHS -> RHS */
  cho (rule_number r = 0; r < nrules + nuseless_productions; r++)
    {
      nếu (filter && !filter (&rules[r]))
        continue;
      nếu (first)
        fprintf (out, "%s\n\n", title);
      không_thì nếu (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  nếu (!first)
    fputs ("\n\n", out);
}

void
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

void
grammar_rules_print_xml (FILE *out, số_nguyên level)
{
  bool first = true;

  cho (rule_number r = 0; r < nrules + nuseless_productions; r++)
    {
      nếu (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        char const *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].number, usefulness);
        nếu (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->symbol->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  nếu (!first)
    xml_puts (out, level + 1, "</rules>");
  không_thì
   xml_puts (out, level + 1, "<rules/>");
}

static void
section (FILE *out, const char *s)
{
  fprintf (out, "%s\n", s);
  cho (số_nguyên i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

void
grammar_dump (FILE *out, const char *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    cho (symbol_number i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    cho (symbol_number i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    cho (rule_number i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule const *rule_i = &rules[i];
        số_nguyên const rhs_itemno = rule_i->rhs - ritem;
        số_nguyên length = rule_rhs_length (rule_i);
        aver (item_number_as_rule_number (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->number,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        cho (item_number *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  cho (rule_number r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->symbol->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

void
grammar_rules_useless_report (const char *message)
{
  cho (rule_number r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    nếu (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

void
grammar_free (void)
{
  nếu (ritem)
    free (ritem - 1);
  free (rules);
  free (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
