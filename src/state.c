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

#समावेश <config.h>
#समावेश "state.h"

#समावेश "system.h"

#समावेश <hash.h>

#समावेश "closure.h"
#समावेश "complain.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

अटल transitions *
transitions_new (पूर्णांक अंक, state **dst)
{
  माप_प्रकार states_आकार = अंक * माप *dst;
  transitions *res = xmalloc (दुरत्व (transitions, states) + states_आकार);
  res->अंक = अंक;
  स_नकल (res->states, dst, states_आकार);
  वापस res;
}


state *
transitions_to (state *s, प्रतीक_संख्या sym)
{
  transitions *trans = s->transitions;
  क्रम (पूर्णांक i = 0; i < trans->अंक; ++i)
    अगर (TRANSITION_प्रतीक (trans, i) == sym)
      वापस trans->states[i];
  पात ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (पूर्णांक अंक, प्रतीक **tokens)
{
  माप_प्रकार symbols_आकार = अंक * माप *tokens;
  errs *res = xmalloc (दुरत्व (errs, symbols) + symbols_आकार);
  res->अंक = अंक;
  अगर (tokens)
    स_नकल (res->symbols, tokens, symbols_आकार);
  वापस res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

अटल reductions *
reductions_new (पूर्णांक अंक, rule **reds)
{
  माप_प्रकार rules_आकार = अंक * माप *reds;
  reductions *res = xmalloc (दुरत्व (reductions, rules) + rules_आकार);
  res->अंक = अंक;
  res->lookaheads = शून्य;
  स_नकल (res->rules, reds, rules_आकार);
  वापस res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_संख्या nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = शून्य;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (प्रतीक_संख्या accessing_प्रतीक,
           माप_प्रकार nitems, item_सूचकांक *core)
{
  aver (nstates < STATE_संख्या_MAXIMUM);

  माप_प्रकार वस्तुएँ_आकार = nitems * माप *core;
  state *res = xmalloc (दुरत्व (state, वस्तुएँ) + वस्तुएँ_आकार);
  res->संख्या = nstates++;
  res->accessing_प्रतीक = accessing_प्रतीक;
  res->transitions = शून्य;
  res->reductions = शून्य;
  res->errs = शून्य;
  res->state_list = शून्य;
  res->consistent = false;
  res->solved_conflicts = शून्य;
  res->solved_conflicts_xml = शून्य;

  res->nitems = nitems;
  स_नकल (res->वस्तुएँ, core, वस्तुएँ_आकार);

  state_hash_जोड़ो (res);

  वापस res;
}

state *
state_new_isocore (state स्थिर *s)
{
  aver (nstates < STATE_संख्या_MAXIMUM);

  माप_प्रकार वस्तुएँ_आकार = s->nitems * माप *s->वस्तुएँ;
  state *res = xmalloc (दुरत्व (state, वस्तुएँ) + वस्तुएँ_आकार);
  res->संख्या = nstates++;
  res->accessing_प्रतीक = s->accessing_प्रतीक;
  res->transitions =
    transitions_new (s->transitions->अंक, s->transitions->states);
  res->reductions = reductions_new (s->reductions->अंक, s->reductions->rules);
  res->errs = शून्य;
  res->state_list = शून्य;
  res->consistent = s->consistent;
  res->solved_conflicts = शून्य;
  res->solved_conflicts_xml = शून्य;

  res->nitems = s->nitems;
  स_नकल (res->वस्तुएँ, s->वस्तुएँ, वस्तुएँ_आकार);

  वापस res;
}


/*---------.
| Free S.  |
`---------*/

अटल व्योम
state_free (state *s)
{
  मुक्त (s->transitions);
  मुक्त (s->reductions);
  मुक्त (s->errs);
  मुक्त (s);
}


व्योम
state_transitions_print (स्थिर state *s, खाता *out)
{
  स्थिर transitions *trans = s->transitions;
  ख_लिखो (out, "transitions of %d (%d):\n",
           s->संख्या, trans->अंक);
  क्रम (पूर्णांक i = 0; i < trans->अंक; ++i)
    ख_लिखो (out, "  %d: (%d, %s, %d)\n",
             i,
             s->संख्या,
             symbols[s->transitions->states[i]->accessing_प्रतीक]->tag,
             s->transitions->states[i]->संख्या);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

व्योम
state_transitions_set (state *s, पूर्णांक अंक, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (अंक, dst);
  अगर (trace_flag & trace_automaton)
    state_transitions_print (s, मानक_त्रुटि);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

व्योम
state_reductions_set (state *s, पूर्णांक अंक, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (अंक, reds);
}


पूर्णांक
state_reduction_find (state स्थिर *s, rule स्थिर *r)
{
  reductions *reds = s->reductions;
  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
    अगर (reds->rules[i] == r)
      वापस i;
  पात ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

व्योम
state_errs_set (state *s, पूर्णांक अंक, प्रतीक **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (अंक, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

व्योम
state_rule_lookaheads_print (state स्थिर *s, rule स्थिर *r, खाता *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  पूर्णांक red = state_reduction_find (s, r);

  /* Print them if there are.  */
  अगर (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      पूर्णांक k;
      अक्षर स्थिर *sep = "";
      ख_लिखो (out, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          ख_लिखो (out, "%s%s", sep, symbols[k]->tag);
          sep = ", ";
        }
      ख_लिखो (out, "]");
    }
}

व्योम
state_rule_lookaheads_print_xml (state स्थिर *s, rule स्थिर *r,
                                       खाता *out, पूर्णांक level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  पूर्णांक red = state_reduction_find (s, r);

  /* Print them if there are.  */
  अगर (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      पूर्णांक k;
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
#घोषणा HT_INITIAL_CAPACITY 257

अटल काष्ठा hash_table *state_table = शून्य;

/* Two states are equal if they have the same core items.  */
अटल अंतरभूत bool
state_compare (state स्थिर *s1, state स्थिर *s2)
{
  अगर (s1->nitems != s2->nitems)
    वापस false;

  क्रम (माप_प्रकार i = 0; i < s1->nitems; ++i)
    अगर (s1->वस्तुएँ[i] != s2->वस्तुएँ[i])
      वापस false;

  वापस true;
}

अटल bool
state_comparator (व्योम स्थिर *s1, व्योम स्थिर *s2)
{
  वापस state_compare (s1, s2);
}

अटल अंतरभूत माप_प्रकार
state_hash (state स्थिर *s, माप_प्रकार tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  माप_प्रकार कुंजी = 0;
  क्रम (माप_प्रकार i = 0; i < s->nitems; ++i)
    कुंजी += s->वस्तुएँ[i];
  वापस कुंजी % tablesize;
}

अटल माप_प्रकार
state_hasher (व्योम स्थिर *s, माप_प्रकार tablesize)
{
  वापस state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

व्योम
state_hash_new (व्योम)
{
  state_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                  शून्य,
                                  state_hasher,
                                  state_comparator,
                                  शून्य);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

व्योम
state_hash_free (व्योम)
{
  hash_free (state_table);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

व्योम
state_hash_जोड़ो (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (माप_प्रकार nitems, स्थिर item_सूचकांक *core)
{
  माप_प्रकार वस्तुएँ_आकार = nitems * माप *core;
  state *probe = xmalloc (दुरत्व (state, वस्तुएँ) + वस्तुएँ_आकार);
  probe->nitems = nitems;
  स_नकल (probe->वस्तुएँ, core, वस्तुएँ_आकार);
  state *entry = hash_lookup (state_table, probe);
  मुक्त (probe);
  वापस entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

अटल व्योम
state_record_reachable_states (state *s, bitset reachable)
{
  अगर (bitset_test (reachable, s->संख्या))
    वापस;
  bitset_set (reachable, s->संख्या);
  क्रम (पूर्णांक i = 0; i < s->transitions->अंक; ++i)
    अगर (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

व्योम
state_remove_unreachable_states (state_संख्या old_to_new[])
{
  state_संख्या nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    {
      अगर (bitset_test (reachable, states[i]->संख्या))
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
state **states = शून्य;


/*----------------------.
| Free all the states.  |
`----------------------*/

व्योम
states_free (व्योम)
{
  closure_free ();
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    state_free (states[i]);
  मुक्त (states);
}
