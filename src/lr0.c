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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "state.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା state_list
{
  ସଂରଚନା state_list *next;
  state *state;
} state_list;

ସ୍ଥିର state_list *first_state = ଖାଲି;
ସ୍ଥିର state_list *last_state = ଖାଲି;

/* Print CORE for debugging. */
ସ୍ଥିର ଶୂନ୍ୟ
core_print (size_t core_size, item_index *core, FILE *out)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < core_size; ++i)
    {
      item_print (ritem + core[i], ଖାଲି, out);
      fputc ('\n', out);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

ସ୍ଥିର state *
state_list_append (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, size_t core_size, item_index *core)
{
  state_list *ଗଣ୍ଠି = xmalloc (ଆକାର *ଗଣ୍ଠି);
  state *res = state_new (sym, core_size, core);

  ଯଦି (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->tag);

  ଗଣ୍ଠି->next = ଖାଲି;
  ଗଣ୍ଠି->state = res;

  ଯଦି (!first_state)
    first_state = ଗଣ୍ଠି;
  ଯଦି (last_state)
    last_state->next = ଗଣ୍ଠି;
  last_state = ଗଣ୍ଠି;

  ଫେରନ୍ତୁ res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset shift_ପ୍ରତୀକ;

ସ୍ଥିର rule **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
ସ୍ଥିର state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
ସ୍ଥିର item_index **kernel_base;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *kernel_size;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
ସ୍ଥିର item_index *kernel_ବସ୍ତୁଗୁଡ଼ିକ;


ସ୍ଥିର ଶୂନ୍ୟ
allocate_itemsets (ଶୂନ୍ୟ)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t ଗଣନା_2 = 0;
  size_t *ପ୍ରତୀକ_ଗଣନା = xcalloc (nsyms + nuseless_nonterminals,
                                  ଆକାର *ପ୍ରତୀକ_ଗଣନା);

  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
    ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*rhsp);
        ଗଣନା_2 += 1;
        ପ୍ରତୀକ_ଗଣନା[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_base = xnmalloc (nsyms, ଆକାର *kernel_base);
  kernel_ବସ୍ତୁଗୁଡ଼ିକ = xnmalloc (ଗଣନା_2, ଆକାର *kernel_ବସ୍ତୁଗୁଡ଼ିକ);

  ଗଣନା_2 = 0;
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < nsyms; i++)
    {
      kernel_base[i] = kernel_ବସ୍ତୁଗୁଡ଼ିକ + ଗଣନା_2;
      ଗଣନା_2 += ପ୍ରତୀକ_ଗଣନା[i];
    }

  ମୁକ୍ତ (ପ୍ରତୀକ_ଗଣନା);
  kernel_size = xnmalloc (nsyms, ଆକାର *kernel_size);
}

/* Print the current kernel (in KERNEL_BASE). */
ସ୍ଥିର ଶୂନ୍ୟ
kernel_print (FILE *out)
{
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < nsyms; ++i)
    ଯଦି (kernel_size[i])
      {
        fprintf (out, "kernel[%s] =\n", symbols[i]->tag);
        core_print (kernel_size[i], kernel_base[i], out);
      }
}

/* Make sure the kernel is in sane state. */
ସ୍ଥିର ଶୂନ୍ୟ
kernel_check (ଶୂନ୍ୟ)
{
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < nsyms - 1; ++i)
    ପ୍ରତିଜ୍ଞା (kernel_base[i] + kernel_size[i] <= kernel_base[i + 1]);
}

ସ୍ଥିର ଶୂନ୍ୟ
allocate_storage (ଶୂନ୍ୟ)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, ଆକାର *shiftset);
  redset = xnmalloc (nrules, ଆକାର *redset);
  state_hash_new ();
  shift_ପ୍ରତୀକ = bitset_create (nsyms, BITSET_FIXED);
}


