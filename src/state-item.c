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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "state-item.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <gethrxtime.h>
#അന്തര്ഭാവ <gl_linked_list.h>
#അന്തര്ഭാവ <gl_xlist.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "nullable.h"

size_t nstate_ഇനങ്ങൾ;
state_item_സംഖ്യ *state_item_map;
state_item *state_ഇനങ്ങൾ;

// Hash functions for index -> bitset hash maps.
തരനിർവചനം ഘടന
{
  പൂർണ്ണസംഖ്യ താക്കോൽ;
  bitset l;
} hash_pair;

സ്ഥിതം size_t
hash_pair_hasher (സ്ഥിരം hash_pair *sl, size_t max)
{
  തിരികെ sl->താക്കോൽ % max;
}

സ്ഥിതം bool
hash_pair_comparator (സ്ഥിരം hash_pair *l, സ്ഥിരം hash_pair *r)
{
  തിരികെ l->താക്കോൽ == r->താക്കോൽ;
}

സ്ഥിതം ശൂന്യം
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  സ്വതന്ത്രം (hp);
}

സ്ഥിതം Hash_table *
hash_pair_table_create (പൂർണ്ണസംഖ്യ size)
{
  തിരികെ hash_xinitialize (size,
                           ശൂന്യ,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

സ്ഥിതം bitset
hash_pair_lookup (Hash_table *tab, പൂർണ്ണസംഖ്യ താക്കോൽ)
{
  hash_pair probe;
  probe.താക്കോൽ = താക്കോൽ;
  hash_pair *hp = hash_lookup (tab, &probe);
  തിരികെ hp ? hp->l : ശൂന്യ;
}

സ്ഥിതം ശൂന്യം
hash_pair_തിരുകുക (Hash_table *tab, പൂർണ്ണസംഖ്യ താക്കോൽ, bitset val)
{
  hash_pair *hp = xmalloc (വലുപ്പം *hp);
  hp->താക്കോൽ = താക്കോൽ;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (ശൂന്യം) res;
  പ്രതിജ്ഞാ (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_സംഖ്യ s, state_item_സംഖ്യ off)
{
  തിരികെ &state_ഇനങ്ങൾ[state_item_index_lookup (s, off)];
}

സ്ഥിതം വരിയിൽ ശൂന്യം
state_item_set (state_item_സംഖ്യ sidx, സ്ഥിരം state *s, item_സംഖ്യ off)
{
  state_ഇനങ്ങൾ[sidx].state = s;
  state_ഇനങ്ങൾ[sidx].item = &ritem[off];
  state_ഇനങ്ങൾ[sidx].lookahead = ശൂന്യ;
  state_ഇനങ്ങൾ[sidx].trans = -1;
  state_ഇനങ്ങൾ[sidx].prods = ശൂന്യ;
  state_ഇനങ്ങൾ[sidx].revs = bitset_create (nstate_ഇനങ്ങൾ, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
സ്ഥിതം ശൂന്യം
init_state_ഇനങ്ങൾ (ശൂന്യം)
{
  nstate_ഇനങ്ങൾ = 0;
  bitsetv production_ഇനങ്ങൾ = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nstates; ++i)
    {
      സ്ഥിരം state *s = states[i];
      nstate_ഇനങ്ങൾ += s->nitems;
      closure (s->ഇനങ്ങൾ, s->nitems);
      വേണ്ടി (size_t j = 0; j < nitemset; ++j)
        എങ്കിൽ (0 < itemset[j]
            && item_സംഖ്യ_is_rule_സംഖ്യ (ritem[itemset[j] - 1]))
          {
            bitset_set (production_ഇനങ്ങൾ[i], itemset[j]);
            ++nstate_ഇനങ്ങൾ;
          }
    }
  state_item_map = xnmalloc (nstates + 1, വലുപ്പം (state_item_സംഖ്യ));
  state_ഇനങ്ങൾ = xnmalloc (nstate_ഇനങ്ങൾ, വലുപ്പം (state_item));
  state_item_സംഖ്യ sidx = 0;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      പൂർണ്ണസംഖ്യ rule_തിരയൽ_idx = 0;
      സ്ഥിരം state *s = states[i];
      സ്ഥിരം reductions *red = s->reductions;
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->ഇനങ്ങൾ[j]);
          state_item *si = &state_ഇനങ്ങൾ[sidx];
          സ്ഥിരം rule *r = item_rule (si->item);
          എങ്കിൽ (rule_തിരയൽ_idx < red->അക്കം && red->rules[rule_തിരയൽ_idx] < r)
            ++rule_തിരയൽ_idx;
          എങ്കിൽ (rule_തിരയൽ_idx < red->അക്കം && r == red->rules[rule_തിരയൽ_idx])
            {
              bitsetv lookahead = red->lookaheads;
              എങ്കിൽ (lookahead)
                si->lookahead = lookahead[rule_തിരയൽ_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_സംഖ്യ off;
      BITSET_FOR_EACH (biter, production_ഇനങ്ങൾ[i], off, 0)
        {
          state_item_set (sidx, s, off);
          എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              എങ്കിൽ (lookahead)
                state_ഇനങ്ങൾ[sidx].lookahead = lookahead[rule_തിരയൽ_idx];
              ++rule_തിരയൽ_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_ഇനങ്ങൾ;
  bitsetv_free (production_ഇനങ്ങൾ);
}

സ്ഥിതം size_t
state_sym_hasher (സ്ഥിരം ശൂന്യം *st, size_t max)
{
  തിരികെ ((state *) st)->accessing_ചിഹ്നം % max;
}

സ്ഥിതം bool
state_sym_comparator (സ്ഥിരം ശൂന്യം *s1, സ്ഥിരം ശൂന്യം *s2)
{
  തിരികെ ((state *) s1)->accessing_ചിഹ്നം == ((state *) s2)->accessing_ചിഹ്നം;
}

സ്ഥിതം state *
state_sym_lookup (ചിഹ്നം_സംഖ്യ sym, Hash_table *h)
{
  state probe;
  probe.accessing_ചിഹ്നം = sym;
  തിരികെ hash_lookup (h, &probe);
}

സ്ഥിതം ശൂന്യം
init_trans (ശൂന്യം)
{
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->അക്കം, ശൂന്യ, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, ശൂന്യ);
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < t->അക്കം; ++j)
        എങ്കിൽ (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      വേണ്ടി (state_item_സംഖ്യ j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_സംഖ്യ *item = state_ഇനങ്ങൾ[j].item;
          എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (*item))
            തുടരുക;
          state *dst = state_sym_lookup (*item, transition_set);
          എങ്കിൽ (!dst)
            തുടരുക;
          // find the item in the destination state that corresponds
          // to the transition of item
          വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < dst->nitems; ++k)
            എങ്കിൽ (item + 1 == ritem + dst->ഇനങ്ങൾ[k])
              {
                state_item_സംഖ്യ dstSI =
                  state_item_index_lookup (dst->സംഖ്യ, k);

                state_ഇനങ്ങൾ[j].trans = dstSI;
                bitset_set (state_ഇനങ്ങൾ[dstSI].revs, j);
                നിർത്തുക;
            }
        }
      hash_free (transition_set);
    }
}

സ്ഥിതം ശൂന്യം
init_prods (ശൂന്യം)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      വേണ്ടി (state_item_സംഖ്യ j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ഇനങ്ങൾ[j];
          item_സംഖ്യ *item = src->item;
          ചിഹ്നം_സംഖ്യ lhs = item_rule (item)->lhs->സംഖ്യ;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          എങ്കിൽ (!itms)
            {
              itms = bitset_create (nstate_ഇനങ്ങൾ, BITSET_SPARSE);
              hash_pair_തിരുകുക (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      വേണ്ടി (state_item_സംഖ്യ j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_ഇനങ്ങൾ[j];
          item_സംഖ്യ item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (item) || ISTOKEN (item))
            തുടരുക;
          ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          എങ്കിൽ (lb)
            {
              bitset copy = bitset_create (nstate_ഇനങ്ങൾ, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_ഇനങ്ങൾ[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_സംഖ്യ prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_ഇനങ്ങൾ[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
സ്ഥിതം വരിയിൽ ശൂന്യം
gen_lookaheads (ശൂന്യം)
{
  വേണ്ടി (state_item_സംഖ്യ i = 0; i < nstate_ഇനങ്ങൾ; ++i)
    {
      state_item *si = &state_ഇനങ്ങൾ[i];
      എങ്കിൽ (item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*(si->item)) || !si->lookahead)
        തുടരുക;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true, 1,
                        (സ്ഥിരം ശൂന്യം **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      ആകുമ്പോൾ (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          എങ്കിൽ (SI_TRANSITION (prev))
            {
              bitset rsi = state_ഇനങ്ങൾ[prev - state_ഇനങ്ങൾ].revs;
              bitset_iterator biter;
              state_item_സംഖ്യ sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_ഇനങ്ങൾ[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = ശൂന്യ;

സ്ഥിതം ശൂന്യം
init_firsts (ശൂന്യം)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  വേണ്ടി (rule_സംഖ്യ i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_സംഖ്യ *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      ആകുമ്പോൾ (item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (*n) || ISVAR (*n))
        തുടരുക;

      ചിഹ്നം_സംഖ്യ lhs = r->lhs->സംഖ്യ;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  ആകുമ്പോൾ (change)
    {
      change = false;
      വേണ്ടി (rule_സംഖ്യ i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          ചിഹ്നം_സംഖ്യ lhs = r->lhs->സംഖ്യ;
          bitset f_lhs = FIRSTS (lhs);
          വേണ്ടി (item_സംഖ്യ *n = r->rhs;
               item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              എങ്കിൽ (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              എങ്കിൽ (!nullable[*n - ntokens])
                നിർത്തുക;
            }
        }
    }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  എങ്കിൽ (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
സ്ഥിതം ശൂന്യം
prune_state_item (സ്ഥിരം state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true, 1,
                    (സ്ഥിരം ശൂന്യം **) &si);

  ആകുമ്പോൾ (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      എങ്കിൽ (SI_DISABLED (dsi - state_ഇനങ്ങൾ))
        തുടരുക;

      എങ്കിൽ (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        സ്ഥിരം state_item *trans = &state_ഇനങ്ങൾ[dsi->trans];
        bitset_reset (trans->revs, dsi - state_ഇനങ്ങൾ);
        എങ്കിൽ (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      എങ്കിൽ (dsi->prods)
        {
          bitset_iterator biter;
          state_item_സംഖ്യ sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              എങ്കിൽ (SI_DISABLED (sin))
                തുടരുക;
              സ്ഥിരം state_item *prod = &state_ഇനങ്ങൾ[sin];
              bitset_reset (prod->revs, dsi - state_ഇനങ്ങൾ);
              എങ്കിൽ (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_സംഖ്യ sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          എങ്കിൽ (SI_DISABLED (sin))
            തുടരുക;
          state_item *rev = &state_ഇനങ്ങൾ[sin];
          എങ്കിൽ (&state_ഇനങ്ങൾ[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          അല്ലെങ്കിൽ എങ്കിൽ (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_ഇനങ്ങൾ);
              എങ്കിൽ (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          അല്ലെങ്കിൽ
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
സ്ഥിതം ശൂന്യം
prune_disabled_paths (ശൂന്യം)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = nstate_ഇനങ്ങൾ - 1; i >= 0; --i)
    {
      state_item *si = &state_ഇനങ്ങൾ[i];
      എങ്കിൽ (si->trans == -1 && item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*si->item))
        prune_state_item (si);
    }
}

ശൂന്യം
state_item_print (സ്ഥിരം state_item *si, FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  fputs (prefix, out);
  item_print (si->item, ശൂന്യ, out);
  putc ('\n', out);
}

സ്ഥിരം rule*
state_item_rule (സ്ഥിരം state_item *si)
{
  തിരികെ item_rule (si->item);
}

/**
 * Report the state_item graph
 */
സ്ഥിതം ശൂന്യം
state_ഇനങ്ങൾ_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_ഇനങ്ങൾ);
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      വേണ്ടി (state_item_സംഖ്യ j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          സ്ഥിരം state_item *si = &state_ഇനങ്ങൾ[j];
          item_print (si->item, ശൂന്യ, out);
          എങ്കിൽ (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          അല്ലെങ്കിൽ
            {
              putc ('\n', out);
              എങ്കിൽ (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_print (&state_ഇനങ്ങൾ[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              സ്ഥിരം അക്ഷരം *txt[2] = { "    => ", "    <- " };
              വേണ്ടി (പൂർണ്ണസംഖ്യ seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  എങ്കിൽ (b)
                    {
                      bitset_iterator biter;
                      state_item_സംഖ്യ sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_print (&state_ഇനങ്ങൾ[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      ചിഹ്നം_സംഖ്യ j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

ശൂന്യം
state_ഇനങ്ങൾ_init (ശൂന്യം)
{
  xtime_t start = gethrxtime ();
  init_state_ഇനങ്ങൾ ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  എങ്കിൽ (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_ഇനങ്ങൾ_report (stderr);
    }
}

ശൂന്യം
state_ഇനങ്ങൾ_free (ശൂന്യം)
{
  വേണ്ടി (state_item_സംഖ്യ i = 0; i < nstate_ഇനങ്ങൾ; ++i)
    എങ്കിൽ (!SI_DISABLED (i))
      {
        state_item *si = &state_ഇനങ്ങൾ[i];
        എങ്കിൽ (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  സ്വതന്ത്രം (state_ഇനങ്ങൾ);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (സ്ഥിരം state_item *si, സ്ഥിരം state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  പൂർണ്ണസംഖ്യ prec1 = s1->prec;
  പൂർണ്ണസംഖ്യ prec2 = s2->prec;
  എങ്കിൽ (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      എങ്കിൽ (prec1 > prec2)
        തിരികെ false;
      // Do not expand if same precedence, but left-associative.
      എങ്കിൽ (prec1 == prec2 && s1->assoc == ഇടത്_assoc)
        തിരികെ false;
    }
  തിരികെ true;
}
