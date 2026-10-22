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
  بنية state_list *ايندڙ;
  state *state;
} state_list;

ساكن state_list *پهريون_state = عدم;
ساكن state_list *آخري_state = عدم;

/* Print CORE for debugging. */
ساكن فراغ
core_print (size_t core_سائيز, شي_اشاريو *core, FILE *ٻاهريون)
{
  لكل (صحيح i = 0; i < core_سائيز; ++i)
    {
      شي_print (ritem + core[i], عدم, ٻاهريون);
      fputc ('\n', ٻاهريون);
    }
}

/*-----------------------------------------------------------------.
| A state was just discovered by transitioning on SYM from another |
| state.  Queue this state for later examination, in order to find |
| its outgoing transitions.  Return it.                            |
`-----------------------------------------------------------------*/

ساكن state *
state_list_append (نشان_عدد sym, size_t core_سائيز, شي_اشاريو *core)
{
  state_list *ڳنڍ = xmalloc (حجم *ڳنڍ);
  state *res = state_new (sym, core_سائيز, core);

  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "state_list_append (state = %d, symbol = %d (%s))\n",
             nstates, sym, symbols[sym]->ٽيگ);

  ڳنڍ->ايندڙ = عدم;
  ڳنڍ->state = res;

  إذا (!پهريون_state)
    پهريون_state = ڳنڍ;
  إذا (آخري_state)
    آخري_state->ايندڙ = ڳنڍ;
  آخري_state = ڳنڍ;

  أرجع res;
}

/* Symbols that can be "shifted" (including nonterminals) from the
   current state.  */
bitset سرڪايو_نشان;

ساكن قاعدو **redset;
/* For the current state, the list of pointers to states that can be
   reached via a shift/goto.  Could be indexed by the reaching symbol,
   but labels of incoming transitions can be recovered by the state
   itself.  */
ساكن state **shiftset;


/* KERNEL_BASE[symbol-number] -> list of item indices (offsets inside
   RITEM) of length KERNEL_SIZE[symbol-number]. */
ساكن شي_اشاريو **kernel_بنياد;
ساكن صحيح *kernel_سائيز;

/* A single dimension array that serves as storage for
   KERNEL_BASE.  */
ساكن شي_اشاريو *kernel_شيون;


ساكن فراغ
allocate_itemsets (فراغ)
{
  /* Count the number of occurrences of all the symbols in RITEMS.
     Note that useless productions (hence useless nonterminals) are
     browsed too, hence we need to allocate room for _all_ the
     symbols.  */
  size_t ڳڻپ = 0;
  size_t *نشان_ڳڻپ = xcalloc (nsyms + nuseless_nonterminals,
                                  حجم *نشان_ڳڻپ);

  لكل (قاعدو_عدد r = 0; r < nrules; ++r)
    لكل (شي_عدد *rhsp = قاعدا[r].rhs; 0 <= *rhsp; ++rhsp)
      {
        نشان_عدد sym = شي_عدد_as_نشان_عدد (*rhsp);
        ڳڻپ += 1;
        نشان_ڳڻپ[sym] += 1;
      }

  /* See comments before new_itemsets.  All the vectors of items
     live inside KERNEL_ITEMS.  The number of active items after
     some symbol S cannot be more than the number of times that S
     appears as an item, which is SYMBOL_COUNT[S].
     We allocate that much space for each symbol.  */

  kernel_بنياد = xnmalloc (nsyms, حجم *kernel_بنياد);
  kernel_شيون = xnmalloc (ڳڻپ, حجم *kernel_شيون);

  ڳڻپ = 0;
  لكل (نشان_عدد i = 0; i < nsyms; i++)
    {
      kernel_بنياد[i] = kernel_شيون + ڳڻپ;
      ڳڻپ += نشان_ڳڻپ[i];
    }

  حرر (نشان_ڳڻپ);
  kernel_سائيز = xnmalloc (nsyms, حجم *kernel_سائيز);
}

