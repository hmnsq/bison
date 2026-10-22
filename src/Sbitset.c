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
Sbitset__new (Sbitset_اشاريو nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  أرجع xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_اشاريو nbits, بنية obstack *obstackp)
{
  Sbitset نتيجو;
  Sbitset ptr;
  Sbitset پڇاڙي;
  aver (nbits);
  نتيجو = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  لكل (ptr = نتيجو, پڇاڙي = نتيجو + Sbitset__nbytes (nbits); ptr < پڇاڙي; ++ptr)
    *ptr = 0;
  أرجع نتيجو;
}

فراغ
Sbitset__delete (Sbitset self)
{
  حرر (self);
}

bool
Sbitset_is_خالي_جايون (Sbitset self, Sbitset_اشاريو nbits)
{
  Sbitset آخري = self + Sbitset__nbytes (nbits) - 1;
  لكل (; self < آخري; ++self)
    إذا (*self != 0)
      أرجع false;
  أرجع ((*آخري) & Sbitset_آخري_byte_نقاب (nbits)) == 0;
}

فراغ
Sbitset__fprint (Sbitset self, Sbitset_اشاريو nbits, FILE *file)
{
  Sbitset_اشاريو i;
  Sbitset itr;
  bool پهريون = true;
  fprintf (file,
           "nbits = %" SBITSET_اشاريو_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      إذا (پهريون)
        پهريون = false;
      وإلا
        fprintf (file, ",");
      fprintf (file, " %" SBITSET_اشاريو_CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
