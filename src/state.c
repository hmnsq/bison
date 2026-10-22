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

#شامل <config.h>
#شامل "state.h"

#شامل "system.h"

#شامل <hash.h>

#شامل "closure.h"
#شامل "complain.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

ایستا transitions *
transitions_new (صحیح رقم, state **dst)
{
  size_t states_size = رقم * اندازه *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->رقم = رقم;
  memcpy (res->states, dst, states_size);
  بازگشت res;
}


state *
transitions_to (state *s, نماد_عدد sym)
{
  transitions *trans = s->transitions;
  برای (صحیح i = 0; i < trans->رقم; ++i)
    اگر (TRANSITION_نماد (trans, i) == sym)
      بازگشت trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (صحیح رقم, نماد **tokens)
{
  size_t symbols_size = رقم * اندازه *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->رقم = رقم;
  اگر (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  بازگشت res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ایستا reductions *
reductions_new (صحیح رقم, rule **reds)
{
  size_t rules_size = رقم * اندازه *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->رقم = رقم;
  res->lookaheads = پوچ;
  memcpy (res->rules, reds, rules_size);
  بازگشت res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_عدد nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = پوچ;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (نماد_عدد accessing_نماد,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_عدد_MAXIMUM);

  size_t اقلام_size = nitems * اندازه *core;
  state *res = xmalloc (offsetof (state, اقلام) + اقلام_size);
  res->عدد = nstates++;
  res->accessing_نماد = accessing_نماد;
  res->transitions = پوچ;
  res->reductions = پوچ;
  res->errs = پوچ;
  res->state_list = پوچ;
  res->consistent = false;
  res->solved_conflicts = پوچ;
  res->solved_conflicts_xml = پوچ;

  res->nitems = nitems;
  memcpy (res->اقلام, core, اقلام_size);

  state_hash_درج (res);

  بازگشت res;
}

state *
state_new_isocore (state ثابت *s)
{
  aver (nstates < STATE_عدد_MAXIMUM);

  size_t اقلام_size = s->nitems * اندازه *s->اقلام;
  state *res = xmalloc (offsetof (state, اقلام) + اقلام_size);
  res->عدد = nstates++;
  res->accessing_نماد = s->accessing_نماد;
  res->transitions =
    transitions_new (s->transitions->رقم, s->transitions->states);
  res->reductions = reductions_new (s->reductions->رقم, s->reductions->rules);
  res->errs = پوچ;
  res->state_list = پوچ;
  res->consistent = s->consistent;
  res->solved_conflicts = پوچ;
  res->solved_conflicts_xml = پوچ;

  res->nitems = s->nitems;
  memcpy (res->اقلام, s->اقلام, اقلام_size);

  بازگشت res;
}


/*---------.
| Free S.  |
`---------*/

ایستا تهی
state_free (state *s)
{
  آزاد (s->transitions);
  آزاد (s->reductions);
  آزاد (s->errs);
  آزاد (s);
}


تهی
state_transitions_print (ثابت state *s, FILE *out)
{
  ثابت transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->عدد, trans->رقم);
  برای (صحیح i = 0; i < trans->رقم; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->عدد,
             symbols[s->transitions->states[i]->accessing_نماد]->tag,
             s->transitions->states[i]->عدد);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

تهی
state_transitions_set (state *s, صحیح رقم, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (رقم, dst);
  اگر (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

تهی
state_reductions_set (state *s, صحیح رقم, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (رقم, reds);
}


صحیح
state_reduction_find (state ثابت *s, rule ثابت *r)
{
  reductions *reds = s->reductions;
  برای (صحیح i = 0; i < reds->رقم; ++i)
    اگر (reds->rules[i] == r)
      بازگشت i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

تهی
state_errs_set (state *s, صحیح رقم, نماد **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (رقم, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

تهی
state_rule_lookaheads_print (state ثابت *s, rule ثابت *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحیح red = state_reduction_find (s, r);

  /* Print them if there are.  */
  اگر (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحیح k;
      نویسه ثابت *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

تهی
state_rule_lookaheads_print_xml (state ثابت *s, rule ثابت *r,
                                       FILE *out, صحیح level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحیح red = state_reduction_find (s, r);

  /* Print them if there are.  */
  اگر (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحیح k;
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

ایستا ساختار hash_table *state_table = پوچ;

/* Two states are equal if they have the same core items.  */
ایستا درونخطی bool
state_compare (state ثابت *s1, state ثابت *s2)
{
  اگر (s1->nitems != s2->nitems)
    بازگشت false;

  برای (size_t i = 0; i < s1->nitems; ++i)
    اگر (s1->اقلام[i] != s2->اقلام[i])
      بازگشت false;

  بازگشت true;
}

ایستا bool
state_comparator (تهی ثابت *s1, تهی ثابت *s2)
{
  بازگشت state_compare (s1, s2);
}

ایستا درونخطی size_t
state_hash (state ثابت *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t کلید = 0;
  برای (size_t i = 0; i < s->nitems; ++i)
    کلید += s->اقلام[i];
  بازگشت کلید % tablesize;
}

ایستا size_t
state_hasher (تهی ثابت *s, size_t tablesize)
{
  بازگشت state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

تهی
state_hash_new (تهی)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  پوچ,
                                  state_hasher,
                                  state_comparator,
                                  پوچ);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

تهی
state_hash_free (تهی)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

تهی
state_hash_درج (state *s)
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
  size_t اقلام_size = nitems * اندازه *core;
  state *probe = xmalloc (offsetof (state, اقلام) + اقلام_size);
  probe->nitems = nitems;
  memcpy (probe->اقلام, core, اقلام_size);
  state *entry = hash_lookup (state_table, probe);
  آزاد (probe);
  بازگشت entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ایستا تهی
state_record_reachable_states (state *s, bitset reachable)
{
  اگر (bitset_test (reachable, s->عدد))
    بازگشت;
  bitset_set (reachable, s->عدد);
  برای (صحیح i = 0; i < s->transitions->رقم; ++i)
    اگر (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

تهی
state_remove_unreachable_states (state_عدد old_to_new[])
{
  state_عدد nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      اگر (bitset_test (reachable, states[i]->عدد))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->عدد = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      وگرنه
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = پوچ;


/*----------------------.
| Free all the states.  |
`----------------------*/

تهی
states_free (تهی)
{
  closure_free ();
  برای (state_عدد i = 0; i < nstates; ++i)
    state_free (states[i]);
  آزاد (states);
}
