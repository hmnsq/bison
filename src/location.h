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
வகைவரை கட்டமைப்பு
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  முழு வரி;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  முழு column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  முழு byte;

} boundary;

/* Set the position of \a p. */
நிலையான உள்வரி வெறுமை
boundary_set (boundary *p, மாறிலி எழுத்து *f, முழு l, முழு c, முழு b)
{
  p->file = f;
  p->வரி = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
நிலையான உள்வரி முழு
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  முழு res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  எனில் (!res)
    res = a.வரி - b.வரி;
  எனில் (!res)
    res = a.column - b.column;
  திரும்பு res;
}

/* Return nonzero if A and B are equal boundaries.  */
நிலையான உள்வரி bool
equal_boundaries (boundary a, boundary b)
{
  திரும்பு (a.column == b.column
          && a.வரி == b.வரி
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
வகைவரை கட்டமைப்பு
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{வெற்று, 0, 0, 0}, {வெற்று, 0, 0, 0}}
வெளி location மாறிலி empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
வெறுமை location_compute (location *loc,
                       boundary *cur, எழுத்து மாறிலி *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
முழு location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
வெறுமை caret_init (வெறுமை);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
வெறுமை caret_free (வெறுமை);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
வெறுமை location_caret (location loc, மாறிலி எழுத்து* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
வெறுமை location_caret_suggestion (location loc, மாறிலி எழுத்து *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
நிலையான உள்வரி முழு
location_cmp (location a, location b)
{
  முழு res = boundary_cmp (a.start, b.start);
  எனில் (!res)
    res = boundary_cmp (a.end, b.end);
  திரும்பு res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
வெறுமை boundary_set_from_string (boundary *bound, எழுத்து *str);

#endif /* ! defined LOCATION_H_ */
