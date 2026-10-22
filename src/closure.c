/* Closures for Bison

   Copyright (C) 1984, 1989, 2000-2002, 2004-2005, 2007, 2009-2015,
   2018-2022 Free Software Foundation, Inc.

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

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <bitsetv.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "derives.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
ಅಂಶ_ಸೂಚ್ಯಂಕ *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
ಸ್ಥಿರ bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
ಸ್ಥಿರ bitsetv fderives = ಖಾಲಿ;
ಸ್ಥಿರ bitsetv firsts = ಖಾಲಿ;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#ವ್ಯಾಖ್ಯಾನಿಸು FDERIVES(Var)   fderives[(Var) - ntokens]
#ವ್ಯಾಖ್ಯಾನಿಸು FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಆವರಣ_print (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *title, ಅಂಶ_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ *array, size_t ಗಾತ್ರ_2)
{
  fprintf (stderr, "Closure: %s\n", title);
  ಗಾಗಿ (size_t i = 0; i < ಗಾತ್ರ_2; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      ಅಂಶ_ಸಂಖ್ಯೆ *rp;
      ಗಾಗಿ (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->ಟ್ಯಾಗ್);
      fprintf (stderr, "  (rule %d)\n", ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (*rp));
    }
  fputs ("\n\n", stderr);
}


ಸ್ಥಿರ ಶೂನ್ಯ
print_firsts (ಶೂನ್ಯ)
{
  fprintf (stderr, "FIRSTS\n");
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->ಟ್ಯಾಗ್);
      bitset_iterator iter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->ಟ್ಯಾಗ್);
    }
  fprintf (stderr, "\n\n");
}


ಸ್ಥಿರ ಶೂನ್ಯ
print_fderives (ಶೂನ್ಯ)
{
  fprintf (stderr, "FDERIVES\n");
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->ಟ್ಯಾಗ್);
      bitset_iterator iter;
      ನಿಯಮ_ಸಂಖ್ಯೆ r;
      BITSET_FOR_EACH (iter, FDERIVES (i), r, 0)
        {
          fprintf (stderr, "    %3d ", r);
          ನಿಯಮ_rhs_print (&ನಿಯಮಗಳು[r], stderr);
          fprintf (stderr, "\n");
        }
    }
  fprintf (stderr, "\n\n");
}

/*-------------------------------------------------------------------.
| Set FIRSTS to be an NNTERMS array of NNTERMS bitsets indicating    |
| which items can represent the beginning of the input corresponding |
| to which other items.                                              |
|                                                                    |
| For example, if some rule expands symbol 5 into the sequence of    |
| symbols 8 3 20, the symbol 8 can be the beginning of the data for  |
| symbol 5, so the bit [8 - ntokens] in first[5 - ntokens] (= FIRST  |
| (5)) is set.                                                       |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_firsts (ಶೂನ್ಯ)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = 0; derives[i - ntokens][j]; ++j)
      {
        ಅಂಶ_ಸಂಖ್ಯೆ sym = derives[i - ntokens][j]->rhs[0];
        ಆದರೆ (ISVAR (sym))
          bitset_ಗಣ (FIRSTS (i), sym - ntokens);
      }

  ಆದರೆ (trace_flag & trace_sets)
    bitsetv_ಮಾತೃಕೆ_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_ಸ್ವವಾಚಕ_ಸಂಕ್ರಾಮಿಕ_ಆವರಣ (firsts);
  ಆದರೆ (trace_flag & trace_sets)
    bitsetv_ಮಾತೃಕೆ_dump (stderr, "RTC: Firsts Output", firsts);

  ಆದರೆ (trace_flag & trace_sets)
    print_firsts ();
}

/*-------------------------------------------------------------------.
| Set FDERIVES to an NNTERMS by NRULES matrix of bits indicating     |
| which rules can help derive the beginning of the data for each     |
| nonterminal.                                                       |
|                                                                    |
| For example, if symbol 5 can be derived as the sequence of symbols |
| 8 3 20, and one of the rules for deriving symbol 8 is rule 4, then |
| the [5 - NTOKENS, 4] bit in FDERIVES is set.                       |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_fderives (ಶೂನ್ಯ)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  ಗಣ_firsts ();

  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = ntokens; j < nsyms; ++j)
      ಆದರೆ (bitset_test (FIRSTS (i), j - ntokens))
        ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ k = 0; derives[j - ntokens][k]; ++k)
          bitset_ಗಣ (FDERIVES (i), derives[j - ntokens][k]->ಸಂಖ್ಯೆ);

  ಆದರೆ (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



ಶೂನ್ಯ
ಆವರಣ_new (ಪೂರ್ಣಾಂಕ n)
{
  itemset = xnmalloc (n, ಗಾತ್ರ *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  ಗಣ_fderives ();
}



ಶೂನ್ಯ
ಆವರಣ (ಅಂಶ_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ *core, size_t n)
{
  ಆದರೆ (trace_flag & trace_ಆವರಣ)
    ಆವರಣ_print ("input", core, n);

  bitset_zero (ruleset);

  ಗಾಗಿ (size_t c = 0; c < n; ++c)
    ಆದರೆ (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  ನಿಯಮ_ಸಂಖ್ಯೆ ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      ಅಂಶ_ಸೂಚ್ಯಂಕ itemno = ನಿಯಮಗಳು[ruleno].rhs - ritem;
      ಆಗ (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  ಆಗ (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  ಆದರೆ (trace_flag & trace_ಆವರಣ)
    ಆವರಣ_print ("output", itemset, nitemset);
}


ಶೂನ್ಯ
ಆವರಣ_free (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
