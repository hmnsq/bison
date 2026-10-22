/* Bison code properties structure and scanner.

   Copyright (C) 2006-2007, 2009-2015, 2018-2022 Free Software
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

#ifndef SCAN_CODE_H_
# define SCAN_CODE_H_

# include "location.h"
# include "named-ref.h"
# include "uniqstr.h"

ಸಂರಚನಾ ಚಿಹ್ನೆ_list;

/**
 * Keeps track of the maximum number of semantic values to the left of a handle
 * (those referenced by $0, $-1, etc.) that are required by the semantic
 * actions of this grammar.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ max_ಎಡ_semantic_context;

/**
 * The obstack used to store the translated actions.
 */
ಬಾಹ್ಯ ಸಂರಚನಾ obstack *obstack_for_actions;

/**
 * A code passage captured from the grammar file and possibly translated,
 * and/or properties associated with such a code passage.  Don't break
 * encapsulation by modifying the fields directly.  Use the provided interface
 * functions.
 */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ code_props {
  /** Set by the init functions.  */
  ಗಣನಾ {
    CODE_PROPS_NONE, CODE_PROPS_PLAIN,
    CODE_PROPS_ಚಿಹ್ನೆ_ACTION, CODE_PROPS_RULE_ACTION
  } kind;

  /**
   * \c NULL iff \c code_props::kind is \c CODE_PROPS_NONE.
   * Memory is allocated in an obstack freed elsewhere.
   */
  ವರ್ಣ ಧ್ರುವ *code;
  /** Undefined iff \c code_props::code is \c NULL.  */
  location location;

  /**
   * \c false iff either:
   *   - \c code_props_translate_code has never previously been invoked for
   *     the \c code_props that would contain the code passage associated
   *     with \c self.  (That \c code_props is not the same as this one if this
   *     one is for a RHS \c symbol_list node.  Instead, it's the \c code_props
   *     for the LHS symbol of the same rule.)
   *   - \c code_props_translate_code has been invoked for that \c code_props,
   *     but the symbol value associated with this \c code_props was not
   *     referenced in the code passage.
   */
  bool is_ಮೌಲ್ಯ_used;

  /**
   * \c true iff this code is an action that is not to be deferred in
   * a non-deterministic parser.
   */
  bool is_predicate;

  /**
   * Whether this is actually used (i.e., not completely masked by
   * other code props).  */
  bool is_used;

  /** \c NULL iff \c code_props::kind is not \c CODE_PROPS_RULE_ACTION.  */
  ಸಂರಚನಾ ಚಿಹ್ನೆ_list *rule;

  /** Named reference. */
  named_ref *named_ref;

  /** Type, for midrule actions.  */
  uniqstr type;
} code_props;

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 * \post
 *   - \c self has been overwritten to contain no code.
 */
ಶೂನ್ಯ code_props_none_init (code_props *self);

/** Equivalent to \c code_props_none_init.  */
# define CODE_PROPS_NONE_INIT                   \
  {                                             \
    /* .kind = */ CODE_PROPS_NONE,              \
    /* .code = */ ರಿಕ್ತ,                         \
    /* .location = */ EMPTY_LOCATION_INIT,      \
    /* .is_value_used = */ false,               \
    /* .is_predicate = */ false,                \
    /* .is_used = */ false,                     \
    /* .rule = */ ರಿಕ್ತ,                         \
    /* .named_ref = */ ರಿಕ್ತ,                    \
    /* .type = */ ರಿಕ್ತ,                         \
  }

/** Initialized by \c CODE_PROPS_NONE_INIT with no further modification.  */
ಬಾಹ್ಯ code_props code_props_none;

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>code != NULL</tt>.
 *   - \c code is an untranslated code passage containing no Bison escapes.
 *   - \c code was extracted from the grammar file at \c code_loc.
 * \post
 *   - \c self has been overwritten to represent the specified plain code
 *     passage.
 *   - \c self will become invalid if the caller frees \c code before invoking
 *     \c code_props_translate_code on \c self.
 */
ಶೂನ್ಯ code_props_plain_init (code_props *self, ವರ್ಣ ಧ್ರುವ *code,
                            location code_loc);

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>code != NULL</tt>.
 *   - \c code is an untranslated code passage.  The only Bison escapes it
 *     might contain are $$ and \@$, referring to a single symbol.
 *   - \c code was extracted from the grammar file at \c code_loc.
 * \post
 *   - \c self has been overwritten to represent the specified symbol action.
 *   - \c self will become invalid if the caller frees \c code before invoking
 *     \c code_props_translate_code on \c self.
 */
ಶೂನ್ಯ code_props_ಚಿಹ್ನೆ_action_init (code_props *self, ವರ್ಣ ಧ್ರುವ *code,
                                    location code_loc);

/**
 * \param  type   type for midrule actions
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>code != NULL</tt>.
 *   - <tt>rule != NULL</tt>.
 *   - \c code is the untranslated action of the rule for which \c rule is the
 *     LHS node.  Thus, \c code possibly contains Bison escapes such as $$, $1,
 *     $2, etc referring to the values of the rule.
 *   - \c code was extracted from the grammar file at \c code_loc.
 * \post
 *   - \c self has been overwritten to represent the specified rule action.
 *   - \c self does not claim responsibility for the memory of \c rule.
 *   - \c self will become invalid if:
 *     - The caller frees \c code before invoking \c code_props_translate_code
 *       on \c self.
 *     - The caller frees \c rule.
 */
ಶೂನ್ಯ code_props_rule_action_init (code_props *self, ವರ್ಣ ಧ್ರುವ *code,
                                  location code_loc, ಸಂರಚನಾ ಚಿಹ್ನೆ_list *rule,
                                  named_ref *name, uniqstr type,
                                  bool is_predicate);

/**
 * \pre
 *   - If there's a code passage contained in \c self and it contains Bison
 *     escapes, all grammar declarations have already been parsed as they may
 *     affect warnings and complaints issued here.
 * \post
 *   - All M4-special symbols and Bison escapes have been translated in
 *     \c self->code.
 *   - <tt>self->code != self->code\@pre</tt> unless
 *     <tt>self->code\@pre = NULL</tt>.
 */
ಶೂನ್ಯ code_props_translate_code (code_props *self);

/**
 * \pre
 *   - None.
 * \post
 *   - The dynamic memory allocated by the previous invocation of
 *     \c code_props_translate_code (if any) was freed.  The \c code_props
 *     instance for which \c code_props_translate_code was invoked is now
 *     invalid.
 */
ಶೂನ್ಯ code_scanner_last_string_free (ಶೂನ್ಯ);

ಶೂನ್ಯ code_scanner_init (ಶೂನ್ಯ);

/**
 * \pre
 *   - None.
 * \post
 *   - All dynamic memory allocated during invocations of
 *     \c code_props_translate_code (if any) has been freed.  All \c code_props
 *     instances may now be invalid.
 */
ಶೂನ್ಯ code_scanner_free (ಶೂನ್ಯ);

#endif /* !SCAN_CODE_H_ */
