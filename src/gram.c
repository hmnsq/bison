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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "complain.h"
#समावेश "getargs.h"
#समावेश "glyphs.h"
#समावेश "gram.h"
#समावेश "print-xml.h"
#समावेश "reader.h"
#समावेश "reduce.h"
#समावेश "symtab.h"

/* Comments for these variables are in gram.h.  */

item_संख्या *ritem = शून्य;
पूर्णांक nritems = 0;

rule *rules = शून्य;
rule_संख्या nrules = 0;

प्रतीक **symbols = शून्य;
पूर्णांक nsyms = 0;
पूर्णांक ntokens = 1;
पूर्णांक nnterms = 0;

प्रतीक_संख्या *token_translations = शून्य;

पूर्णांक max_code = 256;

पूर्णांक required_version = 0;

व्योम
item_print (item_संख्या *item, rule स्थिर *previous_rule, खाता *out)
{
  rule स्थिर *r = item_rule (item);
  rule_lhs_print (r, previous_rule ? previous_rule->lhs : शून्य, out);

  अगर (0 <= *r->rhs)
    {
      // Non-empty rhs.
      क्रम (item_संख्या *sp = r->rhs; sp < item; sp++)
        ख_लिखो (out, " %s", symbols[*sp]->tag);
      ख_लिखो (out, " %s", dot);
      क्रम (item_संख्या *sp = item; 0 <= *sp; ++sp)
        ख_लिखो (out, " %s", symbols[*sp]->tag);
    }
  अन्यथा
    ख_लिखो (out, " %s %s", empty, dot);
}


bool
rule_useful_in_grammar_p (rule स्थिर *r)
{
  वापस r->संख्या < nrules;
}

bool
rule_useless_in_grammar_p (rule स्थिर *r)
{
  वापस !rule_useful_in_grammar_p (r);
}

bool
rule_useless_in_parser_p (rule स्थिर *r)
{
  वापस !r->useful && rule_useful_in_grammar_p (r);
}

bool
rule_useless_chain_p (rule स्थिर *r)
{
  वापस rule_rhs_लंबाई (r) == 1 && !r->action;
}

व्योम
rule_lhs_print (rule स्थिर *r, sym_content स्थिर *previous_lhs, खाता *out)
{
  ख_लिखो (out, "  %3d ", r->संख्या);
  अगर (previous_lhs != r->lhs)
    ख_लिखो (out, "%s:", r->lhs->प्रतीक->tag);
  अन्यथा
    ख_लिखो (out, "%*s|", (पूर्णांक) म_माप (previous_lhs->प्रतीक->tag), "");
}

व्योम
rule_lhs_print_xml (rule स्थिर *r, खाता *out, पूर्णांक level)
{
  xml_printf (out, level, "<lhs>%s</lhs>", r->lhs->प्रतीक->tag);
}

