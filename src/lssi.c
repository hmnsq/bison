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

#شامل <config.h>

#شامل "lssi.h"

#شامل <gl_linked_list.h>
#شامل <gl_xlist.h>
#شامل <stdlib.h>

#شامل "getargs.h"
#شامل "nullable.h"

// Lookahead sensitive state item.
تعریفنوع ساختار lssi
{
  state_item_عدد si;
  ساختار lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ایستا lssi *
new_lssi (state_item_عدد si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (اندازه *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  بازگشت res;
}

ایستا تهی
lssi_free (lssi *sn)
{
  اگر (sn == پوچ)
    بازگشت;
  اگر (sn->free_lookahead)
    bitset_free (sn->lookahead);
  آزاد (sn);
}

ایستا size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  نماد_عدد syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  بازگشت hash % max;
}

ایستا bool
lssi_comparator (lssi *s1, lssi *s2)
{
  اگر (s1->si == s2->si)
    {
      اگر (s1->lookahead == s2->lookahead)
        بازگشت true;
      بازگشت bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  بازگشت false;
}

تعریفنوع gl_list_t lssi_list;

ایستا درونخطی bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  اگر (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      بازگشت false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  بازگشت true;
}

#اگر 0
ایستا تهی
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_اقلام[l->si], out);
  اگر (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      نماد_عدد sin;
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
ایستا bitset
eligible_state_اقلام (state_item *target)
{
  bitset result = bitset_create (nstate_اقلام, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, پوچ, پوچ, پوچ, true, 1,
                    (ثابت تهی **) &target);
  وقتی (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      اگر (bitset_test (result, si - state_اقلام))
        ادامه;
      bitset_set (result, si - state_اقلام);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_عدد sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_اقلام[sin]);
    }
  gl_list_free (queue);
  بازگشت result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_عدد target, نماد_عدد next_sym)
{
  bitset eligible = eligible_state_اقلام (&state_اقلام[target]);
  Hash_table *visited = hash_initialize (32,
                                         پوچ,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, پوچ, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ,
                                          پوچ, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  وقتی (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_عدد last = n->si;
      اگر (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          بشکن;
        }
      state_item *si = &state_اقلام[last];
      // Transitions don't change follow_L
      اگر (si->trans >= 0)
        {
          اگر (bitset_test (eligible, si->trans))
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
      اگر (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_عدد *pos = si->item + 1;
          برای (; !item_عدد_is_rule_عدد (*pos); ++pos)
            {
              item_عدد it = *pos;
              اگر (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  بشکن;
                }
              وگرنه
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  اگر (!nullable[it - ntokens])
                    بشکن;
                }
            }
          اگر (item_عدد_is_rule_عدد (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_عدد nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              اگر (!bitset_test (eligible, nextSI))
                ادامه;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          اگر (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  اگر (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
  برای (lssi *sn = n; sn != پوچ; sn = sn->parent)
    gl_list_add_first (res, &state_اقلام[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  اگر (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ثابت تهی *sip;
      وقتی (gl_list_iterator_next (&it, &sip, پوچ))
        state_item_print ((state_item *) sip, stderr, "");
    }
  بازگشت res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_نماد (نماد_عدد sym, bitset syms)
{
  اگر (!syms)
    بازگشت true;
  bitset_iterator biter;
  نماد_عدد sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      اگر (sym == sn)
        بازگشت true;
      اگر (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        بازگشت true;
    }
  بازگشت false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  اگر (!syms || !ts)
    بازگشت true;
  bitset_iterator biter;
  نماد_عدد sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      اگر (bitset_test (ts, sn))
        بازگشت true;
      اگر (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        بازگشت true;
    }
  بازگشت false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (ثابت state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
  اگر (SI_TRANSITION (si))
    بازگشت result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_عدد sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_اقلام[sin];
    اگر (!production_allowed (prevsi, si))
      ادامه;
    bitset prev_lookahead = prevsi->lookahead;
    اگر (item_عدد_is_rule_عدد (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        اگر (!intersect (prev_lookahead, lookahead))
          ادامه;
      }
    وگرنه
      {
        // shift item
        اگر (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            برای (item_عدد *pos = prevsi->item + 1;
                 !applicable && nlable && item_عدد_is_نماد_عدد (*pos);
                 ++pos)
              {
                نماد_عدد next_sym = item_عدد_as_نماد_عدد (*pos);
                اگر (ISTOKEN (next_sym))
                  {
                    applicable = intersect_نماد (next_sym, lookahead);
                    nlable = false;
                  }
                وگرنه
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    اگر (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            اگر (!applicable && !nlable)
              ادامه;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  بازگشت result;
}
