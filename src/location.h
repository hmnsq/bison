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
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା byte;

} boundary;

/* Set the position of \a p. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
boundary_set (boundary *p, ଅଚଳ ଅକ୍ଷର *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
  p->file = f;
  p->ଧାଡ଼ି = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  ଯଦି (!res)
    res = a.ଧାଡ଼ି - b.ଧାଡ଼ି;
  ଯଦି (!res)
    res = a.column - b.column;
  ଫେରନ୍ତୁ res;
}

/* Return nonzero if A and B are equal boundaries.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
equal_boundaries (boundary a, boundary b)
{
  ଫେରନ୍ତୁ (a.column == b.column
          && a.ଧାଡ଼ି == b.ଧାଡ଼ି
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{ଖାଲି, 0, 0, 0}, {ଖାଲି, 0, 0, 0}}
ବାହ୍ୟ location ଅଚଳ empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
ଶୂନ୍ୟ location_compute (location *loc,
                       boundary *cur, ଅକ୍ଷର ଅଚଳ *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
ଶୂନ୍ୟ caret_init (ଶୂନ୍ୟ);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
ଶୂନ୍ୟ caret_free (ଶୂନ୍ୟ);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
ଶୂନ୍ୟ location_caret (location loc, ଅଚଳ ଅକ୍ଷର* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
ଶୂନ୍ୟ location_caret_suggestion (location loc, ଅଚଳ ଅକ୍ଷର *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
location_cmp (location a, location b)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = boundary_cmp (a.start, b.start);
  ଯଦି (!res)
    res = boundary_cmp (a.end, b.end);
  ଫେରନ୍ତୁ res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
ଶୂନ୍ୟ boundary_set_from_string (boundary *bound, ଅକ୍ଷର *str);

#endif /* ! defined LOCATION_H_ */