माप_प्रकार
rule_rhs_लंबाई (rule स्थिर *r)
{
  माप_प्रकार res = 0;
  क्रम (item_संख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  वापस res;
}

व्योम
rule_rhs_print (rule स्थिर *r, खाता *out)
{
  अगर (0 <= *r->rhs)
    क्रम (item_संख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      ख_लिखो (out, " %s", symbols[*rhsp]->tag);
  अन्यथा
    ख_लिखो (out, " %s", empty);
}

अटल व्योम
rule_rhs_print_xml (rule स्थिर *r, खाता *out, पूर्णांक level)
{
  अगर (*r->rhs >= 0)
    {
      xml_puts (out, level, "<rhs>");
      क्रम (item_संख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
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

व्योम
rule_print (rule स्थिर *r, rule स्थिर *पूर्व_rule, खाता *out)
{
  rule_lhs_print (r, पूर्व_rule ? पूर्व_rule->lhs : शून्य, out);
  rule_rhs_print (r, out);
}

व्योम
ritem_print (खाता *out)
{
  ख_माला_दो ("RITEM\n", out);
  bool first = true;
  क्रम (पूर्णांक i = 0; i < nritems; ++i)
    {
      अगर (first)
        {
          ख_लिखो (out, "  %d: ", i);
          first = false;
        }
      अगर (ritem[i] >= 0)
        ख_लिखो (out, "  %s", symbols[ritem[i]]->tag);
      अन्यथा
        {
          ख_लिखो (out, "  (rule %d)\n", item_संख्या_as_rule_संख्या (ritem[i]));
          first = true;
        }
    }
  ख_माला_दो ("\n\n", out);
}

माप_प्रकार
ritem_longest_rhs (व्योम)
{
  पूर्णांक max = 0;
  क्रम (rule_संख्या r = 0; r < nrules; ++r)
    {
      माप_प्रकार लंबाई = rule_rhs_लंबाई (&rules[r]);
      अगर (लंबाई > max)
        max = लंबाई;
    }

  वापस max;
}

व्योम
grammar_rules_partial_print (खाता *out, स्थिर अक्षर *title,
                             rule_filter filter)
{
  bool first = true;
  rule *previous_rule = शून्य;

  /* rule # : LHS -> RHS */
  क्रम (rule_संख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      अगर (filter && !filter (&rules[r]))
        जारी;
      अगर (first)
        ख_लिखो (out, "%s\n\n", title);
      अन्यथा अगर (previous_rule && previous_rule->lhs != rules[r].lhs)
        अ_दो ('\n', out);
      first = false;
      rule_print (&rules[r], previous_rule, out);
      अ_दो ('\n', out);
      previous_rule = &rules[r];
    }
  अगर (!first)
    ख_माला_दो ("\n\n", out);
}

व्योम
grammar_rules_print (खाता *out)
{
  grammar_rules_partial_print (out, _("Grammar"), rule_useful_in_grammar_p);
}

व्योम
grammar_rules_print_xml (खाता *out, पूर्णांक level)
{
  bool first = true;

  क्रम (rule_संख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      अगर (first)
        xml_puts (out, level + 1, "<rules>");
      first = false;
      {
        अक्षर स्थिर *usefulness
          = rule_useless_in_grammar_p (&rules[r]) ? "useless-in-grammar"
          : rule_useless_in_parser_p (&rules[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (out, level + 2);
        ख_लिखो (out, "<rule number=\"%d\" usefulness=\"%s\"",
                 rules[r].संख्या, usefulness);
        अगर (rules[r].precsym)
          ख_लिखो (out, " percent_prec=\"%s\"",
                   xml_escape (rules[r].precsym->प्रतीक->tag));
        ख_माला_दो (">\n", out);
      }
      rule_lhs_print_xml (&rules[r], out, level + 3);
      rule_rhs_print_xml (&rules[r], out, level + 3);
      xml_puts (out, level + 2, "</rule>");
    }
  अगर (!first)
    xml_puts (out, level + 1, "</rules>");
  अन्यथा
   xml_puts (out, level + 1, "<rules/>");
}

अटल व्योम
section (खाता *out, स्थिर अक्षर *s)
{
  ख_लिखो (out, "%s\n", s);
  क्रम (पूर्णांक i = म_माप (s); 0 < i; --i)
    अ_दो ('-', out);
  अ_दो ('\n', out);
  अ_दो ('\n', out);
}

व्योम
grammar_dump (खाता *out, स्थिर अक्षर *title)
{
  ख_लिखो (out, "%s\n\n", title);
  ख_लिखो (out,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (out, "Tokens");
  {
    ख_लिखो (out, "Value  Sprec  Sassoc  Tag\n");

    क्रम (प्रतीक_संख्या i = 0; i < ntokens; i++)
      ख_लिखो (out, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->tag);
    ख_लिखो (out, "\n\n");
  }

  section (out, "Nonterminals");
  {
    ख_लिखो (out, "Value  Tag\n");

    क्रम (प्रतीक_संख्या i = ntokens; i < nsyms; i++)
      ख_लिखो (out, "%5d  %s\n",
               i, symbols[i]->tag);
    ख_लिखो (out, "\n\n");
  }

  section (out, "Rules");
  {
    ख_लिखो (out,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    क्रम (rule_संख्या i = 0; i < nrules + nuseless_productions; ++i)
      {
        rule स्थिर *rule_i = &rules[i];
        पूर्णांक स्थिर rhs_itemno = rule_i->rhs - ritem;
        पूर्णांक लंबाई = rule_rhs_लंबाई (rule_i);
        aver (item_संख्या_as_rule_संख्या (rule_i->rhs[लंबाई]) == i);
        ख_लिखो (out, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 rule_i->prec ? rule_i->prec->prec : 0,
                 rule_i->prec ? rule_i->prec->assoc : 0,
                 rule_i->useful ? "t" : "f",
                 rule_useless_chain_p (rule_i) ? "t" : "f",
                 rule_i->lhs->संख्या,
                 rhs_itemno, rhs_itemno + लंबाई - 1);
        /* Dumped the RHS. */
        क्रम (item_संख्या *rhsp = rule_i->rhs; 0 <= *rhsp; ++rhsp)
          ख_लिखो (out, " %3d", *rhsp);
        अ_दो ('\n', out);
      }
  }
  ख_लिखो (out, "\n\n");

  section (out, "Rules interpreted");
  क्रम (rule_संख्या r = 0; r < nrules + nuseless_productions; ++r)
    {
      ख_लिखो (out, "%-5d  %s:", r, rules[r].lhs->प्रतीक->tag);
      rule_rhs_print (&rules[r], out);
      अ_दो ('\n', out);
    }
  ख_लिखो (out, "\n\n");
}

व्योम
grammar_rules_useless_report (स्थिर अक्षर *message)
{
  क्रम (rule_संख्या r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    अगर (!reduce_nonterminal_useless_in_grammar (rules[r].lhs)
        && !rules[r].useful)
      complain (&rules[r].location, Wother, "%s", message);
}

व्योम
grammar_free (व्योम)
{
  अगर (ritem)
    मुक्त (ritem - 1);
  मुक्त (rules);
  मुक्त (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
