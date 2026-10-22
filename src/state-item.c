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

#include <config.h>

#include "state-item.h"

#include <assert.h>
#include <gethrxtime.h>
#include <gl_linked_list.h>
#include <gl_xlist.h>
#include <stdlib.h>
#include <time.h>

#include "closure.h"
#include "getargs.h"
#include "nullable.h"

size_t nstate_मगा_बागाय;
state_item_नुमेरो *state_item_map;
state_item *state_मगा_बागाय;

// Hash functions for index -> bitset hash maps.
typedef struct
{
  int सुसि;
  bitset l;
} hash_pair;

static size_t
hash_pair_hasher (const hash_pair *sl, size_t max)
{
  return sl->सुसि % max;
}

static bool
hash_pair_comparator (const hash_pair *l, const hash_pair *r)
{
  return l->सुसि == r->सुसि;
}

static void
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  free (hp);
}

static Hash_table *
hash_pair_table_create (int size)
{
  return hash_xinitialize (size,
                           NULL,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_data_freer) hash_pair_free);
}

static bitset
hash_pair_lookup (Hash_table *tab, int सुसि)
{
  hash_pair probe;
  probe.सुसि = सुसि;
  hash_pair *hp = hash_lookup (tab, &probe);
  return hp ? hp->l : NULL;
}

