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

#ifndef STATE_ITEM_H
# define STATE_ITEM_H

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

# define SI_DISABLED(Sin) (state_বস্তুসমূহ[Sin].trans == -2)
# define SI_PRODUCTION(Si) ((Si) == state_বস্তুসমূহ || *((Si)->item - 1) < 0)
# define SI_TRANSITION(Si) ((Si) != state_বস্তুসমূহ && *((Si)->item - 1) >= 0)

প্রকারলেখ পূর্ণসংখ্যা state_item_সংখ্যা;

প্রকারলেখ কাঠামো
{
  ধ্রুবক state *state;
  item_সংখ্যা *item;
  state_item_সংখ্যা trans;
  bitset prods;
  bitset revs;
  bitset lookahead;
} state_item;

// A path of state-items.
প্রকারলেখ gl_list_t state_item_list;

বাহ্যিক bitsetv firsts;
# define FIRSTS(sym) firsts[(sym) - ntokens]

বাহ্যিক size_t nstate_বস্তুসমূহ;
বাহ্যিক state_item_সংখ্যা *state_item_map;

/** Array mapping state_item_numbers to state_items */
বাহ্যিক state_item *state_বস্তুসমূহ;

state_item *state_item_lookup (state_সংখ্যা s, state_item_সংখ্যা off);

স্থির অন্তঃরেখ state_item_সংখ্যা
state_item_সূচক_lookup (state_সংখ্যা s, state_item_সংখ্যা off)
{
  ফেরত state_item_map[s] + off;
}

শূন্য state_বস্তুসমূহ_init (শূন্য);
শূন্য state_বস্তুসমূহ_free (শূন্য);

শূন্য state_item_print (ধ্রুবক state_item *si, FILE *out, ধ্রুবক অক্ষর *prefix);
ধ্রুবক rule *state_item_rule (ধ্রুবক state_item *si);

bool production_allowed (ধ্রুবক state_item *si, ধ্রুবক state_item *পরবর্তী);

// Iterating on a state_item_list.
স্থির অন্তঃরেখ bool
state_item_list_পরবর্তী (gl_list_iterator_t *it, state_item **si)
{
  ধ্রুবক শূন্য *p = ফাঁকা;
  bool res = gl_list_iterator_পরবর্তী (it, &p, ফাঁকা);
  যদি (res)
    *si = (state_item *) p;
  অন্যথা
    gl_list_iterator_free (it);
  ফেরত res;
}


#endif /* STATE_ITEM_H */
