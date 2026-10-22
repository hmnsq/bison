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

#ضمن <config.h>
#ضمن "state.h"

#ضمن "system.h"

#ضمن <hash.h>

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "print-xml.h"


                        /*-------------------.
                        | Shifts and Gotos.  |
                        `-------------------*/


/*-----------------------------------------.
| Create a new array of NUM shifts/gotos.  |
`-----------------------------------------*/

ساكن transitions *
transitions_new (صحيح عدد, state **dst)
{
  size_t حالتاں_ناپ = عدد * حجم *dst;
  transitions *res = xmalloc (offsetof (transitions, حالتاں) + حالتاں_ناپ);
  res->عدد = عدد;
  حافظہ_نقل (res->حالتاں, dst, حالتاں_ناپ);
  أرجع res;
}


state *
transitions_منزل (state *s, علامت_نمبر sym)
{
  transitions *trans = s->transitions;
  لكل (صحيح i = 0; i < trans->عدد; ++i)
    إذا (TRANSITION_علامت (trans, i) == sym)
      أرجع trans->حالتاں[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (صحيح عدد, علامت **ٹوکن)
{
  size_t symbols_ناپ = عدد * حجم *ٹوکن;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_ناپ);
  res->عدد = عدد;
  إذا (ٹوکن)
    حافظہ_نقل (res->symbols, ٹوکن, symbols_ناپ);
  أرجع res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ساكن reductions *
reductions_new (صحيح عدد, قاعدہ **reds)
{
  size_t قاعدے_ناپ = عدد * حجم *reds;
  reductions *res = xmalloc (offsetof (reductions, قاعدے) + قاعدے_ناپ);
  res->عدد = عدد;
  res->lookaheads = عدم;
  حافظہ_نقل (res->قاعدے, reds, قاعدے_ناپ);
  أرجع res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_نمبر nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = عدم;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (علامت_نمبر accessing_علامت,
           size_t nitems, شے_اشاریہ *core)
{
  aver (nstates < STATE_نمبر_ودھ_توں_ودھ);

  size_t اشیاء_ناپ = nitems * حجم *core;
  state *res = xmalloc (offsetof (state, اشیاء) + اشیاء_ناپ);
  res->نمبر = nstates++;
  res->accessing_علامت = accessing_علامت;
  res->transitions = عدم;
  res->reductions = عدم;
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = false;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = nitems;
  حافظہ_نقل (res->اشیاء, core, اشیاء_ناپ);

  state_hash_داخل_کرو (res);

  أرجع res;
}

state *
state_new_isocore (state ثابت *s)
{
  aver (nstates < STATE_نمبر_ودھ_توں_ودھ);

  size_t اشیاء_ناپ = s->nitems * حجم *s->اشیاء;
  state *res = xmalloc (offsetof (state, اشیاء) + اشیاء_ناپ);
  res->نمبر = nstates++;
  res->accessing_علامت = s->accessing_علامت;
  res->transitions =
    transitions_new (s->transitions->عدد, s->transitions->حالتاں);
  res->reductions = reductions_new (s->reductions->عدد, s->reductions->قاعدے);
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = s->consistent;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = s->nitems;
  حافظہ_نقل (res->اشیاء, s->اشیاء, اشیاء_ناپ);

  أرجع res;
}


/*---------.
| Free S.  |
`---------*/

ساكن فراغ
state_free (state *s)
{
  حرر (s->transitions);
  حرر (s->reductions);
  حرر (s->errs);
  حرر (s);
}


فراغ
state_transitions_print (ثابت state *s, FILE *باہرلا)
{
  ثابت transitions *trans = s->transitions;
  fprintf (باہرلا, "transitions of %d (%d):\n",
           s->نمبر, trans->عدد);
  لكل (صحيح i = 0; i < trans->عدد; ++i)
    fprintf (باہرلا, "  %d: (%d, %s, %d)\n",
             i,
             s->نمبر,
             symbols[s->transitions->حالتاں[i]->accessing_علامت]->ٹیگ,
             s->transitions->حالتاں[i]->نمبر);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

فراغ
state_transitions_سیٹ (state *s, صحيح عدد, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (عدد, dst);
  إذا (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

فراغ
state_reductions_سیٹ (state *s, صحيح عدد, قاعدہ **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (عدد, reds);
}


صحيح
state_reduction_لبھو (state ثابت *s, قاعدہ ثابت *r)
{
  reductions *reds = s->reductions;
  لكل (صحيح i = 0; i < reds->عدد; ++i)
    إذا (reds->قاعدے[i] == r)
      أرجع i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

فراغ
state_errs_سیٹ (state *s, صحيح عدد, علامت **ٹوکن)
{
  aver (!s->errs);
  s->errs = errs_new (عدد, ٹوکن);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

فراغ
state_قاعدہ_lookaheads_print (state ثابت *s, قاعدہ ثابت *r, FILE *باہرلا)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_لبھو (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      حرف ثابت *sep = "";
      fprintf (باہرلا, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (باہرلا, "%s%s", sep, symbols[k]->ٹیگ);
          sep = ", ";
        }
      fprintf (باہرلا, "]");
    }
}

فراغ
state_قاعدہ_lookaheads_print_xml (state ثابت *s, قاعدہ ثابت *r,
                                       FILE *باہرلا, صحيح level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_لبھو (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      xml_puts (باہرلا, level, "<lookaheads>");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          xml_printf (باہرلا, level + 1, "<symbol>%s</symbol>",
                      xml_escape (symbols[k]->ٹیگ));
        }
      xml_puts (باہرلا, level, "</lookaheads>");
    }
}


/*---------------------.
| A state hash table.  |
`---------------------*/

/* Initial capacity of states hash table.  */
#تعریف_کرو HT_INITIAL_سمرتھا 257

ساكن بنية hash_جدول *state_جدول = عدم;

/* Two states are equal if they have the same core items.  */
ساكن مضمن bool
state_compare (state ثابت *s1, state ثابت *s2)
{
  إذا (s1->nitems != s2->nitems)
    أرجع false;

  لكل (size_t i = 0; i < s1->nitems; ++i)
    إذا (s1->اشیاء[i] != s2->اشیاء[i])
      أرجع false;

  أرجع true;
}

ساكن bool
state_comparator (فراغ ثابت *s1, فراغ ثابت *s2)
{
  أرجع state_compare (s1, s2);
}

ساكن مضمن size_t
state_hash (state ثابت *s, size_t tablesize)
{
  /* Add up the state's item numbers to get a hash key.  */
  size_t کلید = 0;
  لكل (size_t i = 0; i < s->nitems; ++i)
    کلید += s->اشیاء[i];
  أرجع کلید % tablesize;
}

ساكن size_t
state_hasher (فراغ ثابت *s, size_t tablesize)
{
  أرجع state_hash (s, tablesize);
}


/*-------------------------------.
| Create the states hash table.  |
`-------------------------------*/

فراغ
state_hash_new (فراغ)
{
  state_جدول = hash_xinitialize (HT_INITIAL_سمرتھا,
                                  عدم,
                                  state_hasher,
                                  state_comparator,
                                  عدم);
}


/*---------------------------------------------.
| Free the states hash table, not the states.  |
`---------------------------------------------*/

فراغ
state_hash_free (فراغ)
{
  hash_free (state_جدول);
}


/*-----------------------------------.
| Insert S in the state hash table.  |
`-----------------------------------*/

فراغ
state_hash_داخل_کرو (state *s)
{
  hash_xinsert (state_جدول, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ثابت شے_اشاریہ *core)
{
  size_t اشیاء_ناپ = nitems * حجم *core;
  state *probe = xmalloc (offsetof (state, اشیاء) + اشیاء_ناپ);
  probe->nitems = nitems;
  حافظہ_نقل (probe->اشیاء, core, اشیاء_ناپ);
  state *اندراج = hash_lookup (state_جدول, probe);
  حرر (probe);
  أرجع اندراج;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ساكن فراغ
state_ریکارڈ_reachable_حالتاں (state *s, bitset reachable)
{
  إذا (bitset_test (reachable, s->نمبر))
    أرجع;
  bitset_سیٹ (reachable, s->نمبر);
  لكل (صحيح i = 0; i < s->transitions->عدد; ++i)
    إذا (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_ریکارڈ_reachable_حالتاں (s->transitions->حالتاں[i], reachable);
}

فراغ
state_remove_unreachable_حالتاں (state_نمبر old_منزل_new[])
{
  state_نمبر nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_ریکارڈ_reachable_حالتاں (حالتاں[0], reachable);
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      إذا (bitset_test (reachable, حالتاں[i]->نمبر))
        {
          حالتاں[nstates_reachable] = حالتاں[i];
          حالتاں[nstates_reachable]->نمبر = nstates_reachable;
          old_منزل_new[i] = nstates_reachable++;
        }
      وإلا
        {
          state_free (حالتاں[i]);
          old_منزل_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **حالتاں = عدم;


/*----------------------.
| Free all the states.  |
`----------------------*/

فراغ
حالتاں_free (فراغ)
{
  گھیرا_free ();
  لكل (state_نمبر i = 0; i < nstates; ++i)
    state_free (حالتاں[i]);
  حرر (حالتاں);
}
