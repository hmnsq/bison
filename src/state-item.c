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
  प्रत्यागम sl->कुञ्चिका % max;
}

स्थिर bool
hash_pair_comparator (ध्रुव hash_pair *l, ध्रुव hash_pair *r)
{
  प्रत्यागम l->कुञ्चिका == r->कुञ्चिका;
}

स्थिर शून्य
hash_pair_free (hash_pair *hp)
{
  bitset_free (hp->l);
  मुक्त (hp);
}

स्थिर Hash_सारणी *
hash_pair_सारणी_create (पूर्णाङ्क आकार)
{
  प्रत्यागम hash_xinitialize (आकार,
                           रिक्त,
                           (Hash_hasher) hash_pair_hasher,
                           (Hash_comparator) hash_pair_comparator,
                           (Hash_दत्त_freer) hash_pair_free);
}

स्थिर bitset
hash_pair_lookup (Hash_सारणी *tab, पूर्णाङ्क कुञ्चिका)
{
  hash_pair probe;
  probe.कुञ्चिका = कुञ्चिका;
  hash_pair *hp = hash_lookup (tab, &probe);
  प्रत्यागम hp ? hp->l : रिक्त;
}

स्थिर शून्य
hash_pair_निवेशय (Hash_सारणी *tab, पूर्णाङ्क कुञ्चिका, bitset val)
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
  प्रत्यागम &state_वस्तूनि[state_item_सूचकांक_lookup (s, off)];
}

स्थिर अन्तःस्थ शून्य
state_item_समुच्चयः (state_item_सङ्ख्या sidx, ध्रुव state *s, item_सङ्ख्या off)
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
  प्रति (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      ध्रुव state *s = अवस्थाः[i];
      nstate_वस्तूनि += s->nitems;
      संवृतिः (s->वस्तूनि, s->nitems);
      प्रति (size_t j = 0; j < nitemset; ++j)
        यदि (0 < itemset[j]
            && item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[itemset[j] - 1]))
          {
            bitset_समुच्चयः (production_वस्तूनि[i], itemset[j]);
            ++nstate_वस्तूनि;
          }
    }
  state_item_map = xnmalloc (nstates + 1, परिमाण (state_item_सङ्ख्या));
  state_वस्तूनि = xnmalloc (nstate_वस्तूनि, परिमाण (state_item));
  state_item_सङ्ख्या sidx = 0;
  प्रति (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      state_item_map[i] = sidx;
      पूर्णाङ्क नियमः_अन्वेषणम्_idx = 0;
      ध्रुव state *s = अवस्थाः[i];
      ध्रुव reductions *red = s->reductions;
      प्रति (पूर्णाङ्क j = 0; j < s->nitems; ++j)
        {
          state_item_समुच्चयः (sidx, s, s->वस्तूनि[j]);
          state_item *si = &state_वस्तूनि[sidx];
          ध्रुव नियमः *r = item_नियमः (si->item);
          यदि (नियमः_अन्वेषणम्_idx < red->अङ्कः && red->नियमाः[नियमः_अन्वेषणम्_idx] < r)
            ++नियमः_अन्वेषणम्_idx;
          यदि (नियमः_अन्वेषणम्_idx < red->अङ्कः && r == red->नियमाः[नियमः_अन्वेषणम्_idx])
            {
              bitsetv lookahead = red->lookaheads;
              यदि (lookahead)
                si->lookahead = lookahead[नियमः_अन्वेषणम्_idx];
            }
          ++sidx;
        }
      bitset_iterator biter;
      item_सङ्ख्या off;
      BITSET_FOR_EACH (biter, production_वस्तूनि[i], off, 0)
        {
          state_item_समुच्चयः (sidx, s, off);
          यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[off]))
            {
              bitsetv lookahead = red->lookaheads;
              यदि (lookahead)
                state_वस्तूनि[sidx].lookahead = lookahead[नियमः_अन्वेषणम्_idx];
              ++नियमः_अन्वेषणम्_idx;
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
  प्रत्यागम ((state *) st)->accessing_चिह्नम् % max;
}

स्थिर bool
state_sym_comparator (ध्रुव शून्य *s1, ध्रुव शून्य *s2)
{
  प्रत्यागम ((state *) s1)->accessing_चिह्नम् == ((state *) s2)->accessing_चिह्नम्;
}

