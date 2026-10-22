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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "strversion.h"

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <intprops.h>

ಪೂರ್ಣಾಙ್ಕ
strversion_to_int (ವರ್ಣ ಧ್ರುವ *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  ಪೂರ್ಣಾಙ್ಕ res = 0;
  errno = 0;
  ವರ್ಣ *cp = ರಿಕ್ತ;

  {
    long major = strtol (version, &cp, 10);
    ಯದಿ (errno || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      ಪ್ರತ್ಯಾಗಮ -1;
  }

  {
    ++cp;
    ವರ್ಣ *prev = cp;
    long minor = strtol (cp, &cp, 10);
    ಯದಿ (errno || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      ಪ್ರತ್ಯಾಗಮ -1;
  }

  ಯದಿ (*cp == '.')
    {
      ++cp;
      ವರ್ಣ *prev = cp;
      long micro = strtol (cp, &cp, 10);
      ಯದಿ (errno || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        ಪ್ರತ್ಯಾಗಮ -1;
    }

  IGNORE_TYPE_LIMITS_END
  ಪ್ರತ್ಯಾಗಮ res;
}
