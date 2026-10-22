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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "lssi.h"

#અન્તર્ભાવ <gl_linked_list.h>
#અન્તર્ભાવ <gl_xlist.h>
#અન્તર્ભાવ <stdlib.h>

#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "nullable.h"

// Lookahead sensitive state item.
પ્રકારવ્યાખ્યા સંરચના lssi
{
  state_item_સંખ્યા si;
  સંરચના lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

સ્થિર lssi *
new_lssi (state_item_સંખ્યા si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (કદ *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  પાછા res;
}

સ્થિર શૂન્ય
lssi_free (lssi *sn)
{
  જો (sn == ખાલી)
    પાછા;
  જો (sn->free_lookahead)
    bitset_free (sn->lookahead);
  મુક્ત (sn);
}

સ્થિર size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  પ્રતીક_સંખ્યા syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  પાછા hash % max;
}

સ્થિર bool
lssi_comparator (lssi *s1, lssi *s2)
{
  જો (s1->si == s2->si)
    {
      જો (s1->lookahead == s2->lookahead)
        પાછા true;
      પાછા bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  પાછા false;
}

પ્રકારવ્યાખ્યા gl_list_t lssi_list;

સ્થિર અંતઃરેખ bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  જો (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      પાછા false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  પાછા true;
}

#જો 0
સ્થિર શૂન્ય
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_વસ્તુઓ[l->si], out);
  જો (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      પ્રતીક_સંખ્યા sin;
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
સ્થિર bitset
eligible_state_વસ્તુઓ (state_item *target)
{
  bitset result = bitset_create (nstate_વસ્તુઓ, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true, 1,
                    (અચલ શૂન્ય **) &target);
  જ્યારે (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      જો (bitset_test (result, si - state_વસ્તુઓ))
        ચાલુ;
      bitset_set (result, si - state_વસ્તુઓ);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_સંખ્યા sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_વસ્તુઓ[sin]);
    }
  gl_list_free (queue);
  પાછા result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_સંખ્યા target, પ્રતીક_સંખ્યા next_sym)
{
  bitset eligible = eligible_state_વસ્તુઓ (&state_વસ્તુઓ[target]);
  Hash_table *visited = hash_initialize (32,
                                         ખાલી,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, ખાલી, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી,
                                          ખાલી, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  જ્યારે (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_સંખ્યા last = n->si;
      જો (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          તોડો;
        }
      state_item *si = &state_વસ્તુઓ[last];
      // Transitions don't change follow_L
      જો (si->trans >= 0)
        {
          જો (bitset_test (eligible, si->trans))
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
      જો (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_સંખ્યા *pos = si->item + 1;
          માટે (; !item_સંખ્યા_is_rule_સંખ્યા (*pos); ++pos)
            {
              item_સંખ્યા it = *pos;
              જો (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  તોડો;
                }
              નહીંતર
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  જો (!nullable[it - ntokens])
                    તોડો;
                }
            }
          જો (item_સંખ્યા_is_rule_સંખ્યા (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_સંખ્યા nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              જો (!bitset_test (eligible, nextSI))
                ચાલુ;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          જો (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  જો (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
  માટે (lssi *sn = n; sn != ખાલી; sn = sn->parent)
    gl_list_add_first (res, &state_વસ્તુઓ[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  જો (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      અચલ શૂન્ય *sip;
      જ્યારે (gl_list_iterator_next (&it, &sip, ખાલી))
        state_item_print ((state_item *) sip, stderr, "");
    }
  પાછા res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_પ્રતીક (પ્રતીક_સંખ્યા sym, bitset syms)
{
  જો (!syms)
    પાછા true;
  bitset_iterator biter;
  પ્રતીક_સંખ્યા sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      જો (sym == sn)
        પાછા true;
      જો (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        પાછા true;
    }
  પાછા false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  જો (!syms || !ts)
    પાછા true;
  bitset_iterator biter;
  પ્રતીક_સંખ્યા sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      જો (bitset_test (ts, sn))
        પાછા true;
      જો (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        પાછા true;
    }
  પાછા false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (અચલ state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
  જો (SI_TRANSITION (si))
    પાછા result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_સંખ્યા sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_વસ્તુઓ[sin];
    જો (!production_allowed (prevsi, si))
      ચાલુ;
    bitset prev_lookahead = prevsi->lookahead;
    જો (item_સંખ્યા_is_rule_સંખ્યા (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        જો (!intersect (prev_lookahead, lookahead))
          ચાલુ;
      }
    નહીંતર
      {
        // shift item
        જો (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            માટે (item_સંખ્યા *pos = prevsi->item + 1;
                 !applicable && nlable && item_સંખ્યા_is_પ્રતીક_સંખ્યા (*pos);
                 ++pos)
              {
                પ્રતીક_સંખ્યા next_sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*pos);
                જો (ISTOKEN (next_sym))
                  {
                    applicable = intersect_પ્રતીક (next_sym, lookahead);
                    nlable = false;
                  }
                નહીંતર
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    જો (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            જો (!applicable && !nlable)
              ચાલુ;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  પાછા result;
}
