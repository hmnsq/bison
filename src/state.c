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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "state.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <hash.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

സ്ഥിതം transitions *
transitions_new (പൂർണ്ണസംഖ്യ അക്കം, state **dst)
{
  size_t states_size = അക്കം * വലുപ്പം *dst;
  transitions *res = xmalloc (offsetof (transitions, states) + states_size);
  res->അക്കം = അക്കം;
  memcpy (res->states, dst, states_size);
  തിരികെ res;
}


state *
transitions_to (state *s, ചിഹ്നം_സംഖ്യ sym)
{
  transitions *trans = s->transitions;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < trans->അക്കം; ++i)
    എങ്കിൽ (TRANSITION_ചിഹ്നം (trans, i) == sym)
      തിരികെ trans->states[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (പൂർണ്ണസംഖ്യ അക്കം, ചിഹ്നം **tokens)
{
  size_t symbols_size = അക്കം * വലുപ്പം *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_size);
  res->അക്കം = അക്കം;
  എങ്കിൽ (tokens)
    memcpy (res->symbols, tokens, symbols_size);
  തിരികെ res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

സ്ഥിതം reductions *
reductions_new (പൂർണ്ണസംഖ്യ അക്കം, rule **reds)
{
  size_t rules_size = അക്കം * വലുപ്പം *reds;
  reductions *res = xmalloc (offsetof (reductions, rules) + rules_size);
  res->അക്കം = അക്കം;
  res->lookaheads = ശൂന്യ;
  memcpy (res->rules, reds, rules_size);
  തിരികെ res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_സംഖ്യ nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = ശൂന്യ;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (ചിഹ്നം_സംഖ്യ accessing_ചിഹ്നം,
           size_t nitems, item_index *core)
{
  aver (nstates < STATE_സംഖ്യ_MAXIMUM);

  size_t ഇനങ്ങൾ_size = nitems * വലുപ്പം *core;
  state *res = xmalloc (offsetof (state, ഇനങ്ങൾ) + ഇനങ്ങൾ_size);
  res->സംഖ്യ = nstates++;
  res->accessing_ചിഹ്നം = accessing_ചിഹ്നം;
  res->transitions = ശൂന്യ;
  res->reductions = ശൂന്യ;
  res->errs = ശൂന്യ;
  res->state_list = ശൂന്യ;
  res->consistent = false;
  res->solved_conflicts = ശൂന്യ;
  res->solved_conflicts_xml = ശൂന്യ;

  res->nitems = nitems;
  memcpy (res->ഇനങ്ങൾ, core, ഇനങ്ങൾ_size);

  state_hash_തിരുകുക (res);

  തിരികെ res;
}

state *
state_new_isocore (state സ്ഥിരം *s)
{
  aver (nstates < STATE_സംഖ്യ_MAXIMUM);

  size_t ഇനങ്ങൾ_size = s->nitems * വലുപ്പം *s->ഇനങ്ങൾ;
  state *res = xmalloc (offsetof (state, ഇനങ്ങൾ) + ഇനങ്ങൾ_size);
  res->സംഖ്യ = nstates++;
  res->accessing_ചിഹ്നം = s->accessing_ചിഹ്നം;
  res->transitions =
    transitions_new (s->transitions->അക്കം, s->transitions->states);
  res->reductions = reductions_new (s->reductions->അക്കം, s->reductions->rules);
  res->errs = ശൂന്യ;
  res->state_list = ശൂന്യ;
  res->consistent = s->consistent;
  res->solved_conflicts = ശൂന്യ;
  res->solved_conflicts_xml = ശൂന്യ;

  res->nitems = s->nitems;
  memcpy (res->ഇനങ്ങൾ, s->ഇനങ്ങൾ, ഇനങ്ങൾ_size);

  തിരികെ res;
}


/*---------.
| Free S.  |
`---------*/

സ്ഥിതം ശൂന്യം
state_free (state *s)
{
  സ്വതന്ത്രം (s->transitions);
  സ്വതന്ത്രം (s->reductions);
  സ്വതന്ത്രം (s->errs);
  സ്വതന്ത്രം (s);
}


ശൂന്യം
state_transitions_print (സ്ഥിരം state *s, FILE *out)
{
  സ്ഥിരം transitions *trans = s->transitions;
  fprintf (out, "transitions of %d (%d):\n",
           s->സംഖ്യ, trans->അക്കം);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < trans->അക്കം; ++i)
    fprintf (out, "  %d: (%d, %s, %d)\n",
             i,
             s->സംഖ്യ,
             symbols[s->transitions->states[i]->accessing_ചിഹ്നം]->tag,
             s->transitions->states[i]->സംഖ്യ);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

ശൂന്യം
state_transitions_set (state *s, പൂർണ്ണസംഖ്യ അക്കം, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (അക്കം, dst);
  എങ്കിൽ (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

ശൂന്യം
state_reductions_set (state *s, പൂർണ്ണസംഖ്യ അക്കം, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (അക്കം, reds);
}


പൂർണ്ണസംഖ്യ
state_reduction_find (state സ്ഥിരം *s, rule സ്ഥിരം *r)
{
  reductions *reds = s->reductions;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
    എങ്കിൽ (reds->rules[i] == r)
      തിരികെ i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

ശൂന്യം
state_errs_set (state *s, പൂർണ്ണസംഖ്യ അക്കം, ചിഹ്നം **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (അക്കം, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

ശൂന്യം
state_rule_lookaheads_print (state സ്ഥിരം *s, rule സ്ഥിരം *r, FILE *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  പൂർണ്ണസംഖ്യ red = state_reduction_find (s, r);

  /* Print them if there are.  */
  എങ്കിൽ (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      പൂർണ്ണസംഖ്യ k;
      അക്ഷരം സ്ഥിരം *sep = "";
      fprintf (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      fprintf (out, "]");
    }
}

ശൂന്യം
state_rule_lookaheads_print_xml (state സ്ഥിരം *s, rule സ്ഥിരം *r,
                                       FILE *out, പൂർണ്ണസംഖ്യ level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  പൂർണ്ണസംഖ്യ red = state_reduction_find (s, r);

  /* Print them if there are.  */
  എങ്കിൽ (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      പൂർണ്ണസംഖ്യ k;
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

സ്ഥിതം ഘടന hash_table *state_table = ശൂന്യ;

/* Two states are equal if they have the same core items.  */
സ്ഥിതം വരിയിൽ bool
state_compare (state സ്ഥിരം *s1, state സ്ഥിരം *s2)
{
  എങ്കിൽ (s1->nitems != s2->nitems)
    തിരികെ false;

  വേണ്ടി (size_t i = 0; i < s1->nitems; ++i)
    എങ്കിൽ (s1->ഇനങ്ങൾ[i] != s2->ഇനങ്ങൾ[i])
      തിരികെ false;

  തിരികെ true;
}

സ്ഥിതം bool
state_comparator (ശൂന്യം സ്ഥിരം *s1, ശൂന്യം സ്ഥിരം *s2)
{
  തിരികെ state_compare (s1, s2);
}

സ്ഥിതം വരിയിൽ size_t
state_hash (state സ്ഥിരം *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t താക്കോൽ = 0;
  വേണ്ടി (size_t i = 0; i < s->nitems; ++i)
    താക്കോൽ += s->ഇനങ്ങൾ[i];
  തിരികെ താക്കോൽ % tablesize;
}

സ്ഥിതം size_t
state_hasher (ശൂന്യം സ്ഥിരം *s, size_t tablesize)
{
  തിരികെ state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

ശൂന്യം
state_hash_new (ശൂന്യം)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  ശൂന്യ,
                                  state_hasher,
                                  state_comparator,
                                  ശൂന്യ);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

ശൂന്യം
state_hash_free (ശൂന്യം)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

ശൂന്യം
state_hash_തിരുകുക (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, സ്ഥിരം item_index *core)
{
  size_t ഇനങ്ങൾ_size = nitems * വലുപ്പം *core;
  state *probe = xmalloc (offsetof (state, ഇനങ്ങൾ) + ഇനങ്ങൾ_size);
  probe->nitems = nitems;
  memcpy (probe->ഇനങ്ങൾ, core, ഇനങ്ങൾ_size);
  state *entry = hash_lookup (state_table, probe);
  സ്വതന്ത്രം (probe);
  തിരികെ entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
state_record_reachable_states (state *s, bitset reachable)
{
  എങ്കിൽ (bitset_test (reachable, s->സംഖ്യ))
    തിരികെ;
  bitset_set (reachable, s->സംഖ്യ);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < s->transitions->അക്കം; ++i)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

ശൂന്യം
state_remove_unreachable_states (state_സംഖ്യ old_to_new[])
{
  state_സംഖ്യ nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      എങ്കിൽ (bitset_test (reachable, states[i]->സംഖ്യ))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->സംഖ്യ = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      അല്ലെങ്കിൽ
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = ശൂന്യ;


/*----------------------.
| Free all the states.  |
`----------------------*/

ശൂന്യം
states_free (ശൂന്യം)
{
  closure_free ();
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    state_free (states[i]);
  സ്വതന്ത്രം (states);
}
