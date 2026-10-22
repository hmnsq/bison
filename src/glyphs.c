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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "glyphs.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <attribute.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <mbswidth.h>
#અન્તર્ભાવ <unicodeio.h>


glyph_buffer_t arrow;
પૂર્ણાંક arrow_width;

glyph_buffer_t down_arrow;
પૂર્ણાંક down_arrow_width;

glyph_buffer_t dot;
પૂર્ણાંક dot_width;

glyph_buffer_t empty;
પૂર્ણાંક empty_width;

અચલ અક્ષર *derivation_separator = " ";
પૂર્ણાંક derivation_separator_width = 1;

પ્રકારવ્યાખ્યા સંરચના
{
  glyph_buffer_t *pbuf;
  અચલ અક્ષર *fallback;
} callback_arg_t;


સ્થિર long
on_success (અચલ અક્ષર *buf, size_t buflen, શૂન્ય *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  પ્રતિજ્ઞા (buflen + 1 < કદ *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  પાછા 1;
}

સ્થિર long
on_failure (unsigned code MAYBE_UNUSED, અચલ અક્ષર *સંદેશ MAYBE_UNUSED,
            શૂન્ય *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  પ્રતિજ્ઞા (strlen (arg->fallback) + 1 < કદ *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  પાછા 0;
}

સ્થિર bool
glyph_set (glyph_buffer_t *glyph, પૂર્ણાંક *width,
           unsigned code, અચલ અક્ષર *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  પૂર્ણાંક res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  પાછા res;
}

શૂન્ય
glyphs_init (શૂન્ય)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", કદ down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
