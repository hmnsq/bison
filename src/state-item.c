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

#ضمن <config.h>

#ضمن "state-item.h"

#ضمن <assert.h>
#ضمن <gethrxtime.h>
#ضمن <gl_linked_list.h>
#ضمن <gl_xlist.h>
#ضمن <stdlib.h>
#ضمن <time.h>

#ضمن "closure.h"
#ضمن "getargs.h"
#ضمن "nullable.h"

size_t nstate_توکي;
state_توکی_شمېره *state_توکی_map;
state_توکی *state_توکي;

// Hash functions for index -> bitset hash maps.
تعريفنوع بنية
{
  صحيح کلی;
  bitset l;
} hash_pair;

ساكن size_t
hash_pair_hasher (ثابت hash_pair *sl, size_t max)
{
  أرجع sl->کلی % max;
}

ساكن bool
hash_pair_comparator (ثابت hash_pair *l, ثابت hash_pair *r)
{
  أرجع l->کلی == r->کلی;
}

ساكن فراغ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  حرر (hp);
}

ساكن Hash_جدول *
hash_pair_جدول_create (صحيح اندازه)
{
  أرجع hash_xinitialize (اندازه,
                           عدم,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_معلومات_freer) hash_pair_free);
}

ساكن bitset
hash_pair_lookup (Hash_جدول *tab, صحيح کلی)
{
  hash_pair probe;
  probe.کلی = کلی;
  hash_pair *hp = hash_lookup (tab, &probe);
  أرجع hp ? hp->l : عدم;
}

