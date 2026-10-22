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

#अन्तर्भाव <config.h>

#अन्तर्भाव "counterexample.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <c-strtod.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <gethrxtime.h>
#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <gl_rbtreehash_list.h>
#अन्तर्भाव <hash.h>
#अन्तर्भाव <mbswidth.h>
#अन्तर्भाव <quote.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <textstyle.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "complain.h"
#अन्तर्भाव "derivation.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lssi.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "nullable.h"
#अन्तर्भाव "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_अन्वेषणम् false

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
स्थिर द्विगुण time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
स्थिर gl_list_ग्रन्थिः_t
list_get_end (gl_list_t list)
{
  gl_list_ग्रन्थिः_t sentinel = gl_list_add_last (list, रिक्त);
  gl_list_ग्रन्थिः_t res = gl_list_previous_ग्रन्थिः (list, sentinel);
  gl_list_remove_ग्रन्थिः (list, sentinel);
  पटिनिवत्त res;
}

प्रकारनिर्देश संरचना
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

स्थिर counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (परिमाण *res);
  res->shift_reduce = shift_reduce;
  सचे (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  अञ्ञथा
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  पटिनिवत्त res;
}

स्थिर शून्य
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  मुक्त (cex);
}

स्थिर शून्य
counterexample_print (ध्रुव counterexample *cex, FILE *out, ध्रुव वर्ण *prefix)
{
  ध्रुव bool flat = getenv ("YYFLAT");
  ध्रुव वर्ण *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ध्रुव वर्ण *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ध्रुव वर्ण *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  ध्रुव वर्ण *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ध्रुव पूर्णाङ्क width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  सचे (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  अञ्ञथा
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  सचे (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  अञ्ञथा
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  सचे (!cex->unifying || is_styled (stderr))
    {
      सचे (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      अञ्ञथा
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  सचे (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  अञ्ञथा
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  सचे (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
संरचना si_bfs_ग्रन्थिः;
प्रकारनिर्देश संरचना si_bfs_ग्रन्थिः
{
  state_item_सङ्ख्या si;
  संरचना si_bfs_ग्रन्थिः *parent;
  पूर्णाङ्क reference_गणनम्;
} si_bfs_ग्रन्थिः;

स्थिर si_bfs_ग्रन्थिः *
si_bfs_new (state_item_सङ्ख्या si, si_bfs_ग्रन्थिः *parent)
{
  si_bfs_ग्रन्थिः *res = xcalloc (1, परिमाण *res);
  res->si = si;
  res->parent = parent;
  res->reference_गणनम् = 1;
  सचे (parent)
    ++parent->reference_गणनम्;
  पटिनिवत्त res;
}

स्थिर bool
si_bfs_contains (ध्रुव si_bfs_ग्रन्थिः *n, state_item_सङ्ख्या sin)
{
  अत्थाय (ध्रुव si_bfs_ग्रन्थिः *अन्वेषणम् = n; अन्वेषणम् != रिक्त; अन्वेषणम् = अन्वेषणम्->parent)
    सचे (अन्वेषणम्->si == sin)
      पटिनिवत्त true;
  पटिनिवत्त false;
}

स्थिर शून्य
si_bfs_free (si_bfs_ग्रन्थिः *n)
{
  सचे (n == रिक्त)
    पटिनिवत्त;
  --n->reference_गणनम्;
  सचे (n->reference_गणनम् == 0)
    {
      si_bfs_free (n->parent);
      मुक्त (n);
    }
}

प्रकारनिर्देश gl_list_t si_bfs_ग्रन्थिः_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
स्थिर अन्तःस्थ derivation_list
expand_to_conflict (state_item_सङ्ख्या start, चिह्नम्_सङ्ख्या conflict_sym)
{
  si_bfs_ग्रन्थिः *init = si_bfs_new (start, रिक्त);

  si_bfs_ग्रन्थिः_list queue
    = gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ध्रुव शून्य **) &init);
  si_bfs_ग्रन्थिः *ग्रन्थिः = रिक्त;
  // breadth-first search for a path of productions to the conflict symbol
  याव (gl_list_size (queue) > 0)
    {
      ग्रन्थिः = (si_bfs_ग्रन्थिः *) gl_list_get_at (queue, 0);
      state_item *silast = &state_वस्तूनि[ग्रन्थिः->si];
      चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*silast->item);
      सचे (sym == conflict_sym)
        भङ्ग;
      सचे (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_सङ्ख्या sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              सचे (si_bfs_contains (ग्रन्थिः, sin))
                अनुवर्त;
              si_bfs_ग्रन्थिः *next = si_bfs_new (sin, ग्रन्थिः);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          सचे (nullable[sym - ntokens])
            {
              si_bfs_ग्रन्थिः *next = si_bfs_new (silast->trans, ग्रन्थिः);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  सचे (gl_list_size (queue) == 0)
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

  अत्थाय (si_bfs_ग्रन्थिः *n = ग्रन्थिः; n != रिक्त; n = n->parent)
    {
      state_item *si = &state_वस्तूनि[n->si];
      item_सङ्ख्या *pos = si->item;
      सचे (SI_PRODUCTION (si))
        {
          item_सङ्ख्या *i = रिक्त;
          अत्थाय (i = pos + 1; !item_सङ्ख्या_is_rule_सङ्ख्या (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          चिह्नम्_सङ्ख्या lhs =
            rules[item_सङ्ख्या_as_rule_सङ्ख्या (*i)].lhs->सङ्ख्या;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      अञ्ञथा
        {
          चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  पटिनिवत्त result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
स्थिर derivation *
complete_diverging_example (चिह्नम्_सङ्ख्या conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  सचे (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ग्रन्थिः_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  अत्थाय (gl_list_ग्रन्थिः_t state_ग्रन्थिः = list_get_end (path);
       state_ग्रन्थिः != रिक्त;
       state_ग्रन्थिः = gl_list_previous_ग्रन्थिः (path, state_ग्रन्थिः))
    {
      state_item *si = (state_item *) gl_list_ग्रन्थिः_मानम् (path, state_ग्रन्थिः);
      item_सङ्ख्या *item = si->item;
      item_सङ्ख्या pos = *item;
      // symbols after dot
      सचे (gl_list_size (result) == 1 && !item_सङ्ख्या_is_rule_सङ्ख्या (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (pos)));
          lookahead_required = false;
        }
      item_सङ्ख्या *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      अत्थाय (state_item_सङ्ख्या nsi = si - state_वस्तूनि;
           !item_सङ्ख्या_is_rule_सङ्ख्या (*i);
           ++i, nsi = state_वस्तूनि[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          सचे (i == item)
            अनुवर्त;
          चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*i);
          सचे (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              अनुवर्त;
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
          सचे (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = रिक्त;
              अत्थाय (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          अञ्ञथा सचे (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          अञ्ञथा
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      ध्रुव rule *r = &rules[item_सङ्ख्या_as_rule_सङ्ख्या (*i)];
      // add derivations for symbols before dot
      अत्थाय (i = item - 1; !item_सङ्ख्या_is_rule_सङ्ख्या (*i) && i >= ritem; i--)
        {
          gl_list_ग्रन्थिः_t p = gl_list_previous_ग्रन्थिः (path, state_ग्रन्थिः);
          सचे (p)
            state_ग्रन्थिः = p;
          सचे (deriv)
            {
              ध्रुव शून्य *tmp_deriv = gl_list_ग्रन्थिः_मानम् (derivs, deriv);
              deriv = gl_list_previous_ग्रन्थिः (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          अञ्ञथा
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->सङ्ख्या, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  पटिनिवत्त res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
स्थिर state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_ग्रन्थिः_t tmp = gl_list_add_last (reduce_path, रिक्त);
  gl_list_ग्रन्थिः_t next_ग्रन्थिः = gl_list_previous_ग्रन्थिः (reduce_path, tmp);
  gl_list_ग्रन्थिः_t ग्रन्थिः = gl_list_previous_ग्रन्थिः (reduce_path, next_ग्रन्थिः);
  gl_list_remove_ग्रन्थिः (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  // FIXME: bool paths_merged;
  अत्थाय (; ग्रन्थिः != रिक्त; next_ग्रन्थिः = ग्रन्थिः,
       ग्रन्थिः = gl_list_previous_ग्रन्थिः (reduce_path, ग्रन्थिः))
    {
      state_item *refsi =
        (state_item *) gl_list_ग्रन्थिः_मानम् (reduce_path, ग्रन्थिः);
      state_item *nextrefsi =
        (state_item *) gl_list_ग्रन्थिः_मानम् (reduce_path, next_ग्रन्थिः);
      सचे (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          अनुवर्त;
        }
      // skip reduction items
      सचे (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        अनुवर्त;

      // bfs to find a shift to the right state
      si_bfs_ग्रन्थिः *init = si_bfs_new (si - state_वस्तूनि, रिक्त);
      si_bfs_ग्रन्थिः_list queue
        = gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ध्रुव शून्य **) &init);
      si_bfs_ग्रन्थिः *sis = रिक्त;
      state_item *prevsi = रिक्त;
      याव (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_ग्रन्थिः *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          सचे (sis->si == 0)
            भङ्ग;

          state_item *अन्वेषणम्_si = &state_वस्तूनि[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = अन्वेषणम्_si->revs;
          bitset_iterator biter;
          state_item_सङ्ख्या sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_वस्तूनि[sin];
              सचे (SI_TRANSITION (अन्वेषणम्_si))
                {
                  सचे (prevsi->state == refsi->state)
                    गच्छ अन्वेषणम्_end;
                }
              अञ्ञथा सचे (!si_bfs_contains (sis, sin))
                {
                  si_bfs_ग्रन्थिः *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    अन्वेषणम्_end:
      // prepend path to shift we found
      सचे (sis)
        {
          gl_list_ग्रन्थिः_t ln = gl_list_add_first (result, &state_वस्तूनि[sis->si]);
          अत्थाय (si_bfs_ग्रन्थिः *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_वस्तूनि[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  सचे (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = रिक्त;
      अत्थाय (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  पटिनिवत्त result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
स्थिर counterexample *
example_from_path (bool shift_reduce,
                   state_item_सङ्ख्या itm2,
                   state_item_list shortest_path, चिह्नम्_सङ्ख्या next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, रिक्त);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_वस्तूनि [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, रिक्त);
  gl_list_free (path_2);
  पटिनिवत्त new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
प्रकारनिर्देश संरचना
{
  parse_state *states[2];
  पूर्णाङ्क complexity;
} अन्वेषणम्_state;

स्थिर अन्वेषणम्_state *
initial_अन्वेषणम्_state (state_item *conflict1, state_item *conflict2)
{
  अन्वेषणम्_state *res = xmalloc (परिमाण *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  पटिनिवत्त res;
}

स्थिर अन्वेषणम्_state *
new_अन्वेषणम्_state (parse_state *ps1, parse_state *ps2, पूर्णाङ्क complexity)
{
  अन्वेषणम्_state *res = xmalloc (परिमाण *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  पटिनिवत्त res;
}

स्थिर अन्वेषणम्_state *
copy_अन्वेषणम्_state (अन्वेषणम्_state *parent)
{
  अन्वेषणम्_state *res = xmalloc (परिमाण *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  पटिनिवत्त res;
}

स्थिर शून्य
अन्वेषणम्_state_free_children (अन्वेषणम्_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

स्थिर शून्य
अन्वेषणम्_state_free (अन्वेषणम्_state *ss)
{
  सचे (ss == रिक्त)
    पटिनिवत्त;
  अन्वेषणम्_state_free_children (ss);
  मुक्त (ss);
}

/* For debugging traces.  */
स्थिर शून्य
अन्वेषणम्_state_print (अन्वेषणम्_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

प्रकारनिर्देश gl_list_t अन्वेषणम्_state_list;

स्थिर अन्तःस्थ bool
अन्वेषणम्_state_list_next (gl_list_iterator_t *it, अन्वेषणम्_state **ss)
{
  ध्रुव शून्य *p = रिक्त;
  bool res = gl_list_iterator_next (it, &p, रिक्त);
  सचे (res)
    *ss = (अन्वेषणम्_state*) p;
  अञ्ञथा
    gl_list_iterator_free (it);
  पटिनिवत्त res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
स्थिर अन्तःस्थ शून्य
ss_set_parse_state (अन्वेषणम्_state *ss, पूर्णाङ्क idx, parse_state *ps)
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
स्थिर counterexample *
complete_diverging_examples (अन्वेषणम्_state *ss,
                             चिह्नम्_सङ्ख्या next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  अत्थाय (पूर्णाङ्क i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  पटिनिवत्त new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
प्रकारनिर्देश संरचना
{
  अन्वेषणम्_state_list states;
  पूर्णाङ्क complexity;
} अन्वेषणम्_state_bundle;

स्थिर शून्य
ssb_free (अन्वेषणम्_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  मुक्त (ssb);
}

स्थिर size_t
ssb_hasher (अन्वेषणम्_state_bundle *ssb)
{
  पटिनिवत्त ssb->complexity;
}

स्थिर पूर्णाङ्क
ssb_comp (ध्रुव अन्वेषणम्_state_bundle *s1, ध्रुव अन्वेषणम्_state_bundle *s2)
{
  पटिनिवत्त s1->complexity - s2->complexity;
}

स्थिर bool
ssb_equals (ध्रुव अन्वेषणम्_state_bundle *s1, ध्रुव अन्वेषणम्_state_bundle *s2)
{
  पटिनिवत्त s1->complexity == s2->complexity;
}

प्रकारनिर्देश gl_list_t ssb_list;

स्थिर size_t
visited_hasher (ध्रुव अन्वेषणम्_state *ss, size_t max)
{
  पटिनिवत्त (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

स्थिर bool
visited_comparator (ध्रुव अन्वेषणम्_state *ss1, ध्रुव अन्वेषणम्_state *ss2)
{
  पटिनिवत्त parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
स्थिर ssb_list ssb_queue;
स्थिर Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
स्थिर bitset scp_set = रिक्त;
/* The set of parser states used for the conflict reduction rule. */
स्थिर bitset rpp_set = रिक्त;

स्थिर शून्य
ssb_append (अन्वेषणम्_state *ss)
{
  सचे (hash_lookup (visited, ss))
    {
      अन्वेषणम्_state_free (ss);
      पटिनिवत्त;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  अन्वेषणम्_state_bundle *ssb = xmalloc (परिमाण *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ग्रन्थिः_t n = gl_list_अन्वेषणम् (ssb_queue, ssb);
  सचे (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त,
                              (gl_listelement_dispose_fn)अन्वेषणम्_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  अञ्ञथा
    {
      मुक्त (ssb);
      ssb = (अन्वेषणम्_state_bundle *) gl_list_ग्रन्थिः_मानम् (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
स्थिर शून्य
production_step (अन्वेषणम्_state *ss, पूर्णाङ्क parser_state)
{
  ध्रुव state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  चिह्नम्_सङ्ख्या other_sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  पूर्णाङ्क complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = रिक्त;
  अत्थाय (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      अन्वेषणम्_state *copy = copy_अन्वेषणम्_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

स्थिर अन्तःस्थ पूर्णाङ्क
reduction_cost (ध्रुव parse_state *ps)
{
  पूर्णाङ्क shifts;
  पूर्णाङ्क productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  पटिनिवत्त SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

स्थिर अन्वेषणम्_state_list
reduction_step (अन्वेषणम्_state *ss, ध्रुव item_सङ्ख्या *conflict_item,
                पूर्णाङ्क parser_state, पूर्णाङ्क rule_len)
{
  (शून्य) conflict_item; // FIXME: Unused
  अन्वेषणम्_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, 1);

  parse_state *ps = ss->states[parser_state];
  ध्रुव state_item *si = parse_state_tail (ps);
  bitset चिह्नम्_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  ध्रुव state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  सचे (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*other_si->item))
    {
      चिह्नम्_सङ्ख्या other_sym =
        item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*other_si->item);
      सचे (!intersect_चिह्नम् (other_sym, चिह्नम्_set))
        पटिनिवत्त result;
      चिह्नम्_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (चिह्नम्_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, चिह्नम्_set);
  parse_state *reduced_ps = रिक्त;
  अत्थाय (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      अन्वेषणम्_state *copy = copy_अन्वेषणम्_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      पूर्णाङ्क r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  सचे (चिह्नम्_set != si->lookahead)
    bitset_free (चिह्नम्_set);
  पटिनिवत्त result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
स्थिर शून्य
अन्वेषणम्_state_prepend (अन्वेषणम्_state *ss, चिह्नम्_सङ्ख्या sym, bitset guide)
{
  (शून्य) sym; // FIXME: Unused.
  ध्रुव state_item *si1src = parse_state_head (ss->states[0]);
  ध्रुव state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  सचे (prod1 != SI_PRODUCTION (si2src))
    {
      पूर्णाङ्क prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = रिक्त;
      अत्थाय (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          ध्रुव state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->सङ्ख्या);
          सचे (!guided && !EXTENDED_अन्वेषणम्)
            अनुवर्त;

          अन्वेषणम्_state *copy = copy_अन्वेषणम्_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          सचे (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      पटिनिवत्त;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ध्रुव पूर्णाङ्क complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = रिक्त;
  अत्थाय (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      ध्रुव state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->सङ्ख्या);
      सचे (!guided1 && !EXTENDED_अन्वेषणम्)
        अनुवर्त;

      parse_state *ps2 = रिक्त;
      अत्थाय (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          ध्रुव state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->सङ्ख्या);
          सचे (!guided2 && !EXTENDED_अन्वेषणम्)
            अनुवर्त;
          // Only consider prepend state items that share the same state.
          सचे (psi1->state != psi2->state)
            अनुवर्त;

          पूर्णाङ्क complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          सचे (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_अन्वेषणम्_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
स्थिर bool
have_common_prefix (ध्रुव item_सङ्ख्या *itm1, ध्रुव item_सङ्ख्या *itm2)
{
  पूर्णाङ्क i = 0;
  अत्थाय (; !item_सङ्ख्या_is_rule_सङ्ख्या (itm1[i]); ++i)
    सचे (itm1[i] != itm2[i])
      पटिनिवत्त false;
  पटिनिवत्त item_सङ्ख्या_is_rule_सङ्ख्या (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
स्थिर ध्रुव item_सङ्ख्या *
item_rule_start (ध्रुव item_सङ्ख्या *item)
{
  ध्रुव item_सङ्ख्या *res = रिक्त;
  अत्थाय (res = item;
       ritem < res && item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*(res - 1));
       --res)
    अनुवर्त;
  पटिनिवत्त res;
}

स्थिर ध्रुव item_सङ्ख्या *
item_rule_end (ध्रुव item_सङ्ख्या *item)
{
  ध्रुव item_सङ्ख्या *res = रिक्त;
  अत्थाय (res = item; item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*res); ++res)
    अनुवर्त;
  पटिनिवत्त res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
स्थिर अन्तःस्थ शून्य
generate_next_states (अन्वेषणम्_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  ध्रुव state_item *si1 = parse_state_tail (ps1);
  ध्रुव state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_सङ्ख्या_is_rule_सङ्ख्या (*si1->item);
  bool si2reduce = item_सङ्ख्या_is_rule_सङ्ख्या (*si2->item);
  सचे (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      सचे (*si1->item == *si2->item)
        {
          पूर्णाङ्क complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = रिक्त;
          parse_state *tps2 = रिक्त;
          अत्थाय (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            अत्थाय (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_अन्वेषणम्_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  अञ्ञथा
    {
      ध्रुव item_सङ्ख्या *rhs1 = item_rule_start (si1->item);
      ध्रुव item_सङ्ख्या *rhe1 = item_rule_end (si1->item);
      पूर्णाङ्क len1 = rhe1 - rhs1;
      पूर्णाङ्क size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ध्रुव item_सङ्ख्या *rhs2 = item_rule_start (si2->item);
      ध्रुव item_सङ्ख्या *rhe2 = item_rule_end (si2->item);
      पूर्णाङ्क len2 = rhe2 - rhs2;
      पूर्णाङ्क size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      सचे (ready1 && ready2)
        {
          अन्वेषणम्_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          अन्वेषणम्_state *red1 = रिक्त;
          अत्थाय (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               अन्वेषणम्_state_list_next (&iter, &red1);
               )
            {
              अन्वेषणम्_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              अन्वेषणम्_state *red2 = रिक्त;
              अत्थाय (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   अन्वेषणम्_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              सचे (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      अञ्ञथा सचे (ready1)
        {
          अन्वेषणम्_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          अन्वेषणम्_state *red1 = रिक्त;
          अत्थाय (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               अन्वेषणम्_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      अञ्ञथा सचे (ready2)
        {
          अन्वेषणम्_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          अन्वेषणम्_state *red2 = रिक्त;
          अत्थाय (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               अन्वेषणम्_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      अञ्ञथा
        {
          ध्रुव चिह्नम्_सङ्ख्या sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          अन्वेषणम्_state_prepend (ss, sym,
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
स्थिर counterexample *
unifying_example (state_item_सङ्ख्या itm1,
                  state_item_सङ्ख्या itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, चिह्नम्_सङ्ख्या next_sym)
{
  state_item *conflict1 = &state_वस्तूनि[itm1];
  state_item *conflict2 = &state_वस्तूनि[itm2];
  अन्वेषणम्_state *initial = initial_अन्वेषणम्_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, रिक्त, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) अन्वेषणम्_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  अन्वेषणम्_state *stage3result = रिक्त;
  counterexample *cex = रिक्त;
  याव (gl_list_size (ssb_queue) > 0)
    {
      ध्रुव अन्वेषणम्_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      अन्वेषणम्_state *ss = रिक्त;
      अत्थाय (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           अन्वेषणम्_state_list_next (&it, &ss);
           )
        {
          सचे (trace_flag & trace_cex)
            अन्वेषणम्_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          सचे (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ध्रुव state_item *si1src = parse_state_head (ps1);
              ध्रुव state_item *si2src = parse_state_head (ps2);
              सचे (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  सचे (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      गच्छ cex_अन्वेषणम्_end;
                    }
                  सचे (!stage3result)
                    stage3result = copy_अन्वेषणम्_state (ss);
                }
            }
          सचे (TIME_LIMIT_ENFORCED)
            {
              द्विगुण time_passed = (gethrxtime () - start) / 1e9;
              सचे (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              सचे (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  गच्छ cex_अन्वेषणम्_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_अन्वेषणम्_end:;
  सचे (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      सचे (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      अञ्ञथा
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  सचे (stage3result)
    अन्वेषणम्_state_free (stage3result);
  पटिनिवत्त cex;
}

स्थिर xtime_t cumulative_time;

शून्य
counterexample_init (शून्य)
{
  // Check cex.timeout.
  {
    ध्रुव वर्ण *variable = "cex.timeout";
    वर्ण *cp = muscle_percent_define_get (variable);
    सचे (*cp != '\0')
      {
        वर्ण *end = रिक्त;
        द्विगुण v = c_strtod (cp, &end);
        सचे (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        अञ्ञथा
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    मुक्त (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_वस्तूनि_init ();
}


शून्य
counterexample_free (शून्य)
{
  सचे (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_वस्तूनि_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
स्थिर शून्य
counterexample_report (state_item_सङ्ख्या itm1, state_item_सङ्ख्या itm2,
                       चिह्नम्_सङ्ख्या next_sym, bool shift_reduce,
                       FILE *out, ध्रुव वर्ण *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  ध्रुव rule *reduce_rule = item_rule (state_वस्तूनि[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = रिक्त;
  अत्थाय (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->सङ्ख्या);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      सचे (reduce_prod_reached)
        bitset_set (rpp_set, si->state->सङ्ख्या);
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
स्थिर शून्य
counterexample_report_shift_reduce (state_item_सङ्ख्या itm1, state_item_सङ्ख्या itm2,
                                    चिह्नम्_सङ्ख्या next_sym,
                                    FILE *out, ध्रुव वर्ण *prefix)
{
  सचे (out == stderr)
    complain (रिक्त, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  अञ्ञथा
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  सचे (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_वस्तूनि[itm1], out, prefix);
      state_item_print (&state_वस्तूनि[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

स्थिर शून्य
counterexample_report_reduce_reduce (state_item_सङ्ख्या itm1, state_item_सङ्ख्या itm2,
                                     bitset conflict_syms,
                                     FILE *out, ध्रुव वर्ण *prefix)
{
  {
    संरचना obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_सङ्ख्या sym;
    ध्रुव वर्ण *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    वर्ण *tokens = obstack_finish0 (&obstack);
    सचे (out == stderr)
      complain (रिक्त, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_गणनम् (conflict_syms)),
                tokens);
    अञ्ञथा
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_गणनम् (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, रिक्त);
  }
  // In the report, print the items.
  सचे (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_वस्तूनि[itm1], out, prefix);
      state_item_print (&state_वस्तूनि[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

स्थिर state_item_सङ्ख्या
find_state_item_सङ्ख्या (ध्रुव rule *r, state_सङ्ख्या sn)
{
  अत्थाय (state_item_सङ्ख्या i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    सचे (!SI_DISABLED (i)
        && item_सङ्ख्या_as_rule_सङ्ख्या (*state_वस्तूनि[i].item) == r->सङ्ख्या)
      पटिनिवत्त i;
  abort ();
}

शून्य
counterexample_report_state (ध्रुव state *s, FILE *out, ध्रुव वर्ण *prefix)
{
  ध्रुव state_सङ्ख्या sn = s->सङ्ख्या;
  ध्रुव reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  अत्थाय (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    {
      ध्रुव rule *r1 = reds->rules[i];
      ध्रुव state_item_सङ्ख्या c1 = find_state_item_सङ्ख्या (r1, sn);
      अत्थाय (state_item_सङ्ख्या c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        सचे (!SI_DISABLED (c2))
          {
            item_सङ्ख्या conf = *state_वस्तूनि[c2].item;
            सचे (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      अत्थाय (पूर्णाङ्क j = i+1; j < reds->अङ्कः; ++j)
        {
          ध्रुव rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          सचे (!bitset_empty_p (lookaheads))
            अत्थाय (state_item_सङ्ख्या c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              सचे (!SI_DISABLED (c2)
                  && item_rule (state_वस्तूनि[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  भङ्ग;
                }
        }
    }
  bitset_free (lookaheads);
}
