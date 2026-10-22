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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ COMPLAIN_H_
# define COMPLAIN_H_ 1

# include <attribute.h>

# include "location.h"

/*---------------.
| Error stream.  |
`---------------*/

/** Enable a style on \a out provided it's stderr.  */
ಶೂನ್ಯ begin_use_class (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *style, FILE *ಹೊರಹೋಗು);

/** Disable a style on \a out provided it's stderr.  */
ಶೂನ್ಯ ಕೊನೆ_use_class (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *style, FILE *ಹೊರಹೋಗು);

/** Flush \a out.  */
ಶೂನ್ಯ flush (FILE *ಹೊರಹೋಗು);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *ಹೊರಹೋಗು);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_ಖಾಲಿಗಳು_ನಿಯಮ,
    warning_midrule_ಮೌಲ್ಯಗಳು,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_ಗಾತ್ರ           /**< The number of warnings.  Must be last.  */
  } warning_ಬಿಟ್;

/** Whether -Werror was set. */
ಬಾಹ್ಯ bool warnings_are_errors;

/** Document --warning arguments.  */
ಶೂನ್ಯ warning_usage (FILE *ಹೊರಹೋಗು);

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
ಶೂನ್ಯ warning_argmatch (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
ಶೂನ್ಯ warnings_argmatch (ಅಕ್ಷರ *ವಾದಗಳು);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
ಶೂನ್ಯ complain_init (ಶೂನ್ಯ);

/** Reclaim resources.  */
ಶೂನ್ಯ complain_free (ಶೂನ್ಯ);

/** Initialize support for colored messages.  */
ಶೂನ್ಯ complain_init_color (ಶೂನ್ಯ);

/** Flags passed to diagnostics functions.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_ನಿಯಮ       = 1 << warning_ಖಾಲಿಗಳು_ನಿಯಮ,
    Wmidrule_ಮೌಲ್ಯಗಳು   = 1 << warning_midrule_ಮೌಲ್ಯಗಳು,
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
ಶೂನ್ಯ complain (location ಸ್ಥಿರಾಂಕ *loc, warnings flags, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
ಶೂನ್ಯ complain_ವಾದಗಳು (location ಸ್ಥಿರಾಂಕ *loc, warnings w,
                    ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *arg[]);

/** Make a subcomplain with location and note.  */
ಶೂನ್ಯ subcomplain (location ಸ್ಥಿರಾಂಕ *loc, warnings flags,
                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
ಶೂನ್ಯ bison_directive (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive);

/** Report an obsolete syntax, suggest the updated one.  */
ಶೂನ್ಯ deprecated_directive (location ಸ್ಥಿರಾಂಕ *loc,
                           ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *obsolete, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *updated);

/** Report a repeated directive.  */
ಶೂನ್ಯ duplicate_directive (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive,
                          location ಮೊದಲ, location ಎರಡನೇ);

/** Report a repeated directive for a rule.  */
ಶೂನ್ಯ duplicate_ನಿಯಮ_directive (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive,
                               location ಮೊದಲ, location ಎರಡನೇ);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
ಶೂನ್ಯ syntax_error (location loc,
                   ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    ಸ್ಥಿತಿ_none,             /**< No diagnostic issued so far.  */
    ಸ್ಥಿತಿ_warning_as_error, /**< A warning was issued (but no error).  */
    ಸ್ಥಿತಿ_complaint         /**< An error was issued.  */
  } err_ಸ್ಥಿತಿ;

/** Whether an error was reported.  */
ಬಾಹ್ಯ err_ಸ್ಥಿತಿ complaint_ಸ್ಥಿತಿ;

#ಆದರೆ_ಅಂತ್ಯ /* !COMPLAIN_H_ */
