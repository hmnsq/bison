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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "lssi.h"

#ଅନ୍ତର୍ଭାଵ <gl_linked_list.h>
#ଅନ୍ତର୍ଭାଵ <gl_xlist.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"

// Lookahead sensitive state item.
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା lssi
{
  state_item_ସଂଖ୍ୟା si;
  ସଂରଚନା lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ସ୍ଥିର lssi *
new_lssi (state_item_ସଂଖ୍ୟା si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (ଆକାର *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
lssi_free (lssi *sn)
{
  ଯଦି (sn == ଖାଲି)
    ଫେରନ୍ତୁ;
  ଯଦି (sn->free_lookahead)
    bitset_free (sn->lookahead);
  ମୁକ୍ତ (sn);
}

ସ୍ଥିର size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  ପ୍ରତୀକ_ସଂଖ୍ୟା syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  ଫେରନ୍ତୁ hash % max;
}

ସ୍ଥିର bool
lssi_comparator (lssi *s1, lssi *s2)
{
  ଯଦି (s1->si == s2->si)
    {
      ଯଦି (s1->lookahead == s2->lookahead)
        ଫେରନ୍ତୁ true;
      ଫେରନ୍ତୁ bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  ଫେରନ୍ତୁ false;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା gl_list_t lssi_list;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  ଯଦି (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      ଫେରନ୍ତୁ false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  ଫେରନ୍ତୁ true;
}

#ଯଦି 0
ସ୍ଥିର ଶୂନ୍ୟ
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_ବସ୍ତୁଗୁଡ଼ିକ[l->si], out);
  ଯଦି (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      ପ୍ରତୀକ_ସଂଖ୍ୟା sin;
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
ସ୍ଥିର bitset
eligible_state_ବସ୍ତୁଗୁଡ଼ିକ (state_item *target)
{
  bitset result = bitset_create (nstate_ବସ୍ତୁଗୁଡ଼ିକ, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true, 1,
                    (ଅଚଳ ଶୂନ୍ୟ **) &target);
  ଯେତେବେଳେ (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      ଯଦି (bitset_test (result, si - state_ବସ୍ତୁଗୁଡ଼ିକ))
        ଜାରି;
      bitset_set (result, si - state_ବସ୍ତୁଗୁଡ଼ିକ);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_ସଂଖ୍ୟା sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_ବସ୍ତୁଗୁଡ଼ିକ[sin]);
    }
  gl_list_free (queue);
  ଫେରନ୍ତୁ result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_ସଂଖ୍ୟା target, ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym)
{
  bitset eligible = eligible_state_ବସ୍ତୁଗୁଡ଼ିକ (&state_ବସ୍ତୁଗୁଡ଼ିକ[target]);
  Hash_table *visited = hash_initialize (32,
                                         ଖାଲି,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, ଖାଲି, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି,
                                          ଖାଲି, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  ଯେତେବେଳେ (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_ସଂଖ୍ୟା last = n->si;
      ଯଦି (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          ଭାଙ୍ଗ;
        }
      state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[last];
      // Transitions don't change follow_L
      ଯଦି (si->trans >= 0)
        {
          ଯଦି (bitset_test (eligible, si->trans))
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
      ଯଦି (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_ସଂଖ୍ୟା *pos = si->item + 1;
          ପାଇଁ (; !item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*pos); ++pos)
            {
              item_ସଂଖ୍ୟା it = *pos;
              ଯଦି (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  ଭାଙ୍ଗ;
                }
              ଅନ୍ୟଥା
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  ଯଦି (!nullable[it - ntokens])
                    ଭାଙ୍ଗ;
                }
            }
          ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_ସଂଖ୍ୟା nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              ଯଦି (!bitset_test (eligible, nextSI))
                ଜାରି;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          ଯଦି (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  ଯଦି (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
  ପାଇଁ (lssi *sn = n; sn != ଖାଲି; sn = sn->parent)
    gl_list_add_first (res, &state_ବସ୍ତୁଗୁଡ଼ିକ[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  ଯଦି (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ଅଚଳ ଶୂନ୍ୟ *sip;
      ଯେତେବେଳେ (gl_list_iterator_next (&it, &sip, ଖାଲି))
        state_item_print ((state_item *) sip, stderr, "");
    }
  ଫେରନ୍ତୁ res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_ପ୍ରତୀକ (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, bitset syms)
{
  ଯଦି (!syms)
    ଫେରନ୍ତୁ true;
  bitset_iterator biter;
  ପ୍ରତୀକ_ସଂଖ୍ୟା sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      ଯଦି (sym == sn)
        ଫେରନ୍ତୁ true;
      ଯଦି (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        ଫେରନ୍ତୁ true;
    }
  ଫେରନ୍ତୁ false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  ଯଦି (!syms || !ts)
    ଫେରନ୍ତୁ true;
  bitset_iterator biter;
  ପ୍ରତୀକ_ସଂଖ୍ୟା sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      ଯଦି (bitset_test (ts, sn))
        ଫେରନ୍ତୁ true;
      ଯଦି (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        ଫେରନ୍ତୁ true;
    }
  ଫେରନ୍ତୁ false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (ଅଚଳ state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
  ଯଦି (SI_TRANSITION (si))
    ଫେରନ୍ତୁ result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_ସଂଖ୍ୟା sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_ବସ୍ତୁଗୁଡ଼ିକ[sin];
    ଯଦି (!production_allowed (prevsi, si))
      ଜାରି;
    bitset prev_lookahead = prevsi->lookahead;
    ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        ଯଦି (!intersect (prev_lookahead, lookahead))
          ଜାରି;
      }
    ଅନ୍ୟଥା
      {
        // shift item
        ଯଦି (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            ପାଇଁ (item_ସଂଖ୍ୟା *pos = prevsi->item + 1;
                 !applicable && nlable && item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*pos);
                 ++pos)
              {
                ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*pos);
                ଯଦି (ISTOKEN (next_sym))
                  {
                    applicable = intersect_ପ୍ରତୀକ (next_sym, lookahead);
                    nlable = false;
                  }
                ଅନ୍ୟଥା
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    ଯଦି (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            ଯଦି (!applicable && !nlable)
              ଜାରି;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  ଫେରନ୍ତୁ result;
}
