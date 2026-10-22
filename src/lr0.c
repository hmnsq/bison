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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "state.h"
#અન્તર્ભાવ "symtab.h"

પ્રકારવ્યાખ્યા સંરચના state_list
{
  સંરચના state_list *next;
  state *state;
} state_list;

સ્થિર state_list *first_state = ખાલી;
સ્થિર state_list *last_state = ખાલી;

/* Print CORE for debugging. */
સ્થિર શૂન્ય
core_print (size_t core_size, item_index *core, FILE *out)
{
  માટે (પૂર્ણાંક i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], ખાલી, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

સ્થિર state *
state_list_append (પ્રતીક_સંખ્યા sym, size_t core_size, item_index *core)
{
  state_list *ગાંઠ = xmalloc (કદ *ગાંઠ);
  state *res = state_new (sym, core_size, core);

  જો (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  ગાંઠ->next = ખાલી;
  ગાંઠ->state = res;

  જો (!first_state)
    first_state = ગાંઠ;
  જો (last_state)
    last_state->next = ગાંઠ;
  last_state = ગાંઠ;

  પાછા res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_પ્રતીક;

સ્થિર rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
સ્થિર state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
સ્થિર item_index **kernel_base;
સ્થિર પૂર્ણાંક *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
સ્થિર item_index *kernel_વસ્તુઓ;


સ્થિર શૂન્ય
allocate_itemsets (શૂન્ય)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t ગણતરી = 0;
  size_t *પ્રતીક_ગણતરી = xcalloc (nsyms + nuseless_nonterminals,
                                  કદ *પ્રતીક_ગણતરી);

  માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
    માટે (item_સંખ્યા *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*rhsp);
        ગણતરી += 1;
        પ્રતીક_ગણતરી[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, કદ *kernel_base);
  kernel_વસ્તુઓ = xnmalloc (ગણતરી, કદ *kernel_વસ્તુઓ);

  ગણતરી = 0;
  માટે (પ્રતીક_સંખ્યા i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_વસ્તુઓ + ગણતરી;
      ગણતરી += પ્રતીક_ગણતરી[i];
    }

  મુક્ત (પ્રતીક_ગણતરી);
  kernel_size = xnmalloc (nsyms, કદ *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
સ્થિર શૂન્ય
kernel_print (FILE *out)
{
  માટે (પ્રતીક_સંખ્યા i = 0; i < nsyms; ++i)
    જો (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
સ્થિર શૂન્ય
kernel_check (શૂન્ય)
{
  માટે (પ્રતીક_સંખ્યા i = 0; i < nsyms - 1; ++i)
    પ્રતિજ્ઞા (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

સ્થિર શૂન્ય
allocate_storage (શૂન્ય)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, કદ *shiftset);
  redset = xnmalloc (nrules, કદ *redset);
  state_hash_new ();
  shift_પ્રતીક = bitset_create (nsyms, BITSET_FIXED);
}


સ્થિર શૂન્ય
free_storage (શૂન્ય)
{
  bitset_free (shift_પ્રતીક);
  મુક્ત (redset);
  મુક્ત (shiftset);
  મુક્ત (kernel_base);
  મુક્ત (kernel_size);
  મુક્ત (kernel_વસ્તુઓ);
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

સ્થિર શૂન્ય
new_itemsets (state *s)
{
  જો (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->સંખ્યા);

  memset (kernel_size, 0, nsyms * કદ *kernel_size);

  bitset_zero (shift_પ્રતીક);

  જો (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  માટે (size_t i = 0; i < nitemset; ++i)
    જો (item_સંખ્યા_is_પ્રતીક_સંખ્યા (ritem[itemset[i]]))
      {
        જો (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], ખાલી, stderr);
            fputc ('\n', stderr);
          }
        પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (ritem[itemset[i]]);
        bitset_set (shift_પ્રતીક, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  જો (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->સંખ્યા);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

સ્થિર state *
get_state (પ્રતીક_સંખ્યા sym, size_t core_size, item_index *core)
{
  જો (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  જો (!s)
    s = state_list_append (sym, core_size, core);

  જો (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->સંખ્યા);

  પાછા s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

સ્થિર શૂન્ય
append_states (state *s)
{
  જો (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->સંખ્યા);

  bitset_iterator iter;
  પ્રતીક_સંખ્યા sym;
  પૂર્ણાંક i = 0;
  BITSET_FOR_EACH (iter, shift_પ્રતીક, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  જો (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->સંખ્યા);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

સ્થિર શૂન્ય
save_reductions (state *s)
{
  પૂર્ણાંક ગણતરી = 0;

  /* Find and count the active items that represent ends of rules. */
  માટે (size_t i = 0; i < nitemset; ++i)
    {
      item_સંખ્યા item = ritem[itemset[i]];
      જો (item_સંખ્યા_is_rule_સંખ્યા (item))
        {
          rule_સંખ્યા r = item_સંખ્યા_as_rule_સંખ્યા (item);
          redset[ગણતરી++] = &rules[r];
          જો (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  જો (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->સંખ્યા);
      માટે (પૂર્ણાંક i = 0; i < ગણતરી; ++i)
        {
          rule_print (redset[i], ખાલી, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, ગણતરી, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

સ્થિર શૂન્ય
set_states (શૂન્ય)
{
  states = xcalloc (nstates, કદ *states);

  જ્યારે (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      જો (!s->transitions)
        state_transitions_set (s, 0, 0);
      જો (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->સંખ્યા] = s;

      first_state = this->next;
      મુક્ત (this);
    }
  first_state = ખાલી;
  last_state = ખાલી;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

શૂન્ય
generate_states (શૂન્ય)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    માટે (rule_સંખ્યા r = 0; r < nrules && rules[r].lhs->પ્રતીક == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  માટે (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      જો (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->સંખ્યા,
                 symbols[s->accessing_પ્રતીક]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->વસ્તુઓ, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_ગણતરી (shift_પ્રતીક), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
