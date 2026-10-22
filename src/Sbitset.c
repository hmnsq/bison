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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "Sbitset.h"

Sbitset
Sbitset__new (Sbitset_सूचकांक nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  प्रत्यागम xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_सूचकांक nbits, संरचना obstack *obstackp)
{
  Sbitset परिणाम;
  Sbitset ptr;
  Sbitset अन्तः;
  aver (nbits);
  परिणाम = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  प्रति (ptr = परिणाम, अन्तः = परिणाम + Sbitset__nbytes (nbits); ptr < अन्तः; ++ptr)
    *ptr = 0;
  प्रत्यागम परिणाम;
}

शून्य
Sbitset__delete (Sbitset self)
{
  मुक्त (self);
}

bool
Sbitset_is_रिक्तानि (Sbitset self, Sbitset_सूचकांक nbits)
{
  Sbitset चरमः = self + Sbitset__nbytes (nbits) - 1;
  प्रति (; self < चरमः; ++self)
    यदि (*self != 0)
      प्रत्यागम false;
  प्रत्यागम ((*चरमः) & Sbitset_चरमः_byte_आवरणम् (nbits)) == 0;
}

शून्य
Sbitset__fprint (Sbitset self, Sbitset_सूचकांक nbits, FILE *file)
{
  Sbitset_सूचकांक i;
  Sbitset itr;
  bool first = true;
  fprintf (file,
           "nbits = %" SBITSET_सूचकांक_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      यदि (first)
        first = false;
      अन्यथा
        fprintf (file, ",");
      fprintf (file, " %" SBITSET_सूचकांक_CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
