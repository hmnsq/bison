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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "state.h"

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <hash.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

স্থির transitions *
transitions_new (পূর্ণসংখ্যা অঙ্ক, state **dst)
{
  size_t states_আকার = অঙ্ক * আকার *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_আকার);
  res->অঙ্ক = অঙ্ক;
  memcpy (res->states, dst, states_আকার);
  ফেরত res;
}


state *
transitions_to (state *s, প্রতীক_সংখ্যা sym)
{
  transitions *trans = s->transitions;
  জন্য (পূর্ণসংখ্যা i = 0; i < trans->অঙ্ক; ++i)
    যদি (TRANSITION_প্রতীক (trans, i) == sym)
      ফেরত trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (পূর্ণসংখ্যা অঙ্ক, প্রতীক **tokens)
{
  size_t symbols_আকার = অঙ্ক * আকার *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_আকার);
  res->অঙ্ক = অঙ্ক;
  যদি (tokens)
    memcpy (res->symbols, tokens, symbols_আকার);
  ফেরত res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

স্থির reductions *
reductions_new (পূর্ণসংখ্যা অঙ্ক, rule **reds)
{
  size_t rules_আকার = অঙ্ক * আকার *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_আকার);
  res->অঙ্ক = অঙ্ক;
  res->lookaheads = ফাঁকা;
  memcpy (res->rules, reds, rules_আকার);
  ফেরত res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_সংখ্যা nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = ফাঁকা;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (প্রতীক_সংখ্যা accessing_প্রতীক,
           size_t nitems, item_সূচক *core)
{
  aver (nstates < STATE_সংখ্যা_MAXIMUM);

  size_t বস্তুসমূহ_আকার = nitems * আকার *core;
  state *res = xmalloc (offsetof (state, বস্তুসমূহ) + বস্তুসমূহ_আকার);
  res->সংখ্যা = nstates++;
  res->accessing_প্রতীক = accessing_প্রতীক;
  res->transitions = ফাঁকা;
  res->reductions = ফাঁকা;
  res->errs = ফাঁকা;
  res->state_list = ফাঁকা;
  res->consistent = false;
  res->solved_conflicts = ফাঁকা;
  res->solved_conflicts_xml = ফাঁকা;

  res->nitems = nitems;
  memcpy (res->বস্তুসমূহ, core, বস্তুসমূহ_আকার);

  state_hash_ঢোকাও (res);

  ফেরত res;
}

state *
state_new_isocore (state ধ্রুবক *s)
{
  aver (nstates < STATE_সংখ্যা_MAXIMUM);

  size_t বস্তুসমূহ_আকার = s->nitems * আকার *s->বস্তুসমূহ;
  state *res = xmalloc (offsetof (state, বস্তুসমূহ) + বস্তুসমূহ_আকার);
  res->সংখ্যা = nstates++;
  res->accessing_প্রতীক = s->accessing_প্রতীক;
  res->transitions =
    transitions_new (s->transitions->অঙ্ক, s->transitions->states);
  res->reductions = reductions_new (s->reductions->অঙ্ক, s->reductions->rules);
  res->errs = ফাঁকা;
  res->state_list = ফাঁকা;
  res->consistent = s->consistent;
  res->solved_conflicts = ফাঁকা;
  res->solved_conflicts_xml = ফাঁকা;

  res->nitems = s->nitems;
  memcpy (res->বস্তুসমূহ, s->বস্তুসমূহ, বস্তুসমূহ_আকার);

  ফেরত res;
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
state_transitions_print (ধ্রুবক state *s, FILE *out)
{
  ধ্রুবক transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->সংখ্যা, trans->অঙ্ক);
  জন্য (পূর্ণসংখ্যা i = 0; i < trans->অঙ্ক; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->সংখ্যা,
             symbols[s->transitions->states[i]->accessing_প্রতীক]->tag,
             s->transitions->states[i]->সংখ্যা);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

শূন্য
state_transitions_set (state *s, পূর্ণসংখ্যা অঙ্ক, state **dst)
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
state_reductions_set (state *s, পূর্ণসংখ্যা অঙ্ক, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (অঙ্ক, reds);
}


পূর্ণসংখ্যা
state_reduction_find (state ধ্রুবক *s, rule ধ্রুবক *r)
{
  reductions *reds = s->reductions;
  জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
    যদি (reds->rules[i] == r)
      ফেরত i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

শূন্য
state_errs_set (state *s, পূর্ণসংখ্যা অঙ্ক, প্রতীক **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (অঙ্ক, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

শূন্য
state_rule_lookaheads_print (state ধ্রুবক *s, rule ধ্রুবক *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  পূর্ণসংখ্যা red = state_reduction_find (s, r);

  /* Print them if there are.  */
  যদি (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      পূর্ণসংখ্যা k;
      অক্ষর ধ্রুবক *sep = "";
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
state_rule_lookaheads_print_xml (state ধ্রুবক *s, rule ধ্রুবক *r,
                                       FILE *out, পূর্ণসংখ্যা level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  পূর্ণসংখ্যা red = state_reduction_find (s, r);

  /* Print them if there are.  */
  যদি (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      পূর্ণসংখ্যা k;
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

স্থির কাঠামো hash_table *state_table = ফাঁকা;

/* Two states are equal if they have the same core items.  */
স্থির অন্তঃরেখ bool
state_compare (state ধ্রুবক *s1, state ধ্রুবক *s2)
{
  যদি (s1->nitems != s2->nitems)
    ফেরত false;

  জন্য (size_t i = 0; i < s1->nitems; ++i)
    যদি (s1->বস্তুসমূহ[i] != s2->বস্তুসমূহ[i])
      ফেরত false;

  ফেরত true;
}

স্থির bool
state_comparator (শূন্য ধ্রুবক *s1, শূন্য ধ্রুবক *s2)
{
  ফেরত state_compare (s1, s2);
}

স্থির অন্তঃরেখ size_t
state_hash (state ধ্রুবক *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t চাবি = 0;
  জন্য (size_t i = 0; i < s->nitems; ++i)
    চাবি += s->বস্তুসমূহ[i];
  ফেরত চাবি % tablesize;
}

স্থির size_t
state_hasher (শূন্য ধ্রুবক *s, size_t tablesize)
{
  ফেরত state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

শূন্য
state_hash_new (শূন্য)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  ফাঁকা,
                                  state_hasher,
                                  state_comparator,
                                  ফাঁকা);
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
state_hash_lookup (size_t nitems, ধ্রুবক item_সূচক *core)
{
  size_t বস্তুসমূহ_আকার = nitems * আকার *core;
  state *probe = xmalloc (offsetof (state, বস্তুসমূহ) + বস্তুসমূহ_আকার);
  probe->nitems = nitems;
  memcpy (probe->বস্তুসমূহ, core, বস্তুসমূহ_আকার);
  state *entry = hash_lookup (state_table, probe);
  মুক্ত (probe);
  ফেরত entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

স্থির শূন্য
state_record_reachable_states (state *s, bitset reachable)
{
  যদি (bitset_test (reachable, s->সংখ্যা))
    ফেরত;
  bitset_set (reachable, s->সংখ্যা);
  জন্য (পূর্ণসংখ্যা i = 0; i < s->transitions->অঙ্ক; ++i)
    যদি (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

শূন্য
state_remove_unreachable_states (state_সংখ্যা old_to_new[])
{
  state_সংখ্যা nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
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
state **states = ফাঁকা;


/*----------------------.
| Free all the states.  |
`----------------------*/

শূন্য
states_free (শূন্য)
{
  closure_free ();
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    state_free (states[i]);
  মুক্ত (states);
}
