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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "glyphs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "print-xml.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "symtab.h"

/* Comments for these variables are in gram.h.  */

item_സംഖ്യ *ritem = ശൂന്യ;
പൂർണ്ണസംഖ്യ nritems = 0;

rule *rules = ശൂന്യ;
rule_സംഖ്യ nrules = 0;

ചിഹ്നം **symbols = ശൂന്യ;
പൂർണ്ണസംഖ്യ nsyms = 0;
പൂർണ്ണസംഖ്യ ntokens = 1;
പൂർണ്ണസംഖ്യ nnterms = 0;

ചിഹ്നം_സംഖ്യ *token_translations = ശൂന്യ;

പൂർണ്ണസംഖ്യ max_code = 256;

പൂർണ്ണസംഖ്യ required_version = 0;

ശൂന്യം
item_print (item_സംഖ്യ *item, rule സ്ഥിരം *previous_rule, FILE *out)
{
  rule സ്ഥിരം *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : ശൂന്യ, out);

  എങ്കിൽ (0 <= *r->rhs)
    {
      // Non-empty rhs.
      വേണ്ടി (item_സംഖ്യ *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      വേണ്ടി (item_സംഖ്യ *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  അല്ലെങ്കിൽ
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule സ്ഥിരം *r)
{
  തിരികെ r->സംഖ്യ < nrules;
}

bool
rule_useless_in_grammar_p (rule സ്ഥിരം *r)
{
  തിരികെ !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule സ്ഥിരം *r)
{
  തിരികെ !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule സ്ഥിരം *r)
{
  തിരികെ rule_rhs_length (r) == 1 && !r->action;
}

ശൂന്യം
rule_lhs_print (rule സ്ഥിരം *r, sym_content സ്ഥിരം *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->സംഖ്യ);
  എങ്കിൽ (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->ചിഹ്നം->tag);
  അല്ലെങ്കിൽ
    fprintf (out, "%*s|", (പൂർണ്ണസംഖ്യ) strlen (previous_lhs->ചിഹ്നം->tag), "");
}

ശൂന്യം
rule_lhs_print_xml (rule സ്ഥിരം *r, FILE *out, പൂർണ്ണസംഖ്യ level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->ചിഹ്നം->tag);
}

size_t
rule_rhs_length (rule സ്ഥിരം *r)
{
  size_t res = 0;
  വേണ്ടി (item_സംഖ്യ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  തിരികെ res;
}

ശൂന്യം
rule_rhs_print (rule സ്ഥിരം *r, FILE *out)
{
  എങ്കിൽ (0 <= *r->rhs)
    വേണ്ടി (item_സംഖ്യ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  അല്ലെങ്കിൽ
    fprintf (out, " %s", empty);
}

സ്ഥിതം ശൂന്യം
rule_rhs_print_xml (rule സ്ഥിരം *r, FILE *out, പൂർണ്ണസംഖ്യ level)
{
  എങ്കിൽ (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      വേണ്ടി (item_സംഖ്യ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  അല്ലെങ്കിൽ
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

ശൂന്യം
rule_print (rule സ്ഥിരം *r, rule സ്ഥിരം *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : ശൂന്യ, out);
  rule_rhs_print (r, out);
}

ശൂന്യം
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nritems; ++i)
    {
      എങ്കിൽ (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      എങ്കിൽ (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      അല്ലെങ്കിൽ
        {
          fprintf (out, "  (rule %d)\n", item_സംഖ്യ_as_rule_സംഖ്യ (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ max = 0;
  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      എങ്കിൽ (length > max)
        max = length;
    }

  തിരികെ max;
}

ശൂന്യം
grammar_rules_partial_print (FILE *out, സ്ഥിരം അക്ഷരം *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = ശൂന്യ;

  /* rule # : LHS -> RHS */
  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules + nuseless_productions; r++)
    {
      എങ്കിൽ (filter && !filter (&rules[r]))
        തുടരുക;
      എങ്കിൽ (first)
        fprintf (out, "%s\n\n", title);
      അല്ലെങ്കിൽ എങ്കിൽ (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  എങ്കിൽ (!first)
    fputs ("\n\n", out);
}

ശൂന്യം
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

ശൂന്യം
grammar_rules_print_xml (FILE *out, പൂർണ്ണസംഖ്യ level)
{
  bool first = true;

  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules + nuseless_productions; r++)
    {
      എങ്കിൽ (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        അക്ഷരം സ്ഥിരം *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].സംഖ്യ, usefulness);
        എങ്കിൽ (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->ചിഹ്നം->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  എങ്കിൽ (!first)
    xml_puts (out, level + 1, "</rules>");
  അല്ലെങ്കിൽ
   xml_puts (out, level + 1, "<rules/>");
}

സ്ഥിതം ശൂന്യം
section (FILE *out, സ്ഥിരം അക്ഷരം *s)
{
  fprintf (out, "%s\n", s);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

ശൂന്യം
grammar_dump (FILE *out, സ്ഥിരം അക്ഷരം *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    വേണ്ടി (rule_സംഖ്യ i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule സ്ഥിരം *rule_i = &rules[i];
        പൂർണ്ണസംഖ്യ സ്ഥിരം rhs_itemno = rule_i->rhs - ritem;
        പൂർണ്ണസംഖ്യ length = rule_rhs_length (rule_i);
        aver (item_സംഖ്യ_as_rule_സംഖ്യ (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->സംഖ്യ,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        വേണ്ടി (item_സംഖ്യ *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->ചിഹ്നം->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

ശൂന്യം
grammar_rules_useless_report (സ്ഥിരം അക്ഷരം *message)
{
  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    എങ്കിൽ (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

ശൂന്യം
grammar_free (ശൂന്യം)
{
  എങ്കിൽ (ritem)
    സ്വതന്ത്രം (ritem - 1);
  സ്വതന്ത്രം (rules);
  സ്വതന്ത്രം (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
