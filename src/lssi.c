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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "lssi.h"

#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <gl_xlist.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"

// Lookahead sensitive state item.
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ lssi
{
  state_item_ಸಂಖ್ಯೆ si;
  ಸಂರಚನಾ lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ಸ್ಥಿರ lssi *
new_lssi (state_item_ಸಂಖ್ಯೆ si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (ಪರಿಮಾಣ *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
lssi_free (lssi *sn)
{
  ಯದಿ (sn == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (sn->free_lookahead)
    bitset_free (sn->lookahead);
  ಮುಕ್ತ (sn);
}

ಸ್ಥಿರ size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  ಪ್ರತ್ಯಾಗಮ hash % max;
}

ಸ್ಥಿರ bool
lssi_comparator (lssi *s1, lssi *s2)
{
  ಯದಿ (s1->si == s2->si)
    {
      ಯದಿ (s1->lookahead == s2->lookahead)
        ಪ್ರತ್ಯಾಗಮ true;
      ಪ್ರತ್ಯಾಗಮ bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

ಪ್ರಕಾರನಿರ್ದೇಶ gl_list_t lssi_list;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  ಯದಿ (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      ಪ್ರತ್ಯಾಗಮ false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  ಪ್ರತ್ಯಾಗಮ true;
}

#ಯದಿ 0
ಸ್ಥಿರ ಶೂನ್ಯ
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_ವಸ್ತುಗಳು[l->si], out);
  ಯದಿ (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sin;
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
ಸ್ಥಿರ bitset
eligible_state_ವಸ್ತುಗಳು (state_item *target)
{
  bitset result = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true, 1,
                    (ಧ್ರುವ ಶೂನ್ಯ **) &target);
  ಯಾವತ್ (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      ಯದಿ (bitset_test (result, si - state_ವಸ್ತುಗಳು))
        ಅನುವರ್ತ;
      bitset_set (result, si - state_ವಸ್ತುಗಳು);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_ಸಂಖ್ಯೆ sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_ವಸ್ತುಗಳು[sin]);
    }
  gl_list_free (queue);
  ಪ್ರತ್ಯಾಗಮ result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_ಸಂಖ್ಯೆ target, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym)
{
  bitset eligible = eligible_state_ವಸ್ತುಗಳು (&state_ವಸ್ತುಗಳು[target]);
  Hash_table *visited = hash_initialize (32,
                                         ರಿಕ್ತ,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, ರಿಕ್ತ, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ,
                                          ರಿಕ್ತ, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  ಯಾವತ್ (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_ಸಂಖ್ಯೆ last = n->si;
      ಯದಿ (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          ಭಙ್ಗ;
        }
      state_item *si = &state_ವಸ್ತುಗಳು[last];
      // Transitions don't change follow_L
      ಯದಿ (si->trans >= 0)
        {
          ಯದಿ (bitset_test (eligible, si->trans))
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
      ಯದಿ (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_ಸಂಖ್ಯೆ *pos = si->item + 1;
          ಪ್ರತಿ (; !item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*pos); ++pos)
            {
              item_ಸಂಖ್ಯೆ it = *pos;
              ಯದಿ (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  ಭಙ್ಗ;
                }
              ಅನ್ಯಥಾ
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  ಯದಿ (!nullable[it - ntokens])
                    ಭಙ್ಗ;
                }
            }
          ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_ಸಂಖ್ಯೆ nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              ಯದಿ (!bitset_test (eligible, nextSI))
                ಅನುವರ್ತ;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          ಯದಿ (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  ಯದಿ (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
  ಪ್ರತಿ (lssi *sn = n; sn != ರಿಕ್ತ; sn = sn->parent)
    gl_list_add_first (res, &state_ವಸ್ತುಗಳು[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  ಯದಿ (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ಧ್ರುವ ಶೂನ್ಯ *sip;
      ಯಾವತ್ (gl_list_iterator_next (&it, &sip, ರಿಕ್ತ))
        state_item_print ((state_item *) sip, stderr, "");
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_ಚಿಹ್ನೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, bitset syms)
{
  ಯದಿ (!syms)
    ಪ್ರತ್ಯಾಗಮ true;
  bitset_iterator biter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      ಯದಿ (sym == sn)
        ಪ್ರತ್ಯಾಗಮ true;
      ಯದಿ (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  ಯದಿ (!syms || !ts)
    ಪ್ರತ್ಯಾಗಮ true;
  bitset_iterator biter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      ಯದಿ (bitset_test (ts, sn))
        ಪ್ರತ್ಯಾಗಮ true;
      ಯದಿ (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (ಧ್ರುವ state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
  ಯದಿ (SI_TRANSITION (si))
    ಪ್ರತ್ಯಾಗಮ result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_ಸಂಖ್ಯೆ sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_ವಸ್ತುಗಳು[sin];
    ಯದಿ (!production_allowed (prevsi, si))
      ಅನುವರ್ತ;
    bitset prev_lookahead = prevsi->lookahead;
    ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        ಯದಿ (!intersect (prev_lookahead, lookahead))
          ಅನುವರ್ತ;
      }
    ಅನ್ಯಥಾ
      {
        // shift item
        ಯದಿ (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *pos = prevsi->item + 1;
                 !applicable && nlable && item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*pos);
                 ++pos)
              {
                ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*pos);
                ಯದಿ (ISTOKEN (next_sym))
                  {
                    applicable = intersect_ಚಿಹ್ನೆ (next_sym, lookahead);
                    nlable = false;
                  }
                ಅನ್ಯಥಾ
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    ಯದಿ (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            ಯದಿ (!applicable && !nlable)
              ಅನುವರ್ತ;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  ಪ್ರತ್ಯಾಗಮ result;
}
