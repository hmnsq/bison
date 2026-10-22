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
transitions_new (صحيح انگ, state **dst)
{
  size_t حالتون_سائيز = انگ * حجم *dst;
  transitions *res = xmalloc (offsetof (transitions, حالتون) + حالتون_سائيز);
  res->انگ = انگ;
  يادگيري_نقل (res->حالتون, dst, حالتون_سائيز);
  أرجع res;
}


state *
transitions_منزل (state *s, نشان_عدد sym)
{
  transitions *trans = s->transitions;
  لكل (صحيح i = 0; i < trans->انگ; ++i)
    إذا (TRANSITION_نشان (trans, i) == sym)
      أرجع trans->حالتون[i];
  abort ();
}


                        /*--------------------.
                        | Error transitions.  |
                        `--------------------*/


/*---------------------------------.
| Create a new array of NUM errs.  |
`---------------------------------*/

errs *
errs_new (صحيح انگ, نشان **ٽوڪن)
{
  size_t symbols_سائيز = انگ * حجم *ٽوڪن;
  errs *res = xmalloc (offsetof (errs, symbols) + symbols_سائيز);
  res->انگ = انگ;
  إذا (ٽوڪن)
    يادگيري_نقل (res->symbols, ٽوڪن, symbols_سائيز);
  أرجع res;
}




                        /*-------------.
                        | Reductions.  |
                        `-------------*/


/*---------------------------------------.
| Create a new array of NUM reductions.  |
`---------------------------------------*/

ساكن reductions *
reductions_new (صحيح انگ, قاعدو **reds)
{
  size_t قاعدا_سائيز = انگ * حجم *reds;
  reductions *res = xmalloc (offsetof (reductions, قاعدا) + قاعدا_سائيز);
  res->انگ = انگ;
  res->lookaheads = عدم;
  يادگيري_نقل (res->قاعدا, reds, قاعدا_سائيز);
  أرجع res;
}



                        /*---------.
                        | States.  |
                        `---------*/


state_عدد nstates = 0;
/* FINAL_STATE is properly set by new_state when it recognizes its
   accessing symbol: $end.  */
state *final_state = عدم;


/*------------------------------------------------------------------.
| Create a new state with ACCESSING_SYMBOL, for those items.  Store |
| it in the state hash table.                                       |
`------------------------------------------------------------------*/

state *
state_new (نشان_عدد accessing_نشان,
           size_t nitems, شي_اشاريو *core)
{
  aver (nstates < STATE_عدد_وڌ_کان_وڌ);

  size_t شيون_سائيز = nitems * حجم *core;
  state *res = xmalloc (offsetof (state, شيون) + شيون_سائيز);
  res->عدد = nstates++;
  res->accessing_نشان = accessing_نشان;
  res->transitions = عدم;
  res->reductions = عدم;
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = false;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = nitems;
  يادگيري_نقل (res->شيون, core, شيون_سائيز);

  state_hash_وجهو (res);

  أرجع res;
}

state *
state_new_isocore (state ثابت *s)
{
  aver (nstates < STATE_عدد_وڌ_کان_وڌ);

  size_t شيون_سائيز = s->nitems * حجم *s->شيون;
  state *res = xmalloc (offsetof (state, شيون) + شيون_سائيز);
  res->عدد = nstates++;
  res->accessing_نشان = s->accessing_نشان;
  res->transitions =
    transitions_new (s->transitions->انگ, s->transitions->حالتون);
  res->reductions = reductions_new (s->reductions->انگ, s->reductions->قاعدا);
  res->errs = عدم;
  res->state_list = عدم;
  res->consistent = s->consistent;
  res->solved_conflicts = عدم;
  res->solved_conflicts_xml = عدم;

  res->nitems = s->nitems;
  يادگيري_نقل (res->شيون, s->شيون, شيون_سائيز);

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
state_transitions_print (ثابت state *s, FILE *ٻاهريون)
{
  ثابت transitions *trans = s->transitions;
  fprintf (ٻاهريون, "transitions of %d (%d):\n",
           s->عدد, trans->انگ);
  لكل (صحيح i = 0; i < trans->انگ; ++i)
    fprintf (ٻاهريون, "  %d: (%d, %s, %d)\n",
             i,
             s->عدد,
             symbols[s->transitions->حالتون[i]->accessing_نشان]->ٽيگ,
             s->transitions->حالتون[i]->عدد);
}


/*---------------------------.
| Set the transitions of S.  |
`---------------------------*/

فراغ
state_transitions_سيٽ (state *s, صحيح انگ, state **dst)
{
  aver (!s->transitions);
  s->transitions = transitions_new (انگ, dst);
  إذا (trace_flag & trace_automaton)
    state_transitions_print (s, stderr);
}


/*--------------------------.
| Set the reductions of S.  |
`--------------------------*/

فراغ
state_reductions_سيٽ (state *s, صحيح انگ, قاعدو **reds)
{
  aver (!s->reductions);
  s->reductions = reductions_new (انگ, reds);
}


صحيح
state_reduction_ڳوليو (state ثابت *s, قاعدو ثابت *r)
{
  reductions *reds = s->reductions;
  لكل (صحيح i = 0; i < reds->انگ; ++i)
    إذا (reds->قاعدا[i] == r)
      أرجع i;
  abort ();
}


/*--------------------.
| Set the errs of S.  |
`--------------------*/

فراغ
state_errs_سيٽ (state *s, صحيح انگ, نشان **ٽوڪن)
{
  aver (!s->errs);
  s->errs = errs_new (انگ, ٽوڪن);
}



/*--------------------------------------------------.
| Print on OUT all the lookahead tokens such that S |
| wants to reduce R.                                |
`--------------------------------------------------*/

فراغ
state_قاعدو_lookaheads_print (state ثابت *s, قاعدو ثابت *r, FILE *ٻاهريون)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_ڳوليو (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      حرف ثابت *sep = "";
      fprintf (ٻاهريون, "  [");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          fprintf (ٻاهريون, "%s%s", sep, symbols[k]->ٽيگ);
          sep = ", ";
        }
      fprintf (ٻاهريون, "]");
    }
}

