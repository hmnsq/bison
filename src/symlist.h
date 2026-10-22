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

#که_نه_تعریف_شوی SYMLIST_H_
# define SYMLIST_H_

# include "location.h"
# include "scan-code.h"
# include "symtab.h"
# include "named-ref.h"

/* A list of symbols, used during the parsing for many different
   purposes: rules, symbol declarations or properties (such as
   %destructor, etc.)...  */
تعريفنوع بنية نښه_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  تعداد {
    SYMLIST_نښه,
    SYMLIST_TYPE
  } content_type;
  اتحاد {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    نښه *sym;
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
  بنية نښه_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  بنية نښه_list *midrule_پلار_قاعده;
  صحيح midrule_پلار_rhs_شاخص;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  نښه *ruleprec;

  /* The action is attached to the LHS of a rule, but action properties for
   * each RHS are also stored here.  */
  کوډ_props action_props;

  /* The location of the first %empty for this rule, or \a
     empty_loc.  */
  location percent_تشه_loc;

  صحيح dprec;
  location dprec_loc;
  صحيح merger;
  location merger_declaration_loc;

  /* Counts of the number of expected conflicts for this rule, or -1 if none
     given. */
  صحيح expected_sr_conflicts;
  صحيح expected_rr_conflicts;

  /* The list.  */
  بنية نښه_list *بل;
} نښه_list;


/** Create a list containing \c sym at \c loc.  */
نښه_list *نښه_list_sym_new (نښه *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
نښه_list *نښه_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
نښه_list *نښه_list_type_سېټ (نښه_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
نښه_list *نښه_list_پیدا_کړه_نښه (نښه_list *syms, ثابت نښه *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
فراغ نښه_list_syms_print (ثابت نښه_list *l, FILE *f);

/** Prepend \c node to \c list.  */
نښه_list *نښه_list_prepend (نښه_list *list, نښه_list *غوټه);

/** The last node of this list. */
نښه_list *نښه_list_وروستی (نښه_list *list);

/** Append \c node to \c list.  */
نښه_list *نښه_list_append (نښه_list *list, نښه_list *غوټه);

/** Free \c list, but not the items it contains.  */
فراغ نښه_list_free (نښه_list *list);

/** Return the length of \c l. */
صحيح نښه_list_اوږدتیا (نښه_list ثابت *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
نښه_list *نښه_list_n_get (نښه_list *l, صحيح n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr نښه_list_n_type_name_get (نښه_list *l, صحيح n);

/** Check whether the node is a border element of a rule. */
bool نښه_list_null (نښه_list *غوټه);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
فراغ نښه_list_کوډ_props_سېټ (نښه_list *غوټه, کوډ_props_type ډول,
                                 کوډ_props ثابت *cprops);

#د_که_پای /* !SYMLIST_H_ */
