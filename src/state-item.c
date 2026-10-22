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
state_ಅಂಶ_ಸಂಖ್ಯೆ *state_ಅಂಶ_map;
state_ಅಂಶ *state_ವಸ್ತುಗಳು;

// Hash functions for index -> bitset hash maps.
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಪೂರ್ಣಾಂಕ ಕೀಲಿ;
  bitset l;
} hash_pair;

ಸ್ಥಿರ size_t
hash_pair_hasher (ಸ್ಥಿರಾಂಕ hash_pair *sl, size_t max)
{
  ಹಿಂತಿರುಗಿ sl->ಕೀಲಿ % max;
}

ಸ್ಥಿರ bool
hash_pair_comparator (ಸ್ಥಿರಾಂಕ hash_pair *l, ಸ್ಥಿರಾಂಕ hash_pair *r)
{
  ಹಿಂತಿರುಗಿ l->ಕೀಲಿ == r->ಕೀಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  ಬಿಡುಗಡೆ (hp);
}

ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *
hash_pair_ಕೋಷ್ಟಕ_create (ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
  ಹಿಂತಿರುಗಿ hash_xinitialize (ಗಾತ್ರ_2,
                           ಖಾಲಿ,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_ದತ್ತಾಂಶ_freer) hash_pair_free);
}

