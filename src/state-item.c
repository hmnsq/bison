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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "state-item.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <gethrxtime.h>
#ଅନ୍ତର୍ଭାଵ <gl_linked_list.h>
#ଅନ୍ତର୍ଭାଵ <gl_xlist.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"

size_t nstate_ବସ୍ତୁଗୁଡ଼ିକ;
state_item_ସଂଖ୍ୟା *state_item_map;
state_item *state_ବସ୍ତୁଗୁଡ଼ିକ;

// Hash functions for index -> bitset hash maps.
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି;
  bitset l;
} hash_pair;

ସ୍ଥିର size_t
hash_pair_hasher (ଅଚଳ hash_pair *sl, size_t max)
{
  ଫେରନ୍ତୁ sl->ଚାବି % max;
}

ସ୍ଥିର bool
hash_pair_comparator (ଅଚଳ hash_pair *l, ଅଚଳ hash_pair *r)
{
  ଫେରନ୍ତୁ l->ଚାବି == r->ଚାବି;
}

ସ୍ଥିର ଶୂନ୍ୟ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  ମୁକ୍ତ (hp);
}

ସ୍ଥିର Hash_table *
hash_pair_table_create (ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
  ଫେରନ୍ତୁ hash_xinitialize (size,
                           ଖାଲି,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

ସ୍ଥିର bitset
hash_pair_lookup (Hash_table *tab, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି)
{
  hash_pair probe;
  probe.ଚାବି = ଚାବି;
  hash_pair *hp = hash_lookup (tab, &probe);
  ଫେରନ୍ତୁ hp ? hp->l : ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ
hash_pair_ଭର୍ତ୍ତି (Hash_table *tab, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, bitset val)
{
  hash_pair *hp = xmalloc (ଆକାର *hp);
  hp->ଚାବି = ଚାବି;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (ଶୂନ୍ୟ) res;
  ପ୍ରତିଜ୍ଞା (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_ସଂଖ୍ୟା s, state_item_ସଂଖ୍ୟା off)
{
  ଫେରନ୍ତୁ &state_ବସ୍ତୁଗୁଡ଼ିକ[state_item_index_lookup (s, off)];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
state_item_set (state_item_ସଂଖ୍ୟା sidx, ଅଚଳ state *s, item_ସଂଖ୍ୟା off)
{
  state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].state = s;
  state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].item = &ritem[off];
  state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].lookahead = ଖାଲି;
  state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].trans = -1;
  state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].prods = ଖାଲି;
  state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].revs = bitset_create (nstate_ବସ୍ତୁଗୁଡ଼ିକ, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ସ୍ଥିର ଶୂନ୍ୟ
init_state_ବସ୍ତୁଗୁଡ଼ିକ (ଶୂନ୍ୟ)
{
  nstate_ବସ୍ତୁଗୁଡ଼ିକ = 0;
  bitsetv production_ବସ୍ତୁଗୁଡ଼ିକ = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      ଅଚଳ state *s = states[i];
      nstate_ବସ୍ତୁଗୁଡ଼ିକ += s->nitems;
      closure (s->ବସ୍ତୁଗୁଡ଼ିକ, s->nitems);
      ପାଇଁ (size_t j = 0; j < nitemset; ++j)
        ଯଦି (0 < itemset[j]
            && item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (ritem[itemset[j] - 1]))
          {
            bitset_set (production_ବସ୍ତୁଗୁଡ଼ିକ[i], itemset[j]);
            ++nstate_ବସ୍ତୁଗୁଡ଼ିକ;
          }
    }
  state_item_map = xnmalloc (nstates + 1, ଆକାର (state_item_ସଂଖ୍ୟା));
  state_ବସ୍ତୁଗୁଡ଼ିକ = xnmalloc (nstate_ବସ୍ତୁଗୁଡ଼ିକ, ଆକାର (state_item));
  state_item_ସଂଖ୍ୟା sidx = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_ସନ୍ଧାନ_idx = 0;
      ଅଚଳ state *s = states[i];
      ଅଚଳ reductions *red = s->reductions;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->ବସ୍ତୁଗୁଡ଼ିକ[j]);
          state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[sidx];
          ଅଚଳ rule *r = item_rule (si->item);
          ଯଦି (rule_ସନ୍ଧାନ_idx < red->ଅଙ୍କ && red->rules[rule_ସନ୍ଧାନ_idx] < r)
            ++rule_ସନ୍ଧାନ_idx;
          ଯଦି (rule_ସନ୍ଧାନ_idx < red->ଅଙ୍କ && r == red->rules[rule_ସନ୍ଧାନ_idx])
            {
              bitsetv lookahead = red->lookaheads;
              ଯଦି (lookahead)
                si->lookahead = lookahead[rule_ସନ୍ଧାନ_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_ସଂଖ୍ୟା off;
      BITSET_FOR_EACH (biter, production_ବସ୍ତୁଗୁଡ଼ିକ[i], off, 0)
        {
          state_item_set (sidx, s, off);
          ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              ଯଦି (lookahead)
                state_ବସ୍ତୁଗୁଡ଼ିକ[sidx].lookahead = lookahead[rule_ସନ୍ଧାନ_idx];
              ++rule_ସନ୍ଧାନ_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_ବସ୍ତୁଗୁଡ଼ିକ;
  bitsetv_free (production_ବସ୍ତୁଗୁଡ଼ିକ);
}

ସ୍ଥିର size_t
state_sym_hasher (ଅଚଳ ଶୂନ୍ୟ *st, size_t max)
{
  ଫେରନ୍ତୁ ((state *) st)->accessing_ପ୍ରତୀକ % max;
}

ସ୍ଥିର bool
state_sym_comparator (ଅଚଳ ଶୂନ୍ୟ *s1, ଅଚଳ ଶୂନ୍ୟ *s2)
{
  ଫେରନ୍ତୁ ((state *) s1)->accessing_ପ୍ରତୀକ == ((state *) s2)->accessing_ପ୍ରତୀକ;
}

ସ୍ଥିର state *
state_sym_lookup (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, Hash_table *h)
{
  state probe;
  probe.accessing_ପ୍ରତୀକ = sym;
  ଫେରନ୍ତୁ hash_lookup (h, &probe);
}

ସ୍ଥିର ଶୂନ୍ୟ
init_trans (ଶୂନ୍ୟ)
{
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->ଅଙ୍କ, ଖାଲି, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, ଖାଲି);
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < t->ଅଙ୍କ; ++j)
        ଯଦି (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      ପାଇଁ (state_item_ସଂଖ୍ୟା j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_ସଂଖ୍ୟା *item = state_ବସ୍ତୁଗୁଡ଼ିକ[j].item;
          ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*item))
            ଜାରି;
          state *dst = state_sym_lookup (*item, transition_set);
          ଯଦି (!dst)
            ଜାରି;
          // find the item in the destination state that corresponds
          // to the transition of item
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < dst->nitems; ++k)
            ଯଦି (item + 1 == ritem + dst->ବସ୍ତୁଗୁଡ଼ିକ[k])
              {
                state_item_ସଂଖ୍ୟା dstSI =
                  state_item_index_lookup (dst->ସଂଖ୍ୟା, k);

                state_ବସ୍ତୁଗୁଡ଼ିକ[j].trans = dstSI;
                bitset_set (state_ବସ୍ତୁଗୁଡ଼ିକ[dstSI].revs, j);
                ଭାଙ୍ଗ;
            }
        }
      hash_free (transition_set);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
