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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "glyphs.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <attribute.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <mbswidth.h>
#അന്തര്ഭാവ <unicodeio.h>


glyph_buffer_t arrow;
പൂർണ്ണസംഖ്യ arrow_width;

glyph_buffer_t down_arrow;
പൂർണ്ണസംഖ്യ down_arrow_width;

glyph_buffer_t dot;
പൂർണ്ണസംഖ്യ dot_width;

glyph_buffer_t empty;
പൂർണ്ണസംഖ്യ empty_width;

സ്ഥിരം അക്ഷരം *derivation_separator = " ";
പൂർണ്ണസംഖ്യ derivation_separator_width = 1;

തരനിർവചനം ഘടന
{
  glyph_buffer_t *pbuf;
  സ്ഥിരം അക്ഷരം *fallback;
} callback_arg_t;


സ്ഥിതം long
on_success (സ്ഥിരം അക്ഷരം *buf, size_t buflen, ശൂന്യം *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  പ്രതിജ്ഞാ (buflen + 1 < വലുപ്പം *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  തിരികെ 1;
}

സ്ഥിതം long
on_failure (unsigned code MAYBE_UNUSED, സ്ഥിരം അക്ഷരം *സന്ദേശം MAYBE_UNUSED,
            ശൂന്യം *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  പ്രതിജ്ഞാ (strlen (arg->fallback) + 1 < വലുപ്പം *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  തിരികെ 0;
}

സ്ഥിതം bool
glyph_set (glyph_buffer_t *glyph, പൂർണ്ണസംഖ്യ *width,
           unsigned code, സ്ഥിരം അക്ഷരം *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  പൂർണ്ണസംഖ്യ res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  തിരികെ res;
}

ശൂന്യം
glyphs_init (ശൂന്യം)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", വലുപ്പം down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
