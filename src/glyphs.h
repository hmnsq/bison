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

#جيڪڏهن_وصف_نه_ٿيل GLYPHS_H
# define GLYPHS_H

/* Initialize the following variables.  */
فراغ glyphs_init (فراغ);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
تعريفنوع حرف glyph_بفر_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
خارجي glyph_بفر_t arrow;
خارجي صحيح arrow_ويڪر;

/* "•", a point in an item (aka, a dotted rule).  */
خارجي glyph_بفر_t dot;
خارجي صحيح dot_ويڪر;

/* "↳ ", below an lhs to announce the rhs.  */
خارجي glyph_بفر_t هيٺ_ڏانهن_arrow;
خارجي صحيح هيٺ_ڏانهن_arrow_ويڪر;

/* "ε", an empty rhs.  */
خارجي glyph_بفر_t خالي_جايون;
خارجي صحيح خالي_جايون_ويڪر;

/* " ", separate symbols in the rhs of a derivation.  */
خارجي ثابت حرف *derivation_separator;
خارجي صحيح derivation_separator_ويڪر;

#جيڪڏهن_پورو /* GLYPHS_H */
