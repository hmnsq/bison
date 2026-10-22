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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

प्रतीक_list *
प्रतीक_list_sym_new (प्रतीक *sym, location loc)
{
  प्रतीक_list *res = xmalloc (माप *res);

  res->content_type = SYMLIST_प्रतीक;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = शून्य;

  res->midrule = शून्य;
  res->midrule_parent_rule = शून्य;
  res->midrule_parent_rhs_सूचकांक = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = शून्य;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->अग्र = शून्य;

  वापस res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

प्रतीक_list *
प्रतीक_list_type_new (uniqstr type_name, location loc)
{
  प्रतीक_list *res = xmalloc (माप *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (माप (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = शून्य;
  res->अग्र = शून्य;

  वापस res;
}


प्रतीक_list *
प्रतीक_list_type_set (प्रतीक_list *syms, uniqstr type_name)
{
  क्रम (प्रतीक_list *l = syms; l; l = l->अग्र)
    प्रतीक_type_set (l->content.sym, type_name, l->sym_loc);
  वापस syms;
}


प्रतीक_list *
प्रतीक_list_find_प्रतीक (प्रतीक_list *l, स्थिर प्रतीक *sym)
{
  क्रम (/* Nothing. */; l && l->content.sym; l = l->अग्र)
    अगर (l->content.sym == sym)
      वापस l;
  वापस शून्य;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

व्योम
प्रतीक_list_syms_print (स्थिर प्रतीक_list *l, खाता *out)
{
  ख_अक्षर_दो ('[', out);
  अक्षर स्थिर *sep = "";
  क्रम (/* Nothing. */; l && l->content.sym; l = l->अग्र)
    {
      ख_माला_दो (sep, out);
      ख_माला_दो (l->content_type == SYMLIST_प्रतीक ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      अगर (l->content_type == SYMLIST_प्रतीक)
        प्रतीक_print (l->content.sym, out);
      ख_माला_दो (l->action_props.is_मूल्य_used ? " (used)" : " (unused)", out);
      अ_दो ('}', out);
      sep = ", ";
    }
  ख_अक्षर_दो (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

प्रतीक_list *
प्रतीक_list_prepend (प्रतीक_list *list, प्रतीक_list *गाँठ)
{
  गाँठ->अग्र = list;
  वापस गाँठ;
}


प्रतीक_list *
प्रतीक_list_last (प्रतीक_list *list)
{
  अगर (!list)
    वापस शून्य;
  प्रतीक_list *अग्र = list;
  जबतक (अग्र->अग्र)
    अग्र = अग्र->अग्र;
  वापस अग्र;
}

प्रतीक_list *
प्रतीक_list_append (प्रतीक_list *list, प्रतीक_list *गाँठ)
{
  अगर (list)
    प्रतीक_list_last (list)->अग्र = गाँठ;
  अन्यथा
    list = गाँठ;
  वापस list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

व्योम
प्रतीक_list_free (प्रतीक_list *list)
{
  क्रम (प्रतीक_list *अग्र; list; list = अग्र)
    {
      अग्र = list->अग्र;
      named_ref_free (list->named_ref);
      अगर (list->content_type == SYMLIST_TYPE)
        मुक्त (list->content.sem_type);
      मुक्त (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

पूर्णांक
प्रतीक_list_लंबाई (प्रतीक_list स्थिर *l)
{
  पूर्णांक res = 0;
  क्रम (/* Nothing. */;
       l && !(l->content_type == SYMLIST_प्रतीक && l->content.sym == शून्य);
       l = l->अग्र)
    ++res;
  वापस res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

प्रतीक_list *
प्रतीक_list_n_get (प्रतीक_list *l, पूर्णांक n)
{
  aver (0 <= n);
  क्रम (पूर्णांक i = 0; i < n; ++i)
    {
      l = l->अग्र;
      aver (l);
    }
  aver (l->content_type == SYMLIST_प्रतीक);
  aver (l->content.sym);
  वापस l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
प्रतीक_list_n_type_name_get (प्रतीक_list *l, पूर्णांक n)
{
  वापस प्रतीक_list_n_get (l, n)->content.sym->content->type_name;
}

bool
प्रतीक_list_null (प्रतीक_list *गाँठ)
{
  वापस (!गाँठ
          || (गाँठ->content_type == SYMLIST_प्रतीक && !गाँठ->content.sym));
}

व्योम
प्रतीक_list_code_props_set (प्रतीक_list *गाँठ, code_props_type kind,
                            code_props स्थिर *cprops)
{
  चयन (गाँठ->content_type)
    {
    हाल SYMLIST_प्रतीक:
      प्रतीक_code_props_set (गाँठ->content.sym, kind, cprops);
      अगर (गाँठ->content.sym->content->status == undeclared)
        गाँठ->content.sym->content->status = used;
      अवरोध;
    हाल SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (गाँठ->content.sem_type->tag,
                            &गाँठ->content.sem_type->location),
         kind, cprops);
      अगर (गाँठ->content.sem_type->status == undeclared)
        गाँठ->content.sem_type->status = used;
      अवरोध;
    }
}
