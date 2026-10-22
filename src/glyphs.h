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
શૂન્ય glyphs_init (શૂન્ય);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
પ્રકારવ્યાખ્યા અક્ષર glyph_buffer_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
બાહ્ય glyph_buffer_t arrow;
બાહ્ય પૂર્ણાંક arrow_width;

/* "•", a point in an item (aka, a dotted rule).  */
બાહ્ય glyph_buffer_t dot;
બાહ્ય પૂર્ણાંક dot_width;

/* "↳ ", below an lhs to announce the rhs.  */
બાહ્ય glyph_buffer_t down_arrow;
બાહ્ય પૂર્ણાંક down_arrow_width;

/* "ε", an empty rhs.  */
બાહ્ય glyph_buffer_t empty;
બાહ્ય પૂર્ણાંક empty_width;

/* " ", separate symbols in the rhs of a derivation.  */
બાહ્ય અચલ અક્ષર *derivation_separator;
બાહ્ય પૂર્ણાંક derivation_separator_width;

#endif /* GLYPHS_H */
