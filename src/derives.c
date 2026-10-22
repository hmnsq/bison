/* Match rules with nonterminals for bison,

   Copyright (C) 1984, 1989, 2000-2003, 2005, 2009-2015, 2018-2022 Free
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

#ಅನ್ತರ್ಭಾವ "getargs.h"

#ಅನ್ತರ್ಭಾವ "derives.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* Linked list of rule numbers.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ rule_list
{
  ಸಂರಚನಾ rule_list *next;
  rule *ಮೌಲ್ಯ;
} rule_list;

rule ***derives;

ಸ್ಥಿರ ಶೂನ್ಯ
print_derives (ಶೂನ್ಯ)
{
  fputs ("DERIVES\n", stderr);

  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      ಪ್ರತಿ (rule **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->code);
          rule_rhs_print (*rp, stderr);
          fprintf (stderr, "\n");
        }
    }

  fputs ("\n\n", stderr);
}


ಶೂನ್ಯ
derives_compute (ಶೂನ್ಯ)
{
  /* DSET[NTERM - NTOKENS] -- A linked list of the numbers of the rules
     whose LHS is NTERM.  */
  rule_list **dset = xcalloc (nnterms, ಪರಿಮಾಣ *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  rule_list *delts = xnmalloc (nrules, ಪರಿಮಾಣ *delts);

  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = nrules - 1; r >= 0; --r)
    {
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = rules[r].lhs->ಸಂಖ್ಯೆ;
      rule_list *p = &delts[r];
      /* A new LHS is found.  */
      p->next = dset[lhs - ntokens];
      p->ಮೌಲ್ಯ = &rules[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, ಪರಿಮಾಣ *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  rule **q = xnmalloc (nnterms + nrules, ಪರಿಮಾಣ *q);

  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      rule_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      ಯಾವತ್ (p)
        {
          *q++ = p->ಮೌಲ್ಯ;
          p = p->next;
        }
      *q++ = ರಿಕ್ತ;
    }

  ಯದಿ (trace_flag & trace_sets)
    print_derives ();

  ಮುಕ್ತ (dset);
  ಮುಕ್ತ (delts);
}


ಶೂನ್ಯ
derives_free (ಶೂನ್ಯ)
{
  ಯದಿ (derives)
    {
      ಮುಕ್ತ (derives[0]);
      ಮುಕ್ತ (derives);
    }
}
