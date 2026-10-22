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
تعریفنوع ساختار
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  صحیح سطر;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  صحیح column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  صحیح byte;

} boundary;

/* Set the position of \a p. */
ایستا درونخطی تهی
boundary_set (boundary *p, ثابت نویسه *f, صحیح l, صحیح c, صحیح b)
{
  p->file = f;
  p->سطر = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ایستا درونخطی صحیح
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  صحیح res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  اگر (!res)
    res = a.سطر - b.سطر;
  اگر (!res)
    res = a.column - b.column;
  بازگشت res;
}

/* Return nonzero if A and B are equal boundaries.  */
ایستا درونخطی bool
equal_boundaries (boundary a, boundary b)
{
  بازگشت (a.column == b.column
          && a.سطر == b.سطر
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
تعریفنوع ساختار
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{پوچ, 0, 0, 0}, {پوچ, 0, 0, 0}}
بیرونی location ثابت empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
تهی location_compute (location *loc,
                       boundary *cur, نویسه ثابت *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
صحیح location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
تهی caret_init (تهی);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
تهی caret_free (تهی);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
تهی location_caret (location loc, ثابت نویسه* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
تهی location_caret_suggestion (location loc, ثابت نویسه *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ایستا درونخطی صحیح
location_cmp (location a, location b)
{
  صحیح res = boundary_cmp (a.start, b.start);
  اگر (!res)
    res = boundary_cmp (a.end, b.end);
  بازگشت res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
تهی boundary_set_from_string (boundary *bound, نویسه *str);

#endif /* ! defined LOCATION_H_ */
