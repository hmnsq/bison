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

ContributionIndex अचर ContributionIndex__none = -1;
ContributionIndex अचर ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, चिह्न *token,
                              bitset actions,
                              Inadequacy_List_गाँठो_गन्ती *गाँठो_गन्ती)
{
  InadequacyList *result = xmalloc (परिमाण *result);
  result->id = *गाँठो_गन्ती;
  IGNORE_TYPE_LIMITS_BEGIN
  यदि (INT_ADD_WRAPV (*गाँठो_गन्ती, 1, गाँठो_गन्ती))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = रिक्त;
  result->manifestingState = manifesting_state;
  result->contribution_गन्ती_2 = bitset_गन्ती (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  फर्काऊ result;
}

शून्य
InadequacyList__delete (InadequacyList *self)
{
  जबसम्म (self)
    {
      InadequacyList *गाँठो = self;
      self = self->next;
      bitset_free (गाँठो->inadequacy.conflict.actions);
      मुक्त (गाँठो);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList अचर *self)
{
  यदि (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->अङ्क))
    फर्काऊ ContributionIndex__none;
  फर्काऊ self->contribution_गन्ती_2 - 1;
}

चिह्न *
InadequacyList__getContributionToken (InadequacyList अचर *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_गन्ती_2); (शून्य) i;
  फर्काऊ self->inadequacy.conflict.token;
}

शून्य
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