/* Print the current kernel (in KERNEL_BASE). */
ساكن فراغ
kernel_print (FILE *ٻاهريون)
{
  لكل (نشان_عدد i = 0; i < nsyms; ++i)
    إذا (kernel_سائيز[i])
      {
        fprintf (ٻاهريون, "kernel[%s] =\n", symbols[i]->ٽيگ);
        core_print (kernel_سائيز[i], kernel_بنياد[i], ٻاهريون);
      }
}

/* Make sure the kernel is in sane state. */
ساكن فراغ
kernel_check (فراغ)
{
  لكل (نشان_عدد i = 0; i < nsyms - 1; ++i)
    أكد (kernel_بنياد[i] + kernel_سائيز[i] <= kernel_بنياد[i + 1]);
}

ساكن فراغ
allocate_storage (فراغ)
{
  allocate_itemsets ();

  shiftset = xnmalloc (nsyms, حجم *shiftset);
  redset = xnmalloc (nrules, حجم *redset);
  state_hash_new ();
  سرڪايو_نشان = bitset_create (nsyms, BITSET_FIXED);
}


ساكن فراغ
free_storage (فراغ)
{
  bitset_free (سرڪايو_نشان);
  حرر (redset);
  حرر (shiftset);
  حرر (kernel_بنياد);
  حرر (kernel_سائيز);
  حرر (kernel_شيون);
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
    fprintf (stderr, "new_itemsets: begin: state = %d\n", s->عدد);

  memset (kernel_سائيز, 0, nsyms * حجم *kernel_سائيز);

  bitset_zero (سرڪايو_نشان);

  إذا (trace_flag & trace_automaton)
    {
      fprintf (stderr, "initial kernel:\n");
      kernel_print (stderr);
    }

  لكل (size_t i = 0; i < nitemset; ++i)
    إذا (شي_عدد_is_نشان_عدد (ritem[itemset[i]]))
      {
        إذا (trace_flag & trace_automaton)
          {
            fputs ("working on: ", stderr);
            شي_print (ritem + itemset[i], عدم, stderr);
            fputc ('\n', stderr);
          }
        نشان_عدد sym = شي_عدد_as_نشان_عدد (ritem[itemset[i]]);
        bitset_سيٽ (سرڪايو_نشان, sym);
        kernel_بنياد[sym][kernel_سائيز[sym]] = itemset[i] + 1;
        kernel_سائيز[sym]++;
      }

  إذا (trace_flag & trace_automaton)
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

ساكن state *
get_state (نشان_عدد sym, size_t core_سائيز, شي_اشاريو *core)
{
  إذا (trace_flag & trace_automaton)
    {
      fprintf (stderr, "Entering get_state, symbol = %d (%s), core:\n",
               sym, symbols[sym]->ٽيگ);
      core_print (core_سائيز, core, stderr);
      fputc ('\n', stderr);
    }

  state *s = state_hash_lookup (core_سائيز, core);
  إذا (!s)
    s = state_list_append (sym, core_سائيز, core);

  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "Exiting get_state => %d\n", s->عدد);

  أرجع s;
}

/*---------------------------------------------------------------.
| Use the information computed by new_itemsets to find the state |
| numbers reached by each shift transition from S.               |
|                                                                |
| SHIFTSET is set up as a vector of those states.                |
`---------------------------------------------------------------*/

ساكن فراغ
append_حالتون (state *s)
{
  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: begin: state = %d\n", s->عدد);

  bitset_iterator iter;
  نشان_عدد sym;
  صحيح i = 0;
  BITSET_FOR_EACH (iter, سرڪايو_نشان, sym, 0)
    {
      shiftset[i] = get_state (sym, kernel_سائيز[sym], kernel_بنياد[sym]);
      ++i;
    }

  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "append_states: end: state = %d\n", s->عدد);
}


