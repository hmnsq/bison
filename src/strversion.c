/* Convert version string to int.

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
#अन्तर्भाव "system.h"

#अन्तर्भाव "strversion.h"

#अन्तर्भाव <errno.h>
#अन्तर्भाव <intprops.h>

पूर्णाङ्क
strversion_to_int (वर्ण ध्रुव *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  पूर्णाङ्क res = 0;
  त्रुटि_सं = 0;
  वर्ण *cp = रिक्त;

  {
    दीर्घ major = म_से_दीर्घ (version, &cp, 10);
    यदि (त्रुटि_सं || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      प्रत्यागम -1;
  }

  {
    ++cp;
    वर्ण *prev = cp;
    दीर्घ minor = म_से_दीर्घ (cp, &cp, 10);
    यदि (त्रुटि_सं || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      प्रत्यागम -1;
  }

  यदि (*cp == '.')
    {
      ++cp;
      वर्ण *prev = cp;
      दीर्घ micro = म_से_दीर्घ (cp, &cp, 10);
      यदि (त्रुटि_सं || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        प्रत्यागम -1;
    }

  IGNORE_TYPE_LIMITS_END
  प्रत्यागम res;
}
