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
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
ಸ್ಥಿರ errority errority_flag[warnings_size];

/** Diagnostics severity.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
ಸ್ಥಿರ severity warnings_flag[warnings_size];

styled_ostream_t errstream = ರಿಕ್ತ;

ಶೂನ್ಯ
begin_use_class (ಧ್ರುವ ವರ್ಣ *s, FILE *out)
{
  ಯದಿ (out == stderr)
    {
      ಯದಿ (color_debug)
        fprintf (out, "<%s>", s);
      ಅನ್ಯಥಾ
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

ಶೂನ್ಯ
end_use_class (ಧ್ರುವ ವರ್ಣ *s, FILE *out)
{
  ಯದಿ (out == stderr)
    {
      ಯದಿ (color_debug)
        fprintf (out, "</%s>", s);
      ಅನ್ಯಥಾ
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
begin_hyperlink (FILE *out, ಧ್ರುವ ವರ್ಣ *ref)
{
  ಯದಿ (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
end_hyperlink (FILE *out)
{
  ಯದಿ (out == stderr)
    styled_ostream_set_hyperlink (errstream, ರಿಕ್ತ, ರಿಕ್ತ);
}

ಶೂನ್ಯ
flush (FILE *out)
{
  ಯದಿ (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  ಯದಿ (out != stderr)
    ಪ್ರತ್ಯಾಗಮ false;
  ಯದಿ (color_debug)
    ಪ್ರತ್ಯಾಗಮ true;
#ಯದಿ HAVE_LIBTEXTSTYLE
  ಪ್ರತ್ಯಾಗಮ (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

ಸ್ಥಿರ ಧ್ರುವ argmatch_warning_doc argmatch_warning_docs[] =
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
  { ರಿಕ್ತ, ರಿಕ್ತ }
};

ಸ್ಥಿರ ಧ್ರುವ argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_ಮೌಲ್ಯಗಳು },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { ರಿಕ್ತ, Wnone }
};

ಧ್ರುವ argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  ರಿಕ್ತ
};

ಶೂನ್ಯ
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

ಶೂನ್ಯ
warning_argmatch (ವರ್ಣ ಧ್ರುವ *arg, size_t no, size_t err)
{
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = *argmatch_warning_ಮೌಲ್ಯ ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  ಯದಿ (!ಮೌಲ್ಯ)
    {
      ಮೌಲ್ಯ = Weverything;
      no = !no;
    }

  ಪ್ರತಿ (size_t b = 0; b < warnings_size; ++b)
    ಯದಿ (ಮೌಲ್ಯ & 1 << b)
      {
        ಯದಿ (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        ಅನ್ಯಥಾ ಯದಿ (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        ಅನ್ಯಥಾ ಯದಿ (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        ಅನ್ಯಥಾ
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
warnings_argmatch (ವರ್ಣ *args)
{
  ಯದಿ (!args)
    warning_argmatch ("all", 0, 0);
  ಅನ್ಯಥಾ ಯದಿ (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (args = strtok (args, ","); args; args = strtok (ರಿಕ್ತ, ","))
      ಯದಿ (STREQ (args, "error"))
        warnings_are_errors = true;
      ಅನ್ಯಥಾ ಯದಿ (STREQ (args, "no-error"))
        warnings_are_errors = false;
      ಅನ್ಯಥಾ
        {
          /* The length of the possible 'no-' prefix: 3, or 0.  */
          size_t no = STRPREFIX_LIT ("no-", args) ? 3 : 0;
          /* The length of the possible 'error=' (possibly after
             'no-') prefix: 6, or 0. */
          size_t err = STRPREFIX_LIT ("error=", args + no) ? 6 : 0;

          warning_argmatch (args, no, err);
        }
}

