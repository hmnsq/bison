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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "strversion.h"

#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <intprops.h>

পূর্ণাঙ্ক
strversion_to_int (বর্ণ ধ্রুব *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  পূর্ণাঙ্ক res = 0;
  errno = 0;
  বর্ণ *cp = রিক্ত;

  {
    long major = strtol (version, &cp, 10);
    যদি (errno || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      প্রত্যাগম -1;
  }

  {
    ++cp;
    বর্ণ *prev = cp;
    long minor = strtol (cp, &cp, 10);
    যদি (errno || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      প্রত্যাগম -1;
  }

  যদি (*cp == '.')
    {
      ++cp;
      বর্ণ *prev = cp;
      long micro = strtol (cp, &cp, 10);
      যদি (errno || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        প্রত্যাগম -1;
    }

  IGNORE_TYPE_LIMITS_END
  প্রত্যাগম res;
}
