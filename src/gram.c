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

नियमः *नियमाः = रिक्त;
नियमः_सङ्ख्या nrules = 0;

चिह्नम् **symbols = रिक्त;
पूर्णाङ्क nsyms = 0;
पूर्णाङ्क ntokens = 1;
पूर्णाङ्क nnterms = 0;

चिह्नम्_सङ्ख्या *token_translations = रिक्त;

पूर्णाङ्क max_संहिता = 256;

पूर्णाङ्क required_संस्करणम् = 0;

शून्य
item_print (item_सङ्ख्या *item, नियमः ध्रुव *previous_नियमः, FILE *बहिर्गमनम्)
{
  नियमः ध्रुव *r = item_नियमः (item);
  नियमः_lhs_print (r, previous_नियमः ? previous_नियमः->lhs : रिक्त, बहिर्गमनम्);

  यदि (0 <= *r->rhs)
    {
      // Non-empty rhs.
      प्रति (item_सङ्ख्या *sp = r->rhs; sp < item; sp++)
        fprintf (बहिर्गमनम्, " %s", symbols[*sp]->चिह्नकम्);
      fprintf (बहिर्गमनम्, " %s", dot);
      प्रति (item_सङ्ख्या *sp = item; 0 <= *sp; ++sp)
        fprintf (बहिर्गमनम्, " %s", symbols[*sp]->चिह्नकम्);
    }
  अन्यथा
    fprintf (बहिर्गमनम्, " %s %s", रिक्तानि, dot);
}


bool
नियमः_useful_अन्तर्_grammar_p (नियमः ध्रुव *r)
{
  प्रत्यागम r->सङ्ख्या < nrules;
}

bool
नियमः_useless_अन्तर्_grammar_p (नियमः ध्रुव *r)
{
  प्रत्यागम !नियमः_useful_अन्तर्_grammar_p (r);
}

bool
नियमः_useless_अन्तर्_parser_p (नियमः ध्रुव *r)
{
  प्रत्यागम !r->useful && नियमः_useful_अन्तर्_grammar_p (r);
}

bool
नियमः_useless_chain_p (नियमः ध्रुव *r)
{
  प्रत्यागम नियमः_rhs_दीर्घता (r) == 1 && !r->action;
}

शून्य
नियमः_lhs_print (नियमः ध्रुव *r, sym_content ध्रुव *previous_lhs, FILE *बहिर्गमनम्)
{
  fprintf (बहिर्गमनम्, "  %3d ", r->सङ्ख्या);
  यदि (previous_lhs != r->lhs)
    fprintf (बहिर्गमनम्, "%s:", r->lhs->चिह्नम्->चिह्नकम्);
  अन्यथा
    fprintf (बहिर्गमनम्, "%*s|", (पूर्णाङ्क) वाक्यदैर्घ्यम् (previous_lhs->चिह्नम्->चिह्नकम्), "");
}

शून्य
नियमः_lhs_print_xml (नियमः ध्रुव *r, FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  xml_printf (बहिर्गमनम्, level, "<lhs>%s</lhs>", r->lhs->चिह्नम्->चिह्नकम्);
}

size_t
नियमः_rhs_दीर्घता (नियमः ध्रुव *r)
{
  size_t res = 0;
  प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  प्रत्यागम res;
}

शून्य
नियमः_rhs_print (नियमः ध्रुव *r, FILE *बहिर्गमनम्)
{
  यदि (0 <= *r->rhs)
    प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (बहिर्गमनम्, " %s", symbols[*rhsp]->चिह्नकम्);
  अन्यथा
    fprintf (बहिर्गमनम्, " %s", रिक्तानि);
}

स्थिर शून्य
नियमः_rhs_print_xml (नियमः ध्रुव *r, FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  यदि (*r->rhs >= 0)
    {
      xml_puts (बहिर्गमनम्, level, "<rhs>");
      प्रति (item_सङ्ख्या *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (बहिर्गमनम्, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->चिह्नकम्));
      xml_puts (बहिर्गमनम्, level, "</rhs>");
    }
  अन्यथा
    {
      xml_puts (बहिर्गमनम्, level, "<rhs>");
      xml_puts (बहिर्गमनम्, level + 1, "<empty/>");
      xml_puts (बहिर्गमनम्, level, "</rhs>");
    }
}

