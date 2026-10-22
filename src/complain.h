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
ଶୂନ୍ୟ begin_use_class (ଅଚଳ ଅକ୍ଷର *style, FILE *out);

/** Disable a style on \a out provided it's stderr.  */
ଶୂନ୍ୟ end_use_class (ଅଚଳ ଅକ୍ଷର *style, FILE *out);

/** Flush \a out.  */
ଶୂନ୍ୟ flush (FILE *out);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_ମୂଲ୍ୟଗୁଡ଼ିକ,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
ବାହ୍ୟ bool warnings_are_errors;

/** Document --warning arguments.  */
ଶୂନ୍ୟ warning_usage (FILE *out);

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
ଶୂନ୍ୟ warning_argmatch (ଅକ୍ଷର ଅଚଳ *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
ଶୂନ୍ୟ warnings_argmatch (ଅକ୍ଷର *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
ଶୂନ୍ୟ complain_init (ଶୂନ୍ୟ);

/** Reclaim resources.  */
ଶୂନ୍ୟ complain_free (ଶୂନ୍ୟ);

/** Initialize support for colored messages.  */
ଶୂନ୍ୟ complain_init_color (ଶୂନ୍ୟ);

/** Flags passed to diagnostics functions.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_ମୂଲ୍ୟଗୁଡ଼ିକ   = 1 << warning_midrule_ମୂଲ୍ୟଗୁଡ଼ିକ,
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
ଶୂନ୍ୟ complain (location ଅଚଳ *loc, warnings flags, ଅକ୍ଷର ଅଚଳ *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
ଶୂନ୍ୟ complain_args (location ଅଚଳ *loc, warnings w,
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *arg[]);

/** Make a subcomplain with location and note.  */
ଶୂନ୍ୟ subcomplain (location ଅଚଳ *loc, warnings flags,
                  ଅକ୍ଷର ଅଚଳ *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
ଶୂନ୍ୟ bison_directive (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *directive);

/** Report an obsolete syntax, suggest the updated one.  */
ଶୂନ୍ୟ deprecated_directive (location ଅଚଳ *loc,
                           ଅକ୍ଷର ଅଚଳ *obsolete, ଅକ୍ଷର ଅଚଳ *updated);

/** Report a repeated directive.  */
ଶୂନ୍ୟ duplicate_directive (ଅକ୍ଷର ଅଚଳ *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
ଶୂନ୍ୟ duplicate_rule_directive (ଅକ୍ଷର ଅଚଳ *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
ଶୂନ୍ୟ syntax_error (location loc,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
ବାହ୍ୟ err_status complaint_status;

#endif /* !COMPLAIN_H_ */
