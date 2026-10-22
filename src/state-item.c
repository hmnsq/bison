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

माप_प्रकार nstate_वस्तुसंच;
state_item_संख्या *state_item_map;
state_item *state_वस्तुसंच;

// Hash functions for index -> bitset hash maps.
प्रकारव्याख्या संरचना
{
  पूर्णांक किल्ली;
  bitset l;
} hash_pair;

स्थिर माप_प्रकार
hash_pair_hasher (स्थिरांक hash_pair *sl, माप_प्रकार max)
{
  परत sl->किल्ली % max;
}

स्थिर bool
hash_pair_comparator (स्थिरांक hash_pair *l, स्थिरांक hash_pair *r)
{
  परत l->किल्ली == r->किल्ली;
}

स्थिर शून्य
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  मुक्त (hp);
}

स्थिर Hash_table *
hash_pair_table_create (पूर्णांक size)
{
  परत hash_xinitialize (size,
                           रिक्त,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

स्थिर bitset
hash_pair_lookup (Hash_table *tab, पूर्णांक किल्ली)
{
  hash_pair probe;
  probe.किल्ली = किल्ली;
  hash_pair *hp = hash_lookup (tab, &probe);
  परत hp ? hp->l : रिक्त;
}

स्थिर शून्य
hash_pair_घाला (Hash_table *tab, पूर्णांक किल्ली, bitset val)
{
  hash_pair *hp = xmalloc (आकार *hp);
  hp->किल्ली = किल्ली;
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
  परत &state_वस्तुसंच[state_item_index_lookup (s, off)];
}

स्थिर ओळीत शून्य
state_item_set (state_item_संख्या sidx, स्थिरांक state *s, item_संख्या off)
{
  state_वस्तुसंच[sidx].state = s;
  state_वस्तुसंच[sidx].item = &ritem[off];
  state_वस्तुसंच[sidx].lookahead = रिक्त;
  state_वस्तुसंच[sidx].trans = -1;
  state_वस्तुसंच[sidx].prods = रिक्त;
  state_वस्तुसंच[sidx].revs = bitset_create (nstate_वस्तुसंच, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
स्थिर शून्य
init_state_वस्तुसंच (शून्य)
{
  nstate_वस्तुसंच = 0;
  bitsetv production_वस्तुसंच = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  साठी (पूर्णांक i = 0; i < nstates; ++i)
    {
      स्थिरांक state *s = states[i];
      nstate_वस्तुसंच += s->nitems;
      closure (s->वस्तुसंच, s->nitems);
      साठी (माप_प्रकार j = 0; j < nitemset; ++j)
        जर (0 < itemset[j]
            && item_संख्या_is_rule_संख्या (ritem[itemset[j] - 1]))
          {
            bitset_set (production_वस्तुसंच[i], itemset[j]);
            ++nstate_वस्तुसंच;
          }
    }
  state_item_map = xnmalloc (nstates + 1, आकार (state_item_संख्या));
  state_वस्तुसंच = xnmalloc (nstate_वस्तुसंच, आकार (state_item));
  state_item_संख्या sidx = 0;
  साठी (पूर्णांक i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      पूर्णांक rule_शोध_idx = 0;
      स्थिरांक state *s = states[i];
      स्थिरांक reductions *red = s->reductions;
      साठी (पूर्णांक j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->वस्तुसंच[j]);
          state_item *si = &state_वस्तुसंच[sidx];
          स्थिरांक rule *r = item_rule (si->item);
          जर (rule_शोध_idx < red->अंक && red->rules[rule_शोध_idx] < r)
            ++rule_शोध_idx;
          जर (rule_शोध_idx < red->अंक && r == red->rules[rule_शोध_idx])
            {
              bitsetv lookahead = red->lookaheads;
              जर (lookahead)
                si->lookahead = lookahead[rule_शोध_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_संख्या off;
      BITSET_FOR_EACH (biter, production_वस्तुसंच[i], off, 0)
        {
          state_item_set (sidx, s, off);
          जर (item_संख्या_is_rule_संख्या (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              जर (lookahead)
                state_वस्तुसंच[sidx].lookahead = lookahead[rule_शोध_idx];
              ++rule_शोध_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_वस्तुसंच;
  bitsetv_free (production_वस्तुसंच);
}

स्थिर माप_प्रकार
state_sym_hasher (स्थिरांक शून्य *st, माप_प्रकार max)
{
  परत ((state *) st)->accessing_चिन्ह % max;
}

स्थिर bool
state_sym_comparator (स्थिरांक शून्य *s1, स्थिरांक शून्य *s2)
{
  परत ((state *) s1)->accessing_चिन्ह == ((state *) s2)->accessing_चिन्ह;
}

स्थिर state *
state_sym_lookup (चिन्ह_संख्या sym, Hash_table *h)
{
  state probe;
  probe.accessing_चिन्ह = sym;
  परत hash_lookup (h, &probe);
}

स्थिर शून्य
init_trans (शून्य)
{
  साठी (state_संख्या i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->अंक, रिक्त, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, रिक्त);
      साठी (पूर्णांक j = 0; j < t->अंक; ++j)
        जर (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      साठी (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_संख्या *item = state_वस्तुसंच[j].item;
          जर (item_संख्या_is_rule_संख्या (*item))
            सुरूठेवा;
          state *dst = state_sym_lookup (*item, transition_set);
          जर (!dst)
            सुरूठेवा;
          // find the item in the destination state that corresponds
          // to the transition of item
          साठी (पूर्णांक k = 0; k < dst->nitems; ++k)
            जर (item + 1 == ritem + dst->वस्तुसंच[k])
              {
                state_item_संख्या dstSI =
                  state_item_index_lookup (dst->संख्या, k);

                state_वस्तुसंच[j].trans = dstSI;
                bitset_set (state_वस्तुसंच[dstSI].revs, j);
                थांबा;
            }
        }
      hash_free (transition_set);
    }
}

स्थिर शून्य
init_prods (शून्य)
{
  साठी (पूर्णांक i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      साठी (state_item_संख्या j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तुसंच[j];
          item_संख्या *item = src->item;
          चिन्ह_संख्या lhs = item_rule (item)->lhs->संख्या;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          जर (!itms)
            {
              itms = bitset_create (nstate_वस्तुसंच, BITSET_SPARSE);
              hash_pair_घाला (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      साठी (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तुसंच[j];
          item_संख्या item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          जर (item_संख्या_is_rule_संख्या (item) || ISTOKEN (item))
            सुरूठेवा;
          चिन्ह_संख्या sym = item_संख्या_as_चिन्ह_संख्या (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          जर (lb)
            {
              bitset copy = bitset_create (nstate_वस्तुसंच, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_वस्तुसंच[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_संख्या prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_वस्तुसंच[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
स्थिर ओळीत शून्य
gen_lookaheads (शून्य)
{
  साठी (state_item_संख्या i = 0; i < nstate_वस्तुसंच; ++i)
    {
      state_item *si = &state_वस्तुसंच[i];
      जर (item_संख्या_is_चिन्ह_संख्या (*(si->item)) || !si->lookahead)
        सुरूठेवा;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                        (स्थिरांक शून्य **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      पर्यंत (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          जर (SI_TRANSITION (prev))
            {
              bitset rsi = state_वस्तुसंच[prev - state_वस्तुसंच].revs;
              bitset_iterator biter;
              state_item_संख्या sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_वस्तुसंच[sin]);
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
  साठी (rule_संख्या i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_संख्या *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      पर्यंत (item_संख्या_is_चिन्ह_संख्या (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      जर (item_संख्या_is_rule_संख्या (*n) || ISVAR (*n))
        सुरूठेवा;

      चिन्ह_संख्या lhs = r->lhs->संख्या;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  पर्यंत (change)
    {
      change = false;
      साठी (rule_संख्या i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          चिन्ह_संख्या lhs = r->lhs->संख्या;
          bitset f_lhs = FIRSTS (lhs);
          साठी (item_संख्या *n = r->rhs;
               item_संख्या_is_चिन्ह_संख्या (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              जर (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              जर (!nullable[*n - ntokens])
                थांबा;
            }
        }
    }
}

स्थिर ओळीत शून्य
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  जर (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
स्थिर शून्य
prune_state_item (स्थिरांक state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                    (स्थिरांक शून्य **) &si);

  पर्यंत (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      जर (SI_DISABLED (dsi - state_वस्तुसंच))
        सुरूठेवा;

      जर (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        स्थिरांक state_item *trans = &state_वस्तुसंच[dsi->trans];
        bitset_reset (trans->revs, dsi - state_वस्तुसंच);
        जर (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      जर (dsi->prods)
        {
          bitset_iterator biter;
          state_item_संख्या sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              जर (SI_DISABLED (sin))
                सुरूठेवा;
              स्थिरांक state_item *prod = &state_वस्तुसंच[sin];
              bitset_reset (prod->revs, dsi - state_वस्तुसंच);
              जर (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          जर (SI_DISABLED (sin))
            सुरूठेवा;
          state_item *rev = &state_वस्तुसंच[sin];
          जर (&state_वस्तुसंच[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          अन्यथा जर (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_वस्तुसंच);
              जर (bitset_empty_p (rev->prods))
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
  साठी (पूर्णांक i = nstate_वस्तुसंच - 1; i >= 0; --i)
    {
      state_item *si = &state_वस्तुसंच[i];
      जर (si->trans == -1 && item_संख्या_is_चिन्ह_संख्या (*si->item))
        prune_state_item (si);
    }
}

शून्य
state_item_print (स्थिरांक state_item *si, खाता *out, स्थिरांक अक्षर *prefix)
{
  ख_माला_दो (prefix, out);
  item_print (si->item, रिक्त, out);
  अ_दो ('\n', out);
}

स्थिरांक rule*
state_item_rule (स्थिरांक state_item *si)
{
  परत item_rule (si->item);
}

/**
 * Report the state_item graph
 */
स्थिर शून्य
state_वस्तुसंच_report (खाता *out)
{
  ख_लिखो (out, "# state items: %zu\n", nstate_वस्तुसंच);
  साठी (state_संख्या i = 0; i < nstates; ++i)
    {
      ख_लिखो (out, "State %d:\n", i);
      साठी (state_item_संख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          स्थिरांक state_item *si = &state_वस्तुसंच[j];
          item_print (si->item, रिक्त, out);
          जर (SI_DISABLED (j))
            ख_माला_दो ("  DISABLED\n", out);
          अन्यथा
            {
              अ_दो ('\n', out);
              जर (si->trans >= 0)
                {
                  ख_माला_दो ("    -> ", out);
                  state_item_print (&state_वस्तुसंच[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              स्थिरांक अक्षर *txt[2] = { "    => ", "    <- " };
              साठी (पूर्णांक seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  जर (b)
                    {
                      bitset_iterator biter;
                      state_item_संख्या sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          ख_माला_दो (txt[seti], out);
                          state_item_print (&state_वस्तुसंच[sin], out, "");
                        }
                    }
                }
            }
          अ_दो ('\n', out);
        }
    }
  ख_लिखो (out, "FIRSTS\n");
  साठी (चिन्ह_संख्या i = ntokens; i < nsyms; ++i)
    {
      ख_लिखो (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      चिन्ह_संख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        ख_लिखो (out, "    %s\n", symbols[j]->tag);
    }
  ख_माला_दो ("\n\n", out);
}

शून्य
state_वस्तुसंच_init (शून्य)
{
  xtime_t start = gethrxtime ();
  init_state_वस्तुसंच ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  जर (trace_flag & trace_cex)
    {
      ख_लिखो (मानक_त्रुटि, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_वस्तुसंच_report (मानक_त्रुटि);
    }
}

शून्य
state_वस्तुसंच_free (शून्य)
{
  साठी (state_item_संख्या i = 0; i < nstate_वस्तुसंच; ++i)
    जर (!SI_DISABLED (i))
      {
        state_item *si = &state_वस्तुसंच[i];
        जर (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  मुक्त (state_वस्तुसंच);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (स्थिरांक state_item *si, स्थिरांक state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  पूर्णांक prec1 = s1->prec;
  पूर्णांक prec2 = s2->prec;
  जर (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      जर (prec1 > prec2)
        परत false;
      // Do not expand if same precedence, but left-associative.
      जर (prec1 == prec2 && s1->assoc == डावा_assoc)
        परत false;
    }
  परत true;
}
