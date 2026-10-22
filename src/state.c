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
  size_t حالتونه_اندازه = عدد * حجم *dst;
  transitions *res = xmalloc (offsetof (transitions, حالتونه) + حالتونه_اندازه);
  res->عدد = عدد;
  حافظه_نقل (res->حالتونه, dst, حالتونه_اندازه);
  أرجع res;
}


state *
transitions_منزل (state *s, نښه_شمېره sym)
{
  transitions *trans = s->transitions;
  لكل (صحيح i = 0; i < trans->عدد; ++i)
    إذا (TRANSITION_نښه (trans, i) == sym)
      أرجع trans->حالتونه[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (صحيح عدد, نښه **نښې)
{
  size_t symbols_اندازه = عدد * حجم *نښې;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_اندازه);
  res->عدد = عدد;
  إذا (نښې)
    حافظه_نقل (res->symbols, نښې, symbols_اندازه);
  أرجع res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ساكن reductions *
reductions_new (صحيح عدد, قاعده **reds)
{
  size_t قواعد_اندازه = عدد * حجم *reds;
  reductions *res = xmalloc (offsetof (reductions, قواعد) + قواعد_اندازه);
  res->عدد = عدد;
  res->lookaheads = عدم;
  حافظه_نقل (res->قواعد, reds, قواعد_اندازه);
  أرجع res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_شمېره nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = عدم;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (نښه_شمېره accessing_نښه,
           size_t nitems, توکی_شاخص *core)
{
  aver (nstates < STATE_شمېره_اعظمي);

  size_t توکي_اندازه = nitems * حجم *core;
  state *res = xmalloc (offsetof (state, توکي) + توکي_اندازه);
  res->شمېره = nstates++;
  res->accessing_نښه = accessing_نښه;
  res->transitions = عدم;
  res->reductions = عدم;
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = false;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = nitems;
  حافظه_نقل (res->توکي, core, توکي_اندازه);

  state_hash_ورننباسه (res);

  أرجع res;
}

state *
state_new_isocore (state ثابت *s)
{
  aver (nstates < STATE_شمېره_اعظمي);

  size_t توکي_اندازه = s->nitems * حجم *s->توکي;
  state *res = xmalloc (offsetof (state, توکي) + توکي_اندازه);
  res->شمېره = nstates++;
  res->accessing_نښه = s->accessing_نښه;
  res->transitions =
    transitions_new (s->transitions->عدد, s->transitions->حالتونه);
  res->reductions = reductions_new (s->reductions->عدد, s->reductions->قواعد);
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = s->consistent;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = s->nitems;
  حافظه_نقل (res->توکي, s->توکي, توکي_اندازه);

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
state_transitions_print (ثابت state *s, FILE *بهر)
{
  ثابت transitions *trans = s->transitions;
  fprintf (بهر, "transitions of %d (%d):\n",
           s->شمېره, trans->عدد);
  لكل (صحيح i = 0; i < trans->عدد; ++i)
    fprintf (بهر, "  %d: (%d, %s, %d)\n",
             i,
             s->شمېره,
             symbols[s->transitions->حالتونه[i]->accessing_نښه]->نښان,
             s->transitions->حالتونه[i]->شمېره);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

فراغ
state_transitions_سېټ (state *s, صحيح عدد, state **dst)
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
state_reductions_سېټ (state *s, صحيح عدد, قاعده **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (عدد, reds);
}


صحيح
state_reduction_پیدا_کړه (state ثابت *s, قاعده ثابت *r)
{
  reductions *reds = s->reductions;
  لكل (صحيح i = 0; i < reds->عدد; ++i)
    إذا (reds->قواعد[i] == r)
      أرجع i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

فراغ
state_errs_سېټ (state *s, صحيح عدد, نښه **نښې)
{
  aver (!s->errs);
  s->errs = errs_new (عدد, نښې);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

فراغ
state_قاعده_lookaheads_print (state ثابت *s, قاعده ثابت *r, FILE *بهر)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_پیدا_کړه (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      حرف ثابت *sep = "";
      fprintf (بهر, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (بهر, "%s%s", sep, symbols[k]->نښان);
          sep = ", ";
        }
      fprintf (بهر, "]");
    }
}

فراغ
state_قاعده_lookaheads_print_xml (state ثابت *s, قاعده ثابت *r,
                                       FILE *بهر, صحيح level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_پیدا_کړه (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      xml_puts (بهر, level, "<lookaheads>");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          xml_printf (بهر, level + 1, "<symbol>%s</symbol>",
                      xml_escape (symbols[k]->نښان));
        }
      xml_puts (بهر, level, "</lookaheads>");
    }
}


/*---------------------.
| A state hash table.  |
`---------------------*/

/* Initial capacity of states hash table.  */
#تعریف_کړه HT_INITIAL_ظرفیت 257

ساكن بنية hash_جدول *state_جدول = عدم;

/* Two states are equal if they have the same core items.  */
ساكن مضمن bool
state_compare (state ثابت *s1, state ثابت *s2)
{
  إذا (s1->nitems != s2->nitems)
    أرجع false;

  لكل (size_t i = 0; i < s1->nitems; ++i)
    إذا (s1->توکي[i] != s2->توکي[i])
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
  size_t کلی = 0;
  لكل (size_t i = 0; i < s->nitems; ++i)
    کلی += s->توکي[i];
  أرجع کلی % tablesize;
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
  state_جدول = hash_xinitialize (HT_INITIAL_ظرفیت,
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
state_hash_ورننباسه (state *s)
{
  hash_xinsert (state_جدول, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ثابت توکی_شاخص *core)
{
  size_t توکي_اندازه = nitems * حجم *core;
  state *probe = xmalloc (offsetof (state, توکي) + توکي_اندازه);
  probe->nitems = nitems;
  حافظه_نقل (probe->توکي, core, توکي_اندازه);
  state *ثبت = hash_lookup (state_جدول, probe);
  حرر (probe);
  أرجع ثبت;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ساكن فراغ
state_ریکارډ_reachable_حالتونه (state *s, bitset reachable)
{
  إذا (bitset_test (reachable, s->شمېره))
    أرجع;
  bitset_سېټ (reachable, s->شمېره);
  لكل (صحيح i = 0; i < s->transitions->عدد; ++i)
    إذا (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_ریکارډ_reachable_حالتونه (s->transitions->حالتونه[i], reachable);
}

فراغ
state_remove_unreachable_حالتونه (state_شمېره old_منزل_new[])
{
  state_شمېره nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_ریکارډ_reachable_حالتونه (حالتونه[0], reachable);
  لكل (state_شمېره i = 0; i < nstates; ++i)
    {
      إذا (bitset_test (reachable, حالتونه[i]->شمېره))
        {
          حالتونه[nstates_reachable] = حالتونه[i];
          حالتونه[nstates_reachable]->شمېره = nstates_reachable;
          old_منزل_new[i] = nstates_reachable++;
        }
      وإلا
        {
          state_free (حالتونه[i]);
          old_منزل_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **حالتونه = عدم;


/*----------------------.
| Free all the states.  |
`----------------------*/

فراغ
حالتونه_free (فراغ)
{
  تړون_free ();
  لكل (state_شمېره i = 0; i < nstates; ++i)
    state_free (حالتونه[i]);
  حرر (حالتونه);
}
