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


glyph_بفر_t arrow;
صحيح arrow_پلنوالی;

glyph_بفر_t ښکته_arrow;
صحيح ښکته_arrow_پلنوالی;

glyph_بفر_t dot;
صحيح dot_پلنوالی;

glyph_بفر_t تشه;
صحيح تشه_پلنوالی;

ثابت حرف *derivation_separator = " ";
صحيح derivation_separator_پلنوالی = 1;

تعريفنوع بنية
{
  glyph_بفر_t *pbuf;
  ثابت حرف *fallback;
} callback_arg_t;


ساكن اوږد
on_success (ثابت حرف *buf, size_t buflen, فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (buflen + 1 < حجم *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  أرجع 1;
}

ساكن اوږد
on_failure (بینښه کوډ MAYBE_ناکارول_شوی, ثابت حرف *پیغام_2 MAYBE_ناکارول_شوی,
            فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (مزي_اوږدوالی (arg->fallback) + 1 < حجم *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  أرجع 0;
}

ساكن bool
glyph_سېټ (glyph_بفر_t *glyph, صحيح *پلنوالی,
           بینښه کوډ, ثابت حرف *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  صحيح res = unicode_منزل_mb (کوډ, on_success, on_failure, &arg);
  *پلنوالی = mbswidth (*glyph, 0);
  أرجع res;
}

فراغ
glyphs_init (فراغ)
{
  glyph_سېټ (&arrow,      &arrow_پلنوالی,      0x2192, "->");
  glyph_سېټ (&dot,        &dot_پلنوالی,        0x2022, ".");
  glyph_سېټ (&ښکته_arrow, &ښکته_arrow_پلنوالی, 0x21b3, "`->");
  glyph_سېټ (&تشه,      &تشه_پلنوالی,      0x03b5, "%empty");

  strncat (ښکته_arrow, " ", حجم ښکته_arrow - مزي_اوږدوالی (ښکته_arrow) - 1);
  ښکته_arrow_پلنوالی += 1;
}
