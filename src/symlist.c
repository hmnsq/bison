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

رمز_list *
رمز_list_sym_new (رمز *sym, location loc)
{
  رمز_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_رمز;
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

  إرجاع res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

رمز_list *
رمز_list_type_new (uniqstr type_name, location loc)
{
  رمز_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (حجم (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = عدم;
  res->next = عدم;

  إرجاع res;
}


رمز_list *
رمز_list_type_set (رمز_list *syms, uniqstr type_name)
{
  لكل (رمز_list *l = syms; l; l = l->next)
    رمز_type_set (l->content.sym, type_name, l->sym_loc);
  إرجاع syms;
}


رمز_list *
رمز_list_find_رمز (رمز_list *l, ثابت رمز *sym)
{
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    إذا (l->content.sym == sym)
      إرجاع l;
  إرجاع عدم;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

فراغ
رمز_list_syms_print (ثابت رمز_list *l, FILE *out)
{
  fputc ('[', out);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_رمز ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      إذا (l->content_type == SYMLIST_رمز)
        رمز_print (l->content.sym, out);
      fputs (l->action_props.is_قيمة_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

رمز_list *
رمز_list_prepend (رمز_list *list, رمز_list *عقدة)
{
  عقدة->next = list;
  إرجاع عقدة;
}


رمز_list *
رمز_list_last (رمز_list *list)
{
  إذا (!list)
    إرجاع عدم;
  رمز_list *next = list;
  بينما (next->next)
    next = next->next;
  إرجاع next;
}

رمز_list *
رمز_list_append (رمز_list *list, رمز_list *عقدة)
{
  إذا (list)
    رمز_list_last (list)->next = عقدة;
  وإلا
    list = عقدة;
  إرجاع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
رمز_list_free (رمز_list *list)
{
  لكل (رمز_list *next; list; list = next)
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
رمز_list_length (رمز_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_رمز && l->content.sym == عدم);
       l = l->next)
    ++res;
  إرجاع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

رمز_list *
رمز_list_n_get (رمز_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_رمز);
  aver (l->content.sym);
  إرجاع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
رمز_list_n_type_name_get (رمز_list *l, صحيح n)
{
  إرجاع رمز_list_n_get (l, n)->content.sym->content->type_name;
}

bool
رمز_list_null (رمز_list *عقدة)
{
  إرجاع (!عقدة
          || (عقدة->content_type == SYMLIST_رمز && !عقدة->content.sym));
}

فراغ
رمز_list_code_props_set (رمز_list *عقدة, code_props_type kind,
                            code_props ثابت *cprops)
{
  بدل (عقدة->content_type)
    {
    حالة SYMLIST_رمز:
      رمز_code_props_set (عقدة->content.sym, kind, cprops);
      إذا (عقدة->content.sym->content->status == undeclared)
        عقدة->content.sym->content->status = used;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (عقدة->content.sem_type->tag,
                            &عقدة->content.sem_type->location),
         kind, cprops);
      إذا (عقدة->content.sem_type->status == undeclared)
        عقدة->content.sem_type->status = used;
      اكسر;
    }
}
