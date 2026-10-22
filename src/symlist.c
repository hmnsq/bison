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

चिह्नम्_list *
चिह्नम्_list_sym_new (चिह्नम् *sym, location loc)
{
  चिह्नम्_list *res = xmalloc (परिमाण *res);

  res->content_type = SYMLIST_चिह्नम्;
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

  पटिनिवत्त res;
}


/*--------------------------------------------.
| Create a list containing TYPE_NAME at LOC.  |
`--------------------------------------------*/

चिह्नम्_list *
चिह्नम्_list_type_new (uniqstr type_name, location loc)
{
  चिह्नम्_list *res = xmalloc (परिमाण *res);

  res->content_type = SYMLIST_TYPE;
  res->content.sem_type = xmalloc (परिमाण (semantic_type));
  res->content.sem_type->tag = type_name;
  res->content.sem_type->location = loc;
  res->content.sem_type->status = undeclared;

  res->sym_loc = loc;
  res->named_ref = रिक्त;
  res->next = रिक्त;

  पटिनिवत्त res;
}


चिह्नम्_list *
चिह्नम्_list_type_set (चिह्नम्_list *syms, uniqstr type_name)
{
  अत्थाय (चिह्नम्_list *l = syms; l; l = l->next)
    चिह्नम्_type_set (l->content.sym, type_name, l->sym_loc);
  पटिनिवत्त syms;
}


चिह्नम्_list *
चिह्नम्_list_find_चिह्नम् (चिह्नम्_list *l, ध्रुव चिह्नम् *sym)
{
  अत्थाय (/* Nothing. */; l && l->content.sym; l = l->next)
    सचे (l->content.sym == sym)
      पटिनिवत्त l;
  पटिनिवत्त रिक्त;
}


/*-----------------------------------------------------------------------.
| Print this list, for which every content_type must be SYMLIST_SYMBOL.  |
`-----------------------------------------------------------------------*/

शून्य
चिह्नम्_list_syms_print (ध्रुव चिह्नम्_list *l, FILE *out)
{
  fputc ('[', out);
  वर्ण ध्रुव *sep = "";
  अत्थाय (/* Nothing. */; l && l->content.sym; l = l->next)
    {
      fputs (sep, out);
      fputs (l->content_type == SYMLIST_चिह्नम् ? "symbol{"
             : l->content_type == SYMLIST_TYPE ? "type{"
             : "invalid content_type{",
             out);
      सचे (l->content_type == SYMLIST_चिह्नम्)
        चिह्नम्_print (l->content.sym, out);
      fputs (l->action_props.is_मानम्_used ? " (used)" : " (unused)", out);
      putc ('}', out);
      sep = ", ";
    }
  fputc (']', out);
}


/*---------------------------.
| Prepend NODE to the LIST.  |
`---------------------------*/

चिह्नम्_list *
चिह्नम्_list_prepend (चिह्नम्_list *list, चिह्नम्_list *ग्रन्थिः)
{
  ग्रन्थिः->next = list;
  पटिनिवत्त ग्रन्थिः;
}


चिह्नम्_list *
चिह्नम्_list_last (चिह्नम्_list *list)
{
  सचे (!list)
    पटिनिवत्त रिक्त;
  चिह्नम्_list *next = list;
  याव (next->next)
    next = next->next;
  पटिनिवत्त next;
}

चिह्नम्_list *
चिह्नम्_list_append (चिह्नम्_list *list, चिह्नम्_list *ग्रन्थिः)
{
  सचे (list)
    चिह्नम्_list_last (list)->next = ग्रन्थिः;
  अञ्ञथा
    list = ग्रन्थिः;
  पटिनिवत्त list;
}



/*-----------------------------------------------.
| Free the LIST, but not the items it contains.  |
`-----------------------------------------------*/

शून्य
चिह्नम्_list_free (चिह्नम्_list *list)
{
  अत्थाय (चिह्नम्_list *next; list; list = next)
    {
      next = list->next;
      named_ref_free (list->named_ref);
      सचे (list->content_type == SYMLIST_TYPE)
        मुक्त (list->content.sem_type);
      मुक्त (list);
    }
}


/*--------------------.
| Return its length.  |
`--------------------*/

पूर्णाङ्क
चिह्नम्_list_length (चिह्नम्_list ध्रुव *l)
{
  पूर्णाङ्क res = 0;
  अत्थाय (/* Nothing. */;
       l && !(l->content_type == SYMLIST_चिह्नम् && l->content.sym == रिक्त);
       l = l->next)
    ++res;
  पटिनिवत्त res;
}


/*------------------------------.
| Get item N in symbol list L.  |
`------------------------------*/

चिह्नम्_list *
चिह्नम्_list_n_get (चिह्नम्_list *l, पूर्णाङ्क n)
{
  aver (0 <= n);
  अत्थाय (पूर्णाङ्क i = 0; i < n; ++i)
    {
      l = l->next;
      aver (l);
    }
  aver (l->content_type == SYMLIST_चिह्नम्);
  aver (l->content.sym);
  पटिनिवत्त l;
}

/*--------------------------------------------------------------.
| Get the data type (alternative in the union) of the value for |
| symbol N in symbol list L.                                    |
`--------------------------------------------------------------*/

uniqstr
चिह्नम्_list_n_type_name_get (चिह्नम्_list *l, पूर्णाङ्क n)
{
  पटिनिवत्त चिह्नम्_list_n_get (l, n)->content.sym->content->type_name;
}

bool
चिह्नम्_list_null (चिह्नम्_list *ग्रन्थिः)
{
  पटिनिवत्त (!ग्रन्थिः
          || (ग्रन्थिः->content_type == SYMLIST_चिह्नम् && !ग्रन्थिः->content.sym));
}

शून्य
चिह्नम्_list_code_props_set (चिह्नम्_list *ग्रन्थिः, code_props_type kind,
                            code_props ध्रुव *cprops)
{
  विकल्प (ग्रन्थिः->content_type)
    {
    पक्ष SYMLIST_चिह्नम्:
      चिह्नम्_code_props_set (ग्रन्थिः->content.sym, kind, cprops);
      सचे (ग्रन्थिः->content.sym->content->status == undeclared)
        ग्रन्थिः->content.sym->content->status = used;
      भङ्ग;
    पक्ष SYMLIST_TYPE:
      semantic_type_code_props_set
        (semantic_type_get (ग्रन्थिः->content.sem_type->tag,
                            &ग्रन्थिः->content.sem_type->location),
         kind, cprops);
      सचे (ग्रन्थिः->content.sem_type->status == undeclared)
        ग्रन्थिः->content.sem_type->status = used;
      भङ्ग;
    }
}
