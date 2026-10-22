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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>
#അന്തര്ഭാവ <bitsetv.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "derives.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "symtab.h"

/* NITEMSET is the size of the array ITEMSET.  */
item_index *itemset;
size_t nitemset;

/* RULESET contains a bit for each rule.  CLOSURE sets the bits for
   all rules which could potentially describe the next input to be
   read.  */
സ്ഥിതം bitset ruleset;

/* internal data.  See comments before set_fderives and set_firsts.  */
സ്ഥിതം bitsetv fderives = ശൂന്യ;
സ്ഥിതം bitsetv firsts = ശൂന്യ;

/* Retrieve the FDERIVES/FIRSTS sets of the nonterminals numbered Var.  */
#define FDERIVES(Var)   fderives[(Var) - ntokens]
#define FIRSTS(Var)   firsts[(Var) - ntokens]


/*-----------------.
| Debugging code.  |
`-----------------*/

സ്ഥിതം ശൂന്യം
closure_print (അക്ഷരം സ്ഥിരം *title, item_index സ്ഥിരം *array, size_t size)
{
  fprintf (stderr, "Closure: %s\n", title);
  വേണ്ടി (size_t i = 0; i < size; ++i)
    {
      fprintf (stderr, "  %2d: .", array[i]);
      item_സംഖ്യ *rp;
      വേണ്ടി (rp = &ritem[array[i]]; 0 <= *rp; ++rp)
        fprintf (stderr, " %s", symbols[*rp]->tag);
      fprintf (stderr, "  (rule %d)\n", item_സംഖ്യ_as_rule_സംഖ്യ (*rp));
    }
  fputs ("\n\n", stderr);
}


സ്ഥിതം ശൂന്യം
print_firsts (ശൂന്യം)
{
  fprintf (stderr, "FIRSTS\n");
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      ചിഹ്നം_സംഖ്യ j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (stderr, "    %s\n", symbols[j + ntokens]->tag);
    }
  fprintf (stderr, "\n\n");
}


സ്ഥിതം ശൂന്യം
print_fderives (ശൂന്യം)
{
  fprintf (stderr, "FDERIVES\n");
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    {
      fprintf (stderr, "  %s derives\n", symbols[i]->tag);
      bitset_iterator iter;
      rule_സംഖ്യ r;
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

സ്ഥിതം ശൂന്യം
set_firsts (ശൂന്യം)
{
  firsts = bitsetv_create (nnterms, nnterms, BITSET_FIXED);

  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    വേണ്ടി (ചിഹ്നം_സംഖ്യ j = 0; derives[i - ntokens][j]; ++j)
      {
        item_സംഖ്യ sym = derives[i - ntokens][j]->rhs[0];
        എങ്കിൽ (ISVAR (sym))
          bitset_set (FIRSTS (i), sym - ntokens);
      }

  എങ്കിൽ (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Input", firsts);
  bitsetv_reflexive_transitive_closure (firsts);
  എങ്കിൽ (trace_flag & trace_sets)
    bitsetv_matrix_dump (stderr, "RTC: Firsts Output", firsts);

  എങ്കിൽ (trace_flag & trace_sets)
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

സ്ഥിതം ശൂന്യം
set_fderives (ശൂന്യം)
{
  fderives = bitsetv_create (nnterms, nrules, BITSET_FIXED);

  set_firsts ();

  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    വേണ്ടി (ചിഹ്നം_സംഖ്യ j = ntokens; j < nsyms; ++j)
      എങ്കിൽ (bitset_test (FIRSTS (i), j - ntokens))
        വേണ്ടി (rule_സംഖ്യ k = 0; derives[j - ntokens][k]; ++k)
          bitset_set (FDERIVES (i), derives[j - ntokens][k]->സംഖ്യ);

  എങ്കിൽ (trace_flag & trace_sets)
    print_fderives ();

  bitsetv_free (firsts);
}



ശൂന്യം
closure_new (പൂർണ്ണസംഖ്യ n)
{
  itemset = xnmalloc (n, വലുപ്പം *itemset);

  ruleset = bitset_create (nrules, BITSET_FIXED);

  set_fderives ();
}



ശൂന്യം
closure (item_index സ്ഥിരം *core, size_t n)
{
  എങ്കിൽ (trace_flag & trace_closure)
    closure_print ("input", core, n);

  bitset_zero (ruleset);

  വേണ്ടി (size_t c = 0; c < n; ++c)
    എങ്കിൽ (ISVAR (ritem[core[c]]))
      bitset_or (ruleset, ruleset, FDERIVES (ritem[core[c]]));

  /* core is sorted on item index in ritem, which is sorted on rule number.
     Compute itemset with the same sort.  */
  nitemset = 0;
  size_t c = 0;

  /* A bit index over RULESET. */
  rule_സംഖ്യ ruleno;
  bitset_iterator iter;
  BITSET_FOR_EACH (iter, ruleset, ruleno, 0)
    {
      item_index itemno = rules[ruleno].rhs - ritem;
      ആകുമ്പോൾ (c < n && core[c] < itemno)
        {
          itemset[nitemset] = core[c];
          nitemset++;
          c++;
        }
      itemset[nitemset] = itemno;
      nitemset++;
    };

  ആകുമ്പോൾ (c < n)
    {
      itemset[nitemset] = core[c];
      nitemset++;
      c++;
    }

  എങ്കിൽ (trace_flag & trace_closure)
    closure_print ("output", itemset, nitemset);
}


ശൂന്യം
closure_free (ശൂന്യം)
{
  സ്വതന്ത്രം (itemset);
  bitset_free (ruleset);
  bitsetv_free (fderives);
}
