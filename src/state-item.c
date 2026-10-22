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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "state-item.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <gethrxtime.h>
#அந்தர்பாவ <gl_linked_list.h>
#அந்தர்பாவ <gl_xlist.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "nullable.h"

size_t nstate_பொருட்கள்;
state_item_எண் *state_item_map;
state_item *state_பொருட்கள்;

// Hash functions for index -> bitset hash maps.
வகைவரை கட்டமைப்பு
{
  முழு சாவி;
  bitset l;
} hash_pair;

நிலையான size_t
hash_pair_hasher (மாறிலி hash_pair *sl, size_t max)
{
  திரும்பு sl->சாவி % max;
}

நிலையான bool
hash_pair_comparator (மாறிலி hash_pair *l, மாறிலி hash_pair *r)
{
  திரும்பு l->சாவி == r->சாவி;
}

நிலையான வெறுமை
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  விடுவி (hp);
}

நிலையான Hash_table *
hash_pair_table_create (முழு size)
{
  திரும்பு hash_xinitialize (size,
                           வெற்று,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

நிலையான bitset
hash_pair_lookup (Hash_table *tab, முழு சாவி)
{
  hash_pair probe;
  probe.சாவி = சாவி;
  hash_pair *hp = hash_lookup (tab, &probe);
  திரும்பு hp ? hp->l : வெற்று;
}

நிலையான வெறுமை
hash_pair_செருகு (Hash_table *tab, முழு சாவி, bitset val)
{
  hash_pair *hp = xmalloc (அளவு *hp);
  hp->சாவி = சாவி;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (வெறுமை) res;
  ப்ரதிஜ்ஞா (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_எண் s, state_item_எண் off)
{
  திரும்பு &state_பொருட்கள்[state_item_index_lookup (s, off)];
}

நிலையான உள்வரி வெறுமை
state_item_set (state_item_எண் sidx, மாறிலி state *s, item_எண் off)
{
  state_பொருட்கள்[sidx].state = s;
  state_பொருட்கள்[sidx].item = &ritem[off];
  state_பொருட்கள்[sidx].lookahead = வெற்று;
  state_பொருட்கள்[sidx].trans = -1;
  state_பொருட்கள்[sidx].prods = வெற்று;
  state_பொருட்கள்[sidx].revs = bitset_create (nstate_பொருட்கள், BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
நிலையான வெறுமை
init_state_பொருட்கள் (வெறுமை)
{
  nstate_பொருட்கள் = 0;
  bitsetv production_பொருட்கள் = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  க்கு (முழு i = 0; i < nstates; ++i)
    {
      மாறிலி state *s = states[i];
      nstate_பொருட்கள் += s->nitems;
      closure (s->பொருட்கள், s->nitems);
      க்கு (size_t j = 0; j < nitemset; ++j)
        எனில் (0 < itemset[j]
            && item_எண்_is_rule_எண் (ritem[itemset[j] - 1]))
          {
            bitset_set (production_பொருட்கள்[i], itemset[j]);
            ++nstate_பொருட்கள்;
          }
    }
  state_item_map = xnmalloc (nstates + 1, அளவு (state_item_எண்));
  state_பொருட்கள் = xnmalloc (nstate_பொருட்கள், அளவு (state_item));
  state_item_எண் sidx = 0;
  க்கு (முழு i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      முழு rule_தேடல்_idx = 0;
      மாறிலி state *s = states[i];
      மாறிலி reductions *red = s->reductions;
      க்கு (முழு j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->பொருட்கள்[j]);
          state_item *si = &state_பொருட்கள்[sidx];
          மாறிலி rule *r = item_rule (si->item);
          எனில் (rule_தேடல்_idx < red->இலக்கம் && red->rules[rule_தேடல்_idx] < r)
            ++rule_தேடல்_idx;
          எனில் (rule_தேடல்_idx < red->இலக்கம் && r == red->rules[rule_தேடல்_idx])
            {
              bitsetv lookahead = red->lookaheads;
              எனில் (lookahead)
                si->lookahead = lookahead[rule_தேடல்_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_எண் off;
      BITSET_FOR_EACH (biter, production_பொருட்கள்[i], off, 0)
        {
          state_item_set (sidx, s, off);
          எனில் (item_எண்_is_rule_எண் (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              எனில் (lookahead)
                state_பொருட்கள்[sidx].lookahead = lookahead[rule_தேடல்_idx];
              ++rule_தேடல்_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_பொருட்கள்;
  bitsetv_free (production_பொருட்கள்);
}

நிலையான size_t
state_sym_hasher (மாறிலி வெறுமை *st, size_t max)
{
  திரும்பு ((state *) st)->accessing_குறியீடு % max;
}

நிலையான bool
state_sym_comparator (மாறிலி வெறுமை *s1, மாறிலி வெறுமை *s2)
{
  திரும்பு ((state *) s1)->accessing_குறியீடு == ((state *) s2)->accessing_குறியீடு;
}

நிலையான state *
state_sym_lookup (குறியீடு_எண் sym, Hash_table *h)
{
  state probe;
  probe.accessing_குறியீடு = sym;
  திரும்பு hash_lookup (h, &probe);
}

நிலையான வெறுமை
init_trans (வெறுமை)
{
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->இலக்கம், வெற்று, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, வெற்று);
      க்கு (முழு j = 0; j < t->இலக்கம்; ++j)
        எனில் (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      க்கு (state_item_எண் j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_எண் *item = state_பொருட்கள்[j].item;
          எனில் (item_எண்_is_rule_எண் (*item))
            தொடர்;
          state *dst = state_sym_lookup (*item, transition_set);
          எனில் (!dst)
            தொடர்;
          // find the item in the destination state that corresponds
          // to the transition of item
          க்கு (முழு k = 0; k < dst->nitems; ++k)
            எனில் (item + 1 == ritem + dst->பொருட்கள்[k])
              {
                state_item_எண் dstSI =
                  state_item_index_lookup (dst->எண், k);

                state_பொருட்கள்[j].trans = dstSI;
                bitset_set (state_பொருட்கள்[dstSI].revs, j);
                நிறுத்து;
            }
        }
      hash_free (transition_set);
    }
}

நிலையான வெறுமை
init_prods (வெறுமை)
{
  க்கு (முழு i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      க்கு (state_item_எண் j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_பொருட்கள்[j];
          item_எண் *item = src->item;
          குறியீடு_எண் lhs = item_rule (item)->lhs->எண்;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          எனில் (!itms)
            {
              itms = bitset_create (nstate_பொருட்கள், BITSET_SPARSE);
              hash_pair_செருகு (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      க்கு (state_item_எண் j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_பொருட்கள்[j];
          item_எண் item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          எனில் (item_எண்_is_rule_எண் (item) || ISTOKEN (item))
            தொடர்;
          குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          எனில் (lb)
            {
              bitset copy = bitset_create (nstate_பொருட்கள், BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_பொருட்கள்[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_எண் prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_பொருட்கள்[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
நிலையான உள்வரி வெறுமை
gen_lookaheads (வெறுமை)
{
  க்கு (state_item_எண் i = 0; i < nstate_பொருட்கள்; ++i)
    {
      state_item *si = &state_பொருட்கள்[i];
      எனில் (item_எண்_is_குறியீடு_எண் (*(si->item)) || !si->lookahead)
        தொடர்;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true, 1,
                        (மாறிலி வெறுமை **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      போது (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          எனில் (SI_TRANSITION (prev))
            {
              bitset rsi = state_பொருட்கள்[prev - state_பொருட்கள்].revs;
              bitset_iterator biter;
              state_item_எண் sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_பொருட்கள்[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = வெற்று;

நிலையான வெறுமை
init_firsts (வெறுமை)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  க்கு (rule_எண் i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_எண் *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      போது (item_எண்_is_குறியீடு_எண் (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      எனில் (item_எண்_is_rule_எண் (*n) || ISVAR (*n))
        தொடர்;

      குறியீடு_எண் lhs = r->lhs->எண்;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  போது (change)
    {
      change = false;
      க்கு (rule_எண் i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          குறியீடு_எண் lhs = r->lhs->எண்;
          bitset f_lhs = FIRSTS (lhs);
          க்கு (item_எண் *n = r->rhs;
               item_எண்_is_குறியீடு_எண் (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              எனில் (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              எனில் (!nullable[*n - ntokens])
                நிறுத்து;
            }
        }
    }
}

நிலையான உள்வரி வெறுமை
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  எனில் (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
நிலையான வெறுமை
prune_state_item (மாறிலி state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true, 1,
                    (மாறிலி வெறுமை **) &si);

  போது (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      எனில் (SI_DISABLED (dsi - state_பொருட்கள்))
        தொடர்;

      எனில் (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        மாறிலி state_item *trans = &state_பொருட்கள்[dsi->trans];
        bitset_reset (trans->revs, dsi - state_பொருட்கள்);
        எனில் (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      எனில் (dsi->prods)
        {
          bitset_iterator biter;
          state_item_எண் sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              எனில் (SI_DISABLED (sin))
                தொடர்;
              மாறிலி state_item *prod = &state_பொருட்கள்[sin];
              bitset_reset (prod->revs, dsi - state_பொருட்கள்);
              எனில் (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_எண் sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          எனில் (SI_DISABLED (sin))
            தொடர்;
          state_item *rev = &state_பொருட்கள்[sin];
          எனில் (&state_பொருட்கள்[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          இல்லையெனில் எனில் (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_பொருட்கள்);
              எனில் (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          இல்லையெனில்
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
நிலையான வெறுமை
prune_disabled_paths (வெறுமை)
{
  க்கு (முழு i = nstate_பொருட்கள் - 1; i >= 0; --i)
    {
      state_item *si = &state_பொருட்கள்[i];
      எனில் (si->trans == -1 && item_எண்_is_குறியீடு_எண் (*si->item))
        prune_state_item (si);
    }
}

வெறுமை
state_item_print (மாறிலி state_item *si, FILE *out, மாறிலி எழுத்து *prefix)
{
  fputs (prefix, out);
  item_print (si->item, வெற்று, out);
  putc ('\n', out);
}

மாறிலி rule*
state_item_rule (மாறிலி state_item *si)
{
  திரும்பு item_rule (si->item);
}

/**
 * Report the state_item graph
 */
நிலையான வெறுமை
state_பொருட்கள்_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_பொருட்கள்);
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      க்கு (state_item_எண் j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          மாறிலி state_item *si = &state_பொருட்கள்[j];
          item_print (si->item, வெற்று, out);
          எனில் (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          இல்லையெனில்
            {
              putc ('\n', out);
              எனில் (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_பொருட்கள்[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              மாறிலி எழுத்து *txt[2] = { "    => ", "    <- " };
              க்கு (முழு seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  எனில் (b)
                    {
                      bitset_iterator biter;
                      state_item_எண் sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_பொருட்கள்[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      குறியீடு_எண் j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

வெறுமை
state_பொருட்கள்_init (வெறுமை)
{
  xtime_t start = gethrxtime ();
  init_state_பொருட்கள் ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  எனில் (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_பொருட்கள்_report (stderr);
    }
}

வெறுமை
state_பொருட்கள்_free (வெறுமை)
{
  க்கு (state_item_எண் i = 0; i < nstate_பொருட்கள்; ++i)
    எனில் (!SI_DISABLED (i))
      {
        state_item *si = &state_பொருட்கள்[i];
        எனில் (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  விடுவி (state_பொருட்கள்);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (மாறிலி state_item *si, மாறிலி state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  முழு prec1 = s1->prec;
  முழு prec2 = s2->prec;
  எனில் (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      எனில் (prec1 > prec2)
        திரும்பு false;
      // Do not expand if same precedence, but left-associative.
      எனில் (prec1 == prec2 && s1->assoc == இடது_assoc)
        திரும்பு false;
    }
  திரும்பு true;
}
