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

شے_نمبر *ritem = عدم;
صحيح nritems = 0;

قاعدہ *قاعدے = عدم;
قاعدہ_نمبر nrules = 0;

علامت **symbols = عدم;
صحيح nsyms = 0;
صحيح ntokens = 1;
صحيح nnterms = 0;

علامت_نمبر *token_translations = عدم;

صحيح max_کوڈ = 256;

صحيح required_نسخہ = 0;

فراغ
شے_print (شے_نمبر *شے, قاعدہ ثابت *پچھلا_قاعدہ, FILE *باہرلا)
{
  قاعدہ ثابت *r = شے_قاعدہ (شے);
  قاعدہ_lhs_print (r, پچھلا_قاعدہ ? پچھلا_قاعدہ->lhs : عدم, باہرلا);

  إذا (0 <= *r->rhs)
    {
      // Non-empty rhs.
      لكل (شے_نمبر *sp = r->rhs; sp < شے; sp++)
        fprintf (باہرلا, " %s", symbols[*sp]->ٹیگ);
      fprintf (باہرلا, " %s", dot);
      لكل (شے_نمبر *sp = شے; 0 <= *sp; ++sp)
        fprintf (باہرلا, " %s", symbols[*sp]->ٹیگ);
    }
  وإلا
    fprintf (باہرلا, " %s %s", خالی_تھاواں, dot);
}


bool
قاعدہ_useful_وچ_grammar_p (قاعدہ ثابت *r)
{
  أرجع r->نمبر < nrules;
}

bool
قاعدہ_useless_وچ_grammar_p (قاعدہ ثابت *r)
{
  أرجع !قاعدہ_useful_وچ_grammar_p (r);
}

bool
قاعدہ_useless_وچ_parser_p (قاعدہ ثابت *r)
{
  أرجع !r->useful && قاعدہ_useful_وچ_grammar_p (r);
}

bool
قاعدہ_useless_chain_p (قاعدہ ثابت *r)
{
  أرجع قاعدہ_rhs_لمائی (r) == 1 && !r->action;
}

فراغ
قاعدہ_lhs_print (قاعدہ ثابت *r, sym_content ثابت *پچھلا_lhs, FILE *باہرلا)
{
  fprintf (باہرلا, "  %3d ", r->نمبر);
  إذا (پچھلا_lhs != r->lhs)
    fprintf (باہرلا, "%s:", r->lhs->علامت->ٹیگ);
  وإلا
    fprintf (باہرلا, "%*s|", (صحيح) لڑی_لمبائی (پچھلا_lhs->علامت->ٹیگ), "");
}

فراغ
قاعدہ_lhs_print_xml (قاعدہ ثابت *r, FILE *باہرلا, صحيح level)
{
  xml_printf (باہرلا, level, "<lhs>%s</lhs>", r->lhs->علامت->ٹیگ);
}

