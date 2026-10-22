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

#شامل <config.h>

#شامل "state-item.h"

#شامل <assert.h>
#شامل <gethrxtime.h>
#شامل <gl_linked_list.h>
#شامل <gl_xlist.h>
#شامل <stdlib.h>
#شامل <time.h>

#شامل "closure.h"
#شامل "getargs.h"
#شامل "nullable.h"

size_t nstate_اقلام;
state_item_عدد *state_item_map;
state_item *state_اقلام;

// Hash functions for index -> bitset hash maps.
تعریفنوع ساختار
{
  صحیح کلید;
  bitset l;
} hash_pair;

ایستا size_t
hash_pair_hasher (ثابت hash_pair *sl, size_t max)
{
  بازگشت sl->کلید % max;
}

ایستا bool
hash_pair_comparator (ثابت hash_pair *l, ثابت hash_pair *r)
{
  بازگشت l->کلید == r->کلید;
}

ایستا تهی
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  آزاد (hp);
}

ایستا Hash_table *
hash_pair_table_create (صحیح size)
{
  بازگشت hash_xinitialize (size,
                           پوچ,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

ایستا bitset
hash_pair_lookup (Hash_table *tab, صحیح کلید)
{
  hash_pair probe;
  probe.کلید = کلید;
  hash_pair *hp = hash_lookup (tab, &probe);
  بازگشت hp ? hp->l : پوچ;
}

ایستا تهی
hash_pair_درج (Hash_table *tab, صحیح کلید, bitset val)
{
  hash_pair *hp = xmalloc (اندازه *hp);
  hp->کلید = کلید;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (تهی) res;
  أكد (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_عدد s, state_item_عدد off)
{
  بازگشت &state_اقلام[state_item_index_lookup (s, off)];
}

ایستا درونخطی تهی
state_item_set (state_item_عدد sidx, ثابت state *s, item_عدد off)
{
  state_اقلام[sidx].state = s;
  state_اقلام[sidx].item = &ritem[off];
  state_اقلام[sidx].lookahead = پوچ;
  state_اقلام[sidx].trans = -1;
  state_اقلام[sidx].prods = پوچ;
  state_اقلام[sidx].revs = bitset_create (nstate_اقلام, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ایستا تهی
init_state_اقلام (تهی)
{
  nstate_اقلام = 0;
  bitsetv production_اقلام = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  برای (صحیح i = 0; i < nstates; ++i)
    {
      ثابت state *s = states[i];
      nstate_اقلام += s->nitems;
      closure (s->اقلام, s->nitems);
      برای (size_t j = 0; j < nitemset; ++j)
        اگر (0 < itemset[j]
            && item_عدد_is_rule_عدد (ritem[itemset[j] - 1]))
          {
            bitset_set (production_اقلام[i], itemset[j]);
            ++nstate_اقلام;
          }
    }
  state_item_map = xnmalloc (nstates + 1, اندازه (state_item_عدد));
  state_اقلام = xnmalloc (nstate_اقلام, اندازه (state_item));
  state_item_عدد sidx = 0;
  برای (صحیح i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      صحیح rule_جستجو_idx = 0;
      ثابت state *s = states[i];
      ثابت reductions *red = s->reductions;
      برای (صحیح j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->اقلام[j]);
          state_item *si = &state_اقلام[sidx];
          ثابت rule *r = item_rule (si->item);
          اگر (rule_جستجو_idx < red->رقم && red->rules[rule_جستجو_idx] < r)
            ++rule_جستجو_idx;
          اگر (rule_جستجو_idx < red->رقم && r == red->rules[rule_جستجو_idx])
            {
              bitsetv lookahead = red->lookaheads;
              اگر (lookahead)
                si->lookahead = lookahead[rule_جستجو_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_عدد off;
      BITSET_FOR_EACH (biter, production_اقلام[i], off, 0)
        {
          state_item_set (sidx, s, off);
          اگر (item_عدد_is_rule_عدد (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              اگر (lookahead)
                state_اقلام[sidx].lookahead = lookahead[rule_جستجو_idx];
              ++rule_جستجو_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_اقلام;
  bitsetv_free (production_اقلام);
}

ایستا size_t
state_sym_hasher (ثابت تهی *st, size_t max)
{
  بازگشت ((state *) st)->accessing_نماد % max;
}

ایستا bool
state_sym_comparator (ثابت تهی *s1, ثابت تهی *s2)
{
  بازگشت ((state *) s1)->accessing_نماد == ((state *) s2)->accessing_نماد;
}

ایستا state *
state_sym_lookup (نماد_عدد sym, Hash_table *h)
{
  state probe;
  probe.accessing_نماد = sym;
  بازگشت hash_lookup (h, &probe);
}

ایستا تهی
init_trans (تهی)
{
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->رقم, پوچ, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, پوچ);
      برای (صحیح j = 0; j < t->رقم; ++j)
        اگر (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      برای (state_item_عدد j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_عدد *item = state_اقلام[j].item;
          اگر (item_عدد_is_rule_عدد (*item))
            ادامه;
          state *dst = state_sym_lookup (*item, transition_set);
          اگر (!dst)
            ادامه;
          // find the item in the destination state that corresponds
          // to the transition of item
          برای (صحیح k = 0; k < dst->nitems; ++k)
            اگر (item + 1 == ritem + dst->اقلام[k])
              {
                state_item_عدد dstSI =
                  state_item_index_lookup (dst->عدد, k);

                state_اقلام[j].trans = dstSI;
                bitset_set (state_اقلام[dstSI].revs, j);
                بشکن;
            }
        }
      hash_free (transition_set);
    }
}

ایستا تهی
init_prods (تهی)
{
  برای (صحیح i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      برای (state_item_عدد j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_اقلام[j];
          item_عدد *item = src->item;
          نماد_عدد lhs = item_rule (item)->lhs->عدد;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          اگر (!itms)
            {
              itms = bitset_create (nstate_اقلام, BITSET_SPARSE);
              hash_pair_درج (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      برای (state_item_عدد j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_اقلام[j];
          item_عدد item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          اگر (item_عدد_is_rule_عدد (item) || ISTOKEN (item))
            ادامه;
          نماد_عدد sym = item_عدد_as_نماد_عدد (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          اگر (lb)
            {
              bitset copy = bitset_create (nstate_اقلام, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_اقلام[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_عدد prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_اقلام[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ایستا درونخطی تهی
gen_lookaheads (تهی)
{
  برای (state_item_عدد i = 0; i < nstate_اقلام; ++i)
    {
      state_item *si = &state_اقلام[i];
      اگر (item_عدد_is_نماد_عدد (*(si->item)) || !si->lookahead)
        ادامه;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, پوچ, پوچ, پوچ, true, 1,
                        (ثابت تهی **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      وقتی (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          اگر (SI_TRANSITION (prev))
            {
              bitset rsi = state_اقلام[prev - state_اقلام].revs;
              bitset_iterator biter;
              state_item_عدد sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_اقلام[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = پوچ;

ایستا تهی
init_firsts (تهی)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  برای (rule_عدد i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_عدد *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      وقتی (item_عدد_is_نماد_عدد (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      اگر (item_عدد_is_rule_عدد (*n) || ISVAR (*n))
        ادامه;

      نماد_عدد lhs = r->lhs->عدد;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  وقتی (change)
    {
      change = false;
      برای (rule_عدد i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          نماد_عدد lhs = r->lhs->عدد;
          bitset f_lhs = FIRSTS (lhs);
          برای (item_عدد *n = r->rhs;
               item_عدد_is_نماد_عدد (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              اگر (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              اگر (!nullable[*n - ntokens])
                بشکن;
            }
        }
    }
}

ایستا درونخطی تهی
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  اگر (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ایستا تهی
prune_state_item (ثابت state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, پوچ, پوچ, پوچ, true, 1,
                    (ثابت تهی **) &si);

  وقتی (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      اگر (SI_DISABLED (dsi - state_اقلام))
        ادامه;

      اگر (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ثابت state_item *trans = &state_اقلام[dsi->trans];
        bitset_reset (trans->revs, dsi - state_اقلام);
        اگر (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      اگر (dsi->prods)
        {
          bitset_iterator biter;
          state_item_عدد sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              اگر (SI_DISABLED (sin))
                ادامه;
              ثابت state_item *prod = &state_اقلام[sin];
              bitset_reset (prod->revs, dsi - state_اقلام);
              اگر (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_عدد sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          اگر (SI_DISABLED (sin))
            ادامه;
          state_item *rev = &state_اقلام[sin];
          اگر (&state_اقلام[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          وگرنه اگر (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_اقلام);
              اگر (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          وگرنه
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ایستا تهی
prune_disabled_paths (تهی)
{
  برای (صحیح i = nstate_اقلام - 1; i >= 0; --i)
    {
      state_item *si = &state_اقلام[i];
      اگر (si->trans == -1 && item_عدد_is_نماد_عدد (*si->item))
        prune_state_item (si);
    }
}

تهی
state_item_print (ثابت state_item *si, FILE *out, ثابت نویسه *prefix)
{
  fputs (prefix, out);
  item_print (si->item, پوچ, out);
  putc ('\n', out);
}

ثابت rule*
state_item_rule (ثابت state_item *si)
{
  بازگشت item_rule (si->item);
}

/**
 * Report the state_item graph
 */
ایستا تهی
state_اقلام_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_اقلام);
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      برای (state_item_عدد j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ثابت state_item *si = &state_اقلام[j];
          item_print (si->item, پوچ, out);
          اگر (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          وگرنه
            {
              putc ('\n', out);
              اگر (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_اقلام[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ثابت نویسه *txt[2] = { "    => ", "    <- " };
              برای (صحیح seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  اگر (b)
                    {
                      bitset_iterator biter;
                      state_item_عدد sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_اقلام[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      نماد_عدد j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

تهی
state_اقلام_init (تهی)
{
  xtime_t start = gethrxtime ();
  init_state_اقلام ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  اگر (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_اقلام_report (stderr);
    }
}

تهی
state_اقلام_free (تهی)
{
  برای (state_item_عدد i = 0; i < nstate_اقلام; ++i)
    اگر (!SI_DISABLED (i))
      {
        state_item *si = &state_اقلام[i];
        اگر (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  آزاد (state_اقلام);
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
  صحیح prec1 = s1->prec;
  صحیح prec2 = s2->prec;
  اگر (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      اگر (prec1 > prec2)
        بازگشت false;
      // Do not expand if same precedence, but left-associative.
      اگر (prec1 == prec2 && s1->assoc == چپ_assoc)
        بازگشت false;
    }
  بازگشت true;
}
