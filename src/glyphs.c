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
صحيح arrow_چوڑائی;

glyph_بفر_t تھلے_arrow;
صحيح تھلے_arrow_چوڑائی;

glyph_بفر_t dot;
صحيح dot_چوڑائی;

glyph_بفر_t خالی_تھاواں;
صحيح خالی_تھاواں_چوڑائی;

ثابت حرف *derivation_separator = " ";
صحيح derivation_separator_چوڑائی = 1;

تعريفنوع بنية
{
  glyph_بفر_t *pbuf;
  ثابت حرف *fallback;
} callback_arg_t;


ساكن لما
on_success (ثابت حرف *buf, size_t buflen, فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (buflen + 1 < حجم *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  أرجع 1;
}

ساكن لما
on_failure (بنانشانی کوڈ MAYBE_بن_ورتیا, ثابت حرف *پیغام MAYBE_بن_ورتیا,
            فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (لڑی_لمبائی (arg->fallback) + 1 < حجم *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  أرجع 0;
}

ساكن bool
glyph_سیٹ (glyph_بفر_t *glyph, صحيح *چوڑائی,
           بنانشانی کوڈ, ثابت حرف *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  صحيح res = unicode_منزل_mb (کوڈ, on_success, on_failure, &arg);
  *چوڑائی = mbswidth (*glyph, 0);
  أرجع res;
}

فراغ
glyphs_init (فراغ)
{
  glyph_سیٹ (&arrow,      &arrow_چوڑائی,      0x2192, "->");
  glyph_سیٹ (&dot,        &dot_چوڑائی,        0x2022, ".");
  glyph_سیٹ (&تھلے_arrow, &تھلے_arrow_چوڑائی, 0x21b3, "`->");
  glyph_سیٹ (&خالی_تھاواں,      &خالی_تھاواں_چوڑائی,      0x03b5, "%empty");

  strncat (تھلے_arrow, " ", حجم تھلے_arrow - لڑی_لمبائی (تھلے_arrow) - 1);
  تھلے_arrow_چوڑائی += 1;
}
