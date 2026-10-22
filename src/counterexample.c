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


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_گەڕان false

/* costs for making various steps in a search */
#define PRODUCTION_COST 50
#define REDUCE_COST 1
#define SHIFT_COST 1
#define UNSHIFT_COST 1
#define EXTENDED_COST 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#define ASSURANCE_LIMIT 2.0

/* The time limit before giving up looking for unifying counterexample. */
ساكن مزدوج time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
ساكن gl_list_گرێ_t
list_get_end (gl_list_t list)
{
  gl_list_گرێ_t sentinel = gl_list_add_last (list, عدم);
  gl_list_گرێ_t res = gl_list_previous_گرێ (list, sentinel);
  gl_list_remove_گرێ (list, sentinel);
  أرجع res;
}

تعريفنوع بنية
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ساكن counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (حجم *res);
  res->shift_reduce = shift_reduce;
  إذا (shift_reduce)
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
counterexample_print (ثابت counterexample *cex, FILE *out, ثابت حرف *prefix)
{
  ثابت bool flat = getenv ("YYFLAT");
  ثابت حرف *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ثابت حرف *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ثابت حرف *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  ثابت حرف *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ثابت صحيح width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  إذا (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  وإلا
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  إذا (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  وإلا
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  إذا (!cex->unifying || is_styled (stderr))
    {
      إذا (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      وإلا
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  إذا (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  وإلا
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  إذا (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
بنية si_bfs_گرێ;
تعريفنوع بنية si_bfs_گرێ
{
  state_item_ژمار si;
  بنية si_bfs_گرێ *parent;
  صحيح reference_ژماردن;
} si_bfs_گرێ;

ساكن si_bfs_گرێ *
si_bfs_new (state_item_ژمار si, si_bfs_گرێ *parent)
{
  si_bfs_گرێ *res = xcalloc (1, حجم *res);
  res->si = si;
  res->parent = parent;
  res->reference_ژماردن = 1;
  إذا (parent)
    ++parent->reference_ژماردن;
  أرجع res;
}

ساكن bool
si_bfs_contains (ثابت si_bfs_گرێ *n, state_item_ژمار sin)
{
  لكل (ثابت si_bfs_گرێ *گەڕان = n; گەڕان != عدم; گەڕان = گەڕان->parent)
    إذا (گەڕان->si == sin)
      أرجع true;
  أرجع false;
}

ساكن فراغ
si_bfs_free (si_bfs_گرێ *n)
{
  إذا (n == عدم)
    أرجع;
  --n->reference_ژماردن;
  إذا (n->reference_ژماردن == 0)
    {
      si_bfs_free (n->parent);
      حرر (n);
    }
}

تعريفنوع gl_list_t si_bfs_گرێ_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ساكن مضمن derivation_list
expand_to_conflict (state_item_ژمار start, هێما_ژمار conflict_sym)
{
  si_bfs_گرێ *init = si_bfs_new (start, عدم);

  si_bfs_گرێ_list queue
    = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ثابت فراغ **) &init);
  si_bfs_گرێ *گرێ = عدم;
  // breadth-first search for a path of productions to the conflict symbol
  طالما (gl_list_size (queue) > 0)
    {
      گرێ = (si_bfs_گرێ *) gl_list_get_at (queue, 0);
      state_item *silast = &state_کاڵاکان[گرێ->si];
      هێما_ژمار sym = item_ژمار_as_هێما_ژمار (*silast->item);
      إذا (sym == conflict_sym)
        اكسر;
      إذا (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_ژمار sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              إذا (si_bfs_contains (گرێ, sin))
                استمر;
              si_bfs_گرێ *next = si_bfs_new (sin, گرێ);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          إذا (nullable[sym - ntokens])
            {
              si_bfs_گرێ *next = si_bfs_new (silast->trans, گرێ);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  إذا (gl_list_size (queue) == 0)
    {
      gl_list_free (queue);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_leaf (conflict_sym);
  derivation_list result = derivation_list_new ();
  derivation_list_append (result, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  لكل (si_bfs_گرێ *n = گرێ; n != عدم; n = n->parent)
    {
      state_item *si = &state_کاڵاکان[n->si];
      item_ژمار *pos = si->item;
      إذا (SI_PRODUCTION (si))
        {
          item_ژمار *i = عدم;
          لكل (i = pos + 1; !item_ژمار_is_rule_ژمار (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          هێما_ژمار lhs =
            rules[item_ژمار_as_rule_ژمار (*i)].lhs->ژمار;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      وإلا
        {
          هێما_ژمار sym = item_ژمار_as_هێما_ژمار (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  أرجع result;
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
complete_diverging_example (هێما_ژمار conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  إذا (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_گرێ_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  لكل (gl_list_گرێ_t state_گرێ = list_get_end (path);
       state_گرێ != عدم;
       state_گرێ = gl_list_previous_گرێ (path, state_گرێ))
    {
      state_item *si = (state_item *) gl_list_گرێ_بەها (path, state_گرێ);
      item_ژمار *item = si->item;
      item_ژمار pos = *item;
      // symbols after dot
      إذا (gl_list_size (result) == 1 && !item_ژمار_is_rule_ژمار (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_ژمار_as_هێما_ژمار (pos)));
          lookahead_required = false;
        }
      item_ژمار *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      لكل (state_item_ژمار nsi = si - state_کاڵاکان;
           !item_ژمار_is_rule_ژمار (*i);
           ++i, nsi = state_کاڵاکان[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          إذا (i == item)
            استمر;
          هێما_ژمار sym = item_ژمار_as_هێما_ژمار (*i);
          إذا (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
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
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = عدم;
              لكل (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          وإلا إذا (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          وإلا
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      ثابت rule *r = &rules[item_ژمار_as_rule_ژمار (*i)];
      // add derivations for symbols before dot
      لكل (i = item - 1; !item_ژمار_is_rule_ژمار (*i) && i >= ritem; i--)
        {
          gl_list_گرێ_t p = gl_list_previous_گرێ (path, state_گرێ);
          إذا (p)
            state_گرێ = p;
          إذا (deriv)
            {
              ثابت فراغ *tmp_deriv = gl_list_گرێ_بەها (derivs, deriv);
              deriv = gl_list_previous_گرێ (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          وإلا
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->ژمار, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  أرجع res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ساكن state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_گرێ_t tmp = gl_list_add_last (reduce_path, عدم);
  gl_list_گرێ_t next_گرێ = gl_list_previous_گرێ (reduce_path, tmp);
  gl_list_گرێ_t گرێ = gl_list_previous_گرێ (reduce_path, next_گرێ);
  gl_list_remove_گرێ (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, true);
  // FIXME: bool paths_merged;
  لكل (; گرێ != عدم; next_گرێ = گرێ,
       گرێ = gl_list_previous_گرێ (reduce_path, گرێ))
    {
      state_item *refsi =
        (state_item *) gl_list_گرێ_بەها (reduce_path, گرێ);
      state_item *nextrefsi =
        (state_item *) gl_list_گرێ_بەها (reduce_path, next_گرێ);
      إذا (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          استمر;
        }
      // skip reduction items
      إذا (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        استمر;

      // bfs to find a shift to the right state
      si_bfs_گرێ *init = si_bfs_new (si - state_کاڵاکان, عدم);
      si_bfs_گرێ_list queue
        = gl_list_create (GL_LINKED_LIST, عدم, عدم,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ثابت فراغ **) &init);
      si_bfs_گرێ *sis = عدم;
      state_item *prevsi = عدم;
      طالما (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_گرێ *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          إذا (sis->si == 0)
            اكسر;

          state_item *گەڕان_si = &state_کاڵاکان[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = گەڕان_si->revs;
          bitset_iterator biter;
          state_item_ژمار sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_کاڵاکان[sin];
              إذا (SI_TRANSITION (گەڕان_si))
                {
                  إذا (prevsi->state == refsi->state)
                    اذهب گەڕان_end;
                }
              وإلا إذا (!si_bfs_contains (sis, sin))
                {
                  si_bfs_گرێ *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    گەڕان_end:
      // prepend path to shift we found
      إذا (sis)
        {
          gl_list_گرێ_t ln = gl_list_add_first (result, &state_کاڵاکان[sis->si]);
          لكل (si_bfs_گرێ *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_کاڵاکان[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  إذا (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  أرجع result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ساكن counterexample *
example_from_path (bool shift_reduce,
                   state_item_ژمار itm2,
                   state_item_list shortest_path, هێما_ژمار next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, عدم);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_کاڵاکان [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, عدم);
  gl_list_free (path_2);
  أرجع new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
  parse_state *states[2];
  صحيح complexity;
} گەڕان_state;

ساكن گەڕان_state *
initial_گەڕان_state (state_item *conflict1, state_item *conflict2)
{
  گەڕان_state *res = xmalloc (حجم *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  أرجع res;
}

ساكن گەڕان_state *
new_گەڕان_state (parse_state *ps1, parse_state *ps2, صحيح complexity)
{
  گەڕان_state *res = xmalloc (حجم *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  أرجع res;
}

ساكن گەڕان_state *
copy_گەڕان_state (گەڕان_state *parent)
{
  گەڕان_state *res = xmalloc (حجم *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  أرجع res;
}

ساكن فراغ
گەڕان_state_free_children (گەڕان_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

ساكن فراغ
گەڕان_state_free (گەڕان_state *ss)
{
  إذا (ss == عدم)
    أرجع;
  گەڕان_state_free_children (ss);
  حرر (ss);
}

/* For debugging traces.  */
ساكن فراغ
گەڕان_state_print (گەڕان_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

تعريفنوع gl_list_t گەڕان_state_list;

ساكن مضمن bool
گەڕان_state_list_next (gl_list_iterator_t *it, گەڕان_state **ss)
{
  ثابت فراغ *p = عدم;
  bool res = gl_list_iterator_next (it, &p, عدم);
  إذا (res)
    *ss = (گەڕان_state*) p;
  وإلا
    gl_list_iterator_free (it);
  أرجع res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ساكن مضمن فراغ
ss_set_parse_state (گەڕان_state *ss, صحيح idx, parse_state *ps)
{
  free_parse_state (ss->states[idx]);
  ss->states[idx] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
ساكن counterexample *
complete_diverging_examples (گەڕان_state *ss,
                             هێما_ژمار next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  لكل (صحيح i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  أرجع new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
تعريفنوع بنية
{
  گەڕان_state_list states;
  صحيح complexity;
} گەڕان_state_bundle;

ساكن فراغ
ssb_free (گەڕان_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  حرر (ssb);
}

ساكن size_t
ssb_hasher (گەڕان_state_bundle *ssb)
{
  أرجع ssb->complexity;
}

ساكن صحيح
ssb_comp (ثابت گەڕان_state_bundle *s1, ثابت گەڕان_state_bundle *s2)
{
  أرجع s1->complexity - s2->complexity;
}

ساكن bool
ssb_equals (ثابت گەڕان_state_bundle *s1, ثابت گەڕان_state_bundle *s2)
{
  أرجع s1->complexity == s2->complexity;
}

تعريفنوع gl_list_t ssb_list;

ساكن size_t
visited_hasher (ثابت گەڕان_state *ss, size_t max)
{
  أرجع (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

ساكن bool
visited_comparator (ثابت گەڕان_state *ss1, ثابت گەڕان_state *ss2)
{
  أرجع parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
ساكن ssb_list ssb_queue;
ساكن Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
ساكن bitset scp_set = عدم;
/* The set of parser states used for the conflict reduction rule. */
ساكن bitset rpp_set = عدم;

ساكن فراغ
ssb_append (گەڕان_state *ss)
{
  إذا (hash_lookup (visited, ss))
    {
      گەڕان_state_free (ss);
      أرجع;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  گەڕان_state_bundle *ssb = xmalloc (حجم *ssb);
  ssb->complexity = ss->complexity;
  gl_list_گرێ_t n = gl_list_گەڕان (ssb_queue, ssb);
  إذا (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, عدم, عدم,
                              (gl_listelement_dispose_fn)گەڕان_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  وإلا
    {
      حرر (ssb);
      ssb = (گەڕان_state_bundle *) gl_list_گرێ_بەها (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ساكن فراغ
production_step (گەڕان_state *ss, صحيح parser_state)
{
  ثابت state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  هێما_ژمار other_sym = item_ژمار_as_هێما_ژمار (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  صحيح complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      گەڕان_state *copy = copy_گەڕان_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ساكن مضمن صحيح
reduction_cost (ثابت parse_state *ps)
{
  صحيح shifts;
  صحيح productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  أرجع SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

ساكن گەڕان_state_list
reduction_step (گەڕان_state *ss, ثابت item_ژمار *conflict_item,
                صحيح parser_state, صحيح rule_len)
{
  (فراغ) conflict_item; // FIXME: Unused
  گەڕان_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, 1);

  parse_state *ps = ss->states[parser_state];
  ثابت state_item *si = parse_state_tail (ps);
  bitset هێما_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  ثابت state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  إذا (item_ژمار_is_هێما_ژمار (*other_si->item))
    {
      هێما_ژمار other_sym =
        item_ژمار_as_هێما_ژمار (*other_si->item);
      إذا (!intersect_هێما (other_sym, هێما_set))
        أرجع result;
      هێما_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (هێما_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, هێما_set);
  parse_state *reduced_ps = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      گەڕان_state *copy = copy_گەڕان_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      صحيح r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  إذا (هێما_set != si->lookahead)
    bitset_free (هێما_set);
  أرجع result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ساكن فراغ
گەڕان_state_prepend (گەڕان_state *ss, هێما_ژمار sym, bitset guide)
{
  (فراغ) sym; // FIXME: Unused.
  ثابت state_item *si1src = parse_state_head (ss->states[0]);
  ثابت state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  إذا (prod1 != SI_PRODUCTION (si2src))
    {
      صحيح prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = عدم;
      لكل (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          ثابت state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->ژمار);
          إذا (!guided && !EXTENDED_گەڕان)
            استمر;

          گەڕان_state *copy = copy_گەڕان_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          إذا (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      أرجع;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ثابت صحيح complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = عدم;
  لكل (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      ثابت state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->ژمار);
      إذا (!guided1 && !EXTENDED_گەڕان)
        استمر;

      parse_state *ps2 = عدم;
      لكل (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          ثابت state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->ژمار);
          إذا (!guided2 && !EXTENDED_گەڕان)
            استمر;
          // Only consider prepend state items that share the same state.
          إذا (psi1->state != psi2->state)
            استمر;

          صحيح complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          إذا (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_گەڕان_state (ps1, ps2, complexity));
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
have_common_prefix (ثابت item_ژمار *itm1, ثابت item_ژمار *itm2)
{
  صحيح i = 0;
  لكل (; !item_ژمار_is_rule_ژمار (itm1[i]); ++i)
    إذا (itm1[i] != itm2[i])
      أرجع false;
  أرجع item_ژمار_is_rule_ژمار (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ساكن ثابت item_ژمار *
item_rule_start (ثابت item_ژمار *item)
{
  ثابت item_ژمار *res = عدم;
  لكل (res = item;
       ritem < res && item_ژمار_is_هێما_ژمار (*(res - 1));
       --res)
    استمر;
  أرجع res;
}

ساكن ثابت item_ژمار *
item_rule_end (ثابت item_ژمار *item)
{
  ثابت item_ژمار *res = عدم;
  لكل (res = item; item_ژمار_is_هێما_ژمار (*res); ++res)
    استمر;
  أرجع res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ساكن مضمن فراغ
generate_next_states (گەڕان_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  ثابت state_item *si1 = parse_state_tail (ps1);
  ثابت state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_ژمار_is_rule_ژمار (*si1->item);
  bool si2reduce = item_ژمار_is_rule_ژمار (*si2->item);
  إذا (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      إذا (*si1->item == *si2->item)
        {
          صحيح complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = عدم;
          parse_state *tps2 = عدم;
          لكل (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            لكل (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_گەڕان_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  وإلا
    {
      ثابت item_ژمار *rhs1 = item_rule_start (si1->item);
      ثابت item_ژمار *rhe1 = item_rule_end (si1->item);
      صحيح len1 = rhe1 - rhs1;
      صحيح size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ثابت item_ژمار *rhs2 = item_rule_start (si2->item);
      ثابت item_ژمار *rhe2 = item_rule_end (si2->item);
      صحيح len2 = rhe2 - rhs2;
      صحيح size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      إذا (ready1 && ready2)
        {
          گەڕان_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          گەڕان_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               گەڕان_state_list_next (&iter, &red1);
               )
            {
              گەڕان_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              گەڕان_state *red2 = عدم;
              لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   گەڕان_state_list_next (&iter2, &red2);
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
          گەڕان_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          گەڕان_state *red1 = عدم;
          لكل (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               گەڕان_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      وإلا إذا (ready2)
        {
          گەڕان_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          گەڕان_state *red2 = عدم;
          لكل (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               گەڕان_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      وإلا
        {
          ثابت هێما_ژمار sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          گەڕان_state_prepend (ss, sym,
                                parse_state_depth (ss->states[0]) >= 0
                                ? rpp_set : scp_set);
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
unifying_example (state_item_ژمار itm1,
                  state_item_ژمار itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, هێما_ژمار next_sym)
{
  state_item *conflict1 = &state_کاڵاکان[itm1];
  state_item *conflict2 = &state_کاڵاکان[itm2];
  گەڕان_state *initial = initial_گەڕان_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, عدم, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) گەڕان_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  گەڕان_state *stage3result = عدم;
  counterexample *cex = عدم;
  طالما (gl_list_size (ssb_queue) > 0)
    {
      ثابت گەڕان_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      گەڕان_state *ss = عدم;
      لكل (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           گەڕان_state_list_next (&it, &ss);
           )
        {
          إذا (trace_flag & trace_cex)
            گەڕان_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          إذا (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ثابت state_item *si1src = parse_state_head (ps1);
              ثابت state_item *si2src = parse_state_head (ps2);
              إذا (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  إذا (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      اذهب cex_گەڕان_end;
                    }
                  إذا (!stage3result)
                    stage3result = copy_گەڕان_state (ss);
                }
            }
          إذا (TIME_LIMIT_ENFORCED)
            {
              مزدوج time_passed = (gethrxtime () - start) / 1e9;
              إذا (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              إذا (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  اذهب cex_گەڕان_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_گەڕان_end:;
  إذا (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      إذا (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      وإلا
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  إذا (stage3result)
    گەڕان_state_free (stage3result);
  أرجع cex;
}

ساكن xtime_t cumulative_time;

فراغ
counterexample_init (فراغ)
{
  // Check cex.timeout.
  {
    ثابت حرف *variable = "cex.timeout";
    حرف *cp = muscle_percent_define_get (variable);
    إذا (*cp != '\0')
      {
        حرف *end = عدم;
        مزدوج v = c_strtod (cp, &end);
        إذا (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
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
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_کاڵاکان_init ();
}


فراغ
counterexample_free (فراغ)
{
  إذا (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_کاڵاکان_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ساكن فراغ
counterexample_report (state_item_ژمار itm1, state_item_ژمار itm2,
                       هێما_ژمار next_sym, bool shift_reduce,
                       FILE *out, ثابت حرف *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  ثابت rule *reduce_rule = item_rule (state_کاڵاکان[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->ژمار);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      إذا (reduce_prod_reached)
        bitset_set (rpp_set, si->state->ژمار);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_time) / 1e9 < CUMULATIVE_TIME_LIMIT
    ? unifying_example (itm1, itm2, shift_reduce, shortest_path, next_sym)
    : example_from_path (shift_reduce, itm2, shortest_path, next_sym);

  gl_list_free (shortest_path);
  counterexample_print (cex, out, prefix);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
ساكن فراغ
counterexample_report_shift_reduce (state_item_ژمار itm1, state_item_ژمار itm2,
                                    هێما_ژمار next_sym,
                                    FILE *out, ثابت حرف *prefix)
{
  إذا (out == stderr)
    complain (عدم, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  وإلا
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  إذا (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_کاڵاکان[itm1], out, prefix);
      state_item_print (&state_کاڵاکان[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

ساكن فراغ
counterexample_report_reduce_reduce (state_item_ژمار itm1, state_item_ژمار itm2,
                                     bitset conflict_syms,
                                     FILE *out, ثابت حرف *prefix)
{
  {
    بنية obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_ژمار sym;
    ثابت حرف *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    حرف *tokens = obstack_finish0 (&obstack);
    إذا (out == stderr)
      complain (عدم, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_ژماردن (conflict_syms)),
                tokens);
    وإلا
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_ژماردن (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, عدم);
  }
  // In the report, print the items.
  إذا (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_کاڵاکان[itm1], out, prefix);
      state_item_print (&state_کاڵاکان[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

ساكن state_item_ژمار
find_state_item_ژمار (ثابت rule *r, state_ژمار sn)
{
  لكل (state_item_ژمار i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    إذا (!SI_DISABLED (i)
        && item_ژمار_as_rule_ژمار (*state_کاڵاکان[i].item) == r->ژمار)
      أرجع i;
  abort ();
}

فراغ
counterexample_report_state (ثابت state *s, FILE *out, ثابت حرف *prefix)
{
  ثابت state_ژمار sn = s->ژمار;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  لكل (صحيح i = 0; i < reds->ژمارە; ++i)
    {
      ثابت rule *r1 = reds->rules[i];
      ثابت state_item_ژمار c1 = find_state_item_ژمار (r1, sn);
      لكل (state_item_ژمار c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        إذا (!SI_DISABLED (c2))
          {
            item_ژمار conf = *state_کاڵاکان[c2].item;
            إذا (item_ژمار_is_هێما_ژمار (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      لكل (صحيح j = i+1; j < reds->ژمارە; ++j)
        {
          ثابت rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          إذا (!bitset_empty_p (lookaheads))
            لكل (state_item_ژمار c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              إذا (!SI_DISABLED (c2)
                  && item_rule (state_کاڵاکان[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  اكسر;
                }
        }
    }
  bitset_free (lookaheads);
}
