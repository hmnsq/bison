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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ state_list
{
  ರಚನೆ state_list *ಮುಂದಿನ;
  state *state;
} state_list;

ಸ್ಥಿರ state_list *ಮೊದಲ_state = ಖಾಲಿ;
ಸ್ಥಿರ state_list *ಕೊನೆಯ_state = ಖಾಲಿ;

/* Print CORE for debugging. */
ಸ್ಥಿರ ಶೂನ್ಯ
core_print (size_t core_ಗಾತ್ರ, ಅಂಶ_ಸೂಚ್ಯಂಕ *core, FILE *ಹೊರಹೋಗು)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < core_ಗಾತ್ರ; ++i)
    {
      ಅಂಶ_print (ritem + core[i], ಖಾಲಿ, ಹೊರಹೋಗು);
      fputc ('\n', ಹೊರಹೋಗು);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ state *
state_list_append (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, size_t core_ಗಾತ್ರ, ಅಂಶ_ಸೂಚ್ಯಂಕ *core)
{
  state_list *ಗಂಟು = xmalloc (ಗಾತ್ರ *ಗಂಟು);
  state *res = state_new (sym, core_ಗಾತ್ರ, core);

  ಆದರೆ (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->ಟ್ಯಾಗ್);

  ಗಂಟು->ಮುಂದಿನ = ಖಾಲಿ;
  ಗಂಟು->state = res;

  ಆದರೆ (!ಮೊದಲ_state)
    ಮೊದಲ_state = ಗಂಟು;
  ಆದರೆ (ಕೊನೆಯ_state)
    ಕೊನೆಯ_state->ಮುಂದಿನ = ಗಂಟು;
  ಕೊನೆಯ_state = ಗಂಟು;

  ಹಿಂತಿರುಗಿ res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset ಸರಿಸು_ಚಿಹ್ನೆ;

ಸ್ಥಿರ ನಿಯಮ **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
ಸ್ಥಿರ state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
ಸ್ಥಿರ ಅಂಶ_ಸೂಚ್ಯಂಕ **kernel_ತಳ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *kernel_ಗಾತ್ರ;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
ಸ್ಥಿರ ಅಂಶ_ಸೂಚ್ಯಂಕ *kernel_ವಸ್ತುಗಳು;


ಸ್ಥಿರ ಶೂನ್ಯ
allocate_itemsets (ಶೂನ್ಯ)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t ಎಣಿಕೆ_2 = 0;
  size_t *ಚಿಹ್ನೆ_ಎಣಿಕೆ = xcalloc (nsyms + nuseless_nonterminals,
                                  ಗಾತ್ರ *ಚಿಹ್ನೆ_ಎಣಿಕೆ);

  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = ನಿಯಮಗಳು[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp);
        ಎಣಿಕೆ_2 += 1;
        ಚಿಹ್ನೆ_ಎಣಿಕೆ[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_ತಳ = xnmalloc (nsyms, ಗಾತ್ರ *kernel_ತಳ);
  kernel_ವಸ್ತುಗಳು = xnmalloc (ಎಣಿಕೆ_2, ಗಾತ್ರ *kernel_ವಸ್ತುಗಳು);

  ಎಣಿಕೆ_2 = 0;
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < nsyms; i++)
    {
      kernel_ತಳ[i] = kernel_ವಸ್ತುಗಳು + ಎಣಿಕೆ_2;
      ಎಣಿಕೆ_2 += ಚಿಹ್ನೆ_ಎಣಿಕೆ[i];
    }

  ಬಿಡುಗಡೆ (ಚಿಹ್ನೆ_ಎಣಿಕೆ);
  kernel_ಗಾತ್ರ = xnmalloc (nsyms, ಗಾತ್ರ *kernel_ಗಾತ್ರ);
}

/* Print the current kernel (in KERNEL_BASE). */
ಸ್ಥಿರ ಶೂನ್ಯ
kernel_print (FILE *ಹೊರಹೋಗು)
{
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < nsyms; ++i)
    ಆದರೆ (kernel_ಗಾತ್ರ[i])
      {
        fprintf (ಹೊರಹೋಗು, "kernel[%s] =\n", symbols[i]->ಟ್ಯಾಗ್);
        core_print (kernel_ಗಾತ್ರ[i], kernel_ತಳ[i], ಹೊರಹೋಗು);
      }
}

/* Make sure the kernel is in sane state. */
ಸ್ಥಿರ ಶೂನ್ಯ
kernel_check (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < nsyms - 1; ++i)
    ಪ್ರತಿಜ್ಞಾ (kernel_ತಳ[i] + kernel_ಗಾತ್ರ[i] <= kernel_ತಳ[i + 1]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
allocate_storage (ಶೂನ್ಯ)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, ಗಾತ್ರ *shiftset);
  redset = xnmalloc (nrules, ಗಾತ್ರ *redset);
  state_hash_new ();
  ಸರಿಸು_ಚಿಹ್ನೆ = bitset_create (nsyms, BITSET_FIXED);
}


