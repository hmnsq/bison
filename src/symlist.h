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
تعريفنوع بنية علامت_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  تعداد {
    SYMLIST_علامت,
    SYMLIST_TYPE
  } content_type;
  اتحاد {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    علامت *sym;
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
  بنية علامت_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  بنية علامت_list *midrule_parent_rule;
  صحيح midrule_parent_rhs_index;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  علامت *ruleprec;

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
  بنية علامت_list *next;
} علامت_list;


/** Create a list containing \c sym at \c loc.  */
علامت_list *علامت_list_sym_new (علامت *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
علامت_list *علامت_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
علامت_list *علامت_list_type_set (علامت_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
علامت_list *علامت_list_find_علامت (علامت_list *syms, ثابت علامت *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
فراغ علامت_list_syms_print (ثابت علامت_list *l, FILE *f);

/** Prepend \c node to \c list.  */
علامت_list *علامت_list_prepend (علامت_list *list, علامت_list *گرہ);

/** The last node of this list. */
علامت_list *علامت_list_last (علامت_list *list);

/** Append \c node to \c list.  */
علامت_list *علامت_list_append (علامت_list *list, علامت_list *گرہ);

/** Free \c list, but not the items it contains.  */
فراغ علامت_list_free (علامت_list *list);

/** Return the length of \c l. */
صحيح علامت_list_length (علامت_list ثابت *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
علامت_list *علامت_list_n_get (علامت_list *l, صحيح n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr علامت_list_n_type_name_get (علامت_list *l, صحيح n);

/** Check whether the node is a border element of a rule. */
bool علامت_list_null (علامت_list *گرہ);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
فراغ علامت_list_code_props_set (علامت_list *گرہ, code_props_type kind,
                                 code_props ثابت *cprops);

#endif /* !SYMLIST_H_ */
