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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "state.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <hash.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

ସ୍ଥିର transitions *
transitions_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, state **dst)
{
  size_t states_size = ଅଙ୍କ * ଆକାର *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->ଅଙ୍କ = ଅଙ୍କ;
  memcpy (res->states, dst, states_size);
  ଫେରନ୍ତୁ res;
}


state *
transitions_to (state *s, ପ୍ରତୀକ_ସଂଖ୍ୟା sym)
{
  transitions *trans = s->transitions;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < trans->ଅଙ୍କ; ++i)
    ଯଦି (TRANSITION_ପ୍ରତୀକ (trans, i) == sym)
      ଫେରନ୍ତୁ trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ପ୍ରତୀକ **tokens)
{
  size_t symbols_size = ଅଙ୍କ * ଆକାର *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->ଅଙ୍କ = ଅଙ୍କ;
  ଯଦି (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  ଫେରନ୍ତୁ res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ସ୍ଥିର reductions *
reductions_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, rule **reds)
{
  size_t rules_size = ଅଙ୍କ * ଆକାର *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->ଅଙ୍କ = ଅଙ୍କ;
  res->lookaheads = ଖାଲି;
  memcpy (res->rules, reds, rules_size);
  ଫେରନ୍ତୁ res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_ସଂଖ୍ୟା nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = ଖାଲି;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (ପ୍ରତୀକ_ସଂଖ୍ୟା accessing_ପ୍ରତୀକ,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_ସଂଖ୍ୟା_MAXIMUM);

  size_t ବସ୍ତୁଗୁଡ଼ିକ_size = nitems * ଆକାର *core;
  state *res = xmalloc (offsetof (state, ବସ୍ତୁଗୁଡ଼ିକ) + ବସ୍ତୁଗୁଡ଼ିକ_size);
  res->ସଂଖ୍ୟା = nstates++;
  res->accessing_ପ୍ରତୀକ = accessing_ପ୍ରତୀକ;
  res->transitions = ଖାଲି;
  res->reductions = ଖାଲି;
  res->errs = ଖାଲି;
  res->state_list = ଖାଲି;
  res->consistent = false;
  res->solved_conflicts = ଖାଲି;
  res->solved_conflicts_xml = ଖାଲି;

  res->nitems = nitems;
  memcpy (res->ବସ୍ତୁଗୁଡ଼ିକ, core, ବସ୍ତୁଗୁଡ଼ିକ_size);

  state_hash_ଭର୍ତ୍ତି (res);

  ଫେରନ୍ତୁ res;
}

state *
state_new_isocore (state ଅଚଳ *s)
{
  aver (nstates < STATE_ସଂଖ୍ୟା_MAXIMUM);

  size_t ବସ୍ତୁଗୁଡ଼ିକ_size = s->nitems * ଆକାର *s->ବସ୍ତୁଗୁଡ଼ିକ;
  state *res = xmalloc (offsetof (state, ବସ୍ତୁଗୁଡ଼ିକ) + ବସ୍ତୁଗୁଡ଼ିକ_size);
  res->ସଂଖ୍ୟା = nstates++;
  res->accessing_ପ୍ରତୀକ = s->accessing_ପ୍ରତୀକ;
  res->transitions =
    transitions_new (s->transitions->ଅଙ୍କ, s->transitions->states);
  res->reductions = reductions_new (s->reductions->ଅଙ୍କ, s->reductions->rules);
  res->errs = ଖାଲି;
  res->state_list = ଖାଲି;
  res->consistent = s->consistent;
  res->solved_conflicts = ଖାଲି;
  res->solved_conflicts_xml = ଖାଲି;

  res->nitems = s->nitems;
  memcpy (res->ବସ୍ତୁଗୁଡ଼ିକ, s->ବସ୍ତୁଗୁଡ଼ିକ, ବସ୍ତୁଗୁଡ଼ିକ_size);

  ଫେରନ୍ତୁ res;
}


/*---------.
| Free S.  |
`---------*/

ସ୍ଥିର ଶୂନ୍ୟ
state_free (state *s)
{
  ମୁକ୍ତ (s->transitions);
  ମୁକ୍ତ (s->reductions);
  ମୁକ୍ତ (s->errs);
  ମୁକ୍ତ (s);
}


ଶୂନ୍ୟ
state_transitions_print (ଅଚଳ state *s, FILE *out)
{
  ଅଚଳ transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->ସଂଖ୍ୟା, trans->ଅଙ୍କ);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < trans->ଅଙ୍କ; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->ସଂଖ୍ୟା,
             symbols[s->transitions->states[i]->accessing_ପ୍ରତୀକ]->tag,
             s->transitions->states[i]->ସଂଖ୍ୟା);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

