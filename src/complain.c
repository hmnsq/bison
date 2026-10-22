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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <argmatch.h>
#अन्तर्भाव <c-ctype.h>
#अन्तर्भाव <progname.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <textstyle.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "fixits.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
स्थिर ध्रुव वर्ण *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
प्रकारनिर्देश गणना
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
स्थिर errority errority_flag[warnings_size];

/** Diagnostics severity.  */
प्रकारनिर्देश गणना
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
स्थिर severity warnings_flag[warnings_size];

styled_ostream_t errstream = रिक्त;

शून्य
begin_use_class (ध्रुव वर्ण *s, FILE *out)
{
  जइ (out == stderr)
    {
      जइ (color_debug)
        fprintf (out, "<%s>", s);
      अण्णहा
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

शून्य
end_use_class (ध्रुव वर्ण *s, FILE *out)
{
  जइ (out == stderr)
    {
      जइ (color_debug)
        fprintf (out, "</%s>", s);
      अण्णहा
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

स्थिर शून्य
begin_hyperlink (FILE *out, ध्रुव वर्ण *ref)
{
  जइ (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, रिक्त);
}

स्थिर शून्य
end_hyperlink (FILE *out)
{
  जइ (out == stderr)
    styled_ostream_set_hyperlink (errstream, रिक्त, रिक्त);
}

शून्य
flush (FILE *out)
{
  जइ (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  जइ (out != stderr)
    प्रत्यागम false;
  जइ (color_debug)
    प्रत्यागम true;
#जइ HAVE_LIBTEXTSTYLE
  प्रत्यागम (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#अण्णहा
  प्रत्यागम false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

स्थिर ध्रुव argmatch_warning_doc argmatch_warning_docs[] =
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
  { रिक्त, रिक्त }
};

स्थिर ध्रुव argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_मानानि },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { रिक्त, Wnone }
};

ध्रुव argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  रिक्त
};

शून्य
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

शून्य
warning_argmatch (वर्ण ध्रुव *arg, size_t no, size_t err)
{
  पूर्णाङ्क मानम् = *argmatch_warning_मानम् ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  जइ (!मानम्)
    {
      मानम् = Weverything;
      no = !no;
    }

  प्रति (size_t b = 0; b < warnings_size; ++b)
    जइ (मानम् & 1 << b)
      {
        जइ (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        अण्णहा जइ (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        अण्णहा जइ (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        अण्णहा
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

शून्य
warnings_argmatch (वर्ण *args)
{
  जइ (!args)
    warning_argmatch ("all", 0, 0);
  अण्णहा जइ (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  अण्णहा
    प्रति (args = strtok (args, ","); args; args = strtok (रिक्त, ","))
      जइ (STREQ (args, "error"))
        warnings_are_errors = true;
      अण्णहा जइ (STREQ (args, "no-error"))
        warnings_are_errors = false;
      अण्णहा
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
स्थिर ध्रुव वर्ण*
severity_style (severity s)
{
  विकल्प (s)
    {
    पक्ष severity_disabled:
    पक्ष severity_unset:
      प्रत्यागम "note";
    पक्ष severity_warning:
      प्रत्यागम "warning";
    पक्ष severity_error:
    पक्ष severity_fatal:
      प्रत्यागम "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
स्थिर ध्रुव वर्ण*
severity_prefix (severity s)
{
  विकल्प (s)
    {
    पक्ष severity_disabled:
    पक्ष severity_unset:
      प्रत्यागम "";
    पक्ष severity_warning:
      प्रत्यागम _("warning");
    पक्ष severity_error:
      प्रत्यागम  _("error");
    पक्ष severity_fatal:
      प्रत्यागम _("fatal error");
    }
  abort ();
}


स्थिर शून्य
severity_print (severity s, FILE *out)
{
  जइ (s != severity_disabled)
    {
      ध्रुव वर्ण* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

शून्य
complain_init_color (शून्य)
{
#जइ HAVE_LIBTEXTSTYLE
  जइ (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      संरचना stat statbuf;
      जइ ((style_file_name == रिक्त || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  अण्णहा
    /* No styling.  */
    style_file_name = रिक्त;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#जइ defined __clang__
# pragma clang diagnostic प्रक्षिप
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#जइ defined __clang__
# pragma clang diagnostic निष्कासय
#endif
}

शून्य
complain_init (शून्य)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  प्रति (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

शून्य
complain_free (शून्य)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

स्थिर severity
warning_severity (warnings flags)
{
  जइ (flags & fatal)
    /* Diagnostics about fatal errors.  */
    प्रत्यागम severity_fatal;
  अण्णहा जइ (flags & complaint)
    /* Diagnostics about errors.  */
    प्रत्यागम severity_error;
  अण्णहा
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      प्रति (size_t b = 0; b < warnings_size; ++b)
        जइ (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            जइ (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      प्रत्यागम res;
    }
}

bool
warning_is_unset (warnings flags)
{
  प्रति (size_t b = 0; b < warnings_size; ++b)
    जइ (flags & 1 << b && warnings_flag[b] != severity_unset)
      प्रत्यागम false;
  प्रत्यागम true;
}

bool
warning_is_enabled (warnings flags)
{
  प्रत्यागम severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

स्थिर शून्य
warnings_print_categories (warnings warn_flags, FILE *out)
{
  प्रति (पूर्णाङ्क wbit = 0; wbit < warnings_size; ++wbit)
    जइ (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ध्रुव वर्ण* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        ध्रुव वर्ण *warning = argmatch_warning_argument (&w);
        वर्ण ref[200];
        snprintf (ref, परिमाण ref,
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
        प्रत्यागम;
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
स्थिर
शून्य
error_message (ध्रुव location *loc, warnings flags,
               severity sever, ध्रुव वर्ण *message, va_list args)
{
  ध्रुव वर्ण* style = flags & note ? "note" : severity_style (sever);

  जइ (loc)
    location_print (*loc, stderr);
  अण्णहा
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  जइ (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  जइ (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  जइ (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      जइ (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

स्थिर शून्य
complains (ध्रुव location *loc, warnings flags,
           ध्रुव वर्ण *message, va_list args)
{
  जइ ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  जइ (severity_warning <= s)
    {
      जइ (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  जइ (flags & fatal)
    exit (EXIT_FAILURE);
}

शून्य
complain (location ध्रुव *loc, warnings flags, ध्रुव वर्ण *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

शून्य
subcomplain (location ध्रुव *loc, warnings flags, ध्रुव वर्ण *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

शून्य
complain_args (location ध्रुव *loc, warnings w,
               पूर्णाङ्क argc, वर्ण *argv[])
{
  विकल्प (argc)
  {
  पक्ष 1:
    complain (loc, w, "%s", _(argv[0]));
    भङ्ग;
  पक्ष 2:
    complain (loc, w, _(argv[0]), argv[1]);
    भङ्ग;
  पक्ष 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    भङ्ग;
  पक्ष 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    भङ्ग;
  पक्ष 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    भङ्ग;
  स्वभाव:
    complain (loc, fatal, "too many arguments for complains");
    भङ्ग;
  }
}


शून्य
bison_directive (location ध्रुव *loc, वर्ण ध्रुव *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

शून्य
deprecated_directive (location ध्रुव *loc, वर्ण ध्रुव *old, वर्ण ध्रुव *upd)
{
  जइ (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

शून्य
duplicate_directive (वर्ण ध्रुव *directive,
                     location first, location second)
{
  जइ (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  अण्णहा
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

शून्य
duplicate_rule_directive (वर्ण ध्रुव *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

शून्य
syntax_error (location loc,
              पूर्णाङ्क argc, ध्रुव वर्ण* argv[])
{
  जइ (complaint_status < status_complaint)
    complaint_status = status_complaint;
  प्रतिज्ञा (argc <= 5);
  ध्रुव वर्ण *format = रिक्त;
  विकल्प (argc)
    {
#define CASE(N, S)                          \
      पक्ष N:                               \
        format = S;                         \
        भङ्ग
    स्वभाव: /* Avoid compiler warnings. */
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

  जाव (*format)
    जइ (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        पूर्णाङ्क i = format[1] - '0';
        ध्रुव वर्ण *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    अण्णहा
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
