/* Locations for Bison

   Copyright (C) 2002, 2004-2015, 2018-2022 Free Software Foundation,
   Inc.

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

#ifndef LOCATION_H_
# define LOCATION_H_

# include <stdbool.h>
# include <stdio.h>
# include <string.h> /* strcmp */

# include "uniqstr.h"

/* A boundary between two characters.  */
تعريفنوع بنية
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  صحيح سطر;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  صحيح column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  صحيح byte;

} boundary;

/* Set the position of \a p. */
ساكن مضمن فراغ
boundary_set (boundary *p, ثابت حرف *f, صحيح l, صحيح c, صحيح b)
{
  p->file = f;
  p->سطر = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ساكن مضمن صحيح
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  صحيح res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  إذا (!res)
    res = a.سطر - b.سطر;
  إذا (!res)
    res = a.column - b.column;
  أرجع res;
}

/* Return nonzero if A and B are equal boundaries.  */
ساكن مضمن bool
equal_boundaries (boundary a, boundary b)
{
  أرجع (a.column == b.column
          && a.سطر == b.سطر
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
تعريفنوع بنية
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{عدم, 0, 0, 0}, {عدم, 0, 0, 0}}
خارجي location ثابت empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
فراغ location_compute (location *loc,
                       boundary *cur, حرف ثابت *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
صحيح location_اطبع (location loc, FILE *out);

/* Prepare the use of location_caret.  */
فراغ caret_init (فراغ);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
فراغ caret_free (فراغ);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
فراغ location_caret (location loc, ثابت حرف* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
فراغ location_caret_suggestion (location loc, ثابت حرف *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ساكن مضمن صحيح
location_cmp (location a, location b)
{
  صحيح res = boundary_cmp (a.start, b.start);
  إذا (!res)
    res = boundary_cmp (a.end, b.end);
  أرجع res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
فراغ boundary_set_from_string (boundary *bound, حرف *str);

#endif /* ! defined LOCATION_H_ */
