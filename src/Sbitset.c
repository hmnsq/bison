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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "Sbitset.h"

Sbitset
Sbitset__new (Sbitset__Index nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  திரும்பு xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset__Index nbits, கட்டமைப்பு obstack *obstackp)
{
  Sbitset result;
  Sbitset ptr;
  Sbitset end;
  aver (nbits);
  result = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  க்கு (ptr = result, end = result + Sbitset__nbytes (nbits); ptr < end; ++ptr)
    *ptr = 0;
  திரும்பு result;
}

வெறுமை
Sbitset__delete (Sbitset self)
{
  விடுவி (self);
}

bool
Sbitset__isEmpty (Sbitset self, Sbitset__Index nbits)
{
  Sbitset last = self + Sbitset__nbytes (nbits) - 1;
  க்கு (; self < last; ++self)
    எனில் (*self != 0)
      திரும்பு false;
  திரும்பு ((*last) & Sbitset__last_byte_mask (nbits)) == 0;
}

வெறுமை
Sbitset__fprint (Sbitset self, Sbitset__Index nbits, FILE *file)
{
  Sbitset__Index i;
  Sbitset itr;
  bool first = true;
  fprintf (file,
           "nbits = %" SBITSET__INDEX__CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      எனில் (first)
        first = false;
      இல்லையெனில்
        fprintf (file, ",");
      fprintf (file, " %" SBITSET__INDEX__CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
