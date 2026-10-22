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

#ifndef SYMLIST_H_
# define SYMLIST_H_

# include "location.h"
# include "scan-code.h"
# include "symtab.h"
# include "named-ref.h"

/* A list of symbols, used during the parsing for many different
   purposes: rules, symbol declarations or properties (such as
   %destructor, etc.)...  */
प्रकारव्याख्या संरचना चिन्ह_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  गणना {
    SYMLIST_चिन्ह,
    SYMLIST_TYPE
  } content_type;
  संघ {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    चिन्ह *sym;
    /**
     * The semantic type iff <tt>symbol_list::content_type = SYMLIST_TYPE</tt>.
     */
    semantic_type *sem_type;
  } content;

  /* Named reference. */
  named_ref *named_ref;

  /* Proper location of the symbol, not all the rule */
  location sym_loc;

  /* If this symbol is the generated lhs for a midrule but this is the rule in
     whose rhs it appears, MIDRULE = a pointer to that midrule.  */
  संरचना चिन्ह_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  संरचना चिन्ह_list *midrule_parent_rule;
  पूर्णांक midrule_parent_rhs_index;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  चिन्ह *ruleprec;

  /* The action is attached to the LHS of a rule, but action properties for
   * each RHS are also stored here.  */
  code_props action_props;

  /* The location of the first %empty for this rule, or \a
     empty_loc.  */
  location percent_empty_loc;

  पूर्णांक dprec;
  location dprec_loc;
  पूर्णांक merger;
  location merger_declaration_loc;

  /* Counts of the number of expected conflicts for this rule, or -1 if none
     given. */
  पूर्णांक expected_sr_conflicts;
  पूर्णांक expected_rr_conflicts;

  /* The list.  */
  संरचना चिन्ह_list *next;
} चिन्ह_list;


/** Create a list containing \c sym at \c loc.  */
चिन्ह_list *चिन्ह_list_sym_new (चिन्ह *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
चिन्ह_list *चिन्ह_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
चिन्ह_list *चिन्ह_list_type_set (चिन्ह_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
चिन्ह_list *चिन्ह_list_find_चिन्ह (चिन्ह_list *syms, स्थिरांक चिन्ह *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
शून्य चिन्ह_list_syms_print (स्थिरांक चिन्ह_list *l, खाता *f);

/** Prepend \c node to \c list.  */
चिन्ह_list *चिन्ह_list_prepend (चिन्ह_list *list, चिन्ह_list *गाठ);

/** The last node of this list. */
चिन्ह_list *चिन्ह_list_last (चिन्ह_list *list);

/** Append \c node to \c list.  */
चिन्ह_list *चिन्ह_list_append (चिन्ह_list *list, चिन्ह_list *गाठ);

/** Free \c list, but not the items it contains.  */
शून्य चिन्ह_list_free (चिन्ह_list *list);

/** Return the length of \c l. */
पूर्णांक चिन्ह_list_length (चिन्ह_list स्थिरांक *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
चिन्ह_list *चिन्ह_list_n_get (चिन्ह_list *l, पूर्णांक n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr चिन्ह_list_n_type_name_get (चिन्ह_list *l, पूर्णांक n);

/** Check whether the node is a border element of a rule. */
bool चिन्ह_list_null (चिन्ह_list *गाठ);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
शून्य चिन्ह_list_code_props_set (चिन्ह_list *गाठ, code_props_type kind,
                                 code_props स्थिरांक *cprops);

#endif /* !SYMLIST_H_ */
