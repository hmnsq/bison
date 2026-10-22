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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFLICTS_H_
# define CONFLICTS_H_
# include "state.h"

ಶೂನ್ಯ conflicts_solve (ಶೂನ್ಯ);

/**
 * Update state numbers recorded in internal arrays such that:
 *   - \c nstates_old is the old number of states.
 *   - Where \c i is the old state number, <tt>old_to_new[i]</tt> is either:
 *     - \c nstates_old if state \c i is removed because it is unreachable.
 *     - The new state number.
 *   - The highest new state number is the number of remaining states - 1.
 *   - The numerical order of the remaining states has not changed.
 */
ಶೂನ್ಯ conflicts_update_state_numbers (state_ಸಂಖ್ಯೆ old_ಗಮ್ಯ_new[],
                                     state_ಸಂಖ್ಯೆ nstates_old);

ಶೂನ್ಯ conflicts_print (ಶೂನ್ಯ);
ಪೂರ್ಣಾಂಕ conflicts_ಒಟ್ಟು_ಎಣಿಕೆ (ಶೂನ್ಯ);
ಶೂನ್ಯ conflicts_output (FILE *ಹೊರಹೋಗು);
ಶೂನ್ಯ conflicts_free (ಶೂನ್ಯ);

bool has_conflicts (ಸ್ಥಿರಾಂಕ state *s);

/* Were there conflicts? */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ expected_sr_conflicts;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ expected_rr_conflicts;

#ಆದರೆ_ಅಂತ್ಯ /* !CONFLICTS_H_ */
