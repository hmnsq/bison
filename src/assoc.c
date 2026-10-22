/* Associativity information.

   Copyright (C) 2002, 2005-2006, 2008-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#अन्तर्भाव "assoc.h"


ध्रुव वर्ण *
assoc_to_string (assoc a)
{
  विकल्प (a)
    {
    पक्ष undef_assoc:
      प्रत्यागम "undefined associativity";

    पक्ष दक्षिणः_assoc:
      प्रत्यागम "%right";

    पक्ष वामः_assoc:
      प्रत्यागम "%left";

    पक्ष non_assoc:
      प्रत्यागम "%nonassoc";

    पक्ष precedence_assoc:
      प्रत्यागम "%precedence";
    }
  abort ();
}
