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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "InadequacyList.h"

#अन्तर्भाव <intprops.h>

Contribution_सूचकांक ध्रुव Contribution_सूचकांक_none = -1;
Contribution_सूचकांक ध्रुव Contribution_सूचकांक_error_action = -2;

InadequacyList *
InadequacyList__new_conflict (state *manifesting_state, चिह्नम् *token,
                              bitset actions,
                              Inadequacy_List_ग्रन्थिः_गणनम् *ग्रन्थिः_गणनम्)
{
  InadequacyList *परिणाम = xmalloc (परिमाण *परिणाम);
  परिणाम->id = *ग्रन्थिः_गणनम्;
  IGNORE_TYPE_LIMITS_BEGIN
  यदि (INT_सङ्कलय_WRAPV (*ग्रन्थिः_गणनम्, 1, ग्रन्थिः_गणनम्))
    aver (false);
  IGNORE_TYPE_LIMITS_अन्तः
  परिणाम->अग्र = रिक्त;
  परिणाम->manifestingState = manifesting_state;
  परिणाम->contribution_गणनम्_2 = bitset_गणनम् (actions);
  परिणाम->inadequacy.conflict.token = token;
  परिणाम->inadequacy.conflict.actions = actions;
  प्रत्यागम परिणाम;
}

शून्य
InadequacyList__delete (InadequacyList *self)
{
  यावत् (self)
    {
      InadequacyList *ग्रन्थिः = self;
      self = self->अग्र;
      bitset_free (ग्रन्थिः->inadequacy.conflict.actions);
      मुक्त (ग्रन्थिः);
    }
}

Contribution_सूचकांक
Inadequacy_List_get_सरणम्_Contribution_सूचकांक (InadequacyList ध्रुव *self)
{
  यदि (!bitset_test (self->inadequacy.conflict.actions,
                    self->manifestingState->reductions->अङ्कः))
    प्रत्यागम Contribution_सूचकांक_none;
  प्रत्यागम self->contribution_गणनम्_2 - 1;
}

चिह्नम् *
InadequacyList__getContributionToken (InadequacyList ध्रुव *self,
                                      Contribution_सूचकांक i)
{
  aver (0 <= i && i < self->contribution_गणनम्_2); (शून्य) i;
  प्रत्यागम self->inadequacy.conflict.token;
}

शून्य
Inadequacy_List_prepend_गन्तव्यम् (InadequacyList *self, InadequacyList **list)
{
  InadequacyList *शीर्ष_old = *list;
  *list = self;
  self->अग्र = शीर्ष_old;
}
