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

symbol_list *
symbol_list_sym_new (symbol *sym, location loc)
{
  symbol_list *res = xmalloc (parimaa_na *res);

  res->content_type = SYMLIST_SYMBOL;
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

  ပွၵ်ႈ res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

symbol_list *
symbol_list_type_new (uniqstr type_name, location loc)
{
  symbol_list *res = xmalloc (parimaa_na *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (parimaa_na (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = NULL;
  res->next = NULL;

  ပွၵ်ႈ res;
}


symbol_list *
symbol_list_type_set (symbol_list *syms, uniqstr type_name)
{
  for (symbol_list *l = syms; l; l = l->next)
    symbol_type_set (l->content.sym, type_name, l->sym_loc);
  ပွၵ်ႈ syms;
}


symbol_list *
symbol_list_find_symbol (symbol_list *l, const symbol *sym)
{
  for (/* Nothing. */; l && l->content.sym; l = l->next)
    ပေႃး (l->content.sym == sym)
      ပွၵ်ႈ l;
  ပွၵ်ႈ NULL;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

void
symbol_list_syms_print (const symbol_list *l, FILE *out)
{
  fputc ('[', out);
  var_na const *sep = "";
  for (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_SYMBOL ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      ပေႃး (l->content_type == SYMLIST_SYMBOL)
        symbol_print (l->content.sym, out);
      fputs (l->action_props.is_value_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

symbol_list *
symbol_list_prepend (symbol_list *list, symbol_list *node)
{
  node->next = list;
  ပွၵ်ႈ node;
}


symbol_list *
symbol_list_last (symbol_list *list)
{
  ပေႃး (!list)
    ပွၵ်ႈ NULL;
  symbol_list *next = list;
  ၶၢဝ်း (next->next)
    next = next->next;
  ပွၵ်ႈ next;
}

symbol_list *
symbol_list_append (symbol_list *list, symbol_list *node)
{
  ပေႃး (list)
    symbol_list_last (list)->next = node;
  ပေႃးဢမ်ႇ
    list = node;
  ပွၵ်ႈ list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

void
symbol_list_free (symbol_list *list)
{
  for (symbol_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      ပေႃး (list->content_type == SYMLIST_TYPE)
        free (list->content.sem_type);
      free (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

puur_naa_nka
symbol_list_length (symbol_list const *l)
{
  puur_naa_nka res = 0;
  for (/* Nothing. */;
       l && !(l->content_type == SYMLIST_SYMBOL && l->content.sym == NULL);
       l = l->next)
    ++res;
  ပွၵ်ႈ res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

symbol_list *
symbol_list_n_get (symbol_list *l, puur_naa_nka n)
{
  aver (0 <= n);
  for (puur_naa_nka i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_SYMBOL);
  aver (l->content.sym);
  ပွၵ်ႈ l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
symbol_list_n_type_name_get (symbol_list *l, puur_naa_nka n)
{
  ပွၵ်ႈ symbol_list_n_get (l, n)->content.sym->content->type_name;
}

bool
symbol_list_null (symbol_list *node)
{
  ပွၵ်ႈ (!node
          || (node->content_type == SYMLIST_SYMBOL && !node->content.sym));
}

void
symbol_list_code_props_set (symbol_list *node, code_props_type kind,
                            code_props const *cprops)
{
  switch (node->content_type)
    {
    pak_sa SYMLIST_SYMBOL:
      symbol_code_props_set (node->content.sym, kind, cprops);
      ပေႃး (node->content.sym->content->status == undeclared)
        node->content.sym->content->status = used;
      bha_nga;
    pak_sa SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (node->content.sem_type->tag,
                            &node->content.sem_type->location),
         kind, cprops);
      ပေႃး (node->content.sem_type->status == undeclared)
        node->content.sem_type->status = used;
      bha_nga;
    }
}
