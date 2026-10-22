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

ContributionIndex ಧ್ರುವ ContributionIndex__none = -1;
ContributionIndex ಧ್ರುವ ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, ಚಿಹ್ನೆ *token,
                              bitset actions,
                              Inadequacy_List_ಗಂಟು_ಎಣಿಕೆ *ಗಂಟು_ಎಣಿಕೆ)
{
  InadequacyList *result = xmalloc (ಪರಿಮಾಣ *result);
  result->id = *ಗಂಟು_ಎಣಿಕೆ;
  IGNORE_TYPE_LIMITS_BEGIN
  ಯದಿ (INT_ADD_WRAPV (*ಗಂಟು_ಎಣಿಕೆ, 1, ಗಂಟು_ಎಣಿಕೆ))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = ರಿಕ್ತ;
  result->manifestingState = manifesting_state;
  result->contribution_ಎಣಿಕೆ_2 = bitset_ಎಣಿಕೆ (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  ಪ್ರತ್ಯಾಗಮ result;
}

ಶೂನ್ಯ
InadequacyList__delete (InadequacyList *self)
{
  ಯಾವತ್ (self)
    {
      InadequacyList *ಗಂಟು = self;
      self = self->next;
      bitset_free (ಗಂಟು->inadequacy.conflict.actions);
      ಮುಕ್ತ (ಗಂಟು);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ಧ್ರುವ *self)
{
  ಯದಿ (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->ಅಂಕೆ))
    ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
  ಪ್ರತ್ಯಾಗಮ self->contribution_ಎಣಿಕೆ_2 - 1;
}

ಚಿಹ್ನೆ *
InadequacyList__getContributionToken (InadequacyList ಧ್ರುವ *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_ಎಣಿಕೆ_2); (ಶೂನ್ಯ) i;
  ಪ್ರತ್ಯಾಗಮ self->inadequacy.conflict.token;
}

ಶೂನ್ಯ
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
