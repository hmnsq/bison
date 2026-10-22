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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "state-item.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <gethrxtime.h>
#অন্তর্ভাব <gl_linked_list.h>
#অন্তর্ভাব <gl_xlist.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "nullable.h"

size_t nstate_বস্তুসমূহ;
state_item_সংখ্যা *state_item_map;
state_item *state_বস্তুসমূহ;

// Hash functions for index -> bitset hash maps.
প্রকারনির্দেশ সংরচনা
{
  পূর্ণাঙ্ক চাবি;
  bitset l;
} hash_pair;

স্থির size_t
hash_pair_hasher (ধ্রুব hash_pair *sl, size_t max)
{
  প্রত্যাগম sl->চাবি % max;
}

স্থির bool
hash_pair_comparator (ধ্রুব hash_pair *l, ধ্রুব hash_pair *r)
{
  প্রত্যাগম l->চাবি == r->চাবি;
}

স্থির শূন্য
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  মুক্ত (hp);
}

স্থির Hash_table *
hash_pair_table_create (পূর্ণাঙ্ক size)
{
  প্রত্যাগম hash_xinitialize (size,
                           রিক্ত,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

স্থির bitset
hash_pair_lookup (Hash_table *tab, পূর্ণাঙ্ক চাবি)
{
  hash_pair probe;
  probe.চাবি = চাবি;
  hash_pair *hp = hash_lookup (tab, &probe);
  প্রত্যাগম hp ? hp->l : রিক্ত;
}

স্থির শূন্য
hash_pair_ঢোকাও (Hash_table *tab, পূর্ণাঙ্ক চাবি, bitset val)
{
  hash_pair *hp = xmalloc (পরিমাণ *hp);
  hp->চাবি = চাবি;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (শূন্য) res;
  প্রতিজ্ঞা (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_সংখ্যা s, state_item_সংখ্যা off)
{
  প্রত্যাগম &state_বস্তুসমূহ[state_item_index_lookup (s, off)];
}

স্থির অন্তঃস্থ শূন্য
state_item_set (state_item_সংখ্যা sidx, ধ্রুব state *s, item_সংখ্যা off)
{
  state_বস্তুসমূহ[sidx].state = s;
  state_বস্তুসমূহ[sidx].item = &ritem[off];
  state_বস্তুসমূহ[sidx].lookahead = রিক্ত;
  state_বস্তুসমূহ[sidx].trans = -1;
  state_বস্তুসমূহ[sidx].prods = রিক্ত;
  state_বস্তুসমূহ[sidx].revs = bitset_create (nstate_বস্তুসমূহ, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
স্থির শূন্য
init_state_বস্তুসমূহ (শূন্য)
{
  nstate_বস্তুসমূহ = 0;
  bitsetv production_বস্তুসমূহ = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  প্রতি (পূর্ণাঙ্ক i = 0; i < nstates; ++i)
    {
      ধ্রুব state *s = states[i];
      nstate_বস্তুসমূহ += s->nitems;
      closure (s->বস্তুসমূহ, s->nitems);
      প্রতি (size_t j = 0; j < nitemset; ++j)
        যদি (0 < itemset[j]
            && item_সংখ্যা_is_rule_সংখ্যা (ritem[itemset[j] - 1]))
          {
            bitset_set (production_বস্তুসমূহ[i], itemset[j]);
            ++nstate_বস্তুসমূহ;
          }
    }
  state_item_map = xnmalloc (nstates + 1, পরিমাণ (state_item_সংখ্যা));
  state_বস্তুসমূহ = xnmalloc (nstate_বস্তুসমূহ, পরিমাণ (state_item));
  state_item_সংখ্যা sidx = 0;
  প্রতি (পূর্ণাঙ্ক i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      পূর্ণাঙ্ক rule_অনুসন্ধান_idx = 0;
      ধ্রুব state *s = states[i];
      ধ্রুব reductions *red = s->reductions;
      প্রতি (পূর্ণাঙ্ক j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->বস্তুসমূহ[j]);
          state_item *si = &state_বস্তুসমূহ[sidx];
          ধ্রুব rule *r = item_rule (si->item);
          যদি (rule_অনুসন্ধান_idx < red->অঙ্ক && red->rules[rule_অনুসন্ধান_idx] < r)
            ++rule_অনুসন্ধান_idx;
          যদি (rule_অনুসন্ধান_idx < red->অঙ্ক && r == red->rules[rule_অনুসন্ধান_idx])
            {
              bitsetv lookahead = red->lookaheads;
              যদি (lookahead)
                si->lookahead = lookahead[rule_অনুসন্ধান_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_সংখ্যা off;
      BITSET_FOR_EACH (biter, production_বস্তুসমূহ[i], off, 0)
        {
          state_item_set (sidx, s, off);
          যদি (item_সংখ্যা_is_rule_সংখ্যা (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              যদি (lookahead)
                state_বস্তুসমূহ[sidx].lookahead = lookahead[rule_অনুসন্ধান_idx];
              ++rule_অনুসন্ধান_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_বস্তুসমূহ;
  bitsetv_free (production_বস্তুসমূহ);
}

স্থির size_t
state_sym_hasher (ধ্রুব শূন্য *st, size_t max)
{
  প্রত্যাগম ((state *) st)->accessing_প্রতীক % max;
}

স্থির bool
state_sym_comparator (ধ্রুব শূন্য *s1, ধ্রুব শূন্য *s2)
{
  প্রত্যাগম ((state *) s1)->accessing_প্রতীক == ((state *) s2)->accessing_প্রতীক;
}

স্থির state *
state_sym_lookup (প্রতীক_সংখ্যা sym, Hash_table *h)
{
  state probe;
  probe.accessing_প্রতীক = sym;
  প্রত্যাগম hash_lookup (h, &probe);
}

স্থির শূন্য
init_trans (শূন্য)
{
  প্রতি (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->অঙ্ক, রিক্ত, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, রিক্ত);
      প্রতি (পূর্ণাঙ্ক j = 0; j < t->অঙ্ক; ++j)
        যদি (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      প্রতি (state_item_সংখ্যা j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_সংখ্যা *item = state_বস্তুসমূহ[j].item;
          যদি (item_সংখ্যা_is_rule_সংখ্যা (*item))
            অনুবর্ত;
          state *dst = state_sym_lookup (*item, transition_set);
          যদি (!dst)
            অনুবর্ত;
          // find the item in the destination state that corresponds
          // to the transition of item
          প্রতি (পূর্ণাঙ্ক k = 0; k < dst->nitems; ++k)
            যদি (item + 1 == ritem + dst->বস্তুসমূহ[k])
              {
                state_item_সংখ্যা dstSI =
                  state_item_index_lookup (dst->সংখ্যা, k);

                state_বস্তুসমূহ[j].trans = dstSI;
                bitset_set (state_বস্তুসমূহ[dstSI].revs, j);
                ভঙ্গ;
            }
        }
      hash_free (transition_set);
    }
}

স্থির শূন্য
init_prods (শূন্য)
{
  প্রতি (পূর্ণাঙ্ক i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      প্রতি (state_item_সংখ্যা j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_বস্তুসমূহ[j];
          item_সংখ্যা *item = src->item;
          প্রতীক_সংখ্যা lhs = item_rule (item)->lhs->সংখ্যা;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          যদি (!itms)
            {
              itms = bitset_create (nstate_বস্তুসমূহ, BITSET_SPARSE);
              hash_pair_ঢোকাও (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      প্রতি (state_item_সংখ্যা j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_বস্তুসমূহ[j];
          item_সংখ্যা item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          যদি (item_সংখ্যা_is_rule_সংখ্যা (item) || ISTOKEN (item))
            অনুবর্ত;
          প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          যদি (lb)
            {
              bitset copy = bitset_create (nstate_বস্তুসমূহ, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_বস্তুসমূহ[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_সংখ্যা prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_বস্তুসমূহ[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
স্থির অন্তঃস্থ শূন্য
gen_lookaheads (শূন্য)
{
  প্রতি (state_item_সংখ্যা i = 0; i < nstate_বস্তুসমূহ; ++i)
    {
      state_item *si = &state_বস্তুসমূহ[i];
      যদি (item_সংখ্যা_is_প্রতীক_সংখ্যা (*(si->item)) || !si->lookahead)
        অনুবর্ত;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, রিক্ত, রিক্ত, রিক্ত, true, 1,
                        (ধ্রুব শূন্য **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      যাবত্ (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          যদি (SI_TRANSITION (prev))
            {
              bitset rsi = state_বস্তুসমূহ[prev - state_বস্তুসমূহ].revs;
              bitset_iterator biter;
              state_item_সংখ্যা sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_বস্তুসমূহ[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = রিক্ত;

স্থির শূন্য
init_firsts (শূন্য)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  প্রতি (rule_সংখ্যা i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_সংখ্যা *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      যাবত্ (item_সংখ্যা_is_প্রতীক_সংখ্যা (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      যদি (item_সংখ্যা_is_rule_সংখ্যা (*n) || ISVAR (*n))
        অনুবর্ত;

      প্রতীক_সংখ্যা lhs = r->lhs->সংখ্যা;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  যাবত্ (change)
    {
      change = false;
      প্রতি (rule_সংখ্যা i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          প্রতীক_সংখ্যা lhs = r->lhs->সংখ্যা;
          bitset f_lhs = FIRSTS (lhs);
          প্রতি (item_সংখ্যা *n = r->rhs;
               item_সংখ্যা_is_প্রতীক_সংখ্যা (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              যদি (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              যদি (!nullable[*n - ntokens])
                ভঙ্গ;
            }
        }
    }
}

স্থির অন্তঃস্থ শূন্য
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  যদি (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
স্থির শূন্য
prune_state_item (ধ্রুব state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, রিক্ত, রিক্ত, রিক্ত, true, 1,
                    (ধ্রুব শূন্য **) &si);

  যাবত্ (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      যদি (SI_DISABLED (dsi - state_বস্তুসমূহ))
        অনুবর্ত;

      যদি (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ধ্রুব state_item *trans = &state_বস্তুসমূহ[dsi->trans];
        bitset_reset (trans->revs, dsi - state_বস্তুসমূহ);
        যদি (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      যদি (dsi->prods)
        {
          bitset_iterator biter;
          state_item_সংখ্যা sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              যদি (SI_DISABLED (sin))
                অনুবর্ত;
              ধ্রুব state_item *prod = &state_বস্তুসমূহ[sin];
              bitset_reset (prod->revs, dsi - state_বস্তুসমূহ);
              যদি (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_সংখ্যা sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          যদি (SI_DISABLED (sin))
            অনুবর্ত;
          state_item *rev = &state_বস্তুসমূহ[sin];
          যদি (&state_বস্তুসমূহ[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          অন্যথা যদি (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_বস্তুসমূহ);
              যদি (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          অন্যথা
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
স্থির শূন্য
prune_disabled_paths (শূন্য)
{
  প্রতি (পূর্ণাঙ্ক i = nstate_বস্তুসমূহ - 1; i >= 0; --i)
    {
      state_item *si = &state_বস্তুসমূহ[i];
      যদি (si->trans == -1 && item_সংখ্যা_is_প্রতীক_সংখ্যা (*si->item))
        prune_state_item (si);
    }
}

শূন্য
state_item_print (ধ্রুব state_item *si, FILE *out, ধ্রুব বর্ণ *prefix)
{
  fputs (prefix, out);
  item_print (si->item, রিক্ত, out);
  putc ('\n', out);
}

ধ্রুব rule*
state_item_rule (ধ্রুব state_item *si)
{
  প্রত্যাগম item_rule (si->item);
}

/**
 * Report the state_item graph
 */
স্থির শূন্য
state_বস্তুসমূহ_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_বস্তুসমূহ);
  প্রতি (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      প্রতি (state_item_সংখ্যা j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ধ্রুব state_item *si = &state_বস্তুসমূহ[j];
          item_print (si->item, রিক্ত, out);
          যদি (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          অন্যথা
            {
              putc ('\n', out);
              যদি (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_বস্তুসমূহ[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ধ্রুব বর্ণ *txt[2] = { "    => ", "    <- " };
              প্রতি (পূর্ণাঙ্ক seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  যদি (b)
                    {
                      bitset_iterator biter;
                      state_item_সংখ্যা sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_বস্তুসমূহ[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      প্রতীক_সংখ্যা j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

শূন্য
state_বস্তুসমূহ_init (শূন্য)
{
  xtime_t start = gethrxtime ();
  init_state_বস্তুসমূহ ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  যদি (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_বস্তুসমূহ_report (stderr);
    }
}

শূন্য
state_বস্তুসমূহ_free (শূন্য)
{
  প্রতি (state_item_সংখ্যা i = 0; i < nstate_বস্তুসমূহ; ++i)
    যদি (!SI_DISABLED (i))
      {
        state_item *si = &state_বস্তুসমূহ[i];
        যদি (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  মুক্ত (state_বস্তুসমূহ);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ধ্রুব state_item *si, ধ্রুব state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  পূর্ণাঙ্ক prec1 = s1->prec;
  পূর্ণাঙ্ক prec2 = s2->prec;
  যদি (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      যদি (prec1 > prec2)
        প্রত্যাগম false;
      // Do not expand if same precedence, but left-associative.
      যদি (prec1 == prec2 && s1->assoc == বাম_assoc)
        প্রত্যাগম false;
    }
  প্রত্যাগম true;
}
