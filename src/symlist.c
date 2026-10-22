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

سيمبول_list *
سيمبول_list_sym_new (سيمبول *sym, location loc)
{
  سيمبول_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_سيمبول;
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

سيمبول_list *
سيمبول_list_type_new (uniqstr type_name, location loc)
{
  سيمبول_list *res = xmalloc (حجم *res);

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


سيمبول_list *
سيمبول_list_type_set (سيمبول_list *syms, uniqstr type_name)
{
  لكل (سيمبول_list *l = syms; l; l = l->next)
    سيمبول_type_set (l->content.sym, type_name, l->sym_loc);
  أرجع syms;
}


سيمبول_list *
سيمبول_list_find_سيمبول (سيمبول_list *l, ثابت سيمبول *sym)
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
سيمبول_list_syms_print (ثابت سيمبول_list *l, FILE *out)
{
  fputc ('[', out);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_سيمبول ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      إذا (l->content_type == SYMLIST_سيمبول)
        سيمبول_print (l->content.sym, out);
      fputs (l->action_props.is_نيلاي_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

سيمبول_list *
سيمبول_list_prepend (سيمبول_list *list, سيمبول_list *نود)
{
  نود->next = list;
  أرجع نود;
}


سيمبول_list *
سيمبول_list_last (سيمبول_list *list)
{
  إذا (!list)
    أرجع عدم;
  سيمبول_list *next = list;
  طالما (next->next)
    next = next->next;
  أرجع next;
}

سيمبول_list *
سيمبول_list_append (سيمبول_list *list, سيمبول_list *نود)
{
  إذا (list)
    سيمبول_list_last (list)->next = نود;
  وإلا
    list = نود;
  أرجع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
سيمبول_list_free (سيمبول_list *list)
{
  لكل (سيمبول_list *next; list; list = next)
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
سيمبول_list_length (سيمبول_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_سيمبول && l->content.sym == عدم);
       l = l->next)
    ++res;
  أرجع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

سيمبول_list *
سيمبول_list_n_get (سيمبول_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_سيمبول);
  aver (l->content.sym);
  أرجع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
سيمبول_list_n_type_name_get (سيمبول_list *l, صحيح n)
{
  أرجع سيمبول_list_n_get (l, n)->content.sym->content->type_name;
}

bool
سيمبول_list_null (سيمبول_list *نود)
{
  أرجع (!نود
          || (نود->content_type == SYMLIST_سيمبول && !نود->content.sym));
}

فراغ
سيمبول_list_code_props_set (سيمبول_list *نود, code_props_type kind,
                            code_props ثابت *cprops)
{
  بدل (نود->content_type)
    {
    حالة SYMLIST_سيمبول:
      سيمبول_code_props_set (نود->content.sym, kind, cprops);
      إذا (نود->content.sym->content->status == undeclared)
        نود->content.sym->content->status = used;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (نود->content.sem_type->tag,
                            &نود->content.sem_type->location),
         kind, cprops);
      إذا (نود->content.sem_type->status == undeclared)
        نود->content.sem_type->status = used;
      اكسر;
    }
}
