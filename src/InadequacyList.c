/* IELR's inadequacy list.

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

#ಅನ್ತರ್ಭಾವ "InadequacyList.h"

#ಅನ್ತರ್ಭಾವ <intprops.h>

Contribution_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ Contribution_ಸೂಚ್ಯಂಕ_none = -1;
Contribution_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ Contribution_ಸೂಚ್ಯಂಕ_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, ಚಿಹ್ನೆ *token,
                              bitset actions,
                              Inadequacy_List_ಗಂಟು_ಎಣಿಕೆ *ಗಂಟು_ಎಣಿಕೆ)
{
  InadequacyList *ಫಲಿತಾಂಶ = xmalloc (ಗಾತ್ರ *ಫಲಿತಾಂಶ);
  ಫಲಿತಾಂಶ->id = *ಗಂಟು_ಎಣಿಕೆ;
  IGNORE_TYPE_LIMITS_BEGIN
  ಆದರೆ (INT_ಕೂಡಿಸು_WRAPV (*ಗಂಟು_ಎಣಿಕೆ, 1, ಗಂಟು_ಎಣಿಕೆ))
    aver (false);
  IGNORE_TYPE_LIMITS_ಕೊನೆ
  ಫಲಿತಾಂಶ->ಮುಂದಿನ = ಖಾಲಿ;
  ಫಲಿತಾಂಶ->manifestingState = manifesting_state;
  ಫಲಿತಾಂಶ->contribution_ಎಣಿಕೆ_2 = bitset_ಎಣಿಕೆ (actions);
  ಫಲಿತಾಂಶ->inadequacy.conflict.token = token;
  ಫಲಿತಾಂಶ->inadequacy.conflict.actions = actions;
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ
InadequacyList__delete (InadequacyList *self)
{
  ಆಗ (self)
    {
      InadequacyList *ಗಂಟು = self;
      self = self->ಮುಂದಿನ;
      bitset_free (ಗಂಟು->inadequacy.conflict.actions);
      ಬಿಡುಗಡೆ (ಗಂಟು);
    }
}

Contribution_ಸೂಚ್ಯಂಕ
Inadequacy_List_get_ಸರಿಸು_Contribution_ಸೂಚ್ಯಂಕ (InadequacyList ಸ್ಥಿರಾಂಕ *self)
{
  ಆದರೆ (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->ಅಂಕೆ))
    ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
  ಹಿಂತಿರುಗಿ self->contribution_ಎಣಿಕೆ_2 - 1;
}

ಚಿಹ್ನೆ *
InadequacyList__getContributionToken (InadequacyList ಸ್ಥಿರಾಂಕ *self,
                                      Contribution_ಸೂಚ್ಯಂಕ i)
{
  aver (0 <= i && i < self->contribution_ಎಣಿಕೆ_2); (ಶೂನ್ಯ) i;
  ಹಿಂತಿರುಗಿ self->inadequacy.conflict.token;
}

ಶೂನ್ಯ
Inadequacy_List_prepend_ಗಮ್ಯ (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *ತಲೆ_old = *list;
  *list = self;
  self->ಮುಂದಿನ = ತಲೆ_old;
}
