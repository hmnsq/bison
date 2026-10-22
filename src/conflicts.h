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

ଶୂନ୍ୟ conflicts_solve (ଶୂନ୍ୟ);

/**
 * Update state numbers recorded in internal arrays such that:
 *   - \c nstates_old is the old number of states.
 *   - Where \c i is the old state number, <tt>old_to_new[i]</tt> is either:
 *     - \c nstates_old if state \c i is removed because it is unreachable.
 *     - The new state number.
 *   - The highest new state number is the number of remaining states - 1.
 *   - The numerical order of the remaining states has not changed.
 */
ଶୂନ୍ୟ conflicts_update_state_numbers (state_ସଂଖ୍ୟା old_to_new[],
                                     state_ସଂଖ୍ୟା nstates_old);

ଶୂନ୍ୟ conflicts_print (ଶୂନ୍ୟ);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା conflicts_total_ଗଣନା (ଶୂନ୍ୟ);
ଶୂନ୍ୟ conflicts_output (FILE *out);
ଶୂନ୍ୟ conflicts_free (ଶୂନ୍ୟ);

bool has_conflicts (ଅଚଳ state *s);

/* Were there conflicts? */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_sr_conflicts;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_rr_conflicts;

#endif /* !CONFLICTS_H_ */
