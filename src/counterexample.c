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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "counterexample.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <c-strtod.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <gethrxtime.h>
#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <gl_rbtreehash_list.h>
#ಅನ್ತರ್ಭಾವ <hash.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <textstyle.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "derivation.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lssi.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_ಹುಡುಕು false

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
ಸ್ಥಿರ ದ್ವಿಗುಣ time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
ಸ್ಥಿರ gl_list_ಗಂಟು_t
list_get_end (gl_list_t list)
{
  gl_list_ಗಂಟು_t sentinel = gl_list_add_last (list, ರಿಕ್ತ);
  gl_list_ಗಂಟು_t res = gl_list_previous_ಗಂಟು (list, sentinel);
  gl_list_remove_ಗಂಟು (list, sentinel);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ಸ್ಥಿರ counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (ಪರಿಮಾಣ *res);
  res->shift_reduce = shift_reduce;
  ಯದಿ (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  ಅನ್ಯಥಾ
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  ಮುಕ್ತ (cex);
}

ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_print (ಧ್ರುವ counterexample *cex, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  ಧ್ರುವ bool flat = getenv ("YYFLAT");
  ಧ್ರುವ ವರ್ಣ *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ಧ್ರುವ ವರ್ಣ *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ಧ್ರುವ ವರ್ಣ *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  ಧ್ರುವ ವರ್ಣ *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  ಯದಿ (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  ಅನ್ಯಥಾ
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  ಯದಿ (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  ಅನ್ಯಥಾ
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  ಯದಿ (!cex->unifying || is_styled (stderr))
    {
      ಯದಿ (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      ಅನ್ಯಥಾ
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  ಯದಿ (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  ಅನ್ಯಥಾ
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  ಯದಿ (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
ಸಂರಚನಾ si_bfs_ಗಂಟು;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ si_bfs_ಗಂಟು
{
  state_item_ಸಂಖ್ಯೆ si;
  ಸಂರಚನಾ si_bfs_ಗಂಟು *parent;
  ಪೂರ್ಣಾಙ್ಕ reference_ಎಣಿಕೆ;
} si_bfs_ಗಂಟು;

ಸ್ಥಿರ si_bfs_ಗಂಟು *
si_bfs_new (state_item_ಸಂಖ್ಯೆ si, si_bfs_ಗಂಟು *parent)
{
  si_bfs_ಗಂಟು *res = xcalloc (1, ಪರಿಮಾಣ *res);
  res->si = si;
  res->parent = parent;
  res->reference_ಎಣಿಕೆ = 1;
  ಯದಿ (parent)
    ++parent->reference_ಎಣಿಕೆ;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ bool
si_bfs_contains (ಧ್ರುವ si_bfs_ಗಂಟು *n, state_item_ಸಂಖ್ಯೆ sin)
{
  ಪ್ರತಿ (ಧ್ರುವ si_bfs_ಗಂಟು *ಹುಡುಕು = n; ಹುಡುಕು != ರಿಕ್ತ; ಹುಡುಕು = ಹುಡುಕು->parent)
    ಯದಿ (ಹುಡುಕು->si == sin)
      ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
si_bfs_free (si_bfs_ಗಂಟು *n)
{
  ಯದಿ (n == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  --n->reference_ಎಣಿಕೆ;
  ಯದಿ (n->reference_ಎಣಿಕೆ == 0)
    {
      si_bfs_free (n->parent);
      ಮುಕ್ತ (n);
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ gl_list_t si_bfs_ಗಂಟು_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ derivation_list
expand_to_conflict (state_item_ಸಂಖ್ಯೆ start, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ conflict_sym)
{
  si_bfs_ಗಂಟು *init = si_bfs_new (start, ರಿಕ್ತ);

  si_bfs_ಗಂಟು_list queue
    = gl_list_create (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ಧ್ರುವ ಶೂನ್ಯ **) &init);
  si_bfs_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;
  // breadth-first search for a path of productions to the conflict symbol
  ಯಾವತ್ (gl_list_size (queue) > 0)
    {
      ಗಂಟು = (si_bfs_ಗಂಟು *) gl_list_get_at (queue, 0);
      state_item *silast = &state_ವಸ್ತುಗಳು[ಗಂಟು->si];
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*silast->item);
      ಯದಿ (sym == conflict_sym)
        ಭಙ್ಗ;
      ಯದಿ (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_ಸಂಖ್ಯೆ sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              ಯದಿ (si_bfs_contains (ಗಂಟು, sin))
                ಅನುವರ್ತ;
              si_bfs_ಗಂಟು *next = si_bfs_new (sin, ಗಂಟು);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          ಯದಿ (nullable[sym - ntokens])
            {
              si_bfs_ಗಂಟು *next = si_bfs_new (silast->trans, ಗಂಟು);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  ಯದಿ (gl_list_size (queue) == 0)
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

  ಪ್ರತಿ (si_bfs_ಗಂಟು *n = ಗಂಟು; n != ರಿಕ್ತ; n = n->parent)
    {
      state_item *si = &state_ವಸ್ತುಗಳು[n->si];
      item_ಸಂಖ್ಯೆ *pos = si->item;
      ಯದಿ (SI_PRODUCTION (si))
        {
          item_ಸಂಖ್ಯೆ *i = ರಿಕ್ತ;
          ಪ್ರತಿ (i = pos + 1; !item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs =
            rules[item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (*i)].lhs->ಸಂಖ್ಯೆ;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      ಅನ್ಯಥಾ
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  ಪ್ರತ್ಯಾಗಮ result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
ಸ್ಥಿರ derivation *
complete_diverging_example (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  ಯದಿ (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ಗಂಟು_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  ಪ್ರತಿ (gl_list_ಗಂಟು_t state_ಗಂಟು = list_get_end (path);
       state_ಗಂಟು != ರಿಕ್ತ;
       state_ಗಂಟು = gl_list_previous_ಗಂಟು (path, state_ಗಂಟು))
    {
      state_item *si = (state_item *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (path, state_ಗಂಟು);
      item_ಸಂಖ್ಯೆ *item = si->item;
      item_ಸಂಖ್ಯೆ pos = *item;
      // symbols after dot
      ಯದಿ (gl_list_size (result) == 1 && !item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (pos)));
          lookahead_required = false;
        }
      item_ಸಂಖ್ಯೆ *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ nsi = si - state_ವಸ್ತುಗಳು;
           !item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*i);
           ++i, nsi = state_ವಸ್ತುಗಳು[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          ಯದಿ (i == item)
            ಅನುವರ್ತ;
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*i);
          ಯದಿ (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              ಅನುವರ್ತ;
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
          ಯದಿ (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = ರಿಕ್ತ;
              ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          ಅನ್ಯಥಾ ಯದಿ (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          ಅನ್ಯಥಾ
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      ಧ್ರುವ rule *r = &rules[item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (*i)];
      // add derivations for symbols before dot
      ಪ್ರತಿ (i = item - 1; !item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*i) && i >= ritem; i--)
        {
          gl_list_ಗಂಟು_t p = gl_list_previous_ಗಂಟು (path, state_ಗಂಟು);
          ಯದಿ (p)
            state_ಗಂಟು = p;
          ಯದಿ (deriv)
            {
              ಧ್ರುವ ಶೂನ್ಯ *tmp_deriv = gl_list_ಗಂಟು_ಮೌಲ್ಯ (derivs, deriv);
              deriv = gl_list_previous_ಗಂಟು (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          ಅನ್ಯಥಾ
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->ಸಂಖ್ಯೆ, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ಸ್ಥಿರ state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_ಗಂಟು_t tmp = gl_list_add_last (reduce_path, ರಿಕ್ತ);
  gl_list_ಗಂಟು_t next_ಗಂಟು = gl_list_previous_ಗಂಟು (reduce_path, tmp);
  gl_list_ಗಂಟು_t ಗಂಟು = gl_list_previous_ಗಂಟು (reduce_path, next_ಗಂಟು);
  gl_list_remove_ಗಂಟು (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
  // FIXME: bool paths_merged;
  ಪ್ರತಿ (; ಗಂಟು != ರಿಕ್ತ; next_ಗಂಟು = ಗಂಟು,
       ಗಂಟು = gl_list_previous_ಗಂಟು (reduce_path, ಗಂಟು))
    {
      state_item *refsi =
        (state_item *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (reduce_path, ಗಂಟು);
      state_item *nextrefsi =
        (state_item *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (reduce_path, next_ಗಂಟು);
      ಯದಿ (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          ಅನುವರ್ತ;
        }
      // skip reduction items
      ಯದಿ (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        ಅನುವರ್ತ;

      // bfs to find a shift to the right state
      si_bfs_ಗಂಟು *init = si_bfs_new (si - state_ವಸ್ತುಗಳು, ರಿಕ್ತ);
      si_bfs_ಗಂಟು_list queue
        = gl_list_create (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ಧ್ರುವ ಶೂನ್ಯ **) &init);
      si_bfs_ಗಂಟು *sis = ರಿಕ್ತ;
      state_item *prevsi = ರಿಕ್ತ;
      ಯಾವತ್ (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_ಗಂಟು *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          ಯದಿ (sis->si == 0)
            ಭಙ್ಗ;

          state_item *ಹುಡುಕು_si = &state_ವಸ್ತುಗಳು[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = ಹುಡುಕು_si->revs;
          bitset_iterator biter;
          state_item_ಸಂಖ್ಯೆ sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_ವಸ್ತುಗಳು[sin];
              ಯದಿ (SI_TRANSITION (ಹುಡುಕು_si))
                {
                  ಯದಿ (prevsi->state == refsi->state)
                    ಗಚ್ಛ ಹುಡುಕು_end;
                }
              ಅನ್ಯಥಾ ಯದಿ (!si_bfs_contains (sis, sin))
                {
                  si_bfs_ಗಂಟು *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    ಹುಡುಕು_end:
      // prepend path to shift we found
      ಯದಿ (sis)
        {
          gl_list_ಗಂಟು_t ln = gl_list_add_first (result, &state_ವಸ್ತುಗಳು[sis->si]);
          ಪ್ರತಿ (si_bfs_ಗಂಟು *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_ವಸ್ತುಗಳು[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  ಯದಿ (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = ರಿಕ್ತ;
      ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  ಪ್ರತ್ಯಾಗಮ result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ಸ್ಥಿರ counterexample *
example_from_path (bool shift_reduce,
                   state_item_ಸಂಖ್ಯೆ itm2,
                   state_item_list shortest_path, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, ರಿಕ್ತ);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_ವಸ್ತುಗಳು [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, ರಿಕ್ತ);
  gl_list_free (path_2);
  ಪ್ರತ್ಯಾಗಮ new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  parse_state *states[2];
  ಪೂರ್ಣಾಙ್ಕ complexity;
} ಹುಡುಕು_state;

ಸ್ಥಿರ ಹುಡುಕು_state *
initial_ಹುಡುಕು_state (state_item *conflict1, state_item *conflict2)
{
  ಹುಡುಕು_state *res = xmalloc (ಪರಿಮಾಣ *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಹುಡುಕು_state *
new_ಹುಡುಕು_state (parse_state *ps1, parse_state *ps2, ಪೂರ್ಣಾಙ್ಕ complexity)
{
  ಹುಡುಕು_state *res = xmalloc (ಪರಿಮಾಣ *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಹುಡುಕು_state *
copy_ಹುಡುಕು_state (ಹುಡುಕು_state *parent)
{
  ಹುಡುಕು_state *res = xmalloc (ಪರಿಮಾಣ *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_free_children (ಹುಡುಕು_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_free (ಹುಡುಕು_state *ss)
{
  ಯದಿ (ss == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  ಹುಡುಕು_state_free_children (ss);
  ಮುಕ್ತ (ss);
}

/* For debugging traces.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_print (ಹುಡುಕು_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

ಪ್ರಕಾರನಿರ್ದೇಶ gl_list_t ಹುಡುಕು_state_list;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ಹುಡುಕು_state_list_next (gl_list_iterator_t *it, ಹುಡುಕು_state **ss)
{
  ಧ್ರುವ ಶೂನ್ಯ *p = ರಿಕ್ತ;
  bool res = gl_list_iterator_next (it, &p, ರಿಕ್ತ);
  ಯದಿ (res)
    *ss = (ಹುಡುಕು_state*) p;
  ಅನ್ಯಥಾ
    gl_list_iterator_free (it);
  ಪ್ರತ್ಯಾಗಮ res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
ss_set_parse_state (ಹುಡುಕು_state *ss, ಪೂರ್ಣಾಙ್ಕ idx, parse_state *ps)
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
ಸ್ಥಿರ counterexample *
complete_diverging_examples (ಹುಡುಕು_state *ss,
                             ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  ಪ್ರತ್ಯಾಗಮ new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  ಹುಡುಕು_state_list states;
  ಪೂರ್ಣಾಙ್ಕ complexity;
} ಹುಡುಕು_state_bundle;

ಸ್ಥಿರ ಶೂನ್ಯ
ssb_free (ಹುಡುಕು_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  ಮುಕ್ತ (ssb);
}

ಸ್ಥಿರ size_t
ssb_hasher (ಹುಡುಕು_state_bundle *ssb)
{
  ಪ್ರತ್ಯಾಗಮ ssb->complexity;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ssb_comp (ಧ್ರುವ ಹುಡುಕು_state_bundle *s1, ಧ್ರುವ ಹುಡುಕು_state_bundle *s2)
{
  ಪ್ರತ್ಯಾಗಮ s1->complexity - s2->complexity;
}

ಸ್ಥಿರ bool
ssb_equals (ಧ್ರುವ ಹುಡುಕು_state_bundle *s1, ಧ್ರುವ ಹುಡುಕು_state_bundle *s2)
{
  ಪ್ರತ್ಯಾಗಮ s1->complexity == s2->complexity;
}

ಪ್ರಕಾರನಿರ್ದೇಶ gl_list_t ssb_list;

ಸ್ಥಿರ size_t
visited_hasher (ಧ್ರುವ ಹುಡುಕು_state *ss, size_t max)
{
  ಪ್ರತ್ಯಾಗಮ (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

ಸ್ಥಿರ bool
visited_comparator (ಧ್ರುವ ಹುಡುಕು_state *ss1, ಧ್ರುವ ಹುಡುಕು_state *ss2)
{
  ಪ್ರತ್ಯಾಗಮ parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
ಸ್ಥಿರ ssb_list ssb_queue;
ಸ್ಥಿರ Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
ಸ್ಥಿರ bitset scp_set = ರಿಕ್ತ;
/* The set of parser states used for the conflict reduction rule. */
ಸ್ಥಿರ bitset rpp_set = ರಿಕ್ತ;

ಸ್ಥಿರ ಶೂನ್ಯ
ssb_append (ಹುಡುಕು_state *ss)
{
  ಯದಿ (hash_lookup (visited, ss))
    {
      ಹುಡುಕು_state_free (ss);
      ಪ್ರತ್ಯಾಗಮ;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  ಹುಡುಕು_state_bundle *ssb = xmalloc (ಪರಿಮಾಣ *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ಗಂಟು_t n = gl_list_ಹುಡುಕು (ssb_queue, ssb);
  ಯದಿ (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ,
                              (gl_listelement_dispose_fn)ಹುಡುಕು_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  ಅನ್ಯಥಾ
    {
      ಮುಕ್ತ (ssb);
      ssb = (ಹುಡುಕು_state_bundle *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
production_step (ಹುಡುಕು_state *ss, ಪೂರ್ಣಾಙ್ಕ parser_state)
{
  ಧ್ರುವ state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ other_sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  ಪೂರ್ಣಾಙ್ಕ complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      ಹುಡುಕು_state *copy = copy_ಹುಡುಕು_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
reduction_cost (ಧ್ರುವ parse_state *ps)
{
  ಪೂರ್ಣಾಙ್ಕ shifts;
  ಪೂರ್ಣಾಙ್ಕ productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  ಪ್ರತ್ಯಾಗಮ SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

ಸ್ಥಿರ ಹುಡುಕು_state_list
reduction_step (ಹುಡುಕು_state *ss, ಧ್ರುವ item_ಸಂಖ್ಯೆ *conflict_item,
                ಪೂರ್ಣಾಙ್ಕ parser_state, ಪೂರ್ಣಾಙ್ಕ rule_len)
{
  (ಶೂನ್ಯ) conflict_item; // FIXME: Unused
  ಹುಡುಕು_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, 1);

  parse_state *ps = ss->states[parser_state];
  ಧ್ರುವ state_item *si = parse_state_tail (ps);
  bitset ಚಿಹ್ನೆ_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  ಧ್ರುವ state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  ಯದಿ (item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*other_si->item))
    {
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ other_sym =
        item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*other_si->item);
      ಯದಿ (!intersect_ಚಿಹ್ನೆ (other_sym, ಚಿಹ್ನೆ_set))
        ಪ್ರತ್ಯಾಗಮ result;
      ಚಿಹ್ನೆ_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (ಚಿಹ್ನೆ_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, ಚಿಹ್ನೆ_set);
  parse_state *reduced_ps = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      ಹುಡುಕು_state *copy = copy_ಹುಡುಕು_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      ಪೂರ್ಣಾಙ್ಕ r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  ಯದಿ (ಚಿಹ್ನೆ_set != si->lookahead)
    bitset_free (ಚಿಹ್ನೆ_set);
  ಪ್ರತ್ಯಾಗಮ result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_prepend (ಹುಡುಕು_state *ss, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, bitset guide)
{
  (ಶೂನ್ಯ) sym; // FIXME: Unused.
  ಧ್ರುವ state_item *si1src = parse_state_head (ss->states[0]);
  ಧ್ರುವ state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  ಯದಿ (prod1 != SI_PRODUCTION (si2src))
    {
      ಪೂರ್ಣಾಙ್ಕ prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = ರಿಕ್ತ;
      ಪ್ರತಿ (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          ಧ್ರುವ state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->ಸಂಖ್ಯೆ);
          ಯದಿ (!guided && !EXTENDED_ಹುಡುಕು)
            ಅನುವರ್ತ;

          ಹುಡುಕು_state *copy = copy_ಹುಡುಕು_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          ಯದಿ (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      ಪ್ರತ್ಯಾಗಮ;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      ಧ್ರುವ state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->ಸಂಖ್ಯೆ);
      ಯದಿ (!guided1 && !EXTENDED_ಹುಡುಕು)
        ಅನುವರ್ತ;

      parse_state *ps2 = ರಿಕ್ತ;
      ಪ್ರತಿ (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          ಧ್ರುವ state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->ಸಂಖ್ಯೆ);
          ಯದಿ (!guided2 && !EXTENDED_ಹುಡುಕು)
            ಅನುವರ್ತ;
          // Only consider prepend state items that share the same state.
          ಯದಿ (psi1->state != psi2->state)
            ಅನುವರ್ತ;

          ಪೂರ್ಣಾಙ್ಕ complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          ಯದಿ (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_ಹುಡುಕು_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
ಸ್ಥಿರ bool
have_common_prefix (ಧ್ರುವ item_ಸಂಖ್ಯೆ *itm1, ಧ್ರುವ item_ಸಂಖ್ಯೆ *itm2)
{
  ಪೂರ್ಣಾಙ್ಕ i = 0;
  ಪ್ರತಿ (; !item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (itm1[i]); ++i)
    ಯದಿ (itm1[i] != itm2[i])
      ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತ್ಯಾಗಮ item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ಸ್ಥಿರ ಧ್ರುವ item_ಸಂಖ್ಯೆ *
item_rule_start (ಧ್ರುವ item_ಸಂಖ್ಯೆ *item)
{
  ಧ್ರುವ item_ಸಂಖ್ಯೆ *res = ರಿಕ್ತ;
  ಪ್ರತಿ (res = item;
       ritem < res && item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(res - 1));
       --res)
    ಅನುವರ್ತ;
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಧ್ರುವ item_ಸಂಖ್ಯೆ *
item_rule_end (ಧ್ರುವ item_ಸಂಖ್ಯೆ *item)
{
  ಧ್ರುವ item_ಸಂಖ್ಯೆ *res = ರಿಕ್ತ;
  ಪ್ರತಿ (res = item; item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*res); ++res)
    ಅನುವರ್ತ;
  ಪ್ರತ್ಯಾಗಮ res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
generate_next_states (ಹುಡುಕು_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  ಧ್ರುವ state_item *si1 = parse_state_tail (ps1);
  ಧ್ರುವ state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*si1->item);
  bool si2reduce = item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*si2->item);
  ಯದಿ (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      ಯದಿ (*si1->item == *si2->item)
        {
          ಪೂರ್ಣಾಙ್ಕ complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = ರಿಕ್ತ;
          parse_state *tps2 = ರಿಕ್ತ;
          ಪ್ರತಿ (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            ಪ್ರತಿ (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_ಹುಡುಕು_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  ಅನ್ಯಥಾ
    {
      ಧ್ರುವ item_ಸಂಖ್ಯೆ *rhs1 = item_rule_start (si1->item);
      ಧ್ರುವ item_ಸಂಖ್ಯೆ *rhe1 = item_rule_end (si1->item);
      ಪೂರ್ಣಾಙ್ಕ len1 = rhe1 - rhs1;
      ಪೂರ್ಣಾಙ್ಕ size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ಧ್ರುವ item_ಸಂಖ್ಯೆ *rhs2 = item_rule_start (si2->item);
      ಧ್ರುವ item_ಸಂಖ್ಯೆ *rhe2 = item_rule_end (si2->item);
      ಪೂರ್ಣಾಙ್ಕ len2 = rhe2 - rhs2;
      ಪೂರ್ಣಾಙ್ಕ size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      ಯದಿ (ready1 && ready2)
        {
          ಹುಡುಕು_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          ಹುಡುಕು_state *red1 = ರಿಕ್ತ;
          ಪ್ರತಿ (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ಹುಡುಕು_state_list_next (&iter, &red1);
               )
            {
              ಹುಡುಕು_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              ಹುಡುಕು_state *red2 = ರಿಕ್ತ;
              ಪ್ರತಿ (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   ಹುಡುಕು_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              ಯದಿ (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      ಅನ್ಯಥಾ ಯದಿ (ready1)
        {
          ಹುಡುಕು_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          ಹುಡುಕು_state *red1 = ರಿಕ್ತ;
          ಪ್ರತಿ (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ಹುಡುಕು_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      ಅನ್ಯಥಾ ಯದಿ (ready2)
        {
          ಹುಡುಕು_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          ಹುಡುಕು_state *red2 = ರಿಕ್ತ;
          ಪ್ರತಿ (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               ಹುಡುಕು_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      ಅನ್ಯಥಾ
        {
          ಧ್ರುವ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          ಹುಡುಕು_state_prepend (ss, sym,
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
ಸ್ಥಿರ counterexample *
unifying_example (state_item_ಸಂಖ್ಯೆ itm1,
                  state_item_ಸಂಖ್ಯೆ itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym)
{
  state_item *conflict1 = &state_ವಸ್ತುಗಳು[itm1];
  state_item *conflict2 = &state_ವಸ್ತುಗಳು[itm2];
  ಹುಡುಕು_state *initial = initial_ಹುಡುಕು_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, ರಿಕ್ತ, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) ಹುಡುಕು_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  ಹುಡುಕು_state *stage3result = ರಿಕ್ತ;
  counterexample *cex = ರಿಕ್ತ;
  ಯಾವತ್ (gl_list_size (ssb_queue) > 0)
    {
      ಧ್ರುವ ಹುಡುಕು_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      ಹುಡುಕು_state *ss = ರಿಕ್ತ;
      ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           ಹುಡುಕು_state_list_next (&it, &ss);
           )
        {
          ಯದಿ (trace_flag & trace_cex)
            ಹುಡುಕು_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          ಯದಿ (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ಧ್ರುವ state_item *si1src = parse_state_head (ps1);
              ಧ್ರುವ state_item *si2src = parse_state_head (ps2);
              ಯದಿ (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  ಯದಿ (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      ಗಚ್ಛ cex_ಹುಡುಕು_end;
                    }
                  ಯದಿ (!stage3result)
                    stage3result = copy_ಹುಡುಕು_state (ss);
                }
            }
          ಯದಿ (TIME_LIMIT_ENFORCED)
            {
              ದ್ವಿಗುಣ time_passed = (gethrxtime () - start) / 1e9;
              ಯದಿ (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              ಯದಿ (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  ಗಚ್ಛ cex_ಹುಡುಕು_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_ಹುಡುಕು_end:;
  ಯದಿ (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      ಯದಿ (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      ಅನ್ಯಥಾ
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  ಯದಿ (stage3result)
    ಹುಡುಕು_state_free (stage3result);
  ಪ್ರತ್ಯಾಗಮ cex;
}

ಸ್ಥಿರ xtime_t cumulative_time;

ಶೂನ್ಯ
counterexample_init (ಶೂನ್ಯ)
{
  // Check cex.timeout.
  {
    ಧ್ರುವ ವರ್ಣ *variable = "cex.timeout";
    ವರ್ಣ *cp = muscle_percent_define_get (variable);
    ಯದಿ (*cp != '\0')
      {
        ವರ್ಣ *end = ರಿಕ್ತ;
        ದ್ವಿಗುಣ v = c_strtod (cp, &end);
        ಯದಿ (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        ಅನ್ಯಥಾ
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    ಮುಕ್ತ (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_ವಸ್ತುಗಳು_init ();
}


ಶೂನ್ಯ
counterexample_free (ಶೂನ್ಯ)
{
  ಯದಿ (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_ವಸ್ತುಗಳು_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_report (state_item_ಸಂಖ್ಯೆ itm1, state_item_ಸಂಖ್ಯೆ itm2,
                       ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym, bool shift_reduce,
                       FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  ಧ್ರುವ rule *reduce_rule = item_rule (state_ವಸ್ತುಗಳು[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->ಸಂಖ್ಯೆ);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      ಯದಿ (reduce_prod_reached)
        bitset_set (rpp_set, si->state->ಸಂಖ್ಯೆ);
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
ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_report_shift_reduce (state_item_ಸಂಖ್ಯೆ itm1, state_item_ಸಂಖ್ಯೆ itm2,
                                    ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ next_sym,
                                    FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  ಯದಿ (out == stderr)
    complain (ರಿಕ್ತ, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  ಅನ್ಯಥಾ
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  ಯದಿ (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_ವಸ್ತುಗಳು[itm1], out, prefix);
      state_item_print (&state_ವಸ್ತುಗಳು[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_report_reduce_reduce (state_item_ಸಂಖ್ಯೆ itm1, state_item_ಸಂಖ್ಯೆ itm2,
                                     bitset conflict_syms,
                                     FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  {
    ಸಂರಚನಾ obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_ಸಂಖ್ಯೆ sym;
    ಧ್ರುವ ವರ್ಣ *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    ವರ್ಣ *tokens = obstack_finish0 (&obstack);
    ಯದಿ (out == stderr)
      complain (ರಿಕ್ತ, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_ಎಣಿಕೆ (conflict_syms)),
                tokens);
    ಅನ್ಯಥಾ
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_ಎಣಿಕೆ (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, ರಿಕ್ತ);
  }
  // In the report, print the items.
  ಯದಿ (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_ವಸ್ತುಗಳು[itm1], out, prefix);
      state_item_print (&state_ವಸ್ತುಗಳು[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

ಸ್ಥಿರ state_item_ಸಂಖ್ಯೆ
find_state_item_ಸಂಖ್ಯೆ (ಧ್ರುವ rule *r, state_ಸಂಖ್ಯೆ sn)
{
  ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    ಯದಿ (!SI_DISABLED (i)
        && item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (*state_ವಸ್ತುಗಳು[i].item) == r->ಸಂಖ್ಯೆ)
      ಪ್ರತ್ಯಾಗಮ i;
  abort ();
}

ಶೂನ್ಯ
counterexample_report_state (ಧ್ರುವ state *s, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix)
{
  ಧ್ರುವ state_ಸಂಖ್ಯೆ sn = s->ಸಂಖ್ಯೆ;
  ಧ್ರುವ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    {
      ಧ್ರುವ rule *r1 = reds->rules[i];
      ಧ್ರುವ state_item_ಸಂಖ್ಯೆ c1 = find_state_item_ಸಂಖ್ಯೆ (r1, sn);
      ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        ಯದಿ (!SI_DISABLED (c2))
          {
            item_ಸಂಖ್ಯೆ conf = *state_ವಸ್ತುಗಳು[c2].item;
            ಯದಿ (item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = i+1; j < reds->ಅಂಕೆ; ++j)
        {
          ಧ್ರುವ rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          ಯದಿ (!bitset_empty_p (lookaheads))
            ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              ಯದಿ (!SI_DISABLED (c2)
                  && item_rule (state_ವಸ್ತುಗಳು[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  ಭಙ್ಗ;
                }
        }
    }
  bitset_free (lookaheads);
}
