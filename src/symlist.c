/* Lists of symbols for Bison

   Copyright (C) 2002, 2005-2007, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_sym_new (ಚಿಹ್ನೆ *sym, location loc)
{
  ಚಿಹ್ನೆ_list *res = xmalloc (ಗಾತ್ರ *res);

  res->content_type = SYMLIST_ಚಿಹ್ನೆ;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = ಖಾಲಿ;

  res->midrule = ಖಾಲಿ;
  res->midrule_ಪೋಷಕ_ನಿಯಮ = ಖಾಲಿ;
  res->midrule_ಪೋಷಕ_rhs_ಸೂಚ್ಯಂಕ = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = ಖಾಲಿಗಳು_loc;
  res->ruleprec = ಖಾಲಿ;
  res->percent_ಖಾಲಿಗಳು_loc = ಖಾಲಿಗಳು_loc;
  ಸಂಕೇತ_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = ಖಾಲಿಗಳು_loc;
  res->merger = 0;
  res->merger_declaration_loc = ಖಾಲಿಗಳು_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->ಮುಂದಿನ = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_type_new (uniqstr type_name, location loc)
{
  ಚಿಹ್ನೆ_list *res = xmalloc (ಗಾತ್ರ *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (ಗಾತ್ರ (semantic_type));
  res->content.sem_type->ಟ್ಯಾಗ್ = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->ಸ್ಥಿತಿ_2 = undeclared;

  res->sym_loc = loc;
  res->named_ref = ಖಾಲಿ;
  res->ಮುಂದಿನ = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ res;
}


ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_type_ಗಣ (ಚಿಹ್ನೆ_list *syms, uniqstr type_name)
{
  ಗಾಗಿ (ಚಿಹ್ನೆ_list *l = syms; l; l = l->ಮುಂದಿನ)
    ಚಿಹ್ನೆ_type_ಗಣ (l->content.sym, type_name, l->sym_loc);
  ಹಿಂತಿರುಗಿ syms;
}


ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_ಕಂಡುಹಿಡಿ_ಚಿಹ್ನೆ (ಚಿಹ್ನೆ_list *l, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym)
{
  ಗಾಗಿ (/* Nothing. */; l && l->content.sym; l = l->ಮುಂದಿನ)
    ಆದರೆ (l->content.sym == sym)
      ಹಿಂತಿರುಗಿ l;
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_list_syms_print (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ_list *l, FILE *ಹೊರಹೋಗು)
{
  fputc ('[', ಹೊರಹೋಗು);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *sep = "";
  ಗಾಗಿ (/* Nothing. */; l && l->content.sym; l = l->ಮುಂದಿನ)
    {
      fputs (sep, ಹೊರಹೋಗು);
      fputs (l->content_type == SYMLIST_ಚಿಹ್ನೆ ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             ಹೊರಹೋಗು);
      ಆದರೆ (l->content_type == SYMLIST_ಚಿಹ್ನೆ)
        ಚಿಹ್ನೆ_print (l->content.sym, ಹೊರಹೋಗು);
      fputs (l->action_props.is_ಮೌಲ್ಯ_ಬಳಸಿದ ? " (used)" : " (unused)", ಹೊರಹೋಗು);
      putc ('}', ಹೊರಹೋಗು);
      sep = ", ";
    }
  fputc (']', ಹೊರಹೋಗು);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_prepend (ಚಿಹ್ನೆ_list *list, ಚಿಹ್ನೆ_list *ಗಂಟು)
{
  ಗಂಟು->ಮುಂದಿನ = list;
  ಹಿಂತಿರುಗಿ ಗಂಟು;
}


ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_ಕೊನೆಯ (ಚಿಹ್ನೆ_list *list)
{
  ಆದರೆ (!list)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ಚಿಹ್ನೆ_list *ಮುಂದಿನ = list;
  ಆಗ (ಮುಂದಿನ->ಮುಂದಿನ)
    ಮುಂದಿನ = ಮುಂದಿನ->ಮುಂದಿನ;
  ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
}

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_append (ಚಿಹ್ನೆ_list *list, ಚಿಹ್ನೆ_list *ಗಂಟು)
{
  ಆದರೆ (list)
    ಚಿಹ್ನೆ_list_ಕೊನೆಯ (list)->ಮುಂದಿನ = ಗಂಟು;
  ಇಲ್ಲದಿದ್ದರೆ
    list = ಗಂಟು;
  ಹಿಂತಿರುಗಿ list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_list_free (ಚಿಹ್ನೆ_list *list)
{
  ಗಾಗಿ (ಚಿಹ್ನೆ_list *ಮುಂದಿನ; list; list = ಮುಂದಿನ)
    {
      ಮುಂದಿನ = list->ಮುಂದಿನ;
      named_ref_free (list->named_ref);
      ಆದರೆ (list->content_type == SYMLIST_TYPE)
        ಬಿಡುಗಡೆ (list->content.sem_type);
      ಬಿಡುಗಡೆ (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

ಪೂರ್ಣಾಂಕ
ಚಿಹ್ನೆ_list_ಉದ್ದಳತೆ (ಚಿಹ್ನೆ_list ಸ್ಥಿರಾಂಕ *l)
{
  ಪೂರ್ಣಾಂಕ res = 0;
  ಗಾಗಿ (/* Nothing. */;
       l && !(l->content_type == SYMLIST_ಚಿಹ್ನೆ && l->content.sym == ಖಾಲಿ);
       l = l->ಮುಂದಿನ)
    ++res;
  ಹಿಂತಿರುಗಿ res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_n_get (ಚಿಹ್ನೆ_list *l, ಪೂರ್ಣಾಂಕ n)
{
  aver (0 <= n);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; ++i)
    {
      l = l->ಮುಂದಿನ;
      aver (l);
    }
  aver (l->content_type == SYMLIST_ಚಿಹ್ನೆ);
  aver (l->content.sym);
  ಹಿಂತಿರುಗಿ l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
ಚಿಹ್ನೆ_list_n_type_name_get (ಚಿಹ್ನೆ_list *l, ಪೂರ್ಣಾಂಕ n)
{
  ಹಿಂತಿರುಗಿ ಚಿಹ್ನೆ_list_n_get (l, n)->content.sym->content->type_name;
}

bool
ಚಿಹ್ನೆ_list_null (ಚಿಹ್ನೆ_list *ಗಂಟು)
{
  ಹಿಂತಿರುಗಿ (!ಗಂಟು
          || (ಗಂಟು->content_type == SYMLIST_ಚಿಹ್ನೆ && !ಗಂಟು->content.sym));
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_list_ಸಂಕೇತ_props_ಗಣ (ಚಿಹ್ನೆ_list *ಗಂಟು, ಸಂಕೇತ_props_type ಬಗೆ,
                            ಸಂಕೇತ_props ಸ್ಥಿರಾಂಕ *cprops)
{
  ಆಯ್ಕೆ (ಗಂಟು->content_type)
    {
    ಸ್ಥಿತಿ SYMLIST_ಚಿಹ್ನೆ:
      ಚಿಹ್ನೆ_ಸಂಕೇತ_props_ಗಣ (ಗಂಟು->content.sym, ಬಗೆ, cprops);
      ಆದರೆ (ಗಂಟು->content.sym->content->ಸ್ಥಿತಿ_2 == undeclared)
        ಗಂಟು->content.sym->content->ಸ್ಥಿತಿ_2 = ಬಳಸಿದ;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ SYMLIST_TYPE:
      semantic_type_ಸಂಕೇತ_props_ಗಣ
        (semantic_type_get (ಗಂಟು->content.sem_type->ಟ್ಯಾಗ್,
                            &ಗಂಟು->content.sem_type->location),
         ಬಗೆ, cprops);
      ಆದರೆ (ಗಂಟು->content.sem_type->ಸ್ಥಿತಿ_2 == undeclared)
        ಗಂಟು->content.sem_type->ಸ್ಥಿತಿ_2 = ಬಳಸಿದ;
      ನಿಲ್ಲಿಸು;
    }
}
