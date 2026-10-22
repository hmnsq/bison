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

Contribution_شاخص ثابت Contribution_شاخص_none = -1;
Contribution_شاخص ثابت Contribution_شاخص_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, نښه *token,
                              bitset actions,
                              Inadequacy_List_غوټه_شمېر *غوټه_شمېر)
{
  InadequacyList *پایله = xmalloc (حجم *پایله);
  پایله->id = *غوټه_شمېر;
  IGNORE_TYPE_LIMITS_BEGIN
  إذا (INT_جمع_کړه_WRAPV (*غوټه_شمېر, 1, غوټه_شمېر))
    aver (false);
  IGNORE_TYPE_LIMITS_پای_ټکی
  پایله->بل = عدم;
  پایله->manifestingState = manifesting_state;
  پایله->contribution_شمېر_2 = bitset_شمېر (actions);
  پایله->inadequacy.conflict.token = token;
  پایله->inadequacy.conflict.actions = actions;
  أرجع پایله;
}

فراغ
InadequacyList__delete (InadequacyList *self)
{
  طالما (self)
    {
      InadequacyList *غوټه = self;
      self = self->بل;
      bitset_free (غوټه->inadequacy.conflict.actions);
      حرر (غوټه);
    }
}

Contribution_شاخص
Inadequacy_List_get_ښويدنه_Contribution_شاخص (InadequacyList ثابت *self)
{
  إذا (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->عدد))
    أرجع Contribution_شاخص_none;
  أرجع self->contribution_شمېر_2 - 1;
}

نښه *
InadequacyList__getContributionToken (InadequacyList ثابت *self,
                                      Contribution_شاخص i)
{
  aver (0 <= i && i < self->contribution_شمېر_2); (فراغ) i;
  أرجع self->inadequacy.conflict.token;
}

فراغ
Inadequacy_List_prepend_منزل (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *سر_old = *list;
  *list = self;
  self->بل = سر_old;
}
