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

نښه_list *
نښه_list_sym_new (نښه *sym, location loc)
{
  نښه_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_نښه;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = عدم;

  res->midrule = عدم;
  res->midrule_پلار_قاعده = عدم;
  res->midrule_پلار_rhs_شاخص = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = تشه_loc;
  res->ruleprec = عدم;
  res->percent_تشه_loc = تشه_loc;
  کوډ_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = تشه_loc;
  res->merger = 0;
  res->merger_declaration_loc = تشه_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->بل = عدم;

  أرجع res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

نښه_list *
نښه_list_type_new (uniqstr type_name, location loc)
{
  نښه_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (حجم (semantic_type));
  res->content.sem_type->نښان = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->حالت = undeclared;

  res->sym_loc = loc;
  res->named_ref = عدم;
  res->بل = عدم;

  أرجع res;
}


نښه_list *
نښه_list_type_سېټ (نښه_list *syms, uniqstr type_name)
{
  لكل (نښه_list *l = syms; l; l = l->بل)
    نښه_type_سېټ (l->content.sym, type_name, l->sym_loc);
  أرجع syms;
}


نښه_list *
نښه_list_پیدا_کړه_نښه (نښه_list *l, ثابت نښه *sym)
{
  لكل (/* Nothing. */; l && l->content.sym; l = l->بل)
    إذا (l->content.sym == sym)
      أرجع l;
  أرجع عدم;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

فراغ
نښه_list_syms_print (ثابت نښه_list *l, FILE *بهر)
{
  fputc ('[', بهر);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->بل)
    {
      fputs (sep, بهر);
      fputs (l->content_type == SYMLIST_نښه ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             بهر);
      إذا (l->content_type == SYMLIST_نښه)
        نښه_print (l->content.sym, بهر);
      fputs (l->action_props.is_ارزښت_کارول_شوی ? " (used)" : " (unused)", بهر);
      putc ('}', بهر);
      sep = ", ";
    }
  fputc (']', بهر);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

نښه_list *
نښه_list_prepend (نښه_list *list, نښه_list *غوټه)
{
  غوټه->بل = list;
  أرجع غوټه;
}


نښه_list *
نښه_list_وروستی (نښه_list *list)
{
  إذا (!list)
    أرجع عدم;
  نښه_list *بل = list;
  طالما (بل->بل)
    بل = بل->بل;
  أرجع بل;
}

نښه_list *
نښه_list_append (نښه_list *list, نښه_list *غوټه)
{
  إذا (list)
    نښه_list_وروستی (list)->بل = غوټه;
  وإلا
    list = غوټه;
  أرجع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
نښه_list_free (نښه_list *list)
{
  لكل (نښه_list *بل; list; list = بل)
    {
      بل = list->بل;
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
نښه_list_اوږدتیا (نښه_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_نښه && l->content.sym == عدم);
       l = l->بل)
    ++res;
  أرجع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

نښه_list *
نښه_list_n_get (نښه_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->بل;
      aver (l);
    }
  aver (l->content_type == SYMLIST_نښه);
  aver (l->content.sym);
  أرجع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
نښه_list_n_type_name_get (نښه_list *l, صحيح n)
{
  أرجع نښه_list_n_get (l, n)->content.sym->content->type_name;
}

bool
نښه_list_null (نښه_list *غوټه)
{
  أرجع (!غوټه
          || (غوټه->content_type == SYMLIST_نښه && !غوټه->content.sym));
}

فراغ
نښه_list_کوډ_props_سېټ (نښه_list *غوټه, کوډ_props_type ډول,
                            کوډ_props ثابت *cprops)
{
  بدل (غوټه->content_type)
    {
    حالة SYMLIST_نښه:
      نښه_کوډ_props_سېټ (غوټه->content.sym, ډول, cprops);
      إذا (غوټه->content.sym->content->حالت == undeclared)
        غوټه->content.sym->content->حالت = کارول_شوی;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_کوډ_props_سېټ
        (semantic_type_get (غوټه->content.sem_type->نښان,
                            &غوټه->content.sem_type->location),
         ډول, cprops);
      إذا (غوټه->content.sem_type->حالت == undeclared)
        غوټه->content.sem_type->حالت = کارول_شوی;
      اكسر;
    }
}
