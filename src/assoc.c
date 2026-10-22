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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "assoc.h"


ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
assoc_ಗಮ್ಯ_string (assoc a)
{
  ಆಯ್ಕೆ (a)
    {
    ಸ್ಥಿತಿ undef_assoc:
      ಹಿಂತಿರುಗಿ "undefined associativity";

    ಸ್ಥಿತಿ ಬಲ_assoc:
      ಹಿಂತಿರುಗಿ "%right";

    ಸ್ಥಿತಿ ಎಡ_assoc:
      ಹಿಂತಿರುಗಿ "%left";

    ಸ್ಥಿತಿ non_assoc:
      ಹಿಂತಿರುಗಿ "%nonassoc";

    ಸ್ಥಿತಿ precedence_assoc:
      ಹಿಂತಿರುಗಿ "%precedence";
    }
  abort ();
}
