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


#परिभाषय TIME_सीमा_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#परिभाषय EXTENDED_अन्वेषणम् false

/* costs for making various steps in a search */
#परिभाषय PRODUCTION_COST 50
#परिभाषय REDUCE_COST 1
#परिभाषय सरणम्_COST 1
#परिभाषय UNSHIFT_COST 1
#परिभाषय EXTENDED_COST 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#परिभाषय ASSURANCE_सीमा 2.0

/* The time limit before giving up looking for unifying counterexample. */
स्थिर द्विगुण time_सीमा = 5.0;

#परिभाषय CUMULATIVE_TIME_सीमा 120.0

// This is the fastest way to get the tail node from the gl_list API.
स्थिर gl_list_ग्रन्थिः_t
list_get_अन्तः (gl_list_t list)
{
  gl_list_ग्रन्थिः_t sentinel = gl_list_सङ्कलय_चरमः (list, रिक्त);
  gl_list_ग्रन्थिः_t res = gl_list_previous_ग्रन्थिः (list, sentinel);
  gl_list_remove_ग्रन्थिः (list, sentinel);
  प्रत्यागम res;
}

प्रकारनिर्देश संरचना
{
  derivation *d1;
  derivation *d2;
  bool सरणम्_reduce;
  bool unifying;
  bool timeout;
} counterexample;

स्थिर counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool सरणम्_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (परिमाण *res);
  res->सरणम्_reduce = सरणम्_reduce;
  यदि (सरणम्_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  अन्यथा
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  प्रत्यागम res;
}

स्थिर शून्य
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  मुक्त (cex);
}

स्थिर शून्य
counterexample_print (ध्रुव counterexample *cex, FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  ध्रुव bool flat = getenv ("YYFLAT");
  ध्रुव वर्ण *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ध्रुव वर्ण *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ध्रुव वर्ण *deriv1_label
    = cex->सरणम्_reduce ? _("Shift derivation") : _("First reduce derivation");
  ध्रुव वर्ण *deriv2_label
    = cex->सरणम्_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ध्रुव पूर्णाङ्क विस्तारः =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  यदि (flat)
    fprintf (बहिर्गमनम्, "  %s%s%*s ", उपसर्गः,
             example1_label, विस्तारः - mbswidth (example1_label, 0), "");
  अन्यथा
    fprintf (बहिर्गमनम्, "  %s%s: ", उपसर्गः, example1_label);
  derivation_print_leaves (cex->d1, बहिर्गमनम्);
  यदि (flat)
    fprintf (बहिर्गमनम्, "  %s%s%*s ", उपसर्गः,
             deriv1_label, विस्तारः - mbswidth (deriv1_label, 0), "");
  अन्यथा
    fprintf (बहिर्गमनम्, "  %s%s", उपसर्गः, deriv1_label);
  derivation_print (cex->d1, बहिर्गमनम्, उपसर्गः);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  यदि (!cex->unifying || is_styled (stderr))
    {
      यदि (flat)
        fprintf (बहिर्गमनम्, "  %s%s%*s ", उपसर्गः,
                 example2_label, विस्तारः - mbswidth (example2_label, 0), "");
      अन्यथा
        fprintf (बहिर्गमनम्, "  %s%s: ", उपसर्गः, example2_label);
      derivation_print_leaves (cex->d2, बहिर्गमनम्);
    }
  यदि (flat)
    fprintf (बहिर्गमनम्, "  %s%s%*s ", उपसर्गः,
             deriv2_label, विस्तारः - mbswidth (deriv2_label, 0), "");
  अन्यथा
    fprintf (बहिर्गमनम्, "  %s%s", उपसर्गः, deriv2_label);
  derivation_print (cex->d2, बहिर्गमनम्, उपसर्गः);

  यदि (बहिर्गमनम् != stderr)
    putc ('\n', बहिर्गमनम्);
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
  यदि (parent)
    ++parent->reference_गणनम्;
  प्रत्यागम res;
}

स्थिर bool
si_bfs_अन्तर्भवति (ध्रुव si_bfs_ग्रन्थिः *n, state_item_सङ्ख्या ज्या)
{
  प्रति (ध्रुव si_bfs_ग्रन्थिः *अन्वेषणम् = n; अन्वेषणम् != रिक्त; अन्वेषणम् = अन्वेषणम्->parent)
    यदि (अन्वेषणम्->si == ज्या)
      प्रत्यागम true;
  प्रत्यागम false;
}

