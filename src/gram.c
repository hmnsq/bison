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

चिह्न **symbols = रिक्त;
पूर्णाङ्क nsyms = 0;
पूर्णाङ्क ntokens = 1;
पूर्णाङ्क nnterms = 0;

चिह्न_सङ्ख्या *token_translations = रिक्त;

पूर्णाङ्क max_code = 256;

पूर्णाङ्क required_version = 0;

शून्य
item_print (item_सङ्ख्या *item, rule अचर *previous_rule, खाता *out)
{
  rule अचर *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : रिक्त, out);

  यदि (0 <= *r->rhs)
    {
      // Non-empty rhs.
      लागि (item_सङ्ख्या *sp = r->rhs; sp < item; sp++)
        ख_लिखो (out, " %s", symbols[*sp]->tag);
      ख_लिखो (out, " %s", dot);
      लागि (item_सङ्ख्या *sp = item; 0 <= *sp; ++sp)
        ख_लिखो (out, " %s", symbols[*sp]->tag);
    }
  अन्यथा
    ख_लिखो (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule अचर *r)
{
  फर्काऊ r->सङ्ख्या < nrules;
}

bool
rule_useless_in_grammar_p (rule अचर *r)
{
  फर्काऊ !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule अचर *r)
{
  फर्काऊ !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule अचर *r)
{
  फर्काऊ rule_rhs_length (r) == 1 && !r->action;
}

शून्य
rule_lhs_print (rule अचर *r, sym_content अचर *previous_lhs, खाता *out)
{
  ख_लिखो (out, "  %3d ", r->सङ्ख्या);
  यदि (previous_lhs != r->lhs)
    ख_लिखो (out, "%s:", r->lhs->चिह्न->tag);
  अन्यथा
    ख_लिखो (out, "%*s|", (पूर्णाङ्क) म_माप (previous_lhs->चिह्न->tag), "");
}

शून्य
rule_lhs_print_xml (rule अचर *r, खाता *out, पूर्णाङ्क level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->चिह्न->tag);
}

माप_प्रकार
rule_rhs_length (rule अचर *r)
{
  माप_प्रकार res = 0;
  लागि (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  फर्काऊ res;
}

शून्य
rule_rhs_print (rule अचर *r, खाता *out)
{
  यदि (0 <= *r->rhs)
    लागि (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      ख_लिखो (out, " %s", symbols[*rhsp]->tag);
  अन्यथा
    ख_लिखो (out, " %s", empty);
}

स्थिर शून्य
rule_rhs_print_xml (rule अचर *r, खाता *out, पूर्णाङ्क level)
{
  यदि (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      लागि (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (out, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->tag));
      xml_puts (out, level, "</rhs>");
    }
  अन्यथा
    {
      xml_puts (out, level, "<rhs>");
      xml_puts (out, level + 1, "<empty/>");
      xml_puts (out, level, "</rhs>");
    }
}

शून्य
rule_print (rule अचर *r, rule अचर *prev_rule, खाता *out)
{
  rule_lhs_print (r, prev_rule ? prev_rule->lhs : रिक्त, out);
  rule_rhs_print (r, out);
}

शून्य
ritem_print (खाता *out)
{
  ख_माला_दो ("RITEM\n", out);
  bool first = true;
  लागि (पूर्णाङ्क i = 0; i < nritems; ++i)
    {
      यदि (first)
        {
          ख_लिखो (out, "  %d: ", i);
          first = false;
        }
      यदि (ritem[i] >= 0)
        ख_लिखो (out, "  %s", symbols[ritem[i]]->tag);
      अन्यथा
        {
          ख_लिखो (out, "  (rule %d)\n", item_सङ्ख्या_as_rule_सङ्ख्या (ritem[i]));
          first = true;
        }
    }
  ख_माला_दो ("\n\n", out);
}

माप_प्रकार
ritem_longest_rhs (शून्य)
{
  पूर्णाङ्क max = 0;
  लागि (rule_सङ्ख्या r = 0; r < nrules; ++r)
    {
      माप_प्रकार length = rule_rhs_length (&rules[r]);
      यदि (length > max)
        max = length;
    }

  फर्काऊ max;
}

शून्य
grammar_rules_partial_print (खाता *out, अचर वर्ण *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = रिक्त;

  /* rule # : LHS -> RHS */
  लागि (rule_सङ्ख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      यदि (filter && !filter (&rules[r]))
        जारी;
      यदि (first)
        ख_लिखो (out, "%s\n\n", title);
      अन्यथा यदि (previous_rule && previous_rule->lhs != rules[r].lhs)
        अ_दो ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      अ_दो ('\n', out);
      previous_rule = &rules[r];
    }
  यदि (!first)
    ख_माला_दो ("\n\n", out);
}

शून्य
grammar_rules_print (खाता *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

शून्य
grammar_rules_print_xml (खाता *out, पूर्णाङ्क level)
{
  bool first = true;

  लागि (rule_सङ्ख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      यदि (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        वर्ण अचर *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        ख_लिखो (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].सङ्ख्या, usefulness);
        यदि (rules[r].precsym)
          ख_लिखो (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->चिह्न->tag));
        ख_माला_दो (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  यदि (!first)
    xml_puts (out, level + 1, "</rules>");
  अन्यथा
   xml_puts (out, level + 1, "<rules/>");
}

स्थिर शून्य
section (खाता *out, अचर वर्ण *s)
{
  ख_लिखो (out, "%s\n", s);
  लागि (पूर्णाङ्क i = म_माप (s); 0 < i; --i)
    अ_दो ('-', out);
  अ_दो ('\n', out);
  अ_दो ('\n', out);
}

शून्य
grammar_dump (खाता *out, अचर वर्ण *title)
{
  ख_लिखो (out, "%s\n\n", title);
  ख_लिखो (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    ख_लिखो (out, "Value  Sprec  Sassoc  Tag\n");

    लागि (चिह्न_सङ्ख्या i = 0; i < ntokens; i++)
      ख_लिखो (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    ख_लिखो (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    ख_लिखो (out, "Value  Tag\n");

    लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; i++)
      ख_लिखो (out, "%5d  %s\n",
               i, symbols[i]->tag);
    ख_लिखो (out, "\n\n");
  }

  section (out, "Rules");
  {
    ख_लिखो (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    लागि (rule_सङ्ख्या i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule अचर *rule_i = &rules[i];
        पूर्णाङ्क अचर rhs_itemno = rule_i->rhs - ritem;
        पूर्णाङ्क length = rule_rhs_length (rule_i);
        aver (item_सङ्ख्या_as_rule_सङ्ख्या (rule_i->rhs[length]) == i);
        ख_लिखो (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->सङ्ख्या,
                 rhs_itemno, rhs_itemno + length - 1);
        /* Dumped the RHS. */
        लागि (item_सङ्ख्या *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          ख_लिखो (out, " %3d", *rhsp);
        अ_दो ('\n', out);
      }
  }
  ख_लिखो (out, "\n\n");

  section (out, "Rules interpreted");
  लागि (rule_सङ्ख्या r = 0; r < nrules + nuseless_productions; ++r)
    {
      ख_लिखो (out, "%-5d  %s:", r, rules[r].lhs->चिह्न->tag);
      rule_rhs_print (&rules[r], out);
      अ_दो ('\n', out);
    }
  ख_लिखो (out, "\n\n");
}

शून्य
grammar_rules_useless_report (अचर वर्ण *message)
{
  लागि (rule_सङ्ख्या r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    यदि (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

शून्य
grammar_free (शून्य)
{
  यदि (ritem)
    मुक्त (ritem - 1);
  मुक्त (rules);
  मुक्त (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
