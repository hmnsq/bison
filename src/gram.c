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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "glyphs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "print-xml.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* Comments for these variables are in gram.h.  */

ಅಂಶ_ಸಂಖ್ಯೆ *ritem = ಖಾಲಿ;
ಪೂರ್ಣಾಂಕ nritems = 0;

ನಿಯಮ *ನಿಯಮಗಳು = ಖಾಲಿ;
ನಿಯಮ_ಸಂಖ್ಯೆ nrules = 0;

ಚಿಹ್ನೆ **symbols = ಖಾಲಿ;
ಪೂರ್ಣಾಂಕ nsyms = 0;
ಪೂರ್ಣಾಂಕ ntokens = 1;
ಪೂರ್ಣಾಂಕ nnterms = 0;

ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *token_translations = ಖಾಲಿ;

ಪೂರ್ಣಾಂಕ max_ಸಂಕೇತ = 256;

ಪೂರ್ಣಾಂಕ required_ಆವೃತ್ತಿ = 0;

ಶೂನ್ಯ
ಅಂಶ_print (ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ, ನಿಯಮ ಸ್ಥಿರಾಂಕ *ಹಿಂದಿನ_ನಿಯಮ, FILE *ಹೊರಹೋಗು)
{
  ನಿಯಮ ಸ್ಥಿರಾಂಕ *r = ಅಂಶ_ನಿಯಮ (ಅಂಶ);
  ನಿಯಮ_lhs_print (r, ಹಿಂದಿನ_ನಿಯಮ ? ಹಿಂದಿನ_ನಿಯಮ->lhs : ಖಾಲಿ, ಹೊರಹೋಗು);

  ಆದರೆ (0 <= *r->rhs)
    {
      // Non-empty rhs.
      ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *sp = r->rhs; sp < ಅಂಶ; sp++)
        fprintf (ಹೊರಹೋಗು, " %s", symbols[*sp]->ಟ್ಯಾಗ್);
      fprintf (ಹೊರಹೋಗು, " %s", dot);
      ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *sp = ಅಂಶ; 0 <= *sp; ++sp)
        fprintf (ಹೊರಹೋಗು, " %s", symbols[*sp]->ಟ್ಯಾಗ್);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, " %s %s", ಖಾಲಿಗಳು, dot);
}


bool
ನಿಯಮ_useful_ಒಳ_grammar_p (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r)
{
  ಹಿಂತಿರುಗಿ r->ಸಂಖ್ಯೆ < nrules;
}

bool
ನಿಯಮ_useless_ಒಳ_grammar_p (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r)
{
  ಹಿಂತಿರುಗಿ !ನಿಯಮ_useful_ಒಳ_grammar_p (r);
}

bool
ನಿಯಮ_useless_ಒಳ_parser_p (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r)
{
  ಹಿಂತಿರುಗಿ !r->useful && ನಿಯಮ_useful_ಒಳ_grammar_p (r);
}

bool
ನಿಯಮ_useless_chain_p (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r)
{
  ಹಿಂತಿರುಗಿ ನಿಯಮ_rhs_ಉದ್ದಳತೆ (r) == 1 && !r->action;
}

ಶೂನ್ಯ
ನಿಯಮ_lhs_print (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, sym_content ಸ್ಥಿರಾಂಕ *ಹಿಂದಿನ_lhs, FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು, "  %3d ", r->ಸಂಖ್ಯೆ);
  ಆದರೆ (ಹಿಂದಿನ_lhs != r->lhs)
    fprintf (ಹೊರಹೋಗು, "%s:", r->lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, "%*s|", (ಪೂರ್ಣಾಂಕ) ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಹಿಂದಿನ_lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್), "");
}

