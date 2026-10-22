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

size_t nstate_اشیاء;
state_شے_نمبر *state_شے_map;
state_شے *state_اشیاء;

// Hash functions for index -> bitset hash maps.
تعريفنوع بنية
{
  صحيح کلید;
  bitset l;
} hash_pair;

ساكن size_t
hash_pair_hasher (ثابت hash_pair *sl, size_t max)
{
  أرجع sl->کلید % max;
}

ساكن bool
hash_pair_comparator (ثابت hash_pair *l, ثابت hash_pair *r)
{
  أرجع l->کلید == r->کلید;
}

ساكن فراغ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  حرر (hp);
}

ساكن Hash_جدول *
hash_pair_جدول_create (صحيح ناپ)
{
  أرجع hash_xinitialize (ناپ,
                           عدم,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_انکڑے_freer) hash_pair_free);
}

ساكن bitset
hash_pair_lookup (Hash_جدول *tab, صحيح کلید)
{
  hash_pair probe;
  probe.کلید = کلید;
  hash_pair *hp = hash_lookup (tab, &probe);
  أرجع hp ? hp->l : عدم;
}

ساكن فراغ
hash_pair_داخل_کرو (Hash_جدول *tab, صحيح کلید, bitset val)
{
  hash_pair *hp = xmalloc (حجم *hp);
  hp->کلید = کلید;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (فراغ) res;
  أكد (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_شے *
state_شے_lookup (state_نمبر s, state_شے_نمبر off)
{
  أرجع &state_اشیاء[state_شے_اشاریہ_lookup (s, off)];
}

ساكن مضمن فراغ
state_شے_سیٹ (state_شے_نمبر sidx, ثابت state *s, شے_نمبر off)
{
  state_اشیاء[sidx].state = s;
  state_اشیاء[sidx].شے = &ritem[off];
  state_اشیاء[sidx].lookahead = عدم;
  state_اشیاء[sidx].trans = -1;
  state_اشیاء[sidx].prods = عدم;
  state_اشیاء[sidx].revs = bitset_create (nstate_اشیاء, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ساكن فراغ
init_state_اشیاء (فراغ)
{
  nstate_اشیاء = 0;
  bitsetv production_اشیاء = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      ثابت state *s = حالتاں[i];
      nstate_اشیاء += s->nitems;
      گھیرا (s->اشیاء, s->nitems);
      لكل (size_t j = 0; j < nitemset; ++j)
        إذا (0 < itemset[j]
            && شے_نمبر_is_قاعدہ_نمبر (ritem[itemset[j] - 1]))
          {
            bitset_سیٹ (production_اشیاء[i], itemset[j]);
            ++nstate_اشیاء;
          }
    }
  state_شے_map = xnmalloc (nstates + 1, حجم (state_شے_نمبر));
  state_اشیاء = xnmalloc (nstate_اشیاء, حجم (state_شے));
  state_شے_نمبر sidx = 0;
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state_شے_map[i] = sidx;
      صحيح قاعدہ_تلاش_idx = 0;
      ثابت state *s = حالتاں[i];
      ثابت reductions *red = s->reductions;
      لكل (صحيح j = 0; j < s->nitems; ++j)
        {
          state_شے_سیٹ (sidx, s, s->اشیاء[j]);
          state_شے *si = &state_اشیاء[sidx];
          ثابت قاعدہ *r = شے_قاعدہ (si->شے);
          إذا (قاعدہ_تلاش_idx < red->عدد && red->قاعدے[قاعدہ_تلاش_idx] < r)
            ++قاعدہ_تلاش_idx;
          إذا (قاعدہ_تلاش_idx < red->عدد && r == red->قاعدے[قاعدہ_تلاش_idx])
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                si->lookahead = lookahead[قاعدہ_تلاش_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      شے_نمبر off;
      BITSET_FOR_EACH (biter, production_اشیاء[i], off, 0)
        {
          state_شے_سیٹ (sidx, s, off);
          إذا (شے_نمبر_is_قاعدہ_نمبر (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                state_اشیاء[sidx].lookahead = lookahead[قاعدہ_تلاش_idx];
              ++قاعدہ_تلاش_idx;
            }
          ++sidx;
        }

    }
  state_شے_map[nstates] = nstate_اشیاء;
  bitsetv_free (production_اشیاء);
}

ساكن size_t
state_sym_hasher (ثابت فراغ *st, size_t max)
{
  أرجع ((state *) st)->accessing_علامت % max;
}

ساكن bool
state_sym_comparator (ثابت فراغ *s1, ثابت فراغ *s2)
{
  أرجع ((state *) s1)->accessing_علامت == ((state *) s2)->accessing_علامت;
}

ساكن state *
state_sym_lookup (علامت_نمبر sym, Hash_جدول *h)
{
  state probe;
  probe.accessing_علامت = sym;
  أرجع hash_lookup (h, &probe);
}

ساكن فراغ
init_trans (فراغ)
{
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = حالتاں[i];
      transitions *t = s->transitions;
      Hash_جدول *transition_سیٹ
        = hash_xinitialize (t->عدد, عدم, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, عدم);
      لكل (صحيح j = 0; j < t->عدد; ++j)
        إذا (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_سیٹ, t->حالتاں[j]);
      لكل (state_شے_نمبر j = state_شے_map[i]; j < state_شے_map[i + 1]; ++j)
        {
          شے_نمبر *شے = state_اشیاء[j].شے;
          إذا (شے_نمبر_is_قاعدہ_نمبر (*شے))
            استمر;
          state *dst = state_sym_lookup (*شے, transition_سیٹ);
          إذا (!dst)
            استمر;
          // find the item in the destination state that corresponds
          // to the transition of item
          لكل (صحيح k = 0; k < dst->nitems; ++k)
            إذا (شے + 1 == ritem + dst->اشیاء[k])
              {
                state_شے_نمبر dstSI =
                  state_شے_اشاریہ_lookup (dst->نمبر, k);

                state_اشیاء[j].trans = dstSI;
                bitset_سیٹ (state_اشیاء[dstSI].revs, j);
                اكسر;
            }
        }
      hash_free (transition_سیٹ);
    }
}

ساكن فراغ
init_prods (فراغ)
{
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state *s = حالتاں[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_جدول *گھیرا_map = hash_pair_جدول_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      لكل (state_شے_نمبر j = state_شے_map[i] + s->nitems;
           j < state_شے_map[i + 1]; ++j)
        {
          state_شے *src = &state_اشیاء[j];
          شے_نمبر *شے = src->شے;
          علامت_نمبر lhs = شے_قاعدہ (شے)->lhs->نمبر;
          bitset itms = hash_pair_lookup (گھیرا_map, lhs);
          إذا (!itms)
            {
              itms = bitset_create (nstate_اشیاء, BITSET_SPARSE);
              hash_pair_داخل_کرو (گھیرا_map, lhs, itms);
            }
          bitset_سیٹ (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      لكل (state_شے_نمبر j = state_شے_map[i]; j < state_شے_map[i + 1]; ++j)
        {
          state_شے *src = &state_اشیاء[j];
          شے_نمبر شے = *(src->شے);
          // Skip reduce items and items with terminals after the dot
          إذا (شے_نمبر_is_قاعدہ_نمبر (شے) || ISTOKEN (شے))
            استمر;
          علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (شے);
          bitset lb = hash_pair_lookup (گھیرا_map, sym);
          إذا (lb)
            {
              bitset copy = bitset_create (nstate_اشیاء, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_اشیاء[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_شے_نمبر prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_سیٹ (state_اشیاء[prod].revs, j);
            }
        }
      hash_free (گھیرا_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ساكن مضمن فراغ
gen_lookaheads (فراغ)
{
  لكل (state_شے_نمبر i = 0; i < nstate_اشیاء; ++i)
    {
      state_شے *si = &state_اشیاء[i];
      إذا (شے_نمبر_is_علامت_نمبر (*(si->شے)) || !si->lookahead)
        استمر;

      bitset lookahead = si->lookahead;
      state_شے_list قطار =
        gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                        (ثابت فراغ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      طالما (gl_list_ناپ (قطار) > 0)
        {
          state_شے *prev = (state_شے *) gl_list_get_at (قطار, 0);
          gl_list_remove_at (قطار, 0);
          prev->lookahead = lookahead;
          إذا (SI_TRANSITION (prev))
            {
              bitset rsi = state_اشیاء[prev - state_اشیاء].revs;
              bitset_iterator biter;
              state_شے_نمبر جیب;
              BITSET_FOR_EACH (biter, rsi, جیب, 0)
                gl_list_جوڑو_پہلا (قطار, &state_اشیاء[جیب]);
            }
        }
      gl_list_free (قطار);
    }
}

bitsetv firsts = عدم;

ساكن فراغ
init_firsts (فراغ)
{
  firsts = bitsetv_create (nnterms, nsyms, BITSET_FIXED);
  لكل (قاعدہ_نمبر i = 0; i < nrules; ++i)
    {
      قاعدہ *r = &قاعدے[i];
      شے_نمبر *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      طالما (شے_نمبر_is_علامت_نمبر (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      إذا (شے_نمبر_is_قاعدہ_نمبر (*n) || ISVAR (*n))
        استمر;

      علامت_نمبر lhs = r->lhs->نمبر;
      bitset_سیٹ (FIRSTS (lhs), *n);
    }
  bool change = true;
  طالما (change)
    {
      change = false;
      لكل (قاعدہ_نمبر i = 0; i < nrules; ++i)
        {
          قاعدہ *r = &قاعدے[i];
          علامت_نمبر lhs = r->lhs->نمبر;
          bitset f_lhs = FIRSTS (lhs);
          لكل (شے_نمبر *n = r->rhs;
               شے_نمبر_is_علامت_نمبر (*n) && ISVAR (*n);
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
disable_state_شے (state_شے *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  إذا (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ساكن فراغ
prune_state_شے (ثابت state_شے *si)
{
  state_شے_list قطار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &si);

  طالما (gl_list_ناپ (قطار) > 0)
    {
      state_شے *dsi = (state_شے *) gl_list_get_at (قطار, 0);
      gl_list_remove_at (قطار, 0);
      إذا (SI_DISABLED (dsi - state_اشیاء))
        استمر;

      إذا (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ثابت state_شے *trans = &state_اشیاء[dsi->trans];
        bitset_reset (trans->revs, dsi - state_اشیاء);
        إذا (bitset_خالی_تھاواں_p (trans->revs))
          gl_list_جوڑو_آخری (قطار, trans);
      }

      إذا (dsi->prods)
        {
          bitset_iterator biter;
          state_شے_نمبر جیب;
          BITSET_FOR_EACH (biter, dsi->prods, جیب, 0)
            {
              إذا (SI_DISABLED (جیب))
                استمر;
              ثابت state_شے *prod = &state_اشیاء[جیب];
              bitset_reset (prod->revs, dsi - state_اشیاء);
              إذا (bitset_خالی_تھاواں_p (prod->revs))
                gl_list_جوڑو_آخری (قطار, prod);
            }
        }

      bitset_iterator biter;
      state_شے_نمبر جیب;
      BITSET_FOR_EACH (biter, dsi->revs, جیب, 0)
        {
          إذا (SI_DISABLED (جیب))
            استمر;
          state_شے *rev = &state_اشیاء[جیب];
          إذا (&state_اشیاء[rev->trans] == dsi)
            gl_list_جوڑو_آخری (قطار, rev);
          وإلا إذا (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_اشیاء);
              إذا (bitset_خالی_تھاواں_p (rev->prods))
                gl_list_جوڑو_آخری (قطار, rev);
            }
          وإلا
            gl_list_جوڑو_آخری (قطار, rev);
        }
      disable_state_شے (dsi);
    }
  gl_list_free (قطار);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ساكن فراغ
prune_disabled_paths (فراغ)
{
  لكل (صحيح i = nstate_اشیاء - 1; i >= 0; --i)
    {
      state_شے *si = &state_اشیاء[i];
      إذا (si->trans == -1 && شے_نمبر_is_علامت_نمبر (*si->شے))
        prune_state_شے (si);
    }
}

فراغ
state_شے_print (ثابت state_شے *si, FILE *باہرلا, ثابت حرف *اگیتر)
{
  fputs (اگیتر, باہرلا);
  شے_print (si->شے, عدم, باہرلا);
  putc ('\n', باہرلا);
}

ثابت قاعدہ*
state_شے_قاعدہ (ثابت state_شے *si)
{
  أرجع شے_قاعدہ (si->شے);
}

/**
 * Report the state_item graph
 */
ساكن فراغ
state_اشیاء_report (FILE *باہرلا)
{
  fprintf (باہرلا, "# state items: %zu\n", nstate_اشیاء);
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      fprintf (باہرلا, "State %d:\n", i);
      لكل (state_شے_نمبر j = state_شے_map[i]; j < state_شے_map[i + 1]; ++j)
        {
          ثابت state_شے *si = &state_اشیاء[j];
          شے_print (si->شے, عدم, باہرلا);
          إذا (SI_DISABLED (j))
            fputs ("  DISABLED\n", باہرلا);
          وإلا
            {
              putc ('\n', باہرلا);
              إذا (si->trans >= 0)
                {
                  fputs ("    -> ", باہرلا);
                  state_شے_print (&state_اشیاء[si->trans], باہرلا, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ثابت حرف *txt[2] = { "    => ", "    <- " };
              لكل (صحيح seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  إذا (b)
                    {
                      bitset_iterator biter;
                      state_شے_نمبر جیب;
                      BITSET_FOR_EACH (biter, b, جیب, 0)
                        {
                          fputs (txt[seti], باہرلا);
                          state_شے_print (&state_اشیاء[جیب], باہرلا, "");
                        }
                    }
                }
            }
          putc ('\n', باہرلا);
        }
    }
  fprintf (باہرلا, "FIRSTS\n");
  لكل (علامت_نمبر i = ntokens; i < nsyms; ++i)
    {
      fprintf (باہرلا, "  %s firsts\n", symbols[i]->ٹیگ);
      bitset_iterator iter;
      علامت_نمبر j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (باہرلا, "    %s\n", symbols[j]->ٹیگ);
    }
  fputs ("\n\n", باہرلا);
}

فراغ
state_اشیاء_init (فراغ)
{
  xtime_t شروع = gethrxtime ();
  init_state_اشیاء ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  إذا (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - شروع) / 1e9);
      state_اشیاء_report (stderr);
    }
}

فراغ
state_اشیاء_free (فراغ)
{
  لكل (state_شے_نمبر i = 0; i < nstate_اشیاء; ++i)
    إذا (!SI_DISABLED (i))
      {
        state_شے *si = &state_اشیاء[i];
        إذا (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  حرر (state_اشیاء);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ثابت state_شے *si, ثابت state_شے *اگلا)
{
  sym_content *s1 = شے_قاعدہ (si->شے)->lhs;
  sym_content *s2 = شے_قاعدہ (اگلا->شے)->lhs;
  صحيح prec1 = s1->prec;
  صحيح prec2 = s2->prec;
  إذا (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      إذا (prec1 > prec2)
        أرجع false;
      // Do not expand if same precedence, but left-associative.
      إذا (prec1 == prec2 && s1->assoc == بایاں_assoc)
        أرجع false;
    }
  أرجع true;
}
