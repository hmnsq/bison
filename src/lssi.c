/* Lookahead sensitive state item searches for counterexample generation

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ضمن "lssi.h"

#ضمن <gl_linked_list.h>
#ضمن <gl_xlist.h>
#ضمن <stdlib.h>

#ضمن "getargs.h"
#ضمن "nullable.h"

// Lookahead sensitive state item.
تعريفنوع بنية lssi
{
  state_توکی_شمېره si;
  بنية lssi *پلار;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ساكن lssi *
new_lssi (state_توکی_شمېره si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (حجم *res);
  res->si = si;
  res->پلار = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  أرجع res;
}

ساكن فراغ
lssi_free (lssi *sn)
{
  إذا (sn == عدم)
    أرجع;
  إذا (sn->free_lookahead)
    bitset_free (sn->lookahead);
  حرر (sn);
}

ساكن size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  نښه_شمېره syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  أرجع hash % max;
}

ساكن bool
lssi_comparator (lssi *s1, lssi *s2)
{
  إذا (s1->si == s2->si)
    {
      إذا (s1->lookahead == s2->lookahead)
        أرجع true;
      أرجع bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  أرجع false;
}

تعريفنوع gl_list_t lssi_list;

ساكن مضمن bool
append_lssi (lssi *sn, Hash_جدول *لیدل_شوی, lssi_list کتار)
{
  إذا (hash_lookup (لیدل_شوی, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      أرجع false;
    }
  hash_xinsert (لیدل_شوی, sn);
  gl_list_جمع_کړه_وروستی (کتار, sn);
  أرجع true;
}

#إذا 0
ساكن فراغ
lssi_print (lssi *l)
{
  FILE *بهر = stderr;
  print_state_توکی (&state_توکي[l->si], بهر);
  إذا (l->lookahead)
    {
      fprintf (بهر, "FOLLOWL = { ");
      bitset_iterator biter;
      نښه_شمېره ساین;
      BITSET_FOR_EACH (biter, l->lookahead, ساین, 0)
        fprintf (بهر, "%s, \n", symbols[ساین]->نښان);
      fprintf (بهر, "}\n");
    }
}
#د_که_پای

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
ساكن bitset
eligible_state_توکي (state_توکی *موخه)
{
  bitset پایله = bitset_create (nstate_توکي, BITSET_FIXED);
  state_توکی_list کتار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &موخه);
  طالما (gl_list_اندازه (کتار) > 0)
    {
      state_توکی *si = (state_توکی *) gl_list_get_at (کتار, 0);
      gl_list_remove_at (کتار, 0);
      إذا (bitset_test (پایله, si - state_توکي))
        استمر;
      bitset_سېټ (پایله, si - state_توکي);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_توکی_شمېره ساین;
      BITSET_FOR_EACH (biter, rsi, ساین, 0)
        gl_list_جمع_کړه_وروستی (کتار, &state_توکي[ساین]);
    }
  gl_list_free (کتار);
  أرجع پایله;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_توکی_list
تر_ټولو_لنډ_لار_from_پیل (state_توکی_شمېره موخه, نښه_شمېره بل_sym)
{
  bitset eligible = eligible_state_توکي (&state_توکي[موخه]);
  Hash_جدول *لیدل_شوی = hash_initialize (32,
                                         عدم,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_معلومات_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_سېټ (il, 0);
  lssi *init = new_lssi (0, عدم, il, true);
  lssi_list کتار = gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم,
                                          عدم, true);
  append_lssi (init, لیدل_شوی, کتار);
  // breadth-first search
  bool پای_ته_رسېدلی = false;
  lssi *n;
  طالما (gl_list_اندازه (کتار) > 0)
    {
      n = (lssi *) gl_list_get_at (کتار, 0);
      gl_list_remove_at (کتار, 0);
      state_توکی_شمېره وروستی = n->si;
      إذا (موخه == وروستی && bitset_test (n->lookahead, بل_sym))
        {
          پای_ته_رسېدلی = true;
          اكسر;
        }
      state_توکی *si = &state_توکي[وروستی];
      // Transitions don't change follow_L
      إذا (si->trans >= 0)
        {
          إذا (bitset_test (eligible, si->trans))
            {
              lssi *بل = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (بل, لیدل_شوی, کتار);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // nonterminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      إذا (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          توکی_شمېره *pos = si->توکی + 1;
          لكل (; !توکی_شمېره_is_قاعده_شمېره (*pos); ++pos)
            {
              توکی_شمېره it = *pos;
              إذا (ISTOKEN (it))
                {
                  bitset_سېټ (lookahead, it);
                  اكسر;
                }
              وإلا
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  إذا (!nullable[it - ntokens])
                    اكسر;
                }
            }
          إذا (توکی_شمېره_is_قاعده_شمېره (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_کارول_شوی = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_توکی_شمېره بل_SI;
          BITSET_FOR_EACH (biter, si->prods, بل_SI, 0)
            {
              إذا (!bitset_test (eligible, بل_SI))
                استمر;
              lssi *بل = new_lssi (بل_SI, n, lookahead,
                                     !lookahead_کارول_شوی);
              lookahead_کارول_شوی = append_lssi (بل, لیدل_شوی, کتار)
                               || lookahead_کارول_شوی;
            }
          إذا (!lookahead_کارول_شوی)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  إذا (!پای_ته_رسېدلی)
    {
      gl_list_free (کتار);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_توکی_list res =
    gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (lssi *sn = n; sn != عدم; sn = sn->پلار)
    gl_list_جمع_کړه_لومړی (res, &state_توکي[sn->si]);

  hash_free (لیدل_شوی);
  gl_list_free (کتار);

  إذا (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ثابت فراغ *sip;
      طالما (gl_list_iterator_بل (&it, &sip, عدم))
        state_توکی_print ((state_توکی *) sip, stderr, "");
    }
  أرجع res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_نښه (نښه_شمېره sym, bitset syms)
{
  إذا (!syms)
    أرجع true;
  bitset_iterator biter;
  نښه_شمېره sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      إذا (sym == sn)
        أرجع true;
      إذا (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        أرجع true;
    }
  أرجع false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  إذا (!syms || !ts)
    أرجع true;
  bitset_iterator biter;
  نښه_شمېره sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      إذا (bitset_test (ts, sn))
        أرجع true;
      إذا (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        أرجع true;
    }
  أرجع false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_توکی_list
lssi_سرچپه_production (ثابت state_توکی *si, bitset lookahead)
{
  state_توکی_list پایله =
    gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
  إذا (SI_TRANSITION (si))
    أرجع پایله;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_توکی_شمېره ساین;
  BITSET_FOR_EACH (biter, si->revs, ساین, 0)
  {
    state_توکی *prevsi = &state_توکي[ساین];
    إذا (!production_allowed (prevsi, si))
      استمر;
    bitset prev_lookahead = prevsi->lookahead;
    إذا (توکی_شمېره_is_قاعده_شمېره (*(prevsi->توکی)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        إذا (!intersect (prev_lookahead, lookahead))
          استمر;
      }
    وإلا
      {
        // shift item
        إذا (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            لكل (توکی_شمېره *pos = prevsi->توکی + 1;
                 !applicable && nlable && توکی_شمېره_is_نښه_شمېره (*pos);
                 ++pos)
              {
                نښه_شمېره بل_sym = توکی_شمېره_as_نښه_شمېره (*pos);
                إذا (ISTOKEN (بل_sym))
                  {
                    applicable = intersect_نښه (بل_sym, lookahead);
                    nlable = false;
                  }
                وإلا
                  {
                    applicable = intersect (FIRSTS (بل_sym), lookahead);
                    إذا (!applicable)
                      nlable = nullable[بل_sym - ntokens];
                  }
              }
            إذا (!applicable && !nlable)
              استمر;
          }
      }
    gl_list_جمع_کړه_وروستی (پایله, prevsi);
  }
  أرجع پایله;
}
