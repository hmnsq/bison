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

size_t nstate_वस्तूनि;
state_item_सङ्ख्या *state_item_map;
state_item *state_वस्तूनि;

// Hash functions for index -> bitset hash maps.
प्रकारनिर्देश संरचना
{
  पूर्णाङ्क कुञ्चिका;
  bitset l;
} hash_pair;

स्थिर size_t
hash_pair_hasher (ध्रुव hash_pair *sl, size_t max)
{
  पटिनिवत्त sl->कुञ्चिका % max;
}

स्थिर bool
hash_pair_comparator (ध्रुव hash_pair *l, ध्रुव hash_pair *r)
{
  पटिनिवत्त l->कुञ्चिका == r->कुञ्चिका;
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
  पटिनिवत्त hash_xinitialize (size,
                           रिक्त,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

स्थिर bitset
hash_pair_lookup (Hash_table *tab, पूर्णाङ्क कुञ्चिका)
{
  hash_pair probe;
  probe.कुञ्चिका = कुञ्चिका;
  hash_pair *hp = hash_lookup (tab, &probe);
  पटिनिवत्त hp ? hp->l : रिक्त;
}

स्थिर शून्य
hash_pair_निवेशय (Hash_table *tab, पूर्णाङ्क कुञ्चिका, bitset val)
{
  hash_pair *hp = xmalloc (परिमाण *hp);
  hp->कुञ्चिका = कुञ्चिका;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (शून्य) res;
  प्रतिज्ञा (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_सङ्ख्या s, state_item_सङ्ख्या off)
{
  पटिनिवत्त &state_वस्तूनि[state_item_index_lookup (s, off)];
}

स्थिर अन्तःस्थ शून्य
state_item_set (state_item_सङ्ख्या sidx, ध्रुव state *s, item_सङ्ख्या off)
{
  state_वस्तूनि[sidx].state = s;
  state_वस्तूनि[sidx].item = &ritem[off];
  state_वस्तूनि[sidx].lookahead = रिक्त;
  state_वस्तूनि[sidx].trans = -1;
  state_वस्तूनि[sidx].prods = रिक्त;
  state_वस्तूनि[sidx].revs = bitset_create (nstate_वस्तूनि, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
स्थिर शून्य
init_state_वस्तूनि (शून्य)
{
  nstate_वस्तूनि = 0;
  bitsetv production_वस्तूनि = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  अत्थाय (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      ध्रुव state *s = states[i];
      nstate_वस्तूनि += s->nitems;
      closure (s->वस्तूनि, s->nitems);
      अत्थाय (size_t j = 0; j < nitemset; ++j)
        सचे (0 < itemset[j]
            && item_सङ्ख्या_is_rule_सङ्ख्या (ritem[itemset[j] - 1]))
          {
            bitset_set (production_वस्तूनि[i], itemset[j]);
            ++nstate_वस्तूनि;
          }
    }
  state_item_map = xnmalloc (nstates + 1, परिमाण (state_item_सङ्ख्या));
  state_वस्तूनि = xnmalloc (nstate_वस्तूनि, परिमाण (state_item));
  state_item_सङ्ख्या sidx = 0;
  अत्थाय (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      पूर्णाङ्क rule_अन्वेषणम्_idx = 0;
      ध्रुव state *s = states[i];
      ध्रुव reductions *red = s->reductions;
      अत्थाय (पूर्णाङ्क j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->वस्तूनि[j]);
          state_item *si = &state_वस्तूनि[sidx];
          ध्रुव rule *r = item_rule (si->item);
          सचे (rule_अन्वेषणम्_idx < red->अङ्कः && red->rules[rule_अन्वेषणम्_idx] < r)
            ++rule_अन्वेषणम्_idx;
          सचे (rule_अन्वेषणम्_idx < red->अङ्कः && r == red->rules[rule_अन्वेषणम्_idx])
            {
              bitsetv lookahead = red->lookaheads;
              सचे (lookahead)
                si->lookahead = lookahead[rule_अन्वेषणम्_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_सङ्ख्या off;
      BITSET_FOR_EACH (biter, production_वस्तूनि[i], off, 0)
        {
          state_item_set (sidx, s, off);
          सचे (item_सङ्ख्या_is_rule_सङ्ख्या (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              सचे (lookahead)
                state_वस्तूनि[sidx].lookahead = lookahead[rule_अन्वेषणम्_idx];
              ++rule_अन्वेषणम्_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_वस्तूनि;
  bitsetv_free (production_वस्तूनि);
}

स्थिर size_t
state_sym_hasher (ध्रुव शून्य *st, size_t max)
{
  पटिनिवत्त ((state *) st)->accessing_चिह्नम् % max;
}

स्थिर bool
state_sym_comparator (ध्रुव शून्य *s1, ध्रुव शून्य *s2)
{
  पटिनिवत्त ((state *) s1)->accessing_चिह्नम् == ((state *) s2)->accessing_चिह्नम्;
}

स्थिर state *
state_sym_lookup (चिह्नम्_सङ्ख्या sym, Hash_table *h)
{
  state probe;
  probe.accessing_चिह्नम् = sym;
  पटिनिवत्त hash_lookup (h, &probe);
}

स्थिर शून्य
init_trans (शून्य)
{
  अत्थाय (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->अङ्कः, रिक्त, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, रिक्त);
      अत्थाय (पूर्णाङ्क j = 0; j < t->अङ्कः; ++j)
        सचे (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      अत्थाय (state_item_सङ्ख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_सङ्ख्या *item = state_वस्तूनि[j].item;
          सचे (item_सङ्ख्या_is_rule_सङ्ख्या (*item))
            अनुवर्त;
          state *dst = state_sym_lookup (*item, transition_set);
          सचे (!dst)
            अनुवर्त;
          // find the item in the destination state that corresponds
          // to the transition of item
          अत्थाय (पूर्णाङ्क k = 0; k < dst->nitems; ++k)
            सचे (item + 1 == ritem + dst->वस्तूनि[k])
              {
                state_item_सङ्ख्या dstSI =
                  state_item_index_lookup (dst->सङ्ख्या, k);

                state_वस्तूनि[j].trans = dstSI;
                bitset_set (state_वस्तूनि[dstSI].revs, j);
                भङ्ग;
            }
        }
      hash_free (transition_set);
    }
}

स्थिर शून्य
init_prods (शून्य)
{
  अत्थाय (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      अत्थाय (state_item_सङ्ख्या j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तूनि[j];
          item_सङ्ख्या *item = src->item;
          चिह्नम्_सङ्ख्या lhs = item_rule (item)->lhs->सङ्ख्या;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          सचे (!itms)
            {
              itms = bitset_create (nstate_वस्तूनि, BITSET_SPARSE);
              hash_pair_निवेशय (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      अत्थाय (state_item_सङ्ख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तूनि[j];
          item_सङ्ख्या item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          सचे (item_सङ्ख्या_is_rule_सङ्ख्या (item) || ISTOKEN (item))
            अनुवर्त;
          चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          सचे (lb)
            {
              bitset copy = bitset_create (nstate_वस्तूनि, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_वस्तूनि[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_सङ्ख्या prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_वस्तूनि[prod].revs, j);
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
  अत्थाय (state_item_सङ्ख्या i = 0; i < nstate_वस्तूनि; ++i)
    {
      state_item *si = &state_वस्तूनि[i];
      सचे (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*(si->item)) || !si->lookahead)
        अनुवर्त;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                        (ध्रुव शून्य **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      याव (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          सचे (SI_TRANSITION (prev))
            {
              bitset rsi = state_वस्तूनि[prev - state_वस्तूनि].revs;
              bitset_iterator biter;
              state_item_सङ्ख्या sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_वस्तूनि[sin]);
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
  अत्थाय (rule_सङ्ख्या i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_सङ्ख्या *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      याव (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      सचे (item_सङ्ख्या_is_rule_सङ्ख्या (*n) || ISVAR (*n))
        अनुवर्त;

      चिह्नम्_सङ्ख्या lhs = r->lhs->सङ्ख्या;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  याव (change)
    {
      change = false;
      अत्थाय (rule_सङ्ख्या i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          चिह्नम्_सङ्ख्या lhs = r->lhs->सङ्ख्या;
          bitset f_lhs = FIRSTS (lhs);
          अत्थाय (item_सङ्ख्या *n = r->rhs;
               item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              सचे (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              सचे (!nullable[*n - ntokens])
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
  सचे (si->prods)
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

  याव (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      सचे (SI_DISABLED (dsi - state_वस्तूनि))
        अनुवर्त;

      सचे (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ध्रुव state_item *trans = &state_वस्तूनि[dsi->trans];
        bitset_reset (trans->revs, dsi - state_वस्तूनि);
        सचे (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      सचे (dsi->prods)
        {
          bitset_iterator biter;
          state_item_सङ्ख्या sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              सचे (SI_DISABLED (sin))
                अनुवर्त;
              ध्रुव state_item *prod = &state_वस्तूनि[sin];
              bitset_reset (prod->revs, dsi - state_वस्तूनि);
              सचे (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_सङ्ख्या sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          सचे (SI_DISABLED (sin))
            अनुवर्त;
          state_item *rev = &state_वस्तूनि[sin];
          सचे (&state_वस्तूनि[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          अञ्ञथा सचे (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_वस्तूनि);
              सचे (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          अञ्ञथा
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
  अत्थाय (पूर्णाङ्क i = nstate_वस्तूनि - 1; i >= 0; --i)
    {
      state_item *si = &state_वस्तूनि[i];
      सचे (si->trans == -1 && item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*si->item))
        prune_state_item (si);
    }
}

शून्य
state_item_print (ध्रुव state_item *si, FILE *out, ध्रुव वर्ण *prefix)
{
  fputs (prefix, out);
  item_print (si->item, रिक्त, out);
  putc ('\n', out);
}

ध्रुव rule*
state_item_rule (ध्रुव state_item *si)
{
  पटिनिवत्त item_rule (si->item);
}

/**
 * Report the state_item graph
 */
स्थिर शून्य
state_वस्तूनि_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_वस्तूनि);
  अत्थाय (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      अत्थाय (state_item_सङ्ख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ध्रुव state_item *si = &state_वस्तूनि[j];
          item_print (si->item, रिक्त, out);
          सचे (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          अञ्ञथा
            {
              putc ('\n', out);
              सचे (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_वस्तूनि[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ध्रुव वर्ण *txt[2] = { "    => ", "    <- " };
              अत्थाय (पूर्णाङ्क seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  सचे (b)
                    {
                      bitset_iterator biter;
                      state_item_सङ्ख्या sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_वस्तूनि[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  अत्थाय (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      चिह्नम्_सङ्ख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

शून्य
state_वस्तूनि_init (शून्य)
{
  xtime_t start = gethrxtime ();
  init_state_वस्तूनि ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  सचे (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_वस्तूनि_report (stderr);
    }
}

शून्य
state_वस्तूनि_free (शून्य)
{
  अत्थाय (state_item_सङ्ख्या i = 0; i < nstate_वस्तूनि; ++i)
    सचे (!SI_DISABLED (i))
      {
        state_item *si = &state_वस्तूनि[i];
        सचे (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  मुक्त (state_वस्तूनि);
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
  सचे (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      सचे (prec1 > prec2)
        पटिनिवत्त false;
      // Do not expand if same precedence, but left-associative.
      सचे (prec1 == prec2 && s1->assoc == वामः_assoc)
        पटिनिवत्त false;
    }
  पटिनिवत्त true;
}
