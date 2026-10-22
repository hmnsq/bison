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

#جيڪڏهن_وصف_نه_ٿيل COMPLAIN_H_
# define COMPLAIN_H_ 1

# include <attribute.h>

# include "location.h"

/*---------------.
| Error stream.  |
`---------------*/

/** Enable a style on \a out provided it's stderr.  */
فراغ begin_use_class (ثابت حرف *style, FILE *ٻاهريون);

/** Disable a style on \a out provided it's stderr.  */
فراغ پڇاڙي_use_class (ثابت حرف *style, FILE *ٻاهريون);

/** Flush \a out.  */
فراغ flush (FILE *ٻاهريون);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *ٻاهريون);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
تعريفنوع تعداد
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_خالي_جايون_قاعدو,
    warning_midrule_قدرون,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_سائيز           /**< The number of warnings.  Must be last.  */
  } warning_ٻٽو_انگ;

/** Whether -Werror was set. */
خارجي bool warnings_are_errors;

/** Document --warning arguments.  */
فراغ warning_usage (FILE *ٻاهريون);

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
فراغ warning_argmatch (حرف ثابت *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
فراغ warnings_argmatch (حرف *دليلون);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
فراغ complain_init (فراغ);

/** Reclaim resources.  */
فراغ complain_free (فراغ);

/** Initialize support for colored messages.  */
فراغ complain_init_color (فراغ);

/** Flags passed to diagnostics functions.  */
تعريفنوع تعداد
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_قاعدو       = 1 << warning_خالي_جايون_قاعدو,
    Wmidrule_قدرون   = 1 << warning_midrule_قدرون,
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
فراغ complain (location ثابت *loc, warnings flags, حرف ثابت *پيغام, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
فراغ complain_دليلون (location ثابت *loc, warnings w,
                    صحيح argc, حرف *arg[]);

/** Make a subcomplain with location and note.  */
فراغ subcomplain (location ثابت *loc, warnings flags,
                  حرف ثابت *پيغام, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
فراغ bison_directive (location ثابت *loc, حرف ثابت *directive);

/** Report an obsolete syntax, suggest the updated one.  */
فراغ deprecated_directive (location ثابت *loc,
                           حرف ثابت *obsolete, حرف ثابت *updated);

/** Report a repeated directive.  */
فراغ duplicate_directive (حرف ثابت *directive,
                          location پهريون, location ٻيو);

/** Report a repeated directive for a rule.  */
فراغ duplicate_قاعدو_directive (حرف ثابت *directive,
                               location پهريون, location ٻيو);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
فراغ syntax_error (location loc,
                   صحيح argc, ثابت حرف* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
تعريفنوع تعداد
  {
    حالت_none,             /**< No diagnostic issued so far.  */
    حالت_warning_as_error, /**< A warning was issued (but no error).  */
    حالت_complaint         /**< An error was issued.  */
  } err_حالت;

/** Whether an error was reported.  */
خارجي err_حالت complaint_حالت;

#جيڪڏهن_پورو /* !COMPLAIN_H_ */
