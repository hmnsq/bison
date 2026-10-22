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

#ضمن <config.h>
#ضمن "system.h"

#ضمن "strversion.h"

#ضمن <errno.h>
#ضمن <intprops.h>

صحيح
strversion_منزل_int (حرف ثابت *نسخه)
{
  IGNORE_TYPE_LIMITS_BEGIN
  صحيح res = 0;
  errno = 0;
  حرف *cp = عدم;

  {
    اوږد major = strtol (نسخه, &cp, 10);
    إذا (errno || cp == نسخه || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      أرجع -1;
  }

  {
    ++cp;
    حرف *prev = cp;
    اوږد minor = strtol (cp, &cp, 10);
    إذا (errno || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_جمع_کړه_WRAPV (minor, res, &res))
      أرجع -1;
  }

  إذا (*cp == '.')
    {
      ++cp;
      حرف *prev = cp;
      اوږد micro = strtol (cp, &cp, 10);
      إذا (errno || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_جمع_کړه_WRAPV (micro, res, &res))
        أرجع -1;
    }

  IGNORE_TYPE_LIMITS_پای_ټکی
  أرجع res;
}
