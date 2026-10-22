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

#अन्तर्भाव <config.h>

#अन्तर्भाव "state-item.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <gethrxtime.h>
#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "nullable.h"

माप_प्रकार nstate_वस्तुएँ;
state_item_संख्या *state_item_map;
state_item *state_वस्तुएँ;

// Hash functions for index -> bitset hash maps.
प्रकारनिर्देश संरचना
{
  पूर्णाङ्क कुंजी;
  bitset l;
} hash_pair;

स्थिर माप_प्रकार
hash_pair_hasher (ध्रुव hash_pair *sl, माप_प्रकार max)
{
  प्रत्यागम sl->कुंजी % max;
}

स्थिर bool
hash_pair_comparator (ध्रुव hash_pair *l, ध्रुव hash_pair *r)
{
  प्रत्यागम l->कुंजी == r->कुंजी;
}

स्थिर शून्य
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  मुक्त (hp);
}

स्थिर Hash_table *
hash_pair_table_create (पूर्णाङ्क size)
{
  प्रत्यागम hash_xinitialize (size,
                           रिक्त,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

स्थिर bitset
hash_pair_lookup (Hash_table *tab, पूर्णाङ्क कुंजी)
{
  hash_pair probe;
  probe.कुंजी = कुंजी;
  hash_pair *hp = hash_lookup (tab, &probe);
  प्रत्यागम hp ? hp->l : रिक्त;
}

स्थिर शून्य
hash_pair_जोड़ो (Hash_table *tab, पूर्णाङ्क कुंजी, bitset val)
{
  hash_pair *hp = xmalloc (परिमाण *hp);
  hp->कुंजी = कुंजी;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (शून्य) res;
  प्रतिज्ञा (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_संख्या s, state_item_संख्या off)
{
  प्रत्यागम &state_वस्तुएँ[state_item_index_lookup (s, off)];
}

स्थिर अन्तःस्थ शून्य
state_item_set (state_item_संख्या sidx, ध्रुव state *s, item_संख्या off)
{
  state_वस्तुएँ[sidx].state = s;
  state_वस्तुएँ[sidx].item = &ritem[off];
  state_वस्तुएँ[sidx].lookahead = रिक्त;
  state_वस्तुएँ[sidx].trans = -1;
  state_वस्तुएँ[sidx].prods = रिक्त;
  state_वस्तुएँ[sidx].revs = bitset_create (nstate_वस्तुएँ, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
स्थिर शून्य
init_state_वस्तुएँ (शून्य)
{
  nstate_वस्तुएँ = 0;
  bitsetv production_वस्तुएँ = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  प्रति (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      ध्रुव state *s = states[i];
      nstate_वस्तुएँ += s->nitems;
      closure (s->वस्तुएँ, s->nitems);
      प्रति (माप_प्रकार j = 0; j < nitemset; ++j)
        यदि (0 < itemset[j]
            && item_संख्या_is_rule_संख्या (ritem[itemset[j] - 1]))
          {
            bitset_set (production_वस्तुएँ[i], itemset[j]);
            ++nstate_वस्तुएँ;
          }
    }
  state_item_map = xnmalloc (nstates + 1, परिमाण (state_item_संख्या));
  state_वस्तुएँ = xnmalloc (nstate_वस्तुएँ, परिमाण (state_item));
  state_item_संख्या sidx = 0;
  प्रति (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      पूर्णाङ्क rule_खोज_idx = 0;
      ध्रुव state *s = states[i];
      ध्रुव reductions *red = s->reductions;
      प्रति (पूर्णाङ्क j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->वस्तुएँ[j]);
          state_item *si = &state_वस्तुएँ[sidx];
          ध्रुव rule *r = item_rule (si->item);
          यदि (rule_खोज_idx < red->अंक && red->rules[rule_खोज_idx] < r)
            ++rule_खोज_idx;
          यदि (rule_खोज_idx < red->अंक && r == red->rules[rule_खोज_idx])
            {
              bitsetv lookahead = red->lookaheads;
              यदि (lookahead)
                si->lookahead = lookahead[rule_खोज_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_संख्या off;
      BITSET_FOR_EACH (biter, production_वस्तुएँ[i], off, 0)
        {
          state_item_set (sidx, s, off);
          यदि (item_संख्या_is_rule_संख्या (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              यदि (lookahead)
                state_वस्तुएँ[sidx].lookahead = lookahead[rule_खोज_idx];
              ++rule_खोज_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_वस्तुएँ;
  bitsetv_free (production_वस्तुएँ);
}

स्थिर माप_प्रकार
state_sym_hasher (ध्रुव शून्य *st, माप_प्रकार max)
{
  प्रत्यागम ((state *) st)->accessing_प्रतीक % max;
}

स्थिर bool
state_sym_comparator (ध्रुव शून्य *s1, ध्रुव शून्य *s2)
{
  प्रत्यागम ((state *) s1)->accessing_प्रतीक == ((state *) s2)->accessing_प्रतीक;
}

स्थिर state *
state_sym_lookup (प्रतीक_संख्या sym, Hash_table *h)
{
  state probe;
  probe.accessing_प्रतीक = sym;
  प्रत्यागम hash_lookup (h, &probe);
}

स्थिर शून्य
init_trans (शून्य)
{
  प्रति (state_संख्या i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->अंक, रिक्त, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, रिक्त);
      प्रति (पूर्णाङ्क j = 0; j < t->अंक; ++j)
        यदि (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      प्रति (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_संख्या *item = state_वस्तुएँ[j].item;
          यदि (item_संख्या_is_rule_संख्या (*item))
            अनुवर्त;
          state *dst = state_sym_lookup (*item, transition_set);
          यदि (!dst)
            अनुवर्त;
          // find the item in the destination state that corresponds
          // to the transition of item
          प्रति (पूर्णाङ्क k = 0; k < dst->nitems; ++k)
            यदि (item + 1 == ritem + dst->वस्तुएँ[k])
              {
                state_item_संख्या dstSI =
                  state_item_index_lookup (dst->संख्या, k);

                state_वस्तुएँ[j].trans = dstSI;
                bitset_set (state_वस्तुएँ[dstSI].revs, j);
                भङ्ग;
            }
        }
      hash_free (transition_set);
    }
}

स्थिर शून्य
init_prods (शून्य)
{
  प्रति (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      प्रति (state_item_संख्या j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तुएँ[j];
          item_संख्या *item = src->item;
          प्रतीक_संख्या lhs = item_rule (item)->lhs->संख्या;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          यदि (!itms)
            {
              itms = bitset_create (nstate_वस्तुएँ, BITSET_SPARSE);
              hash_pair_जोड़ो (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      प्रति (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तुएँ[j];
          item_संख्या item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          यदि (item_संख्या_is_rule_संख्या (item) || ISTOKEN (item))
            अनुवर्त;
          प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          यदि (lb)
            {
              bitset copy = bitset_create (nstate_वस्तुएँ, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_वस्तुएँ[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_संख्या prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_वस्तुएँ[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
स्थिर अन्तःस्थ शून्य
gen_lookaheads (शून्य)
{
  प्रति (state_item_संख्या i = 0; i < nstate_वस्तुएँ; ++i)
    {
      state_item *si = &state_वस्तुएँ[i];
      यदि (item_संख्या_is_प्रतीक_संख्या (*(si->item)) || !si->lookahead)
        अनुवर्त;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                        (ध्रुव शून्य **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      यावत् (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          यदि (SI_TRANSITION (prev))
            {
              bitset rsi = state_वस्तुएँ[prev - state_वस्तुएँ].revs;
              bitset_iterator biter;
              state_item_संख्या sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_वस्तुएँ[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = रिक्त;

स्थिर शून्य
init_firsts (शून्य)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  प्रति (rule_संख्या i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_संख्या *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      यावत् (item_संख्या_is_प्रतीक_संख्या (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      यदि (item_संख्या_is_rule_संख्या (*n) || ISVAR (*n))
        अनुवर्त;

      प्रतीक_संख्या lhs = r->lhs->संख्या;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  यावत् (change)
    {
      change = false;
      प्रति (rule_संख्या i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          प्रतीक_संख्या lhs = r->lhs->संख्या;
          bitset f_lhs = FIRSTS (lhs);
          प्रति (item_संख्या *n = r->rhs;
               item_संख्या_is_प्रतीक_संख्या (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              यदि (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              यदि (!nullable[*n - ntokens])
                भङ्ग;
            }
        }
    }
}

स्थिर अन्तःस्थ शून्य
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  यदि (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
स्थिर शून्य
prune_state_item (ध्रुव state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                    (ध्रुव शून्य **) &si);

  यावत् (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      यदि (SI_DISABLED (dsi - state_वस्तुएँ))
        अनुवर्त;

      यदि (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ध्रुव state_item *trans = &state_वस्तुएँ[dsi->trans];
        bitset_reset (trans->revs, dsi - state_वस्तुएँ);
        यदि (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      यदि (dsi->prods)
        {
          bitset_iterator biter;
          state_item_संख्या sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              यदि (SI_DISABLED (sin))
                अनुवर्त;
              ध्रुव state_item *prod = &state_वस्तुएँ[sin];
              bitset_reset (prod->revs, dsi - state_वस्तुएँ);
              यदि (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          यदि (SI_DISABLED (sin))
            अनुवर्त;
          state_item *rev = &state_वस्तुएँ[sin];
          यदि (&state_वस्तुएँ[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          अन्यथा यदि (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_वस्तुएँ);
              यदि (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          अन्यथा
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
स्थिर शून्य
prune_disabled_paths (शून्य)
{
  प्रति (पूर्णाङ्क i = nstate_वस्तुएँ - 1; i >= 0; --i)
    {
      state_item *si = &state_वस्तुएँ[i];
      यदि (si->trans == -1 && item_संख्या_is_प्रतीक_संख्या (*si->item))
        prune_state_item (si);
    }
}

शून्य
state_item_print (ध्रुव state_item *si, खाता *out, ध्रुव वर्ण *prefix)
{
  ख_माला_दो (prefix, out);
  item_print (si->item, रिक्त, out);
  अ_दो ('\n', out);
}

ध्रुव rule*
state_item_rule (ध्रुव state_item *si)
{
  प्रत्यागम item_rule (si->item);
}

/**
 * Report the state_item graph
 */
स्थिर शून्य
state_वस्तुएँ_report (खाता *out)
{
  ख_लिखो (out, "# state items: %zu\n", nstate_वस्तुएँ);
  प्रति (state_संख्या i = 0; i < nstates; ++i)
    {
      ख_लिखो (out, "State %d:\n", i);
      प्रति (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ध्रुव state_item *si = &state_वस्तुएँ[j];
          item_print (si->item, रिक्त, out);
          यदि (SI_DISABLED (j))
            ख_माला_दो ("  DISABLED\n", out);
          अन्यथा
            {
              अ_दो ('\n', out);
              यदि (si->trans >= 0)
                {
                  ख_माला_दो ("    -> ", out);
                  state_item_print (&state_वस्तुएँ[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ध्रुव वर्ण *txt[2] = { "    => ", "    <- " };
              प्रति (पूर्णाङ्क seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  यदि (b)
                    {
                      bitset_iterator biter;
                      state_item_संख्या sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          ख_माला_दो (txt[seti], out);
                          state_item_print (&state_वस्तुएँ[sin], out, "");
                        }
                    }
                }
            }
          अ_दो ('\n', out);
        }
    }
  ख_लिखो (out, "FIRSTS\n");
  प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      प्रतीक_संख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        ख_लिखो (out, "    %s\n", symbols[j]->tag);
    }
  ख_माला_दो ("\n\n", out);
}

शून्य
state_वस्तुएँ_init (शून्य)
{
  xtime_t start = gethrxtime ();
  init_state_वस्तुएँ ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  यदि (trace_flag & trace_cex)
    {
      ख_लिखो (मानक_त्रुटि, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_वस्तुएँ_report (मानक_त्रुटि);
    }
}

शून्य
state_वस्तुएँ_free (शून्य)
{
  प्रति (state_item_संख्या i = 0; i < nstate_वस्तुएँ; ++i)
    यदि (!SI_DISABLED (i))
      {
        state_item *si = &state_वस्तुएँ[i];
        यदि (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  मुक्त (state_वस्तुएँ);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ध्रुव state_item *si, ध्रुव state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  पूर्णाङ्क prec1 = s1->prec;
  पूर्णाङ्क prec2 = s2->prec;
  यदि (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      यदि (prec1 > prec2)
        प्रत्यागम false;
      // Do not expand if same precedence, but left-associative.
      यदि (prec1 == prec2 && s1->assoc == बायाँ_assoc)
        प्रत्यागम false;
    }
  प्रत्यागम true;
}
