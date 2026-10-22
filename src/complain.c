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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <argmatch.h>
#ଅନ୍ତର୍ଭାଵ <c-ctype.h>
#ଅନ୍ତର୍ଭାଵ <progname.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <textstyle.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "fixits.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
ସ୍ଥିର errority errority_flag[warnings_size];

/** Diagnostics severity.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
ସ୍ଥିର severity warnings_flag[warnings_size];

styled_ostream_t errstream = ଖାଲି;

ଶୂନ୍ୟ
begin_use_class (ଅଚଳ ଅକ୍ଷର *s, FILE *out)
{
  ଯଦି (out == stderr)
    {
      ଯଦି (color_debug)
        fprintf (out, "<%s>", s);
      ଅନ୍ୟଥା
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

ଶୂନ୍ୟ
end_use_class (ଅଚଳ ଅକ୍ଷର *s, FILE *out)
{
  ଯଦି (out == stderr)
    {
      ଯଦି (color_debug)
        fprintf (out, "</%s>", s);
      ଅନ୍ୟଥା
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
begin_hyperlink (FILE *out, ଅଚଳ ଅକ୍ଷର *ref)
{
  ଯଦି (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ
end_hyperlink (FILE *out)
{
  ଯଦି (out == stderr)
    styled_ostream_set_hyperlink (errstream, ଖାଲି, ଖାଲି);
}

ଶୂନ୍ୟ
flush (FILE *out)
{
  ଯଦି (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  ଯଦି (out != stderr)
    ଫେରନ୍ତୁ false;
  ଯଦି (color_debug)
    ଫେରନ୍ତୁ true;
#ଯଦି HAVE_LIBTEXTSTYLE
  ଫେରନ୍ତୁ (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

ସ୍ଥିର ଅଚଳ argmatch_warning_doc argmatch_warning_docs[] =
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
  { ଖାଲି, ଖାଲି }
};

ସ୍ଥିର ଅଚଳ argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_ମୂଲ୍ୟଗୁଡ଼ିକ },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { ଖାଲି, Wnone }
};

ଅଚଳ argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  ଖାଲି
};

ଶୂନ୍ୟ
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

ଶୂନ୍ୟ
warning_argmatch (ଅକ୍ଷର ଅଚଳ *arg, size_t no, size_t err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = *argmatch_warning_ମୂଲ୍ୟ ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  ଯଦି (!ମୂଲ୍ୟ)
    {
      ମୂଲ୍ୟ = Weverything;
      no = !no;
    }

  ପାଇଁ (size_t b = 0; b < warnings_size; ++b)
    ଯଦି (ମୂଲ୍ୟ & 1 << b)
      {
        ଯଦି (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        ଅନ୍ୟଥା ଯଦି (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        ଅନ୍ୟଥା ଯଦି (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        ଅନ୍ୟଥା
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

ଶୂନ୍ୟ
warnings_argmatch (ଅକ୍ଷର *args)
{
  ଯଦି (!args)
    warning_argmatch ("all", 0, 0);
  ଅନ୍ୟଥା ଯଦି (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  ଅନ୍ୟଥା
    ପାଇଁ (args = strtok (args, ","); args; args = strtok (ଖାଲି, ","))
      ଯଦି (STREQ (args, "error"))
        warnings_are_errors = true;
      ଅନ୍ୟଥା ଯଦି (STREQ (args, "no-error"))
        warnings_are_errors = false;
      ଅନ୍ୟଥା
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
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*
severity_style (severity s)
{
  ବାଛ (s)
    {
    ସ୍ଥିତି severity_disabled:
    ସ୍ଥିତି severity_unset:
      ଫେରନ୍ତୁ "note";
    ସ୍ଥିତି severity_warning:
      ଫେରନ୍ତୁ "warning";
    ସ୍ଥିତି severity_error:
    ସ୍ଥିତି severity_fatal:
      ଫେରନ୍ତୁ "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର*
severity_prefix (severity s)
{
  ବାଛ (s)
    {
    ସ୍ଥିତି severity_disabled:
    ସ୍ଥିତି severity_unset:
      ଫେରନ୍ତୁ "";
    ସ୍ଥିତି severity_warning:
      ଫେରନ୍ତୁ _("warning");
    ସ୍ଥିତି severity_error:
      ଫେରନ୍ତୁ  _("error");
    ସ୍ଥିତି severity_fatal:
      ଫେରନ୍ତୁ _("fatal error");
    }
  abort ();
}


ସ୍ଥିର ଶୂନ୍ୟ
severity_print (severity s, FILE *out)
{
  ଯଦି (s != severity_disabled)
    {
      ଅଚଳ ଅକ୍ଷର* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

ଶୂନ୍ୟ
complain_init_color (ଶୂନ୍ୟ)
{
#ଯଦି HAVE_LIBTEXTSTYLE
  ଯଦି (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      ସଂରଚନା stat statbuf;
      ଯଦି ((style_file_name == ଖାଲି || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  ଅନ୍ୟଥା
    /* No styling.  */
    style_file_name = ଖାଲି;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ଯଦି defined __clang__
