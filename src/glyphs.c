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

#شامل <config.h>

#شامل "glyphs.h"

#شامل <assert.h>
#شامل <attribute.h>
#شامل <stdbool.h>
#شامل <string.h>
#شامل <mbswidth.h>
#شامل <unicodeio.h>


glyph_buffer_t arrow;
صحیح arrow_width;

glyph_buffer_t down_arrow;
صحیح down_arrow_width;

glyph_buffer_t dot;
صحیح dot_width;

glyph_buffer_t empty;
صحیح empty_width;

ثابت نویسه *derivation_separator = " ";
صحیح derivation_separator_width = 1;

تعریفنوع ساختار
{
  glyph_buffer_t *pbuf;
  ثابت نویسه *fallback;
} callback_arg_t;


ایستا long
on_success (ثابت نویسه *buf, size_t buflen, تهی *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (buflen + 1 < اندازه *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  بازگشت 1;
}

ایستا long
on_failure (unsigned code MAYBE_UNUSED, ثابت نویسه *پیام MAYBE_UNUSED,
            تهی *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (strlen (arg->fallback) + 1 < اندازه *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  بازگشت 0;
}

ایستا bool
glyph_set (glyph_buffer_t *glyph, صحیح *width,
           unsigned code, ثابت نویسه *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  صحیح res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  بازگشت res;
}

تهی
glyphs_init (تهی)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", اندازه down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
