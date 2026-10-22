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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "Sbitset.h"

Sbitset
Sbitset__new (Sbitset_সূচক nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  ফেরত xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_সূচক nbits, কাঠামো obstack *obstackp)
{
  Sbitset ফলাফল;
  Sbitset ptr;
  Sbitset end;
  aver (nbits);
  ফলাফল = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  জন্য (ptr = ফলাফল, end = ফলাফল + Sbitset__nbytes (nbits); ptr < end; ++ptr)
    *ptr = 0;
  ফেরত ফলাফল;
}

শূন্য
Sbitset__delete (Sbitset self)
{
  মুক্ত (self);
}

bool
Sbitset__isEmpty (Sbitset self, Sbitset_সূচক nbits)
{
  Sbitset last = self + Sbitset__nbytes (nbits) - 1;
  জন্য (; self < last; ++self)
    যদি (*self != 0)
      ফেরত false;
  ফেরত ((*last) & Sbitset__last_byte_mask (nbits)) == 0;
}

শূন্য
Sbitset__fprint (Sbitset self, Sbitset_সূচক nbits, FILE *file)
{
  Sbitset_সূচক i;
  Sbitset itr;
  bool first = true;
  fprintf (file,
           "nbits = %" SBITSET_সূচক_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      যদি (first)
        first = false;
      অন্যথা
        fprintf (file, ",");
      fprintf (file, " %" SBITSET_সূচক_CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
