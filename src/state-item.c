/* Counterexample Generation Search Nodes

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

#ಅನ್ತರ್ಭಾವ "state-item.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <gethrxtime.h>
#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <gl_xlist.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"

size_t nstate_ವಸ್ತುಗಳು;
state_item_ಸಂಖ್ಯೆ *state_item_map;
state_item *state_ವಸ್ತುಗಳು;

// Hash functions for index -> bitset hash maps.
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ;
  bitset l;
} hash_pair;

ಸ್ಥಿರ size_t
hash_pair_hasher (ಧ್ರುವ hash_pair *sl, size_t max)
{
  ಪ್ರತ್ಯಾಗಮ sl->ಕೀಲಿ % max;
}

ಸ್ಥಿರ bool
hash_pair_comparator (ಧ್ರುವ hash_pair *l, ಧ್ರುವ hash_pair *r)
{
  ಪ್ರತ್ಯಾಗಮ l->ಕೀಲಿ == r->ಕೀಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  ಮುಕ್ತ (hp);
}

ಸ್ಥಿರ Hash_table *
hash_pair_table_create (ಪೂರ್ಣಾಙ್ಕ size)
{
  ಪ್ರತ್ಯಾಗಮ hash_xinitialize (size,
                           ರಿಕ್ತ,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

ಸ್ಥಿರ bitset
hash_pair_lookup (Hash_table *tab, ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ)
{
  hash_pair probe;
  probe.ಕೀಲಿ = ಕೀಲಿ;
  hash_pair *hp = hash_lookup (tab, &probe);
  ಪ್ರತ್ಯಾಗಮ hp ? hp->l : ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hash_pair_ಸೇರಿಸು (Hash_table *tab, ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, bitset val)
{
  hash_pair *hp = xmalloc (ಪರಿಮಾಣ *hp);
  hp->ಕೀಲಿ = ಕೀಲಿ;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (ಶೂನ್ಯ) res;
  ಪ್ರತಿಜ್ಞಾ (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_ಸಂಖ್ಯೆ s, state_item_ಸಂಖ್ಯೆ off)
{
  ಪ್ರತ್ಯಾಗಮ &state_ವಸ್ತುಗಳು[state_item_index_lookup (s, off)];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
state_item_set (state_item_ಸಂಖ್ಯೆ sidx, ಧ್ರುವ state *s, item_ಸಂಖ್ಯೆ off)
{
  state_ವಸ್ತುಗಳು[sidx].state = s;
  state_ವಸ್ತುಗಳು[sidx].item = &ritem[off];
  state_ವಸ್ತುಗಳು[sidx].lookahead = ರಿಕ್ತ;
  state_ವಸ್ತುಗಳು[sidx].trans = -1;
  state_ವಸ್ತುಗಳು[sidx].prods = ರಿಕ್ತ;
  state_ವಸ್ತುಗಳು[sidx].revs = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ಸ್ಥಿರ ಶೂನ್ಯ
init_state_ವಸ್ತುಗಳು (ಶೂನ್ಯ)
{
  nstate_ವಸ್ತುಗಳು = 0;
  bitsetv production_ವಸ್ತುಗಳು = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nstates; ++i)
    {
      ಧ್ರುವ state *s = states[i];
      nstate_ವಸ್ತುಗಳು += s->nitems;
      closure (s->ವಸ್ತುಗಳು, s->nitems);
      ಪ್ರತಿ (size_t j = 0; j < nitemset; ++j)
        ಯದಿ (0 < itemset[j]
            && item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (ritem[itemset[j] - 1]))
          {
            bitset_set (production_ವಸ್ತುಗಳು[i], itemset[j]);
            ++nstate_ವಸ್ತುಗಳು;
          }
    }
  state_item_map = xnmalloc (nstates + 1, ಪರಿಮಾಣ (state_item_ಸಂಖ್ಯೆ));
  state_ವಸ್ತುಗಳು = xnmalloc (nstate_ವಸ್ತುಗಳು, ಪರಿಮಾಣ (state_item));
  state_item_ಸಂಖ್ಯೆ sidx = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      ಪೂರ್ಣಾಙ್ಕ rule_ಹುಡುಕು_idx = 0;
      ಧ್ರುವ state *s = states[i];
      ಧ್ರುವ reductions *red = s->reductions;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->ವಸ್ತುಗಳು[j]);
          state_item *si = &state_ವಸ್ತುಗಳು[sidx];
          ಧ್ರುವ rule *r = item_rule (si->item);
          ಯದಿ (rule_ಹುಡುಕು_idx < red->ಅಂಕೆ && red->rules[rule_ಹುಡುಕು_idx] < r)
            ++rule_ಹುಡುಕು_idx;
          ಯದಿ (rule_ಹುಡುಕು_idx < red->ಅಂಕೆ && r == red->rules[rule_ಹುಡುಕು_idx])
            {
              bitsetv lookahead = red->lookaheads;
              ಯದಿ (lookahead)
                si->lookahead = lookahead[rule_ಹುಡುಕು_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_ಸಂಖ್ಯೆ off;
      BITSET_FOR_EACH (biter, production_ವಸ್ತುಗಳು[i], off, 0)
        {
          state_item_set (sidx, s, off);
          ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              ಯದಿ (lookahead)
                state_ವಸ್ತುಗಳು[sidx].lookahead = lookahead[rule_ಹುಡುಕು_idx];
              ++rule_ಹುಡುಕು_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_ವಸ್ತುಗಳು;
  bitsetv_free (production_ವಸ್ತುಗಳು);
}

ಸ್ಥಿರ size_t
state_sym_hasher (ಧ್ರುವ ಶೂನ್ಯ *st, size_t max)
{
  ಪ್ರತ್ಯಾಗಮ ((state *) st)->accessing_ಚಿಹ್ನೆ % max;
}

ಸ್ಥಿರ bool
state_sym_comparator (ಧ್ರುವ ಶೂನ್ಯ *s1, ಧ್ರುವ ಶೂನ್ಯ *s2)
{
  ಪ್ರತ್ಯಾಗಮ ((state *) s1)->accessing_ಚಿಹ್ನೆ == ((state *) s2)->accessing_ಚಿಹ್ನೆ;
}

ಸ್ಥಿರ state *
state_sym_lookup (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, Hash_table *h)
{
  state probe;
  probe.accessing_ಚಿಹ್ನೆ = sym;
  ಪ್ರತ್ಯಾಗಮ hash_lookup (h, &probe);
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_trans (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->ಅಂಕೆ, ರಿಕ್ತ, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, ರಿಕ್ತ);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < t->ಅಂಕೆ; ++j)
        ಯದಿ (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_ಸಂಖ್ಯೆ *item = state_ವಸ್ತುಗಳು[j].item;
          ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*item))
            ಅನುವರ್ತ;
          state *dst = state_sym_lookup (*item, transition_set);
          ಯದಿ (!dst)
            ಅನುವರ್ತ;
          // find the item in the destination state that corresponds
          // to the transition of item
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < dst->nitems; ++k)
            ಯದಿ (item + 1 == ritem + dst->ವಸ್ತುಗಳು[k])
              {
                state_item_ಸಂಖ್ಯೆ dstSI =
                  state_item_index_lookup (dst->ಸಂಖ್ಯೆ, k);

                state_ವಸ್ತುಗಳು[j].trans = dstSI;
                bitset_set (state_ವಸ್ತುಗಳು[dstSI].revs, j);
                ಭಙ್ಗ;
            }
        }
      hash_free (transition_set);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_prods (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ವಸ್ತುಗಳು[j];
          item_ಸಂಖ್ಯೆ *item = src->item;
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = item_rule (item)->lhs->ಸಂಖ್ಯೆ;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          ಯದಿ (!itms)
            {
              itms = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_SPARSE);
              hash_pair_ಸೇರಿಸು (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ವಸ್ತುಗಳು[j];
          item_ಸಂಖ್ಯೆ item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (item) || ISTOKEN (item))
            ಅನುವರ್ತ;
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          ಯದಿ (lb)
            {
              bitset copy = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_ವಸ್ತುಗಳು[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_ಸಂಖ್ಯೆ prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_ವಸ್ತುಗಳು[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
gen_lookaheads (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ i = 0; i < nstate_ವಸ್ತುಗಳು; ++i)
    {
      state_item *si = &state_ವಸ್ತುಗಳು[i];
      ಯದಿ (item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(si->item)) || !si->lookahead)
        ಅನುವರ್ತ;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true, 1,
                        (ಧ್ರುವ ಶೂನ್ಯ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      ಯಾವತ್ (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          ಯದಿ (SI_TRANSITION (prev))
            {
              bitset rsi = state_ವಸ್ತುಗಳು[prev - state_ವಸ್ತುಗಳು].revs;
              bitset_iterator biter;
              state_item_ಸಂಖ್ಯೆ sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_ವಸ್ತುಗಳು[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = ರಿಕ್ತ;

ಸ್ಥಿರ ಶೂನ್ಯ
init_firsts (ಶೂನ್ಯ)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_ಸಂಖ್ಯೆ *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      ಯಾವತ್ (item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*n) || ISVAR (*n))
        ಅನುವರ್ತ;

      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = r->lhs->ಸಂಖ್ಯೆ;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  ಯಾವತ್ (change)
    {
      change = false;
      ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = r->lhs->ಸಂಖ್ಯೆ;
          bitset f_lhs = FIRSTS (lhs);
          ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *n = r->rhs;
               item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              ಯದಿ (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              ಯದಿ (!nullable[*n - ntokens])
                ಭಙ್ಗ;
            }
        }
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  ಯದಿ (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ಸ್ಥಿರ ಶೂನ್ಯ
prune_state_item (ಧ್ರುವ state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true, 1,
                    (ಧ್ರುವ ಶೂನ್ಯ **) &si);

  ಯಾವತ್ (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      ಯದಿ (SI_DISABLED (dsi - state_ವಸ್ತುಗಳು))
        ಅನುವರ್ತ;

      ಯದಿ (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ಧ್ರುವ state_item *trans = &state_ವಸ್ತುಗಳು[dsi->trans];
        bitset_reset (trans->revs, dsi - state_ವಸ್ತುಗಳು);
        ಯದಿ (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      ಯದಿ (dsi->prods)
        {
          bitset_iterator biter;
          state_item_ಸಂಖ್ಯೆ sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              ಯದಿ (SI_DISABLED (sin))
                ಅನುವರ್ತ;
              ಧ್ರುವ state_item *prod = &state_ವಸ್ತುಗಳು[sin];
              bitset_reset (prod->revs, dsi - state_ವಸ್ತುಗಳು);
              ಯದಿ (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_ಸಂಖ್ಯೆ sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          ಯದಿ (SI_DISABLED (sin))
            ಅನುವರ್ತ;
          state_item *rev = &state_ವಸ್ತುಗಳು[sin];
          ಯದಿ (&state_ವಸ್ತುಗಳು[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          ಅನ್ಯಥಾ ಯದಿ (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_ವಸ್ತುಗಳು);
              ಯದಿ (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          ಅನ್ಯಥಾ
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ಸ್ಥಿರ ಶೂನ್ಯ
prune_disabled_paths (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = nstate_ವಸ್ತುಗಳು - 1; i >= 0; --i)
    {
      state_item *si = &state_ವಸ್ತುಗಳು[i];
      ಯದಿ (si->trans == -1 && item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*si->item))
        prune_state_item (si);
    }
}

ಶೂನ್ಯ
state_item_print (ಧ್ರುವ state_item *si, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  fputs (prefix, out);
  item_print (si->item, ರಿಕ್ತ, out);
  putc ('\n', out);
}

ಧ್ರುವ rule*
state_item_rule (ಧ್ರುವ state_item *si)
{
  ಪ್ರತ್ಯಾಗಮ item_rule (si->item);
}

/**
 * Report the state_item graph
 */
