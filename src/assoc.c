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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "assoc.h"


സ്ഥിരം അക്ഷരം *
assoc_to_string (assoc a)
{
  തിരഞ്ഞെടുപ്പ് (a)
    {
    അവസ്ഥ undef_assoc:
      തിരികെ "undefined associativity";

    അവസ്ഥ വലത്_assoc:
      തിരികെ "%right";

    അവസ്ഥ ഇടത്_assoc:
      തിരികെ "%left";

    അവസ്ഥ non_assoc:
      തിരികെ "%nonassoc";

    അവസ്ഥ precedence_assoc:
      തിരികെ "%precedence";
    }
  abort ();
}
