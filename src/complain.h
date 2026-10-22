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
শূন্য begin_use_class (ধ্রুব বর্ণ *style, FILE *out);

/** Disable a style on \a out provided it's stderr.  */
শূন্য end_use_class (ধ্রুব বর্ণ *style, FILE *out);

/** Flush \a out.  */
শূন্য flush (FILE *out);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
প্রকারনির্দেশ গণনা
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_মানসমূহ,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
বাহ্য bool warnings_are_errors;

/** Document --warning arguments.  */
শূন্য warning_usage (FILE *out);

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
শূন্য warning_argmatch (বর্ণ ধ্রুব *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
শূন্য warnings_argmatch (বর্ণ *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
শূন্য complain_init (শূন্য);

/** Reclaim resources.  */
শূন্য complain_free (শূন্য);

/** Initialize support for colored messages.  */
শূন্য complain_init_color (শূন্য);

/** Flags passed to diagnostics functions.  */
প্রকারনির্দেশ গণনা
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_মানসমূহ   = 1 << warning_midrule_মানসমূহ,
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
শূন্য complain (location ধ্রুব *loc, warnings flags, বর্ণ ধ্রুব *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
শূন্য complain_args (location ধ্রুব *loc, warnings w,
                    পূর্ণাঙ্ক argc, বর্ণ *arg[]);

/** Make a subcomplain with location and note.  */
শূন্য subcomplain (location ধ্রুব *loc, warnings flags,
                  বর্ণ ধ্রুব *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
শূন্য bison_directive (location ধ্রুব *loc, বর্ণ ধ্রুব *directive);

/** Report an obsolete syntax, suggest the updated one.  */
শূন্য deprecated_directive (location ধ্রুব *loc,
                           বর্ণ ধ্রুব *obsolete, বর্ণ ধ্রুব *updated);

/** Report a repeated directive.  */
শূন্য duplicate_directive (বর্ণ ধ্রুব *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
শূন্য duplicate_rule_directive (বর্ণ ধ্রুব *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
শূন্য syntax_error (location loc,
                   পূর্ণাঙ্ক argc, ধ্রুব বর্ণ* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
প্রকারনির্দেশ গণনা
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
বাহ্য err_status complaint_status;

#endif /* !COMPLAIN_H_ */
