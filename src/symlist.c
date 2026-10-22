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

#ضمن <config.h>
#ضمن "system.h"

#ضمن "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

علام_list *
علام_list_sym_new (علام *sym, location loc)
{
  علام_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_علام;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = عدم;

  res->midrule = عدم;
  res->midrule_parent_rule = عدم;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = عدم;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = عدم;

  أرجع res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

علام_list *
علام_list_type_new (uniqstr type_name, location loc)
{
  علام_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (حجم (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = عدم;
  res->next = عدم;

  أرجع res;
}


علام_list *
علام_list_type_set (علام_list *syms, uniqstr type_name)
{
  لكل (علام_list *l = syms; l; l = l->next)
    علام_type_set (l->content.sym, type_name, l->sym_loc);
  أرجع syms;
}


علام_list *
علام_list_find_علام (علام_list *l, ثابت علام *sym)
{
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    إذا (l->content.sym == sym)
      أرجع l;
  أرجع عدم;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

فراغ
علام_list_syms_بغ (ثابت علام_list *l, FILE *out)
{
  fputc ('[', out);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_علام ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      إذا (l->content_type == SYMLIST_علام)
        علام_بغ (l->content.sym, out);
      fputs (l->action_props.is_درج_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

علام_list *
علام_list_prepend (علام_list *list, علام_list *كلي)
{
  كلي->next = list;
  أرجع كلي;
}


علام_list *
علام_list_last (علام_list *list)
{
  إذا (!list)
    أرجع عدم;
  علام_list *next = list;
  طالما (next->next)
    next = next->next;
  أرجع next;
}

علام_list *
علام_list_append (علام_list *list, علام_list *كلي)
{
  إذا (list)
    علام_list_last (list)->next = كلي;
  وإلا
    list = كلي;
  أرجع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
علام_list_free (علام_list *list)
{
  لكل (علام_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      إذا (list->content_type == SYMLIST_TYPE)
        حرر (list->content.sem_type);
      حرر (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

صحيح
علام_list_length (علام_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_علام && l->content.sym == عدم);
       l = l->next)
    ++res;
  أرجع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

علام_list *
علام_list_n_get (علام_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_علام);
  aver (l->content.sym);
  أرجع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
علام_list_n_type_name_get (علام_list *l, صحيح n)
{
  أرجع علام_list_n_get (l, n)->content.sym->content->type_name;
}

bool
علام_list_null (علام_list *كلي)
{
  أرجع (!كلي
          || (كلي->content_type == SYMLIST_علام && !كلي->content.sym));
}

فراغ
علام_list_code_props_set (علام_list *كلي, code_props_type kind,
                            code_props ثابت *cprops)
{
  بدل (كلي->content_type)
    {
    حالة SYMLIST_علام:
      علام_code_props_set (كلي->content.sym, kind, cprops);
      إذا (كلي->content.sym->content->status == undeclared)
        كلي->content.sym->content->status = used;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (كلي->content.sem_type->tag,
                            &كلي->content.sem_type->location),
         kind, cprops);
      إذا (كلي->content.sem_type->status == undeclared)
        كلي->content.sem_type->status = used;
      اكسر;
    }
}
