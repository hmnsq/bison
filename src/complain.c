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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <argmatch.h>
#അന്തര്ഭാവ <c-ctype.h>
#അന്തര്ഭാവ <progname.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <textstyle.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "fixits.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
സ്ഥിതം സ്ഥിരം അക്ഷരം *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
തരനിർവചനം എണ്ണൽ
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
സ്ഥിതം errority errority_flag[warnings_size];

/** Diagnostics severity.  */
തരനിർവചനം എണ്ണൽ
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
സ്ഥിതം severity warnings_flag[warnings_size];

styled_ostream_t errstream = ശൂന്യ;

ശൂന്യം
begin_use_class (സ്ഥിരം അക്ഷരം *s, FILE *out)
{
  എങ്കിൽ (out == stderr)
    {
      എങ്കിൽ (color_debug)
        fprintf (out, "<%s>", s);
      അല്ലെങ്കിൽ
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

ശൂന്യം
end_use_class (സ്ഥിരം അക്ഷരം *s, FILE *out)
{
  എങ്കിൽ (out == stderr)
    {
      എങ്കിൽ (color_debug)
        fprintf (out, "</%s>", s);
      അല്ലെങ്കിൽ
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

സ്ഥിതം ശൂന്യം
begin_hyperlink (FILE *out, സ്ഥിരം അക്ഷരം *ref)
{
  എങ്കിൽ (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, ശൂന്യ);
}

സ്ഥിതം ശൂന്യം
end_hyperlink (FILE *out)
{
  എങ്കിൽ (out == stderr)
    styled_ostream_set_hyperlink (errstream, ശൂന്യ, ശൂന്യ);
}

ശൂന്യം
flush (FILE *out)
{
  എങ്കിൽ (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  എങ്കിൽ (out != stderr)
    തിരികെ false;
  എങ്കിൽ (color_debug)
    തിരികെ true;
#എങ്കിൽ HAVE_LIBTEXTSTYLE
  തിരികെ (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#അല്ലെങ്കിൽ
  തിരികെ false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

സ്ഥിതം സ്ഥിരം argmatch_warning_doc argmatch_warning_docs[] =
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
  { ശൂന്യ, ശൂന്യ }
};

സ്ഥിതം സ്ഥിരം argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_മൂല്യങ്ങൾ },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { ശൂന്യ, Wnone }
};

സ്ഥിരം argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  ശൂന്യ
};

ശൂന്യം
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

ശൂന്യം
warning_argmatch (അക്ഷരം സ്ഥിരം *arg, size_t no, size_t err)
{
  പൂർണ്ണസംഖ്യ മൂല്യം = *argmatch_warning_മൂല്യം ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  എങ്കിൽ (!മൂല്യം)
    {
      മൂല്യം = Weverything;
      no = !no;
    }

  വേണ്ടി (size_t b = 0; b < warnings_size; ++b)
    എങ്കിൽ (മൂല്യം & 1 << b)
      {
        എങ്കിൽ (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        അല്ലെങ്കിൽ എങ്കിൽ (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        അല്ലെങ്കിൽ എങ്കിൽ (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        അല്ലെങ്കിൽ
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

ശൂന്യം
warnings_argmatch (അക്ഷരം *args)
{
  എങ്കിൽ (!args)
    warning_argmatch ("all", 0, 0);
  അല്ലെങ്കിൽ എങ്കിൽ (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  അല്ലെങ്കിൽ
    വേണ്ടി (args = strtok (args, ","); args; args = strtok (ശൂന്യ, ","))
      എങ്കിൽ (STREQ (args, "error"))
        warnings_are_errors = true;
      അല്ലെങ്കിൽ എങ്കിൽ (STREQ (args, "no-error"))
        warnings_are_errors = false;
      അല്ലെങ്കിൽ
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
സ്ഥിതം സ്ഥിരം അക്ഷരം*
severity_style (severity s)
{
  തിരഞ്ഞെടുപ്പ് (s)
    {
    അവസ്ഥ severity_disabled:
    അവസ്ഥ severity_unset:
      തിരികെ "note";
    അവസ്ഥ severity_warning:
      തിരികെ "warning";
    അവസ്ഥ severity_error:
    അവസ്ഥ severity_fatal:
      തിരികെ "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം*
severity_prefix (severity s)
{
  തിരഞ്ഞെടുപ്പ് (s)
    {
    അവസ്ഥ severity_disabled:
    അവസ്ഥ severity_unset:
      തിരികെ "";
    അവസ്ഥ severity_warning:
      തിരികെ _("warning");
    അവസ്ഥ severity_error:
      തിരികെ  _("error");
    അവസ്ഥ severity_fatal:
      തിരികെ _("fatal error");
    }
  abort ();
}


സ്ഥിതം ശൂന്യം
severity_print (severity s, FILE *out)
{
  എങ്കിൽ (s != severity_disabled)
    {
      സ്ഥിരം അക്ഷരം* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

ശൂന്യം
complain_init_color (ശൂന്യം)
{
#എങ്കിൽ HAVE_LIBTEXTSTYLE
  എങ്കിൽ (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      ഘടന stat statbuf;
      എങ്കിൽ ((style_file_name == ശൂന്യ || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  അല്ലെങ്കിൽ
    /* No styling.  */
    style_file_name = ശൂന്യ;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#എങ്കിൽ defined __clang__
# pragma clang diagnostic തള്ളുക
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#എങ്കിൽ defined __clang__
# pragma clang diagnostic എടുക്കുക
#endif
}

ശൂന്യം
complain_init (ശൂന്യം)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  വേണ്ടി (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

ശൂന്യം
complain_free (ശൂന്യം)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

സ്ഥിതം severity
warning_severity (warnings flags)
{
  എങ്കിൽ (flags & fatal)
    /* Diagnostics about fatal errors.  */
    തിരികെ severity_fatal;
  അല്ലെങ്കിൽ എങ്കിൽ (flags & complaint)
    /* Diagnostics about errors.  */
    തിരികെ severity_error;
  അല്ലെങ്കിൽ
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      വേണ്ടി (size_t b = 0; b < warnings_size; ++b)
        എങ്കിൽ (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            എങ്കിൽ (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      തിരികെ res;
    }
}

bool
warning_is_unset (warnings flags)
{
  വേണ്ടി (size_t b = 0; b < warnings_size; ++b)
    എങ്കിൽ (flags & 1 << b && warnings_flag[b] != severity_unset)
      തിരികെ false;
  തിരികെ true;
}

bool
warning_is_enabled (warnings flags)
{
  തിരികെ severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

സ്ഥിതം ശൂന്യം
warnings_print_categories (warnings warn_flags, FILE *out)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ wbit = 0; wbit < warnings_size; ++wbit)
    എങ്കിൽ (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        സ്ഥിരം അക്ഷരം* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        സ്ഥിരം അക്ഷരം *warning = argmatch_warning_argument (&w);
        അക്ഷരം ref[200];
        snprintf (ref, വലുപ്പം ref,
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
        തിരികെ;
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
സ്ഥിതം
ശൂന്യം
error_message (സ്ഥിരം location *loc, warnings flags,
               severity sever, സ്ഥിരം അക്ഷരം *message, va_list args)
{
  സ്ഥിരം അക്ഷരം* style = flags & note ? "note" : severity_style (sever);

  എങ്കിൽ (loc)
    location_print (*loc, stderr);
  അല്ലെങ്കിൽ
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  എങ്കിൽ (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  എങ്കിൽ (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  എങ്കിൽ (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      എങ്കിൽ (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

സ്ഥിതം ശൂന്യം
complains (സ്ഥിരം location *loc, warnings flags,
           സ്ഥിരം അക്ഷരം *message, va_list args)
{
  എങ്കിൽ ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  എങ്കിൽ (severity_warning <= s)
    {
      എങ്കിൽ (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  എങ്കിൽ (flags & fatal)
    exit (EXIT_FAILURE);
}

ശൂന്യം
complain (location സ്ഥിരം *loc, warnings flags, സ്ഥിരം അക്ഷരം *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

ശൂന്യം
subcomplain (location സ്ഥിരം *loc, warnings flags, സ്ഥിരം അക്ഷരം *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

ശൂന്യം
complain_args (location സ്ഥിരം *loc, warnings w,
               പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[])
{
  തിരഞ്ഞെടുപ്പ് (argc)
  {
  അവസ്ഥ 1:
    complain (loc, w, "%s", _(argv[0]));
    നിർത്തുക;
  അവസ്ഥ 2:
    complain (loc, w, _(argv[0]), argv[1]);
    നിർത്തുക;
  അവസ്ഥ 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    നിർത്തുക;
  അവസ്ഥ 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    നിർത്തുക;
  അവസ്ഥ 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    നിർത്തുക;
  സ്ഥിരസ്ഥിതി:
    complain (loc, fatal, "too many arguments for complains");
    നിർത്തുക;
  }
}


ശൂന്യം
bison_directive (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

ശൂന്യം
deprecated_directive (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം *old, അക്ഷരം സ്ഥിരം *upd)
{
  എങ്കിൽ (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

ശൂന്യം
duplicate_directive (അക്ഷരം സ്ഥിരം *directive,
                     location first, location second)
{
  എങ്കിൽ (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  അല്ലെങ്കിൽ
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

ശൂന്യം
duplicate_rule_directive (അക്ഷരം സ്ഥിരം *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

ശൂന്യം
syntax_error (location loc,
              പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം* argv[])
{
  എങ്കിൽ (complaint_status < status_complaint)
    complaint_status = status_complaint;
  പ്രതിജ്ഞാ (argc <= 5);
  സ്ഥിരം അക്ഷരം *format = ശൂന്യ;
  തിരഞ്ഞെടുപ്പ് (argc)
    {
#define CASE(N, S)                          \
      അവസ്ഥ N:                               \
        format = S;                         \
        നിർത്തുക
    സ്ഥിരസ്ഥിതി: /* Avoid compiler warnings. */
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

  ആകുമ്പോൾ (*format)
    എങ്കിൽ (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        പൂർണ്ണസംഖ്യ i = format[1] - '0';
        സ്ഥിരം അക്ഷരം *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    അല്ലെങ്കിൽ
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
