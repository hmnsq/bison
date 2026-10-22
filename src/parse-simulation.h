/* Parser simulator for unifying counterexample search

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PARSE_SIMULATION_H
# define PARSE_SIMULATION_H

# include <stdio.h>
# include <gl_xlist.h>

# include "derivation.h"
# include "state-item.h"

/*
  Simulating states of the parser:
  Each state is an array of state-items and an array of derivations.
  Each consecutive state-item represents a transition/goto or production,
  and the derivations are the derivation trees associated with the symbols
  transitioned on each step. In more detail:

  Parse states are stored as a tree. Each new parse state contains two "chunks,"
  one corresponding to its state-items and the other corresponding to its derivations.
  Chunks only have new elements which weren't present in its parent.
  Each chunk also stores the head, tail, and total_size of the list it represents.
  So if a parse state was to be copied it retains the list metadata but its
  contents are empty.

  A transition gets the state-item which the last state-item of the parse state
  transitions to. This is appended to the state-item list, and a derivation with
  just the symbol being transitioned on is appended to the derivation list.
  A production appends the new state-item, but does not have a derivation
  associated with it.

  A reduction looks at the rule of the last state-item in the state, and pops
  the last few state-items that make up the rhs of the rule along with their
  derivations. The derivations become the derivation of the lhs which is then
  shifted over.

  Effectively, every time a derivation is appended, it represents a shift in
  the parser. So a parse state that contains
   start: A . B C D
   start: A B C D .
  and the state-items in between will represent a parser that has BCD on the
  parse stack.

  However, the above example cannot be reduced, as it's missing A.
  Since we start at a state-item that can have a dot in the middle of a rule,
  it's necessary to support a prepend operation. Luckily the prepend operations
  are very similar to transitions and productions with the difference being that
  they operate on the head of the state-item list instead of the tail.

  A production
  A transition gets the state-item which the last state-item of the parse state
  transitions to. This is appended to the state-item list, and a derivation with
  just the symbol being transitioned on is appended to the derivation list.

 */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ parse_state parse_state;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t parse_state_list;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
parse_state_list_ಮುಂದಿನ (gl_list_iterator_t *it, parse_state **ps)
{
  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p = ಖಾಲಿ;
  bool res = gl_list_iterator_ಮುಂದಿನ (it, &p, ಖಾಲಿ);
  ಆದರೆ (res)
    *ps = (parse_state *) p;
  ಇಲ್ಲದಿದ್ದರೆ
    gl_list_iterator_free (it);
  ಹಿಂತಿರುಗಿ res;
}

parse_state *new_parse_state (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *conflict);

size_t parse_state_hasher (ಸ್ಥಿರಾಂಕ parse_state *ps, size_t max);

bool parse_state_comparator (ಸ್ಥಿರಾಂಕ parse_state *ps1, ಸ್ಥಿರಾಂಕ parse_state *ps2);

/* Memory management */

ಶೂನ್ಯ parse_state_retain (parse_state *ps);
/* This allows a parse_state to free its contents list
 * when its reference count reaches 1. This is used to
 * free memory while the parse state is in a hash set. */
ಶೂನ್ಯ parse_state_free_contents_early (parse_state *ps);
ಶೂನ್ಯ free_parse_state (parse_state *ps);

/* counts the amount of shift and production steps in this parse state */
ಶೂನ್ಯ parse_state_completed_ಹೆಜ್ಜೆಗಳು (ಸ್ಥಿರಾಂಕ parse_state *ps, ಪೂರ್ಣಾಂಕ *shifts, ಪೂರ್ಣಾಂಕ *productions);

/* parse state getters */
bool parse_state_derivation_completed (ಸ್ಥಿರಾಂಕ parse_state *ps);
derivation *parse_state_derivation (ಸ್ಥಿರಾಂಕ parse_state *ps);
ಸ್ಥಿರಾಂಕ state_ಅಂಶ *parse_state_ತಲೆ (ಸ್ಥಿರಾಂಕ parse_state *ps);
ಸ್ಥಿರಾಂಕ state_ಅಂಶ *parse_state_ಬಾಲ (ಸ್ಥಿರಾಂಕ parse_state *ps);
ಪೂರ್ಣಾಂಕ parse_state_ಉದ್ದಳತೆ (ಸ್ಥಿರಾಂಕ parse_state *ps);
ಪೂರ್ಣಾಂಕ parse_state_ಆಳ (ಸ್ಥಿರಾಂಕ parse_state *ps);

/* returns the linked lists that the parse state is supposed to represent */
ಶೂನ್ಯ parse_state_lists (parse_state *ps, state_ಅಂಶ_list *state_ವಸ್ತುಗಳು,
                        derivation_list *derivs);

/* various functions that return a list of states based off of
 * whatever operation is simulated. After whatever operation, every possible
 * transition on nullable nonterminals will be added to the returned list. */

/* Look at the tail state-item of the parse state and transition on the symbol
 * after its dot. The symbol gets added to derivs, and the resulting state-item
 * is appended to state-items. */
parse_state_list simulate_transition (parse_state *ps);

/* Look at all of the productions for the nonterminal following the dot in the tail
 * state-item. Appends to state-items each production state-item which may start with
 * compat_sym. */
parse_state_list simulate_production (parse_state *ps, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ compat_sym);

/* Removes the last rule_len state-items along with their derivations. A new state-item is
 * appended representing the goto after the reduction. A derivation for the nonterminal that
 * was just reduced is appended which consists of the list of derivations that were just removed. */
parse_state_list simulate_reduction (parse_state *ps, ಪೂರ್ಣಾಂಕ ನಿಯಮ_len,
                              bitset ಚಿಹ್ನೆ_ಗಣ);

/* Generate states with a state-item prepended for each state-item that has a
 * transition or production step to ps's head. */
parse_state_list parser_prepend (parse_state *ps);

/* For debugging traces.  */
ಶೂನ್ಯ print_parse_state (parse_state *ps);

#ಆದರೆ_ಅಂತ್ಯ /* PARSE_SIMULATION_H */