ಸ್ಥಿರ bitset
hash_pair_lookup (Hash_ಕೋಷ್ಟಕ *tab, ಪೂರ್ಣಾಂಕ ಕೀಲಿ)
{
  hash_pair probe;
  probe.ಕೀಲಿ = ಕೀಲಿ;
  hash_pair *hp = hash_lookup (tab, &probe);
  ಹಿಂತಿರುಗಿ hp ? hp->l : ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hash_pair_ಸೇರಿಸು (Hash_ಕೋಷ್ಟಕ *tab, ಪೂರ್ಣಾಂಕ ಕೀಲಿ, bitset val)
{
  hash_pair *hp = xmalloc (ಗಾತ್ರ *hp);
  hp->ಕೀಲಿ = ಕೀಲಿ;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (ಶೂನ್ಯ) res;
  ಪ್ರತಿಜ್ಞಾ (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_ಅಂಶ *
state_ಅಂಶ_lookup (state_ಸಂಖ್ಯೆ s, state_ಅಂಶ_ಸಂಖ್ಯೆ off)
{
  ಹಿಂತಿರುಗಿ &state_ವಸ್ತುಗಳು[state_ಅಂಶ_ಸೂಚ್ಯಂಕ_lookup (s, off)];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
state_ಅಂಶ_ಗಣ (state_ಅಂಶ_ಸಂಖ್ಯೆ sidx, ಸ್ಥಿರಾಂಕ state *s, ಅಂಶ_ಸಂಖ್ಯೆ off)
{
  state_ವಸ್ತುಗಳು[sidx].state = s;
  state_ವಸ್ತುಗಳು[sidx].ಅಂಶ = &ritem[off];
  state_ವಸ್ತುಗಳು[sidx].lookahead = ಖಾಲಿ;
  state_ವಸ್ತುಗಳು[sidx].trans = -1;
  state_ವಸ್ತುಗಳು[sidx].prods = ಖಾಲಿ;
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
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nstates; ++i)
    {
      ಸ್ಥಿರಾಂಕ state *s = ಸ್ಥಿತಿಗಳು[i];
      nstate_ವಸ್ತುಗಳು += s->nitems;
      ಆವರಣ (s->ವಸ್ತುಗಳು, s->nitems);
      ಗಾಗಿ (size_t j = 0; j < nitemset; ++j)
        ಆದರೆ (0 < itemset[j]
            && ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[itemset[j] - 1]))
          {
            bitset_ಗಣ (production_ವಸ್ತುಗಳು[i], itemset[j]);
            ++nstate_ವಸ್ತುಗಳು;
          }
    }
  state_ಅಂಶ_map = xnmalloc (nstates + 1, ಗಾತ್ರ (state_ಅಂಶ_ಸಂಖ್ಯೆ));
  state_ವಸ್ತುಗಳು = xnmalloc (nstate_ವಸ್ತುಗಳು, ಗಾತ್ರ (state_ಅಂಶ));
  state_ಅಂಶ_ಸಂಖ್ಯೆ sidx = 0;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nstates; ++i)
    {
      state_ಅಂಶ_map[i] = sidx;
      ಪೂರ್ಣಾಂಕ ನಿಯಮ_ಹುಡುಕು_idx = 0;
      ಸ್ಥಿರಾಂಕ state *s = ಸ್ಥಿತಿಗಳು[i];
      ಸ್ಥಿರಾಂಕ reductions *red = s->reductions;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < s->nitems; ++j)
        {
          state_ಅಂಶ_ಗಣ (sidx, s, s->ವಸ್ತುಗಳು[j]);
          state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[sidx];
          ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = ಅಂಶ_ನಿಯಮ (si->ಅಂಶ);
          ಆದರೆ (ನಿಯಮ_ಹುಡುಕು_idx < red->ಅಂಕೆ && red->ನಿಯಮಗಳು[ನಿಯಮ_ಹುಡುಕು_idx] < r)
            ++ನಿಯಮ_ಹುಡುಕು_idx;
          ಆದರೆ (ನಿಯಮ_ಹುಡುಕು_idx < red->ಅಂಕೆ && r == red->ನಿಯಮಗಳು[ನಿಯಮ_ಹುಡುಕು_idx])
            {
              bitsetv lookahead = red->lookaheads;
              ಆದರೆ (lookahead)
                si->lookahead = lookahead[ನಿಯಮ_ಹುಡುಕು_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      ಅಂಶ_ಸಂಖ್ಯೆ off;
      BITSET_FOR_EACH (biter, production_ವಸ್ತುಗಳು[i], off, 0)
        {
          state_ಅಂಶ_ಗಣ (sidx, s, off);
          ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              ಆದರೆ (lookahead)
                state_ವಸ್ತುಗಳು[sidx].lookahead = lookahead[ನಿಯಮ_ಹುಡುಕು_idx];
              ++ನಿಯಮ_ಹುಡುಕು_idx;
            }
          ++sidx;
        }

    }
  state_ಅಂಶ_map[nstates] = nstate_ವಸ್ತುಗಳು;
  bitsetv_free (production_ವಸ್ತುಗಳು);
}

ಸ್ಥಿರ size_t
state_sym_hasher (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *st, size_t max)
{
  ಹಿಂತಿರುಗಿ ((state *) st)->accessing_ಚಿಹ್ನೆ % max;
}

ಸ್ಥಿರ bool
state_sym_comparator (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *s1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *s2)
{
  ಹಿಂತಿರುಗಿ ((state *) s1)->accessing_ಚಿಹ್ನೆ == ((state *) s2)->accessing_ಚಿಹ್ನೆ;
}

ಸ್ಥಿರ state *
state_sym_lookup (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, Hash_ಕೋಷ್ಟಕ *h)
{
  state probe;
  probe.accessing_ಚಿಹ್ನೆ = sym;
  ಹಿಂತಿರುಗಿ hash_lookup (h, &probe);
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_trans (ಶೂನ್ಯ)
{
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = ಸ್ಥಿತಿಗಳು[i];
      transitions *t = s->transitions;
      Hash_ಕೋಷ್ಟಕ *transition_ಗಣ
        = hash_xinitialize (t->ಅಂಕೆ, ಖಾಲಿ, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, ಖಾಲಿ);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < t->ಅಂಕೆ; ++j)
        ಆದರೆ (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_ಗಣ, t->ಸ್ಥಿತಿಗಳು[j]);
      ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ j = state_ಅಂಶ_map[i]; j < state_ಅಂಶ_map[i + 1]; ++j)
        {
          ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ = state_ವಸ್ತುಗಳು[j].ಅಂಶ;
          ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*ಅಂಶ))
            ಮುಂದುವರಿ;
          state *dst = state_sym_lookup (*ಅಂಶ, transition_ಗಣ);
          ಆದರೆ (!dst)
            ಮುಂದುವರಿ;
          // find the item in the destination state that corresponds
          // to the transition of item
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < dst->nitems; ++k)
            ಆದರೆ (ಅಂಶ + 1 == ritem + dst->ವಸ್ತುಗಳು[k])
              {
                state_ಅಂಶ_ಸಂಖ್ಯೆ dstSI =
                  state_ಅಂಶ_ಸೂಚ್ಯಂಕ_lookup (dst->ಸಂಖ್ಯೆ, k);

                state_ವಸ್ತುಗಳು[j].trans = dstSI;
                bitset_ಗಣ (state_ವಸ್ತುಗಳು[dstSI].revs, j);
                ನಿಲ್ಲಿಸು;
            }
        }
      hash_free (transition_ಗಣ);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
