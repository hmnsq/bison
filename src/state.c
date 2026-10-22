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
transitions_new (ಪೂರ್ಣಾಂಕ ಅಂಕೆ, state **dst)
{
  size_t ಸ್ಥಿತಿಗಳು_ಗಾತ್ರ = ಅಂಕೆ * ಗಾತ್ರ *dst;
  transitions *res = xmalloc (offsetof (transitions, ಸ್ಥಿತಿಗಳು) + ಸ್ಥಿತಿಗಳು_ಗಾತ್ರ);
  res->ಅಂಕೆ = ಅಂಕೆ;
  ಸ್ಮರಣೆ_ನಕಲು (res->ಸ್ಥಿತಿಗಳು, dst, ಸ್ಥಿತಿಗಳು_ಗಾತ್ರ);
  ಹಿಂತಿರುಗಿ res;
}


state *
transitions_ಗಮ್ಯ (state *s, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  transitions *trans = s->transitions;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < trans->ಅಂಕೆ; ++i)
    ಆದರೆ (TRANSITION_ಚಿಹ್ನೆ (trans, i) == sym)
      ಹಿಂತಿರುಗಿ trans->ಸ್ಥಿತಿಗಳು[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ಚಿಹ್ನೆ **ಟೋಕನುಗಳು)
{
  size_t symbols_ಗಾತ್ರ = ಅಂಕೆ * ಗಾತ್ರ *ಟೋಕನುಗಳು;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_ಗಾತ್ರ);
  res->ಅಂಕೆ = ಅಂಕೆ;
  ಆದರೆ (ಟೋಕನುಗಳು)
    ಸ್ಮರಣೆ_ನಕಲು (res->symbols, ಟೋಕನುಗಳು, symbols_ಗಾತ್ರ);
  ಹಿಂತಿರುಗಿ res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ಸ್ಥಿರ reductions *
reductions_new (ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ನಿಯಮ **reds)
{
  size_t ನಿಯಮಗಳು_ಗಾತ್ರ = ಅಂಕೆ * ಗಾತ್ರ *reds;
  reductions *res = xmalloc (offsetof (reductions, ನಿಯಮಗಳು) + ನಿಯಮಗಳು_ಗಾತ್ರ);
  res->ಅಂಕೆ = ಅಂಕೆ;
  res->lookaheads = ಖಾಲಿ;
  ಸ್ಮರಣೆ_ನಕಲು (res->ನಿಯಮಗಳು, reds, ನಿಯಮಗಳು_ಗಾತ್ರ);
  ಹಿಂತಿರುಗಿ res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_ಸಂಖ್ಯೆ nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = ಖಾಲಿ;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ accessing_ಚಿಹ್ನೆ,
           size_t nitems, ಅಂಶ_ಸೂಚ್ಯಂಕ *core)
{
  aver (nstates < STATE_ಸಂಖ್ಯೆ_ಗರಿಷ್ಠ);

  size_t ವಸ್ತುಗಳು_ಗಾತ್ರ = nitems * ಗಾತ್ರ *core;
  state *res = xmalloc (offsetof (state, ವಸ್ತುಗಳು) + ವಸ್ತುಗಳು_ಗಾತ್ರ);
  res->ಸಂಖ್ಯೆ = nstates++;
  res->accessing_ಚಿಹ್ನೆ = accessing_ಚಿಹ್ನೆ;
  res->transitions = ಖಾಲಿ;
  res->reductions = ಖಾಲಿ;
  res->errs = ಖಾಲಿ;
  res->state_list = ಖಾಲಿ;
  res->consistent = false;
  res->solved_conflicts = ಖಾಲಿ;
  res->solved_conflicts_xml = ಖಾಲಿ;

  res->nitems = nitems;
  ಸ್ಮರಣೆ_ನಕಲು (res->ವಸ್ತುಗಳು, core, ವಸ್ತುಗಳು_ಗಾತ್ರ);

  state_hash_ಸೇರಿಸು (res);

  ಹಿಂತಿರುಗಿ res;
}

state *
state_new_isocore (state ಸ್ಥಿರಾಂಕ *s)
{
  aver (nstates < STATE_ಸಂಖ್ಯೆ_ಗರಿಷ್ಠ);

  size_t ವಸ್ತುಗಳು_ಗಾತ್ರ = s->nitems * ಗಾತ್ರ *s->ವಸ್ತುಗಳು;
  state *res = xmalloc (offsetof (state, ವಸ್ತುಗಳು) + ವಸ್ತುಗಳು_ಗಾತ್ರ);
  res->ಸಂಖ್ಯೆ = nstates++;
  res->accessing_ಚಿಹ್ನೆ = s->accessing_ಚಿಹ್ನೆ;
  res->transitions =
    transitions_new (s->transitions->ಅಂಕೆ, s->transitions->ಸ್ಥಿತಿಗಳು);
  res->reductions = reductions_new (s->reductions->ಅಂಕೆ, s->reductions->ನಿಯಮಗಳು);
  res->errs = ಖಾಲಿ;
  res->state_list = ಖಾಲಿ;
  res->consistent = s->consistent;
  res->solved_conflicts = ಖಾಲಿ;
  res->solved_conflicts_xml = ಖಾಲಿ;

  res->nitems = s->nitems;
  ಸ್ಮರಣೆ_ನಕಲು (res->ವಸ್ತುಗಳು, s->ವಸ್ತುಗಳು, ವಸ್ತುಗಳು_ಗಾತ್ರ);

  ಹಿಂತಿರುಗಿ res;
}


