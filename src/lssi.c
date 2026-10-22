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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "lssi.h"

#അന്തര്ഭാവ <gl_linked_list.h>
#അന്തര്ഭാവ <gl_xlist.h>
#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "nullable.h"

// Lookahead sensitive state item.
തരനിർവചനം ഘടന lssi
{
  state_item_സംഖ്യ si;
  ഘടന lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

സ്ഥിതം lssi *
new_lssi (state_item_സംഖ്യ si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (വലുപ്പം *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
lssi_free (lssi *sn)
{
  എങ്കിൽ (sn == ശൂന്യ)
    തിരികെ;
  എങ്കിൽ (sn->free_lookahead)
    bitset_free (sn->lookahead);
  സ്വതന്ത്രം (sn);
}

സ്ഥിതം size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  ചിഹ്നം_സംഖ്യ syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  തിരികെ hash % max;
}

സ്ഥിതം bool
lssi_comparator (lssi *s1, lssi *s2)
{
  എങ്കിൽ (s1->si == s2->si)
    {
      എങ്കിൽ (s1->lookahead == s2->lookahead)
        തിരികെ true;
      തിരികെ bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  തിരികെ false;
}

തരനിർവചനം gl_list_t lssi_list;

സ്ഥിതം വരിയിൽ bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  എങ്കിൽ (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      തിരികെ false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  തിരികെ true;
}

#എങ്കിൽ 0
സ്ഥിതം ശൂന്യം
lssi_print (lssi *l)
{
  FILE *out = stderr;
  print_state_item (&state_ഇനങ്ങൾ[l->si], out);
  എങ്കിൽ (l->lookahead)
    {
      fprintf (out, "FOLLOWL = { ");
      bitset_iterator biter;
      ചിഹ്നം_സംഖ്യ sin;
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
സ്ഥിതം bitset
eligible_state_ഇനങ്ങൾ (state_item *target)
{
  bitset result = bitset_create (nstate_ഇനങ്ങൾ, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true, 1,
                    (സ്ഥിരം ശൂന്യം **) &target);
  ആകുമ്പോൾ (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      എങ്കിൽ (bitset_test (result, si - state_ഇനങ്ങൾ))
        തുടരുക;
      bitset_set (result, si - state_ഇനങ്ങൾ);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_സംഖ്യ sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_ഇനങ്ങൾ[sin]);
    }
  gl_list_free (queue);
  തിരികെ result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_സംഖ്യ target, ചിഹ്നം_സംഖ്യ next_sym)
{
  bitset eligible = eligible_state_ഇനങ്ങൾ (&state_ഇനങ്ങൾ[target]);
  Hash_table *visited = hash_initialize (32,
                                         ശൂന്യ,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, ശൂന്യ, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ,
                                          ശൂന്യ, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  ആകുമ്പോൾ (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_സംഖ്യ last = n->si;
      എങ്കിൽ (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          നിർത്തുക;
        }
      state_item *si = &state_ഇനങ്ങൾ[last];
      // Transitions don't change follow_L
      എങ്കിൽ (si->trans >= 0)
        {
          എങ്കിൽ (bitset_test (eligible, si->trans))
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
      എങ്കിൽ (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_സംഖ്യ *pos = si->item + 1;
          വേണ്ടി (; !item_സംഖ്യ_is_rule_സംഖ്യ (*pos); ++pos)
            {
              item_സംഖ്യ it = *pos;
              എങ്കിൽ (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  നിർത്തുക;
                }
              അല്ലെങ്കിൽ
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  എങ്കിൽ (!nullable[it - ntokens])
                    നിർത്തുക;
                }
            }
          എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_സംഖ്യ nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              എങ്കിൽ (!bitset_test (eligible, nextSI))
                തുടരുക;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          എങ്കിൽ (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  എങ്കിൽ (!finished)
    {
      gl_list_free (queue);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
  വേണ്ടി (lssi *sn = n; sn != ശൂന്യ; sn = sn->parent)
    gl_list_add_first (res, &state_ഇനങ്ങൾ[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  എങ്കിൽ (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      സ്ഥിരം ശൂന്യം *sip;
      ആകുമ്പോൾ (gl_list_iterator_next (&it, &sip, ശൂന്യ))
        state_item_print ((state_item *) sip, stderr, "");
    }
  തിരികെ res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_ചിഹ്നം (ചിഹ്നം_സംഖ്യ sym, bitset syms)
{
  എങ്കിൽ (!syms)
    തിരികെ true;
  bitset_iterator biter;
  ചിഹ്നം_സംഖ്യ sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      എങ്കിൽ (sym == sn)
        തിരികെ true;
      എങ്കിൽ (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        തിരികെ true;
    }
  തിരികെ false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  എങ്കിൽ (!syms || !ts)
    തിരികെ true;
  bitset_iterator biter;
  ചിഹ്നം_സംഖ്യ sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      എങ്കിൽ (bitset_test (ts, sn))
        തിരികെ true;
      എങ്കിൽ (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        തിരികെ true;
    }
  തിരികെ false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (സ്ഥിരം state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
  എങ്കിൽ (SI_TRANSITION (si))
    തിരികെ result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_സംഖ്യ sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_ഇനങ്ങൾ[sin];
    എങ്കിൽ (!production_allowed (prevsi, si))
      തുടരുക;
    bitset prev_lookahead = prevsi->lookahead;
    എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        എങ്കിൽ (!intersect (prev_lookahead, lookahead))
          തുടരുക;
      }
    അല്ലെങ്കിൽ
      {
        // shift item
        എങ്കിൽ (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            വേണ്ടി (item_സംഖ്യ *pos = prevsi->item + 1;
                 !applicable && nlable && item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*pos);
                 ++pos)
              {
                ചിഹ്നം_സംഖ്യ next_sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*pos);
                എങ്കിൽ (ISTOKEN (next_sym))
                  {
                    applicable = intersect_ചിഹ്നം (next_sym, lookahead);
                    nlable = false;
                  }
                അല്ലെങ്കിൽ
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    എങ്കിൽ (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            എങ്കിൽ (!applicable && !nlable)
              തുടരുക;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  തിരികെ result;
}
