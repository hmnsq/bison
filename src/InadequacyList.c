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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "InadequacyList.h"

#અન્તર્ભાવ <intprops.h>

ContributionIndex અચલ ContributionIndex__none = -1;
ContributionIndex અચલ ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, પ્રતીક *token,
                              bitset actions,
                              Inadequacy_List_ગાંઠ_ગણતરી *ગાંઠ_ગણતરી)
{
  InadequacyList *result = xmalloc (કદ *result);
  result->id = *ગાંઠ_ગણતરી;
  IGNORE_TYPE_LIMITS_BEGIN
  જો (INT_ADD_WRAPV (*ગાંઠ_ગણતરી, 1, ગાંઠ_ગણતરી))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = ખાલી;
  result->manifestingState = manifesting_state;
  result->contribution_ગણતરી_2 = bitset_ગણતરી (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  પાછા result;
}

શૂન્ય
InadequacyList__delete (InadequacyList *self)
{
  જ્યારે (self)
    {
      InadequacyList *ગાંઠ = self;
      self = self->next;
      bitset_free (ગાંઠ->inadequacy.conflict.actions);
      મુક્ત (ગાંઠ);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList અચલ *self)
{
  જો (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->અંક))
    પાછા ContributionIndex__none;
  પાછા self->contribution_ગણતરી_2 - 1;
}

પ્રતીક *
InadequacyList__getContributionToken (InadequacyList અચલ *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_ગણતરી_2); (શૂન્ય) i;
  પાછા self->inadequacy.conflict.token;
}

શૂન્ય
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
