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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "InadequacyList.h"

#अन्तर्भाव <intprops.h>

ContributionIndex ध्रुव ContributionIndex__none = -1;
ContributionIndex ध्रुव ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, चिह्नम् *token,
                              bitset actions,
                              Inadequacy_List_ग्रन्थिः_गणनम् *ग्रन्थिः_गणनम्)
{
  InadequacyList *result = xmalloc (परिमाण *result);
  result->id = *ग्रन्थिः_गणनम्;
  IGNORE_TYPE_LIMITS_BEGIN
  जइ (INT_ADD_WRAPV (*ग्रन्थिः_गणनम्, 1, ग्रन्थिः_गणनम्))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = रिक्त;
  result->manifestingState = manifesting_state;
  result->contribution_गणनम्_2 = bitset_गणनम् (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  प्रत्यागम result;
}

शून्य
InadequacyList__delete (InadequacyList *self)
{
  जाव (self)
    {
      InadequacyList *ग्रन्थिः = self;
      self = self->next;
      bitset_free (ग्रन्थिः->inadequacy.conflict.actions);
      मुक्त (ग्रन्थिः);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ध्रुव *self)
{
  जइ (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->अङ्कः))
    प्रत्यागम ContributionIndex__none;
  प्रत्यागम self->contribution_गणनम्_2 - 1;
}

चिह्नम् *
InadequacyList__getContributionToken (InadequacyList ध्रुव *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_गणनम्_2); (शून्य) i;
  प्रत्यागम self->inadequacy.conflict.token;
}

शून्य
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
