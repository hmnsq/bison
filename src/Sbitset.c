/* A simple, memory-efficient bitset implementation.

   Copyright (C) 2009-2015, 2018-2022 Free Software Foundation, Inc.

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

#ضمن "Sbitset.h"

Sbitset
Sbitset__new (Sbitset_شاخص nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  أرجع xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_شاخص nbits, بنية obstack *obstackp)
{
  Sbitset پایله;
  Sbitset ptr;
  Sbitset پای_ټکی;
  aver (nbits);
  پایله = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  لكل (ptr = پایله, پای_ټکی = پایله + Sbitset__nbytes (nbits); ptr < پای_ټکی; ++ptr)
    *ptr = 0;
  أرجع پایله;
}

فراغ
Sbitset__delete (Sbitset self)
{
  حرر (self);
}

bool
Sbitset_is_تشه (Sbitset self, Sbitset_شاخص nbits)
{
  Sbitset وروستی = self + Sbitset__nbytes (nbits) - 1;
  لكل (; self < وروستی; ++self)
    إذا (*self != 0)
      أرجع false;
  أرجع ((*وروستی) & Sbitset_وروستی_byte_ماسک (nbits)) == 0;
}

فراغ
Sbitset__fprint (Sbitset self, Sbitset_شاخص nbits, FILE *file)
{
  Sbitset_شاخص i;
  Sbitset itr;
  bool لومړی = true;
  fprintf (file,
           "nbits = %" SBITSET_شاخص_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      إذا (لومړی)
        لومړی = false;
      وإلا
        fprintf (file, ",");
      fprintf (file, " %" SBITSET_شاخص_CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
