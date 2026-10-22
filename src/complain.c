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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <argmatch.h>
#અન્તર્ભાવ <c-ctype.h>
#અન્તર્ભાવ <progname.h>
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <textstyle.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "fixits.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
સ્થિર અચલ અક્ષર *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
પ્રકારવ્યાખ્યા ગણના
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
સ્થિર errority errority_flag[warnings_size];

/** Diagnostics severity.  */
પ્રકારવ્યાખ્યા ગણના
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
સ્થિર severity warnings_flag[warnings_size];

styled_ostream_t errstream = ખાલી;

શૂન્ય
begin_use_class (અચલ અક્ષર *s, FILE *out)
{
  જો (out == stderr)
    {
      જો (color_debug)
        fprintf (out, "<%s>", s);
      નહીંતર
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

શૂન્ય
end_use_class (અચલ અક્ષર *s, FILE *out)
{
  જો (out == stderr)
    {
      જો (color_debug)
        fprintf (out, "</%s>", s);
      નહીંતર
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

સ્થિર શૂન્ય
begin_hyperlink (FILE *out, અચલ અક્ષર *ref)
{
  જો (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, ખાલી);
}

સ્થિર શૂન્ય
end_hyperlink (FILE *out)
{
  જો (out == stderr)
    styled_ostream_set_hyperlink (errstream, ખાલી, ખાલી);
}

શૂન્ય
flush (FILE *out)
{
  જો (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  જો (out != stderr)
    પાછા false;
  જો (color_debug)
    પાછા true;
#જો HAVE_LIBTEXTSTYLE
  પાછા (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#નહીંતર
  પાછા false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

સ્થિર અચલ argmatch_warning_doc argmatch_warning_docs[] =
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
  { ખાલી, ખાલી }
};

સ્થિર અચલ argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_કિંમતો },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { ખાલી, Wnone }
};

અચલ argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  ખાલી
};

શૂન્ય
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

શૂન્ય
warning_argmatch (અક્ષર અચલ *arg, size_t no, size_t err)
{
  પૂર્ણાંક કિંમત = *argmatch_warning_કિંમત ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  જો (!કિંમત)
    {
      કિંમત = Weverything;
      no = !no;
    }

  માટે (size_t b = 0; b < warnings_size; ++b)
    જો (કિંમત & 1 << b)
      {
        જો (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        નહીંતર જો (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        નહીંતર જો (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        નહીંતર
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

શૂન્ય
warnings_argmatch (અક્ષર *args)
{
  જો (!args)
    warning_argmatch ("all", 0, 0);
  નહીંતર જો (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  નહીંતર
    માટે (args = strtok (args, ","); args; args = strtok (ખાલી, ","))
      જો (STREQ (args, "error"))
        warnings_are_errors = true;
      નહીંતર જો (STREQ (args, "no-error"))
        warnings_are_errors = false;
      નહીંતર
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
સ્થિર અચલ અક્ષર*
severity_style (severity s)
{
  પસંદગી (s)
    {
    સ્થિતિ severity_disabled:
    સ્થિતિ severity_unset:
      પાછા "note";
    સ્થિતિ severity_warning:
      પાછા "warning";
    સ્થિતિ severity_error:
    સ્થિતિ severity_fatal:
      પાછા "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
સ્થિર અચલ અક્ષર*
severity_prefix (severity s)
{
  પસંદગી (s)
    {
    સ્થિતિ severity_disabled:
    સ્થિતિ severity_unset:
      પાછા "";
    સ્થિતિ severity_warning:
      પાછા _("warning");
    સ્થિતિ severity_error:
      પાછા  _("error");
    સ્થિતિ severity_fatal:
      પાછા _("fatal error");
    }
  abort ();
}


સ્થિર શૂન્ય
severity_print (severity s, FILE *out)
{
  જો (s != severity_disabled)
    {
      અચલ અક્ષર* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

શૂન્ય
complain_init_color (શૂન્ય)
{
#જો HAVE_LIBTEXTSTYLE
  જો (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      સંરચના stat statbuf;
      જો ((style_file_name == ખાલી || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  નહીંતર
    /* No styling.  */
    style_file_name = ખાલી;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#જો defined __clang__
# pragma clang diagnostic ધકેલો
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#જો defined __clang__
# pragma clang diagnostic કાઢો
#endif
}

શૂન્ય
complain_init (શૂન્ય)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  માટે (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

શૂન્ય
complain_free (શૂન્ય)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

સ્થિર severity
warning_severity (warnings flags)
{
  જો (flags & fatal)
    /* Diagnostics about fatal errors.  */
    પાછા severity_fatal;
  નહીંતર જો (flags & complaint)
    /* Diagnostics about errors.  */
    પાછા severity_error;
  નહીંતર
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      માટે (size_t b = 0; b < warnings_size; ++b)
        જો (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            જો (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      પાછા res;
    }
}

bool
warning_is_unset (warnings flags)
{
  માટે (size_t b = 0; b < warnings_size; ++b)
    જો (flags & 1 << b && warnings_flag[b] != severity_unset)
      પાછા false;
  પાછા true;
}

bool
warning_is_enabled (warnings flags)
{
  પાછા severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

સ્થિર શૂન્ય
warnings_print_categories (warnings warn_flags, FILE *out)
{
  માટે (પૂર્ણાંક wbit = 0; wbit < warnings_size; ++wbit)
    જો (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        અચલ અક્ષર* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        અચલ અક્ષર *warning = argmatch_warning_argument (&w);
        અક્ષર ref[200];
        snprintf (ref, કદ ref,
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
        પાછા;
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
સ્થિર
શૂન્ય
error_message (અચલ location *loc, warnings flags,
               severity sever, અચલ અક્ષર *message, va_list args)
{
  અચલ અક્ષર* style = flags & note ? "note" : severity_style (sever);

  જો (loc)
    location_print (*loc, stderr);
  નહીંતર
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  જો (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  જો (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  જો (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      જો (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

સ્થિર શૂન્ય
complains (અચલ location *loc, warnings flags,
           અચલ અક્ષર *message, va_list args)
{
  જો ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  જો (severity_warning <= s)
    {
      જો (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  જો (flags & fatal)
    exit (EXIT_FAILURE);
}

શૂન્ય
complain (location અચલ *loc, warnings flags, અચલ અક્ષર *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

શૂન્ય
subcomplain (location અચલ *loc, warnings flags, અચલ અક્ષર *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

શૂન્ય
complain_args (location અચલ *loc, warnings w,
               પૂર્ણાંક argc, અક્ષર *argv[])
{
  પસંદગી (argc)
  {
  સ્થિતિ 1:
    complain (loc, w, "%s", _(argv[0]));
    તોડો;
  સ્થિતિ 2:
    complain (loc, w, _(argv[0]), argv[1]);
    તોડો;
  સ્થિતિ 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    તોડો;
  સ્થિતિ 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    તોડો;
  સ્થિતિ 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    તોડો;
  મૂળભૂત:
    complain (loc, fatal, "too many arguments for complains");
    તોડો;
  }
}


શૂન્ય
bison_directive (location અચલ *loc, અક્ષર અચલ *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

શૂન્ય
deprecated_directive (location અચલ *loc, અક્ષર અચલ *old, અક્ષર અચલ *upd)
{
  જો (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

શૂન્ય
duplicate_directive (અક્ષર અચલ *directive,
                     location first, location second)
{
  જો (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  નહીંતર
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

શૂન્ય
duplicate_rule_directive (અક્ષર અચલ *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

શૂન્ય
syntax_error (location loc,
              પૂર્ણાંક argc, અચલ અક્ષર* argv[])
{
  જો (complaint_status < status_complaint)
    complaint_status = status_complaint;
  પ્રતિજ્ઞા (argc <= 5);
  અચલ અક્ષર *format = ખાલી;
  પસંદગી (argc)
    {
#define CASE(N, S)                          \
      સ્થિતિ N:                               \
        format = S;                         \
        તોડો
    મૂળભૂત: /* Avoid compiler warnings. */
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

  જ્યારે (*format)
    જો (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        પૂર્ણાંક i = format[1] - '0';
        અચલ અક્ષર *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    નહીંતર
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
