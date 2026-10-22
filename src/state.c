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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "state.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <hash.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

ಸ್ಥಿರ transitions *
transitions_new (ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, state **dst)
{
  size_t states_size = ಅಂಕೆ * ಪರಿಮಾಣ *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->ಅಂಕೆ = ಅಂಕೆ;
  memcpy (res->states, dst, states_size);
  ಪ್ರತ್ಯಾಗಮ res;
}


state *
transitions_to (state *s, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  transitions *trans = s->transitions;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < trans->ಅಂಕೆ; ++i)
    ಯದಿ (TRANSITION_ಚಿಹ್ನೆ (trans, i) == sym)
      ಪ್ರತ್ಯಾಗಮ trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಚಿಹ್ನೆ **tokens)
{
  size_t symbols_size = ಅಂಕೆ * ಪರಿಮಾಣ *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->ಅಂಕೆ = ಅಂಕೆ;
  ಯದಿ (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  ಪ್ರತ್ಯಾಗಮ res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ಸ್ಥಿರ reductions *
reductions_new (ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, rule **reds)
{
  size_t rules_size = ಅಂಕೆ * ಪರಿಮಾಣ *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->ಅಂಕೆ = ಅಂಕೆ;
  res->lookaheads = ರಿಕ್ತ;
  memcpy (res->rules, reds, rules_size);
  ಪ್ರತ್ಯಾಗಮ res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_ಸಂಖ್ಯೆ nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = ರಿಕ್ತ;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ accessing_ಚಿಹ್ನೆ,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_ಸಂಖ್ಯೆ_MAXIMUM);

  size_t ವಸ್ತುಗಳು_size = nitems * ಪರಿಮಾಣ *core;
  state *res = xmalloc (offsetof (state, ವಸ್ತುಗಳು) + ವಸ್ತುಗಳು_size);
  res->ಸಂಖ್ಯೆ = nstates++;
  res->accessing_ಚಿಹ್ನೆ = accessing_ಚಿಹ್ನೆ;
  res->transitions = ರಿಕ್ತ;
  res->reductions = ರಿಕ್ತ;
  res->errs = ರಿಕ್ತ;
  res->state_list = ರಿಕ್ತ;
  res->consistent = false;
  res->solved_conflicts = ರಿಕ್ತ;
  res->solved_conflicts_xml = ರಿಕ್ತ;

  res->nitems = nitems;
  memcpy (res->ವಸ್ತುಗಳು, core, ವಸ್ತುಗಳು_size);

  state_hash_ಸೇರಿಸು (res);

  ಪ್ರತ್ಯಾಗಮ res;
}

state *
state_new_isocore (state ಧ್ರುವ *s)
{
  aver (nstates < STATE_ಸಂಖ್ಯೆ_MAXIMUM);

  size_t ವಸ್ತುಗಳು_size = s->nitems * ಪರಿಮಾಣ *s->ವಸ್ತುಗಳು;
  state *res = xmalloc (offsetof (state, ವಸ್ತುಗಳು) + ವಸ್ತುಗಳು_size);
  res->ಸಂಖ್ಯೆ = nstates++;
  res->accessing_ಚಿಹ್ನೆ = s->accessing_ಚಿಹ್ನೆ;
  res->transitions =
    transitions_new (s->transitions->ಅಂಕೆ, s->transitions->states);
  res->reductions = reductions_new (s->reductions->ಅಂಕೆ, s->reductions->rules);
  res->errs = ರಿಕ್ತ;
  res->state_list = ರಿಕ್ತ;
  res->consistent = s->consistent;
  res->solved_conflicts = ರಿಕ್ತ;
  res->solved_conflicts_xml = ರಿಕ್ತ;

  res->nitems = s->nitems;
  memcpy (res->ವಸ್ತುಗಳು, s->ವಸ್ತುಗಳು, ವಸ್ತುಗಳು_size);

  ಪ್ರತ್ಯಾಗಮ res;
}


/*---------.
| Free S.  |
`---------*/

ಸ್ಥಿರ ಶೂನ್ಯ
state_free (state *s)
{
  ಮುಕ್ತ (s->transitions);
  ಮುಕ್ತ (s->reductions);
  ಮುಕ್ತ (s->errs);
  ಮುಕ್ತ (s);
}


ಶೂನ್ಯ
state_transitions_print (ಧ್ರುವ state *s, FILE *out)
{
  ಧ್ರುವ transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->ಸಂಖ್ಯೆ, trans->ಅಂಕೆ);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < trans->ಅಂಕೆ; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->ಸಂಖ್ಯೆ,
             symbols[s->transitions->states[i]->accessing_ಚಿಹ್ನೆ]->tag,
             s->transitions->states[i]->ಸಂಖ್ಯೆ);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

