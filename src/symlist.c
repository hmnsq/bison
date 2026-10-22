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

#include <config.h>
#include "system.h"

#include "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

सिम्बोलो_list *
सिम्बोलो_list_sym_new (सिम्बोलो *sym, location loc)
{
  सिम्बोलो_list *res = xmalloc (sizeof *res);

  res->content_type = SYMLIST_सिम्बोलो;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = NULL;

  res->midrule = NULL;
  res->midrule_parent_rule = NULL;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = NULL;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = NULL;

  return res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

सिम्बोलो_list *
सिम्बोलो_list_type_new (uniqstr type_name, location loc)
{
  सिम्बोलो_list *res = xmalloc (sizeof *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (sizeof (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = NULL;
  res->next = NULL;

  return res;
}


सिम्बोलो_list *
सिम्बोलो_list_type_set (सिम्बोलो_list *syms, uniqstr type_name)
{
  for (सिम्बोलो_list *l = syms; l; l = l->next)
    सिम्बोलो_type_set (l->content.sym, type_name, l->sym_loc);
  return syms;
}


सिम्बोलो_list *
सिम्बोलो_list_find_सिम्बोलो (सिम्बोलो_list *l, const सिम्बोलो *sym)
{
  for (/* Nothing. */; l && l->content.sym; l = l->next)
    if (l->content.sym == sym)
      return l;
  return NULL;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

void
सिम्बोलो_list_syms_इलिम्बाग (const सिम्बोलो_list *l, FILE *out)
{
  fputc ('[', out);
  char const *sep = "";
  for (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_सिम्बोलो ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      if (l->content_type == SYMLIST_सिम्बोलो)
        सिम्बोलो_इलिम्बाग (l->content.sym, out);
      fputs (l->action_props.is_हालागा_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

सिम्बोलो_list *
सिम्बोलो_list_prepend (सिम्बोलो_list *list, सिम्बोलो_list *बुहोल)
{
  बुहोल->next = list;
  return बुहोल;
}


सिम्बोलो_list *
सिम्बोलो_list_last (सिम्बोलो_list *list)
{
  if (!list)
    return NULL;
  सिम्बोलो_list *next = list;
  while (next->next)
    next = next->next;
  return next;
}

सिम्बोलो_list *
सिम्बोलो_list_append (सिम्बोलो_list *list, सिम्बोलो_list *बुहोल)
{
  if (list)
    सिम्बोलो_list_last (list)->next = बुहोल;
  else
    list = बुहोल;
  return list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

void
सिम्बोलो_list_free (सिम्बोलो_list *list)
{
  for (सिम्बोलो_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      if (list->content_type == SYMLIST_TYPE)
        free (list->content.sem_type);
      free (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

int
सिम्बोलो_list_length (सिम्बोलो_list const *l)
{
  int res = 0;
  for (/* Nothing. */;
       l && !(l->content_type == SYMLIST_सिम्बोलो && l->content.sym == NULL);
       l = l->next)
    ++res;
  return res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

सिम्बोलो_list *
सिम्बोलो_list_n_get (सिम्बोलो_list *l, int n)
{
  aver (0 <= n);
  for (int i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_सिम्बोलो);
  aver (l->content.sym);
  return l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
सिम्बोलो_list_n_type_name_get (सिम्बोलो_list *l, int n)
{
  return सिम्बोलो_list_n_get (l, n)->content.sym->content->type_name;
}

bool
सिम्बोलो_list_null (सिम्बोलो_list *बुहोल)
{
  return (!बुहोल
          || (बुहोल->content_type == SYMLIST_सिम्बोलो && !बुहोल->content.sym));
}

void
सिम्बोलो_list_code_props_set (सिम्बोलो_list *बुहोल, code_props_type kind,
                            code_props const *cprops)
{
  switch (बुहोल->content_type)
    {
    case SYMLIST_सिम्बोलो:
      सिम्बोलो_code_props_set (बुहोल->content.sym, kind, cprops);
      if (बुहोल->content.sym->content->status == undeclared)
        बुहोल->content.sym->content->status = used;
      break;
    case SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (बुहोल->content.sem_type->tag,
                            &बुहोल->content.sem_type->location),
         kind, cprops);
      if (बुहोल->content.sem_type->status == undeclared)
        बुहोल->content.sem_type->status = used;
      break;
    }
}
