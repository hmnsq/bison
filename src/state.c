/* Type definitions for the finite state machine for Bison.

   Copyright (C) 2001-2007, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#ضمن <config.h>
#ضمن "state.h"

#ضمن "system.h"

#ضمن <hash.h>

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

ساكن transitions *
transitions_new (صحيح رەقەم, state **dst)
{
  size_t states_size = رەقەم * حجم *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->رەقەم = رەقەم;
  memcpy (res->states, dst, states_size);
  أرجع res;
}


state *
transitions_to (state *s, بەلگە_سان sym)
{
  transitions *trans = s->transitions;
  لكل (صحيح i = 0; i < trans->رەقەم; ++i)
    إذا (TRANSITION_بەلگە (trans, i) == sym)
      أرجع trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (صحيح رەقەم, بەلگە **tokens)
{
  size_t symbols_size = رەقەم * حجم *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->رەقەم = رەقەم;
  إذا (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  أرجع res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ساكن reductions *
reductions_new (صحيح رەقەم, rule **reds)
{
  size_t rules_size = رەقەم * حجم *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->رەقەم = رەقەم;
  res->lookaheads = عدم;
  memcpy (res->rules, reds, rules_size);
  أرجع res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_سان nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = عدم;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (بەلگە_سان accessing_بەلگە,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_سان_MAXIMUM);

  size_t بۇيۇملار_size = nitems * حجم *core;
  state *res = xmalloc (offsetof (state, بۇيۇملار) + بۇيۇملار_size);
  res->سان = nstates++;
  res->accessing_بەلگە = accessing_بەلگە;
  res->transitions = عدم;
  res->reductions = عدم;
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = false;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = nitems;
  memcpy (res->بۇيۇملار, core, بۇيۇملار_size);

  state_hash_قىستۇرۇش (res);

  أرجع res;
}

state *
state_new_isocore (state ثابت *s)
{
  aver (nstates < STATE_سان_MAXIMUM);

  size_t بۇيۇملار_size = s->nitems * حجم *s->بۇيۇملار;
  state *res = xmalloc (offsetof (state, بۇيۇملار) + بۇيۇملار_size);
  res->سان = nstates++;
  res->accessing_بەلگە = s->accessing_بەلگە;
  res->transitions =
    transitions_new (s->transitions->رەقەم, s->transitions->states);
  res->reductions = reductions_new (s->reductions->رەقەم, s->reductions->rules);
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = s->consistent;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = s->nitems;
  memcpy (res->بۇيۇملار, s->بۇيۇملار, بۇيۇملار_size);

  أرجع res;
}


/*---------.
| Free S.  |
`---------*/

ساكن فراغ
state_free (state *s)
{
  حرر (s->transitions);
  حرر (s->reductions);
  حرر (s->errs);
  حرر (s);
}


فراغ
state_transitions_print (ثابت state *s, FILE *out)
{
  ثابت transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->سان, trans->رەقەم);
  لكل (صحيح i = 0; i < trans->رەقەم; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->سان,
             symbols[s->transitions->states[i]->accessing_بەلگە]->tag,
             s->transitions->states[i]->سان);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

فراغ
state_transitions_set (state *s, صحيح رەقەم, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (رەقەم, dst);
  إذا (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

فراغ
state_reductions_set (state *s, صحيح رەقەم, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (رەقەم, reds);
}


صحيح
state_reduction_find (state ثابت *s, rule ثابت *r)
{
  reductions *reds = s->reductions;
  لكل (صحيح i = 0; i < reds->رەقەم; ++i)
    إذا (reds->rules[i] == r)
      أرجع i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

فراغ
state_errs_set (state *s, صحيح رەقەم, بەلگە **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (رەقەم, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

فراغ
state_rule_lookaheads_print (state ثابت *s, rule ثابت *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_find (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      حرف ثابت *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

فراغ
state_rule_lookaheads_print_xml (state ثابت *s, rule ثابت *r,
                                       FILE *out, صحيح level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_find (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      xml_puts (out, level, "<lookaheads>");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          xml_printf (out, level + 1, "<symbol>%s</symbol>",
                      xml_escape (symbols[k]->tag));
        }
      xml_puts (out, level, "</lookaheads>");
    }
}


/*---------------------.
| A state hash table.  |
`---------------------*/

/* Initial capacity of states hash table.  */
#define HT_INITIAL_CAPACITY 257

ساكن بنية hash_table *state_table = عدم;

/* Two states are equal if they have the same core items.  */
ساكن مضمن bool
state_compare (state ثابت *s1, state ثابت *s2)
{
  إذا (s1->nitems != s2->nitems)
    أرجع false;

  لكل (size_t i = 0; i < s1->nitems; ++i)
    إذا (s1->بۇيۇملار[i] != s2->بۇيۇملار[i])
      أرجع false;

  أرجع true;
}

ساكن bool
state_comparator (فراغ ثابت *s1, فراغ ثابت *s2)
{
  أرجع state_compare (s1, s2);
}

ساكن مضمن size_t
state_hash (state ثابت *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t ئاچقۇچ = 0;
  لكل (size_t i = 0; i < s->nitems; ++i)
    ئاچقۇچ += s->بۇيۇملار[i];
  أرجع ئاچقۇچ % tablesize;
}

ساكن size_t
state_hasher (فراغ ثابت *s, size_t tablesize)
{
  أرجع state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

فراغ
state_hash_new (فراغ)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  عدم,
                                  state_hasher,
                                  state_comparator,
                                  عدم);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

فراغ
state_hash_free (فراغ)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

فراغ
state_hash_قىستۇرۇش (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ثابت item_index *core)
{
  size_t بۇيۇملار_size = nitems * حجم *core;
  state *probe = xmalloc (offsetof (state, بۇيۇملار) + بۇيۇملار_size);
  probe->nitems = nitems;
  memcpy (probe->بۇيۇملار, core, بۇيۇملار_size);
  state *entry = hash_lookup (state_table, probe);
  حرر (probe);
  أرجع entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ساكن فراغ
state_record_reachable_states (state *s, bitset reachable)
{
  إذا (bitset_test (reachable, s->سان))
    أرجع;
  bitset_set (reachable, s->سان);
  لكل (صحيح i = 0; i < s->transitions->رەقەم; ++i)
    إذا (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

فراغ
state_remove_unreachable_states (state_سان old_to_new[])
{
  state_سان nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  لكل (state_سان i = 0; i < nstates; ++i)
    {
      إذا (bitset_test (reachable, states[i]->سان))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->سان = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      وإلا
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = عدم;


/*----------------------.
| Free all the states.  |
`----------------------*/

فراغ
states_free (فراغ)
{
  closure_free ();
  لكل (state_سان i = 0; i < nstates; ++i)
    state_free (states[i]);
  حرر (states);
}
