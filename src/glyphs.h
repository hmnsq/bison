/* Graphical symbols.

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ifndef GLYPHS_H
# define GLYPHS_H

/* Initialize the following variables.  */
تهی glyphs_init (تهی);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
تعریفنوع نویسه glyph_buffer_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
بیرونی glyph_buffer_t arrow;
بیرونی صحیح arrow_width;

/* "•", a point in an item (aka, a dotted rule).  */
بیرونی glyph_buffer_t dot;
بیرونی صحیح dot_width;

/* "↳ ", below an lhs to announce the rhs.  */
بیرونی glyph_buffer_t down_arrow;
بیرونی صحیح down_arrow_width;

/* "ε", an empty rhs.  */
بیرونی glyph_buffer_t empty;
بیرونی صحیح empty_width;

/* " ", separate symbols in the rhs of a derivation.  */
بیرونی ثابت نویسه *derivation_separator;
بیرونی صحیح derivation_separator_width;

#endif /* GLYPHS_H */
