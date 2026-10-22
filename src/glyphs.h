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

#अगर_अघोषित GLYPHS_H
# define GLYPHS_H

/* Initialize the following variables.  */
व्योम glyphs_init (व्योम);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
प्रकार अक्षर glyph_बफर_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
बाह्य glyph_बफर_t arrow;
बाह्य पूर्णांक arrow_width;

/* "•", a point in an item (aka, a dotted rule).  */
बाह्य glyph_बफर_t dot;
बाह्य पूर्णांक dot_width;

/* "↳ ", below an lhs to announce the rhs.  */
बाह्य glyph_बफर_t down_arrow;
बाह्य पूर्णांक down_arrow_width;

/* "ε", an empty rhs.  */
बाह्य glyph_बफर_t empty;
बाह्य पूर्णांक empty_width;

/* " ", separate symbols in the rhs of a derivation.  */
बाह्य स्थिर अक्षर *derivation_separator;
बाह्य पूर्णांक derivation_separator_width;

#पूर्ण_अगर /* GLYPHS_H */
