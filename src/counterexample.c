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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "counterexample.h"

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <c-strtod.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <gethrxtime.h>
#অন্তর্ভাব <gl_linked_list.h>
#অন্তর্ভাব <gl_rbtreehash_list.h>
#অন্তর্ভাব <hash.h>
#অন্তর্ভাব <mbswidth.h>
#অন্তর্ভাব <quote.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <textstyle.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "derivation.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lssi.h"
#অন্তর্ভাব "muscle-tab.h"
#অন্তর্ভাব "nullable.h"
#অন্তর্ভাব "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_অনুসন্ধান false

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
স্থির দ্বিগুণ time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
স্থির gl_list_গ্রন্থি_t
list_get_end (gl_list_t list)
{
  gl_list_গ্রন্থি_t sentinel = gl_list_add_last (list, ফাঁকা);
  gl_list_গ্রন্থি_t res = gl_list_previous_গ্রন্থি (list, sentinel);
  gl_list_remove_গ্রন্থি (list, sentinel);
  ফেরত res;
}

প্রকারলেখ কাঠামো
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

স্থির counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (আকার *res);
  res->shift_reduce = shift_reduce;
  যদি (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  অন্যথা
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  ফেরত res;
}

স্থির শূন্য
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  মুক্ত (cex);
}

