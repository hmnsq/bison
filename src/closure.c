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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>
#ଅନ୍ତର୍ଭାଵ <bitsetv.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "derives.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
ସ୍ଥିର bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
ସ୍ଥିର bitsetv fderives = ଖାଲି;
ସ୍ଥିର bitsetv firsts = ଖାଲି;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

ସ୍ଥିର ଶୂନ୍ୟ
closure_print (ଅକ୍ଷର ଅଚଳ *title, item_index ଅଚଳ *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  ପାଇଁ (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_ସଂଖ୍ୟା *rp;
      ପାଇଁ (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (*rp));
    }
  fputs ("\n\n", stderr);
}


ସ୍ଥିର ଶୂନ୍ୟ
print_firsts (ଶୂନ୍ୟ)
{
  fprintf (stderr, "FIRSTS\n");
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      ପ୍ରତୀକ_ସଂଖ୍ୟା j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


ସ୍ଥିର ଶୂନ୍ୟ
print_fderives (ଶୂନ୍ୟ)
{
  fprintf (stderr, "FDERIVES\n");
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_ସଂଖ୍ୟା r;
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

ସ୍ଥିର ଶୂନ୍ୟ
set_firsts (ଶୂନ୍ୟ)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା j = 0; derives[i - ntokens][j]; ++j)
      {
        item_ସଂଖ୍ୟା sym = derives[i - ntokens][j]->rhs[0];
        ଯଦି (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  ଯଦି (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  ଯଦି (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  ଯଦି (trace_flag & trace_sets)
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

ସ୍ଥିର ଶୂନ୍ୟ
set_fderives (ଶୂନ୍ୟ)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା j = ntokens; j < nsyms; ++j)
      ଯଦି (bitset_test (FIRSTS (i), j - ntokens))
        ପାଇଁ (rule_ସଂଖ୍ୟା k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->ସଂଖ୍ୟା);

  ଯଦି (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



ଶୂନ୍ୟ
closure_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
  itemset = xnmalloc (n, ଆକାର *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



ଶୂନ୍ୟ
closure (item_index ଅଚଳ *core, size_t n)
{
  ଯଦି (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  ପାଇଁ (size_t c = 0; c < n; ++c)
    ଯଦି (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_ସଂଖ୍ୟା ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      ଯେତେବେଳେ (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  ଯେତେବେଳେ (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  ଯଦି (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


ଶୂନ୍ୟ
closure_free (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
