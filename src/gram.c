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

شي_عدد *ritem = عدم;
صحيح nritems = 0;

قاعدو *قاعدا = عدم;
قاعدو_عدد nrules = 0;

نشان **symbols = عدم;
صحيح nsyms = 0;
صحيح ntokens = 1;
صحيح nnterms = 0;

نشان_عدد *token_translations = عدم;

صحيح max_ڪوڊ = 256;

صحيح required_نسخو = 0;

فراغ
شي_print (شي_عدد *شي, قاعدو ثابت *اڳيون_قاعدو, FILE *ٻاهريون)
{
  قاعدو ثابت *r = شي_قاعدو (شي);
  قاعدو_lhs_print (r, اڳيون_قاعدو ? اڳيون_قاعدو->lhs : عدم, ٻاهريون);

  إذا (0 <= *r->rhs)
    {
      // Non-empty rhs.
      لكل (شي_عدد *sp = r->rhs; sp < شي; sp++)
        fprintf (ٻاهريون, " %s", symbols[*sp]->ٽيگ);
      fprintf (ٻاهريون, " %s", dot);
      لكل (شي_عدد *sp = شي; 0 <= *sp; ++sp)
        fprintf (ٻاهريون, " %s", symbols[*sp]->ٽيگ);
    }
  وإلا
    fprintf (ٻاهريون, " %s %s", خالي_جايون, dot);
}


bool
قاعدو_useful_اندريون_grammar_p (قاعدو ثابت *r)
{
  أرجع r->عدد < nrules;
}

bool
قاعدو_useless_اندريون_grammar_p (قاعدو ثابت *r)
{
  أرجع !قاعدو_useful_اندريون_grammar_p (r);
}

bool
قاعدو_useless_اندريون_parser_p (قاعدو ثابت *r)
{
  أرجع !r->useful && قاعدو_useful_اندريون_grammar_p (r);
}

bool
قاعدو_useless_chain_p (قاعدو ثابت *r)
{
  أرجع قاعدو_rhs_ڊيگهائي (r) == 1 && !r->action;
}

فراغ
قاعدو_lhs_print (قاعدو ثابت *r, sym_content ثابت *اڳيون_lhs, FILE *ٻاهريون)
{
  fprintf (ٻاهريون, "  %3d ", r->عدد);
  إذا (اڳيون_lhs != r->lhs)
    fprintf (ٻاهريون, "%s:", r->lhs->نشان->ٽيگ);
  وإلا
    fprintf (ٻاهريون, "%*s|", (صحيح) لڙي_ڊيگهه (اڳيون_lhs->نشان->ٽيگ), "");
}

فراغ
قاعدو_lhs_print_xml (قاعدو ثابت *r, FILE *ٻاهريون, صحيح level)
{
  xml_printf (ٻاهريون, level, "<lhs>%s</lhs>", r->lhs->نشان->ٽيگ);
}