/*---------.
| Free S.  |
`---------*/

ಸ್ಥಿರ ಶೂನ್ಯ
state_free (state *s)
{
  ಬಿಡುಗಡೆ (s->transitions);
  ಬಿಡುಗಡೆ (s->reductions);
  ಬಿಡುಗಡೆ (s->errs);
  ಬಿಡುಗಡೆ (s);
}


ಶೂನ್ಯ
state_transitions_print (ಸ್ಥಿರಾಂಕ state *s, FILE *ಹೊರಹೋಗು)
{
  ಸ್ಥಿರಾಂಕ transitions *trans = s->transitions;
  fprintf (ಹೊರಹೋಗು, "transitions of %d (%d):\n",
           s->ಸಂಖ್ಯೆ, trans->ಅಂಕೆ);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < trans->ಅಂಕೆ; ++i)
    fprintf (ಹೊರಹೋಗು, "  %d: (%d, %s, %d)\n",
             i,
             s->ಸಂಖ್ಯೆ,
             symbols[s->transitions->ಸ್ಥಿತಿಗಳು[i]->accessing_ಚಿಹ್ನೆ]->ಟ್ಯಾಗ್,
             s->transitions->ಸ್ಥಿತಿಗಳು[i]->ಸಂಖ್ಯೆ);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

