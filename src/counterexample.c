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

#समावेश <config.h>

#समावेश "counterexample.h"

#समावेश "system.h"

#समावेश <c-strtod.h>
#समावेश <errno.h>
#समावेश <gethrxtime.h>
#समावेश <gl_linked_list.h>
#समावेश <gl_rbtreehash_list.h>
#समावेश <hash.h>
#समावेश <mbswidth.h>
#समावेश <quote.h>
#समावेश <stdlib.h>
#समावेश <textstyle.h>
#समावेश <time.h>

#समावेश "closure.h"
#समावेश "complain.h"
#समावेश "derivation.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "lalr.h"
#समावेश "lssi.h"
#समावेश "muscle-tab.h"
#समावेश "nullable.h"
#समावेश "parse-simulation.h"


#घोषणा TIME_LIMIT_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#घोषणा EXTENDED_खोज false

/* costs for making various steps in a search */
#घोषणा PRODUCTION_COST 50
#घोषणा REDUCE_COST 1
#घोषणा SHIFT_COST 1
#घोषणा UNSHIFT_COST 1
#घोषणा EXTENDED_COST 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#घोषणा ASSURANCE_LIMIT 2.0

/* The time limit before giving up looking for unifying counterexample. */
अटल द्विगुन time_limit = 5.0;

#घोषणा CUMULATIVE_TIME_LIMIT 120.0

// This is the fastest way to get the tail node from the gl_list API.
अटल gl_list_गाँठ_t
list_get_end (gl_list_t list)
{
  gl_list_गाँठ_t sentinel = gl_list_add_last (list, शून्य);
  gl_list_गाँठ_t res = gl_list_previous_गाँठ (list, sentinel);
  gl_list_remove_गाँठ (list, sentinel);
  वापस res;
}

प्रकार काष्ठा
{
  derivation *d1;
  derivation *d2;
  bool shift_reduce;
  bool unifying;
  bool timeout;
} counterexample;

अटल counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool shift_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (माप *res);
  res->shift_reduce = shift_reduce;
  अगर (shift_reduce)
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
  वापस res;
}

अटल व्योम
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  मुक्त (cex);
}

