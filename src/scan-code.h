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

#यदि_अपरिभाषितम् अवलोकनम्_संहिता_H
# define अवलोकनम्_संहिता_H

# include "location.h"
# include "named-ref.h"
# include "uniqstr.h"

संरचना चिह्नम्_list;

/**
 * Keeps track of the maximum number of semantic values to the left of a handle
 * (those referenced by $0, $-1, etc.) that are required by the semantic
 * actions of this grammar.
 */
बाह्य पूर्णाङ्क max_वामः_semantic_context;

/**
 * The obstack used to store the translated actions.
 */
बाह्य संरचना obstack *obstack_for_actions;

/**
 * A code passage captured from the grammar file and possibly translated,
 * and/or properties associated with such a code passage.  Don't break
 * encapsulation by modifying the fields directly.  Use the provided interface
 * functions.
 */
प्रकारनिर्देश संरचना संहिता_props {
  /** Set by the init functions.  */
  गणना {
    संहिता_PROPS_NONE, संहिता_PROPS_PLAIN,
    संहिता_PROPS_चिह्नम्_ACTION, संहिता_PROPS_नियमः_ACTION
  } विधा;

  /**
   * \c NULL iff \c code_props::kind is \c CODE_PROPS_NONE.
   * Memory is allocated in an obstack freed elsewhere.
   */
  वर्ण ध्रुव *संहिता;
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
  bool is_मानम्_प्रयुक्तम्;

  /**
   * \c true iff this code is an action that is not to be deferred in
   * a non-deterministic parser.
   */
  bool is_predicate;

  /**
   * Whether this is actually used (i.e., not completely masked by
   * other code props).  */
  bool is_प्रयुक्तम्;

  /** \c NULL iff \c code_props::kind is not \c CODE_PROPS_RULE_ACTION.  */
  संरचना चिह्नम्_list *नियमः;

  /** Named reference. */
  named_ref *named_ref;

  /** Type, for midrule actions.  */
  uniqstr type;
} संहिता_props;

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 * \post
 *   - \c self has been overwritten to contain no code.
 */
शून्य संहिता_props_none_init (संहिता_props *self);

/** Equivalent to \c code_props_none_init.  */
# define संहिता_PROPS_NONE_INIT                   \
  {                                             \
    /* .kind = */ संहिता_PROPS_NONE,              \
    /* .code = */ रिक्त,                         \
    /* .location = */ रिक्तानि_LOCATION_INIT,      \
    /* .is_value_used = */ false,               \
    /* .is_predicate = */ false,                \
    /* .is_used = */ false,                     \
    /* .rule = */ रिक्त,                         \
    /* .named_ref = */ रिक्त,                    \
    /* .type = */ रिक्त,                         \
  }

/** Initialized by \c CODE_PROPS_NONE_INIT with no further modification.  */
बाह्य संहिता_props संहिता_props_none;

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
शून्य संहिता_props_plain_init (संहिता_props *self, वर्ण ध्रुव *संहिता,
                            location संहिता_loc);

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
शून्य संहिता_props_चिह्नम्_action_init (संहिता_props *self, वर्ण ध्रुव *संहिता,
                                    location संहिता_loc);

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
शून्य संहिता_props_नियमः_action_init (संहिता_props *self, वर्ण ध्रुव *संहिता,
                                  location संहिता_loc, संरचना चिह्नम्_list *नियमः,
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
शून्य संहिता_props_translate_संहिता (संहिता_props *self);

/**
 * \pre
 *   - None.
 * \post
 *   - The dynamic memory allocated by the previous invocation of
 *     \c code_props_translate_code (if any) was freed.  The \c code_props
 *     instance for which \c code_props_translate_code was invoked is now
 *     invalid.
 */
शून्य संहिता_scanner_चरमः_string_free (शून्य);

शून्य संहिता_scanner_init (शून्य);

/**
 * \pre
 *   - None.
 * \post
 *   - All dynamic memory allocated during invocations of
 *     \c code_props_translate_code (if any) has been freed.  All \c code_props
 *     instances may now be invalid.
 */
शून्य संहिता_scanner_free (शून्य);

#यदि_अन्तः /* !SCAN_CODE_H_ */
