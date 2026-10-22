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


err_स्थितिः complaint_स्थितिः = स्थितिः_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
प्रकारनिर्देश गणना
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
स्थिर errority errority_flag[warnings_आकार];

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
स्थिर severity warnings_flag[warnings_आकार];

styled_ostream_t errstream = रिक्त;

शून्य
begin_use_class (ध्रुव वर्ण *s, FILE *बहिर्गमनम्)
{
  यदि (बहिर्गमनम् == stderr)
    {
      यदि (color_debug)
        fprintf (बहिर्गमनम्, "<%s>", s);
      अन्यथा
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_गन्तव्यम्_current_style (errstream);
        }
    }
}

शून्य
अन्तः_use_class (ध्रुव वर्ण *s, FILE *बहिर्गमनम्)
{
  यदि (बहिर्गमनम् == stderr)
    {
      यदि (color_debug)
        fprintf (बहिर्गमनम्, "</%s>", s);
      अन्यथा
        {
          styled_ostream_अन्तः_use_class (errstream, s);
          styled_ostream_flush_गन्तव्यम्_current_style (errstream);
        }
    }
}

स्थिर शून्य
begin_hyperlink (FILE *बहिर्गमनम्, ध्रुव वर्ण *ref)
{
  यदि (बहिर्गमनम् == stderr)
    styled_ostream_समुच्चयः_hyperlink (errstream, ref, रिक्त);
}

स्थिर शून्य
अन्तः_hyperlink (FILE *बहिर्गमनम्)
{
  यदि (बहिर्गमनम् == stderr)
    styled_ostream_समुच्चयः_hyperlink (errstream, रिक्त, रिक्त);
}

शून्य
flush (FILE *बहिर्गमनम्)
{
  यदि (बहिर्गमनम् == stderr)
    ostream_flush (errstream, FLUSH_THIS_प्रवाहः);
  fflush (बहिर्गमनम्);
}

bool
is_styled (FILE *बहिर्गमनम्)
{
  यदि (बहिर्गमनम् != stderr)
    प्रत्यागम false;
  यदि (color_debug)
    प्रत्यागम true;
#यदि HAVE_LIBTEXTSTYLE
  प्रत्यागम (color_बहुलकम् == color_yes
          || color_बहुलकम् == color_html
          || (color_बहुलकम् == color_tty && isatty (STDERR_FILENO)));
#अन्यथा
  प्रत्यागम false;
#यदि_अन्तः
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_गणः (warning, warnings)

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

स्थिर ध्रुव argmatch_warning_arg argmatch_warning_तर्काः[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_नियमः },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_मानानि },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { रिक्त, Wnone }
};

ध्रुव argmatch_warning_गणः_type argmatch_warning_गणः =
{
  argmatch_warning_तर्काः,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  रिक्त
};

शून्य
warning_usage (FILE *बहिर्गमनम्)
{
  argmatch_warning_usage (बहिर्गमनम्);
}

