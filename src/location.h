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
প্রকারলেখ কাঠামো
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  পূর্ণসংখ্যা সারি;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  পূর্ণসংখ্যা column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  পূর্ণসংখ্যা byte;

} boundary;

/* Set the position of \a p. */
স্থির অন্তঃরেখ শূন্য
boundary_set (boundary *p, ধ্রুবক অক্ষর *f, পূর্ণসংখ্যা l, পূর্ণসংখ্যা c, পূর্ণসংখ্যা b)
{
  p->file = f;
  p->সারি = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  পূর্ণসংখ্যা res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  যদি (!res)
    res = a.সারি - b.সারি;
  যদি (!res)
    res = a.column - b.column;
  ফেরত res;
}

/* Return nonzero if A and B are equal boundaries.  */
স্থির অন্তঃরেখ bool
equal_boundaries (boundary a, boundary b)
{
  ফেরত (a.column == b.column
          && a.সারি == b.সারি
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
প্রকারলেখ কাঠামো
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{ফাঁকা, 0, 0, 0}, {ফাঁকা, 0, 0, 0}}
বাহ্যিক location ধ্রুবক empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
শূন্য location_compute (location *loc,
                       boundary *cur, অক্ষর ধ্রুবক *token, size_t আকার_2);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
পূর্ণসংখ্যা location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
শূন্য caret_init (শূন্য);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
শূন্য caret_free (শূন্য);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
শূন্য location_caret (location loc, ধ্রুবক অক্ষর* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
শূন্য location_caret_suggestion (location loc, ধ্রুবক অক্ষর *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
location_cmp (location a, location b)
{
  পূর্ণসংখ্যা res = boundary_cmp (a.start, b.start);
  যদি (!res)
    res = boundary_cmp (a.end, b.end);
  ফেরত res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
শূন্য boundary_set_from_string (boundary *bound, অক্ষর *str);

#endif /* ! defined LOCATION_H_ */
