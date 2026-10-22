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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "InadequacyList.h"

#ଅନ୍ତର୍ଭାଵ <intprops.h>

ContributionIndex ଅଚଳ ContributionIndex__none = -1;
ContributionIndex ଅଚଳ ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, ପ୍ରତୀକ *token,
                              bitset actions,
                              Inadequacy_List_ଗଣ୍ଠି_ଗଣନା *ଗଣ୍ଠି_ଗଣନା)
{
  InadequacyList *result = xmalloc (ଆକାର *result);
  result->id = *ଗଣ୍ଠି_ଗଣନା;
  IGNORE_TYPE_LIMITS_BEGIN
  ଯଦି (INT_ADD_WRAPV (*ଗଣ୍ଠି_ଗଣନା, 1, ଗଣ୍ଠି_ଗଣନା))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = ଖାଲି;
  result->manifestingState = manifesting_state;
  result->contribution_ଗଣନା_2 = bitset_ଗଣନା (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  ଫେରନ୍ତୁ result;
}

ଶୂନ୍ୟ
InadequacyList__delete (InadequacyList *self)
{
  ଯେତେବେଳେ (self)
    {
      InadequacyList *ଗଣ୍ଠି = self;
      self = self->next;
      bitset_free (ଗଣ୍ଠି->inadequacy.conflict.actions);
      ମୁକ୍ତ (ଗଣ୍ଠି);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ଅଚଳ *self)
{
  ଯଦି (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->ଅଙ୍କ))
    ଫେରନ୍ତୁ ContributionIndex__none;
  ଫେରନ୍ତୁ self->contribution_ଗଣନା_2 - 1;
}

ପ୍ରତୀକ *
InadequacyList__getContributionToken (InadequacyList ଅଚଳ *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_ଗଣନା_2); (ଶୂନ୍ୟ) i;
  ଫେରନ୍ତୁ self->inadequacy.conflict.token;
}

ଶୂନ୍ୟ
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