शून्य
नियमः_print (नियमः ध्रुव *r, नियमः ध्रुव *पूर्व_नियमः, FILE *बहिर्गमनम्)
{
  नियमः_lhs_print (r, पूर्व_नियमः ? पूर्व_नियमः->lhs : रिक्त, बहिर्गमनम्);
  नियमः_rhs_print (r, बहिर्गमनम्);
}

शून्य
ritem_print (FILE *बहिर्गमनम्)
{
  fputs ("RITEM\n", बहिर्गमनम्);
  bool first = true;
  प्रति (पूर्णाङ्क i = 0; i < nritems; ++i)
    {
      यदि (first)
        {
          fprintf (बहिर्गमनम्, "  %d: ", i);
          first = false;
        }
      यदि (ritem[i] >= 0)
        fprintf (बहिर्गमनम्, "  %s", symbols[ritem[i]]->चिह्नकम्);
      अन्यथा
        {
          fprintf (बहिर्गमनम्, "  (rule %d)\n", item_सङ्ख्या_as_नियमः_सङ्ख्या (ritem[i]));
          first = true;
        }
    }
  fputs ("\n\n", बहिर्गमनम्);
}

size_t
ritem_longest_rhs (शून्य)
{
  पूर्णाङ्क max = 0;
  प्रति (नियमः_सङ्ख्या r = 0; r < nrules; ++r)
    {
      size_t दीर्घता = नियमः_rhs_दीर्घता (&नियमाः[r]);
      यदि (दीर्घता > max)
        max = दीर्घता;
    }

  प्रत्यागम max;
}

शून्य
grammar_नियमाः_partial_print (FILE *बहिर्गमनम्, ध्रुव वर्ण *title,
                             नियमः_filter filter)
{
  bool first = true;
  नियमः *previous_नियमः = रिक्त;

  /* rule # : LHS -> RHS */
  प्रति (नियमः_सङ्ख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      यदि (filter && !filter (&नियमाः[r]))
        अनुवर्त;
      यदि (first)
        fprintf (बहिर्गमनम्, "%s\n\n", title);
      अन्यथा यदि (previous_नियमः && previous_नियमः->lhs != नियमाः[r].lhs)
        putc ('\n', बहिर्गमनम्);
      first = false;
      नियमः_print (&नियमाः[r], previous_नियमः, बहिर्गमनम्);
      putc ('\n', बहिर्गमनम्);
      previous_नियमः = &नियमाः[r];
    }
  यदि (!first)
    fputs ("\n\n", बहिर्गमनम्);
}

शून्य
grammar_नियमाः_print (FILE *बहिर्गमनम्)
{
  grammar_नियमाः_partial_print (बहिर्गमनम्, _("Grammar"), नियमः_useful_अन्तर्_grammar_p);
}

