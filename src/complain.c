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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <argmatch.h>
#ضمن <c-ctype.h>
#ضمن <progname.h>
#ضمن <stdarg.h>
#ضمن <sys/stat.h>
#ضمن <textstyle.h>

#ضمن "complain.h"
#ضمن "files.h"
#ضمن "fixits.h"
#ضمن "getargs.h"
#ضمن "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
ساكن ثابت حرف *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_حالت complaint_حالت = حالت_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
تعريفنوع تعداد
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
ساكن errority errority_flag[warnings_سائيز];

/** Diagnostics severity.  */
تعريفنوع تعداد
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
ساكن severity warnings_flag[warnings_سائيز];

styled_ostream_t errstream = عدم;

فراغ
begin_use_class (ثابت حرف *s, FILE *ٻاهريون)
{
  إذا (ٻاهريون == stderr)
    {
      إذا (color_debug)
        fprintf (ٻاهريون, "<%s>", s);
      وإلا
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_منزل_current_style (errstream);
        }
    }
}

فراغ
پڇاڙي_use_class (ثابت حرف *s, FILE *ٻاهريون)
{
  إذا (ٻاهريون == stderr)
    {
      إذا (color_debug)
        fprintf (ٻاهريون, "</%s>", s);
      وإلا
        {
          styled_ostream_پڇاڙي_use_class (errstream, s);
          styled_ostream_flush_منزل_current_style (errstream);
        }
    }
}

ساكن فراغ
begin_hyperlink (FILE *ٻاهريون, ثابت حرف *ref)
{
  إذا (ٻاهريون == stderr)
    styled_ostream_سيٽ_hyperlink (errstream, ref, عدم);
}

ساكن فراغ
پڇاڙي_hyperlink (FILE *ٻاهريون)
{
  إذا (ٻاهريون == stderr)
    styled_ostream_سيٽ_hyperlink (errstream, عدم, عدم);
}

فراغ
flush (FILE *ٻاهريون)
{
  إذا (ٻاهريون == stderr)
    ostream_flush (errstream, FLUSH_THIS_وهڪرو);
  fflush (ٻاهريون);
}

bool
is_styled (FILE *ٻاهريون)
{
  إذا (ٻاهريون != stderr)
    أرجع false;
  إذا (color_debug)
    أرجع true;
#إذا HAVE_LIBTEXTSTYLE
  أرجع (color_گهڻائي == color_yes
          || color_گهڻائي == color_html
          || (color_گهڻائي == color_tty && isatty (STDERR_FILENO)));
#وإلا
  أرجع false;
#جيڪڏهن_پورو
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_گروهه (warning, warnings)

ساكن ثابت argmatch_warning_doc argmatch_warning_docs[] =
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
  { عدم, عدم }
};

ساكن ثابت argmatch_warning_arg argmatch_warning_دليلون[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_قاعدو },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_قدرون },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { عدم, Wnone }
};

ثابت argmatch_warning_گروهه_type argmatch_warning_گروهه =
{
  argmatch_warning_دليلون,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  عدم
};

فراغ
warning_usage (FILE *ٻاهريون)
{
  argmatch_warning_usage (ٻاهريون);
}