अटल व्योम
counterexample_print (स्थिर counterexample *cex, खाता *out, स्थिर अक्षर *prefix)
{
  स्थिर bool flat = दो_पर्या ("YYFLAT");
  स्थिर अक्षर *example1_label
    = cex->unifying ? _("Example") : _("First example");
  स्थिर अक्षर *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  स्थिर अक्षर *deriv1_label
    = cex->shift_reduce ? _("Shift derivation") : _("First reduce derivation");
  स्थिर अक्षर *deriv2_label
    = cex->shift_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  स्थिर पूर्णांक width =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  अगर (flat)
    ख_लिखो (out, "  %s%s%*s ", prefix,
             example1_label, width - mbswidth (example1_label, 0), "");
  अन्यथा
    ख_लिखो (out, "  %s%s: ", prefix, example1_label);
  derivation_print_leaves (cex->d1, out);
  अगर (flat)
    ख_लिखो (out, "  %s%s%*s ", prefix,
             deriv1_label, width - mbswidth (deriv1_label, 0), "");
  अन्यथा
    ख_लिखो (out, "  %s%s", prefix, deriv1_label);
  derivation_print (cex->d1, out, prefix);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  अगर (!cex->unifying || is_styled (मानक_त्रुटि))
    {
      अगर (flat)
        ख_लिखो (out, "  %s%s%*s ", prefix,
                 example2_label, width - mbswidth (example2_label, 0), "");
      अन्यथा
        ख_लिखो (out, "  %s%s: ", prefix, example2_label);
      derivation_print_leaves (cex->d2, out);
    }
  अगर (flat)
    ख_लिखो (out, "  %s%s%*s ", prefix,
             deriv2_label, width - mbswidth (deriv2_label, 0), "");
  अन्यथा
    ख_लिखो (out, "  %s%s", prefix, deriv2_label);
  derivation_print (cex->d2, out, prefix);

  अगर (out != मानक_त्रुटि)
    अ_दो ('\n', out);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
काष्ठा si_bfs_गाँठ;
प्रकार काष्ठा si_bfs_गाँठ
{
  state_item_संख्या si;
  काष्ठा si_bfs_गाँठ *parent;
  पूर्णांक reference_गिनती;
} si_bfs_गाँठ;

अटल si_bfs_गाँठ *
si_bfs_new (state_item_संख्या si, si_bfs_गाँठ *parent)
{
  si_bfs_गाँठ *res = xcalloc (1, माप *res);
  res->si = si;
  res->parent = parent;
  res->reference_गिनती = 1;
  अगर (parent)
    ++parent->reference_गिनती;
  वापस res;
}

अटल bool
si_bfs_contains (स्थिर si_bfs_गाँठ *n, state_item_संख्या sin)
{
  क्रम (स्थिर si_bfs_गाँठ *खोज = n; खोज != शून्य; खोज = खोज->parent)
    अगर (खोज->si == sin)
      वापस true;
  वापस false;
}

अटल व्योम
si_bfs_free (si_bfs_गाँठ *n)
{
  अगर (n == शून्य)
    वापस;
  --n->reference_गिनती;
  अगर (n->reference_गिनती == 0)
    {
      si_bfs_free (n->parent);
      मुक्त (n);
    }
}

प्रकार gl_list_t si_bfs_गाँठ_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
अटल अंतरभूत derivation_list
expand_to_conflict (state_item_संख्या start, प्रतीक_संख्या conflict_sym)
{
  si_bfs_गाँठ *init = si_bfs_new (start, शून्य);

  si_bfs_गाँठ_list queue
    = gl_list_create (GL_LINKED_LIST, शून्य, शून्य,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (स्थिर व्योम **) &init);
  si_bfs_गाँठ *गाँठ = शून्य;
  // breadth-first search for a path of productions to the conflict symbol
  जबतक (gl_list_आकार (queue) > 0)
    {
      गाँठ = (si_bfs_गाँठ *) gl_list_get_at (queue, 0);
      state_item *silast = &state_वस्तुएँ[गाँठ->si];
      प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (*silast->item);
      अगर (sym == conflict_sym)
        अवरोध;
      अगर (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_item_संख्या sin;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, sin, 0)
            {
              // ignore productions already in the path
              अगर (si_bfs_contains (गाँठ, sin))
                जारी;
              si_bfs_गाँठ *अग्र = si_bfs_new (sin, गाँठ);
              gl_list_add_last (queue, अग्र);
            }
          // for nullable nonterminals, add its goto to the search
          अगर (nullable[sym - ntokens])
            {
              si_bfs_गाँठ *अग्र = si_bfs_new (silast->trans, गाँठ);
              gl_list_add_last (queue, अग्र);
            }
        }
      gl_list_remove_at (queue, 0);
    }
  अगर (gl_list_आकार (queue) == 0)
    {
      gl_list_free (queue);
      ख_माला_दो ("Error expanding derivation\n", मानक_त्रुटि);
      पात ();
    }

  derivation *dinit = derivation_new_leaf (conflict_sym);
  derivation_list परिणाम = derivation_list_new ();
  derivation_list_append (परिणाम, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  क्रम (si_bfs_गाँठ *n = गाँठ; n != शून्य; n = n->parent)
    {
      state_item *si = &state_वस्तुएँ[n->si];
      item_संख्या *pos = si->item;
      अगर (SI_PRODUCTION (si))
        {
          item_संख्या *i = शून्य;
          क्रम (i = pos + 1; !item_संख्या_is_rule_संख्या (*i); ++i)
            derivation_list_append (परिणाम, derivation_new_leaf (*i));
          प्रतीक_संख्या lhs =
            rules[item_संख्या_as_rule_संख्या (*i)].lhs->संख्या;
          derivation *deriv = derivation_new (lhs, परिणाम,
                                              state_item_rule (si));
          परिणाम = derivation_list_new ();
          derivation_list_append (परिणाम, deriv);
        }
      अन्यथा
        {
          प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (*(pos - 1));
          derivation *deriv = derivation_new_leaf (sym);
          derivation_list_prepend (परिणाम, deriv);
        }
    }
  gl_list_free (queue);
  derivation_free ((derivation*)gl_list_get_at (परिणाम, 0));
  gl_list_remove_at (परिणाम, 0);
  वापस परिणाम;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
अटल derivation *
complete_diverging_example (प्रतीक_संख्या conflict_sym,
                            state_item_list path, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list परिणाम = derivation_list_new ();
  bool lookahead_required = false;
  अगर (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_add_last (परिणाम, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_गाँठ_t deriv = list_get_end (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  क्रम (gl_list_गाँठ_t state_गाँठ = list_get_end (path);
       state_गाँठ != शून्य;
       state_गाँठ = gl_list_previous_गाँठ (path, state_गाँठ))
    {
      state_item *si = (state_item *) gl_list_गाँठ_मूल्य (path, state_गाँठ);
      item_संख्या *item = si->item;
      item_संख्या pos = *item;
      // symbols after dot
      अगर (gl_list_आकार (परिणाम) == 1 && !item_संख्या_is_rule_संख्या (pos)
          && gl_list_get_at (परिणाम, 0) == derivation_dot ())
        {
          derivation_list_append (परिणाम,
            derivation_new_leaf (item_संख्या_as_प्रतीक_संख्या (pos)));
          lookahead_required = false;
        }
      item_संख्या *i = item;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      क्रम (state_item_संख्या nsi = si - state_वस्तुएँ;
           !item_संख्या_is_rule_संख्या (*i);
           ++i, nsi = state_वस्तुएँ[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          अगर (i == item)
            जारी;
          प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (*i);
          अगर (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (परिणाम, derivation_new_leaf (sym));
              lookahead_required = false;
              जारी;
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
          अगर (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list अग्र_derivs =
                expand_to_conflict (nsi, conflict_sym);
              derivation *d = शून्य;
              क्रम (gl_list_iterator_t it = gl_list_iterator (अग्र_derivs);
                   derivation_list_अग्र (&it, &d);)
                derivation_list_append (परिणाम, d);
              i += gl_list_आकार (अग्र_derivs) - 1;
              derivation_list_free (अग्र_derivs);
            }
          अन्यथा अगर (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_leaf (sym);
              derivation_list_append (परिणाम, d);
            }
          अन्यथा
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (परिणाम, derivation_new_leaf (1));
              lookahead_required = false;
            }
        }
      स्थिर rule *r = &rules[item_संख्या_as_rule_संख्या (*i)];
      // add derivations for symbols before dot
      क्रम (i = item - 1; !item_संख्या_is_rule_संख्या (*i) && i >= ritem; i--)
        {
          gl_list_गाँठ_t p = gl_list_previous_गाँठ (path, state_गाँठ);
          अगर (p)
            state_गाँठ = p;
          अगर (deriv)
            {
              स्थिर व्योम *tmp_deriv = gl_list_गाँठ_मूल्य (derivs, deriv);
              deriv = gl_list_previous_गाँठ (derivs, deriv);
              derivation_list_prepend (परिणाम, (derivation*)tmp_deriv);
            }
          अन्यथा
            derivation_list_prepend (परिणाम, derivation_new_leaf (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->संख्या, परिणाम, r);
      परिणाम = derivation_list_new ();
      derivation_list_append (परिणाम, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (परिणाम, 0);
  derivation_retain (res);
  derivation_list_free (परिणाम);
  derivation_list_free (derivs);
  वापस res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
अटल state_item_list
nonunifying_shift_path (state_item_list reduce_path, state_item *shift_conflict)
{
  gl_list_गाँठ_t tmp = gl_list_add_last (reduce_path, शून्य);
  gl_list_गाँठ_t अग्र_गाँठ = gl_list_previous_गाँठ (reduce_path, tmp);
  gl_list_गाँठ_t गाँठ = gl_list_previous_गाँठ (reduce_path, अग्र_गाँठ);
  gl_list_remove_गाँठ (reduce_path, tmp);
  state_item *si = shift_conflict;
  state_item_list परिणाम =
    gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
  // FIXME: bool paths_merged;
  क्रम (; गाँठ != शून्य; अग्र_गाँठ = गाँठ,
       गाँठ = gl_list_previous_गाँठ (reduce_path, गाँठ))
    {
      state_item *refsi =
        (state_item *) gl_list_गाँठ_मूल्य (reduce_path, गाँठ);
      state_item *nextrefsi =
        (state_item *) gl_list_गाँठ_मूल्य (reduce_path, अग्र_गाँठ);
      अगर (nextrefsi == si)
        {
          gl_list_add_first (परिणाम, refsi);
          si = refsi;
          जारी;
        }
      // skip reduction items
      अगर (nextrefsi->item != refsi->item + 1 && refsi->item != ritem)
        जारी;

      // bfs to find a shift to the right state
      si_bfs_गाँठ *init = si_bfs_new (si - state_वस्तुएँ, शून्य);
      si_bfs_गाँठ_list queue
        = gl_list_create (GL_LINKED_LIST, शून्य, शून्य,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (स्थिर व्योम **) &init);
      si_bfs_गाँठ *sis = शून्य;
      state_item *prevsi = शून्य;
      जबतक (gl_list_आकार (queue) > 0)
        {
          sis = (si_bfs_गाँठ *) gl_list_get_at (queue, 0);
          // if we end up in the start state, the shift couldn't be found.
          अगर (sis->si == 0)
            अवरोध;

          state_item *खोज_si = &state_वस्तुएँ[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = खोज_si->revs;
          bitset_iterator biter;
          state_item_संख्या sin;
          BITSET_FOR_EACH (biter, rsi, sin, 0)
            {
              prevsi = &state_वस्तुएँ[sin];
              अगर (SI_TRANSITION (खोज_si))
                {
                  अगर (prevsi->state == refsi->state)
                    जाओ खोज_end;
                }
              अन्यथा अगर (!si_bfs_contains (sis, sin))
                {
                  si_bfs_गाँठ *prevsis = si_bfs_new (sin, sis);
                  gl_list_add_last (queue, prevsis);
                }
            }
          gl_list_remove_at (queue, 0);
        }
    खोज_end:
      // prepend path to shift we found
      अगर (sis)
        {
          gl_list_गाँठ_t ln = gl_list_add_first (परिणाम, &state_वस्तुएँ[sis->si]);
          क्रम (si_bfs_गाँठ *n = sis->parent; n; n = n->parent)
            ln = gl_list_add_after (परिणाम, ln, &state_वस्तुएँ[n->si]);

        }
      si = prevsi;
      gl_list_free (queue);
    }
  अगर (trace_flag & trace_cex)
    {
      ख_माला_दो ("SHIFT ITEM PATH:\n", मानक_त्रुटि);
      state_item *sip = शून्य;
      क्रम (gl_list_iterator_t it = gl_list_iterator (परिणाम);
           state_item_list_अग्र (&it, &sip);
           )
        state_item_print (sip, मानक_त्रुटि, "");
    }
  वापस परिणाम;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
अटल counterexample *
example_from_path (bool shift_reduce,
                   state_item_संख्या itm2,
                   state_item_list shortest_path, प्रतीक_संख्या अग्र_sym)
{
  derivation *deriv1 =
    complete_diverging_example (अग्र_sym, shortest_path, शून्य);
  state_item_list path_2
    = shift_reduce
    ? nonunifying_shift_path (shortest_path, &state_वस्तुएँ [itm2])
    : shortest_path_from_start (itm2, अग्र_sym);
  derivation *deriv2 = complete_diverging_example (अग्र_sym, path_2, शून्य);
  gl_list_free (path_2);
  वापस new_counterexample (deriv1, deriv2, shift_reduce, false, true);
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
प्रकार काष्ठा
{
  parse_state *states[2];
  पूर्णांक complexity;
} खोज_state;

अटल खोज_state *
initial_खोज_state (state_item *conflict1, state_item *conflict2)
{
  खोज_state *res = xmalloc (माप *res);
  res->states[0] = new_parse_state (conflict1);
  res->states[1] = new_parse_state (conflict2);
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = 0;
  वापस res;
}

अटल खोज_state *
new_खोज_state (parse_state *ps1, parse_state *ps2, पूर्णांक complexity)
{
  खोज_state *res = xmalloc (माप *res);
  res->states[0] = ps1;
  res->states[1] = ps2;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  res->complexity = complexity;
  वापस res;
}

अटल खोज_state *
copy_खोज_state (खोज_state *parent)
{
  खोज_state *res = xmalloc (माप *res);
  *res = *parent;
  parse_state_retain (res->states[0]);
  parse_state_retain (res->states[1]);
  वापस res;
}

अटल व्योम
खोज_state_free_children (खोज_state *ss)
{
  free_parse_state (ss->states[0]);
  free_parse_state (ss->states[1]);
}

अटल व्योम
खोज_state_free (खोज_state *ss)
{
  अगर (ss == शून्य)
    वापस;
  खोज_state_free_children (ss);
  मुक्त (ss);
}

/* For debugging traces.  */
अटल व्योम
खोज_state_print (खोज_state *ss)
{
  ख_माला_दो ("CONFLICT 1 ", मानक_त्रुटि);
  print_parse_state (ss->states[0]);
  ख_माला_दो ("CONFLICT 2 ", मानक_त्रुटि);
  print_parse_state (ss->states[1]);
  अ_दो ('\n', मानक_त्रुटि);
}

प्रकार gl_list_t खोज_state_list;

अटल अंतरभूत bool
खोज_state_list_अग्र (gl_list_iterator_t *it, खोज_state **ss)
{
  स्थिर व्योम *p = शून्य;
  bool res = gl_list_iterator_अग्र (it, &p, शून्य);
  अगर (res)
    *ss = (खोज_state*) p;
  अन्यथा
    gl_list_iterator_free (it);
  वापस res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
अटल अंतरभूत व्योम
ss_set_parse_state (खोज_state *ss, पूर्णांक idx, parse_state *ps)
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
अटल counterexample *
complete_diverging_examples (खोज_state *ss,
                             प्रतीक_संख्या अग्र_sym,
                             bool shift_reduce)
{
  derivation *new_derivs[2];
  क्रम (पूर्णांक i = 0; i < 2; ++i)
    {
      state_item_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->states[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (अग्र_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  वापस new_counterexample (new_derivs[0], new_derivs[1],
                             shift_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
प्रकार काष्ठा
{
  खोज_state_list states;
  पूर्णांक complexity;
} खोज_state_bundle;

अटल व्योम
ssb_free (खोज_state_bundle *ssb)
{
  gl_list_free (ssb->states);
  मुक्त (ssb);
}

अटल माप_प्रकार
ssb_hasher (खोज_state_bundle *ssb)
{
  वापस ssb->complexity;
}

अटल पूर्णांक
ssb_comp (स्थिर खोज_state_bundle *s1, स्थिर खोज_state_bundle *s2)
{
  वापस s1->complexity - s2->complexity;
}

अटल bool
ssb_equals (स्थिर खोज_state_bundle *s1, स्थिर खोज_state_bundle *s2)
{
  वापस s1->complexity == s2->complexity;
}

प्रकार gl_list_t ssb_list;

अटल माप_प्रकार
visited_hasher (स्थिर खोज_state *ss, माप_प्रकार max)
{
  वापस (parse_state_hasher (ss->states[0], max)
          + parse_state_hasher (ss->states[1], max)) % max;
}

अटल bool
visited_comparator (स्थिर खोज_state *ss1, स्थिर खोज_state *ss2)
{
  वापस parse_state_comparator (ss1->states[0], ss2->states[0])
    && parse_state_comparator (ss1->states[1], ss2->states[1]);
}

/* Priority queue for search states with minimal complexity. */
अटल ssb_list ssb_queue;
अटल Hash_table *visited;
/* The set of parser states on the shortest lookahead-sensitive path. */
अटल bitset scp_set = शून्य;
/* The set of parser states used for the conflict reduction rule. */
अटल bitset rpp_set = शून्य;

अटल व्योम
ssb_append (खोज_state *ss)
{
  अगर (hash_lookup (visited, ss))
    {
      खोज_state_free (ss);
      वापस;
    }
  hash_xinsert (visited, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->states[0]);
  parse_state_free_contents_early (ss->states[1]);
  parse_state_retain (ss->states[0]);
  parse_state_retain (ss->states[1]);
  खोज_state_bundle *ssb = xmalloc (माप *ssb);
  ssb->complexity = ss->complexity;
  gl_list_गाँठ_t n = gl_list_खोज (ssb_queue, ssb);
  अगर (!n)
    {
      ssb->states =
        gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य,
                              (gl_listelement_dispose_fn)खोज_state_free_children,
                              true);
      gl_sortedlist_add (ssb_queue, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  अन्यथा
    {
      मुक्त (ssb);
      ssb = (खोज_state_bundle *) gl_list_गाँठ_मूल्य (ssb_queue, n);
    }
  gl_list_add_last (ssb->states, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
अटल व्योम
production_step (खोज_state *ss, पूर्णांक parser_state)
{
  स्थिर state_item *other_si = parse_state_पुच्छ (ss->states[1 - parser_state]);
  प्रतीक_संख्या other_sym = item_संख्या_as_प्रतीक_संख्या (*other_si->item);
  parse_state_list prods =
    simulate_production (ss->states[parser_state], other_sym);
  पूर्णांक complexity = ss->complexity + PRODUCTION_COST;

  parse_state *ps = शून्य;
  क्रम (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_अग्र (&it, &ps);
       )
    {
      खोज_state *copy = copy_खोज_state (ss);
      ss_set_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

अटल अंतरभूत पूर्णांक
reduction_cost (स्थिर parse_state *ps)
{
  पूर्णांक shifts;
  पूर्णांक productions;
  parse_state_completed_steps (ps, &shifts, &productions);
  वापस SHIFT_COST * shifts + PRODUCTION_COST * productions;
}

अटल खोज_state_list
reduction_step (खोज_state *ss, स्थिर item_संख्या *conflict_item,
                पूर्णांक parser_state, पूर्णांक rule_len)
{
  (व्योम) conflict_item; // FIXME: Unused
  खोज_state_list परिणाम =
    gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, 1);

  parse_state *ps = ss->states[parser_state];
  स्थिर state_item *si = parse_state_पुच्छ (ps);
  bitset प्रतीक_set = si->lookahead;
  parse_state *other = ss->states[1 - parser_state];
  स्थिर state_item *other_si = parse_state_पुच्छ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  अगर (item_संख्या_is_प्रतीक_संख्या (*other_si->item))
    {
      प्रतीक_संख्या other_sym =
        item_संख्या_as_प्रतीक_संख्या (*other_si->item);
      अगर (!intersect_प्रतीक (other_sym, प्रतीक_set))
        वापस परिणाम;
      प्रतीक_set = bitset_create (nsyms, BITSET_FIXED);
      bitset_set (प्रतीक_set, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, rule_len, प्रतीक_set);
  parse_state *reduced_ps = शून्य;
  क्रम (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_अग्र (&it, &reduced_ps);
       )
    {
      खोज_state *copy = copy_खोज_state (ss);
      ss_set_parse_state (copy, parser_state, reduced_ps);
      पूर्णांक r_cost = reduction_cost (reduced_ps);
      copy->complexity += r_cost + PRODUCTION_COST + 2 * SHIFT_COST;
      gl_list_add_last (परिणाम, copy);
    }
  gl_list_free (reduced);
  अगर (प्रतीक_set != si->lookahead)
    bitset_free (प्रतीक_set);
  वापस परिणाम;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
अटल व्योम
खोज_state_prepend (खोज_state *ss, प्रतीक_संख्या sym, bitset guide)
{
  (व्योम) sym; // FIXME: Unused.
  स्थिर state_item *si1src = parse_state_शीर्ष (ss->states[0]);
  स्थिर state_item *si2src = parse_state_शीर्ष (ss->states[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  अगर (prod1 != SI_PRODUCTION (si2src))
    {
      पूर्णांक prod_state = prod1 ? 0 : 1;
      parse_state_list पूर्व = parser_prepend (ss->states[prod_state]);
      parse_state *ps = शून्य;
      क्रम (gl_list_iterator_t iter = gl_list_iterator (पूर्व);
           parse_state_list_अग्र (&iter, &ps);
           )
        {
          स्थिर state_item *psi = parse_state_शीर्ष (ps);
          bool guided = bitset_test (guide, psi->state->संख्या);
          अगर (!guided && !EXTENDED_खोज)
            जारी;

          खोज_state *copy = copy_खोज_state (ss);
          ss_set_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_COST;
          अगर (!guided)
            copy->complexity += EXTENDED_COST;
          ssb_append (copy);
        }
      gl_list_free (पूर्व);
      वापस;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  स्थिर पूर्णांक complexity_cost = 2 * (prod1 ? PRODUCTION_COST : UNSHIFT_COST);

  parse_state_list prev1 = parser_prepend (ss->states[0]);
  parse_state_list prev2 = parser_prepend (ss->states[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = शून्य;
  क्रम (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_अग्र (&iter1, &ps1);
       )
    {
      स्थिर state_item *psi1 = parse_state_शीर्ष (ps1);
      bool guided1 = bitset_test (guide, psi1->state->संख्या);
      अगर (!guided1 && !EXTENDED_खोज)
        जारी;

      parse_state *ps2 = शून्य;
      क्रम (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_अग्र (&iter2, &ps2);
           )
        {
          स्थिर state_item *psi2 = parse_state_शीर्ष (ps2);

          bool guided2 = bitset_test (guide, psi2->state->संख्या);
          अगर (!guided2 && !EXTENDED_खोज)
            जारी;
          // Only consider prepend state items that share the same state.
          अगर (psi1->state != psi2->state)
            जारी;

          पूर्णांक complexity = ss->complexity + complexity_cost;
          // penalty for not being along the guide path
          अगर (!guided1 || !guided2)
            complexity += EXTENDED_COST;
          ssb_append (new_खोज_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
अटल bool
have_common_prefix (स्थिर item_संख्या *itm1, स्थिर item_संख्या *itm2)
{
  पूर्णांक i = 0;
  क्रम (; !item_संख्या_is_rule_संख्या (itm1[i]); ++i)
    अगर (itm1[i] != itm2[i])
      वापस false;
  वापस item_संख्या_is_rule_संख्या (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
अटल स्थिर item_संख्या *
item_rule_start (स्थिर item_संख्या *item)
{
  स्थिर item_संख्या *res = शून्य;
  क्रम (res = item;
       ritem < res && item_संख्या_is_प्रतीक_संख्या (*(res - 1));
       --res)
    जारी;
  वापस res;
}

अटल स्थिर item_संख्या *
item_rule_end (स्थिर item_संख्या *item)
{
  स्थिर item_संख्या *res = शून्य;
  क्रम (res = item; item_संख्या_is_प्रतीक_संख्या (*res); ++res)
    जारी;
  वापस res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
अटल अंतरभूत व्योम
generate_अग्र_states (खोज_state *ss, state_item *conflict1,
                      state_item *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->states[0];
  parse_state *ps2 = ss->states[1];
  स्थिर state_item *si1 = parse_state_पुच्छ (ps1);
  स्थिर state_item *si2 = parse_state_पुच्छ (ps2);
  bool si1reduce = item_संख्या_is_rule_संख्या (*si1->item);
  bool si2reduce = item_संख्या_is_rule_संख्या (*si2->item);
  अगर (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      अगर (*si1->item == *si2->item)
        {
          पूर्णांक complexity = ss->complexity + 2 * SHIFT_COST;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = शून्य;
          parse_state *tps2 = शून्य;
          क्रम (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_अग्र (&it1, &tps1);
               )
            क्रम (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_अग्र (&it2, &tps2);
                 )
              ssb_append (new_खोज_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_step (ss, 0);
      production_step (ss, 1);
    }
  // One of the states requires a reduction
  अन्यथा
    {
      स्थिर item_संख्या *rhs1 = item_rule_start (si1->item);
      स्थिर item_संख्या *rhe1 = item_rule_end (si1->item);
      पूर्णांक len1 = rhe1 - rhs1;
      पूर्णांक size1 = parse_state_लंबाई (ps1);
      bool ready1 = si1reduce && len1 < size1;

      स्थिर item_संख्या *rhs2 = item_rule_start (si2->item);
      स्थिर item_संख्या *rhe2 = item_rule_end (si2->item);
      पूर्णांक len2 = rhe2 - rhs2;
      पूर्णांक size2 = parse_state_लंबाई (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      अगर (ready1 && ready2)
        {
          खोज_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          gl_list_add_last (reduced1, ss);
          खोज_state *red1 = शून्य;
          क्रम (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               खोज_state_list_अग्र (&iter, &red1);
               )
            {
              खोज_state_list reduced2 =
                reduction_step (red1, conflict2->item, 1, len2);
              खोज_state *red2 = शून्य;
              क्रम (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   खोज_state_list_अग्र (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              अगर (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      अन्यथा अगर (ready1)
        {
          खोज_state_list reduced1 = reduction_step (ss, conflict1->item, 0, len1);
          खोज_state *red1 = शून्य;
          क्रम (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               खोज_state_list_अग्र (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      अन्यथा अगर (ready2)
        {
          खोज_state_list reduced2 = reduction_step (ss, conflict2->item, 1, len2);
          खोज_state *red2 = शून्य;
          क्रम (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               खोज_state_list_अग्र (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      अन्यथा
        {
          स्थिर प्रतीक_संख्या sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          खोज_state_prepend (ss, sym,
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
अटल counterexample *
unifying_example (state_item_संख्या itm1,
                  state_item_संख्या itm2,
                  bool shift_reduce,
                  state_item_list reduce_path, प्रतीक_संख्या अग्र_sym)
{
  state_item *conflict1 = &state_वस्तुएँ[itm1];
  state_item *conflict2 = &state_वस्तुएँ[itm2];
  खोज_state *initial = initial_खोज_state (conflict1, conflict2);
  ssb_queue = gl_list_create_empty (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  visited =
    hash_initialize (32, शून्य, (Hash_hasher) visited_hasher,
                     (Hash_comparator) visited_comparator,
                     (Hash_तथ्य_freer) खोज_state_free);
  ssb_append (initial);
  xtime_t start = gethrxtime ();
  bool assurance_printed = false;
  खोज_state *stage3result = शून्य;
  counterexample *cex = शून्य;
  जबतक (gl_list_आकार (ssb_queue) > 0)
    {
      स्थिर खोज_state_bundle *ssb = gl_list_get_at (ssb_queue, 0);

      खोज_state *ss = शून्य;
      क्रम (gl_list_iterator_t it = gl_list_iterator (ssb->states);
           खोज_state_list_अग्र (&it, &ss);
           )
        {
          अगर (trace_flag & trace_cex)
            खोज_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->states[0];
          parse_state *ps2 = ss->states[1];
          अगर (parse_state_depth (ps1) < 0 && parse_state_depth (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              स्थिर state_item *si1src = parse_state_शीर्ष (ps1);
              स्थिर state_item *si2src = parse_state_शीर्ष (ps2);
              अगर (item_rule (si1src->item)->lhs == item_rule (si2src->item)->lhs
                  && have_common_prefix (si1src->item, si2src->item))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  अगर (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, shift_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      जाओ cex_खोज_end;
                    }
                  अगर (!stage3result)
                    stage3result = copy_खोज_state (ss);
                }
            }
          अगर (TIME_LIMIT_ENFORCED)
            {
              द्विगुन time_passed = (gethrxtime () - start) / 1e9;
              अगर (!assurance_printed && time_passed > ASSURANCE_LIMIT
                  && stage3result)
                {
                  ख_माला_दो ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         मानक_त्रुटि);
                  assurance_printed = true;
                }
              अगर (time_passed > time_limit)
                {
                  ख_लिखो (मानक_त्रुटि, "time limit exceeded: %f\n", time_passed);
                  जाओ cex_खोज_end;
                }
            }
          generate_अग्र_states (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_queue,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_खोज_end:;
  अगर (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      अगर (stage3result)
        cex = complete_diverging_examples (stage3result, अग्र_sym, shift_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      अन्यथा
        cex = example_from_path (shift_reduce, itm2, reduce_path, अग्र_sym);
    }
  gl_list_free (ssb_queue);
  hash_free (visited);
  अगर (stage3result)
    खोज_state_free (stage3result);
  वापस cex;
}

अटल xtime_t cumulative_time;

व्योम
counterexample_init (व्योम)
{
  // Check cex.timeout.
  {
    स्थिर अक्षर *variable = "cex.timeout";
    अक्षर *cp = muscle_percent_define_get (variable);
    अगर (*cp != '\0')
      {
        अक्षर *end = शून्य;
        द्विगुन v = c_strtod (cp, &end);
        अगर (*end == '\0' && त्रुटि_सं == 0 && v >= 0)
          time_limit = v;
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
  scp_set = bitset_create (nstates, BITSET_FIXED);
  rpp_set = bitset_create (nstates, BITSET_FIXED);
  state_वस्तुएँ_init ();
}


व्योम
counterexample_free (व्योम)
{
  अगर (scp_set)
    {
      bitset_free (scp_set);
      bitset_free (rpp_set);
      state_वस्तुएँ_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
अटल व्योम
counterexample_report (state_item_संख्या itm1, state_item_संख्या itm2,
                       प्रतीक_संख्या अग्र_sym, bool shift_reduce,
                       खाता *out, स्थिर अक्षर *prefix)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_item_list shortest_path = shortest_path_from_start (itm1, अग्र_sym);
  bool reduce_prod_reached = false;
  स्थिर rule *reduce_rule = item_rule (state_वस्तुएँ[itm1].item);

  bitset_zero (scp_set);
  bitset_zero (rpp_set);

  state_item *si = शून्य;
  क्रम (gl_list_iterator_t it = gl_list_iterator (shortest_path);
       state_item_list_अग्र (&it, &si);
       )
    {
      bitset_set (scp_set, si->state->संख्या);
      reduce_prod_reached = reduce_prod_reached
                          || item_rule (si->item) == reduce_rule;
      अगर (reduce_prod_reached)
        bitset_set (rpp_set, si->state->संख्या);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_time) / 1e9 < CUMULATIVE_TIME_LIMIT
    ? unifying_example (itm1, itm2, shift_reduce, shortest_path, अग्र_sym)
    : example_from_path (shift_reduce, itm2, shortest_path, अग्र_sym);

  gl_list_free (shortest_path);
  counterexample_print (cex, out, prefix);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
अटल व्योम
counterexample_report_shift_reduce (state_item_संख्या itm1, state_item_संख्या itm2,
                                    प्रतीक_संख्या अग्र_sym,
                                    खाता *out, स्थिर अक्षर *prefix)
{
  अगर (out == मानक_त्रुटि)
    complain (शून्य, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[अग्र_sym]->tag);
  अन्यथा
    {
      ख_माला_दो (prefix, out);
      ख_लिखो (out, _("shift/reduce conflict on token %s"), symbols[अग्र_sym]->tag);
      ख_लिखो (out, "%s\n", _(":"));
    }
  // In the report, print the items.
  अगर (out != मानक_त्रुटि || trace_flag & trace_cex)
    {
      state_item_print (&state_वस्तुएँ[itm1], out, prefix);
      state_item_print (&state_वस्तुएँ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, अग्र_sym, true, out, prefix);
}

अटल व्योम
counterexample_report_reduce_reduce (state_item_संख्या itm1, state_item_संख्या itm2,
                                     bitset conflict_syms,
                                     खाता *out, स्थिर अक्षर *prefix)
{
  {
    काष्ठा obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_item_संख्या sym;
    स्थिर अक्षर *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->tag);
        sep = ", ";
      }
    अक्षर *tokens = obstack_finish0 (&obstack);
    अगर (out == मानक_त्रुटि)
      complain (शून्य, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_गिनती (conflict_syms)),
                tokens);
    अन्यथा
      {
        ख_माला_दो (prefix, out);
        ख_लिखो (out,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_गिनती (conflict_syms)),
                 tokens);
        ख_लिखो (out, "%s\n", _(":"));
      }
    obstack_free (&obstack, शून्य);
  }
  // In the report, print the items.
  अगर (out != मानक_त्रुटि || trace_flag & trace_cex)
    {
      state_item_print (&state_वस्तुएँ[itm1], out, prefix);
      state_item_print (&state_वस्तुएँ[itm2], out, prefix);
    }
  counterexample_report (itm1, itm2, bitset_first (conflict_syms),
                         false, out, prefix);
}

अटल state_item_संख्या
find_state_item_संख्या (स्थिर rule *r, state_संख्या sn)
{
  क्रम (state_item_संख्या i = state_item_map[sn]; i < state_item_map[sn + 1]; ++i)
    अगर (!SI_DISABLED (i)
        && item_संख्या_as_rule_संख्या (*state_वस्तुएँ[i].item) == r->संख्या)
      वापस i;
  पात ();
}

व्योम
counterexample_report_state (स्थिर state *s, खाता *out, स्थिर अक्षर *prefix)
{
  स्थिर state_संख्या sn = s->संख्या;
  स्थिर reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
    {
      स्थिर rule *r1 = reds->rules[i];
      स्थिर state_item_संख्या c1 = find_state_item_संख्या (r1, sn);
      क्रम (state_item_संख्या c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
        अगर (!SI_DISABLED (c2))
          {
            item_संख्या conf = *state_वस्तुएँ[c2].item;
            अगर (item_संख्या_is_प्रतीक_संख्या (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_shift_reduce (c1, c2, conf, out, prefix);
          }
      क्रम (पूर्णांक j = i+1; j < reds->अंक; ++j)
        {
          स्थिर rule *r2 = reds->rules[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          अगर (!bitset_empty_p (lookaheads))
            क्रम (state_item_संख्या c2 = state_item_map[sn]; c2 < state_item_map[sn + 1]; ++c2)
              अगर (!SI_DISABLED (c2)
                  && item_rule (state_वस्तुएँ[c2].item) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, out, prefix);
                  अवरोध;
                }
        }
    }
  bitset_free (lookaheads);
}