size_t
قاعدو_rhs_ڊيگهائي (قاعدو ثابت *r)
{
  size_t res = 0;
  لكل (شي_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  أرجع res;
}

فراغ
قاعدو_rhs_print (قاعدو ثابت *r, FILE *ٻاهريون)
{
  إذا (0 <= *r->rhs)
    لكل (شي_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (ٻاهريون, " %s", symbols[*rhsp]->ٽيگ);
  وإلا
    fprintf (ٻاهريون, " %s", خالي_جايون);
}

ساكن فراغ
قاعدو_rhs_print_xml (قاعدو ثابت *r, FILE *ٻاهريون, صحيح level)
{
  إذا (*r->rhs >= 0)
    {
      xml_puts (ٻاهريون, level, "<rhs>");
      لكل (شي_عدد *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (ٻاهريون, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->ٽيگ));
      xml_puts (ٻاهريون, level, "</rhs>");
    }
  وإلا
    {
      xml_puts (ٻاهريون, level, "<rhs>");
      xml_puts (ٻاهريون, level + 1, "<empty/>");
      xml_puts (ٻاهريون, level, "</rhs>");
    }
}

فراغ
قاعدو_print (قاعدو ثابت *r, قاعدو ثابت *prev_قاعدو, FILE *ٻاهريون)
{
  قاعدو_lhs_print (r, prev_قاعدو ? prev_قاعدو->lhs : عدم, ٻاهريون);
  قاعدو_rhs_print (r, ٻاهريون);
}

فراغ
ritem_print (FILE *ٻاهريون)
{
  fputs ("RITEM\n", ٻاهريون);
  bool پهريون = true;
  لكل (صحيح i = 0; i < nritems; ++i)
    {
      إذا (پهريون)
        {
          fprintf (ٻاهريون, "  %d: ", i);
          پهريون = false;
        }
      إذا (ritem[i] >= 0)
        fprintf (ٻاهريون, "  %s", symbols[ritem[i]]->ٽيگ);
      وإلا
        {
          fprintf (ٻاهريون, "  (rule %d)\n", شي_عدد_as_قاعدو_عدد (ritem[i]));
          پهريون = true;
        }
    }
  fputs ("\n\n", ٻاهريون);
}

size_t
ritem_longest_rhs (فراغ)
{
  صحيح max = 0;
  لكل (قاعدو_عدد r = 0; r < nrules; ++r)
    {
      size_t ڊيگهائي = قاعدو_rhs_ڊيگهائي (&قاعدا[r]);
      إذا (ڊيگهائي > max)
        max = ڊيگهائي;
    }

  أرجع max;
}

فراغ
grammar_قاعدا_partial_print (FILE *ٻاهريون, ثابت حرف *title,
                             قاعدو_filter filter)
{
  bool پهريون = true;
  قاعدو *اڳيون_قاعدو = عدم;

  /* rule # : LHS -> RHS */
  لكل (قاعدو_عدد r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (filter && !filter (&قاعدا[r]))
        استمر;
      إذا (پهريون)
        fprintf (ٻاهريون, "%s\n\n", title);
      وإلا إذا (اڳيون_قاعدو && اڳيون_قاعدو->lhs != قاعدا[r].lhs)
        putc ('\n', ٻاهريون);
      پهريون = false;
      قاعدو_print (&قاعدا[r], اڳيون_قاعدو, ٻاهريون);
      putc ('\n', ٻاهريون);
      اڳيون_قاعدو = &قاعدا[r];
    }
  إذا (!پهريون)
    fputs ("\n\n", ٻاهريون);
}

فراغ
grammar_قاعدا_print (FILE *ٻاهريون)
{
  grammar_قاعدا_partial_print (ٻاهريون, _("Grammar"), قاعدو_useful_اندريون_grammar_p);
}

فراغ
grammar_قاعدا_print_xml (FILE *ٻاهريون, صحيح level)
{
  bool پهريون = true;

  لكل (قاعدو_عدد r = 0; r < nrules + nuseless_productions; r++)
    {
      إذا (پهريون)
        xml_puts (ٻاهريون, level + 1, "<rules>");
      پهريون = false;
      {
        حرف ثابت *usefulness
          = قاعدو_useless_اندريون_grammar_p (&قاعدا[r]) ? "useless-in-grammar"
          : قاعدو_useless_اندريون_parser_p (&قاعدا[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (ٻاهريون, level + 2);
        fprintf (ٻاهريون, "<rule number=\"%d\" usefulness=\"%s\"",
                 قاعدا[r].عدد, usefulness);
        إذا (قاعدا[r].precsym)
          fprintf (ٻاهريون, " percent_prec=\"%s\"",
                   xml_escape (قاعدا[r].precsym->نشان->ٽيگ));
        fputs (">\n", ٻاهريون);
      }
      قاعدو_lhs_print_xml (&قاعدا[r], ٻاهريون, level + 3);
      قاعدو_rhs_print_xml (&قاعدا[r], ٻاهريون, level + 3);
      xml_puts (ٻاهريون, level + 2, "</rule>");
    }
  إذا (!پهريون)
    xml_puts (ٻاهريون, level + 1, "</rules>");
  وإلا
   xml_puts (ٻاهريون, level + 1, "<rules/>");
}

ساكن فراغ
section (FILE *ٻاهريون, ثابت حرف *s)
{
  fprintf (ٻاهريون, "%s\n", s);
  لكل (صحيح i = لڙي_ڊيگهه (s); 0 < i; --i)
    putc ('-', ٻاهريون);
  putc ('\n', ٻاهريون);
  putc ('\n', ٻاهريون);
}

فراغ
grammar_dump (FILE *ٻاهريون, ثابت حرف *title)
{
  fprintf (ٻاهريون, "%s\n\n", title);
  fprintf (ٻاهريون,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (ٻاهريون, "Tokens");
  {
    fprintf (ٻاهريون, "Value  Sprec  Sassoc  Tag\n");

    لكل (نشان_عدد i = 0; i < ntokens; i++)
      fprintf (ٻاهريون, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->ٽيگ);
    fprintf (ٻاهريون, "\n\n");
  }

  section (ٻاهريون, "Nonterminals");
  {
    fprintf (ٻاهريون, "Value  Tag\n");

    لكل (نشان_عدد i = ntokens; i < nsyms; i++)
      fprintf (ٻاهريون, "%5d  %s\n",
               i, symbols[i]->ٽيگ);
    fprintf (ٻاهريون, "\n\n");
  }

  section (ٻاهريون, "Rules");
  {
    fprintf (ٻاهريون,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    لكل (قاعدو_عدد i = 0; i < nrules + nuseless_productions; ++i)
      {
        قاعدو ثابت *قاعدو_i = &قاعدا[i];
        صحيح ثابت rhs_itemno = قاعدو_i->rhs - ritem;
        صحيح ڊيگهائي = قاعدو_rhs_ڊيگهائي (قاعدو_i);
        aver (شي_عدد_as_قاعدو_عدد (قاعدو_i->rhs[ڊيگهائي]) == i);
        fprintf (ٻاهريون, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 قاعدو_i->prec ? قاعدو_i->prec->prec : 0,
                 قاعدو_i->prec ? قاعدو_i->prec->assoc : 0,
                 قاعدو_i->useful ? "t" : "f",
                 قاعدو_useless_chain_p (قاعدو_i) ? "t" : "f",
                 قاعدو_i->lhs->عدد,
                 rhs_itemno, rhs_itemno + ڊيگهائي - 1);
        /* Dumped the RHS. */
        لكل (شي_عدد *rhsp = قاعدو_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (ٻاهريون, " %3d", *rhsp);
        putc ('\n', ٻاهريون);
      }
  }
  fprintf (ٻاهريون, "\n\n");

  section (ٻاهريون, "Rules interpreted");
  لكل (قاعدو_عدد r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (ٻاهريون, "%-5d  %s:", r, قاعدا[r].lhs->نشان->ٽيگ);
      قاعدو_rhs_print (&قاعدا[r], ٻاهريون);
      putc ('\n', ٻاهريون);
    }
  fprintf (ٻاهريون, "\n\n");
}

فراغ
grammar_قاعدا_useless_report (ثابت حرف *پيغام)
{
  لكل (قاعدو_عدد r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    إذا (!reduce_nonterminal_useless_اندريون_grammar (قاعدا[r].lhs)
        && !قاعدا[r].useful)
      complain (&قاعدا[r].location, Wother, "%s", پيغام);
}

فراغ
grammar_free (فراغ)
{
  إذا (ritem)
    حرر (ritem - 1);
  حرر (قاعدا);
  حرر (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