ଶୂନ୍ୟ
state_transitions_set (state *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (ଅଙ୍କ, dst);
  ଯଦି (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

ଶୂନ୍ୟ
state_reductions_set (state *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (ଅଙ୍କ, reds);
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
state_reduction_find (state ଅଚଳ *s, rule ଅଚଳ *r)
{
  reductions *reds = s->reductions;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
    ଯଦି (reds->rules[i] == r)
      ଫେରନ୍ତୁ i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

ଶୂନ୍ୟ
state_errs_set (state *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ପ୍ରତୀକ **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (ଅଙ୍କ, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

ଶୂନ୍ୟ
state_rule_lookaheads_print (state ଅଚଳ *s, rule ଅଚଳ *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା red = state_reduction_find (s, r);

  /* Print them if there are.  */
  ଯଦି (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;
      ଅକ୍ଷର ଅଚଳ *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

ଶୂନ୍ୟ
state_rule_lookaheads_print_xml (state ଅଚଳ *s, rule ଅଚଳ *r,
                                       FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା red = state_reduction_find (s, r);

  /* Print them if there are.  */
  ଯଦି (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;
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

ସ୍ଥିର ସଂରଚନା hash_table *state_table = ଖାଲି;

/* Two states are equal if they have the same core items.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
state_compare (state ଅଚଳ *s1, state ଅଚଳ *s2)
{
  ଯଦି (s1->nitems != s2->nitems)
    ଫେରନ୍ତୁ false;

  ପାଇଁ (size_t i = 0; i < s1->nitems; ++i)
    ଯଦି (s1->ବସ୍ତୁଗୁଡ଼ିକ[i] != s2->ବସ୍ତୁଗୁଡ଼ିକ[i])
      ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
state_comparator (ଶୂନ୍ୟ ଅଚଳ *s1, ଶୂନ୍ୟ ଅଚଳ *s2)
{
  ଫେରନ୍ତୁ state_compare (s1, s2);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
state_hash (state ଅଚଳ *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t ଚାବି = 0;
  ପାଇଁ (size_t i = 0; i < s->nitems; ++i)
    ଚାବି += s->ବସ୍ତୁଗୁଡ଼ିକ[i];
  ଫେରନ୍ତୁ ଚାବି % tablesize;
}

ସ୍ଥିର size_t
state_hasher (ଶୂନ୍ୟ ଅଚଳ *s, size_t tablesize)
{
  ଫେରନ୍ତୁ state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

ଶୂନ୍ୟ
state_hash_new (ଶୂନ୍ୟ)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  ଖାଲି,
                                  state_hasher,
                                  state_comparator,
                                  ଖାଲି);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

ଶୂନ୍ୟ
state_hash_free (ଶୂନ୍ୟ)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

ଶୂନ୍ୟ
state_hash_ଭର୍ତ୍ତି (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ଅଚଳ item_index *core)
{
  size_t ବସ୍ତୁଗୁଡ଼ିକ_size = nitems * ଆକାର *core;
  state *probe = xmalloc (offsetof (state, ବସ୍ତୁଗୁଡ଼ିକ) + ବସ୍ତୁଗୁଡ଼ିକ_size);
  probe->nitems = nitems;
  memcpy (probe->ବସ୍ତୁଗୁଡ଼ିକ, core, ବସ୍ତୁଗୁଡ଼ିକ_size);
  state *entry = hash_lookup (state_table, probe);
  ମୁକ୍ତ (probe);
  ଫେରନ୍ତୁ entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
state_record_reachable_states (state *s, bitset reachable)
{
  ଯଦି (bitset_test (reachable, s->ସଂଖ୍ୟା))
    ଫେରନ୍ତୁ;
  bitset_set (reachable, s->ସଂଖ୍ୟା);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < s->transitions->ଅଙ୍କ; ++i)
    ଯଦି (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

ଶୂନ୍ୟ
state_remove_unreachable_states (state_ସଂଖ୍ୟା old_to_new[])
{
  state_ସଂଖ୍ୟା nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      ଯଦି (bitset_test (reachable, states[i]->ସଂଖ୍ୟା))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->ସଂଖ୍ୟା = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      ଅନ୍ୟଥା
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = ଖାଲି;


/*----------------------.
| Free all the states.  |
`----------------------*/

ଶୂନ୍ୟ
states_free (ଶୂନ୍ୟ)
{
  closure_free ();
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    state_free (states[i]);
  ମୁକ୍ତ (states);
}
