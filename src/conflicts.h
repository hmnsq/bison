/* Find and resolve or report lookahead conflicts for bison,

   Copyright (C) 2000-2002, 2004, 2007, 2009-2015, 2018-2022 Free
   Software Foundation, Inc.

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

#ifndef CONFLICTS_H_
# define CONFLICTS_H_
# include "state.h"

শূন্য conflicts_solve (শূন্য);

/**
 * Update state numbers recorded in internal arrays such that:
 *   - \c nstates_old is the old number of states.
 *   - Where \c i is the old state number, <tt>old_to_new[i]</tt> is either:
 *     - \c nstates_old if state \c i is removed because it is unreachable.
 *     - The new state number.
 *   - The highest new state number is the number of remaining states - 1.
 *   - The numerical order of the remaining states has not changed.
 */
শূন্য conflicts_update_state_numbers (state_সংখ্যা old_to_new[],
                                     state_সংখ্যা nstates_old);

শূন্য conflicts_print (শূন্য);
পূর্ণাঙ্ক conflicts_total_গুনতি (শূন্য);
শূন্য conflicts_output (FILE *out);
শূন্য conflicts_free (শূন্য);

bool has_conflicts (ধ্রুব state *s);

/* Were there conflicts? */
বাহ্য পূর্ণাঙ্ক expected_sr_conflicts;
বাহ্য পূর্ণাঙ্ক expected_rr_conflicts;

#endif /* !CONFLICTS_H_ */
