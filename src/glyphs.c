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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "glyphs.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <attribute.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <mbswidth.h>
#அந்தர்பாவ <unicodeio.h>


glyph_buffer_t arrow;
முழு arrow_width;

glyph_buffer_t down_arrow;
முழு down_arrow_width;

glyph_buffer_t dot;
முழு dot_width;

glyph_buffer_t empty;
முழு empty_width;

மாறிலி எழுத்து *derivation_separator = " ";
முழு derivation_separator_width = 1;

வகைவரை கட்டமைப்பு
{
  glyph_buffer_t *pbuf;
  மாறிலி எழுத்து *fallback;
} callback_arg_t;


நிலையான long
on_success (மாறிலி எழுத்து *buf, size_t buflen, வெறுமை *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ப்ரதிஜ்ஞா (buflen + 1 < அளவு *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  திரும்பு 1;
}

நிலையான long
on_failure (unsigned code MAYBE_UNUSED, மாறிலி எழுத்து *செய்தி MAYBE_UNUSED,
            வெறுமை *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ப்ரதிஜ்ஞா (strlen (arg->fallback) + 1 < அளவு *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  திரும்பு 0;
}

நிலையான bool
glyph_set (glyph_buffer_t *glyph, முழு *width,
           unsigned code, மாறிலி எழுத்து *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  முழு res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  திரும்பு res;
}

வெறுமை
glyphs_init (வெறுமை)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", அளவு down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
