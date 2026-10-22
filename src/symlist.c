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

نشان_list *
نشان_list_sym_new (نشان *sym, location loc)
{
  نشان_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_نشان;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = عدم;

  res->midrule = عدم;
  res->midrule_والد_قاعدو = عدم;
  res->midrule_والد_rhs_اشاريو = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = خالي_جايون_loc;
  res->ruleprec = عدم;
  res->percent_خالي_جايون_loc = خالي_جايون_loc;
  ڪوڊ_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = خالي_جايون_loc;
  res->merger = 0;
  res->merger_declaration_loc = خالي_جايون_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->ايندڙ = عدم;

  أرجع res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

نشان_list *
نشان_list_type_new (uniqstr type_name, location loc)
{
  نشان_list *res = xmalloc (حجم *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (حجم (semantic_type));
  res->content.sem_type->ٽيگ = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->حالت = undeclared;

  res->sym_loc = loc;
  res->named_ref = عدم;
  res->ايندڙ = عدم;

  أرجع res;
}


نشان_list *
نشان_list_type_سيٽ (نشان_list *syms, uniqstr type_name)
{
  لكل (نشان_list *l = syms; l; l = l->ايندڙ)
    نشان_type_سيٽ (l->content.sym, type_name, l->sym_loc);
  أرجع syms;
}


نشان_list *
نشان_list_ڳوليو_نشان (نشان_list *l, ثابت نشان *sym)
{
  لكل (/* Nothing. */; l && l->content.sym; l = l->ايندڙ)
    إذا (l->content.sym == sym)
      أرجع l;
  أرجع عدم;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

فراغ
نشان_list_syms_print (ثابت نشان_list *l, FILE *ٻاهريون)
{
  fputc ('[', ٻاهريون);
  حرف ثابت *sep = "";
  لكل (/* Nothing. */; l && l->content.sym; l = l->ايندڙ)
    {
      fputs (sep, ٻاهريون);
      fputs (l->content_type == SYMLIST_نشان ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             ٻاهريون);
      إذا (l->content_type == SYMLIST_نشان)
        نشان_print (l->content.sym, ٻاهريون);
      fputs (l->action_props.is_قدر_استعمال_ٿيل ? " (used)" : " (unused)", ٻاهريون);
      putc ('}', ٻاهريون);
      sep = ", ";
    }
  fputc (']', ٻاهريون);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

نشان_list *
نشان_list_prepend (نشان_list *list, نشان_list *ڳنڍ)
{
  ڳنڍ->ايندڙ = list;
  أرجع ڳنڍ;
}


نشان_list *
نشان_list_آخري (نشان_list *list)
{
  إذا (!list)
    أرجع عدم;
  نشان_list *ايندڙ = list;
  طالما (ايندڙ->ايندڙ)
    ايندڙ = ايندڙ->ايندڙ;
  أرجع ايندڙ;
}

نشان_list *
نشان_list_append (نشان_list *list, نشان_list *ڳنڍ)
{
  إذا (list)
    نشان_list_آخري (list)->ايندڙ = ڳنڍ;
  وإلا
    list = ڳنڍ;
  أرجع list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

فراغ
نشان_list_free (نشان_list *list)
{
  لكل (نشان_list *ايندڙ; list; list = ايندڙ)
    {
      ايندڙ = list->ايندڙ;
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
نشان_list_ڊيگهائي (نشان_list ثابت *l)
{
  صحيح res = 0;
  لكل (/* Nothing. */;
       l && !(l->content_type == SYMLIST_نشان && l->content.sym == عدم);
       l = l->ايندڙ)
    ++res;
  أرجع res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

نشان_list *
نشان_list_n_get (نشان_list *l, صحيح n)
{
  aver (0 <= n);
  لكل (صحيح i = 0; i < n; ++i)
    {
      l = l->ايندڙ;
      aver (l);
    }
  aver (l->content_type == SYMLIST_نشان);
  aver (l->content.sym);
  أرجع l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
نشان_list_n_type_name_get (نشان_list *l, صحيح n)
{
  أرجع نشان_list_n_get (l, n)->content.sym->content->type_name;
}

bool
نشان_list_null (نشان_list *ڳنڍ)
{
  أرجع (!ڳنڍ
          || (ڳنڍ->content_type == SYMLIST_نشان && !ڳنڍ->content.sym));
}

فراغ
نشان_list_ڪوڊ_props_سيٽ (نشان_list *ڳنڍ, ڪوڊ_props_type قسم,
                            ڪوڊ_props ثابت *cprops)
{
  بدل (ڳنڍ->content_type)
    {
    حالة SYMLIST_نشان:
      نشان_ڪوڊ_props_سيٽ (ڳنڍ->content.sym, قسم, cprops);
      إذا (ڳنڍ->content.sym->content->حالت == undeclared)
        ڳنڍ->content.sym->content->حالت = استعمال_ٿيل;
      اكسر;
    حالة SYMLIST_TYPE:
      semantic_type_ڪوڊ_props_سيٽ
        (semantic_type_get (ڳنڍ->content.sem_type->ٽيگ,
                            &ڳنڍ->content.sem_type->location),
         قسم, cprops);
      إذا (ڳنڍ->content.sem_type->حالت == undeclared)
        ڳنڍ->content.sem_type->حالت = استعمال_ٿيل;
      اكسر;
    }
}