ಸ್ಥಿರ ಶೂನ್ಯ
free_storage (ಶೂನ್ಯ)
{
  bitset_free (ಸರಿಸು_ಚಿಹ್ನೆ);
  ಬಿಡುಗಡೆ (redset);
  ಬಿಡುಗಡೆ (shiftset);
  ಬಿಡುಗಡೆ (kernel_ತಳ);
  ಬಿಡುಗಡೆ (kernel_ಗಾತ್ರ);
  ಬಿಡುಗಡೆ (kernel_ವಸ್ತುಗಳು);
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
  ಆದರೆ (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->ಸಂಖ್ಯೆ);

  memset (kernel_ಗಾತ್ರ, 0, nsyms * ಗಾತ್ರ *kernel_ಗಾತ್ರ);

  bitset_zero (ಸರಿಸು_ಚಿಹ್ನೆ);

  ಆದರೆ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  ಗಾಗಿ (size_t i = 0; i < nitemset; ++i)
    ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[itemset[i]]))
      {
        ಆದರೆ (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            ಅಂಶ_print (ritem + itemset[i], ಖಾಲಿ, stderr);
            fputc ('\n', stderr);
          }
        ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[itemset[i]]);
        bitset_ಗಣ (ಸರಿಸು_ಚಿಹ್ನೆ, sym);
        kernel_ತಳ[sym][kernel_ಗಾತ್ರ[sym]] = itemset[i] + 1;
        kernel_ಗಾತ್ರ[sym]++;
      }

  ಆದರೆ (trace_flag & trace_automaton)
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
get_state (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, size_t core_ಗಾತ್ರ, ಅಂಶ_ಸೂಚ್ಯಂಕ *core)
{
  ಆದರೆ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->ಟ್ಯಾಗ್);
      core_print (core_ಗಾತ್ರ, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_ಗಾತ್ರ, core);
  ಆದರೆ (!s)
    s = state_list_append (sym, core_ಗಾತ್ರ, core);

  ಆದರೆ (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->ಸಂಖ್ಯೆ);

  ಹಿಂತಿರುಗಿ s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
append_ಸ್ಥಿತಿಗಳು (state *s)
{
  ಆದರೆ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->ಸಂಖ್ಯೆ);

  bitset_iterator iter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym;
  ಪೂರ್ಣಾಂಕ i = 0;
  BITSET_FOR_EACH (iter, ಸರಿಸು_ಚಿಹ್ನೆ, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_ಗಾತ್ರ[sym], kernel_ತಳ[sym]);
      ++i;
    }

  ಆದರೆ (trace_flag & trace_automaton)
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
  ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

  /* Find and count the active items that represent ends of rules. */
  ಗಾಗಿ (size_t i = 0; i < nitemset; ++i)
    {
      ಅಂಶ_ಸಂಖ್ಯೆ ಅಂಶ = ritem[itemset[i]];
      ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ಅಂಶ))
        {
          ನಿಯಮ_ಸಂಖ್ಯೆ r = ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (ಅಂಶ);
          redset[ಎಣಿಕೆ_2++] = &ನಿಯಮಗಳು[r];
          ಆದರೆ (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  ಆದರೆ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->ಸಂಖ್ಯೆ);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_2; ++i)
        {
          ನಿಯಮ_print (redset[i], ಖಾಲಿ, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_ಗಣ (s, ಎಣಿಕೆ_2, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಸ್ಥಿತಿಗಳು (ಶೂನ್ಯ)
{
  ಸ್ಥಿತಿಗಳು = xcalloc (nstates, ಗಾತ್ರ *ಸ್ಥಿತಿಗಳು);

  ಆಗ (ಮೊದಲ_state)
    {
      state_list *this = ಮೊದಲ_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      ಆದರೆ (!s->transitions)
        state_transitions_ಗಣ (s, 0, 0);
      ಆದರೆ (!s->reductions)
        state_reductions_ಗಣ (s, 0, 0);

      ಸ್ಥಿತಿಗಳು[s->ಸಂಖ್ಯೆ] = s;

      ಮೊದಲ_state = this->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (this);
    }
  ಮೊದಲ_state = ಖಾಲಿ;
  ಕೊನೆಯ_state = ಖಾಲಿ;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

ಶೂನ್ಯ
generate_ಸ್ಥಿತಿಗಳು (ಶೂನ್ಯ)
{
  allocate_storage ();
  ಆವರಣ_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_ಗಾತ್ರ[0] = 0;
    ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules && ನಿಯಮಗಳು[r].lhs->ಚಿಹ್ನೆ == acceptsymbol; ++r)
      kernel_ತಳ[0][kernel_ಗಾತ್ರ[0]++] = ನಿಯಮಗಳು[r].rhs - ritem;
    state_list_append (0, kernel_ಗಾತ್ರ[0], kernel_ತಳ[0]);
  }

  /* States are queued when they are created; process them all.  */
  ಗಾಗಿ (state_list *list = ಮೊದಲ_state; list; list = list->ಮುಂದಿನ)
    {
      state *s = list->state;
      ಆದರೆ (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->ಸಂಖ್ಯೆ,
                 symbols[s->accessing_ಚಿಹ್ನೆ]->ಟ್ಯಾಗ್);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      ಆವರಣ (s->ವಸ್ತುಗಳು, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_ಸ್ಥಿತಿಗಳು (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_ಗಣ (s, bitset_ಎಣಿಕೆ (ಸರಿಸು_ಚಿಹ್ನೆ), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  ಗಣ_ಸ್ಥಿತಿಗಳು ();
}
