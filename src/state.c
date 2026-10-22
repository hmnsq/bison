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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "state.h"

#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ <hash.h>

#অন্তর্ভাৱ "closure.h"
#অন্তর্ভাৱ "complain.h"
#অন্তর্ভাৱ "getargs.h"
#অন্তর্ভাৱ "gram.h"
#অন্তর্ভাৱ "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

স্থির transitions *
transitions_new (পূর্ণাঙ্ক অঙ্ক, state **dst)
{
  size_t states_size = অঙ্ক * পরিমাণ *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->অঙ্ক = অঙ্ক;
  memcpy (res->states, dst, states_size);
  ঘূৰাই res;
}


state *
transitions_to (state *s, প্ৰতীক_সংখ্যা sym)
{
  transitions *trans = s->transitions;
  বাবে (পূর্ণাঙ্ক i = 0; i < trans->অঙ্ক; ++i)
    যদি (TRANSITION_প্ৰতীক (trans, i) == sym)
      ঘূৰাই trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (পূর্ণাঙ্ক অঙ্ক, প্ৰতীক **tokens)
{
  size_t symbols_size = অঙ্ক * পরিমাণ *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->অঙ্ক = অঙ্ক;
  যদি (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  ঘূৰাই res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

স্থির reductions *
reductions_new (পূর্ণাঙ্ক অঙ্ক, rule **reds)
{
  size_t rules_size = অঙ্ক * পরিমাণ *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->অঙ্ক = অঙ্ক;
  res->lookaheads = রিক্ত;
  memcpy (res->rules, reds, rules_size);
  ঘূৰাই res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_সংখ্যা nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = রিক্ত;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (প্ৰতীক_সংখ্যা accessing_প্ৰতীক,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_সংখ্যা_MAXIMUM);

  size_t বস্তুসমূহ_size = nitems * পরিমাণ *core;
  state *res = xmalloc (offsetof (state, বস্তুসমূহ) + বস্তুসমূহ_size);
  res->সংখ্যা = nstates++;
  res->accessing_প্ৰতীক = accessing_প্ৰতীক;
  res->transitions = রিক্ত;
  res->reductions = রিক্ত;
  res->errs = রিক্ত;
  res->state_list = রিক্ত;
  res->consistent = false;
  res->solved_conflicts = রিক্ত;
  res->solved_conflicts_xml = রিক্ত;

  res->nitems = nitems;
  memcpy (res->বস্তুসমূহ, core, বস্তুসমূহ_size);

  state_hash_ঢোকাও (res);

  ঘূৰাই res;
}

state *
state_new_isocore (state ধ্রুৱ *s)
{
  aver (nstates < STATE_সংখ্যা_MAXIMUM);

  size_t বস্তুসমূহ_size = s->nitems * পরিমাণ *s->বস্তুসমূহ;
  state *res = xmalloc (offsetof (state, বস্তুসমূহ) + বস্তুসমূহ_size);
  res->সংখ্যা = nstates++;
  res->accessing_প্ৰতীক = s->accessing_প্ৰতীক;
  res->transitions =
    transitions_new (s->transitions->অঙ্ক, s->transitions->states);
  res->reductions = reductions_new (s->reductions->অঙ্ক, s->reductions->rules);
  res->errs = রিক্ত;
  res->state_list = রিক্ত;
  res->consistent = s->consistent;
  res->solved_conflicts = রিক্ত;
  res->solved_conflicts_xml = রিক্ত;

  res->nitems = s->nitems;
  memcpy (res->বস্তুসমূহ, s->বস্তুসমূহ, বস্তুসমূহ_size);

  ঘূৰাই res;
}


/*---------.
| Free S.  |
`---------*/

স্থির শূন্য
state_free (state *s)
{
  মুক্ত (s->transitions);
  মুক্ত (s->reductions);
  মুক্ত (s->errs);
  মুক্ত (s);
}


শূন্য
state_transitions_print (ধ্রুৱ state *s, FILE *out)
{
  ধ্রুৱ transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->সংখ্যা, trans->অঙ্ক);
  বাবে (পূর্ণাঙ্ক i = 0; i < trans->অঙ্ক; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->সংখ্যা,
             symbols[s->transitions->states[i]->accessing_প্ৰতীক]->tag,
             s->transitions->states[i]->সংখ্যা);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

শূন্য
state_transitions_set (state *s, পূর্ণাঙ্ক অঙ্ক, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (অঙ্ক, dst);
  যদি (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

শূন্য
state_reductions_set (state *s, পূর্ণাঙ্ক অঙ্ক, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (অঙ্ক, reds);
}


পূর্ণাঙ্ক
state_reduction_find (state ধ্রুৱ *s, rule ধ্রুৱ *r)
{
  reductions *reds = s->reductions;
  বাবে (পূর্ণাঙ্ক i = 0; i < reds->অঙ্ক; ++i)
    যদি (reds->rules[i] == r)
      ঘূৰাই i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

শূন্য
state_errs_set (state *s, পূর্ণাঙ্ক অঙ্ক, প্ৰতীক **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (অঙ্ক, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

শূন্য
state_rule_lookaheads_print (state ধ্রুৱ *s, rule ধ্রুৱ *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  পূর্ণাঙ্ক red = state_reduction_find (s, r);

  /* Print them if there are.  */
  যদি (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      পূর্ণাঙ্ক k;
      ৱর্ণ ধ্রুৱ *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

শূন্য
state_rule_lookaheads_print_xml (state ধ্রুৱ *s, rule ধ্রুৱ *r,
                                       FILE *out, পূর্ণাঙ্ক level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  পূর্ণাঙ্ক red = state_reduction_find (s, r);

  /* Print them if there are.  */
  যদি (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      পূর্ণাঙ্ক k;
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

স্থির সংরচনা hash_table *state_table = রিক্ত;

/* Two states are equal if they have the same core items.  */
স্থির অন্তঃস্থ bool
state_compare (state ধ্রুৱ *s1, state ধ্রুৱ *s2)
{
  যদি (s1->nitems != s2->nitems)
    ঘূৰাই false;

  বাবে (size_t i = 0; i < s1->nitems; ++i)
    যদি (s1->বস্তুসমূহ[i] != s2->বস্তুসমূহ[i])
      ঘূৰাই false;

  ঘূৰাই true;
}

স্থির bool
state_comparator (শূন্য ধ্রুৱ *s1, শূন্য ধ্রুৱ *s2)
{
  ঘূৰাই state_compare (s1, s2);
}

স্থির অন্তঃস্থ size_t
state_hash (state ধ্রুৱ *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t চাবি = 0;
  বাবে (size_t i = 0; i < s->nitems; ++i)
    চাবি += s->বস্তুসমূহ[i];
  ঘূৰাই চাবি % tablesize;
}

স্থির size_t
state_hasher (শূন্য ধ্রুৱ *s, size_t tablesize)
{
  ঘূৰাই state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

শূন্য
state_hash_new (শূন্য)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  রিক্ত,
                                  state_hasher,
                                  state_comparator,
                                  রিক্ত);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

শূন্য
state_hash_free (শূন্য)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

শূন্য
state_hash_ঢোকাও (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ধ্রুৱ item_index *core)
{
  size_t বস্তুসমূহ_size = nitems * পরিমাণ *core;
  state *probe = xmalloc (offsetof (state, বস্তুসমূহ) + বস্তুসমূহ_size);
  probe->nitems = nitems;
  memcpy (probe->বস্তুসমূহ, core, বস্তুসমূহ_size);
  state *entry = hash_lookup (state_table, probe);
  মুক্ত (probe);
  ঘূৰাই entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

স্থির শূন্য
state_record_reachable_states (state *s, bitset reachable)
{
  যদি (bitset_test (reachable, s->সংখ্যা))
    ঘূৰাই;
  bitset_set (reachable, s->সংখ্যা);
  বাবে (পূর্ণাঙ্ক i = 0; i < s->transitions->অঙ্ক; ++i)
    যদি (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

শূন্য
state_remove_unreachable_states (state_সংখ্যা old_to_new[])
{
  state_সংখ্যা nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  বাবে (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      যদি (bitset_test (reachable, states[i]->সংখ্যা))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->সংখ্যা = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      অন্যথা
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = রিক্ত;


/*----------------------.
| Free all the states.  |
`----------------------*/

শূন্য
states_free (শূন্য)
{
  closure_free ();
  বাবে (state_সংখ্যা i = 0; i < nstates; ++i)
    state_free (states[i]);
  মুক্ত (states);
}
