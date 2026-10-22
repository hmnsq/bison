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
തരനിർവചനം ഘടന
{
  /* The name of the file that contains the boundary.  */
  uniqstr file;

  /* If positive, the line (starting at 1) that contains the boundary.
     If this is INT_MAX, the line number has overflowed.

     Meaningless and not displayed if nonpositive.
  */
  പൂർണ്ണസംഖ്യ വരി;

  /* If positive, the column (starting at 1) just after the boundary.
     This is neither a byte count, nor a character count; it is a
     (visual) column count.  If this is INT_MAX, the column number has
     overflowed.

     Meaningless and not displayed if nonpositive.  */
  പൂർണ്ണസംഖ്യ column;

  /* If nonnegative, the byte number (starting at 0) in the current
     line.  Not displayed (unless --trace=location).  */
  പൂർണ്ണസംഖ്യ byte;

} boundary;

/* Set the position of \a p. */
സ്ഥിതം വരിയിൽ ശൂന്യം
boundary_set (boundary *p, സ്ഥിരം അക്ഷരം *f, പൂർണ്ണസംഖ്യ l, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ b)
{
  p->file = f;
  p->വരി = l;
  p->column = c;
  p->byte = b;
}

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
boundary_cmp (boundary a, boundary b)
{
  /* Locations with no file first.  */
  പൂർണ്ണസംഖ്യ res =
    a.file && b.file ? strcmp (a.file, b.file)
    : a.file ? 1
    : b.file ? -1
    : 0;
  എങ്കിൽ (!res)
    res = a.വരി - b.വരി;
  എങ്കിൽ (!res)
    res = a.column - b.column;
  തിരികെ res;
}

/* Return nonzero if A and B are equal boundaries.  */
സ്ഥിതം വരിയിൽ bool
equal_boundaries (boundary a, boundary b)
{
  തിരികെ (a.column == b.column
          && a.വരി == b.വരി
          && UNIQSTR_EQ (a.file, b.file));
}

/* A location, that is, a region of source code.  */
തരനിർവചനം ഘടന
{
  /* Boundary just before the location starts.  */
  boundary start;

  /* Boundary just after the location ends.  */
  boundary end;

} location;

# define GRAM_LTYPE location

# define EMPTY_LOCATION_INIT {{ശൂന്യ, 0, 0, 0}, {ശൂന്യ, 0, 0, 0}}
ബാഹ്യ location സ്ഥിരം empty_loc;

/* Set *LOC and adjust scanner cursor to account for token TOKEN of
   size SIZE.  */
ശൂന്യം location_compute (location *loc,
                       boundary *cur, അക്ഷരം സ്ഥിരം *token, size_t size);

/* Print location to file.
   Return number of actually printed characters.
   Warning: uses quotearg's slot 3. */
പൂർണ്ണസംഖ്യ location_print (location loc, FILE *out);

/* Prepare the use of location_caret.  */
ശൂന്യം caret_init (ശൂന്യം);

/* Free any allocated resources and close any open file handles that are
   left-over by the usage of location_caret.  */
ശൂന്യം caret_free (ശൂന്യം);

/* If -fcaret is enabled, quote the line containing LOC onto OUT.
   Highlight the part of LOC with the color STYLE.  */
ശൂന്യം location_caret (location loc, സ്ഥിരം അക്ഷരം* style, FILE *out);

/* If -fcaret is enabled, display a suggestion of replacement for LOC
   with S.  To call after location_caret.  */
ശൂന്യം location_caret_suggestion (location loc, സ്ഥിരം അക്ഷരം *s, FILE *out);

/* Return -1, 0, 1, depending whether a is before, equal, or
   after b.  */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
location_cmp (location a, location b)
{
  പൂർണ്ണസംഖ്യ res = boundary_cmp (a.start, b.start);
  എങ്കിൽ (!res)
    res = boundary_cmp (a.end, b.end);
  തിരികെ res;
}

/* Whether this is the empty location.  */
bool location_empty (location loc);

/* STR must be formatted as 'file:line.column@byte' or 'file:line.column'.
   It may be '<command line>:3.-1@-1', with -1 to denote no-column/no-byte.
   STR will be modified.  */
ശൂന്യം boundary_set_from_string (boundary *bound, അക്ഷരം *str);

#endif /* ! defined LOCATION_H_ */
