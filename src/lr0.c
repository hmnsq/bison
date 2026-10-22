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

#include <config.h>
#include "system.h"

#include <bitset.h>

#include "closure.h"
#include "complain.h"
#include "getargs.h"
#include "gram.h"
#include "lalr.h"
#include "lr0.h"
#include "reader.h"
#include "reduce.h"
#include "state.h"
#include "symtab.h"

typedef struct state_list
{
  struct state_list *next;
  state *state;
} state_list;

static state_list *first_state = NULL;
static state_list *last_state = NULL;

/* Print CORE for debugging. */
static void
core_इलिम्बाग (size_t core_size, item_index *core, FILE *out)
{
  for (int i = 0; i < core_size; ++i)
    {
      item_इलिम्बाग (ritem + core[i], NULL, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

static state *
state_list_append (सिम्बोलो_नुमेरो sym, size_t core_size, item_index *core)
{
  state_list *बुहोल = xmalloc (sizeof *बुहोल);
  state *res = state_new (sym, core_size, core);

  if (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  बुहोल->next = NULL;
  बुहोल->state = res;

  if (!first_state)
    first_state = बुहोल;
  if (last_state)
    last_state->next = बुहोल;
  last_state = बुहोल;

  return res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_सिम्बोलो;

static rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
static state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
static item_index **kernel_base;
static int *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
static item_index *kernel_मगा_बागाय;


static void
allocate_itemsets (void)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t बिलांग = 0;
  size_t *सिम्बोलो_बिलांग = xcalloc (nsyms + nuseless_nonterminals,
                                  sizeof *सिम्बोलो_बिलांग);

  for (rule_नुमेरो r = 0; r < nrules; ++r)
    for (item_नुमेरो *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        सिम्बोलो_नुमेरो sym = item_नुमेरो_as_सिम्बोलो_नुमेरो (*rhsp);
        बिलांग += 1;
        सिम्बोलो_बिलांग[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, sizeof *kernel_base);
  kernel_मगा_बागाय = xnmalloc (बिलांग, sizeof *kernel_मगा_बागाय);

  बिलांग = 0;
  for (सिम्बोलो_नुमेरो i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_मगा_बागाय + बिलांग;
      बिलांग += सिम्बोलो_बिलांग[i];
    }

  free (सिम्बोलो_बिलांग);
  kernel_size = xnmalloc (nsyms, sizeof *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
static void
kernel_इलिम्बाग (FILE *out)
{
  for (सिम्बोलो_नुमेरो i = 0; i < nsyms; ++i)
    if (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_इलिम्बाग (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
static void
kernel_check (void)
{
  for (सिम्बोलो_नुमेरो i = 0; i < nsyms - 1; ++i)
    assert (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

static void
allocate_storage (void)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, sizeof *shiftset);
  redset = xnmalloc (nrules, sizeof *redset);
  state_hash_new ();
  shift_सिम्बोलो = bitset_create (nsyms, BITSET_FIXED);
}


static void
free_storage (void)
{
  bitset_free (shift_सिम्बोलो);
  free (redset);
  free (shiftset);
  free (kernel_base);
  free (kernel_size);
  free (kernel_मगा_बागाय);
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

static void
new_itemsets (state *s)
{
  if (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->नुमेरो_2);

  memset (kernel_size, 0, nsyms * sizeof *kernel_size);

  bitset_zero (shift_सिम्बोलो);

  if (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_इलिम्बाग (stderr);
    }

  for (size_t i = 0; i < nitemset; ++i)
    if (item_नुमेरो_is_सिम्बोलो_नुमेरो (ritem[itemset[i]]))
      {
        if (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_इलिम्बाग (ritem + itemset[i], NULL, stderr);
            fputc ('\n', stderr);
          }
        सिम्बोलो_नुमेरो sym = item_नुमेरो_as_सिम्बोलो_नुमेरो (ritem[itemset[i]]);
        bitset_set (shift_सिम्बोलो, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  if (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_इलिम्बाग (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->नुमेरो_2);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

static state *
get_state (सिम्बोलो_नुमेरो sym, size_t core_size, item_index *core)
{
  if (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_इलिम्बाग (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  if (!s)
    s = state_list_append (sym, core_size, core);

  if (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->नुमेरो_2);

  return s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

static void
append_states (state *s)
{
  if (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->नुमेरो_2);

  bitset_iterator iter;
  सिम्बोलो_नुमेरो sym;
  int i = 0;
  BITSET_FOR_EACH (iter, shift_सिम्बोलो, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  if (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->नुमेरो_2);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

static void
save_reductions (state *s)
{
  int बिलांग = 0;

  /* Find and count the active items that represent ends of rules. */
  for (size_t i = 0; i < nitemset; ++i)
    {
      item_नुमेरो item = ritem[itemset[i]];
      if (item_नुमेरो_is_rule_नुमेरो (item))
        {
          rule_नुमेरो r = item_नुमेरो_as_rule_नुमेरो (item);
          redset[बिलांग++] = &rules[r];
          if (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  if (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->नुमेरो_2);
      for (int i = 0; i < बिलांग; ++i)
        {
          rule_इलिम्बाग (redset[i], NULL, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, बिलांग, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

static void
set_states (void)
{
  states = xcalloc (nstates, sizeof *states);

  while (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      if (!s->transitions)
        state_transitions_set (s, 0, 0);
      if (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->नुमेरो_2] = s;

      first_state = this->next;
      free (this);
    }
  first_state = NULL;
  last_state = NULL;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

void
generate_states (void)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    for (rule_नुमेरो r = 0; r < nrules && rules[r].lhs->सिम्बोलो == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  for (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      if (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->नुमेरो_2,
                 symbols[s->accessing_सिम्बोलो]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->मगा_बागाय, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_बिलांग (shift_सिम्बोलो), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
