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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "Sbitset.h"

Sbitset
Sbitset__new (Sbitset_सूचकांक nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  वापस xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_सूचकांक nbits, काष्ठा obstack *obstackp)
{
  Sbitset परिणाम;
  Sbitset ptr;
  Sbitset end;
  aver (nbits);
  परिणाम = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  क्रम (ptr = परिणाम, end = परिणाम + Sbitset__nbytes (nbits); ptr < end; ++ptr)
    *ptr = 0;
  वापस परिणाम;
}

व्योम
Sbitset__delete (Sbitset self)
{
  मुक्त (self);
}

bool
Sbitset__isEmpty (Sbitset self, Sbitset_सूचकांक nbits)
{
  Sbitset last = self + Sbitset__nbytes (nbits) - 1;
  क्रम (; self < last; ++self)
    अगर (*self != 0)
      वापस false;
  वापस ((*last) & Sbitset__last_byte_mask (nbits)) == 0;
}

व्योम
Sbitset__fprint (Sbitset self, Sbitset_सूचकांक nbits, खाता *file)
{
  Sbitset_सूचकांक i;
  Sbitset itr;
  bool first = true;
  ख_लिखो (file,
           "nbits = %" SBITSET_सूचकांक_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      अगर (first)
        first = false;
      अन्यथा
        ख_लिखो (file, ",");
      ख_लिखो (file, " %" SBITSET_सूचकांक_CONVERSION_SPEC, i);
    }
  ख_लिखो (file, " }");
}
