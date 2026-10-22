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

توکی_شمېره *ritem = عدم;
صحيح nritems = 0;

قاعده *قواعد = عدم;
قاعده_شمېره nrules = 0;

نښه **symbols = عدم;
صحيح nsyms = 0;
صحيح ntokens = 1;
صحيح nnterms = 0;

نښه_شمېره *token_translations = عدم;

صحيح max_کوډ = 256;

صحيح required_نسخه = 0;

فراغ
توکی_print (توکی_شمېره *توکی, قاعده ثابت *پخوانی_قاعده, FILE *بهر)
{
  قاعده ثابت *r = توکی_قاعده (توکی);
  قاعده_lhs_print (r, پخوانی_قاعده ? پخوانی_قاعده->lhs : عدم, بهر);

  إذا (0 <= *r->rhs)
    {
      // Non-empty rhs.
      لكل (توکی_شمېره *sp = r->rhs; sp < توکی; sp++)
        fprintf (بهر, " %s", symbols[*sp]->نښان);
      fprintf (بهر, " %s", dot);
      لكل (توکی_شمېره *sp = توکی; 0 <= *sp; ++sp)
        fprintf (بهر, " %s", symbols[*sp]->نښان);
    }
  وإلا
    fprintf (بهر, " %s %s", تشه, dot);
}


bool
قاعده_useful_دننه_grammar_p (قاعده ثابت *r)
{
  أرجع r->شمېره < nrules;
}

bool
قاعده_useless_دننه_grammar_p (قاعده ثابت *r)
{
  أرجع !قاعده_useful_دننه_grammar_p (r);
}

bool
قاعده_useless_دننه_parser_p (قاعده ثابت *r)
{
  أرجع !r->useful && قاعده_useful_دننه_grammar_p (r);
}

bool
قاعده_useless_chain_p (قاعده ثابت *r)
{
  أرجع قاعده_rhs_اوږدتیا (r) == 1 && !r->action;
}

فراغ
قاعده_lhs_print (قاعده ثابت *r, sym_content ثابت *پخوانی_lhs, FILE *بهر)
{
  fprintf (بهر, "  %3d ", r->شمېره);
  إذا (پخوانی_lhs != r->lhs)
    fprintf (بهر, "%s:", r->lhs->نښه->نښان);
  وإلا
    fprintf (بهر, "%*s|", (صحيح) مزي_اوږدوالی (پخوانی_lhs->نښه->نښان), "");
}

فراغ
قاعده_lhs_print_xml (قاعده ثابت *r, FILE *بهر, صحيح level)
{
  xml_printf (بهر, level, "<lhs>%s</lhs>", r->lhs->نښه->نښان);
}

