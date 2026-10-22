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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

ചിഹ്നം_list *
ചിഹ്നം_list_sym_new (ചിഹ്നം *sym, location loc)
{
  ചിഹ്നം_list *res = xmalloc (വലുപ്പം *res);

  res->content_type = SYMLIST_ചിഹ്നം;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = ശൂന്യ;

  res->midrule = ശൂന്യ;
  res->midrule_parent_rule = ശൂന്യ;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = ശൂന്യ;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = ശൂന്യ;

  തിരികെ res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

ചിഹ്നം_list *
ചിഹ്നം_list_type_new (uniqstr type_name, location loc)
{
  ചിഹ്നം_list *res = xmalloc (വലുപ്പം *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (വലുപ്പം (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = ശൂന്യ;
  res->next = ശൂന്യ;

  തിരികെ res;
}


ചിഹ്നം_list *
ചിഹ്നം_list_type_set (ചിഹ്നം_list *syms, uniqstr type_name)
{
  വേണ്ടി (ചിഹ്നം_list *l = syms; l; l = l->next)
    ചിഹ്നം_type_set (l->content.sym, type_name, l->sym_loc);
  തിരികെ syms;
}


ചിഹ്നം_list *
ചിഹ്നം_list_find_ചിഹ്നം (ചിഹ്നം_list *l, സ്ഥിരം ചിഹ്നം *sym)
{
  വേണ്ടി (/* Nothing. */; l && l->content.sym; l = l->next)
    എങ്കിൽ (l->content.sym == sym)
      തിരികെ l;
  തിരികെ ശൂന്യ;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

ശൂന്യം
ചിഹ്നം_list_syms_print (സ്ഥിരം ചിഹ്നം_list *l, FILE *out)
{
  fputc ('[', out);
  അക്ഷരം സ്ഥിരം *sep = "";
  വേണ്ടി (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_ചിഹ്നം ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      എങ്കിൽ (l->content_type == SYMLIST_ചിഹ്നം)
        ചിഹ്നം_print (l->content.sym, out);
      fputs (l->action_props.is_മൂല്യം_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

ചിഹ്നം_list *
ചിഹ്നം_list_prepend (ചിഹ്നം_list *list, ചിഹ്നം_list *കണ്ണി)
{
  കണ്ണി->next = list;
  തിരികെ കണ്ണി;
}


ചിഹ്നം_list *
ചിഹ്നം_list_last (ചിഹ്നം_list *list)
{
  എങ്കിൽ (!list)
    തിരികെ ശൂന്യ;
  ചിഹ്നം_list *next = list;
  ആകുമ്പോൾ (next->next)
    next = next->next;
  തിരികെ next;
}

ചിഹ്നം_list *
ചിഹ്നം_list_append (ചിഹ്നം_list *list, ചിഹ്നം_list *കണ്ണി)
{
  എങ്കിൽ (list)
    ചിഹ്നം_list_last (list)->next = കണ്ണി;
  അല്ലെങ്കിൽ
    list = കണ്ണി;
  തിരികെ list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

ശൂന്യം
ചിഹ്നം_list_free (ചിഹ്നം_list *list)
{
  വേണ്ടി (ചിഹ്നം_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      എങ്കിൽ (list->content_type == SYMLIST_TYPE)
        സ്വതന്ത്രം (list->content.sem_type);
      സ്വതന്ത്രം (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

പൂർണ്ണസംഖ്യ
ചിഹ്നം_list_length (ചിഹ്നം_list സ്ഥിരം *l)
{
  പൂർണ്ണസംഖ്യ res = 0;
  വേണ്ടി (/* Nothing. */;
       l && !(l->content_type == SYMLIST_ചിഹ്നം && l->content.sym == ശൂന്യ);
       l = l->next)
    ++res;
  തിരികെ res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

ചിഹ്നം_list *
ചിഹ്നം_list_n_get (ചിഹ്നം_list *l, പൂർണ്ണസംഖ്യ n)
{
  aver (0 <= n);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_ചിഹ്നം);
  aver (l->content.sym);
  തിരികെ l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
ചിഹ്നം_list_n_type_name_get (ചിഹ്നം_list *l, പൂർണ്ണസംഖ്യ n)
{
  തിരികെ ചിഹ്നം_list_n_get (l, n)->content.sym->content->type_name;
}

bool
ചിഹ്നം_list_null (ചിഹ്നം_list *കണ്ണി)
{
  തിരികെ (!കണ്ണി
          || (കണ്ണി->content_type == SYMLIST_ചിഹ്നം && !കണ്ണി->content.sym));
}

ശൂന്യം
ചിഹ്നം_list_code_props_set (ചിഹ്നം_list *കണ്ണി, code_props_type kind,
                            code_props സ്ഥിരം *cprops)
{
  തിരഞ്ഞെടുപ്പ് (കണ്ണി->content_type)
    {
    അവസ്ഥ SYMLIST_ചിഹ്നം:
      ചിഹ്നം_code_props_set (കണ്ണി->content.sym, kind, cprops);
      എങ്കിൽ (കണ്ണി->content.sym->content->status == undeclared)
        കണ്ണി->content.sym->content->status = used;
      നിർത്തുക;
    അവസ്ഥ SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (കണ്ണി->content.sem_type->tag,
                            &കണ്ണി->content.sem_type->location),
         kind, cprops);
      എങ്കിൽ (കണ്ണി->content.sem_type->status == undeclared)
        കണ്ണി->content.sem_type->status = used;
      നിർത്തുക;
    }
}
