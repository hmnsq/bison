/* Declaration for error-reporting function for Bison.

   Copyright (C) 2000-2002, 2004-2006, 2009-2015, 2018-2022 Free
   Software Foundation, Inc.

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

/* Based on error.c and error.h,
   written by David MacKenzie <djm@gnu.ai.mit.edu>.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <argmatch.h>
#ಅನ್ತರ್ಭಾವ <c-ctype.h>
#ಅನ್ತರ್ಭಾವ <progname.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <textstyle.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "fixits.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_ಸ್ಥಿತಿ complaint_ಸ್ಥಿತಿ = ಸ್ಥಿತಿ_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
ಸ್ಥಿರ errority errority_flag[warnings_ಗಾತ್ರ];

/** Diagnostics severity.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
ಸ್ಥಿರ severity warnings_flag[warnings_ಗಾತ್ರ];

styled_ostream_t errstream = ಖಾಲಿ;

ಶೂನ್ಯ
begin_use_class (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (ಹೊರಹೋಗು == stderr)
    {
      ಆದರೆ (color_debug)
        fprintf (ಹೊರಹೋಗು, "<%s>", s);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_ಗಮ್ಯ_current_style (errstream);
        }
    }
}

ಶೂನ್ಯ
ಕೊನೆ_use_class (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (ಹೊರಹೋಗು == stderr)
    {
      ಆದರೆ (color_debug)
        fprintf (ಹೊರಹೋಗು, "</%s>", s);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          styled_ostream_ಕೊನೆ_use_class (errstream, s);
          styled_ostream_flush_ಗಮ್ಯ_current_style (errstream);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
begin_hyperlink (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref)
{
  ಆದರೆ (ಹೊರಹೋಗು == stderr)
    styled_ostream_ಗಣ_hyperlink (errstream, ref, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೊನೆ_hyperlink (FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (ಹೊರಹೋಗು == stderr)
    styled_ostream_ಗಣ_hyperlink (errstream, ಖಾಲಿ, ಖಾಲಿ);
}

ಶೂನ್ಯ
flush (FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (ಹೊರಹೋಗು == stderr)
    ostream_flush (errstream, FLUSH_THIS_ಹರಿವು);
  fflush (ಹೊರಹೋಗು);
}

bool
is_styled (FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (ಹೊರಹೋಗು != stderr)
    ಹಿಂತಿರುಗಿ false;
  ಆದರೆ (color_debug)
    ಹಿಂತಿರುಗಿ true;
#ಆದರೆ HAVE_LIBTEXTSTYLE
  ಹಿಂತಿರುಗಿ (color_ಬಹುಲಕ == color_yes
          || color_ಬಹುಲಕ == color_html
          || (color_ಬಹುಲಕ == color_tty && isatty (STDERR_FILENO)));
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_ಗುಂಪು (warning, warnings)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_warning_doc argmatch_warning_docs[] =
{
  { "conflicts-sr",     N_("S/R conflicts (enabled by default)") },
  { "conflicts-rr",     N_("R/R conflicts (enabled by default)") },
  { "counterexamples",  N_("generate conflict counterexamples") },
  { "dangling-alias",   N_("string aliases not attached to a symbol") },
  { "deprecated",       N_("obsolete constructs") },
  { "empty-rule",       N_("empty rules without %empty") },
  { "midrule-values",   N_("unset or unused midrule values") },
  { "precedence",       N_("useless precedence and associativity") },
  { "yacc",             N_("incompatibilities with POSIX Yacc") },
  { "other",            N_("all other warnings (enabled by default)") },
  { "all",              N_("all the warnings except 'counterexamples', 'dangling-alias' and 'yacc'") },
  { "no-CATEGORY",      N_("turn off warnings in CATEGORY") },
  { "none",             N_("turn off all the warnings") },
  { "error[=CATEGORY]", N_("treat warnings as errors") },
  { ಖಾಲಿ, ಖಾಲಿ }
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_warning_arg argmatch_warning_ವಾದಗಳು[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_ನಿಯಮ },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_ಮೌಲ್ಯಗಳು },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { ಖಾಲಿ, Wnone }
};

ಸ್ಥಿರಾಂಕ argmatch_warning_ಗುಂಪು_type argmatch_warning_ಗುಂಪು =
{
  argmatch_warning_ವಾದಗಳು,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  ಖಾಲಿ
};

ಶೂನ್ಯ
warning_usage (FILE *ಹೊರಹೋಗು)
{
  argmatch_warning_usage (ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
warning_argmatch (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, size_t no, size_t err)
{
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = *argmatch_warning_ಮೌಲ್ಯ ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  ಆದರೆ (!ಮೌಲ್ಯ)
    {
      ಮೌಲ್ಯ = Weverything;
      no = !no;
    }

  ಗಾಗಿ (size_t b = 0; b < warnings_ಗಾತ್ರ; ++b)
    ಆದರೆ (ಮೌಲ್ಯ & 1 << b)
      {
        ಆದರೆ (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        ಇಲ್ಲದಿದ್ದರೆ
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

ಶೂನ್ಯ
warnings_argmatch (ಅಕ್ಷರ *ವಾದಗಳು)
{
  ಆದರೆ (!ವಾದಗಳು)
    warning_argmatch ("all", 0, 0);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (ವಾದಗಳು, "help"))
    {
      warning_usage (stdout);
      ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (ವಾದಗಳು = strtok (ವಾದಗಳು, ","); ವಾದಗಳು; ವಾದಗಳು = strtok (ಖಾಲಿ, ","))
      ಆದರೆ (STREQ (ವಾದಗಳು, "error"))
        warnings_are_errors = true;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (ವಾದಗಳು, "no-error"))
        warnings_are_errors = false;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* The length of the possible 'no-' prefix: 3, or 0.  */
          size_t no = STRPREFIX_LIT ("no-", ವಾದಗಳು) ? 3 : 0;
          /* The length of the possible 'error=' (possibly after
             'no-') prefix: 6, or 0. */
          size_t err = STRPREFIX_LIT ("error=", ವಾದಗಳು + no) ? 6 : 0;

          warning_argmatch (ವಾದಗಳು, no, err);
        }
}

