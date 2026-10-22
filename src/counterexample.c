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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "counterexample.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <c-strtod.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <gethrxtime.h>
#അന്തര്ഭാവ <gl_linked_list.h>
#അന്തര്ഭാവ <gl_rbtreehash_list.h>
#അന്തര്ഭാവ <hash.h>
#അന്തര്ഭാവ <mbswidth.h>
#അന്തര്ഭാവ <quote.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <textstyle.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "derivation.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lssi.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "nullable.h"
#അന്തര്ഭാവ "parse-simulation.h"


#define TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#define EXTENDED_തിരയൽ false

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
സ്ഥിതം ഇരട്ട time_limit = 5.0;

#define CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
സ്ഥിതം gl_list_കണ്ണി_t
list_get_end (gl_list_t list)
{
  gl_list_കണ്ണി_t sentinel = gl_list_add_last (list, ശൂന്യ);
  gl_list_കണ്ണി_t res = gl_list_previous_കണ്ണി (list, sentinel);
  gl_list_remove_കണ്ണി (list, sentinel);
  തിരികെ res;
}

തരനിർവചനം ഘടന
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

സ്ഥിതം counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (വലുപ്പം *res);
  res->shift_reduce = shift_reduce;
  എങ്കിൽ (shift_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  അല്ലെങ്കിൽ
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  സ്വതന്ത്രം (cex);
}

