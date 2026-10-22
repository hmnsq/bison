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

ContributionIndex स्थिरांक ContributionIndex__none = -1;
ContributionIndex स्थिरांक ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, चिन्ह *token,
                              bitset actions,
                              Inadequacy_List_गाठ_मोजणी *गाठ_मोजणी)
{
  InadequacyList *result = xmalloc (आकार *result);
  result->id = *गाठ_मोजणी;
  IGNORE_TYPE_LIMITS_BEGIN
  जर (INT_ADD_WRAPV (*गाठ_मोजणी, 1, गाठ_मोजणी))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = रिक्त;
  result->manifestingState = manifesting_state;
  result->contribution_मोजणी_2 = bitset_मोजणी (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  परत result;
}

शून्य
InadequacyList__delete (InadequacyList *self)
{
  पर्यंत (self)
    {
      InadequacyList *गाठ = self;
      self = self->next;
      bitset_free (गाठ->inadequacy.conflict.actions);
      मुक्त (गाठ);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList स्थिरांक *self)
{
  जर (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->अंक))
    परत ContributionIndex__none;
  परत self->contribution_मोजणी_2 - 1;
}

चिन्ह *
InadequacyList__getContributionToken (InadequacyList स्थिरांक *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_मोजणी_2); (शून्य) i;
  परत self->inadequacy.conflict.token;
}

शून्य
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
