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
শূন্য glyphs_init (শূন্য);

/* In gnulib/lib/unicodeio.h unicode_to_mb uses a buffer of 25 bytes.
   In down_arrow, we append one space.  */
প্রকারলেখ অক্ষর glyph_বাফার_t[26];

/* "→", separates the lhs of a rule from its rhs.  */
বাহ্যিক glyph_বাফার_t arrow;
বাহ্যিক পূর্ণসংখ্যা arrow_width;

/* "•", a point in an item (aka, a dotted rule).  */
বাহ্যিক glyph_বাফার_t dot;
বাহ্যিক পূর্ণসংখ্যা dot_width;

/* "↳ ", below an lhs to announce the rhs.  */
বাহ্যিক glyph_বাফার_t down_arrow;
বাহ্যিক পূর্ণসংখ্যা down_arrow_width;

/* "ε", an empty rhs.  */
বাহ্যিক glyph_বাফার_t empty;
বাহ্যিক পূর্ণসংখ্যা empty_width;

/* " ", separate symbols in the rhs of a derivation.  */
বাহ্যিক ধ্রুবক অক্ষর *derivation_separator;
বাহ্যিক পূর্ণসংখ্যা derivation_separator_width;

#endif /* GLYPHS_H */
