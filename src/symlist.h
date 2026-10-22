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

#अगर_अघोषित SYMLIST_H_
# define SYMLIST_H_

# include "location.h"
# include "scan-code.h"
# include "symtab.h"
# include "named-ref.h"

/* A list of symbols, used during the parsing for many different
   purposes: rules, symbol declarations or properties (such as
   %destructor, etc.)...  */
प्रकार काष्ठा प्रतीक_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  क्रमागत {
    SYMLIST_प्रतीक,
    SYMLIST_TYPE
  } content_type;
  जोड़ {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    प्रतीक *sym;
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
  काष्ठा प्रतीक_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  काष्ठा प्रतीक_list *midrule_parent_rule;
  पूर्णांक midrule_parent_rhs_सूचकांक;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  प्रतीक *ruleprec;

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
  काष्ठा प्रतीक_list *अग्र;
} प्रतीक_list;


/** Create a list containing \c sym at \c loc.  */
प्रतीक_list *प्रतीक_list_sym_new (प्रतीक *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
प्रतीक_list *प्रतीक_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
प्रतीक_list *प्रतीक_list_type_set (प्रतीक_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
प्रतीक_list *प्रतीक_list_find_प्रतीक (प्रतीक_list *syms, स्थिर प्रतीक *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
व्योम प्रतीक_list_syms_print (स्थिर प्रतीक_list *l, खाता *f);

/** Prepend \c node to \c list.  */
प्रतीक_list *प्रतीक_list_prepend (प्रतीक_list *list, प्रतीक_list *गाँठ);

/** The last node of this list. */
प्रतीक_list *प्रतीक_list_last (प्रतीक_list *list);

/** Append \c node to \c list.  */
प्रतीक_list *प्रतीक_list_append (प्रतीक_list *list, प्रतीक_list *गाँठ);

/** Free \c list, but not the items it contains.  */
व्योम प्रतीक_list_free (प्रतीक_list *list);

/** Return the length of \c l. */
पूर्णांक प्रतीक_list_लंबाई (प्रतीक_list स्थिर *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
प्रतीक_list *प्रतीक_list_n_get (प्रतीक_list *l, पूर्णांक n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr प्रतीक_list_n_type_name_get (प्रतीक_list *l, पूर्णांक n);

/** Check whether the node is a border element of a rule. */
bool प्रतीक_list_null (प्रतीक_list *गाँठ);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
व्योम प्रतीक_list_code_props_set (प्रतीक_list *गाँठ, code_props_type kind,
                                 code_props स्थिर *cprops);

#पूर्ण_अगर /* !SYMLIST_H_ */
