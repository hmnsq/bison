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

#अगर_अघोषित CONFLICTS_H_
# define CONFLICTS_H_
# include "state.h"

व्योम conflicts_solve (व्योम);

/**
 * Update state numbers recorded in internal arrays such that:
 *   - \c nstates_old is the old number of states.
 *   - Where \c i is the old state number, <tt>old_to_new[i]</tt> is either:
 *     - \c nstates_old if state \c i is removed because it is unreachable.
 *     - The new state number.
 *   - The highest new state number is the number of remaining states - 1.
 *   - The numerical order of the remaining states has not changed.
 */
व्योम conflicts_update_state_numbers (state_संख्या old_to_new[],
                                     state_संख्या nstates_old);

व्योम conflicts_print (व्योम);
पूर्णांक conflicts_योग_गिनती (व्योम);
व्योम conflicts_output (खाता *out);
व्योम conflicts_free (व्योम);

bool has_conflicts (स्थिर state *s);

/* Were there conflicts? */
बाह्य पूर्णांक expected_sr_conflicts;
बाह्य पूर्णांक expected_rr_conflicts;

#पूर्ण_अगर /* !CONFLICTS_H_ */
