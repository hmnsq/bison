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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "strversion.h"

#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <intprops.h>

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
strversion_to_int (ଅକ୍ଷର ଅଚଳ *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
  errno = 0;
  ଅକ୍ଷର *cp = ଖାଲି;

  {
    long major = strtol (version, &cp, 10);
    ଯଦି (errno || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      ଫେରନ୍ତୁ -1;
  }

  {
    ++cp;
    ଅକ୍ଷର *prev = cp;
    long minor = strtol (cp, &cp, 10);
    ଯଦି (errno || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      ଫେରନ୍ତୁ -1;
  }

  ଯଦି (*cp == '.')
    {
      ++cp;
      ଅକ୍ଷର *prev = cp;
      long micro = strtol (cp, &cp, 10);
      ଯଦି (errno || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        ଫେରନ୍ତୁ -1;
    }

  IGNORE_TYPE_LIMITS_END
  ଫେରନ୍ତୁ res;
}
