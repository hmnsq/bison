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


#وصف_ڪريو وقت_حد_بندي_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#وصف_ڪريو EXTENDED_ڳولا false

/* costs for making various steps in a search */
#وصف_ڪريو PRODUCTION_لاڳت 50
#وصف_ڪريو REDUCE_لاڳت 1
#وصف_ڪريو سرڪايو_لاڳت 1
#وصف_ڪريو UNSHIFT_لاڳت 1
#وصف_ڪريو EXTENDED_لاڳت 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#وصف_ڪريو ASSURANCE_حد_بندي 2.0

/* The time limit before giving up looking for unifying counterexample. */
ساكن مزدوج وقت_حد_بندي = 5.0;

#وصف_ڪريو CUMULATIVE_وقت_حد_بندي 120.0

// This is the fastest way to get the tail node from the gl_list API.
ساكن gl_list_ڳنڍ_t
list_get_پڇاڙي (gl_list_t list)
{
  gl_list_ڳنڍ_t sentinel = gl_list_جوڙيو_آخري (list, عدم);
  gl_list_ڳنڍ_t res = gl_list_اڳيون_ڳنڍ (list, sentinel);
  gl_list_remove_ڳنڍ (list, sentinel);
  أرجع res;
}

تعريفنوع بنية
{
  derivation *d1;
  derivation *d2;
  bool سرڪايو_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ساكن counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool سرڪايو_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (حجم *res);
  res->سرڪايو_reduce = سرڪايو_reduce;
  إذا (سرڪايو_reduce)
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
counterexample_print (ثابت counterexample *cex, FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  ثابت bool flat = getenv ("YYFLAT");
  ثابت حرف *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ثابت حرف *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ثابت حرف *deriv1_label
    = cex->سرڪايو_reduce ? _("Shift derivation") : _("First reduce derivation");
  ثابت حرف *deriv2_label
    = cex->سرڪايو_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ثابت صحيح ويڪر =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  إذا (flat)
    fprintf (ٻاهريون, "  %s%s%*s ", اڳياڙي,
             example1_label, ويڪر - mbswidth (example1_label, 0), "");
  وإلا
    fprintf (ٻاهريون, "  %s%s: ", اڳياڙي, example1_label);
  derivation_print_leaves (cex->d1, ٻاهريون);
  إذا (flat)
    fprintf (ٻاهريون, "  %s%s%*s ", اڳياڙي,
             deriv1_label, ويڪر - mbswidth (deriv1_label, 0), "");
  وإلا
    fprintf (ٻاهريون, "  %s%s", اڳياڙي, deriv1_label);
  derivation_print (cex->d1, ٻاهريون, اڳياڙي);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  إذا (!cex->unifying || is_styled (stderr))
    {
      إذا (flat)
        fprintf (ٻاهريون, "  %s%s%*s ", اڳياڙي,
                 example2_label, ويڪر - mbswidth (example2_label, 0), "");
      وإلا
        fprintf (ٻاهريون, "  %s%s: ", اڳياڙي, example2_label);
      derivation_print_leaves (cex->d2, ٻاهريون);
    }
  إذا (flat)
    fprintf (ٻاهريون, "  %s%s%*s ", اڳياڙي,
             deriv2_label, ويڪر - mbswidth (deriv2_label, 0), "");
  وإلا
    fprintf (ٻاهريون, "  %s%s", اڳياڙي, deriv2_label);
  derivation_print (cex->d2, ٻاهريون, اڳياڙي);

  إذا (ٻاهريون != stderr)
    putc ('\n', ٻاهريون);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
بنية si_bfs_ڳنڍ;
تعريفنوع بنية si_bfs_ڳنڍ
{
  state_شي_عدد si;
  بنية si_bfs_ڳنڍ *والد;
  صحيح reference_ڳڻپ;
} si_bfs_ڳنڍ;

ساكن si_bfs_ڳنڍ *
si_bfs_new (state_شي_عدد si, si_bfs_ڳنڍ *والد)
{
  si_bfs_ڳنڍ *res = xcalloc (1, حجم *res);
  res->si = si;
  res->والد = والد;
  res->reference_ڳڻپ = 1;
  إذا (والد)
    ++والد->reference_ڳڻپ;
  أرجع res;
}

ساكن bool
si_bfs_شامل_آهي (ثابت si_bfs_ڳنڍ *n, state_شي_عدد جيب)
{
  لكل (ثابت si_bfs_ڳنڍ *ڳولا = n; ڳولا != عدم; ڳولا = ڳولا->والد)
    إذا (ڳولا->si == جيب)
      أرجع true;
  أرجع false;
}

ساكن فراغ
si_bfs_free (si_bfs_ڳنڍ *n)
{
  إذا (n == عدم)
    أرجع;
  --n->reference_ڳڻپ;
  إذا (n->reference_ڳڻپ == 0)
    {
      si_bfs_free (n->والد);
      حرر (n);
    }
}

تعريفنوع gl_list_t si_bfs_ڳنڍ_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ساكن مضمن derivation_list
expand_منزل_conflict (state_شي_عدد شروعات, نشان_عدد conflict_sym)
{
  si_bfs_ڳنڍ *init = si_bfs_new (شروعات, عدم);

  si_bfs_ڳنڍ_list قطار
    = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ثابت فراغ **) &init);
  si_bfs_ڳنڍ *ڳنڍ = عدم;
  // breadth-first search for a path of productions to the conflict symbol
  طالما (gl_list_سائيز (قطار) > 0)
    {
      ڳنڍ = (si_bfs_ڳنڍ *) gl_list_get_at (قطار, 0);
      state_شي *silast = &state_شيون[ڳنڍ->si];
      نشان_عدد sym = شي_عدد_as_نشان_عدد (*silast->شي);
      إذا (sym == conflict_sym)
        اكسر;
      إذا (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_شي_عدد جيب;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, جيب, 0)
            {
              // ignore productions already in the path
              إذا (si_bfs_شامل_آهي (ڳنڍ, جيب))
                استمر;
              si_bfs_ڳنڍ *ايندڙ = si_bfs_new (جيب, ڳنڍ);
              gl_list_جوڙيو_آخري (قطار, ايندڙ);
            }
          // for nullable nonterminals, add its goto to the search
          إذا (nullable[sym - ntokens])
            {
              si_bfs_ڳنڍ *ايندڙ = si_bfs_new (silast->trans, ڳنڍ);
              gl_list_جوڙيو_آخري (قطار, ايندڙ);
            }
        }
      gl_list_remove_at (قطار, 0);
    }
  إذا (gl_list_سائيز (قطار) == 0)
    {
      gl_list_free (قطار);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_پن (conflict_sym);
  derivation_list نتيجو = derivation_list_new ();
  derivation_list_append (نتيجو, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  لكل (si_bfs_ڳنڍ *n = ڳنڍ; n != عدم; n = n->والد)
    {
      state_شي *si = &state_شيون[n->si];
      شي_عدد *pos = si->شي;
      إذا (SI_PRODUCTION (si))
        {
          شي_عدد *i = عدم;
          لكل (i = pos + 1; !شي_عدد_is_قاعدو_عدد (*i); ++i)
            derivation_list_append (نتيجو, derivation_new_پن (*i));
          نشان_عدد lhs =
            قاعدا[شي_عدد_as_قاعدو_عدد (*i)].lhs->عدد;
          derivation *deriv = derivation_new (lhs, نتيجو,
                                              state_شي_قاعدو (si));
          نتيجو = derivation_list_new ();
          derivation_list_append (نتيجو, deriv);
        }
      وإلا
        {
          نشان_عدد sym = شي_عدد_as_نشان_عدد (*(pos - 1));
          derivation *deriv = derivation_new_پن (sym);
          derivation_list_prepend (نتيجو, deriv);
        }
    }
  gl_list_free (قطار);
  derivation_free ((derivation*)gl_list_get_at (نتيجو, 0));
  gl_list_remove_at (نتيجو, 0);
  أرجع نتيجو;
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
complete_diverging_example (نشان_عدد conflict_sym,
                            state_شي_list رستو, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list نتيجو = derivation_list_new ();
  bool lookahead_required = false;
  إذا (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_جوڙيو_آخري (نتيجو, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ڳنڍ_t deriv = list_get_پڇاڙي (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  لكل (gl_list_ڳنڍ_t state_ڳنڍ = list_get_پڇاڙي (رستو);
       state_ڳنڍ != عدم;
       state_ڳنڍ = gl_list_اڳيون_ڳنڍ (رستو, state_ڳنڍ))
    {
      state_شي *si = (state_شي *) gl_list_ڳنڍ_قدر (رستو, state_ڳنڍ);
      شي_عدد *شي = si->شي;
      شي_عدد pos = *شي;
      // symbols after dot
      إذا (gl_list_سائيز (نتيجو) == 1 && !شي_عدد_is_قاعدو_عدد (pos)
          && gl_list_get_at (نتيجو, 0) == derivation_dot ())
        {
          derivation_list_append (نتيجو,
            derivation_new_پن (شي_عدد_as_نشان_عدد (pos)));
          lookahead_required = false;
        }
      شي_عدد *i = شي;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      لكل (state_شي_عدد nsi = si - state_شيون;
           !شي_عدد_is_قاعدو_عدد (*i);
           ++i, nsi = state_شيون[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          إذا (i == شي)
            استمر;
          نشان_عدد sym = شي_عدد_as_نشان_عدد (*i);
          إذا (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (نتيجو, derivation_new_پن (sym));
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
              derivation_list ايندڙ_derivs =
                expand_منزل_conflict (nsi, conflict_sym);
              derivation *d = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (ايندڙ_derivs);
                   derivation_list_ايندڙ (&it, &d);)
                derivation_list_append (نتيجو, d);
              i += gl_list_سائيز (ايندڙ_derivs) - 1;
              derivation_list_free (ايندڙ_derivs);
            }
          وإلا إذا (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_پن (sym);
              derivation_list_append (نتيجو, d);
            }
          وإلا
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (نتيجو, derivation_new_پن (1));
              lookahead_required = false;
            }
        }
      ثابت قاعدو *r = &قاعدا[شي_عدد_as_قاعدو_عدد (*i)];
      // add derivations for symbols before dot
      لكل (i = شي - 1; !شي_عدد_is_قاعدو_عدد (*i) && i >= ritem; i--)
        {
          gl_list_ڳنڍ_t p = gl_list_اڳيون_ڳنڍ (رستو, state_ڳنڍ);
          إذا (p)
            state_ڳنڍ = p;
          إذا (deriv)
            {
              ثابت فراغ *tmp_deriv = gl_list_ڳنڍ_قدر (derivs, deriv);
              deriv = gl_list_اڳيون_ڳنڍ (derivs, deriv);
              derivation_list_prepend (نتيجو, (derivation*)tmp_deriv);
            }
          وإلا
            derivation_list_prepend (نتيجو, derivation_new_پن (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->عدد, نتيجو, r);
      نتيجو = derivation_list_new ();
      derivation_list_append (نتيجو, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (نتيجو, 0);
  derivation_retain (res);
  derivation_list_free (نتيجو);
  derivation_list_free (derivs);
  أرجع res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ساكن state_شي_list
nonunifying_سرڪايو_رستو (state_شي_list reduce_رستو, state_شي *سرڪايو_conflict)
{
  gl_list_ڳنڍ_t tmp = gl_list_جوڙيو_آخري (reduce_رستو, عدم);
  gl_list_ڳنڍ_t ايندڙ_ڳنڍ = gl_list_اڳيون_ڳنڍ (reduce_رستو, tmp);
  gl_list_ڳنڍ_t ڳنڍ = gl_list_اڳيون_ڳنڍ (reduce_رستو, ايندڙ_ڳنڍ);
  gl_list_remove_ڳنڍ (reduce_رستو, tmp);
  state_شي *si = سرڪايو_conflict;
  state_شي_list نتيجو =
    gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم, عدم, true);
  // FIXME: bool paths_merged;
  لكل (; ڳنڍ != عدم; ايندڙ_ڳنڍ = ڳنڍ,
       ڳنڍ = gl_list_اڳيون_ڳنڍ (reduce_رستو, ڳنڍ))
    {
      state_شي *refsi =
        (state_شي *) gl_list_ڳنڍ_قدر (reduce_رستو, ڳنڍ);
      state_شي *nextrefsi =
        (state_شي *) gl_list_ڳنڍ_قدر (reduce_رستو, ايندڙ_ڳنڍ);
      إذا (nextrefsi == si)
        {
          gl_list_جوڙيو_پهريون (نتيجو, refsi);
          si = refsi;
          استمر;
        }
      // skip reduction items
      إذا (nextrefsi->شي != refsi->شي + 1 && refsi->شي != ritem)
        استمر;

      // bfs to find a shift to the right state
      si_bfs_ڳنڍ *init = si_bfs_new (si - state_شيون, عدم);
      si_bfs_ڳنڍ_list قطار
        = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ثابت فراغ **) &init);
      si_bfs_ڳنڍ *sis = عدم;
      state_شي *prevsi = عدم;
      طالما (gl_list_سائيز (قطار) > 0)
        {
          sis = (si_bfs_ڳنڍ *) gl_list_get_at (قطار, 0);
          // if we end up in the start state, the shift couldn't be found.
          إذا (sis->si == 0)
            اكسر;

          state_شي *ڳولا_si = &state_شيون[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = ڳولا_si->revs;
          bitset_iterator biter;
          state_شي_عدد جيب;
          BITSET_FOR_EACH (biter, rsi, جيب, 0)
            {
              prevsi = &state_شيون[جيب];
              إذا (SI_TRANSITION (ڳولا_si))
                {
                  إذا (prevsi->state == refsi->state)
                    اذهب ڳولا_پڇاڙي;
                }
              وإلا إذا (!si_bfs_شامل_آهي (sis, جيب))
                {
                  si_bfs_ڳنڍ *prevsis = si_bfs_new (جيب, sis);
                  gl_list_جوڙيو_آخري (قطار, prevsis);
                }
            }
          gl_list_remove_at (قطار, 0);
        }
    ڳولا_پڇاڙي:
      // prepend path to shift we found
      إذا (sis)
        {
          gl_list_ڳنڍ_t ln = gl_list_جوڙيو_پهريون (نتيجو, &state_شيون[sis->si]);
          لكل (si_bfs_ڳنڍ *n = sis->والد; n; n = n->والد)
            ln = gl_list_جوڙيو_after (نتيجو, ln, &state_شيون[n->si]);

        }
      si = prevsi;
      gl_list_free (قطار);
    }
  إذا (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_شي *sip = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (نتيجو);
           state_شي_list_ايندڙ (&it, &sip);
           )
        state_شي_print (sip, stderr, "");
    }
  أرجع نتيجو;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ساكن counterexample *
example_from_رستو (bool سرڪايو_reduce,
                   state_شي_عدد itm2,
                   state_شي_list سڀ_کان_ٿورو_رستو, نشان_عدد ايندڙ_sym)
{
  derivation *deriv1 =
    complete_diverging_example (ايندڙ_sym, سڀ_کان_ٿورو_رستو, عدم);
  state_شي_list رستو_2
    = سرڪايو_reduce
    ? nonunifying_سرڪايو_رستو (سڀ_کان_ٿورو_رستو, &state_شيون [itm2])
    : سڀ_کان_ٿورو_رستو_from_شروعات (itm2, ايندڙ_sym);
  derivation *deriv2 = complete_diverging_example (ايندڙ_sym, رستو_2, عدم);
  gl_list_free (رستو_2);
  أرجع new_counterexample (deriv1, deriv2, سرڪايو_reduce, false, true);
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
  parse_state *حالتون[2];
  صحيح complexity;
} ڳولا_state;

ساكن ڳولا_state *
initial_ڳولا_state (state_شي *conflict1, state_شي *conflict2)
{
  ڳولا_state *res = xmalloc (حجم *res);
  res->حالتون[0] = new_parse_state (conflict1);
  res->حالتون[1] = new_parse_state (conflict2);
  parse_state_retain (res->حالتون[0]);
  parse_state_retain (res->حالتون[1]);
  res->complexity = 0;
  أرجع res;
}

ساكن ڳولا_state *
new_ڳولا_state (parse_state *ps1, parse_state *ps2, صحيح complexity)
{
  ڳولا_state *res = xmalloc (حجم *res);
  res->حالتون[0] = ps1;
  res->حالتون[1] = ps2;
  parse_state_retain (res->حالتون[0]);
  parse_state_retain (res->حالتون[1]);
  res->complexity = complexity;
  أرجع res;
}

ساكن ڳولا_state *
copy_ڳولا_state (ڳولا_state *والد)
{
  ڳولا_state *res = xmalloc (حجم *res);
  *res = *والد;
  parse_state_retain (res->حالتون[0]);
  parse_state_retain (res->حالتون[1]);
  أرجع res;
}

ساكن فراغ
ڳولا_state_free_children (ڳولا_state *ss)
{
  free_parse_state (ss->حالتون[0]);
  free_parse_state (ss->حالتون[1]);
}

ساكن فراغ
ڳولا_state_free (ڳولا_state *ss)
{
  إذا (ss == عدم)
    أرجع;
  ڳولا_state_free_children (ss);
  حرر (ss);
}

/* For debugging traces.  */
ساكن فراغ
ڳولا_state_print (ڳولا_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->حالتون[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->حالتون[1]);
  putc ('\n', stderr);
}

تعريفنوع gl_list_t ڳولا_state_list;

ساكن مضمن bool
ڳولا_state_list_ايندڙ (gl_list_iterator_t *it, ڳولا_state **ss)
{
  ثابت فراغ *p = عدم;
  bool res = gl_list_iterator_ايندڙ (it, &p, عدم);
  إذا (res)
    *ss = (ڳولا_state*) p;
  وإلا
    gl_list_iterator_free (it);
  أرجع res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ساكن مضمن فراغ
ss_سيٽ_parse_state (ڳولا_state *ss, صحيح idx, parse_state *ps)
{
  free_parse_state (ss->حالتون[idx]);
  ss->حالتون[idx] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
ساكن counterexample *
complete_diverging_examples (ڳولا_state *ss,
                             نشان_عدد ايندڙ_sym,
                             bool سرڪايو_reduce)
{
  derivation *new_derivs[2];
  لكل (صحيح i = 0; i < 2; ++i)
    {
      state_شي_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->حالتون[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (ايندڙ_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  أرجع new_counterexample (new_derivs[0], new_derivs[1],
                             سرڪايو_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
تعريفنوع بنية
{
  ڳولا_state_list حالتون;
  صحيح complexity;
} ڳولا_state_bundle;

ساكن فراغ
ssb_free (ڳولا_state_bundle *ssb)
{
  gl_list_free (ssb->حالتون);
  حرر (ssb);
}

ساكن size_t
ssb_hasher (ڳولا_state_bundle *ssb)
{
  أرجع ssb->complexity;
}

ساكن صحيح
ssb_comp (ثابت ڳولا_state_bundle *s1, ثابت ڳولا_state_bundle *s2)
{
  أرجع s1->complexity - s2->complexity;
}

ساكن bool
ssb_equals (ثابت ڳولا_state_bundle *s1, ثابت ڳولا_state_bundle *s2)
{
  أرجع s1->complexity == s2->complexity;
}

تعريفنوع gl_list_t ssb_list;

ساكن size_t
گهميل_hasher (ثابت ڳولا_state *ss, size_t max)
{
  أرجع (parse_state_hasher (ss->حالتون[0], max)
          + parse_state_hasher (ss->حالتون[1], max)) % max;
}

ساكن bool
گهميل_comparator (ثابت ڳولا_state *ss1, ثابت ڳولا_state *ss2)
{
  أرجع parse_state_comparator (ss1->حالتون[0], ss2->حالتون[0])
    && parse_state_comparator (ss1->حالتون[1], ss2->حالتون[1]);
}

/* Priority queue for search states with minimal complexity. */
ساكن ssb_list ssb_قطار;
ساكن Hash_جدول *گهميل;
/* The set of parser states on the shortest lookahead-sensitive path. */
ساكن bitset scp_سيٽ = عدم;
/* The set of parser states used for the conflict reduction rule. */
ساكن bitset rpp_سيٽ = عدم;

ساكن فراغ
ssb_append (ڳولا_state *ss)
{
  إذا (hash_lookup (گهميل, ss))
    {
      ڳولا_state_free (ss);
      أرجع;
    }
  hash_xinsert (گهميل, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->حالتون[0]);
  parse_state_free_contents_early (ss->حالتون[1]);
  parse_state_retain (ss->حالتون[0]);
  parse_state_retain (ss->حالتون[1]);
  ڳولا_state_bundle *ssb = xmalloc (حجم *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ڳنڍ_t n = gl_list_ڳولا (ssb_قطار, ssb);
  إذا (!n)
    {
      ssb->حالتون =
        gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم,
                              (gl_listelement_dispose_fn)ڳولا_state_free_children,
                              true);
      gl_sortedlist_جوڙيو (ssb_قطار, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  وإلا
    {
      حرر (ssb);
      ssb = (ڳولا_state_bundle *) gl_list_ڳنڍ_قدر (ssb_قطار, n);
    }
  gl_list_جوڙيو_آخري (ssb->حالتون, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ساكن فراغ
production_قدم (ڳولا_state *ss, صحيح parser_state)
{
  ثابت state_شي *other_si = parse_state_پڇ (ss->حالتون[1 - parser_state]);
  نشان_عدد other_sym = شي_عدد_as_نشان_عدد (*other_si->شي);
  parse_state_list prods =
    simulate_production (ss->حالتون[parser_state], other_sym);
  صحيح complexity = ss->complexity + PRODUCTION_لاڳت;

  parse_state *ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_ايندڙ (&it, &ps);
       )
    {
      ڳولا_state *copy = copy_ڳولا_state (ss);
      ss_سيٽ_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ساكن مضمن صحيح
reduction_لاڳت (ثابت parse_state *ps)
{
  صحيح shifts;
  صحيح productions;
  parse_state_completed_قدمن (ps, &shifts, &productions);
  أرجع سرڪايو_لاڳت * shifts + PRODUCTION_لاڳت * productions;
}

ساكن ڳولا_state_list
reduction_قدم (ڳولا_state *ss, ثابت شي_عدد *conflict_شي,
                صحيح parser_state, صحيح قاعدو_len)
{
  (فراغ) conflict_شي; // FIXME: Unused
  ڳولا_state_list نتيجو =
    gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم, عدم, 1);

  parse_state *ps = ss->حالتون[parser_state];
  ثابت state_شي *si = parse_state_پڇ (ps);
  bitset نشان_سيٽ = si->lookahead;
  parse_state *other = ss->حالتون[1 - parser_state];
  ثابت state_شي *other_si = parse_state_پڇ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  إذا (شي_عدد_is_نشان_عدد (*other_si->شي))
    {
      نشان_عدد other_sym =
        شي_عدد_as_نشان_عدد (*other_si->شي);
      إذا (!intersect_نشان (other_sym, نشان_سيٽ))
        أرجع نتيجو;
      نشان_سيٽ = bitset_create (nsyms, BITSET_FIXED);
      bitset_سيٽ (نشان_سيٽ, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, قاعدو_len, نشان_سيٽ);
  parse_state *reduced_ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_ايندڙ (&it, &reduced_ps);
       )
    {
      ڳولا_state *copy = copy_ڳولا_state (ss);
      ss_سيٽ_parse_state (copy, parser_state, reduced_ps);
      صحيح r_لاڳت = reduction_لاڳت (reduced_ps);
      copy->complexity += r_لاڳت + PRODUCTION_لاڳت + 2 * سرڪايو_لاڳت;
      gl_list_جوڙيو_آخري (نتيجو, copy);
    }
  gl_list_free (reduced);
  إذا (نشان_سيٽ != si->lookahead)
    bitset_free (نشان_سيٽ);
  أرجع نتيجو;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ساكن فراغ
ڳولا_state_prepend (ڳولا_state *ss, نشان_عدد sym, bitset guide)
{
  (فراغ) sym; // FIXME: Unused.
  ثابت state_شي *si1src = parse_state_سر (ss->حالتون[0]);
  ثابت state_شي *si2src = parse_state_سر (ss->حالتون[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  إذا (prod1 != SI_PRODUCTION (si2src))
    {
      صحيح prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->حالتون[prod_state]);
      parse_state *ps = عدم;
      لكل (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_ايندڙ (&iter, &ps);
           )
        {
          ثابت state_شي *psi = parse_state_سر (ps);
          bool guided = bitset_test (guide, psi->state->عدد);
          إذا (!guided && !EXTENDED_ڳولا)
            استمر;

          ڳولا_state *copy = copy_ڳولا_state (ss);
          ss_سيٽ_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_لاڳت;
          إذا (!guided)
            copy->complexity += EXTENDED_لاڳت;
          ssb_append (copy);
        }
      gl_list_free (prev);
      أرجع;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ثابت صحيح complexity_لاڳت = 2 * (prod1 ? PRODUCTION_لاڳت : UNSHIFT_لاڳت);

  parse_state_list prev1 = parser_prepend (ss->حالتون[0]);
  parse_state_list prev2 = parser_prepend (ss->حالتون[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = عدم;
  لكل (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_ايندڙ (&iter1, &ps1);
       )
    {
      ثابت state_شي *psi1 = parse_state_سر (ps1);
      bool guided1 = bitset_test (guide, psi1->state->عدد);
      إذا (!guided1 && !EXTENDED_ڳولا)
        استمر;

      parse_state *ps2 = عدم;
      لكل (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_ايندڙ (&iter2, &ps2);
           )
        {
          ثابت state_شي *psi2 = parse_state_سر (ps2);

          bool guided2 = bitset_test (guide, psi2->state->عدد);
          إذا (!guided2 && !EXTENDED_ڳولا)
            استمر;
          // Only consider prepend state items that share the same state.
          إذا (psi1->state != psi2->state)
            استمر;

          صحيح complexity = ss->complexity + complexity_لاڳت;
          // penalty for not being along the guide path
          إذا (!guided1 || !guided2)
            complexity += EXTENDED_لاڳت;
          ssb_append (new_ڳولا_state (ps1, ps2, complexity));
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
have_common_اڳياڙي (ثابت شي_عدد *itm1, ثابت شي_عدد *itm2)
{
  صحيح i = 0;
  لكل (; !شي_عدد_is_قاعدو_عدد (itm1[i]); ++i)
    إذا (itm1[i] != itm2[i])
      أرجع false;
  أرجع شي_عدد_is_قاعدو_عدد (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ساكن ثابت شي_عدد *
شي_قاعدو_شروعات (ثابت شي_عدد *شي)
{
  ثابت شي_عدد *res = عدم;
  لكل (res = شي;
       ritem < res && شي_عدد_is_نشان_عدد (*(res - 1));
       --res)
    استمر;
  أرجع res;
}

ساكن ثابت شي_عدد *
شي_قاعدو_پڇاڙي (ثابت شي_عدد *شي)
{
  ثابت شي_عدد *res = عدم;
  لكل (res = شي; شي_عدد_is_نشان_عدد (*res); ++res)
    استمر;
  أرجع res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ساكن مضمن فراغ
generate_ايندڙ_حالتون (ڳولا_state *ss, state_شي *conflict1,
                      state_شي *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->حالتون[0];
  parse_state *ps2 = ss->حالتون[1];
  ثابت state_شي *si1 = parse_state_پڇ (ps1);
  ثابت state_شي *si2 = parse_state_پڇ (ps2);
  bool si1reduce = شي_عدد_is_قاعدو_عدد (*si1->شي);
  bool si2reduce = شي_عدد_is_قاعدو_عدد (*si2->شي);
  إذا (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      إذا (*si1->شي == *si2->شي)
        {
          صحيح complexity = ss->complexity + 2 * سرڪايو_لاڳت;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = عدم;
          parse_state *tps2 = عدم;
          لكل (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_ايندڙ (&it1, &tps1);
               )
            لكل (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_ايندڙ (&it2, &tps2);
                 )
              ssb_append (new_ڳولا_state (tps1, tps2, complexity));
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
      ثابت شي_عدد *rhs1 = شي_قاعدو_شروعات (si1->شي);
      ثابت شي_عدد *rhe1 = شي_قاعدو_پڇاڙي (si1->شي);
      صحيح len1 = rhe1 - rhs1;
      صحيح size1 = parse_state_ڊيگهائي (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ثابت شي_عدد *rhs2 = شي_قاعدو_شروعات (si2->شي);
      ثابت شي_عدد *rhe2 = شي_قاعدو_پڇاڙي (si2->شي);
      صحيح len2 = rhe2 - rhs2;
      صحيح size2 = parse_state_ڊيگهائي (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      إذا (ready1 && ready2)
        {
          ڳولا_state_list reduced1 = reduction_قدم (ss, conflict1->شي, 0, len1);
          gl_list_جوڙيو_آخري (reduced1, ss);
          ڳولا_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ڳولا_state_list_ايندڙ (&iter, &red1);
               )
            {
              ڳولا_state_list reduced2 =
                reduction_قدم (red1, conflict2->شي, 1, len2);
              ڳولا_state *red2 = عدم;
              لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   ڳولا_state_list_ايندڙ (&iter2, &red2);
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
          ڳولا_state_list reduced1 = reduction_قدم (ss, conflict1->شي, 0, len1);
          ڳولا_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ڳولا_state_list_ايندڙ (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      وإلا إذا (ready2)
        {
          ڳولا_state_list reduced2 = reduction_قدم (ss, conflict2->شي, 1, len2);
          ڳولا_state *red2 = عدم;
          لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               ڳولا_state_list_ايندڙ (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      وإلا
        {
          ثابت نشان_عدد sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          ڳولا_state_prepend (ss, sym,
                                parse_state_اونهائي (ss->حالتون[0]) >= 0
                                ? rpp_سيٽ : scp_سيٽ);
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
unifying_example (state_شي_عدد itm1,
                  state_شي_عدد itm2,
                  bool سرڪايو_reduce,
                  state_شي_list reduce_رستو, نشان_عدد ايندڙ_sym)
{
  state_شي *conflict1 = &state_شيون[itm1];
  state_شي *conflict2 = &state_شيون[itm2];
  ڳولا_state *initial = initial_ڳولا_state (conflict1, conflict2);
  ssb_قطار = gl_list_create_خالي_جايون (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  گهميل =
    hash_initialize (32, عدم, (Hash_hasher) گهميل_hasher,
                     (Hash_comparator) گهميل_comparator,
                     (Hash_معلومات_freer) ڳولا_state_free);
  ssb_append (initial);
  xtime_t شروعات = gethrxtime ();
  bool assurance_printed = false;
  ڳولا_state *stage3result = عدم;
  counterexample *cex = عدم;
  طالما (gl_list_سائيز (ssb_قطار) > 0)
    {
      ثابت ڳولا_state_bundle *ssb = gl_list_get_at (ssb_قطار, 0);

      ڳولا_state *ss = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (ssb->حالتون);
           ڳولا_state_list_ايندڙ (&it, &ss);
           )
        {
          إذا (trace_flag & trace_cex)
            ڳولا_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->حالتون[0];
          parse_state *ps2 = ss->حالتون[1];
          إذا (parse_state_اونهائي (ps1) < 0 && parse_state_اونهائي (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ثابت state_شي *si1src = parse_state_سر (ps1);
              ثابت state_شي *si2src = parse_state_سر (ps2);
              إذا (شي_قاعدو (si1src->شي)->lhs == شي_قاعدو (si2src->شي)->lhs
                  && have_common_اڳياڙي (si1src->شي, si2src->شي))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  إذا (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, سرڪايو_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      اذهب cex_ڳولا_پڇاڙي;
                    }
                  إذا (!stage3result)
                    stage3result = copy_ڳولا_state (ss);
                }
            }
          إذا (وقت_حد_بندي_ENFORCED)
            {
              مزدوج وقت_passed = (gethrxtime () - شروعات) / 1e9;
              إذا (!assurance_printed && وقت_passed > ASSURANCE_حد_بندي
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              إذا (وقت_passed > وقت_حد_بندي)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", وقت_passed);
                  اذهب cex_ڳولا_پڇاڙي;
                }
            }
          generate_ايندڙ_حالتون (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_قطار,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_ڳولا_پڇاڙي:;
  إذا (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      إذا (stage3result)
        cex = complete_diverging_examples (stage3result, ايندڙ_sym, سرڪايو_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      وإلا
        cex = example_from_رستو (سرڪايو_reduce, itm2, reduce_رستو, ايندڙ_sym);
    }
  gl_list_free (ssb_قطار);
  hash_free (گهميل);
  إذا (stage3result)
    ڳولا_state_free (stage3result);
  أرجع cex;
}

ساكن xtime_t cumulative_وقت;

فراغ
counterexample_init (فراغ)
{
  // Check cex.timeout.
  {
    ثابت حرف *variable = "cex.timeout";
    حرف *cp = muscle_percent_define_get (variable);
    إذا (*cp != '\0')
      {
        حرف *پڇاڙي = عدم;
        مزدوج v = c_strtod (cp, &پڇاڙي);
        إذا (*پڇاڙي == '\0' && errno == 0 && v >= 0)
          وقت_حد_بندي = v;
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
  cumulative_وقت = gethrxtime ();
  scp_سيٽ = bitset_create (nstates, BITSET_FIXED);
  rpp_سيٽ = bitset_create (nstates, BITSET_FIXED);
  state_شيون_init ();
}


فراغ
counterexample_free (فراغ)
{
  إذا (scp_سيٽ)
    {
      bitset_free (scp_سيٽ);
      bitset_free (rpp_سيٽ);
      state_شيون_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ساكن فراغ
counterexample_report (state_شي_عدد itm1, state_شي_عدد itm2,
                       نشان_عدد ايندڙ_sym, bool سرڪايو_reduce,
                       FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_شي_list سڀ_کان_ٿورو_رستو = سڀ_کان_ٿورو_رستو_from_شروعات (itm1, ايندڙ_sym);
  bool reduce_prod_reached = false;
  ثابت قاعدو *reduce_قاعدو = شي_قاعدو (state_شيون[itm1].شي);

  bitset_zero (scp_سيٽ);
  bitset_zero (rpp_سيٽ);

  state_شي *si = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (سڀ_کان_ٿورو_رستو);
       state_شي_list_ايندڙ (&it, &si);
       )
    {
      bitset_سيٽ (scp_سيٽ, si->state->عدد);
      reduce_prod_reached = reduce_prod_reached
                          || شي_قاعدو (si->شي) == reduce_قاعدو;
      إذا (reduce_prod_reached)
        bitset_سيٽ (rpp_سيٽ, si->state->عدد);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_وقت) / 1e9 < CUMULATIVE_وقت_حد_بندي
    ? unifying_example (itm1, itm2, سرڪايو_reduce, سڀ_کان_ٿورو_رستو, ايندڙ_sym)
    : example_from_رستو (سرڪايو_reduce, itm2, سڀ_کان_ٿورو_رستو, ايندڙ_sym);

  gl_list_free (سڀ_کان_ٿورو_رستو);
  counterexample_print (cex, ٻاهريون, اڳياڙي);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
ساكن فراغ
counterexample_report_سرڪايو_reduce (state_شي_عدد itm1, state_شي_عدد itm2,
                                    نشان_عدد ايندڙ_sym,
                                    FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  إذا (ٻاهريون == stderr)
    complain (عدم, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[ايندڙ_sym]->ٽيگ);
  وإلا
    {
      fputs (اڳياڙي, ٻاهريون);
      fprintf (ٻاهريون, _("shift/reduce conflict on token %s"), symbols[ايندڙ_sym]->ٽيگ);
      fprintf (ٻاهريون, "%s\n", _(":"));
    }
  // In the report, print the items.
  إذا (ٻاهريون != stderr || trace_flag & trace_cex)
    {
      state_شي_print (&state_شيون[itm1], ٻاهريون, اڳياڙي);
      state_شي_print (&state_شيون[itm2], ٻاهريون, اڳياڙي);
    }
  counterexample_report (itm1, itm2, ايندڙ_sym, true, ٻاهريون, اڳياڙي);
}

ساكن فراغ
counterexample_report_reduce_reduce (state_شي_عدد itm1, state_شي_عدد itm2,
                                     bitset conflict_syms,
                                     FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  {
    بنية obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_شي_عدد sym;
    ثابت حرف *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->ٽيگ);
        sep = ", ";
      }
    حرف *ٽوڪن = obstack_finish0 (&obstack);
    إذا (ٻاهريون == stderr)
      complain (عدم, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_ڳڻپ (conflict_syms)),
                ٽوڪن);
    وإلا
      {
        fputs (اڳياڙي, ٻاهريون);
        fprintf (ٻاهريون,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_ڳڻپ (conflict_syms)),
                 ٽوڪن);
        fprintf (ٻاهريون, "%s\n", _(":"));
      }
    obstack_free (&obstack, عدم);
  }
  // In the report, print the items.
  إذا (ٻاهريون != stderr || trace_flag & trace_cex)
    {
      state_شي_print (&state_شيون[itm1], ٻاهريون, اڳياڙي);
      state_شي_print (&state_شيون[itm2], ٻاهريون, اڳياڙي);
    }
  counterexample_report (itm1, itm2, bitset_پهريون (conflict_syms),
                         false, ٻاهريون, اڳياڙي);
}

ساكن state_شي_عدد
ڳوليو_state_شي_عدد (ثابت قاعدو *r, state_عدد sn)
{
  لكل (state_شي_عدد i = state_شي_map[sn]; i < state_شي_map[sn + 1]; ++i)
    إذا (!SI_DISABLED (i)
        && شي_عدد_as_قاعدو_عدد (*state_شيون[i].شي) == r->عدد)
      أرجع i;
  abort ();
}

فراغ
counterexample_report_state (ثابت state *s, FILE *ٻاهريون, ثابت حرف *اڳياڙي)
{
  ثابت state_عدد sn = s->عدد;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  لكل (صحيح i = 0; i < reds->انگ; ++i)
    {
      ثابت قاعدو *r1 = reds->قاعدا[i];
      ثابت state_شي_عدد c1 = ڳوليو_state_شي_عدد (r1, sn);
      لكل (state_شي_عدد c2 = state_شي_map[sn]; c2 < state_شي_map[sn + 1]; ++c2)
        إذا (!SI_DISABLED (c2))
          {
            شي_عدد conf = *state_شيون[c2].شي;
            إذا (شي_عدد_is_نشان_عدد (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_سرڪايو_reduce (c1, c2, conf, ٻاهريون, اڳياڙي);
          }
      لكل (صحيح j = i+1; j < reds->انگ; ++j)
        {
          ثابت قاعدو *r2 = reds->قاعدا[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          إذا (!bitset_خالي_جايون_p (lookaheads))
            لكل (state_شي_عدد c2 = state_شي_map[sn]; c2 < state_شي_map[sn + 1]; ++c2)
              إذا (!SI_DISABLED (c2)
                  && شي_قاعدو (state_شيون[c2].شي) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, ٻاهريون, اڳياڙي);
                  اكسر;
                }
        }
    }
  bitset_free (lookaheads);
}
