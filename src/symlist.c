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

प्रतीक_list *
प्रतीक_list_sym_new (प्रतीक *sym, location loc)
{
  प्रतीक_list *res = xmalloc (परिमाण *res);

  res->content_type = SYMLIST_प्रतीक;
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

  प्रत्यागम res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

प्रतीक_list *
प्रतीक_list_type_new (uniqstr type_name, location loc)
{
  प्रतीक_list *res = xmalloc (परिमाण *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (परिमाण (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = रिक्त;
  res->next = रिक्त;

  प्रत्यागम res;
}


प्रतीक_list *
प्रतीक_list_type_set (प्रतीक_list *syms, uniqstr type_name)
{
  प्रति (प्रतीक_list *l = syms; l; l = l->next)
    प्रतीक_type_set (l->content.sym, type_name, l->sym_loc);
  प्रत्यागम syms;
}


प्रतीक_list *
प्रतीक_list_find_प्रतीक (प्रतीक_list *l, ध्रुव प्रतीक *sym)
{
  प्रति (/* Nothing. */; l && l->content.sym; l = l->next)
    यदि (l->content.sym == sym)
      प्रत्यागम l;
  प्रत्यागम रिक्त;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

शून्य
प्रतीक_list_syms_print (ध्रुव प्रतीक_list *l, खाता *out)
{
  ख_अक्षर_दो ('[', out);
  वर्ण ध्रुव *sep = "";
  प्रति (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      ख_माला_दो (sep, out);
      ख_माला_दो (l->content_type == SYMLIST_प्रतीक ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      यदि (l->content_type == SYMLIST_प्रतीक)
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
  गाँठ->next = list;
  प्रत्यागम गाँठ;
}


प्रतीक_list *
प्रतीक_list_last (प्रतीक_list *list)
{
  यदि (!list)
    प्रत्यागम रिक्त;
  प्रतीक_list *next = list;
  यावत् (next->next)
    next = next->next;
  प्रत्यागम next;
}

प्रतीक_list *
प्रतीक_list_append (प्रतीक_list *list, प्रतीक_list *गाँठ)
{
  यदि (list)
    प्रतीक_list_last (list)->next = गाँठ;
  अन्यथा
    list = गाँठ;
  प्रत्यागम list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

शून्य
प्रतीक_list_free (प्रतीक_list *list)
{
  प्रति (प्रतीक_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      यदि (list->content_type == SYMLIST_TYPE)
        मुक्त (list->content.sem_type);
      मुक्त (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

पूर्णाङ्क
प्रतीक_list_length (प्रतीक_list ध्रुव *l)
{
  पूर्णाङ्क res = 0;
  प्रति (/* Nothing. */;
       l && !(l->content_type == SYMLIST_प्रतीक && l->content.sym == रिक्त);
       l = l->next)
    ++res;
  प्रत्यागम res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

प्रतीक_list *
प्रतीक_list_n_get (प्रतीक_list *l, पूर्णाङ्क n)
{
  aver (0 <= n);
  प्रति (पूर्णाङ्क i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_प्रतीक);
  aver (l->content.sym);
  प्रत्यागम l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
प्रतीक_list_n_type_name_get (प्रतीक_list *l, पूर्णाङ्क n)
{
  प्रत्यागम प्रतीक_list_n_get (l, n)->content.sym->content->type_name;
}

bool
प्रतीक_list_null (प्रतीक_list *गाँठ)
{
  प्रत्यागम (!गाँठ
          || (गाँठ->content_type == SYMLIST_प्रतीक && !गाँठ->content.sym));
}

शून्य
प्रतीक_list_code_props_set (प्रतीक_list *गाँठ, code_props_type kind,
                            code_props ध्रुव *cprops)
{
  विकल्प (गाँठ->content_type)
    {
    पक्ष SYMLIST_प्रतीक:
      प्रतीक_code_props_set (गाँठ->content.sym, kind, cprops);
      यदि (गाँठ->content.sym->content->status == undeclared)
        गाँठ->content.sym->content->status = used;
      भङ्ग;
    पक्ष SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (गाँठ->content.sem_type->tag,
                            &गाँठ->content.sem_type->location),
         kind, cprops);
      यदि (गाँठ->content.sem_type->status == undeclared)
        गाँठ->content.sem_type->status = used;
      भङ्ग;
    }
}
