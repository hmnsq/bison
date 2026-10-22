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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "counterexample.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <c-strtod.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <gethrxtime.h>
#ଅନ୍ତର୍ଭାଵ <gl_linked_list.h>
#ଅନ୍ତର୍ଭାଵ <gl_rbtreehash_list.h>
#ଅନ୍ତର୍ଭାଵ <hash.h>
#ଅନ୍ତର୍ଭାଵ <mbswidth.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <textstyle.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "derivation.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lssi.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"
#ଅନ୍ତର୍ଭାଵ "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_ସନ୍ଧାନ false

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
ସ୍ଥିର ଦ୍ୱିଗୁଣ time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
ସ୍ଥିର gl_list_ଗଣ୍ଠି_t
list_get_end (gl_list_t list)
{
  gl_list_ଗଣ୍ଠି_t sentinel = gl_list_add_last (list, ଖାଲି);
  gl_list_ଗଣ୍ଠି_t res = gl_list_previous_ଗଣ୍ଠି (list, sentinel);
  gl_list_remove_ଗଣ୍ଠି (list, sentinel);
  ଫେରନ୍ତୁ res;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ସ୍ଥିର counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (ଆକାର *res);
  res->shift_reduce = shift_reduce;
  ଯଦି (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  ଅନ୍ୟଥା
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  ମୁକ୍ତ (cex);
}

ସ୍ଥିର ଶୂନ୍ୟ
counterexample_print (ଅଚଳ counterexample *cex, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  ଅଚଳ bool flat = getenv ("YYFLAT");
  ଅଚଳ ଅକ୍ଷର *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ଅଚଳ ଅକ୍ଷର *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ଅଚଳ ଅକ୍ଷର *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  ଅଚଳ ଅକ୍ଷର *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  ଯଦି (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  ଅନ୍ୟଥା
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  ଯଦି (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  ଅନ୍ୟଥା
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  ଯଦି (!cex->unifying || is_styled (stderr))
    {
      ଯଦି (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      ଅନ୍ୟଥା
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  ଯଦି (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  ଅନ୍ୟଥା
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  ଯଦି (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
ସଂରଚନା si_bfs_ଗଣ୍ଠି;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା si_bfs_ଗଣ୍ଠି
{
  state_item_ସଂଖ୍ୟା si;
  ସଂରଚନା si_bfs_ଗଣ୍ଠି *parent;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା reference_ଗଣନା;
} si_bfs_ଗଣ୍ଠି;

ସ୍ଥିର si_bfs_ଗଣ୍ଠି *
si_bfs_new (state_item_ସଂଖ୍ୟା si, si_bfs_ଗଣ୍ଠି *parent)
{
  si_bfs_ଗଣ୍ଠି *res = xcalloc (1, ଆକାର *res);
  res->si = si;
  res->parent = parent;
  res->reference_ଗଣନା = 1;
  ଯଦି (parent)
    ++parent->reference_ଗଣନା;
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର bool
si_bfs_contains (ଅଚଳ si_bfs_ଗଣ୍ଠି *n, state_item_ସଂଖ୍ୟା sin)
{
  ପାଇଁ (ଅଚଳ si_bfs_ଗଣ୍ଠି *ସନ୍ଧାନ = n; ସନ୍ଧାନ != ଖାଲି; ସନ୍ଧାନ = ସନ୍ଧାନ->parent)
    ଯଦି (ସନ୍ଧାନ->si == sin)
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
si_bfs_free (si_bfs_ଗଣ୍ଠି *n)
{
  ଯଦି (n == ଖାଲି)
    ଫେରନ୍ତୁ;
  --n->reference_ଗଣନା;
  ଯଦି (n->reference_ଗଣନା == 0)
    {
      si_bfs_free (n->parent);
      ମୁକ୍ତ (n);
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା gl_list_t si_bfs_ଗଣ୍ଠି_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ derivation_list
expand_to_conflict (state_item_ସଂଖ୍ୟା start, ପ୍ରତୀକ_ସଂଖ୍ୟା conflict_sym)
{
  si_bfs_ଗଣ୍ଠି *init = si_bfs_new (start, ଖାଲି);

  si_bfs_ଗଣ୍ଠି_list queue
    = gl_list_create (GL_LINKED_LIST, ଖାଲି, ଖାଲି,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ଅଚଳ ଶୂନ୍ୟ **) &init);
  si_bfs_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;
  // breadth-first search for a path of productions to the conflict symbol
  ଯେତେବେଳେ (gl_list_size (queue) > 0)
    {
      ଗଣ୍ଠି = (si_bfs_ଗଣ୍ଠି *) gl_list_get_at (queue, 0);
      state_item *silast = &state_ବସ୍ତୁଗୁଡ଼ିକ[ଗଣ୍ଠି->si];
      ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*silast->item);
      ଯଦି (sym == conflict_sym)
        ଭାଙ୍ଗ;
      ଯଦି (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_ସଂଖ୍ୟା sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              ଯଦି (si_bfs_contains (ଗଣ୍ଠି, sin))
                ଜାରି;
              si_bfs_ଗଣ୍ଠି *next = si_bfs_new (sin, ଗଣ୍ଠି);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          ଯଦି (nullable[sym - ntokens])
            {
              si_bfs_ଗଣ୍ଠି *next = si_bfs_new (silast->trans, ଗଣ୍ଠି);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  ଯଦି (gl_list_size (queue) == 0)
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

  ପାଇଁ (si_bfs_ଗଣ୍ଠି *n = ଗଣ୍ଠି; n != ଖାଲି; n = n->parent)
    {
      state_item *si = &state_ବସ୍ତୁଗୁଡ଼ିକ[n->si];
      item_ସଂଖ୍ୟା *pos = si->item;
      ଯଦି (SI_PRODUCTION (si))
        {
          item_ସଂଖ୍ୟା *i = ଖାଲି;
          ପାଇଁ (i = pos + 1; !item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          ପ୍ରତୀକ_ସଂଖ୍ୟା lhs =
            rules[item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (*i)].lhs->ସଂଖ୍ୟା;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      ଅନ୍ୟଥା
        {
          ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  ଫେରନ୍ତୁ result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
ସ୍ଥିର derivation *
complete_diverging_example (ପ୍ରତୀକ_ସଂଖ୍ୟା conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  ଯଦି (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ଗଣ୍ଠି_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  ପାଇଁ (gl_list_ଗଣ୍ଠି_t state_ଗଣ୍ଠି = list_get_end (path);
       state_ଗଣ୍ଠି != ଖାଲି;
       state_ଗଣ୍ଠି = gl_list_previous_ଗଣ୍ଠି (path, state_ଗଣ୍ଠି))
    {
      state_item *si = (state_item *) gl_list_ଗଣ୍ଠି_ମୂଲ୍ୟ (path, state_ଗଣ୍ଠି);
      item_ସଂଖ୍ୟା *item = si->item;
      item_ସଂଖ୍ୟା pos = *item;
      // symbols after dot
      ଯଦି (gl_list_size (result) == 1 && !item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (pos)));
          lookahead_required = false;
        }
      item_ସଂଖ୍ୟା *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      ପାଇଁ (state_item_ସଂଖ୍ୟା nsi = si - state_ବସ୍ତୁଗୁଡ଼ିକ;
           !item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*i);
           ++i, nsi = state_ବସ୍ତୁଗୁଡ଼ିକ[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          ଯଦି (i == item)
            ଜାରି;
          ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*i);
          ଯଦି (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              ଜାରି;
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
          ଯଦି (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = ଖାଲି;
              ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          ଅନ୍ୟଥା ଯଦି (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          ଅନ୍ୟଥା
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      ଅଚଳ rule *r = &rules[item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (*i)];
      // add derivations for symbols before dot
      ପାଇଁ (i = item - 1; !item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*i) && i >= ritem; i--)
        {
          gl_list_ଗଣ୍ଠି_t p = gl_list_previous_ଗଣ୍ଠି (path, state_ଗଣ୍ଠି);
          ଯଦି (p)
            state_ଗଣ୍ଠି = p;
          ଯଦି (deriv)
            {
              ଅଚଳ ଶୂନ୍ୟ *tmp_deriv = gl_list_ଗଣ୍ଠି_ମୂଲ୍ୟ (derivs, deriv);
              deriv = gl_list_previous_ଗଣ୍ଠି (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          ଅନ୍ୟଥା
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->ସଂଖ୍ୟା, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  ଫେରନ୍ତୁ res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ସ୍ଥିର state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_ଗଣ୍ଠି_t tmp = gl_list_add_last (reduce_path, ଖାଲି);
  gl_list_ଗଣ୍ଠି_t next_ଗଣ୍ଠି = gl_list_previous_ଗଣ୍ଠି (reduce_path, tmp);
  gl_list_ଗଣ୍ଠି_t ଗଣ୍ଠି = gl_list_previous_ଗଣ୍ଠି (reduce_path, next_ଗଣ୍ଠି);
  gl_list_remove_ଗଣ୍ଠି (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
  // FIXME: bool paths_merged;
  ପାଇଁ (; ଗଣ୍ଠି != ଖାଲି; next_ଗଣ୍ଠି = ଗଣ୍ଠି,
       ଗଣ୍ଠି = gl_list_previous_ଗଣ୍ଠି (reduce_path, ଗଣ୍ଠି))
    {
      state_item *refsi =
        (state_item *) gl_list_ଗଣ୍ଠି_ମୂଲ୍ୟ (reduce_path, ଗଣ୍ଠି);
      state_item *nextrefsi =
        (state_item *) gl_list_ଗଣ୍ଠି_ମୂଲ୍ୟ (reduce_path, next_ଗଣ୍ଠି);
      ଯଦି (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          ଜାରି;
        }
      // skip reduction items
      ଯଦି (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        ଜାରି;

      // bfs to find a shift to the right state
      si_bfs_ଗଣ୍ଠି *init = si_bfs_new (si - state_ବସ୍ତୁଗୁଡ଼ିକ, ଖାଲି);
      si_bfs_ଗଣ୍ଠି_list queue
        = gl_list_create (GL_LINKED_LIST, ଖାଲି, ଖାଲି,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ଅଚଳ ଶୂନ୍ୟ **) &init);
      si_bfs_ଗଣ୍ଠି *sis = ଖାଲି;
      state_item *prevsi = ଖାଲି;
      ଯେତେବେଳେ (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_ଗଣ୍ଠି *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          ଯଦି (sis->si == 0)
            ଭାଙ୍ଗ;

          state_item *ସନ୍ଧାନ_si = &state_ବସ୍ତୁଗୁଡ଼ିକ[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = ସନ୍ଧାନ_si->revs;
          bitset_iterator biter;
          state_item_ସଂଖ୍ୟା sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_ବସ୍ତୁଗୁଡ଼ିକ[sin];
              ଯଦି (SI_TRANSITION (ସନ୍ଧାନ_si))
                {
                  ଯଦି (prevsi->state == refsi->state)
                    ଯାଅ ସନ୍ଧାନ_end;
                }
              ଅନ୍ୟଥା ଯଦି (!si_bfs_contains (sis, sin))
                {
                  si_bfs_ଗଣ୍ଠି *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    ସନ୍ଧାନ_end:
      // prepend path to shift we found
      ଯଦି (sis)
        {
          gl_list_ଗଣ୍ଠି_t ln = gl_list_add_first (result, &state_ବସ୍ତୁଗୁଡ଼ିକ[sis->si]);
          ପାଇଁ (si_bfs_ଗଣ୍ଠି *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_ବସ୍ତୁଗୁଡ଼ିକ[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  ଯଦି (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = ଖାଲି;
      ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  ଫେରନ୍ତୁ result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ସ୍ଥିର counterexample *
example_from_path (bool shift_reduce,
                   state_item_ସଂଖ୍ୟା itm2,
                   state_item_list shortest_path, ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, ଖାଲି);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_ବସ୍ତୁଗୁଡ଼ିକ [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, ଖାଲି);
  gl_list_free (path_2);
  ଫେରନ୍ତୁ new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  parse_state *states[2];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity;
} ସନ୍ଧାନ_state;

ସ୍ଥିର ସନ୍ଧାନ_state *
initial_ସନ୍ଧାନ_state (state_item *conflict1, state_item *conflict2)
{
  ସନ୍ଧାନ_state *res = xmalloc (ଆକାର *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ସନ୍ଧାନ_state *
new_ସନ୍ଧାନ_state (parse_state *ps1, parse_state *ps2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity)
{
  ସନ୍ଧାନ_state *res = xmalloc (ଆକାର *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ସନ୍ଧାନ_state *
copy_ସନ୍ଧାନ_state (ସନ୍ଧାନ_state *parent)
{
  ସନ୍ଧାନ_state *res = xmalloc (ଆକାର *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
ସନ୍ଧାନ_state_free_children (ସନ୍ଧାନ_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

ସ୍ଥିର ଶୂନ୍ୟ
ସନ୍ଧାନ_state_free (ସନ୍ଧାନ_state *ss)
{
  ଯଦି (ss == ଖାଲି)
    ଫେରନ୍ତୁ;
  ସନ୍ଧାନ_state_free_children (ss);
  ମୁକ୍ତ (ss);
}

/* For debugging traces.  */
ସ୍ଥିର ଶୂନ୍ୟ
ସନ୍ଧାନ_state_print (ସନ୍ଧାନ_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା gl_list_t ସନ୍ଧାନ_state_list;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ସନ୍ଧାନ_state_list_next (gl_list_iterator_t *it, ସନ୍ଧାନ_state **ss)
{
  ଅଚଳ ଶୂନ୍ୟ *p = ଖାଲି;
  bool res = gl_list_iterator_next (it, &p, ଖାଲି);
  ଯଦି (res)
    *ss = (ସନ୍ଧାନ_state*) p;
  ଅନ୍ୟଥା
    gl_list_iterator_free (it);
  ଫେରନ୍ତୁ res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
ss_set_parse_state (ସନ୍ଧାନ_state *ss, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx, parse_state *ps)
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
ସ୍ଥିର counterexample *
complete_diverging_examples (ସନ୍ଧାନ_state *ss,
                             ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  ଫେରନ୍ତୁ new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  ସନ୍ଧାନ_state_list states;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity;
} ସନ୍ଧାନ_state_bundle;

ସ୍ଥିର ଶୂନ୍ୟ
ssb_free (ସନ୍ଧାନ_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  ମୁକ୍ତ (ssb);
}

ସ୍ଥିର size_t
ssb_hasher (ସନ୍ଧାନ_state_bundle *ssb)
{
  ଫେରନ୍ତୁ ssb->complexity;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ssb_comp (ଅଚଳ ସନ୍ଧାନ_state_bundle *s1, ଅଚଳ ସନ୍ଧାନ_state_bundle *s2)
{
  ଫେରନ୍ତୁ s1->complexity - s2->complexity;
}

ସ୍ଥିର bool
ssb_equals (ଅଚଳ ସନ୍ଧାନ_state_bundle *s1, ଅଚଳ ସନ୍ଧାନ_state_bundle *s2)
{
  ଫେରନ୍ତୁ s1->complexity == s2->complexity;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା gl_list_t ssb_list;

ସ୍ଥିର size_t
visited_hasher (ଅଚଳ ସନ୍ଧାନ_state *ss, size_t max)
{
  ଫେରନ୍ତୁ (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

ସ୍ଥିର bool
visited_comparator (ଅଚଳ ସନ୍ଧାନ_state *ss1, ଅଚଳ ସନ୍ଧାନ_state *ss2)
{
  ଫେରନ୍ତୁ parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
ସ୍ଥିର ssb_list ssb_queue;
ସ୍ଥିର Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
ସ୍ଥିର bitset scp_set = ଖାଲି;
/* The set of parser states used for the conflict reduction rule. */
ସ୍ଥିର bitset rpp_set = ଖାଲି;

ସ୍ଥିର ଶୂନ୍ୟ
ssb_append (ସନ୍ଧାନ_state *ss)
{
  ଯଦି (hash_lookup (visited, ss))
    {
      ସନ୍ଧାନ_state_free (ss);
      ଫେରନ୍ତୁ;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  ସନ୍ଧାନ_state_bundle *ssb = xmalloc (ଆକାର *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ଗଣ୍ଠି_t n = gl_list_ସନ୍ଧାନ (ssb_queue, ssb);
  ଯଦି (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି,
                              (gl_listelement_dispose_fn)ସନ୍ଧାନ_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  ଅନ୍ୟଥା
    {
      ମୁକ୍ତ (ssb);
      ssb = (ସନ୍ଧାନ_state_bundle *) gl_list_ଗଣ୍ଠି_ମୂଲ୍ୟ (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ସ୍ଥିର ଶୂନ୍ୟ
production_step (ସନ୍ଧାନ_state *ss, ପୂର୍ଣ୍ଣସଂଖ୍ୟା parser_state)
{
  ଅଚଳ state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  ପ୍ରତୀକ_ସଂଖ୍ୟା other_sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      ସନ୍ଧାନ_state *copy = copy_ସନ୍ଧାନ_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
reduction_cost (ଅଚଳ parse_state *ps)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା shifts;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  ଫେରନ୍ତୁ SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

ସ୍ଥିର ସନ୍ଧାନ_state_list
reduction_step (ସନ୍ଧାନ_state *ss, ଅଚଳ item_ସଂଖ୍ୟା *conflict_item,
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା parser_state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_len)
{
  (ଶୂନ୍ୟ) conflict_item; // FIXME: Unused
  ସନ୍ଧାନ_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, 1);

  parse_state *ps = ss->states[parser_state];
  ଅଚଳ state_item *si = parse_state_tail (ps);
  bitset ପ୍ରତୀକ_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  ଅଚଳ state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  ଯଦି (item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*other_si->item))
    {
      ପ୍ରତୀକ_ସଂଖ୍ୟା other_sym =
        item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*other_si->item);
      ଯଦି (!intersect_ପ୍ରତୀକ (other_sym, ପ୍ରତୀକ_set))
        ଫେରନ୍ତୁ result;
      ପ୍ରତୀକ_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (ପ୍ରତୀକ_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, ପ୍ରତୀକ_set);
  parse_state *reduced_ps = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      ସନ୍ଧାନ_state *copy = copy_ସନ୍ଧାନ_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  ଯଦି (ପ୍ରତୀକ_set != si->lookahead)
    bitset_free (ପ୍ରତୀକ_set);
  ଫେରନ୍ତୁ result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ସ୍ଥିର ଶୂନ୍ୟ
ସନ୍ଧାନ_state_prepend (ସନ୍ଧାନ_state *ss, ପ୍ରତୀକ_ସଂଖ୍ୟା sym, bitset guide)
{
  (ଶୂନ୍ୟ) sym; // FIXME: Unused.
  ଅଚଳ state_item *si1src = parse_state_head (ss->states[0]);
  ଅଚଳ state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  ଯଦି (prod1 != SI_PRODUCTION (si2src))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = ଖାଲି;
      ପାଇଁ (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          ଅଚଳ state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->ସଂଖ୍ୟା);
          ଯଦି (!guided && !EXTENDED_ସନ୍ଧାନ)
            ଜାରି;

          ସନ୍ଧାନ_state *copy = copy_ସନ୍ଧାନ_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          ଯଦି (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      ଫେରନ୍ତୁ;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      ଅଚଳ state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->ସଂଖ୍ୟା);
      ଯଦି (!guided1 && !EXTENDED_ସନ୍ଧାନ)
        ଜାରି;

      parse_state *ps2 = ଖାଲି;
      ପାଇଁ (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          ଅଚଳ state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->ସଂଖ୍ୟା);
          ଯଦି (!guided2 && !EXTENDED_ସନ୍ଧାନ)
            ଜାରି;
          // Only consider prepend state items that share the same state.
          ଯଦି (psi1->state != psi2->state)
            ଜାରି;

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          ଯଦି (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_ସନ୍ଧାନ_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
ସ୍ଥିର bool
have_common_prefix (ଅଚଳ item_ସଂଖ୍ୟା *itm1, ଅଚଳ item_ସଂଖ୍ୟା *itm2)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
  ପାଇଁ (; !item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (itm1[i]); ++i)
    ଯଦି (itm1[i] != itm2[i])
      ଫେରନ୍ତୁ false;
  ଫେରନ୍ତୁ item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ସ୍ଥିର ଅଚଳ item_ସଂଖ୍ୟା *
item_rule_start (ଅଚଳ item_ସଂଖ୍ୟା *item)
{
  ଅଚଳ item_ସଂଖ୍ୟା *res = ଖାଲି;
  ପାଇଁ (res = item;
       ritem < res && item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*(res - 1));
       --res)
    ଜାରି;
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଅଚଳ item_ସଂଖ୍ୟା *
item_rule_end (ଅଚଳ item_ସଂଖ୍ୟା *item)
{
  ଅଚଳ item_ସଂଖ୍ୟା *res = ଖାଲି;
  ପାଇଁ (res = item; item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (*res); ++res)
    ଜାରି;
  ଫେରନ୍ତୁ res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
generate_next_states (ସନ୍ଧାନ_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  ଅଚଳ state_item *si1 = parse_state_tail (ps1);
  ଅଚଳ state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*si1->item);
  bool si2reduce = item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*si2->item);
  ଯଦି (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      ଯଦି (*si1->item == *si2->item)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = ଖାଲି;
          parse_state *tps2 = ଖାଲି;
          ପାଇଁ (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            ପାଇଁ (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_ସନ୍ଧାନ_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  ଅନ୍ୟଥା
    {
      ଅଚଳ item_ସଂଖ୍ୟା *rhs1 = item_rule_start (si1->item);
      ଅଚଳ item_ସଂଖ୍ୟା *rhe1 = item_rule_end (si1->item);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len1 = rhe1 - rhs1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ଅଚଳ item_ସଂଖ୍ୟା *rhs2 = item_rule_start (si2->item);
      ଅଚଳ item_ସଂଖ୍ୟା *rhe2 = item_rule_end (si2->item);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len2 = rhe2 - rhs2;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      ଯଦି (ready1 && ready2)
        {
          ସନ୍ଧାନ_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          ସନ୍ଧାନ_state *red1 = ଖାଲି;
          ପାଇଁ (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ସନ୍ଧାନ_state_list_next (&iter, &red1);
               )
            {
              ସନ୍ଧାନ_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              ସନ୍ଧାନ_state *red2 = ଖାଲି;
              ପାଇଁ (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   ସନ୍ଧାନ_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              ଯଦି (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      ଅନ୍ୟଥା ଯଦି (ready1)
        {
          ସନ୍ଧାନ_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          ସନ୍ଧାନ_state *red1 = ଖାଲି;
          ପାଇଁ (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ସନ୍ଧାନ_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      ଅନ୍ୟଥା ଯଦି (ready2)
        {
          ସନ୍ଧାନ_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          ସନ୍ଧାନ_state *red2 = ଖାଲି;
          ପାଇଁ (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               ସନ୍ଧାନ_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      ଅନ୍ୟଥା
        {
          ଅଚଳ ପ୍ରତୀକ_ସଂଖ୍ୟା sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          ସନ୍ଧାନ_state_prepend (ss, sym,
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
ସ୍ଥିର counterexample *
unifying_example (state_item_ସଂଖ୍ୟା itm1,
                  state_item_ସଂଖ୍ୟା itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym)
{
  state_item *conflict1 = &state_ବସ୍ତୁଗୁଡ଼ିକ[itm1];
  state_item *conflict2 = &state_ବସ୍ତୁଗୁଡ଼ିକ[itm2];
  ସନ୍ଧାନ_state *initial = initial_ସନ୍ଧାନ_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, ଖାଲି, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) ସନ୍ଧାନ_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  ସନ୍ଧାନ_state *stage3result = ଖାଲି;
  counterexample *cex = ଖାଲି;
  ଯେତେବେଳେ (gl_list_size (ssb_queue) > 0)
    {
      ଅଚଳ ସନ୍ଧାନ_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      ସନ୍ଧାନ_state *ss = ଖାଲି;
      ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           ସନ୍ଧାନ_state_list_next (&it, &ss);
           )
        {
          ଯଦି (trace_flag & trace_cex)
            ସନ୍ଧାନ_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          ଯଦି (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ଅଚଳ state_item *si1src = parse_state_head (ps1);
              ଅଚଳ state_item *si2src = parse_state_head (ps2);
              ଯଦି (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  ଯଦି (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      ଯାଅ cex_ସନ୍ଧାନ_end;
                    }
                  ଯଦି (!stage3result)
                    stage3result = copy_ସନ୍ଧାନ_state (ss);
                }
            }
          ଯଦି (TIME_LIMIT_ENFORCED)
            {
              ଦ୍ୱିଗୁଣ time_passed = (gethrxtime () - start) / 1e9;
              ଯଦି (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              ଯଦି (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  ଯାଅ cex_ସନ୍ଧାନ_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_ସନ୍ଧାନ_end:;
  ଯଦି (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      ଯଦି (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      ଅନ୍ୟଥା
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  ଯଦି (stage3result)
    ସନ୍ଧାନ_state_free (stage3result);
  ଫେରନ୍ତୁ cex;
}

ସ୍ଥିର xtime_t cumulative_time;

ଶୂନ୍ୟ
counterexample_init (ଶୂନ୍ୟ)
{
  // Check cex.timeout.
  {
    ଅଚଳ ଅକ୍ଷର *variable = "cex.timeout";
    ଅକ୍ଷର *cp = muscle_percent_define_get (variable);
    ଯଦି (*cp != '\0')
      {
        ଅକ୍ଷର *end = ଖାଲି;
        ଦ୍ୱିଗୁଣ v = c_strtod (cp, &end);
        ଯଦି (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        ଅନ୍ୟଥା
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    ମୁକ୍ତ (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_ବସ୍ତୁଗୁଡ଼ିକ_init ();
}


ଶୂନ୍ୟ
counterexample_free (ଶୂନ୍ୟ)
{
  ଯଦି (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_ବସ୍ତୁଗୁଡ଼ିକ_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ସ୍ଥିର ଶୂନ୍ୟ
counterexample_report (state_item_ସଂଖ୍ୟା itm1, state_item_ସଂଖ୍ୟା itm2,
                       ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym, bool shift_reduce,
                       FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  ଅଚଳ rule *reduce_rule = item_rule (state_ବସ୍ତୁଗୁଡ଼ିକ[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->ସଂଖ୍ୟା);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      ଯଦି (reduce_prod_reached)
        bitset_set (rpp_set, si->state->ସଂଖ୍ୟା);
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
ସ୍ଥିର ଶୂନ୍ୟ
counterexample_report_shift_reduce (state_item_ସଂଖ୍ୟା itm1, state_item_ସଂଖ୍ୟା itm2,
                                    ପ୍ରତୀକ_ସଂଖ୍ୟା next_sym,
                                    FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  ଯଦି (out == stderr)
    complain (ଖାଲି, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  ଅନ୍ୟଥା
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  ଯଦି (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_ବସ୍ତୁଗୁଡ଼ିକ[itm1], out, prefix);
      state_item_print (&state_ବସ୍ତୁଗୁଡ଼ିକ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

ସ୍ଥିର ଶୂନ୍ୟ
counterexample_report_reduce_reduce (state_item_ସଂଖ୍ୟା itm1, state_item_ସଂଖ୍ୟା itm2,
                                     bitset conflict_syms,
                                     FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  {
    ସଂରଚନା obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_ସଂଖ୍ୟା sym;
    ଅଚଳ ଅକ୍ଷର *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    ଅକ୍ଷର *tokens = obstack_finish0 (&obstack);
    ଯଦି (out == stderr)
      complain (ଖାଲି, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_ଗଣନା (conflict_syms)),
                tokens);
    ଅନ୍ୟଥା
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_ଗଣନା (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, ଖାଲି);
  }
  // In the report, print the items.
  ଯଦି (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_ବସ୍ତୁଗୁଡ଼ିକ[itm1], out, prefix);
      state_item_print (&state_ବସ୍ତୁଗୁଡ଼ିକ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

ସ୍ଥିର state_item_ସଂଖ୍ୟା
find_state_item_ସଂଖ୍ୟା (ଅଚଳ rule *r, state_ସଂଖ୍ୟା sn)
{
  ପାଇଁ (state_item_ସଂଖ୍ୟା i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    ଯଦି (!SI_DISABLED (i)
        && item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (*state_ବସ୍ତୁଗୁଡ଼ିକ[i].item) == r->ସଂଖ୍ୟା)
      ଫେରନ୍ତୁ i;
  abort ();
}

ଶୂନ୍ୟ
counterexample_report_state (ଅଚଳ state *s, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix)
{
  ଅଚଳ state_ସଂଖ୍ୟା sn = s->ସଂଖ୍ୟା;
  ଅଚଳ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
    {
      ଅଚଳ rule *r1 = reds->rules[i];
      ଅଚଳ state_item_ସଂଖ୍ୟା c1 = find_state_item_ସଂଖ୍ୟା (r1, sn);
      ପାଇଁ (state_item_ସଂଖ୍ୟା c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        ଯଦି (!SI_DISABLED (c2))
          {
            item_ସଂଖ୍ୟା conf = *state_ବସ୍ତୁଗୁଡ଼ିକ[c2].item;
            ଯଦି (item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i+1; j < reds->ଅଙ୍କ; ++j)
        {
          ଅଚଳ rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          ଯଦି (!bitset_empty_p (lookaheads))
            ପାଇଁ (state_item_ସଂଖ୍ୟା c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              ଯଦି (!SI_DISABLED (c2)
                  && item_rule (state_ବସ୍ତୁଗୁଡ଼ିକ[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  ଭାଙ୍ଗ;
                }
        }
    }
  bitset_free (lookaheads);
}
