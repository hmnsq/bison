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
ശൂന്യം begin_use_class (സ്ഥിരം അക്ഷരം *style, FILE *out);

/** Disable a style on \a out provided it's stderr.  */
ശൂന്യം end_use_class (സ്ഥിരം അക്ഷരം *style, FILE *out);

/** Flush \a out.  */
ശൂന്യം flush (FILE *out);

/** Whether there's styling on OUT.  */
bool is_styled (FILE *out);

/*-------------.
| --warnings.  |
`-------------*/

/** The bits assigned to each warning type.  */
തരനിർവചനം എണ്ണൽ
  {
    warning_conflicts_rr,
    warning_conflicts_sr,
    warning_counterexamples,
    warning_dangling_alias,
    warning_deprecated,
    warning_empty_rule,
    warning_midrule_മൂല്യങ്ങൾ,
    warning_other,
    warning_precedence,
    warning_yacc,           /**< POSIXME.  */

    warnings_size           /**< The number of warnings.  Must be last.  */
  } warning_bit;

/** Whether -Werror was set. */
ബാഹ്യ bool warnings_are_errors;

/** Document --warning arguments.  */
ശൂന്യം warning_usage (FILE *out);

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
ശൂന്യം warning_argmatch (അക്ഷരം സ്ഥിരം *arg, size_t no, size_t err);

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */
ശൂന്യം warnings_argmatch (അക്ഷരം *args);


/*-----------.
| complain.  |
`-----------*/

/** Initialize this module.  */
ശൂന്യം complain_init (ശൂന്യം);

/** Reclaim resources.  */
ശൂന്യം complain_free (ശൂന്യം);

/** Initialize support for colored messages.  */
ശൂന്യം complain_init_color (ശൂന്യം);

/** Flags passed to diagnostics functions.  */
തരനിർവചനം എണ്ണൽ
  {
    Wnone             = 0,       /**< Issue no warnings.  */

    Wconflicts_rr     = 1 << warning_conflicts_rr,
    Wconflicts_sr     = 1 << warning_conflicts_sr,
    Wcounterexamples  = 1 << warning_counterexamples,
    Wdangling_alias   = 1 << warning_dangling_alias,
    Wdeprecated       = 1 << warning_deprecated,
    Wempty_rule       = 1 << warning_empty_rule,
    Wmidrule_മൂല്യങ്ങൾ   = 1 << warning_midrule_മൂല്യങ്ങൾ,
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
ശൂന്യം complain (location സ്ഥിരം *loc, warnings flags, അക്ഷരം സ്ഥിരം *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));

/** Likewise, but with an \a argc/argv interface.  */
ശൂന്യം complain_args (location സ്ഥിരം *loc, warnings w,
                    പൂർണ്ണസംഖ്യ argc, അക്ഷരം *arg[]);

/** Make a subcomplain with location and note.  */
ശൂന്യം subcomplain (location സ്ഥിരം *loc, warnings flags,
                  അക്ഷരം സ്ഥിരം *message, ...)
  ATTRIBUTE_FORMAT ((__printf__, 3, 4));


/** GNU Bison extension not valid with POSIX Yacc.  */
ശൂന്യം bison_directive (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *directive);

/** Report an obsolete syntax, suggest the updated one.  */
ശൂന്യം deprecated_directive (location സ്ഥിരം *loc,
                           അക്ഷരം സ്ഥിരം *obsolete, അക്ഷരം സ്ഥിരം *updated);

/** Report a repeated directive.  */
ശൂന്യം duplicate_directive (അക്ഷരം സ്ഥിരം *directive,
                          location first, location second);

/** Report a repeated directive for a rule.  */
ശൂന്യം duplicate_rule_directive (അക്ഷരം സ്ഥിരം *directive,
                               location first, location second);

/** Report a syntax error, where argv[0] is the unexpected
    token, and argv[1...argc] are the expected ones.  */
ശൂന്യം syntax_error (location loc,
                   പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം* argv[]);

/** Warnings treated as errors shouldn't stop the execution as regular
    errors should (because due to their nature, it is safe to go
    on). Thus, there are three possible execution statuses.  */
തരനിർവചനം എണ്ണൽ
  {
    status_none,             /**< No diagnostic issued so far.  */
    status_warning_as_error, /**< A warning was issued (but no error).  */
    status_complaint         /**< An error was issued.  */
  } err_status;

/** Whether an error was reported.  */
ബാഹ്യ err_status complaint_status;

#endif /* !COMPLAIN_H_ */