സ്ഥിതം ശൂന്യം
counterexample_print (സ്ഥിരം counterexample *cex, FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  സ്ഥിരം bool flat = getenv ("YYFLAT");
  സ്ഥിരം അക്ഷരം *example1_label
    = cex->unifying ? _("Example") : _("First example");
  സ്ഥിരം അക്ഷരം *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  സ്ഥിരം അക്ഷരം *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  സ്ഥിരം അക്ഷരം *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  സ്ഥിരം പൂർണ്ണസംഖ്യ width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  എങ്കിൽ (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  അല്ലെങ്കിൽ
    fprintf (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  എങ്കിൽ (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  അല്ലെങ്കിൽ
    fprintf (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  എങ്കിൽ (!cex->unifying || is_styled (stderr))
    {
      എങ്കിൽ (flat)
        fprintf (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      അല്ലെങ്കിൽ
        fprintf (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  എങ്കിൽ (flat)
    fprintf (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  അല്ലെങ്കിൽ
    fprintf (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  എങ്കിൽ (out != stderr)
    putc ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
ഘടന si_bfs_കണ്ണി;
തരനിർവചനം ഘടന si_bfs_കണ്ണി
{
  state_item_സംഖ്യ si;
  ഘടന si_bfs_കണ്ണി *parent;
  പൂർണ്ണസംഖ്യ reference_എണ്ണം;
} si_bfs_കണ്ണി;

സ്ഥിതം si_bfs_കണ്ണി *
si_bfs_new (state_item_സംഖ്യ si, si_bfs_കണ്ണി *parent)
{
  si_bfs_കണ്ണി *res = xcalloc (1, വലുപ്പം *res);
  res->si = si;
  res->parent = parent;
  res->reference_എണ്ണം = 1;
  എങ്കിൽ (parent)
    ++parent->reference_എണ്ണം;
  തിരികെ res;
}

സ്ഥിതം bool
si_bfs_contains (സ്ഥിരം si_bfs_കണ്ണി *n, state_item_സംഖ്യ sin)
{
  വേണ്ടി (സ്ഥിരം si_bfs_കണ്ണി *തിരയൽ = n; തിരയൽ != ശൂന്യ; തിരയൽ = തിരയൽ->parent)
    എങ്കിൽ (തിരയൽ->si == sin)
      തിരികെ true;
  തിരികെ false;
}

സ്ഥിതം ശൂന്യം
si_bfs_free (si_bfs_കണ്ണി *n)
{
  എങ്കിൽ (n == ശൂന്യ)
    തിരികെ;
  --n->reference_എണ്ണം;
  എങ്കിൽ (n->reference_എണ്ണം == 0)
    {
      si_bfs_free (n->parent);
      സ്വതന്ത്രം (n);
    }
}

തരനിർവചനം gl_list_t si_bfs_കണ്ണി_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
സ്ഥിതം വരിയിൽ derivation_list
expand_to_conflict (state_item_സംഖ്യ start, ചിഹ്നം_സംഖ്യ conflict_sym)
{
  si_bfs_കണ്ണി *init = si_bfs_new (start, ശൂന്യ);

  si_bfs_കണ്ണി_list queue
    = gl_list_create (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (സ്ഥിരം ശൂന്യം **) &init);
  si_bfs_കണ്ണി *കണ്ണി = ശൂന്യ;
  // breadth-first search for a path of productions to the conflict symbol
  ആകുമ്പോൾ (gl_list_size (queue) > 0)
    {
      കണ്ണി = (si_bfs_കണ്ണി *) gl_list_get_at (queue, 0);
      state_item *silast = &state_ഇനങ്ങൾ[കണ്ണി->si];
      ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*silast->item);
      എങ്കിൽ (sym == conflict_sym)
        നിർത്തുക;
      എങ്കിൽ (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_സംഖ്യ sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              എങ്കിൽ (si_bfs_contains (കണ്ണി, sin))
                തുടരുക;
              si_bfs_കണ്ണി *next = si_bfs_new (sin, കണ്ണി);
              gl_list_add_last (queue, next);
            }
          // for nullable nonterminals, add its goto to the search
          എങ്കിൽ (nullable[sym - ntokens])
            {
              si_bfs_കണ്ണി *next = si_bfs_new (silast->trans, കണ്ണി);
              gl_list_add_last (queue, next);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  എങ്കിൽ (gl_list_size (queue) == 0)
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

  വേണ്ടി (si_bfs_കണ്ണി *n = കണ്ണി; n != ശൂന്യ; n = n->parent)
    {
      state_item *si = &state_ഇനങ്ങൾ[n->si];
      item_സംഖ്യ *pos = si->item;
      എങ്കിൽ (SI_PRODUCTION (si))
        {
          item_സംഖ്യ *i = ശൂന്യ;
          വേണ്ടി (i = pos + 1; !item_സംഖ്യ_is_rule_സംഖ്യ (*i); ++i)
            derivation_list_append (result, derivation_new_leaf (*i));
          ചിഹ്നം_സംഖ്യ lhs =
            rules[item_സംഖ്യ_as_rule_സംഖ്യ (*i)].lhs->സംഖ്യ;
          derivation *deriv = derivation_new (lhs, result,
                                              state_item_rule (si));
          result = derivation_list_new ();
          derivation_list_append (result, deriv);
        }
      അല്ലെങ്കിൽ
        {
          ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (result, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (result, 0));
  gl_list_remove_at (result, 0);
  തിരികെ result;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
സ്ഥിതം derivation *
complete_diverging_example (ചിഹ്നം_സംഖ്യ conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list result = derivation_list_new ();
  bool lookahead_required = false;
  എങ്കിൽ (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (result, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_കണ്ണി_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  വേണ്ടി (gl_list_കണ്ണി_t state_കണ്ണി = list_get_end (path);
       state_കണ്ണി != ശൂന്യ;
       state_കണ്ണി = gl_list_previous_കണ്ണി (path, state_കണ്ണി))
    {
      state_item *si = (state_item *) gl_list_കണ്ണി_മൂല്യം (path, state_കണ്ണി);
      item_സംഖ്യ *item = si->item;
      item_സംഖ്യ pos = *item;
      // symbols after dot
      എങ്കിൽ (gl_list_size (result) == 1 && !item_സംഖ്യ_is_rule_സംഖ്യ (pos)
          && gl_list_get_at (result, 0) == derivation_dot ())
        {
          derivation_list_append (result,
            derivation_new_leaf (item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (pos)));
          lookahead_required = false;
        }
      item_സംഖ്യ *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      വേണ്ടി (state_item_സംഖ്യ nsi = si - state_ഇനങ്ങൾ;
           !item_സംഖ്യ_is_rule_സംഖ്യ (*i);
           ++i, nsi = state_ഇനങ്ങൾ[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          എങ്കിൽ (i == item)
            തുടരുക;
          ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*i);
          എങ്കിൽ (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (result, derivation_new_leaf (sym));
              lookahead_required = false;
              തുടരുക;
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
          എങ്കിൽ (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list next_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = ശൂന്യ;
              വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (next_derivs);
                   derivation_list_next (&it, &d);)
                derivation_list_append (result, d);
              i += gl_list_size (next_derivs) - 1;
              derivation_list_free (next_derivs);
            }
          അല്ലെങ്കിൽ എങ്കിൽ (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (result, d);
            }
          അല്ലെങ്കിൽ
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (result, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      സ്ഥിരം rule *r = &rules[item_സംഖ്യ_as_rule_സംഖ്യ (*i)];
      // add derivations for symbols before dot
      വേണ്ടി (i = item - 1; !item_സംഖ്യ_is_rule_സംഖ്യ (*i) && i >= ritem; i--)
        {
          gl_list_കണ്ണി_t p = gl_list_previous_കണ്ണി (path, state_കണ്ണി);
          എങ്കിൽ (p)
            state_കണ്ണി = p;
          എങ്കിൽ (deriv)
            {
              സ്ഥിരം ശൂന്യം *tmp_deriv = gl_list_കണ്ണി_മൂല്യം (derivs, deriv);
              deriv = gl_list_previous_കണ്ണി (derivs, deriv);
              derivation_list_prepend (result, (derivation*)tmp_deriv);
            }
          അല്ലെങ്കിൽ
            derivation_list_prepend (result, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->സംഖ്യ, result, r);
      result = derivation_list_new ();
      derivation_list_append (result, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (result, 0);
  derivation_retain (res);
  derivation_list_free (result);
  derivation_list_free (derivs);
  തിരികെ res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
സ്ഥിതം state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_കണ്ണി_t tmp = gl_list_add_last (reduce_path, ശൂന്യ);
  gl_list_കണ്ണി_t next_കണ്ണി = gl_list_previous_കണ്ണി (reduce_path, tmp);
  gl_list_കണ്ണി_t കണ്ണി = gl_list_previous_കണ്ണി (reduce_path, next_കണ്ണി);
  gl_list_remove_കണ്ണി (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
  // FIXME: bool paths_merged;
  വേണ്ടി (; കണ്ണി != ശൂന്യ; next_കണ്ണി = കണ്ണി,
       കണ്ണി = gl_list_previous_കണ്ണി (reduce_path, കണ്ണി))
    {
      state_item *refsi =
        (state_item *) gl_list_കണ്ണി_മൂല്യം (reduce_path, കണ്ണി);
      state_item *nextrefsi =
        (state_item *) gl_list_കണ്ണി_മൂല്യം (reduce_path, next_കണ്ണി);
      എങ്കിൽ (nextrefsi == si)
        {
          gl_list_add_first (result, refsi);
          si = refsi;
          തുടരുക;
        }
      // skip reduction items
      എങ്കിൽ (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        തുടരുക;

      // bfs to find a shift to the right state
      si_bfs_കണ്ണി *init = si_bfs_new (si - state_ഇനങ്ങൾ, ശൂന്യ);
      si_bfs_കണ്ണി_list queue
        = gl_list_create (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (സ്ഥിരം ശൂന്യം **) &init);
      si_bfs_കണ്ണി *sis = ശൂന്യ;
      state_item *prevsi = ശൂന്യ;
      ആകുമ്പോൾ (gl_list_size (queue) > 0)
        {
          sis = (si_bfs_കണ്ണി *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          എങ്കിൽ (sis->si == 0)
            നിർത്തുക;

          state_item *തിരയൽ_si = &state_ഇനങ്ങൾ[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = തിരയൽ_si->revs;
          bitset_iterator biter;
          state_item_സംഖ്യ sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_ഇനങ്ങൾ[sin];
              എങ്കിൽ (SI_TRANSITION (തിരയൽ_si))
                {
                  എങ്കിൽ (prevsi->state == refsi->state)
                    പോകുക തിരയൽ_end;
                }
              അല്ലെങ്കിൽ എങ്കിൽ (!si_bfs_contains (sis, sin))
                {
                  si_bfs_കണ്ണി *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    തിരയൽ_end:
      // prepend path to shift we found
      എങ്കിൽ (sis)
        {
          gl_list_കണ്ണി_t ln = gl_list_add_first (result, &state_ഇനങ്ങൾ[sis->si]);
          വേണ്ടി (si_bfs_കണ്ണി *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (result, ln, &state_ഇനങ്ങൾ[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  എങ്കിൽ (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = ശൂന്യ;
      വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (result);
           state_item_list_next (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  തിരികെ result;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
സ്ഥിതം counterexample *
example_from_path (bool shift_reduce,
                   state_item_സംഖ്യ itm2,
                   state_item_list shortest_path, ചിഹ്നം_സംഖ്യ next_sym)
{
  derivation *deriv1 =
    complete_diverging_example (next_sym, shortest_path, ശൂന്യ);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_ഇനങ്ങൾ [itm2])
    : shortest_path_from_start (itm2, next_sym);
  derivation *deriv2 = complete_diverging_example (next_sym, path_2, ശൂന്യ);
  gl_list_free (path_2);
  തിരികെ new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
തരനിർവചനം ഘടന
{
  parse_state *states[2];
  പൂർണ്ണസംഖ്യ complexity;
} തിരയൽ_state;

സ്ഥിതം തിരയൽ_state *
initial_തിരയൽ_state (state_item *conflict1, state_item *conflict2)
{
  തിരയൽ_state *res = xmalloc (വലുപ്പം *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  തിരികെ res;
}

സ്ഥിതം തിരയൽ_state *
new_തിരയൽ_state (parse_state *ps1, parse_state *ps2, പൂർണ്ണസംഖ്യ complexity)
{
  തിരയൽ_state *res = xmalloc (വലുപ്പം *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  തിരികെ res;
}

സ്ഥിതം തിരയൽ_state *
copy_തിരയൽ_state (തിരയൽ_state *parent)
{
  തിരയൽ_state *res = xmalloc (വലുപ്പം *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
തിരയൽ_state_free_children (തിരയൽ_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

സ്ഥിതം ശൂന്യം
തിരയൽ_state_free (തിരയൽ_state *ss)
{
  എങ്കിൽ (ss == ശൂന്യ)
    തിരികെ;
  തിരയൽ_state_free_children (ss);
  സ്വതന്ത്രം (ss);
}

/* For debugging traces.  */
സ്ഥിതം ശൂന്യം
തിരയൽ_state_print (തിരയൽ_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->states[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->states[1]);
  putc ('\n', stderr);
}

തരനിർവചനം gl_list_t തിരയൽ_state_list;

സ്ഥിതം വരിയിൽ bool
തിരയൽ_state_list_next (gl_list_iterator_t *it, തിരയൽ_state **ss)
{
  സ്ഥിരം ശൂന്യം *p = ശൂന്യ;
  bool res = gl_list_iterator_next (it, &p, ശൂന്യ);
  എങ്കിൽ (res)
    *ss = (തിരയൽ_state*) p;
  അല്ലെങ്കിൽ
    gl_list_iterator_free (it);
  തിരികെ res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
ss_set_parse_state (തിരയൽ_state *ss, പൂർണ്ണസംഖ്യ idx, parse_state *ps)
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
സ്ഥിതം counterexample *
complete_diverging_examples (തിരയൽ_state *ss,
                             ചിഹ്നം_സംഖ്യ next_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (next_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  തിരികെ new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
തരനിർവചനം ഘടന
{
  തിരയൽ_state_list states;
  പൂർണ്ണസംഖ്യ complexity;
} തിരയൽ_state_bundle;

സ്ഥിതം ശൂന്യം
ssb_free (തിരയൽ_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  സ്വതന്ത്രം (ssb);
}

സ്ഥിതം size_t
ssb_hasher (തിരയൽ_state_bundle *ssb)
{
  തിരികെ ssb->complexity;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
ssb_comp (സ്ഥിരം തിരയൽ_state_bundle *s1, സ്ഥിരം തിരയൽ_state_bundle *s2)
{
  തിരികെ s1->complexity - s2->complexity;
}

സ്ഥിതം bool
ssb_equals (സ്ഥിരം തിരയൽ_state_bundle *s1, സ്ഥിരം തിരയൽ_state_bundle *s2)
{
  തിരികെ s1->complexity == s2->complexity;
}

തരനിർവചനം gl_list_t ssb_list;

സ്ഥിതം size_t
visited_hasher (സ്ഥിരം തിരയൽ_state *ss, size_t max)
{
  തിരികെ (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

സ്ഥിതം bool
visited_comparator (സ്ഥിരം തിരയൽ_state *ss1, സ്ഥിരം തിരയൽ_state *ss2)
{
  തിരികെ parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
സ്ഥിതം ssb_list ssb_queue;
സ്ഥിതം Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
സ്ഥിതം bitset scp_set = ശൂന്യ;
/* The set of parser states used for the conflict reduction rule. */
സ്ഥിതം bitset rpp_set = ശൂന്യ;

സ്ഥിതം ശൂന്യം
ssb_append (തിരയൽ_state *ss)
{
  എങ്കിൽ (hash_lookup (visited, ss))
    {
      തിരയൽ_state_free (ss);
      തിരികെ;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  തിരയൽ_state_bundle *ssb = xmalloc (വലുപ്പം *ssb);
  ssb->complexity = ss->complexity;
  gl_list_കണ്ണി_t n = gl_list_തിരയൽ (ssb_queue, ssb);
  എങ്കിൽ (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ,
                              (gl_listelement_dispose_fn)തിരയൽ_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  അല്ലെങ്കിൽ
    {
      സ്വതന്ത്രം (ssb);
      ssb = (തിരയൽ_state_bundle *) gl_list_കണ്ണി_മൂല്യം (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
സ്ഥിതം ശൂന്യം
production_step (തിരയൽ_state *ss, പൂർണ്ണസംഖ്യ parser_state)
{
  സ്ഥിരം state_item *other_si = parse_state_tail (ss->states[1 - parser_state]);
  ചിഹ്നം_സംഖ്യ other_sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  പൂർണ്ണസംഖ്യ complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_next (&it, &ps);
       )
    {
      തിരയൽ_state *copy = copy_തിരയൽ_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
reduction_cost (സ്ഥിരം parse_state *ps)
{
  പൂർണ്ണസംഖ്യ shifts;
  പൂർണ്ണസംഖ്യ productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  തിരികെ SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

സ്ഥിതം തിരയൽ_state_list
reduction_step (തിരയൽ_state *ss, സ്ഥിരം item_സംഖ്യ *conflict_item,
                പൂർണ്ണസംഖ്യ parser_state, പൂർണ്ണസംഖ്യ rule_len)
{
  (ശൂന്യം) conflict_item; // FIXME: Unused
  തിരയൽ_state_list result =
    gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, 1);

  parse_state *ps = ss->states[parser_state];
  സ്ഥിരം state_item *si = parse_state_tail (ps);
  bitset ചിഹ്നം_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  സ്ഥിരം state_item *other_si = parse_state_tail (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  എങ്കിൽ (item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*other_si->item))
    {
      ചിഹ്നം_സംഖ്യ other_sym =
        item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*other_si->item);
      എങ്കിൽ (!intersect_ചിഹ്നം (other_sym, ചിഹ്നം_set))
        തിരികെ result;
      ചിഹ്നം_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (ചിഹ്നം_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, ചിഹ്നം_set);
  parse_state *reduced_ps = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_next (&it, &reduced_ps);
       )
    {
      തിരയൽ_state *copy = copy_തിരയൽ_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      പൂർണ്ണസംഖ്യ r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (result, copy);
    }
  gl_list_free (reduced);
  എങ്കിൽ (ചിഹ്നം_set != si->lookahead)
    bitset_free (ചിഹ്നം_set);
  തിരികെ result;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
സ്ഥിതം ശൂന്യം
തിരയൽ_state_prepend (തിരയൽ_state *ss, ചിഹ്നം_സംഖ്യ sym, bitset guide)
{
  (ശൂന്യം) sym; // FIXME: Unused.
  സ്ഥിരം state_item *si1src = parse_state_head (ss->states[0]);
  സ്ഥിരം state_item *si2src = parse_state_head (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  എങ്കിൽ (prod1 != SI_PRODUCTION (si2src))
    {
      പൂർണ്ണസംഖ്യ prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->states[prod_state]);
      parse_state *ps = ശൂന്യ;
      വേണ്ടി (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_next (&iter, &ps);
           )
        {
          സ്ഥിരം state_item *psi = parse_state_head (ps);
          bool guided = bitset_test (guide, psi->state->സംഖ്യ);
          എങ്കിൽ (!guided && !EXTENDED_തിരയൽ)
            തുടരുക;

          തിരയൽ_state *copy = copy_തിരയൽ_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          എങ്കിൽ (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (prev);
      തിരികെ;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  സ്ഥിരം പൂർണ്ണസംഖ്യ complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_next (&iter1, &ps1);
       )
    {
      സ്ഥിരം state_item *psi1 = parse_state_head (ps1);
      bool guided1 = bitset_test (guide, psi1->state->സംഖ്യ);
      എങ്കിൽ (!guided1 && !EXTENDED_തിരയൽ)
        തുടരുക;

      parse_state *ps2 = ശൂന്യ;
      വേണ്ടി (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_next (&iter2, &ps2);
           )
        {
          സ്ഥിരം state_item *psi2 = parse_state_head (ps2);

          bool guided2 = bitset_test (guide, psi2->state->സംഖ്യ);
          എങ്കിൽ (!guided2 && !EXTENDED_തിരയൽ)
            തുടരുക;
          // Only consider prepend state items that share the same state.
          എങ്കിൽ (psi1->state != psi2->state)
            തുടരുക;

          പൂർണ്ണസംഖ്യ complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          എങ്കിൽ (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_തിരയൽ_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
സ്ഥിതം bool
have_common_prefix (സ്ഥിരം item_സംഖ്യ *itm1, സ്ഥിരം item_സംഖ്യ *itm2)
{
  പൂർണ്ണസംഖ്യ i = 0;
  വേണ്ടി (; !item_സംഖ്യ_is_rule_സംഖ്യ (itm1[i]); ++i)
    എങ്കിൽ (itm1[i] != itm2[i])
      തിരികെ false;
  തിരികെ item_സംഖ്യ_is_rule_സംഖ്യ (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
സ്ഥിതം സ്ഥിരം item_സംഖ്യ *
item_rule_start (സ്ഥിരം item_സംഖ്യ *item)
{
  സ്ഥിരം item_സംഖ്യ *res = ശൂന്യ;
  വേണ്ടി (res = item;
       ritem < res && item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*(res - 1));
       --res)
    തുടരുക;
  തിരികെ res;
}

സ്ഥിതം സ്ഥിരം item_സംഖ്യ *
item_rule_end (സ്ഥിരം item_സംഖ്യ *item)
{
  സ്ഥിരം item_സംഖ്യ *res = ശൂന്യ;
  വേണ്ടി (res = item; item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (*res); ++res)
    തുടരുക;
  തിരികെ res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
generate_next_states (തിരയൽ_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  സ്ഥിരം state_item *si1 = parse_state_tail (ps1);
  സ്ഥിരം state_item *si2 = parse_state_tail (ps2);
  bool si1reduce = item_സംഖ്യ_is_rule_സംഖ്യ (*si1->item);
  bool si2reduce = item_സംഖ്യ_is_rule_സംഖ്യ (*si2->item);
  എങ്കിൽ (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      എങ്കിൽ (*si1->item == *si2->item)
        {
          പൂർണ്ണസംഖ്യ complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = ശൂന്യ;
          parse_state *tps2 = ശൂന്യ;
          വേണ്ടി (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_next (&it1, &tps1);
               )
            വേണ്ടി (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_next (&it2, &tps2);
                 )
              ssb_append (new_തിരയൽ_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  അല്ലെങ്കിൽ
    {
      സ്ഥിരം item_സംഖ്യ *rhs1 = item_rule_start (si1->item);
      സ്ഥിരം item_സംഖ്യ *rhe1 = item_rule_end (si1->item);
      പൂർണ്ണസംഖ്യ len1 = rhe1 - rhs1;
      പൂർണ്ണസംഖ്യ size1 = parse_state_length (ps1);
      bool ready1 = si1reduce && len1 < size1;

      സ്ഥിരം item_സംഖ്യ *rhs2 = item_rule_start (si2->item);
      സ്ഥിരം item_സംഖ്യ *rhe2 = item_rule_end (si2->item);
      പൂർണ്ണസംഖ്യ len2 = rhe2 - rhs2;
      പൂർണ്ണസംഖ്യ size2 = parse_state_length (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      എങ്കിൽ (ready1 && ready2)
        {
          തിരയൽ_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          തിരയൽ_state *red1 = ശൂന്യ;
          വേണ്ടി (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               തിരയൽ_state_list_next (&iter, &red1);
               )
            {
              തിരയൽ_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              തിരയൽ_state *red2 = ശൂന്യ;
              വേണ്ടി (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   തിരയൽ_state_list_next (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              എങ്കിൽ (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (ready1)
        {
          തിരയൽ_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          തിരയൽ_state *red1 = ശൂന്യ;
          വേണ്ടി (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               തിരയൽ_state_list_next (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (ready2)
        {
          തിരയൽ_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          തിരയൽ_state *red2 = ശൂന്യ;
          വേണ്ടി (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               തിരയൽ_state_list_next (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      അല്ലെങ്കിൽ
        {
          സ്ഥിരം ചിഹ്നം_സംഖ്യ sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          തിരയൽ_state_prepend (ss, sym,
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
സ്ഥിതം counterexample *
unifying_example (state_item_സംഖ്യ itm1,
                  state_item_സംഖ്യ itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, ചിഹ്നം_സംഖ്യ next_sym)
{
  state_item *conflict1 = &state_ഇനങ്ങൾ[itm1];
  state_item *conflict2 = &state_ഇനങ്ങൾ[itm2];
  തിരയൽ_state *initial = initial_തിരയൽ_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, ശൂന്യ, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_data_freer) തിരയൽ_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  തിരയൽ_state *stage3result = ശൂന്യ;
  counterexample *cex = ശൂന്യ;
  ആകുമ്പോൾ (gl_list_size (ssb_queue) > 0)
    {
      സ്ഥിരം തിരയൽ_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      തിരയൽ_state *ss = ശൂന്യ;
      വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           തിരയൽ_state_list_next (&it, &ss);
           )
        {
          എങ്കിൽ (trace_flag & trace_cex)
            തിരയൽ_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          എങ്കിൽ (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              സ്ഥിരം state_item *si1src = parse_state_head (ps1);
              സ്ഥിരം state_item *si2src = parse_state_head (ps2);
              എങ്കിൽ (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  എങ്കിൽ (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      പോകുക cex_തിരയൽ_end;
                    }
                  എങ്കിൽ (!stage3result)
                    stage3result = copy_തിരയൽ_state (ss);
                }
            }
          എങ്കിൽ (TIME_LIMIT_ENFORCED)
            {
              ഇരട്ട time_passed = (gethrxtime () - start) / 1e9;
              എങ്കിൽ (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              എങ്കിൽ (time_passed > time_limit)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  പോകുക cex_തിരയൽ_end;
                }
            }
          generate_next_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_തിരയൽ_end:;
  എങ്കിൽ (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      എങ്കിൽ (stage3result)
        cex = complete_diverging_examples (stage3result, next_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      അല്ലെങ്കിൽ
        cex = example_from_path (shift_reduce, itm2, reduce_path, next_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  എങ്കിൽ (stage3result)
    തിരയൽ_state_free (stage3result);
  തിരികെ cex;
}

സ്ഥിതം xtime_t cumulative_time;

ശൂന്യം
counterexample_init (ശൂന്യം)
{
  // Check cex.timeout.
  {
    സ്ഥിരം അക്ഷരം *variable = "cex.timeout";
    അക്ഷരം *cp = muscle_percent_define_get (variable);
    എങ്കിൽ (*cp != '\0')
      {
        അക്ഷരം *end = ശൂന്യ;
        ഇരട്ട v = c_strtod (cp, &end);
        എങ്കിൽ (*end == '\0' && errno == 0 && v >= 0)
          time_limit = v;
        അല്ലെങ്കിൽ
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    സ്വതന്ത്രം (cp);
  }
  cumulative_time = gethrxtime ();
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_ഇനങ്ങൾ_init ();
}


ശൂന്യം
counterexample_free (ശൂന്യം)
{
  എങ്കിൽ (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_ഇനങ്ങൾ_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
സ്ഥിതം ശൂന്യം
counterexample_report (state_item_സംഖ്യ itm1, state_item_സംഖ്യ itm2,
                       ചിഹ്നം_സംഖ്യ next_sym, bool shift_reduce,
                       FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, next_sym);
  bool reduce_prod_reached = false;
  സ്ഥിരം rule *reduce_rule = item_rule (state_ഇനങ്ങൾ[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_next (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->സംഖ്യ);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      എങ്കിൽ (reduce_prod_reached)
        bitset_set (rpp_set, si->state->സംഖ്യ);
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
സ്ഥിതം ശൂന്യം
counterexample_report_shift_reduce (state_item_സംഖ്യ itm1, state_item_സംഖ്യ itm2,
                                    ചിഹ്നം_സംഖ്യ next_sym,
                                    FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  എങ്കിൽ (out == stderr)
    complain (ശൂന്യ, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
  അല്ലെങ്കിൽ
    {
      fputs (prefix, out);
      fprintf (out, _("shift/reduce conflict on token %s"), symbols[next_sym]->tag);
      fprintf (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  എങ്കിൽ (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_ഇനങ്ങൾ[itm1], out, prefix);
      state_item_print (&state_ഇനങ്ങൾ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, next_sym, true, out, prefix);
}

സ്ഥിതം ശൂന്യം
counterexample_report_reduce_reduce (state_item_സംഖ്യ itm1, state_item_സംഖ്യ itm2,
                                     bitset conflict_syms,
                                     FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  {
    ഘടന obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_സംഖ്യ sym;
    സ്ഥിരം അക്ഷരം *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    അക്ഷരം *tokens = obstack_finish0 (&obstack);
    എങ്കിൽ (out == stderr)
      complain (ശൂന്യ, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_എണ്ണം (conflict_syms)),
                tokens);
    അല്ലെങ്കിൽ
      {
        fputs (prefix, out);
        fprintf (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_എണ്ണം (conflict_syms)),
                 tokens);
        fprintf (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, ശൂന്യ);
  }
  // In the report, print the items.
  എങ്കിൽ (out != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_ഇനങ്ങൾ[itm1], out, prefix);
      state_item_print (&state_ഇനങ്ങൾ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

സ്ഥിതം state_item_സംഖ്യ
find_state_item_സംഖ്യ (സ്ഥിരം rule *r, state_സംഖ്യ sn)
{
  വേണ്ടി (state_item_സംഖ്യ i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    എങ്കിൽ (!SI_DISABLED (i)
        && item_സംഖ്യ_as_rule_സംഖ്യ (*state_ഇനങ്ങൾ[i].item) == r->സംഖ്യ)
      തിരികെ i;
  abort ();
}

ശൂന്യം
counterexample_report_state (സ്ഥിരം state *s, FILE *out, സ്ഥിരം അക്ഷരം *prefix)
{
  സ്ഥിരം state_സംഖ്യ sn = s->സംഖ്യ;
  സ്ഥിരം reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
    {
      സ്ഥിരം rule *r1 = reds->rules[i];
      സ്ഥിരം state_item_സംഖ്യ c1 = find_state_item_സംഖ്യ (r1, sn);
      വേണ്ടി (state_item_സംഖ്യ c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        എങ്കിൽ (!SI_DISABLED (c2))
          {
            item_സംഖ്യ conf = *state_ഇനങ്ങൾ[c2].item;
            എങ്കിൽ (item_സംഖ്യ_is_ചിഹ്നം_സംഖ്യ (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = i+1; j < reds->അക്കം; ++j)
        {
          സ്ഥിരം rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          എങ്കിൽ (!bitset_empty_p (lookaheads))
            വേണ്ടി (state_item_സംഖ്യ c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              എങ്കിൽ (!SI_DISABLED (c2)
                  && item_rule (state_ഇനങ്ങൾ[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  നിർത്തുക;
                }
        }
    }
  bitset_free (lookaheads);
}
