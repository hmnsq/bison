/* Declaration for error-reporting function for Bison.

   Copyright (C) 2000-2002, 2006, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#अगर_अघोषित COMPLAIN_H_
# define COMPLAIN_H_ 1

# include <attribute.h>

# include "location.h"

/*---------------.
| Error stream.  |
`---------------*/

/** Enable a style on \a out provided it's stderr.  */
व्योम begin_use_class (स्थिर अक्षर *style, खाता *out);

/** Disable a style on \a out provided it's stderr.  */
व्योम end_use_class (स्थिर अक्षर *style, खाता *out);

/** Flush \a out.  */
व्योम flush (खाता *out);

/** Whether there's styling on OUT.  */
bool is_styled (खाता *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
प्रकार क्रमागत
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_मूल्यावली,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_आकार           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
बाह्य bool warnings_are_errors;

/** Document --warning arguments.  */
व्योम warning_usage (खाता *out);

/** Decode a single argument from -W.
 *
 *  \param arg      the subarguments to decode.
 *                  If null, then activate all the flags.
 *  \param no       length of the potential "no-" prefix.
 *                  Can be 0 or 3. If 3, negate the action of the subargument.
 *  \param err      length of a potential "error=".
 *                  Can be 0 or 6. If 6, treat the subargument as a CATEGORY.
 *
 *  If VALUE != 0 then KEY sets flags and no-KEY clears them.
 *  If VALUE == 0 then KEY clears all flags from \c all and no-KEY sets all
 *  flags from \c all.  Thus no-none = all and no-all = none.
 */
व्योम warning_argmatch (अक्षर स्थिर *arg, माप_प्रकार no, माप_प्रकार err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
व्योम warnings_argmatch (अक्षर *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
व्योम complain_init (व्योम);

/** Reclaim resources.  */
व्योम complain_free (व्योम);

/** Initialize support for colored messages.  */
व्योम complain_init_color (व्योम);

/** Flags passed to diagnostics functions.  */
प्रकार क्रमागत
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_मूल्यावली   = 1 << warning_midrule_मूल्यावली,
    Wother            = 1 << warning_other,
    Wprecedence       = 1 << warning_precedence,
    Wyacc             = 1 << warning_yacc,

    complaint         = 1 << 11, /**< All complaints.  */
    fatal             = 1 << 12, /**< All fatal errors.  */
    silent            = 1 << 13, /**< Do not display the warning type.  */
    no_caret          = 1 << 14, /**< Do not display caret location.  */
    note              = 1 << 15, /**< Display as a note.  */

    /**< All above warnings.  */
    Weverything       = ~complaint & ~fatal & ~silent,
    Wall              = Weverything & ~Wcounterexamples & ~Wdangling_alias & ~Wyacc
  } warnings;

/** Whether the warnings of \a flags are all unset.
    (Never enabled, never disabled). */
bool warning_is_unset (warnings flags);

/** Whether warnings of \a flags should be reported. */
bool warning_is_enabled (warnings flags);

/** Make a complaint, with maybe a location.  */
व्योम complain (location स्थिर *loc, warnings flags, अक्षर स्थिर *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
व्योम complain_args (location स्थिर *loc, warnings w,
                    पूर्णांक argc, अक्षर *arg[]);

/** Make a subcomplain with location and note.  */
व्योम subcomplain (location स्थिर *loc, warnings flags,
                  अक्षर स्थिर *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
व्योम bison_directive (location स्थिर *loc, अक्षर स्थिर *directive);

/** Report an obsolete syntax, suggest the updated one.  */
व्योम deprecated_directive (location स्थिर *loc,
                           अक्षर स्थिर *obsolete, अक्षर स्थिर *updated);

/** Report a repeated directive.  */
व्योम duplicate_directive (अक्षर स्थिर *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
व्योम duplicate_rule_directive (अक्षर स्थिर *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
व्योम syntax_error (location loc,
                   पूर्णांक argc, स्थिर अक्षर* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
प्रकार क्रमागत
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
बाह्य err_status complaint_status;

#पूर्ण_अगर /* !COMPLAIN_H_ */
