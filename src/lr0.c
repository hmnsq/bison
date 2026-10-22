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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitset.h>

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "reader.h"
#ضمن "reduce.h"
#ضمن "state.h"
#ضمن "symtab.h"

تعريفنوع بنية state_list
{
  بنية state_list *next;
  state *state;
} state_list;

ساكن state_list *first_state = عدم;
ساكن state_list *last_state = عدم;

/* Print CORE for debugging. */
ساكن فراغ
core_اطبع (size_t core_size, item_index *core, FILE *out)
{
  لكل (صحيح i = 0; i < core_size; ++i)
    {
      item_اطبع (ritem + core[i], عدم, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

ساكن state *
state_list_append (رمز_رقم sym, size_t core_size, item_index *core)
{
  state_list *عقدة = xmalloc (حجم *عقدة);
  state *res = state_new (sym, core_size, core);

  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  عقدة->next = عدم;
  عقدة->state = res;

  إذا (!first_state)
    first_state = عقدة;
  إذا (last_state)
    last_state->next = عقدة;
  last_state = عقدة;

  أرجع res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_رمز;

ساكن rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
ساكن state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
ساكن item_index **kernel_base;
ساكن صحيح *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
ساكن item_index *kernel_عناصر;


ساكن فراغ
allocate_itemsets (فراغ)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t عدد = 0;
  size_t *رمز_عدد = xcalloc (nsyms + nuseless_nonterminals,
                                  حجم *رمز_عدد);

  لكل (rule_رقم r = 0; r < nrules; ++r)
    لكل (item_رقم *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        رمز_رقم sym = item_رقم_as_رمز_رقم (*rhsp);
        عدد += 1;
        رمز_عدد[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, حجم *kernel_base);
  kernel_عناصر = xnmalloc (عدد, حجم *kernel_عناصر);

  عدد = 0;
  لكل (رمز_رقم i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_عناصر + عدد;
      عدد += رمز_عدد[i];
    }

  حرر (رمز_عدد);
  kernel_size = xnmalloc (nsyms, حجم *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
ساكن فراغ
kernel_اطبع (FILE *out)
{
  لكل (رمز_رقم i = 0; i < nsyms; ++i)
    إذا (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_اطبع (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
ساكن فراغ
kernel_check (فراغ)
{
  لكل (رمز_رقم i = 0; i < nsyms - 1; ++i)
    أكد (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

ساكن فراغ
allocate_storage (فراغ)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, حجم *shiftset);
  redset = xnmalloc (nrules, حجم *redset);
  state_hash_new ();
  shift_رمز = bitset_create (nsyms, BITSET_FIXED);
}


ساكن فراغ
free_storage (فراغ)
{
  bitset_free (shift_رمز);
  حرر (redset);
  حرر (shiftset);
  حرر (kernel_base);
  حرر (kernel_size);
  حرر (kernel_عناصر);
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

ساكن فراغ
new_itemsets (state *s)
{
  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->رقم_2);

  memset (kernel_size, 0, nsyms * حجم *kernel_size);

  bitset_zero (shift_رمز);

  إذا (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_اطبع (stderr);
    }

  لكل (size_t i = 0; i < nitemset; ++i)
    إذا (item_رقم_is_رمز_رقم (ritem[itemset[i]]))
      {
        إذا (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_اطبع (ritem + itemset[i], عدم, stderr);
            fputc ('\n', stderr);
          }
        رمز_رقم sym = item_رقم_as_رمز_رقم (ritem[itemset[i]]);
        bitset_set (shift_رمز, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  إذا (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_اطبع (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->رقم_2);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

ساكن state *
get_state (رمز_رقم sym, size_t core_size, item_index *core)
{
  إذا (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_اطبع (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  إذا (!s)
    s = state_list_append (sym, core_size, core);

  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->رقم_2);

  أرجع s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

ساكن فراغ
append_states (state *s)
{
  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->رقم_2);

  bitset_iterator iter;
  رمز_رقم sym;
  صحيح i = 0;
  BITSET_FOR_EACH (iter, shift_رمز, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->رقم_2);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

ساكن فراغ
save_reductions (state *s)
{
  صحيح عدد = 0;

  /* Find and count the active items that represent ends of rules. */
  لكل (size_t i = 0; i < nitemset; ++i)
    {
      item_رقم item = ritem[itemset[i]];
      إذا (item_رقم_is_rule_رقم (item))
        {
          rule_رقم r = item_رقم_as_rule_رقم (item);
          redset[عدد++] = &rules[r];
          إذا (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  إذا (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->رقم_2);
      لكل (صحيح i = 0; i < عدد; ++i)
        {
          rule_اطبع (redset[i], عدم, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, عدد, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

ساكن فراغ
set_states (فراغ)
{
  states = xcalloc (nstates, حجم *states);

  طالما (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      إذا (!s->transitions)
        state_transitions_set (s, 0, 0);
      إذا (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->رقم_2] = s;

      first_state = this->next;
      حرر (this);
    }
  first_state = عدم;
  last_state = عدم;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

فراغ
generate_states (فراغ)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    لكل (rule_رقم r = 0; r < nrules && rules[r].lhs->رمز == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  لكل (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      إذا (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->رقم_2,
                 symbols[s->accessing_رمز]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->عناصر, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_عدد (shift_رمز), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