ಶೂನ್ಯ
ನಿಯಮ_lhs_print_xml (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  xml_printf (ಹೊರಹೋಗು, level, "<lhs>%s</lhs>", r->lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
}

size_t
ನಿಯಮ_rhs_ಉದ್ದಳತೆ (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r)
{
  size_t res = 0;
  ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
    ++res;
  ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ
ನಿಯಮ_rhs_print (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (0 <= *r->rhs)
    ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
      fprintf (ಹೊರಹೋಗು, " %s", symbols[*rhsp]->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, " %s", ಖಾಲಿಗಳು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ನಿಯಮ_rhs_print_xml (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  ಆದರೆ (*r->rhs >= 0)
    {
      xml_puts (ಹೊರಹೋಗು, level, "<rhs>");
      ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = r->rhs; 0 <= *rhsp; ++rhsp)
        xml_printf (ಹೊರಹೋಗು, level + 1, "<symbol>%s</symbol>",
                    xml_escape (symbols[*rhsp]->ಟ್ಯಾಗ್));
      xml_puts (ಹೊರಹೋಗು, level, "</rhs>");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      xml_puts (ಹೊರಹೋಗು, level, "<rhs>");
      xml_puts (ಹೊರಹೋಗು, level + 1, "<empty/>");
      xml_puts (ಹೊರಹೋಗು, level, "</rhs>");
    }
}

ಶೂನ್ಯ
ನಿಯಮ_print (ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, ನಿಯಮ ಸ್ಥಿರಾಂಕ *prev_ನಿಯಮ, FILE *ಹೊರಹೋಗು)
{
  ನಿಯಮ_lhs_print (r, prev_ನಿಯಮ ? prev_ನಿಯಮ->lhs : ಖಾಲಿ, ಹೊರಹೋಗು);
  ನಿಯಮ_rhs_print (r, ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
ritem_print (FILE *ಹೊರಹೋಗು)
{
  fputs ("RITEM\n", ಹೊರಹೋಗು);
  bool ಮೊದಲ = true;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nritems; ++i)
    {
      ಆದರೆ (ಮೊದಲ)
        {
          fprintf (ಹೊರಹೋಗು, "  %d: ", i);
          ಮೊದಲ = false;
        }
      ಆದರೆ (ritem[i] >= 0)
        fprintf (ಹೊರಹೋಗು, "  %s", symbols[ritem[i]]->ಟ್ಯಾಗ್);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          fprintf (ಹೊರಹೋಗು, "  (rule %d)\n", ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[i]));
          ಮೊದಲ = true;
        }
    }
  fputs ("\n\n", ಹೊರಹೋಗು);
}

size_t
ritem_longest_rhs (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ max = 0;
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    {
      size_t ಉದ್ದಳತೆ = ನಿಯಮ_rhs_ಉದ್ದಳತೆ (&ನಿಯಮಗಳು[r]);
      ಆದರೆ (ಉದ್ದಳತೆ > max)
        max = ಉದ್ದಳತೆ;
    }

  ಹಿಂತಿರುಗಿ max;
}

ಶೂನ್ಯ
grammar_ನಿಯಮಗಳು_partial_print (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *title,
                             ನಿಯಮ_filter filter)
{
  bool ಮೊದಲ = true;
  ನಿಯಮ *ಹಿಂದಿನ_ನಿಯಮ = ಖಾಲಿ;

  /* rule # : LHS -> RHS */
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules + nuseless_productions; r++)
    {
      ಆದರೆ (filter && !filter (&ನಿಯಮಗಳು[r]))
        ಮುಂದುವರಿ;
      ಆದರೆ (ಮೊದಲ)
        fprintf (ಹೊರಹೋಗು, "%s\n\n", title);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹಿಂದಿನ_ನಿಯಮ && ಹಿಂದಿನ_ನಿಯಮ->lhs != ನಿಯಮಗಳು[r].lhs)
        putc ('\n', ಹೊರಹೋಗು);
      ಮೊದಲ = false;
      ನಿಯಮ_print (&ನಿಯಮಗಳು[r], ಹಿಂದಿನ_ನಿಯಮ, ಹೊರಹೋಗು);
      putc ('\n', ಹೊರಹೋಗು);
      ಹಿಂದಿನ_ನಿಯಮ = &ನಿಯಮಗಳು[r];
    }
  ಆದರೆ (!ಮೊದಲ)
    fputs ("\n\n", ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
grammar_ನಿಯಮಗಳು_print (FILE *ಹೊರಹೋಗು)
{
  grammar_ನಿಯಮಗಳು_partial_print (ಹೊರಹೋಗು, _("Grammar"), ನಿಯಮ_useful_ಒಳ_grammar_p);
}

ಶೂನ್ಯ
grammar_ನಿಯಮಗಳು_print_xml (FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  bool ಮೊದಲ = true;

  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules + nuseless_productions; r++)
    {
      ಆದರೆ (ಮೊದಲ)
        xml_puts (ಹೊರಹೋಗು, level + 1, "<rules>");
      ಮೊದಲ = false;
      {
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *usefulness
          = ನಿಯಮ_useless_ಒಳ_grammar_p (&ನಿಯಮಗಳು[r]) ? "useless-in-grammar"
          : ನಿಯಮ_useless_ಒಳ_parser_p (&ನಿಯಮಗಳು[r])  ? "useless-in-parser"
          :                                         "useful";
        xml_indent (ಹೊರಹೋಗು, level + 2);
        fprintf (ಹೊರಹೋಗು, "<rule number=\"%d\" usefulness=\"%s\"",
                 ನಿಯಮಗಳು[r].ಸಂಖ್ಯೆ, usefulness);
        ಆದರೆ (ನಿಯಮಗಳು[r].precsym)
          fprintf (ಹೊರಹೋಗು, " percent_prec=\"%s\"",
                   xml_escape (ನಿಯಮಗಳು[r].precsym->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್));
        fputs (">\n", ಹೊರಹೋಗು);
      }
      ನಿಯಮ_lhs_print_xml (&ನಿಯಮಗಳು[r], ಹೊರಹೋಗು, level + 3);
      ನಿಯಮ_rhs_print_xml (&ನಿಯಮಗಳು[r], ಹೊರಹೋಗು, level + 3);
      xml_puts (ಹೊರಹೋಗು, level + 2, "</rule>");
    }
  ಆದರೆ (!ಮೊದಲ)
    xml_puts (ಹೊರಹೋಗು, level + 1, "</rules>");
  ಇಲ್ಲದಿದ್ದರೆ
   xml_puts (ಹೊರಹೋಗು, level + 1, "<rules/>");
}

ಸ್ಥಿರ ಶೂನ್ಯ
section (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  fprintf (ಹೊರಹೋಗು, "%s\n", s);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s); 0 < i; --i)
    putc ('-', ಹೊರಹೋಗು);
  putc ('\n', ಹೊರಹೋಗು);
  putc ('\n', ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
grammar_dump (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *title)
{
  fprintf (ಹೊರಹೋಗು, "%s\n\n", title);
  fprintf (ಹೊರಹೋಗು,
           "ntokens = %d, nnterms = %d, nsyms = %d, nrules = %d, nritems = %d\n\n",
           ntokens, nnterms, nsyms, nrules, nritems);

  section (ಹೊರಹೋಗು, "Tokens");
  {
    fprintf (ಹೊರಹೋಗು, "Value  Sprec  Sassoc  Tag\n");

    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; i++)
      fprintf (ಹೊರಹೋಗು, "%5d  %5d   %5d  %s\n",
               i,
               symbols[i]->content->prec, symbols[i]->content->assoc,
               symbols[i]->ಟ್ಯಾಗ್);
    fprintf (ಹೊರಹೋಗು, "\n\n");
  }

  section (ಹೊರಹೋಗು, "Nonterminals");
  {
    fprintf (ಹೊರಹೋಗು, "Value  Tag\n");

    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; i++)
      fprintf (ಹೊರಹೋಗು, "%5d  %s\n",
               i, symbols[i]->ಟ್ಯಾಗ್);
    fprintf (ಹೊರಹೋಗು, "\n\n");
  }

  section (ಹೊರಹೋಗು, "Rules");
  {
    fprintf (ಹೊರಹೋಗು,
             "Num (Prec, Assoc, Useful, UselessChain) Lhs"
             " -> (Ritem Range) Rhs\n");
    ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ i = 0; i < nrules + nuseless_productions; ++i)
      {
        ನಿಯಮ ಸ್ಥಿರಾಂಕ *ನಿಯಮ_i = &ನಿಯಮಗಳು[i];
        ಪೂರ್ಣಾಂಕ ಸ್ಥಿರಾಂಕ rhs_itemno = ನಿಯಮ_i->rhs - ritem;
        ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = ನಿಯಮ_rhs_ಉದ್ದಳತೆ (ನಿಯಮ_i);
        aver (ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (ನಿಯಮ_i->rhs[ಉದ್ದಳತೆ]) == i);
        fprintf (ಹೊರಹೋಗು, "%3d (%2d, %2d, %2s, %2s)   %2d -> (%2u-%2u)",
                 i,
                 ನಿಯಮ_i->prec ? ನಿಯಮ_i->prec->prec : 0,
                 ನಿಯಮ_i->prec ? ನಿಯಮ_i->prec->assoc : 0,
                 ನಿಯಮ_i->useful ? "t" : "f",
                 ನಿಯಮ_useless_chain_p (ನಿಯಮ_i) ? "t" : "f",
                 ನಿಯಮ_i->lhs->ಸಂಖ್ಯೆ,
                 rhs_itemno, rhs_itemno + ಉದ್ದಳತೆ - 1);
        /* Dumped the RHS. */
        ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = ನಿಯಮ_i->rhs; 0 <= *rhsp; ++rhsp)
          fprintf (ಹೊರಹೋಗು, " %3d", *rhsp);
        putc ('\n', ಹೊರಹೋಗು);
      }
  }
  fprintf (ಹೊರಹೋಗು, "\n\n");

  section (ಹೊರಹೋಗು, "Rules interpreted");
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules + nuseless_productions; ++r)
    {
      fprintf (ಹೊರಹೋಗು, "%-5d  %s:", r, ನಿಯಮಗಳು[r].lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
      ನಿಯಮ_rhs_print (&ನಿಯಮಗಳು[r], ಹೊರಹೋಗು);
      putc ('\n', ಹೊರಹೋಗು);
    }
  fprintf (ಹೊರಹೋಗು, "\n\n");
}

ಶೂನ್ಯ
grammar_ನಿಯಮಗಳು_useless_report (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    /* Don't complain about rules whose LHS is useless, we already
       complained about it.  */
    ಆದರೆ (!reduce_nonterminal_useless_ಒಳ_grammar (ನಿಯಮಗಳು[r].lhs)
        && !ನಿಯಮಗಳು[r].useful)
      complain (&ನಿಯಮಗಳು[r].location, Wother, "%s", ಸಂದೇಶ);
}

ಶೂನ್ಯ
grammar_free (ಶೂನ್ಯ)
{
  ಆದರೆ (ritem)
    ಬಿಡುಗಡೆ (ritem - 1);
  ಬಿಡುಗಡೆ (ನಿಯಮಗಳು);
  ಬಿಡುಗಡೆ (token_translations);
  /* Free the symbol table data structure.  */
  symbols_free ();
  free_merger_functions ();
}