ಸ್ಥಿರ ಶೂನ್ಯ
state_ವಸ್ತುಗಳು_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_ವಸ್ತುಗಳು);
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ಧ್ರುವ state_item *si = &state_ವಸ್ತುಗಳು[j];
          item_print (si->item, ರಿಕ್ತ, out);
          ಯದಿ (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          ಅನ್ಯಥಾ
            {
              putc ('\n', out);
              ಯದಿ (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_ವಸ್ತುಗಳು[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ಧ್ರುವ ವರ್ಣ *txt[2] = { "    => ", "    <- " };
              ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  ಯದಿ (b)
                    {
                      bitset_iterator biter;
                      state_item_ಸಂಖ್ಯೆ sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_ವಸ್ತುಗಳು[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

ಶೂನ್ಯ
state_ವಸ್ತುಗಳು_init (ಶೂನ್ಯ)
{
  xtime_t start = gethrxtime ();
  init_state_ವಸ್ತುಗಳು ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  ಯದಿ (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_ವಸ್ತುಗಳು_report (stderr);
    }
}

ಶೂನ್ಯ
state_ವಸ್ತುಗಳು_free (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ i = 0; i < nstate_ವಸ್ತುಗಳು; ++i)
    ಯದಿ (!SI_DISABLED (i))
      {
        state_item *si = &state_ವಸ್ತುಗಳು[i];
        ಯದಿ (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  ಮುಕ್ತ (state_ವಸ್ತುಗಳು);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ಧ್ರುವ state_item *si, ಧ್ರುವ state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  ಪೂರ್ಣಾಙ್ಕ prec1 = s1->prec;
  ಪೂರ್ಣಾಙ್ಕ prec2 = s2->prec;
  ಯದಿ (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      ಯದಿ (prec1 > prec2)
        ಪ್ರತ್ಯಾಗಮ false;
      // Do not expand if same precedence, but left-associative.
      ಯದಿ (prec1 == prec2 && s1->assoc == ಎಡ_assoc)
        ಪ್ರತ್ಯಾಗಮ false;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}