शून्य
warning_argmatch (वर्ण ध्रुव *arg, size_t no, size_t err)
{
  पूर्णाङ्क मानम् = *argmatch_warning_मानम् ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  यदि (!मानम्)
    {
      मानम् = Weverything;
      no = !no;
    }

  प्रति (size_t b = 0; b < warnings_आकार; ++b)
    यदि (मानम् & 1 << b)
      {
        यदि (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        अन्यथा यदि (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        अन्यथा यदि (no)
          /* -Wno-foo.  */
          warnings_flag[b] = severity_disabled;
        अन्यथा
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
warnings_argmatch (वर्ण *तर्काः)
{
  यदि (!तर्काः)
    warning_argmatch ("all", 0, 0);
  अन्यथा यदि (STREQ (तर्काः, "help"))
    {
      warning_usage (stdout);
      निर्गच्छ (EXIT_SUCCESS);
    }
  अन्यथा
    प्रति (तर्काः = strtok (तर्काः, ","); तर्काः; तर्काः = strtok (रिक्त, ","))
      यदि (STREQ (तर्काः, "error"))
        warnings_are_errors = true;
      अन्यथा यदि (STREQ (तर्काः, "no-error"))
        warnings_are_errors = false;
      अन्यथा
        {
          /* The length of the possible 'no-' prefix: 3, or 0.  */
          size_t no = STRPREFIX_LIT ("no-", तर्काः) ? 3 : 0;
          /* The length of the possible 'error=' (possibly after
             'no-') prefix: 6, or 0. */
          size_t err = STRPREFIX_LIT ("error=", तर्काः + no) ? 6 : 0;

          warning_argmatch (तर्काः, no, err);
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
severity_उपसर्गः (severity s)
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
severity_print (severity s, FILE *बहिर्गमनम्)
{
  यदि (s != severity_disabled)
    {
      ध्रुव वर्ण* style = severity_style (s);
      begin_use_class (style, बहिर्गमनम्);
      fprintf (बहिर्गमनम्, "%s:", severity_उपसर्गः (s));
      अन्तः_use_class (style, बहिर्गमनम्);
      fputc (' ', बहिर्गमनम्);
    }
}


/*-----------.
| complain.  |
`-----------*/

शून्य
complain_init_color (शून्य)
{
#यदि HAVE_LIBTEXTSTYLE
  यदि (is_styled (stderr))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      संरचना stat statbuf;
      यदि ((style_file_name == रिक्त || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  अन्यथा
    /* No styling.  */
    style_file_name = रिक्त;
#यदि_अन्तः

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#यदि defined __clang__
# pragma clang diagnostic प्रक्षिप
# pragma clang diagnostic ignored "-Wunreachable-code"
#यदि_अन्तः
  errstream =
    color_बहुलकम् == color_html
    ? html_styled_ostream_create (file_ostream_create (stderr),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#यदि defined __clang__
# pragma clang diagnostic निष्कासय
#यदि_अन्तः
}

शून्य
complain_init (शून्य)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  प्रति (size_t b = 0; b < warnings_आकार; ++b)
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
  यदि (flags & fatal)
    /* Diagnostics about fatal errors.  */
    प्रत्यागम severity_fatal;
  अन्यथा यदि (flags & complaint)
    /* Diagnostics about errors.  */
    प्रत्यागम severity_error;
  अन्यथा
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      प्रति (size_t b = 0; b < warnings_आकार; ++b)
        यदि (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            यदि (res == severity_warning
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
  प्रति (size_t b = 0; b < warnings_आकार; ++b)
    यदि (flags & 1 << b && warnings_flag[b] != severity_unset)
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
warnings_print_categories (warnings warn_flags, FILE *बहिर्गमनम्)
{
  प्रति (पूर्णाङ्क wbit = 0; wbit < warnings_आकार; ++wbit)
    यदि (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        ध्रुव वर्ण* style = severity_style (s);
        fputs (" [", बहिर्गमनम्);
        begin_use_class (style, बहिर्गमनम्);
        // E.g., "counterexamples".
        ध्रुव वर्ण *warning = argmatch_warning_तर्कः (&w);
        वर्ण ref[200];
        snprintf (ref, परिमाण ref,
                  "%s#W%s", diagnostics_url, warning);
        begin_hyperlink (बहिर्गमनम्, ref);
        ostream_printf (errstream,
                        "-W%s%s",
                        s == severity_error ? "error=" : "",
                        warning);
        अन्तः_hyperlink (बहिर्गमनम्);
        // Because we mix stdio with ostream I/O, we need to flush
        // here for sake of color == debug.
        flush (बहिर्गमनम्);
        अन्तः_use_class (style, बहिर्गमनम्);
        fputc (']', बहिर्गमनम्);
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
error_सन्देशः (ध्रुव location *loc, warnings flags,
               severity sever, ध्रुव वर्ण *सन्देशः, va_list तर्काः)
{
  ध्रुव वर्ण* style = flags & note ? "note" : severity_style (sever);

  यदि (loc)
    location_print (*loc, stderr);
  अन्यथा
    fprintf (stderr, "%s", grammar_file ? grammar_file : क्रमादेशः_name);
  fprintf (stderr, ": ");

  यदि (sever != severity_disabled)
    {
      begin_use_class (style, stderr);
      fprintf (stderr, "%s:", flags & note ? _("note") : severity_उपसर्गः (sever));
      अन्तः_use_class (style, stderr);
      fputc (' ', stderr);
    }

  vfprintf (stderr, सन्देशः, तर्काः);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  यदि (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, stderr);

  size_t l = वाक्यदैर्घ्यम् (सन्देशः);
  यदि (l < 2 || सन्देशः[l - 2] != ':' || सन्देशः[l - 1] != ' ')
    {
      putc ('\n', stderr);
      flush (stderr);
      यदि (loc && !(flags & no_caret))
        location_caret (*loc, style, stderr);
    }
  flush (stderr);
}

/** Raise a complaint (fatal error, error or just warning).  */

स्थिर शून्य
complains (ध्रुव location *loc, warnings flags,
           ध्रुव वर्ण *सन्देशः, va_list तर्काः)
{
  यदि ((flags & complaint) && complaint_स्थितिः < स्थितिः_complaint)
    complaint_स्थितिः = स्थितिः_complaint;

  severity s = warning_severity (flags);
  यदि (severity_warning <= s)
    {
      यदि (severity_error <= s && ! complaint_स्थितिः)
        complaint_स्थितिः = स्थितिः_warning_as_error;
      error_सन्देशः (loc, flags, s, सन्देशः, तर्काः);
    }

  यदि (flags & fatal)
    निर्गच्छ (EXIT_FAILURE);
}

शून्य
complain (location ध्रुव *loc, warnings flags, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः;
  va_start (तर्काः, सन्देशः);
  complains (loc, flags, सन्देशः, तर्काः);
  va_अन्तः (तर्काः);
}

शून्य
subcomplain (location ध्रुव *loc, warnings flags, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः;
  va_start (तर्काः, सन्देशः);
  complains (loc, flags | note | silent, सन्देशः, तर्काः);
  va_अन्तः (तर्काः);
}

शून्य
complain_तर्काः (location ध्रुव *loc, warnings w,
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
  यदि (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, stderr);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_पञ्जिकाकोशः (loc, upd);
    }
}

शून्य
duplicate_directive (वर्ण ध्रुव *directive,
                     location first, location second)
{
  यदि (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  अन्यथा
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_पञ्जिकाकोशः (&second, "");
}

शून्य
duplicate_नियमः_directive (वर्ण ध्रुव *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_पञ्जिकाकोशः (&second, "");
}

शून्य
syntax_error (location loc,
              पूर्णाङ्क argc, ध्रुव वर्ण* argv[])
{
  यदि (complaint_स्थितिः < स्थितिः_complaint)
    complaint_स्थितिः = स्थितिः_complaint;
  प्रतिज्ञा (argc <= 5);
  ध्रुव वर्ण *format = रिक्त;
  विकल्प (argc)
    {
#परिभाषय CASE(N, S)                          \
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
#अपरिभाषय CASE
    }
  location_print (loc, stderr);
  fputs (": ", stderr);
  severity_print (severity_error, stderr);

  यावत् (*format)
    यदि (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        पूर्णाङ्क i = format[1] - '0';
        ध्रुव वर्ण *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, stderr);
        fputs (argv[i], stderr);
        अन्तः_use_class (style, stderr);
        format += 4;
      }
    अन्यथा
      {
        fputc (*format, stderr);
        ++format;
      }
  fputc ('\n', stderr);
  location_caret (loc, "error", stderr);
}
