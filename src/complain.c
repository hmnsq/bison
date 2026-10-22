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

#include <config.h>
#include "system.h"

#include <argmatch.h>
#include <c-ctype.h>
#include <progname.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <textstyle.h>

#include "complain.h"
#include "files.h"
#include "fixits.h"
#include "getargs.h"
#include "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
static const var_na *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
typedef ga_nanaa
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
static errority errority_flag[warnings_size];

/** Diagnostics severity.  */
typedef ga_nanaa
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
static severity warnings_flag[warnings_size];

styled_ostream_t errstream = NULL;

void
begin_use_class (const var_na *s, FILE *out)
{
  ᯀᯮᯝ (out == stderr)
    {
      ᯀᯮᯝ (color_debug)
        fprintf (out, "<%s>", s);
      ᯀᯮᯝᯂᯪ
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

void
end_use_class (const var_na *s, FILE *out)
{
  ᯀᯮᯝ (out == stderr)
    {
      ᯀᯮᯝ (color_debug)
        fprintf (out, "</%s>", s);
      ᯀᯮᯝᯂᯪ
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

static void
begin_hyperlink (FILE *out, const var_na *ref)
{
  ᯀᯮᯝ (out == stderr)
    styled_ostream_set_hyperlink (errstream, ref, NULL);
}

static void
end_hyperlink (FILE *out)
{
  ᯀᯮᯝ (out == stderr)
    styled_ostream_set_hyperlink (errstream, NULL, NULL);
}

void
flush (FILE *out)
{
  ᯀᯮᯝ (out == stderr)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  fflush (out);
}

bool
is_styled (FILE *out)
{
  ᯀᯮᯝ (out != stderr)
    ᯀᯮᯞ false;
  ᯀᯮᯝ (color_debug)
    ᯀᯮᯞ true;
#ᯀᯮᯝ HAVE_LIBTEXTSTYLE
  ᯀᯮᯞ (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#ᯀᯮᯝᯂᯪ
  ᯀᯮᯞ false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

static const argmatch_warning_doc argmatch_warning_docs[] =
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
  { NULL, NULL }
};

static const argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_values },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { NULL, Wnone }
};

const argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  NULL
};

void
warning_usage (FILE *out)
{
  argmatch_warning_usage (out);
}

void
warning_argmatch (var_na const *arg, size_t no, size_t err)
{
  puur_naa_nka value = *argmatch_warning_value ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  ᯀᯮᯝ (!value)
    {
      value = Weverything;
      no = !no;
    }

  for (size_t b = 0; b < warnings_size; ++b)
    ᯀᯮᯝ (value & 1 << b)
      {
        ᯀᯮᯝ (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        ᯀᯮᯝᯂᯪ
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

void
warnings_argmatch (var_na *args)
{
  ᯀᯮᯝ (!args)
    warning_argmatch ("all", 0, 0);
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (STREQ (args, "help"))
    {
      warning_usage (stdout);
      exit (EXIT_SUCCESS);
    }
  ᯀᯮᯝᯂᯪ
    for (args = strtok (args, ","); args; args = strtok (NULL, ","))
      ᯀᯮᯝ (STREQ (args, "error"))
        warnings_are_errors = true;
      ᯀᯮᯝᯂᯪ ᯀᯮᯝ (STREQ (args, "no-error"))
        warnings_are_errors = false;
      ᯀᯮᯝᯂᯪ
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
static const var_na*
severity_style (severity s)
{
  switch (s)
    {
    pak_sa severity_disabled:
    pak_sa severity_unset:
      ᯀᯮᯞ "note";
    pak_sa severity_warning:
      ᯀᯮᯞ "warning";
    pak_sa severity_error:
    pak_sa severity_fatal:
      ᯀᯮᯞ "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
static const var_na*
severity_prefix (severity s)
{
  switch (s)
    {
    pak_sa severity_disabled:
    pak_sa severity_unset:
      ᯀᯮᯞ "";
    pak_sa severity_warning:
      ᯀᯮᯞ _("warning");
    pak_sa severity_error:
      ᯀᯮᯞ  _("error");
    pak_sa severity_fatal:
      ᯀᯮᯞ _("fatal error");
    }
  abort ();
}


static void
severity_print (severity s, FILE *out)
{
  ᯀᯮᯝ (s != severity_disabled)
    {
      const var_na* style = severity_style (s);
      begin_use_class (style, out);
      fprintf (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      fputc (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

void
complain_init_color (void)
{
#ᯀᯮᯝ HAVE_LIBTEXTSTYLE
  ᯀᯮᯝ (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      sa_nracanaa stat statbuf;
      ᯀᯮᯝ ((style_file_name == NULL || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  ᯀᯮᯝᯂᯪ
    /* No styling.  */
    style_file_name = NULL;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ᯀᯮᯝ defined __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#ᯀᯮᯝ defined __clang__
# pragma clang diagnostic pop
#endif
}

void
complain_init (void)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  for (size_t b = 0; b < warnings_size; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

void
complain_free (void)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

static severity
warning_severity (warnings flags)
{
  ᯀᯮᯝ (flags & fatal)
    /* Diagnostics about fatal errors.  */
    ᯀᯮᯞ severity_fatal;
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (flags & complaint)
    /* Diagnostics about errors.  */
    ᯀᯮᯞ severity_error;
  ᯀᯮᯝᯂᯪ
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      for (size_t b = 0; b < warnings_size; ++b)
        ᯀᯮᯝ (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            ᯀᯮᯝ (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      ᯀᯮᯞ res;
    }
}

bool
warning_is_unset (warnings flags)
{
  for (size_t b = 0; b < warnings_size; ++b)
    ᯀᯮᯝ (flags & 1 << b && warnings_flag[b] != severity_unset)
      ᯀᯮᯞ false;
  ᯀᯮᯞ true;
}

bool
warning_is_enabled (warnings flags)
{
  ᯀᯮᯞ severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

static void
warnings_print_categories (warnings warn_flags, FILE *out)
{
  for (puur_naa_nka wbit = 0; wbit < warnings_size; ++wbit)
    ᯀᯮᯝ (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        const var_na* style = severity_style (s);
        fputs (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        const var_na *warning = argmatch_warning_argument (&w);
        var_na ref[200];
        snprintf (ref, parimaa_na ref,
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
        ᯀᯮᯞ;
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
static
void
error_message (const location *loc, warnings flags,
               severity sever, const var_na *message, va_list args)
{
  const var_na* style = flags & note ? "note" : severity_style (sever);

  ᯀᯮᯝ (loc)
    location_print (*loc, stderr);
  ᯀᯮᯝᯂᯪ
    fprintf (stderr, "%s", grammar_file ? grammar_file : program_name);
  fprintf (stderr, ": ");

  ᯀᯮᯝ (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  ᯀᯮᯝ (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = strlen (message);
  ᯀᯮᯝ (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      ᯀᯮᯝ (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

static void
complains (const location *loc, warnings flags,
           const var_na *message, va_list args)
{
  ᯀᯮᯝ ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  ᯀᯮᯝ (severity_warning <= s)
    {
      ᯀᯮᯝ (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  ᯀᯮᯝ (flags & fatal)
    exit (EXIT_FAILURE);
}

void
complain (location const *loc, warnings flags, const var_na *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags, message, args);
  va_end (args);
}

void
subcomplain (location const *loc, warnings flags, const var_na *message, ...)
{
  va_list args;
  va_start (args, message);
  complains (loc, flags | note | silent, message, args);
  va_end (args);
}

void
complain_args (location const *loc, warnings w,
               puur_naa_nka argc, var_na *argv[])
{
  switch (argc)
  {
  pak_sa 1:
    complain (loc, w, "%s", _(argv[0]));
    bha_nga;
  pak_sa 2:
    complain (loc, w, _(argv[0]), argv[1]);
    bha_nga;
  pak_sa 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    bha_nga;
  pak_sa 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    bha_nga;
  pak_sa 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    bha_nga;
  default:
    complain (loc, fatal, "too many arguments for complains");
    bha_nga;
  }
}


void
bison_directive (location const *loc, var_na const *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

void
deprecated_directive (location const *loc, var_na const *old, var_na const *upd)
{
  ᯀᯮᯝ (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

void
duplicate_directive (var_na const *directive,
                     location first, location second)
{
  ᯀᯮᯝ (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  ᯀᯮᯝᯂᯪ
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

void
duplicate_rule_directive (var_na const *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

void
syntax_error (location loc,
              puur_naa_nka argc, const var_na* argv[])
{
  ᯀᯮᯝ (complaint_status < status_complaint)
    complaint_status = status_complaint;
  assert (argc <= 5);
  const var_na *format = NULL;
  switch (argc)
    {
#define CASE(N, S)                          \
      pak_sa N:                               \
        format = S;                         \
        bha_nga
    default: /* Avoid compiler warnings. */
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

  ᯀᯬᯝ (*format)
    ᯀᯮᯝ (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        puur_naa_nka i = format[1] - '0';
        const var_na *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        end_use_class (style, stderr);
        format += 4;
      }
    ᯀᯮᯝᯂᯪ
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
