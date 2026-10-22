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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "strversion.h"

#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <intprops.h>

പൂർണ്ണസംഖ്യ
strversion_to_int (അക്ഷരം സ്ഥിരം *version)
{
  IGNORE_TYPE_LIMITS_BEGIN
  പൂർണ്ണസംഖ്യ res = 0;
  errno = 0;
  അക്ഷരം *cp = ശൂന്യ;

  {
    long major = strtol (version, &cp, 10);
    എങ്കിൽ (errno || cp == version || *cp != '.' || major < 0
        || INT_MULTIPLY_WRAPV (major, 10000, &res))
      തിരികെ -1;
  }

  {
    ++cp;
    അക്ഷരം *prev = cp;
    long minor = strtol (cp, &cp, 10);
    എങ്കിൽ (errno || cp == prev || (*cp != '\0' && *cp != '.')
        || ! (0 <= minor && minor < 100)
        || INT_MULTIPLY_WRAPV (minor, 100, &minor)
        || INT_ADD_WRAPV (minor, res, &res))
      തിരികെ -1;
  }

  എങ്കിൽ (*cp == '.')
    {
      ++cp;
      അക്ഷരം *prev = cp;
      long micro = strtol (cp, &cp, 10);
      എങ്കിൽ (errno || cp == prev || (*cp != '\0' && *cp != '.')
          || ! (0 <= micro && micro < 100)
          || INT_ADD_WRAPV (micro, res, &res))
        തിരികെ -1;
    }

  IGNORE_TYPE_LIMITS_END
  തിരികെ res;
}