init_prods (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nstates; ++i)
    {
      state *s = ಸ್ಥಿತಿಗಳು[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_ಕೋಷ್ಟಕ *ಆವರಣ_map = hash_pair_ಕೋಷ್ಟಕ_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ j = state_ಅಂಶ_map[i] + s->nitems;
           j < state_ಅಂಶ_map[i + 1]; ++j)
        {
          state_ಅಂಶ *src = &state_ವಸ್ತುಗಳು[j];
          ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ = src->ಅಂಶ;
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = ಅಂಶ_ನಿಯಮ (ಅಂಶ)->lhs->ಸಂಖ್ಯೆ;
          bitset itms = hash_pair_lookup (ಆವರಣ_map, lhs);
          ಆದರೆ (!itms)
            {
              itms = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_SPARSE);
              hash_pair_ಸೇರಿಸು (ಆವರಣ_map, lhs, itms);
            }
          bitset_ಗಣ (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ j = state_ಅಂಶ_map[i]; j < state_ಅಂಶ_map[i + 1]; ++j)
        {
          state_ಅಂಶ *src = &state_ವಸ್ತುಗಳು[j];
          ಅಂಶ_ಸಂಖ್ಯೆ ಅಂಶ = *(src->ಅಂಶ);
          // Skip reduce items and items with terminals after the dot
          ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ಅಂಶ) || ISTOKEN (ಅಂಶ))
            ಮುಂದುವರಿ;
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ಅಂಶ);
          bitset lb = hash_pair_lookup (ಆವರಣ_map, sym);
          ಆದರೆ (lb)
            {
              bitset copy = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_ವಸ್ತುಗಳು[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_ಅಂಶ_ಸಂಖ್ಯೆ prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_ಗಣ (state_ವಸ್ತುಗಳು[prod].revs, j);
            }
        }
      hash_free (ಆವರಣ_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
gen_lookaheads (ಶೂನ್ಯ)
{
  ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ i = 0; i < nstate_ವಸ್ತುಗಳು; ++i)
    {
      state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[i];
      ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(si->ಅಂಶ)) || !si->lookahead)
        ಮುಂದುವರಿ;

      bitset lookahead = si->lookahead;
      state_ಅಂಶ_list ಸರತಿ =
        gl_list_create (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true, 1,
                        (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      ಆಗ (gl_list_ಗಾತ್ರ (ಸರತಿ) > 0)
        {
          state_ಅಂಶ *prev = (state_ಅಂಶ *) gl_list_get_at (ಸರತಿ, 0);
          gl_list_remove_at (ಸರತಿ, 0);
          prev->lookahead = lookahead;
          ಆದರೆ (SI_TRANSITION (prev))
            {
              bitset rsi = state_ವಸ್ತುಗಳು[prev - state_ವಸ್ತುಗಳು].revs;
              bitset_iterator biter;
              state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
              BITSET_FOR_EACH (biter, rsi, ಜ್ಯಾ, 0)
                gl_list_ಕೂಡಿಸು_ಮೊದಲ (ಸರತಿ, &state_ವಸ್ತುಗಳು[ಜ್ಯಾ]);
            }
        }
      gl_list_free (ಸರತಿ);
    }
}

bitsetv firsts = ಖಾಲಿ;

