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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>

#شامل "closure.h"
#شامل "complain.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "state.h"
#شامل "symtab.h"

تعریفنوع ساختار state_list
{
  ساختار state_list *next;
  state *state;
} state_list;

ایستا state_list *first_state = پوچ;
ایستا state_list *last_state = پوچ;

/* Print CORE for debugging. */
ایستا تهی
core_print (size_t core_size, item_index *core, FILE *out)
{
  برای (صحیح i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], پوچ, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

ایستا state *
state_list_append (نماد_عدد sym, size_t core_size, item_index *core)
{
  state_list *گره = xmalloc (اندازه *گره);
  state *res = state_new (sym, core_size, core);

  اگر (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  گره->next = پوچ;
  گره->state = res;

  اگر (!first_state)
    first_state = گره;
  اگر (last_state)
    last_state->next = گره;
  last_state = گره;

  بازگشت res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_نماد;

ایستا rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
ایستا state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
ایستا item_index **kernel_base;
ایستا صحیح *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
ایستا item_index *kernel_اقلام;


ایستا تهی
allocate_itemsets (تهی)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t شمار = 0;
  size_t *نماد_شمار = xcalloc (nsyms + nuseless_nonterminals,
                                  اندازه *نماد_شمار);

  برای (rule_عدد r = 0; r < nrules; ++r)
    برای (item_عدد *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        نماد_عدد sym = item_عدد_as_نماد_عدد (*rhsp);
        شمار += 1;
        نماد_شمار[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, اندازه *kernel_base);
  kernel_اقلام = xnmalloc (شمار, اندازه *kernel_اقلام);

  شمار = 0;
  برای (نماد_عدد i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_اقلام + شمار;
      شمار += نماد_شمار[i];
    }

  آزاد (نماد_شمار);
  kernel_size = xnmalloc (nsyms, اندازه *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
ایستا تهی
kernel_print (FILE *out)
{
  برای (نماد_عدد i = 0; i < nsyms; ++i)
    اگر (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
ایستا تهی
kernel_check (تهی)
{
  برای (نماد_عدد i = 0; i < nsyms - 1; ++i)
    أكد (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

ایستا تهی
allocate_storage (تهی)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, اندازه *shiftset);
  redset = xnmalloc (nrules, اندازه *redset);
  state_hash_new ();
  shift_نماد = bitset_create (nsyms, BITSET_FIXED);
}


ایستا تهی
free_storage (تهی)
{
  bitset_free (shift_نماد);
  آزاد (redset);
  آزاد (shiftset);
  آزاد (kernel_base);
  آزاد (kernel_size);
  آزاد (kernel_اقلام);
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

ایستا تهی
new_itemsets (state *s)
{
  اگر (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->عدد);

  memset (kernel_size, 0, nsyms * اندازه *kernel_size);

  bitset_zero (shift_نماد);

  اگر (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  برای (size_t i = 0; i < nitemset; ++i)
    اگر (item_عدد_is_نماد_عدد (ritem[itemset[i]]))
      {
        اگر (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], پوچ, stderr);
            fputc ('\n', stderr);
          }
        نماد_عدد sym = item_عدد_as_نماد_عدد (ritem[itemset[i]]);
        bitset_set (shift_نماد, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  اگر (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->عدد);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

ایستا state *
get_state (نماد_عدد sym, size_t core_size, item_index *core)
{
  اگر (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  اگر (!s)
    s = state_list_append (sym, core_size, core);

  اگر (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->عدد);

  بازگشت s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

ایستا تهی
append_states (state *s)
{
  اگر (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->عدد);

  bitset_iterator iter;
  نماد_عدد sym;
  صحیح i = 0;
  BITSET_FOR_EACH (iter, shift_نماد, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  اگر (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->عدد);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

ایستا تهی
save_reductions (state *s)
{
  صحیح شمار = 0;

  /* Find and count the active items that represent ends of rules. */
  برای (size_t i = 0; i < nitemset; ++i)
    {
      item_عدد item = ritem[itemset[i]];
      اگر (item_عدد_is_rule_عدد (item))
        {
          rule_عدد r = item_عدد_as_rule_عدد (item);
          redset[شمار++] = &rules[r];
          اگر (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  اگر (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->عدد);
      برای (صحیح i = 0; i < شمار; ++i)
        {
          rule_print (redset[i], پوچ, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, شمار, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

ایستا تهی
set_states (تهی)
{
  states = xcalloc (nstates, اندازه *states);

  وقتی (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      اگر (!s->transitions)
        state_transitions_set (s, 0, 0);
      اگر (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->عدد] = s;

      first_state = this->next;
      آزاد (this);
    }
  first_state = پوچ;
  last_state = پوچ;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

تهی
generate_states (تهی)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    برای (rule_عدد r = 0; r < nrules && rules[r].lhs->نماد == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  برای (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      اگر (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->عدد,
                 symbols[s->accessing_نماد]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->اقلام, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_شمار (shift_نماد), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
