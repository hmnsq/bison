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

#समावेश <config.h>

#समावेश "state-item.h"

#समावेश <assert.h>
#समावेश <gethrxtime.h>
#समावेश <gl_linked_list.h>
#समावेश <gl_xlist.h>
#समावेश <stdlib.h>
#समावेश <time.h>

#समावेश "closure.h"
#समावेश "getargs.h"
#समावेश "nullable.h"

माप_प्रकार nstate_वस्तुएँ;
state_item_संख्या *state_item_map;
state_item *state_वस्तुएँ;

// Hash functions for index -> bitset hash maps.
प्रकार काष्ठा
{
  पूर्णांक कुंजी;
  bitset l;
} hash_pair;

अटल माप_प्रकार
hash_pair_hasher (स्थिर hash_pair *sl, माप_प्रकार max)
{
  वापस sl->कुंजी % max;
}

अटल bool
hash_pair_comparator (स्थिर hash_pair *l, स्थिर hash_pair *r)
{
  वापस l->कुंजी == r->कुंजी;
}

अटल व्योम
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  मुक्त (hp);
}

अटल Hash_table *
hash_pair_table_create (पूर्णांक आकार)
{
  वापस hash_xinitialize (आकार,
                           शून्य,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_तथ्य_freer) hash_pair_free);
}

अटल bitset
hash_pair_lookup (Hash_table *tab, पूर्णांक कुंजी)
{
  hash_pair probe;
  probe.कुंजी = कुंजी;
  hash_pair *hp = hash_lookup (tab, &probe);
  वापस hp ? hp->l : शून्य;
}

