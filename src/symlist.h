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
تعريفنوع بنية سيمبول_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  تعداد {
    SYMLIST_سيمبول,
    SYMLIST_TYPE
  } content_type;
  اتحاد {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    سيمبول *sym;
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
  بنية سيمبول_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  بنية سيمبول_list *midrule_parent_rule;
  صحيح midrule_parent_rhs_index;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  سيمبول *ruleprec;

  /* The action is attached to the LHS of a rule, but action properties for
   * each RHS are also stored here.  */
  code_props action_props;

  /* The location of the first %empty for this rule, or \a
     empty_loc.  */
  location percent_empty_loc;

  صحيح dprec;
  location dprec_loc;
  صحيح merger;
  location merger_declaration_loc;

  /* Counts of the number of expected conflicts for this rule, or -1 if none
     given. */
  صحيح expected_sr_conflicts;
  صحيح expected_rr_conflicts;

  /* The list.  */
  بنية سيمبول_list *next;
} سيمبول_list;


/** Create a list containing \c sym at \c loc.  */
سيمبول_list *سيمبول_list_sym_new (سيمبول *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
سيمبول_list *سيمبول_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
سيمبول_list *سيمبول_list_type_set (سيمبول_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
سيمبول_list *سيمبول_list_find_سيمبول (سيمبول_list *syms, ثابت سيمبول *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
فراغ سيمبول_list_syms_print (ثابت سيمبول_list *l, FILE *f);

/** Prepend \c node to \c list.  */
سيمبول_list *سيمبول_list_prepend (سيمبول_list *list, سيمبول_list *نود);

/** The last node of this list. */
سيمبول_list *سيمبول_list_last (سيمبول_list *list);

/** Append \c node to \c list.  */
سيمبول_list *سيمبول_list_append (سيمبول_list *list, سيمبول_list *نود);

/** Free \c list, but not the items it contains.  */
فراغ سيمبول_list_free (سيمبول_list *list);

/** Return the length of \c l. */
صحيح سيمبول_list_length (سيمبول_list ثابت *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
سيمبول_list *سيمبول_list_n_get (سيمبول_list *l, صحيح n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr سيمبول_list_n_type_name_get (سيمبول_list *l, صحيح n);

/** Check whether the node is a border element of a rule. */
bool سيمبول_list_null (سيمبول_list *نود);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
فراغ سيمبول_list_code_props_set (سيمبول_list *نود, code_props_type kind,
                                 code_props ثابت *cprops);

#endif /* !SYMLIST_H_ */
