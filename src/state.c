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
transitions_new (पूर्णाङ्क अङ्क, state **dst)
{
  माप_प्रकार states_size = अङ्क * परिमाण *dst;
  transitions *res = xmalloc (दुरत्व (transitions, states) + states_size);
  res->अङ्क = अङ्क;
  स_नकल (res->states, dst, states_size);
  फर्काऊ res;
}


state *
transitions_to (state *s, चिह्न_सङ्ख्या sym)
{
  transitions *trans = s->transitions;
  लागि (पूर्णाङ्क i = 0; i < trans->अङ्क; ++i)
    यदि (TRANSITION_चिह्न (trans, i) == sym)
      फर्काऊ trans->states[i];
  पात ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (पूर्णाङ्क अङ्क, चिह्न **tokens)
{
  माप_प्रकार symbols_size = अङ्क * परिमाण *tokens;
  errs *res = xmalloc (दुरत्व (errs, symbols) + symbols_size);
  res->अङ्क = अङ्क;
  यदि (tokens)
    स_नकल (res->symbols, tokens, symbols_size);
  फर्काऊ res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

स्थिर reductions *
reductions_new (पूर्णाङ्क अङ्क, rule **reds)
{
  माप_प्रकार rules_size = अङ्क * परिमाण *reds;
  reductions *res = xmalloc (दुरत्व (reductions, rules) + rules_size);
  res->अङ्क = अङ्क;
  res->lookaheads = रिक्त;
  स_नकल (res->rules, reds, rules_size);
  फर्काऊ res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_सङ्ख्या nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = रिक्त;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (चिह्न_सङ्ख्या accessing_चिह्न,
           माप_प्रकार nitems, item_index *core)
{
  aver (nstates < STATE_सङ्ख्या_MAXIMUM);

  माप_प्रकार वस्तुहरू_size = nitems * परिमाण *core;
  state *res = xmalloc (दुरत्व (state, वस्तुहरू) + वस्तुहरू_size);
  res->सङ्ख्या = nstates++;
  res->accessing_चिह्न = accessing_चिह्न;
  res->transitions = रिक्त;
  res->reductions = रिक्त;
  res->errs = रिक्त;
  res->state_list = रिक्त;
  res->consistent = false;
  res->solved_conflicts = रिक्त;
  res->solved_conflicts_xml = रिक्त;

  res->nitems = nitems;
  स_नकल (res->वस्तुहरू, core, वस्तुहरू_size);

  state_hash_घुसाउ (res);

  फर्काऊ res;
}

state *
state_new_isocore (state अचर *s)
{
  aver (nstates < STATE_सङ्ख्या_MAXIMUM);

  माप_प्रकार वस्तुहरू_size = s->nitems * परिमाण *s->वस्तुहरू;
  state *res = xmalloc (दुरत्व (state, वस्तुहरू) + वस्तुहरू_size);
  res->सङ्ख्या = nstates++;
  res->accessing_चिह्न = s->accessing_चिह्न;
  res->transitions =
    transitions_new (s->transitions->अङ्क, s->transitions->states);
  res->reductions = reductions_new (s->reductions->अङ्क, s->reductions->rules);
  res->errs = रिक्त;
  res->state_list = रिक्त;
  res->consistent = s->consistent;
  res->solved_conflicts = रिक्त;
  res->solved_conflicts_xml = रिक्त;

  res->nitems = s->nitems;
  स_नकल (res->वस्तुहरू, s->वस्तुहरू, वस्तुहरू_size);

  फर्काऊ res;
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
state_transitions_print (अचर state *s, खाता *out)
{
  अचर transitions *trans = s->transitions;
  ख_लिखो (out, "transitions of %d (%d):\n",
           s->सङ्ख्या, trans->अङ्क);
  लागि (पूर्णाङ्क i = 0; i < trans->अङ्क; ++i)
    ख_लिखो (out, "  %d: (%d, %s, %d)\n",
             i,
             s->सङ्ख्या,
             symbols[s->transitions->states[i]->accessing_चिह्न]->tag,
             s->transitions->states[i]->सङ्ख्या);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

शून्य
state_transitions_set (state *s, पूर्णाङ्क अङ्क, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (अङ्क, dst);
  यदि (trace_flag & trace_automaton)
    state_transitions_print (s, मानक_त्रुटि);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

शून्य
state_reductions_set (state *s, पूर्णाङ्क अङ्क, rule **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (अङ्क, reds);
}


पूर्णाङ्क
state_reduction_find (state अचर *s, rule अचर *r)
{
  reductions *reds = s->reductions;
  लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
    यदि (reds->rules[i] == r)
      फर्काऊ i;
  पात ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

शून्य
state_errs_set (state *s, पूर्णाङ्क अङ्क, चिह्न **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (अङ्क, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

शून्य
state_rule_lookaheads_print (state अचर *s, rule अचर *r, खाता *out)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  पूर्णाङ्क red = state_reduction_find (s, r);

  /* Print them if there are.  */
  यदि (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      पूर्णाङ्क k;
      वर्ण अचर *sep = "";
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
state_rule_lookaheads_print_xml (state अचर *s, rule अचर *r,
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
state_compare (state अचर *s1, state अचर *s2)
{
  यदि (s1->nitems != s2->nitems)
    फर्काऊ false;

  लागि (माप_प्रकार i = 0; i < s1->nitems; ++i)
    यदि (s1->वस्तुहरू[i] != s2->वस्तुहरू[i])
      फर्काऊ false;

  फर्काऊ true;
}

स्थिर bool
state_comparator (शून्य अचर *s1, शून्य अचर *s2)
{
  फर्काऊ state_compare (s1, s2);
}

स्थिर अन्तःस्थ माप_प्रकार
state_hash (state अचर *s, माप_प्रकार tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  माप_प्रकार साँचो = 0;
  लागि (माप_प्रकार i = 0; i < s->nitems; ++i)
    साँचो += s->वस्तुहरू[i];
  फर्काऊ साँचो % tablesize;
}

स्थिर माप_प्रकार
state_hasher (शून्य अचर *s, माप_प्रकार tablesize)
{
  फर्काऊ state_hash (s, tablesize);
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
state_hash_घुसाउ (state *s)
{
  hash_xinsert (state_table, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (माप_प्रकार nitems, अचर item_index *core)
{
  माप_प्रकार वस्तुहरू_size = nitems * परिमाण *core;
  state *probe = xmalloc (दुरत्व (state, वस्तुहरू) + वस्तुहरू_size);
  probe->nitems = nitems;
  स_नकल (probe->वस्तुहरू, core, वस्तुहरू_size);
  state *entry = hash_lookup (state_table, probe);
  मुक्त (probe);
  फर्काऊ entry;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

स्थिर शून्य
state_record_reachable_states (state *s, bitset reachable)
{
  यदि (bitset_test (reachable, s->सङ्ख्या))
    फर्काऊ;
  bitset_set (reachable, s->सङ्ख्या);
  लागि (पूर्णाङ्क i = 0; i < s->transitions->अङ्क; ++i)
    यदि (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_record_reachable_states (s->transitions->states[i], reachable);
}

शून्य
state_remove_unreachable_states (state_सङ्ख्या old_to_new[])
{
  state_सङ्ख्या nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_record_reachable_states (states[0], reachable);
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      यदि (bitset_test (reachable, states[i]->सङ्ख्या))
        {
          states[nstates_reachable] = states[i];
          states[nstates_reachable]->सङ्ख्या = nstates_reachable;
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
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    state_free (states[i]);
  मुक्त (states);
}