स्थिर state *
state_sym_lookup (चिह्नम्_सङ्ख्या sym, Hash_सारणी *h)
{
  state probe;
  probe.accessing_चिह्नम् = sym;
  प्रत्यागम hash_lookup (h, &probe);
}

स्थिर शून्य
init_trans (शून्य)
{
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      // Generate a hash set that maps from accepting symbols to the states
      // this state transitions to.
      state *s = अवस्थाः[i];
      transitions *t = s->transitions;
      Hash_सारणी *transition_समुच्चयः
        = hash_xinitialize (t->अङ्कः, रिक्त, (Hash_hasher) state_sym_hasher,
                            (Hash_comparator) state_sym_comparator, रिक्त);
      प्रति (पूर्णाङ्क j = 0; j < t->अङ्कः; ++j)
        यदि (!TRANSITION_IS_DISABLED (t, j))
          hash_xinsert (transition_समुच्चयः, t->अवस्थाः[j]);
      प्रति (state_item_सङ्ख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          item_सङ्ख्या *item = state_वस्तूनि[j].item;
          यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (*item))
            अनुवर्त;
          state *dst = state_sym_lookup (*item, transition_समुच्चयः);
          यदि (!dst)
            अनुवर्त;
          // find the item in the destination state that corresponds
          // to the transition of item
          प्रति (पूर्णाङ्क k = 0; k < dst->nitems; ++k)
            यदि (item + 1 == ritem + dst->वस्तूनि[k])
              {
                state_item_सङ्ख्या dstSI =
                  state_item_सूचकांक_lookup (dst->सङ्ख्या, k);

                state_वस्तूनि[j].trans = dstSI;
                bitset_समुच्चयः (state_वस्तूनि[dstSI].revs, j);
                भङ्ग;
            }
        }
      hash_free (transition_समुच्चयः);
    }
}

स्थिर शून्य
init_prods (शून्य)
{
  प्रति (पूर्णाङ्क i = 0; i < nstates; ++i)
    {
      state *s = अवस्थाः[i];
      // closure_map is a hash map from nonterminals to a set
      // of the items that produce those nonterminals
      Hash_सारणी *संवृतिः_map = hash_pair_सारणी_create (nsyms - ntokens);

      // Add the nitems of state to skip to the production portion
      // of that state's state_items
      प्रति (state_item_सङ्ख्या j = state_item_map[i] + s->nitems;
           j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तूनि[j];
          item_सङ्ख्या *item = src->item;
          चिह्नम्_सङ्ख्या lhs = item_नियमः (item)->lhs->सङ्ख्या;
          bitset itms = hash_pair_lookup (संवृतिः_map, lhs);
          यदि (!itms)
            {
              itms = bitset_create (nstate_वस्तूनि, BITSET_SPARSE);
              hash_pair_निवेशय (संवृतिः_map, lhs, itms);
            }
          bitset_समुच्चयः (itms, j);
        }
      // For each item with a dot followed by a nonterminal,
      // try to create a production edge.
      प्रति (state_item_सङ्ख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          state_item *src = &state_वस्तूनि[j];
          item_सङ्ख्या item = *(src->item);
          // Skip reduce items and items with terminals after the dot
          यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (item) || ISTOKEN (item))
            अनुवर्त;
          चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (item);
          bitset lb = hash_pair_lookup (संवृतिः_map, sym);
          यदि (lb)
            {
              bitset copy = bitset_create (nstate_वस्तूनि, BITSET_SPARSE);
              bitset_copy (copy, lb);
              // update prods.
              state_वस्तूनि[j].prods = copy;

              // update revs.
              bitset_iterator biter;
              state_item_सङ्ख्या prod;
              BITSET_FOR_EACH (biter, copy, prod, 0)
                bitset_समुच्चयः (state_वस्तूनि[prod].revs, j);
            }
        }
      hash_free (संवृतिः_map);
    }
}

/* Since lookaheads are only generated for reductions, we need to
   propagate lookahead sets backwards as the searches require each
   state_item to have a lookahead. */
