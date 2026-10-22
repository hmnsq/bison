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

#شامل <config.h>
#شامل "system.h"

#شامل "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

نماد_list *
نماد_list_sym_new (نماد *sym, location loc)
{
  نماد_list *res = xmalloc (اندازه *res);

  res->content_type = SYMLIST_نماد;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = پوچ;

  res->midrule = پوچ;
  res->midrule_parent_rule = پوچ;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = پوچ;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = پوچ;

  بازگشت res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

نماد_list *
نماد_list_type_new (uniqstr type_name, location loc)
{
  نماد_list *res = xmalloc (اندازه *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (اندازه (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = پوچ;
  res->next = پوچ;

  بازگشت res;
}


نماد_list *
نماد_list_type_set (نماد_list *syms, uniqstr type_name)
{
  برای (نماد_list *l = syms; l; l = l->next)
    نماد_type_set (l->content.sym, type_name, l->sym_loc);
  بازگشت syms;
}


نماد_list *
نماد_list_find_نماد (نماد_list *l, ثابت نماد *sym)
{
  برای (/* Nothing. */; l && l->content.sym; l = l->next)
    اگر (l->content.sym == sym)
      بازگشت l;
  بازگشت پوچ;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

تهی
نماد_list_syms_print (ثابت نماد_list *l, FILE *out)
{
  fputc ('[', out);
  نویسه ثابت *sep = "";
  برای (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_نماد ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      اگر (l->content_type == SYMLIST_نماد)
        نماد_print (l->content.sym, out);
      fputs (l->action_props.is_مقدار_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

نماد_list *
نماد_list_prepend (نماد_list *list, نماد_list *گره)
{
  گره->next = list;
  بازگشت گره;
}


نماد_list *
نماد_list_last (نماد_list *list)
{
  اگر (!list)
    بازگشت پوچ;
  نماد_list *next = list;
  وقتی (next->next)
    next = next->next;
  بازگشت next;
}

نماد_list *
نماد_list_append (نماد_list *list, نماد_list *گره)
{
  اگر (list)
    نماد_list_last (list)->next = گره;
  وگرنه
    list = گره;
  بازگشت list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

تهی
نماد_list_free (نماد_list *list)
{
  برای (نماد_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      اگر (list->content_type == SYMLIST_TYPE)
        آزاد (list->content.sem_type);
      آزاد (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

صحیح
نماد_list_length (نماد_list ثابت *l)
{
  صحیح res = 0;
  برای (/* Nothing. */;
       l && !(l->content_type == SYMLIST_نماد && l->content.sym == پوچ);
       l = l->next)
    ++res;
  بازگشت res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

نماد_list *
نماد_list_n_get (نماد_list *l, صحیح n)
{
  aver (0 <= n);
  برای (صحیح i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_نماد);
  aver (l->content.sym);
  بازگشت l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
نماد_list_n_type_name_get (نماد_list *l, صحیح n)
{
  بازگشت نماد_list_n_get (l, n)->content.sym->content->type_name;
}

bool
نماد_list_null (نماد_list *گره)
{
  بازگشت (!گره
          || (گره->content_type == SYMLIST_نماد && !گره->content.sym));
}

تهی
نماد_list_code_props_set (نماد_list *گره, code_props_type kind,
                            code_props ثابت *cprops)
{
  انتخاب (گره->content_type)
    {
    حالت SYMLIST_نماد:
      نماد_code_props_set (گره->content.sym, kind, cprops);
      اگر (گره->content.sym->content->status == undeclared)
        گره->content.sym->content->status = used;
      بشکن;
    حالت SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (گره->content.sem_type->tag,
                            &گره->content.sem_type->location),
         kind, cprops);
      اگر (گره->content.sem_type->status == undeclared)
        گره->content.sem_type->status = used;
      بشکن;
    }
}
