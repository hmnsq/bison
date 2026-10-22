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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "InadequacyList.h"

#অন্তর্ভাব <intprops.h>

Contribution_সূচক ধ্রুবক Contribution_সূচক_none = -1;
Contribution_সূচক ধ্রুবক Contribution_সূচক_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, প্রতীক *token,
                              bitset actions,
                              Inadequacy_List_গ্রন্থি_গুনতি *গ্রন্থি_গুনতি)
{
  InadequacyList *ফলাফল = xmalloc (আকার *ফলাফল);
  ফলাফল->id = *গ্রন্থি_গুনতি;
  IGNORE_TYPE_LIMITS_BEGIN
  যদি (INT_ADD_WRAPV (*গ্রন্থি_গুনতি, 1, গ্রন্থি_গুনতি))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  ফলাফল->পরবর্তী = ফাঁকা;
  ফলাফল->manifestingState = manifesting_state;
  ফলাফল->contribution_গুনতি_2 = bitset_গুনতি (actions);
  ফলাফল->inadequacy.conflict.token = token;
  ফলাফল->inadequacy.conflict.actions = actions;
  ফেরত ফলাফল;
}

শূন্য
InadequacyList__delete (InadequacyList *self)
{
  যখন (self)
    {
      InadequacyList *গ্রন্থি = self;
      self = self->পরবর্তী;
      bitset_free (গ্রন্থি->inadequacy.conflict.actions);
      মুক্ত (গ্রন্থি);
    }
}

Contribution_সূচক
Inadequacy_List_get_Shift_Contribution_সূচক (InadequacyList ধ্রুবক *self)
{
  যদি (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->অঙ্ক))
    ফেরত Contribution_সূচক_none;
  ফেরত self->contribution_গুনতি_2 - 1;
}

প্রতীক *
InadequacyList__getContributionToken (InadequacyList ধ্রুবক *self,
                                      Contribution_সূচক i)
{
  aver (0 <= i && i < self->contribution_গুনতি_2); (শূন্য) i;
  ফেরত self->inadequacy.conflict.token;
}

শূন্য
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *শীর্ষ_old = *list;
  *list = self;
  self->পরবর্তী = শীর্ষ_old;
}
