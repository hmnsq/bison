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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "glyphs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "print-xml.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "symtab.h"

/* Comments for these variables are in gram.h.  */

item_எண் *ritem = வெற்று;
முழு nritems = 0;

rule *rules = வெற்று;
rule_எண் nrules = 0;

குறியீடு **symbols = வெற்று;
முழு nsyms = 0;
முழு ntokens = 1;
முழு nnterms = 0;

குறியீடு_எண் *token_translations = வெற்று;

முழு max_code = 256;

முழு required_version = 0;

வெறுமை
item_print (item_எண் *item, rule மாறிலி *previous_rule, FILE *out)
{
  rule மாறிலி *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : வெற்று, out);

  எனில் (0 <= *r->rhs)
    {
      // Non-empty rhs.
      க்கு (item_எண் *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      க்கு (item_எண் *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  இல்லையெனில்
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule மாறிலி *r)
{
  திரும்பு r->எண் < nrules;
}

bool
rule_useless_in_grammar_p (rule மாறிலி *r)
{
  திரும்பு !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule மாறிலி *r)
{
  திரும்பு !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule மாறிலி *r)
{
  திரும்பு rule_rhs_length (r) == 1 && !r->action;
}

வெறுமை
rule_lhs_print (rule மாறிலி *r, sym_content மாறிலி *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->எண்);
  எனில் (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->குறியீடு->tag);
  இல்லையெனில்
    fprintf (out, "%*s|", (முழு) strlen (previous_lhs->குறியீடு->tag), "");
}

வெறுமை
rule_lhs_print_xml (rule மாறிலி *r, FILE *out, முழு level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->குறியீடு->tag);
}

size_t
rule_rhs_length (rule மாறிலி *r)
{
  size_t res = 0;
  க்கு (item_எண் *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  திரும்பு res;
}

வெறுமை
rule_rhs_print (rule மாறிலி *r, FILE *out)
{
  எனில் (0 <= *r->rhs)
    க்கு (item_எண் *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  இல்லையெனில்
    fprintf (out, " %s", empty);
}

நிலையான வெறுமை
rule_rhs_print_xml (rule மாறிலி *r, FILE *out, முழு level)
{
  எனில் (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      க்கு (item_எண் *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  இல்லையெனில்
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

வெறுமை
rule_print (rule மாறிலி *r, rule மாறிலி *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : வெற்று, out);
  rule_rhs_print (r, out);
}

வெறுமை
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  க்கு (முழு i = 0; i < nritems; ++i)
    {
      எனில் (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      எனில் (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      இல்லையெனில்
        {
          fprintf (out, "  (rule %d)\n", item_எண்_as_rule_எண் (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (வெறுமை)
{
  முழு max = 0;
  க்கு (rule_எண் r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      எனில் (length > max)
        max = length;
    }

  திரும்பு max;
}

வெறுமை
grammar_rules_partial_print (FILE *out, மாறிலி எழுத்து *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = வெற்று;

  /* rule # : LHS -> RHS */
  க்கு (rule_எண் r = 0; r < nrules + nuseless_productions; r++)
    {
      எனில் (filter && !filter (&rules[r]))
        தொடர்;
      எனில் (first)
        fprintf (out, "%s\n\n", title);
      இல்லையெனில் எனில் (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  எனில் (!first)
    fputs ("\n\n", out);
}

வெறுமை
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

வெறுமை
grammar_rules_print_xml (FILE *out, முழு level)
{
  bool first = true;

  க்கு (rule_எண் r = 0; r < nrules + nuseless_productions; r++)
    {
      எனில் (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        எழுத்து மாறிலி *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].எண், usefulness);
        எனில் (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->குறியீடு->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  எனில் (!first)
    xml_puts (out, level + 1, "</rules>");
  இல்லையெனில்
   xml_puts (out, level + 1, "<rules/>");
}

நிலையான வெறுமை
section (FILE *out, மாறிலி எழுத்து *s)
{
  fprintf (out, "%s\n", s);
  க்கு (முழு i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

வெறுமை
grammar_dump (FILE *out, மாறிலி எழுத்து *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    க்கு (குறியீடு_எண் i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    க்கு (rule_எண் i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule மாறிலி *rule_i = &rules[i];
        முழு மாறிலி rhs_itemno = rule_i->rhs - ritem;
        முழு length = rule_rhs_length (rule_i);
        aver (item_எண்_as_rule_எண் (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->எண்,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        க்கு (item_எண் *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  க்கு (rule_எண் r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->குறியீடு->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

வெறுமை
grammar_rules_useless_report (மாறிலி எழுத்து *message)
{
  க்கு (rule_எண் r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    எனில் (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

வெறுமை
grammar_free (வெறுமை)
{
  எனில் (ritem)
    விடுவி (ritem - 1);
  விடுவி (rules);
  விடுவி (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
