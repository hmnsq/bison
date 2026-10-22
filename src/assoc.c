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


ಧ್ರುವ ವರ್ಣ *
assoc_to_string (assoc a)
{
  ವಿಕಲ್ಪ (a)
    {
    ಪಕ್ಷ undef_assoc:
      ಪ್ರತ್ಯಾಗಮ "undefined associativity";

    ಪಕ್ಷ ಬಲ_assoc:
      ಪ್ರತ್ಯಾಗಮ "%right";

    ಪಕ್ಷ ಎಡ_assoc:
      ಪ್ರತ್ಯಾಗಮ "%left";

    ಪಕ್ಷ non_assoc:
      ಪ್ರತ್ಯಾಗಮ "%nonassoc";

    ಪಕ್ಷ precedence_assoc:
      ಪ್ರತ್ಯಾಗಮ "%precedence";
    }
  abort ();
}