size_t
قاعده_rhs_اوږدتیا (قاعده ثابت *r)
{
  size_t res = 0;
  لكل (توکی_شمېره *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  أرجع res;
}

فراغ
قاعده_rhs_print (قاعده ثابت *r, FILE *بهر)
{
  إذا (0 <= *r->rhs)
    لكل (توکی_شمېره *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (بهر, " %s", symbols[*rhsp]->نښان);
  وإلا
    fprintf (بهر, " %s", تشه);
}

ساكن فراغ
قاعده_rhs_print_xml (قاعده ثابت *r, FILE *بهر, صحيح level)
{
  إذا (*r->rhs >= 0)
    {
      xml_puts (بهر, level, "<rhs>");
      لكل (توکی_شمېره *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (بهر, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->نښان));
      xml_puts (بهر, level, "</rhs>");
    }
  وإلا
    {
      xml_puts (بهر, level, "<rhs>");
      xml_puts (بهر, level + 1, "<empty/>");
      xml_puts (بهر, level, "</rhs>");
    }
}

فراغ
قاعده_print (قاعده ثابت *r, قاعده ثابت *prev_قاعده, FILE *بهر)
{
  قاعده_lhs_print (r, prev_قاعده ? prev_قاعده->lhs : عدم, بهر);
  قاعده_rhs_print (r, بهر);
}

فراغ
ritem_print (FILE *بهر)
{
  fputs ("RITEM\n", بهر);
  bool لومړی = true;
  لكل (صحيح i = 0; i < nritems; ++i)
    {
      إذا (لومړی)
        {
          fprintf (بهر, "  %d: ", i);
          لومړی = false;
        }
      إذا (ritem[i] >= 0)
        fprintf (بهر, "  %s", symbols[ritem[i]]->نښان);
      وإلا
        {
          fprintf (بهر, "  (rule %d)\n", توکی_شمېره_as_قاعده_شمېره (ritem[i]));
          لومړی = true;
        }
    }
  fputs ("\n\n", بهر);
}

size_t
ritem_longest_rhs (فراغ)
{
  صحيح max = 0;
  لكل (قاعده_شمېره r = 0; r < nrules; ++r)
    {
      size_t اوږدتیا = قاعده_rhs_اوږدتیا (&قواعد[r]);
      إذا (اوږدتیا > max)
        max = اوږدتیا;
    }

  أرجع max;
}

فراغ
grammar_قواعد_partial_print (FILE *بهر, ثابت حرف *title,
                             قاعده_filter filter)
{
  bool لومړی = true;
  قاعده *پخوانی_قاعده = عدم;

  /* rule # : LHS -> RHS */
  لكل (قاعده_شمېره r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (filter && !filter (&قواعد[r]))
        استمر;
      إذا (لومړی)
        fprintf (بهر, "%s\n\n", title);
      وإلا إذا (پخوانی_قاعده && پخوانی_قاعده->lhs != قواعد[r].lhs)
        putc ('\n', بهر);
      لومړی = false;
      قاعده_print (&قواعد[r], پخوانی_قاعده, بهر);
      putc ('\n', بهر);
      پخوانی_قاعده = &قواعد[r];
    }
  إذا (!لومړی)
    fputs ("\n\n", بهر);
}

فراغ
grammar_قواعد_print (FILE *بهر)
{
  grammar_قواعد_partial_print (بهر, _("Grammar"), قاعده_useful_دننه_grammar_p);
}

فراغ
grammar_قواعد_print_xml (FILE *بهر, صحيح level)
{
  bool لومړی = true;

  لكل (قاعده_شمېره r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (لومړی)
        xml_puts (بهر, level + 1, "<rules>");
      لومړی = false;
      {
        حرف ثابت *usefulness
          = قاعده_useless_دننه_grammar_p (&قواعد[r]) ? "useless-in-grammar"
          : قاعده_useless_دننه_parser_p (&قواعد[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (بهر, level + 2);
        fprintf (بهر, "<rule number=\"%d\" usefulness=\"%s\"",
                 قواعد[r].شمېره, usefulness);
        إذا (قواعد[r].precsym)
          fprintf (بهر, " percent_prec=\"%s\"",
                   xml_escape (قواعد[r].precsym->نښه->نښان));
        fputs (">\n", بهر);
      }
      قاعده_lhs_print_xml (&قواعد[r], بهر, level + 3);
      قاعده_rhs_print_xml (&قواعد[r], بهر, level + 3);
      xml_puts (بهر, level + 2, "</rule>");
    }
  إذا (!لومړی)
    xml_puts (بهر, level + 1, "</rules>");
  وإلا
   xml_puts (بهر, level + 1, "<rules/>");
}

ساكن فراغ
section (FILE *بهر, ثابت حرف *s)
{
  fprintf (بهر, "%s\n", s);
  لكل (صحيح i = مزي_اوږدوالی (s); 0 < i; --i)
    putc ('-', بهر);
  putc ('\n', بهر);
  putc ('\n', بهر);
}

فراغ
grammar_dump (FILE *بهر, ثابت حرف *title)
{
  fprintf (بهر, "%s\n\n", title);
  fprintf (بهر,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (بهر, "Tokens");
  {
    fprintf (بهر, "Value  Sprec  Sassoc  Tag\n");

    لكل (نښه_شمېره i = 0; i < ntokens; i++)
      fprintf (بهر, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->نښان);
    fprintf (بهر, "\n\n");
  }

  section (بهر, "Nonterminals");
  {
    fprintf (بهر, "Value  Tag\n");

    لكل (نښه_شمېره i = ntokens; i < nsyms; i++)
      fprintf (بهر, "%5d  %s\n",
               i, symbols[i]->نښان);
    fprintf (بهر, "\n\n");
  }

  section (بهر, "Rules");
  {
    fprintf (بهر,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    لكل (قاعده_شمېره i = 0; i < nrules + nuseless_productions; ++i)
      {
        قاعده ثابت *قاعده_i = &قواعد[i];
        صحيح ثابت rhs_itemno = قاعده_i->rhs - ritem;
        صحيح اوږدتیا = قاعده_rhs_اوږدتیا (قاعده_i);
        aver (توکی_شمېره_as_قاعده_شمېره (قاعده_i->rhs[اوږدتیا]) == i);
        fprintf (بهر, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 قاعده_i->prec ? قاعده_i->prec->prec : 0,
                 قاعده_i->prec ? قاعده_i->prec->assoc : 0,
                 قاعده_i->useful ? "t" : "f",
                 قاعده_useless_chain_p (قاعده_i) ? "t" : "f",
                 قاعده_i->lhs->شمېره,
                 rhs_itemno, rhs_itemno + اوږدتیا - 1);
        /* Dumped the RHS. */
        لكل (توکی_شمېره *rhsp = قاعده_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (بهر, " %3d", *rhsp);
        putc ('\n', بهر);
      }
  }
  fprintf (بهر, "\n\n");

  section (بهر, "Rules interpreted");
  لكل (قاعده_شمېره r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (بهر, "%-5d  %s:", r, قواعد[r].lhs->نښه->نښان);
      قاعده_rhs_print (&قواعد[r], بهر);
      putc ('\n', بهر);
    }
  fprintf (بهر, "\n\n");
}

فراغ
grammar_قواعد_useless_report (ثابت حرف *پیغام)
{
  لكل (قاعده_شمېره r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    إذا (!reduce_nonterminal_useless_دننه_grammar (قواعد[r].lhs)
        && !قواعد[r].useful)
      complain (&قواعد[r].location, Wother, "%s", پیغام);
}

فراغ
grammar_free (فراغ)
{
  إذا (ritem)
    حرر (ritem - 1);
  حرر (قواعد);
  حرر (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
