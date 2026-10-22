/* Lookahead sensitive state item searches for counterexample generation

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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LSSI_H
# define LSSI_H

# include "state-item.h"

/*
  All state-item graph nodes should also include a precise follow set (follow_L).
  However, ignoring follow_L saves a lot of memory and is a pretty good approximation.
  These functions exist to enforce restrictions caused by follow_L sets.
 */

/*
 * find shortest lookahead-sensitive path of state-items to target such that
 * next_sym is in the follow_L set of target in that position.
*/
state_ಅಂಶ_list ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ_from_ಆರಂಭ (state_ಅಂಶ_ಸಂಖ್ಯೆ ಗುರಿ,
                                          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym);

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool intersect_ಚಿಹ್ನೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, bitset syms);

/**
 * Determine if any symbol in ts is in syms
 * or can begin with a nonterminal in syms.
 */
bool intersect (bitset ts, bitset syms);

/**
 * Compute a set of sequences of state-items that can make production steps
 * to this state-item such that the resulting possible lookahead symbols are
 * as given.
 */
state_ಅಂಶ_list lssi_ಹಿಮ್ಮುಖ_production (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si, bitset lookahead);

#ಆದರೆ_ಅಂತ್ಯ /* LSSI_H */
