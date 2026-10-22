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

# define SI_DISABLED(Sin) (state_ഇനങ്ങൾ[Sin].trans == -2)
# define SI_PRODUCTION(Si) ((Si) == state_ഇനങ്ങൾ || *((Si)->item - 1) < 0)
# define SI_TRANSITION(Si) ((Si) != state_ഇനങ്ങൾ && *((Si)->item - 1) >= 0)

തരനിർവചനം പൂർണ്ണസംഖ്യ state_item_സംഖ്യ;

തരനിർവചനം ഘടന
{
  സ്ഥിരം state *state;
  item_സംഖ്യ *item;
  state_item_സംഖ്യ trans;
  bitset prods;
  bitset revs;
  bitset lookahead;
} state_item;

// A path of state-items.
തരനിർവചനം gl_list_t state_item_list;

ബാഹ്യ bitsetv firsts;
# define FIRSTS(sym) firsts[(sym) - ntokens]

ബാഹ്യ size_t nstate_ഇനങ്ങൾ;
ബാഹ്യ state_item_സംഖ്യ *state_item_map;

/** Array mapping state_item_numbers to state_items */
ബാഹ്യ state_item *state_ഇനങ്ങൾ;

state_item *state_item_lookup (state_സംഖ്യ s, state_item_സംഖ്യ off);

സ്ഥിതം വരിയിൽ state_item_സംഖ്യ
state_item_index_lookup (state_സംഖ്യ s, state_item_സംഖ്യ off)
{
  തിരികെ state_item_map[s] + off;
}

ശൂന്യം state_ഇനങ്ങൾ_init (ശൂന്യം);
ശൂന്യം state_ഇനങ്ങൾ_free (ശൂന്യം);

ശൂന്യം state_item_print (സ്ഥിരം state_item *si, FILE *out, സ്ഥിരം അക്ഷരം *prefix);
സ്ഥിരം rule *state_item_rule (സ്ഥിരം state_item *si);

bool production_allowed (സ്ഥിരം state_item *si, സ്ഥിരം state_item *next);

// Iterating on a state_item_list.
സ്ഥിതം വരിയിൽ bool
state_item_list_next (gl_list_iterator_t *it, state_item **si)
{
  സ്ഥിരം ശൂന്യം *p = ശൂന്യ;
  bool res = gl_list_iterator_next (it, &p, ശൂന്യ);
  എങ്കിൽ (res)
    *si = (state_item *) p;
  അല്ലെങ്കിൽ
    gl_list_iterator_free (it);
  തിരികെ res;
}


#endif /* STATE_ITEM_H */