शून्य
grammar_नियमाः_print_xml (FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  bool first = true;

  प्रति (नियमः_सङ्ख्या r = 0; r < nrules + nuseless_productions; r++)
    {
      यदि (first)
        xml_puts (बहिर्गमनम्, level + 1, "<rules>");
      first = false;
      {
        वर्ण ध्रुव *usefulness
          = नियमः_useless_अन्तर्_grammar_p (&नियमाः[r]) ? "useless-in-grammar"
          : नियमः_useless_अन्तर्_parser_p (&नियमाः[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (बहिर्गमनम्, level + 2);
        fprintf (बहिर्गमनम्, "<rule number=\"%d\" usefulness=\"%s\"",
                 नियमाः[r].सङ्ख्या, usefulness);
        यदि (नियमाः[r].precsym)
          fprintf (बहिर्गमनम्, " percent_prec=\"%s\"",
                   xml_escape (नियमाः[r].precsym->चिह्नम्->चिह्नकम्));
        fputs (">\n", बहिर्गमनम्);
      }
      नियमः_lhs_print_xml (&नियमाः[r], बहिर्गमनम्, level + 3);
      नियमः_rhs_print_xml (&नियमाः[r], बहिर्गमनम्, level + 3);
      xml_puts (बहिर्गमनम्, level + 2, "</rule>");
    }
  यदि (!first)
    xml_puts (बहिर्गमनम्, level + 1, "</rules>");
  अन्यथा
   xml_puts (बहिर्गमनम्, level + 1, "<rules/>");
}

स्थिर शून्य
section (FILE *बहिर्गमनम्, ध्रुव वर्ण *s)
{
  fprintf (बहिर्गमनम्, "%s\n", s);
  प्रति (पूर्णाङ्क i = वाक्यदैर्घ्यम् (s); 0 < i; --i)
    putc ('-', बहिर्गमनम्);
  putc ('\n', बहिर्गमनम्);
  putc ('\n', बहिर्गमनम्);
}

शून्य
grammar_dump (FILE *बहिर्गमनम्, ध्रुव वर्ण *title)
{
  fprintf (बहिर्गमनम्, "%s\n\n", title);
  fprintf (बहिर्गमनम्,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (बहिर्गमनम्, "Tokens");
  {
    fprintf (बहिर्गमनम्, "Value  Sprec  Sassoc  Tag\n");

    प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; i++)
      fprintf (बहिर्गमनम्, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->चिह्नकम्);
    fprintf (बहिर्गमनम्, "\n\n");
  }

  section (बहिर्गमनम्, "Nonterminals");
  {
    fprintf (बहिर्गमनम्, "Value  Tag\n");

    प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; i++)
      fprintf (बहिर्गमनम्, "%5d  %s\n",
               i, symbols[i]->चिह्नकम्);
    fprintf (बहिर्गमनम्, "\n\n");
  }

  section (बहिर्गमनम्, "Rules");
  {
    fprintf (बहिर्गमनम्,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    प्रति (नियमः_सङ्ख्या i = 0; i < nrules + nuseless_productions; ++i)
      {
        नियमः ध्रुव *नियमः_i = &नियमाः[i];
        पूर्णाङ्क ध्रुव rhs_itemno = नियमः_i->rhs - ritem;
        पूर्णाङ्क दीर्घता = नियमः_rhs_दीर्घता (नियमः_i);
        aver (item_सङ्ख्या_as_नियमः_सङ्ख्या (नियमः_i->rhs[दीर्घता]) == i);
        fprintf (बहिर्गमनम्, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 नियमः_i->prec ? नियमः_i->prec->prec : 0,
                 नियमः_i->prec ? नियमः_i->prec->assoc : 0,
                 नियमः_i->useful ? "t" : "f",
                 नियमः_useless_chain_p (नियमः_i) ? "t" : "f",
                 नियमः_i->lhs->सङ्ख्या,
                 rhs_itemno, rhs_itemno + दीर्घता - 1);
        /* Dumped the RHS. */
        प्रति (item_सङ्ख्या *rhsp = नियमः_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (बहिर्गमनम्, " %3d", *rhsp);
        putc ('\n', बहिर्गमनम्);
      }
  }
  fprintf (बहिर्गमनम्, "\n\n");

  section (बहिर्गमनम्, "Rules interpreted");
  प्रति (नियमः_सङ्ख्या r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (बहिर्गमनम्, "%-5d  %s:", r, नियमाः[r].lhs->चिह्नम्->चिह्नकम्);
      नियमः_rhs_print (&नियमाः[r], बहिर्गमनम्);
      putc ('\n', बहिर्गमनम्);
    }
  fprintf (बहिर्गमनम्, "\n\n");
}

शून्य
grammar_नियमाः_useless_report (ध्रुव वर्ण *सन्देशः)
{
  प्रति (नियमः_सङ्ख्या r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    यदि (!reduce_nonterminal_useless_अन्तर्_grammar (नियमाः[r].lhs)
        && !नियमाः[r].useful)
      complain (&नियमाः[r].location, Wother, "%s", सन्देशः);
}

शून्य
grammar_free (शून्य)
{
  यदि (ritem)
    मुक्त (ritem - 1);
  मुक्त (नियमाः);
  मुक्त (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
