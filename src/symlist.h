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
প্রকারলেখ কাঠামো প্রতীক_list
{
  /**
   * Whether this node contains a symbol, a semantic type, a \c <*>, or a
   * \c <>.
   */
  গণনা {
    SYMLIST_প্রতীক,
    SYMLIST_TYPE
  } content_type;
  সংঘ {
    /**
     * The symbol or \c NULL iff
     * <tt>symbol_list::content_type = SYMLIST_SYMBOL</tt>.
     */
    প্রতীক *sym;
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
  কাঠামো প্রতীক_list *midrule;

  /* If this symbol is the generated lhs for a midrule and this is that
     midrule, MIDRULE_PARENT_RULE = a pointer to the rule in whose rhs it
     appears, and MIDRULE_PARENT_RHS_INDEX = its rhs index (1-origin) in the
     parent rule.  */
  কাঠামো প্রতীক_list *midrule_parent_rule;
  পূর্ণসংখ্যা midrule_parent_rhs_সূচক;

  /*--------------------------------------------------------------.
  | Used for rules only (attached to the "LHS", one per rule even |
  | when several RHSs are bound to a single lhs via "|").         |
  `--------------------------------------------------------------*/

  /* Location of the RHS. */
  location rhs_loc;

  /* Precedence/associativity.  */
  প্রতীক *ruleprec;

  /* The action is attached to the LHS of a rule, but action properties for
   * each RHS are also stored here.  */
  code_props action_props;

  /* The location of the first %empty for this rule, or \a
     empty_loc.  */
  location percent_empty_loc;

  পূর্ণসংখ্যা dprec;
  location dprec_loc;
  পূর্ণসংখ্যা merger;
  location merger_declaration_loc;

  /* Counts of the number of expected conflicts for this rule, or -1 if none
     given. */
  পূর্ণসংখ্যা expected_sr_conflicts;
  পূর্ণসংখ্যা expected_rr_conflicts;

  /* The list.  */
  কাঠামো প্রতীক_list *পরবর্তী;
} প্রতীক_list;


/** Create a list containing \c sym at \c loc.  */
প্রতীক_list *প্রতীক_list_sym_new (প্রতীক *sym, location loc);

/** Create a list containing \c type_name at \c loc.  */
প্রতীক_list *প্রতীক_list_type_new (uniqstr type_name, location loc);

/** Assign the type \c type_name to all the members of \c syms.
 ** \returns \c syms */
প্রতীক_list *প্রতীক_list_type_set (প্রতীক_list *syms, uniqstr type_name);

/** Find a symbol with the same content as \c sym within \c syms.  */
প্রতীক_list *প্রতীক_list_find_প্রতীক (প্রতীক_list *syms, ধ্রুবক প্রতীক *sym);

/** Print this list.

  \pre For every node \c n in the list, <tt>n->content_type =
  SYMLIST_SYMBOL</tt>.  */
শূন্য প্রতীক_list_syms_print (ধ্রুবক প্রতীক_list *l, FILE *f);

/** Prepend \c node to \c list.  */
প্রতীক_list *প্রতীক_list_prepend (প্রতীক_list *list, প্রতীক_list *গ্রন্থি);

/** The last node of this list. */
প্রতীক_list *প্রতীক_list_last (প্রতীক_list *list);

/** Append \c node to \c list.  */
প্রতীক_list *প্রতীক_list_append (প্রতীক_list *list, প্রতীক_list *গ্রন্থি);

/** Free \c list, but not the items it contains.  */
শূন্য প্রতীক_list_free (প্রতীক_list *list);

/** Return the length of \c l. */
পূর্ণসংখ্যা প্রতীক_list_দৈর্ঘ্য (প্রতীক_list ধ্রুবক *l);

/** Get item \c n in symbol list \c l.
 ** \pre  0 <= n
 ** \post res != NULL
 **/
প্রতীক_list *প্রতীক_list_n_get (প্রতীক_list *l, পূর্ণসংখ্যা n);

/** Get the data type (alternative in the union) of the value for
    symbol N in rule RULE.  */
uniqstr প্রতীক_list_n_type_name_get (প্রতীক_list *l, পূর্ণসংখ্যা n);

/** Check whether the node is a border element of a rule. */
bool প্রতীক_list_null (প্রতীক_list *গ্রন্থি);

/** Set the \c \%destructor or \c \%printer for \c node as \c cprops.  */
শূন্য প্রতীক_list_code_props_set (প্রতীক_list *গ্রন্থি, code_props_type kind,
                                 code_props ধ্রুবক *cprops);

#endif /* !SYMLIST_H_ */
