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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "InadequacyList.h"

#അന്തര്ഭാവ <intprops.h>

ContributionIndex സ്ഥിരം ContributionIndex__none = -1;
ContributionIndex സ്ഥിരം ContributionIndex__error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, ചിഹ്നം *token,
                              bitset actions,
                              Inadequacy_List_കണ്ണി_എണ്ണം *കണ്ണി_എണ്ണം)
{
  InadequacyList *result = xmalloc (വലുപ്പം *result);
  result->id = *കണ്ണി_എണ്ണം;
  IGNORE_TYPE_LIMITS_BEGIN
  എങ്കിൽ (INT_ADD_WRAPV (*കണ്ണി_എണ്ണം, 1, കണ്ണി_എണ്ണം))
    aver (false);
  IGNORE_TYPE_LIMITS_END
  result->next = ശൂന്യ;
  result->manifestingState = manifesting_state;
  result->contribution_എണ്ണം_2 = bitset_എണ്ണം (actions);
  result->inadequacy.conflict.token = token;
  result->inadequacy.conflict.actions = actions;
  തിരികെ result;
}

ശൂന്യം
InadequacyList__delete (InadequacyList *self)
{
  ആകുമ്പോൾ (self)
    {
      InadequacyList *കണ്ണി = self;
      self = self->next;
      bitset_free (കണ്ണി->inadequacy.conflict.actions);
      സ്വതന്ത്രം (കണ്ണി);
    }
}

ContributionIndex
InadequacyList__getShiftContributionIndex (InadequacyList സ്ഥിരം *self)
{
  എങ്കിൽ (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->അക്കം))
    തിരികെ ContributionIndex__none;
  തിരികെ self->contribution_എണ്ണം_2 - 1;
}

ചിഹ്നം *
InadequacyList__getContributionToken (InadequacyList സ്ഥിരം *self,
                                      ContributionIndex i)
{
  aver (0 <= i && i < self->contribution_എണ്ണം_2); (ശൂന്യം) i;
  തിരികെ self->inadequacy.conflict.token;
}

ശൂന്യം
InadequacyList__prependTo (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *head_old = *list;
  *list = self;
  self->next = head_old;
}