ಸ್ಥಿರ ಶೂನ್ಯ
init_firsts (ಶೂನ್ಯ)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ i = 0; i < nrules; ++i)
    {
      ನಿಯಮ *r = &ನಿಯಮಗಳು[i];
      ಅಂಶ_ಸಂಖ್ಯೆ *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      ಆಗ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*n) || ISVAR (*n))
        ಮುಂದುವರಿ;

      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = r->lhs->ಸಂಖ್ಯೆ;
      bitset_ಗಣ (FIRSTS (lhs), *n);
    }
  bool change = true;
  ಆಗ (change)
    {
      change = false;
      ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ i = 0; i < nrules; ++i)
        {
          ನಿಯಮ *r = &ನಿಯಮಗಳು[i];
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = r->lhs->ಸಂಖ್ಯೆ;
          bitset f_lhs = FIRSTS (lhs);
          ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *n = r->rhs;
               ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              ಆದರೆ (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              ಆದರೆ (!nullable[*n - ntokens])
                ನಿಲ್ಲಿಸು;
            }
        }
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
disable_state_ಅಂಶ (state_ಅಂಶ *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  ಆದರೆ (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ಸ್ಥಿರ ಶೂನ್ಯ
prune_state_ಅಂಶ (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si)
{
  state_ಅಂಶ_list ಸರತಿ =
    gl_list_create (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true, 1,
                    (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ **) &si);

  ಆಗ (gl_list_ಗಾತ್ರ (ಸರತಿ) > 0)
    {
      state_ಅಂಶ *dsi = (state_ಅಂಶ *) gl_list_get_at (ಸರತಿ, 0);
      gl_list_remove_at (ಸರತಿ, 0);
      ಆದರೆ (SI_DISABLED (dsi - state_ವಸ್ತುಗಳು))
        ಮುಂದುವರಿ;

      ಆದರೆ (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ಸ್ಥಿರಾಂಕ state_ಅಂಶ *trans = &state_ವಸ್ತುಗಳು[dsi->trans];
        bitset_reset (trans->revs, dsi - state_ವಸ್ತುಗಳು);
        ಆದರೆ (bitset_ಖಾಲಿಗಳು_p (trans->revs))
          gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, trans);
      }

      ಆದರೆ (dsi->prods)
        {
          bitset_iterator biter;
          state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
          BITSET_FOR_EACH (biter, dsi->prods, ಜ್ಯಾ, 0)
            {
              ಆದರೆ (SI_DISABLED (ಜ್ಯಾ))
                ಮುಂದುವರಿ;
              ಸ್ಥಿರಾಂಕ state_ಅಂಶ *prod = &state_ವಸ್ತುಗಳು[ಜ್ಯಾ];
              bitset_reset (prod->revs, dsi - state_ವಸ್ತುಗಳು);
              ಆದರೆ (bitset_ಖಾಲಿಗಳು_p (prod->revs))
                gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, prod);
            }
        }

      bitset_iterator biter;
      state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
      BITSET_FOR_EACH (biter, dsi->revs, ಜ್ಯಾ, 0)
        {
          ಆದರೆ (SI_DISABLED (ಜ್ಯಾ))
            ಮುಂದುವರಿ;
          state_ಅಂಶ *rev = &state_ವಸ್ತುಗಳು[ಜ್ಯಾ];
          ಆದರೆ (&state_ವಸ್ತುಗಳು[rev->trans] == dsi)
            gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, rev);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_ವಸ್ತುಗಳು);
              ಆದರೆ (bitset_ಖಾಲಿಗಳು_p (rev->prods))
                gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, rev);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, rev);
        }
      disable_state_ಅಂಶ (dsi);
    }
  gl_list_free (ಸರತಿ);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ಸ್ಥಿರ ಶೂನ್ಯ
prune_disabled_paths (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = nstate_ವಸ್ತುಗಳು - 1; i >= 0; --i)
    {
      state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[i];
      ಆದರೆ (si->trans == -1 && ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*si->ಅಂಶ))
        prune_state_ಅಂಶ (si);
    }
}

ಶೂನ್ಯ
state_ಅಂಶ_print (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
  ಅಂಶ_print (si->ಅಂಶ, ಖಾಲಿ, ಹೊರಹೋಗು);
  putc ('\n', ಹೊರಹೋಗು);
}

