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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "state.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <hash.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

நிலையான transitions *
transitions_new (முழு இலக்கம், state **dst)
{
  size_t states_size = இலக்கம் * அளவு *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->இலக்கம் = இலக்கம்;
  memcpy (res->states, dst, states_size);
  திரும்பு res;
}


state *
transitions_to (state *s, குறியீடு_எண் sym)
{
  transitions *trans = s->transitions;
  க்கு (முழு i = 0; i < trans->இலக்கம்; ++i)
    எனில் (TRANSITION_குறியீடு (trans, i) == sym)
      திரும்பு trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (முழு இலக்கம், குறியீடு **tokens)
{
  size_t symbols_size = இலக்கம் * அளவு *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->இலக்கம் = இலக்கம்;
  எனில் (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  திரும்பு res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

நிலையான reductions *
reductions_new (முழு இலக்கம், rule **reds)
{
  size_t rules_size = இலக்கம் * அளவு *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->இலக்கம் = இலக்கம்;
  res->lookaheads = வெற்று;
  memcpy (res->rules, reds, rules_size);
  திரும்பு res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_எண் nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = வெற்று;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (குறியீடு_எண் accessing_குறியீடு,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_எண்_MAXIMUM);

  size_t பொருட்கள்_size = nitems * அளவு *core;
  state *res = xmalloc (offsetof (state, பொருட்கள்) + பொருட்கள்_size);
  res->எண் = nstates++;
  res->accessing_குறியீடு = accessing_குறியீடு;
  res->transitions = வெற்று;
  res->reductions = வெற்று;
  res->errs = வெற்று;
  res->state_list = வெற்று;
  res->consistent = false;
  res->solved_conflicts = வெற்று;
  res->solved_conflicts_xml = வெற்று;

  res->nitems = nitems;
  memcpy (res->பொருட்கள், core, பொருட்கள்_size);

  state_hash_செருகு (res);

  திரும்பு res;
}

state *
state_new_isocore (state மாறிலி *s)
{
  aver (nstates < STATE_எண்_MAXIMUM);

  size_t பொருட்கள்_size = s->nitems * அளவு *s->பொருட்கள்;
  state *res = xmalloc (offsetof (state, பொருட்கள்) + பொருட்கள்_size);
  res->எண் = nstates++;
  res->accessing_குறியீடு = s->accessing_குறியீடு;
  res->transitions =
    transitions_new (s->transitions->இலக்கம், s->transitions->states);
  res->reductions = reductions_new (s->reductions->இலக்கம், s->reductions->rules);
  res->errs = வெற்று;
  res->state_list = வெற்று;
  res->consistent = s->consistent;
  res->solved_conflicts = வெற்று;
  res->solved_conflicts_xml = வெற்று;

  res->nitems = s->nitems;
  memcpy (res->பொருட்கள், s->பொருட்கள், பொருட்கள்_size);

  திரும்பு res;
}


/*---------.
| Free S.  |
`---------*/

நிலையான வெறுமை
state_free (state *s)
{
  விடுவி (s->transitions);
  விடுவி (s->reductions);
  விடுவி (s->errs);
  விடுவி (s);
}


வெறுமை
state_transitions_print (மாறிலி state *s, FILE *out)
{
  மாறிலி transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->எண், trans->இலக்கம்);
  க்கு (முழு i = 0; i < trans->இலக்கம்; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->எண்,
             symbols[s->transitions->states[i]->accessing_குறியீடு]->tag,
             s->transitions->states[i]->எண்);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

வெறுமை
state_transitions_set (state *s, முழு இலக்கம், state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (இலக்கம், dst);
  எனில் (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

வெறுமை
state_reductions_set (state *s, முழு இலக்கம், rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (இலக்கம், reds);
}


முழு
state_reduction_find (state மாறிலி *s, rule மாறிலி *r)
{
  reductions *reds = s->reductions;
  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
    எனில் (reds->rules[i] == r)
      திரும்பு i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

வெறுமை
state_errs_set (state *s, முழு இலக்கம், குறியீடு **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (இலக்கம், tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

வெறுமை
state_rule_lookaheads_print (state மாறிலி *s, rule மாறிலி *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  முழு red = state_reduction_find (s, r);

  /* Print them if there are.  */
  எனில் (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      முழு k;
      எழுத்து மாறிலி *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

வெறுமை
state_rule_lookaheads_print_xml (state மாறிலி *s, rule மாறிலி *r,
                                       FILE *out, முழு level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  முழு red = state_reduction_find (s, r);

  /* Print them if there are.  */
  எனில் (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      முழு k;
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

நிலையான கட்டமைப்பு hash_table *state_table = வெற்று;

/* Two states are equal if they have the same core items.  */
நிலையான உள்வரி bool
state_compare (state மாறிலி *s1, state மாறிலி *s2)
{
  எனில் (s1->nitems != s2->nitems)
    திரும்பு false;

  க்கு (size_t i = 0; i < s1->nitems; ++i)
    எனில் (s1->பொருட்கள்[i] != s2->பொருட்கள்[i])
      திரும்பு false;

  திரும்பு true;
}

நிலையான bool
state_comparator (வெறுமை மாறிலி *s1, வெறுமை மாறிலி *s2)
{
  திரும்பு state_compare (s1, s2);
}

நிலையான உள்வரி size_t
state_hash (state மாறிலி *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t சாவி = 0;
  க்கு (size_t i = 0; i < s->nitems; ++i)
    சாவி += s->பொருட்கள்[i];
  திரும்பு சாவி % tablesize;
}

நிலையான size_t
state_hasher (வெறுமை மாறிலி *s, size_t tablesize)
{
  திரும்பு state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

வெறுமை
state_hash_new (வெறுமை)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  வெற்று,
                                  state_hasher,
                                  state_comparator,
                                  வெற்று);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

வெறுமை
state_hash_free (வெறுமை)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

வெறுமை
state_hash_செருகு (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, மாறிலி item_index *core)
{
  size_t பொருட்கள்_size = nitems * அளவு *core;
  state *probe = xmalloc (offsetof (state, பொருட்கள்) + பொருட்கள்_size);
  probe->nitems = nitems;
  memcpy (probe->பொருட்கள், core, பொருட்கள்_size);
  state *entry = hash_lookup (state_table, probe);
  விடுவி (probe);
  திரும்பு entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

நிலையான வெறுமை
state_record_reachable_states (state *s, bitset reachable)
{
  எனில் (bitset_test (reachable, s->எண்))
    திரும்பு;
  bitset_set (reachable, s->எண்);
  க்கு (முழு i = 0; i < s->transitions->இலக்கம்; ++i)
    எனில் (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

வெறுமை
state_remove_unreachable_states (state_எண் old_to_new[])
{
  state_எண் nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      எனில் (bitset_test (reachable, states[i]->எண்))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->எண் = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      இல்லையெனில்
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = வெற்று;


/*----------------------.
| Free all the states.  |
`----------------------*/

வெறுமை
states_free (வெறுமை)
{
  closure_free ();
  க்கு (state_எண் i = 0; i < nstates; ++i)
    state_free (states[i]);
  விடுவி (states);
}
