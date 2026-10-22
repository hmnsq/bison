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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <bitsetv.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "derives.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
நிலையான bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
நிலையான bitsetv fderives = வெற்று;
நிலையான bitsetv firsts = வெற்று;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

நிலையான வெறுமை
closure_print (எழுத்து மாறிலி *title, item_index மாறிலி *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  க்கு (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_எண் *rp;
      க்கு (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_எண்_as_rule_எண் (*rp));
    }
  fputs ("\n\n", stderr);
}


நிலையான வெறுமை
print_firsts (வெறுமை)
{
  fprintf (stderr, "FIRSTS\n");
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      குறியீடு_எண் j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


நிலையான வெறுமை
print_fderives (வெறுமை)
{
  fprintf (stderr, "FDERIVES\n");
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_எண் r;
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

நிலையான வெறுமை
set_firsts (வெறுமை)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    க்கு (குறியீடு_எண் j = 0; derives[i - ntokens][j]; ++j)
      {
        item_எண் sym = derives[i - ntokens][j]->rhs[0];
        எனில் (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  எனில் (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  எனில் (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  எனில் (trace_flag & trace_sets)
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

நிலையான வெறுமை
set_fderives (வெறுமை)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    க்கு (குறியீடு_எண் j = ntokens; j < nsyms; ++j)
      எனில் (bitset_test (FIRSTS (i), j - ntokens))
        க்கு (rule_எண் k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->எண்);

  எனில் (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



வெறுமை
closure_new (முழு n)
{
  itemset = xnmalloc (n, அளவு *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



வெறுமை
closure (item_index மாறிலி *core, size_t n)
{
  எனில் (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  க்கு (size_t c = 0; c < n; ++c)
    எனில் (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_எண் ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      போது (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  போது (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  எனில் (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


வெறுமை
closure_free (வெறுமை)
{
  விடுவி (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
