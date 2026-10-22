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
صحيح arrow_ويڪر;

glyph_بفر_t هيٺ_ڏانهن_arrow;
صحيح هيٺ_ڏانهن_arrow_ويڪر;

glyph_بفر_t dot;
صحيح dot_ويڪر;

glyph_بفر_t خالي_جايون;
صحيح خالي_جايون_ويڪر;

ثابت حرف *derivation_separator = " ";
صحيح derivation_separator_ويڪر = 1;

تعريفنوع بنية
{
  glyph_بفر_t *pbuf;
  ثابت حرف *fallback;
} callback_arg_t;


ساكن ڊگهو
on_success (ثابت حرف *buf, size_t buflen, فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (buflen + 1 < حجم *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  أرجع 1;
}

ساكن ڊگهو
on_failure (بنانشان ڪوڊ MAYBE_اڻ_استعمال_ٿيل, ثابت حرف *نياپو MAYBE_اڻ_استعمال_ٿيل,
            فراغ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  أكد (لڙي_ڊيگهه (arg->fallback) + 1 < حجم *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  أرجع 0;
}

ساكن bool
glyph_سيٽ (glyph_بفر_t *glyph, صحيح *ويڪر,
           بنانشان ڪوڊ, ثابت حرف *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  صحيح res = unicode_منزل_mb (ڪوڊ, on_success, on_failure, &arg);
  *ويڪر = mbswidth (*glyph, 0);
  أرجع res;
}

فراغ
glyphs_init (فراغ)
{
  glyph_سيٽ (&arrow,      &arrow_ويڪر,      0x2192, "->");
  glyph_سيٽ (&dot,        &dot_ويڪر,        0x2022, ".");
  glyph_سيٽ (&هيٺ_ڏانهن_arrow, &هيٺ_ڏانهن_arrow_ويڪر, 0x21b3, "`->");
  glyph_سيٽ (&خالي_جايون,      &خالي_جايون_ويڪر,      0x03b5, "%empty");

  strncat (هيٺ_ڏانهن_arrow, " ", حجم هيٺ_ڏانهن_arrow - لڙي_ڊيگهه (هيٺ_ڏانهن_arrow) - 1);
  هيٺ_ڏانهن_arrow_ويڪر += 1;
}
