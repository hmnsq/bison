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

#यदि_अपरिभाषितम् GLYPHS_H
# define GLYPHS_H

/* Initialize the following variables.  */
शून्य glyphs_init (शून्य);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
प्रकारनिर्देश वर्ण glyph_बफर_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
बाह्य glyph_बफर_t arrow;
बाह्य पूर्णाङ्क arrow_विस्तारः;

/* "•", a point in an item (aka, a dotted rule).  */
बाह्य glyph_बफर_t dot;
बाह्य पूर्णाङ्क dot_विस्तारः;

/* "↳ ", below an lhs to announce the rhs.  */
बाह्य glyph_बफर_t अधस्तात्_arrow;
बाह्य पूर्णाङ्क अधस्तात्_arrow_विस्तारः;

/* "ε", an empty rhs.  */
बाह्य glyph_बफर_t रिक्तानि;
बाह्य पूर्णाङ्क रिक्तानि_विस्तारः;

/* " ", separate symbols in the rhs of a derivation.  */
बाह्य ध्रुव वर्ण *derivation_separator;
बाह्य पूर्णाङ्क derivation_separator_विस्तारः;

#यदि_अन्तः /* GLYPHS_H */
