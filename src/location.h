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
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  ಪೂರ್ಣಾಙ್ಕ ಸಾಲು;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  ಪೂರ್ಣಾಙ್ಕ column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  ಪೂರ್ಣಾಙ್ಕ byte;

} boundary;

/* Set the position of \a p. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
boundary_set (boundary *p, ಧ್ರುವ ವರ್ಣ *f, ಪೂರ್ಣಾಙ್ಕ l, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ b)
{
  p->file = f;
  p->ಸಾಲು = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  ಪೂರ್ಣಾಙ್ಕ res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  ಯದಿ (!res)
    res = a.ಸಾಲು - b.ಸಾಲು;
  ಯದಿ (!res)
    res = a.column - b.column;
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Return nonzero if A and B are equal boundaries.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
equal_boundaries (boundary a, boundary b)
{
  ಪ್ರತ್ಯಾಗಮ (a.column == b.column
          && a.ಸಾಲು == b.ಸಾಲು
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{ರಿಕ್ತ, 0, 0, 0}, {ರಿಕ್ತ, 0, 0, 0}}
ಬಾಹ್ಯ location ಧ್ರುವ empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
ಶೂನ್ಯ location_compute (location *loc,
                       boundary *cur, ವರ್ಣ ಧ್ರುವ *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
ಪೂರ್ಣಾಙ್ಕ location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
ಶೂನ್ಯ caret_init (ಶೂನ್ಯ);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
ಶೂನ್ಯ caret_free (ಶೂನ್ಯ);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
ಶೂನ್ಯ location_caret (location loc, ಧ್ರುವ ವರ್ಣ* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
ಶೂನ್ಯ location_caret_suggestion (location loc, ಧ್ರುವ ವರ್ಣ *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
location_cmp (location a, location b)
{
  ಪೂರ್ಣಾಙ್ಕ res = boundary_cmp (a.start, b.start);
  ಯದಿ (!res)
    res = boundary_cmp (a.end, b.end);
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
ಶೂನ್ಯ boundary_set_from_string (boundary *bound, ವರ್ಣ *str);

#endif /* ! defined LOCATION_H_ */
