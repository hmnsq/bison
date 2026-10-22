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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "assoc.h"


ধ্রুবক অক্ষর *
assoc_to_string (assoc a)
{
  নির্বাচন (a)
    {
    ক্ষেত্র undef_assoc:
      ফেরত "undefined associativity";

    ক্ষেত্র ডান_assoc:
      ফেরত "%right";

    ক্ষেত্র বাম_assoc:
      ফেরত "%left";

    ক্ষেত্র non_assoc:
      ফেরত "%nonassoc";

    ক্ষেত্র precedence_assoc:
      ফেরত "%precedence";
    }
  abort ();
}
