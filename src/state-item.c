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

#ضمن <config.h>

#ضمن "state-item.h"

#ضمن <assert.h>
#ضمن <gethrxtime.h>
#ضمن <gl_linked_list.h>
#ضمن <gl_xlist.h>
#ضمن <stdlib.h>
#ضمن <time.h>

#ضمن "closure.h"
#ضمن "getargs.h"
#ضمن "nullable.h"

size_t nstate_items;
state_item_number *state_item_map;
state_item *state_items;

// Hash functions for index -> bitset hash maps.
تعريفنوع بنية
{
  صحيح key;
  bitset l;
} hash_pair;

ساكن size_t
hash_pair_hasher (ثابت hash_pair *sl, size_t max)
{
  أرجع sl->key % max;
}

ساكن bool
hash_pair_comparator (ثابت hash_pair *l, ثابت hash_pair *r)
{
  أرجع l->key == r->key;
}

ساكن فراغ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  حرر (hp);
}

ساكن Hash_table *
hash_pair_table_create (صحيح size)
{
  أرجع hash_xinitialize (size,
                           عدم,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

ساكن bitset
hash_pair_lookup (Hash_table *tab, صحيح key)
{
  hash_pair probe;
  probe.key = key;
  hash_pair *hp = hash_lookup (tab, &probe);
  أرجع hp ? hp->l : عدم;
}

ساكن فراغ
hash_pair_insert (Hash_table *tab, صحيح key, bitset val)
{
  hash_pair *hp = xmalloc (حجم *hp);
  hp->key = key;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (فراغ) res;
  أكد (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_number s, state_item_number off)
{
  أرجع &state_items[state_item_index_lookup (s, off)];
}

ساكن مضمن فراغ
state_item_set (state_item_number sidx, ثابت state *s, item_number off)
{
  state_items[sidx].state = s;
  state_items[sidx].item = &ritem[off];
  state_items[sidx].lookahead = عدم;
  state_items[sidx].trans = -1;
  state_items[sidx].prods = عدم;
  state_items[sidx].revs = bitset_create (nstate_items, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ساكن فراغ
init_state_items (فراغ)
{
  nstate_items = 0;
  bitsetv production_items = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      ثابت state *s = states[i];
      nstate_items += s->nitems;
      closure (s->items, s->nitems);
      لكل (size_t j = 0; j < nitemset; ++j)
        𐡄𐡍 (0 < itemset[j]
            && item_number_is_rule_number (ritem[itemset[j] - 1]))
          {
            bitset_set (production_items[i], itemset[j]);
            ++nstate_items;
          }
    }
  state_item_map = xnmalloc (nstates + 1, حجم (state_item_number));
  state_items = xnmalloc (nstate_items, حجم (state_item));
  state_item_number sidx = 0;
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      صحيح rule_search_idx = 0;
      ثابت state *s = states[i];
      ثابت reductions *red = s->reductions;
      لكل (صحيح j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->items[j]);
          state_item *si = &state_items[sidx];
          ثابت rule *r = item_rule (si->item);
          𐡄𐡍 (rule_search_idx < red->num && red->rules[rule_search_idx] < r)
            ++rule_search_idx;
          𐡄𐡍 (rule_search_idx < red->num && r == red->rules[rule_search_idx])
            {
              bitsetv lookahead = red->lookaheads;
              𐡄𐡍 (lookahead)
                si->lookahead = lookahead[rule_search_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_number off;
      BITSET_FOR_EACH (biter, production_items[i], off, 0)
        {
          state_item_set (sidx, s, off);
          𐡄𐡍 (item_number_is_rule_number (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              𐡄𐡍 (lookahead)
                state_items[sidx].lookahead = lookahead[rule_search_idx];
              ++rule_search_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_items;
  bitsetv_free (production_items);
}

ساكن size_t
state_sym_hasher (ثابت فراغ *st, size_t max)
{
  أرجع ((state *) st)->accessing_symbol % max;
}

ساكن bool
state_sym_comparator (ثابت فراغ *s1, ثابت فراغ *s2)
{
  أرجع ((state *) s1)->accessing_symbol == ((state *) s2)->accessing_symbol;
}

ساكن state *
state_sym_lookup (symbol_number sym, Hash_table *h)
{
  state probe;
  probe.accessing_symbol = sym;
  أرجع hash_lookup (h, &probe);
}

ساكن فراغ
init_trans (فراغ)
{
  لكل (state_number i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->num, عدم, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, عدم);
      لكل (صحيح j = 0; j < t->num; ++j)
        𐡄𐡍 (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      لكل (state_item_number j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_number *item = state_items[j].item;
          𐡄𐡍 (item_number_is_rule_number (*item))
            استمر;
          state *dst = state_sym_lookup (*item, transition_set);
          𐡄𐡍 (!dst)
            استمر;
          // find the item in the destination state that corresponds
          // to the transition of item
          لكل (صحيح k = 0; k < dst->nitems; ++k)
            𐡄𐡍 (item + 1 == ritem + dst->items[k])
              {
                state_item_number dstSI =
                  state_item_index_lookup (dst->number, k);

                state_items[j].trans = dstSI;
                bitset_set (state_items[dstSI].revs, j);
                اكسر;
            }
        }
      hash_free (transition_set);
    }
}

ساكن فراغ
init_prods (فراغ)
{
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      لكل (state_item_number j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_items[j];
          item_number *item = src->item;
          symbol_number lhs = item_rule (item)->lhs->number;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          𐡄𐡍 (!itms)
            {
              itms = bitset_create (nstate_items, BITSET_SPARSE);
              hash_pair_insert (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      لكل (state_item_number j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_items[j];
          item_number item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          𐡄𐡍 (item_number_is_rule_number (item) || ISTOKEN (item))
            استمر;
          symbol_number sym = item_number_as_symbol_number (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          𐡄𐡍 (lb)
            {
              bitset copy = bitset_create (nstate_items, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_items[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_number prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_items[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ساكن مضمن فراغ
gen_lookaheads (فراغ)
{
  لكل (state_item_number i = 0; i < nstate_items; ++i)
    {
      state_item *si = &state_items[i];
      𐡄𐡍 (item_number_is_symbol_number (*(si->item)) || !si->lookahead)
        استمر;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                        (ثابت فراغ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      طالما (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          𐡄𐡍 (SI_TRANSITION (prev))
            {
              bitset rsi = state_items[prev - state_items].revs;
              bitset_iterator biter;
              state_item_number sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_items[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = عدم;

ساكن فراغ
init_firsts (فراغ)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  لكل (rule_number i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_number *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      طالما (item_number_is_symbol_number (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      𐡄𐡍 (item_number_is_rule_number (*n) || ISVAR (*n))
        استمر;

      symbol_number lhs = r->lhs->number;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  طالما (change)
    {
      change = false;
      لكل (rule_number i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          symbol_number lhs = r->lhs->number;
          bitset f_lhs = FIRSTS (lhs);
          لكل (item_number *n = r->rhs;
               item_number_is_symbol_number (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              𐡄𐡍 (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              𐡄𐡍 (!nullable[*n - ntokens])
                اكسر;
            }
        }
    }
}

ساكن مضمن فراغ
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  𐡄𐡍 (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ساكن فراغ
prune_state_item (ثابت state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &si);

  طالما (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      𐡄𐡍 (SI_DISABLED (dsi - state_items))
        استمر;

      𐡄𐡍 (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ثابت state_item *trans = &state_items[dsi->trans];
        bitset_reset (trans->revs, dsi - state_items);
        𐡄𐡍 (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      𐡄𐡍 (dsi->prods)
        {
          bitset_iterator biter;
          state_item_number sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              𐡄𐡍 (SI_DISABLED (sin))
                استمر;
              ثابت state_item *prod = &state_items[sin];
              bitset_reset (prod->revs, dsi - state_items);
              𐡄𐡍 (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_number sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          𐡄𐡍 (SI_DISABLED (sin))
            استمر;
          state_item *rev = &state_items[sin];
          𐡄𐡍 (&state_items[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          وإلا 𐡄𐡍 (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_items);
              𐡄𐡍 (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          وإلا
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ساكن فراغ
prune_disabled_paths (فراغ)
{
  لكل (صحيح i = nstate_items - 1; i >= 0; --i)
    {
      state_item *si = &state_items[i];
      𐡄𐡍 (si->trans == -1 && item_number_is_symbol_number (*si->item))
        prune_state_item (si);
    }
}

فراغ
state_item_print (ثابت state_item *si, FILE *out, ثابت حرف *prefix)
{
  fputs (prefix, out);
  item_print (si->item, عدم, out);
  putc ('\n', out);
}

ثابت rule*
state_item_rule (ثابت state_item *si)
{
  أرجع item_rule (si->item);
}

/**
 * Report the state_item graph
 */
ساكن فراغ
state_items_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_items);
  لكل (state_number i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      لكل (state_item_number j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ثابت state_item *si = &state_items[j];
          item_print (si->item, عدم, out);
          𐡄𐡍 (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          وإلا
            {
              putc ('\n', out);
              𐡄𐡍 (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_items[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ثابت حرف *txt[2] = { "    => ", "    <- " };
              لكل (صحيح seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  𐡄𐡍 (b)
                    {
                      bitset_iterator biter;
                      state_item_number sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_items[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  لكل (symbol_number i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      symbol_number j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

فراغ
state_items_init (فراغ)
{
  xtime_t start = gethrxtime ();
  init_state_items ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  𐡄𐡍 (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_items_report (stderr);
    }
}

فراغ
state_items_free (فراغ)
{
  لكل (state_item_number i = 0; i < nstate_items; ++i)
    𐡄𐡍 (!SI_DISABLED (i))
      {
        state_item *si = &state_items[i];
        𐡄𐡍 (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  حرر (state_items);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ثابت state_item *si, ثابت state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  صحيح prec1 = s1->prec;
  صحيح prec2 = s2->prec;
  𐡄𐡍 (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      𐡄𐡍 (prec1 > prec2)
        أرجع false;
      // Do not expand if same precedence, but left-associative.
      𐡄𐡍 (prec1 == prec2 && s1->assoc == left_assoc)
        أرجع false;
    }
  أرجع true;
}
