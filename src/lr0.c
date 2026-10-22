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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "complain.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "symtab.h"

प्रकारनिर्देश संरचना state_list
{
  संरचना state_list *next;
  state *state;
} state_list;

स्थिर state_list *first_state = रिक्त;
स्थिर state_list *last_state = रिक्त;

/* Print CORE for debugging. */
स्थिर शून्य
core_print (size_t core_size, item_index *core, FILE *out)
{
  प्रति (पूर्णाङ्क i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], रिक्त, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

स्थिर state *
state_list_append (चिह्नम्_सङ्ख्या sym, size_t core_size, item_index *core)
{
  state_list *ग्रन्थिः = xmalloc (परिमाण *ग्रन्थिः);
  state *res = state_new (sym, core_size, core);

  जइ (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  ग्रन्थिः->next = रिक्त;
  ग्रन्थिः->state = res;

  जइ (!first_state)
    first_state = ग्रन्थिः;
  जइ (last_state)
    last_state->next = ग्रन्थिः;
  last_state = ग्रन्थिः;

  प्रत्यागम res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_चिह्नम्;

स्थिर rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
स्थिर state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
स्थिर item_index **kernel_base;
स्थिर पूर्णाङ्क *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
स्थिर item_index *kernel_वस्तूनि;


स्थिर शून्य
allocate_itemsets (शून्य)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t गणनम् = 0;
  size_t *चिह्नम्_गणनम् = xcalloc (nsyms + nuseless_nonterminals,
                                  परिमाण *चिह्नम्_गणनम्);

  प्रति (rule_सङ्ख्या r = 0; r < nrules; ++r)
    प्रति (item_सङ्ख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp);
        गणनम् += 1;
        चिह्नम्_गणनम्[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, परिमाण *kernel_base);
  kernel_वस्तूनि = xnmalloc (गणनम्, परिमाण *kernel_वस्तूनि);

  गणनम् = 0;
  प्रति (चिह्नम्_सङ्ख्या i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_वस्तूनि + गणनम्;
      गणनम् += चिह्नम्_गणनम्[i];
    }

  मुक्त (चिह्नम्_गणनम्);
  kernel_size = xnmalloc (nsyms, परिमाण *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
स्थिर शून्य
kernel_print (FILE *out)
{
  प्रति (चिह्नम्_सङ्ख्या i = 0; i < nsyms; ++i)
    जइ (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
स्थिर शून्य
kernel_check (शून्य)
{
  प्रति (चिह्नम्_सङ्ख्या i = 0; i < nsyms - 1; ++i)
    प्रतिज्ञा (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

स्थिर शून्य
allocate_storage (शून्य)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, परिमाण *shiftset);
  redset = xnmalloc (nrules, परिमाण *redset);
  state_hash_new ();
  shift_चिह्नम् = bitset_create (nsyms, BITSET_FIXED);
}


स्थिर शून्य
free_storage (शून्य)
{
  bitset_free (shift_चिह्नम्);
  मुक्त (redset);
  मुक्त (shiftset);
  मुक्त (kernel_base);
  मुक्त (kernel_size);
  मुक्त (kernel_वस्तूनि);
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

स्थिर शून्य
new_itemsets (state *s)
{
  जइ (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->सङ्ख्या);

  memset (kernel_size, 0, nsyms * परिमाण *kernel_size);

  bitset_zero (shift_चिह्नम्);

  जइ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  प्रति (size_t i = 0; i < nitemset; ++i)
    जइ (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (ritem[itemset[i]]))
      {
        जइ (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], रिक्त, stderr);
            fputc ('\n', stderr);
          }
        चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (ritem[itemset[i]]);
        bitset_set (shift_चिह्नम्, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  जइ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->सङ्ख्या);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

स्थिर state *
get_state (चिह्नम्_सङ्ख्या sym, size_t core_size, item_index *core)
{
  जइ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  जइ (!s)
    s = state_list_append (sym, core_size, core);

  जइ (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->सङ्ख्या);

  प्रत्यागम s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

स्थिर शून्य
append_states (state *s)
{
  जइ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->सङ्ख्या);

  bitset_iterator iter;
  चिह्नम्_सङ्ख्या sym;
  पूर्णाङ्क i = 0;
  BITSET_FOR_EACH (iter, shift_चिह्नम्, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  जइ (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->सङ्ख्या);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

स्थिर शून्य
save_reductions (state *s)
{
  पूर्णाङ्क गणनम् = 0;

  /* Find and count the active items that represent ends of rules. */
  प्रति (size_t i = 0; i < nitemset; ++i)
    {
      item_सङ्ख्या item = ritem[itemset[i]];
      जइ (item_सङ्ख्या_is_rule_सङ्ख्या (item))
        {
          rule_सङ्ख्या r = item_सङ्ख्या_as_rule_सङ्ख्या (item);
          redset[गणनम्++] = &rules[r];
          जइ (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  जइ (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->सङ्ख्या);
      प्रति (पूर्णाङ्क i = 0; i < गणनम्; ++i)
        {
          rule_print (redset[i], रिक्त, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, गणनम्, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

स्थिर शून्य
set_states (शून्य)
{
  states = xcalloc (nstates, परिमाण *states);

  जाव (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      जइ (!s->transitions)
        state_transitions_set (s, 0, 0);
      जइ (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->सङ्ख्या] = s;

      first_state = this->next;
      मुक्त (this);
    }
  first_state = रिक्त;
  last_state = रिक्त;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

शून्य
generate_states (शून्य)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    प्रति (rule_सङ्ख्या r = 0; r < nrules && rules[r].lhs->चिह्नम् == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  प्रति (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      जइ (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->सङ्ख्या,
                 symbols[s->accessing_चिह्नम्]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->वस्तूनि, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_गणनम् (shift_चिह्नम्), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
