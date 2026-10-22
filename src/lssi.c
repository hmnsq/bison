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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "lssi.h"

#அந்தர்பாவ <gl_linked_list.h>
#அந்தர்பாவ <gl_xlist.h>
#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "nullable.h"

// Lookahead sensitive state item.
வகைவரை கட்டமைப்பு lssi
{
  state_item_எண் si;
  கட்டமைப்பு lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

நிலையான lssi *
new_lssi (state_item_எண் si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (அளவு *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  திரும்பு res;
}

நிலையான வெறுமை
lssi_free (lssi *sn)
{
  எனில் (sn == வெற்று)
    திரும்பு;
  எனில் (sn->free_lookahead)
    bitset_free (sn->lookahead);
  விடுவி (sn);
}

நிலையான size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  குறியீடு_எண் syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  திரும்பு hash % max;
}

நிலையான bool
lssi_comparator (lssi *s1, lssi *s2)
{
  எனில் (s1->si == s2->si)
    {
      எனில் (s1->lookahead == s2->lookahead)
        திரும்பு true;
      திரும்பு bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  திரும்பு false;
}

வகைவரை gl_list_t lssi_list;

நிலையான உள்வரி bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  எனில் (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      திரும்பு false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  திரும்பு true;
}

#எனில் 0
நிலையான வெறுமை
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_பொருட்கள்[l->si], out);
  எனில் (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      குறியீடு_எண் sin;
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
நிலையான bitset
eligible_state_பொருட்கள் (state_item *target)
{
  bitset result = bitset_create (nstate_பொருட்கள், BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true, 1,
                    (மாறிலி வெறுமை **) &target);
  போது (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      எனில் (bitset_test (result, si - state_பொருட்கள்))
        தொடர்;
      bitset_set (result, si - state_பொருட்கள்);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_எண் sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_பொருட்கள்[sin]);
    }
  gl_list_free (queue);
  திரும்பு result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_எண் target, குறியீடு_எண் next_sym)
{
  bitset eligible = eligible_state_பொருட்கள் (&state_பொருட்கள்[target]);
  Hash_table *visited = hash_initialize (32,
                                         வெற்று,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, வெற்று, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று,
                                          வெற்று, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  போது (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_எண் last = n->si;
      எனில் (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          நிறுத்து;
        }
      state_item *si = &state_பொருட்கள்[last];
      // Transitions don't change follow_L
      எனில் (si->trans >= 0)
        {
          எனில் (bitset_test (eligible, si->trans))
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
      எனில் (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_எண் *pos = si->item + 1;
          க்கு (; !item_எண்_is_rule_எண் (*pos); ++pos)
            {
              item_எண் it = *pos;
              எனில் (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  நிறுத்து;
                }
              இல்லையெனில்
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  எனில் (!nullable[it - ntokens])
                    நிறுத்து;
                }
            }
          எனில் (item_எண்_is_rule_எண் (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_எண் nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              எனில் (!bitset_test (eligible, nextSI))
                தொடர்;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          எனில் (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  எனில் (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
  க்கு (lssi *sn = n; sn != வெற்று; sn = sn->parent)
    gl_list_add_first (res, &state_பொருட்கள்[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  எனில் (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      மாறிலி வெறுமை *sip;
      போது (gl_list_iterator_next (&it, &sip, வெற்று))
        state_item_print ((state_item *) sip, stderr, "");
    }
  திரும்பு res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_குறியீடு (குறியீடு_எண் sym, bitset syms)
{
  எனில் (!syms)
    திரும்பு true;
  bitset_iterator biter;
  குறியீடு_எண் sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      எனில் (sym == sn)
        திரும்பு true;
      எனில் (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        திரும்பு true;
    }
  திரும்பு false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  எனில் (!syms || !ts)
    திரும்பு true;
  bitset_iterator biter;
  குறியீடு_எண் sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      எனில் (bitset_test (ts, sn))
        திரும்பு true;
      எனில் (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        திரும்பு true;
    }
  திரும்பு false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (மாறிலி state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
  எனில் (SI_TRANSITION (si))
    திரும்பு result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_எண் sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_பொருட்கள்[sin];
    எனில் (!production_allowed (prevsi, si))
      தொடர்;
    bitset prev_lookahead = prevsi->lookahead;
    எனில் (item_எண்_is_rule_எண் (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        எனில் (!intersect (prev_lookahead, lookahead))
          தொடர்;
      }
    இல்லையெனில்
      {
        // shift item
        எனில் (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            க்கு (item_எண் *pos = prevsi->item + 1;
                 !applicable && nlable && item_எண்_is_குறியீடு_எண் (*pos);
                 ++pos)
              {
                குறியீடு_எண் next_sym = item_எண்_as_குறியீடு_எண் (*pos);
                எனில் (ISTOKEN (next_sym))
                  {
                    applicable = intersect_குறியீடு (next_sym, lookahead);
                    nlable = false;
                  }
                இல்லையெனில்
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    எனில் (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            எனில் (!applicable && !nlable)
              தொடர்;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  திரும்பு result;
}
