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

size_t nstate_شيون;
state_شي_عدد *state_شي_map;
state_شي *state_شيون;

// Hash functions for index -> bitset hash maps.
تعريفنوع بنية
{
  صحيح ڪنجي;
  bitset l;
} hash_pair;

ساكن size_t
hash_pair_hasher (ثابت hash_pair *sl, size_t max)
{
  أرجع sl->ڪنجي % max;
}

ساكن bool
hash_pair_comparator (ثابت hash_pair *l, ثابت hash_pair *r)
{
  أرجع l->ڪنجي == r->ڪنجي;
}

ساكن فراغ
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  حرر (hp);
}

ساكن Hash_جدول *
hash_pair_جدول_create (صحيح سائيز)
{
  أرجع hash_xinitialize (سائيز,
                           عدم,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_معلومات_freer) hash_pair_free);
}

ساكن bitset
hash_pair_lookup (Hash_جدول *tab, صحيح ڪنجي)
{
  hash_pair probe;
  probe.ڪنجي = ڪنجي;
  hash_pair *hp = hash_lookup (tab, &probe);
  أرجع hp ? hp->l : عدم;
}

ساكن فراغ
hash_pair_وجهو (Hash_جدول *tab, صحيح ڪنجي, bitset val)
{
  hash_pair *hp = xmalloc (حجم *hp);
  hp->ڪنجي = ڪنجي;
  hp->l = val;
  hash_pair *res = hash_xinsert (tab, hp);
  // This must be the first insertion.
  (فراغ) res;
  أكد (res == hp);
}

/* A state_item from a state's id and the offset of the item within
   the state. */
state_شي *
state_شي_lookup (state_عدد s, state_شي_عدد off)
{
  أرجع &state_شيون[state_شي_اشاريو_lookup (s, off)];
}

ساكن مضمن فراغ
state_شي_سيٽ (state_شي_عدد sidx, ثابت state *s, شي_عدد off)
{
  state_شيون[sidx].state = s;
  state_شيون[sidx].شي = &ritem[off];
  state_شيون[sidx].lookahead = عدم;
  state_شيون[sidx].trans = -1;
  state_شيون[sidx].prods = عدم;
  state_شيون[sidx].revs = bitset_create (nstate_شيون, BITSET_SPARSE);
}

/**
 * Initialize state_items set
 */
