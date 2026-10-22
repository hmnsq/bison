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

#شامل <config.h>

#شامل "counterexample.h"

#شامل "system.h"

#شامل <c-strtod.h>
#شامل <errno.h>
#شامل <gethrxtime.h>
#شامل <gl_linked_list.h>
#شامل <gl_rbtreehash_list.h>
#شامل <hash.h>
#شامل <mbswidth.h>
#شامل <quote.h>
#شامل <stdlib.h>
#شامل <textstyle.h>
#شامل <time.h>

#شامل "closure.h"
#شامل "complain.h"
#شامل "derivation.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "lssi.h"
#شامل "muscle-tab.h"
#شامل "nullable.h"
#شامل "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_جستجو false

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
ایستا دوگانه time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
ایستا gl_list_گره_t
list_get_end (gl_list_t list)
{
  gl_list_گره_t sentinel = gl_list_add_last (list, پوچ);
  gl_list_گره_t res = gl_list_previous_گره (list, sentinel);
  gl_list_remove_گره (list, sentinel);
  بازگشت res;
}

تعریفنوع ساختار
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ایستا counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (اندازه *res);
  res->shift_reduce = shift_reduce;
  اگر (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  وگرنه
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  بازگشت res;
}

ایستا تهی
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  آزاد (cex);
}

