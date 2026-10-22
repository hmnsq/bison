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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "InadequacyList.h"

#அந்தர்பாவ <intprops.h>

ContributionIndex மாறிலி ContributionIndex__none = -1;
ContributionIndex மாறிலி ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, குறியீடு *token,
                              bitset actions,
                              Inadequacy_List_முனை_மொத்தம் *முனை_மொத்தம்)
{
  InadequacyList *result = xmalloc (அளவு *result);
  result->id = *முனை_மொத்தம்;
  IGNORE_TYPE_LIMITS_BEGIN
  எனில் (INT_ADD_WRAPV (*முனை_மொத்தம், 1, முனை_மொத்தம்))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = வெற்று;
  result->manifestingState = manifesting_state;
  result->contribution_மொத்தம்_2 = bitset_மொத்தம் (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  திரும்பு result;
}

வெறுமை
InadequacyList__delete (InadequacyList *self)
{
  போது (self)
    {
      InadequacyList *முனை = self;
      self = self->next;
      bitset_free (முனை->inadequacy.conflict.actions);
      விடுவி (முனை);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList மாறிலி *self)
{
  எனில் (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->இலக்கம்))
    திரும்பு ContributionIndex__none;
  திரும்பு self->contribution_மொத்தம்_2 - 1;
}

குறியீடு *
InadequacyList__getContributionToken (InadequacyList மாறிலி *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_மொத்தம்_2); (வெறுமை) i;
  திரும்பு self->inadequacy.conflict.token;
}

வெறுமை
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
