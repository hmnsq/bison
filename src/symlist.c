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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

પ્રતીક_list *
પ્રતીક_list_sym_new (પ્રતીક *sym, location loc)
{
  પ્રતીક_list *res = xmalloc (કદ *res);

  res->content_type = SYMLIST_પ્રતીક;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = ખાલી;

  res->midrule = ખાલી;
  res->midrule_parent_rule = ખાલી;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = ખાલી;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = ખાલી;

  પાછા res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

પ્રતીક_list *
પ્રતીક_list_type_new (uniqstr type_name, location loc)
{
  પ્રતીક_list *res = xmalloc (કદ *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (કદ (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = ખાલી;
  res->next = ખાલી;

  પાછા res;
}


પ્રતીક_list *
પ્રતીક_list_type_set (પ્રતીક_list *syms, uniqstr type_name)
{
  માટે (પ્રતીક_list *l = syms; l; l = l->next)
    પ્રતીક_type_set (l->content.sym, type_name, l->sym_loc);
  પાછા syms;
}


પ્રતીક_list *
પ્રતીક_list_find_પ્રતીક (પ્રતીક_list *l, અચલ પ્રતીક *sym)
{
  માટે (/* Nothing. */; l && l->content.sym; l = l->next)
    જો (l->content.sym == sym)
      પાછા l;
  પાછા ખાલી;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

શૂન્ય
પ્રતીક_list_syms_print (અચલ પ્રતીક_list *l, FILE *out)
{
  fputc ('[', out);
  અક્ષર અચલ *sep = "";
  માટે (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_પ્રતીક ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      જો (l->content_type == SYMLIST_પ્રતીક)
        પ્રતીક_print (l->content.sym, out);
      fputs (l->action_props.is_કિંમત_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

પ્રતીક_list *
પ્રતીક_list_prepend (પ્રતીક_list *list, પ્રતીક_list *ગાંઠ)
{
  ગાંઠ->next = list;
  પાછા ગાંઠ;
}


પ્રતીક_list *
પ્રતીક_list_last (પ્રતીક_list *list)
{
  જો (!list)
    પાછા ખાલી;
  પ્રતીક_list *next = list;
  જ્યારે (next->next)
    next = next->next;
  પાછા next;
}

પ્રતીક_list *
પ્રતીક_list_append (પ્રતીક_list *list, પ્રતીક_list *ગાંઠ)
{
  જો (list)
    પ્રતીક_list_last (list)->next = ગાંઠ;
  નહીંતર
    list = ગાંઠ;
  પાછા list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

શૂન્ય
પ્રતીક_list_free (પ્રતીક_list *list)
{
  માટે (પ્રતીક_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      જો (list->content_type == SYMLIST_TYPE)
        મુક્ત (list->content.sem_type);
      મુક્ત (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

પૂર્ણાંક
પ્રતીક_list_length (પ્રતીક_list અચલ *l)
{
  પૂર્ણાંક res = 0;
  માટે (/* Nothing. */;
       l && !(l->content_type == SYMLIST_પ્રતીક && l->content.sym == ખાલી);
       l = l->next)
    ++res;
  પાછા res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

પ્રતીક_list *
પ્રતીક_list_n_get (પ્રતીક_list *l, પૂર્ણાંક n)
{
  aver (0 <= n);
  માટે (પૂર્ણાંક i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_પ્રતીક);
  aver (l->content.sym);
  પાછા l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
પ્રતીક_list_n_type_name_get (પ્રતીક_list *l, પૂર્ણાંક n)
{
  પાછા પ્રતીક_list_n_get (l, n)->content.sym->content->type_name;
}

bool
પ્રતીક_list_null (પ્રતીક_list *ગાંઠ)
{
  પાછા (!ગાંઠ
          || (ગાંઠ->content_type == SYMLIST_પ્રતીક && !ગાંઠ->content.sym));
}

શૂન્ય
પ્રતીક_list_code_props_set (પ્રતીક_list *ગાંઠ, code_props_type kind,
                            code_props અચલ *cprops)
{
  પસંદગી (ગાંઠ->content_type)
    {
    સ્થિતિ SYMLIST_પ્રતીક:
      પ્રતીક_code_props_set (ગાંઠ->content.sym, kind, cprops);
      જો (ગાંઠ->content.sym->content->status == undeclared)
        ગાંઠ->content.sym->content->status = used;
      તોડો;
    સ્થિતિ SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (ગાંઠ->content.sem_type->tag,
                            &ગાંઠ->content.sem_type->location),
         kind, cprops);
      જો (ગાંઠ->content.sem_type->status == undeclared)
        ગાંઠ->content.sem_type->status = used;
      તોડો;
    }
}
