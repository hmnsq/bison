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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "state-item.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <gethrxtime.h>
#અન્તર્ભાવ <gl_linked_list.h>
#અન્તર્ભાવ <gl_xlist.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "nullable.h"

size_t nstate_વસ્તુઓ;
state_item_સંખ્યા *state_item_map;
state_item *state_વસ્તુઓ;

// Hash functions for index -> bitset hash maps.
પ્રકારવ્યાખ્યા સંરચના
{
  પૂર્ણાંક ચાવી;
  bitset l;
} hash_pair;

સ્થિર size_t
hash_pair_hasher (અચલ hash_pair *sl, size_t max)
{
  પાછા sl->ચાવી % max;
}

સ્થિર bool
hash_pair_comparator (અચલ hash_pair *l, અચલ hash_pair *r)
{
  પાછા l->ચાવી == r->ચાવી;
}

સ્થિર શૂન્ય
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  મુક્ત (hp);
}

સ્થિર Hash_table *
hash_pair_table_create (પૂર્ણાંક size)
{
  પાછા hash_xinitialize (size,
                           ખાલી,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

સ્થિર bitset
hash_pair_lookup (Hash_table *tab, પૂર્ણાંક ચાવી)
{
  hash_pair probe;
  probe.ચાવી = ચાવી;
  hash_pair *hp = hash_lookup (tab, &probe);
  પાછા hp ? hp->l : ખાલી;
}

સ્થિર શૂન્ય
hash_pair_ઉમેરો (Hash_table *tab, પૂર્ણાંક ચાવી, bitset val)
{
  hash_pair *hp = xmalloc (કદ *hp);
  hp->ચાવી = ચાવી;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (શૂન્ય) res;
  પ્રતિજ્ઞા (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_સંખ્યા s, state_item_સંખ્યા off)
{
  પાછા &state_વસ્તુઓ[state_item_index_lookup (s, off)];
}

સ્થિર અંતઃરેખ શૂન્ય
state_item_set (state_item_સંખ્યા sidx, અચલ state *s, item_સંખ્યા off)
{
  state_વસ્તુઓ[sidx].state = s;
  state_વસ્તુઓ[sidx].item = &ritem[off];
  state_વસ્તુઓ[sidx].lookahead = ખાલી;
  state_વસ્તુઓ[sidx].trans = -1;
  state_વસ્તુઓ[sidx].prods = ખાલી;
  state_વસ્તુઓ[sidx].revs = bitset_create (nstate_વસ્તુઓ, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
સ્થિર શૂન્ય
init_state_વસ્તુઓ (શૂન્ય)
{
  nstate_વસ્તુઓ = 0;
  bitsetv production_વસ્તુઓ = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  માટે (પૂર્ણાંક i = 0; i < nstates; ++i)
    {
      અચલ state *s = states[i];
      nstate_વસ્તુઓ += s->nitems;
      closure (s->વસ્તુઓ, s->nitems);
      માટે (size_t j = 0; j < nitemset; ++j)
        જો (0 < itemset[j]
            && item_સંખ્યા_is_rule_સંખ્યા (ritem[itemset[j] - 1]))
          {
            bitset_set (production_વસ્તુઓ[i], itemset[j]);
            ++nstate_વસ્તુઓ;
          }
    }
  state_item_map = xnmalloc (nstates + 1, કદ (state_item_સંખ્યા));
  state_વસ્તુઓ = xnmalloc (nstate_વસ્તુઓ, કદ (state_item));
  state_item_સંખ્યા sidx = 0;
  માટે (પૂર્ણાંક i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      પૂર્ણાંક rule_શોધ_idx = 0;
      અચલ state *s = states[i];
      અચલ reductions *red = s->reductions;
      માટે (પૂર્ણાંક j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->વસ્તુઓ[j]);
          state_item *si = &state_વસ્તુઓ[sidx];
          અચલ rule *r = item_rule (si->item);
          જો (rule_શોધ_idx < red->અંક && red->rules[rule_શોધ_idx] < r)
            ++rule_શોધ_idx;
          જો (rule_શોધ_idx < red->અંક && r == red->rules[rule_શોધ_idx])
            {
              bitsetv lookahead = red->lookaheads;
              જો (lookahead)
                si->lookahead = lookahead[rule_શોધ_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_સંખ્યા off;
      BITSET_FOR_EACH (biter, production_વસ્તુઓ[i], off, 0)
        {
          state_item_set (sidx, s, off);
          જો (item_સંખ્યા_is_rule_સંખ્યા (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              જો (lookahead)
                state_વસ્તુઓ[sidx].lookahead = lookahead[rule_શોધ_idx];
              ++rule_શોધ_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_વસ્તુઓ;
  bitsetv_free (production_વસ્તુઓ);
}

સ્થિર size_t
state_sym_hasher (અચલ શૂન્ય *st, size_t max)
{
  પાછા ((state *) st)->accessing_પ્રતીક % max;
}

સ્થિર bool
state_sym_comparator (અચલ શૂન્ય *s1, અચલ શૂન્ય *s2)
{
  પાછા ((state *) s1)->accessing_પ્રતીક == ((state *) s2)->accessing_પ્રતીક;
}

સ્થિર state *
state_sym_lookup (પ્રતીક_સંખ્યા sym, Hash_table *h)
{
  state probe;
  probe.accessing_પ્રતીક = sym;
  પાછા hash_lookup (h, &probe);
}

સ્થિર શૂન્ય
init_trans (શૂન્ય)
{
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->અંક, ખાલી, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, ખાલી);
      માટે (પૂર્ણાંક j = 0; j < t->અંક; ++j)
        જો (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      માટે (state_item_સંખ્યા j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_સંખ્યા *item = state_વસ્તુઓ[j].item;
          જો (item_સંખ્યા_is_rule_સંખ્યા (*item))
            ચાલુ;
          state *dst = state_sym_lookup (*item, transition_set);
          જો (!dst)
            ચાલુ;
          // find the item in the destination state that corresponds
          // to the transition of item
          માટે (પૂર્ણાંક k = 0; k < dst->nitems; ++k)
            જો (item + 1 == ritem + dst->વસ્તુઓ[k])
              {
                state_item_સંખ્યા dstSI =
                  state_item_index_lookup (dst->સંખ્યા, k);

                state_વસ્તુઓ[j].trans = dstSI;
                bitset_set (state_વસ્તુઓ[dstSI].revs, j);
                તોડો;
            }
        }
      hash_free (transition_set);
    }
}

સ્થિર શૂન્ય
init_prods (શૂન્ય)
{
  માટે (પૂર્ણાંક i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      માટે (state_item_સંખ્યા j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_વસ્તુઓ[j];
          item_સંખ્યા *item = src->item;
          પ્રતીક_સંખ્યા lhs = item_rule (item)->lhs->સંખ્યા;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          જો (!itms)
            {
              itms = bitset_create (nstate_વસ્તુઓ, BITSET_SPARSE);
              hash_pair_ઉમેરો (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      માટે (state_item_સંખ્યા j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_વસ્તુઓ[j];
          item_સંખ્યા item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          જો (item_સંખ્યા_is_rule_સંખ્યા (item) || ISTOKEN (item))
            ચાલુ;
          પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          જો (lb)
            {
              bitset copy = bitset_create (nstate_વસ્તુઓ, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_વસ્તુઓ[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_સંખ્યા prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_વસ્તુઓ[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
સ્થિર અંતઃરેખ શૂન્ય
gen_lookaheads (શૂન્ય)
{
  માટે (state_item_સંખ્યા i = 0; i < nstate_વસ્તુઓ; ++i)
    {
      state_item *si = &state_વસ્તુઓ[i];
      જો (item_સંખ્યા_is_પ્રતીક_સંખ્યા (*(si->item)) || !si->lookahead)
        ચાલુ;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true, 1,
                        (અચલ શૂન્ય **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      જ્યારે (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          જો (SI_TRANSITION (prev))
            {
              bitset rsi = state_વસ્તુઓ[prev - state_વસ્તુઓ].revs;
              bitset_iterator biter;
              state_item_સંખ્યા sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_વસ્તુઓ[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = ખાલી;

સ્થિર શૂન્ય
init_firsts (શૂન્ય)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  માટે (rule_સંખ્યા i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_સંખ્યા *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      જ્યારે (item_સંખ્યા_is_પ્રતીક_સંખ્યા (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      જો (item_સંખ્યા_is_rule_સંખ્યા (*n) || ISVAR (*n))
        ચાલુ;

      પ્રતીક_સંખ્યા lhs = r->lhs->સંખ્યા;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  જ્યારે (change)
    {
      change = false;
      માટે (rule_સંખ્યા i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          પ્રતીક_સંખ્યા lhs = r->lhs->સંખ્યા;
          bitset f_lhs = FIRSTS (lhs);
          માટે (item_સંખ્યા *n = r->rhs;
               item_સંખ્યા_is_પ્રતીક_સંખ્યા (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              જો (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              જો (!nullable[*n - ntokens])
                તોડો;
            }
        }
    }
}

સ્થિર અંતઃરેખ શૂન્ય
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  જો (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
સ્થિર શૂન્ય
prune_state_item (અચલ state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true, 1,
                    (અચલ શૂન્ય **) &si);

  જ્યારે (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      જો (SI_DISABLED (dsi - state_વસ્તુઓ))
        ચાલુ;

      જો (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        અચલ state_item *trans = &state_વસ્તુઓ[dsi->trans];
        bitset_reset (trans->revs, dsi - state_વસ્તુઓ);
        જો (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      જો (dsi->prods)
        {
          bitset_iterator biter;
          state_item_સંખ્યા sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              જો (SI_DISABLED (sin))
                ચાલુ;
              અચલ state_item *prod = &state_વસ્તુઓ[sin];
              bitset_reset (prod->revs, dsi - state_વસ્તુઓ);
              જો (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_સંખ્યા sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          જો (SI_DISABLED (sin))
            ચાલુ;
          state_item *rev = &state_વસ્તુઓ[sin];
          જો (&state_વસ્તુઓ[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          નહીંતર જો (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_વસ્તુઓ);
              જો (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          નહીંતર
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
સ્થિર શૂન્ય
prune_disabled_paths (શૂન્ય)
{
  માટે (પૂર્ણાંક i = nstate_વસ્તુઓ - 1; i >= 0; --i)
    {
      state_item *si = &state_વસ્તુઓ[i];
      જો (si->trans == -1 && item_સંખ્યા_is_પ્રતીક_સંખ્યા (*si->item))
        prune_state_item (si);
    }
}

શૂન્ય
state_item_print (અચલ state_item *si, FILE *out, અચલ અક્ષર *prefix)
{
  fputs (prefix, out);
  item_print (si->item, ખાલી, out);
  putc ('\n', out);
}

અચલ rule*
state_item_rule (અચલ state_item *si)
{
  પાછા item_rule (si->item);
}

/**
 * Report the state_item graph
 */
સ્થિર શૂન્ય
state_વસ્તુઓ_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_વસ્તુઓ);
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      માટે (state_item_સંખ્યા j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          અચલ state_item *si = &state_વસ્તુઓ[j];
          item_print (si->item, ખાલી, out);
          જો (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          નહીંતર
            {
              putc ('\n', out);
              જો (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_વસ્તુઓ[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              અચલ અક્ષર *txt[2] = { "    => ", "    <- " };
              માટે (પૂર્ણાંક seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  જો (b)
                    {
                      bitset_iterator biter;
                      state_item_સંખ્યા sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_વસ્તુઓ[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      પ્રતીક_સંખ્યા j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

શૂન્ય
state_વસ્તુઓ_init (શૂન્ય)
{
  xtime_t start = gethrxtime ();
  init_state_વસ્તુઓ ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  જો (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_વસ્તુઓ_report (stderr);
    }
}

શૂન્ય
state_વસ્તુઓ_free (શૂન્ય)
{
  માટે (state_item_સંખ્યા i = 0; i < nstate_વસ્તુઓ; ++i)
    જો (!SI_DISABLED (i))
      {
        state_item *si = &state_વસ્તુઓ[i];
        જો (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  મુક્ત (state_વસ્તુઓ);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (અચલ state_item *si, અચલ state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  પૂર્ણાંક prec1 = s1->prec;
  પૂર્ણાંક prec2 = s2->prec;
  જો (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      જો (prec1 > prec2)
        પાછા false;
      // Do not expand if same precedence, but left-associative.
      જો (prec1 == prec2 && s1->assoc == ડાબું_assoc)
        પાછા false;
    }
  પાછા true;
}