स्थिर अन्तःस्थ शून्य
gen_lookaheads (शून्य)
{
  प्रति (state_item_सङ्ख्या i = 0; i < nstate_वस्तूनि; ++i)
    {
      state_item *si = &state_वस्तूनि[i];
      यदि (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*(si->item)) || !si->lookahead)
        अनुवर्त;

      bitset lookahead = si->lookahead;
      state_item_list queue =
        gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                        (ध्रुव शून्य **) &si);

      // For each reduction item, traverse through all state_items
      // accessible through reverse transition steps, and set their
      // lookaheads to the reduction items lookahead
      यावत् (gl_list_आकार (queue) > 0)
        {
          state_item *पूर्व = (state_item *) gl_list_get_at (queue, 0);
          gl_list_remove_at (queue, 0);
          पूर्व->lookahead = lookahead;
          यदि (SI_TRANSITION (पूर्व))
            {
              bitset rsi = state_वस्तूनि[पूर्व - state_वस्तूनि].revs;
              bitset_iterator biter;
              state_item_सङ्ख्या ज्या;
              BITSET_FOR_EACH (biter, rsi, ज्या, 0)
                gl_list_सङ्कलय_first (queue, &state_वस्तूनि[ज्या]);
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
  प्रति (नियमः_सङ्ख्या i = 0; i < nrules; ++i)
    {
      नियमः *r = &नियमाः[i];
      item_सङ्ख्या *n = r->rhs;
      // Iterate through nullable nonterminals to try to find a terminal.
      यावत् (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*n) && ISVAR (*n)
             && nullable[*n - ntokens])
        ++n;
      यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (*n) || ISVAR (*n))
        अनुवर्त;

      चिह्नम्_सङ्ख्या lhs = r->lhs->सङ्ख्या;
      bitset_समुच्चयः (FIRSTS (lhs), *n);
    }
  bool change = true;
  यावत् (change)
    {
      change = false;
      प्रति (नियमः_सङ्ख्या i = 0; i < nrules; ++i)
        {
          नियमः *r = &नियमाः[i];
          चिह्नम्_सङ्ख्या lhs = r->lhs->सङ्ख्या;
          bitset f_lhs = FIRSTS (lhs);
          प्रति (item_सङ्ख्या *n = r->rhs;
               item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*n) && ISVAR (*n);
               ++n)
            {
              bitset f = FIRSTS (*n);
              यदि (!bitset_subset_p (f_lhs, f))
                {
                  change = true;
                  bitset_union (f_lhs, f_lhs, f);
                }
              यदि (!nullable[*n - ntokens])
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
  यदि (si->prods)
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

  यावत् (gl_list_आकार (queue) > 0)
    {
      state_item *dsi = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      यदि (SI_DISABLED (dsi - state_वस्तूनि))
        अनुवर्त;

      यदि (dsi->trans >= 0 && !SI_DISABLED (dsi->trans))
      {
        ध्रुव state_item *trans = &state_वस्तूनि[dsi->trans];
        bitset_reset (trans->revs, dsi - state_वस्तूनि);
        यदि (bitset_रिक्तानि_p (trans->revs))
          gl_list_सङ्कलय_चरमः (queue, trans);
      }

      यदि (dsi->prods)
        {
          bitset_iterator biter;
          state_item_सङ्ख्या ज्या;
          BITSET_FOR_EACH (biter, dsi->prods, ज्या, 0)
            {
              यदि (SI_DISABLED (ज्या))
                अनुवर्त;
              ध्रुव state_item *prod = &state_वस्तूनि[ज्या];
              bitset_reset (prod->revs, dsi - state_वस्तूनि);
              यदि (bitset_रिक्तानि_p (prod->revs))
                gl_list_सङ्कलय_चरमः (queue, prod);
            }
        }

      bitset_iterator biter;
      state_item_सङ्ख्या ज्या;
      BITSET_FOR_EACH (biter, dsi->revs, ज्या, 0)
        {
          यदि (SI_DISABLED (ज्या))
            अनुवर्त;
          state_item *rev = &state_वस्तूनि[ज्या];
          यदि (&state_वस्तूनि[rev->trans] == dsi)
            gl_list_सङ्कलय_चरमः (queue, rev);
          अन्यथा यदि (rev->prods)
            {
              bitset_reset (rev->prods, dsi - state_वस्तूनि);
              यदि (bitset_रिक्तानि_p (rev->prods))
                gl_list_सङ्कलय_चरमः (queue, rev);
            }
          अन्यथा
            gl_list_सङ्कलय_चरमः (queue, rev);
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
  प्रति (पूर्णाङ्क i = nstate_वस्तूनि - 1; i >= 0; --i)
    {
      state_item *si = &state_वस्तूनि[i];
      यदि (si->trans == -1 && item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*si->item))
        prune_state_item (si);
    }
}

शून्य
state_item_print (ध्रुव state_item *si, FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  fputs (उपसर्गः, बहिर्गमनम्);
  item_print (si->item, रिक्त, बहिर्गमनम्);
  putc ('\n', बहिर्गमनम्);
}

ध्रुव नियमः*
state_item_नियमः (ध्रुव state_item *si)
{
  प्रत्यागम item_नियमः (si->item);
}

/**
 * Report the state_item graph
 */
स्थिर शून्य
state_वस्तूनि_report (FILE *बहिर्गमनम्)
{
  fprintf (बहिर्गमनम्, "# state items: %zu\n", nstate_वस्तूनि);
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      fprintf (बहिर्गमनम्, "State %d:\n", i);
      प्रति (state_item_सङ्ख्या j = state_item_map[i]; j < state_item_map[i + 1]; ++j)
        {
          ध्रुव state_item *si = &state_वस्तूनि[j];
          item_print (si->item, रिक्त, बहिर्गमनम्);
          यदि (SI_DISABLED (j))
            fputs ("  DISABLED\n", बहिर्गमनम्);
          अन्यथा
            {
              putc ('\n', बहिर्गमनम्);
              यदि (si->trans >= 0)
                {
                  fputs ("    -> ", बहिर्गमनम्);
                  state_item_print (&state_वस्तूनि[si->trans], बहिर्गमनम्, "");
                }

              bitset sets[2] = { si->prods, si->revs };
              ध्रुव वर्ण *txt[2] = { "    => ", "    <- " };
              प्रति (पूर्णाङ्क seti = 0; seti < 2; ++seti)
                {
                  bitset b = sets[seti];
                  यदि (b)
                    {
                      bitset_iterator biter;
                      state_item_सङ्ख्या ज्या;
                      BITSET_FOR_EACH (biter, b, ज्या, 0)
                        {
                          fputs (txt[seti], बहिर्गमनम्);
                          state_item_print (&state_वस्तूनि[ज्या], बहिर्गमनम्, "");
                        }
                    }
                }
            }
          putc ('\n', बहिर्गमनम्);
        }
    }
  fprintf (बहिर्गमनम्, "FIRSTS\n");
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      fprintf (बहिर्गमनम्, "  %s firsts\n", symbols[i]->चिह्नकम्);
      bitset_iterator iter;
      चिह्नम्_सङ्ख्या j;
      BITSET_FOR_EACH (iter, FIRSTS (i), j, 0)
        fprintf (बहिर्गमनम्, "    %s\n", symbols[j]->चिह्नकम्);
    }
  fputs ("\n\n", बहिर्गमनम्);
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
  यदि (trace_flag & trace_cex)
    {
      fprintf (stderr, "state_items_init: %.3fs\n", (gethrxtime () - start) / 1e9);
      state_वस्तूनि_report (stderr);
    }
}

शून्य
state_वस्तूनि_free (शून्य)
{
  प्रति (state_item_सङ्ख्या i = 0; i < nstate_वस्तूनि; ++i)
    यदि (!SI_DISABLED (i))
      {
        state_item *si = &state_वस्तूनि[i];
        यदि (si->prods)
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
production_allowed (ध्रुव state_item *si, ध्रुव state_item *अग्र)
{
  sym_content *s1 = item_नियमः (si->item)->lhs;
  sym_content *s2 = item_नियमः (अग्र->item)->lhs;
  पूर्णाङ्क prec1 = s1->prec;
  पूर्णाङ्क prec2 = s2->prec;
  यदि (prec1 >= 0 && prec2 >= 0)
    {
      // Do not expand if lower precedence.
      यदि (prec1 > prec2)
        प्रत्यागम false;
      // Do not expand if same precedence, but left-associative.
      यदि (prec1 == prec2 && s1->assoc == वामः_assoc)
        प्रत्यागम false;
    }
  प्रत्यागम true;
}
