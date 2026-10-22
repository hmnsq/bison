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
ശൂന്യം fixits_register (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം* update);

/* Apply the fixits: update the source file.  */
ശൂന്യം fixits_run (ശൂന്യം);

/* Whether there are no fixits. */
bool fixits_empty (ശൂന്യം);

/* Free the registered fixits.  */
ശൂന്യം fixits_free (ശൂന്യം);

#endif /* !FIXITS_H_ */