/* Color style for this type of message.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*
severity_style (severity s)
{
  ವಿಕಲ್ಪ (s)
    {
    ಪಕ್ಷ severity_disabled:
    ಪಕ್ಷ severity_unset:
      ಪ್ರತ್ಯಾಗಮ "note";
    ಪಕ್ಷ severity_warning:
      ಪ್ರತ್ಯಾಗಮ "warning";
    ಪಕ್ಷ severity_error:
    ಪಕ್ಷ severity_fatal:
      ಪ್ರತ್ಯಾಗಮ "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ*
severity_prefix (severity s)
{
  ವಿಕಲ್ಪ (s)
    {
    ಪಕ್ಷ severity_disabled:
    ಪಕ್ಷ severity_unset:
      ಪ್ರತ್ಯಾಗಮ "";
    ಪಕ್ಷ severity_warning:
      ಪ್ರತ್ಯಾಗಮ _("warning");
    ಪಕ್ಷ severity_error:
      ಪ್ರತ್ಯಾಗಮ  _("error");
    ಪಕ್ಷ severity_fatal:
      ಪ್ರತ್ಯಾಗಮ _("fatal error");
    }
  abort ();
}


ಸ್ಥಿರ ಶೂನ್ಯ
severity_print (severity s, FILE *out)
{
  ಯದಿ (s != severity_disabled)
    {
      ಧ್ರುವ ವರ್ಣ* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

ಶೂನ್ಯ
complain_init_color (ಶೂನ್ಯ)
{
#ಯದಿ HAVE_LIBTEXTSTYLE
  ಯದಿ (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      ಸಂರಚನಾ stat statbuf;
      ಯದಿ ((style_file_name == ರಿಕ್ತ || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  ಅನ್ಯಥಾ
    /* No styling.  */
    style_file_name = ರಿಕ್ತ;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ಯದಿ defined __clang__
# pragma clang diagnostic ತಳ್ಳು
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#ಯದಿ defined __clang__
# pragma clang diagnostic ತೆಗೆ
#endif
}

ಶೂನ್ಯ
complain_init (ಶೂನ್ಯ)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  ಪ್ರತಿ (size_t b = 0; b < warnings_size; ++b)
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
  ಯದಿ (flags & fatal)
    /* Diagnostics about fatal errors.  */
    ಪ್ರತ್ಯಾಗಮ severity_fatal;
  ಅನ್ಯಥಾ ಯದಿ (flags & complaint)
    /* Diagnostics about errors.  */
    ಪ್ರತ್ಯಾಗಮ severity_error;
  ಅನ್ಯಥಾ
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      ಪ್ರತಿ (size_t b = 0; b < warnings_size; ++b)
        ಯದಿ (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            ಯದಿ (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      ಪ್ರತ್ಯಾಗಮ res;
    }
}

bool
warning_is_unset (warnings flags)
{
  ಪ್ರತಿ (size_t b = 0; b < warnings_size; ++b)
    ಯದಿ (flags & 1 << b && warnings_flag[b] != severity_unset)
      ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತ್ಯಾಗಮ true;
}

