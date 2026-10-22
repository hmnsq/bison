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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "glyphs.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <attribute.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <mbswidth.h>
#অন্তর্ভাব <unicodeio.h>


glyph_বাফার_t arrow;
পূর্ণসংখ্যা arrow_width;

glyph_বাফার_t down_arrow;
পূর্ণসংখ্যা down_arrow_width;

glyph_বাফার_t dot;
পূর্ণসংখ্যা dot_width;

glyph_বাফার_t empty;
পূর্ণসংখ্যা empty_width;

ধ্রুবক অক্ষর *derivation_separator = " ";
পূর্ণসংখ্যা derivation_separator_width = 1;

প্রকারলেখ কাঠামো
{
  glyph_বাফার_t *pbuf;
  ধ্রুবক অক্ষর *fallback;
} callback_arg_t;


স্থির long
on_success (ধ্রুবক অক্ষর *buf, size_t buflen, শূন্য *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  প্রতিজ্ঞা (buflen + 1 < আকার *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  ফেরত 1;
}

স্থির long
on_failure (unsigned code MAYBE_UNUSED, ধ্রুবক অক্ষর *বার্তা MAYBE_UNUSED,
            শূন্য *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  প্রতিজ্ঞা (strlen (arg->fallback) + 1 < আকার *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  ফেরত 0;
}

স্থির bool
glyph_set (glyph_বাফার_t *glyph, পূর্ণসংখ্যা *width,
           unsigned code, ধ্রুবক অক্ষর *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  পূর্ণসংখ্যা res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  ফেরত res;
}

শূন্য
glyphs_init (শূন্য)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", আকার down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
