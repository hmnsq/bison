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

#ifndef COMPLAIN_H_
# define COMPLAIN_H_ 1

# include <attribute.h>

# include "location.h"

/*---------------.
| Error stream.  |
`---------------*/

/** Enable a style on \a out provided it's stderr.  */
शून्य begin_use_class (ध्रुव वर्ण *style, FILE *out);

/** Disable a style on \a out provided it's stderr.  */
शून्य end_use_class (ध्रुव वर्ण *style, FILE *out);

/** Flush \a out.  */
शून्य flush (FILE *out);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
प्रकारनिर्देश गणना
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_मानानि,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
बाह्य bool warnings_are_errors;

/** Document --warning arguments.  */
शून्य warning_usage (FILE *out);

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
शून्य warning_argmatch (वर्ण ध्रुव *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
शून्य warnings_argmatch (वर्ण *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
शून्य complain_init (शून्य);

/** Reclaim resources.  */
शून्य complain_free (शून्य);

/** Initialize support for colored messages.  */
शून्य complain_init_color (शून्य);

/** Flags passed to diagnostics functions.  */
प्रकारनिर्देश गणना
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_मानानि   = 1 << warning_midrule_मानानि,
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
शून्य complain (location ध्रुव *loc, warnings flags, वर्ण ध्रुव *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
शून्य complain_args (location ध्रुव *loc, warnings w,
                    पूर्णाङ्क argc, वर्ण *arg[]);

/** Make a subcomplain with location and note.  */
शून्य subcomplain (location ध्रुव *loc, warnings flags,
                  वर्ण ध्रुव *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
शून्य bison_directive (location ध्रुव *loc, वर्ण ध्रुव *directive);

/** Report an obsolete syntax, suggest the updated one.  */
शून्य deprecated_directive (location ध्रुव *loc,
                           वर्ण ध्रुव *obsolete, वर्ण ध्रुव *updated);

/** Report a repeated directive.  */
शून्य duplicate_directive (वर्ण ध्रुव *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
शून्य duplicate_rule_directive (वर्ण ध्रुव *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
शून्य syntax_error (location loc,
                   पूर्णाङ्क argc, ध्रुव वर्ण* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
प्रकारनिर्देश गणना
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
बाह्य err_status complaint_status;

#endif /* !COMPLAIN_H_ */
