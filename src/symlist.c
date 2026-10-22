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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

குறியீடு_list *
குறியீடு_list_sym_new (குறியீடு *sym, location loc)
{
  குறியீடு_list *res = xmalloc (அளவு *res);

  res->content_type = SYMLIST_குறியீடு;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = வெற்று;

  res->midrule = வெற்று;
  res->midrule_parent_rule = வெற்று;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = வெற்று;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = வெற்று;

  திரும்பு res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

குறியீடு_list *
குறியீடு_list_type_new (uniqstr type_name, location loc)
{
  குறியீடு_list *res = xmalloc (அளவு *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (அளவு (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = வெற்று;
  res->next = வெற்று;

  திரும்பு res;
}


குறியீடு_list *
குறியீடு_list_type_set (குறியீடு_list *syms, uniqstr type_name)
{
  க்கு (குறியீடு_list *l = syms; l; l = l->next)
    குறியீடு_type_set (l->content.sym, type_name, l->sym_loc);
  திரும்பு syms;
}


குறியீடு_list *
குறியீடு_list_find_குறியீடு (குறியீடு_list *l, மாறிலி குறியீடு *sym)
{
  க்கு (/* Nothing. */; l && l->content.sym; l = l->next)
    எனில் (l->content.sym == sym)
      திரும்பு l;
  திரும்பு வெற்று;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

வெறுமை
குறியீடு_list_syms_print (மாறிலி குறியீடு_list *l, FILE *out)
{
  fputc ('[', out);
  எழுத்து மாறிலி *sep = "";
  க்கு (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_குறியீடு ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      எனில் (l->content_type == SYMLIST_குறியீடு)
        குறியீடு_print (l->content.sym, out);
      fputs (l->action_props.is_மதிப்பு_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

குறியீடு_list *
குறியீடு_list_prepend (குறியீடு_list *list, குறியீடு_list *முனை)
{
  முனை->next = list;
  திரும்பு முனை;
}


குறியீடு_list *
குறியீடு_list_last (குறியீடு_list *list)
{
  எனில் (!list)
    திரும்பு வெற்று;
  குறியீடு_list *next = list;
  போது (next->next)
    next = next->next;
  திரும்பு next;
}

குறியீடு_list *
குறியீடு_list_append (குறியீடு_list *list, குறியீடு_list *முனை)
{
  எனில் (list)
    குறியீடு_list_last (list)->next = முனை;
  இல்லையெனில்
    list = முனை;
  திரும்பு list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

வெறுமை
குறியீடு_list_free (குறியீடு_list *list)
{
  க்கு (குறியீடு_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      எனில் (list->content_type == SYMLIST_TYPE)
        விடுவி (list->content.sem_type);
      விடுவி (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

முழு
குறியீடு_list_length (குறியீடு_list மாறிலி *l)
{
  முழு res = 0;
  க்கு (/* Nothing. */;
       l && !(l->content_type == SYMLIST_குறியீடு && l->content.sym == வெற்று);
       l = l->next)
    ++res;
  திரும்பு res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

குறியீடு_list *
குறியீடு_list_n_get (குறியீடு_list *l, முழு n)
{
  aver (0 <= n);
  க்கு (முழு i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_குறியீடு);
  aver (l->content.sym);
  திரும்பு l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
குறியீடு_list_n_type_name_get (குறியீடு_list *l, முழு n)
{
  திரும்பு குறியீடு_list_n_get (l, n)->content.sym->content->type_name;
}

bool
குறியீடு_list_null (குறியீடு_list *முனை)
{
  திரும்பு (!முனை
          || (முனை->content_type == SYMLIST_குறியீடு && !முனை->content.sym));
}

வெறுமை
குறியீடு_list_code_props_set (குறியீடு_list *முனை, code_props_type kind,
                            code_props மாறிலி *cprops)
{
  தேர்வு (முனை->content_type)
    {
    நிலை SYMLIST_குறியீடு:
      குறியீடு_code_props_set (முனை->content.sym, kind, cprops);
      எனில் (முனை->content.sym->content->status == undeclared)
        முனை->content.sym->content->status = used;
      நிறுத்து;
    நிலை SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (முனை->content.sem_type->tag,
                            &முனை->content.sem_type->location),
         kind, cprops);
      எனில் (முனை->content.sem_type->status == undeclared)
        முனை->content.sem_type->status = used;
      நிறுத்து;
    }
}
