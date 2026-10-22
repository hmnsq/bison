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
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ಚಿಹ್ನೆ_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  ಗಣನಾ {
    SYMLIST_ಚಿಹ್ನೆ,
    SYMLIST_TYPE
  } content_type;
  ಸಙ್ಘ {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    ಚಿಹ್ನೆ *sym;
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
  ಸಂರಚನಾ ಚಿಹ್ನೆ_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  ಸಂರಚನಾ ಚಿಹ್ನೆ_list *midrule_parent_rule;
  ಪೂರ್ಣಾಙ್ಕ midrule_parent_rhs_index;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  ಚಿಹ್ನೆ *ruleprec;

  /* The action is attached to the LHS of a rule, but action properties for
   * each RHS are also stored here.  */
  code_props action_props;

  /* The location of the first %empty for this rule, or \a
     empty_loc.  */
  location percent_empty_loc;

  ಪೂರ್ಣಾಙ್ಕ dprec;
  location dprec_loc;
  ಪೂರ್ಣಾಙ್ಕ merger;
  location merger_declaration_loc;

  /* Counts of the number of expected conflicts for this rule, or -1 if none
     given. */
  ಪೂರ್ಣಾಙ್ಕ expected_sr_conflicts;
  ಪೂರ್ಣಾಙ್ಕ expected_rr_conflicts;

  /* The list.  */
  ಸಂರಚನಾ ಚಿಹ್ನೆ_list *next;
} ಚಿಹ್ನೆ_list;


/** Create a list containing \c sym at \c loc.  */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_sym_new (ಚಿಹ್ನೆ *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_type_set (ಚಿಹ್ನೆ_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_find_ಚಿಹ್ನೆ (ಚಿಹ್ನೆ_list *syms, ಧ್ರುವ ಚಿಹ್ನೆ *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_list_syms_print (ಧ್ರುವ ಚಿಹ್ನೆ_list *l, FILE *f);

/** Prepend \c node to \c list.  */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_prepend (ಚಿಹ್ನೆ_list *list, ಚಿಹ್ನೆ_list *ಗಂಟು);

/** The last node of this list. */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_last (ಚಿಹ್ನೆ_list *list);

/** Append \c node to \c list.  */
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_append (ಚಿಹ್ನೆ_list *list, ಚಿಹ್ನೆ_list *ಗಂಟು);

/** Free \c list, but not the items it contains.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_list_free (ಚಿಹ್ನೆ_list *list);

/** Return the length of \c l. */
ಪೂರ್ಣಾಙ್ಕ ಚಿಹ್ನೆ_list_length (ಚಿಹ್ನೆ_list ಧ್ರುವ *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
ಚಿಹ್ನೆ_list *ಚಿಹ್ನೆ_list_n_get (ಚಿಹ್ನೆ_list *l, ಪೂರ್ಣಾಙ್ಕ n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr ಚಿಹ್ನೆ_list_n_type_name_get (ಚಿಹ್ನೆ_list *l, ಪೂರ್ಣಾಙ್ಕ n);

/** Check whether the node is a border element of a rule. */
bool ಚಿಹ್ನೆ_list_null (ಚಿಹ್ನೆ_list *ಗಂಟು);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
ಶೂನ್ಯ ಚಿಹ್ನೆ_list_code_props_set (ಚಿಹ್ನೆ_list *ಗಂಟು, code_props_type kind,
                                 code_props ಧ್ರುವ *cprops);

#endif /* !SYMLIST_H_ */
