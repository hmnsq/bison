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

#ضمن <config.h>
#ضمن "system.h"

#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "glyphs.h"
#ضمن "gram.h"
#ضمن "print-xml.h"
#ضمن "reader.h"
#ضمن "reduce.h"
#ضمن "symtab.h"

/* Comments for these variables are in gram.h.  */

item_ژمار *ritem = عدم;
صحيح nritems = 0;

rule *rules = عدم;
rule_ژمار nrules = 0;

هێما **symbols = عدم;
صحيح nsyms = 0;
صحيح ntokens = 1;
صحيح nnterms = 0;

هێما_ژمار *token_translations = عدم;

صحيح max_code = 256;

صحيح required_version = 0;

فراغ
item_print (item_ژمار *item, rule ثابت *previous_rule, FILE *out)
{
  rule ثابت *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : عدم, out);

  إذا (0 <= *r->rhs)
    {
      // Non-empty rhs.
      لكل (item_ژمار *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      لكل (item_ژمار *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  وإلا
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule ثابت *r)
{
  أرجع r->ژمار < nrules;
}

bool
rule_useless_in_grammar_p (rule ثابت *r)
{
  أرجع !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule ثابت *r)
{
  أرجع !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule ثابت *r)
{
  أرجع rule_rhs_length (r) == 1 && !r->action;
}

فراغ
rule_lhs_print (rule ثابت *r, sym_content ثابت *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->ژمار);
  إذا (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->هێما->tag);
  وإلا
    fprintf (out, "%*s|", (صحيح) strlen (previous_lhs->هێما->tag), "");
}

فراغ
rule_lhs_print_xml (rule ثابت *r, FILE *out, صحيح level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->هێما->tag);
}

size_t
rule_rhs_length (rule ثابت *r)
{
  size_t res = 0;
  لكل (item_ژمار *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  أرجع res;
}

فراغ
rule_rhs_print (rule ثابت *r, FILE *out)
{
  إذا (0 <= *r->rhs)
    لكل (item_ژمار *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  وإلا
    fprintf (out, " %s", empty);
}

ساكن فراغ
rule_rhs_print_xml (rule ثابت *r, FILE *out, صحيح level)
{
  إذا (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      لكل (item_ژمار *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  وإلا
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

فراغ
rule_print (rule ثابت *r, rule ثابت *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : عدم, out);
  rule_rhs_print (r, out);
}

فراغ
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  لكل (صحيح i = 0; i < nritems; ++i)
    {
      إذا (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      إذا (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      وإلا
        {
          fprintf (out, "  (rule %d)\n", item_ژمار_as_rule_ژمار (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (فراغ)
{
  صحيح max = 0;
  لكل (rule_ژمار r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      إذا (length > max)
        max = length;
    }

  أرجع max;
}

فراغ
grammar_rules_partial_print (FILE *out, ثابت حرف *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = عدم;

  /* rule # : LHS -> RHS */
  لكل (rule_ژمار r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (filter && !filter (&rules[r]))
        استمر;
      إذا (first)
        fprintf (out, "%s\n\n", title);
      وإلا إذا (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  إذا (!first)
    fputs ("\n\n", out);
}

فراغ
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

فراغ
grammar_rules_print_xml (FILE *out, صحيح level)
{
  bool first = true;

  لكل (rule_ژمار r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        حرف ثابت *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].ژمار, usefulness);
        إذا (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->هێما->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  إذا (!first)
    xml_puts (out, level + 1, "</rules>");
  وإلا
   xml_puts (out, level + 1, "<rules/>");
}

ساكن فراغ
section (FILE *out, ثابت حرف *s)
{
  fprintf (out, "%s\n", s);
  لكل (صحيح i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

فراغ
grammar_dump (FILE *out, ثابت حرف *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    لكل (هێما_ژمار i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    لكل (هێما_ژمار i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    لكل (rule_ژمار i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule ثابت *rule_i = &rules[i];
        صحيح ثابت rhs_itemno = rule_i->rhs - ritem;
        صحيح length = rule_rhs_length (rule_i);
        aver (item_ژمار_as_rule_ژمار (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->ژمار,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        لكل (item_ژمار *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  لكل (rule_ژمار r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->هێما->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

فراغ
grammar_rules_useless_report (ثابت حرف *message)
{
  لكل (rule_ژمار r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    إذا (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

فراغ
grammar_free (فراغ)
{
  إذا (ritem)
    حرر (ritem - 1);
  حرر (rules);
  حرر (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
