/* Generate the LR(0) parser states for Bison.

   Copyright (C) 1984, 1986, 1989, 2000-2002, 2004-2015, 2018-2022 Free
   Software Foundation, Inc.

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


/* See comments in state.h for the data structures that represent it.
   The entry point is generate_states.  */

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lr0.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "state.h"
#অন্তর্ভাব "symtab.h"

প্রকারলেখ কাঠামো state_list
{
  কাঠামো state_list *পরবর্তী;
  state *state;
} state_list;

স্থির state_list *first_state = ফাঁকা;
স্থির state_list *last_state = ফাঁকা;

/* Print CORE for debugging. */
স্থির শূন্য
core_print (size_t core_আকার, item_সূচক *core, FILE *out)
{
  জন্য (পূর্ণসংখ্যা i = 0; i < core_আকার; ++i)
    {
      item_print (ritem + core[i], ফাঁকা, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

স্থির state *
state_list_append (প্রতীক_সংখ্যা sym, size_t core_আকার, item_সূচক *core)
{
  state_list *গ্রন্থি = xmalloc (আকার *গ্রন্থি);
  state *res = state_new (sym, core_আকার, core);

  যদি (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  গ্রন্থি->পরবর্তী = ফাঁকা;
  গ্রন্থি->state = res;

  যদি (!first_state)
    first_state = গ্রন্থি;
  যদি (last_state)
    last_state->পরবর্তী = গ্রন্থি;
  last_state = গ্রন্থি;

  ফেরত res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_প্রতীক;

স্থির rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
স্থির state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
স্থির item_সূচক **kernel_base;
স্থির পূর্ণসংখ্যা *kernel_আকার;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
স্থির item_সূচক *kernel_বস্তুসমূহ;


স্থির শূন্য
allocate_itemsets (শূন্য)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t গুনতি = 0;
  size_t *প্রতীক_গুনতি = xcalloc (nsyms + nuseless_nonterminals,
                                  আকার *প্রতীক_গুনতি);

  জন্য (rule_সংখ্যা r = 0; r < nrules; ++r)
    জন্য (item_সংখ্যা *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*rhsp);
        গুনতি += 1;
        প্রতীক_গুনতি[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, আকার *kernel_base);
  kernel_বস্তুসমূহ = xnmalloc (গুনতি, আকার *kernel_বস্তুসমূহ);

  গুনতি = 0;
  জন্য (প্রতীক_সংখ্যা i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_বস্তুসমূহ + গুনতি;
      গুনতি += প্রতীক_গুনতি[i];
    }

  মুক্ত (প্রতীক_গুনতি);
  kernel_আকার = xnmalloc (nsyms, আকার *kernel_আকার);
}

/* Print the current kernel (in KERNEL_BASE). */
স্থির শূন্য
kernel_print (FILE *out)
{
  জন্য (প্রতীক_সংখ্যা i = 0; i < nsyms; ++i)
    যদি (kernel_আকার[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_আকার[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
স্থির শূন্য
kernel_check (শূন্য)
{
  জন্য (প্রতীক_সংখ্যা i = 0; i < nsyms - 1; ++i)
    প্রতিজ্ঞা (kernel_base[i] + kernel_আকার[i] <= kernel_base[i + 1]);
}

স্থির শূন্য
allocate_storage (শূন্য)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, আকার *shiftset);
  redset = xnmalloc (nrules, আকার *redset);
  state_hash_new ();
  shift_প্রতীক = bitset_create (nsyms, BITSET_FIXED);
}


স্থির শূন্য
free_storage (শূন্য)
{
  bitset_free (shift_প্রতীক);
  মুক্ত (redset);
  মুক্ত (shiftset);
  মুক্ত (kernel_base);
  মুক্ত (kernel_আকার);
  মুক্ত (kernel_বস্তুসমূহ);
  state_hash_free ();
}




/*------------------------------------------------------------------.
| Find which term/nterm symbols can be "shifted" in S, and for each |
| one record which items would be active after that transition.     |
| Uses the contents of itemset.                                     |
|                                                                   |
| shift_symbol is a bitset of the term/nterm symbols that can be    |
| shifted.  For each symbol in the grammar, kernel_base[symbol]     |
| points to a vector of item numbers activated if that symbol is    |
| shifted, and kernel_size[symbol] is their numbers.                |
|                                                                   |
| itemset is sorted on item index in ritem, which is sorted on rule |
| number.  Compute each kernel_base[symbol] with the same sort.     |
`------------------------------------------------------------------*/

স্থির শূন্য
new_itemsets (state *s)
{
  যদি (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->সংখ্যা);

  memset (kernel_আকার, 0, nsyms * আকার *kernel_আকার);

  bitset_zero (shift_প্রতীক);

  যদি (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  জন্য (size_t i = 0; i < nitemset; ++i)
    যদি (item_সংখ্যা_is_প্রতীক_সংখ্যা (ritem[itemset[i]]))
      {
        যদি (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], ফাঁকা, stderr);
            fputc ('\n', stderr);
          }
        প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (ritem[itemset[i]]);
        bitset_set (shift_প্রতীক, sym);
        kernel_base[sym][kernel_আকার[sym]] = itemset[i] + 1;
        kernel_আকার[sym]++;
      }

  যদি (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->সংখ্যা);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

স্থির state *
get_state (প্রতীক_সংখ্যা sym, size_t core_আকার, item_সূচক *core)
{
  যদি (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_আকার, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_আকার, core);
  যদি (!s)
    s = state_list_append (sym, core_আকার, core);

  যদি (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->সংখ্যা);

  ফেরত s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

স্থির শূন্য
append_states (state *s)
{
  যদি (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->সংখ্যা);

  bitset_iterator iter;
  প্রতীক_সংখ্যা sym;
  পূর্ণসংখ্যা i = 0;
  BITSET_FOR_EACH (iter, shift_প্রতীক, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_আকার[sym], kernel_base[sym]);
      ++i;
    }

  যদি (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->সংখ্যা);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

স্থির শূন্য
save_reductions (state *s)
{
  পূর্ণসংখ্যা গুনতি = 0;

  /* Find and count the active items that represent ends of rules. */
  জন্য (size_t i = 0; i < nitemset; ++i)
    {
      item_সংখ্যা item = ritem[itemset[i]];
      যদি (item_সংখ্যা_is_rule_সংখ্যা (item))
        {
          rule_সংখ্যা r = item_সংখ্যা_as_rule_সংখ্যা (item);
          redset[গুনতি++] = &rules[r];
          যদি (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  যদি (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->সংখ্যা);
      জন্য (পূর্ণসংখ্যা i = 0; i < গুনতি; ++i)
        {
          rule_print (redset[i], ফাঁকা, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, গুনতি, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

স্থির শূন্য
set_states (শূন্য)
{
  states = xcalloc (nstates, আকার *states);

  যখন (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      যদি (!s->transitions)
        state_transitions_set (s, 0, 0);
      যদি (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->সংখ্যা] = s;

      first_state = this->পরবর্তী;
      মুক্ত (this);
    }
  first_state = ফাঁকা;
  last_state = ফাঁকা;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

শূন্য
generate_states (শূন্য)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_আকার[0] = 0;
    জন্য (rule_সংখ্যা r = 0; r < nrules && rules[r].lhs->প্রতীক == acceptsymbol; ++r)
      kernel_base[0][kernel_আকার[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_আকার[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  জন্য (state_list *list = first_state; list; list = list->পরবর্তী)
    {
      state *s = list->state;
      যদি (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->সংখ্যা,
                 symbols[s->accessing_প্রতীক]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->বস্তুসমূহ, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_গুনতি (shift_প্রতীক), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