ساكن فراغ
hash_pair_ورننباسه (Hash_جدول *tab, صحيح کلی, bitset val)
{
  hash_pair *hp = xmalloc (حجم *hp);
  hp->کلی = کلی;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (فراغ) res;
  أكد (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_توکی *
state_توکی_lookup (state_شمېره s, state_توکی_شمېره off)
{
  أرجع &state_توکي[state_توکی_شاخص_lookup (s, off)];
}

ساكن مضمن فراغ
state_توکی_سېټ (state_توکی_شمېره sidx, ثابت state *s, توکی_شمېره off)
{
  state_توکي[sidx].state = s;
  state_توکي[sidx].توکی = &ritem[off];
  state_توکي[sidx].lookahead = عدم;
  state_توکي[sidx].trans = -1;
  state_توکي[sidx].prods = عدم;
  state_توکي[sidx].revs = bitset_create (nstate_توکي, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ساكن فراغ
init_state_توکي (فراغ)
{
  nstate_توکي = 0;
  bitsetv production_توکي = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      ثابت state *s = حالتونه[i];
      nstate_توکي += s->nitems;
      تړون (s->توکي, s->nitems);
      لكل (size_t j = 0; j < nitemset; ++j)
        إذا (0 < itemset[j]
            && توکی_شمېره_is_قاعده_شمېره (ritem[itemset[j] - 1]))
          {
            bitset_سېټ (production_توکي[i], itemset[j]);
            ++nstate_توکي;
          }
    }
  state_توکی_map = xnmalloc (nstates + 1, حجم (state_توکی_شمېره));
  state_توکي = xnmalloc (nstate_توکي, حجم (state_توکی));
  state_توکی_شمېره sidx = 0;
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state_توکی_map[i] = sidx;
      صحيح قاعده_پلټنه_idx = 0;
      ثابت state *s = حالتونه[i];
      ثابت reductions *red = s->reductions;
      لكل (صحيح j = 0; j < s->nitems; ++j)
        {
          state_توکی_سېټ (sidx, s, s->توکي[j]);
          state_توکی *si = &state_توکي[sidx];
          ثابت قاعده *r = توکی_قاعده (si->توکی);
          إذا (قاعده_پلټنه_idx < red->عدد && red->قواعد[قاعده_پلټنه_idx] < r)
            ++قاعده_پلټنه_idx;
          إذا (قاعده_پلټنه_idx < red->عدد && r == red->قواعد[قاعده_پلټنه_idx])
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                si->lookahead = lookahead[قاعده_پلټنه_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      توکی_شمېره off;
      BITSET_FOR_EACH (biter, production_توکي[i], off, 0)
        {
          state_توکی_سېټ (sidx, s, off);
          إذا (توکی_شمېره_is_قاعده_شمېره (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                state_توکي[sidx].lookahead = lookahead[قاعده_پلټنه_idx];
              ++قاعده_پلټنه_idx;
            }
          ++sidx;
        }

    }
  state_توکی_map[nstates] = nstate_توکي;
  bitsetv_free (production_توکي);
}

ساكن size_t
state_sym_hasher (ثابت فراغ *st, size_t max)
{
  أرجع ((state *) st)->accessing_نښه % max;
}

ساكن bool
state_sym_comparator (ثابت فراغ *s1, ثابت فراغ *s2)
{
  أرجع ((state *) s1)->accessing_نښه == ((state *) s2)->accessing_نښه;
}

ساكن state *
state_sym_lookup (نښه_شمېره sym, Hash_جدول *h)
{
  state probe;
  probe.accessing_نښه = sym;
  أرجع hash_lookup (h, &probe);
}

ساكن فراغ
init_trans (فراغ)
{
  لكل (state_شمېره i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = حالتونه[i];
      transitions *t = s->transitions;
      Hash_جدول *transition_سېټ
        = hash_xinitialize (t->عدد, عدم, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, عدم);
      لكل (صحيح j = 0; j < t->عدد; ++j)
        إذا (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_سېټ, t->حالتونه[j]);
      لكل (state_توکی_شمېره j = state_توکی_map[i]; j < state_توکی_map[i + 1]; ++j)
        {
          توکی_شمېره *توکی = state_توکي[j].توکی;
          إذا (توکی_شمېره_is_قاعده_شمېره (*توکی))
            استمر;
          state *dst = state_sym_lookup (*توکی, transition_سېټ);
          إذا (!dst)
            استمر;
          // find the item in the destination state that corresponds
          // to the transition of item
          لكل (صحيح k = 0; k < dst->nitems; ++k)
            إذا (توکی + 1 == ritem + dst->توکي[k])
              {
                state_توکی_شمېره dstSI =
                  state_توکی_شاخص_lookup (dst->شمېره, k);

                state_توکي[j].trans = dstSI;
                bitset_سېټ (state_توکي[dstSI].revs, j);
                اكسر;
            }
        }
      hash_free (transition_سېټ);
    }
}

ساكن فراغ
init_prods (فراغ)
{
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state *s = حالتونه[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_جدول *تړون_map = hash_pair_جدول_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      لكل (state_توکی_شمېره j = state_توکی_map[i] + s->nitems;
           j < state_توکی_map[i + 1]; ++j)
        {
          state_توکی *src = &state_توکي[j];
          توکی_شمېره *توکی = src->توکی;
          نښه_شمېره lhs = توکی_قاعده (توکی)->lhs->شمېره;
          bitset itms = hash_pair_lookup (تړون_map, lhs);
          إذا (!itms)
            {
              itms = bitset_create (nstate_توکي, BITSET_SPARSE);
              hash_pair_ورننباسه (تړون_map, lhs, itms);
            }
          bitset_سېټ (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      لكل (state_توکی_شمېره j = state_توکی_map[i]; j < state_توکی_map[i + 1]; ++j)
        {
          state_توکی *src = &state_توکي[j];
          توکی_شمېره توکی = *(src->توکی);
          // Skip reduce items and items with terminals after the dot
          إذا (توکی_شمېره_is_قاعده_شمېره (توکی) || ISTOKEN (توکی))
            استمر;
          نښه_شمېره sym = توکی_شمېره_as_نښه_شمېره (توکی);
          bitset lb = hash_pair_lookup (تړون_map, sym);
          إذا (lb)
            {
              bitset copy = bitset_create (nstate_توکي, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_توکي[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_توکی_شمېره prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_سېټ (state_توکي[prod].revs, j);
            }
        }
      hash_free (تړون_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ساكن مضمن فراغ
gen_lookaheads (فراغ)
{
  لكل (state_توکی_شمېره i = 0; i < nstate_توکي; ++i)
    {
      state_توکی *si = &state_توکي[i];
      إذا (توکی_شمېره_is_نښه_شمېره (*(si->توکی)) || !si->lookahead)
        استمر;

      bitset lookahead = si->lookahead;
      state_توکی_list کتار =
        gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                        (ثابت فراغ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      طالما (gl_list_اندازه (کتار) > 0)
        {
          state_توکی *prev = (state_توکی *) gl_list_get_at (کتار, 0);
          gl_list_remove_at (کتار, 0);
          prev->lookahead = lookahead;
          إذا (SI_TRANSITION (prev))
            {
              bitset rsi = state_توکي[prev - state_توکي].revs;
              bitset_iterator biter;
              state_توکی_شمېره ساین;
              BITSET_FOR_EACH (biter, rsi, ساین, 0)
                gl_list_جمع_کړه_لومړی (کتار, &state_توکي[ساین]);
            }
        }
      gl_list_free (کتار);
    }
}

bitsetv firsts = عدم;

ساكن فراغ
init_firsts (فراغ)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  لكل (قاعده_شمېره i = 0; i < nrules; ++i)
    {
      قاعده *r = &قواعد[i];
      توکی_شمېره *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      طالما (توکی_شمېره_is_نښه_شمېره (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      إذا (توکی_شمېره_is_قاعده_شمېره (*n) || ISVAR (*n))
        استمر;

      نښه_شمېره lhs = r->lhs->شمېره;
      bitset_سېټ (FIRSTS (lhs), *n);
    }
  bool change = true;
  طالما (change)
    {
      change = false;
      لكل (قاعده_شمېره i = 0; i < nrules; ++i)
        {
          قاعده *r = &قواعد[i];
          نښه_شمېره lhs = r->lhs->شمېره;
          bitset f_lhs = FIRSTS (lhs);
          لكل (توکی_شمېره *n = r->rhs;
               توکی_شمېره_is_نښه_شمېره (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              إذا (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              إذا (!nullable[*n - ntokens])
                اكسر;
            }
        }
    }
}

ساكن مضمن فراغ
disable_state_توکی (state_توکی *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  إذا (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ساكن فراغ
prune_state_توکی (ثابت state_توکی *si)
{
  state_توکی_list کتار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &si);

  طالما (gl_list_اندازه (کتار) > 0)
    {
      state_توکی *dsi = (state_توکی *) gl_list_get_at (کتار, 0);
      gl_list_remove_at (کتار, 0);
      إذا (SI_DISABLED (dsi - state_توکي))
        استمر;

      إذا (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ثابت state_توکی *trans = &state_توکي[dsi->trans];
        bitset_reset (trans->revs, dsi - state_توکي);
        إذا (bitset_تشه_p (trans->revs))
          gl_list_جمع_کړه_وروستی (کتار, trans);
      }

      إذا (dsi->prods)
        {
          bitset_iterator biter;
          state_توکی_شمېره ساین;
          BITSET_FOR_EACH (biter, dsi->prods, ساین, 0)
            {
              إذا (SI_DISABLED (ساین))
                استمر;
              ثابت state_توکی *prod = &state_توکي[ساین];
              bitset_reset (prod->revs, dsi - state_توکي);
              إذا (bitset_تشه_p (prod->revs))
                gl_list_جمع_کړه_وروستی (کتار, prod);
            }
        }

      bitset_iterator biter;
      state_توکی_شمېره ساین;
      BITSET_FOR_EACH (biter, dsi->revs, ساین, 0)
        {
          إذا (SI_DISABLED (ساین))
            استمر;
          state_توکی *rev = &state_توکي[ساین];
          إذا (&state_توکي[rev->trans] == dsi)
            gl_list_جمع_کړه_وروستی (کتار, rev);
          وإلا إذا (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_توکي);
              إذا (bitset_تشه_p (rev->prods))
                gl_list_جمع_کړه_وروستی (کتار, rev);
            }
          وإلا
            gl_list_جمع_کړه_وروستی (کتار, rev);
        }
      disable_state_توکی (dsi);
    }
  gl_list_free (کتار);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ساكن فراغ
prune_disabled_paths (فراغ)
{
  لكل (صحيح i = nstate_توکي - 1; i >= 0; --i)
    {
      state_توکی *si = &state_توکي[i];
      إذا (si->trans == -1 && توکی_شمېره_is_نښه_شمېره (*si->توکی))
        prune_state_توکی (si);
    }
}

فراغ
state_توکی_print (ثابت state_توکی *si, FILE *بهر, ثابت حرف *مختاړی)
{
  fputs (مختاړی, بهر);
  توکی_print (si->توکی, عدم, بهر);
  putc ('\n', بهر);
}

ثابت قاعده*
state_توکی_قاعده (ثابت state_توکی *si)
{
  أرجع توکی_قاعده (si->توکی);
}

/**
 * Report the state_item graph
 */
ساكن فراغ
state_توکي_report (FILE *بهر)
{
  fprintf (بهر, "# state items: %zu\n", nstate_توکي);
  لكل (state_شمېره i = 0; i < nstates; ++i)
    {
      fprintf (بهر, "State %d:\n", i);
      لكل (state_توکی_شمېره j = state_توکی_map[i]; j < state_توکی_map[i + 1]; ++j)
        {
          ثابت state_توکی *si = &state_توکي[j];
          توکی_print (si->توکی, عدم, بهر);
          إذا (SI_DISABLED (j))
            fputs ("  DISABLED\n", بهر);
          وإلا
            {
              putc ('\n', بهر);
              إذا (si->trans >= 0)
                {
                  fputs ("    -> ", بهر);
                  state_توکی_print (&state_توکي[si->trans], بهر, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ثابت حرف *txt[2] = { "    => ", "    <- " };
              لكل (صحيح seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  إذا (b)
                    {
                      bitset_iterator biter;
                      state_توکی_شمېره ساین;
                      BITSET_FOR_EACH (biter, b, ساین, 0)
                        {
                          fputs (txt[seti], بهر);
                          state_توکی_print (&state_توکي[ساین], بهر, "");
                        }
                    }
                }
            }
          putc ('\n', بهر);
        }
    }
  fprintf (بهر, "FIRSTS\n");
  لكل (نښه_شمېره i = ntokens; i < nsyms; ++i)
    {
      fprintf (بهر, "  %s firsts\n", symbols[i]->نښان);
      bitset_iterator iter;
      نښه_شمېره j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (بهر, "    %s\n", symbols[j]->نښان);
    }
  fputs ("\n\n", بهر);
}

فراغ
state_توکي_init (فراغ)
{
  xtime_t پیل = gethrxtime ();
  init_state_توکي ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  إذا (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - پیل) / 1e9);
      state_توکي_report (stderr);
    }
}

فراغ
state_توکي_free (فراغ)
{
  لكل (state_توکی_شمېره i = 0; i < nstate_توکي; ++i)
    إذا (!SI_DISABLED (i))
      {
        state_توکی *si = &state_توکي[i];
        إذا (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  حرر (state_توکي);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ثابت state_توکی *si, ثابت state_توکی *بل)
{
  sym_content *s1 = توکی_قاعده (si->توکی)->lhs;
  sym_content *s2 = توکی_قاعده (بل->توکی)->lhs;
  صحيح prec1 = s1->prec;
  صحيح prec2 = s2->prec;
  إذا (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      إذا (prec1 > prec2)
        أرجع false;
      // Do not expand if same precedence, but left-associative.
      إذا (prec1 == prec2 && s1->assoc == کیڼ_assoc)
        أرجع false;
    }
  أرجع true;
}
