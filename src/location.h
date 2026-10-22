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
प्रकारनिर्देश संरचना
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  पूर्णाङ्क पङ्क्ति;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  पूर्णाङ्क column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  पूर्णाङ्क byte;

} boundary;

/* Set the position of \a p. */
स्थिर अन्तःस्थ शून्य
boundary_set (boundary *p, अचर वर्ण *f, पूर्णाङ्क l, पूर्णाङ्क c, पूर्णाङ्क b)
{
  p->file = f;
  p->पङ्क्ति = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
स्थिर अन्तःस्थ पूर्णाङ्क
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  पूर्णाङ्क res =
    a.file && b.file ? म_भेद (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  यदि (!res)
    res = a.पङ्क्ति - b.पङ्क्ति;
  यदि (!res)
    res = a.column - b.column;
  फर्काऊ res;
}

/* Return nonzero if A and B are equal boundaries.  */
स्थिर अन्तःस्थ bool
equal_boundaries (boundary a, boundary b)
{
  फर्काऊ (a.column == b.column
          && a.पङ्क्ति == b.पङ्क्ति
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
प्रकारनिर्देश संरचना
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{रिक्त, 0, 0, 0}, {रिक्त, 0, 0, 0}}
बाह्य location अचर empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
शून्य location_compute (location *loc,
                       boundary *cur, वर्ण अचर *token, माप_प्रकार size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
पूर्णाङ्क location_print (location loc, खाता *out);

/* Prepare the use of location_caret.  */
शून्य caret_init (शून्य);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
शून्य caret_free (शून्य);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
शून्य location_caret (location loc, अचर वर्ण* style, खाता *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
शून्य location_caret_suggestion (location loc, अचर वर्ण *s, खाता *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
स्थिर अन्तःस्थ पूर्णाङ्क
location_cmp (location a, location b)
{
  पूर्णाङ्क res = boundary_cmp (a.start, b.start);
  यदि (!res)
    res = boundary_cmp (a.end, b.end);
  फर्काऊ res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
शून्य boundary_set_from_string (boundary *bound, वर्ण *str);

#endif /* ! defined LOCATION_H_ */