bool
warning_is_enabled (warnings flags)
{
  ಪ್ರತ್ಯಾಗಮ severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

ಸ್ಥಿರ ಶೂನ್ಯ
warnings_print_categories (warnings warn_flags, FILE *out)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ wbit = 0; wbit < warnings_size; ++wbit)
    ಯದಿ (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ಧ್ರುವ ವರ್ಣ* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        ಧ್ರುವ ವರ್ಣ *warning = argmatch_warning_argument (&w);
        ವರ್ಣ ref[200];
        snprintf (ref, ಪರಿಮಾಣ ref,
                  "%s#W%s", diagnostics_url, warning);
        begin_hyperlink (out, ref);
        ostream_printf (errstream,
                        "-W%s%s",
                        s == severity_error ? "error=" : "",
                        warning);
        end_hyperlink (out);
        // Because we mix stdio with ostream I/O, we need to flush
        // here for sake of color == debug.
        flush (out);
        end_use_class (style, out);
        fputc (']', out);
        /* Display only the first match, the second is "-Wall".  */
        ಪ್ರತ್ಯಾಗಮ;
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
error_message (ಧ್ರುವ location *loc, warnings flags,
               severity sever, ಧ್ರುವ ವರ್ಣ *message, va_list args)
{
  ಧ್ರುವ ವರ್ಣ* style = flags & note ? "note" : severity_style (sever);

  ಯದಿ (loc)
    location_print (*loc, stderr);
  ಅನ್ಯಥಾ
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  ಯದಿ (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  ಯದಿ (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  ಯದಿ (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      ಯದಿ (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

ಸ್ಥಿರ ಶೂನ್ಯ
complains (ಧ್ರುವ location *loc, warnings flags,
           ಧ್ರುವ ವರ್ಣ *message, va_list args)
{
  ಯದಿ ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  ಯದಿ (severity_warning <= s)
    {
      ಯದಿ (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  ಯದಿ (flags & fatal)
    exit (EXIT_FAILURE);
}

ಶೂನ್ಯ
complain (location ಧ್ರುವ *loc, warnings flags, ಧ್ರುವ ವರ್ಣ *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

ಶೂನ್ಯ
subcomplain (location ಧ್ರುವ *loc, warnings flags, ಧ್ರುವ ವರ್ಣ *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

ಶೂನ್ಯ
complain_args (location ಧ್ರುವ *loc, warnings w,
               ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[])
{
  ವಿಕಲ್ಪ (argc)
  {
  ಪಕ್ಷ 1:
    complain (loc, w, "%s", _(argv[0]));
    ಭಙ್ಗ;
  ಪಕ್ಷ 2:
    complain (loc, w, _(argv[0]), argv[1]);
    ಭಙ್ಗ;
  ಪಕ್ಷ 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    ಭಙ್ಗ;
  ಪಕ್ಷ 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    ಭಙ್ಗ;
  ಪಕ್ಷ 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    ಭಙ್ಗ;
  ಸ್ವಭಾವ:
    complain (loc, fatal, "too many arguments for complains");
    ಭಙ್ಗ;
  }
}


ಶೂನ್ಯ
bison_directive (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

ಶೂನ್ಯ
deprecated_directive (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ *old, ವರ್ಣ ಧ್ರುವ *upd)
{
  ಯದಿ (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

ಶೂನ್ಯ
duplicate_directive (ವರ್ಣ ಧ್ರುವ *directive,
                     location first, location second)
{
  ಯದಿ (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  ಅನ್ಯಥಾ
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

ಶೂನ್ಯ
duplicate_rule_directive (ವರ್ಣ ಧ್ರುವ *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

ಶೂನ್ಯ
syntax_error (location loc,
              ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ* argv[])
{
  ಯದಿ (complaint_status < status_complaint)
    complaint_status = status_complaint;
  ಪ್ರತಿಜ್ಞಾ (argc <= 5);
  ಧ್ರುವ ವರ್ಣ *format = ರಿಕ್ತ;
  ವಿಕಲ್ಪ (argc)
    {
#define CASE(N, S)                          \
      ಪಕ್ಷ N:                               \
        format = S;                         \
        ಭಙ್ಗ
    ಸ್ವಭಾವ: /* Avoid compiler warnings. */
      CASE (0, _("syntax error"));
      CASE (1, _("unexpected %0$s"));
      CASE (2, _("expected %1$s before %0$s"));
      CASE (3, _("expected %1$s or %2$s before %0$s"));
      CASE (4, _("expected %1$s or %2$s or %3$s before %0$s"));
      CASE (5, _("expected %1$s or %2$s or %3$s or %4$s before %0$s"));
#undef CASE
    }
  location_print (loc, stderr);
  fputs (": ", stderr);
  severity_print (severity_error, stderr);

  ಯಾವತ್ (*format)
    ಯದಿ (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        ಪೂರ್ಣಾಙ್ಕ i = format[1] - '0';
        ಧ್ರುವ ವರ್ಣ *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    ಅನ್ಯಥಾ
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
