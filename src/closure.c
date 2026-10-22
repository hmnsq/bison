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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <bitsetv.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "derives.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
माप_प्रकार nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
स्थिर bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
स्थिर bitsetv fderives = रिक्त;
स्थिर bitsetv firsts = रिक्त;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

स्थिर शून्य
closure_print (वर्ण अचर *title, item_index अचर *array, माप_प्रकार size)
{
  ख_लिखो (मानक_त्रुटि, "Closure: %s\n", title);
  लागि (माप_प्रकार i = 0; i < size; ++i)
    {
      ख_लिखो (मानक_त्रुटि, "  %2d: .", array[i]);
      item_सङ्ख्या *rp;
      लागि (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        ख_लिखो (मानक_त्रुटि, " %s", symbols[*rp]->tag);
      ख_लिखो (मानक_त्रुटि, "  (rule %d)\n", item_सङ्ख्या_as_rule_सङ्ख्या (*rp));
    }
  ख_माला_दो ("\n\n", मानक_त्रुटि);
}


स्थिर शून्य
print_firsts (शून्य)
{
  ख_लिखो (मानक_त्रुटि, "FIRSTS\n");
  लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (मानक_त्रुटि, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      चिह्न_सङ्ख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        ख_लिखो (मानक_त्रुटि, "    %s\n", symbols[j + ntokens]->tag);
    }
  ख_लिखो (मानक_त्रुटि, "\n\n");
}


स्थिर शून्य
print_fderives (शून्य)
{
  ख_लिखो (मानक_त्रुटि, "FDERIVES\n");
  लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (मानक_त्रुटि, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_सङ्ख्या r;
      BITSET_FOR_EACH (iter, FDERIVES (i), r, 0)
        {
          ख_लिखो (मानक_त्रुटि, "    %3d ", r);
          rule_rhs_print (&rules[r], मानक_त्रुटि);
          ख_लिखो (मानक_त्रुटि, "\n");
        }
    }
  ख_लिखो (मानक_त्रुटि, "\n\n");
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

स्थिर शून्य
set_firsts (शून्य)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; ++i)
    लागि (चिह्न_सङ्ख्या j = 0; derives[i - ntokens][j]; ++j)
      {
        item_सङ्ख्या sym = derives[i - ntokens][j]->rhs[0];
        यदि (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  यदि (trace_flag & trace_sets)
    bitsetv_matrix_dump (मानक_त्रुटि, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  यदि (trace_flag & trace_sets)
    bitsetv_matrix_dump (मानक_त्रुटि, "RTC: Firsts Output", firsts);

  यदि (trace_flag & trace_sets)
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

स्थिर शून्य
set_fderives (शून्य)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; ++i)
    लागि (चिह्न_सङ्ख्या j = ntokens; j < nsyms; ++j)
      यदि (bitset_test (FIRSTS (i), j - ntokens))
        लागि (rule_सङ्ख्या k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->सङ्ख्या);

  यदि (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



शून्य
closure_new (पूर्णाङ्क n)
{
  itemset = xnmalloc (n, परिमाण *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



शून्य
closure (item_index अचर *core, माप_प्रकार n)
{
  यदि (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  लागि (माप_प्रकार c = 0; c < n; ++c)
    यदि (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  माप_प्रकार c = 0;

  /* A bit index over RULESET. */
  rule_सङ्ख्या ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      जबसम्म (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  जबसम्म (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  यदि (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


शून्य
closure_free (शून्य)
{
  मुक्त (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
