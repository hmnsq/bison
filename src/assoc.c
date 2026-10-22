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

#ضمن <config.h>
#ضمن "system.h"

#ضمن "assoc.h"


ثابت حرف *
assoc_to_string (assoc a)
{
  بدل (a)
    {
    حالة undef_assoc:
      إرجاع "undefined associativity";

    حالة يمين_assoc:
      إرجاع "%right";

    حالة يسار_assoc:
      إرجاع "%left";

    حالة non_assoc:
      إرجاع "%nonassoc";

    حالة precedence_assoc:
      إرجاع "%precedence";
    }
  abort ();
}