static void
hash_pair_इसिंगित (Hash_table *tab, int सुसि, bitset val)
{
  hash_pair *hp = xmalloc (sizeof *hp);
  hp->सुसि = सुसि;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (void) res;
  assert (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_item *
state_item_lookup (state_नुमेरो s, state_item_नुमेरो off)
{
  return &state_मगा_बागाय[state_item_index_lookup (s, off)];
}

static inline void
state_item_set (state_item_नुमेरो sidx, const state *s, item_नुमेरो off)
{
  state_मगा_बागाय[sidx].state = s;
  state_मगा_बागाय[sidx].item = &ritem[off];
  state_मगा_बागाय[sidx].lookahead = NULL;
  state_मगा_बागाय[sidx].trans = -1;
  state_मगा_बागाय[sidx].prods = NULL;
  state_मगा_बागाय[sidx].revs = bitset_create (nstate_मगा_बागाय, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
static void
init_state_मगा_बागाय (void)
{
  nstate_मगा_बागाय = 0;
  bitsetv production_मगा_बागाय = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  for (int i = 0; i < nstates; ++i)
    {
      const state *s = states[i];
      nstate_मगा_बागाय += s->nitems;
      closure (s->मगा_बागाय, s->nitems);
      for (size_t j = 0; j < nitemset; ++j)
        if (0 < itemset[j]
            && item_नुमेरो_is_rule_नुमेरो (ritem[itemset[j] - 1]))
          {
            bitset_set (production_मगा_बागाय[i], itemset[j]);
            ++nstate_मगा_बागाय;
          }
    }
  state_item_map = xnmalloc (nstates + 1, sizeof (state_item_नुमेरो));
  state_मगा_बागाय = xnmalloc (nstate_मगा_बागाय, sizeof (state_item));
  state_item_नुमेरो sidx = 0;
  for (int i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      int rule_हानाप_idx = 0;
      const state *s = states[i];
      const reductions *red = s->reductions;
      for (int j = 0; j < s->nitems; ++j)
        {
          state_item_set (sidx, s, s->मगा_बागाय[j]);
          state_item *si = &state_मगा_बागाय[sidx];
          const rule *r = item_rule (si->item);
          if (rule_हानाप_idx < red->नुमेरो && red->rules[rule_हानाप_idx] < r)
            ++rule_हानाप_idx;
          if (rule_हानाप_idx < red->नुमेरो && r == red->rules[rule_हानाप_idx])
            {
              bitsetv lookahead = red->lookaheads;
              if (lookahead)
                si->lookahead = lookahead[rule_हानाप_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_नुमेरो off;
      BITSET_FOR_EACH (biter, production_मगा_बागाय[i], off, 0)
        {
          state_item_set (sidx, s, off);
          if (item_नुमेरो_is_rule_नुमेरो (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              if (lookahead)
                state_मगा_बागाय[sidx].lookahead = lookahead[rule_हानाप_idx];
              ++rule_हानाप_idx;
            }
          ++sidx;
        }

    }
  state_item_map[nstates] = nstate_मगा_बागाय;
  bitsetv_free (production_मगा_बागाय);
}

static size_t
state_sym_hasher (const void *st, size_t max)
{
  return ((state *) st)->accessing_सिम्बोलो % max;
}

static bool
state_sym_comparator (const void *s1, const void *s2)
{
  return ((state *) s1)->accessing_सिम्बोलो == ((state *) s2)->accessing_सिम्बोलो;
}

static state *
state_sym_lookup (सिम्बोलो_नुमेरो sym, Hash_table *h)
{
  state probe;
  probe.accessing_सिम्बोलो = sym;
  return hash_lookup (h, &probe);
}

static void
init_trans (void)
{
  for (state_नुमेरो i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = states[i];
      transitions *t = s->transitions;
      Hash_table *transition_set
        = hash_xinitialize (t->नुमेरो, NULL, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, NULL);
      for (int j = 0; j < t->नुमेरो; ++j)
        if (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_set, t->states[j]);
      for (state_item_नुमेरो j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_नुमेरो *item = state_मगा_बागाय[j].item;
          if (item_नुमेरो_is_rule_नुमेरो (*item))
            continue;
          state *dst = state_sym_lookup (*item, transition_set);
          if (!dst)
            continue;
          // find the item in the destination state that corresponds
          // to the transition of item
          for (int k = 0; k < dst->nitems; ++k)
            if (item + 1 == ritem + dst->मगा_बागाय[k])
              {
                state_item_नुमेरो dstSI =
                  state_item_index_lookup (dst->नुमेरो_2, k);

                state_मगा_बागाय[j].trans = dstSI;
                bitset_set (state_मगा_बागाय[dstSI].revs, j);
                break;
            }
        }
      hash_free (transition_set);
    }
}

static void
init_prods (void)
{
  for (int i = 0; i < nstates; ++i)
    {
      state *s = states[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_table *closure_map = hash_pair_table_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      for (state_item_नुमेरो j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_मगा_बागाय[j];
          item_नुमेरो *item = src->item;
          सिम्बोलो_नुमेरो lhs = item_rule (item)->lhs->नुमेरो_2;
          bitset itms = hash_pair_lookup (closure_map, lhs);
          if (!itms)
            {
              itms = bitset_create (nstate_मगा_बागाय, BITSET_SPARSE);
              hash_pair_इसिंगित (closure_map, lhs, itms);
            }
          bitset_set (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      for (state_item_नुमेरो j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_मगा_बागाय[j];
          item_नुमेरो item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          if (item_नुमेरो_is_rule_नुमेरो (item) || ISTOKEN (item))
            continue;
          सिम्बोलो_नुमेरो sym = item_नुमेरो_as_सिम्बोलो_नुमेरो (item);
          bitset lb = hash_pair_lookup (closure_map, sym);
          if (lb)
            {
              bitset copy = bitset_create (nstate_मगा_बागाय, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_मगा_बागाय[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_नुमेरो prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_set (state_मगा_बागाय[prod].revs, j);
            }
        }
      hash_free (closure_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
static inline void
gen_lookaheads (void)
{
  for (state_item_नुमेरो i = 0; i < nstate_मगा_बागाय; ++i)
    {
      state_item *si = &state_मगा_बागाय[i];
      if (item_नुमेरो_is_सिम्बोलो_नुमेरो (*(si->item)) || !si->lookahead)
        continue;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true, 1,
                        (const void **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      while (gl_list_size (queue) > 0)
        {
          state_item *prev = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          prev->lookahead = lookahead;
          if (SI_TRANSITION (prev))
            {
              bitset rsi = state_मगा_बागाय[prev - state_मगा_बागाय].revs;
              bitset_iterator biter;
              state_item_नुमेरो sin;
              BITSET_FOR_EACH (biter, rsi, sin, 0)
                gl_list_add_first (queue, &state_मगा_बागाय[sin]);
            }
        }
      gl_list_free (queue);
    }
}

bitsetv firsts = NULL;

static void
init_firsts (void)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  for (rule_नुमेरो i = 0; i < nrules; ++i)
    {
      rule *r = &rules[i];
      item_नुमेरो *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      while (item_नुमेरो_is_सिम्बोलो_नुमेरो (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      if (item_नुमेरो_is_rule_नुमेरो (*n) || ISVAR (*n))
        continue;

      सिम्बोलो_नुमेरो lhs = r->lhs->नुमेरो_2;
      bitset_set (FIRSTS (lhs), *n);
    }
  bool change = true;
  while (change)
    {
      change = false;
      for (rule_नुमेरो i = 0; i < nrules; ++i)
        {
          rule *r = &rules[i];
          सिम्बोलो_नुमेरो lhs = r->lhs->नुमेरो_2;
          bitset f_lhs = FIRSTS (lhs);
          for (item_नुमेरो *n = r->rhs;
               item_नुमेरो_is_सिम्बोलो_नुमेरो (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              if (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              if (!nullable[*n - ntokens])
                break;
            }
        }
    }
}

static inline void
disable_state_item (state_item *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  if (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
static void
prune_state_item (const state_item *si)
{
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, NULL, NULL, NULL, true, 1,
                    (const void **) &si);

  while (gl_list_size (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      if (SI_DISABLED (dsi - state_मगा_बागाय))
        continue;

      if (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        const state_item *trans = &state_मगा_बागाय[dsi->trans];
        bitset_reset (trans->revs, dsi - state_मगा_बागाय);
        if (bitset_empty_p (trans->revs))
          gl_list_add_last (queue, trans);
      }

      if (dsi->prods)
        {
          bitset_iterator biter;
          state_item_नुमेरो sin;
          BITSET_FOR_EACH (biter, dsi->prods, sin, 0)
            {
              if (SI_DISABLED (sin))
                continue;
              const state_item *prod = &state_मगा_बागाय[sin];
              bitset_reset (prod->revs, dsi - state_मगा_बागाय);
              if (bitset_empty_p (prod->revs))
                gl_list_add_last (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_नुमेरो sin;
      BITSET_FOR_EACH (biter, dsi->revs, sin, 0)
        {
          if (SI_DISABLED (sin))
            continue;
          state_item *rev = &state_मगा_बागाय[sin];
          if (&state_मगा_बागाय[rev->trans] == dsi)
            gl_list_add_last (queue, rev);
          else if (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_मगा_बागाय);
              if (bitset_empty_p (rev->prods))
                gl_list_add_last (queue, rev);
            }
          else
            gl_list_add_last (queue, rev);
        }
      disable_state_item (dsi);
    }
  gl_list_free (queue);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
static void
prune_disabled_paths (void)
{
  for (int i = nstate_मगा_बागाय - 1; i >= 0; --i)
    {
      state_item *si = &state_मगा_बागाय[i];
      if (si->trans == -1 && item_नुमेरो_is_सिम्बोलो_नुमेरो (*si->item))
        prune_state_item (si);
    }
}

void
state_item_इलिम्बाग (const state_item *si, FILE *out, const char *prefix)
{
  fputs (prefix, out);
  item_इलिम्बाग (si->item, NULL, out);
  putc ('\n', out);
}

const rule*
state_item_rule (const state_item *si)
{
  return item_rule (si->item);
}

/**
 * Report the state_item graph
 */
static void
state_मगा_बागाय_report (FILE *out)
{
  fprintf (out, "# state items: %zu\n", nstate_मगा_बागाय);
  for (state_नुमेरो i = 0; i < nstates; ++i)
    {
      fprintf (out, "State %d:\n", i);
      for (state_item_नुमेरो j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          const state_item *si = &state_मगा_बागाय[j];
          item_इलिम्बाग (si->item, NULL, out);
          if (SI_DISABLED (j))
            fputs ("  DISABLED\n", out);
          else
            {
              putc ('\n', out);
              if (si->trans >= 0)
                {
                  fputs ("    -> ", out);
                  state_item_इलिम्बाग (&state_मगा_बागाय[si->trans], out, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              const char *txt[2] = { "    => ", "    <- " };
              for (int seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  if (b)
                    {
                      bitset_iterator biter;
                      state_item_नुमेरो sin;
                      BITSET_FOR_EACH (biter, b, sin, 0)
                        {
                          fputs (txt[seti], out);
                          state_item_इलिम्बाग (&state_मगा_बागाय[sin], out, "");
                        }
                    }
                }
            }
          putc ('\n', out);
        }
    }
  fprintf (out, "FIRSTS\n");
  for (सिम्बोलो_नुमेरो i = ntokens; i < nsyms; ++i)
    {
      fprintf (out, "  %s firsts\n", symbols[i]->tag);
      bitset_iterator iter;
      सिम्बोलो_नुमेरो j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (out, "    %s\n", symbols[j]->tag);
    }
  fputs ("\n\n", out);
}

void
state_मगा_बागाय_init (void)
{
  xtime_t start = gethrxtime ();
  init_state_मगा_बागाय ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  if (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_मगा_बागाय_report (stderr);
    }
}

void
state_मगा_बागाय_free (void)
{
  for (state_item_नुमेरो i = 0; i < nstate_मगा_बागाय; ++i)
    if (!SI_DISABLED (i))
      {
        state_item *si = &state_मगा_बागाय[i];
        if (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  free (state_मगा_बागाय);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (const state_item *si, const state_item *next)
{
  sym_content *s1 = item_rule (si->item)->lhs;
  sym_content *s2 = item_rule (next->item)->lhs;
  int prec1 = s1->prec;
  int prec2 = s2->prec;
  if (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      if (prec1 > prec2)
        return false;
      // Do not expand if same precedence, but left-associative.
      if (prec1 == prec2 && s1->assoc == कालिवा_assoc)
        return false;
    }
  return true;
}
