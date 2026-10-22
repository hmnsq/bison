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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "counterexample.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <c-strtod.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <gethrxtime.h>
#அந்தர்பாவ <gl_linked_list.h>
#அந்தர்பாவ <gl_rbtreehash_list.h>
#அந்தர்பாவ <hash.h>
#அந்தர்பாவ <mbswidth.h>
#அந்தர்பாவ <quote.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <textstyle.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "derivation.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lssi.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "nullable.h"
#அந்தர்பாவ "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_தேடல் false

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
நிலையான இரட்டை time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
நிலையான gl_list_முனை_t
list_get_end (gl_list_t list)
{
  gl_list_முனை_t sentinel = gl_list_add_last (list, வெற்று);
  gl_list_முனை_t res = gl_list_previous_முனை (list, sentinel);
  gl_list_remove_முனை (list, sentinel);
  திரும்பு res;
}

வகைவரை கட்டமைப்பு
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

நிலையான counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (அளவு *res);
  res->shift_reduce = shift_reduce;
  எனில் (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  இல்லையெனில்
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  திரும்பு res;
}

நிலையான வெறுமை
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  விடுவி (cex);
}

நிலையான வெறுமை
counterexample_print (மாறிலி counterexample *cex, FILE *out, மாறிலி எழுத்து *prefix)
{
  மாறிலி bool flat = getenv ("YYFLAT");
  மாறிலி எழுத்து *example1_label
    = cex->unifying ? _("Example") : _("First example");
  மாறிலி எழுத்து *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  மாறிலி எழுத்து *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  மாறிலி எழுத்து *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  மாறிலி முழு width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  எனில் (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  இல்லையெனில்
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  எனில் (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  இல்லையெனில்
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  எனில் (!cex->unifying || is_styled (stderr))
    {
      எனில் (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      இல்லையெனில்
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  எனில் (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  இல்லையெனில்
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  எனில் (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
கட்டமைப்பு si_bfs_முனை;
வகைவரை கட்டமைப்பு si_bfs_முனை
{
  state_item_எண் si;
  கட்டமைப்பு si_bfs_முனை *parent;
  முழு reference_மொத்தம்;
} si_bfs_முனை;

நிலையான si_bfs_முனை *
si_bfs_new (state_item_எண் si, si_bfs_முனை *parent)
{
  si_bfs_முனை *res = xcalloc (1, அளவு *res);
  res->si = si;
  res->parent = parent;
  res->reference_மொத்தம் = 1;
  எனில் (parent)
    ++parent->reference_மொத்தம்;
  திரும்பு res;
}

நிலையான bool
si_bfs_contains (மாறிலி si_bfs_முனை *n, state_item_எண் sin)
{
  க்கு (மாறிலி si_bfs_முனை *தேடல் = n; தேடல் != வெற்று; தேடல் = தேடல்->parent)
    எனில் (தேடல்->si == sin)
      திரும்பு true;
  திரும்பு false;
}

நிலையான வெறுமை
si_bfs_free (si_bfs_முனை *n)
{
  எனில் (n == வெற்று)
    திரும்பு;
  --n->reference_மொத்தம்;
  எனில் (n->reference_மொத்தம் == 0)
    {
      si_bfs_free (n->parent);
      விடுவி (n);
    }
}

வகைவரை gl_list_t si_bfs_முனை_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
நிலையான உள்வரி derivation_list
expand_to_conflict (state_item_எண் start, குறியீடு_எண் conflict_sym)
{
  si_bfs_முனை *init = si_bfs_new (start, வெற்று);

  si_bfs_முனை_list queue
    = gl_list_create (GL_LINKED_LIST, வெற்று, வெற்று,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (மாறிலி வெறுமை **) &init);
  si_bfs_முனை *முனை = வெற்று;
  // breadth-first search for a path of productions to the conflict symbol
  போது (gl_list_size (queue) > 0)
    {
      முனை = (si_bfs_முனை *) gl_list_get_at (queue, 0);
      state_item *silast = &state_பொருட்கள்[முனை->si];
      குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (*silast->item);
      எனில் (sym == conflict_sym)
        நிறுத்து;
      எனில் (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_எண் sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              எனில் (si_bfs_contains (முனை, sin))
                தொடர்;
              si_bfs_முனை *next = si_bfs_new (sin, முனை);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          எனில் (nullable[sym - ntokens])
            {
              si_bfs_முனை *next = si_bfs_new (silast->trans, முனை);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  எனில் (gl_list_size (queue) == 0)
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

  க்கு (si_bfs_முனை *n = முனை; n != வெற்று; n = n->parent)
    {
      state_item *si = &state_பொருட்கள்[n->si];
      item_எண் *pos = si->item;
      எனில் (SI_PRODUCTION (si))
        {
          item_எண் *i = வெற்று;
          க்கு (i = pos + 1; !item_எண்_is_rule_எண் (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          குறியீடு_எண் lhs =
            rules[item_எண்_as_rule_எண் (*i)].lhs->எண்;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      இல்லையெனில்
        {
          குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  திரும்பு result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
நிலையான derivation *
complete_diverging_example (குறியீடு_எண் conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  எனில் (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_முனை_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  க்கு (gl_list_முனை_t state_முனை = list_get_end (path);
       state_முனை != வெற்று;
       state_முனை = gl_list_previous_முனை (path, state_முனை))
    {
      state_item *si = (state_item *) gl_list_முனை_மதிப்பு (path, state_முனை);
      item_எண் *item = si->item;
      item_எண் pos = *item;
      // symbols after dot
      எனில் (gl_list_size (result) == 1 && !item_எண்_is_rule_எண் (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_எண்_as_குறியீடு_எண் (pos)));
          lookahead_required = false;
        }
      item_எண் *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      க்கு (state_item_எண் nsi = si - state_பொருட்கள்;
           !item_எண்_is_rule_எண் (*i);
           ++i, nsi = state_பொருட்கள்[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          எனில் (i == item)
            தொடர்;
          குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (*i);
          எனில் (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              தொடர்;
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
          எனில் (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = வெற்று;
              க்கு (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          இல்லையெனில் எனில் (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          இல்லையெனில்
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      மாறிலி rule *r = &rules[item_எண்_as_rule_எண் (*i)];
      // add derivations for symbols before dot
      க்கு (i = item - 1; !item_எண்_is_rule_எண் (*i) && i >= ritem; i--)
        {
          gl_list_முனை_t p = gl_list_previous_முனை (path, state_முனை);
          எனில் (p)
            state_முனை = p;
          எனில் (deriv)
            {
              மாறிலி வெறுமை *tmp_deriv = gl_list_முனை_மதிப்பு (derivs, deriv);
              deriv = gl_list_previous_முனை (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          இல்லையெனில்
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->எண், result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  திரும்பு res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
நிலையான state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_முனை_t tmp = gl_list_add_last (reduce_path, வெற்று);
  gl_list_முனை_t next_முனை = gl_list_previous_முனை (reduce_path, tmp);
  gl_list_முனை_t முனை = gl_list_previous_முனை (reduce_path, next_முனை);
  gl_list_remove_முனை (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
  // FIXME: bool paths_merged;
  க்கு (; முனை != வெற்று; next_முனை = முனை,
       முனை = gl_list_previous_முனை (reduce_path, முனை))
    {
      state_item *refsi =
        (state_item *) gl_list_முனை_மதிப்பு (reduce_path, முனை);
      state_item *nextrefsi =
        (state_item *) gl_list_முனை_மதிப்பு (reduce_path, next_முனை);
      எனில் (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          தொடர்;
        }
      // skip reduction items
      எனில் (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        தொடர்;

      // bfs to find a shift to the right state
      si_bfs_முனை *init = si_bfs_new (si - state_பொருட்கள், வெற்று);
      si_bfs_முனை_list queue
        = gl_list_create (GL_LINKED_LIST, வெற்று, வெற்று,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (மாறிலி வெறுமை **) &init);
      si_bfs_முனை *sis = வெற்று;
      state_item *prevsi = வெற்று;
      போது (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_முனை *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          எனில் (sis->si == 0)
            நிறுத்து;

          state_item *தேடல்_si = &state_பொருட்கள்[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = தேடல்_si->revs;
          bitset_iterator biter;
          state_item_எண் sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_பொருட்கள்[sin];
              எனில் (SI_TRANSITION (தேடல்_si))
                {
                  எனில் (prevsi->state == refsi->state)
                    செல் தேடல்_end;
                }
              இல்லையெனில் எனில் (!si_bfs_contains (sis, sin))
                {
                  si_bfs_முனை *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    தேடல்_end:
      // prepend path to shift we found
      எனில் (sis)
        {
          gl_list_முனை_t ln = gl_list_add_first (result, &state_பொருட்கள்[sis->si]);
          க்கு (si_bfs_முனை *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_பொருட்கள்[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  எனில் (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = வெற்று;
      க்கு (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  திரும்பு result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
நிலையான counterexample *
example_from_path (bool shift_reduce,
                   state_item_எண் itm2,
                   state_item_list shortest_path, குறியீடு_எண் next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, வெற்று);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_பொருட்கள் [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, வெற்று);
  gl_list_free (path_2);
  திரும்பு new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
வகைவரை கட்டமைப்பு
{
  parse_state *states[2];
  முழு complexity;
} தேடல்_state;

நிலையான தேடல்_state *
initial_தேடல்_state (state_item *conflict1, state_item *conflict2)
{
  தேடல்_state *res = xmalloc (அளவு *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  திரும்பு res;
}

நிலையான தேடல்_state *
new_தேடல்_state (parse_state *ps1, parse_state *ps2, முழு complexity)
{
  தேடல்_state *res = xmalloc (அளவு *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  திரும்பு res;
}

நிலையான தேடல்_state *
copy_தேடல்_state (தேடல்_state *parent)
{
  தேடல்_state *res = xmalloc (அளவு *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  திரும்பு res;
}

நிலையான வெறுமை
தேடல்_state_free_children (தேடல்_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

நிலையான வெறுமை
தேடல்_state_free (தேடல்_state *ss)
{
  எனில் (ss == வெற்று)
    திரும்பு;
  தேடல்_state_free_children (ss);
  விடுவி (ss);
}

/* For debugging traces.  */
நிலையான வெறுமை
தேடல்_state_print (தேடல்_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

வகைவரை gl_list_t தேடல்_state_list;

நிலையான உள்வரி bool
தேடல்_state_list_next (gl_list_iterator_t *it, தேடல்_state **ss)
{
  மாறிலி வெறுமை *p = வெற்று;
  bool res = gl_list_iterator_next (it, &p, வெற்று);
  எனில் (res)
    *ss = (தேடல்_state*) p;
  இல்லையெனில்
    gl_list_iterator_free (it);
  திரும்பு res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
நிலையான உள்வரி வெறுமை
ss_set_parse_state (தேடல்_state *ss, முழு idx, parse_state *ps)
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
நிலையான counterexample *
complete_diverging_examples (தேடல்_state *ss,
                             குறியீடு_எண் next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  க்கு (முழு i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  திரும்பு new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
வகைவரை கட்டமைப்பு
{
  தேடல்_state_list states;
  முழு complexity;
} தேடல்_state_bundle;

நிலையான வெறுமை
ssb_free (தேடல்_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  விடுவி (ssb);
}

நிலையான size_t
ssb_hasher (தேடல்_state_bundle *ssb)
{
  திரும்பு ssb->complexity;
}

நிலையான முழு
ssb_comp (மாறிலி தேடல்_state_bundle *s1, மாறிலி தேடல்_state_bundle *s2)
{
  திரும்பு s1->complexity - s2->complexity;
}

நிலையான bool
ssb_equals (மாறிலி தேடல்_state_bundle *s1, மாறிலி தேடல்_state_bundle *s2)
{
  திரும்பு s1->complexity == s2->complexity;
}

வகைவரை gl_list_t ssb_list;

நிலையான size_t
visited_hasher (மாறிலி தேடல்_state *ss, size_t max)
{
  திரும்பு (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

நிலையான bool
visited_comparator (மாறிலி தேடல்_state *ss1, மாறிலி தேடல்_state *ss2)
{
  திரும்பு parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
நிலையான ssb_list ssb_queue;
நிலையான Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
நிலையான bitset scp_set = வெற்று;
/* The set of parser states used for the conflict reduction rule. */
நிலையான bitset rpp_set = வெற்று;

நிலையான வெறுமை
ssb_append (தேடல்_state *ss)
{
  எனில் (hash_lookup (visited, ss))
    {
      தேடல்_state_free (ss);
      திரும்பு;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  தேடல்_state_bundle *ssb = xmalloc (அளவு *ssb);
  ssb->complexity = ss->complexity;
  gl_list_முனை_t n = gl_list_தேடல் (ssb_queue, ssb);
  எனில் (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று,
                              (gl_listelement_dispose_fn)தேடல்_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  இல்லையெனில்
    {
      விடுவி (ssb);
      ssb = (தேடல்_state_bundle *) gl_list_முனை_மதிப்பு (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
நிலையான வெறுமை
production_step (தேடல்_state *ss, முழு parser_state)
{
  மாறிலி state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  குறியீடு_எண் other_sym = item_எண்_as_குறியீடு_எண் (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  முழு complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = வெற்று;
  க்கு (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      தேடல்_state *copy = copy_தேடல்_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

நிலையான உள்வரி முழு
reduction_cost (மாறிலி parse_state *ps)
{
  முழு shifts;
  முழு productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  திரும்பு SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

நிலையான தேடல்_state_list
reduction_step (தேடல்_state *ss, மாறிலி item_எண் *conflict_item,
                முழு parser_state, முழு rule_len)
{
  (வெறுமை) conflict_item; // FIXME: Unused
  தேடல்_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, 1);

  parse_state *ps = ss->states[parser_state];
  மாறிலி state_item *si = parse_state_tail (ps);
  bitset குறியீடு_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  மாறிலி state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  எனில் (item_எண்_is_குறியீடு_எண் (*other_si->item))
    {
      குறியீடு_எண் other_sym =
        item_எண்_as_குறியீடு_எண் (*other_si->item);
      எனில் (!intersect_குறியீடு (other_sym, குறியீடு_set))
        திரும்பு result;
      குறியீடு_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (குறியீடு_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, குறியீடு_set);
  parse_state *reduced_ps = வெற்று;
  க்கு (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      தேடல்_state *copy = copy_தேடல்_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      முழு r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  எனில் (குறியீடு_set != si->lookahead)
    bitset_free (குறியீடு_set);
  திரும்பு result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
நிலையான வெறுமை
தேடல்_state_prepend (தேடல்_state *ss, குறியீடு_எண் sym, bitset guide)
{
  (வெறுமை) sym; // FIXME: Unused.
  மாறிலி state_item *si1src = parse_state_head (ss->states[0]);
  மாறிலி state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  எனில் (prod1 != SI_PRODUCTION (si2src))
    {
      முழு prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = வெற்று;
      க்கு (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          மாறிலி state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->எண்);
          எனில் (!guided && !EXTENDED_தேடல்)
            தொடர்;

          தேடல்_state *copy = copy_தேடல்_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          எனில் (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      திரும்பு;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  மாறிலி முழு complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = வெற்று;
  க்கு (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      மாறிலி state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->எண்);
      எனில் (!guided1 && !EXTENDED_தேடல்)
        தொடர்;

      parse_state *ps2 = வெற்று;
      க்கு (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          மாறிலி state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->எண்);
          எனில் (!guided2 && !EXTENDED_தேடல்)
            தொடர்;
          // Only consider prepend state items that share the same state.
          எனில் (psi1->state != psi2->state)
            தொடர்;

          முழு complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          எனில் (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_தேடல்_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
நிலையான bool
have_common_prefix (மாறிலி item_எண் *itm1, மாறிலி item_எண் *itm2)
{
  முழு i = 0;
  க்கு (; !item_எண்_is_rule_எண் (itm1[i]); ++i)
    எனில் (itm1[i] != itm2[i])
      திரும்பு false;
  திரும்பு item_எண்_is_rule_எண் (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
நிலையான மாறிலி item_எண் *
item_rule_start (மாறிலி item_எண் *item)
{
  மாறிலி item_எண் *res = வெற்று;
  க்கு (res = item;
       ritem < res && item_எண்_is_குறியீடு_எண் (*(res - 1));
       --res)
    தொடர்;
  திரும்பு res;
}

நிலையான மாறிலி item_எண் *
item_rule_end (மாறிலி item_எண் *item)
{
  மாறிலி item_எண் *res = வெற்று;
  க்கு (res = item; item_எண்_is_குறியீடு_எண் (*res); ++res)
    தொடர்;
  திரும்பு res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
நிலையான உள்வரி வெறுமை
generate_next_states (தேடல்_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  மாறிலி state_item *si1 = parse_state_tail (ps1);
  மாறிலி state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_எண்_is_rule_எண் (*si1->item);
  bool si2reduce = item_எண்_is_rule_எண் (*si2->item);
  எனில் (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      எனில் (*si1->item == *si2->item)
        {
          முழு complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = வெற்று;
          parse_state *tps2 = வெற்று;
          க்கு (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            க்கு (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_தேடல்_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  இல்லையெனில்
    {
      மாறிலி item_எண் *rhs1 = item_rule_start (si1->item);
      மாறிலி item_எண் *rhe1 = item_rule_end (si1->item);
      முழு len1 = rhe1 - rhs1;
      முழு size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      மாறிலி item_எண் *rhs2 = item_rule_start (si2->item);
      மாறிலி item_எண் *rhe2 = item_rule_end (si2->item);
      முழு len2 = rhe2 - rhs2;
      முழு size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      எனில் (ready1 && ready2)
        {
          தேடல்_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          தேடல்_state *red1 = வெற்று;
          க்கு (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               தேடல்_state_list_next (&iter, &red1);
               )
            {
              தேடல்_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              தேடல்_state *red2 = வெற்று;
              க்கு (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   தேடல்_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              எனில் (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      இல்லையெனில் எனில் (ready1)
        {
          தேடல்_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          தேடல்_state *red1 = வெற்று;
          க்கு (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               தேடல்_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      இல்லையெனில் எனில் (ready2)
        {
          தேடல்_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          தேடல்_state *red2 = வெற்று;
          க்கு (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               தேடல்_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      இல்லையெனில்
        {
          மாறிலி குறியீடு_எண் sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          தேடல்_state_prepend (ss, sym,
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
நிலையான counterexample *
unifying_example (state_item_எண் itm1,
                  state_item_எண் itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, குறியீடு_எண் next_sym)
{
  state_item *conflict1 = &state_பொருட்கள்[itm1];
  state_item *conflict2 = &state_பொருட்கள்[itm2];
  தேடல்_state *initial = initial_தேடல்_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, வெற்று, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) தேடல்_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  தேடல்_state *stage3result = வெற்று;
  counterexample *cex = வெற்று;
  போது (gl_list_size (ssb_queue) > 0)
    {
      மாறிலி தேடல்_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      தேடல்_state *ss = வெற்று;
      க்கு (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           தேடல்_state_list_next (&it, &ss);
           )
        {
          எனில் (trace_flag & trace_cex)
            தேடல்_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          எனில் (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              மாறிலி state_item *si1src = parse_state_head (ps1);
              மாறிலி state_item *si2src = parse_state_head (ps2);
              எனில் (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  எனில் (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      செல் cex_தேடல்_end;
                    }
                  எனில் (!stage3result)
                    stage3result = copy_தேடல்_state (ss);
                }
            }
          எனில் (TIME_LIMIT_ENFORCED)
            {
              இரட்டை time_passed = (gethrxtime () - start) / 1e9;
              எனில் (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              எனில் (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  செல் cex_தேடல்_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_தேடல்_end:;
  எனில் (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      எனில் (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      இல்லையெனில்
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  எனில் (stage3result)
    தேடல்_state_free (stage3result);
  திரும்பு cex;
}

நிலையான xtime_t cumulative_time;

வெறுமை
counterexample_init (வெறுமை)
{
  // Check cex.timeout.
  {
    மாறிலி எழுத்து *variable = "cex.timeout";
    எழுத்து *cp = muscle_percent_define_get (variable);
    எனில் (*cp != '\0')
      {
        எழுத்து *end = வெற்று;
        இரட்டை v = c_strtod (cp, &end);
        எனில் (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        இல்லையெனில்
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    விடுவி (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_பொருட்கள்_init ();
}


வெறுமை
counterexample_free (வெறுமை)
{
  எனில் (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_பொருட்கள்_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
நிலையான வெறுமை
counterexample_report (state_item_எண் itm1, state_item_எண் itm2,
                       குறியீடு_எண் next_sym, bool shift_reduce,
                       FILE *out, மாறிலி எழுத்து *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  மாறிலி rule *reduce_rule = item_rule (state_பொருட்கள்[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = வெற்று;
  க்கு (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->எண்);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      எனில் (reduce_prod_reached)
        bitset_set (rpp_set, si->state->எண்);
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
நிலையான வெறுமை
counterexample_report_shift_reduce (state_item_எண் itm1, state_item_எண் itm2,
                                    குறியீடு_எண் next_sym,
                                    FILE *out, மாறிலி எழுத்து *prefix)
{
  எனில் (out == stderr)
    complain (வெற்று, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  இல்லையெனில்
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  எனில் (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_பொருட்கள்[itm1], out, prefix);
      state_item_print (&state_பொருட்கள்[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

நிலையான வெறுமை
counterexample_report_reduce_reduce (state_item_எண் itm1, state_item_எண் itm2,
                                     bitset conflict_syms,
                                     FILE *out, மாறிலி எழுத்து *prefix)
{
  {
    கட்டமைப்பு obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_எண் sym;
    மாறிலி எழுத்து *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    எழுத்து *tokens = obstack_finish0 (&obstack);
    எனில் (out == stderr)
      complain (வெற்று, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_மொத்தம் (conflict_syms)),
                tokens);
    இல்லையெனில்
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_மொத்தம் (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, வெற்று);
  }
  // In the report, print the items.
  எனில் (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_பொருட்கள்[itm1], out, prefix);
      state_item_print (&state_பொருட்கள்[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

நிலையான state_item_எண்
find_state_item_எண் (மாறிலி rule *r, state_எண் sn)
{
  க்கு (state_item_எண் i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    எனில் (!SI_DISABLED (i)
        && item_எண்_as_rule_எண் (*state_பொருட்கள்[i].item) == r->எண்)
      திரும்பு i;
  abort ();
}

வெறுமை
counterexample_report_state (மாறிலி state *s, FILE *out, மாறிலி எழுத்து *prefix)
{
  மாறிலி state_எண் sn = s->எண்;
  மாறிலி reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
    {
      மாறிலி rule *r1 = reds->rules[i];
      மாறிலி state_item_எண் c1 = find_state_item_எண் (r1, sn);
      க்கு (state_item_எண் c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        எனில் (!SI_DISABLED (c2))
          {
            item_எண் conf = *state_பொருட்கள்[c2].item;
            எனில் (item_எண்_is_குறியீடு_எண் (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      க்கு (முழு j = i+1; j < reds->இலக்கம்; ++j)
        {
          மாறிலி rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          எனில் (!bitset_empty_p (lookaheads))
            க்கு (state_item_எண் c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              எனில் (!SI_DISABLED (c2)
                  && item_rule (state_பொருட்கள்[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  நிறுத்து;
                }
        }
    }
  bitset_free (lookaheads);
}
