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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "state.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <hash.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

સ્થિર transitions *
transitions_new (પૂર્ણાંક અંક, state **dst)
{
  size_t states_size = અંક * કદ *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->અંક = અંક;
  memcpy (res->states, dst, states_size);
  પાછા res;
}


state *
transitions_to (state *s, પ્રતીક_સંખ્યા sym)
{
  transitions *trans = s->transitions;
  માટે (પૂર્ણાંક i = 0; i < trans->અંક; ++i)
    જો (TRANSITION_પ્રતીક (trans, i) == sym)
      પાછા trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (પૂર્ણાંક અંક, પ્રતીક **tokens)
{
  size_t symbols_size = અંક * કદ *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->અંક = અંક;
  જો (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  પાછા res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

સ્થિર reductions *
reductions_new (પૂર્ણાંક અંક, rule **reds)
{
  size_t rules_size = અંક * કદ *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->અંક = અંક;
  res->lookaheads = ખાલી;
  memcpy (res->rules, reds, rules_size);
  પાછા res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_સંખ્યા nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = ખાલી;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (પ્રતીક_સંખ્યા accessing_પ્રતીક,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_સંખ્યા_MAXIMUM);

  size_t વસ્તુઓ_size = nitems * કદ *core;
  state *res = xmalloc (offsetof (state, વસ્તુઓ) + વસ્તુઓ_size);
  res->સંખ્યા = nstates++;
  res->accessing_પ્રતીક = accessing_પ્રતીક;
  res->transitions = ખાલી;
  res->reductions = ખાલી;
  res->errs = ખાલી;
  res->state_list = ખાલી;
  res->consistent = false;
  res->solved_conflicts = ખાલી;
  res->solved_conflicts_xml = ખાલી;

  res->nitems = nitems;
  memcpy (res->વસ્તુઓ, core, વસ્તુઓ_size);

  state_hash_ઉમેરો (res);

  પાછા res;
}

state *
state_new_isocore (state અચલ *s)
{
  aver (nstates < STATE_સંખ્યા_MAXIMUM);

  size_t વસ્તુઓ_size = s->nitems * કદ *s->વસ્તુઓ;
  state *res = xmalloc (offsetof (state, વસ્તુઓ) + વસ્તુઓ_size);
  res->સંખ્યા = nstates++;
  res->accessing_પ્રતીક = s->accessing_પ્રતીક;
  res->transitions =
    transitions_new (s->transitions->અંક, s->transitions->states);
  res->reductions = reductions_new (s->reductions->અંક, s->reductions->rules);
  res->errs = ખાલી;
  res->state_list = ખાલી;
  res->consistent = s->consistent;
  res->solved_conflicts = ખાલી;
  res->solved_conflicts_xml = ખાલી;

  res->nitems = s->nitems;
  memcpy (res->વસ્તુઓ, s->વસ્તુઓ, વસ્તુઓ_size);

  પાછા res;
}


/*---------.
| Free S.  |
`---------*/

સ્થિર શૂન્ય
state_free (state *s)
{
  મુક્ત (s->transitions);
  મુક્ત (s->reductions);
  મુક્ત (s->errs);
  મુક્ત (s);
}


શૂન્ય
state_transitions_print (અચલ state *s, FILE *out)
{
  અચલ transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->સંખ્યા, trans->અંક);
  માટે (પૂર્ણાંક i = 0; i < trans->અંક; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->સંખ્યા,
             symbols[s->transitions->states[i]->accessing_પ્રતીક]->tag,
             s->transitions->states[i]->સંખ્યા);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

શૂન્ય
state_transitions_set (state *s, પૂર્ણાંક અંક, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (અંક, dst);
  જો (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

શૂન્ય
state_reductions_set (state *s, પૂર્ણાંક અંક, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (અંક, reds);
}


પૂર્ણાંક
state_reduction_find (state અચલ *s, rule અચલ *r)
{
  reductions *reds = s->reductions;
  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
    જો (reds->rules[i] == r)
      પાછા i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

શૂન્ય
state_errs_set (state *s, પૂર્ણાંક અંક, પ્રતીક **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (અંક, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

શૂન્ય
state_rule_lookaheads_print (state અચલ *s, rule અચલ *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  પૂર્ણાંક red = state_reduction_find (s, r);

  /* Print them if there are.  */
  જો (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      પૂર્ણાંક k;
      અક્ષર અચલ *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

શૂન્ય
state_rule_lookaheads_print_xml (state અચલ *s, rule અચલ *r,
                                       FILE *out, પૂર્ણાંક level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  પૂર્ણાંક red = state_reduction_find (s, r);

  /* Print them if there are.  */
  જો (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      પૂર્ણાંક k;
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

સ્થિર સંરચના hash_table *state_table = ખાલી;

/* Two states are equal if they have the same core items.  */
સ્થિર અંતઃરેખ bool
state_compare (state અચલ *s1, state અચલ *s2)
{
  જો (s1->nitems != s2->nitems)
    પાછા false;

  માટે (size_t i = 0; i < s1->nitems; ++i)
    જો (s1->વસ્તુઓ[i] != s2->વસ્તુઓ[i])
      પાછા false;

  પાછા true;
}

સ્થિર bool
state_comparator (શૂન્ય અચલ *s1, શૂન્ય અચલ *s2)
{
  પાછા state_compare (s1, s2);
}

સ્થિર અંતઃરેખ size_t
state_hash (state અચલ *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t ચાવી = 0;
  માટે (size_t i = 0; i < s->nitems; ++i)
    ચાવી += s->વસ્તુઓ[i];
  પાછા ચાવી % tablesize;
}

સ્થિર size_t
state_hasher (શૂન્ય અચલ *s, size_t tablesize)
{
  પાછા state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

શૂન્ય
state_hash_new (શૂન્ય)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  ખાલી,
                                  state_hasher,
                                  state_comparator,
                                  ખાલી);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

શૂન્ય
state_hash_free (શૂન્ય)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

શૂન્ય
state_hash_ઉમેરો (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, અચલ item_index *core)
{
  size_t વસ્તુઓ_size = nitems * કદ *core;
  state *probe = xmalloc (offsetof (state, વસ્તુઓ) + વસ્તુઓ_size);
  probe->nitems = nitems;
  memcpy (probe->વસ્તુઓ, core, વસ્તુઓ_size);
  state *entry = hash_lookup (state_table, probe);
  મુક્ત (probe);
  પાછા entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

સ્થિર શૂન્ય
state_record_reachable_states (state *s, bitset reachable)
{
  જો (bitset_test (reachable, s->સંખ્યા))
    પાછા;
  bitset_set (reachable, s->સંખ્યા);
  માટે (પૂર્ણાંક i = 0; i < s->transitions->અંક; ++i)
    જો (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

શૂન્ય
state_remove_unreachable_states (state_સંખ્યા old_to_new[])
{
  state_સંખ્યા nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      જો (bitset_test (reachable, states[i]->સંખ્યા))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->સંખ્યા = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      નહીંતર
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = ખાલી;


/*----------------------.
| Free all the states.  |
`----------------------*/

શૂન્ય
states_free (શૂન્ય)
{
  closure_free ();
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    state_free (states[i]);
  મુક્ત (states);
}
