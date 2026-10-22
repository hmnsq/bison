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
  state_item_لمب si;
  بنية lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ساكن lssi *
new_lssi (state_item_لمب si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (حجم *res);
  res->si = si;
  res->parent = p;
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
  علام_لمب syn;
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
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  إذا (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      أرجع false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  أرجع true;
}

#إذا 0
ساكن فراغ
lssi_بغ (lssi *l)
{
  FILE *out = stderr;
  بغ_state_item (&state_ابوبو[l->si], out);
  إذا (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      علام_لمب sin;
      BITSET_FOR_EACH (biter, l->lookahead, sin, 0)
        fprintf (out, "%s, \n", symbols[sin]->tag);
      fprintf (out, "}\n");
    }
}
#endif

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
ساكن bitset
eligible_state_ابوبو (state_item *target)
{
  bitset result = bitset_create (nstate_ابوبو, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &target);
  طالما (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      إذا (bitset_test (result, si - state_ابوبو))
        استمر;
      bitset_set (result, si - state_ابوبو);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_لمب sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_ابوبو[sin]);
    }
  gl_list_free (queue);
  أرجع result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_لمب target, علام_لمب next_sym)
{
  bitset eligible = eligible_state_ابوبو (&state_ابوبو[target]);
  Hash_table *visited = hash_initialize (32,
                                         عدم,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, عدم, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, عدم, عدم,
                                          عدم, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  طالما (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_لمب last = n->si;
      إذا (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          اكسر;
        }
      state_item *si = &state_ابوبو[last];
      // Transitions don't change follow_L
      إذا (si->trans >= 0)
        {
          إذا (bitset_test (eligible, si->trans))
            {
              lssi *next = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (next, visited, queue);
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
          item_لمب *pos = si->item + 1;
          لكل (; !item_لمب_is_rule_لمب (*pos); ++pos)
            {
              item_لمب it = *pos;
              إذا (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  اكسر;
                }
              وإلا
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  إذا (!nullable[it - ntokens])
                    اكسر;
                }
            }
          إذا (item_لمب_is_rule_لمب (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_لمب nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              إذا (!bitset_test (eligible, nextSI))
                استمر;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          إذا (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  إذا (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (lssi *sn = n; sn != عدم; sn = sn->parent)
    gl_list_add_first (res, &state_ابوبو[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  إذا (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ثابت فراغ *sip;
      طالما (gl_list_iterator_next (&it, &sip, عدم))
        state_item_بغ ((state_item *) sip, stderr, "");
    }
  أرجع res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_علام (علام_لمب sym, bitset syms)
{
  إذا (!syms)
    أرجع true;
  bitset_iterator biter;
  علام_لمب sn;
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
  علام_لمب sn;
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
state_item_list
lssi_reverse_production (ثابت state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, true);
  إذا (SI_TRANSITION (si))
    أرجع result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_لمب sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_ابوبو[sin];
    إذا (!production_allowed (prevsi, si))
      استمر;
    bitset prev_lookahead = prevsi->lookahead;
    إذا (item_لمب_is_rule_لمب (*(prevsi->item)))
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
            لكل (item_لمب *pos = prevsi->item + 1;
                 !applicable && nlable && item_لمب_is_علام_لمب (*pos);
                 ++pos)
              {
                علام_لمب next_sym = item_لمب_as_علام_لمب (*pos);
                إذا (ISTOKEN (next_sym))
                  {
                    applicable = intersect_علام (next_sym, lookahead);
                    nlable = false;
                  }
                وإلا
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    إذا (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            إذا (!applicable && !nlable)
              استمر;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  أرجع result;
}
