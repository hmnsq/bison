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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "state.h"
#അന്തര്ഭാവ "symtab.h"

തരനിർവചനം ഘടന state_list
{
  ഘടന state_list *next;
  state *state;
} state_list;

സ്ഥിതം state_list *first_state = ശൂന്യ;
സ്ഥിതം state_list *last_state = ശൂന്യ;

/* Print CORE for debugging. */
സ്ഥിതം ശൂന്യം
core_print (size_t core_size, item_index *core, FILE *out)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], ശൂന്യ, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

സ്ഥിതം state *
state_list_append (ചിഹ്നം_സംഖ്യ sym, size_t core_size, item_index *core)
{
  state_list *കണ്ണി = xmalloc (വലുപ്പം *കണ്ണി);
  state *res = state_new (sym, core_size, core);

  എങ്കിൽ (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  കണ്ണി->next = ശൂന്യ;
  കണ്ണി->state = res;

  എങ്കിൽ (!first_state)
    first_state = കണ്ണി;
  എങ്കിൽ (last_state)
    last_state->next = കണ്ണി;
  last_state = കണ്ണി;

  തിരികെ res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_ചിഹ്നം;

സ്ഥിതം rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
സ്ഥിതം state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
സ്ഥിതം item_index **kernel_base;
സ്ഥിതം പൂർണ്ണസംഖ്യ *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
സ്ഥിതം item_index *kernel_ഇനങ്ങൾ;


സ്ഥിതം ശൂന്യം
allocate_itemsets (ശൂന്യം)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t എണ്ണം = 0;
  size_t *ചിഹ്നം_എണ്ണം = xcalloc (nsyms + nuseless_nonterminals,
                                  വലുപ്പം *ചിഹ്നം_എണ്ണം);

  വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; ++r)
    വേണ്ടി (item_സംഖ്യ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*rhsp);
        എണ്ണം += 1;
        ചിഹ്നം_എണ്ണം[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, വലുപ്പം *kernel_base);
  kernel_ഇനങ്ങൾ = xnmalloc (എണ്ണം, വലുപ്പം *kernel_ഇനങ്ങൾ);

  എണ്ണം = 0;
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_ഇനങ്ങൾ + എണ്ണം;
      എണ്ണം += ചിഹ്നം_എണ്ണം[i];
    }

  സ്വതന്ത്രം (ചിഹ്നം_എണ്ണം);
  kernel_size = xnmalloc (nsyms, വലുപ്പം *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
സ്ഥിതം ശൂന്യം
kernel_print (FILE *out)
{
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < nsyms; ++i)
    എങ്കിൽ (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
സ്ഥിതം ശൂന്യം
kernel_check (ശൂന്യം)
{
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < nsyms - 1; ++i)
    പ്രതിജ്ഞാ (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

സ്ഥിതം ശൂന്യം
allocate_storage (ശൂന്യം)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, വലുപ്പം *shiftset);
  redset = xnmalloc (nrules, വലുപ്പം *redset);
  state_hash_new ();
  shift_ചിഹ്നം = bitset_create (nsyms, BITSET_FIXED);
}


സ്ഥിതം ശൂന്യം
free_storage (ശൂന്യം)
{
  bitset_free (shift_ചിഹ്നം);
  സ്വതന്ത്രം (redset);
  സ്വതന്ത്രം (shiftset);
  സ്വതന്ത്രം (kernel_base);
  സ്വതന്ത്രം (kernel_size);
  സ്വതന്ത്രം (kernel_ഇനങ്ങൾ);
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

സ്ഥിതം ശൂന്യം
new_itemsets (state *s)
{
  എങ്കിൽ (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->സംഖ്യ);

  memset (kernel_size, 0, nsyms * വലുപ്പം *kernel_size);

  bitset_zero (shift_ചിഹ്നം);

  എങ്കിൽ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  വേണ്ടി (size_t i = 0; i < nitemset; ++i)
    എങ്കിൽ (item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (ritem[itemset[i]]))
      {
        എങ്കിൽ (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], ശൂന്യ, stderr);
            fputc ('\n', stderr);
          }
        ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (ritem[itemset[i]]);
        bitset_set (shift_ചിഹ്നം, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  എങ്കിൽ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->സംഖ്യ);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

സ്ഥിതം state *
get_state (ചിഹ്നം_സംഖ്യ sym, size_t core_size, item_index *core)
{
  എങ്കിൽ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  എങ്കിൽ (!s)
    s = state_list_append (sym, core_size, core);

  എങ്കിൽ (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->സംഖ്യ);

  തിരികെ s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
append_states (state *s)
{
  എങ്കിൽ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->സംഖ്യ);

  bitset_iterator iter;
  ചിഹ്നം_സംഖ്യ sym;
  പൂർണ്ണസംഖ്യ i = 0;
  BITSET_FOR_EACH (iter, shift_ചിഹ്നം, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  എങ്കിൽ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->സംഖ്യ);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
save_reductions (state *s)
{
  പൂർണ്ണസംഖ്യ എണ്ണം = 0;

  /* Find and count the active items that represent ends of rules. */
  വേണ്ടി (size_t i = 0; i < nitemset; ++i)
    {
      item_സംഖ്യ item = ritem[itemset[i]];
      എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (item))
        {
          rule_സംഖ്യ r = item_സംഖ്യ_as_rule_സംഖ്യ (item);
          redset[എണ്ണം++] = &rules[r];
          എങ്കിൽ (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  എങ്കിൽ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->സംഖ്യ);
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; ++i)
        {
          rule_print (redset[i], ശൂന്യ, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, എണ്ണം, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

സ്ഥിതം ശൂന്യം
set_states (ശൂന്യം)
{
  states = xcalloc (nstates, വലുപ്പം *states);

  ആകുമ്പോൾ (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      എങ്കിൽ (!s->transitions)
        state_transitions_set (s, 0, 0);
      എങ്കിൽ (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->സംഖ്യ] = s;

      first_state = this->next;
      സ്വതന്ത്രം (this);
    }
  first_state = ശൂന്യ;
  last_state = ശൂന്യ;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

ശൂന്യം
generate_states (ശൂന്യം)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules && rules[r].lhs->ചിഹ്നം == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  വേണ്ടി (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      എങ്കിൽ (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->സംഖ്യ,
                 symbols[s->accessing_ചിഹ്നം]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->ഇനങ്ങൾ, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_എണ്ണം (shift_ചിഹ്നം), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
