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

#अन्तर्भाव <config.h>

#अन्तर्भाव "glyphs.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <attribute.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <mbswidth.h>
#अन्तर्भाव <unicodeio.h>


glyph_buffer_t arrow;
पूर्णाङ्क arrow_width;

glyph_buffer_t down_arrow;
पूर्णाङ्क down_arrow_width;

glyph_buffer_t dot;
पूर्णाङ्क dot_width;

glyph_buffer_t empty;
पूर्णाङ्क empty_width;

ध्रुव वर्ण *derivation_separator = " ";
पूर्णाङ्क derivation_separator_width = 1;

प्रकारनिर्देश संरचना
{
  glyph_buffer_t *pbuf;
  ध्रुव वर्ण *fallback;
} callback_arg_t;


स्थिर long
on_success (ध्रुव वर्ण *buf, size_t buflen, शून्य *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  प्रतिज्ञा (buflen + 1 < परिमाण *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  प्रत्यागम 1;
}

स्थिर long
on_failure (unsigned code MAYBE_UNUSED, ध्रुव वर्ण *सन्देशः MAYBE_UNUSED,
            शून्य *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  प्रतिज्ञा (strlen (arg->fallback) + 1 < परिमाण *arg->pbuf);
  strcpy (*arg->pbuf, arg->fallback);
  प्रत्यागम 0;
}

स्थिर bool
glyph_set (glyph_buffer_t *glyph, पूर्णाङ्क *width,
           unsigned code, ध्रुव वर्ण *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  पूर्णाङ्क res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  प्रत्यागम res;
}

शून्य
glyphs_init (शून्य)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  strncat (down_arrow, " ", परिमाण down_arrow - strlen (down_arrow) - 1);
  down_arrow_width += 1;
}