size_t
قاعدہ_rhs_لمائی (قاعدہ ثابت *r)
{
  size_t res = 0;
  لكل (شے_نمبر *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  أرجع res;
}

فراغ
قاعدہ_rhs_print (قاعدہ ثابت *r, FILE *باہرلا)
{
  إذا (0 <= *r->rhs)
    لكل (شے_نمبر *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (باہرلا, " %s", symbols[*rhsp]->ٹیگ);
  وإلا
    fprintf (باہرلا, " %s", خالی_تھاواں);
}

ساكن فراغ
قاعدہ_rhs_print_xml (قاعدہ ثابت *r, FILE *باہرلا, صحيح level)
{
  إذا (*r->rhs >= 0)
    {
      xml_puts (باہرلا, level, "<rhs>");
      لكل (شے_نمبر *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (باہرلا, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->ٹیگ));
      xml_puts (باہرلا, level, "</rhs>");
    }
  وإلا
    {
      xml_puts (باہرلا, level, "<rhs>");
      xml_puts (باہرلا, level + 1, "<empty/>");
      xml_puts (باہرلا, level, "</rhs>");
    }
}

فراغ
قاعدہ_print (قاعدہ ثابت *r, قاعدہ ثابت *prev_قاعدہ, FILE *باہرلا)
{
  قاعدہ_lhs_print (r, prev_قاعدہ ? prev_قاعدہ->lhs : عدم, باہرلا);
  قاعدہ_rhs_print (r, باہرلا);
}

فراغ
ritem_print (FILE *باہرلا)
{
  fputs ("RITEM\n", باہرلا);
  bool پہلا = true;
  لكل (صحيح i = 0; i < nritems; ++i)
    {
      إذا (پہلا)
        {
          fprintf (باہرلا, "  %d: ", i);
          پہلا = false;
        }
      إذا (ritem[i] >= 0)
        fprintf (باہرلا, "  %s", symbols[ritem[i]]->ٹیگ);
      وإلا
        {
          fprintf (باہرلا, "  (rule %d)\n", شے_نمبر_as_قاعدہ_نمبر (ritem[i]));
          پہلا = true;
        }
    }
  fputs ("\n\n", باہرلا);
}

size_t
ritem_longest_rhs (فراغ)
{
  صحيح max = 0;
  لكل (قاعدہ_نمبر r = 0; r < nrules; ++r)
    {
      size_t لمائی = قاعدہ_rhs_لمائی (&قاعدے[r]);
      إذا (لمائی > max)
        max = لمائی;
    }

  أرجع max;
}

فراغ
grammar_قاعدے_partial_print (FILE *باہرلا, ثابت حرف *title,
                             قاعدہ_filter filter)
{
  bool پہلا = true;
  قاعدہ *پچھلا_قاعدہ = عدم;

  /* rule # : LHS -> RHS */
  لكل (قاعدہ_نمبر r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (filter && !filter (&قاعدے[r]))
        استمر;
      إذا (پہلا)
        fprintf (باہرلا, "%s\n\n", title);
      وإلا إذا (پچھلا_قاعدہ && پچھلا_قاعدہ->lhs != قاعدے[r].lhs)
        putc ('\n', باہرلا);
      پہلا = false;
      قاعدہ_print (&قاعدے[r], پچھلا_قاعدہ, باہرلا);
      putc ('\n', باہرلا);
      پچھلا_قاعدہ = &قاعدے[r];
    }
  إذا (!پہلا)
    fputs ("\n\n", باہرلا);
}

فراغ
grammar_قاعدے_print (FILE *باہرلا)
{
  grammar_قاعدے_partial_print (باہرلا, _("Grammar"), قاعدہ_useful_وچ_grammar_p);
}

فراغ
grammar_قاعدے_print_xml (FILE *باہرلا, صحيح level)
{
  bool پہلا = true;

  لكل (قاعدہ_نمبر r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (پہلا)
        xml_puts (باہرلا, level + 1, "<rules>");
      پہلا = false;
      {
        حرف ثابت *usefulness
          = قاعدہ_useless_وچ_grammar_p (&قاعدے[r]) ? "useless-in-grammar"
          : قاعدہ_useless_وچ_parser_p (&قاعدے[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (باہرلا, level + 2);
        fprintf (باہرلا, "<rule number=\"%d\" usefulness=\"%s\"",
                 قاعدے[r].نمبر, usefulness);
        إذا (قاعدے[r].precsym)
          fprintf (باہرلا, " percent_prec=\"%s\"",
                   xml_escape (قاعدے[r].precsym->علامت->ٹیگ));
        fputs (">\n", باہرلا);
      }
      قاعدہ_lhs_print_xml (&قاعدے[r], باہرلا, level + 3);
      قاعدہ_rhs_print_xml (&قاعدے[r], باہرلا, level + 3);
      xml_puts (باہرلا, level + 2, "</rule>");
    }
  إذا (!پہلا)
    xml_puts (باہرلا, level + 1, "</rules>");
  وإلا
   xml_puts (باہرلا, level + 1, "<rules/>");
}

ساكن فراغ
section (FILE *باہرلا, ثابت حرف *s)
{
  fprintf (باہرلا, "%s\n", s);
  لكل (صحيح i = لڑی_لمبائی (s); 0 < i; --i)
    putc ('-', باہرلا);
  putc ('\n', باہرلا);
  putc ('\n', باہرلا);
}

فراغ
grammar_dump (FILE *باہرلا, ثابت حرف *title)
{
  fprintf (باہرلا, "%s\n\n", title);
  fprintf (باہرلا,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (باہرلا, "Tokens");
  {
    fprintf (باہرلا, "Value  Sprec  Sassoc  Tag\n");

    لكل (علامت_نمبر i = 0; i < ntokens; i++)
      fprintf (باہرلا, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->ٹیگ);
    fprintf (باہرلا, "\n\n");
  }

  section (باہرلا, "Nonterminals");
  {
    fprintf (باہرلا, "Value  Tag\n");

    لكل (علامت_نمبر i = ntokens; i < nsyms; i++)
      fprintf (باہرلا, "%5d  %s\n",
               i, symbols[i]->ٹیگ);
    fprintf (باہرلا, "\n\n");
  }

  section (باہرلا, "Rules");
  {
    fprintf (باہرلا,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    لكل (قاعدہ_نمبر i = 0; i < nrules + nuseless_productions; ++i)
      {
        قاعدہ ثابت *قاعدہ_i = &قاعدے[i];
        صحيح ثابت rhs_itemno = قاعدہ_i->rhs - ritem;
        صحيح لمائی = قاعدہ_rhs_لمائی (قاعدہ_i);
        aver (شے_نمبر_as_قاعدہ_نمبر (قاعدہ_i->rhs[لمائی]) == i);
        fprintf (باہرلا, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 قاعدہ_i->prec ? قاعدہ_i->prec->prec : 0,
                 قاعدہ_i->prec ? قاعدہ_i->prec->assoc : 0,
                 قاعدہ_i->useful ? "t" : "f",
                 قاعدہ_useless_chain_p (قاعدہ_i) ? "t" : "f",
                 قاعدہ_i->lhs->نمبر,
                 rhs_itemno, rhs_itemno + لمائی - 1);
        /* Dumped the RHS. */
        لكل (شے_نمبر *rhsp = قاعدہ_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (باہرلا, " %3d", *rhsp);
        putc ('\n', باہرلا);
      }
  }
  fprintf (باہرلا, "\n\n");

  section (باہرلا, "Rules interpreted");
  لكل (قاعدہ_نمبر r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (باہرلا, "%-5d  %s:", r, قاعدے[r].lhs->علامت->ٹیگ);
      قاعدہ_rhs_print (&قاعدے[r], باہرلا);
      putc ('\n', باہرلا);
    }
  fprintf (باہرلا, "\n\n");
}

فراغ
grammar_قاعدے_useless_report (ثابت حرف *سنیہا)
{
  لكل (قاعدہ_نمبر r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    إذا (!reduce_nonterminal_useless_وچ_grammar (قاعدے[r].lhs)
        && !قاعدے[r].useful)
      complain (&قاعدے[r].location, Wother, "%s", سنیہا);
}

فراغ
grammar_free (فراغ)
{
  إذا (ritem)
    حرر (ritem - 1);
  حرر (قاعدے);
  حرر (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
