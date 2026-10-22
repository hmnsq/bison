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


glyph_बफर_t arrow;
पूर्णाङ्क arrow_विस्तारः;

glyph_बफर_t अधस्तात्_arrow;
पूर्णाङ्क अधस्तात्_arrow_विस्तारः;

glyph_बफर_t dot;
पूर्णाङ्क dot_विस्तारः;

glyph_बफर_t रिक्तानि;
पूर्णाङ्क रिक्तानि_विस्तारः;

ध्रुव वर्ण *derivation_separator = " ";
पूर्णाङ्क derivation_separator_विस्तारः = 1;

प्रकारनिर्देश संरचना
{
  glyph_बफर_t *pbuf;
  ध्रुव वर्ण *fallback;
} callback_arg_t;


स्थिर दीर्घ
on_success (ध्रुव वर्ण *buf, size_t buflen, शून्य *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  प्रतिज्ञा (buflen + 1 < परिमाण *arg->pbuf);
  *stpncpy (*arg->pbuf, buf, buflen) = '\0';
  प्रत्यागम 1;
}

स्थिर दीर्घ
on_failure (अचिह्नित संहिता MAYBE_अप्रयुक्तम्, ध्रुव वर्ण *सन्देशः_2 MAYBE_अप्रयुक्तम्,
            शून्य *callback_arg)
{
  callback_arg_t *arg = (callback_arg_t *) callback_arg;
  प्रतिज्ञा (वाक्यदैर्घ्यम् (arg->fallback) + 1 < परिमाण *arg->pbuf);
  वाक्यं_प्रतिलिख (*arg->pbuf, arg->fallback);
  प्रत्यागम 0;
}

स्थिर bool
glyph_समुच्चयः (glyph_बफर_t *glyph, पूर्णाङ्क *विस्तारः,
           अचिह्नित संहिता, ध्रुव वर्ण *fallback)
{
  callback_arg_t arg = { glyph, fallback };
  पूर्णाङ्क res = unicode_गन्तव्यम्_mb (संहिता, on_success, on_failure, &arg);
  *विस्तारः = mbswidth (*glyph, 0);
  प्रत्यागम res;
}

शून्य
glyphs_init (शून्य)
{
  glyph_समुच्चयः (&arrow,      &arrow_विस्तारः,      0x2192, "->");
  glyph_समुच्चयः (&dot,        &dot_विस्तारः,        0x2022, ".");
  glyph_समुच्चयः (&अधस्तात्_arrow, &अधस्तात्_arrow_विस्तारः, 0x21b3, "`->");
  glyph_समुच्चयः (&रिक्तानि,      &रिक्तानि_विस्तारः,      0x03b5, "%empty");

  strncat (अधस्तात्_arrow, " ", परिमाण अधस्तात्_arrow - वाक्यदैर्घ्यम् (अधस्तात्_arrow) - 1);
  अधस्तात्_arrow_विस्तारः += 1;
}
