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

بەلگە_list *
بەلگە_list_sym_new (بەلگە *sym, location loc)
{
  بەلگە_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_بەلگە;
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

بەلگە_list *
بەلگە_list_type_new (uniqstr type_name, location loc)
{
  بەلگە_list *res = xmalloc (حجم *res);

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


بەلگە_list *
بەلگە_list_type_set (بەلگە_list *syms, uniqstr type_name)
{
  لكل (بەلگە_list *l = syms; l; l = l->next)
    بەلگە_type_set (l->content.sym, type_name, l->sym_loc);
  أرجع syms;
}


بەلگە_list *
بەلگە_list_find_بەلگە (بەلگە_list *l, ثابت بەلگە *sym)
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
بەلگە_list_syms_print (ثابت بەلگە_list *l, FILE *out)
{
  fputc ('[', out);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_بەلگە ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      إذا (l->content_type == SYMLIST_بەلگە)
        بەلگە_print (l->content.sym, out);
      fputs (l->action_props.is_قىممەت_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

بەلگە_list *
بەلگە_list_prepend (بەلگە_list *list, بەلگە_list *تۈگۈن)
{
  تۈگۈن->next = list;
  أرجع تۈگۈن;
}


بەلگە_list *
بەلگە_list_last (بەلگە_list *list)
{
  إذا (!list)
    أرجع عدم;
  بەلگە_list *next = list;
  طالما (next->next)
    next = next->next;
  أرجع next;
}

بەلگە_list *
بەلگە_list_append (بەلگە_list *list, بەلگە_list *تۈگۈن)
{
  إذا (list)
    بەلگە_list_last (list)->next = تۈگۈن;
  وإلا
    list = تۈگۈن;
  أرجع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
بەلگە_list_free (بەلگە_list *list)
{
  لكل (بەلگە_list *next; list; list = next)
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
بەلگە_list_length (بەلگە_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_بەلگە && l->content.sym == عدم);
       l = l->next)
    ++res;
  أرجع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

بەلگە_list *
بەلگە_list_n_get (بەلگە_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_بەلگە);
  aver (l->content.sym);
  أرجع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
بەلگە_list_n_type_name_get (بەلگە_list *l, صحيح n)
{
  أرجع بەلگە_list_n_get (l, n)->content.sym->content->type_name;
}

bool
بەلگە_list_null (بەلگە_list *تۈگۈن)
{
  أرجع (!تۈگۈن
          || (تۈگۈن->content_type == SYMLIST_بەلگە && !تۈگۈن->content.sym));
}

فراغ
بەلگە_list_code_props_set (بەلگە_list *تۈگۈن, code_props_type kind,
                            code_props ثابت *cprops)
{
  بدل (تۈگۈن->content_type)
    {
    حالة SYMLIST_بەلگە:
      بەلگە_code_props_set (تۈگۈن->content.sym, kind, cprops);
      إذا (تۈگۈن->content.sym->content->status == undeclared)
        تۈگۈن->content.sym->content->status = used;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (تۈگۈن->content.sem_type->tag,
                            &تۈگۈن->content.sem_type->location),
         kind, cprops);
      إذا (تۈگۈن->content.sem_type->status == undeclared)
        تۈگۈن->content.sem_type->status = used;
      اكسر;
    }
}
