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
வெறுமை begin_use_class (மாறிலி எழுத்து *style, FILE *out);

/** Disable a style on \a out provided it's stderr.  */
வெறுமை end_use_class (மாறிலி எழுத்து *style, FILE *out);

/** Flush \a out.  */
வெறுமை flush (FILE *out);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
வகைவரை எண்ணிக்கை
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_மதிப்புகள்,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
வெளி bool warnings_are_errors;

/** Document --warning arguments.  */
வெறுமை warning_usage (FILE *out);

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
வெறுமை warning_argmatch (எழுத்து மாறிலி *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
வெறுமை warnings_argmatch (எழுத்து *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
வெறுமை complain_init (வெறுமை);

/** Reclaim resources.  */
வெறுமை complain_free (வெறுமை);

/** Initialize support for colored messages.  */
வெறுமை complain_init_color (வெறுமை);

/** Flags passed to diagnostics functions.  */
வகைவரை எண்ணிக்கை
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_மதிப்புகள்   = 1 << warning_midrule_மதிப்புகள்,
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
வெறுமை complain (location மாறிலி *loc, warnings flags, எழுத்து மாறிலி *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
வெறுமை complain_args (location மாறிலி *loc, warnings w,
                    முழு argc, எழுத்து *arg[]);

/** Make a subcomplain with location and note.  */
வெறுமை subcomplain (location மாறிலி *loc, warnings flags,
                  எழுத்து மாறிலி *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
வெறுமை bison_directive (location மாறிலி *loc, எழுத்து மாறிலி *directive);

/** Report an obsolete syntax, suggest the updated one.  */
வெறுமை deprecated_directive (location மாறிலி *loc,
                           எழுத்து மாறிலி *obsolete, எழுத்து மாறிலி *updated);

/** Report a repeated directive.  */
வெறுமை duplicate_directive (எழுத்து மாறிலி *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
வெறுமை duplicate_rule_directive (எழுத்து மாறிலி *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
வெறுமை syntax_error (location loc,
                   முழு argc, மாறிலி எழுத்து* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
வகைவரை எண்ணிக்கை
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
வெளி err_status complaint_status;

#endif /* !COMPLAIN_H_ */
