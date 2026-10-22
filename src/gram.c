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

item_नुमेरो *ritem = NULL;
int nritems = 0;

rule *rules = NULL;
rule_नुमेरो nrules = 0;

सिम्बोलो **symbols = NULL;
int nsyms = 0;
int ntokens = 1;
int nnterms = 0;

सिम्बोलो_नुमेरो *token_translations = NULL;

int max_code = 256;

int required_version = 0;

void
item_इलिम्बाग (item_नुमेरो *item, rule const *previous_rule, FILE *out)
{
  rule const *r = item_rule (item);
  rule_lhs_इलिम्बाग (r, previous_rule ? previous_rule->lhs : NULL, out);

  if (0 <= *r->rhs)
    {
      // Non-empty rhs.
      for (item_नुमेरो *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      for (item_नुमेरो *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  else
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule const *r)
{
  return r->नुमेरो_2 < nrules;
}

bool
rule_useless_in_grammar_p (rule const *r)
{
  return !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule const *r)
{
  return !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule const *r)
{
  return rule_rhs_length (r) == 1 && !r->action;
}

void
rule_lhs_इलिम्बाग (rule const *r, sym_content const *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->नुमेरो_2);
  if (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->सिम्बोलो->tag);
  else
    fprintf (out, "%*s|", (int) strlen (previous_lhs->सिम्बोलो->tag), "");
}

void
rule_lhs_इलिम्बाग_xml (rule const *r, FILE *out, int level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->सिम्बोलो->tag);
}

size_t
rule_rhs_length (rule const *r)
{
  size_t res = 0;
  for (item_नुमेरो *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  return res;
}

void
rule_rhs_इलिम्बाग (rule const *r, FILE *out)
{
  if (0 <= *r->rhs)
    for (item_नुमेरो *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  else
    fprintf (out, " %s", empty);
}

static void
rule_rhs_इलिम्बाग_xml (rule const *r, FILE *out, int level)
{
  if (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      for (item_नुमेरो *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  else
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

void
rule_इलिम्बाग (rule const *r, rule const *prev_rule, FILE *out)
{
  rule_lhs_इलिम्बाग (r, prev_rule ? prev_rule->lhs : NULL, out);
  rule_rhs_इलिम्बाग (r, out);
}

void
ritem_इलिम्बाग (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  for (int i = 0; i < nritems; ++i)
    {
      if (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      if (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      else
        {
          fprintf (out, "  (rule %d)\n", item_नुमेरो_as_rule_नुमेरो (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (void)
{
  int max = 0;
  for (rule_नुमेरो r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      if (length > max)
        max = length;
    }

  return max;
}

void
grammar_rules_partial_इलिम्बाग (FILE *out, const char *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = NULL;

  /* rule # : LHS -> RHS */
  for (rule_नुमेरो r = 0; r < nrules + nuseless_productions; r++)
    {
      if (filter && !filter (&rules[r]))
        continue;
      if (first)
        fprintf (out, "%s\n\n", title);
      else if (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_इलिम्बाग (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  if (!first)
    fputs ("\n\n", out);
}

void
grammar_rules_इलिम्बाग (FILE *out)
{
  grammar_rules_partial_इलिम्बाग (out, _("Grammar"), rule_useful_in_grammar_p);
}

void
grammar_rules_इलिम्बाग_xml (FILE *out, int level)
{
  bool first = true;

  for (rule_नुमेरो r = 0; r < nrules + nuseless_productions; r++)
    {
      if (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        char const *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].नुमेरो_2, usefulness);
        if (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->सिम्बोलो->tag));
        fputs (">\n", out);
      }
      rule_lhs_इलिम्बाग_xml (&rules[r], out, level + 3);
      rule_rhs_इलिम्बाग_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  if (!first)
    xml_puts (out, level + 1, "</rules>");
  else
   xml_puts (out, level + 1, "<rules/>");
}

static void
section (FILE *out, const char *s)
{
  fprintf (out, "%s\n", s);
  for (int i = strlen (s); 0 < i; --i)
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

    for (सिम्बोलो_नुमेरो i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    for (सिम्बोलो_नुमेरो i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    for (rule_नुमेरो i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule const *rule_i = &rules[i];
        int const rhs_itemno = rule_i->rhs - ritem;
        int length = rule_rhs_length (rule_i);
        aver (item_नुमेरो_as_rule_नुमेरो (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->नुमेरो_2,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        for (item_नुमेरो *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  for (rule_नुमेरो r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->सिम्बोलो->tag);
      rule_rhs_इलिम्बाग (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

void
grammar_rules_useless_report (const char *message)
{
  for (rule_नुमेरो r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    if (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

void
grammar_free (void)
{
  if (ritem)
    free (ritem - 1);
  free (rules);
  free (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
