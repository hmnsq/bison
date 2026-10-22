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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "state.h"
#அந்தர்பாவ "symtab.h"

வகைவரை கட்டமைப்பு state_list
{
  கட்டமைப்பு state_list *next;
  state *state;
} state_list;

நிலையான state_list *first_state = வெற்று;
நிலையான state_list *last_state = வெற்று;

/* Print CORE for debugging. */
நிலையான வெறுமை
core_print (size_t core_size, item_index *core, FILE *out)
{
  க்கு (முழு i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], வெற்று, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

நிலையான state *
state_list_append (குறியீடு_எண் sym, size_t core_size, item_index *core)
{
  state_list *முனை = xmalloc (அளவு *முனை);
  state *res = state_new (sym, core_size, core);

  எனில் (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  முனை->next = வெற்று;
  முனை->state = res;

  எனில் (!first_state)
    first_state = முனை;
  எனில் (last_state)
    last_state->next = முனை;
  last_state = முனை;

  திரும்பு res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_குறியீடு;

நிலையான rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
நிலையான state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
நிலையான item_index **kernel_base;
நிலையான முழு *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
நிலையான item_index *kernel_பொருட்கள்;


நிலையான வெறுமை
allocate_itemsets (வெறுமை)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t மொத்தம் = 0;
  size_t *குறியீடு_மொத்தம் = xcalloc (nsyms + nuseless_nonterminals,
                                  அளவு *குறியீடு_மொத்தம்);

  க்கு (rule_எண் r = 0; r < nrules; ++r)
    க்கு (item_எண் *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (*rhsp);
        மொத்தம் += 1;
        குறியீடு_மொத்தம்[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, அளவு *kernel_base);
  kernel_பொருட்கள் = xnmalloc (மொத்தம், அளவு *kernel_பொருட்கள்);

  மொத்தம் = 0;
  க்கு (குறியீடு_எண் i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_பொருட்கள் + மொத்தம்;
      மொத்தம் += குறியீடு_மொத்தம்[i];
    }

  விடுவி (குறியீடு_மொத்தம்);
  kernel_size = xnmalloc (nsyms, அளவு *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
நிலையான வெறுமை
kernel_print (FILE *out)
{
  க்கு (குறியீடு_எண் i = 0; i < nsyms; ++i)
    எனில் (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
நிலையான வெறுமை
kernel_check (வெறுமை)
{
  க்கு (குறியீடு_எண் i = 0; i < nsyms - 1; ++i)
    ப்ரதிஜ்ஞா (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

நிலையான வெறுமை
allocate_storage (வெறுமை)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, அளவு *shiftset);
  redset = xnmalloc (nrules, அளவு *redset);
  state_hash_new ();
  shift_குறியீடு = bitset_create (nsyms, BITSET_FIXED);
}


நிலையான வெறுமை
free_storage (வெறுமை)
{
  bitset_free (shift_குறியீடு);
  விடுவி (redset);
  விடுவி (shiftset);
  விடுவி (kernel_base);
  விடுவி (kernel_size);
  விடுவி (kernel_பொருட்கள்);
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

நிலையான வெறுமை
new_itemsets (state *s)
{
  எனில் (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->எண்);

  memset (kernel_size, 0, nsyms * அளவு *kernel_size);

  bitset_zero (shift_குறியீடு);

  எனில் (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  க்கு (size_t i = 0; i < nitemset; ++i)
    எனில் (item_எண்_is_குறியீடு_எண் (ritem[itemset[i]]))
      {
        எனில் (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], வெற்று, stderr);
            fputc ('\n', stderr);
          }
        குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (ritem[itemset[i]]);
        bitset_set (shift_குறியீடு, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  எனில் (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->எண்);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

நிலையான state *
get_state (குறியீடு_எண் sym, size_t core_size, item_index *core)
{
  எனில் (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  எனில் (!s)
    s = state_list_append (sym, core_size, core);

  எனில் (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->எண்);

  திரும்பு s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

நிலையான வெறுமை
append_states (state *s)
{
  எனில் (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->எண்);

  bitset_iterator iter;
  குறியீடு_எண் sym;
  முழு i = 0;
  BITSET_FOR_EACH (iter, shift_குறியீடு, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  எனில் (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->எண்);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

நிலையான வெறுமை
save_reductions (state *s)
{
  முழு மொத்தம் = 0;

  /* Find and count the active items that represent ends of rules. */
  க்கு (size_t i = 0; i < nitemset; ++i)
    {
      item_எண் item = ritem[itemset[i]];
      எனில் (item_எண்_is_rule_எண் (item))
        {
          rule_எண் r = item_எண்_as_rule_எண் (item);
          redset[மொத்தம்++] = &rules[r];
          எனில் (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  எனில் (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->எண்);
      க்கு (முழு i = 0; i < மொத்தம்; ++i)
        {
          rule_print (redset[i], வெற்று, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, மொத்தம், redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

நிலையான வெறுமை
set_states (வெறுமை)
{
  states = xcalloc (nstates, அளவு *states);

  போது (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      எனில் (!s->transitions)
        state_transitions_set (s, 0, 0);
      எனில் (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->எண்] = s;

      first_state = this->next;
      விடுவி (this);
    }
  first_state = வெற்று;
  last_state = வெற்று;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

வெறுமை
generate_states (வெறுமை)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    க்கு (rule_எண் r = 0; r < nrules && rules[r].lhs->குறியீடு == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  க்கு (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      எனில் (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->எண்,
                 symbols[s->accessing_குறியீடு]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->பொருட்கள், s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_மொத்தம் (shift_குறியீடு), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
