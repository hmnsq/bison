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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "glyphs.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <attribute.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <mbswidth.h>
#ଅନ୍ତର୍ଭାଵ <unicodeio.h>


glyph_buffer_t arrow;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା arrow_width;

glyph_buffer_t down_arrow;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା down_arrow_width;

glyph_buffer_t dot;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା dot_width;

glyph_buffer_t empty;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା empty_width;

ଅଚଳ ଅକ୍ଷର *derivation_separator = " ";
ପୂର୍ଣ୍ଣସଂଖ୍ୟା derivation_separator_width = 1;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  glyph_buffer_t *pbuf;
  ଅଚଳ ଅକ୍ଷର *fallback;
} callback_arg_t;


ସ୍ଥିର long
on_success (ଅଚଳ ଅକ୍ଷର *buf, size_t buflen, ଶୂନ୍ୟ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ପ୍ରତିଜ୍ଞା (buflen + 1 < ଆକାର *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର long
on_failure (unsigned code MAYBE_UNUSED, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ MAYBE_UNUSED,
            ଶୂନ୍ୟ *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  ପ୍ରତିଜ୍ଞା (strlen (arg->fallback) + 1 < ଆକାର *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
glyph_set (glyph_buffer_t *glyph, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *width,
           unsigned code, ଅଚଳ ଅକ୍ଷର *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ
glyphs_init (ଶୂନ୍ୟ)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", ଆକାର down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
