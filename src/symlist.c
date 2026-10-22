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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

প্ৰতীক_list *
প্ৰতীক_list_sym_new (প্ৰতীক *sym, location loc)
{
  প্ৰতীক_list *res = xmalloc (পরিমাণ *res);

  res->content_type = SYMLIST_প্ৰতীক;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = রিক্ত;

  res->midrule = রিক্ত;
  res->midrule_parent_rule = রিক্ত;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = রিক্ত;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = রিক্ত;

  ঘূৰাই res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

প্ৰতীক_list *
প্ৰতীক_list_type_new (uniqstr type_name, location loc)
{
  প্ৰতীক_list *res = xmalloc (পরিমাণ *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (পরিমাণ (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = রিক্ত;
  res->next = রিক্ত;

  ঘূৰাই res;
}


প্ৰতীক_list *
প্ৰতীক_list_type_set (প্ৰতীক_list *syms, uniqstr type_name)
{
  বাবে (প্ৰতীক_list *l = syms; l; l = l->next)
    প্ৰতীক_type_set (l->content.sym, type_name, l->sym_loc);
  ঘূৰাই syms;
}


প্ৰতীক_list *
প্ৰতীক_list_find_প্ৰতীক (প্ৰতীক_list *l, ধ্রুৱ প্ৰতীক *sym)
{
  বাবে (/* Nothing. */; l && l->content.sym; l = l->next)
    যদি (l->content.sym == sym)
      ঘূৰাই l;
  ঘূৰাই রিক্ত;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

শূন্য
প্ৰতীক_list_syms_print (ধ্রুৱ প্ৰতীক_list *l, FILE *out)
{
  fputc ('[', out);
  ৱর্ণ ধ্রুৱ *sep = "";
  বাবে (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_প্ৰতীক ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      যদি (l->content_type == SYMLIST_প্ৰতীক)
        প্ৰতীক_print (l->content.sym, out);
      fputs (l->action_props.is_মান_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

প্ৰতীক_list *
প্ৰতীক_list_prepend (প্ৰতীক_list *list, প্ৰতীক_list *গ্ৰন্থি)
{
  গ্ৰন্থি->next = list;
  ঘূৰাই গ্ৰন্থি;
}


প্ৰতীক_list *
প্ৰতীক_list_last (প্ৰতীক_list *list)
{
  যদি (!list)
    ঘূৰাই রিক্ত;
  প্ৰতীক_list *next = list;
  যেতিয়ালৈ (next->next)
    next = next->next;
  ঘূৰাই next;
}

প্ৰতীক_list *
প্ৰতীক_list_append (প্ৰতীক_list *list, প্ৰতীক_list *গ্ৰন্থি)
{
  যদি (list)
    প্ৰতীক_list_last (list)->next = গ্ৰন্থি;
  অন্যথা
    list = গ্ৰন্থি;
  ঘূৰাই list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

শূন্য
প্ৰতীক_list_free (প্ৰতীক_list *list)
{
  বাবে (প্ৰতীক_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      যদি (list->content_type == SYMLIST_TYPE)
        মুক্ত (list->content.sem_type);
      মুক্ত (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

পূর্ণাঙ্ক
প্ৰতীক_list_length (প্ৰতীক_list ধ্রুৱ *l)
{
  পূর্ণাঙ্ক res = 0;
  বাবে (/* Nothing. */;
       l && !(l->content_type == SYMLIST_প্ৰতীক && l->content.sym == রিক্ত);
       l = l->next)
    ++res;
  ঘূৰাই res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

প্ৰতীক_list *
প্ৰতীক_list_n_get (প্ৰতীক_list *l, পূর্ণাঙ্ক n)
{
  aver (0 <= n);
  বাবে (পূর্ণাঙ্ক i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_প্ৰতীক);
  aver (l->content.sym);
  ঘূৰাই l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
প্ৰতীক_list_n_type_name_get (প্ৰতীক_list *l, পূর্ণাঙ্ক n)
{
  ঘূৰাই প্ৰতীক_list_n_get (l, n)->content.sym->content->type_name;
}

bool
প্ৰতীক_list_null (প্ৰতীক_list *গ্ৰন্থি)
{
  ঘূৰাই (!গ্ৰন্থি
          || (গ্ৰন্থি->content_type == SYMLIST_প্ৰতীক && !গ্ৰন্থি->content.sym));
}

শূন্য
প্ৰতীক_list_code_props_set (প্ৰতীক_list *গ্ৰন্থি, code_props_type kind,
                            code_props ধ্রুৱ *cprops)
{
  ৱিকল্প (গ্ৰন্থি->content_type)
    {
    পক্ষ SYMLIST_প্ৰতীক:
      প্ৰতীক_code_props_set (গ্ৰন্থি->content.sym, kind, cprops);
      যদি (গ্ৰন্থি->content.sym->content->status == undeclared)
        গ্ৰন্থি->content.sym->content->status = used;
      ভঙ্গ;
    পক্ষ SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (গ্ৰন্থি->content.sem_type->tag,
                            &গ্ৰন্থি->content.sem_type->location),
         kind, cprops);
      যদি (গ্ৰন্থি->content.sem_type->status == undeclared)
        গ্ৰন্থি->content.sem_type->status = used;
      ভঙ্গ;
    }
}