ಸ್ಥಿರಾಂಕ ನಿಯಮ*
state_ಅಂಶ_ನಿಯಮ (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si)
{
  ಹಿಂತಿರುಗಿ ಅಂಶ_ನಿಯಮ (si->ಅಂಶ);
}

/**
 * Report the state_item graph
 */
ಸ್ಥಿರ ಶೂನ್ಯ
state_ವಸ್ತುಗಳು_report (FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು, "# state items: %zu\n", nstate_ವಸ್ತುಗಳು);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      fprintf (ಹೊರಹೋಗು, "State %d:\n", i);
      ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ j = state_ಅಂಶ_map[i]; j < state_ಅಂಶ_map[i + 1]; ++j)
        {
          ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[j];
          ಅಂಶ_print (si->ಅಂಶ, ಖಾಲಿ, ಹೊರಹೋಗು);
          ಆದರೆ (SI_DISABLED (j))
            fputs ("  DISABLED\n", ಹೊರಹೋಗು);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              putc ('\n', ಹೊರಹೋಗು);
              ಆದರೆ (si->trans >= 0)
                {
                  fputs ("    -> ", ಹೊರಹೋಗು);
                  state_ಅಂಶ_print (&state_ವಸ್ತುಗಳು[si->trans], ಹೊರಹೋಗು, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *txt[2] = { "    => ", "    <- " };
              ಗಾಗಿ (ಪೂರ್ಣಾಂಕ seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  ಆದರೆ (b)
                    {
                      bitset_iterator biter;
                      state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
                      BITSET_FOR_EACH (biter, b, ಜ್ಯಾ, 0)
                        {
                          fputs (txt[seti], ಹೊರಹೋಗು);
                          state_ಅಂಶ_print (&state_ವಸ್ತುಗಳು[ಜ್ಯಾ], ಹೊರಹೋಗು, "");
                        }
                    }
                }
            }
          putc ('\n', ಹೊರಹೋಗು);
        }
    }
  fprintf (ಹೊರಹೋಗು, "FIRSTS\n");
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      fprintf (ಹೊರಹೋಗು, "  %s firsts\n", symbols[i]->ಟ್ಯಾಗ್);
      bitset_iterator iter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (ಹೊರಹೋಗು, "    %s\n", symbols[j]->ಟ್ಯಾಗ್);
    }
  fputs ("\n\n", ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
state_ವಸ್ತುಗಳು_init (ಶೂನ್ಯ)
{
  xtime_t ಆರಂಭ = gethrxtime ();
  init_state_ವಸ್ತುಗಳು ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  ಆದರೆ (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - ಆರಂಭ) / 1e9);
      state_ವಸ್ತುಗಳು_report (stderr);
    }
}

ಶೂನ್ಯ
state_ವಸ್ತುಗಳು_free (ಶೂನ್ಯ)
{
  ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ i = 0; i < nstate_ವಸ್ತುಗಳು; ++i)
    ಆದರೆ (!SI_DISABLED (i))
      {
        state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[i];
        ಆದರೆ (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  ಬಿಡುಗಡೆ (state_ವಸ್ತುಗಳು);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si, ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ಮುಂದಿನ)
{
  sym_content *s1 = ಅಂಶ_ನಿಯಮ (si->ಅಂಶ)->lhs;
  sym_content *s2 = ಅಂಶ_ನಿಯಮ (ಮುಂದಿನ->ಅಂಶ)->lhs;
  ಪೂರ್ಣಾಂಕ prec1 = s1->prec;
  ಪೂರ್ಣಾಂಕ prec2 = s2->prec;
  ಆದರೆ (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      ಆದರೆ (prec1 > prec2)
        ಹಿಂತಿರುಗಿ false;
      // Do not expand if same precedence, but left-associative.
      ಆದರೆ (prec1 == prec2 && s1->assoc == ಎಡ_assoc)
        ಹಿಂತಿರುಗಿ false;
    }
  ಹಿಂತಿರುಗಿ true;
}
