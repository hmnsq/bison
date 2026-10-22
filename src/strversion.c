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

পূর্ণসংখ্যা
strversion_to_int (অক্ষর ধ্রুবক *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  পূর্ণসংখ্যা res = 0;
  errno = 0;
  অক্ষর *cp = ফাঁকা;

  {
    long major = strtol (version, &cp, 10);
    যদি (errno || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      ফেরত -1;
  }

  {
    ++cp;
    অক্ষর *পূর্ব = cp;
    long minor = strtol (cp, &cp, 10);
    যদি (errno || cp == পূর্ব || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      ফেরত -1;
  }

  যদি (*cp == '.')
    {
      ++cp;
      অক্ষর *পূর্ব = cp;
      long micro = strtol (cp, &cp, 10);
      যদি (errno || cp == পূর্ব || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        ফেরত -1;
    }

  IGNORE_TYPE_LIMITS_END
  ফেরত res;
}