ایستا تهی
counterexample_print (ثابت counterexample *cex, FILE *out, ثابت نویسه *prefix)
{
  ثابت bool flat = getenv ("YYFLAT");
  ثابت نویسه *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ثابت نویسه *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ثابت نویسه *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  ثابت نویسه *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ثابت صحیح width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  اگر (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  وگرنه
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  اگر (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  وگرنه
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  اگر (!cex->unifying || is_styled (stderr))
    {
      اگر (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      وگرنه
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  اگر (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  وگرنه
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  اگر (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
ساختار si_bfs_گره;
تعریفنوع ساختار si_bfs_گره
{
  state_item_عدد si;
  ساختار si_bfs_گره *parent;
  صحیح reference_شمار;
} si_bfs_گره;

ایستا si_bfs_گره *
si_bfs_new (state_item_عدد si, si_bfs_گره *parent)
{
  si_bfs_گره *res = xcalloc (1, اندازه *res);
  res->si = si;
  res->parent = parent;
  res->reference_شمار = 1;
  اگر (parent)
    ++parent->reference_شمار;
  بازگشت res;
}

ایستا bool
si_bfs_contains (ثابت si_bfs_گره *n, state_item_عدد sin)
{
  برای (ثابت si_bfs_گره *جستجو = n; جستجو != پوچ; جستجو = جستجو->parent)
    اگر (جستجو->si == sin)
      بازگشت true;
  بازگشت false;
}

ایستا تهی
si_bfs_free (si_bfs_گره *n)
{
  اگر (n == پوچ)
    بازگشت;
  --n->reference_شمار;
  اگر (n->reference_شمار == 0)
    {
      si_bfs_free (n->parent);
      آزاد (n);
    }
}

تعریفنوع gl_list_t si_bfs_گره_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ایستا درونخطی derivation_list
expand_to_conflict (state_item_عدد start, نماد_عدد conflict_sym)
{
  si_bfs_گره *init = si_bfs_new (start, پوچ);

  si_bfs_گره_list queue
    = gl_list_create (GL_LINKED_LIST, پوچ, پوچ,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ثابت تهی **) &init);
  si_bfs_گره *گره = پوچ;
  // breadth-first search for a path of productions to the conflict symbol
  وقتی (gl_list_size (queue) > 0)
    {
      گره = (si_bfs_گره *) gl_list_get_at (queue, 0);
      state_item *silast = &state_اقلام[گره->si];
      نماد_عدد sym = item_عدد_as_نماد_عدد (*silast->item);
      اگر (sym == conflict_sym)
        بشکن;
      اگر (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_عدد sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              اگر (si_bfs_contains (گره, sin))
                ادامه;
              si_bfs_گره *next = si_bfs_new (sin, گره);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          اگر (nullable[sym - ntokens])
            {
              si_bfs_گره *next = si_bfs_new (silast->trans, گره);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  اگر (gl_list_size (queue) == 0)
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

  برای (si_bfs_گره *n = گره; n != پوچ; n = n->parent)
    {
      state_item *si = &state_اقلام[n->si];
      item_عدد *pos = si->item;
      اگر (SI_PRODUCTION (si))
        {
          item_عدد *i = پوچ;
          برای (i = pos + 1; !item_عدد_is_rule_عدد (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          نماد_عدد lhs =
            rules[item_عدد_as_rule_عدد (*i)].lhs->عدد;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      وگرنه
        {
          نماد_عدد sym = item_عدد_as_نماد_عدد (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  بازگشت result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
ایستا derivation *
complete_diverging_example (نماد_عدد conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  اگر (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_گره_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  برای (gl_list_گره_t state_گره = list_get_end (path);
       state_گره != پوچ;
       state_گره = gl_list_previous_گره (path, state_گره))
    {
      state_item *si = (state_item *) gl_list_گره_مقدار (path, state_گره);
      item_عدد *item = si->item;
      item_عدد pos = *item;
      // symbols after dot
      اگر (gl_list_size (result) == 1 && !item_عدد_is_rule_عدد (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_عدد_as_نماد_عدد (pos)));
          lookahead_required = false;
        }
      item_عدد *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      برای (state_item_عدد nsi = si - state_اقلام;
           !item_عدد_is_rule_عدد (*i);
           ++i, nsi = state_اقلام[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          اگر (i == item)
            ادامه;
          نماد_عدد sym = item_عدد_as_نماد_عدد (*i);
          اگر (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              ادامه;
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
          اگر (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = پوچ;
              برای (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          وگرنه اگر (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          وگرنه
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      ثابت rule *r = &rules[item_عدد_as_rule_عدد (*i)];
      // add derivations for symbols before dot
      برای (i = item - 1; !item_عدد_is_rule_عدد (*i) && i >= ritem; i--)
        {
          gl_list_گره_t p = gl_list_previous_گره (path, state_گره);
          اگر (p)
            state_گره = p;
          اگر (deriv)
            {
              ثابت تهی *tmp_deriv = gl_list_گره_مقدار (derivs, deriv);
              deriv = gl_list_previous_گره (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          وگرنه
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->عدد, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  بازگشت res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ایستا state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_گره_t tmp = gl_list_add_last (reduce_path, پوچ);
  gl_list_گره_t next_گره = gl_list_previous_گره (reduce_path, tmp);
  gl_list_گره_t گره = gl_list_previous_گره (reduce_path, next_گره);
  gl_list_remove_گره (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
  // FIXME: bool paths_merged;
  برای (; گره != پوچ; next_گره = گره,
       گره = gl_list_previous_گره (reduce_path, گره))
    {
      state_item *refsi =
        (state_item *) gl_list_گره_مقدار (reduce_path, گره);
      state_item *nextrefsi =
        (state_item *) gl_list_گره_مقدار (reduce_path, next_گره);
      اگر (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          ادامه;
        }
      // skip reduction items
      اگر (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        ادامه;

      // bfs to find a shift to the right state
      si_bfs_گره *init = si_bfs_new (si - state_اقلام, پوچ);
      si_bfs_گره_list queue
        = gl_list_create (GL_LINKED_LIST, پوچ, پوچ,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ثابت تهی **) &init);
      si_bfs_گره *sis = پوچ;
      state_item *prevsi = پوچ;
      وقتی (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_گره *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          اگر (sis->si == 0)
            بشکن;

          state_item *جستجو_si = &state_اقلام[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = جستجو_si->revs;
          bitset_iterator biter;
          state_item_عدد sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_اقلام[sin];
              اگر (SI_TRANSITION (جستجو_si))
                {
                  اگر (prevsi->state == refsi->state)
                    برو جستجو_end;
                }
              وگرنه اگر (!si_bfs_contains (sis, sin))
                {
                  si_bfs_گره *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    جستجو_end:
      // prepend path to shift we found
      اگر (sis)
        {
          gl_list_گره_t ln = gl_list_add_first (result, &state_اقلام[sis->si]);
          برای (si_bfs_گره *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_اقلام[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  اگر (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = پوچ;
      برای (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  بازگشت result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ایستا counterexample *
example_from_path (bool shift_reduce,
                   state_item_عدد itm2,
                   state_item_list shortest_path, نماد_عدد next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, پوچ);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_اقلام [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, پوچ);
  gl_list_free (path_2);
  بازگشت new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
تعریفنوع ساختار
{
  parse_state *states[2];
  صحیح complexity;
} جستجو_state;

ایستا جستجو_state *
initial_جستجو_state (state_item *conflict1, state_item *conflict2)
{
  جستجو_state *res = xmalloc (اندازه *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  بازگشت res;
}

ایستا جستجو_state *
new_جستجو_state (parse_state *ps1, parse_state *ps2, صحیح complexity)
{
  جستجو_state *res = xmalloc (اندازه *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  بازگشت res;
}

ایستا جستجو_state *
copy_جستجو_state (جستجو_state *parent)
{
  جستجو_state *res = xmalloc (اندازه *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  بازگشت res;
}

ایستا تهی
جستجو_state_free_children (جستجو_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

ایستا تهی
جستجو_state_free (جستجو_state *ss)
{
  اگر (ss == پوچ)
    بازگشت;
  جستجو_state_free_children (ss);
  آزاد (ss);
}

/* For debugging traces.  */
ایستا تهی
جستجو_state_print (جستجو_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

تعریفنوع gl_list_t جستجو_state_list;

ایستا درونخطی bool
جستجو_state_list_next (gl_list_iterator_t *it, جستجو_state **ss)
{
  ثابت تهی *p = پوچ;
  bool res = gl_list_iterator_next (it, &p, پوچ);
  اگر (res)
    *ss = (جستجو_state*) p;
  وگرنه
    gl_list_iterator_free (it);
  بازگشت res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ایستا درونخطی تهی
ss_set_parse_state (جستجو_state *ss, صحیح idx, parse_state *ps)
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
ایستا counterexample *
complete_diverging_examples (جستجو_state *ss,
                             نماد_عدد next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  برای (صحیح i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  بازگشت new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
تعریفنوع ساختار
{
  جستجو_state_list states;
  صحیح complexity;
} جستجو_state_bundle;

ایستا تهی
ssb_free (جستجو_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  آزاد (ssb);
}

ایستا size_t
ssb_hasher (جستجو_state_bundle *ssb)
{
  بازگشت ssb->complexity;
}

ایستا صحیح
ssb_comp (ثابت جستجو_state_bundle *s1, ثابت جستجو_state_bundle *s2)
{
  بازگشت s1->complexity - s2->complexity;
}

ایستا bool
ssb_equals (ثابت جستجو_state_bundle *s1, ثابت جستجو_state_bundle *s2)
{
  بازگشت s1->complexity == s2->complexity;
}

تعریفنوع gl_list_t ssb_list;

ایستا size_t
visited_hasher (ثابت جستجو_state *ss, size_t max)
{
  بازگشت (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

ایستا bool
visited_comparator (ثابت جستجو_state *ss1, ثابت جستجو_state *ss2)
{
  بازگشت parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
ایستا ssb_list ssb_queue;
ایستا Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
ایستا bitset scp_set = پوچ;
/* The set of parser states used for the conflict reduction rule. */
ایستا bitset rpp_set = پوچ;

ایستا تهی
ssb_append (جستجو_state *ss)
{
  اگر (hash_lookup (visited, ss))
    {
      جستجو_state_free (ss);
      بازگشت;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  جستجو_state_bundle *ssb = xmalloc (اندازه *ssb);
  ssb->complexity = ss->complexity;
  gl_list_گره_t n = gl_list_جستجو (ssb_queue, ssb);
  اگر (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ,
                              (gl_listelement_dispose_fn)جستجو_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  وگرنه
    {
      آزاد (ssb);
      ssb = (جستجو_state_bundle *) gl_list_گره_مقدار (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ایستا تهی
production_step (جستجو_state *ss, صحیح parser_state)
{
  ثابت state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  نماد_عدد other_sym = item_عدد_as_نماد_عدد (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  صحیح complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = پوچ;
  برای (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      جستجو_state *copy = copy_جستجو_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ایستا درونخطی صحیح
reduction_cost (ثابت parse_state *ps)
{
  صحیح shifts;
  صحیح productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  بازگشت SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

ایستا جستجو_state_list
reduction_step (جستجو_state *ss, ثابت item_عدد *conflict_item,
                صحیح parser_state, صحیح rule_len)
{
  (تهی) conflict_item; // FIXME: Unused
  جستجو_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, 1);

  parse_state *ps = ss->states[parser_state];
  ثابت state_item *si = parse_state_tail (ps);
  bitset نماد_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  ثابت state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  اگر (item_عدد_is_نماد_عدد (*other_si->item))
    {
      نماد_عدد other_sym =
        item_عدد_as_نماد_عدد (*other_si->item);
      اگر (!intersect_نماد (other_sym, نماد_set))
        بازگشت result;
      نماد_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (نماد_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, نماد_set);
  parse_state *reduced_ps = پوچ;
  برای (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      جستجو_state *copy = copy_جستجو_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      صحیح r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  اگر (نماد_set != si->lookahead)
    bitset_free (نماد_set);
  بازگشت result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ایستا تهی
جستجو_state_prepend (جستجو_state *ss, نماد_عدد sym, bitset guide)
{
  (تهی) sym; // FIXME: Unused.
  ثابت state_item *si1src = parse_state_head (ss->states[0]);
  ثابت state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  اگر (prod1 != SI_PRODUCTION (si2src))
    {
      صحیح prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = پوچ;
      برای (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          ثابت state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->عدد);
          اگر (!guided && !EXTENDED_جستجو)
            ادامه;

          جستجو_state *copy = copy_جستجو_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          اگر (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      بازگشت;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ثابت صحیح complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = پوچ;
  برای (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      ثابت state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->عدد);
      اگر (!guided1 && !EXTENDED_جستجو)
        ادامه;

      parse_state *ps2 = پوچ;
      برای (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          ثابت state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->عدد);
          اگر (!guided2 && !EXTENDED_جستجو)
            ادامه;
          // Only consider prepend state items that share the same state.
          اگر (psi1->state != psi2->state)
            ادامه;

          صحیح complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          اگر (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_جستجو_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
ایستا bool
have_common_prefix (ثابت item_عدد *itm1, ثابت item_عدد *itm2)
{
  صحیح i = 0;
  برای (; !item_عدد_is_rule_عدد (itm1[i]); ++i)
    اگر (itm1[i] != itm2[i])
      بازگشت false;
  بازگشت item_عدد_is_rule_عدد (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ایستا ثابت item_عدد *
item_rule_start (ثابت item_عدد *item)
{
  ثابت item_عدد *res = پوچ;
  برای (res = item;
       ritem < res && item_عدد_is_نماد_عدد (*(res - 1));
       --res)
    ادامه;
  بازگشت res;
}

ایستا ثابت item_عدد *
item_rule_end (ثابت item_عدد *item)
{
  ثابت item_عدد *res = پوچ;
  برای (res = item; item_عدد_is_نماد_عدد (*res); ++res)
    ادامه;
  بازگشت res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ایستا درونخطی تهی
generate_next_states (جستجو_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  ثابت state_item *si1 = parse_state_tail (ps1);
  ثابت state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_عدد_is_rule_عدد (*si1->item);
  bool si2reduce = item_عدد_is_rule_عدد (*si2->item);
  اگر (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      اگر (*si1->item == *si2->item)
        {
          صحیح complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = پوچ;
          parse_state *tps2 = پوچ;
          برای (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            برای (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_جستجو_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  وگرنه
    {
      ثابت item_عدد *rhs1 = item_rule_start (si1->item);
      ثابت item_عدد *rhe1 = item_rule_end (si1->item);
      صحیح len1 = rhe1 - rhs1;
      صحیح size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ثابت item_عدد *rhs2 = item_rule_start (si2->item);
      ثابت item_عدد *rhe2 = item_rule_end (si2->item);
      صحیح len2 = rhe2 - rhs2;
      صحیح size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      اگر (ready1 && ready2)
        {
          جستجو_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          جستجو_state *red1 = پوچ;
          برای (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               جستجو_state_list_next (&iter, &red1);
               )
            {
              جستجو_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              جستجو_state *red2 = پوچ;
              برای (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   جستجو_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              اگر (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      وگرنه اگر (ready1)
        {
          جستجو_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          جستجو_state *red1 = پوچ;
          برای (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               جستجو_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      وگرنه اگر (ready2)
        {
          جستجو_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          جستجو_state *red2 = پوچ;
          برای (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               جستجو_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      وگرنه
        {
          ثابت نماد_عدد sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          جستجو_state_prepend (ss, sym,
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
ایستا counterexample *
unifying_example (state_item_عدد itm1,
                  state_item_عدد itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, نماد_عدد next_sym)
{
  state_item *conflict1 = &state_اقلام[itm1];
  state_item *conflict2 = &state_اقلام[itm2];
  جستجو_state *initial = initial_جستجو_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, پوچ, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) جستجو_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  جستجو_state *stage3result = پوچ;
  counterexample *cex = پوچ;
  وقتی (gl_list_size (ssb_queue) > 0)
    {
      ثابت جستجو_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      جستجو_state *ss = پوچ;
      برای (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           جستجو_state_list_next (&it, &ss);
           )
        {
          اگر (trace_flag & trace_cex)
            جستجو_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          اگر (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ثابت state_item *si1src = parse_state_head (ps1);
              ثابت state_item *si2src = parse_state_head (ps2);
              اگر (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  اگر (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      برو cex_جستجو_end;
                    }
                  اگر (!stage3result)
                    stage3result = copy_جستجو_state (ss);
                }
            }
          اگر (TIME_LIMIT_ENFORCED)
            {
              دوگانه time_passed = (gethrxtime () - start) / 1e9;
              اگر (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              اگر (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  برو cex_جستجو_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_جستجو_end:;
  اگر (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      اگر (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      وگرنه
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  اگر (stage3result)
    جستجو_state_free (stage3result);
  بازگشت cex;
}

ایستا xtime_t cumulative_time;

تهی
counterexample_init (تهی)
{
  // Check cex.timeout.
  {
    ثابت نویسه *variable = "cex.timeout";
    نویسه *cp = muscle_percent_define_get (variable);
    اگر (*cp != '\0')
      {
        نویسه *end = پوچ;
        دوگانه v = c_strtod (cp, &end);
        اگر (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        وگرنه
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    آزاد (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_اقلام_init ();
}


تهی
counterexample_free (تهی)
{
  اگر (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_اقلام_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ایستا تهی
counterexample_report (state_item_عدد itm1, state_item_عدد itm2,
                       نماد_عدد next_sym, bool shift_reduce,
                       FILE *out, ثابت نویسه *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  ثابت rule *reduce_rule = item_rule (state_اقلام[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = پوچ;
  برای (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->عدد);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      اگر (reduce_prod_reached)
        bitset_set (rpp_set, si->state->عدد);
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
ایستا تهی
counterexample_report_shift_reduce (state_item_عدد itm1, state_item_عدد itm2,
                                    نماد_عدد next_sym,
                                    FILE *out, ثابت نویسه *prefix)
{
  اگر (out == stderr)
    complain (پوچ, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  وگرنه
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  اگر (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_اقلام[itm1], out, prefix);
      state_item_print (&state_اقلام[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

ایستا تهی
counterexample_report_reduce_reduce (state_item_عدد itm1, state_item_عدد itm2,
                                     bitset conflict_syms,
                                     FILE *out, ثابت نویسه *prefix)
{
  {
    ساختار obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_عدد sym;
    ثابت نویسه *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    نویسه *tokens = obstack_finish0 (&obstack);
    اگر (out == stderr)
      complain (پوچ, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_شمار (conflict_syms)),
                tokens);
    وگرنه
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_شمار (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, پوچ);
  }
  // In the report, print the items.
  اگر (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_اقلام[itm1], out, prefix);
      state_item_print (&state_اقلام[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

ایستا state_item_عدد
find_state_item_عدد (ثابت rule *r, state_عدد sn)
{
  برای (state_item_عدد i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    اگر (!SI_DISABLED (i)
        && item_عدد_as_rule_عدد (*state_اقلام[i].item) == r->عدد)
      بازگشت i;
  abort ();
}

تهی
counterexample_report_state (ثابت state *s, FILE *out, ثابت نویسه *prefix)
{
  ثابت state_عدد sn = s->عدد;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  برای (صحیح i = 0; i < reds->رقم; ++i)
    {
      ثابت rule *r1 = reds->rules[i];
      ثابت state_item_عدد c1 = find_state_item_عدد (r1, sn);
      برای (state_item_عدد c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        اگر (!SI_DISABLED (c2))
          {
            item_عدد conf = *state_اقلام[c2].item;
            اگر (item_عدد_is_نماد_عدد (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      برای (صحیح j = i+1; j < reds->رقم; ++j)
        {
          ثابت rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          اگر (!bitset_empty_p (lookaheads))
            برای (state_item_عدد c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              اگر (!SI_DISABLED (c2)
                  && item_rule (state_اقلام[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  بشکن;
                }
        }
    }
  bitset_free (lookaheads);
}
