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

Contribution_اشاريو ثابت Contribution_اشاريو_none = -1;
Contribution_اشاريو ثابت Contribution_اشاريو_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, نشان *token,
                              bitset actions,
                              Inadequacy_List_ڳنڍ_ڳڻپ *ڳنڍ_ڳڻپ)
{
  InadequacyList *نتيجو = xmalloc (حجم *نتيجو);
  نتيجو->id = *ڳنڍ_ڳڻپ;
  IGNORE_TYPE_LIMITS_BEGIN
  إذا (INT_جوڙيو_WRAPV (*ڳنڍ_ڳڻپ, 1, ڳنڍ_ڳڻپ))
    aver (false);
  IGNORE_TYPE_LIMITS_پڇاڙي
  نتيجو->ايندڙ = عدم;
  نتيجو->manifestingState = manifesting_state;
  نتيجو->contribution_ڳڻپ_2 = bitset_ڳڻپ (actions);
  نتيجو->inadequacy.conflict.token = token;
  نتيجو->inadequacy.conflict.actions = actions;
  أرجع نتيجو;
}

فراغ
InadequacyList__delete (InadequacyList *self)
{
  طالما (self)
    {
      InadequacyList *ڳنڍ = self;
      self = self->ايندڙ;
      bitset_free (ڳنڍ->inadequacy.conflict.actions);
      حرر (ڳنڍ);
    }
}

Contribution_اشاريو
Inadequacy_List_get_سرڪايو_Contribution_اشاريو (InadequacyList ثابت *self)
{
  إذا (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->انگ))
    أرجع Contribution_اشاريو_none;
  أرجع self->contribution_ڳڻپ_2 - 1;
}

نشان *
InadequacyList__getContributionToken (InadequacyList ثابت *self,
                                      Contribution_اشاريو i)
{
  aver (0 <= i && i < self->contribution_ڳڻپ_2); (فراغ) i;
  أرجع self->inadequacy.conflict.token;
}

فراغ
Inadequacy_List_prepend_منزل (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *سر_old = *list;
  *list = self;
  self->ايندڙ = سر_old;
}
