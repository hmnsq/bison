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
InadequacyList__new_conflict (state *manifesting_state, symbol *token,
                              bitset actions,
                              InadequacyListNodeCount *node_count)
{
  InadequacyList *result = xmalloc (حجم *result);
  result->id = *node_count;
  IGNORE_TYPE_LIMITS_BEGIN
  אם (INT_ADD_WRAPV (*node_count, 1, node_count))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = عدم;
  result->manifestingState = manifesting_state;
  result->contributionCount = bitset_count (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  החזר result;
}

فراغ
InadequacyList__delete (InadequacyList *self)
{
  כלעוד (self)
    {
      InadequacyList *node = self;
      self = self->next;
      bitset_free (node->inadequacy.conflict.actions);
      حرر (node);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList ثابت *self)
{
  אם (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->num))
    החזר ContributionIndex__none;
  החזר self->contributionCount - 1;
}

symbol *
InadequacyList__getContributionToken (InadequacyList ثابت *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contributionCount); (فراغ) i;
  החזר self->inadequacy.conflict.token;
}

فراغ
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