ସ୍ଥିର ଶୂନ୍ୟ
free_storage (ଶୂନ୍ୟ)
{
  bitset_free (shift_ପ୍ରତୀକ);
  ମୁକ୍ତ (redset);
  ମୁକ୍ତ (shiftset);
  ମୁକ୍ତ (kernel_base);
  ମୁକ୍ତ (kernel_size);
  ମୁକ୍ତ (kernel_ବସ୍ତୁଗୁଡ଼ିକ);
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

ସ୍ଥିର ଶୂନ୍ୟ
new_itemsets (state *s)
{
  ଯଦି (trace_flag & trace_automaton)
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->ସଂଖ୍ୟା);

  memset (kernel_size, 0, nsyms * ଆକାର *kernel_size);

  bitset_zero (shift_ପ୍ରତୀକ);

  ଯଦି (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  ପାଇଁ (size_t i = 0; i < nitemset; ++i)
    ଯଦି (item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (ritem[itemset[i]]))
      {
        ଯଦି (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            item_print (ritem + itemset[i], ଖାଲି, stderr);
            fputc ('\n', stderr);
          }
        ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (ritem[itemset[i]]);
        bitset_set (shift_ପ୍ରତୀକ, sym);
        kernel_base[sym][kernel_size[sym]] = itemset[i] + 1;
        kernel_size[sym]++;
      }

  ଯଦି (trace_flag & trace_automaton)
    {
      fprintf (stderr, "final kernel:\n");
      kernel_print (stderr);
      fprintf (stderr, "new_itemsets: end: state = %d\n\n", s->ସଂଖ୍ୟା);
    }
  kernel_check ();
}



/*--------------------------------------------------------------.
| Find the state we would get to (from the current state) by    |
| shifting SYM.  Create a new state if no equivalent one exists |
| already.  Used by append_states.                              |
`--------------------------------------------------------------*/

ସ୍ଥିର state *
get_state (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, size_t core_size, item_index *core)
{
  ଯଦି (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->tag);
      core_print (core_size, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_size, core);
  ଯଦି (!s)
    s = state_list_append (sym, core_size, core);

  ଯଦି (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->ସଂଖ୍ୟା);

  ଫେରନ୍ତୁ s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
append_states (state *s)
{
  ଯଦି (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->ସଂଖ୍ୟା);

  bitset_iterator iter;
  ପ୍ରତୀକ_ସଂଖ୍ୟା sym;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
  BITSET_FOR_EACH (iter, shift_ପ୍ରତୀକ, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_size[sym], kernel_base[sym]);
      ++i;
    }

  ଯଦି (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->ସଂଖ୍ୟା);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
save_reductions (state *s)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

  /* Find and count the active items that represent ends of rules. */
  ପାଇଁ (size_t i = 0; i < nitemset; ++i)
    {
      item_ସଂଖ୍ୟା item = ritem[itemset[i]];
      ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (item))
        {
          rule_ସଂଖ୍ୟା r = item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (item);
          redset[ଗଣନା_2++] = &rules[r];
          ଯଦି (r == 0)
            {
              /* This is "reduce 0", i.e., accept. */
              aver (!final_state);
              final_state = s;
            }
        }
    }

  ଯଦି (trace_flag & trace_automaton)
    {
      fprintf (stderr, "reduction[%d] = {\n", s->ସଂଖ୍ୟା);
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; ++i)
        {
          rule_print (redset[i], ଖାଲି, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_set (s, ଗଣନା_2, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

ସ୍ଥିର ଶୂନ୍ୟ
set_states (ଶୂନ୍ୟ)
{
  states = xcalloc (nstates, ଆକାର *states);

  ଯେତେବେଳେ (first_state)
    {
      state_list *this = first_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      ଯଦି (!s->transitions)
        state_transitions_set (s, 0, 0);
      ଯଦି (!s->reductions)
        state_reductions_set (s, 0, 0);

      states[s->ସଂଖ୍ୟା] = s;

      first_state = this->next;
      ମୁକ୍ତ (this);
    }
  first_state = ଖାଲି;
  last_state = ଖାଲି;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

ଶୂନ୍ୟ
generate_states (ଶୂନ୍ୟ)
{
  allocate_storage ();
  closure_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_size[0] = 0;
    ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules && rules[r].lhs->ପ୍ରତୀକ == acceptsymbol; ++r)
      kernel_base[0][kernel_size[0]++] = rules[r].rhs - ritem;
    state_list_append (0, kernel_size[0], kernel_base[0]);
  }

  /* States are queued when they are created; process them all.  */
  ପାଇଁ (state_list *list = first_state; list; list = list->next)
    {
      state *s = list->state;
      ଯଦି (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->ସଂଖ୍ୟା,
                 symbols[s->accessing_ପ୍ରତୀକ]->tag);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      closure (s->ବସ୍ତୁଗୁଡ଼ିକ, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_states (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_set (s, bitset_ଗଣନା (shift_ପ୍ରତୀକ), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  set_states ();
}
