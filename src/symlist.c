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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "symlist.h"

/*--------------------------------------.
| Create a list containing SYM at LOC.  |
`--------------------------------------*/

चिह्न_list *
चिह्न_list_sym_new (चिह्न *sym, location loc)
{
  चिह्न_list *res = xmalloc (परिमाण *res);

  res->content_type = SYMLIST_चिह्न;
  res->content.sym = sym;
  res->sym_loc = loc;
  res->named_ref = रिक्त;

  res->midrule = रिक्त;
  res->midrule_parent_rule = रिक्त;
  res->midrule_parent_rhs_index = 0;

  /* Members used for LHS only.  */
  res->rhs_loc = empty_loc;
  res->ruleprec = रिक्त;
  res->percent_empty_loc = empty_loc;
  code_props_none_init (&res->action_props);
  res->dprec = 0;
  res->dprec_loc = empty_loc;
  res->merger = 0;
  res->merger_declaration_loc = empty_loc;
  res->expected_sr_conflicts = -1;
  res->expected_rr_conflicts = -1;

  res->next = रिक्त;

  फर्काऊ res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

चिह्न_list *
चिह्न_list_type_new (uniqstr type_name, location loc)
{
  चिह्न_list *res = xmalloc (परिमाण *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (परिमाण (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = रिक्त;
  res->next = रिक्त;

  फर्काऊ res;
}


चिह्न_list *
चिह्न_list_type_set (चिह्न_list *syms, uniqstr type_name)
{
  लागि (चिह्न_list *l = syms; l; l = l->next)
    चिह्न_type_set (l->content.sym, type_name, l->sym_loc);
  फर्काऊ syms;
}


चिह्न_list *
चिह्न_list_find_चिह्न (चिह्न_list *l, अचर चिह्न *sym)
{
  लागि (/* Nothing. */; l && l->content.sym; l = l->next)
    यदि (l->content.sym == sym)
      फर्काऊ l;
  फर्काऊ रिक्त;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

शून्य
चिह्न_list_syms_print (अचर चिह्न_list *l, खाता *out)
{
  ख_अक्षर_दो ('[', out);
  वर्ण अचर *sep = "";
  लागि (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      ख_माला_दो (sep, out);
      ख_माला_दो (l->content_type == SYMLIST_चिह्न ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      यदि (l->content_type == SYMLIST_चिह्न)
        चिह्न_print (l->content.sym, out);
      ख_माला_दो (l->action_props.is_मान_used ? " (used)" : " (unused)", out);
      अ_दो ('}', out);
      sep = ", ";
    }
  ख_अक्षर_दो (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

चिह्न_list *
चिह्न_list_prepend (चिह्न_list *list, चिह्न_list *गाँठो)
{
  गाँठो->next = list;
  फर्काऊ गाँठो;
}


चिह्न_list *
चिह्न_list_last (चिह्न_list *list)
{
  यदि (!list)
    फर्काऊ रिक्त;
  चिह्न_list *next = list;
  जबसम्म (next->next)
    next = next->next;
  फर्काऊ next;
}

चिह्न_list *
चिह्न_list_append (चिह्न_list *list, चिह्न_list *गाँठो)
{
  यदि (list)
    चिह्न_list_last (list)->next = गाँठो;
  अन्यथा
    list = गाँठो;
  फर्काऊ list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

शून्य
चिह्न_list_free (चिह्न_list *list)
{
  लागि (चिह्न_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      यदि (list->content_type == SYMLIST_TYPE)
        मुक्त (list->content.sem_type);
      मुक्त (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

पूर्णाङ्क
चिह्न_list_length (चिह्न_list अचर *l)
{
  पूर्णाङ्क res = 0;
  लागि (/* Nothing. */;
       l && !(l->content_type == SYMLIST_चिह्न && l->content.sym == रिक्त);
       l = l->next)
    ++res;
  फर्काऊ res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

चिह्न_list *
चिह्न_list_n_get (चिह्न_list *l, पूर्णाङ्क n)
{
  aver (0 <= n);
  लागि (पूर्णाङ्क i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_चिह्न);
  aver (l->content.sym);
  फर्काऊ l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
चिह्न_list_n_type_name_get (चिह्न_list *l, पूर्णाङ्क n)
{
  फर्काऊ चिह्न_list_n_get (l, n)->content.sym->content->type_name;
}

bool
चिह्न_list_null (चिह्न_list *गाँठो)
{
  फर्काऊ (!गाँठो
          || (गाँठो->content_type == SYMLIST_चिह्न && !गाँठो->content.sym));
}

शून्य
चिह्न_list_code_props_set (चिह्न_list *गाँठो, code_props_type kind,
                            code_props अचर *cprops)
{
  विकल्प (गाँठो->content_type)
    {
    पक्ष SYMLIST_चिह्न:
      चिह्न_code_props_set (गाँठो->content.sym, kind, cprops);
      यदि (गाँठो->content.sym->content->status == undeclared)
        गाँठो->content.sym->content->status = used;
      रोक;
    पक्ष SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (गाँठो->content.sem_type->tag,
                            &गाँठो->content.sem_type->location),
         kind, cprops);
      यदि (गाँठो->content.sem_type->status == undeclared)
        गाँठो->content.sem_type->status = used;
      रोक;
    }
}
