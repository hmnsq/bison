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
પ્રકારવ્યાખ્યા સંરચના પ્રતીક_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  ગણના {
    SYMLIST_પ્રતીક,
    SYMLIST_TYPE
  } content_type;
  સંઘ {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    પ્રતીક *sym;
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
  સંરચના પ્રતીક_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  સંરચના પ્રતીક_list *midrule_parent_rule;
  પૂર્ણાંક midrule_parent_rhs_index;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  પ્રતીક *ruleprec;

  /* The action is attached to the LHS of a rule, but action properties for
   * each RHS are also stored here.  */
  code_props action_props;

  /* The location of the first %empty for this rule, or \a
     empty_loc.  */
  location percent_empty_loc;

  પૂર્ણાંક dprec;
  location dprec_loc;
  પૂર્ણાંક merger;
  location merger_declaration_loc;

  /* Counts of the number of expected conflicts for this rule, or -1 if none
     given. */
  પૂર્ણાંક expected_sr_conflicts;
  પૂર્ણાંક expected_rr_conflicts;

  /* The list.  */
  સંરચના પ્રતીક_list *next;
} પ્રતીક_list;


/** Create a list containing \c sym at \c loc.  */
પ્રતીક_list *પ્રતીક_list_sym_new (પ્રતીક *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
પ્રતીક_list *પ્રતીક_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
પ્રતીક_list *પ્રતીક_list_type_set (પ્રતીક_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
પ્રતીક_list *પ્રતીક_list_find_પ્રતીક (પ્રતીક_list *syms, અચલ પ્રતીક *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
શૂન્ય પ્રતીક_list_syms_print (અચલ પ્રતીક_list *l, FILE *f);

/** Prepend \c node to \c list.  */
પ્રતીક_list *પ્રતીક_list_prepend (પ્રતીક_list *list, પ્રતીક_list *ગાંઠ);

/** The last node of this list. */
પ્રતીક_list *પ્રતીક_list_last (પ્રતીક_list *list);

/** Append \c node to \c list.  */
પ્રતીક_list *પ્રતીક_list_append (પ્રતીક_list *list, પ્રતીક_list *ગાંઠ);

/** Free \c list, but not the items it contains.  */
શૂન્ય પ્રતીક_list_free (પ્રતીક_list *list);

/** Return the length of \c l. */
પૂર્ણાંક પ્રતીક_list_length (પ્રતીક_list અચલ *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
પ્રતીક_list *પ્રતીક_list_n_get (પ્રતીક_list *l, પૂર્ણાંક n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr પ્રતીક_list_n_type_name_get (પ્રતીક_list *l, પૂર્ણાંક n);

/** Check whether the node is a border element of a rule. */
bool પ્રતીક_list_null (પ્રતીક_list *ગાંઠ);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
શૂન્ય પ્રતીક_list_code_props_set (પ્રતીક_list *ગાંઠ, code_props_type kind,
                                 code_props અચલ *cprops);

#endif /* !SYMLIST_H_ */
