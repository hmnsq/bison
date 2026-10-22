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

#که_نه_تعریف_شوی STATE_توکی_H
# define STATE_توکی_H

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

# define SI_DISABLED(Sin) (state_توکي[Sin].trans == -2)
# define SI_PRODUCTION(Si) ((Si) == state_توکي || *((Si)->توکی - 1) < 0)
# define SI_TRANSITION(Si) ((Si) != state_توکي && *((Si)->توکی - 1) >= 0)

تعريفنوع صحيح state_توکی_شمېره;

تعريفنوع بنية
{
  ثابت state *state;
  توکی_شمېره *توکی;
  state_توکی_شمېره trans;
  bitset prods;
  bitset revs;
  bitset lookahead;
} state_توکی;

// A path of state-items.
تعريفنوع gl_list_t state_توکی_list;

خارجي bitsetv firsts;
# define FIRSTS(sym) firsts[(sym) - ntokens]

خارجي size_t nstate_توکي;
خارجي state_توکی_شمېره *state_توکی_map;

/** Array mapping state_item_numbers to state_items */
خارجي state_توکی *state_توکي;

state_توکی *state_توکی_lookup (state_شمېره s, state_توکی_شمېره off);

ساكن مضمن state_توکی_شمېره
state_توکی_شاخص_lookup (state_شمېره s, state_توکی_شمېره off)
{
  أرجع state_توکی_map[s] + off;
}

فراغ state_توکي_init (فراغ);
فراغ state_توکي_free (فراغ);

فراغ state_توکی_print (ثابت state_توکی *si, FILE *بهر, ثابت حرف *مختاړی);
ثابت قاعده *state_توکی_قاعده (ثابت state_توکی *si);

bool production_allowed (ثابت state_توکی *si, ثابت state_توکی *بل);

// Iterating on a state_item_list.
ساكن مضمن bool
state_توکی_list_بل (gl_list_iterator_t *it, state_توکی **si)
{
  ثابت فراغ *p = عدم;
  bool res = gl_list_iterator_بل (it, &p, عدم);
  إذا (res)
    *si = (state_توکی *) p;
  وإلا
    gl_list_iterator_free (it);
  أرجع res;
}


#د_که_پای /* STATE_ITEM_H */
