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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "strversion.h"

#समावेश <errno.h>
#समावेश <intprops.h>

पूर्णांक
strversion_to_int (अक्षर स्थिर *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  पूर्णांक res = 0;
  त्रुटि_सं = 0;
  अक्षर *cp = शून्य;

  {
    दीर्घ major = म_से_दीर्घ (version, &cp, 10);
    अगर (त्रुटि_सं || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      वापस -1;
  }

  {
    ++cp;
    अक्षर *पूर्व = cp;
    दीर्घ minor = म_से_दीर्घ (cp, &cp, 10);
    अगर (त्रुटि_सं || cp == पूर्व || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      वापस -1;
  }

  अगर (*cp == '.')
    {
      ++cp;
      अक्षर *पूर्व = cp;
      दीर्घ micro = म_से_दीर्घ (cp, &cp, 10);
      अगर (त्रुटि_सं || cp == पूर्व || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        वापस -1;
    }

  IGNORE_TYPE_LIMITS_END
  वापस res;
}
