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

Contribution_اشاریہ ثابت Contribution_اشاریہ_none = -1;
Contribution_اشاریہ ثابت Contribution_اشاریہ_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, علامت *token,
                              bitset actions,
                              Inadequacy_List_گرہ_گنتی *گرہ_گنتی)
{
  InadequacyList *نتیجہ = xmalloc (حجم *نتیجہ);
  نتیجہ->id = *گرہ_گنتی;
  IGNORE_TYPE_LIMITS_BEGIN
  إذا (INT_جوڑو_WRAPV (*گرہ_گنتی, 1, گرہ_گنتی))
    aver (false);
  IGNORE_TYPE_LIMITS_سرا
  نتیجہ->اگلا = عدم;
  نتیجہ->manifestingState = manifesting_state;
  نتیجہ->contribution_گنتی_2 = bitset_گنتی (actions);
  نتیجہ->inadequacy.conflict.token = token;
  نتیجہ->inadequacy.conflict.actions = actions;
  أرجع نتیجہ;
}

فراغ
InadequacyList__delete (InadequacyList *self)
{
  طالما (self)
    {
      InadequacyList *گرہ = self;
      self = self->اگلا;
      bitset_free (گرہ->inadequacy.conflict.actions);
      حرر (گرہ);
    }
}

Contribution_اشاریہ
Inadequacy_List_get_کھسکاؤ_Contribution_اشاریہ (InadequacyList ثابت *self)
{
  إذا (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->عدد))
    أرجع Contribution_اشاریہ_none;
  أرجع self->contribution_گنتی_2 - 1;
}

علامت *
InadequacyList__getContributionToken (InadequacyList ثابت *self,
                                      Contribution_اشاریہ i)
{
  aver (0 <= i && i < self->contribution_گنتی_2); (فراغ) i;
  أرجع self->inadequacy.conflict.token;
}

فراغ
Inadequacy_List_prepend_منزل (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *سر_old = *list;
  *list = self;
  self->اگلا = سر_old;
}
