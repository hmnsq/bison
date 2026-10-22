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
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
ಸ್ಥಿರ bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
ಸ್ಥಿರ bitsetv fderives = ರಿಕ್ತ;
ಸ್ಥಿರ bitsetv firsts = ರಿಕ್ತ;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
closure_print (ವರ್ಣ ಧ್ರುವ *title, item_index ಧ್ರುವ *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  ಪ್ರತಿ (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_ಸಂಖ್ಯೆ *rp;
      ಪ್ರತಿ (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (*rp));
    }
  fputs ("\n\n", stderr);
}


ಸ್ಥಿರ ಶೂನ್ಯ
print_firsts (ಶೂನ್ಯ)
{
  fprintf (stderr, "FIRSTS\n");
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


ಸ್ಥಿರ ಶೂನ್ಯ
print_fderives (ಶೂನ್ಯ)
{
  fprintf (stderr, "FDERIVES\n");
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_ಸಂಖ್ಯೆ r;
      BITSET_FOR_EACH (iter, FDERIVES (i), r, 0)
        {
          fprintf (stderr, "    %3d ", r);
          rule_rhs_print (&rules[r], stderr);
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
set_firsts (ಶೂನ್ಯ)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = 0; derives[i - ntokens][j]; ++j)
      {
        item_ಸಂಖ್ಯೆ sym = derives[i - ntokens][j]->rhs[0];
        ಯದಿ (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  ಯದಿ (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  ಯದಿ (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  ಯದಿ (trace_flag & trace_sets)
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
set_fderives (ಶೂನ್ಯ)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = ntokens; j < nsyms; ++j)
      ಯದಿ (bitset_test (FIRSTS (i), j - ntokens))
        ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->ಸಂಖ್ಯೆ);

  ಯದಿ (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



ಶೂನ್ಯ
closure_new (ಪೂರ್ಣಾಙ್ಕ n)
{
  itemset = xnmalloc (n, ಪರಿಮಾಣ *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



ಶೂನ್ಯ
closure (item_index ಧ್ರುವ *core, size_t n)
{
  ಯದಿ (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  ಪ್ರತಿ (size_t c = 0; c < n; ++c)
    ಯದಿ (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_ಸಂಖ್ಯೆ ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      ಯಾವತ್ (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  ಯಾವತ್ (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  ಯದಿ (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


ಶೂನ್ಯ
closure_free (ಶೂನ್ಯ)
{
  ಮುಕ್ತ (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