ساكن فراغ
init_state_شيون (فراغ)
{
  nstate_شيون = 0;
  bitsetv production_شيون = bitsetv_create (nstates, nritems, BITSET_SPARSE);
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      ثابت state *s = حالتون[i];
      nstate_شيون += s->nitems;
      گهيرو (s->شيون, s->nitems);
      لكل (size_t j = 0; j < nitemset; ++j)
        إذا (0 < itemset[j]
            && شي_عدد_is_قاعدو_عدد (ritem[itemset[j] - 1]))
          {
            bitset_سيٽ (production_شيون[i], itemset[j]);
            ++nstate_شيون;
          }
    }
  state_شي_map = xnmalloc (nstates + 1, حجم (state_شي_عدد));
  state_شيون = xnmalloc (nstate_شيون, حجم (state_شي));
  state_شي_عدد sidx = 0;
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state_شي_map[i] = sidx;
      صحيح قاعدو_ڳولا_idx = 0;
      ثابت state *s = حالتون[i];
      ثابت reductions *red = s->reductions;
      لكل (صحيح j = 0; j < s->nitems; ++j)
        {
          state_شي_سيٽ (sidx, s, s->شيون[j]);
          state_شي *si = &state_شيون[sidx];
          ثابت قاعدو *r = شي_قاعدو (si->شي);
          إذا (قاعدو_ڳولا_idx < red->انگ && red->قاعدا[قاعدو_ڳولا_idx] < r)
            ++قاعدو_ڳولا_idx;
          إذا (قاعدو_ڳولا_idx < red->انگ && r == red->قاعدا[قاعدو_ڳولا_idx])
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                si->lookahead = lookahead[قاعدو_ڳولا_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      شي_عدد off;
      BITSET_FOR_EACH (biter, production_شيون[i], off, 0)
        {
          state_شي_سيٽ (sidx, s, off);
          إذا (شي_عدد_is_قاعدو_عدد (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              إذا (lookahead)
                state_شيون[sidx].lookahead = lookahead[قاعدو_ڳولا_idx];
              ++قاعدو_ڳولا_idx;
            }
          ++sidx;
        }

    }
  state_شي_map[nstates] = nstate_شيون;
  bitsetv_free (production_شيون);
}

ساكن size_t
state_sym_hasher (ثابت فراغ *st, size_t max)
{
  أرجع ((state *) st)->accessing_نشان % max;
}

ساكن bool
state_sym_comparator (ثابت فراغ *s1, ثابت فراغ *s2)
{
  أرجع ((state *) s1)->accessing_نشان == ((state *) s2)->accessing_نشان;
}

ساكن state *
state_sym_lookup (نشان_عدد sym, Hash_جدول *h)
{
  state probe;
  probe.accessing_نشان = sym;
  أرجع hash_lookup (h, &probe);
}

ساكن فراغ
init_trans (فراغ)
{
  لكل (state_عدد i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = حالتون[i];
      transitions *t = s->transitions;
      Hash_جدول *transition_سيٽ
        = hash_xinitialize (t->انگ, عدم, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, عدم);
      لكل (صحيح j = 0; j < t->انگ; ++j)
        إذا (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_سيٽ, t->حالتون[j]);
      لكل (state_شي_عدد j = state_شي_map[i]; j < state_شي_map[i + 1]; ++j)
        {
          شي_عدد *شي = state_شيون[j].شي;
          إذا (شي_عدد_is_قاعدو_عدد (*شي))
            استمر;
          state *dst = state_sym_lookup (*شي, transition_سيٽ);
          إذا (!dst)
            استمر;
          // find the item in the destination state that corresponds
          // to the transition of item
          لكل (صحيح k = 0; k < dst->nitems; ++k)
            إذا (شي + 1 == ritem + dst->شيون[k])
              {
                state_شي_عدد dstSI =
                  state_شي_اشاريو_lookup (dst->عدد, k);

                state_شيون[j].trans = dstSI;
                bitset_سيٽ (state_شيون[dstSI].revs, j);
                اكسر;
            }
        }
      hash_free (transition_سيٽ);
    }
}

ساكن فراغ
init_prods (فراغ)
{
  لكل (صحيح i = 0; i < nstates; ++i)
    {
      state *s = حالتون[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_جدول *گهيرو_map = hash_pair_جدول_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      لكل (state_شي_عدد j = state_شي_map[i] + s->nitems;
           j < state_شي_map[i + 1]; ++j)
        {
          state_شي *src = &state_شيون[j];
          شي_عدد *شي = src->شي;
          نشان_عدد lhs = شي_قاعدو (شي)->lhs->عدد;
          bitset itms = hash_pair_lookup (گهيرو_map, lhs);
          إذا (!itms)
            {
              itms = bitset_create (nstate_شيون, BITSET_SPARSE);
              hash_pair_وجهو (گهيرو_map, lhs, itms);
            }
          bitset_سيٽ (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      لكل (state_شي_عدد j = state_شي_map[i]; j < state_شي_map[i + 1]; ++j)
        {
          state_شي *src = &state_شيون[j];
          شي_عدد شي = *(src->شي);
          // Skip reduce items and items with terminals after the dot
          إذا (شي_عدد_is_قاعدو_عدد (شي) || ISTOKEN (شي))
            استمر;
          نشان_عدد sym = شي_عدد_as_نشان_عدد (شي);
          bitset lb = hash_pair_lookup (گهيرو_map, sym);
          إذا (lb)
            {
              bitset copy = bitset_create (nstate_شيون, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_شيون[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_شي_عدد prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_سيٽ (state_شيون[prod].revs, j);
            }
        }
      hash_free (گهيرو_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
ساكن مضمن فراغ
gen_lookaheads (فراغ)
{
  لكل (state_شي_عدد i = 0; i < nstate_شيون; ++i)
    {
      state_شي *si = &state_شيون[i];
      إذا (شي_عدد_is_نشان_عدد (*(si->شي)) || !si->lookahead)
        استمر;

      bitset lookahead = si->lookahead;
      state_شي_list قطار =
        gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                        (ثابت فراغ **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      طالما (gl_list_سائيز (قطار) > 0)
        {
          state_شي *prev = (state_شي *) gl_list_get_at (قطار, 0);
          gl_list_remove_at (قطار, 0);
          prev->lookahead = lookahead;
          إذا (SI_TRANSITION (prev))
            {
              bitset rsi = state_شيون[prev - state_شيون].revs;
              bitset_iterator biter;
              state_شي_عدد جيب;
              BITSET_FOR_EACH (biter, rsi, جيب, 0)
                gl_list_جوڙيو_پهريون (قطار, &state_شيون[جيب]);
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
  لكل (قاعدو_عدد i = 0; i < nrules; ++i)
    {
      قاعدو *r = &قاعدا[i];
      شي_عدد *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      طالما (شي_عدد_is_نشان_عدد (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      إذا (شي_عدد_is_قاعدو_عدد (*n) || ISVAR (*n))
        استمر;

      نشان_عدد lhs = r->lhs->عدد;
      bitset_سيٽ (FIRSTS (lhs), *n);
    }
  bool change = true;
  طالما (change)
    {
      change = false;
      لكل (قاعدو_عدد i = 0; i < nrules; ++i)
        {
          قاعدو *r = &قاعدا[i];
          نشان_عدد lhs = r->lhs->عدد;
          bitset f_lhs = FIRSTS (lhs);
          لكل (شي_عدد *n = r->rhs;
               شي_عدد_is_نشان_عدد (*n) && ISVAR (*n);
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
disable_state_شي (state_شي *si)
{
  si->trans = -2;
  bitset_free (si->revs);
  إذا (si->prods)
    bitset_free (si->prods);
}

/* Disable all state_item paths that lead to/from SI and nowhere
   else.  */
ساكن فراغ
prune_state_شي (ثابت state_شي *si)
{
  state_شي_list قطار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &si);

  طالما (gl_list_سائيز (قطار) > 0)
    {
      state_شي *dsi = (state_شي *) gl_list_get_at (قطار, 0);
      gl_list_remove_at (قطار, 0);
      إذا (SI_DISABLED (dsi - state_شيون))
        استمر;

      إذا (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ثابت state_شي *trans = &state_شيون[dsi->trans];
        bitset_reset (trans->revs, dsi - state_شيون);
        إذا (bitset_خالي_جايون_p (trans->revs))
          gl_list_جوڙيو_آخري (قطار, trans);
      }

      إذا (dsi->prods)
        {
          bitset_iterator biter;
          state_شي_عدد جيب;
          BITSET_FOR_EACH (biter, dsi->prods, جيب, 0)
            {
              إذا (SI_DISABLED (جيب))
                استمر;
              ثابت state_شي *prod = &state_شيون[جيب];
              bitset_reset (prod->revs, dsi - state_شيون);
              إذا (bitset_خالي_جايون_p (prod->revs))
                gl_list_جوڙيو_آخري (قطار, prod);
            }
        }

      bitset_iterator biter;
      state_شي_عدد جيب;
      BITSET_FOR_EACH (biter, dsi->revs, جيب, 0)
        {
          إذا (SI_DISABLED (جيب))
            استمر;
          state_شي *rev = &state_شيون[جيب];
          إذا (&state_شيون[rev->trans] == dsi)
            gl_list_جوڙيو_آخري (قطار, rev);
          وإلا إذا (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_شيون);
              إذا (bitset_خالي_جايون_p (rev->prods))
                gl_list_جوڙيو_آخري (قطار, rev);
            }
          وإلا
            gl_list_جوڙيو_آخري (قطار, rev);
        }
      disable_state_شي (dsi);
    }
  gl_list_free (قطار);
}

/* To make searches more efficient, prune away paths that are caused
   by disabled transitions.  */
ساكن فراغ
prune_disabled_paths (فراغ)
{
  لكل (صحيح i = nstate_شيون - 1; i >= 0; --i)
    {
      state_شي *si = &state_شيون[i];
      إذا (si->trans == -1 && شي_عدد_is_نشان_عدد (*si->شي))
        prune_state_شي (si);
    }
}

فراغ
state_شي_print (ثابت state_شي *si, FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  fputs (اڳياڙي, ٻاهريون);
  شي_print (si->شي, عدم, ٻاهريون);
  putc ('\n', ٻاهريون);
}

ثابت قاعدو*
state_شي_قاعدو (ثابت state_شي *si)
{
  أرجع شي_قاعدو (si->شي);
}

/**
 * Report the state_item graph
 */
ساكن فراغ
state_شيون_report (FILE *ٻاهريون)
{
  fprintf (ٻاهريون, "# state items: %zu\n", nstate_شيون);
  لكل (state_عدد i = 0; i < nstates; ++i)
    {
      fprintf (ٻاهريون, "State %d:\n", i);
      لكل (state_شي_عدد j = state_شي_map[i]; j < state_شي_map[i + 1]; ++j)
        {
          ثابت state_شي *si = &state_شيون[j];
          شي_print (si->شي, عدم, ٻاهريون);
          إذا (SI_DISABLED (j))
            fputs ("  DISABLED\n", ٻاهريون);
          وإلا
            {
              putc ('\n', ٻاهريون);
              إذا (si->trans >= 0)
                {
                  fputs ("    -> ", ٻاهريون);
                  state_شي_print (&state_شيون[si->trans], ٻاهريون, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ثابت حرف *txt[2] = { "    => ", "    <- " };
              لكل (صحيح seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  إذا (b)
                    {
                      bitset_iterator biter;
                      state_شي_عدد جيب;
                      BITSET_FOR_EACH (biter, b, جيب, 0)
                        {
                          fputs (txt[seti], ٻاهريون);
                          state_شي_print (&state_شيون[جيب], ٻاهريون, "");
                        }
                    }
                }
            }
          putc ('\n', ٻاهريون);
        }
    }
  fprintf (ٻاهريون, "FIRSTS\n");
  لكل (نشان_عدد i = ntokens; i < nsyms; ++i)
    {
      fprintf (ٻاهريون, "  %s firsts\n", symbols[i]->ٽيگ);
      bitset_iterator iter;
      نشان_عدد j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (ٻاهريون, "    %s\n", symbols[j]->ٽيگ);
    }
  fputs ("\n\n", ٻاهريون);
}

فراغ
state_شيون_init (فراغ)
{
  xtime_t شروعات = gethrxtime ();
  init_state_شيون ();
  init_trans ();
  init_prods ();
  gen_lookaheads ();
  init_firsts ();
  prune_disabled_paths ();
  إذا (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - شروعات) / 1e9);
      state_شيون_report (stderr);
    }
}

فراغ
state_شيون_free (فراغ)
{
  لكل (state_شي_عدد i = 0; i < nstate_شيون; ++i)
    إذا (!SI_DISABLED (i))
      {
        state_شي *si = &state_شيون[i];
        إذا (si->prods)
          bitset_free (si->prods);
        bitset_free (si->revs);
      }
  حرر (state_شيون);
  bitsetv_free (firsts);
}

/**
 * Determine, using precedence and associativity, whether the next
 * production is allowed from the current production.
 */
bool
production_allowed (ثابت state_شي *si, ثابت state_شي *ايندڙ)
{
  sym_content *s1 = شي_قاعدو (si->شي)->lhs;
  sym_content *s2 = شي_قاعدو (ايندڙ->شي)->lhs;
  صحيح prec1 = s1->prec;
  صحيح prec2 = s2->prec;
  إذا (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      إذا (prec1 > prec2)
        أرجع false;
      // Do not expand if same precedence, but left-associative.
      إذا (prec1 == prec2 && s1->assoc == کاٻو_assoc)
        أرجع false;
    }
  أرجع true;
}
