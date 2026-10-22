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
transitions_new (पूर्णाङ्क अङ्कः, state **dst)
{
  size_t अवस्थाः_आकार = अङ्कः * परिमाण *dst;
  transitions *res = xmalloc (offsetof (transitions, अवस्थाः) + अवस्थाः_आकार);
  res->अङ्कः = अङ्कः;
  स्मृतिं_प्रतिलिख (res->अवस्थाः, dst, अवस्थाः_आकार);
  प्रत्यागम res;
}


state *
transitions_गन्तव्यम् (state *s, चिह्नम्_सङ्ख्या sym)
{
  transitions *trans = s->transitions;
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; ++i)
    यदि (TRANSITION_चिह्नम् (trans, i) == sym)
      प्रत्यागम trans->अवस्थाः[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (पूर्णाङ्क अङ्कः, चिह्नम् **tokens)
{
  size_t symbols_आकार = अङ्कः * परिमाण *tokens;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_आकार);
  res->अङ्कः = अङ्कः;
  यदि (tokens)
    स्मृतिं_प्रतिलिख (res->symbols, tokens, symbols_आकार);
  प्रत्यागम res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

स्थिर reductions *
reductions_new (पूर्णाङ्क अङ्कः, नियमः **reds)
{
  size_t नियमाः_आकार = अङ्कः * परिमाण *reds;
  reductions *res = xmalloc (offsetof (reductions, नियमाः) + नियमाः_आकार);
  res->अङ्कः = अङ्कः;
  res->lookaheads = रिक्त;
  स्मृतिं_प्रतिलिख (res->नियमाः, reds, नियमाः_आकार);
  प्रत्यागम res;
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
state_new (चिह्नम्_सङ्ख्या accessing_चिह्नम्,
           size_t nitems, item_सूचकांक *core)
{
  aver (nstates < STATE_सङ्ख्या_अधिकतमम्);

  size_t वस्तूनि_आकार = nitems * परिमाण *core;
  state *res = xmalloc (offsetof (state, वस्तूनि) + वस्तूनि_आकार);
  res->सङ्ख्या = nstates++;
  res->accessing_चिह्नम् = accessing_चिह्नम्;
  res->transitions = रिक्त;
  res->reductions = रिक्त;
  res->errs = रिक्त;
  res->state_list = रिक्त;
  res->consistent = false;
  res->solved_conflicts = रिक्त;
  res->solved_conflicts_xml = रिक्त;

  res->nitems = nitems;
  स्मृतिं_प्रतिलिख (res->वस्तूनि, core, वस्तूनि_आकार);

  state_hash_निवेशय (res);

  प्रत्यागम res;
}

state *
state_new_isocore (state ध्रुव *s)
{
  aver (nstates < STATE_सङ्ख्या_अधिकतमम्);

  size_t वस्तूनि_आकार = s->nitems * परिमाण *s->वस्तूनि;
  state *res = xmalloc (offsetof (state, वस्तूनि) + वस्तूनि_आकार);
  res->सङ्ख्या = nstates++;
  res->accessing_चिह्नम् = s->accessing_चिह्नम्;
  res->transitions =
    transitions_new (s->transitions->अङ्कः, s->transitions->अवस्थाः);
  res->reductions = reductions_new (s->reductions->अङ्कः, s->reductions->नियमाः);
  res->errs = रिक्त;
  res->state_list = रिक्त;
  res->consistent = s->consistent;
  res->solved_conflicts = रिक्त;
  res->solved_conflicts_xml = रिक्त;

  res->nitems = s->nitems;
  स्मृतिं_प्रतिलिख (res->वस्तूनि, s->वस्तूनि, वस्तूनि_आकार);

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
state_transitions_print (ध्रुव state *s, FILE *बहिर्गमनम्)
{
  ध्रुव transitions *trans = s->transitions;
  fprintf (बहिर्गमनम्, "transitions of %d (%d):\n",
           s->सङ्ख्या, trans->अङ्कः);
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; ++i)
    fprintf (बहिर्गमनम्, "  %d: (%d, %s, %d)\n",
             i,
             s->सङ्ख्या,
             symbols[s->transitions->अवस्थाः[i]->accessing_चिह्नम्]->चिह्नकम्,
             s->transitions->अवस्थाः[i]->सङ्ख्या);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

शून्य
state_transitions_समुच्चयः (state *s, पूर्णाङ्क अङ्कः, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (अङ्कः, dst);
  यदि (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

शून्य
state_reductions_समुच्चयः (state *s, पूर्णाङ्क अङ्कः, नियमः **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (अङ्कः, reds);
}


पूर्णाङ्क
state_reduction_find (state ध्रुव *s, नियमः ध्रुव *r)
{
  reductions *reds = s->reductions;
  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    यदि (reds->नियमाः[i] == r)
      प्रत्यागम i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

शून्य
state_errs_समुच्चयः (state *s, पूर्णाङ्क अङ्कः, चिह्नम् **tokens)
{
  aver (!s->errs);
  s->errs = errs_new (अङ्कः, tokens);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

शून्य
state_नियमः_lookaheads_print (state ध्रुव *s, नियमः ध्रुव *r, FILE *बहिर्गमनम्)
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
      fprintf (बहिर्गमनम्, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (बहिर्गमनम्, "%s%s", sep, symbols[k]->चिह्नकम्);
          sep = ", ";
        }
      fprintf (बहिर्गमनम्, "]");
    }
}

शून्य
state_नियमः_lookaheads_print_xml (state ध्रुव *s, नियमः ध्रुव *r,
                                       FILE *बहिर्गमनम्, पूर्णाङ्क level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  पूर्णाङ्क red = state_reduction_find (s, r);

  /* Print them if there are.  */
  यदि (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      पूर्णाङ्क k;
      xml_puts (बहिर्गमनम्, level, "<lookaheads>");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          xml_printf (बहिर्गमनम्, level + 1, "<symbol>%s</symbol>",
                      xml_escape (symbols[k]->चिह्नकम्));
        }
      xml_puts (बहिर्गमनम्, level, "</lookaheads>");
    }
}


/*---------------------.
| A state hash table.  |
`---------------------*/

/* Initial capacity of states hash table.  */
#परिभाषय HT_INITIAL_क्षमता 257

स्थिर संरचना hash_सारणी *state_सारणी = रिक्त;

/* Two states are equal if they have the same core items.  */
स्थिर अन्तःस्थ bool
state_compare (state ध्रुव *s1, state ध्रुव *s2)
{
  यदि (s1->nitems != s2->nitems)
    प्रत्यागम false;

  प्रति (size_t i = 0; i < s1->nitems; ++i)
    यदि (s1->वस्तूनि[i] != s2->वस्तूनि[i])
      प्रत्यागम false;

  प्रत्यागम true;
}

स्थिर bool
state_comparator (शून्य ध्रुव *s1, शून्य ध्रुव *s2)
{
  प्रत्यागम state_compare (s1, s2);
}

स्थिर अन्तःस्थ size_t
state_hash (state ध्रुव *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t कुञ्चिका = 0;
  प्रति (size_t i = 0; i < s->nitems; ++i)
    कुञ्चिका += s->वस्तूनि[i];
  प्रत्यागम कुञ्चिका % tablesize;
}

स्थिर size_t
state_hasher (शून्य ध्रुव *s, size_t tablesize)
{
  प्रत्यागम state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

शून्य
state_hash_new (शून्य)
{
  state_सारणी = hash_xinitialize (HT_INITIAL_क्षमता,
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
  hash_free (state_सारणी);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

शून्य
state_hash_निवेशय (state *s)
{
  hash_xinsert (state_सारणी, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ध्रुव item_सूचकांक *core)
{
  size_t वस्तूनि_आकार = nitems * परिमाण *core;
  state *probe = xmalloc (offsetof (state, वस्तूनि) + वस्तूनि_आकार);
  probe->nitems = nitems;
  स्मृतिं_प्रतिलिख (probe->वस्तूनि, core, वस्तूनि_आकार);
  state *प्रविष्टिः = hash_lookup (state_सारणी, probe);
  मुक्त (probe);
  प्रत्यागम प्रविष्टिः;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

स्थिर शून्य
state_लेखः_reachable_अवस्थाः (state *s, bitset reachable)
{
  यदि (bitset_test (reachable, s->सङ्ख्या))
    प्रत्यागम;
  bitset_समुच्चयः (reachable, s->सङ्ख्या);
  प्रति (पूर्णाङ्क i = 0; i < s->transitions->अङ्कः; ++i)
    यदि (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_लेखः_reachable_अवस्थाः (s->transitions->अवस्थाः[i], reachable);
}

शून्य
state_remove_unreachable_अवस्थाः (state_सङ्ख्या old_गन्तव्यम्_new[])
{
  state_सङ्ख्या nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_लेखः_reachable_अवस्थाः (अवस्थाः[0], reachable);
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      यदि (bitset_test (reachable, अवस्थाः[i]->सङ्ख्या))
        {
          अवस्थाः[nstates_reachable] = अवस्थाः[i];
          अवस्थाः[nstates_reachable]->सङ्ख्या = nstates_reachable;
          old_गन्तव्यम्_new[i] = nstates_reachable++;
        }
      अन्यथा
        {
          state_free (अवस्थाः[i]);
          old_गन्तव्यम्_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **अवस्थाः = रिक्त;


/*----------------------.
| Free all the states.  |
`----------------------*/

शून्य
अवस्थाः_free (शून्य)
{
  संवृतिः_free ();
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    state_free (अवस्थाः[i]);
  मुक्त (अवस्थाः);
}
