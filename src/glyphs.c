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

#समावेश <config.h>

#समावेश "glyphs.h"

#समावेश <assert.h>
#समावेश <attribute.h>
#समावेश <stdbool.h>
#समावेश <string.h>
#समावेश <mbswidth.h>
#समावेश <unicodeio.h>


glyph_बफर_t arrow;
पूर्णांक arrow_width;

glyph_बफर_t down_arrow;
पूर्णांक down_arrow_width;

glyph_बफर_t dot;
पूर्णांक dot_width;

glyph_बफर_t empty;
पूर्णांक empty_width;

स्थिर अक्षर *derivation_separator = " ";
पूर्णांक derivation_separator_width = 1;

प्रकार काष्ठा
{
  glyph_बफर_t *pbuf;
  स्थिर अक्षर *fallback;
} callback_arg_t;


अटल दीर्घ
on_success (स्थिर अक्षर *buf, माप_प्रकार buflen, व्योम *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  निश्चित (buflen + 1 < माप *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  वापस 1;
}

अटल दीर्घ
on_failure (अचिन्हित code MAYBE_UNUSED, स्थिर अक्षर *संदेश MAYBE_UNUSED,
            व्योम *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  निश्चित (म_माप (arg->fallback) + 1 < माप *arg->pbuf);
  म_नकल (*arg->pbuf, arg->fallback);
  वापस 0;
}

अटल bool
glyph_set (glyph_बफर_t *glyph, पूर्णांक *width,
           अचिन्हित code, स्थिर अक्षर *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  पूर्णांक res = unicode_to_mb (code, on_success, on_failure, &arg);
  *width = mbswidth (*glyph, 0);
  वापस res;
}

व्योम
glyphs_init (व्योम)
{
  glyph_set (&arrow,      &arrow_width,      0x2192, "->");
  glyph_set (&dot,        &dot_width,        0x2022, ".");
  glyph_set (&down_arrow, &down_arrow_width, 0x21b3, "`->");
  glyph_set (&empty,      &empty_width,      0x03b5, "%empty");

  म_जोड़न (down_arrow, " ", माप down_arrow - म_माप (down_arrow) - 1);
  down_arrow_width += 1;
}
