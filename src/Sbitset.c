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
Sbitset__new (Sbitset_اشاریہ nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  أرجع xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_اشاریہ nbits, بنية obstack *obstackp)
{
  Sbitset نتیجہ;
  Sbitset ptr;
  Sbitset سرا;
  aver (nbits);
  نتیجہ = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  لكل (ptr = نتیجہ, سرا = نتیجہ + Sbitset__nbytes (nbits); ptr < سرا; ++ptr)
    *ptr = 0;
  أرجع نتیجہ;
}

فراغ
Sbitset__delete (Sbitset self)
{
  حرر (self);
}

bool
Sbitset_is_خالی_تھاواں (Sbitset self, Sbitset_اشاریہ nbits)
{
  Sbitset آخری = self + Sbitset__nbytes (nbits) - 1;
  لكل (; self < آخری; ++self)
    إذا (*self != 0)
      أرجع false;
  أرجع ((*آخری) & Sbitset_آخری_byte_نقاب (nbits)) == 0;
}

فراغ
Sbitset__fprint (Sbitset self, Sbitset_اشاریہ nbits, FILE *file)
{
  Sbitset_اشاریہ i;
  Sbitset itr;
  bool پہلا = true;
  fprintf (file,
           "nbits = %" SBITSET_اشاریہ_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      إذا (پہلا)
        پہلا = false;
      وإلا
        fprintf (file, ",");
      fprintf (file, " %" SBITSET_اشاریہ_CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