ಶೂನ್ಯ
state_transitions_ಗಣ (state *s, ಪೂರ್ಣಾಂಕ ಅಂಕೆ, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (ಅಂಕೆ, dst);
  ಆದರೆ (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

ಶೂನ್ಯ
state_reductions_ಗಣ (state *s, ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ನಿಯಮ **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (ಅಂಕೆ, reds);
}


ಪೂರ್ಣಾಂಕ
state_reduction_ಕಂಡುಹಿಡಿ (state ಸ್ಥಿರಾಂಕ *s, ನಿಯಮ ಸ್ಥಿರಾಂಕ *r)
{
  reductions *reds = s->reductions;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಆದರೆ (reds->ನಿಯಮಗಳು[i] == r)
      ಹಿಂತಿರುಗಿ i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

ಶೂನ್ಯ
state_errs_ಗಣ (state *s, ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ಚಿಹ್ನೆ **ಟೋಕನುಗಳು)
{
  aver (!s->errs);
  s->errs = errs_new (ಅಂಕೆ, ಟೋಕನುಗಳು);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

ಶೂನ್ಯ
state_ನಿಯಮ_lookaheads_print (state ಸ್ಥಿರಾಂಕ *s, ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, FILE *ಹೊರಹೋಗು)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  ಪೂರ್ಣಾಂಕ red = state_reduction_ಕಂಡುಹಿಡಿ (s, r);

  /* Print them if there are.  */
  ಆದರೆ (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      ಪೂರ್ಣಾಂಕ k;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *sep = "";
      fprintf (ಹೊರಹೋಗು, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (ಹೊರಹೋಗು, "%s%s", sep, symbols[k]->ಟ್ಯಾಗ್);
          sep = ", ";
        }
      fprintf (ಹೊರಹೋಗು, "]");
    }
}

ಶೂನ್ಯ
state_ನಿಯಮ_lookaheads_print_xml (state ಸ್ಥಿರಾಂಕ *s, ನಿಯಮ ಸ್ಥಿರಾಂಕ *r,
                                       FILE *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  ಪೂರ್ಣಾಂಕ red = state_reduction_ಕಂಡುಹಿಡಿ (s, r);

  /* Print them if there are.  */
  ಆದರೆ (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      ಪೂರ್ಣಾಂಕ k;
      xml_puts (ಹೊರಹೋಗು, level, "<lookaheads>");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          xml_printf (ಹೊರಹೋಗು, level + 1, "<symbol>%s</symbol>",
                      xml_escape (symbols[k]->ಟ್ಯಾಗ್));
        }
      xml_puts (ಹೊರಹೋಗು, level, "</lookaheads>");
    }
}


/*---------------------.
| A state hash table.  |
`---------------------*/

/* Initial capacity of states hash table.  */
#ವ್ಯಾಖ್ಯಾನಿಸು HT_INITIAL_ಸಾಮರ್ಥ್ಯ 257

ಸ್ಥಿರ ರಚನೆ hash_ಕೋಷ್ಟಕ *state_ಕೋಷ್ಟಕ = ಖಾಲಿ;

/* Two states are equal if they have the same core items.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
state_compare (state ಸ್ಥಿರಾಂಕ *s1, state ಸ್ಥಿರಾಂಕ *s2)
{
  ಆದರೆ (s1->nitems != s2->nitems)
    ಹಿಂತಿರುಗಿ false;

  ಗಾಗಿ (size_t i = 0; i < s1->nitems; ++i)
    ಆದರೆ (s1->ವಸ್ತುಗಳು[i] != s2->ವಸ್ತುಗಳು[i])
      ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
state_comparator (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *s1, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *s2)
{
  ಹಿಂತಿರುಗಿ state_compare (s1, s2);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t
state_hash (state ಸ್ಥಿರಾಂಕ *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t ಕೀಲಿ = 0;
  ಗಾಗಿ (size_t i = 0; i < s->nitems; ++i)
    ಕೀಲಿ += s->ವಸ್ತುಗಳು[i];
  ಹಿಂತಿರುಗಿ ಕೀಲಿ % tablesize;
}

ಸ್ಥಿರ size_t
state_hasher (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *s, size_t tablesize)
{
  ಹಿಂತಿರುಗಿ state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

ಶೂನ್ಯ
state_hash_new (ಶೂನ್ಯ)
{
  state_ಕೋಷ್ಟಕ = hash_xinitialize (HT_INITIAL_ಸಾಮರ್ಥ್ಯ,
                                  ಖಾಲಿ,
                                  state_hasher,
                                  state_comparator,
                                  ಖಾಲಿ);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

ಶೂನ್ಯ
state_hash_free (ಶೂನ್ಯ)
{
  hash_free (state_ಕೋಷ್ಟಕ);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

ಶೂನ್ಯ
state_hash_ಸೇರಿಸು (state *s)
{
  hash_xinsert (state_ಕೋಷ್ಟಕ, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸೂಚ್ಯಂಕ *core)
{
  size_t ವಸ್ತುಗಳು_ಗಾತ್ರ = nitems * ಗಾತ್ರ *core;
  state *probe = xmalloc (offsetof (state, ವಸ್ತುಗಳು) + ವಸ್ತುಗಳು_ಗಾತ್ರ);
  probe->nitems = nitems;
  ಸ್ಮರಣೆ_ನಕಲು (probe->ವಸ್ತುಗಳು, core, ವಸ್ತುಗಳು_ಗಾತ್ರ);
  state *ನಮೂದು = hash_lookup (state_ಕೋಷ್ಟಕ, probe);
  ಬಿಡುಗಡೆ (probe);
  ಹಿಂತಿರುಗಿ ನಮೂದು;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
state_ದಾಖಲೆ_reachable_ಸ್ಥಿತಿಗಳು (state *s, bitset reachable)
{
  ಆದರೆ (bitset_test (reachable, s->ಸಂಖ್ಯೆ))
    ಹಿಂತಿರುಗಿ;
  bitset_ಗಣ (reachable, s->ಸಂಖ್ಯೆ);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < s->transitions->ಅಂಕೆ; ++i)
    ಆದರೆ (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_ದಾಖಲೆ_reachable_ಸ್ಥಿತಿಗಳು (s->transitions->ಸ್ಥಿತಿಗಳು[i], reachable);
}

ಶೂನ್ಯ
state_remove_unreachable_ಸ್ಥಿತಿಗಳು (state_ಸಂಖ್ಯೆ old_ಗಮ್ಯ_new[])
{
  state_ಸಂಖ್ಯೆ nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_ದಾಖಲೆ_reachable_ಸ್ಥಿತಿಗಳು (ಸ್ಥಿತಿಗಳು[0], reachable);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಆದರೆ (bitset_test (reachable, ಸ್ಥಿತಿಗಳು[i]->ಸಂಖ್ಯೆ))
        {
          ಸ್ಥಿತಿಗಳು[nstates_reachable] = ಸ್ಥಿತಿಗಳು[i];
          ಸ್ಥಿತಿಗಳು[nstates_reachable]->ಸಂಖ್ಯೆ = nstates_reachable;
          old_ಗಮ್ಯ_new[i] = nstates_reachable++;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          state_free (ಸ್ಥಿತಿಗಳು[i]);
          old_ಗಮ್ಯ_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **ಸ್ಥಿತಿಗಳು = ಖಾಲಿ;


/*----------------------.
| Free all the states.  |
`----------------------*/

ಶೂನ್ಯ
ಸ್ಥಿತಿಗಳು_free (ಶೂನ್ಯ)
{
  ಆವರಣ_free ();
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    state_free (ಸ್ಥಿತಿಗಳು[i]);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿಗಳು);
}
