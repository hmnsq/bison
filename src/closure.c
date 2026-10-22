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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>
#شامل <bitsetv.h>

#شامل "closure.h"
#شامل "derives.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "reader.h"
#شامل "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
ایستا bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
ایستا bitsetv fderives = پوچ;
ایستا bitsetv firsts = پوچ;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

ایستا تهی
closure_print (نویسه ثابت *title, item_index ثابت *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  برای (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_عدد *rp;
      برای (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_عدد_as_rule_عدد (*rp));
    }
  fputs ("\n\n", stderr);
}


ایستا تهی
print_firsts (تهی)
{
  fprintf (stderr, "FIRSTS\n");
  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      نماد_عدد j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


ایستا تهی
print_fderives (تهی)
{
  fprintf (stderr, "FDERIVES\n");
  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_عدد r;
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

ایستا تهی
set_firsts (تهی)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    برای (نماد_عدد j = 0; derives[i - ntokens][j]; ++j)
      {
        item_عدد sym = derives[i - ntokens][j]->rhs[0];
        اگر (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  اگر (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  اگر (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  اگر (trace_flag & trace_sets)
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

ایستا تهی
set_fderives (تهی)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    برای (نماد_عدد j = ntokens; j < nsyms; ++j)
      اگر (bitset_test (FIRSTS (i), j - ntokens))
        برای (rule_عدد k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->عدد);

  اگر (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



تهی
closure_new (صحیح n)
{
  itemset = xnmalloc (n, اندازه *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



تهی
closure (item_index ثابت *core, size_t n)
{
  اگر (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  برای (size_t c = 0; c < n; ++c)
    اگر (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_عدد ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      وقتی (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  وقتی (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  اگر (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


تهی
closure_free (تهی)
{
  آزاد (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
