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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>
#অন্তর্ভাব <bitsetv.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "derives.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
স্থির bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
স্থির bitsetv fderives = রিক্ত;
স্থির bitsetv firsts = রিক্ত;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

স্থির শূন্য
closure_print (বর্ণ ধ্রুব *title, item_index ধ্রুব *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  প্রতি (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_সংখ্যা *rp;
      প্রতি (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_সংখ্যা_as_rule_সংখ্যা (*rp));
    }
  fputs ("\n\n", stderr);
}


স্থির শূন্য
print_firsts (শূন্য)
{
  fprintf (stderr, "FIRSTS\n");
  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      প্রতীক_সংখ্যা j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


স্থির শূন্য
print_fderives (শূন্য)
{
  fprintf (stderr, "FDERIVES\n");
  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_সংখ্যা r;
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

স্থির শূন্য
set_firsts (শূন্য)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    প্রতি (প্রতীক_সংখ্যা j = 0; derives[i - ntokens][j]; ++j)
      {
        item_সংখ্যা sym = derives[i - ntokens][j]->rhs[0];
        যদি (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  যদি (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  যদি (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  যদি (trace_flag & trace_sets)
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

স্থির শূন্য
set_fderives (শূন্য)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    প্রতি (প্রতীক_সংখ্যা j = ntokens; j < nsyms; ++j)
      যদি (bitset_test (FIRSTS (i), j - ntokens))
        প্রতি (rule_সংখ্যা k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->সংখ্যা);

  যদি (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



শূন্য
closure_new (পূর্ণাঙ্ক n)
{
  itemset = xnmalloc (n, পরিমাণ *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



শূন্য
closure (item_index ধ্রুব *core, size_t n)
{
  যদি (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  প্রতি (size_t c = 0; c < n; ++c)
    যদি (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_সংখ্যা ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      যাবত্ (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  যাবত্ (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  যদি (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


শূন্য
closure_free (শূন্য)
{
  মুক্ত (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
