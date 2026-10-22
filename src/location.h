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
প্রকারনির্দেশ সংরচনা
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  পূর্ণাঙ্ক সাৰি;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  পূর্ণাঙ্ক column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  পূর্ণাঙ্ক byte;

} boundary;

/* Set the position of \a p. */
স্থির অন্তঃস্থ শূন্য
boundary_set (boundary *p, ধ্রুৱ ৱর্ণ *f, পূর্ণাঙ্ক l, পূর্ণাঙ্ক c, পূর্ণাঙ্ক b)
{
  p->file = f;
  p->সাৰি = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  পূর্ণাঙ্ক res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  যদি (!res)
    res = a.সাৰি - b.সাৰি;
  যদি (!res)
    res = a.column - b.column;
  ঘূৰাই res;
}

/* Return nonzero if A and B are equal boundaries.  */
স্থির অন্তঃস্থ bool
equal_boundaries (boundary a, boundary b)
{
  ঘূৰাই (a.column == b.column
          && a.সাৰি == b.সাৰি
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
প্রকারনির্দেশ সংরচনা
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{রিক্ত, 0, 0, 0}, {রিক্ত, 0, 0, 0}}
বাহ্য location ধ্রুৱ empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
শূন্য location_compute (location *loc,
                       boundary *cur, ৱর্ণ ধ্রুৱ *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
পূর্ণাঙ্ক location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
শূন্য caret_init (শূন্য);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
শূন্য caret_free (শূন্য);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
শূন্য location_caret (location loc, ধ্রুৱ ৱর্ণ* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
শূন্য location_caret_suggestion (location loc, ধ্রুৱ ৱর্ণ *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
location_cmp (location a, location b)
{
  পূর্ণাঙ্ক res = boundary_cmp (a.start, b.start);
  যদি (!res)
    res = boundary_cmp (a.end, b.end);
  ঘূৰাই res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
শূন্য boundary_set_from_string (boundary *bound, ৱর্ণ *str);

#endif /* ! defined LOCATION_H_ */
