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

علامت_list *
علامت_list_sym_new (علامت *sym, location loc)
{
  علامت_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_علامت;
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

علامت_list *
علامت_list_type_new (uniqstr type_name, location loc)
{
  علامت_list *res = xmalloc (حجم *res);

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


علامت_list *
علامت_list_type_set (علامت_list *syms, uniqstr type_name)
{
  لكل (علامت_list *l = syms; l; l = l->next)
    علامت_type_set (l->content.sym, type_name, l->sym_loc);
  أرجع syms;
}


علامت_list *
علامت_list_find_علامت (علامت_list *l, ثابت علامت *sym)
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
علامت_list_syms_print (ثابت علامت_list *l, FILE *out)
{
  fputc ('[', out);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_علامت ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      إذا (l->content_type == SYMLIST_علامت)
        علامت_print (l->content.sym, out);
      fputs (l->action_props.is_قدر_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

علامت_list *
علامت_list_prepend (علامت_list *list, علامت_list *گرہ)
{
  گرہ->next = list;
  أرجع گرہ;
}


علامت_list *
علامت_list_last (علامت_list *list)
{
  إذا (!list)
    أرجع عدم;
  علامت_list *next = list;
  طالما (next->next)
    next = next->next;
  أرجع next;
}

علامت_list *
علامت_list_append (علامت_list *list, علامت_list *گرہ)
{
  إذا (list)
    علامت_list_last (list)->next = گرہ;
  وإلا
    list = گرہ;
  أرجع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
علامت_list_free (علامت_list *list)
{
  لكل (علامت_list *next; list; list = next)
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
علامت_list_length (علامت_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_علامت && l->content.sym == عدم);
       l = l->next)
    ++res;
  أرجع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

علامت_list *
علامت_list_n_get (علامت_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_علامت);
  aver (l->content.sym);
  أرجع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
علامت_list_n_type_name_get (علامت_list *l, صحيح n)
{
  أرجع علامت_list_n_get (l, n)->content.sym->content->type_name;
}

bool
علامت_list_null (علامت_list *گرہ)
{
  أرجع (!گرہ
          || (گرہ->content_type == SYMLIST_علامت && !گرہ->content.sym));
}

فراغ
علامت_list_code_props_set (علامت_list *گرہ, code_props_type kind,
                            code_props ثابت *cprops)
{
  بدل (گرہ->content_type)
    {
    حالة SYMLIST_علامت:
      علامت_code_props_set (گرہ->content.sym, kind, cprops);
      إذا (گرہ->content.sym->content->status == undeclared)
        گرہ->content.sym->content->status = used;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (گرہ->content.sem_type->tag,
                            &گرہ->content.sem_type->location),
         kind, cprops);
      إذا (گرہ->content.sem_type->status == undeclared)
        گرہ->content.sem_type->status = used;
      اكسر;
    }
}
