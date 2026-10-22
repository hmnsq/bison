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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "Sbitset.h"

Sbitset
Sbitset__new (Sbitset_ಸೂಚ್ಯಂಕ nbits)
{
  /* Some functions, like Sbitset__last_byte_mask, will fail if nbits = 0.  */
  aver (nbits);
  ಹಿಂತಿರುಗಿ xcalloc (1, Sbitset__nbytes (nbits));
}

Sbitset
Sbitset__new_on_obstack (Sbitset_ಸೂಚ್ಯಂಕ nbits, ರಚನೆ obstack *obstackp)
{
  Sbitset ಫಲಿತಾಂಶ;
  Sbitset ptr;
  Sbitset ಕೊನೆ;
  aver (nbits);
  ಫಲಿತಾಂಶ = obstack_alloc (obstackp, Sbitset__nbytes (nbits));
  ಗಾಗಿ (ptr = ಫಲಿತಾಂಶ, ಕೊನೆ = ಫಲಿತಾಂಶ + Sbitset__nbytes (nbits); ptr < ಕೊನೆ; ++ptr)
    *ptr = 0;
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ
Sbitset__delete (Sbitset self)
{
  ಬಿಡುಗಡೆ (self);
}

bool
Sbitset_is_ಖಾಲಿಗಳು (Sbitset self, Sbitset_ಸೂಚ್ಯಂಕ nbits)
{
  Sbitset ಕೊನೆಯ = self + Sbitset__nbytes (nbits) - 1;
  ಗಾಗಿ (; self < ಕೊನೆಯ; ++self)
    ಆದರೆ (*self != 0)
      ಹಿಂತಿರುಗಿ false;
  ಹಿಂತಿರುಗಿ ((*ಕೊನೆಯ) & Sbitset_ಕೊನೆಯ_byte_ಮುಖವಾಡ (nbits)) == 0;
}

ಶೂನ್ಯ
Sbitset__fprint (Sbitset self, Sbitset_ಸೂಚ್ಯಂಕ nbits, FILE *file)
{
  Sbitset_ಸೂಚ್ಯಂಕ i;
  Sbitset itr;
  bool ಮೊದಲ = true;
  fprintf (file,
           "nbits = %" SBITSET_ಸೂಚ್ಯಂಕ_CONVERSION_SPEC ", set = {",
           nbits);
  SBITSET__FOR_EACH (self, nbits, itr, i)
    {
      ಆದರೆ (ಮೊದಲ)
        ಮೊದಲ = false;
      ಇಲ್ಲದಿದ್ದರೆ
        fprintf (file, ",");
      fprintf (file, " %" SBITSET_ಸೂಚ್ಯಂಕ_CONVERSION_SPEC, i);
    }
  fprintf (file, " }");
}
