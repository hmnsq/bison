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

size_t nstate_ابوبو;
state_item_لمب *state_item_map;
state_item *state_ابوبو;

// Hash functions for index -> bitset hash maps.
تعريفنوع بنية
{
  صحيح مبودي;
  bitset l;
} hash_pair;

ساكن size_t
hash_pair_hasher (ثابت hash_pair *sl, size_t max)
{
  أرجع sl->مبودي % max;
}

ساكن bool
hash_pair_comparator (ثابت hash_pair *l, ثابت hash_pair *r)
{
  أرجع l->مبودي == r->مبودي;
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
hash_pair_lookup (Hash_table *tab, صحيح مبودي)
{
  hash_pair probe;
  probe.مبودي = مبودي;
  hash_pair *hp = hash_lookup (tab, &probe);
  أرجع hp ? hp->l : عدم;
}

ساكن فراغ
hash_pair_سك (Hash_table *tab, صحيح مبودي, bitset val)
{
  hash_pair *hp = xmalloc (حجم *hp);
  hp->مبودي = مبودي;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (فراغ) res;
  أكد (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_لمب s, state_item_لمب off)
{
  أرجع &state_ابوبو[state_item_index_lookup (s, off)];
}

ساكن مضمن فراغ
state_item_set (state_item_لمب sidx, ثابت state *s, item_لمب off)
{
  state_ابوبو[sidx].state = s;
  state_ابوبو[sidx].item = &ritem[off];
  state_ابوبو[sidx].lookahead = عدم;
  state_ابوبو[sidx].trans = -1;
  state_ابوبو[sidx].prods = عدم;
  state_ابوبو[sidx].revs = bitset_create (nstate_ابوبو, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ساكن فراغ
init_state_ابوبو (فراغ)
{
  nstate_ابوبو = 0;
  bitsetv production_ابوبو = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      ثابت state *s = states[i];
      nstate_ابوبو += s->nitems;
      closure (s->ابوبو, s->nitems);
      لكل (size_t j = 0; j < nitemset; ++j)
        إذا (0 < itemset[j]
            && item_لمب_is_rule_لمب (ritem[itemset[j] - 1]))
          {
            bitset_set (production_ابوبو[i], itemset[j]);
            ++nstate_ابوبو;
          }
    }
  state_item_map = xnmalloc (nstates + 1, حجم (state_item_لمب));
  state_ابوبو = xnmalloc (nstate_ابوبو, حجم (state_item));
  state_item_لمب sidx = 0;
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      صحيح rule_نيما_idx = 0;
      ثابت state *s = states[i];
      ثابت reductions *red = s->reductions;
      لكل (صحيح j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->ابوبو[j]);
          state_item *si = &state_ابوبو[sidx];
          ثابت rule *r = item_rule (si->item);
          إذا (rule_نيما_idx < red->لمب && red->rules[rule_نيما_idx] < r)
            ++rule_نيما_idx;
          إذا (rule_نيما_idx < red->لمب && r == red->rules[rule_نيما_idx])
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                si->lookahead = lookahead[rule_نيما_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_لمب off;
      BITSET_FOR_EACH (biter, production_ابوبو[i], off, 0)
        {
          state_item_set (sidx, s, off);
          إذا (item_لمب_is_rule_لمب (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                state_ابوبو[sidx].lookahead = lookahead[rule_نيما_idx];
              ++rule_نيما_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_ابوبو;
  bitsetv_free (production_ابوبو);
}

ساكن size_t
state_sym_hasher (ثابت فراغ *st, size_t max)
{
  أرجع ((state *) st)->accessing_علام % max;
}

ساكن bool
state_sym_comparator (ثابت فراغ *s1, ثابت فراغ *s2)
{
  أرجع ((state *) s1)->accessing_علام == ((state *) s2)->accessing_علام;
}

ساكن state *
state_sym_lookup (علام_لمب sym, Hash_table *h)
{
  state probe;
  probe.accessing_علام = sym;
  أرجع hash_lookup (h, &probe);
}

ساكن فراغ
init_trans (فراغ)
{
  لكل (state_لمب i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->لمب, عدم, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, عدم);
      لكل (صحيح j = 0; j < t->لمب; ++j)
        إذا (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      لكل (state_item_لمب j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_لمب *item = state_ابوبو[j].item;
          إذا (item_لمب_is_rule_لمب (*item))
            استمر;
          state *dst = state_sym_lookup (*item, transition_set);
          إذا (!dst)
            استمر;
          // find the item in the destination state that corresponds
          // to the transition of item
          لكل (صحيح k = 0; k < dst->nitems; ++k)
            إذا (item + 1 == ritem + dst->ابوبو[k])
              {
                state_item_لمب dstSI =
                  state_item_index_lookup (dst->لمب_2, k);

                state_ابوبو[j].trans = dstSI;
                bitset_set (state_ابوبو[dstSI].revs, j);
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
      لكل (state_item_لمب j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ابوبو[j];
          item_لمب *item = src->item;
          علام_لمب lhs = item_rule (item)->lhs->لمب_2;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          إذا (!itms)
            {
              itms = bitset_create (nstate_ابوبو, BITSET_SPARSE);
              hash_pair_سك (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      لكل (state_item_لمب j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ابوبو[j];
          item_لمب item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          إذا (item_لمب_is_rule_لمب (item) || ISTOKEN (item))
            استمر;
          علام_لمب sym = item_لمب_as_علام_لمب (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          إذا (lb)
            {
              bitset copy = bitset_create (nstate_ابوبو, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_ابوبو[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_لمب prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_ابوبو[prod].revs, j);
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
  لكل (state_item_لمب i = 0; i < nstate_ابوبو; ++i)
    {
      state_item *si = &state_ابوبو[i];
      إذا (item_لمب_is_علام_لمب (*(si->item)) || !si->lookahead)
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
          إذا (SI_TRANSITION (prev))
            {
              bitset rsi = state_ابوبو[prev - state_ابوبو].revs;
              bitset_iterator biter;
              state_item_لمب sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_ابوبو[sin]);
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
  لكل (rule_لمب i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_لمب *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      طالما (item_لمب_is_علام_لمب (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      إذا (item_لمب_is_rule_لمب (*n) || ISVAR (*n))
        استمر;

      علام_لمب lhs = r->lhs->لمب_2;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  طالما (change)
    {
      change = false;
      لكل (rule_لمب i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          علام_لمب lhs = r->lhs->لمب_2;
          bitset f_lhs = FIRSTS (lhs);
          لكل (item_لمب *n = r->rhs;
               item_لمب_is_علام_لمب (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              إذا (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              إذا (!nullable[*n - ntokens])
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
  إذا (si->prods)
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
      إذا (SI_DISABLED (dsi - state_ابوبو))
        استمر;

      إذا (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ثابت state_item *trans = &state_ابوبو[dsi->trans];
        bitset_reset (trans->revs, dsi - state_ابوبو);
        إذا (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      إذا (dsi->prods)
        {
          bitset_iterator biter;
          state_item_لمب sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              إذا (SI_DISABLED (sin))
                استمر;
              ثابت state_item *prod = &state_ابوبو[sin];
              bitset_reset (prod->revs, dsi - state_ابوبو);
              إذا (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_لمب sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          إذا (SI_DISABLED (sin))
            استمر;
          state_item *rev = &state_ابوبو[sin];
          إذا (&state_ابوبو[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          وإلا إذا (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_ابوبو);
              إذا (bitset_empty_p (rev->prods))
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
  لكل (صحيح i = nstate_ابوبو - 1; i >= 0; --i)
    {
      state_item *si = &state_ابوبو[i];
      إذا (si->trans == -1 && item_لمب_is_علام_لمب (*si->item))
        prune_state_item (si);
    }
}

فراغ
state_item_بغ (ثابت state_item *si, FILE *out, ثابت حرف *prefix)
{
  fputs (prefix, out);
  item_بغ (si->item, عدم, out);
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
state_ابوبو_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_ابوبو);
  لكل (state_لمب i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      لكل (state_item_لمب j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ثابت state_item *si = &state_ابوبو[j];
          item_بغ (si->item, عدم, out);
          إذا (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          وإلا
            {
              putc ('\n', out);
              إذا (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_بغ (&state_ابوبو[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ثابت حرف *txt[2] = { "    => ", "    <- " };
              لكل (صحيح seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  إذا (b)
                    {
                      bitset_iterator biter;
                      state_item_لمب sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_بغ (&state_ابوبو[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  لكل (علام_لمب i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      علام_لمب j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

فراغ
state_ابوبو_init (فراغ)
{
  xtime_t start = gethrxtime ();
  init_state_ابوبو ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  إذا (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_ابوبو_report (stderr);
    }
}

فراغ
state_ابوبو_free (فراغ)
{
  لكل (state_item_لمب i = 0; i < nstate_ابوبو; ++i)
    إذا (!SI_DISABLED (i))
      {
        state_item *si = &state_ابوبو[i];
        إذا (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  حرر (state_ابوبو);
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
  إذا (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      إذا (prec1 > prec2)
        أرجع false;
      // Do not expand if same precedence, but left-associative.
      إذا (prec1 == prec2 && s1->assoc == هغو_assoc)
        أرجع false;
    }
  أرجع true;
}