/*----------------------------------------------------------------.
| Find which rules can be used for reduction transitions from the |
| current state and make a reductions structure for the state to  |
| record their rule numbers.                                      |
`----------------------------------------------------------------*/

ساكن فراغ
save_reductions (state *s)
{
  صحيح ڳڻپ = 0;

  /* Find and count the active items that represent ends of rules. */
  لكل (size_t i = 0; i < nitemset; ++i)
    {
      شي_عدد شي = ritem[itemset[i]];
      إذا (شي_عدد_is_قاعدو_عدد (شي))
        {
          قاعدو_عدد r = شي_عدد_as_قاعدو_عدد (شي);
          redset[ڳڻپ++] = &قاعدا[r];
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
      fprintf (stderr, "reduction[%d] = {\n", s->عدد);
      لكل (صحيح i = 0; i < ڳڻپ; ++i)
        {
          قاعدو_print (redset[i], عدم, stderr);
          fputc ('\n', stderr);
        }
      fputs ("}\n", stderr);
    }

  /* Make a reductions structure and copy the data into it.  */
  state_reductions_سيٽ (s, ڳڻپ, redset);
}


/*---------------.
| Build STATES.  |
`---------------*/

ساكن فراغ
سيٽ_حالتون (فراغ)
{
  حالتون = xcalloc (nstates, حجم *حالتون);

  طالما (پهريون_state)
    {
      state_list *this = پهريون_state;

      /* Pessimization, but simplification of the code: make sure all
         the states have valid transitions and reductions members,
         even if reduced to 0.  It is too soon for errs, which are
         computed later, but set_conflicts.  */
      state *s = this->state;
      إذا (!s->transitions)
        state_transitions_سيٽ (s, 0, 0);
      إذا (!s->reductions)
        state_reductions_سيٽ (s, 0, 0);

      حالتون[s->عدد] = s;

      پهريون_state = this->ايندڙ;
      حرر (this);
    }
  پهريون_state = عدم;
  آخري_state = عدم;
}


/*-------------------------------------------------------------------.
| Compute the LR(0) parser states (see state.h for details) from the |
| grammar.                                                           |
`-------------------------------------------------------------------*/

فراغ
generate_حالتون (فراغ)
{
  allocate_storage ();
  گهيرو_new (nritems);

  /* Create the initial state, whose accessing symbol (by convention)
     is 0, aka $end.  */
  {
    /* The items of its core: beginning of all the rules of $accept.  */
    kernel_سائيز[0] = 0;
    لكل (قاعدو_عدد r = 0; r < nrules && قاعدا[r].lhs->نشان == acceptsymbol; ++r)
      kernel_بنياد[0][kernel_سائيز[0]++] = قاعدا[r].rhs - ritem;
    state_list_append (0, kernel_سائيز[0], kernel_بنياد[0]);
  }

  /* States are queued when they are created; process them all.  */
  لكل (state_list *list = پهريون_state; list; list = list->ايندڙ)
    {
      state *s = list->state;
      إذا (trace_flag & trace_automaton)
        fprintf (stderr, "Processing state %d (reached by %s)\n",
                 s->عدد,
                 symbols[s->accessing_نشان]->ٽيگ);
      /* Set up itemset for the transitions out of this state.  itemset gets a
         vector of all the items that could be accepted next.  */
      گهيرو (s->شيون, s->nitems);
      /* Record the reductions allowed out of this state.  */
      save_reductions (s);
      /* Find the itemsets of the states that shifts/gotos can reach.  */
      new_itemsets (s);
      /* Find or create the core structures for those states.  */
      append_حالتون (s);

      /* Create the shifts structures for the shifts to those states,
         now that the state numbers transitioning to are known.  */
      state_transitions_سيٽ (s, bitset_ڳڻپ (سرڪايو_نشان), shiftset);
    }

  /* discard various storage */
  free_storage ();

  /* Set up STATES. */
  سيٽ_حالتون ();
}
