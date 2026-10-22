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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ನಿಯಮ_list
{
  ರಚನೆ ನಿಯಮ_list *ಮುಂದಿನ;
  ನಿಯಮ *ಮೌಲ್ಯ;
} ನಿಯಮ_list;

ನಿಯಮ ***derives;

ಸ್ಥಿರ ಶೂನ್ಯ
print_derives (ಶೂನ್ಯ)
{
  fputs ("DERIVES\n", stderr);

  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->ಟ್ಯಾಗ್);
      ಗಾಗಿ (ನಿಯಮ **rp = derives[i - ntokens]; *rp; ++rp)
        {
          fprintf (stderr, "    %3d ", (*rp)->ಸಂಕೇತ_2);
          ನಿಯಮ_rhs_print (*rp, stderr);
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
  ನಿಯಮ_list **dset = xcalloc (nnterms, ಗಾತ್ರ *dset);

  /* DELTS[RULE] -- There are NRULES rule number to attach to nterms.
     Instead of performing NRULES allocations for each, have an array
     indexed by rule numbers.  */
  ನಿಯಮ_list *delts = xnmalloc (nrules, ಗಾತ್ರ *delts);

  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = nrules - 1; r >= 0; --r)
    {
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = ನಿಯಮಗಳು[r].lhs->ಸಂಖ್ಯೆ;
      ನಿಯಮ_list *p = &delts[r];
      /* A new LHS is found.  */
      p->ಮುಂದಿನ = dset[lhs - ntokens];
      p->ಮೌಲ್ಯ = &ನಿಯಮಗಳು[r];
      dset[lhs - ntokens] = p;
    }

  /* DSET contains what we need under the form of a linked list.  Make
     it a single array.  */

  derives = xnmalloc (nnterms, ಗಾತ್ರ *derives);
  /* Q is the storage for DERIVES[...] (DERIVES[0] = q).  */
  ನಿಯಮ **q = xnmalloc (nnterms + nrules, ಗಾತ್ರ *q);

  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      ನಿಯಮ_list *p = dset[i - ntokens];
      derives[i - ntokens] = q;
      ಆಗ (p)
        {
          *q++ = p->ಮೌಲ್ಯ;
          p = p->ಮುಂದಿನ;
        }
      *q++ = ಖಾಲಿ;
    }

  ಆದರೆ (trace_flag & trace_sets)
    print_derives ();

  ಬಿಡುಗಡೆ (dset);
  ಬಿಡುಗಡೆ (delts);
}


ಶೂನ್ಯ
derives_free (ಶೂನ್ಯ)
{
  ಆದರೆ (derives)
    {
      ಬಿಡುಗಡೆ (derives[0]);
      ಬಿಡುಗಡೆ (derives);
    }
}
