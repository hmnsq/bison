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


glyph_buffer_t arrow;
ಪೂರ್ಣಾಙ್ಕ arrow_width;

glyph_buffer_t down_arrow;
ಪೂರ್ಣಾಙ್ಕ down_arrow_width;

glyph_buffer_t dot;
ಪೂರ್ಣಾಙ್ಕ dot_width;

glyph_buffer_t empty;
ಪೂರ್ಣಾಙ್ಕ empty_width;

ಧ್ರುವ ವರ್ಣ *derivation_separator = " ";
ಪೂರ್ಣಾಙ್ಕ derivation_separator_width = 1;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  glyph_buffer_t *pbuf;
  ಧ್ರುವ ವರ್ಣ *fallback;
} callback_arg_t;


ಸ್ಥಿರ long
on_success (ಧ್ರುವ ವರ್ಣ *buf, size_t buflen, ಶೂನ್ಯ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ಪ್ರತಿಜ್ಞಾ (buflen + 1 < ಪರಿಮಾಣ *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ long
on_failure (unsigned code MAYBE_UNUSED, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ MAYBE_UNUSED,
            ಶೂನ್ಯ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ಪ್ರತಿಜ್ಞಾ (strlen (arg->fallback) + 1 < ಪರಿಮಾಣ *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
glyph_set (glyph_buffer_t *glyph, ಪೂರ್ಣಾಙ್ಕ *width,
           unsigned code, ಧ್ರುವ ವರ್ಣ *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  ಪೂರ್ಣಾಙ್ಕ res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ
glyphs_init (ಶೂನ್ಯ)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", ಪರಿಮಾಣ down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
