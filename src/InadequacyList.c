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

#شامل <config.h>
#شامل "system.h"

#شامل "InadequacyList.h"

#شامل <intprops.h>

ContributionIndex ثابت ContributionIndex__none = -1;
ContributionIndex ثابت ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, نماد *token,
                              bitset actions,
                              Inadequacy_List_گره_شمار *گره_شمار)
{
  InadequacyList *result = xmalloc (اندازه *result);
  result->id = *گره_شمار;
  IGNORE_TYPE_LIMITS_BEGIN
  اگر (INT_ADD_WRAPV (*گره_شمار, 1, گره_شمار))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = پوچ;
  result->manifestingState = manifesting_state;
  result->contribution_شمار_2 = bitset_شمار (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  بازگشت result;
}

تهی
InadequacyList__delete (InadequacyList *self)
{
  وقتی (self)
    {
      InadequacyList *گره = self;
      self = self->next;
      bitset_free (گره->inadequacy.conflict.actions);
      آزاد (گره);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ثابت *self)
{
  اگر (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->رقم))
    بازگشت ContributionIndex__none;
  بازگشت self->contribution_شمار_2 - 1;
}

نماد *
InadequacyList__getContributionToken (InadequacyList ثابت *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_شمار_2); (تهی) i;
  بازگشت self->inadequacy.conflict.token;
}

تهی
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
