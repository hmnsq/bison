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
size_t nitemset;

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
closure_print (वर्ण ध्रुव *title, item_index ध्रुव *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  प्रति (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_सङ्ख्या *rp;
      प्रति (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_सङ्ख्या_as_rule_सङ्ख्या (*rp));
    }
  fputs ("\n\n", stderr);
}


स्थिर शून्य
print_firsts (शून्य)
{
  fprintf (stderr, "FIRSTS\n");
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      चिह्नम्_सङ्ख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


स्थिर शून्य
print_fderives (शून्य)
{
  fprintf (stderr, "FDERIVES\n");
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_सङ्ख्या r;
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

स्थिर शून्य
set_firsts (शून्य)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    प्रति (चिह्नम्_सङ्ख्या j = 0; derives[i - ntokens][j]; ++j)
      {
        item_सङ्ख्या sym = derives[i - ntokens][j]->rhs[0];
        जइ (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  जइ (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  जइ (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  जइ (trace_flag & trace_sets)
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

  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    प्रति (चिह्नम्_सङ्ख्या j = ntokens; j < nsyms; ++j)
      जइ (bitset_test (FIRSTS (i), j - ntokens))
        प्रति (rule_सङ्ख्या k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->सङ्ख्या);

  जइ (trace_flag & trace_sets)
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
closure (item_index ध्रुव *core, size_t n)
{
  जइ (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  प्रति (size_t c = 0; c < n; ++c)
    जइ (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_सङ्ख्या ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      जाव (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  जाव (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  जइ (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


शून्य
closure_free (शून्य)
{
  मुक्त (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
