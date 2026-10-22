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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

প্রতীক_list *
প্রতীক_list_sym_new (প্রতীক *sym, location loc)
{
  প্রতীক_list *res = xmalloc (আকার *res);

  res->content_type = SYMLIST_প্রতীক;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = ফাঁকা;

  res->midrule = ফাঁকা;
  res->midrule_parent_rule = ফাঁকা;
  res->midrule_parent_rhs_সূচক = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = ফাঁকা;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->পরবর্তী = ফাঁকা;

  ফেরত res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

প্রতীক_list *
প্রতীক_list_type_new (uniqstr type_name, location loc)
{
  প্রতীক_list *res = xmalloc (আকার *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (আকার (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = ফাঁকা;
  res->পরবর্তী = ফাঁকা;

  ফেরত res;
}


প্রতীক_list *
প্রতীক_list_type_set (প্রতীক_list *syms, uniqstr type_name)
{
  জন্য (প্রতীক_list *l = syms; l; l = l->পরবর্তী)
    প্রতীক_type_set (l->content.sym, type_name, l->sym_loc);
  ফেরত syms;
}


প্রতীক_list *
প্রতীক_list_find_প্রতীক (প্রতীক_list *l, ধ্রুবক প্রতীক *sym)
{
  জন্য (/* Nothing. */; l && l->content.sym; l = l->পরবর্তী)
    যদি (l->content.sym == sym)
      ফেরত l;
  ফেরত ফাঁকা;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

শূন্য
প্রতীক_list_syms_print (ধ্রুবক প্রতীক_list *l, FILE *out)
{
  fputc ('[', out);
  অক্ষর ধ্রুবক *sep = "";
  জন্য (/* Nothing. */; l && l->content.sym; l = l->পরবর্তী)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_প্রতীক ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      যদি (l->content_type == SYMLIST_প্রতীক)
        প্রতীক_print (l->content.sym, out);
      fputs (l->action_props.is_মান_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

প্রতীক_list *
প্রতীক_list_prepend (প্রতীক_list *list, প্রতীক_list *গ্রন্থি)
{
  গ্রন্থি->পরবর্তী = list;
  ফেরত গ্রন্থি;
}


প্রতীক_list *
প্রতীক_list_last (প্রতীক_list *list)
{
  যদি (!list)
    ফেরত ফাঁকা;
  প্রতীক_list *পরবর্তী = list;
  যখন (পরবর্তী->পরবর্তী)
    পরবর্তী = পরবর্তী->পরবর্তী;
  ফেরত পরবর্তী;
}

প্রতীক_list *
প্রতীক_list_append (প্রতীক_list *list, প্রতীক_list *গ্রন্থি)
{
  যদি (list)
    প্রতীক_list_last (list)->পরবর্তী = গ্রন্থি;
  অন্যথা
    list = গ্রন্থি;
  ফেরত list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

শূন্য
প্রতীক_list_free (প্রতীক_list *list)
{
  জন্য (প্রতীক_list *পরবর্তী; list; list = পরবর্তী)
    {
      পরবর্তী = list->পরবর্তী;
      named_ref_free (list->named_ref);
      যদি (list->content_type == SYMLIST_TYPE)
        মুক্ত (list->content.sem_type);
      মুক্ত (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

পূর্ণসংখ্যা
প্রতীক_list_দৈর্ঘ্য (প্রতীক_list ধ্রুবক *l)
{
  পূর্ণসংখ্যা res = 0;
  জন্য (/* Nothing. */;
       l && !(l->content_type == SYMLIST_প্রতীক && l->content.sym == ফাঁকা);
       l = l->পরবর্তী)
    ++res;
  ফেরত res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

প্রতীক_list *
প্রতীক_list_n_get (প্রতীক_list *l, পূর্ণসংখ্যা n)
{
  aver (0 <= n);
  জন্য (পূর্ণসংখ্যা i = 0; i < n; ++i)
    {
      l = l->পরবর্তী;
      aver (l);
    }
  aver (l->content_type == SYMLIST_প্রতীক);
  aver (l->content.sym);
  ফেরত l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
প্রতীক_list_n_type_name_get (প্রতীক_list *l, পূর্ণসংখ্যা n)
{
  ফেরত প্রতীক_list_n_get (l, n)->content.sym->content->type_name;
}

bool
প্রতীক_list_null (প্রতীক_list *গ্রন্থি)
{
  ফেরত (!গ্রন্থি
          || (গ্রন্থি->content_type == SYMLIST_প্রতীক && !গ্রন্থি->content.sym));
}

শূন্য
প্রতীক_list_code_props_set (প্রতীক_list *গ্রন্থি, code_props_type kind,
                            code_props ধ্রুবক *cprops)
{
  নির্বাচন (গ্রন্থি->content_type)
    {
    ক্ষেত্র SYMLIST_প্রতীক:
      প্রতীক_code_props_set (গ্রন্থি->content.sym, kind, cprops);
      যদি (গ্রন্থি->content.sym->content->status == undeclared)
        গ্রন্থি->content.sym->content->status = used;
      বিরতি;
    ক্ষেত্র SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (গ্রন্থি->content.sem_type->tag,
                            &গ্রন্থি->content.sem_type->location),
         kind, cprops);
      যদি (গ্রন্থি->content.sem_type->status == undeclared)
        গ্রন্থি->content.sem_type->status = used;
      বিরতি;
    }
}
