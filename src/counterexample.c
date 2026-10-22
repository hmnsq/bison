/* Conflict counterexample generation

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

#ضمن "counterexample.h"

#ضمن "system.h"

#ضمن <c-strtod.h>
#ضمن <errno.h>
#ضمن <gethrxtime.h>
#ضمن <gl_linked_list.h>
#ضمن <gl_rbtreehash_list.h>
#ضمن <hash.h>
#ضمن <mbswidth.h>
#ضمن <quote.h>
#ضمن <stdlib.h>
#ضمن <textstyle.h>
#ضمن <time.h>

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "derivation.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "lssi.h"
#ضمن "muscle-tab.h"
#ضمن "nullable.h"
#ضمن "parse-simulation.h"


#تعریف_کرو ویلا_حد_بندی_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#تعریف_کرو EXTENDED_تلاش false

/* costs for making various steps in a search */
#تعریف_کرو PRODUCTION_لاگت 50
#تعریف_کرو REDUCE_لاگت 1
#تعریف_کرو کھسکاؤ_لاگت 1
#تعریف_کرو UNSHIFT_لاگت 1
#تعریف_کرو EXTENDED_لاگت 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#تعریف_کرو ASSURANCE_حد_بندی 2.0

/* The time limit before giving up looking for unifying counterexample. */
ساكن مزدوج ویلا_حد_بندی = 5.0;

#تعریف_کرو CUMULATIVE_ویلا_حد_بندی 120.0

// This is the fastest way to get the tail node from the gl_list API.
ساكن gl_list_گرہ_t
list_get_سرا (gl_list_t list)
{
  gl_list_گرہ_t sentinel = gl_list_جوڑو_آخری (list, عدم);
  gl_list_گرہ_t res = gl_list_پچھلا_گرہ (list, sentinel);
  gl_list_remove_گرہ (list, sentinel);
  أرجع res;
}

تعريفنوع بنية
{
  derivation *d1;
  derivation *d2;
  bool کھسکاؤ_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ساكن counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool کھسکاؤ_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (حجم *res);
  res->کھسکاؤ_reduce = کھسکاؤ_reduce;
  إذا (کھسکاؤ_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  وإلا
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  أرجع res;
}

ساكن فراغ
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  حرر (cex);
}