स्थिर शून्य
si_bfs_free (si_bfs_ग्रन्थिः *n)
{
  यदि (n == रिक्त)
    प्रत्यागम;
  --n->reference_गणनम्;
  यदि (n->reference_गणनम् == 0)
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
expand_गन्तव्यम्_conflict (state_item_सङ्ख्या start, चिह्नम्_सङ्ख्या conflict_sym)
{
  si_bfs_ग्रन्थिः *init = si_bfs_new (start, रिक्त);

  si_bfs_ग्रन्थिः_list queue
    = gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ध्रुव शून्य **) &init);
  si_bfs_ग्रन्थिः *ग्रन्थिः = रिक्त;
  // breadth-first search for a path of productions to the conflict symbol
  यावत् (gl_list_आकार (queue) > 0)
    {
      ग्रन्थिः = (si_bfs_ग्रन्थिः *) gl_list_get_at (queue, 0);
      state_item *silast = &state_वस्तूनि[ग्रन्थिः->si];
      चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*silast->item);
      यदि (sym == conflict_sym)
        भङ्ग;
      यदि (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_सङ्ख्या ज्या;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, ज्या, 0)
            {
              // ignore productions already in the path
              यदि (si_bfs_अन्तर्भवति (ग्रन्थिः, ज्या))
                अनुवर्त;
              si_bfs_ग्रन्थिः *अग्र = si_bfs_new (ज्या, ग्रन्थिः);
              gl_list_सङ्कलय_चरमः (queue, अग्र);
            }
          // for nullable nonterminals, add its goto to the search
          यदि (nullable[sym - ntokens])
            {
              si_bfs_ग्रन्थिः *अग्र = si_bfs_new (silast->trans, ग्रन्थिः);
              gl_list_सङ्कलय_चरमः (queue, अग्र);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  यदि (gl_list_आकार (queue) == 0)
    {
      gl_list_free (queue);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_पर्णम् (conflict_sym);
  derivation_list परिणाम = derivation_list_new ();
  derivation_list_append (परिणाम, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  प्रति (si_bfs_ग्रन्थिः *n = ग्रन्थिः; n != रिक्त; n = n->parent)
    {
      state_item *si = &state_वस्तूनि[n->si];
      item_सङ्ख्या *pos = si->item;
      यदि (SI_PRODUCTION (si))
        {
          item_सङ्ख्या *i = रिक्त;
          प्रति (i = pos + 1; !item_सङ्ख्या_is_नियमः_सङ्ख्या (*i); ++i)
            derivation_list_append (परिणाम, derivation_new_पर्णम् (*i));
          चिह्नम्_सङ्ख्या lhs =
            नियमाः[item_सङ्ख्या_as_नियमः_सङ्ख्या (*i)].lhs->सङ्ख्या;
          derivation *deriv = derivation_new (lhs, परिणाम,
                                              state_item_नियमः (si));
          परिणाम = derivation_list_new ();
          derivation_list_append (परिणाम, deriv);
        }
      अन्यथा
        {
          चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*(pos - 1));
          derivation *deriv = derivation_new_पर्णम् (sym);
          derivation_list_prepend (परिणाम, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (परिणाम, 0));
  gl_list_remove_at (परिणाम, 0);
  प्रत्यागम परिणाम;
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
                            state_item_list मार्गः, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list परिणाम = derivation_list_new ();
  bool lookahead_required = false;
  यदि (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_सङ्कलय_चरमः (परिणाम, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ग्रन्थिः_t deriv = list_get_अन्तः (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  प्रति (gl_list_ग्रन्थिः_t state_ग्रन्थिः = list_get_अन्तः (मार्गः);
       state_ग्रन्थिः != रिक्त;
       state_ग्रन्थिः = gl_list_previous_ग्रन्थिः (मार्गः, state_ग्रन्थिः))
    {
      state_item *si = (state_item *) gl_list_ग्रन्थिः_मानम् (मार्गः, state_ग्रन्थिः);
      item_सङ्ख्या *item = si->item;
      item_सङ्ख्या pos = *item;
      // symbols after dot
      यदि (gl_list_आकार (परिणाम) == 1 && !item_सङ्ख्या_is_नियमः_सङ्ख्या (pos)
          && gl_list_get_at (परिणाम, 0) == derivation_dot ())
        {
          derivation_list_append (परिणाम,
            derivation_new_पर्णम् (item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (pos)));
          lookahead_required = false;
        }
      item_सङ्ख्या *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      प्रति (state_item_सङ्ख्या nsi = si - state_वस्तूनि;
           !item_सङ्ख्या_is_नियमः_सङ्ख्या (*i);
           ++i, nsi = state_वस्तूनि[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          यदि (i == item)
            अनुवर्त;
          चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*i);
          यदि (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (परिणाम, derivation_new_पर्णम् (sym));
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
          यदि (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list अग्र_derivs =
                expand_गन्तव्यम्_conflict (nsi, conflict_sym);
              derivation *d = रिक्त;
              प्रति (gl_list_iterator_t it = gl_list_iterator (अग्र_derivs);
                   derivation_list_अग्र (&it, &d);)
                derivation_list_append (परिणाम, d);
              i += gl_list_आकार (अग्र_derivs) - 1;
              derivation_list_free (अग्र_derivs);
            }
          अन्यथा यदि (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_पर्णम् (sym);
              derivation_list_append (परिणाम, d);
            }
          अन्यथा
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (परिणाम, derivation_new_पर्णम् (1));
              lookahead_required = false;
            }
        }
      ध्रुव नियमः *r = &नियमाः[item_सङ्ख्या_as_नियमः_सङ्ख्या (*i)];
      // add derivations for symbols before dot
      प्रति (i = item - 1; !item_सङ्ख्या_is_नियमः_सङ्ख्या (*i) && i >= ritem; i--)
        {
          gl_list_ग्रन्थिः_t p = gl_list_previous_ग्रन्थिः (मार्गः, state_ग्रन्थिः);
          यदि (p)
            state_ग्रन्थिः = p;
          यदि (deriv)
            {
              ध्रुव शून्य *tmp_deriv = gl_list_ग्रन्थिः_मानम् (derivs, deriv);
              deriv = gl_list_previous_ग्रन्थिः (derivs, deriv);
              derivation_list_prepend (परिणाम, (derivation*)tmp_deriv);
            }
          अन्यथा
            derivation_list_prepend (परिणाम, derivation_new_पर्णम् (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->सङ्ख्या, परिणाम, r);
      परिणाम = derivation_list_new ();
      derivation_list_append (परिणाम, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (परिणाम, 0);
  derivation_retain (res);
  derivation_list_free (परिणाम);
  derivation_list_free (derivs);
  प्रत्यागम res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
स्थिर state_item_list
nonunifying_सरणम्_मार्गः (state_item_list reduce_मार्गः, state_item *सरणम्_conflict)
{
  gl_list_ग्रन्थिः_t tmp = gl_list_सङ्कलय_चरमः (reduce_मार्गः, रिक्त);
  gl_list_ग्रन्थिः_t अग्र_ग्रन्थिः = gl_list_previous_ग्रन्थिः (reduce_मार्गः, tmp);
  gl_list_ग्रन्थिः_t ग्रन्थिः = gl_list_previous_ग्रन्थिः (reduce_मार्गः, अग्र_ग्रन्थिः);
  gl_list_remove_ग्रन्थिः (reduce_मार्गः, tmp);
  state_item *si = सरणम्_conflict;
  state_item_list परिणाम =
    gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  // FIXME: bool paths_merged;
  प्रति (; ग्रन्थिः != रिक्त; अग्र_ग्रन्थिः = ग्रन्थिः,
       ग्रन्थिः = gl_list_previous_ग्रन्थिः (reduce_मार्गः, ग्रन्थिः))
    {
      state_item *refsi =
        (state_item *) gl_list_ग्रन्थिः_मानम् (reduce_मार्गः, ग्रन्थिः);
      state_item *nextrefsi =
        (state_item *) gl_list_ग्रन्थिः_मानम् (reduce_मार्गः, अग्र_ग्रन्थिः);
      यदि (nextrefsi == si)
        {
          gl_list_सङ्कलय_first (परिणाम, refsi);
          si = refsi;
          अनुवर्त;
        }
      // skip reduction items
      यदि (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        अनुवर्त;

      // bfs to find a shift to the right state
      si_bfs_ग्रन्थिः *init = si_bfs_new (si - state_वस्तूनि, रिक्त);
      si_bfs_ग्रन्थिः_list queue
        = gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ध्रुव शून्य **) &init);
      si_bfs_ग्रन्थिः *sis = रिक्त;
      state_item *prevsi = रिक्त;
      यावत् (gl_list_आकार (queue) > 0)
        {
          sis = (si_bfs_ग्रन्थिः *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          यदि (sis->si == 0)
            भङ्ग;

          state_item *अन्वेषणम्_si = &state_वस्तूनि[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = अन्वेषणम्_si->revs;
          bitset_iterator biter;
          state_item_सङ्ख्या ज्या;
          BITSET_FOR_EACH (biter, rsi, ज्या, 0)
            {
              prevsi = &state_वस्तूनि[ज्या];
              यदि (SI_TRANSITION (अन्वेषणम्_si))
                {
                  यदि (prevsi->state == refsi->state)
                    गच्छ अन्वेषणम्_अन्तः;
                }
              अन्यथा यदि (!si_bfs_अन्तर्भवति (sis, ज्या))
                {
                  si_bfs_ग्रन्थिः *prevsis = si_bfs_new (ज्या, sis);
                  gl_list_सङ्कलय_चरमः (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    अन्वेषणम्_अन्तः:
      // prepend path to shift we found
      यदि (sis)
        {
          gl_list_ग्रन्थिः_t ln = gl_list_सङ्कलय_first (परिणाम, &state_वस्तूनि[sis->si]);
          प्रति (si_bfs_ग्रन्थिः *n = sis->parent; n; n = n->parent)
            ln = gl_list_सङ्कलय_after (परिणाम, ln, &state_वस्तूनि[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  यदि (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_item *sip = रिक्त;
      प्रति (gl_list_iterator_t it = gl_list_iterator (परिणाम);
           state_item_list_अग्र (&it, &sip);
           )
        state_item_print (sip, stderr, "");
    }
  प्रत्यागम परिणाम;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
स्थिर counterexample *
example_from_मार्गः (bool सरणम्_reduce,
                   state_item_सङ्ख्या itm2,
                   state_item_list ह्रस्वतमम्_मार्गः, चिह्नम्_सङ्ख्या अग्र_sym)
{
  derivation *deriv1 =
    complete_diverging_example (अग्र_sym, ह्रस्वतमम्_मार्गः, रिक्त);
  state_item_list मार्गः_2
    = सरणम्_reduce
    ? nonunifying_सरणम्_मार्गः (ह्रस्वतमम्_मार्गः, &state_वस्तूनि [itm2])
    : ह्रस्वतमम्_मार्गः_from_start (itm2, अग्र_sym);
  derivation *deriv2 = complete_diverging_example (अग्र_sym, मार्गः_2, रिक्त);
  gl_list_free (मार्गः_2);
  प्रत्यागम new_counterexample (deriv1, deriv2, सरणम्_reduce, false, true);
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
  parse_state *अवस्थाः[2];
  पूर्णाङ्क complexity;
} अन्वेषणम्_state;

स्थिर अन्वेषणम्_state *
initial_अन्वेषणम्_state (state_item *conflict1, state_item *conflict2)
{
  अन्वेषणम्_state *res = xmalloc (परिमाण *res);
  res->अवस्थाः[0] = new_parse_state (conflict1);
  res->अवस्थाः[1] = new_parse_state (conflict2);
  parse_state_retain (res->अवस्थाः[0]);
  parse_state_retain (res->अवस्थाः[1]);
  res->complexity = 0;
  प्रत्यागम res;
}

स्थिर अन्वेषणम्_state *
new_अन्वेषणम्_state (parse_state *ps1, parse_state *ps2, पूर्णाङ्क complexity)
{
  अन्वेषणम्_state *res = xmalloc (परिमाण *res);
  res->अवस्थाः[0] = ps1;
  res->अवस्थाः[1] = ps2;
  parse_state_retain (res->अवस्थाः[0]);
  parse_state_retain (res->अवस्थाः[1]);
  res->complexity = complexity;
  प्रत्यागम res;
}

स्थिर अन्वेषणम्_state *
copy_अन्वेषणम्_state (अन्वेषणम्_state *parent)
{
  अन्वेषणम्_state *res = xmalloc (परिमाण *res);
  *res = *parent;
  parse_state_retain (res->अवस्थाः[0]);
  parse_state_retain (res->अवस्थाः[1]);
  प्रत्यागम res;
}

स्थिर शून्य
अन्वेषणम्_state_free_children (अन्वेषणम्_state *ss)
{
  free_parse_state (ss->अवस्थाः[0]);
  free_parse_state (ss->अवस्थाः[1]);
}

स्थिर शून्य
अन्वेषणम्_state_free (अन्वेषणम्_state *ss)
{
  यदि (ss == रिक्त)
    प्रत्यागम;
  अन्वेषणम्_state_free_children (ss);
  मुक्त (ss);
}

/* For debugging traces.  */
स्थिर शून्य
अन्वेषणम्_state_print (अन्वेषणम्_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->अवस्थाः[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->अवस्थाः[1]);
  putc ('\n', stderr);
}

प्रकारनिर्देश gl_list_t अन्वेषणम्_state_list;

स्थिर अन्तःस्थ bool
अन्वेषणम्_state_list_अग्र (gl_list_iterator_t *it, अन्वेषणम्_state **ss)
{
  ध्रुव शून्य *p = रिक्त;
  bool res = gl_list_iterator_अग्र (it, &p, रिक्त);
  यदि (res)
    *ss = (अन्वेषणम्_state*) p;
  अन्यथा
    gl_list_iterator_free (it);
  प्रत्यागम res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
स्थिर अन्तःस्थ शून्य
ss_समुच्चयः_parse_state (अन्वेषणम्_state *ss, पूर्णाङ्क idx, parse_state *ps)
{
  free_parse_state (ss->अवस्थाः[idx]);
  ss->अवस्थाः[idx] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
स्थिर counterexample *
complete_diverging_examples (अन्वेषणम्_state *ss,
                             चिह्नम्_सङ्ख्या अग्र_sym,
                             bool सरणम्_reduce)
{
  derivation *new_derivs[2];
  प्रति (पूर्णाङ्क i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->अवस्थाः[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (अग्र_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  प्रत्यागम new_counterexample (new_derivs[0], new_derivs[1],
                             सरणम्_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
प्रकारनिर्देश संरचना
{
  अन्वेषणम्_state_list अवस्थाः;
  पूर्णाङ्क complexity;
} अन्वेषणम्_state_bundle;

स्थिर शून्य
ssb_free (अन्वेषणम्_state_bundle *ssb)
{
  gl_list_free (ssb->अवस्थाः);
  मुक्त (ssb);
}

स्थिर size_t
ssb_hasher (अन्वेषणम्_state_bundle *ssb)
{
  प्रत्यागम ssb->complexity;
}

स्थिर पूर्णाङ्क
ssb_comp (ध्रुव अन्वेषणम्_state_bundle *s1, ध्रुव अन्वेषणम्_state_bundle *s2)
{
  प्रत्यागम s1->complexity - s2->complexity;
}

स्थिर bool
ssb_equals (ध्रुव अन्वेषणम्_state_bundle *s1, ध्रुव अन्वेषणम्_state_bundle *s2)
{
  प्रत्यागम s1->complexity == s2->complexity;
}

प्रकारनिर्देश gl_list_t ssb_list;

स्थिर size_t
visited_hasher (ध्रुव अन्वेषणम्_state *ss, size_t max)
{
  प्रत्यागम (parse_state_hasher (ss->अवस्थाः[0], max)
          + parse_state_hasher (ss->अवस्थाः[1], max)) % max;
}

स्थिर bool
visited_comparator (ध्रुव अन्वेषणम्_state *ss1, ध्रुव अन्वेषणम्_state *ss2)
{
  प्रत्यागम parse_state_comparator (ss1->अवस्थाः[0], ss2->अवस्थाः[0])
    && parse_state_comparator (ss1->अवस्थाः[1], ss2->अवस्थाः[1]);
}

/* Priority queue for search states with minimal complexity. */
स्थिर ssb_list ssb_queue;
स्थिर Hash_सारणी *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
स्थिर bitset scp_समुच्चयः = रिक्त;
/* The set of parser states used for the conflict reduction rule. */
स्थिर bitset rpp_समुच्चयः = रिक्त;

स्थिर शून्य
ssb_append (अन्वेषणम्_state *ss)
{
  यदि (hash_lookup (visited, ss))
    {
      अन्वेषणम्_state_free (ss);
      प्रत्यागम;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->अवस्थाः[0]);
  parse_state_free_contents_early (ss->अवस्थाः[1]);
  parse_state_retain (ss->अवस्थाः[0]);
  parse_state_retain (ss->अवस्थाः[1]);
  अन्वेषणम्_state_bundle *ssb = xmalloc (परिमाण *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ग्रन्थिः_t n = gl_list_अन्वेषणम् (ssb_queue, ssb);
  यदि (!n)
    {
      ssb->अवस्थाः =
        gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त,
                              (gl_listelement_dispose_fn)अन्वेषणम्_state_free_children,
                              true);
      gl_sortedlist_सङ्कलय (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  अन्यथा
    {
      मुक्त (ssb);
      ssb = (अन्वेषणम्_state_bundle *) gl_list_ग्रन्थिः_मानम् (ssb_queue, n);
    }
  gl_list_सङ्कलय_चरमः (ssb->अवस्थाः, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
स्थिर शून्य
production_चरणः (अन्वेषणम्_state *ss, पूर्णाङ्क parser_state)
{
  ध्रुव state_item *other_si = parse_state_पुच्छ (ss->अवस्थाः[1 - parser_state]);
  चिह्नम्_सङ्ख्या other_sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->अवस्थाः[parser_state], other_sym);
  पूर्णाङ्क complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_अग्र (&it, &ps);
       )
    {
      अन्वेषणम्_state *copy = copy_अन्वेषणम्_state (ss);
      ss_समुच्चयः_parse_state (copy, parser_state, ps);
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
  parse_state_completed_चरणाः (ps, &shifts, &productions);
  प्रत्यागम सरणम्_COST * shifts + PRODUCTION_COST * productions;
}

स्थिर अन्वेषणम्_state_list
reduction_चरणः (अन्वेषणम्_state *ss, ध्रुव item_सङ्ख्या *conflict_item,
                पूर्णाङ्क parser_state, पूर्णाङ्क नियमः_len)
{
  (शून्य) conflict_item; // FIXME: Unused
  अन्वेषणम्_state_list परिणाम =
    gl_list_create_रिक्तानि (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, 1);

  parse_state *ps = ss->अवस्थाः[parser_state];
  ध्रुव state_item *si = parse_state_पुच्छ (ps);
  bitset चिह्नम्_समुच्चयः = si->lookahead;
  parse_state *other = ss->अवस्थाः[1 - parser_state];
  ध्रुव state_item *other_si = parse_state_पुच्छ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  यदि (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*other_si->item))
    {
      चिह्नम्_सङ्ख्या other_sym =
        item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*other_si->item);
      यदि (!intersect_चिह्नम् (other_sym, चिह्नम्_समुच्चयः))
        प्रत्यागम परिणाम;
      चिह्नम्_समुच्चयः = bitset_create (nsyms, BITSET_FIXED);
      bitset_समुच्चयः (चिह्नम्_समुच्चयः, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, नियमः_len, चिह्नम्_समुच्चयः);
  parse_state *reduced_ps = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_अग्र (&it, &reduced_ps);
       )
    {
      अन्वेषणम्_state *copy = copy_अन्वेषणम्_state (ss);
      ss_समुच्चयः_parse_state (copy, parser_state, reduced_ps);
      पूर्णाङ्क r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * सरणम्_COST;
      gl_list_सङ्कलय_चरमः (परिणाम, copy);
    }
  gl_list_free (reduced);
  यदि (चिह्नम्_समुच्चयः != si->lookahead)
    bitset_free (चिह्नम्_समुच्चयः);
  प्रत्यागम परिणाम;
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
  ध्रुव state_item *si1src = parse_state_शीर्ष (ss->अवस्थाः[0]);
  ध्रुव state_item *si2src = parse_state_शीर्ष (ss->अवस्थाः[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  यदि (prod1 != SI_PRODUCTION (si2src))
    {
      पूर्णाङ्क prod_state = prod1 ? 0 : 1;
      parse_state_list पूर्व = parser_prepend (ss->अवस्थाः[prod_state]);
      parse_state *ps = रिक्त;
      प्रति (gl_list_iterator_t iter = gl_list_iterator (पूर्व);
           parse_state_list_अग्र (&iter, &ps);
           )
        {
          ध्रुव state_item *psi = parse_state_शीर्ष (ps);
          bool guided = bitset_test (guide, psi->state->सङ्ख्या);
          यदि (!guided && !EXTENDED_अन्वेषणम्)
            अनुवर्त;

          अन्वेषणम्_state *copy = copy_अन्वेषणम्_state (ss);
          ss_समुच्चयः_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          यदि (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (पूर्व);
      प्रत्यागम;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ध्रुव पूर्णाङ्क complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->अवस्थाः[0]);
  parse_state_list prev2 = parser_prepend (ss->अवस्थाः[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = रिक्त;
  प्रति (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_अग्र (&iter1, &ps1);
       )
    {
      ध्रुव state_item *psi1 = parse_state_शीर्ष (ps1);
      bool guided1 = bitset_test (guide, psi1->state->सङ्ख्या);
      यदि (!guided1 && !EXTENDED_अन्वेषणम्)
        अनुवर्त;

      parse_state *ps2 = रिक्त;
      प्रति (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_अग्र (&iter2, &ps2);
           )
        {
          ध्रुव state_item *psi2 = parse_state_शीर्ष (ps2);

          bool guided2 = bitset_test (guide, psi2->state->सङ्ख्या);
          यदि (!guided2 && !EXTENDED_अन्वेषणम्)
            अनुवर्त;
          // Only consider prepend state items that share the same state.
          यदि (psi1->state != psi2->state)
            अनुवर्त;

          पूर्णाङ्क complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          यदि (!guided1 || !guided2)
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
have_common_उपसर्गः (ध्रुव item_सङ्ख्या *itm1, ध्रुव item_सङ्ख्या *itm2)
{
  पूर्णाङ्क i = 0;
  प्रति (; !item_सङ्ख्या_is_नियमः_सङ्ख्या (itm1[i]); ++i)
    यदि (itm1[i] != itm2[i])
      प्रत्यागम false;
  प्रत्यागम item_सङ्ख्या_is_नियमः_सङ्ख्या (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
स्थिर ध्रुव item_सङ्ख्या *
item_नियमः_start (ध्रुव item_सङ्ख्या *item)
{
  ध्रुव item_सङ्ख्या *res = रिक्त;
  प्रति (res = item;
       ritem < res && item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*(res - 1));
       --res)
    अनुवर्त;
  प्रत्यागम res;
}

स्थिर ध्रुव item_सङ्ख्या *
item_नियमः_अन्तः (ध्रुव item_सङ्ख्या *item)
{
  ध्रुव item_सङ्ख्या *res = रिक्त;
  प्रति (res = item; item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (*res); ++res)
    अनुवर्त;
  प्रत्यागम res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
स्थिर अन्तःस्थ शून्य
generate_अग्र_अवस्थाः (अन्वेषणम्_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->अवस्थाः[0];
  parse_state *ps2 = ss->अवस्थाः[1];
  ध्रुव state_item *si1 = parse_state_पुच्छ (ps1);
  ध्रुव state_item *si2 = parse_state_पुच्छ (ps2);
  bool si1reduce = item_सङ्ख्या_is_नियमः_सङ्ख्या (*si1->item);
  bool si2reduce = item_सङ्ख्या_is_नियमः_सङ्ख्या (*si2->item);
  यदि (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      यदि (*si1->item == *si2->item)
        {
          पूर्णाङ्क complexity = ss->complexity + 2 * सरणम्_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = रिक्त;
          parse_state *tps2 = रिक्त;
          प्रति (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_अग्र (&it1, &tps1);
               )
            प्रति (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_अग्र (&it2, &tps2);
                 )
              ssb_append (new_अन्वेषणम्_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_चरणः (ss, 0);
      production_चरणः (ss, 1);
    }
  // One of the states requires a reduction
  अन्यथा
    {
      ध्रुव item_सङ्ख्या *rhs1 = item_नियमः_start (si1->item);
      ध्रुव item_सङ्ख्या *rhe1 = item_नियमः_अन्तः (si1->item);
      पूर्णाङ्क len1 = rhe1 - rhs1;
      पूर्णाङ्क size1 = parse_state_दीर्घता (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ध्रुव item_सङ्ख्या *rhs2 = item_नियमः_start (si2->item);
      ध्रुव item_सङ्ख्या *rhe2 = item_नियमः_अन्तः (si2->item);
      पूर्णाङ्क len2 = rhe2 - rhs2;
      पूर्णाङ्क size2 = parse_state_दीर्घता (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      यदि (ready1 && ready2)
        {
          अन्वेषणम्_state_list reduced1 = reduction_चरणः (ss, conflict1->item, 0, len1);
          gl_list_सङ्कलय_चरमः (reduced1, ss);
          अन्वेषणम्_state *red1 = रिक्त;
          प्रति (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               अन्वेषणम्_state_list_अग्र (&iter, &red1);
               )
            {
              अन्वेषणम्_state_list reduced2 =
                reduction_चरणः (red1, conflict2->item, 1, len2);
              अन्वेषणम्_state *red2 = रिक्त;
              प्रति (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   अन्वेषणम्_state_list_अग्र (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              यदि (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      अन्यथा यदि (ready1)
        {
          अन्वेषणम्_state_list reduced1 = reduction_चरणः (ss, conflict1->item, 0, len1);
          अन्वेषणम्_state *red1 = रिक्त;
          प्रति (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               अन्वेषणम्_state_list_अग्र (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      अन्यथा यदि (ready2)
        {
          अन्वेषणम्_state_list reduced2 = reduction_चरणः (ss, conflict2->item, 1, len2);
          अन्वेषणम्_state *red2 = रिक्त;
          प्रति (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               अन्वेषणम्_state_list_अग्र (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      अन्यथा
        {
          ध्रुव चिह्नम्_सङ्ख्या sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          अन्वेषणम्_state_prepend (ss, sym,
                                parse_state_गभीरता (ss->अवस्थाः[0]) >= 0
                                ? rpp_समुच्चयः : scp_समुच्चयः);
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
                  bool सरणम्_reduce,
                  state_item_list reduce_मार्गः, चिह्नम्_सङ्ख्या अग्र_sym)
{
  state_item *conflict1 = &state_वस्तूनि[itm1];
  state_item *conflict2 = &state_वस्तूनि[itm2];
  अन्वेषणम्_state *initial = initial_अन्वेषणम्_state (conflict1, conflict2);
  ssb_queue = gl_list_create_रिक्तानि (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, रिक्त, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_दत्त_freer) अन्वेषणम्_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  अन्वेषणम्_state *stage3result = रिक्त;
  counterexample *cex = रिक्त;
  यावत् (gl_list_आकार (ssb_queue) > 0)
    {
      ध्रुव अन्वेषणम्_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      अन्वेषणम्_state *ss = रिक्त;
      प्रति (gl_list_iterator_t it = gl_list_iterator (ssb->अवस्थाः);
           अन्वेषणम्_state_list_अग्र (&it, &ss);
           )
        {
          यदि (trace_flag & trace_cex)
            अन्वेषणम्_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->अवस्थाः[0];
          parse_state *ps2 = ss->अवस्थाः[1];
          यदि (parse_state_गभीरता (ps1) < 0 && parse_state_गभीरता (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ध्रुव state_item *si1src = parse_state_शीर्ष (ps1);
              ध्रुव state_item *si2src = parse_state_शीर्ष (ps2);
              यदि (item_नियमः (si1src->item)->lhs == item_नियमः (si2src->item)->lhs
                  && have_common_उपसर्गः (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  यदि (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, सरणम्_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      गच्छ cex_अन्वेषणम्_अन्तः;
                    }
                  यदि (!stage3result)
                    stage3result = copy_अन्वेषणम्_state (ss);
                }
            }
          यदि (TIME_सीमा_ENFORCED)
            {
              द्विगुण time_passed = (gethrxtime () - start) / 1e9;
              यदि (!assurance_printed && time_passed > ASSURANCE_सीमा
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              यदि (time_passed > time_सीमा)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", time_passed);
                  गच्छ cex_अन्वेषणम्_अन्तः;
                }
            }
          generate_अग्र_अवस्थाः (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_अन्वेषणम्_अन्तः:;
  यदि (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      यदि (stage3result)
        cex = complete_diverging_examples (stage3result, अग्र_sym, सरणम्_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      अन्यथा
        cex = example_from_मार्गः (सरणम्_reduce, itm2, reduce_मार्गः, अग्र_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  यदि (stage3result)
    अन्वेषणम्_state_free (stage3result);
  प्रत्यागम cex;
}

स्थिर xtime_t cumulative_time;

शून्य
counterexample_init (शून्य)
{
  // Check cex.timeout.
  {
    ध्रुव वर्ण *variable = "cex.timeout";
    वर्ण *cp = muscle_percent_define_get (variable);
    यदि (*cp != '\0')
      {
        वर्ण *अन्तः = रिक्त;
        द्विगुण v = c_strtod (cp, &अन्तः);
        यदि (*अन्तः == '\0' && errno == 0 && v >= 0)
          time_सीमा = v;
        अन्यथा
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
  scp_समुच्चयः = bitset_create (nstates, BITSET_FIXED);
  rpp_समुच्चयः = bitset_create (nstates, BITSET_FIXED);
  state_वस्तूनि_init ();
}


शून्य
counterexample_free (शून्य)
{
  यदि (scp_समुच्चयः)
    {
      bitset_free (scp_समुच्चयः);
      bitset_free (rpp_समुच्चयः);
      state_वस्तूनि_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
स्थिर शून्य
counterexample_report (state_item_सङ्ख्या itm1, state_item_सङ्ख्या itm2,
                       चिह्नम्_सङ्ख्या अग्र_sym, bool सरणम्_reduce,
                       FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list ह्रस्वतमम्_मार्गः = ह्रस्वतमम्_मार्गः_from_start (itm1, अग्र_sym);
  bool reduce_prod_reached = false;
  ध्रुव नियमः *reduce_नियमः = item_नियमः (state_वस्तूनि[itm1].item);

  bitset_zero (scp_समुच्चयः);
  bitset_zero (rpp_समुच्चयः);

  state_item *si = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (ह्रस्वतमम्_मार्गः);
       state_item_list_अग्र (&it, &si);
       )
    {
      bitset_समुच्चयः (scp_समुच्चयः, si->state->सङ्ख्या);
      reduce_prod_reached = reduce_prod_reached
                          || item_नियमः (si->item) == reduce_नियमः;
      यदि (reduce_prod_reached)
        bitset_समुच्चयः (rpp_समुच्चयः, si->state->सङ्ख्या);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_time) / 1e9 < CUMULATIVE_TIME_सीमा
    ? unifying_example (itm1, itm2, सरणम्_reduce, ह्रस्वतमम्_मार्गः, अग्र_sym)
    : example_from_मार्गः (सरणम्_reduce, itm2, ह्रस्वतमम्_मार्गः, अग्र_sym);

  gl_list_free (ह्रस्वतमम्_मार्गः);
  counterexample_print (cex, बहिर्गमनम्, उपसर्गः);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
स्थिर शून्य
counterexample_report_सरणम्_reduce (state_item_सङ्ख्या itm1, state_item_सङ्ख्या itm2,
                                    चिह्नम्_सङ्ख्या अग्र_sym,
                                    FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  यदि (बहिर्गमनम् == stderr)
    complain (रिक्त, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[अग्र_sym]->चिह्नकम्);
  अन्यथा
    {
      fputs (उपसर्गः, बहिर्गमनम्);
      fprintf (बहिर्गमनम्, _("shift/reduce conflict on token %s"), symbols[अग्र_sym]->चिह्नकम्);
      fprintf (बहिर्गमनम्, "%s\n", _(":"));
    }
  // In the report, print the items.
  यदि (बहिर्गमनम् != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_वस्तूनि[itm1], बहिर्गमनम्, उपसर्गः);
      state_item_print (&state_वस्तूनि[itm2], बहिर्गमनम्, उपसर्गः);
    }
  counterexample_report (itm1, itm2, अग्र_sym, true, बहिर्गमनम्, उपसर्गः);
}

स्थिर शून्य
counterexample_report_reduce_reduce (state_item_सङ्ख्या itm1, state_item_सङ्ख्या itm2,
                                     bitset conflict_syms,
                                     FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  {
    संरचना obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_सङ्ख्या sym;
    ध्रुव वर्ण *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->चिह्नकम्);
        sep = ", ";
      }
    वर्ण *tokens = obstack_finish0 (&obstack);
    यदि (बहिर्गमनम् == stderr)
      complain (रिक्त, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_गणनम् (conflict_syms)),
                tokens);
    अन्यथा
      {
        fputs (उपसर्गः, बहिर्गमनम्);
        fprintf (बहिर्गमनम्,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_गणनम् (conflict_syms)),
                 tokens);
        fprintf (बहिर्गमनम्, "%s\n", _(":"));
      }
    obstack_free (&obstack, रिक्त);
  }
  // In the report, print the items.
  यदि (बहिर्गमनम् != stderr || trace_flag & trace_cex)
    {
      state_item_print (&state_वस्तूनि[itm1], बहिर्गमनम्, उपसर्गः);
      state_item_print (&state_वस्तूनि[itm2], बहिर्गमनम्, उपसर्गः);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, बहिर्गमनम्, उपसर्गः);
}

स्थिर state_item_सङ्ख्या
find_state_item_सङ्ख्या (ध्रुव नियमः *r, state_सङ्ख्या sn)
{
  प्रति (state_item_सङ्ख्या i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    यदि (!SI_DISABLED (i)
        && item_सङ्ख्या_as_नियमः_सङ्ख्या (*state_वस्तूनि[i].item) == r->सङ्ख्या)
      प्रत्यागम i;
  abort ();
}

शून्य
counterexample_report_state (ध्रुव state *s, FILE *बहिर्गमनम्, ध्रुव वर्ण *उपसर्गः)
{
  ध्रुव state_सङ्ख्या sn = s->सङ्ख्या;
  ध्रुव reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    {
      ध्रुव नियमः *r1 = reds->नियमाः[i];
      ध्रुव state_item_सङ्ख्या c1 = find_state_item_सङ्ख्या (r1, sn);
      प्रति (state_item_सङ्ख्या c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        यदि (!SI_DISABLED (c2))
          {
            item_सङ्ख्या conf = *state_वस्तूनि[c2].item;
            यदि (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_सरणम्_reduce (c1, c2, conf, बहिर्गमनम्, उपसर्गः);
          }
      प्रति (पूर्णाङ्क j = i+1; j < reds->अङ्कः; ++j)
        {
          ध्रुव नियमः *r2 = reds->नियमाः[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          यदि (!bitset_रिक्तानि_p (lookaheads))
            प्रति (state_item_सङ्ख्या c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              यदि (!SI_DISABLED (c2)
                  && item_नियमः (state_वस्तूनि[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, बहिर्गमनम्, उपसर्गः);
                  भङ्ग;
                }
        }
    }
  bitset_free (lookaheads);
}
