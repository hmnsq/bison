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
શૂન્ય begin_use_class (અચલ અક્ષર *style, FILE *out);

/** Disable a style on \a out provided it's stderr.  */
શૂન્ય end_use_class (અચલ અક્ષર *style, FILE *out);

/** Flush \a out.  */
શૂન્ય flush (FILE *out);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
પ્રકારવ્યાખ્યા ગણના
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_કિંમતો,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
બાહ્ય bool warnings_are_errors;

/** Document --warning arguments.  */
શૂન્ય warning_usage (FILE *out);

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
શૂન્ય warning_argmatch (અક્ષર અચલ *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
શૂન્ય warnings_argmatch (અક્ષર *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
શૂન્ય complain_init (શૂન્ય);

/** Reclaim resources.  */
શૂન્ય complain_free (શૂન્ય);

/** Initialize support for colored messages.  */
શૂન્ય complain_init_color (શૂન્ય);

/** Flags passed to diagnostics functions.  */
પ્રકારવ્યાખ્યા ગણના
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_કિંમતો   = 1 << warning_midrule_કિંમતો,
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
શૂન્ય complain (location અચલ *loc, warnings flags, અક્ષર અચલ *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
શૂન્ય complain_args (location અચલ *loc, warnings w,
                    પૂર્ણાંક argc, અક્ષર *arg[]);

/** Make a subcomplain with location and note.  */
શૂન્ય subcomplain (location અચલ *loc, warnings flags,
                  અક્ષર અચલ *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
શૂન્ય bison_directive (location અચલ *loc, અક્ષર અચલ *directive);

/** Report an obsolete syntax, suggest the updated one.  */
શૂન્ય deprecated_directive (location અચલ *loc,
                           અક્ષર અચલ *obsolete, અક્ષર અચલ *updated);

/** Report a repeated directive.  */
શૂન્ય duplicate_directive (અક્ષર અચલ *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
શૂન્ય duplicate_rule_directive (અક્ષર અચલ *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
શૂન્ય syntax_error (location loc,
                   પૂર્ણાંક argc, અચલ અક્ષર* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
પ્રકારવ્યાખ્યા ગણના
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
બાહ્ય err_status complaint_status;

#endif /* !COMPLAIN_H_ */