ساكن فراغ
counterexample_print (ثابت counterexample *cex, FILE *باہرلا, ثابت حرف *اگیتر)
{
  ثابت bool flat = getenv ("YYFLAT");
  ثابت حرف *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ثابت حرف *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ثابت حرف *deriv1_label
    = cex->کھسکاؤ_reduce ? _("Shift derivation") : _("First reduce derivation");
  ثابت حرف *deriv2_label
    = cex->کھسکاؤ_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ثابت صحيح چوڑائی =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  إذا (flat)
    fprintf (باہرلا, "  %s%s%*s ", اگیتر,
             example1_label, چوڑائی - mbswidth (example1_label, 0), "");
  وإلا
    fprintf (باہرلا, "  %s%s: ", اگیتر, example1_label);
  derivation_print_leaves (cex->d1, باہرلا);
  إذا (flat)
    fprintf (باہرلا, "  %s%s%*s ", اگیتر,
             deriv1_label, چوڑائی - mbswidth (deriv1_label, 0), "");
  وإلا
    fprintf (باہرلا, "  %s%s", اگیتر, deriv1_label);
  derivation_print (cex->d1, باہرلا, اگیتر);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  إذا (!cex->unifying || is_styled (stderr))
    {
      إذا (flat)
        fprintf (باہرلا, "  %s%s%*s ", اگیتر,
                 example2_label, چوڑائی - mbswidth (example2_label, 0), "");
      وإلا
        fprintf (باہرلا, "  %s%s: ", اگیتر, example2_label);
      derivation_print_leaves (cex->d2, باہرلا);
    }
  إذا (flat)
    fprintf (باہرلا, "  %s%s%*s ", اگیتر,
             deriv2_label, چوڑائی - mbswidth (deriv2_label, 0), "");
  وإلا
    fprintf (باہرلا, "  %s%s", اگیتر, deriv2_label);
  derivation_print (cex->d2, باہرلا, اگیتر);

  إذا (باہرلا != stderr)
    putc ('\n', باہرلا);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
بنية si_bfs_گرہ;
تعريفنوع بنية si_bfs_گرہ
{
  state_شے_نمبر si;
  بنية si_bfs_گرہ *ماپا;
  صحيح reference_گنتی;
} si_bfs_گرہ;

ساكن si_bfs_گرہ *
si_bfs_new (state_شے_نمبر si, si_bfs_گرہ *ماپا)
{
  si_bfs_گرہ *res = xcalloc (1, حجم *res);
  res->si = si;
  res->ماپا = ماپا;
  res->reference_گنتی = 1;
  إذا (ماپا)
    ++ماپا->reference_گنتی;
  أرجع res;
}

ساكن bool
si_bfs_شامل_اے (ثابت si_bfs_گرہ *n, state_شے_نمبر جیب)
{
  لكل (ثابت si_bfs_گرہ *تلاش = n; تلاش != عدم; تلاش = تلاش->ماپا)
    إذا (تلاش->si == جیب)
      أرجع true;
  أرجع false;
}

ساكن فراغ
si_bfs_free (si_bfs_گرہ *n)
{
  إذا (n == عدم)
    أرجع;
  --n->reference_گنتی;
  إذا (n->reference_گنتی == 0)
    {
      si_bfs_free (n->ماپا);
      حرر (n);
    }
}

تعريفنوع gl_list_t si_bfs_گرہ_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ساكن مضمن derivation_list
expand_منزل_conflict (state_شے_نمبر شروع, علامت_نمبر conflict_sym)
{
  si_bfs_گرہ *init = si_bfs_new (شروع, عدم);

  si_bfs_گرہ_list قطار
    = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ثابت فراغ **) &init);
  si_bfs_گرہ *گرہ = عدم;
  // breadth-first search for a path of productions to the conflict symbol
  طالما (gl_list_ناپ (قطار) > 0)
    {
      گرہ = (si_bfs_گرہ *) gl_list_get_at (قطار, 0);
      state_شے *silast = &state_اشیاء[گرہ->si];
      علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (*silast->شے);
      إذا (sym == conflict_sym)
        اكسر;
      إذا (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_شے_نمبر جیب;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, جیب, 0)
            {
              // ignore productions already in the path
              إذا (si_bfs_شامل_اے (گرہ, جیب))
                استمر;
              si_bfs_گرہ *اگلا = si_bfs_new (جیب, گرہ);
              gl_list_جوڑو_آخری (قطار, اگلا);
            }
          // for nullable nonterminals, add its goto to the search
          إذا (nullable[sym - ntokens])
            {
              si_bfs_گرہ *اگلا = si_bfs_new (silast->trans, گرہ);
              gl_list_جوڑو_آخری (قطار, اگلا);
            }
        }
      gl_list_remove_at (قطار, 0);
    }
  إذا (gl_list_ناپ (قطار) == 0)
    {
      gl_list_free (قطار);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_پتی (conflict_sym);
  derivation_list نتیجہ = derivation_list_new ();
  derivation_list_append (نتیجہ, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  لكل (si_bfs_گرہ *n = گرہ; n != عدم; n = n->ماپا)
    {
      state_شے *si = &state_اشیاء[n->si];
      شے_نمبر *pos = si->شے;
      إذا (SI_PRODUCTION (si))
        {
          شے_نمبر *i = عدم;
          لكل (i = pos + 1; !شے_نمبر_is_قاعدہ_نمبر (*i); ++i)
            derivation_list_append (نتیجہ, derivation_new_پتی (*i));
          علامت_نمبر lhs =
            قاعدے[شے_نمبر_as_قاعدہ_نمبر (*i)].lhs->نمبر;
          derivation *deriv = derivation_new (lhs, نتیجہ,
                                              state_شے_قاعدہ (si));
          نتیجہ = derivation_list_new ();
          derivation_list_append (نتیجہ, deriv);
        }
      وإلا
        {
          علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (*(pos - 1));
          derivation *deriv = derivation_new_پتی (sym);
          derivation_list_prepend (نتیجہ, deriv);
        }
    }
  gl_list_free (قطار);
  derivation_free ((derivation*)gl_list_get_at (نتیجہ, 0));
  gl_list_remove_at (نتیجہ, 0);
  أرجع نتیجہ;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
ساكن derivation *
complete_diverging_example (علامت_نمبر conflict_sym,
                            state_شے_list رستہ, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list نتیجہ = derivation_list_new ();
  bool lookahead_required = false;
  إذا (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_جوڑو_آخری (نتیجہ, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_گرہ_t deriv = list_get_سرا (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  لكل (gl_list_گرہ_t state_گرہ = list_get_سرا (رستہ);
       state_گرہ != عدم;
       state_گرہ = gl_list_پچھلا_گرہ (رستہ, state_گرہ))
    {
      state_شے *si = (state_شے *) gl_list_گرہ_قدر (رستہ, state_گرہ);
      شے_نمبر *شے = si->شے;
      شے_نمبر pos = *شے;
      // symbols after dot
      إذا (gl_list_ناپ (نتیجہ) == 1 && !شے_نمبر_is_قاعدہ_نمبر (pos)
          && gl_list_get_at (نتیجہ, 0) == derivation_dot ())
        {
          derivation_list_append (نتیجہ,
            derivation_new_پتی (شے_نمبر_as_علامت_نمبر (pos)));
          lookahead_required = false;
        }
      شے_نمبر *i = شے;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      لكل (state_شے_نمبر nsi = si - state_اشیاء;
           !شے_نمبر_is_قاعدہ_نمبر (*i);
           ++i, nsi = state_اشیاء[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          إذا (i == شے)
            استمر;
          علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (*i);
          إذا (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (نتیجہ, derivation_new_پتی (sym));
              lookahead_required = false;
              استمر;
            }
          // Since PATH is a path to the conflict state-item,
          // for a reduce conflict item, we will want to have a derivation
          // that shows the conflict symbol from its lookahead set being used.
          //
          // Since reductions have the dot at the end of the item,
          // this loop will be first executed on the last item in the path
          // that's not a reduction. When that happens,
          // the symbol after the dot should be a nonterminal,
          // and we can look through successive nullable nonterminals
          // for one with the conflict symbol in its first set.
          إذا (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list اگلا_derivs =
                expand_منزل_conflict (nsi, conflict_sym);
              derivation *d = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (اگلا_derivs);
                   derivation_list_اگلا (&it, &d);)
                derivation_list_append (نتیجہ, d);
              i += gl_list_ناپ (اگلا_derivs) - 1;
              derivation_list_free (اگلا_derivs);
            }
          وإلا إذا (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_پتی (sym);
              derivation_list_append (نتیجہ, d);
            }
          وإلا
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (نتیجہ, derivation_new_پتی (1));
              lookahead_required = false;
            }
        }
      ثابت قاعدہ *r = &قاعدے[شے_نمبر_as_قاعدہ_نمبر (*i)];
      // add derivations for symbols before dot
      لكل (i = شے - 1; !شے_نمبر_is_قاعدہ_نمبر (*i) && i >= ritem; i--)
        {
          gl_list_گرہ_t p = gl_list_پچھلا_گرہ (رستہ, state_گرہ);
          إذا (p)
            state_گرہ = p;
          إذا (deriv)
            {
              ثابت فراغ *tmp_deriv = gl_list_گرہ_قدر (derivs, deriv);
              deriv = gl_list_پچھلا_گرہ (derivs, deriv);
              derivation_list_prepend (نتیجہ, (derivation*)tmp_deriv);
            }
          وإلا
            derivation_list_prepend (نتیجہ, derivation_new_پتی (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->نمبر, نتیجہ, r);
      نتیجہ = derivation_list_new ();
      derivation_list_append (نتیجہ, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (نتیجہ, 0);
  derivation_retain (res);
  derivation_list_free (نتیجہ);
  derivation_list_free (derivs);
  أرجع res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ساكن state_شے_list
nonunifying_کھسکاؤ_رستہ (state_شے_list reduce_رستہ, state_شے *کھسکاؤ_conflict)
{
  gl_list_گرہ_t tmp = gl_list_جوڑو_آخری (reduce_رستہ, عدم);
  gl_list_گرہ_t اگلا_گرہ = gl_list_پچھلا_گرہ (reduce_رستہ, tmp);
  gl_list_گرہ_t گرہ = gl_list_پچھلا_گرہ (reduce_رستہ, اگلا_گرہ);
  gl_list_remove_گرہ (reduce_رستہ, tmp);
  state_شے *si = کھسکاؤ_conflict;
  state_شے_list نتیجہ =
    gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, true);
  // FIXME: bool paths_merged;
  لكل (; گرہ != عدم; اگلا_گرہ = گرہ,
       گرہ = gl_list_پچھلا_گرہ (reduce_رستہ, گرہ))
    {
      state_شے *refsi =
        (state_شے *) gl_list_گرہ_قدر (reduce_رستہ, گرہ);
      state_شے *nextrefsi =
        (state_شے *) gl_list_گرہ_قدر (reduce_رستہ, اگلا_گرہ);
      إذا (nextrefsi == si)
        {
          gl_list_جوڑو_پہلا (نتیجہ, refsi);
          si = refsi;
          استمر;
        }
      // skip reduction items
      إذا (nextrefsi->شے != refsi->شے + 1 && refsi->شے != ritem)
        استمر;

      // bfs to find a shift to the right state
      si_bfs_گرہ *init = si_bfs_new (si - state_اشیاء, عدم);
      si_bfs_گرہ_list قطار
        = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ثابت فراغ **) &init);
      si_bfs_گرہ *sis = عدم;
      state_شے *prevsi = عدم;
      طالما (gl_list_ناپ (قطار) > 0)
        {
          sis = (si_bfs_گرہ *) gl_list_get_at (قطار, 0);
          // if we end up in the start state, the shift couldn't be found.
          إذا (sis->si == 0)
            اكسر;

          state_شے *تلاش_si = &state_اشیاء[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = تلاش_si->revs;
          bitset_iterator biter;
          state_شے_نمبر جیب;
          BITSET_FOR_EACH (biter, rsi, جیب, 0)
            {
              prevsi = &state_اشیاء[جیب];
              إذا (SI_TRANSITION (تلاش_si))
                {
                  إذا (prevsi->state == refsi->state)
                    اذهب تلاش_سرا;
                }
              وإلا إذا (!si_bfs_شامل_اے (sis, جیب))
                {
                  si_bfs_گرہ *prevsis = si_bfs_new (جیب, sis);
                  gl_list_جوڑو_آخری (قطار, prevsis);
                }
            }
          gl_list_remove_at (قطار, 0);
        }
    تلاش_سرا:
      // prepend path to shift we found
      إذا (sis)
        {
          gl_list_گرہ_t ln = gl_list_جوڑو_پہلا (نتیجہ, &state_اشیاء[sis->si]);
          لكل (si_bfs_گرہ *n = sis->ماپا; n; n = n->ماپا)
            ln = gl_list_جوڑو_after (نتیجہ, ln, &state_اشیاء[n->si]);

        }
      si = prevsi;
      gl_list_free (قطار);
    }
  إذا (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_شے *sip = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (نتیجہ);
           state_شے_list_اگلا (&it, &sip);
           )
        state_شے_print (sip, stderr, "");
    }
  أرجع نتیجہ;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ساكن counterexample *
example_from_رستہ (bool کھسکاؤ_reduce,
                   state_شے_نمبر itm2,
                   state_شے_list سبھ_توں_تھوڑا_رستہ, علامت_نمبر اگلا_sym)
{
  derivation *deriv1 =
    complete_diverging_example (اگلا_sym, سبھ_توں_تھوڑا_رستہ, عدم);
  state_شے_list رستہ_2
    = کھسکاؤ_reduce
    ? nonunifying_کھسکاؤ_رستہ (سبھ_توں_تھوڑا_رستہ, &state_اشیاء [itm2])
    : سبھ_توں_تھوڑا_رستہ_from_شروع (itm2, اگلا_sym);
  derivation *deriv2 = complete_diverging_example (اگلا_sym, رستہ_2, عدم);
  gl_list_free (رستہ_2);
  أرجع new_counterexample (deriv1, deriv2, کھسکاؤ_reduce, false, true);
}

/*
 *
 * UNIFYING COUNTER EXAMPLES
 *
 */

/* A search state keeps track of two parser simulations,
 * one starting at each conflict. Complexity is a metric
 * which sums different parser actions with varying weights.
 */
تعريفنوع بنية
{
  parse_state *حالتاں[2];
  صحيح complexity;
} تلاش_state;

ساكن تلاش_state *
initial_تلاش_state (state_شے *conflict1, state_شے *conflict2)
{
  تلاش_state *res = xmalloc (حجم *res);
  res->حالتاں[0] = new_parse_state (conflict1);
  res->حالتاں[1] = new_parse_state (conflict2);
  parse_state_retain (res->حالتاں[0]);
  parse_state_retain (res->حالتاں[1]);
  res->complexity = 0;
  أرجع res;
}

ساكن تلاش_state *
new_تلاش_state (parse_state *ps1, parse_state *ps2, صحيح complexity)
{
  تلاش_state *res = xmalloc (حجم *res);
  res->حالتاں[0] = ps1;
  res->حالتاں[1] = ps2;
  parse_state_retain (res->حالتاں[0]);
  parse_state_retain (res->حالتاں[1]);
  res->complexity = complexity;
  أرجع res;
}

ساكن تلاش_state *
copy_تلاش_state (تلاش_state *ماپا)
{
  تلاش_state *res = xmalloc (حجم *res);
  *res = *ماپا;
  parse_state_retain (res->حالتاں[0]);
  parse_state_retain (res->حالتاں[1]);
  أرجع res;
}

ساكن فراغ
تلاش_state_free_children (تلاش_state *ss)
{
  free_parse_state (ss->حالتاں[0]);
  free_parse_state (ss->حالتاں[1]);
}

ساكن فراغ
تلاش_state_free (تلاش_state *ss)
{
  إذا (ss == عدم)
    أرجع;
  تلاش_state_free_children (ss);
  حرر (ss);
}

/* For debugging traces.  */
ساكن فراغ
تلاش_state_print (تلاش_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->حالتاں[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->حالتاں[1]);
  putc ('\n', stderr);
}

تعريفنوع gl_list_t تلاش_state_list;

ساكن مضمن bool
تلاش_state_list_اگلا (gl_list_iterator_t *it, تلاش_state **ss)
{
  ثابت فراغ *p = عدم;
  bool res = gl_list_iterator_اگلا (it, &p, عدم);
  إذا (res)
    *ss = (تلاش_state*) p;
  وإلا
    gl_list_iterator_free (it);
  أرجع res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ساكن مضمن فراغ
ss_سیٹ_parse_state (تلاش_state *ss, صحيح idx, parse_state *ps)
{
  free_parse_state (ss->حالتاں[idx]);
  ss->حالتاں[idx] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
ساكن counterexample *
complete_diverging_examples (تلاش_state *ss,
                             علامت_نمبر اگلا_sym,
                             bool کھسکاؤ_reduce)
{
  derivation *new_derivs[2];
  لكل (صحيح i = 0; i < 2; ++i)
    {
      state_شے_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->حالتاں[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (اگلا_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  أرجع new_counterexample (new_derivs[0], new_derivs[1],
                             کھسکاؤ_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
تعريفنوع بنية
{
  تلاش_state_list حالتاں;
  صحيح complexity;
} تلاش_state_bundle;

ساكن فراغ
ssb_free (تلاش_state_bundle *ssb)
{
  gl_list_free (ssb->حالتاں);
  حرر (ssb);
}

ساكن size_t
ssb_hasher (تلاش_state_bundle *ssb)
{
  أرجع ssb->complexity;
}

ساكن صحيح
ssb_comp (ثابت تلاش_state_bundle *s1, ثابت تلاش_state_bundle *s2)
{
  أرجع s1->complexity - s2->complexity;
}

ساكن bool
ssb_equals (ثابت تلاش_state_bundle *s1, ثابت تلاش_state_bundle *s2)
{
  أرجع s1->complexity == s2->complexity;
}

تعريفنوع gl_list_t ssb_list;

ساكن size_t
ویکھیا_hasher (ثابت تلاش_state *ss, size_t max)
{
  أرجع (parse_state_hasher (ss->حالتاں[0], max)
          + parse_state_hasher (ss->حالتاں[1], max)) % max;
}

ساكن bool
ویکھیا_comparator (ثابت تلاش_state *ss1, ثابت تلاش_state *ss2)
{
  أرجع parse_state_comparator (ss1->حالتاں[0], ss2->حالتاں[0])
    && parse_state_comparator (ss1->حالتاں[1], ss2->حالتاں[1]);
}

/* Priority queue for search states with minimal complexity. */
ساكن ssb_list ssb_قطار;
ساكن Hash_جدول *ویکھیا;
/* The set of parser states on the shortest lookahead-sensitive path. */
ساكن bitset scp_سیٹ = عدم;
/* The set of parser states used for the conflict reduction rule. */
ساكن bitset rpp_سیٹ = عدم;

ساكن فراغ
ssb_append (تلاش_state *ss)
{
  إذا (hash_lookup (ویکھیا, ss))
    {
      تلاش_state_free (ss);
      أرجع;
    }
  hash_xinsert (ویکھیا, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->حالتاں[0]);
  parse_state_free_contents_early (ss->حالتاں[1]);
  parse_state_retain (ss->حالتاں[0]);
  parse_state_retain (ss->حالتاں[1]);
  تلاش_state_bundle *ssb = xmalloc (حجم *ssb);
  ssb->complexity = ss->complexity;
  gl_list_گرہ_t n = gl_list_تلاش (ssb_قطار, ssb);
  إذا (!n)
    {
      ssb->حالتاں =
        gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم,
                              (gl_listelement_dispose_fn)تلاش_state_free_children,
                              true);
      gl_sortedlist_جوڑو (ssb_قطار, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  وإلا
    {
      حرر (ssb);
      ssb = (تلاش_state_bundle *) gl_list_گرہ_قدر (ssb_قطار, n);
    }
  gl_list_جوڑو_آخری (ssb->حالتاں, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ساكن فراغ
production_قدم (تلاش_state *ss, صحيح parser_state)
{
  ثابت state_شے *other_si = parse_state_پوچھ (ss->حالتاں[1 - parser_state]);
  علامت_نمبر other_sym = شے_نمبر_as_علامت_نمبر (*other_si->شے);
  parse_state_list prods =
    simulate_production (ss->حالتاں[parser_state], other_sym);
  صحيح complexity = ss->complexity + PRODUCTION_لاگت;

  parse_state *ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_اگلا (&it, &ps);
       )
    {
      تلاش_state *copy = copy_تلاش_state (ss);
      ss_سیٹ_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ساكن مضمن صحيح
reduction_لاگت (ثابت parse_state *ps)
{
  صحيح shifts;
  صحيح productions;
  parse_state_completed_قدماں (ps, &shifts, &productions);
  أرجع کھسکاؤ_لاگت * shifts + PRODUCTION_لاگت * productions;
}

ساكن تلاش_state_list
reduction_قدم (تلاش_state *ss, ثابت شے_نمبر *conflict_شے,
                صحيح parser_state, صحيح قاعدہ_len)
{
  (فراغ) conflict_شے; // FIXME: Unused
  تلاش_state_list نتیجہ =
    gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, 1);

  parse_state *ps = ss->حالتاں[parser_state];
  ثابت state_شے *si = parse_state_پوچھ (ps);
  bitset علامت_سیٹ = si->lookahead;
  parse_state *other = ss->حالتاں[1 - parser_state];
  ثابت state_شے *other_si = parse_state_پوچھ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  إذا (شے_نمبر_is_علامت_نمبر (*other_si->شے))
    {
      علامت_نمبر other_sym =
        شے_نمبر_as_علامت_نمبر (*other_si->شے);
      إذا (!intersect_علامت (other_sym, علامت_سیٹ))
        أرجع نتیجہ;
      علامت_سیٹ = bitset_create (nsyms, BITSET_FIXED);
      bitset_سیٹ (علامت_سیٹ, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, قاعدہ_len, علامت_سیٹ);
  parse_state *reduced_ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_اگلا (&it, &reduced_ps);
       )
    {
      تلاش_state *copy = copy_تلاش_state (ss);
      ss_سیٹ_parse_state (copy, parser_state, reduced_ps);
      صحيح r_لاگت = reduction_لاگت (reduced_ps);
      copy->complexity += r_لاگت + PRODUCTION_لاگت + 2 * کھسکاؤ_لاگت;
      gl_list_جوڑو_آخری (نتیجہ, copy);
    }
  gl_list_free (reduced);
  إذا (علامت_سیٹ != si->lookahead)
    bitset_free (علامت_سیٹ);
  أرجع نتیجہ;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ساكن فراغ
تلاش_state_prepend (تلاش_state *ss, علامت_نمبر sym, bitset guide)
{
  (فراغ) sym; // FIXME: Unused.
  ثابت state_شے *si1src = parse_state_سر (ss->حالتاں[0]);
  ثابت state_شے *si2src = parse_state_سر (ss->حالتاں[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  إذا (prod1 != SI_PRODUCTION (si2src))
    {
      صحيح prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->حالتاں[prod_state]);
      parse_state *ps = عدم;
      لكل (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_اگلا (&iter, &ps);
           )
        {
          ثابت state_شے *psi = parse_state_سر (ps);
          bool guided = bitset_test (guide, psi->state->نمبر);
          إذا (!guided && !EXTENDED_تلاش)
            استمر;

          تلاش_state *copy = copy_تلاش_state (ss);
          ss_سیٹ_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_لاگت;
          إذا (!guided)
            copy->complexity += EXTENDED_لاگت;
          ssb_append (copy);
        }
      gl_list_free (prev);
      أرجع;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ثابت صحيح complexity_لاگت = 2 * (prod1 ? PRODUCTION_لاگت : UNSHIFT_لاگت);

  parse_state_list prev1 = parser_prepend (ss->حالتاں[0]);
  parse_state_list prev2 = parser_prepend (ss->حالتاں[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = عدم;
  لكل (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_اگلا (&iter1, &ps1);
       )
    {
      ثابت state_شے *psi1 = parse_state_سر (ps1);
      bool guided1 = bitset_test (guide, psi1->state->نمبر);
      إذا (!guided1 && !EXTENDED_تلاش)
        استمر;

      parse_state *ps2 = عدم;
      لكل (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_اگلا (&iter2, &ps2);
           )
        {
          ثابت state_شے *psi2 = parse_state_سر (ps2);

          bool guided2 = bitset_test (guide, psi2->state->نمبر);
          إذا (!guided2 && !EXTENDED_تلاش)
            استمر;
          // Only consider prepend state items that share the same state.
          إذا (psi1->state != psi2->state)
            استمر;

          صحيح complexity = ss->complexity + complexity_لاگت;
          // penalty for not being along the guide path
          إذا (!guided1 || !guided2)
            complexity += EXTENDED_لاگت;
          ssb_append (new_تلاش_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
ساكن bool
have_common_اگیتر (ثابت شے_نمبر *itm1, ثابت شے_نمبر *itm2)
{
  صحيح i = 0;
  لكل (; !شے_نمبر_is_قاعدہ_نمبر (itm1[i]); ++i)
    إذا (itm1[i] != itm2[i])
      أرجع false;
  أرجع شے_نمبر_is_قاعدہ_نمبر (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ساكن ثابت شے_نمبر *
شے_قاعدہ_شروع (ثابت شے_نمبر *شے)
{
  ثابت شے_نمبر *res = عدم;
  لكل (res = شے;
       ritem < res && شے_نمبر_is_علامت_نمبر (*(res - 1));
       --res)
    استمر;
  أرجع res;
}

ساكن ثابت شے_نمبر *
شے_قاعدہ_سرا (ثابت شے_نمبر *شے)
{
  ثابت شے_نمبر *res = عدم;
  لكل (res = شے; شے_نمبر_is_علامت_نمبر (*res); ++res)
    استمر;
  أرجع res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ساكن مضمن فراغ
generate_اگلا_حالتاں (تلاش_state *ss, state_شے *conflict1,
                      state_شے *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->حالتاں[0];
  parse_state *ps2 = ss->حالتاں[1];
  ثابت state_شے *si1 = parse_state_پوچھ (ps1);
  ثابت state_شے *si2 = parse_state_پوچھ (ps2);
  bool si1reduce = شے_نمبر_is_قاعدہ_نمبر (*si1->شے);
  bool si2reduce = شے_نمبر_is_قاعدہ_نمبر (*si2->شے);
  إذا (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      إذا (*si1->شے == *si2->شے)
        {
          صحيح complexity = ss->complexity + 2 * کھسکاؤ_لاگت;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = عدم;
          parse_state *tps2 = عدم;
          لكل (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_اگلا (&it1, &tps1);
               )
            لكل (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_اگلا (&it2, &tps2);
                 )
              ssb_append (new_تلاش_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_قدم (ss, 0);
      production_قدم (ss, 1);
    }
  // One of the states requires a reduction
  وإلا
    {
      ثابت شے_نمبر *rhs1 = شے_قاعدہ_شروع (si1->شے);
      ثابت شے_نمبر *rhe1 = شے_قاعدہ_سرا (si1->شے);
      صحيح len1 = rhe1 - rhs1;
      صحيح size1 = parse_state_لمائی (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ثابت شے_نمبر *rhs2 = شے_قاعدہ_شروع (si2->شے);
      ثابت شے_نمبر *rhe2 = شے_قاعدہ_سرا (si2->شے);
      صحيح len2 = rhe2 - rhs2;
      صحيح size2 = parse_state_لمائی (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      إذا (ready1 && ready2)
        {
          تلاش_state_list reduced1 = reduction_قدم (ss, conflict1->شے, 0, len1);
          gl_list_جوڑو_آخری (reduced1, ss);
          تلاش_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               تلاش_state_list_اگلا (&iter, &red1);
               )
            {
              تلاش_state_list reduced2 =
                reduction_قدم (red1, conflict2->شے, 1, len2);
              تلاش_state *red2 = عدم;
              لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   تلاش_state_list_اگلا (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              إذا (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      وإلا إذا (ready1)
        {
          تلاش_state_list reduced1 = reduction_قدم (ss, conflict1->شے, 0, len1);
          تلاش_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               تلاش_state_list_اگلا (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      وإلا إذا (ready2)
        {
          تلاش_state_list reduced2 = reduction_قدم (ss, conflict2->شے, 1, len2);
          تلاش_state *red2 = عدم;
          لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               تلاش_state_list_اگلا (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      وإلا
        {
          ثابت علامت_نمبر sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          تلاش_state_prepend (ss, sym,
                                parse_state_ڈونگھائی (ss->حالتاں[0]) >= 0
                                ? rpp_سیٹ : scp_سیٹ);
        }
    }
}

/*
 * Perform the actual counterexample search,
 * keeps track of what stage of the search algorithm
 * we are at and gives the appropriate counterexample
 * type based off of time constraints.
 */
ساكن counterexample *
unifying_example (state_شے_نمبر itm1,
                  state_شے_نمبر itm2,
                  bool کھسکاؤ_reduce,
                  state_شے_list reduce_رستہ, علامت_نمبر اگلا_sym)
{
  state_شے *conflict1 = &state_اشیاء[itm1];
  state_شے *conflict2 = &state_اشیاء[itm2];
  تلاش_state *initial = initial_تلاش_state (conflict1, conflict2);
  ssb_قطار = gl_list_create_خالی_تھاواں (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  ویکھیا =
    hash_initialize (32, عدم, (Hash_hasher) ویکھیا_hasher,
                     (Hash_comparator) ویکھیا_comparator,
                     (Hash_انکڑے_freer) تلاش_state_free);
  ssb_append (initial);
  xtime_t شروع = gethrxtime ();
  bool assurance_printed = false;
  تلاش_state *stage3result = عدم;
  counterexample *cex = عدم;
  طالما (gl_list_ناپ (ssb_قطار) > 0)
    {
      ثابت تلاش_state_bundle *ssb = gl_list_get_at (ssb_قطار, 0);

      تلاش_state *ss = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (ssb->حالتاں);
           تلاش_state_list_اگلا (&it, &ss);
           )
        {
          إذا (trace_flag & trace_cex)
            تلاش_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->حالتاں[0];
          parse_state *ps2 = ss->حالتاں[1];
          إذا (parse_state_ڈونگھائی (ps1) < 0 && parse_state_ڈونگھائی (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ثابت state_شے *si1src = parse_state_سر (ps1);
              ثابت state_شے *si2src = parse_state_سر (ps2);
              إذا (شے_قاعدہ (si1src->شے)->lhs == شے_قاعدہ (si2src->شے)->lhs
                  && have_common_اگیتر (si1src->شے, si2src->شے))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  إذا (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, کھسکاؤ_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      اذهب cex_تلاش_سرا;
                    }
                  إذا (!stage3result)
                    stage3result = copy_تلاش_state (ss);
                }
            }
          إذا (ویلا_حد_بندی_ENFORCED)
            {
              مزدوج ویلا_passed = (gethrxtime () - شروع) / 1e9;
              إذا (!assurance_printed && ویلا_passed > ASSURANCE_حد_بندی
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              إذا (ویلا_passed > ویلا_حد_بندی)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", ویلا_passed);
                  اذهب cex_تلاش_سرا;
                }
            }
          generate_اگلا_حالتاں (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_قطار,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_تلاش_سرا:;
  إذا (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      إذا (stage3result)
        cex = complete_diverging_examples (stage3result, اگلا_sym, کھسکاؤ_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      وإلا
        cex = example_from_رستہ (کھسکاؤ_reduce, itm2, reduce_رستہ, اگلا_sym);
    }
  gl_list_free (ssb_قطار);
  hash_free (ویکھیا);
  إذا (stage3result)
    تلاش_state_free (stage3result);
  أرجع cex;
}

ساكن xtime_t cumulative_ویلا;

فراغ
counterexample_init (فراغ)
{
  // Check cex.timeout.
  {
    ثابت حرف *variable = "cex.timeout";
    حرف *cp = muscle_percent_define_get (variable);
    إذا (*cp != '\0')
      {
        حرف *سرا = عدم;
        مزدوج v = c_strtod (cp, &سرا);
        إذا (*سرا == '\0' && errno == 0 && v >= 0)
          ویلا_حد_بندی = v;
        وإلا
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    حرر (cp);
  }
  cumulative_ویلا = gethrxtime ();
  scp_سیٹ = bitset_create (nstates, BITSET_FIXED);
  rpp_سیٹ = bitset_create (nstates, BITSET_FIXED);
  state_اشیاء_init ();
}


فراغ
counterexample_free (فراغ)
{
  إذا (scp_سیٹ)
    {
      bitset_free (scp_سیٹ);
      bitset_free (rpp_سیٹ);
      state_اشیاء_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ساكن فراغ
counterexample_report (state_شے_نمبر itm1, state_شے_نمبر itm2,
                       علامت_نمبر اگلا_sym, bool کھسکاؤ_reduce,
                       FILE *باہرلا, ثابت حرف *اگیتر)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_شے_list سبھ_توں_تھوڑا_رستہ = سبھ_توں_تھوڑا_رستہ_from_شروع (itm1, اگلا_sym);
  bool reduce_prod_reached = false;
  ثابت قاعدہ *reduce_قاعدہ = شے_قاعدہ (state_اشیاء[itm1].شے);

  bitset_zero (scp_سیٹ);
  bitset_zero (rpp_سیٹ);

  state_شے *si = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (سبھ_توں_تھوڑا_رستہ);
       state_شے_list_اگلا (&it, &si);
       )
    {
      bitset_سیٹ (scp_سیٹ, si->state->نمبر);
      reduce_prod_reached = reduce_prod_reached
                          || شے_قاعدہ (si->شے) == reduce_قاعدہ;
      إذا (reduce_prod_reached)
        bitset_سیٹ (rpp_سیٹ, si->state->نمبر);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_ویلا) / 1e9 < CUMULATIVE_ویلا_حد_بندی
    ? unifying_example (itm1, itm2, کھسکاؤ_reduce, سبھ_توں_تھوڑا_رستہ, اگلا_sym)
    : example_from_رستہ (کھسکاؤ_reduce, itm2, سبھ_توں_تھوڑا_رستہ, اگلا_sym);

  gl_list_free (سبھ_توں_تھوڑا_رستہ);
  counterexample_print (cex, باہرلا, اگیتر);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
ساكن فراغ
counterexample_report_کھسکاؤ_reduce (state_شے_نمبر itm1, state_شے_نمبر itm2,
                                    علامت_نمبر اگلا_sym,
                                    FILE *باہرلا, ثابت حرف *اگیتر)
{
  إذا (باہرلا == stderr)
    complain (عدم, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[اگلا_sym]->ٹیگ);
  وإلا
    {
      fputs (اگیتر, باہرلا);
      fprintf (باہرلا, _("shift/reduce conflict on token %s"), symbols[اگلا_sym]->ٹیگ);
      fprintf (باہرلا, "%s\n", _(":"));
    }
  // In the report, print the items.
  إذا (باہرلا != stderr || trace_flag & trace_cex)
    {
      state_شے_print (&state_اشیاء[itm1], باہرلا, اگیتر);
      state_شے_print (&state_اشیاء[itm2], باہرلا, اگیتر);
    }
  counterexample_report (itm1, itm2, اگلا_sym, true, باہرلا, اگیتر);
}

ساكن فراغ
counterexample_report_reduce_reduce (state_شے_نمبر itm1, state_شے_نمبر itm2,
                                     bitset conflict_syms,
                                     FILE *باہرلا, ثابت حرف *اگیتر)
{
  {
    بنية obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_شے_نمبر sym;
    ثابت حرف *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->ٹیگ);
        sep = ", ";
      }
    حرف *ٹوکن = obstack_finish0 (&obstack);
    إذا (باہرلا == stderr)
      complain (عدم, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_گنتی (conflict_syms)),
                ٹوکن);
    وإلا
      {
        fputs (اگیتر, باہرلا);
        fprintf (باہرلا,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_گنتی (conflict_syms)),
                 ٹوکن);
        fprintf (باہرلا, "%s\n", _(":"));
      }
    obstack_free (&obstack, عدم);
  }
  // In the report, print the items.
  إذا (باہرلا != stderr || trace_flag & trace_cex)
    {
      state_شے_print (&state_اشیاء[itm1], باہرلا, اگیتر);
      state_شے_print (&state_اشیاء[itm2], باہرلا, اگیتر);
    }
  counterexample_report (itm1, itm2, bitset_پہلا (conflict_syms),
                         false, باہرلا, اگیتر);
}

ساكن state_شے_نمبر
لبھو_state_شے_نمبر (ثابت قاعدہ *r, state_نمبر sn)
{
  لكل (state_شے_نمبر i = state_شے_map[sn]; i < state_شے_map[sn + 1]; ++i)
    إذا (!SI_DISABLED (i)
        && شے_نمبر_as_قاعدہ_نمبر (*state_اشیاء[i].شے) == r->نمبر)
      أرجع i;
  abort ();
}

فراغ
counterexample_report_state (ثابت state *s, FILE *باہرلا, ثابت حرف *اگیتر)
{
  ثابت state_نمبر sn = s->نمبر;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  لكل (صحيح i = 0; i < reds->عدد; ++i)
    {
      ثابت قاعدہ *r1 = reds->قاعدے[i];
      ثابت state_شے_نمبر c1 = لبھو_state_شے_نمبر (r1, sn);
      لكل (state_شے_نمبر c2 = state_شے_map[sn]; c2 < state_شے_map[sn + 1]; ++c2)
        إذا (!SI_DISABLED (c2))
          {
            شے_نمبر conf = *state_اشیاء[c2].شے;
            إذا (شے_نمبر_is_علامت_نمبر (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_کھسکاؤ_reduce (c1, c2, conf, باہرلا, اگیتر);
          }
      لكل (صحيح j = i+1; j < reds->عدد; ++j)
        {
          ثابت قاعدہ *r2 = reds->قاعدے[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          إذا (!bitset_خالی_تھاواں_p (lookaheads))
            لكل (state_شے_نمبر c2 = state_شے_map[sn]; c2 < state_شے_map[sn + 1]; ++c2)
              إذا (!SI_DISABLED (c2)
                  && شے_قاعدہ (state_اشیاء[c2].شے) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, باہرلا, اگیتر);
                  اكسر;
                }
        }
    }
  bitset_free (lookaheads);
}
