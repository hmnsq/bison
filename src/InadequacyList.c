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

#ضمن <config.h>
#ضمن "system.h"

#ضمن "InadequacyList.h"

#ضمن <intprops.h>

ContributionIndex ثابت ContributionIndex__none = -1;
ContributionIndex ثابت ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, رمز *token,
                              bitset actions,
                              Inadequacy_List_عقدة_تعداد *عقدة_تعداد)
{
  InadequacyList *result = xmalloc (حجم *result);
  result->id = *عقدة_تعداد;
  IGNORE_TYPE_LIMITS_BEGIN
  إذا (INT_ADD_WRAPV (*عقدة_تعداد, 1, عقدة_تعداد))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = عدم;
  result->manifestingState = manifesting_state;
  result->contribution_تعداد_2 = bitset_تعداد (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  إرجاع result;
}

فراغ
InadequacyList__delete (InadequacyList *self)
{
  بينما (self)
    {
      InadequacyList *عقدة = self;
      self = self->next;
      bitset_free (عقدة->inadequacy.conflict.actions);
      حرر (عقدة);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ثابت *self)
{
  إذا (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->رقم))
    إرجاع ContributionIndex__none;
  إرجاع self->contribution_تعداد_2 - 1;
}

رمز *
InadequacyList__getContributionToken (InadequacyList ثابت *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_تعداد_2); (فراغ) i;
  إرجاع self->inadequacy.conflict.token;
}

فراغ
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