/* Color style for this type of message.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*
severity_style (severity s)
{
  ಆಯ್ಕೆ (s)
    {
    ಸ್ಥಿತಿ severity_disabled:
    ಸ್ಥಿತಿ severity_unset:
      ಹಿಂತಿರುಗಿ "note";
    ಸ್ಥಿತಿ severity_warning:
      ಹಿಂತಿರುಗಿ "warning";
    ಸ್ಥಿತಿ severity_error:
    ಸ್ಥಿತಿ severity_fatal:
      ಹಿಂತಿರುಗಿ "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*
severity_ಪೂರ್ವಪ್ರತ್ಯಯ (severity s)
{
  ಆಯ್ಕೆ (s)
    {
    ಸ್ಥಿತಿ severity_disabled:
    ಸ್ಥಿತಿ severity_unset:
      ಹಿಂತಿರುಗಿ "";
    ಸ್ಥಿತಿ severity_warning:
      ಹಿಂತಿರುಗಿ _("warning");
    ಸ್ಥಿತಿ severity_error:
      ಹಿಂತಿರುಗಿ  _("error");
    ಸ್ಥಿತಿ severity_fatal:
      ಹಿಂತಿರುಗಿ _("fatal error");
    }
  abort ();
}


ಸ್ಥಿರ ಶೂನ್ಯ
severity_print (severity s, FILE *ಹೊರಹೋಗು)
{
  ಆದರೆ (s != severity_disabled)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* style = severity_style (s);
      begin_use_class (style, ಹೊರಹೋಗು);
      fprintf (ಹೊರಹೋಗು, "%s:", severity_ಪೂರ್ವಪ್ರತ್ಯಯ (s));
      ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
      fputc (' ', ಹೊರಹೋಗು);
    }
}


/*-----------.
| complain.  |
`-----------*/

