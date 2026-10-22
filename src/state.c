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

#अन्तर्भाव <config.h>
#अन्तर्भाव "state.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <hash.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "complain.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

स्थिर transitions *
transitions_new (पूर्णाङ्क अंक, state **dst)
{
  माप_प्रकार states_size = अंक * परिमाण *dst;
  transitions *res = xmalloc (दुरत्व (transitions, states) + states_size);
  res->अंक = अंक;
  स_नकल (res->states, dst, states_size);
  प्रत्यागम res;
}


state *
transitions_to (state *s, प्रतीक_संख्या sym)
{
  transitions *trans = s->transitions;
  प्रति (पूर्णाङ्क i = 0; i < trans->अंक; ++i)
    यदि (TRANSITION_प्रतीक (trans, i) == sym)
      प्रत्यागम trans->states[i];
  पात ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (पूर्णाङ्क अंक, प्रतीक **tokens)
{
  माप_प्रकार symbols_size = अंक * परिमाण *tokens;
  errs *res = xmalloc (दुरत्व (errs, symbols) + symbols_size);
  res->अंक = अंक;
  यदि (tokens)
    स_नकल (res->symbols, tokens, symbols_size);
  प्रत्यागम res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

स्थिर reductions *
reductions_new (पूर्णाङ्क अंक, rule **reds)
{
  माप_प्रकार rules_size = अंक * परिमाण *reds;
  reductions *res = xmalloc (दुरत्व (reductions, rules) + rules_size);
  res->अंक = अंक;
  res->lookaheads = रिक्त;
  स_नकल (res->rules, reds, rules_size);
  प्रत्यागम res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_संख्या nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = रिक्त;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (प्रतीक_संख्या accessing_प्रतीक,
           माप_प्रकार nitems, item_index *core)
{
  aver (nstates < STATE_संख्या_MAXIMUM);

  माप_प्रकार वस्तुएँ_size = nitems * परिमाण *core;
  state *res = xmalloc (दुरत्व (state, वस्तुएँ) + वस्तुएँ_size);
  res->संख्या = nstates++;
  res->accessing_प्रतीक = accessing_प्रतीक;
  res->transitions = रिक्त;
  res->reductions = रिक्त;
  res->errs = रिक्त;
  res->state_list = रिक्त;
  res->consistent = false;
  res->solved_conflicts = रिक्त;
  res->solved_conflicts_xml = रिक्त;

  res->nitems = nitems;
  स_नकल (res->वस्तुएँ, core, वस्तुएँ_size);

  state_hash_जोड़ो (res);

  प्रत्यागम res;
}

state *
state_new_isocore (state ध्रुव *s)
{
  aver (nstates < STATE_संख्या_MAXIMUM);

  माप_प्रकार वस्तुएँ_size = s->nitems * परिमाण *s->वस्तुएँ;
  state *res = xmalloc (दुरत्व (state, वस्तुएँ) + वस्तुएँ_size);
  res->संख्या = nstates++;
  res->accessing_प्रतीक = s->accessing_प्रतीक;
  res->transitions =
    transitions_new (s->transitions->अंक, s->transitions->states);
  res->reductions = reductions_new (s->reductions->अंक, s->reductions->rules);
  res->errs = रिक्त;
  res->state_list = रिक्त;
  res->consistent = s->consistent;
  res->solved_conflicts = रिक्त;
  res->solved_conflicts_xml = रिक्त;

  res->nitems = s->nitems;
  स_नकल (res->वस्तुएँ, s->वस्तुएँ, वस्तुएँ_size);

  प्रत्यागम res;
}


/*---------.
| Free S.  |
`---------*/

स्थिर शून्य
state_free (state *s)
{
  मुक्त (s->transitions);
  मुक्त (s->reductions);
  मुक्त (s->errs);
  मुक्त (s);
}


शून्य
state_transitions_print (ध्रुव state *s, खाता *out)
{
  ध्रुव transitions *trans = s->transitions;
  ख_लिखो (out, "transitions of %d (%d):\n",
           s->संख्या, trans->अंक);
  प्रति (पूर्णाङ्क i = 0; i < trans->अंक; ++i)
    ख_लिखो (out, "  %d: (%d, %s, %d)\n",
             i,
             s->संख्या,
             symbols[s->transitions->states[i]->accessing_प्रतीक]->tag,
             s->transitions->states[i]->संख्या);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

शून्य
state_transitions_set (state *s, पूर्णाङ्क अंक, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (अंक, dst);
  यदि (trace_flag & trace_automaton)
    state_transitions_print (s, मानक_त्रुटि);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

शून्य
state_reductions_set (state *s, पूर्णाङ्क अंक, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (अंक, reds);
}


पूर्णाङ्क
state_reduction_find (state ध्रुव *s, rule ध्रुव *r)
{
  reductions *reds = s->reductions;
  प्रति (पूर्णाङ्क i = 0; i < reds->अंक; ++i)
    यदि (reds->rules[i] == r)
      प्रत्यागम i;
  पात ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

शून्य
state_errs_set (state *s, पूर्णाङ्क अंक, प्रतीक **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (अंक, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

शून्य
state_rule_lookaheads_print (state ध्रुव *s, rule ध्रुव *r, खाता *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  पूर्णाङ्क red = state_reduction_find (s, r);

  /* Print them if there are.  */
  यदि (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      पूर्णाङ्क k;
      वर्ण ध्रुव *sep = "";
      ख_लिखो (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          ख_लिखो (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      ख_लिखो (out, "]");
    }
}

शून्य
state_rule_lookaheads_print_xml (state ध्रुव *s, rule ध्रुव *r,
                                       खाता *out, पूर्णाङ्क level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  पूर्णाङ्क red = state_reduction_find (s, r);

  /* Print them if there are.  */
  यदि (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      पूर्णाङ्क k;
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

स्थिर संरचना hash_table *state_table = रिक्त;

/* Two states are equal if they have the same core items.  */
स्थिर अन्तःस्थ bool
state_compare (state ध्रुव *s1, state ध्रुव *s2)
{
  यदि (s1->nitems != s2->nitems)
    प्रत्यागम false;

  प्रति (माप_प्रकार i = 0; i < s1->nitems; ++i)
    यदि (s1->वस्तुएँ[i] != s2->वस्तुएँ[i])
      प्रत्यागम false;

  प्रत्यागम true;
}

स्थिर bool
state_comparator (शून्य ध्रुव *s1, शून्य ध्रुव *s2)
{
  प्रत्यागम state_compare (s1, s2);
}

स्थिर अन्तःस्थ माप_प्रकार
state_hash (state ध्रुव *s, माप_प्रकार tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  माप_प्रकार कुंजी = 0;
  प्रति (माप_प्रकार i = 0; i < s->nitems; ++i)
    कुंजी += s->वस्तुएँ[i];
  प्रत्यागम कुंजी % tablesize;
}

स्थिर माप_प्रकार
state_hasher (शून्य ध्रुव *s, माप_प्रकार tablesize)
{
  प्रत्यागम state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

शून्य
state_hash_new (शून्य)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  रिक्त,
                                  state_hasher,
                                  state_comparator,
                                  रिक्त);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

शून्य
state_hash_free (शून्य)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

शून्य
state_hash_जोड़ो (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (माप_प्रकार nitems, ध्रुव item_index *core)
{
  माप_प्रकार वस्तुएँ_size = nitems * परिमाण *core;
  state *probe = xmalloc (दुरत्व (state, वस्तुएँ) + वस्तुएँ_size);
  probe->nitems = nitems;
  स_नकल (probe->वस्तुएँ, core, वस्तुएँ_size);
  state *entry = hash_lookup (state_table, probe);
  मुक्त (probe);
  प्रत्यागम entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

स्थिर शून्य
state_record_reachable_states (state *s, bitset reachable)
{
  यदि (bitset_test (reachable, s->संख्या))
    प्रत्यागम;
  bitset_set (reachable, s->संख्या);
  प्रति (पूर्णाङ्क i = 0; i < s->transitions->अंक; ++i)
    यदि (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

शून्य
state_remove_unreachable_states (state_संख्या old_to_new[])
{
  state_संख्या nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  प्रति (state_संख्या i = 0; i < nstates; ++i)
    {
      यदि (bitset_test (reachable, states[i]->संख्या))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->संख्या = nstates_reachable;
          old_to_new[i] = nstates_reachable++;
        }
      अन्यथा
        {
          state_free (states[i]);
          old_to_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **states = रिक्त;


/*----------------------.
| Free all the states.  |
`----------------------*/

शून्य
states_free (शून्य)
{
  closure_free ();
  प्रति (state_संख्या i = 0; i < nstates; ++i)
    state_free (states[i]);
  मुक्त (states);
}
