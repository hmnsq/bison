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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_sym_new (ପ୍ରତୀକ *sym, location loc)
{
  ପ୍ରତୀକ_list *res = xmalloc (ଆକାର *res);

  res->content_type = SYMLIST_ପ୍ରତୀକ;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = ଖାଲି;

  res->midrule = ଖାଲି;
  res->midrule_parent_rule = ଖାଲି;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = ଖାଲି;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = ଖାଲି;

  ଫେରନ୍ତୁ res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_type_new (uniqstr type_name, location loc)
{
  ପ୍ରତୀକ_list *res = xmalloc (ଆକାର *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (ଆକାର (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = ଖାଲି;
  res->next = ଖାଲି;

  ଫେରନ୍ତୁ res;
}


ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_type_set (ପ୍ରତୀକ_list *syms, uniqstr type_name)
{
  ପାଇଁ (ପ୍ରତୀକ_list *l = syms; l; l = l->next)
    ପ୍ରତୀକ_type_set (l->content.sym, type_name, l->sym_loc);
  ଫେରନ୍ତୁ syms;
}


ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_find_ପ୍ରତୀକ (ପ୍ରତୀକ_list *l, ଅଚଳ ପ୍ରତୀକ *sym)
{
  ପାଇଁ (/* Nothing. */; l && l->content.sym; l = l->next)
    ଯଦି (l->content.sym == sym)
      ଫେରନ୍ତୁ l;
  ଫେରନ୍ତୁ ଖାଲି;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

ଶୂନ୍ୟ
ପ୍ରତୀକ_list_syms_print (ଅଚଳ ପ୍ରତୀକ_list *l, FILE *out)
{
  fputc ('[', out);
  ଅକ୍ଷର ଅଚଳ *sep = "";
  ପାଇଁ (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_ପ୍ରତୀକ ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      ଯଦି (l->content_type == SYMLIST_ପ୍ରତୀକ)
        ପ୍ରତୀକ_print (l->content.sym, out);
      fputs (l->action_props.is_ମୂଲ୍ୟ_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_prepend (ପ୍ରତୀକ_list *list, ପ୍ରତୀକ_list *ଗଣ୍ଠି)
{
  ଗଣ୍ଠି->next = list;
  ଫେରନ୍ତୁ ଗଣ୍ଠି;
}


ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_last (ପ୍ରତୀକ_list *list)
{
  ଯଦି (!list)
    ଫେରନ୍ତୁ ଖାଲି;
  ପ୍ରତୀକ_list *next = list;
  ଯେତେବେଳେ (next->next)
    next = next->next;
  ଫେରନ୍ତୁ next;
}

ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_append (ପ୍ରତୀକ_list *list, ପ୍ରତୀକ_list *ଗଣ୍ଠି)
{
  ଯଦି (list)
    ପ୍ରତୀକ_list_last (list)->next = ଗଣ୍ଠି;
  ଅନ୍ୟଥା
    list = ଗଣ୍ଠି;
  ଫେରନ୍ତୁ list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

ଶୂନ୍ୟ
ପ୍ରତୀକ_list_free (ପ୍ରତୀକ_list *list)
{
  ପାଇଁ (ପ୍ରତୀକ_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      ଯଦି (list->content_type == SYMLIST_TYPE)
        ମୁକ୍ତ (list->content.sem_type);
      ମୁକ୍ତ (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ପ୍ରତୀକ_list_length (ପ୍ରତୀକ_list ଅଚଳ *l)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
  ପାଇଁ (/* Nothing. */;
       l && !(l->content_type == SYMLIST_ପ୍ରତୀକ && l->content.sym == ଖାଲି);
       l = l->next)
    ++res;
  ଫେରନ୍ତୁ res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

ପ୍ରତୀକ_list *
ପ୍ରତୀକ_list_n_get (ପ୍ରତୀକ_list *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
  aver (0 <= n);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_ପ୍ରତୀକ);
  aver (l->content.sym);
  ଫେରନ୍ତୁ l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
ପ୍ରତୀକ_list_n_type_name_get (ପ୍ରତୀକ_list *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
  ଫେରନ୍ତୁ ପ୍ରତୀକ_list_n_get (l, n)->content.sym->content->type_name;
}

bool
ପ୍ରତୀକ_list_null (ପ୍ରତୀକ_list *ଗଣ୍ଠି)
{
  ଫେରନ୍ତୁ (!ଗଣ୍ଠି
          || (ଗଣ୍ଠି->content_type == SYMLIST_ପ୍ରତୀକ && !ଗଣ୍ଠି->content.sym));
}

ଶୂନ୍ୟ
ପ୍ରତୀକ_list_code_props_set (ପ୍ରତୀକ_list *ଗଣ୍ଠି, code_props_type kind,
                            code_props ଅଚଳ *cprops)
{
  ବାଛ (ଗଣ୍ଠି->content_type)
    {
    ସ୍ଥିତି SYMLIST_ପ୍ରତୀକ:
      ପ୍ରତୀକ_code_props_set (ଗଣ୍ଠି->content.sym, kind, cprops);
      ଯଦି (ଗଣ୍ଠି->content.sym->content->status == undeclared)
        ଗଣ୍ଠି->content.sym->content->status = used;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (ଗଣ୍ଠି->content.sem_type->tag,
                            &ଗଣ୍ଠି->content.sem_type->location),
         kind, cprops);
      ଯଦି (ଗଣ୍ଠି->content.sem_type->status == undeclared)
        ଗଣ୍ଠି->content.sem_type->status = used;
      ଭାଙ୍ଗ;
    }
}
