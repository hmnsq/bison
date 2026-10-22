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

#अन्तर्भाव <config.h>

#अन्तर्भाव "lssi.h"

#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "getargs.h"
#अन्तर्भाव "nullable.h"

// Lookahead sensitive state item.
प्रकारनिर्देश संरचना lssi
{
  state_item_संख्या si;
  संरचना lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

स्थिर lssi *
new_lssi (state_item_संख्या si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (परिमाण *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  प्रत्यागम res;
}

स्थिर शून्य
lssi_free (lssi *sn)
{
  यदि (sn == रिक्त)
    प्रत्यागम;
  यदि (sn->free_lookahead)
    bitset_free (sn->lookahead);
  मुक्त (sn);
}

स्थिर size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  प्रतीक_संख्या syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  प्रत्यागम hash % max;
}

स्थिर bool
lssi_comparator (lssi *s1, lssi *s2)
{
  यदि (s1->si == s2->si)
    {
      यदि (s1->lookahead == s2->lookahead)
        प्रत्यागम true;
      प्रत्यागम bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  प्रत्यागम false;
}

प्रकारनिर्देश gl_list_t lssi_list;

स्थिर अन्तःस्थ bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  यदि (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      प्रत्यागम false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  प्रत्यागम true;
}

#यदि 0
स्थिर शून्य
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_वस्तुएँ[l->si], out);
  यदि (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      प्रतीक_संख्या sin;
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
स्थिर bitset
eligible_state_वस्तुएँ (state_item *target)
{
  bitset result = bitset_create (nstate_वस्तुएँ, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                    (ध्रुव शून्य **) &target);
  यावत् (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      यदि (bitset_test (result, si - state_वस्तुएँ))
        अनुवर्त;
      bitset_set (result, si - state_वस्तुएँ);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_वस्तुएँ[sin]);
    }
  gl_list_free (queue);
  प्रत्यागम result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_संख्या target, प्रतीक_संख्या next_sym)
{
  bitset eligible = eligible_state_वस्तुएँ (&state_वस्तुएँ[target]);
  Hash_table *visited = hash_initialize (32,
                                         रिक्त,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, रिक्त, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त,
                                          रिक्त, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  यावत् (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_संख्या last = n->si;
      यदि (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          भङ्ग;
        }
      state_item *si = &state_वस्तुएँ[last];
      // Transitions don't change follow_L
      यदि (si->trans >= 0)
        {
          यदि (bitset_test (eligible, si->trans))
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
      यदि (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_संख्या *pos = si->item + 1;
          प्रति (; !item_संख्या_is_rule_संख्या (*pos); ++pos)
            {
              item_संख्या it = *pos;
              यदि (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  भङ्ग;
                }
              अन्यथा
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  यदि (!nullable[it - ntokens])
                    भङ्ग;
                }
            }
          यदि (item_संख्या_is_rule_संख्या (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_संख्या nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              यदि (!bitset_test (eligible, nextSI))
                अनुवर्त;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          यदि (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  यदि (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  प्रति (lssi *sn = n; sn != रिक्त; sn = sn->parent)
    gl_list_add_first (res, &state_वस्तुएँ[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  यदि (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ध्रुव शून्य *sip;
      यावत् (gl_list_iterator_next (&it, &sip, रिक्त))
        state_item_print ((state_item *) sip, stderr, "");
    }
  प्रत्यागम res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_प्रतीक (प्रतीक_संख्या sym, bitset syms)
{
  यदि (!syms)
    प्रत्यागम true;
  bitset_iterator biter;
  प्रतीक_संख्या sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      यदि (sym == sn)
        प्रत्यागम true;
      यदि (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        प्रत्यागम true;
    }
  प्रत्यागम false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  यदि (!syms || !ts)
    प्रत्यागम true;
  bitset_iterator biter;
  प्रतीक_संख्या sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      यदि (bitset_test (ts, sn))
        प्रत्यागम true;
      यदि (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        प्रत्यागम true;
    }
  प्रत्यागम false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (ध्रुव state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  यदि (SI_TRANSITION (si))
    प्रत्यागम result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_संख्या sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_वस्तुएँ[sin];
    यदि (!production_allowed (prevsi, si))
      अनुवर्त;
    bitset prev_lookahead = prevsi->lookahead;
    यदि (item_संख्या_is_rule_संख्या (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        यदि (!intersect (prev_lookahead, lookahead))
          अनुवर्त;
      }
    अन्यथा
      {
        // shift item
        यदि (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            प्रति (item_संख्या *pos = prevsi->item + 1;
                 !applicable && nlable && item_संख्या_is_प्रतीक_संख्या (*pos);
                 ++pos)
              {
                प्रतीक_संख्या next_sym = item_संख्या_as_प्रतीक_संख्या (*pos);
                यदि (ISTOKEN (next_sym))
                  {
                    applicable = intersect_प्रतीक (next_sym, lookahead);
                    nlable = false;
                  }
                अन्यथा
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    यदि (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            यदि (!applicable && !nlable)
              अनुवर्त;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  प्रत्यागम result;
}