# pragma clang diagnostic ଠେଲ
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#ଯଦି defined __clang__
# pragma clang diagnostic କାଢ଼
#endif
}

ଶୂନ୍ୟ
complain_init (ଶୂନ୍ୟ)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  ପାଇଁ (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

ଶୂନ୍ୟ
complain_free (ଶୂନ୍ୟ)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

ସ୍ଥିର severity
warning_severity (warnings flags)
{
  ଯଦି (flags & fatal)
    /* Diagnostics about fatal errors.  */
    ଫେରନ୍ତୁ severity_fatal;
  ଅନ୍ୟଥା ଯଦି (flags & complaint)
    /* Diagnostics about errors.  */
    ଫେରନ୍ତୁ severity_error;
  ଅନ୍ୟଥା
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      ପାଇଁ (size_t b = 0; b < warnings_size; ++b)
        ଯଦି (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            ଯଦି (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      ଫେରନ୍ତୁ res;
    }
}

bool
warning_is_unset (warnings flags)
{
  ପାଇଁ (size_t b = 0; b < warnings_size; ++b)
    ଯଦି (flags & 1 << b && warnings_flag[b] != severity_unset)
      ଫେରନ୍ତୁ false;
  ଫେରନ୍ତୁ true;
}

bool
warning_is_enabled (warnings flags)
{
  ଫେରନ୍ତୁ severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

ସ୍ଥିର ଶୂନ୍ୟ
warnings_print_categories (warnings warn_flags, FILE *out)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା wbit = 0; wbit < warnings_size; ++wbit)
    ଯଦି (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ଅଚଳ ଅକ୍ଷର* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        ଅଚଳ ଅକ୍ଷର *warning = argmatch_warning_argument (&w);
        ଅକ୍ଷର ref[200];
        snprintf (ref, ଆକାର ref,
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
        ଫେରନ୍ତୁ;
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
ସ୍ଥିର
ଶୂନ୍ୟ
error_message (ଅଚଳ location *loc, warnings flags,
               severity sever, ଅଚଳ ଅକ୍ଷର *message, va_list args)
{
  ଅଚଳ ଅକ୍ଷର* style = flags & note ? "note" : severity_style (sever);

  ଯଦି (loc)
    location_print (*loc, stderr);
  ଅନ୍ୟଥା
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  ଯଦି (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  ଯଦି (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  ଯଦି (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      ଯଦି (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

ସ୍ଥିର ଶୂନ୍ୟ
complains (ଅଚଳ location *loc, warnings flags,
           ଅଚଳ ଅକ୍ଷର *message, va_list args)
{
  ଯଦି ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  ଯଦି (severity_warning <= s)
    {
      ଯଦି (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  ଯଦି (flags & fatal)
    exit (EXIT_FAILURE);
}

ଶୂନ୍ୟ
complain (location ଅଚଳ *loc, warnings flags, ଅଚଳ ଅକ୍ଷର *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

ଶୂନ୍ୟ
subcomplain (location ଅଚଳ *loc, warnings flags, ଅଚଳ ଅକ୍ଷର *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

ଶୂନ୍ୟ
complain_args (location ଅଚଳ *loc, warnings w,
               ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
  ବାଛ (argc)
  {
  ସ୍ଥିତି 1:
    complain (loc, w, "%s", _(argv[0]));
    ଭାଙ୍ଗ;
  ସ୍ଥିତି 2:
    complain (loc, w, _(argv[0]), argv[1]);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    ଭାଙ୍ଗ;
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    complain (loc, fatal, "too many arguments for complains");
    ଭାଙ୍ଗ;
  }
}


ଶୂନ୍ୟ
bison_directive (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

ଶୂନ୍ୟ
deprecated_directive (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ *old, ଅକ୍ଷର ଅଚଳ *upd)
{
  ଯଦି (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

ଶୂନ୍ୟ
duplicate_directive (ଅକ୍ଷର ଅଚଳ *directive,
                     location first, location second)
{
  ଯଦି (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  ଅନ୍ୟଥା
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

ଶୂନ୍ୟ
duplicate_rule_directive (ଅକ୍ଷର ଅଚଳ *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

ଶୂନ୍ୟ
syntax_error (location loc,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର* argv[])
{
  ଯଦି (complaint_status < status_complaint)
    complaint_status = status_complaint;
  ପ୍ରତିଜ୍ଞା (argc <= 5);
  ଅଚଳ ଅକ୍ଷର *format = ଖାଲି;
  ବାଛ (argc)
    {
#define CASE(N, S)                          \
      ସ୍ଥିତି N:                               \
        format = S;                         \
        ଭାଙ୍ଗ
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: /* Avoid compiler warnings. */
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

  ଯେତେବେଳେ (*format)
    ଯଦି (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = format[1] - '0';
        ଅଚଳ ଅକ୍ଷର *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    ଅନ୍ୟଥା
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
