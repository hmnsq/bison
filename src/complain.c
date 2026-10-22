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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <argmatch.h>
#அந்தர்பாவ <c-ctype.h>
#அந்தர்பாவ <progname.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <textstyle.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "fixits.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
நிலையான மாறிலி எழுத்து *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
வகைவரை எண்ணிக்கை
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
நிலையான errority errority_flag[warnings_size];

/** Diagnostics severity.  */
வகைவரை எண்ணிக்கை
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
நிலையான severity warnings_flag[warnings_size];

styled_ostream_t errstream = வெற்று;

வெறுமை
begin_use_class (மாறிலி எழுத்து *s, FILE *out)
{
  எனில் (out == stderr)
    {
      எனில் (color_debug)
        fprintf (out, "<%s>", s);
      இல்லையெனில்
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

வெறுமை
end_use_class (மாறிலி எழுத்து *s, FILE *out)
{
  எனில் (out == stderr)
    {
      எனில் (color_debug)
        fprintf (out, "</%s>", s);
      இல்லையெனில்
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

நிலையான வெறுமை
begin_hyperlink (FILE *out, மாறிலி எழுத்து *ref)
{
  எனில் (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, வெற்று);
}

நிலையான வெறுமை
end_hyperlink (FILE *out)
{
  எனில் (out == stderr)
    styled_ostream_set_hyperlink (errstream, வெற்று, வெற்று);
}

வெறுமை
flush (FILE *out)
{
  எனில் (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  எனில் (out != stderr)
    திரும்பு false;
  எனில் (color_debug)
    திரும்பு true;
#எனில் HAVE_LIBTEXTSTYLE
  திரும்பு (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#இல்லையெனில்
  திரும்பு false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

நிலையான மாறிலி argmatch_warning_doc argmatch_warning_docs[] =
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
  { வெற்று, வெற்று }
};

நிலையான மாறிலி argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_மதிப்புகள் },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { வெற்று, Wnone }
};

மாறிலி argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  வெற்று
};

வெறுமை
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

வெறுமை
warning_argmatch (எழுத்து மாறிலி *arg, size_t no, size_t err)
{
  முழு மதிப்பு = *argmatch_warning_மதிப்பு ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  எனில் (!மதிப்பு)
    {
      மதிப்பு = Weverything;
      no = !no;
    }

  க்கு (size_t b = 0; b < warnings_size; ++b)
    எனில் (மதிப்பு & 1 << b)
      {
        எனில் (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        இல்லையெனில் எனில் (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        இல்லையெனில் எனில் (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        இல்லையெனில்
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

வெறுமை
warnings_argmatch (எழுத்து *args)
{
  எனில் (!args)
    warning_argmatch ("all", 0, 0);
  இல்லையெனில் எனில் (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  இல்லையெனில்
    க்கு (args = strtok (args, ","); args; args = strtok (வெற்று, ","))
      எனில் (STREQ (args, "error"))
        warnings_are_errors = true;
      இல்லையெனில் எனில் (STREQ (args, "no-error"))
        warnings_are_errors = false;
      இல்லையெனில்
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
நிலையான மாறிலி எழுத்து*
severity_style (severity s)
{
  தேர்வு (s)
    {
    நிலை severity_disabled:
    நிலை severity_unset:
      திரும்பு "note";
    நிலை severity_warning:
      திரும்பு "warning";
    நிலை severity_error:
    நிலை severity_fatal:
      திரும்பு "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
நிலையான மாறிலி எழுத்து*
severity_prefix (severity s)
{
  தேர்வு (s)
    {
    நிலை severity_disabled:
    நிலை severity_unset:
      திரும்பு "";
    நிலை severity_warning:
      திரும்பு _("warning");
    நிலை severity_error:
      திரும்பு  _("error");
    நிலை severity_fatal:
      திரும்பு _("fatal error");
    }
  abort ();
}


நிலையான வெறுமை
severity_print (severity s, FILE *out)
{
  எனில் (s != severity_disabled)
    {
      மாறிலி எழுத்து* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

வெறுமை
complain_init_color (வெறுமை)
{
#எனில் HAVE_LIBTEXTSTYLE
  எனில் (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      கட்டமைப்பு stat statbuf;
      எனில் ((style_file_name == வெற்று || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  இல்லையெனில்
    /* No styling.  */
    style_file_name = வெற்று;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#எனில் defined __clang__
# pragma clang diagnostic தள்ளு
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#எனில் defined __clang__
# pragma clang diagnostic எடு
#endif
}

வெறுமை
complain_init (வெறுமை)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  க்கு (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

வெறுமை
complain_free (வெறுமை)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

நிலையான severity
warning_severity (warnings flags)
{
  எனில் (flags & fatal)
    /* Diagnostics about fatal errors.  */
    திரும்பு severity_fatal;
  இல்லையெனில் எனில் (flags & complaint)
    /* Diagnostics about errors.  */
    திரும்பு severity_error;
  இல்லையெனில்
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      க்கு (size_t b = 0; b < warnings_size; ++b)
        எனில் (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            எனில் (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      திரும்பு res;
    }
}

bool
warning_is_unset (warnings flags)
{
  க்கு (size_t b = 0; b < warnings_size; ++b)
    எனில் (flags & 1 << b && warnings_flag[b] != severity_unset)
      திரும்பு false;
  திரும்பு true;
}

bool
warning_is_enabled (warnings flags)
{
  திரும்பு severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

நிலையான வெறுமை
warnings_print_categories (warnings warn_flags, FILE *out)
{
  க்கு (முழு wbit = 0; wbit < warnings_size; ++wbit)
    எனில் (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        மாறிலி எழுத்து* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        மாறிலி எழுத்து *warning = argmatch_warning_argument (&w);
        எழுத்து ref[200];
        snprintf (ref, அளவு ref,
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
        திரும்பு;
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
நிலையான
வெறுமை
error_message (மாறிலி location *loc, warnings flags,
               severity sever, மாறிலி எழுத்து *message, va_list args)
{
  மாறிலி எழுத்து* style = flags & note ? "note" : severity_style (sever);

  எனில் (loc)
    location_print (*loc, stderr);
  இல்லையெனில்
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  எனில் (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  எனில் (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  எனில் (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      எனில் (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

நிலையான வெறுமை
complains (மாறிலி location *loc, warnings flags,
           மாறிலி எழுத்து *message, va_list args)
{
  எனில் ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  எனில் (severity_warning <= s)
    {
      எனில் (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  எனில் (flags & fatal)
    exit (EXIT_FAILURE);
}

வெறுமை
complain (location மாறிலி *loc, warnings flags, மாறிலி எழுத்து *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

வெறுமை
subcomplain (location மாறிலி *loc, warnings flags, மாறிலி எழுத்து *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

வெறுமை
complain_args (location மாறிலி *loc, warnings w,
               முழு argc, எழுத்து *argv[])
{
  தேர்வு (argc)
  {
  நிலை 1:
    complain (loc, w, "%s", _(argv[0]));
    நிறுத்து;
  நிலை 2:
    complain (loc, w, _(argv[0]), argv[1]);
    நிறுத்து;
  நிலை 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    நிறுத்து;
  நிலை 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    நிறுத்து;
  நிலை 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    நிறுத்து;
  இயல்பு:
    complain (loc, fatal, "too many arguments for complains");
    நிறுத்து;
  }
}


வெறுமை
bison_directive (location மாறிலி *loc, எழுத்து மாறிலி *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

வெறுமை
deprecated_directive (location மாறிலி *loc, எழுத்து மாறிலி *old, எழுத்து மாறிலி *upd)
{
  எனில் (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

வெறுமை
duplicate_directive (எழுத்து மாறிலி *directive,
                     location first, location second)
{
  எனில் (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  இல்லையெனில்
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

வெறுமை
duplicate_rule_directive (எழுத்து மாறிலி *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

வெறுமை
syntax_error (location loc,
              முழு argc, மாறிலி எழுத்து* argv[])
{
  எனில் (complaint_status < status_complaint)
    complaint_status = status_complaint;
  ப்ரதிஜ்ஞா (argc <= 5);
  மாறிலி எழுத்து *format = வெற்று;
  தேர்வு (argc)
    {
#define CASE(N, S)                          \
      நிலை N:                               \
        format = S;                         \
        நிறுத்து
    இயல்பு: /* Avoid compiler warnings. */
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

  போது (*format)
    எனில் (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        முழு i = format[1] - '0';
        மாறிலி எழுத்து *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    இல்லையெனில்
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
