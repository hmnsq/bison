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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "InadequacyList.h"

#समावेश <intprops.h>

Contribution_सूचकांक स्थिर Contribution_सूचकांक_none = -1;
Contribution_सूचकांक स्थिर Contribution_सूचकांक_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, प्रतीक *token,
                              bitset actions,
                              Inadequacy_List_गाँठ_गिनती *गाँठ_गिनती)
{
  InadequacyList *परिणाम = xmalloc (माप *परिणाम);
  परिणाम->id = *गाँठ_गिनती;
  IGNORE_TYPE_LIMITS_BEGIN
  अगर (INT_ADD_WRAPV (*गाँठ_गिनती, 1, गाँठ_गिनती))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  परिणाम->अग्र = शून्य;
  परिणाम->manifestingState = manifesting_state;
  परिणाम->contribution_गिनती_2 = bitset_गिनती (actions);
  परिणाम->inadequacy.conflict.token = token;
  परिणाम->inadequacy.conflict.actions = actions;
  वापस परिणाम;
}

व्योम
InadequacyList__delete (InadequacyList *self)
{
  जबतक (self)
    {
      InadequacyList *गाँठ = self;
      self = self->अग्र;
      bitset_free (गाँठ->inadequacy.conflict.actions);
      मुक्त (गाँठ);
    }
}

Contribution_सूचकांक
Inadequacy_List_get_Shift_Contribution_सूचकांक (InadequacyList स्थिर *self)
{
  अगर (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->अंक))
    वापस Contribution_सूचकांक_none;
  वापस self->contribution_गिनती_2 - 1;
}

प्रतीक *
InadequacyList__getContributionToken (InadequacyList स्थिर *self,
                                      Contribution_सूचकांक i)
{
  aver (0 <= i && i < self->contribution_गिनती_2); (व्योम) i;
  वापस self->inadequacy.conflict.token;
}

व्योम
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *शीर्ष_old = *list;
  *list = self;
  self->अग्र = शीर्ष_old;
}
