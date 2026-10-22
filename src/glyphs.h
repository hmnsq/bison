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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GLYPHS_H
# define GLYPHS_H

/* Initialize the following variables.  */
ಶೂನ್ಯ glyphs_init (ಶೂನ್ಯ);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ glyph_ಬಫರ್_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
ಬಾಹ್ಯ glyph_ಬಫರ್_t arrow;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ arrow_ಅಗಲ;

/* "•", a point in an item (aka, a dotted rule).  */
ಬಾಹ್ಯ glyph_ಬಫರ್_t dot;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ dot_ಅಗಲ;

/* "↳ ", below an lhs to announce the rhs.  */
ಬಾಹ್ಯ glyph_ಬಫರ್_t ಕೆಳಕ್ಕೆ_arrow;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ಕೆಳಕ್ಕೆ_arrow_ಅಗಲ;

/* "ε", an empty rhs.  */
ಬಾಹ್ಯ glyph_ಬಫರ್_t ಖಾಲಿಗಳು;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ಖಾಲಿಗಳು_ಅಗಲ;

/* " ", separate symbols in the rhs of a derivation.  */
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *derivation_separator;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ derivation_separator_ಅಗಲ;

#ಆದರೆ_ಅಂತ್ಯ /* GLYPHS_H */
