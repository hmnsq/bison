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


#تعریف_کړه وخت_حد_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#تعریف_کړه EXTENDED_پلټنه false

/* costs for making various steps in a search */
#تعریف_کړه PRODUCTION_لګښت 50
#تعریف_کړه REDUCE_لګښت 1
#تعریف_کړه ښويدنه_لګښت 1
#تعریف_کړه UNSHIFT_لګښت 1
#تعریف_کړه EXTENDED_لګښت 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#تعریف_کړه ASSURANCE_حد 2.0

/* The time limit before giving up looking for unifying counterexample. */
ساكن مزدوج وخت_حد = 5.0;

#تعریف_کړه CUMULATIVE_وخت_حد 120.0

// This is the fastest way to get the tail node from the gl_list API.
ساكن gl_list_غوټه_t
list_get_پای_ټکی (gl_list_t list)
{
  gl_list_غوټه_t sentinel = gl_list_جمع_کړه_وروستی (list, عدم);
  gl_list_غوټه_t res = gl_list_پخوانی_غوټه (list, sentinel);
  gl_list_remove_غوټه (list, sentinel);
  أرجع res;
}

تعريفنوع بنية
{
  derivation *d1;
  derivation *d2;
  bool ښويدنه_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ساكن counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool ښويدنه_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (حجم *res);
  res->ښويدنه_reduce = ښويدنه_reduce;
  إذا (ښويدنه_reduce)
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
counterexample_print (ثابت counterexample *cex, FILE *بهر, ثابت حرف *مختاړی)
{
  ثابت bool flat = getenv ("YYFLAT");
  ثابت حرف *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ثابت حرف *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ثابت حرف *deriv1_label
    = cex->ښويدنه_reduce ? _("Shift derivation") : _("First reduce derivation");
  ثابت حرف *deriv2_label
    = cex->ښويدنه_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ثابت صحيح پلنوالی =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  إذا (flat)
    fprintf (بهر, "  %s%s%*s ", مختاړی,
             example1_label, پلنوالی - mbswidth (example1_label, 0), "");
  وإلا
    fprintf (بهر, "  %s%s: ", مختاړی, example1_label);
  derivation_print_leaves (cex->d1, بهر);
  إذا (flat)
    fprintf (بهر, "  %s%s%*s ", مختاړی,
             deriv1_label, پلنوالی - mbswidth (deriv1_label, 0), "");
  وإلا
    fprintf (بهر, "  %s%s", مختاړی, deriv1_label);
  derivation_print (cex->d1, بهر, مختاړی);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  إذا (!cex->unifying || is_styled (stderr))
    {
      إذا (flat)
        fprintf (بهر, "  %s%s%*s ", مختاړی,
                 example2_label, پلنوالی - mbswidth (example2_label, 0), "");
      وإلا
        fprintf (بهر, "  %s%s: ", مختاړی, example2_label);
      derivation_print_leaves (cex->d2, بهر);
    }
  إذا (flat)
    fprintf (بهر, "  %s%s%*s ", مختاړی,
             deriv2_label, پلنوالی - mbswidth (deriv2_label, 0), "");
  وإلا
    fprintf (بهر, "  %s%s", مختاړی, deriv2_label);
  derivation_print (cex->d2, بهر, مختاړی);

  إذا (بهر != stderr)
    putc ('\n', بهر);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
بنية si_bfs_غوټه;
تعريفنوع بنية si_bfs_غوټه
{
  state_توکی_شمېره si;
  بنية si_bfs_غوټه *پلار;
  صحيح reference_شمېر;
} si_bfs_غوټه;

ساكن si_bfs_غوټه *
si_bfs_new (state_توکی_شمېره si, si_bfs_غوټه *پلار)
{
  si_bfs_غوټه *res = xcalloc (1, حجم *res);
  res->si = si;
  res->پلار = پلار;
  res->reference_شمېر = 1;
  إذا (پلار)
    ++پلار->reference_شمېر;
  أرجع res;
}

ساكن bool
si_bfs_لري (ثابت si_bfs_غوټه *n, state_توکی_شمېره ساین)
{
  لكل (ثابت si_bfs_غوټه *پلټنه = n; پلټنه != عدم; پلټنه = پلټنه->پلار)
    إذا (پلټنه->si == ساین)
      أرجع true;
  أرجع false;
}

ساكن فراغ
si_bfs_free (si_bfs_غوټه *n)
{
  إذا (n == عدم)
    أرجع;
  --n->reference_شمېر;
  إذا (n->reference_شمېر == 0)
    {
      si_bfs_free (n->پلار);
      حرر (n);
    }
}

تعريفنوع gl_list_t si_bfs_غوټه_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ساكن مضمن derivation_list
expand_منزل_conflict (state_توکی_شمېره پیل, نښه_شمېره conflict_sym)
{
  si_bfs_غوټه *init = si_bfs_new (پیل, عدم);

  si_bfs_غوټه_list کتار
    = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ثابت فراغ **) &init);
  si_bfs_غوټه *غوټه = عدم;
  // breadth-first search for a path of productions to the conflict symbol
  طالما (gl_list_اندازه (کتار) > 0)
    {
      غوټه = (si_bfs_غوټه *) gl_list_get_at (کتار, 0);
      state_توکی *silast = &state_توکي[غوټه->si];
      نښه_شمېره sym = توکی_شمېره_as_نښه_شمېره (*silast->توکی);
      إذا (sym == conflict_sym)
        اكسر;
      إذا (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_توکی_شمېره ساین;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, ساین, 0)
            {
              // ignore productions already in the path
              إذا (si_bfs_لري (غوټه, ساین))
                استمر;
              si_bfs_غوټه *بل = si_bfs_new (ساین, غوټه);
              gl_list_جمع_کړه_وروستی (کتار, بل);
            }
          // for nullable nonterminals, add its goto to the search
          إذا (nullable[sym - ntokens])
            {
              si_bfs_غوټه *بل = si_bfs_new (silast->trans, غوټه);
              gl_list_جمع_کړه_وروستی (کتار, بل);
            }
        }
      gl_list_remove_at (کتار, 0);
    }
  إذا (gl_list_اندازه (کتار) == 0)
    {
      gl_list_free (کتار);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_ونې_پاڼه (conflict_sym);
  derivation_list پایله = derivation_list_new ();
  derivation_list_append (پایله, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  لكل (si_bfs_غوټه *n = غوټه; n != عدم; n = n->پلار)
    {
      state_توکی *si = &state_توکي[n->si];
      توکی_شمېره *pos = si->توکی;
      إذا (SI_PRODUCTION (si))
        {
          توکی_شمېره *i = عدم;
          لكل (i = pos + 1; !توکی_شمېره_is_قاعده_شمېره (*i); ++i)
            derivation_list_append (پایله, derivation_new_ونې_پاڼه (*i));
          نښه_شمېره lhs =
            قواعد[توکی_شمېره_as_قاعده_شمېره (*i)].lhs->شمېره;
          derivation *deriv = derivation_new (lhs, پایله,
                                              state_توکی_قاعده (si));
          پایله = derivation_list_new ();
          derivation_list_append (پایله, deriv);
        }
      وإلا
        {
          نښه_شمېره sym = توکی_شمېره_as_نښه_شمېره (*(pos - 1));
          derivation *deriv = derivation_new_ونې_پاڼه (sym);
          derivation_list_prepend (پایله, deriv);
        }
    }
  gl_list_free (کتار);
  derivation_free ((derivation*)gl_list_get_at (پایله, 0));
  gl_list_remove_at (پایله, 0);
  أرجع پایله;
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
complete_diverging_example (نښه_شمېره conflict_sym,
                            state_توکی_list لار, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list پایله = derivation_list_new ();
  bool lookahead_required = false;
  إذا (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_جمع_کړه_وروستی (پایله, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_غوټه_t deriv = list_get_پای_ټکی (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  لكل (gl_list_غوټه_t state_غوټه = list_get_پای_ټکی (لار);
       state_غوټه != عدم;
       state_غوټه = gl_list_پخوانی_غوټه (لار, state_غوټه))
    {
      state_توکی *si = (state_توکی *) gl_list_غوټه_ارزښت (لار, state_غوټه);
      توکی_شمېره *توکی = si->توکی;
      توکی_شمېره pos = *توکی;
      // symbols after dot
      إذا (gl_list_اندازه (پایله) == 1 && !توکی_شمېره_is_قاعده_شمېره (pos)
          && gl_list_get_at (پایله, 0) == derivation_dot ())
        {
          derivation_list_append (پایله,
            derivation_new_ونې_پاڼه (توکی_شمېره_as_نښه_شمېره (pos)));
          lookahead_required = false;
        }
      توکی_شمېره *i = توکی;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      لكل (state_توکی_شمېره nsi = si - state_توکي;
           !توکی_شمېره_is_قاعده_شمېره (*i);
           ++i, nsi = state_توکي[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          إذا (i == توکی)
            استمر;
          نښه_شمېره sym = توکی_شمېره_as_نښه_شمېره (*i);
          إذا (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (پایله, derivation_new_ونې_پاڼه (sym));
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
              derivation_list بل_derivs =
                expand_منزل_conflict (nsi, conflict_sym);
              derivation *d = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (بل_derivs);
                   derivation_list_بل (&it, &d);)
                derivation_list_append (پایله, d);
              i += gl_list_اندازه (بل_derivs) - 1;
              derivation_list_free (بل_derivs);
            }
          وإلا إذا (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_ونې_پاڼه (sym);
              derivation_list_append (پایله, d);
            }
          وإلا
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (پایله, derivation_new_ونې_پاڼه (1));
              lookahead_required = false;
            }
        }
      ثابت قاعده *r = &قواعد[توکی_شمېره_as_قاعده_شمېره (*i)];
      // add derivations for symbols before dot
      لكل (i = توکی - 1; !توکی_شمېره_is_قاعده_شمېره (*i) && i >= ritem; i--)
        {
          gl_list_غوټه_t p = gl_list_پخوانی_غوټه (لار, state_غوټه);
          إذا (p)
            state_غوټه = p;
          إذا (deriv)
            {
              ثابت فراغ *tmp_deriv = gl_list_غوټه_ارزښت (derivs, deriv);
              deriv = gl_list_پخوانی_غوټه (derivs, deriv);
              derivation_list_prepend (پایله, (derivation*)tmp_deriv);
            }
          وإلا
            derivation_list_prepend (پایله, derivation_new_ونې_پاڼه (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->شمېره, پایله, r);
      پایله = derivation_list_new ();
      derivation_list_append (پایله, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (پایله, 0);
  derivation_retain (res);
  derivation_list_free (پایله);
  derivation_list_free (derivs);
  أرجع res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ساكن state_توکی_list
nonunifying_ښويدنه_لار (state_توکی_list reduce_لار, state_توکی *ښويدنه_conflict)
{
  gl_list_غوټه_t tmp = gl_list_جمع_کړه_وروستی (reduce_لار, عدم);
  gl_list_غوټه_t بل_غوټه = gl_list_پخوانی_غوټه (reduce_لار, tmp);
  gl_list_غوټه_t غوټه = gl_list_پخوانی_غوټه (reduce_لار, بل_غوټه);
  gl_list_remove_غوټه (reduce_لار, tmp);
  state_توکی *si = ښويدنه_conflict;
  state_توکی_list پایله =
    gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
  // FIXME: bool paths_merged;
  لكل (; غوټه != عدم; بل_غوټه = غوټه,
       غوټه = gl_list_پخوانی_غوټه (reduce_لار, غوټه))
    {
      state_توکی *refsi =
        (state_توکی *) gl_list_غوټه_ارزښت (reduce_لار, غوټه);
      state_توکی *nextrefsi =
        (state_توکی *) gl_list_غوټه_ارزښت (reduce_لار, بل_غوټه);
      إذا (nextrefsi == si)
        {
          gl_list_جمع_کړه_لومړی (پایله, refsi);
          si = refsi;
          استمر;
        }
      // skip reduction items
      إذا (nextrefsi->توکی != refsi->توکی + 1 && refsi->توکی != ritem)
        استمر;

      // bfs to find a shift to the right state
      si_bfs_غوټه *init = si_bfs_new (si - state_توکي, عدم);
      si_bfs_غوټه_list کتار
        = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ثابت فراغ **) &init);
      si_bfs_غوټه *sis = عدم;
      state_توکی *prevsi = عدم;
      طالما (gl_list_اندازه (کتار) > 0)
        {
          sis = (si_bfs_غوټه *) gl_list_get_at (کتار, 0);
          // if we end up in the start state, the shift couldn't be found.
          إذا (sis->si == 0)
            اكسر;

          state_توکی *پلټنه_si = &state_توکي[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = پلټنه_si->revs;
          bitset_iterator biter;
          state_توکی_شمېره ساین;
          BITSET_FOR_EACH (biter, rsi, ساین, 0)
            {
              prevsi = &state_توکي[ساین];
              إذا (SI_TRANSITION (پلټنه_si))
                {
                  إذا (prevsi->state == refsi->state)
                    اذهب پلټنه_پای_ټکی;
                }
              وإلا إذا (!si_bfs_لري (sis, ساین))
                {
                  si_bfs_غوټه *prevsis = si_bfs_new (ساین, sis);
                  gl_list_جمع_کړه_وروستی (کتار, prevsis);
                }
            }
          gl_list_remove_at (کتار, 0);
        }
    پلټنه_پای_ټکی:
      // prepend path to shift we found
      إذا (sis)
        {
          gl_list_غوټه_t ln = gl_list_جمع_کړه_لومړی (پایله, &state_توکي[sis->si]);
          لكل (si_bfs_غوټه *n = sis->پلار; n; n = n->پلار)
            ln = gl_list_جمع_کړه_after (پایله, ln, &state_توکي[n->si]);

        }
      si = prevsi;
      gl_list_free (کتار);
    }
  إذا (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_توکی *sip = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (پایله);
           state_توکی_list_بل (&it, &sip);
           )
        state_توکی_print (sip, stderr, "");
    }
  أرجع پایله;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ساكن counterexample *
example_from_لار (bool ښويدنه_reduce,
                   state_توکی_شمېره itm2,
                   state_توکی_list تر_ټولو_لنډ_لار, نښه_شمېره بل_sym)
{
  derivation *deriv1 =
    complete_diverging_example (بل_sym, تر_ټولو_لنډ_لار, عدم);
  state_توکی_list لار_2
    = ښويدنه_reduce
    ? nonunifying_ښويدنه_لار (تر_ټولو_لنډ_لار, &state_توکي [itm2])
    : تر_ټولو_لنډ_لار_from_پیل (itm2, بل_sym);
  derivation *deriv2 = complete_diverging_example (بل_sym, لار_2, عدم);
  gl_list_free (لار_2);
  أرجع new_counterexample (deriv1, deriv2, ښويدنه_reduce, false, true);
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
  parse_state *حالتونه[2];
  صحيح complexity;
} پلټنه_state;

ساكن پلټنه_state *
initial_پلټنه_state (state_توکی *conflict1, state_توکی *conflict2)
{
  پلټنه_state *res = xmalloc (حجم *res);
  res->حالتونه[0] = new_parse_state (conflict1);
  res->حالتونه[1] = new_parse_state (conflict2);
  parse_state_retain (res->حالتونه[0]);
  parse_state_retain (res->حالتونه[1]);
  res->complexity = 0;
  أرجع res;
}

ساكن پلټنه_state *
new_پلټنه_state (parse_state *ps1, parse_state *ps2, صحيح complexity)
{
  پلټنه_state *res = xmalloc (حجم *res);
  res->حالتونه[0] = ps1;
  res->حالتونه[1] = ps2;
  parse_state_retain (res->حالتونه[0]);
  parse_state_retain (res->حالتونه[1]);
  res->complexity = complexity;
  أرجع res;
}

ساكن پلټنه_state *
copy_پلټنه_state (پلټنه_state *پلار)
{
  پلټنه_state *res = xmalloc (حجم *res);
  *res = *پلار;
  parse_state_retain (res->حالتونه[0]);
  parse_state_retain (res->حالتونه[1]);
  أرجع res;
}

ساكن فراغ
پلټنه_state_free_children (پلټنه_state *ss)
{
  free_parse_state (ss->حالتونه[0]);
  free_parse_state (ss->حالتونه[1]);
}

ساكن فراغ
پلټنه_state_free (پلټنه_state *ss)
{
  إذا (ss == عدم)
    أرجع;
  پلټنه_state_free_children (ss);
  حرر (ss);
}

/* For debugging traces.  */
ساكن فراغ
پلټنه_state_print (پلټنه_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->حالتونه[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->حالتونه[1]);
  putc ('\n', stderr);
}

تعريفنوع gl_list_t پلټنه_state_list;

ساكن مضمن bool
پلټنه_state_list_بل (gl_list_iterator_t *it, پلټنه_state **ss)
{
  ثابت فراغ *p = عدم;
  bool res = gl_list_iterator_بل (it, &p, عدم);
  إذا (res)
    *ss = (پلټنه_state*) p;
  وإلا
    gl_list_iterator_free (it);
  أرجع res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ساكن مضمن فراغ
ss_سېټ_parse_state (پلټنه_state *ss, صحيح idx, parse_state *ps)
{
  free_parse_state (ss->حالتونه[idx]);
  ss->حالتونه[idx] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
ساكن counterexample *
complete_diverging_examples (پلټنه_state *ss,
                             نښه_شمېره بل_sym,
                             bool ښويدنه_reduce)
{
  derivation *new_derivs[2];
  لكل (صحيح i = 0; i < 2; ++i)
    {
      state_توکی_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->حالتونه[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (بل_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  أرجع new_counterexample (new_derivs[0], new_derivs[1],
                             ښويدنه_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
تعريفنوع بنية
{
  پلټنه_state_list حالتونه;
  صحيح complexity;
} پلټنه_state_bundle;

ساكن فراغ
ssb_free (پلټنه_state_bundle *ssb)
{
  gl_list_free (ssb->حالتونه);
  حرر (ssb);
}

ساكن size_t
ssb_hasher (پلټنه_state_bundle *ssb)
{
  أرجع ssb->complexity;
}

ساكن صحيح
ssb_comp (ثابت پلټنه_state_bundle *s1, ثابت پلټنه_state_bundle *s2)
{
  أرجع s1->complexity - s2->complexity;
}

ساكن bool
ssb_equals (ثابت پلټنه_state_bundle *s1, ثابت پلټنه_state_bundle *s2)
{
  أرجع s1->complexity == s2->complexity;
}

تعريفنوع gl_list_t ssb_list;

ساكن size_t
لیدل_شوی_hasher (ثابت پلټنه_state *ss, size_t max)
{
  أرجع (parse_state_hasher (ss->حالتونه[0], max)
          + parse_state_hasher (ss->حالتونه[1], max)) % max;
}

ساكن bool
لیدل_شوی_comparator (ثابت پلټنه_state *ss1, ثابت پلټنه_state *ss2)
{
  أرجع parse_state_comparator (ss1->حالتونه[0], ss2->حالتونه[0])
    && parse_state_comparator (ss1->حالتونه[1], ss2->حالتونه[1]);
}

/* Priority queue for search states with minimal complexity. */
ساكن ssb_list ssb_کتار;
ساكن Hash_جدول *لیدل_شوی;
/* The set of parser states on the shortest lookahead-sensitive path. */
ساكن bitset scp_سېټ = عدم;
/* The set of parser states used for the conflict reduction rule. */
ساكن bitset rpp_سېټ = عدم;

ساكن فراغ
ssb_append (پلټنه_state *ss)
{
  إذا (hash_lookup (لیدل_شوی, ss))
    {
      پلټنه_state_free (ss);
      أرجع;
    }
  hash_xinsert (لیدل_شوی, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->حالتونه[0]);
  parse_state_free_contents_early (ss->حالتونه[1]);
  parse_state_retain (ss->حالتونه[0]);
  parse_state_retain (ss->حالتونه[1]);
  پلټنه_state_bundle *ssb = xmalloc (حجم *ssb);
  ssb->complexity = ss->complexity;
  gl_list_غوټه_t n = gl_list_پلټنه (ssb_کتار, ssb);
  إذا (!n)
    {
      ssb->حالتونه =
        gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم,
                              (gl_listelement_dispose_fn)پلټنه_state_free_children,
                              true);
      gl_sortedlist_جمع_کړه (ssb_کتار, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  وإلا
    {
      حرر (ssb);
      ssb = (پلټنه_state_bundle *) gl_list_غوټه_ارزښت (ssb_کتار, n);
    }
  gl_list_جمع_کړه_وروستی (ssb->حالتونه, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ساكن فراغ
production_ګام (پلټنه_state *ss, صحيح parser_state)
{
  ثابت state_توکی *other_si = parse_state_لکۍ (ss->حالتونه[1 - parser_state]);
  نښه_شمېره other_sym = توکی_شمېره_as_نښه_شمېره (*other_si->توکی);
  parse_state_list prods =
    simulate_production (ss->حالتونه[parser_state], other_sym);
  صحيح complexity = ss->complexity + PRODUCTION_لګښت;

  parse_state *ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_بل (&it, &ps);
       )
    {
      پلټنه_state *copy = copy_پلټنه_state (ss);
      ss_سېټ_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ساكن مضمن صحيح
reduction_لګښت (ثابت parse_state *ps)
{
  صحيح shifts;
  صحيح productions;
  parse_state_completed_ګامونه (ps, &shifts, &productions);
  أرجع ښويدنه_لګښت * shifts + PRODUCTION_لګښت * productions;
}

ساكن پلټنه_state_list
reduction_ګام (پلټنه_state *ss, ثابت توکی_شمېره *conflict_توکی,
                صحيح parser_state, صحيح قاعده_len)
{
  (فراغ) conflict_توکی; // FIXME: Unused
  پلټنه_state_list پایله =
    gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, 1);

  parse_state *ps = ss->حالتونه[parser_state];
  ثابت state_توکی *si = parse_state_لکۍ (ps);
  bitset نښه_سېټ = si->lookahead;
  parse_state *other = ss->حالتونه[1 - parser_state];
  ثابت state_توکی *other_si = parse_state_لکۍ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  إذا (توکی_شمېره_is_نښه_شمېره (*other_si->توکی))
    {
      نښه_شمېره other_sym =
        توکی_شمېره_as_نښه_شمېره (*other_si->توکی);
      إذا (!intersect_نښه (other_sym, نښه_سېټ))
        أرجع پایله;
      نښه_سېټ = bitset_create (nsyms, BITSET_FIXED);
      bitset_سېټ (نښه_سېټ, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, قاعده_len, نښه_سېټ);
  parse_state *reduced_ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_بل (&it, &reduced_ps);
       )
    {
      پلټنه_state *copy = copy_پلټنه_state (ss);
      ss_سېټ_parse_state (copy, parser_state, reduced_ps);
      صحيح r_لګښت = reduction_لګښت (reduced_ps);
      copy->complexity += r_لګښت + PRODUCTION_لګښت + 2 * ښويدنه_لګښت;
      gl_list_جمع_کړه_وروستی (پایله, copy);
    }
  gl_list_free (reduced);
  إذا (نښه_سېټ != si->lookahead)
    bitset_free (نښه_سېټ);
  أرجع پایله;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ساكن فراغ
پلټنه_state_prepend (پلټنه_state *ss, نښه_شمېره sym, bitset guide)
{
  (فراغ) sym; // FIXME: Unused.
  ثابت state_توکی *si1src = parse_state_سر (ss->حالتونه[0]);
  ثابت state_توکی *si2src = parse_state_سر (ss->حالتونه[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  إذا (prod1 != SI_PRODUCTION (si2src))
    {
      صحيح prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->حالتونه[prod_state]);
      parse_state *ps = عدم;
      لكل (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_بل (&iter, &ps);
           )
        {
          ثابت state_توکی *psi = parse_state_سر (ps);
          bool guided = bitset_test (guide, psi->state->شمېره);
          إذا (!guided && !EXTENDED_پلټنه)
            استمر;

          پلټنه_state *copy = copy_پلټنه_state (ss);
          ss_سېټ_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_لګښت;
          إذا (!guided)
            copy->complexity += EXTENDED_لګښت;
          ssb_append (copy);
        }
      gl_list_free (prev);
      أرجع;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ثابت صحيح complexity_لګښت = 2 * (prod1 ? PRODUCTION_لګښت : UNSHIFT_لګښت);

  parse_state_list prev1 = parser_prepend (ss->حالتونه[0]);
  parse_state_list prev2 = parser_prepend (ss->حالتونه[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = عدم;
  لكل (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_بل (&iter1, &ps1);
       )
    {
      ثابت state_توکی *psi1 = parse_state_سر (ps1);
      bool guided1 = bitset_test (guide, psi1->state->شمېره);
      إذا (!guided1 && !EXTENDED_پلټنه)
        استمر;

      parse_state *ps2 = عدم;
      لكل (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_بل (&iter2, &ps2);
           )
        {
          ثابت state_توکی *psi2 = parse_state_سر (ps2);

          bool guided2 = bitset_test (guide, psi2->state->شمېره);
          إذا (!guided2 && !EXTENDED_پلټنه)
            استمر;
          // Only consider prepend state items that share the same state.
          إذا (psi1->state != psi2->state)
            استمر;

          صحيح complexity = ss->complexity + complexity_لګښت;
          // penalty for not being along the guide path
          إذا (!guided1 || !guided2)
            complexity += EXTENDED_لګښت;
          ssb_append (new_پلټنه_state (ps1, ps2, complexity));
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
have_common_مختاړی (ثابت توکی_شمېره *itm1, ثابت توکی_شمېره *itm2)
{
  صحيح i = 0;
  لكل (; !توکی_شمېره_is_قاعده_شمېره (itm1[i]); ++i)
    إذا (itm1[i] != itm2[i])
      أرجع false;
  أرجع توکی_شمېره_is_قاعده_شمېره (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ساكن ثابت توکی_شمېره *
توکی_قاعده_پیل (ثابت توکی_شمېره *توکی)
{
  ثابت توکی_شمېره *res = عدم;
  لكل (res = توکی;
       ritem < res && توکی_شمېره_is_نښه_شمېره (*(res - 1));
       --res)
    استمر;
  أرجع res;
}

ساكن ثابت توکی_شمېره *
توکی_قاعده_پای_ټکی (ثابت توکی_شمېره *توکی)
{
  ثابت توکی_شمېره *res = عدم;
  لكل (res = توکی; توکی_شمېره_is_نښه_شمېره (*res); ++res)
    استمر;
  أرجع res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ساكن مضمن فراغ
generate_بل_حالتونه (پلټنه_state *ss, state_توکی *conflict1,
                      state_توکی *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->حالتونه[0];
  parse_state *ps2 = ss->حالتونه[1];
  ثابت state_توکی *si1 = parse_state_لکۍ (ps1);
  ثابت state_توکی *si2 = parse_state_لکۍ (ps2);
  bool si1reduce = توکی_شمېره_is_قاعده_شمېره (*si1->توکی);
  bool si2reduce = توکی_شمېره_is_قاعده_شمېره (*si2->توکی);
  إذا (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      إذا (*si1->توکی == *si2->توکی)
        {
          صحيح complexity = ss->complexity + 2 * ښويدنه_لګښت;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = عدم;
          parse_state *tps2 = عدم;
          لكل (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_بل (&it1, &tps1);
               )
            لكل (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_بل (&it2, &tps2);
                 )
              ssb_append (new_پلټنه_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_ګام (ss, 0);
      production_ګام (ss, 1);
    }
  // One of the states requires a reduction
  وإلا
    {
      ثابت توکی_شمېره *rhs1 = توکی_قاعده_پیل (si1->توکی);
      ثابت توکی_شمېره *rhe1 = توکی_قاعده_پای_ټکی (si1->توکی);
      صحيح len1 = rhe1 - rhs1;
      صحيح size1 = parse_state_اوږدتیا (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ثابت توکی_شمېره *rhs2 = توکی_قاعده_پیل (si2->توکی);
      ثابت توکی_شمېره *rhe2 = توکی_قاعده_پای_ټکی (si2->توکی);
      صحيح len2 = rhe2 - rhs2;
      صحيح size2 = parse_state_اوږدتیا (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      إذا (ready1 && ready2)
        {
          پلټنه_state_list reduced1 = reduction_ګام (ss, conflict1->توکی, 0, len1);
          gl_list_جمع_کړه_وروستی (reduced1, ss);
          پلټنه_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               پلټنه_state_list_بل (&iter, &red1);
               )
            {
              پلټنه_state_list reduced2 =
                reduction_ګام (red1, conflict2->توکی, 1, len2);
              پلټنه_state *red2 = عدم;
              لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   پلټنه_state_list_بل (&iter2, &red2);
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
          پلټنه_state_list reduced1 = reduction_ګام (ss, conflict1->توکی, 0, len1);
          پلټنه_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               پلټنه_state_list_بل (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      وإلا إذا (ready2)
        {
          پلټنه_state_list reduced2 = reduction_ګام (ss, conflict2->توکی, 1, len2);
          پلټنه_state *red2 = عدم;
          لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               پلټنه_state_list_بل (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      وإلا
        {
          ثابت نښه_شمېره sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          پلټنه_state_prepend (ss, sym,
                                parse_state_ژوروالی (ss->حالتونه[0]) >= 0
                                ? rpp_سېټ : scp_سېټ);
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
unifying_example (state_توکی_شمېره itm1,
                  state_توکی_شمېره itm2,
                  bool ښويدنه_reduce,
                  state_توکی_list reduce_لار, نښه_شمېره بل_sym)
{
  state_توکی *conflict1 = &state_توکي[itm1];
  state_توکی *conflict2 = &state_توکي[itm2];
  پلټنه_state *initial = initial_پلټنه_state (conflict1, conflict2);
  ssb_کتار = gl_list_create_تشه (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  لیدل_شوی =
    hash_initialize (32, عدم, (Hash_hasher) لیدل_شوی_hasher,
                     (Hash_comparator) لیدل_شوی_comparator,
                     (Hash_معلومات_freer) پلټنه_state_free);
  ssb_append (initial);
  xtime_t پیل = gethrxtime ();
  bool assurance_printed = false;
  پلټنه_state *stage3result = عدم;
  counterexample *cex = عدم;
  طالما (gl_list_اندازه (ssb_کتار) > 0)
    {
      ثابت پلټنه_state_bundle *ssb = gl_list_get_at (ssb_کتار, 0);

      پلټنه_state *ss = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (ssb->حالتونه);
           پلټنه_state_list_بل (&it, &ss);
           )
        {
          إذا (trace_flag & trace_cex)
            پلټنه_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->حالتونه[0];
          parse_state *ps2 = ss->حالتونه[1];
          إذا (parse_state_ژوروالی (ps1) < 0 && parse_state_ژوروالی (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ثابت state_توکی *si1src = parse_state_سر (ps1);
              ثابت state_توکی *si2src = parse_state_سر (ps2);
              إذا (توکی_قاعده (si1src->توکی)->lhs == توکی_قاعده (si2src->توکی)->lhs
                  && have_common_مختاړی (si1src->توکی, si2src->توکی))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  إذا (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, ښويدنه_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      اذهب cex_پلټنه_پای_ټکی;
                    }
                  إذا (!stage3result)
                    stage3result = copy_پلټنه_state (ss);
                }
            }
          إذا (وخت_حد_ENFORCED)
            {
              مزدوج وخت_passed = (gethrxtime () - پیل) / 1e9;
              إذا (!assurance_printed && وخت_passed > ASSURANCE_حد
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              إذا (وخت_passed > وخت_حد)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", وخت_passed);
                  اذهب cex_پلټنه_پای_ټکی;
                }
            }
          generate_بل_حالتونه (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_کتار,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_پلټنه_پای_ټکی:;
  إذا (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      إذا (stage3result)
        cex = complete_diverging_examples (stage3result, بل_sym, ښويدنه_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      وإلا
        cex = example_from_لار (ښويدنه_reduce, itm2, reduce_لار, بل_sym);
    }
  gl_list_free (ssb_کتار);
  hash_free (لیدل_شوی);
  إذا (stage3result)
    پلټنه_state_free (stage3result);
  أرجع cex;
}

ساكن xtime_t cumulative_وخت;

فراغ
counterexample_init (فراغ)
{
  // Check cex.timeout.
  {
    ثابت حرف *variable = "cex.timeout";
    حرف *cp = muscle_percent_define_get (variable);
    إذا (*cp != '\0')
      {
        حرف *پای_ټکی = عدم;
        مزدوج v = c_strtod (cp, &پای_ټکی);
        إذا (*پای_ټکی == '\0' && errno == 0 && v >= 0)
          وخت_حد = v;
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
  cumulative_وخت = gethrxtime ();
  scp_سېټ = bitset_create (nstates, BITSET_FIXED);
  rpp_سېټ = bitset_create (nstates, BITSET_FIXED);
  state_توکي_init ();
}


فراغ
counterexample_free (فراغ)
{
  إذا (scp_سېټ)
    {
      bitset_free (scp_سېټ);
      bitset_free (rpp_سېټ);
      state_توکي_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ساكن فراغ
counterexample_report (state_توکی_شمېره itm1, state_توکی_شمېره itm2,
                       نښه_شمېره بل_sym, bool ښويدنه_reduce,
                       FILE *بهر, ثابت حرف *مختاړی)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_توکی_list تر_ټولو_لنډ_لار = تر_ټولو_لنډ_لار_from_پیل (itm1, بل_sym);
  bool reduce_prod_reached = false;
  ثابت قاعده *reduce_قاعده = توکی_قاعده (state_توکي[itm1].توکی);

  bitset_zero (scp_سېټ);
  bitset_zero (rpp_سېټ);

  state_توکی *si = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (تر_ټولو_لنډ_لار);
       state_توکی_list_بل (&it, &si);
       )
    {
      bitset_سېټ (scp_سېټ, si->state->شمېره);
      reduce_prod_reached = reduce_prod_reached
                          || توکی_قاعده (si->توکی) == reduce_قاعده;
      إذا (reduce_prod_reached)
        bitset_سېټ (rpp_سېټ, si->state->شمېره);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_وخت) / 1e9 < CUMULATIVE_وخت_حد
    ? unifying_example (itm1, itm2, ښويدنه_reduce, تر_ټولو_لنډ_لار, بل_sym)
    : example_from_لار (ښويدنه_reduce, itm2, تر_ټولو_لنډ_لار, بل_sym);

  gl_list_free (تر_ټولو_لنډ_لار);
  counterexample_print (cex, بهر, مختاړی);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
ساكن فراغ
counterexample_report_ښويدنه_reduce (state_توکی_شمېره itm1, state_توکی_شمېره itm2,
                                    نښه_شمېره بل_sym,
                                    FILE *بهر, ثابت حرف *مختاړی)
{
  إذا (بهر == stderr)
    complain (عدم, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[بل_sym]->نښان);
  وإلا
    {
      fputs (مختاړی, بهر);
      fprintf (بهر, _("shift/reduce conflict on token %s"), symbols[بل_sym]->نښان);
      fprintf (بهر, "%s\n", _(":"));
    }
  // In the report, print the items.
  إذا (بهر != stderr || trace_flag & trace_cex)
    {
      state_توکی_print (&state_توکي[itm1], بهر, مختاړی);
      state_توکی_print (&state_توکي[itm2], بهر, مختاړی);
    }
  counterexample_report (itm1, itm2, بل_sym, true, بهر, مختاړی);
}

ساكن فراغ
counterexample_report_reduce_reduce (state_توکی_شمېره itm1, state_توکی_شمېره itm2,
                                     bitset conflict_syms,
                                     FILE *بهر, ثابت حرف *مختاړی)
{
  {
    بنية obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_توکی_شمېره sym;
    ثابت حرف *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->نښان);
        sep = ", ";
      }
    حرف *نښې = obstack_finish0 (&obstack);
    إذا (بهر == stderr)
      complain (عدم, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_شمېر (conflict_syms)),
                نښې);
    وإلا
      {
        fputs (مختاړی, بهر);
        fprintf (بهر,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_شمېر (conflict_syms)),
                 نښې);
        fprintf (بهر, "%s\n", _(":"));
      }
    obstack_free (&obstack, عدم);
  }
  // In the report, print the items.
  إذا (بهر != stderr || trace_flag & trace_cex)
    {
      state_توکی_print (&state_توکي[itm1], بهر, مختاړی);
      state_توکی_print (&state_توکي[itm2], بهر, مختاړی);
    }
  counterexample_report (itm1, itm2, bitset_لومړی (conflict_syms),
                         false, بهر, مختاړی);
}

ساكن state_توکی_شمېره
پیدا_کړه_state_توکی_شمېره (ثابت قاعده *r, state_شمېره sn)
{
  لكل (state_توکی_شمېره i = state_توکی_map[sn]; i < state_توکی_map[sn + 1]; ++i)
    إذا (!SI_DISABLED (i)
        && توکی_شمېره_as_قاعده_شمېره (*state_توکي[i].توکی) == r->شمېره)
      أرجع i;
  abort ();
}

فراغ
counterexample_report_state (ثابت state *s, FILE *بهر, ثابت حرف *مختاړی)
{
  ثابت state_شمېره sn = s->شمېره;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  لكل (صحيح i = 0; i < reds->عدد; ++i)
    {
      ثابت قاعده *r1 = reds->قواعد[i];
      ثابت state_توکی_شمېره c1 = پیدا_کړه_state_توکی_شمېره (r1, sn);
      لكل (state_توکی_شمېره c2 = state_توکی_map[sn]; c2 < state_توکی_map[sn + 1]; ++c2)
        إذا (!SI_DISABLED (c2))
          {
            توکی_شمېره conf = *state_توکي[c2].توکی;
            إذا (توکی_شمېره_is_نښه_شمېره (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_ښويدنه_reduce (c1, c2, conf, بهر, مختاړی);
          }
      لكل (صحيح j = i+1; j < reds->عدد; ++j)
        {
          ثابت قاعده *r2 = reds->قواعد[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          إذا (!bitset_تشه_p (lookaheads))
            لكل (state_توکی_شمېره c2 = state_توکی_map[sn]; c2 < state_توکی_map[sn + 1]; ++c2)
              إذا (!SI_DISABLED (c2)
                  && توکی_قاعده (state_توکي[c2].توکی) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, بهر, مختاړی);
                  اكسر;
                }
        }
    }
  bitset_free (lookaheads);
}