فراغ
warning_argmatch (حرف ثابت *arg, size_t no, size_t err)
{
  صحيح قدر = *argmatch_warning_قدر ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  إذا (!قدر)
    {
      قدر = Weverything;
      no = !no;
    }

  لكل (size_t b = 0; b < warnings_سائيز; ++b)
    إذا (قدر & 1 << b)
      {
        إذا (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        وإلا إذا (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        وإلا إذا (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        وإلا
          /* -Wfoo. */
          warnings_flag[b] = severity_warning;
      }
}

/** Decode a comma-separated list of arguments from -W.
 *
 *  \param args     comma separated list of effective subarguments to decode.
 *                  If 0, then activate all the flags.
 */

فراغ
warnings_argmatch (حرف *دليلون)
{
  إذا (!دليلون)
    warning_argmatch ("all", 0, 0);
  وإلا إذا (STREQ (دليلون, "help"))
    {
      warning_usage (stdout);
      ٻاهر_نڪرو (EXIT_SUCCESS);
    }
  وإلا
    لكل (دليلون = strtok (دليلون, ","); دليلون; دليلون = strtok (عدم, ","))
      إذا (STREQ (دليلون, "error"))
        warnings_are_errors = true;
      وإلا إذا (STREQ (دليلون, "no-error"))
        warnings_are_errors = false;
      وإلا
        {
          /* The length of the possible 'no-' prefix: 3, or 0.  */
          size_t no = STRPREFIX_LIT ("no-", دليلون) ? 3 : 0;
          /* The length of the possible 'error=' (possibly after
             'no-') prefix: 6, or 0. */
          size_t err = STRPREFIX_LIT ("error=", دليلون + no) ? 6 : 0;

          warning_argmatch (دليلون, no, err);
        }
}

/* Color style for this type of message.  */
ساكن ثابت حرف*
severity_style (severity s)
{
  بدل (s)
    {
    حالة severity_disabled:
    حالة severity_unset:
      أرجع "note";
    حالة severity_warning:
      أرجع "warning";
    حالة severity_error:
    حالة severity_fatal:
      أرجع "error";
    }
  abort ();
}

/* Prefix for this type of message.  */
ساكن ثابت حرف*
severity_اڳياڙي (severity s)
{
  بدل (s)
    {
    حالة severity_disabled:
    حالة severity_unset:
      أرجع "";
    حالة severity_warning:
      أرجع _("warning");
    حالة severity_error:
      أرجع  _("error");
    حالة severity_fatal:
      أرجع _("fatal error");
    }
  abort ();
}


ساكن فراغ
severity_print (severity s, FILE *ٻاهريون)
{
  إذا (s != severity_disabled)
    {
      ثابت حرف* style = severity_style (s);
      begin_use_class (style, ٻاهريون);
      fprintf (ٻاهريون, "%s:", severity_اڳياڙي (s));
      پڇاڙي_use_class (style, ٻاهريون);
      fputc (' ', ٻاهريون);
    }
}


/*-----------.
| complain.  |
`-----------*/

فراغ
complain_init_color (فراغ)
{
#إذا HAVE_LIBTEXTSTYLE
  إذا (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      بنية stat statbuf;
      إذا ((style_file_name == عدم || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  وإلا
    /* No styling.  */
    style_file_name = عدم;
#جيڪڏهن_پورو

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#إذا defined __clang__
# pragma clang diagnostic ڌڪو
# pragma clang diagnostic ignored "-Wunreachable-code"
#جيڪڏهن_پورو
  errstream =
    color_گهڻائي == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#إذا defined __clang__
# pragma clang diagnostic ڪڍو
#جيڪڏهن_پورو
}

فراغ
complain_init (فراغ)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  لكل (size_t b = 0; b < warnings_سائيز; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

فراغ
complain_free (فراغ)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

ساكن severity
warning_severity (warnings flags)
{
  إذا (flags & fatal)
    /* Diagnostics about fatal errors.  */
    أرجع severity_fatal;
  وإلا إذا (flags & complaint)
    /* Diagnostics about errors.  */
    أرجع severity_error;
  وإلا
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      لكل (size_t b = 0; b < warnings_سائيز; ++b)
        إذا (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            إذا (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      أرجع res;
    }
}

bool
warning_is_unset (warnings flags)
{
  لكل (size_t b = 0; b < warnings_سائيز; ++b)
    إذا (flags & 1 << b && warnings_flag[b] != severity_unset)
      أرجع false;
  أرجع true;
}

bool
warning_is_enabled (warnings flags)
{
  أرجع severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

ساكن فراغ
warnings_print_categories (warnings warn_flags, FILE *ٻاهريون)
{
  لكل (صحيح wbit = 0; wbit < warnings_سائيز; ++wbit)
    إذا (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ثابت حرف* style = severity_style (s);
        fputs (" [", ٻاهريون);
        begin_use_class (style, ٻاهريون);
        // E.g., "counterexamples".
        ثابت حرف *warning = argmatch_warning_دليل (&w);
        حرف ref[200];
        snprintf (ref, حجم ref,
                  "%s#W%s", diagnostics_url, warning);
        begin_hyperlink (ٻاهريون, ref);
        ostream_printf (errstream,
                        "-W%s%s",
                        s == severity_error ? "error=" : "",
                        warning);
        پڇاڙي_hyperlink (ٻاهريون);
        // Because we mix stdio with ostream I/O, we need to flush
        // here for sake of color == debug.
        flush (ٻاهريون);
        پڇاڙي_use_class (style, ٻاهريون);
        fputc (']', ٻاهريون);
        /* Display only the first match, the second is "-Wall".  */
        أرجع;
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
ساكن
فراغ
error_پيغام (ثابت location *loc, warnings flags,
               severity sever, ثابت حرف *پيغام, va_list دليلون)
{
  ثابت حرف* style = flags & note ? "note" : severity_style (sever);

  إذا (loc)
    location_print (*loc, stderr);
  وإلا
    fprintf (stderr, "%s", grammar_file ? grammar_file : پروگرام_name);
  fprintf (stderr, ": ");

  إذا (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_اڳياڙي (sever));
      پڇاڙي_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, پيغام, دليلون);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  إذا (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = لڙي_ڊيگهه (پيغام);
  إذا (l < 2 || پيغام[l - 2] != ':' || پيغام[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      إذا (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

ساكن فراغ
complains (ثابت location *loc, warnings flags,
           ثابت حرف *پيغام, va_list دليلون)
{
  إذا ((flags & complaint) && complaint_حالت < حالت_complaint)
    complaint_حالت = حالت_complaint;

  severity s = warning_severity (flags);
  إذا (severity_warning <= s)
    {
      إذا (severity_error <= s && ! complaint_حالت)
        complaint_حالت = حالت_warning_as_error;
      error_پيغام (loc, flags, s, پيغام, دليلون);
    }

  إذا (flags & fatal)
    ٻاهر_نڪرو (EXIT_FAILURE);
}

فراغ
complain (location ثابت *loc, warnings flags, ثابت حرف *پيغام, ...)
{
  va_list دليلون;
  va_شروعات (دليلون, پيغام);
  complains (loc, flags, پيغام, دليلون);
  va_پڇاڙي (دليلون);
}

فراغ
subcomplain (location ثابت *loc, warnings flags, ثابت حرف *پيغام, ...)
{
  va_list دليلون;
  va_شروعات (دليلون, پيغام);
  complains (loc, flags | note | silent, پيغام, دليلون);
  va_پڇاڙي (دليلون);
}

فراغ
complain_دليلون (location ثابت *loc, warnings w,
               صحيح argc, حرف *argv[])
{
  بدل (argc)
  {
  حالة 1:
    complain (loc, w, "%s", _(argv[0]));
    اكسر;
  حالة 2:
    complain (loc, w, _(argv[0]), argv[1]);
    اكسر;
  حالة 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    اكسر;
  حالة 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    اكسر;
  حالة 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    اكسر;
  افتراضي:
    complain (loc, fatal, "too many arguments for complains");
    اكسر;
  }
}


فراغ
bison_directive (location ثابت *loc, حرف ثابت *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

فراغ
deprecated_directive (location ثابت *loc, حرف ثابت *old, حرف ثابت *upd)
{
  إذا (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_رجسٽر (loc, upd);
    }
}

فراغ
duplicate_directive (حرف ثابت *directive,
                     location پهريون, location ٻيو)
{
  إذا (feature_flag & feature_caret)
    complain (&ٻيو, Wother, _("duplicate directive"));
  وإلا
    complain (&ٻيو, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&پهريون, Wother, _("previous declaration"));
  fixits_رجسٽر (&ٻيو, "");
}

فراغ
duplicate_قاعدو_directive (حرف ثابت *directive,
                          location پهريون, location ٻيو)
{
  complain (&ٻيو, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&پهريون, complaint, _("previous declaration"));
  fixits_رجسٽر (&ٻيو, "");
}

فراغ
syntax_error (location loc,
              صحيح argc, ثابت حرف* argv[])
{
  إذا (complaint_حالت < حالت_complaint)
    complaint_حالت = حالت_complaint;
  أكد (argc <= 5);
  ثابت حرف *format = عدم;
  بدل (argc)
    {
#وصف_ڪريو CASE(N, S)                          \
      حالة N:                               \
        format = S;                         \
        اكسر
    افتراضي: /* Avoid compiler warnings. */
      CASE (0, _("syntax error"));
      CASE (1, _("unexpected %0$s"));
      CASE (2, _("expected %1$s before %0$s"));
      CASE (3, _("expected %1$s or %2$s before %0$s"));
      CASE (4, _("expected %1$s or %2$s or %3$s before %0$s"));
      CASE (5, _("expected %1$s or %2$s or %3$s or %4$s before %0$s"));
#وصف_رد CASE
    }
  location_print (loc, stderr);
  fputs (": ", stderr);
  severity_print (severity_error, stderr);

  طالما (*format)
    إذا (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        صحيح i = format[1] - '0';
        ثابت حرف *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        پڇاڙي_use_class (style, stderr);
        format += 4;
      }
    وإلا
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
