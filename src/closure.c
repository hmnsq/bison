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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>
#અન્તર્ભાવ <bitsetv.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "derives.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
સ્થિર bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
સ્થિર bitsetv fderives = ખાલી;
સ્થિર bitsetv firsts = ખાલી;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

સ્થિર શૂન્ય
closure_print (અક્ષર અચલ *title, item_index અચલ *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  માટે (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_સંખ્યા *rp;
      માટે (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_સંખ્યા_as_rule_સંખ્યા (*rp));
    }
  fputs ("\n\n", stderr);
}


સ્થિર શૂન્ય
print_firsts (શૂન્ય)
{
  fprintf (stderr, "FIRSTS\n");
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      પ્રતીક_સંખ્યા j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


સ્થિર શૂન્ય
print_fderives (શૂન્ય)
{
  fprintf (stderr, "FDERIVES\n");
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_સંખ્યા r;
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

સ્થિર શૂન્ય
set_firsts (શૂન્ય)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    માટે (પ્રતીક_સંખ્યા j = 0; derives[i - ntokens][j]; ++j)
      {
        item_સંખ્યા sym = derives[i - ntokens][j]->rhs[0];
        જો (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  જો (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  જો (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  જો (trace_flag & trace_sets)
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

સ્થિર શૂન્ય
set_fderives (શૂન્ય)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    માટે (પ્રતીક_સંખ્યા j = ntokens; j < nsyms; ++j)
      જો (bitset_test (FIRSTS (i), j - ntokens))
        માટે (rule_સંખ્યા k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->સંખ્યા);

  જો (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



શૂન્ય
closure_new (પૂર્ણાંક n)
{
  itemset = xnmalloc (n, કદ *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



શૂન્ય
closure (item_index અચલ *core, size_t n)
{
  જો (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  માટે (size_t c = 0; c < n; ++c)
    જો (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_સંખ્યા ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      જ્યારે (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  જ્યારે (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  જો (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


શૂન્ય
closure_free (શૂન્ય)
{
  મુક્ત (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