अटल व्योम
hash_pair_जोड़ो (Hash_table *tab, पूर्णांक कुंजी, bitset val)
{
  hash_pair *hp = xmalloc (माप *hp);
  hp->कुंजी = कुंजी;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (व्योम) res;
  निश्चित (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_संख्या s, state_item_संख्या off)
{
  वापस &state_वस्तुएँ[state_item_सूचकांक_lookup (s, off)];
}

अटल अंतरभूत व्योम
state_item_set (state_item_संख्या sidx, स्थिर state *s, item_संख्या off)
{
  state_वस्तुएँ[sidx].state = s;
  state_वस्तुएँ[sidx].item = &ritem[off];
  state_वस्तुएँ[sidx].lookahead = शून्य;
  state_वस्तुएँ[sidx].trans = -1;
  state_वस्तुएँ[sidx].prods = शून्य;
  state_वस्तुएँ[sidx].revs = bitset_create (nstate_वस्तुएँ, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
अटल व्योम
init_state_वस्तुएँ (व्योम)
{
  nstate_वस्तुएँ = 0;
  bitsetv production_वस्तुएँ = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  क्रम (पूर्णांक i = 0; i < nstates; ++i)
    {
      स्थिर state *s = states[i];
      nstate_वस्तुएँ += s->nitems;
      closure (s->वस्तुएँ, s->nitems);
      क्रम (माप_प्रकार j = 0; j < nitemset; ++j)
        अगर (0 < itemset[j]
            && item_संख्या_is_rule_संख्या (ritem[itemset[j] - 1]))
          {
            bitset_set (production_वस्तुएँ[i], itemset[j]);
            ++nstate_वस्तुएँ;
          }
    }
  state_item_map = xnmalloc (nstates + 1, माप (state_item_संख्या));
  state_वस्तुएँ = xnmalloc (nstate_वस्तुएँ, माप (state_item));
  state_item_संख्या sidx = 0;
  क्रम (पूर्णांक i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      पूर्णांक rule_खोज_idx = 0;
      स्थिर state *s = states[i];
      स्थिर reductions *red = s->reductions;
      क्रम (पूर्णांक j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->वस्तुएँ[j]);
          state_item *si = &state_वस्तुएँ[sidx];
          स्थिर rule *r = item_rule (si->item);
          अगर (rule_खोज_idx < red->अंक && red->rules[rule_खोज_idx] < r)
            ++rule_खोज_idx;
          अगर (rule_खोज_idx < red->अंक && r == red->rules[rule_खोज_idx])
            {
              bitsetv lookahead = red->lookaheads;
              अगर (lookahead)
                si->lookahead = lookahead[rule_खोज_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_संख्या off;
      BITSET_FOR_EACH (biter, production_वस्तुएँ[i], off, 0)
        {
          state_item_set (sidx, s, off);
          अगर (item_संख्या_is_rule_संख्या (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              अगर (lookahead)
                state_वस्तुएँ[sidx].lookahead = lookahead[rule_खोज_idx];
              ++rule_खोज_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_वस्तुएँ;
  bitsetv_free (production_वस्तुएँ);
}

अटल माप_प्रकार
state_sym_hasher (स्थिर व्योम *st, माप_प्रकार max)
{
  वापस ((state *) st)->accessing_प्रतीक % max;
}

अटल bool
state_sym_comparator (स्थिर व्योम *s1, स्थिर व्योम *s2)
{
  वापस ((state *) s1)->accessing_प्रतीक == ((state *) s2)->accessing_प्रतीक;
}

अटल state *
state_sym_lookup (प्रतीक_संख्या sym, Hash_table *h)
{
  state probe;
  probe.accessing_प्रतीक = sym;
  वापस hash_lookup (h, &probe);
}

अटल व्योम
init_trans (व्योम)
{
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->अंक, शून्य, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, शून्य);
      क्रम (पूर्णांक j = 0; j < t->अंक; ++j)
        अगर (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      क्रम (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_संख्या *item = state_वस्तुएँ[j].item;
          अगर (item_संख्या_is_rule_संख्या (*item))
            जारी;
          state *dst = state_sym_lookup (*item, transition_set);
          अगर (!dst)
            जारी;
          // find the item in the destination state that corresponds
          // to the transition of item
          क्रम (पूर्णांक k = 0; k < dst->nitems; ++k)
            अगर (item + 1 == ritem + dst->वस्तुएँ[k])
              {
                state_item_संख्या dstSI =
                  state_item_सूचकांक_lookup (dst->संख्या, k);

                state_वस्तुएँ[j].trans = dstSI;
                bitset_set (state_वस्तुएँ[dstSI].revs, j);
                अवरोध;
            }
        }
      hash_free (transition_set);
    }
}

अटल व्योम
init_prods (व्योम)
{
  क्रम (पूर्णांक i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      क्रम (state_item_संख्या j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तुएँ[j];
          item_संख्या *item = src->item;
          प्रतीक_संख्या lhs = item_rule (item)->lhs->संख्या;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          अगर (!itms)
            {
              itms = bitset_create (nstate_वस्तुएँ, BITSET_SPARSE);
              hash_pair_जोड़ो (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      क्रम (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तुएँ[j];
          item_संख्या item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          अगर (item_संख्या_is_rule_संख्या (item) || ISTOKEN (item))
            जारी;
          प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          अगर (lb)
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
अटल अंतरभूत व्योम
gen_lookaheads (व्योम)
{
  क्रम (state_item_संख्या i = 0; i < nstate_वस्तुएँ; ++i)
    {
      state_item *si = &state_वस्तुएँ[i];
      अगर (item_संख्या_is_प्रतीक_संख्या (*(si->item)) || !si->lookahead)
        जारी;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, शून्य, शून्य, शून्य, true, 1,
                        (स्थिर व्योम **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      जबतक (gl_list_आकार (queue) > 0)
        {
          state_item *पूर्व = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          पूर्व->lookahead = lookahead;
          अगर (SI_TRANSITION (पूर्व))
            {
              bitset rsi = state_वस्तुएँ[पूर्व - state_वस्तुएँ].revs;
              bitset_iterator biter;
              state_item_संख्या sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_वस्तुएँ[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = शून्य;

अटल व्योम
init_firsts (व्योम)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  क्रम (rule_संख्या i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_संख्या *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      जबतक (item_संख्या_is_प्रतीक_संख्या (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      अगर (item_संख्या_is_rule_संख्या (*n) || ISVAR (*n))
        जारी;

      प्रतीक_संख्या lhs = r->lhs->संख्या;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  जबतक (change)
    {
      change = false;
      क्रम (rule_संख्या i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          प्रतीक_संख्या lhs = r->lhs->संख्या;
          bitset f_lhs = FIRSTS (lhs);
          क्रम (item_संख्या *n = r->rhs;
               item_संख्या_is_प्रतीक_संख्या (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              अगर (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              अगर (!nullable[*n - ntokens])
                अवरोध;
            }
        }
    }
}

अटल अंतरभूत व्योम
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  अगर (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
अटल व्योम
prune_state_item (स्थिर state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, शून्य, शून्य, शून्य, true, 1,
                    (स्थिर व्योम **) &si);

  जबतक (gl_list_आकार (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      अगर (SI_DISABLED (dsi - state_वस्तुएँ))
        जारी;

      अगर (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        स्थिर state_item *trans = &state_वस्तुएँ[dsi->trans];
        bitset_reset (trans->revs, dsi - state_वस्तुएँ);
        अगर (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      अगर (dsi->prods)
        {
          bitset_iterator biter;
          state_item_संख्या sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              अगर (SI_DISABLED (sin))
                जारी;
              स्थिर state_item *prod = &state_वस्तुएँ[sin];
              bitset_reset (prod->revs, dsi - state_वस्तुएँ);
              अगर (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          अगर (SI_DISABLED (sin))
            जारी;
          state_item *rev = &state_वस्तुएँ[sin];
          अगर (&state_वस्तुएँ[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          अन्यथा अगर (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_वस्तुएँ);
              अगर (bitset_empty_p (rev->prods))
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
अटल व्योम
prune_disabled_paths (व्योम)
{
  क्रम (पूर्णांक i = nstate_वस्तुएँ - 1; i >= 0; --i)
    {
      state_item *si = &state_वस्तुएँ[i];
      अगर (si->trans == -1 && item_संख्या_is_प्रतीक_संख्या (*si->item))
        prune_state_item (si);
    }
}

व्योम
state_item_print (स्थिर state_item *si, खाता *out, स्थिर अक्षर *prefix)
{
  ख_माला_दो (prefix, out);
  item_print (si->item, शून्य, out);
  अ_दो ('\n', out);
}

स्थिर rule*
state_item_rule (स्थिर state_item *si)
{
  वापस item_rule (si->item);
}

/**
 * Report the state_item graph
 */
अटल व्योम
state_वस्तुएँ_report (खाता *out)
{
  ख_लिखो (out, "# state items: %zu\n", nstate_वस्तुएँ);
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    {
      ख_लिखो (out, "State %d:\n", i);
      क्रम (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          स्थिर state_item *si = &state_वस्तुएँ[j];
          item_print (si->item, शून्य, out);
          अगर (SI_DISABLED (j))
            ख_माला_दो ("  DISABLED\n", out);
          अन्यथा
            {
              अ_दो ('\n', out);
              अगर (si->trans >= 0)
                {
                  ख_माला_दो ("    -> ", out);
                  state_item_print (&state_वस्तुएँ[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              स्थिर अक्षर *txt[2] = { "    => ", "    <- " };
              क्रम (पूर्णांक seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  अगर (b)
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
  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      प्रतीक_संख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        ख_लिखो (out, "    %s\n", symbols[j]->tag);
    }
  ख_माला_दो ("\n\n", out);
}

व्योम
state_वस्तुएँ_init (व्योम)
{
  xtime_t start = gethrxtime ();
  init_state_वस्तुएँ ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  अगर (trace_flag & trace_cex)
    {
      ख_लिखो (मानक_त्रुटि, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_वस्तुएँ_report (मानक_त्रुटि);
    }
}

व्योम
state_वस्तुएँ_free (व्योम)
{
  क्रम (state_item_संख्या i = 0; i < nstate_वस्तुएँ; ++i)
    अगर (!SI_DISABLED (i))
      {
        state_item *si = &state_वस्तुएँ[i];
        अगर (si->prods)
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
production_allowed (स्थिर state_item *si, स्थिर state_item *अग्र)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (अग्र->item)->lhs;
  पूर्णांक prec1 = s1->prec;
  पूर्णांक prec2 = s2->prec;
  अगर (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      अगर (prec1 > prec2)
        वापस false;
      // Do not expand if same precedence, but left-associative.
      अगर (prec1 == prec2 && s1->assoc == बायाँ_assoc)
        वापस false;
    }
  वापस true;
}
