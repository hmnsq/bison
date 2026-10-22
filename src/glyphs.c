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

#ضمن <config.h>

#ضمن "glyphs.h"

#ضمن <assert.h>
#ضمن <attribute.h>
#ضمن <stdbool.h>
#ضمن <string.h>
#ضمن <mbswidth.h>
#ضمن <unicodeio.h>


glyph_buffer_t arrow;
صحيح arrow_width;

glyph_buffer_t down_arrow;
صحيح down_arrow_width;

glyph_buffer_t dot;
صحيح dot_width;

glyph_buffer_t empty;
صحيح empty_width;

ثابت حرف *derivation_separator = " ";
صحيح derivation_separator_width = 1;

تعريفنوع بنية
{
  glyph_buffer_t *pbuf;
  ثابت حرف *fallback;
} callback_arg_t;


ساكن long
on_success (ثابت حرف *buf, size_t buflen, فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (buflen + 1 < حجم *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  أرجع 1;
}

ساكن long
on_failure (unsigned code MAYBE_UNUSED, ثابت حرف *msg MAYBE_UNUSED,
            فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (strlen (arg->fallback) + 1 < حجم *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  أرجع 0;
}

ساكن bool
glyph_set (glyph_buffer_t *glyph, صحيح *width,
           unsigned code, ثابت حرف *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  صحيح res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  أرجع res;
}

فراغ
glyphs_init (فراغ)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", حجم down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
