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
  ಚಿಹ್ನೆ_list *res = xmalloc (ಪರಿಮಾಣ *res);

  res->content_type = SYMLIST_ಚಿಹ್ನೆ;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = ರಿಕ್ತ;

  res->midrule = ರಿಕ್ತ;
  res->midrule_parent_rule = ರಿಕ್ತ;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = ರಿಕ್ತ;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_type_new (uniqstr type_name, location loc)
{
  ಚಿಹ್ನೆ_list *res = xmalloc (ಪರಿಮಾಣ *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (ಪರಿಮಾಣ (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = ರಿಕ್ತ;
  res->next = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ res;
}


ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_type_set (ಚಿಹ್ನೆ_list *syms, uniqstr type_name)
{
  ಪ್ರತಿ (ಚಿಹ್ನೆ_list *l = syms; l; l = l->next)
    ಚಿಹ್ನೆ_type_set (l->content.sym, type_name, l->sym_loc);
  ಪ್ರತ್ಯಾಗಮ syms;
}


ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_find_ಚಿಹ್ನೆ (ಚಿಹ್ನೆ_list *l, ಧ್ರುವ ಚಿಹ್ನೆ *sym)
{
  ಪ್ರತಿ (/* Nothing. */; l && l->content.sym; l = l->next)
    ಯದಿ (l->content.sym == sym)
      ಪ್ರತ್ಯಾಗಮ l;
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_list_syms_print (ಧ್ರುವ ಚಿಹ್ನೆ_list *l, FILE *out)
{
  fputc ('[', out);
  ವರ್ಣ ಧ್ರುವ *sep = "";
  ಪ್ರತಿ (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_ಚಿಹ್ನೆ ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      ಯದಿ (l->content_type == SYMLIST_ಚಿಹ್ನೆ)
        ಚಿಹ್ನೆ_print (l->content.sym, out);
      fputs (l->action_props.is_ಮೌಲ್ಯ_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_prepend (ಚಿಹ್ನೆ_list *list, ಚಿಹ್ನೆ_list *ಗಂಟು)
{
  ಗಂಟು->next = list;
  ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
}


ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_last (ಚಿಹ್ನೆ_list *list)
{
  ಯದಿ (!list)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ಚಿಹ್ನೆ_list *next = list;
  ಯಾವತ್ (next->next)
    next = next->next;
  ಪ್ರತ್ಯಾಗಮ next;
}

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_append (ಚಿಹ್ನೆ_list *list, ಚಿಹ್ನೆ_list *ಗಂಟು)
{
  ಯದಿ (list)
    ಚಿಹ್ನೆ_list_last (list)->next = ಗಂಟು;
  ಅನ್ಯಥಾ
    list = ಗಂಟು;
  ಪ್ರತ್ಯಾಗಮ list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

ಶೂನ್ಯ
ಚಿಹ್ನೆ_list_free (ಚಿಹ್ನೆ_list *list)
{
  ಪ್ರತಿ (ಚಿಹ್ನೆ_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      ಯದಿ (list->content_type == SYMLIST_TYPE)
        ಮುಕ್ತ (list->content.sem_type);
      ಮುಕ್ತ (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

ಪೂರ್ಣಾಙ್ಕ
ಚಿಹ್ನೆ_list_length (ಚಿಹ್ನೆ_list ಧ್ರುವ *l)
{
  ಪೂರ್ಣಾಙ್ಕ res = 0;
  ಪ್ರತಿ (/* Nothing. */;
       l && !(l->content_type == SYMLIST_ಚಿಹ್ನೆ && l->content.sym == ರಿಕ್ತ);
       l = l->next)
    ++res;
  ಪ್ರತ್ಯಾಗಮ res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

ಚಿಹ್ನೆ_list *
ಚಿಹ್ನೆ_list_n_get (ಚಿಹ್ನೆ_list *l, ಪೂರ್ಣಾಙ್ಕ n)
{
  aver (0 <= n);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_ಚಿಹ್ನೆ);
  aver (l->content.sym);
  ಪ್ರತ್ಯಾಗಮ l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
ಚಿಹ್ನೆ_list_n_type_name_get (ಚಿಹ್ನೆ_list *l, ಪೂರ್ಣಾಙ್ಕ n)
{
  ಪ್ರತ್ಯಾಗಮ ಚಿಹ್ನೆ_list_n_get (l, n)->content.sym->content->type_name;
}

bool
ಚಿಹ್ನೆ_list_null (ಚಿಹ್ನೆ_list *ಗಂಟು)
{
  ಪ್ರತ್ಯಾಗಮ (!ಗಂಟು
          || (ಗಂಟು->content_type == SYMLIST_ಚಿಹ್ನೆ && !ಗಂಟು->content.sym));
}

ಶೂನ್ಯ
ಚಿಹ್ನೆ_list_code_props_set (ಚಿಹ್ನೆ_list *ಗಂಟು, code_props_type kind,
                            code_props ಧ್ರುವ *cprops)
{
  ವಿಕಲ್ಪ (ಗಂಟು->content_type)
    {
    ಪಕ್ಷ SYMLIST_ಚಿಹ್ನೆ:
      ಚಿಹ್ನೆ_code_props_set (ಗಂಟು->content.sym, kind, cprops);
      ಯದಿ (ಗಂಟು->content.sym->content->status == undeclared)
        ಗಂಟು->content.sym->content->status = used;
      ಭಙ್ಗ;
    ಪಕ್ಷ SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (ಗಂಟು->content.sem_type->tag,
                            &ಗಂಟು->content.sem_type->location),
         kind, cprops);
      ಯದಿ (ಗಂಟು->content.sem_type->status == undeclared)
        ಗಂಟು->content.sem_type->status = used;
      ಭಙ್ಗ;
    }
}
