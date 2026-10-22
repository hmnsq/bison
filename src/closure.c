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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <bitsetv.h>

#ضمن "closure.h"
#ضمن "derives.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "reader.h"
#ضمن "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
توکی_شاخص *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
ساكن bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
ساكن bitsetv fderives = عدم;
ساكن bitsetv firsts = عدم;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#تعریف_کړه FDERIVES(Var)   fderives[(Var) - ntokens]
#تعریف_کړه FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

ساكن فراغ
تړون_print (حرف ثابت *title, توکی_شاخص ثابت *array, size_t اندازه)
{
  fprintf (stderr, "Closure: %s\n", title);
  لكل (size_t i = 0; i < اندازه; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      توکی_شمېره *rp;
      لكل (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->نښان);
      fprintf (stderr, "  (rule %d)\n", توکی_شمېره_as_قاعده_شمېره (*rp));
    }
  fputs ("\n\n", stderr);
}


ساكن فراغ
print_firsts (فراغ)
{
  fprintf (stderr, "FIRSTS\n");
  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->نښان);
      bitset_iterator iter;
      نښه_شمېره j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->نښان);
    }
  fprintf (stderr, "\n\n");
}


ساكن فراغ
print_fderives (فراغ)
{
  fprintf (stderr, "FDERIVES\n");
  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->نښان);
      bitset_iterator iter;
      قاعده_شمېره r;
      BITSET_FOR_EACH (iter, FDERIVES (i), r, 0)
        {
          fprintf (stderr, "    %3d ", r);
          قاعده_rhs_print (&قواعد[r], stderr);
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

ساكن فراغ
سېټ_firsts (فراغ)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    لكل (نښه_شمېره j = 0; derives[i - ntokens][j]; ++j)
      {
        توکی_شمېره sym = derives[i - ntokens][j]->rhs[0];
        إذا (ISVAR (sym))
          bitset_سېټ (FIRSTS (i), sym - ntokens);
      }

  إذا (trace_flag & trace_sets)
    bitsetv_ماتریکس_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_انعکاسي_متعدي_تړون (firsts);
  إذا (trace_flag & trace_sets)
    bitsetv_ماتریکس_dump (stderr, "RTC: Firsts Output", firsts);

  إذا (trace_flag & trace_sets)
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

ساكن فراغ
سېټ_fderives (فراغ)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  سېټ_firsts ();

  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    لكل (نښه_شمېره j = ntokens; j < nsyms; ++j)
      إذا (bitset_test (FIRSTS (i), j - ntokens))
        لكل (قاعده_شمېره k = 0; derives[j - ntokens][k]; ++k)
          bitset_سېټ (FDERIVES (i), derives[j - ntokens][k]->شمېره);

  إذا (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



فراغ
تړون_new (صحيح n)
{
  itemset = xnmalloc (n, حجم *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  سېټ_fderives ();
}



فراغ
تړون (توکی_شاخص ثابت *core, size_t n)
{
  إذا (trace_flag & trace_تړون)
    تړون_print ("input", core, n);

  bitset_zero (ruleset);

  لكل (size_t c = 0; c < n; ++c)
    إذا (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  قاعده_شمېره ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      توکی_شاخص itemno = قواعد[ruleno].rhs - ritem;
      طالما (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  طالما (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  إذا (trace_flag & trace_تړون)
    تړون_print ("output", itemset, nitemset);
}


فراغ
تړون_free (فراغ)
{
  حرر (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
