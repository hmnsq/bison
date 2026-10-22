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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "lssi.h"

#অন্তর্ভাব <gl_linked_list.h>
#অন্তর্ভাব <gl_xlist.h>
#অন্তর্ভাব <stdlib.h>

#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "nullable.h"

// Lookahead sensitive state item.
প্রকারলেখ কাঠামো lssi
{
  state_item_সংখ্যা si;
  কাঠামো lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

স্থির lssi *
new_lssi (state_item_সংখ্যা si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (আকার *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  ফেরত res;
}

স্থির শূন্য
lssi_free (lssi *sn)
{
  যদি (sn == ফাঁকা)
    ফেরত;
  যদি (sn->free_lookahead)
    bitset_free (sn->lookahead);
  মুক্ত (sn);
}

স্থির size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  প্রতীক_সংখ্যা syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  ফেরত hash % max;
}

স্থির bool
lssi_comparator (lssi *s1, lssi *s2)
{
  যদি (s1->si == s2->si)
    {
      যদি (s1->lookahead == s2->lookahead)
        ফেরত true;
      ফেরত bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  ফেরত false;
}

প্রকারলেখ gl_list_t lssi_list;

স্থির অন্তঃরেখ bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  যদি (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      ফেরত false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  ফেরত true;
}

#যদি 0
স্থির শূন্য
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_বস্তুসমূহ[l->si], out);
  যদি (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      প্রতীক_সংখ্যা sin;
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
স্থির bitset
eligible_state_বস্তুসমূহ (state_item *target)
{
  bitset ফলাফল = bitset_create (nstate_বস্তুসমূহ, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true, 1,
                    (ধ্রুবক শূন্য **) &target);
  যখন (gl_list_আকার (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      যদি (bitset_test (ফলাফল, si - state_বস্তুসমূহ))
        চালু;
      bitset_set (ফলাফল, si - state_বস্তুসমূহ);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_সংখ্যা sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_বস্তুসমূহ[sin]);
    }
  gl_list_free (queue);
  ফেরত ফলাফল;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_সংখ্যা target, প্রতীক_সংখ্যা পরবর্তী_sym)
{
  bitset eligible = eligible_state_বস্তুসমূহ (&state_বস্তুসমূহ[target]);
  Hash_table *visited = hash_initialize (32,
                                         ফাঁকা,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_তথ্য_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, ফাঁকা, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা,
                                          ফাঁকা, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  যখন (gl_list_আকার (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_সংখ্যা last = n->si;
      যদি (target == last && bitset_test (n->lookahead, পরবর্তী_sym))
        {
          finished = true;
          বিরতি;
        }
      state_item *si = &state_বস্তুসমূহ[last];
      // Transitions don't change follow_L
      যদি (si->trans >= 0)
        {
          যদি (bitset_test (eligible, si->trans))
            {
              lssi *পরবর্তী = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (পরবর্তী, visited, queue);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // nonterminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      যদি (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_সংখ্যা *pos = si->item + 1;
          জন্য (; !item_সংখ্যা_is_rule_সংখ্যা (*pos); ++pos)
            {
              item_সংখ্যা it = *pos;
              যদি (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  বিরতি;
                }
              অন্যথা
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  যদি (!nullable[it - ntokens])
                    বিরতি;
                }
            }
          যদি (item_সংখ্যা_is_rule_সংখ্যা (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_সংখ্যা পরবর্তী_SI;
          BITSET_FOR_EACH (biter, si->prods, পরবর্তী_SI, 0)
            {
              যদি (!bitset_test (eligible, পরবর্তী_SI))
                চালু;
              lssi *পরবর্তী = new_lssi (পরবর্তী_SI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (পরবর্তী, visited, queue)
                               || lookahead_used;
            }
          যদি (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  যদি (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
  জন্য (lssi *sn = n; sn != ফাঁকা; sn = sn->parent)
    gl_list_add_first (res, &state_বস্তুসমূহ[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  যদি (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ধ্রুবক শূন্য *sip;
      যখন (gl_list_iterator_পরবর্তী (&it, &sip, ফাঁকা))
        state_item_print ((state_item *) sip, stderr, "");
    }
  ফেরত res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_প্রতীক (প্রতীক_সংখ্যা sym, bitset syms)
{
  যদি (!syms)
    ফেরত true;
  bitset_iterator biter;
  প্রতীক_সংখ্যা sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      যদি (sym == sn)
        ফেরত true;
      যদি (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        ফেরত true;
    }
  ফেরত false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  যদি (!syms || !ts)
    ফেরত true;
  bitset_iterator biter;
  প্রতীক_সংখ্যা sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      যদি (bitset_test (ts, sn))
        ফেরত true;
      যদি (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        ফেরত true;
    }
  ফেরত false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (ধ্রুবক state_item *si, bitset lookahead)
{
  state_item_list ফলাফল =
    gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
  যদি (SI_TRANSITION (si))
    ফেরত ফলাফল;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_সংখ্যা sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_বস্তুসমূহ[sin];
    যদি (!production_allowed (prevsi, si))
      চালু;
    bitset পূর্ব_lookahead = prevsi->lookahead;
    যদি (item_সংখ্যা_is_rule_সংখ্যা (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        যদি (!intersect (পূর্ব_lookahead, lookahead))
          চালু;
      }
    অন্যথা
      {
        // shift item
        যদি (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            জন্য (item_সংখ্যা *pos = prevsi->item + 1;
                 !applicable && nlable && item_সংখ্যা_is_প্রতীক_সংখ্যা (*pos);
                 ++pos)
              {
                প্রতীক_সংখ্যা পরবর্তী_sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*pos);
                যদি (ISTOKEN (পরবর্তী_sym))
                  {
                    applicable = intersect_প্রতীক (পরবর্তী_sym, lookahead);
                    nlable = false;
                  }
                অন্যথা
                  {
                    applicable = intersect (FIRSTS (পরবর্তী_sym), lookahead);
                    যদি (!applicable)
                      nlable = nullable[পরবর্তী_sym - ntokens];
                  }
              }
            যদি (!applicable && !nlable)
              চালু;
          }
      }
    gl_list_add_last (ফলাফল, prevsi);
  }
  ফেরত ফলাফল;
}