فراغ
state_قاعدو_lookaheads_print_xml (state ثابت *s, قاعدو ثابت *r,
                                       FILE *ٻاهريون, صحيح level)
{
  /* Find the reduction we are handling.  */
  reductions *reds = s->reductions;
  صحيح red = state_reduction_ڳوليو (s, r);

  /* Print them if there are.  */
  إذا (reds->lookaheads && red != -1)
    {
      bitset_iterator biter;
      صحيح k;
      xml_puts (ٻاهريون, level, "<lookaheads>");
      BITSET_FOR_EACH (biter, reds->lookaheads[red], k, 0)
        {
          xml_printf (ٻاهريون, level + 1, "<symbol>%s</symbol>",
                      xml_escape (symbols[k]->ٽيگ));
        }
      xml_puts (ٻاهريون, level, "</lookaheads>");
    }
}


/*---------------------.
| A state hash table.  |
`---------------------*/

/* Initial capacity of states hash table.  */
#وصف_ڪريو HT_INITIAL_صلاحيت 257

ساكن بنية hash_جدول *state_جدول = عدم;

/* Two states are equal if they have the same core items.  */
ساكن مضمن bool
state_compare (state ثابت *s1, state ثابت *s2)
{
  إذا (s1->nitems != s2->nitems)
    أرجع false;

  لكل (size_t i = 0; i < s1->nitems; ++i)
    إذا (s1->شيون[i] != s2->شيون[i])
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
  size_t ڪنجي = 0;
  لكل (size_t i = 0; i < s->nitems; ++i)
    ڪنجي += s->شيون[i];
  أرجع ڪنجي % tablesize;
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
  state_جدول = hash_xinitialize (HT_INITIAL_صلاحيت,
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
state_hash_وجهو (state *s)
{
  hash_xinsert (state_جدول, s);
}


/*------------------------------------------------------------------.
| Find the state associated to the CORE, and return it.  If it does |
| not exist yet, return NULL.                                       |
`------------------------------------------------------------------*/

state *
state_hash_lookup (size_t nitems, ثابت شي_اشاريو *core)
{
  size_t شيون_سائيز = nitems * حجم *core;
  state *probe = xmalloc (offsetof (state, شيون) + شيون_سائيز);
  probe->nitems = nitems;
  يادگيري_نقل (probe->شيون, core, شيون_سائيز);
  state *داخلا_2 = hash_lookup (state_جدول, probe);
  حرر (probe);
  أرجع داخلا_2;
}


/*--------------------------------------------------------.
| Record S and all states reachable from S in REACHABLE.  |
`--------------------------------------------------------*/

ساكن فراغ
state_رڪارڊ_reachable_حالتون (state *s, bitset reachable)
{
  إذا (bitset_test (reachable, s->عدد))
    أرجع;
  bitset_سيٽ (reachable, s->عدد);
  لكل (صحيح i = 0; i < s->transitions->انگ; ++i)
    إذا (!TRANSITION_IS_DISABLED (s->transitions, i))
      state_رڪارڊ_reachable_حالتون (s->transitions->حالتون[i], reachable);
}

فراغ
state_remove_unreachable_حالتون (state_عدد old_منزل_new[])
{
  state_عدد nstates_reachable = 0;
  bitset reachable = bitset_create (nstates, BITSET_FIXED);
  state_رڪارڊ_reachable_حالتون (حالتون[0], reachable);
  لكل (state_عدد i = 0; i < nstates; ++i)
    {
      إذا (bitset_test (reachable, حالتون[i]->عدد))
        {
          حالتون[nstates_reachable] = حالتون[i];
          حالتون[nstates_reachable]->عدد = nstates_reachable;
          old_منزل_new[i] = nstates_reachable++;
        }
      وإلا
        {
          state_free (حالتون[i]);
          old_منزل_new[i] = nstates;
        }
    }
  nstates = nstates_reachable;
  bitset_free (reachable);
}

/* All the decorated states, indexed by the state number.  */
state **حالتون = عدم;


/*----------------------.
| Free all the states.  |
`----------------------*/

فراغ
حالتون_free (فراغ)
{
  گهيرو_free ();
  لكل (state_عدد i = 0; i < nstates; ++i)
    state_free (حالتون[i]);
  حرر (حالتون);
}