স্থির শূন্য
counterexample_print (ধ্রুবক counterexample *cex, FILE *out, ধ্রুবক অক্ষর *prefix)
{
  ধ্রুবক bool flat = getenv ("YYFLAT");
  ধ্রুবক অক্ষর *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ধ্রুবক অক্ষর *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ধ্রুবক অক্ষর *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  ধ্রুবক অক্ষর *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ধ্রুবক পূর্ণসংখ্যা width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  যদি (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  অন্যথা
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  যদি (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  অন্যথা
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  যদি (!cex->unifying || is_styled (stderr))
    {
      যদি (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      অন্যথা
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  যদি (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  অন্যথা
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  যদি (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
কাঠামো si_bfs_গ্রন্থি;
প্রকারলেখ কাঠামো si_bfs_গ্রন্থি
{
  state_item_সংখ্যা si;
  কাঠামো si_bfs_গ্রন্থি *parent;
  পূর্ণসংখ্যা reference_গুনতি;
} si_bfs_গ্রন্থি;

স্থির si_bfs_গ্রন্থি *
si_bfs_new (state_item_সংখ্যা si, si_bfs_গ্রন্থি *parent)
{
  si_bfs_গ্রন্থি *res = xcalloc (1, আকার *res);
  res->si = si;
  res->parent = parent;
  res->reference_গুনতি = 1;
  যদি (parent)
    ++parent->reference_গুনতি;
  ফেরত res;
}

স্থির bool
si_bfs_contains (ধ্রুবক si_bfs_গ্রন্থি *n, state_item_সংখ্যা sin)
{
  জন্য (ধ্রুবক si_bfs_গ্রন্থি *অনুসন্ধান = n; অনুসন্ধান != ফাঁকা; অনুসন্ধান = অনুসন্ধান->parent)
    যদি (অনুসন্ধান->si == sin)
      ফেরত true;
  ফেরত false;
}

স্থির শূন্য
si_bfs_free (si_bfs_গ্রন্থি *n)
{
  যদি (n == ফাঁকা)
    ফেরত;
  --n->reference_গুনতি;
  যদি (n->reference_গুনতি == 0)
    {
      si_bfs_free (n->parent);
      মুক্ত (n);
    }
}

প্রকারলেখ gl_list_t si_bfs_গ্রন্থি_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
স্থির অন্তঃরেখ derivation_list
expand_to_conflict (state_item_সংখ্যা start, প্রতীক_সংখ্যা conflict_sym)
{
  si_bfs_গ্রন্থি *init = si_bfs_new (start, ফাঁকা);

  si_bfs_গ্রন্থি_list queue
    = gl_list_create (GL_LINKED_LIST, ফাঁকা, ফাঁকা,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ধ্রুবক শূন্য **) &init);
  si_bfs_গ্রন্থি *গ্রন্থি = ফাঁকা;
  // breadth-first search for a path of productions to the conflict symbol
  যখন (gl_list_আকার (queue) > 0)
    {
      গ্রন্থি = (si_bfs_গ্রন্থি *) gl_list_get_at (queue, 0);
      state_item *silast = &state_বস্তুসমূহ[গ্রন্থি->si];
      প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*silast->item);
      যদি (sym == conflict_sym)
        বিরতি;
      যদি (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_সংখ্যা sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              যদি (si_bfs_contains (গ্রন্থি, sin))
                চালু;
              si_bfs_গ্রন্থি *পরবর্তী = si_bfs_new (sin, গ্রন্থি);
              gl_list_add_last (queue, পরবর্তী);
            }
          // for nullable nonterminals, add its goto to the search
          যদি (nullable[sym - ntokens])
            {
              si_bfs_গ্রন্থি *পরবর্তী = si_bfs_new (silast->trans, গ্রন্থি);
              gl_list_add_last (queue, পরবর্তী);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  যদি (gl_list_আকার (queue) == 0)
    {
      gl_list_free (queue);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_leaf (conflict_sym);
  derivation_list ফলাফল = derivation_list_new ();
  derivation_list_append (ফলাফল, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  জন্য (si_bfs_গ্রন্থি *n = গ্রন্থি; n != ফাঁকা; n = n->parent)
    {
      state_item *si = &state_বস্তুসমূহ[n->si];
      item_সংখ্যা *pos = si->item;
      যদি (SI_PRODUCTION (si))
        {
          item_সংখ্যা *i = ফাঁকা;
          জন্য (i = pos + 1; !item_সংখ্যা_is_rule_সংখ্যা (*i); ++i)
            derivation_list_append (ফলাফল, derivation_new_leaf (*i));
          প্রতীক_সংখ্যা lhs =
            rules[item_সংখ্যা_as_rule_সংখ্যা (*i)].lhs->সংখ্যা;
          derivation *deriv = derivation_new (lhs, ফলাফল,
                                              state_item_rule (si));
          ফলাফল = derivation_list_new ();
          derivation_list_append (ফলাফল, deriv);
        }
      অন্যথা
        {
          প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (ফলাফল, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (ফলাফল, 0));
  gl_list_remove_at (ফলাফল, 0);
  ফেরত ফলাফল;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
স্থির derivation *
complete_diverging_example (প্রতীক_সংখ্যা conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list ফলাফল = derivation_list_new ();
  bool lookahead_required = false;
  যদি (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (ফলাফল, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_গ্রন্থি_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  জন্য (gl_list_গ্রন্থি_t state_গ্রন্থি = list_get_end (path);
       state_গ্রন্থি != ফাঁকা;
       state_গ্রন্থি = gl_list_previous_গ্রন্থি (path, state_গ্রন্থি))
    {
      state_item *si = (state_item *) gl_list_গ্রন্থি_মান (path, state_গ্রন্থি);
      item_সংখ্যা *item = si->item;
      item_সংখ্যা pos = *item;
      // symbols after dot
      যদি (gl_list_আকার (ফলাফল) == 1 && !item_সংখ্যা_is_rule_সংখ্যা (pos)
          && gl_list_get_at (ফলাফল, 0) == derivation_dot ())
        {
          derivation_list_append (ফলাফল,
            derivation_new_leaf (item_সংখ্যা_as_প্রতীক_সংখ্যা (pos)));
          lookahead_required = false;
        }
      item_সংখ্যা *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      জন্য (state_item_সংখ্যা nsi = si - state_বস্তুসমূহ;
           !item_সংখ্যা_is_rule_সংখ্যা (*i);
           ++i, nsi = state_বস্তুসমূহ[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          যদি (i == item)
            চালু;
          প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*i);
          যদি (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (ফলাফল, derivation_new_leaf (sym));
              lookahead_required = false;
              চালু;
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
          যদি (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list পরবর্তী_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = ফাঁকা;
              জন্য (gl_list_iterator_t it = gl_list_iterator (পরবর্তী_derivs);
                   derivation_list_পরবর্তী (&it, &d);)
                derivation_list_append (ফলাফল, d);
              i += gl_list_আকার (পরবর্তী_derivs) - 1;
              derivation_list_free (পরবর্তী_derivs);
            }
          অন্যথা যদি (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (ফলাফল, d);
            }
          অন্যথা
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (ফলাফল, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      ধ্রুবক rule *r = &rules[item_সংখ্যা_as_rule_সংখ্যা (*i)];
      // add derivations for symbols before dot
      জন্য (i = item - 1; !item_সংখ্যা_is_rule_সংখ্যা (*i) && i >= ritem; i--)
        {
          gl_list_গ্রন্থি_t p = gl_list_previous_গ্রন্থি (path, state_গ্রন্থি);
          যদি (p)
            state_গ্রন্থি = p;
          যদি (deriv)
            {
              ধ্রুবক শূন্য *tmp_deriv = gl_list_গ্রন্থি_মান (derivs, deriv);
              deriv = gl_list_previous_গ্রন্থি (derivs, deriv);
              derivation_list_prepend (ফলাফল, (derivation*)tmp_deriv);
            }
          অন্যথা
            derivation_list_prepend (ফলাফল, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->সংখ্যা, ফলাফল, r);
      ফলাফল = derivation_list_new ();
      derivation_list_append (ফলাফল, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (ফলাফল, 0);
  derivation_retain (res);
  derivation_list_free (ফলাফল);
  derivation_list_free (derivs);
  ফেরত res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
স্থির state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_গ্রন্থি_t tmp = gl_list_add_last (reduce_path, ফাঁকা);
  gl_list_গ্রন্থি_t পরবর্তী_গ্রন্থি = gl_list_previous_গ্রন্থি (reduce_path, tmp);
  gl_list_গ্রন্থি_t গ্রন্থি = gl_list_previous_গ্রন্থি (reduce_path, পরবর্তী_গ্রন্থি);
  gl_list_remove_গ্রন্থি (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list ফলাফল =
    gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
  // FIXME: bool paths_merged;
  জন্য (; গ্রন্থি != ফাঁকা; পরবর্তী_গ্রন্থি = গ্রন্থি,
       গ্রন্থি = gl_list_previous_গ্রন্থি (reduce_path, গ্রন্থি))
    {
      state_item *refsi =
        (state_item *) gl_list_গ্রন্থি_মান (reduce_path, গ্রন্থি);
      state_item *nextrefsi =
        (state_item *) gl_list_গ্রন্থি_মান (reduce_path, পরবর্তী_গ্রন্থি);
      যদি (nextrefsi == si)
        {
          gl_list_add_first (ফলাফল, refsi);
          si = refsi;
          চালু;
        }
      // skip reduction items
      যদি (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        চালু;

      // bfs to find a shift to the right state
      si_bfs_গ্রন্থি *init = si_bfs_new (si - state_বস্তুসমূহ, ফাঁকা);
      si_bfs_গ্রন্থি_list queue
        = gl_list_create (GL_LINKED_LIST, ফাঁকা, ফাঁকা,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ধ্রুবক শূন্য **) &init);
      si_bfs_গ্রন্থি *sis = ফাঁকা;
      state_item *prevsi = ফাঁকা;
      যখন (gl_list_আকার (queue) > 0)
        {
          sis = (si_bfs_গ্রন্থি *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          যদি (sis->si == 0)
            বিরতি;

          state_item *অনুসন্ধান_si = &state_বস্তুসমূহ[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = অনুসন্ধান_si->revs;
          bitset_iterator biter;
          state_item_সংখ্যা sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_বস্তুসমূহ[sin];
              যদি (SI_TRANSITION (অনুসন্ধান_si))
                {
                  যদি (prevsi->state == refsi->state)
                    যাও অনুসন্ধান_end;
                }
              অন্যথা যদি (!si_bfs_contains (sis, sin))
                {
                  si_bfs_গ্রন্থি *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    অনুসন্ধান_end:
      // prepend path to shift we found
      যদি (sis)
        {
          gl_list_গ্রন্থি_t ln = gl_list_add_first (ফলাফল, &state_বস্তুসমূহ[sis->si]);
          জন্য (si_bfs_গ্রন্থি *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (ফলাফল, ln, &state_বস্তুসমূহ[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  যদি (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = ফাঁকা;
      জন্য (gl_list_iterator_t it = gl_list_iterator (ফলাফল);
           state_item_list_পরবর্তী (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  ফেরত ফলাফল;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
স্থির counterexample *
example_from_path (bool shift_reduce,
                   state_item_সংখ্যা itm2,
                   state_item_list shortest_path, প্রতীক_সংখ্যা পরবর্তী_sym)
{
  derivation *deriv1 =
    complete_diverging_example (পরবর্তী_sym, shortest_path, ফাঁকা);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_বস্তুসমূহ [itm2])
    : shortest_path_from_start (itm2, পরবর্তী_sym);
  derivation *deriv2 = complete_diverging_example (পরবর্তী_sym, path_2, ফাঁকা);
  gl_list_free (path_2);
  ফেরত new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
প্রকারলেখ কাঠামো
{
  parse_state *states[2];
  পূর্ণসংখ্যা complexity;
} অনুসন্ধান_state;

স্থির অনুসন্ধান_state *
initial_অনুসন্ধান_state (state_item *conflict1, state_item *conflict2)
{
  অনুসন্ধান_state *res = xmalloc (আকার *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  ফেরত res;
}

স্থির অনুসন্ধান_state *
new_অনুসন্ধান_state (parse_state *ps1, parse_state *ps2, পূর্ণসংখ্যা complexity)
{
  অনুসন্ধান_state *res = xmalloc (আকার *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  ফেরত res;
}

স্থির অনুসন্ধান_state *
copy_অনুসন্ধান_state (অনুসন্ধান_state *parent)
{
  অনুসন্ধান_state *res = xmalloc (আকার *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  ফেরত res;
}

স্থির শূন্য
অনুসন্ধান_state_free_children (অনুসন্ধান_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

স্থির শূন্য
অনুসন্ধান_state_free (অনুসন্ধান_state *ss)
{
  যদি (ss == ফাঁকা)
    ফেরত;
  অনুসন্ধান_state_free_children (ss);
  মুক্ত (ss);
}

/* For debugging traces.  */
স্থির শূন্য
অনুসন্ধান_state_print (অনুসন্ধান_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

প্রকারলেখ gl_list_t অনুসন্ধান_state_list;

স্থির অন্তঃরেখ bool
অনুসন্ধান_state_list_পরবর্তী (gl_list_iterator_t *it, অনুসন্ধান_state **ss)
{
  ধ্রুবক শূন্য *p = ফাঁকা;
  bool res = gl_list_iterator_পরবর্তী (it, &p, ফাঁকা);
  যদি (res)
    *ss = (অনুসন্ধান_state*) p;
  অন্যথা
    gl_list_iterator_free (it);
  ফেরত res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
স্থির অন্তঃরেখ শূন্য
ss_set_parse_state (অনুসন্ধান_state *ss, পূর্ণসংখ্যা idx, parse_state *ps)
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
স্থির counterexample *
complete_diverging_examples (অনুসন্ধান_state *ss,
                             প্রতীক_সংখ্যা পরবর্তী_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  জন্য (পূর্ণসংখ্যা i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (পরবর্তী_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  ফেরত new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
প্রকারলেখ কাঠামো
{
  অনুসন্ধান_state_list states;
  পূর্ণসংখ্যা complexity;
} অনুসন্ধান_state_bundle;

স্থির শূন্য
ssb_free (অনুসন্ধান_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  মুক্ত (ssb);
}

স্থির size_t
ssb_hasher (অনুসন্ধান_state_bundle *ssb)
{
  ফেরত ssb->complexity;
}

স্থির পূর্ণসংখ্যা
ssb_comp (ধ্রুবক অনুসন্ধান_state_bundle *s1, ধ্রুবক অনুসন্ধান_state_bundle *s2)
{
  ফেরত s1->complexity - s2->complexity;
}

স্থির bool
ssb_equals (ধ্রুবক অনুসন্ধান_state_bundle *s1, ধ্রুবক অনুসন্ধান_state_bundle *s2)
{
  ফেরত s1->complexity == s2->complexity;
}

প্রকারলেখ gl_list_t ssb_list;

স্থির size_t
visited_hasher (ধ্রুবক অনুসন্ধান_state *ss, size_t max)
{
  ফেরত (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

স্থির bool
visited_comparator (ধ্রুবক অনুসন্ধান_state *ss1, ধ্রুবক অনুসন্ধান_state *ss2)
{
  ফেরত parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
স্থির ssb_list ssb_queue;
স্থির Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
স্থির bitset scp_set = ফাঁকা;
/* The set of parser states used for the conflict reduction rule. */
স্থির bitset rpp_set = ফাঁকা;

স্থির শূন্য
ssb_append (অনুসন্ধান_state *ss)
{
  যদি (hash_lookup (visited, ss))
    {
      অনুসন্ধান_state_free (ss);
      ফেরত;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  অনুসন্ধান_state_bundle *ssb = xmalloc (আকার *ssb);
  ssb->complexity = ss->complexity;
  gl_list_গ্রন্থি_t n = gl_list_অনুসন্ধান (ssb_queue, ssb);
  যদি (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা,
                              (gl_listelement_dispose_fn)অনুসন্ধান_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  অন্যথা
    {
      মুক্ত (ssb);
      ssb = (অনুসন্ধান_state_bundle *) gl_list_গ্রন্থি_মান (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
স্থির শূন্য
production_step (অনুসন্ধান_state *ss, পূর্ণসংখ্যা parser_state)
{
  ধ্রুবক state_item *other_si = parse_state_লেজ (ss->states[1 - parser_state]);
  প্রতীক_সংখ্যা other_sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  পূর্ণসংখ্যা complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = ফাঁকা;
  জন্য (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_পরবর্তী (&it, &ps);
       )
    {
      অনুসন্ধান_state *copy = copy_অনুসন্ধান_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
reduction_cost (ধ্রুবক parse_state *ps)
{
  পূর্ণসংখ্যা shifts;
  পূর্ণসংখ্যা productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  ফেরত SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

স্থির অনুসন্ধান_state_list
reduction_step (অনুসন্ধান_state *ss, ধ্রুবক item_সংখ্যা *conflict_item,
                পূর্ণসংখ্যা parser_state, পূর্ণসংখ্যা rule_len)
{
  (শূন্য) conflict_item; // FIXME: Unused
  অনুসন্ধান_state_list ফলাফল =
    gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, 1);

  parse_state *ps = ss->states[parser_state];
  ধ্রুবক state_item *si = parse_state_লেজ (ps);
  bitset প্রতীক_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  ধ্রুবক state_item *other_si = parse_state_লেজ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  যদি (item_সংখ্যা_is_প্রতীক_সংখ্যা (*other_si->item))
    {
      প্রতীক_সংখ্যা other_sym =
        item_সংখ্যা_as_প্রতীক_সংখ্যা (*other_si->item);
      যদি (!intersect_প্রতীক (other_sym, প্রতীক_set))
        ফেরত ফলাফল;
      প্রতীক_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (প্রতীক_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, প্রতীক_set);
  parse_state *reduced_ps = ফাঁকা;
  জন্য (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_পরবর্তী (&it, &reduced_ps);
       )
    {
      অনুসন্ধান_state *copy = copy_অনুসন্ধান_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      পূর্ণসংখ্যা r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (ফলাফল, copy);
    }
  gl_list_free (reduced);
  যদি (প্রতীক_set != si->lookahead)
    bitset_free (প্রতীক_set);
  ফেরত ফলাফল;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
স্থির শূন্য
অনুসন্ধান_state_prepend (অনুসন্ধান_state *ss, প্রতীক_সংখ্যা sym, bitset guide)
{
  (শূন্য) sym; // FIXME: Unused.
  ধ্রুবক state_item *si1src = parse_state_শীর্ষ (ss->states[0]);
  ধ্রুবক state_item *si2src = parse_state_শীর্ষ (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  যদি (prod1 != SI_PRODUCTION (si2src))
    {
      পূর্ণসংখ্যা prod_state = prod1 ? 0 : 1;
      parse_state_list পূর্ব = parser_prepend (ss->states[prod_state]);
      parse_state *ps = ফাঁকা;
      জন্য (gl_list_iterator_t iter = gl_list_iterator (পূর্ব);
           parse_state_list_পরবর্তী (&iter, &ps);
           )
        {
          ধ্রুবক state_item *psi = parse_state_শীর্ষ (ps);
          bool guided = bitset_test (guide, psi->state->সংখ্যা);
          যদি (!guided && !EXTENDED_অনুসন্ধান)
            চালু;

          অনুসন্ধান_state *copy = copy_অনুসন্ধান_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          যদি (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (পূর্ব);
      ফেরত;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ধ্রুবক পূর্ণসংখ্যা complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = ফাঁকা;
  জন্য (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_পরবর্তী (&iter1, &ps1);
       )
    {
      ধ্রুবক state_item *psi1 = parse_state_শীর্ষ (ps1);
      bool guided1 = bitset_test (guide, psi1->state->সংখ্যা);
      যদি (!guided1 && !EXTENDED_অনুসন্ধান)
        চালু;

      parse_state *ps2 = ফাঁকা;
      জন্য (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_পরবর্তী (&iter2, &ps2);
           )
        {
          ধ্রুবক state_item *psi2 = parse_state_শীর্ষ (ps2);

          bool guided2 = bitset_test (guide, psi2->state->সংখ্যা);
          যদি (!guided2 && !EXTENDED_অনুসন্ধান)
            চালু;
          // Only consider prepend state items that share the same state.
          যদি (psi1->state != psi2->state)
            চালু;

          পূর্ণসংখ্যা complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          যদি (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_অনুসন্ধান_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
স্থির bool
have_common_prefix (ধ্রুবক item_সংখ্যা *itm1, ধ্রুবক item_সংখ্যা *itm2)
{
  পূর্ণসংখ্যা i = 0;
  জন্য (; !item_সংখ্যা_is_rule_সংখ্যা (itm1[i]); ++i)
    যদি (itm1[i] != itm2[i])
      ফেরত false;
  ফেরত item_সংখ্যা_is_rule_সংখ্যা (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
স্থির ধ্রুবক item_সংখ্যা *
item_rule_start (ধ্রুবক item_সংখ্যা *item)
{
  ধ্রুবক item_সংখ্যা *res = ফাঁকা;
  জন্য (res = item;
       ritem < res && item_সংখ্যা_is_প্রতীক_সংখ্যা (*(res - 1));
       --res)
    চালু;
  ফেরত res;
}

স্থির ধ্রুবক item_সংখ্যা *
item_rule_end (ধ্রুবক item_সংখ্যা *item)
{
  ধ্রুবক item_সংখ্যা *res = ফাঁকা;
  জন্য (res = item; item_সংখ্যা_is_প্রতীক_সংখ্যা (*res); ++res)
    চালু;
  ফেরত res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
স্থির অন্তঃরেখ শূন্য
generate_পরবর্তী_states (অনুসন্ধান_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  ধ্রুবক state_item *si1 = parse_state_লেজ (ps1);
  ধ্রুবক state_item *si2 = parse_state_লেজ (ps2);
  bool si1reduce = item_সংখ্যা_is_rule_সংখ্যা (*si1->item);
  bool si2reduce = item_সংখ্যা_is_rule_সংখ্যা (*si2->item);
  যদি (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      যদি (*si1->item == *si2->item)
        {
          পূর্ণসংখ্যা complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = ফাঁকা;
          parse_state *tps2 = ফাঁকা;
          জন্য (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_পরবর্তী (&it1, &tps1);
               )
            জন্য (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_পরবর্তী (&it2, &tps2);
                 )
              ssb_append (new_অনুসন্ধান_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  অন্যথা
    {
      ধ্রুবক item_সংখ্যা *rhs1 = item_rule_start (si1->item);
      ধ্রুবক item_সংখ্যা *rhe1 = item_rule_end (si1->item);
      পূর্ণসংখ্যা len1 = rhe1 - rhs1;
      পূর্ণসংখ্যা size1 = parse_state_দৈর্ঘ্য (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ধ্রুবক item_সংখ্যা *rhs2 = item_rule_start (si2->item);
      ধ্রুবক item_সংখ্যা *rhe2 = item_rule_end (si2->item);
      পূর্ণসংখ্যা len2 = rhe2 - rhs2;
      পূর্ণসংখ্যা size2 = parse_state_দৈর্ঘ্য (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      যদি (ready1 && ready2)
        {
          অনুসন্ধান_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          অনুসন্ধান_state *red1 = ফাঁকা;
          জন্য (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               অনুসন্ধান_state_list_পরবর্তী (&iter, &red1);
               )
            {
              অনুসন্ধান_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              অনুসন্ধান_state *red2 = ফাঁকা;
              জন্য (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   অনুসন্ধান_state_list_পরবর্তী (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              যদি (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      অন্যথা যদি (ready1)
        {
          অনুসন্ধান_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          অনুসন্ধান_state *red1 = ফাঁকা;
          জন্য (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               অনুসন্ধান_state_list_পরবর্তী (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      অন্যথা যদি (ready2)
        {
          অনুসন্ধান_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          অনুসন্ধান_state *red2 = ফাঁকা;
          জন্য (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               অনুসন্ধান_state_list_পরবর্তী (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      অন্যথা
        {
          ধ্রুবক প্রতীক_সংখ্যা sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          অনুসন্ধান_state_prepend (ss, sym,
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
স্থির counterexample *
unifying_example (state_item_সংখ্যা itm1,
                  state_item_সংখ্যা itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, প্রতীক_সংখ্যা পরবর্তী_sym)
{
  state_item *conflict1 = &state_বস্তুসমূহ[itm1];
  state_item *conflict2 = &state_বস্তুসমূহ[itm2];
  অনুসন্ধান_state *initial = initial_অনুসন্ধান_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, ফাঁকা, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_তথ্য_freer) অনুসন্ধান_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  অনুসন্ধান_state *stage3result = ফাঁকা;
  counterexample *cex = ফাঁকা;
  যখন (gl_list_আকার (ssb_queue) > 0)
    {
      ধ্রুবক অনুসন্ধান_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      অনুসন্ধান_state *ss = ফাঁকা;
      জন্য (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           অনুসন্ধান_state_list_পরবর্তী (&it, &ss);
           )
        {
          যদি (trace_flag & trace_cex)
            অনুসন্ধান_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          যদি (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ধ্রুবক state_item *si1src = parse_state_শীর্ষ (ps1);
              ধ্রুবক state_item *si2src = parse_state_শীর্ষ (ps2);
              যদি (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  যদি (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      যাও cex_অনুসন্ধান_end;
                    }
                  যদি (!stage3result)
                    stage3result = copy_অনুসন্ধান_state (ss);
                }
            }
          যদি (TIME_LIMIT_ENFORCED)
            {
              দ্বিগুণ time_passed = (gethrxtime () - start) / 1e9;
              যদি (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              যদি (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  যাও cex_অনুসন্ধান_end;
                }
            }
          generate_পরবর্তী_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_অনুসন্ধান_end:;
  যদি (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      যদি (stage3result)
        cex = complete_diverging_examples (stage3result, পরবর্তী_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      অন্যথা
        cex = example_from_path (shift_reduce, itm2, reduce_path, পরবর্তী_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  যদি (stage3result)
    অনুসন্ধান_state_free (stage3result);
  ফেরত cex;
}

স্থির xtime_t cumulative_time;

শূন্য
counterexample_init (শূন্য)
{
  // Check cex.timeout.
  {
    ধ্রুবক অক্ষর *variable = "cex.timeout";
    অক্ষর *cp = muscle_percent_define_get (variable);
    যদি (*cp != '\0')
      {
        অক্ষর *end = ফাঁকা;
        দ্বিগুণ v = c_strtod (cp, &end);
        যদি (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        অন্যথা
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    মুক্ত (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_বস্তুসমূহ_init ();
}


শূন্য
counterexample_free (শূন্য)
{
  যদি (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_বস্তুসমূহ_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
স্থির শূন্য
counterexample_report (state_item_সংখ্যা itm1, state_item_সংখ্যা itm2,
                       প্রতীক_সংখ্যা পরবর্তী_sym, bool shift_reduce,
                       FILE *out, ধ্রুবক অক্ষর *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, পরবর্তী_sym);
  bool reduce_prod_reached = false;
  ধ্রুবক rule *reduce_rule = item_rule (state_বস্তুসমূহ[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = ফাঁকা;
  জন্য (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_পরবর্তী (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->সংখ্যা);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      যদি (reduce_prod_reached)
        bitset_set (rpp_set, si->state->সংখ্যা);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_time) / 1e9 < CUMULATIVE_TIME_LIMIT
    ? unifying_example (itm1, itm2, shift_reduce, shortest_path, পরবর্তী_sym)
    : example_from_path (shift_reduce, itm2, shortest_path, পরবর্তী_sym);

  gl_list_free (shortest_path);
  counterexample_print (cex, out, prefix);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
স্থির শূন্য
counterexample_report_shift_reduce (state_item_সংখ্যা itm1, state_item_সংখ্যা itm2,
                                    প্রতীক_সংখ্যা পরবর্তী_sym,
                                    FILE *out, ধ্রুবক অক্ষর *prefix)
{
  যদি (out == stderr)
    complain (ফাঁকা, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[পরবর্তী_sym]->tag);
  অন্যথা
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[পরবর্তী_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  যদি (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_বস্তুসমূহ[itm1], out, prefix);
      state_item_print (&state_বস্তুসমূহ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, পরবর্তী_sym, true, out, prefix);
}

স্থির শূন্য
counterexample_report_reduce_reduce (state_item_সংখ্যা itm1, state_item_সংখ্যা itm2,
                                     bitset conflict_syms,
                                     FILE *out, ধ্রুবক অক্ষর *prefix)
{
  {
    কাঠামো obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_সংখ্যা sym;
    ধ্রুবক অক্ষর *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    অক্ষর *tokens = obstack_finish0 (&obstack);
    যদি (out == stderr)
      complain (ফাঁকা, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_গুনতি (conflict_syms)),
                tokens);
    অন্যথা
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_গুনতি (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, ফাঁকা);
  }
  // In the report, print the items.
  যদি (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_বস্তুসমূহ[itm1], out, prefix);
      state_item_print (&state_বস্তুসমূহ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

স্থির state_item_সংখ্যা
find_state_item_সংখ্যা (ধ্রুবক rule *r, state_সংখ্যা sn)
{
  জন্য (state_item_সংখ্যা i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    যদি (!SI_DISABLED (i)
        && item_সংখ্যা_as_rule_সংখ্যা (*state_বস্তুসমূহ[i].item) == r->সংখ্যা)
      ফেরত i;
  abort ();
}

শূন্য
counterexample_report_state (ধ্রুবক state *s, FILE *out, ধ্রুবক অক্ষর *prefix)
{
  ধ্রুবক state_সংখ্যা sn = s->সংখ্যা;
  ধ্রুবক reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
    {
      ধ্রুবক rule *r1 = reds->rules[i];
      ধ্রুবক state_item_সংখ্যা c1 = find_state_item_সংখ্যা (r1, sn);
      জন্য (state_item_সংখ্যা c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        যদি (!SI_DISABLED (c2))
          {
            item_সংখ্যা conf = *state_বস্তুসমূহ[c2].item;
            যদি (item_সংখ্যা_is_প্রতীক_সংখ্যা (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      জন্য (পূর্ণসংখ্যা j = i+1; j < reds->অঙ্ক; ++j)
        {
          ধ্রুবক rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          যদি (!bitset_empty_p (lookaheads))
            জন্য (state_item_সংখ্যা c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              যদি (!SI_DISABLED (c2)
                  && item_rule (state_বস্তুসমূহ[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  বিরতি;
                }
        }
    }
  bitset_free (lookaheads);
}
