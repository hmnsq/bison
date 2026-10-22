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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LOCATION_H_
# define LOCATION_H_

# include <stdbool.h>
# include <stdio.h>
# include <string.h> /* strcmp */

# include "uniqstr.h"

/* A boundary between two characters.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  ಪೂರ್ಣಾಂಕ ಸಾಲು;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  ಪೂರ್ಣಾಂಕ ಸ್ತಂಭ;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  ಪೂರ್ಣಾಂಕ byte;

} boundary;

/* Set the position of \a p. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
boundary_ಗಣ (boundary *p, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *f, ಪೂರ್ಣಾಂಕ l, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ b)
{
  p->file = f;
  p->ಸಾಲು = l;
  p->ಸ್ತಂಭ = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  ಪೂರ್ಣಾಂಕ res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  ಆದರೆ (!res)
    res = a.ಸಾಲು - b.ಸಾಲು;
  ಆದರೆ (!res)
    res = a.ಸ್ತಂಭ - b.ಸ್ತಂಭ;
  ಹಿಂತಿರುಗಿ res;
}

/* Return nonzero if A and B are equal boundaries.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
equal_boundaries (boundary a, boundary b)
{
  ಹಿಂತಿರುಗಿ (a.ಸ್ತಂಭ == b.ಸ್ತಂಭ
          && a.ಸಾಲು == b.ಸಾಲು
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  /* Boundary just before the location starts.  */
  boundary ಆರಂಭ;

  /* Boundary just after the location ends.  */
  boundary ಕೊನೆ;

} location;

# define GRAM_LTYPE location

# define ಖಾಲಿಗಳು_LOCATION_INIT {{ಖಾಲಿ, 0, 0, 0}, {ಖಾಲಿ, 0, 0, 0}}
ಬಾಹ್ಯ location ಸ್ಥಿರಾಂಕ ಖಾಲಿಗಳು_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
ಶೂನ್ಯ location_compute (location *loc,
                       boundary *cur, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *token, size_t ಗಾತ್ರ_2);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
ಪೂರ್ಣಾಂಕ location_print (location loc, FILE *ಹೊರಹೋಗು);

/* Prepare the use of location_caret.  */
ಶೂನ್ಯ caret_init (ಶೂನ್ಯ);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
ಶೂನ್ಯ caret_free (ಶೂನ್ಯ);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
ಶೂನ್ಯ location_caret (location loc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* style, FILE *ಹೊರಹೋಗು);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
ಶೂನ್ಯ location_caret_suggestion (location loc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, FILE *ಹೊರಹೋಗು);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
location_cmp (location a, location b)
{
  ಪೂರ್ಣಾಂಕ res = boundary_cmp (a.ಆರಂಭ, b.ಆರಂಭ);
  ಆದರೆ (!res)
    res = boundary_cmp (a.ಕೊನೆ, b.ಕೊನೆ);
  ಹಿಂತಿರುಗಿ res;
}

/* Whether this is the empty location.  */
bool location_ಖಾಲಿಗಳು (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
ಶೂನ್ಯ boundary_ಗಣ_from_string (boundary *bound, ಅಕ್ಷರ *str);

#ಆದರೆ_ಅಂತ್ಯ /* ! defined LOCATION_H_ */
