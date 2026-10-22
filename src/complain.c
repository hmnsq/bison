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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <argmatch.h>
#অন্তর্ভাব <c-ctype.h>
#অন্তর্ভাব <progname.h>
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <textstyle.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "fixits.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
স্থির ধ্রুবক অক্ষর *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
প্রকারলেখ গণনা
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
স্থির errority errority_flag[warnings_আকার];

/** Diagnostics severity.  */
প্রকারলেখ গণনা
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
স্থির severity warnings_flag[warnings_আকার];

styled_ostream_t errstream = ফাঁকা;

শূন্য
begin_use_class (ধ্রুবক অক্ষর *s, FILE *out)
{
  যদি (out == stderr)
    {
      যদি (color_debug)
        fprintf (out, "<%s>", s);
      অন্যথা
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

শূন্য
end_use_class (ধ্রুবক অক্ষর *s, FILE *out)
{
  যদি (out == stderr)
    {
      যদি (color_debug)
        fprintf (out, "</%s>", s);
      অন্যথা
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

স্থির শূন্য
begin_hyperlink (FILE *out, ধ্রুবক অক্ষর *ref)
{
  যদি (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, ফাঁকা);
}

স্থির শূন্য
end_hyperlink (FILE *out)
{
  যদি (out == stderr)
    styled_ostream_set_hyperlink (errstream, ফাঁকা, ফাঁকা);
}

শূন্য
flush (FILE *out)
{
  যদি (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  যদি (out != stderr)
    ফেরত false;
  যদি (color_debug)
    ফেরত true;
#যদি HAVE_LIBTEXTSTYLE
  ফেরত (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#অন্যথা
  ফেরত false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

স্থির ধ্রুবক argmatch_warning_doc argmatch_warning_docs[] =
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
  { ফাঁকা, ফাঁকা }
};

স্থির ধ্রুবক argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_মানসমূহ },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { ফাঁকা, Wnone }
};

ধ্রুবক argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  ফাঁকা
};

শূন্য
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

শূন্য
warning_argmatch (অক্ষর ধ্রুবক *arg, size_t no, size_t err)
{
  পূর্ণসংখ্যা মান = *argmatch_warning_মান ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  যদি (!মান)
    {
      মান = Weverything;
      no = !no;
    }

  জন্য (size_t b = 0; b < warnings_আকার; ++b)
    যদি (মান & 1 << b)
      {
        যদি (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        অন্যথা যদি (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        অন্যথা যদি (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        অন্যথা
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

শূন্য
warnings_argmatch (অক্ষর *args)
{
  যদি (!args)
    warning_argmatch ("all", 0, 0);
  অন্যথা যদি (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  অন্যথা
    জন্য (args = strtok (args, ","); args; args = strtok (ফাঁকা, ","))
      যদি (STREQ (args, "error"))
        warnings_are_errors = true;
      অন্যথা যদি (STREQ (args, "no-error"))
        warnings_are_errors = false;
      অন্যথা
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
স্থির ধ্রুবক অক্ষর*
severity_style (severity s)
{
  নির্বাচন (s)
    {
    ক্ষেত্র severity_disabled:
    ক্ষেত্র severity_unset:
      ফেরত "note";
    ক্ষেত্র severity_warning:
      ফেরত "warning";
    ক্ষেত্র severity_error:
    ক্ষেত্র severity_fatal:
      ফেরত "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
স্থির ধ্রুবক অক্ষর*
severity_prefix (severity s)
{
  নির্বাচন (s)
    {
    ক্ষেত্র severity_disabled:
    ক্ষেত্র severity_unset:
      ফেরত "";
    ক্ষেত্র severity_warning:
      ফেরত _("warning");
    ক্ষেত্র severity_error:
      ফেরত  _("error");
    ক্ষেত্র severity_fatal:
      ফেরত _("fatal error");
    }
  abort ();
}


স্থির শূন্য
severity_print (severity s, FILE *out)
{
  যদি (s != severity_disabled)
    {
      ধ্রুবক অক্ষর* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

শূন্য
complain_init_color (শূন্য)
{
#যদি HAVE_LIBTEXTSTYLE
  যদি (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      কাঠামো stat statbuf;
      যদি ((style_file_name == ফাঁকা || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  অন্যথা
    /* No styling.  */
    style_file_name = ফাঁকা;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#যদি defined __clang__
# pragma clang diagnostic ঠেলো
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#যদি defined __clang__
# pragma clang diagnostic বের_করো
#endif
}

শূন্য
complain_init (শূন্য)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  জন্য (size_t b = 0; b < warnings_আকার; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

শূন্য
complain_free (শূন্য)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

স্থির severity
warning_severity (warnings flags)
{
  যদি (flags & fatal)
    /* Diagnostics about fatal errors.  */
    ফেরত severity_fatal;
  অন্যথা যদি (flags & complaint)
    /* Diagnostics about errors.  */
    ফেরত severity_error;
  অন্যথা
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      জন্য (size_t b = 0; b < warnings_আকার; ++b)
        যদি (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            যদি (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      ফেরত res;
    }
}

bool
warning_is_unset (warnings flags)
{
  জন্য (size_t b = 0; b < warnings_আকার; ++b)
    যদি (flags & 1 << b && warnings_flag[b] != severity_unset)
      ফেরত false;
  ফেরত true;
}

bool
warning_is_enabled (warnings flags)
{
  ফেরত severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

স্থির শূন্য
warnings_print_categories (warnings warn_flags, FILE *out)
{
  জন্য (পূর্ণসংখ্যা wbit = 0; wbit < warnings_আকার; ++wbit)
    যদি (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ধ্রুবক অক্ষর* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        ধ্রুবক অক্ষর *warning = argmatch_warning_argument (&w);
        অক্ষর ref[200];
        snprintf (ref, আকার ref,
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
        ফেরত;
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
স্থির
শূন্য
error_message (ধ্রুবক location *loc, warnings flags,
               severity sever, ধ্রুবক অক্ষর *message, va_list args)
{
  ধ্রুবক অক্ষর* style = flags & note ? "note" : severity_style (sever);

  যদি (loc)
    location_print (*loc, stderr);
  অন্যথা
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  যদি (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  যদি (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  যদি (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      যদি (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

স্থির শূন্য
complains (ধ্রুবক location *loc, warnings flags,
           ধ্রুবক অক্ষর *message, va_list args)
{
  যদি ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  যদি (severity_warning <= s)
    {
      যদি (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  যদি (flags & fatal)
    exit (EXIT_FAILURE);
}

শূন্য
complain (location ধ্রুবক *loc, warnings flags, ধ্রুবক অক্ষর *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

শূন্য
subcomplain (location ধ্রুবক *loc, warnings flags, ধ্রুবক অক্ষর *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

শূন্য
complain_args (location ধ্রুবক *loc, warnings w,
               পূর্ণসংখ্যা argc, অক্ষর *argv[])
{
  নির্বাচন (argc)
  {
  ক্ষেত্র 1:
    complain (loc, w, "%s", _(argv[0]));
    বিরতি;
  ক্ষেত্র 2:
    complain (loc, w, _(argv[0]), argv[1]);
    বিরতি;
  ক্ষেত্র 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    বিরতি;
  ক্ষেত্র 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    বিরতি;
  ক্ষেত্র 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    বিরতি;
  পূর্বনির্ধারিত:
    complain (loc, fatal, "too many arguments for complains");
    বিরতি;
  }
}


শূন্য
bison_directive (location ধ্রুবক *loc, অক্ষর ধ্রুবক *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

শূন্য
deprecated_directive (location ধ্রুবক *loc, অক্ষর ধ্রুবক *old, অক্ষর ধ্রুবক *upd)
{
  যদি (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

শূন্য
duplicate_directive (অক্ষর ধ্রুবক *directive,
                     location first, location second)
{
  যদি (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  অন্যথা
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

শূন্য
duplicate_rule_directive (অক্ষর ধ্রুবক *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

শূন্য
syntax_error (location loc,
              পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর* argv[])
{
  যদি (complaint_status < status_complaint)
    complaint_status = status_complaint;
  প্রতিজ্ঞা (argc <= 5);
  ধ্রুবক অক্ষর *format = ফাঁকা;
  নির্বাচন (argc)
    {
#define CASE(N, S)                          \
      ক্ষেত্র N:                               \
        format = S;                         \
        বিরতি
    পূর্বনির্ধারিত: /* Avoid compiler warnings. */
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

  যখন (*format)
    যদি (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        পূর্ণসংখ্যা i = format[1] - '0';
        ধ্রুবক অক্ষর *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    অন্যথা
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