init_prods (ଶୂନ୍ୟ)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      ପାଇଁ (state_item_ସଂଖ୍ୟା j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ବସ୍ତୁଗୁଡ଼ିକ[j];
          item_ସଂଖ୍ୟା *item = src->item;
          ପ୍ରତୀକ_ସଂଖ୍ୟା lhs = item_rule (item)->lhs->ସଂଖ୍ୟା;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          ଯଦି (!itms)
            {
              itms = bitset_create (nstate_ବସ୍ତୁଗୁଡ଼ିକ, BITSET_SPARSE);
              hash_pair_ଭର୍ତ୍ତି (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      ପାଇଁ (state_item_ସଂଖ୍ୟା j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ବସ୍ତୁଗୁଡ଼ିକ[j];
          item_ସଂଖ୍ୟା item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (item) || ISTOKEN (item))
            ଜାରି;
          ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          ଯଦି (lb)
            {
              bitset copy = bitset_create (nstate_ବସ୍ତୁଗୁଡ଼ିକ, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_ବସ୍ତୁଗୁଡ଼ିକ[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_ସଂଖ୍ୟା prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_ବସ୍ତୁଗୁଡ଼ିକ[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
gen_lookaheads (ଶୂନ୍ୟ)
{
  ପାଇଁ (state_item_ସଂଖ୍ୟା i = 0; i < nstate_ବସ୍ତୁଗୁଡ଼ିକ; ++i)
    {
      state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[i];
      ଯଦି (item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*(si->item)) || !si->lookahead)
        ଜାରି;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true, 1,
                        (ଅଚଳ ଶୂନ୍ୟ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      ଯେତେବେଳେ (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          ଯଦି (SI_TRANSITION (prev))
            {
              bitset rsi = state_ବସ୍ତୁଗୁଡ଼ିକ[prev - state_ବସ୍ତୁଗୁଡ଼ିକ].revs;
              bitset_iterator biter;
              state_item_ସଂଖ୍ୟା sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_ବସ୍ତୁଗୁଡ଼ିକ[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = ଖାଲି;

ସ୍ଥିର ଶୂନ୍ୟ
init_firsts (ଶୂନ୍ୟ)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  ପାଇଁ (rule_ସଂଖ୍ୟା i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_ସଂଖ୍ୟା *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      ଯେତେବେଳେ (item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*n) || ISVAR (*n))
        ଜାରି;

      ପ୍ରତୀକ_ସଂଖ୍ୟା lhs = r->lhs->ସଂଖ୍ୟା;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  ଯେତେବେଳେ (change)
    {
      change = false;
      ପାଇଁ (rule_ସଂଖ୍ୟା i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          ପ୍ରତୀକ_ସଂଖ୍ୟା lhs = r->lhs->ସଂଖ୍ୟା;
          bitset f_lhs = FIRSTS (lhs);
          ପାଇଁ (item_ସଂଖ୍ୟା *n = r->rhs;
               item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              ଯଦି (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              ଯଦି (!nullable[*n - ntokens])
                ଭାଙ୍ଗ;
            }
        }
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  ଯଦି (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ସ୍ଥିର ଶୂନ୍ୟ
prune_state_item (ଅଚଳ state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true, 1,
                    (ଅଚଳ ଶୂନ୍ୟ **) &si);

  ଯେତେବେଳେ (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      ଯଦି (SI_DISABLED (dsi - state_ବସ୍ତୁଗୁଡ଼ିକ))
        ଜାରି;

      ଯଦି (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ଅଚଳ state_item *trans = &state_ବସ୍ତୁଗୁଡ଼ିକ[dsi->trans];
        bitset_reset (trans->revs, dsi - state_ବସ୍ତୁଗୁଡ଼ିକ);
        ଯଦି (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      ଯଦି (dsi->prods)
        {
          bitset_iterator biter;
          state_item_ସଂଖ୍ୟା sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              ଯଦି (SI_DISABLED (sin))
                ଜାରି;
              ଅଚଳ state_item *prod = &state_ବସ୍ତୁଗୁଡ଼ିକ[sin];
              bitset_reset (prod->revs, dsi - state_ବସ୍ତୁଗୁଡ଼ିକ);
              ଯଦି (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_ସଂଖ୍ୟା sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          ଯଦି (SI_DISABLED (sin))
            ଜାରି;
          state_item *rev = &state_ବସ୍ତୁଗୁଡ଼ିକ[sin];
          ଯଦି (&state_ବସ୍ତୁଗୁଡ଼ିକ[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          ଅନ୍ୟଥା ଯଦି (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_ବସ୍ତୁଗୁଡ଼ିକ);
              ଯଦି (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          ଅନ୍ୟଥା
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ସ୍ଥିର ଶୂନ୍ୟ
prune_disabled_paths (ଶୂନ୍ୟ)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = nstate_ବସ୍ତୁଗୁଡ଼ିକ - 1; i >= 0; --i)
    {
      state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[i];
      ଯଦି (si->trans == -1 && item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*si->item))
        prune_state_item (si);
    }
}

ଶୂନ୍ୟ
state_item_print (ଅଚଳ state_item *si, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  fputs (prefix, out);
  item_print (si->item, ଖାଲି, out);
  putc ('\n', out);
}

ଅଚଳ rule*
state_item_rule (ଅଚଳ state_item *si)
{
  ଫେରନ୍ତୁ item_rule (si->item);
}

/**
 * Report the state_item graph
 */
ସ୍ଥିର ଶୂନ୍ୟ
state_ବସ୍ତୁଗୁଡ଼ିକ_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_ବସ୍ତୁଗୁଡ଼ିକ);
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      ପାଇଁ (state_item_ସଂଖ୍ୟା j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ଅଚଳ state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[j];
          item_print (si->item, ଖାଲି, out);
          ଯଦି (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          ଅନ୍ୟଥା
            {
              putc ('\n', out);
              ଯଦି (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_ବସ୍ତୁଗୁଡ଼ିକ[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ଅଚଳ ଅକ୍ଷର *txt[2] = { "    => ", "    <- " };
              ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  ଯଦି (b)
                    {
                      bitset_iterator biter;
                      state_item_ସଂଖ୍ୟା sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_ବସ୍ତୁଗୁଡ଼ିକ[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      ପ୍ରତୀକ_ସଂଖ୍ୟା j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

ଶୂନ୍ୟ
state_ବସ୍ତୁଗୁଡ଼ିକ_init (ଶୂନ୍ୟ)
{
  xtime_t start = gethrxtime ();
  init_state_ବସ୍ତୁଗୁଡ଼ିକ ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  ଯଦି (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_ବସ୍ତୁଗୁଡ଼ିକ_report (stderr);
    }
}

ଶୂନ୍ୟ
state_ବସ୍ତୁଗୁଡ଼ିକ_free (ଶୂନ୍ୟ)
{
  ପାଇଁ (state_item_ସଂଖ୍ୟା i = 0; i < nstate_ବସ୍ତୁଗୁଡ଼ିକ; ++i)
    ଯଦି (!SI_DISABLED (i))
      {
        state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[i];
        ଯଦି (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  ମୁକ୍ତ (state_ବସ୍ତୁଗୁଡ଼ିକ);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ଅଚଳ state_item *si, ଅଚଳ state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec1 = s1->prec;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec2 = s2->prec;
  ଯଦି (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      ଯଦି (prec1 > prec2)
        ଫେରନ୍ତୁ false;
      // Do not expand if same precedence, but left-associative.
      ଯଦି (prec1 == prec2 && s1->assoc == ବାମ_assoc)
        ଫେରନ୍ତୁ false;
    }
  ଫେରନ୍ତୁ true;
}
