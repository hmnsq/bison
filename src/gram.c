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

#شامل <config.h>
#شامل "system.h"

#شامل "complain.h"
#شامل "getargs.h"
#شامل "glyphs.h"
#شامل "gram.h"
#شامل "print-xml.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "symtab.h"

/* Comments for these variables are in gram.h.  */

item_عدد *ritem = پوچ;
صحیح nritems = 0;

rule *rules = پوچ;
rule_عدد nrules = 0;

نماد **symbols = پوچ;
صحیح nsyms = 0;
صحیح ntokens = 1;
صحیح nnterms = 0;

نماد_عدد *token_translations = پوچ;

صحیح max_code = 256;

صحیح required_version = 0;

تهی
item_print (item_عدد *item, rule ثابت *previous_rule, FILE *out)
{
  rule ثابت *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : پوچ, out);

  اگر (0 <= *r->rhs)
    {
      // Non-empty rhs.
      برای (item_عدد *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      برای (item_عدد *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  وگرنه
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule ثابت *r)
{
  بازگشت r->عدد < nrules;
}

bool
rule_useless_in_grammar_p (rule ثابت *r)
{
  بازگشت !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule ثابت *r)
{
  بازگشت !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule ثابت *r)
{
  بازگشت rule_rhs_length (r) == 1 && !r->action;
}

تهی
rule_lhs_print (rule ثابت *r, sym_content ثابت *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->عدد);
  اگر (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->نماد->tag);
  وگرنه
    fprintf (out, "%*s|", (صحیح) strlen (previous_lhs->نماد->tag), "");
}

تهی
rule_lhs_print_xml (rule ثابت *r, FILE *out, صحیح level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->نماد->tag);
}

size_t
rule_rhs_length (rule ثابت *r)
{
  size_t res = 0;
  برای (item_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  بازگشت res;
}

تهی
rule_rhs_print (rule ثابت *r, FILE *out)
{
  اگر (0 <= *r->rhs)
    برای (item_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  وگرنه
    fprintf (out, " %s", empty);
}

ایستا تهی
rule_rhs_print_xml (rule ثابت *r, FILE *out, صحیح level)
{
  اگر (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      برای (item_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  وگرنه
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

تهی
rule_print (rule ثابت *r, rule ثابت *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : پوچ, out);
  rule_rhs_print (r, out);
}

تهی
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  برای (صحیح i = 0; i < nritems; ++i)
    {
      اگر (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      اگر (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      وگرنه
        {
          fprintf (out, "  (rule %d)\n", item_عدد_as_rule_عدد (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (تهی)
{
  صحیح max = 0;
  برای (rule_عدد r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      اگر (length > max)
        max = length;
    }

  بازگشت max;
}

تهی
grammar_rules_partial_print (FILE *out, ثابت نویسه *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = پوچ;

  /* rule # : LHS -> RHS */
  برای (rule_عدد r = 0; r < nrules + nuseless_productions; r++)
    {
      اگر (filter && !filter (&rules[r]))
        ادامه;
      اگر (first)
        fprintf (out, "%s\n\n", title);
      وگرنه اگر (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  اگر (!first)
    fputs ("\n\n", out);
}

تهی
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

تهی
grammar_rules_print_xml (FILE *out, صحیح level)
{
  bool first = true;

  برای (rule_عدد r = 0; r < nrules + nuseless_productions; r++)
    {
      اگر (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        نویسه ثابت *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].عدد, usefulness);
        اگر (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->نماد->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  اگر (!first)
    xml_puts (out, level + 1, "</rules>");
  وگرنه
   xml_puts (out, level + 1, "<rules/>");
}

ایستا تهی
section (FILE *out, ثابت نویسه *s)
{
  fprintf (out, "%s\n", s);
  برای (صحیح i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

تهی
grammar_dump (FILE *out, ثابت نویسه *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    برای (نماد_عدد i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    برای (نماد_عدد i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    برای (rule_عدد i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule ثابت *rule_i = &rules[i];
        صحیح ثابت rhs_itemno = rule_i->rhs - ritem;
        صحیح length = rule_rhs_length (rule_i);
        aver (item_عدد_as_rule_عدد (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->عدد,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        برای (item_عدد *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  برای (rule_عدد r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->نماد->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

تهی
grammar_rules_useless_report (ثابت نویسه *message)
{
  برای (rule_عدد r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    اگر (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

تهی
grammar_free (تهی)
{
  اگر (ritem)
    آزاد (ritem - 1);
  آزاد (rules);
  آزاد (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
