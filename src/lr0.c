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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <bitset.h>

#समावेश "closure.h"
#समावेश "complain.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "lalr.h"
#समावेश "lr0.h"
#समावेश "reader.h"
#समावेश "reduce.h"
#समावेश "state.h"
#समावेश "symtab.h"

प्रकार काष्ठा state_list
{
  काष्ठा state_list *अग्र;
  state *state;
} state_list;

अटल state_list *first_state = शून्य;
अटल state_list *last_state = शून्य;

/* Print CORE for debugging. */
अटल व्योम
core_print (माप_प्रकार core_आकार, item_सूचकांक *core, खाता *out)
{
  क्रम (पूर्णांक i = 0; i < core_आकार; ++i)
    {
      item_print (ritem + core[i], शून्य, out);
      ख_अक्षर_दो ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

अटल state *
state_list_append (प्रतीक_संख्या sym, माप_प्रकार core_आकार, item_सूचकांक *core)
{
  state_list *गाँठ = xmalloc (माप *गाँठ);
  state *res = state_new (sym, core_आकार, core);

  अगर (trace_flag & trace_automaton)
    ख_लिखो (मानक_त्रुटि, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  गाँठ->अग्र = शून्य;
  गाँठ->state = res;

  अगर (!first_state)
    first_state = गाँठ;
  अगर (last_state)
    last_state->अग्र = गाँठ;
  last_state = गाँठ;

  वापस res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_प्रतीक;

अटल rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
अटल state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
अटल item_सूचकांक **kernel_base;
अटल पूर्णांक *kernel_आकार;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
अटल item_सूचकांक *kernel_वस्तुएँ;


अटल व्योम
allocate_itemsets (व्योम)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  माप_प्रकार गिनती = 0;
  माप_प्रकार *प्रतीक_गिनती = xcalloc (nsyms + nuseless_nonterminals,
                                  माप *प्रतीक_गिनती);

  क्रम (rule_संख्या r = 0; r < nrules; ++r)
    क्रम (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (*rhsp);
        गिनती += 1;
        प्रतीक_गिनती[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, माप *kernel_base);
  kernel_वस्तुएँ = xnmalloc (गिनती, माप *kernel_वस्तुएँ);

  गिनती = 0;
  क्रम (प्रतीक_संख्या i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_वस्तुएँ + गिनती;
      गिनती += प्रतीक_गिनती[i];
    }

  मुक्त (प्रतीक_गिनती);
  kernel_आकार = xnmalloc (nsyms, माप *kernel_आकार);
}

/* Print the current kernel (in KERNEL_BASE). */
अटल व्योम
kernel_print (खाता *out)
{
  क्रम (प्रतीक_संख्या i = 0; i < nsyms; ++i)
    अगर (kernel_आकार[i])
      {
        ख_लिखो (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_आकार[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
अटल व्योम
kernel_check (व्योम)
{
  क्रम (प्रतीक_संख्या i = 0; i < nsyms - 1; ++i)
    निश्चित (kernel_base[i] + kernel_आकार[i] <= kernel_base[i + 1]);
}

अटल व्योम
allocate_storage (व्योम)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, माप *shiftset);
  redset = xnmalloc (nrules, माप *redset);
  state_hash_new ();
  shift_प्रतीक = bitset_create (nsyms, BITSET_FIXED);
}


अटल व्योम
free_storage (व्योम)
{
  bitset_free (shift_प्रतीक);
  मुक्त (redset);
  मुक्त (shiftset);
  मुक्त (kernel_base);
  मुक्त (kernel_आकार);
  मुक्त (kernel_वस्तुएँ);
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

अटल व्योम
new_itemsets (state *s)
{
  अगर (trace_flag & trace_automaton)
    ख_लिखो (मानक_त्रुटि, "new_itemsets: begin: state = %d\n", s->संख्या);

  स_रखो (kernel_आकार, 0, nsyms * माप *kernel_आकार);

  bitset_zero (shift_प्रतीक);

  अगर (trace_flag & trace_automaton)
    {
      ख_लिखो (मानक_त्रुटि, "initial kernel:\n");
      kernel_print (मानक_त्रुटि);
    }

  क्रम (माप_प्रकार i = 0; i < nitemset; ++i)
    अगर (item_संख्या_is_प्रतीक_संख्या (ritem[itemset[i]]))
      {
        अगर (trace_flag & trace_automaton)
          {
            ख_माला_दो ("working on: ", मानक_त्रुटि);
            item_print (ritem + itemset[i], शून्य, मानक_त्रुटि);
            ख_अक्षर_दो ('\n', मानक_त्रुटि);
          }
        प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (ritem[itemset[i]]);
        bitset_set (shift_प्रतीक, sym);
        kernel_base[sym][kernel_आकार[sym]] = itemset[i] + 1;
        kernel_आकार[sym]++;
      }

  अगर (trace_flag & trace_automaton)
    {
      ख_लिखो (मानक_त्रुटि, "final kernel:\n");
      kernel_print (मानक_त्रुटि);
      ख_लिखो (मानक_त्रुटि, "new_itemsets: end: state = %d\n\n", s->संख्या);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

अटल state *
get_state (प्रतीक_संख्या sym, माप_प्रकार core_आकार, item_सूचकांक *core)
{
  अगर (trace_flag & trace_automaton)
    {
      ख_लिखो (मानक_त्रुटि, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_आकार, core, मानक_त्रुटि);
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
    }

  state *s = state_hash_lookup (core_आकार, core);
  अगर (!s)
    s = state_list_append (sym, core_आकार, core);

  अगर (trace_flag & trace_automaton)
    ख_लिखो (मानक_त्रुटि, "Exiting get_state => %d\n", s->संख्या);

  वापस s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

अटल व्योम
append_states (state *s)
{
  अगर (trace_flag & trace_automaton)
    ख_लिखो (मानक_त्रुटि, "append_states: begin: state = %d\n", s->संख्या);

  bitset_iterator iter;
  प्रतीक_संख्या sym;
  पूर्णांक i = 0;
  BITSET_FOR_EACH (iter, shift_प्रतीक, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_आकार[sym], kernel_base[sym]);
      ++i;
    }

  अगर (trace_flag & trace_automaton)
    ख_लिखो (मानक_त्रुटि, "append_states: end: state = %d\n", s->संख्या);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

अटल व्योम
save_reductions (state *s)
{
  पूर्णांक गिनती = 0;

  /* Find and count the active items that represent ends of rules. */
  क्रम (माप_प्रकार i = 0; i < nitemset; ++i)
    {
      item_संख्या item = ritem[itemset[i]];
      अगर (item_संख्या_is_rule_संख्या (item))
        {
          rule_संख्या r = item_संख्या_as_rule_संख्या (item);
          redset[गिनती++] = &rules[r];
          अगर (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  अगर (trace_flag & trace_automaton)
    {
      ख_लिखो (मानक_त्रुटि, "reduction[%d] = {\n", s->संख्या);
      क्रम (पूर्णांक i = 0; i < गिनती; ++i)
        {
          rule_print (redset[i], शून्य, मानक_त्रुटि);
          ख_अक्षर_दो ('\n', मानक_त्रुटि);
        }
      ख_माला_दो ("}\n", मानक_त्रुटि);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, गिनती, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

अटल व्योम
set_states (व्योम)
{
  states = xcalloc (nstates, माप *states);

  जबतक (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      अगर (!s->transitions)
        state_transitions_set (s, 0, 0);
      अगर (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->संख्या] = s;

      first_state = this->अग्र;
      मुक्त (this);
    }
  first_state = शून्य;
  last_state = शून्य;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

व्योम
generate_states (व्योम)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_आकार[0] = 0;
    क्रम (rule_संख्या r = 0; r < nrules && rules[r].lhs->प्रतीक == acceptsymbol; ++r)
      kernel_base[0][kernel_आकार[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_आकार[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  क्रम (state_list *list = first_state; list; list = list->अग्र)
    {
      state *s = list->state;
      अगर (trace_flag & trace_automaton)
        ख_लिखो (मानक_त्रुटि, "Processing state %d (reached by %s)\n",
                 s->संख्या,
                 symbols[s->accessing_प्रतीक]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->वस्तुएँ, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_गिनती (shift_प्रतीक), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
