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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "glyphs.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <attribute.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <unicodeio.h>


glyph_ಬಫರ್_t arrow;
ಪೂರ್ಣಾಂಕ arrow_ಅಗಲ;

glyph_ಬಫರ್_t ಕೆಳಕ್ಕೆ_arrow;
ಪೂರ್ಣಾಂಕ ಕೆಳಕ್ಕೆ_arrow_ಅಗಲ;

glyph_ಬಫರ್_t dot;
ಪೂರ್ಣಾಂಕ dot_ಅಗಲ;

glyph_ಬಫರ್_t ಖಾಲಿಗಳು;
ಪೂರ್ಣಾಂಕ ಖಾಲಿಗಳು_ಅಗಲ;

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *derivation_separator = " ";
ಪೂರ್ಣಾಂಕ derivation_separator_ಅಗಲ = 1;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  glyph_ಬಫರ್_t *pbuf;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fallback;
} callback_arg_t;


ಸ್ಥಿರ ದೀರ್ಘ
on_success (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf, size_t buflen, ಶೂನ್ಯ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ಪ್ರತಿಜ್ಞಾ (buflen + 1 < ಗಾತ್ರ *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ದೀರ್ಘ
on_failure (ಚಿಹ್ನೆರಹಿತ ಸಂಕೇತ_2 MAYBE_ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_2 MAYBE_ಬಳಸದ,
            ಶೂನ್ಯ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ಪ್ರತಿಜ್ಞಾ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (arg->fallback) + 1 < ಗಾತ್ರ *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
glyph_ಗಣ (glyph_ಬಫರ್_t *glyph, ಪೂರ್ಣಾಂಕ *ಅಗಲ,
           ಚಿಹ್ನೆರಹಿತ ಸಂಕೇತ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  ಪೂರ್ಣಾಂಕ res = unicode_ಗಮ್ಯ_mb (ಸಂಕೇತ_2, on_success, on_failure, &arg);
  *ಅಗಲ = mbswidth (*glyph, 0);
  ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ
glyphs_init (ಶೂನ್ಯ)
{
  glyph_ಗಣ (&arrow,      &arrow_ಅಗಲ,      0x2192, "->");
  glyph_ಗಣ (&dot,        &dot_ಅಗಲ,        0x2022, ".");
  glyph_ಗಣ (&ಕೆಳಕ್ಕೆ_arrow, &ಕೆಳಕ್ಕೆ_arrow_ಅಗಲ, 0x21b3, "`->");
  glyph_ಗಣ (&ಖಾಲಿಗಳು,      &ಖಾಲಿಗಳು_ಅಗಲ,      0x03b5, "%empty");

  strncat (ಕೆಳಕ್ಕೆ_arrow, " ", ಗಾತ್ರ ಕೆಳಕ್ಕೆ_arrow - ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಕೆಳಕ್ಕೆ_arrow) - 1);
  ಕೆಳಕ್ಕೆ_arrow_ಅಗಲ += 1;
}
