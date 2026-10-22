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

#include <config.h>
#include "system.h"

#include "InadequacyList.h"

#include <intprops.h>

ContributionIndex const ContributionIndex__none = -1;
ContributionIndex const ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, सिम्बोलो *token,
                              bitset actions,
                              Inadequacy_List_बुहोल_बिलांग *बुहोल_बिलांग)
{
  InadequacyList *result = xmalloc (sizeof *result);
  result->id = *बुहोल_बिलांग;
  IGNORE_TYPE_LIMITS_BEGIN
  if (INT_ADD_WRAPV (*बुहोल_बिलांग, 1, बुहोल_बिलांग))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = NULL;
  result->manifestingState = manifesting_state;
  result->contribution_बिलांग_2 = bitset_बिलांग (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  return result;
}

void
InadequacyList__delete (InadequacyList *self)
{
  while (self)
    {
      InadequacyList *बुहोल = self;
      self = self->next;
      bitset_free (बुहोल->inadequacy.conflict.actions);
      free (बुहोल);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList const *self)
{
  if (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->नुमेरो))
    return ContributionIndex__none;
  return self->contribution_बिलांग_2 - 1;
}

सिम्बोलो *
InadequacyList__getContributionToken (InadequacyList const *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_बिलांग_2); (void) i;
  return self->inadequacy.conflict.token;
}

void
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
