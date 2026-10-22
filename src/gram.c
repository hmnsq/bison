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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "complain.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "glyphs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "print-xml.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "symtab.h"

/* Comments for these variables are in gram.h.  */

item_सङ्ख्या *ritem = रिक्त;
पूर्णाङ्क nritems = 0;

rule *rules = रिक्त;
rule_सङ्ख्या nrules = 0;

चिह्नम् **symbols = रिक्त;
पूर्णाङ्क nsyms = 0;
पूर्णाङ्क ntokens = 1;
पूर्णाङ्क nnterms = 0;

चिह्नम्_सङ्ख्या *token_translations = रिक्त;

पूर्णाङ्क max_code = 256;

पूर्णाङ्क required_version = 0;

शून्य
item_print (item_सङ्ख्या *item, rule ध्रुव *previous_rule, FILE *out)
{
  rule ध्रुव *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : रिक्त, out);

  जइ (0 <= *r->rhs)
    {
      // Non-empty rhs.
      प्रति (item_सङ्ख्या *sp = r->rhs; sp < item; sp++)
        fprintf (out, " %s", symbols[*sp]->tag);
      fprintf (out, " %s", dot);
      प्रति (item_सङ्ख्या *sp = item; 0 <= *sp; ++sp)
        fprintf (out, " %s", symbols[*sp]->tag);
    }
  अण्णहा
    fprintf (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule ध्रुव *r)
{
  प्रत्यागम r->सङ्ख्या < nrules;
}

bool
rule_useless_in_grammar_p (rule ध्रुव *r)
{
  प्रत्यागम !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule ध्रुव *r)
{
  प्रत्यागम !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule ध्रुव *r)
{
  प्रत्यागम rule_rhs_length (r) == 1 && !r->action;
}

शून्य
rule_lhs_print (rule ध्रुव *r, sym_content ध्रुव *previous_lhs, FILE *out)
{
  fprintf (out, "  %3d ", r->सङ्ख्या);
  जइ (previous_lhs != r->lhs)
    fprintf (out, "%s:", r->lhs->चिह्नम्->tag);
  अण्णहा
    fprintf (out, "%*s|", (पूर्णाङ्क) strlen (previous_lhs->चिह्नम्->tag), "");
}

शून्य
rule_lhs_print_xml (rule ध्रुव *r, FILE *out, पूर्णाङ्क level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->चिह्नम्->tag);
}

size_t
rule_rhs_length (rule ध्रुव *r)
{
  size_t res = 0;
  प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  प्रत्यागम res;
}

शून्य
rule_rhs_print (rule ध्रुव *r, FILE *out)
{
  जइ (0 <= *r->rhs)
    प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (out, " %s", symbols[*rhsp]->tag);
  अण्णहा
    fprintf (out, " %s", empty);
}

स्थिर शून्य
rule_rhs_print_xml (rule ध्रुव *r, FILE *out, पूर्णाङ्क level)
{
  जइ (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  अण्णहा
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

शून्य
rule_print (rule ध्रुव *r, rule ध्रुव *prev_rule, FILE *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : रिक्त, out);
  rule_rhs_print (r, out);
}

शून्य
ritem_print (FILE *out)
{
  fputs ("RITEM\n", out);
  bool first = true;
  प्रति (पूर्णाङ्क i = 0; i < nritems; ++i)
    {
      जइ (first)
        {
          fprintf (out, "  %d: ", i);
          first = false;
        }
      जइ (ritem[i] >= 0)
        fprintf (out, "  %s", symbols[ritem[i]]->tag);
      अण्णहा
        {
          fprintf (out, "  (rule %d)\n", item_सङ्ख्या_as_rule_सङ्ख्या (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", out);
}

size_t
ritem_longest_rhs (शून्य)
{
  पूर्णाङ्क max = 0;
  प्रति (rule_सङ्ख्या r = 0; r < nrules; ++r)
    {
      size_t length = rule_rhs_length (&rules[r]);
      जइ (length > max)
        max = length;
    }

  प्रत्यागम max;
}

शून्य
grammar_rules_partial_print (FILE *out, ध्रुव वर्ण *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = रिक्त;

  /* rule # : LHS -> RHS */
  प्रति (rule_सङ्ख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      जइ (filter && !filter (&rules[r]))
        अनुवर्त;
      जइ (first)
        fprintf (out, "%s\n\n", title);
      अण्णहा जइ (previous_rule && previous_rule->lhs != rules[r].lhs)
        putc ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      putc ('\n', out);
      previous_rule = &rules[r];
    }
  जइ (!first)
    fputs ("\n\n", out);
}

शून्य
grammar_rules_print (FILE *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

शून्य
grammar_rules_print_xml (FILE *out, पूर्णाङ्क level)
{
  bool first = true;

  प्रति (rule_सङ्ख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      जइ (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        वर्ण ध्रुव *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        fprintf (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].सङ्ख्या, usefulness);
        जइ (rules[r].precsym)
          fprintf (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->चिह्नम्->tag));
        fputs (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  जइ (!first)
    xml_puts (out, level + 1, "</rules>");
  अण्णहा
   xml_puts (out, level + 1, "<rules/>");
}

स्थिर शून्य
section (FILE *out, ध्रुव वर्ण *s)
{
  fprintf (out, "%s\n", s);
  प्रति (पूर्णाङ्क i = strlen (s); 0 < i; --i)
    putc ('-', out);
  putc ('\n', out);
  putc ('\n', out);
}

शून्य
grammar_dump (FILE *out, ध्रुव वर्ण *title)
{
  fprintf (out, "%s\n\n", title);
  fprintf (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    fprintf (out, "Value  Sprec  Sassoc  Tag\n");

    प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; i++)
      fprintf (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    fprintf (out, "Value  Tag\n");

    प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; i++)
      fprintf (out, "%5d  %s\n",
               i, symbols[i]->tag);
    fprintf (out, "\n\n");
  }

  section (out, "Rules");
  {
    fprintf (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    प्रति (rule_सङ्ख्या i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule ध्रुव *rule_i = &rules[i];
        पूर्णाङ्क ध्रुव rhs_itemno = rule_i->rhs - ritem;
        पूर्णाङ्क length = rule_rhs_length (rule_i);
        aver (item_सङ्ख्या_as_rule_सङ्ख्या (rule_i->rhs[length]) == i);
        fprintf (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->सङ्ख्या,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        प्रति (item_सङ्ख्या *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (out, " %3d", *rhsp);
        putc ('\n', out);
      }
  }
  fprintf (out, "\n\n");

  section (out, "Rules interpreted");
  प्रति (rule_सङ्ख्या r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (out, "%-5d  %s:", r, rules[r].lhs->चिह्नम्->tag);
      rule_rhs_print (&rules[r], out);
      putc ('\n', out);
    }
  fprintf (out, "\n\n");
}

शून्य
grammar_rules_useless_report (ध्रुव वर्ण *message)
{
  प्रति (rule_सङ्ख्या r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    जइ (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

शून्य
grammar_free (शून्य)
{
  जइ (ritem)
    मुक्त (ritem - 1);
  मुक्त (rules);
  मुक्त (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
