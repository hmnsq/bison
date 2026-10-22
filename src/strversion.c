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

ಪೂರ್ಣಾಂಕ
strversion_ಗಮ್ಯ_int (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆವೃತ್ತಿ)
{
  IGNORE_TYPE_LIMITS_BEGIN
  ಪೂರ್ಣಾಂಕ res = 0;
  errno = 0;
  ಅಕ್ಷರ *cp = ಖಾಲಿ;

  {
    ದೀರ್ಘ major = strtol (ಆವೃತ್ತಿ, &cp, 10);
    ಆದರೆ (errno || cp == ಆವೃತ್ತಿ || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      ಹಿಂತಿರುಗಿ -1;
  }

  {
    ++cp;
    ಅಕ್ಷರ *prev = cp;
    ದೀರ್ಘ minor = strtol (cp, &cp, 10);
    ಆದರೆ (errno || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ಕೂಡಿಸು_WRAPV (minor, res, &res))
      ಹಿಂತಿರುಗಿ -1;
  }

  ಆದರೆ (*cp == '.')
    {
      ++cp;
      ಅಕ್ಷರ *prev = cp;
      ದೀರ್ಘ micro = strtol (cp, &cp, 10);
      ಆದರೆ (errno || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ಕೂಡಿಸು_WRAPV (micro, res, &res))
        ಹಿಂತಿರುಗಿ -1;
    }

  IGNORE_TYPE_LIMITS_ಕೊನೆ
  ಹಿಂತಿರುಗಿ res;
}
