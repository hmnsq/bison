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

#شامل <config.h>
#شامل "system.h"

#شامل <argmatch.h>
#شامل <c-ctype.h>
#شامل <progname.h>
#شامل <stdarg.h>
#شامل <sys/stat.h>
#شامل <textstyle.h>

#شامل "complain.h"
#شامل "files.h"
#شامل "fixits.h"
#شامل "getargs.h"
#شامل "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
ایستا ثابت نویسه *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
تعریفنوع شمارش
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
ایستا errority errority_flag[warnings_size];

/** Diagnostics severity.  */
تعریفنوع شمارش
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
ایستا severity warnings_flag[warnings_size];

styled_ostream_t errstream = پوچ;

تهی
begin_use_class (ثابت نویسه *s, FILE *out)
{
  اگر (out == stderr)
    {
      اگر (color_debug)
        fprintf (out, "<%s>", s);
      وگرنه
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

تهی
end_use_class (ثابت نویسه *s, FILE *out)
{
  اگر (out == stderr)
    {
      اگر (color_debug)
        fprintf (out, "</%s>", s);
      وگرنه
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

ایستا تهی
begin_hyperlink (FILE *out, ثابت نویسه *ref)
{
  اگر (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, پوچ);
}

ایستا تهی
end_hyperlink (FILE *out)
{
  اگر (out == stderr)
    styled_ostream_set_hyperlink (errstream, پوچ, پوچ);
}

تهی
flush (FILE *out)
{
  اگر (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  اگر (out != stderr)
    بازگشت false;
  اگر (color_debug)
    بازگشت true;
#اگر HAVE_LIBTEXTSTYLE
  بازگشت (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#وگرنه
  بازگشت false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

ایستا ثابت argmatch_warning_doc argmatch_warning_docs[] =
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
  { پوچ, پوچ }
};

ایستا ثابت argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_مقادیر },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { پوچ, Wnone }
};

ثابت argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  پوچ
};

تهی
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

تهی
warning_argmatch (نویسه ثابت *arg, size_t no, size_t err)
{
  صحیح مقدار = *argmatch_warning_مقدار ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  اگر (!مقدار)
    {
      مقدار = Weverything;
      no = !no;
    }

  برای (size_t b = 0; b < warnings_size; ++b)
    اگر (مقدار & 1 << b)
      {
        اگر (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        وگرنه اگر (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        وگرنه اگر (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        وگرنه
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

تهی
warnings_argmatch (نویسه *args)
{
  اگر (!args)
    warning_argmatch ("all", 0, 0);
  وگرنه اگر (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  وگرنه
    برای (args = strtok (args, ","); args; args = strtok (پوچ, ","))
      اگر (STREQ (args, "error"))
        warnings_are_errors = true;
      وگرنه اگر (STREQ (args, "no-error"))
        warnings_are_errors = false;
      وگرنه
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
ایستا ثابت نویسه*
severity_style (severity s)
{
  انتخاب (s)
    {
    حالت severity_disabled:
    حالت severity_unset:
      بازگشت "note";
    حالت severity_warning:
      بازگشت "warning";
    حالت severity_error:
    حالت severity_fatal:
      بازگشت "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
ایستا ثابت نویسه*
severity_prefix (severity s)
{
  انتخاب (s)
    {
    حالت severity_disabled:
    حالت severity_unset:
      بازگشت "";
    حالت severity_warning:
      بازگشت _("warning");
    حالت severity_error:
      بازگشت  _("error");
    حالت severity_fatal:
      بازگشت _("fatal error");
    }
  abort ();
}


ایستا تهی
severity_print (severity s, FILE *out)
{
  اگر (s != severity_disabled)
    {
      ثابت نویسه* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

تهی
complain_init_color (تهی)
{
#اگر HAVE_LIBTEXTSTYLE
  اگر (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      ساختار stat statbuf;
      اگر ((style_file_name == پوچ || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  وگرنه
    /* No styling.  */
    style_file_name = پوچ;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#اگر defined __clang__
# pragma clang diagnostic افزودن
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#اگر defined __clang__
# pragma clang diagnostic برداشت
#endif
}

تهی
complain_init (تهی)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  برای (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

تهی
complain_free (تهی)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

ایستا severity
warning_severity (warnings flags)
{
  اگر (flags & fatal)
    /* Diagnostics about fatal errors.  */
    بازگشت severity_fatal;
  وگرنه اگر (flags & complaint)
    /* Diagnostics about errors.  */
    بازگشت severity_error;
  وگرنه
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      برای (size_t b = 0; b < warnings_size; ++b)
        اگر (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            اگر (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      بازگشت res;
    }
}

bool
warning_is_unset (warnings flags)
{
  برای (size_t b = 0; b < warnings_size; ++b)
    اگر (flags & 1 << b && warnings_flag[b] != severity_unset)
      بازگشت false;
  بازگشت true;
}

bool
warning_is_enabled (warnings flags)
{
  بازگشت severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

ایستا تهی
warnings_print_categories (warnings warn_flags, FILE *out)
{
  برای (صحیح wbit = 0; wbit < warnings_size; ++wbit)
    اگر (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ثابت نویسه* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        ثابت نویسه *warning = argmatch_warning_argument (&w);
        نویسه ref[200];
        snprintf (ref, اندازه ref,
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
        بازگشت;
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
ایستا
تهی
error_message (ثابت location *loc, warnings flags,
               severity sever, ثابت نویسه *message, va_list args)
{
  ثابت نویسه* style = flags & note ? "note" : severity_style (sever);

  اگر (loc)
    location_print (*loc, stderr);
  وگرنه
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  اگر (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  اگر (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  اگر (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      اگر (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

ایستا تهی
complains (ثابت location *loc, warnings flags,
           ثابت نویسه *message, va_list args)
{
  اگر ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  اگر (severity_warning <= s)
    {
      اگر (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  اگر (flags & fatal)
    exit (EXIT_FAILURE);
}

تهی
complain (location ثابت *loc, warnings flags, ثابت نویسه *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

تهی
subcomplain (location ثابت *loc, warnings flags, ثابت نویسه *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

تهی
complain_args (location ثابت *loc, warnings w,
               صحیح argc, نویسه *argv[])
{
  انتخاب (argc)
  {
  حالت 1:
    complain (loc, w, "%s", _(argv[0]));
    بشکن;
  حالت 2:
    complain (loc, w, _(argv[0]), argv[1]);
    بشکن;
  حالت 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    بشکن;
  حالت 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    بشکن;
  حالت 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    بشکن;
  پیشفرض:
    complain (loc, fatal, "too many arguments for complains");
    بشکن;
  }
}


تهی
bison_directive (location ثابت *loc, نویسه ثابت *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

تهی
deprecated_directive (location ثابت *loc, نویسه ثابت *old, نویسه ثابت *upd)
{
  اگر (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

تهی
duplicate_directive (نویسه ثابت *directive,
                     location first, location second)
{
  اگر (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  وگرنه
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

تهی
duplicate_rule_directive (نویسه ثابت *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

تهی
syntax_error (location loc,
              صحیح argc, ثابت نویسه* argv[])
{
  اگر (complaint_status < status_complaint)
    complaint_status = status_complaint;
  أكد (argc <= 5);
  ثابت نویسه *format = پوچ;
  انتخاب (argc)
    {
#define CASE(N, S)                          \
      حالت N:                               \
        format = S;                         \
        بشکن
    پیشفرض: /* Avoid compiler warnings. */
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

  وقتی (*format)
    اگر (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        صحیح i = format[1] - '0';
        ثابت نویسه *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    وگرنه
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
