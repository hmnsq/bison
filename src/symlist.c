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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

चिन्ह_list *
चिन्ह_list_sym_new (चिन्ह *sym, location loc)
{
  चिन्ह_list *res = xmalloc (आकार *res);

  res->content_type = SYMLIST_चिन्ह;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = रिक्त;

  res->midrule = रिक्त;
  res->midrule_parent_rule = रिक्त;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = रिक्त;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = रिक्त;

  परत res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

चिन्ह_list *
चिन्ह_list_type_new (uniqstr type_name, location loc)
{
  चिन्ह_list *res = xmalloc (आकार *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (आकार (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = रिक्त;
  res->next = रिक्त;

  परत res;
}


चिन्ह_list *
चिन्ह_list_type_set (चिन्ह_list *syms, uniqstr type_name)
{
  साठी (चिन्ह_list *l = syms; l; l = l->next)
    चिन्ह_type_set (l->content.sym, type_name, l->sym_loc);
  परत syms;
}


चिन्ह_list *
चिन्ह_list_find_चिन्ह (चिन्ह_list *l, स्थिरांक चिन्ह *sym)
{
  साठी (/* Nothing. */; l && l->content.sym; l = l->next)
    जर (l->content.sym == sym)
      परत l;
  परत रिक्त;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

शून्य
चिन्ह_list_syms_print (स्थिरांक चिन्ह_list *l, खाता *out)
{
  ख_अक्षर_दो ('[', out);
  अक्षर स्थिरांक *sep = "";
  साठी (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      ख_माला_दो (sep, out);
      ख_माला_दो (l->content_type == SYMLIST_चिन्ह ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      जर (l->content_type == SYMLIST_चिन्ह)
        चिन्ह_print (l->content.sym, out);
      ख_माला_दो (l->action_props.is_मूल्य_used ? " (used)" : " (unused)", out);
      अ_दो ('}', out);
      sep = ", ";
    }
  ख_अक्षर_दो (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

चिन्ह_list *
चिन्ह_list_prepend (चिन्ह_list *list, चिन्ह_list *गाठ)
{
  गाठ->next = list;
  परत गाठ;
}


चिन्ह_list *
चिन्ह_list_last (चिन्ह_list *list)
{
  जर (!list)
    परत रिक्त;
  चिन्ह_list *next = list;
  पर्यंत (next->next)
    next = next->next;
  परत next;
}

चिन्ह_list *
चिन्ह_list_append (चिन्ह_list *list, चिन्ह_list *गाठ)
{
  जर (list)
    चिन्ह_list_last (list)->next = गाठ;
  अन्यथा
    list = गाठ;
  परत list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

शून्य
चिन्ह_list_free (चिन्ह_list *list)
{
  साठी (चिन्ह_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      जर (list->content_type == SYMLIST_TYPE)
        मुक्त (list->content.sem_type);
      मुक्त (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

पूर्णांक
चिन्ह_list_length (चिन्ह_list स्थिरांक *l)
{
  पूर्णांक res = 0;
  साठी (/* Nothing. */;
       l && !(l->content_type == SYMLIST_चिन्ह && l->content.sym == रिक्त);
       l = l->next)
    ++res;
  परत res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

चिन्ह_list *
चिन्ह_list_n_get (चिन्ह_list *l, पूर्णांक n)
{
  aver (0 <= n);
  साठी (पूर्णांक i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_चिन्ह);
  aver (l->content.sym);
  परत l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
चिन्ह_list_n_type_name_get (चिन्ह_list *l, पूर्णांक n)
{
  परत चिन्ह_list_n_get (l, n)->content.sym->content->type_name;
}

bool
चिन्ह_list_null (चिन्ह_list *गाठ)
{
  परत (!गाठ
          || (गाठ->content_type == SYMLIST_चिन्ह && !गाठ->content.sym));
}

शून्य
चिन्ह_list_code_props_set (चिन्ह_list *गाठ, code_props_type kind,
                            code_props स्थिरांक *cprops)
{
  निवड (गाठ->content_type)
    {
    स्थिती SYMLIST_चिन्ह:
      चिन्ह_code_props_set (गाठ->content.sym, kind, cprops);
      जर (गाठ->content.sym->content->status == undeclared)
        गाठ->content.sym->content->status = used;
      थांबा;
    स्थिती SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (गाठ->content.sem_type->tag,
                            &गाठ->content.sem_type->location),
         kind, cprops);
      जर (गाठ->content.sem_type->status == undeclared)
        गाठ->content.sem_type->status = used;
      थांबा;
    }
}