ಶೂನ್ಯ
state_transitions_set (state *s, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (ಅಂಕೆ, dst);
  ಯದಿ (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

ಶೂನ್ಯ
state_reductions_set (state *s, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (ಅಂಕೆ, reds);
}


ಪೂರ್ಣಾಙ್ಕ
state_reduction_find (state ಧ್ರುವ *s, rule ಧ್ರುವ *r)
{
  reductions *reds = s->reductions;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಯದಿ (reds->rules[i] == r)
      ಪ್ರತ್ಯಾಗಮ i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

ಶೂನ್ಯ
state_errs_set (state *s, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಚಿಹ್ನೆ **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (ಅಂಕೆ, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

ಶೂನ್ಯ
state_rule_lookaheads_print (state ಧ್ರುವ *s, rule ಧ್ರುವ *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  ಪೂರ್ಣಾಙ್ಕ red = state_reduction_find (s, r);

  /* Print them if there are.  */
  ಯದಿ (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      ಪೂರ್ಣಾಙ್ಕ k;
      ವರ್ಣ ಧ್ರುವ *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

ಶೂನ್ಯ
state_rule_lookaheads_print_xml (state ಧ್ರುವ *s, rule ಧ್ರುವ *r,
                                       FILE *out, ಪೂರ್ಣಾಙ್ಕ level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  ಪೂರ್ಣಾಙ್ಕ red = state_reduction_find (s, r);

  /* Print them if there are.  */
  ಯದಿ (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      ಪೂರ್ಣಾಙ್ಕ k;
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

ಸ್ಥಿರ ಸಂರಚನಾ hash_table *state_table = ರಿಕ್ತ;

/* Two states are equal if they have the same core items.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
state_compare (state ಧ್ರುವ *s1, state ಧ್ರುವ *s2)
{
  ಯದಿ (s1->nitems != s2->nitems)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತಿ (size_t i = 0; i < s1->nitems; ++i)
    ಯದಿ (s1->ವಸ್ತುಗಳು[i] != s2->ವಸ್ತುಗಳು[i])
      ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
state_comparator (ಶೂನ್ಯ ಧ್ರುವ *s1, ಶೂನ್ಯ ಧ್ರುವ *s2)
{
  ಪ್ರತ್ಯಾಗಮ state_compare (s1, s2);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
state_hash (state ಧ್ರುವ *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t ಕೀಲಿ = 0;
  ಪ್ರತಿ (size_t i = 0; i < s->nitems; ++i)
    ಕೀಲಿ += s->ವಸ್ತುಗಳು[i];
  ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ % tablesize;
}

ಸ್ಥಿರ size_t
state_hasher (ಶೂನ್ಯ ಧ್ರುವ *s, size_t tablesize)
{
  ಪ್ರತ್ಯಾಗಮ state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

ಶೂನ್ಯ
state_hash_new (ಶೂನ್ಯ)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  ರಿಕ್ತ,
                                  state_hasher,
                                  state_comparator,
                                  ರಿಕ್ತ);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

ಶೂನ್ಯ
state_hash_free (ಶೂನ್ಯ)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

ಶೂನ್ಯ
state_hash_ಸೇರಿಸು (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ಧ್ರುವ item_index *core)
{
  size_t ವಸ್ತುಗಳು_size = nitems * ಪರಿಮಾಣ *core;
  state *probe = xmalloc (offsetof (state, ವಸ್ತುಗಳು) + ವಸ್ತುಗಳು_size);
  probe->nitems = nitems;
  memcpy (probe->ವಸ್ತುಗಳು, core, ವಸ್ತುಗಳು_size);
  state *entry = hash_lookup (state_table, probe);
  ಮುಕ್ತ (probe);
  ಪ್ರತ್ಯಾಗಮ entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
state_record_reachable_states (state *s, bitset reachable)
{
  ಯದಿ (bitset_test (reachable, s->ಸಂಖ್ಯೆ))
    ಪ್ರತ್ಯಾಗಮ;
  bitset_set (reachable, s->ಸಂಖ್ಯೆ);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < s->transitions->ಅಂಕೆ; ++i)
    ಯದಿ (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

ಶೂನ್ಯ
state_remove_unreachable_states (state_ಸಂಖ್ಯೆ old_to_new[])
{
  state_ಸಂಖ್ಯೆ nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಯದಿ (bitset_test (reachable, states[i]->ಸಂಖ್ಯೆ))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->ಸಂಖ್ಯೆ = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      ಅನ್ಯಥಾ
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = ರಿಕ್ತ;


/*----------------------.
| Free all the states.  |
`----------------------*/

ಶೂನ್ಯ
states_free (ಶೂನ್ಯ)
{
  closure_free ();
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    state_free (states[i]);
  ಮುಕ್ತ (states);
}
