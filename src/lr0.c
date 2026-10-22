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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ state_list
{
  ಸಂರಚನಾ state_list *next;
  state *state;
} state_list;

ಸ್ಥಿರ state_list *first_state = ರಿಕ್ತ;
ಸ್ಥಿರ state_list *last_state = ರಿಕ್ತ;

/* Print CORE for debugging. */
ಸ್ಥಿರ ಶೂನ್ಯ
core_print (size_t core_size, item_index *core, FILE *out)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], ರಿಕ್ತ, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ state *
state_list_append (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, size_t core_size, item_index *core)
{
  state_list *ಗಂಟು = xmalloc (ಪರಿಮಾಣ *ಗಂಟು);
  state *res = state_new (sym, core_size, core);

  ಯದಿ (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  ಗಂಟು->next = ರಿಕ್ತ;
  ಗಂಟು->state = res;

  ಯದಿ (!first_state)
    first_state = ಗಂಟು;
  ಯದಿ (last_state)
    last_state->next = ಗಂಟು;
  last_state = ಗಂಟು;

  ಪ್ರತ್ಯಾಗಮ res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_ಚಿಹ್ನೆ;

ಸ್ಥಿರ rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
ಸ್ಥಿರ state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
ಸ್ಥಿರ item_index **kernel_base;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
ಸ್ಥಿರ item_index *kernel_ವಸ್ತುಗಳು;


ಸ್ಥಿರ ಶೂನ್ಯ
allocate_itemsets (ಶೂನ್ಯ)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t ಎಣಿಕೆ = 0;
  size_t *ಚಿಹ್ನೆ_ಎಣಿಕೆ = xcalloc (nsyms + nuseless_nonterminals,
                                  ಪರಿಮಾಣ *ಚಿಹ್ನೆ_ಎಣಿಕೆ);

  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp);
        ಎಣಿಕೆ += 1;
        ಚಿಹ್ನೆ_ಎಣಿಕೆ[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, ಪರಿಮಾಣ *kernel_base);
  kernel_ವಸ್ತುಗಳು = xnmalloc (ಎಣಿಕೆ, ಪರಿಮಾಣ *kernel_ವಸ್ತುಗಳು);

  ಎಣಿಕೆ = 0;
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_ವಸ್ತುಗಳು + ಎಣಿಕೆ;
      ಎಣಿಕೆ += ಚಿಹ್ನೆ_ಎಣಿಕೆ[i];
    }

  ಮುಕ್ತ (ಚಿಹ್ನೆ_ಎಣಿಕೆ);
  kernel_size = xnmalloc (nsyms, ಪರಿಮಾಣ *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
ಸ್ಥಿರ ಶೂನ್ಯ
kernel_print (FILE *out)
{
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < nsyms; ++i)
    ಯದಿ (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
ಸ್ಥಿರ ಶೂನ್ಯ
kernel_check (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < nsyms - 1; ++i)
    ಪ್ರತಿಜ್ಞಾ (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
allocate_storage (ಶೂನ್ಯ)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, ಪರಿಮಾಣ *shiftset);
  redset = xnmalloc (nrules, ಪರಿಮಾಣ *redset);
  state_hash_new ();
  shift_ಚಿಹ್ನೆ = bitset_create (nsyms, BITSET_FIXED);
}


ಸ್ಥಿರ ಶೂನ್ಯ
free_storage (ಶೂನ್ಯ)
{
  bitset_free (shift_ಚಿಹ್ನೆ);
  ಮುಕ್ತ (redset);
  ಮುಕ್ತ (shiftset);
  ಮುಕ್ತ (kernel_base);
  ಮುಕ್ತ (kernel_size);
  ಮುಕ್ತ (kernel_ವಸ್ತುಗಳು);
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

ಸ್ಥಿರ ಶೂನ್ಯ
new_itemsets (state *s)
{
  ಯದಿ (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->ಸಂಖ್ಯೆ);

  memset (kernel_size, 0, nsyms * ಪರಿಮಾಣ *kernel_size);

  bitset_zero (shift_ಚಿಹ್ನೆ);

  ಯದಿ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  ಪ್ರತಿ (size_t i = 0; i < nitemset; ++i)
    ಯದಿ (item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[itemset[i]]))
      {
        ಯದಿ (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], ರಿಕ್ತ, stderr);
            fputc ('\n', stderr);
          }
        ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[itemset[i]]);
        bitset_set (shift_ಚಿಹ್ನೆ, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  ಯದಿ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->ಸಂಖ್ಯೆ);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

ಸ್ಥಿರ state *
get_state (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, size_t core_size, item_index *core)
{
  ಯದಿ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  ಯದಿ (!s)
    s = state_list_append (sym, core_size, core);

  ಯದಿ (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->ಸಂಖ್ಯೆ);

  ಪ್ರತ್ಯಾಗಮ s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
append_states (state *s)
{
  ಯದಿ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->ಸಂಖ್ಯೆ);

  bitset_iterator iter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym;
  ಪೂರ್ಣಾಙ್ಕ i = 0;
  BITSET_FOR_EACH (iter, shift_ಚಿಹ್ನೆ, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  ಯದಿ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->ಸಂಖ್ಯೆ);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
save_reductions (state *s)
{
  ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;

  /* Find and count the active items that represent ends of rules. */
  ಪ್ರತಿ (size_t i = 0; i < nitemset; ++i)
    {
      item_ಸಂಖ್ಯೆ item = ritem[itemset[i]];
      ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (item))
        {
          rule_ಸಂಖ್ಯೆ r = item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (item);
          redset[ಎಣಿಕೆ++] = &rules[r];
          ಯದಿ (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  ಯದಿ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->ಸಂಖ್ಯೆ);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; ++i)
        {
          rule_print (redset[i], ರಿಕ್ತ, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, ಎಣಿಕೆ, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
set_states (ಶೂನ್ಯ)
{
  states = xcalloc (nstates, ಪರಿಮಾಣ *states);

  ಯಾವತ್ (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      ಯದಿ (!s->transitions)
        state_transitions_set (s, 0, 0);
      ಯದಿ (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->ಸಂಖ್ಯೆ] = s;

      first_state = this->next;
      ಮುಕ್ತ (this);
    }
  first_state = ರಿಕ್ತ;
  last_state = ರಿಕ್ತ;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

ಶೂನ್ಯ
generate_states (ಶೂನ್ಯ)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules && rules[r].lhs->ಚಿಹ್ನೆ == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  ಪ್ರತಿ (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      ಯದಿ (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->ಸಂಖ್ಯೆ,
                 symbols[s->accessing_ಚಿಹ್ನೆ]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->ವಸ್ತುಗಳು, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_ಎಣಿಕೆ (shift_ಚಿಹ್ನೆ), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
