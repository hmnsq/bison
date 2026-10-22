/* Support for fixing grammar files.

   Copyright (C) 2019-2022 Free Software Foundation, Inc.

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

#ifndef FIXITS_H_
# define FIXITS_H_ 1

# include "location.h"

/* Declare a fix to apply.  */
تهی fixits_register (location ثابت *loc, نویسه ثابت* update);

/* Apply the fixits: update the source file.  */
تهی fixits_run (تهی);

/* Whether there are no fixits. */
bool fixits_empty (تهی);

/* Free the registered fixits.  */
تهی fixits_free (تهی);

#endif /* !FIXITS_H_ */
