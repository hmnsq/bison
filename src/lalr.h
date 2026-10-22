/* Compute lookahead criteria for bison,

   Copyright (C) 1984, 1986, 1989, 2000, 2002, 2004, 2006-2007,
   2009-2015, 2018-2022 Free Software Foundation, Inc.

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

#ifndef LALR_H_
# define LALR_H_

# include <bitset.h>
# include <bitsetv.h>

/* Import the definition of RULE_T. */
# include "gram.h"

/* Import the definition of CORE, TRANSITIONS and REDUCTIONS. */
# include "state.h"


/** Build the LALR(1) automaton.

   Find which rules need lookahead in each state, and which lookahead
   tokens they accept.

   Also builds:
     - #goto_map
     - #from_state
     - #to_state
     - #goto_follows
*/
வெறுமை lalr (வெறுமை);

/**
 * Set #nLA and allocate all reduction lookahead sets.  Normally invoked by
 * #lalr.
 */
வெறுமை initialize_LA (வெறுமை);

/**
 * Build only:
 *   - #goto_map
 *   - #from_state
 *   - #to_state
 * Normally invoked by #lalr.
 */
வெறுமை set_goto_map (வெறுமை);

/**
 * Update state numbers recorded in #goto_map, #from_state, and #to_state such
 * that:
 *   - \c nstates_old is the old number of states.
 *   - Where \c i is the old state number, <tt>old_to_new[i]</tt> is either:
 *     - \c nstates_old if state \c i is removed because it is unreachable.
 *       Thus, remove all goto entries involving this state.
 *     - The new state number.
 */
வெறுமை lalr_update_state_numbers (state_எண் old_to_new[],
                                state_எண் nstates_old);


/** Release the information related to lookahead tokens.

   Can be performed once the action tables are computed.  */
வெறுமை lalr_free (வெறுமை);

வகைவரை size_t goto_எண்;
# define GOTO_எண்_MAXIMUM ((goto_எண்) -1)

/** Index into #from_state and #to_state.

   All the transitions that accept a particular variable are grouped
   together in FROM_STATE and TO_STATE, with indexes from GOTO_MAP[I -
   NTOKENS] to GOTO_MAP[I - NTOKENS + 1] - 1 (including both).  */
வெளி goto_எண் *goto_map;

/** The size of #from_state and #to_state.  */
வெளி goto_எண் ngotos;

/** State number which a transition leads from.  */
வெளி state_எண் *from_state;

/** State number it leads to.  */
வெளி state_எண் *to_state;

/** The number of the goto from state SRC labeled with nterm SYM.  */
goto_எண் map_goto (state_எண் src, குறியீடு_எண் sym);

/* goto_follows[i] is the set of tokens following goto i.  */
வெளி bitsetv goto_follows;

#endif /* !LALR_H_ */
