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
цяло nritems = 0;

rule *rules = NULL;
rule_number nrules = 0;

symbol **symbols = NULL;
цяло nsyms = 0;
цяло ntokens = 1;
цяло nnterms = 0;

symbol_number *token_translations = NULL;

цяло max_code = 256;

цяло required_version = 0;

void
item_print (item_number *item, rule const *previous_rule, FILE *out)
{
  rule const *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : NULL, out);

  ако (0 <= *r->rhs)
    {
      // Non-empty rhs.
      за (item_number *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      за (item_number *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  иначе
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule const *r)
{
  върни r->number < nrules;
}

bool
rule_useless_in_grammar_p (rule const *r)
{
  върни !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule const *r)
{
  върни !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule const *r)
{
  върни rule_rhs_length (r) == 1 && !r->action;
}

void
rule_lhs_print (rule const *r, sym_content const *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->number);
  ако (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->symbol->tag);
  иначе
    fprintf (out, "%*s|", (цяло) strlen (previous_lhs->symbol->tag), "");
}

void
rule_lhs_print_xml (rule const *r, FILE *out, цяло level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->symbol->tag);
}

size_t
rule_rhs_length (rule const *r)
{
  size_t res = 0;
  за (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  върни res;
}

void
rule_rhs_print (rule const *r, FILE *out)
{
  ако (0 <= *r->rhs)
    за (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  иначе
    fprintf (out, " %s", empty);
}

static void
rule_rhs_print_xml (rule const *r, FILE *out, цяло level)
{
  ако (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      за (item_number *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  иначе
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
  за (цяло i = 0; i < nritems; ++i)
    {
      ако (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      ако (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      иначе
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
  цяло max = 0;
  за (rule_number r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      ако (length > max)
        max = length;
    }

  върни max;
}

void
grammar_rules_partial_print (FILE *out, const char *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = NULL;

  /* rule # : LHS -> RHS */
  за (rule_number r = 0; r < nrules + nuseless_productions; r++)
    {
      ако (filter && !filter (&rules[r]))
        continue;
      ако (first)
        fprintf (out, "%s\n\n", title);
      иначе ако (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  ако (!first)
    fputs ("\n\n", out);
}

void
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

void
grammar_rules_print_xml (FILE *out, цяло level)
{
  bool first = true;

  за (rule_number r = 0; r < nrules + nuseless_productions; r++)
    {
      ако (first)
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
        ако (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->symbol->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  ако (!first)
    xml_puts (out, level + 1, "</rules>");
  иначе
   xml_puts (out, level + 1, "<rules/>");
}

static void
section (FILE *out, const char *s)
{
  fprintf (out, "%s\n", s);
  за (цяло i = strlen (s); 0 < i; --i)
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

    за (symbol_number i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    за (symbol_number i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    за (rule_number i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule const *rule_i = &rules[i];
        цяло const rhs_itemno = rule_i->rhs - ritem;
        цяло length = rule_rhs_length (rule_i);
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
        за (item_number *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  за (rule_number r = 0; r < nrules + nuseless_productions; ++r)
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
  за (rule_number r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    ако (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

void
grammar_free (void)
{
  ако (ritem)
    free (ritem - 1);
  free (rules);
  free (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
