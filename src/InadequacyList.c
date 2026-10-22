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

ContributionIndex ধ্রুব ContributionIndex__none = -1;
ContributionIndex ধ্রুব ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, প্রতীক *token,
                              bitset actions,
                              Inadequacy_List_গ্রন্থি_গুনতি *গ্রন্থি_গুনতি)
{
  InadequacyList *result = xmalloc (পরিমাণ *result);
  result->id = *গ্রন্থি_গুনতি;
  IGNORE_TYPE_LIMITS_BEGIN
  যদি (INT_ADD_WRAPV (*গ্রন্থি_গুনতি, 1, গ্রন্থি_গুনতি))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = রিক্ত;
  result->manifestingState = manifesting_state;
  result->contribution_গুনতি_2 = bitset_গুনতি (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  প্রত্যাগম result;
}

শূন্য
InadequacyList__delete (InadequacyList *self)
{
  যাবত্ (self)
    {
      InadequacyList *গ্রন্থি = self;
      self = self->next;
      bitset_free (গ্রন্থি->inadequacy.conflict.actions);
      মুক্ত (গ্রন্থি);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ধ্রুব *self)
{
  যদি (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->অঙ্ক))
    প্রত্যাগম ContributionIndex__none;
  প্রত্যাগম self->contribution_গুনতি_2 - 1;
}

প্রতীক *
InadequacyList__getContributionToken (InadequacyList ধ্রুব *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_গুনতি_2); (শূন্য) i;
  প্রত্যাগম self->inadequacy.conflict.token;
}

শূন্য
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
