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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "counterexample.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <c-strtod.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <gethrxtime.h>
#અન્તર્ભાવ <gl_linked_list.h>
#અન્તર્ભાવ <gl_rbtreehash_list.h>
#અન્તર્ભાવ <hash.h>
#અન્તર્ભાવ <mbswidth.h>
#અન્તર્ભાવ <quote.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <textstyle.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "derivation.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lssi.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "nullable.h"
#અન્તર્ભાવ "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_શોધ false

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
સ્થિર દ્વિગુણ time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
સ્થિર gl_list_ગાંઠ_t
list_get_end (gl_list_t list)
{
  gl_list_ગાંઠ_t sentinel = gl_list_add_last (list, ખાલી);
  gl_list_ગાંઠ_t res = gl_list_previous_ગાંઠ (list, sentinel);
  gl_list_remove_ગાંઠ (list, sentinel);
  પાછા res;
}

પ્રકારવ્યાખ્યા સંરચના
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

સ્થિર counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (કદ *res);
  res->shift_reduce = shift_reduce;
  જો (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  નહીંતર
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  પાછા res;
}

સ્થિર શૂન્ય
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  મુક્ત (cex);
}

સ્થિર શૂન્ય
counterexample_print (અચલ counterexample *cex, FILE *out, અચલ અક્ષર *prefix)
{
  અચલ bool flat = getenv ("YYFLAT");
  અચલ અક્ષર *example1_label
    = cex->unifying ? _("Example") : _("First example");
  અચલ અક્ષર *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  અચલ અક્ષર *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  અચલ અક્ષર *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  અચલ પૂર્ણાંક width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  જો (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  નહીંતર
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  જો (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  નહીંતર
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  જો (!cex->unifying || is_styled (stderr))
    {
      જો (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      નહીંતર
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  જો (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  નહીંતર
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  જો (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
સંરચના si_bfs_ગાંઠ;
પ્રકારવ્યાખ્યા સંરચના si_bfs_ગાંઠ
{
  state_item_સંખ્યા si;
  સંરચના si_bfs_ગાંઠ *parent;
  પૂર્ણાંક reference_ગણતરી;
} si_bfs_ગાંઠ;

સ્થિર si_bfs_ગાંઠ *
si_bfs_new (state_item_સંખ્યા si, si_bfs_ગાંઠ *parent)
{
  si_bfs_ગાંઠ *res = xcalloc (1, કદ *res);
  res->si = si;
  res->parent = parent;
  res->reference_ગણતરી = 1;
  જો (parent)
    ++parent->reference_ગણતરી;
  પાછા res;
}

સ્થિર bool
si_bfs_contains (અચલ si_bfs_ગાંઠ *n, state_item_સંખ્યા sin)
{
  માટે (અચલ si_bfs_ગાંઠ *શોધ = n; શોધ != ખાલી; શોધ = શોધ->parent)
    જો (શોધ->si == sin)
      પાછા true;
  પાછા false;
}

સ્થિર શૂન્ય
si_bfs_free (si_bfs_ગાંઠ *n)
{
  જો (n == ખાલી)
    પાછા;
  --n->reference_ગણતરી;
  જો (n->reference_ગણતરી == 0)
    {
      si_bfs_free (n->parent);
      મુક્ત (n);
    }
}

પ્રકારવ્યાખ્યા gl_list_t si_bfs_ગાંઠ_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
સ્થિર અંતઃરેખ derivation_list
expand_to_conflict (state_item_સંખ્યા start, પ્રતીક_સંખ્યા conflict_sym)
{
  si_bfs_ગાંઠ *init = si_bfs_new (start, ખાલી);

  si_bfs_ગાંઠ_list queue
    = gl_list_create (GL_LINKED_LIST, ખાલી, ખાલી,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (અચલ શૂન્ય **) &init);
  si_bfs_ગાંઠ *ગાંઠ = ખાલી;
  // breadth-first search for a path of productions to the conflict symbol
  જ્યારે (gl_list_size (queue) > 0)
    {
      ગાંઠ = (si_bfs_ગાંઠ *) gl_list_get_at (queue, 0);
      state_item *silast = &state_વસ્તુઓ[ગાંઠ->si];
      પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*silast->item);
      જો (sym == conflict_sym)
        તોડો;
      જો (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_સંખ્યા sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              જો (si_bfs_contains (ગાંઠ, sin))
                ચાલુ;
              si_bfs_ગાંઠ *next = si_bfs_new (sin, ગાંઠ);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          જો (nullable[sym - ntokens])
            {
              si_bfs_ગાંઠ *next = si_bfs_new (silast->trans, ગાંઠ);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  જો (gl_list_size (queue) == 0)
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

  માટે (si_bfs_ગાંઠ *n = ગાંઠ; n != ખાલી; n = n->parent)
    {
      state_item *si = &state_વસ્તુઓ[n->si];
      item_સંખ્યા *pos = si->item;
      જો (SI_PRODUCTION (si))
        {
          item_સંખ્યા *i = ખાલી;
          માટે (i = pos + 1; !item_સંખ્યા_is_rule_સંખ્યા (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          પ્રતીક_સંખ્યા lhs =
            rules[item_સંખ્યા_as_rule_સંખ્યા (*i)].lhs->સંખ્યા;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      નહીંતર
        {
          પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  પાછા result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
સ્થિર derivation *
complete_diverging_example (પ્રતીક_સંખ્યા conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  જો (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ગાંઠ_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  માટે (gl_list_ગાંઠ_t state_ગાંઠ = list_get_end (path);
       state_ગાંઠ != ખાલી;
       state_ગાંઠ = gl_list_previous_ગાંઠ (path, state_ગાંઠ))
    {
      state_item *si = (state_item *) gl_list_ગાંઠ_કિંમત (path, state_ગાંઠ);
      item_સંખ્યા *item = si->item;
      item_સંખ્યા pos = *item;
      // symbols after dot
      જો (gl_list_size (result) == 1 && !item_સંખ્યા_is_rule_સંખ્યા (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_સંખ્યા_as_પ્રતીક_સંખ્યા (pos)));
          lookahead_required = false;
        }
      item_સંખ્યા *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      માટે (state_item_સંખ્યા nsi = si - state_વસ્તુઓ;
           !item_સંખ્યા_is_rule_સંખ્યા (*i);
           ++i, nsi = state_વસ્તુઓ[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          જો (i == item)
            ચાલુ;
          પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*i);
          જો (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              ચાલુ;
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
          જો (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = ખાલી;
              માટે (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          નહીંતર જો (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          નહીંતર
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      અચલ rule *r = &rules[item_સંખ્યા_as_rule_સંખ્યા (*i)];
      // add derivations for symbols before dot
      માટે (i = item - 1; !item_સંખ્યા_is_rule_સંખ્યા (*i) && i >= ritem; i--)
        {
          gl_list_ગાંઠ_t p = gl_list_previous_ગાંઠ (path, state_ગાંઠ);
          જો (p)
            state_ગાંઠ = p;
          જો (deriv)
            {
              અચલ શૂન્ય *tmp_deriv = gl_list_ગાંઠ_કિંમત (derivs, deriv);
              deriv = gl_list_previous_ગાંઠ (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          નહીંતર
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->સંખ્યા, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  પાછા res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
સ્થિર state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_ગાંઠ_t tmp = gl_list_add_last (reduce_path, ખાલી);
  gl_list_ગાંઠ_t next_ગાંઠ = gl_list_previous_ગાંઠ (reduce_path, tmp);
  gl_list_ગાંઠ_t ગાંઠ = gl_list_previous_ગાંઠ (reduce_path, next_ગાંઠ);
  gl_list_remove_ગાંઠ (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
  // FIXME: bool paths_merged;
  માટે (; ગાંઠ != ખાલી; next_ગાંઠ = ગાંઠ,
       ગાંઠ = gl_list_previous_ગાંઠ (reduce_path, ગાંઠ))
    {
      state_item *refsi =
        (state_item *) gl_list_ગાંઠ_કિંમત (reduce_path, ગાંઠ);
      state_item *nextrefsi =
        (state_item *) gl_list_ગાંઠ_કિંમત (reduce_path, next_ગાંઠ);
      જો (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          ચાલુ;
        }
      // skip reduction items
      જો (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        ચાલુ;

      // bfs to find a shift to the right state
      si_bfs_ગાંઠ *init = si_bfs_new (si - state_વસ્તુઓ, ખાલી);
      si_bfs_ગાંઠ_list queue
        = gl_list_create (GL_LINKED_LIST, ખાલી, ખાલી,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (અચલ શૂન્ય **) &init);
      si_bfs_ગાંઠ *sis = ખાલી;
      state_item *prevsi = ખાલી;
      જ્યારે (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_ગાંઠ *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          જો (sis->si == 0)
            તોડો;

          state_item *શોધ_si = &state_વસ્તુઓ[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = શોધ_si->revs;
          bitset_iterator biter;
          state_item_સંખ્યા sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_વસ્તુઓ[sin];
              જો (SI_TRANSITION (શોધ_si))
                {
                  જો (prevsi->state == refsi->state)
                    જાઓ શોધ_end;
                }
              નહીંતર જો (!si_bfs_contains (sis, sin))
                {
                  si_bfs_ગાંઠ *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    શોધ_end:
      // prepend path to shift we found
      જો (sis)
        {
          gl_list_ગાંઠ_t ln = gl_list_add_first (result, &state_વસ્તુઓ[sis->si]);
          માટે (si_bfs_ગાંઠ *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_વસ્તુઓ[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  જો (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = ખાલી;
      માટે (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  પાછા result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
સ્થિર counterexample *
example_from_path (bool shift_reduce,
                   state_item_સંખ્યા itm2,
                   state_item_list shortest_path, પ્રતીક_સંખ્યા next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, ખાલી);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_વસ્તુઓ [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, ખાલી);
  gl_list_free (path_2);
  પાછા new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
પ્રકારવ્યાખ્યા સંરચના
{
  parse_state *states[2];
  પૂર્ણાંક complexity;
} શોધ_state;

સ્થિર શોધ_state *
initial_શોધ_state (state_item *conflict1, state_item *conflict2)
{
  શોધ_state *res = xmalloc (કદ *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  પાછા res;
}

સ્થિર શોધ_state *
new_શોધ_state (parse_state *ps1, parse_state *ps2, પૂર્ણાંક complexity)
{
  શોધ_state *res = xmalloc (કદ *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  પાછા res;
}

સ્થિર શોધ_state *
copy_શોધ_state (શોધ_state *parent)
{
  શોધ_state *res = xmalloc (કદ *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  પાછા res;
}

સ્થિર શૂન્ય
શોધ_state_free_children (શોધ_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

સ્થિર શૂન્ય
શોધ_state_free (શોધ_state *ss)
{
  જો (ss == ખાલી)
    પાછા;
  શોધ_state_free_children (ss);
  મુક્ત (ss);
}

/* For debugging traces.  */
સ્થિર શૂન્ય
શોધ_state_print (શોધ_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

પ્રકારવ્યાખ્યા gl_list_t શોધ_state_list;

સ્થિર અંતઃરેખ bool
શોધ_state_list_next (gl_list_iterator_t *it, શોધ_state **ss)
{
  અચલ શૂન્ય *p = ખાલી;
  bool res = gl_list_iterator_next (it, &p, ખાલી);
  જો (res)
    *ss = (શોધ_state*) p;
  નહીંતર
    gl_list_iterator_free (it);
  પાછા res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
સ્થિર અંતઃરેખ શૂન્ય
ss_set_parse_state (શોધ_state *ss, પૂર્ણાંક idx, parse_state *ps)
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
સ્થિર counterexample *
complete_diverging_examples (શોધ_state *ss,
                             પ્રતીક_સંખ્યા next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  માટે (પૂર્ણાંક i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  પાછા new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
પ્રકારવ્યાખ્યા સંરચના
{
  શોધ_state_list states;
  પૂર્ણાંક complexity;
} શોધ_state_bundle;

સ્થિર શૂન્ય
ssb_free (શોધ_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  મુક્ત (ssb);
}

સ્થિર size_t
ssb_hasher (શોધ_state_bundle *ssb)
{
  પાછા ssb->complexity;
}

સ્થિર પૂર્ણાંક
ssb_comp (અચલ શોધ_state_bundle *s1, અચલ શોધ_state_bundle *s2)
{
  પાછા s1->complexity - s2->complexity;
}

સ્થિર bool
ssb_equals (અચલ શોધ_state_bundle *s1, અચલ શોધ_state_bundle *s2)
{
  પાછા s1->complexity == s2->complexity;
}

પ્રકારવ્યાખ્યા gl_list_t ssb_list;

સ્થિર size_t
visited_hasher (અચલ શોધ_state *ss, size_t max)
{
  પાછા (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

સ્થિર bool
visited_comparator (અચલ શોધ_state *ss1, અચલ શોધ_state *ss2)
{
  પાછા parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
સ્થિર ssb_list ssb_queue;
સ્થિર Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
સ્થિર bitset scp_set = ખાલી;
/* The set of parser states used for the conflict reduction rule. */
સ્થિર bitset rpp_set = ખાલી;

સ્થિર શૂન્ય
ssb_append (શોધ_state *ss)
{
  જો (hash_lookup (visited, ss))
    {
      શોધ_state_free (ss);
      પાછા;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  શોધ_state_bundle *ssb = xmalloc (કદ *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ગાંઠ_t n = gl_list_શોધ (ssb_queue, ssb);
  જો (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી,
                              (gl_listelement_dispose_fn)શોધ_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  નહીંતર
    {
      મુક્ત (ssb);
      ssb = (શોધ_state_bundle *) gl_list_ગાંઠ_કિંમત (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
સ્થિર શૂન્ય
production_step (શોધ_state *ss, પૂર્ણાંક parser_state)
{
  અચલ state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  પ્રતીક_સંખ્યા other_sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  પૂર્ણાંક complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = ખાલી;
  માટે (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      શોધ_state *copy = copy_શોધ_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

સ્થિર અંતઃરેખ પૂર્ણાંક
reduction_cost (અચલ parse_state *ps)
{
  પૂર્ણાંક shifts;
  પૂર્ણાંક productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  પાછા SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

સ્થિર શોધ_state_list
reduction_step (શોધ_state *ss, અચલ item_સંખ્યા *conflict_item,
                પૂર્ણાંક parser_state, પૂર્ણાંક rule_len)
{
  (શૂન્ય) conflict_item; // FIXME: Unused
  શોધ_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, 1);

  parse_state *ps = ss->states[parser_state];
  અચલ state_item *si = parse_state_tail (ps);
  bitset પ્રતીક_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  અચલ state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  જો (item_સંખ્યા_is_પ્રતીક_સંખ્યા (*other_si->item))
    {
      પ્રતીક_સંખ્યા other_sym =
        item_સંખ્યા_as_પ્રતીક_સંખ્યા (*other_si->item);
      જો (!intersect_પ્રતીક (other_sym, પ્રતીક_set))
        પાછા result;
      પ્રતીક_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (પ્રતીક_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, પ્રતીક_set);
  parse_state *reduced_ps = ખાલી;
  માટે (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      શોધ_state *copy = copy_શોધ_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      પૂર્ણાંક r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  જો (પ્રતીક_set != si->lookahead)
    bitset_free (પ્રતીક_set);
  પાછા result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
સ્થિર શૂન્ય
શોધ_state_prepend (શોધ_state *ss, પ્રતીક_સંખ્યા sym, bitset guide)
{
  (શૂન્ય) sym; // FIXME: Unused.
  અચલ state_item *si1src = parse_state_head (ss->states[0]);
  અચલ state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  જો (prod1 != SI_PRODUCTION (si2src))
    {
      પૂર્ણાંક prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = ખાલી;
      માટે (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          અચલ state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->સંખ્યા);
          જો (!guided && !EXTENDED_શોધ)
            ચાલુ;

          શોધ_state *copy = copy_શોધ_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          જો (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      પાછા;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  અચલ પૂર્ણાંક complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = ખાલી;
  માટે (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      અચલ state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->સંખ્યા);
      જો (!guided1 && !EXTENDED_શોધ)
        ચાલુ;

      parse_state *ps2 = ખાલી;
      માટે (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          અચલ state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->સંખ્યા);
          જો (!guided2 && !EXTENDED_શોધ)
            ચાલુ;
          // Only consider prepend state items that share the same state.
          જો (psi1->state != psi2->state)
            ચાલુ;

          પૂર્ણાંક complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          જો (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_શોધ_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
સ્થિર bool
have_common_prefix (અચલ item_સંખ્યા *itm1, અચલ item_સંખ્યા *itm2)
{
  પૂર્ણાંક i = 0;
  માટે (; !item_સંખ્યા_is_rule_સંખ્યા (itm1[i]); ++i)
    જો (itm1[i] != itm2[i])
      પાછા false;
  પાછા item_સંખ્યા_is_rule_સંખ્યા (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
સ્થિર અચલ item_સંખ્યા *
item_rule_start (અચલ item_સંખ્યા *item)
{
  અચલ item_સંખ્યા *res = ખાલી;
  માટે (res = item;
       ritem < res && item_સંખ્યા_is_પ્રતીક_સંખ્યા (*(res - 1));
       --res)
    ચાલુ;
  પાછા res;
}

સ્થિર અચલ item_સંખ્યા *
item_rule_end (અચલ item_સંખ્યા *item)
{
  અચલ item_સંખ્યા *res = ખાલી;
  માટે (res = item; item_સંખ્યા_is_પ્રતીક_સંખ્યા (*res); ++res)
    ચાલુ;
  પાછા res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
સ્થિર અંતઃરેખ શૂન્ય
generate_next_states (શોધ_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  અચલ state_item *si1 = parse_state_tail (ps1);
  અચલ state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_સંખ્યા_is_rule_સંખ્યા (*si1->item);
  bool si2reduce = item_સંખ્યા_is_rule_સંખ્યા (*si2->item);
  જો (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      જો (*si1->item == *si2->item)
        {
          પૂર્ણાંક complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = ખાલી;
          parse_state *tps2 = ખાલી;
          માટે (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            માટે (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_શોધ_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  નહીંતર
    {
      અચલ item_સંખ્યા *rhs1 = item_rule_start (si1->item);
      અચલ item_સંખ્યા *rhe1 = item_rule_end (si1->item);
      પૂર્ણાંક len1 = rhe1 - rhs1;
      પૂર્ણાંક size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      અચલ item_સંખ્યા *rhs2 = item_rule_start (si2->item);
      અચલ item_સંખ્યા *rhe2 = item_rule_end (si2->item);
      પૂર્ણાંક len2 = rhe2 - rhs2;
      પૂર્ણાંક size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      જો (ready1 && ready2)
        {
          શોધ_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          શોધ_state *red1 = ખાલી;
          માટે (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               શોધ_state_list_next (&iter, &red1);
               )
            {
              શોધ_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              શોધ_state *red2 = ખાલી;
              માટે (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   શોધ_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              જો (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      નહીંતર જો (ready1)
        {
          શોધ_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          શોધ_state *red1 = ખાલી;
          માટે (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               શોધ_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      નહીંતર જો (ready2)
        {
          શોધ_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          શોધ_state *red2 = ખાલી;
          માટે (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               શોધ_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      નહીંતર
        {
          અચલ પ્રતીક_સંખ્યા sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          શોધ_state_prepend (ss, sym,
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
સ્થિર counterexample *
unifying_example (state_item_સંખ્યા itm1,
                  state_item_સંખ્યા itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, પ્રતીક_સંખ્યા next_sym)
{
  state_item *conflict1 = &state_વસ્તુઓ[itm1];
  state_item *conflict2 = &state_વસ્તુઓ[itm2];
  શોધ_state *initial = initial_શોધ_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, ખાલી, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) શોધ_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  શોધ_state *stage3result = ખાલી;
  counterexample *cex = ખાલી;
  જ્યારે (gl_list_size (ssb_queue) > 0)
    {
      અચલ શોધ_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      શોધ_state *ss = ખાલી;
      માટે (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           શોધ_state_list_next (&it, &ss);
           )
        {
          જો (trace_flag & trace_cex)
            શોધ_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          જો (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              અચલ state_item *si1src = parse_state_head (ps1);
              અચલ state_item *si2src = parse_state_head (ps2);
              જો (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  જો (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      જાઓ cex_શોધ_end;
                    }
                  જો (!stage3result)
                    stage3result = copy_શોધ_state (ss);
                }
            }
          જો (TIME_LIMIT_ENFORCED)
            {
              દ્વિગુણ time_passed = (gethrxtime () - start) / 1e9;
              જો (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              જો (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  જાઓ cex_શોધ_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_શોધ_end:;
  જો (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      જો (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      નહીંતર
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  જો (stage3result)
    શોધ_state_free (stage3result);
  પાછા cex;
}

સ્થિર xtime_t cumulative_time;

શૂન્ય
counterexample_init (શૂન્ય)
{
  // Check cex.timeout.
  {
    અચલ અક્ષર *variable = "cex.timeout";
    અક્ષર *cp = muscle_percent_define_get (variable);
    જો (*cp != '\0')
      {
        અક્ષર *end = ખાલી;
        દ્વિગુણ v = c_strtod (cp, &end);
        જો (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        નહીંતર
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    મુક્ત (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_વસ્તુઓ_init ();
}


શૂન્ય
counterexample_free (શૂન્ય)
{
  જો (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_વસ્તુઓ_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
સ્થિર શૂન્ય
counterexample_report (state_item_સંખ્યા itm1, state_item_સંખ્યા itm2,
                       પ્રતીક_સંખ્યા next_sym, bool shift_reduce,
                       FILE *out, અચલ અક્ષર *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  અચલ rule *reduce_rule = item_rule (state_વસ્તુઓ[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = ખાલી;
  માટે (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->સંખ્યા);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      જો (reduce_prod_reached)
        bitset_set (rpp_set, si->state->સંખ્યા);
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
સ્થિર શૂન્ય
counterexample_report_shift_reduce (state_item_સંખ્યા itm1, state_item_સંખ્યા itm2,
                                    પ્રતીક_સંખ્યા next_sym,
                                    FILE *out, અચલ અક્ષર *prefix)
{
  જો (out == stderr)
    complain (ખાલી, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  નહીંતર
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  જો (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_વસ્તુઓ[itm1], out, prefix);
      state_item_print (&state_વસ્તુઓ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

સ્થિર શૂન્ય
counterexample_report_reduce_reduce (state_item_સંખ્યા itm1, state_item_સંખ્યા itm2,
                                     bitset conflict_syms,
                                     FILE *out, અચલ અક્ષર *prefix)
{
  {
    સંરચના obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_સંખ્યા sym;
    અચલ અક્ષર *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    અક્ષર *tokens = obstack_finish0 (&obstack);
    જો (out == stderr)
      complain (ખાલી, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_ગણતરી (conflict_syms)),
                tokens);
    નહીંતર
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_ગણતરી (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, ખાલી);
  }
  // In the report, print the items.
  જો (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_વસ્તુઓ[itm1], out, prefix);
      state_item_print (&state_વસ્તુઓ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

સ્થિર state_item_સંખ્યા
find_state_item_સંખ્યા (અચલ rule *r, state_સંખ્યા sn)
{
  માટે (state_item_સંખ્યા i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    જો (!SI_DISABLED (i)
        && item_સંખ્યા_as_rule_સંખ્યા (*state_વસ્તુઓ[i].item) == r->સંખ્યા)
      પાછા i;
  abort ();
}

શૂન્ય
counterexample_report_state (અચલ state *s, FILE *out, અચલ અક્ષર *prefix)
{
  અચલ state_સંખ્યા sn = s->સંખ્યા;
  અચલ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
    {
      અચલ rule *r1 = reds->rules[i];
      અચલ state_item_સંખ્યા c1 = find_state_item_સંખ્યા (r1, sn);
      માટે (state_item_સંખ્યા c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        જો (!SI_DISABLED (c2))
          {
            item_સંખ્યા conf = *state_વસ્તુઓ[c2].item;
            જો (item_સંખ્યા_is_પ્રતીક_સંખ્યા (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      માટે (પૂર્ણાંક j = i+1; j < reds->અંક; ++j)
        {
          અચલ rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          જો (!bitset_empty_p (lookaheads))
            માટે (state_item_સંખ્યા c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              જો (!SI_DISABLED (c2)
                  && item_rule (state_વસ્તુઓ[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  તોડો;
                }
        }
    }
  bitset_free (lookaheads);
}