ಶೂನ್ಯ
complain_init_color (ಶೂನ್ಯ)
{
#ಆದರೆ HAVE_LIBTEXTSTYLE
  ಆದರೆ (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      ರಚನೆ stat statbuf;
      ಆದರೆ ((style_file_name == ಖಾಲಿ || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  ಇಲ್ಲದಿದ್ದರೆ
    /* No styling.  */
    style_file_name = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ಆದರೆ defined __clang__
# pragma clang diagnostic ತಳ್ಳು
# pragma clang diagnostic ignored "-Wunreachable-code"
#ಆದರೆ_ಅಂತ್ಯ
  errstream =
    color_ಬಹುಲಕ == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#ಆದರೆ defined __clang__
# pragma clang diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ
complain_init (ಶೂನ್ಯ)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  ಗಾಗಿ (size_t b = 0; b < warnings_ಗಾತ್ರ; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

ಶೂನ್ಯ
complain_free (ಶೂನ್ಯ)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

ಸ್ಥಿರ severity
warning_severity (warnings flags)
{
  ಆದರೆ (flags & fatal)
    /* Diagnostics about fatal errors.  */
    ಹಿಂತಿರುಗಿ severity_fatal;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & complaint)
    /* Diagnostics about errors.  */
    ಹಿಂತಿರುಗಿ severity_error;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      ಗಾಗಿ (size_t b = 0; b < warnings_ಗಾತ್ರ; ++b)
        ಆದರೆ (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            ಆದರೆ (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      ಹಿಂತಿರುಗಿ res;
    }
}

bool
warning_is_unset (warnings flags)
{
  ಗಾಗಿ (size_t b = 0; b < warnings_ಗಾತ್ರ; ++b)
    ಆದರೆ (flags & 1 << b && warnings_flag[b] != severity_unset)
      ಹಿಂತಿರುಗಿ false;
  ಹಿಂತಿರುಗಿ true;
}

bool
warning_is_enabled (warnings flags)
{
  ಹಿಂತಿರುಗಿ severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

ಸ್ಥಿರ ಶೂನ್ಯ
warnings_print_categories (warnings warn_flags, FILE *ಹೊರಹೋಗು)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ wbit = 0; wbit < warnings_ಗಾತ್ರ; ++wbit)
    ಆದರೆ (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* style = severity_style (s);
        fputs (" [", ಹೊರಹೋಗು);
        begin_use_class (style, ಹೊರಹೋಗು);
        // E.g., "counterexamples".
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *warning = argmatch_warning_ವಾದ (&w);
        ಅಕ್ಷರ ref[200];
        snprintf (ref, ಗಾತ್ರ ref,
                  "%s#W%s", diagnostics_url, warning);
        begin_hyperlink (ಹೊರಹೋಗು, ref);
        ostream_printf (errstream,
                        "-W%s%s",
                        s == severity_error ? "error=" : "",
                        warning);
        ಕೊನೆ_hyperlink (ಹೊರಹೋಗು);
        // Because we mix stdio with ostream I/O, we need to flush
        // here for sake of color == debug.
        flush (ಹೊರಹೋಗು);
        ಕೊನೆ_use_class (style, ಹೊರಹೋಗು);
        fputc (']', ಹೊರಹೋಗು);
        /* Display only the first match, the second is "-Wall".  */
        ಹಿಂತಿರುಗಿ;
      }
}

/** Report an error message.
 *
 * \param loc     the location, defaulting to the current file,
 *                or the program name.
 * \param flags   the category for this message.
 * \param sever   to decide the prefix to put before the message
 *                (e.g., "warning").
 * \param message the error message, a printf format string.  Iff it
 *                ends with ": ", then no trailing newline is printed,
 *                and the caller should print the remaining
 *                newline-terminated message to stderr.
 * \param args    the arguments of the format string.
 */
ಸ್ಥಿರ
ಶೂನ್ಯ
error_ಸಂದೇಶ (ಸ್ಥಿರಾಂಕ location *loc, warnings flags,
               severity sever, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, va_list ವಾದಗಳು)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* style = flags & note ? "note" : severity_style (sever);

  ಆದರೆ (loc)
    location_print (*loc, stderr);
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (stderr, "%s", grammar_file ? grammar_file : ಕಾರ್ಯಕ್ರಮ_name);
  fprintf (stderr, ": ");

  ಆದರೆ (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_ಪೂರ್ವಪ್ರತ್ಯಯ (sever));
      ಕೊನೆ_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, ಸಂದೇಶ, ವಾದಗಳು);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  ಆದರೆ (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ);
  ಆದರೆ (l < 2 || ಸಂದೇಶ[l - 2] != ':' || ಸಂದೇಶ[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      ಆದರೆ (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

ಸ್ಥಿರ ಶೂನ್ಯ
complains (ಸ್ಥಿರಾಂಕ location *loc, warnings flags,
           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, va_list ವಾದಗಳು)
{
  ಆದರೆ ((flags & complaint) && complaint_ಸ್ಥಿತಿ < ಸ್ಥಿತಿ_complaint)
    complaint_ಸ್ಥಿತಿ = ಸ್ಥಿತಿ_complaint;

  severity s = warning_severity (flags);
  ಆದರೆ (severity_warning <= s)
    {
      ಆದರೆ (severity_error <= s && ! complaint_ಸ್ಥಿತಿ)
        complaint_ಸ್ಥಿತಿ = ಸ್ಥಿತಿ_warning_as_error;
      error_ಸಂದೇಶ (loc, flags, s, ಸಂದೇಶ, ವಾದಗಳು);
    }

  ಆದರೆ (flags & fatal)
    ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

ಶೂನ್ಯ
complain (location ಸ್ಥಿರಾಂಕ *loc, warnings flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು;
  va_ಆರಂಭ (ವಾದಗಳು, ಸಂದೇಶ);
  complains (loc, flags, ಸಂದೇಶ, ವಾದಗಳು);
  va_ಕೊನೆ (ವಾದಗಳು);
}

ಶೂನ್ಯ
subcomplain (location ಸ್ಥಿರಾಂಕ *loc, warnings flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು;
  va_ಆರಂಭ (ವಾದಗಳು, ಸಂದೇಶ);
  complains (loc, flags | note | silent, ಸಂದೇಶ, ವಾದಗಳು);
  va_ಕೊನೆ (ವಾದಗಳು);
}

ಶೂನ್ಯ
complain_ವಾದಗಳು (location ಸ್ಥಿರಾಂಕ *loc, warnings w,
               ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
  ಆಯ್ಕೆ (argc)
  {
  ಸ್ಥಿತಿ 1:
    complain (loc, w, "%s", _(argv[0]));
    ನಿಲ್ಲಿಸು;
  ಸ್ಥಿತಿ 2:
    complain (loc, w, _(argv[0]), argv[1]);
    ನಿಲ್ಲಿಸು;
  ಸ್ಥಿತಿ 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    ನಿಲ್ಲಿಸು;
  ಸ್ಥಿತಿ 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    ನಿಲ್ಲಿಸು;
  ಸ್ಥಿತಿ 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    ನಿಲ್ಲಿಸು;
  ಪೂರ್ವನಿಯೋಜಿತ:
    complain (loc, fatal, "too many arguments for complains");
    ನಿಲ್ಲಿಸು;
  }
}


ಶೂನ್ಯ
bison_directive (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

ಶೂನ್ಯ
deprecated_directive (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *old, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *upd)
{
  ಆದರೆ (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_ರಿಜಿಸ್ಟರ್ (loc, upd);
    }
}

ಶೂನ್ಯ
duplicate_directive (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive,
                     location ಮೊದಲ, location ಎರಡನೇ)
{
  ಆದರೆ (feature_flag & feature_caret)
    complain (&ಎರಡನೇ, Wother, _("duplicate directive"));
  ಇಲ್ಲದಿದ್ದರೆ
    complain (&ಎರಡನೇ, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&ಮೊದಲ, Wother, _("previous declaration"));
  fixits_ರಿಜಿಸ್ಟರ್ (&ಎರಡನೇ, "");
}

ಶೂನ್ಯ
duplicate_ನಿಯಮ_directive (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *directive,
                          location ಮೊದಲ, location ಎರಡನೇ)
{
  complain (&ಎರಡನೇ, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&ಮೊದಲ, complaint, _("previous declaration"));
  fixits_ರಿಜಿಸ್ಟರ್ (&ಎರಡನೇ, "");
}

ಶೂನ್ಯ
syntax_error (location loc,
              ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* argv[])
{
  ಆದರೆ (complaint_ಸ್ಥಿತಿ < ಸ್ಥಿತಿ_complaint)
    complaint_ಸ್ಥಿತಿ = ಸ್ಥಿತಿ_complaint;
  ಪ್ರತಿಜ್ಞಾ (argc <= 5);
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format = ಖಾಲಿ;
  ಆಯ್ಕೆ (argc)
    {
#ವ್ಯಾಖ್ಯಾನಿಸು CASE(N, S)                          \
      ಸ್ಥಿತಿ N:                               \
        format = S;                         \
        ನಿಲ್ಲಿಸು
    ಪೂರ್ವನಿಯೋಜಿತ: /* Avoid compiler warnings. */
      CASE (0, _("syntax error"));
      CASE (1, _("unexpected %0$s"));
      CASE (2, _("expected %1$s before %0$s"));
      CASE (3, _("expected %1$s or %2$s before %0$s"));
      CASE (4, _("expected %1$s or %2$s or %3$s before %0$s"));
      CASE (5, _("expected %1$s or %2$s or %3$s or %4$s before %0$s"));
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CASE
    }
  location_print (loc, stderr);
  fputs (": ", stderr);
  severity_print (severity_error, stderr);

  ಆಗ (*format)
    ಆದರೆ (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        ಪೂರ್ಣಾಂಕ i = format[1] - '0';
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        ಕೊನೆ_use_class (style, stderr);
        format += 4;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
