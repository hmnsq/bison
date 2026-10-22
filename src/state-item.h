/* Counterexample Generation Search Nodes

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STATE_ಅಂಶ_H
# define STATE_ಅಂಶ_H

# include <bitsetv.h>
# include <gl_list.h>
# include <hash.h>

# include "gram.h"
# include "state.h"

/* Initializes a graph connecting (state, production item) pairs to
   pairs they can make a transition or production step to.  This graph
   is used to search for paths that represent counterexamples of some
   conflict.

   state_items is an array of state state-item pairs ordered by state.
   state_item_map maps state numbers to the first item which
   corresponds to it in the array.  A state's portion in state_items
   begins with its items in the same order as it was in the state.
   This is then followed by productions from the closure of the state
   in order by rule.

   There are two type of edges in this graph transitions and
   productions.  Transitions are the same as transitions from the
   parser except edges are only between items from the same
   rule.

   Productions are edges from items with a nonterminal after the dot to
   the production of that nonterminal in the same state. These edges are
   stored as a bitset in a state-item.

   The inverses of these edges are stored in a bitset in the state-item,
   "revs."  A state-item that begins with a dot will have reverse
   production edges, and all others will have reverse transition
   edges. */

# define SI_DISABLED(Sin) (state_ವಸ್ತುಗಳು[Sin].trans == -2)
# define SI_PRODUCTION(Si) ((Si) == state_ವಸ್ತುಗಳು || *((Si)->ಅಂಶ - 1) < 0)
# define SI_TRANSITION(Si) ((Si) != state_ವಸ್ತುಗಳು && *((Si)->ಅಂಶ - 1) >= 0)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ state_ಅಂಶ_ಸಂಖ್ಯೆ;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಸ್ಥಿರಾಂಕ state *state;
  ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ;
  state_ಅಂಶ_ಸಂಖ್ಯೆ trans;
  bitset prods;
  bitset revs;
  bitset lookahead;
} state_ಅಂಶ;

// A path of state-items.
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t state_ಅಂಶ_list;

ಬಾಹ್ಯ bitsetv firsts;
# define FIRSTS(sym) firsts[(sym) - ntokens]

ಬಾಹ್ಯ size_t nstate_ವಸ್ತುಗಳು;
ಬಾಹ್ಯ state_ಅಂಶ_ಸಂಖ್ಯೆ *state_ಅಂಶ_map;

/** Array mapping state_item_numbers to state_items */
ಬಾಹ್ಯ state_ಅಂಶ *state_ವಸ್ತುಗಳು;

state_ಅಂಶ *state_ಅಂಶ_lookup (state_ಸಂಖ್ಯೆ s, state_ಅಂಶ_ಸಂಖ್ಯೆ off);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ state_ಅಂಶ_ಸಂಖ್ಯೆ
state_ಅಂಶ_ಸೂಚ್ಯಂಕ_lookup (state_ಸಂಖ್ಯೆ s, state_ಅಂಶ_ಸಂಖ್ಯೆ off)
{
  ಹಿಂತಿರುಗಿ state_ಅಂಶ_map[s] + off;
}

ಶೂನ್ಯ state_ವಸ್ತುಗಳು_init (ಶೂನ್ಯ);
ಶೂನ್ಯ state_ವಸ್ತುಗಳು_free (ಶೂನ್ಯ);

ಶೂನ್ಯ state_ಅಂಶ_print (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ);
ಸ್ಥಿರಾಂಕ ನಿಯಮ *state_ಅಂಶ_ನಿಯಮ (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si);

bool production_allowed (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si, ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ಮುಂದಿನ);

// Iterating on a state_item_list.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
state_ಅಂಶ_list_ಮುಂದಿನ (gl_list_iterator_t *it, state_ಅಂಶ **si)
{
  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p = ಖಾಲಿ;
  bool res = gl_list_iterator_ಮುಂದಿನ (it, &p, ಖಾಲಿ);
  ಆದರೆ (res)
    *si = (state_ಅಂಶ *) p;
  ಇಲ್ಲದಿದ್ದರೆ
    gl_list_iterator_free (it);
  ಹಿಂತಿರುಗಿ res;
}


#ಆದರೆ_ಅಂತ್ಯ /* STATE_ITEM_H */
