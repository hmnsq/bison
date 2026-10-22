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
स्थिर स्थिरांक अक्षर *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
प्रकारव्याख्या गणना
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
स्थिर errority errority_flag[warnings_size];

/** Diagnostics severity.  */
प्रकारव्याख्या गणना
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
begin_use_class (स्थिरांक अक्षर *s, खाता *out)
{
  जर (out == मानक_त्रुटि)
    {
      जर (color_debug)
        ख_लिखो (out, "<%s>", s);
      अन्यथा
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

शून्य
end_use_class (स्थिरांक अक्षर *s, खाता *out)
{
  जर (out == मानक_त्रुटि)
    {
      जर (color_debug)
        ख_लिखो (out, "</%s>", s);
      अन्यथा
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

स्थिर शून्य
begin_hyperlink (खाता *out, स्थिरांक अक्षर *ref)
{
  जर (out == मानक_त्रुटि)
    styled_ostream_set_hyperlink (errstream, ref, रिक्त);
}

स्थिर शून्य
end_hyperlink (खाता *out)
{
  जर (out == मानक_त्रुटि)
    styled_ostream_set_hyperlink (errstream, रिक्त, रिक्त);
}

शून्य
flush (खाता *out)
{
  जर (out == मानक_त्रुटि)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  ख_साफ (out);
}

bool
is_styled (खाता *out)
{
  जर (out != मानक_त्रुटि)
    परत false;
  जर (color_debug)
    परत true;
#जर HAVE_LIBTEXTSTYLE
  परत (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#अन्यथा
  परत false;
#endif
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

स्थिर स्थिरांक argmatch_warning_doc argmatch_warning_docs[] =
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

स्थिर स्थिरांक argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_मूल्ये },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { रिक्त, Wnone }
};

स्थिरांक argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  रिक्त
};

शून्य
warning_usage (खाता *out)
{
  argmatch_warning_usage (out);
}

शून्य
warning_argmatch (अक्षर स्थिरांक *arg, माप_प्रकार no, माप_प्रकार err)
{
  पूर्णांक मूल्य = *argmatch_warning_मूल्य ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  जर (!मूल्य)
    {
      मूल्य = Weverything;
      no = !no;
    }

  साठी (माप_प्रकार b = 0; b < warnings_size; ++b)
    जर (मूल्य & 1 << b)
      {
        जर (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        अन्यथा जर (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        अन्यथा जर (no)
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
warnings_argmatch (अक्षर *args)
{
  जर (!args)
    warning_argmatch ("all", 0, 0);
  अन्यथा जर (STREQ (args, "help"))
    {
      warning_usage (मानक_निकास);
      निकास (निकास_सफल);
    }
  अन्यथा
    साठी (args = म_मोहर (args, ","); args; args = म_मोहर (रिक्त, ","))
      जर (STREQ (args, "error"))
        warnings_are_errors = true;
      अन्यथा जर (STREQ (args, "no-error"))
        warnings_are_errors = false;
      अन्यथा
        {
          /* The length of the possible 'no-' prefix: 3, or 0.  */
          माप_प्रकार no = STRPREFIX_LIT ("no-", args) ? 3 : 0;
          /* The length of the possible 'error=' (possibly after
             'no-') prefix: 6, or 0. */
          माप_प्रकार err = STRPREFIX_LIT ("error=", args + no) ? 6 : 0;

          warning_argmatch (args, no, err);
        }
}

/* Color style for this type of message.  */
स्थिर स्थिरांक अक्षर*
severity_style (severity s)
{
  निवड (s)
    {
    स्थिती severity_disabled:
    स्थिती severity_unset:
      परत "note";
    स्थिती severity_warning:
      परत "warning";
    स्थिती severity_error:
    स्थिती severity_fatal:
      परत "error";
    }
  पात ();
}

/* Prefix for this type of message.  */
स्थिर स्थिरांक अक्षर*
severity_prefix (severity s)
{
  निवड (s)
    {
    स्थिती severity_disabled:
    स्थिती severity_unset:
      परत "";
    स्थिती severity_warning:
      परत _("warning");
    स्थिती severity_error:
      परत  _("error");
    स्थिती severity_fatal:
      परत _("fatal error");
    }
  पात ();
}


स्थिर शून्य
severity_print (severity s, खाता *out)
{
  जर (s != severity_disabled)
    {
      स्थिरांक अक्षर* style = severity_style (s);
      begin_use_class (style, out);
      ख_लिखो (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      ख_अक्षर_दो (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

शून्य
complain_init_color (शून्य)
{
#जर HAVE_LIBTEXTSTYLE
  जर (is_styled (मानक_त्रुटि))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      संरचना stat statbuf;
      जर ((style_file_name == रिक्त || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  अन्यथा
    /* No styling.  */
    style_file_name = रिक्त;
#endif

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#जर defined __clang__
# pragma clang diagnostic ढकला
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (मानक_त्रुटि),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#जर defined __clang__
# pragma clang diagnostic काढा
#endif
}

शून्य
complain_init (शून्य)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  साठी (माप_प्रकार b = 0; b < warnings_size; ++b)
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
  जर (flags & fatal)
    /* Diagnostics about fatal errors.  */
    परत severity_fatal;
  अन्यथा जर (flags & complaint)
    /* Diagnostics about errors.  */
    परत severity_error;
  अन्यथा
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      साठी (माप_प्रकार b = 0; b < warnings_size; ++b)
        जर (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            जर (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      परत res;
    }
}

bool
warning_is_unset (warnings flags)
{
  साठी (माप_प्रकार b = 0; b < warnings_size; ++b)
    जर (flags & 1 << b && warnings_flag[b] != severity_unset)
      परत false;
  परत true;
}

bool
warning_is_enabled (warnings flags)
{
  परत severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

स्थिर शून्य
warnings_print_categories (warnings warn_flags, खाता *out)
{
  साठी (पूर्णांक wbit = 0; wbit < warnings_size; ++wbit)
    जर (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        स्थिरांक अक्षर* style = severity_style (s);
        ख_माला_दो (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        स्थिरांक अक्षर *warning = argmatch_warning_argument (&w);
        अक्षर ref[200];
        snprintf (ref, आकार ref,
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
        ख_अक्षर_दो (']', out);
        /* Display only the first match, the second is "-Wall".  */
        परत;
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
error_message (स्थिरांक location *loc, warnings flags,
               severity sever, स्थिरांक अक्षर *message, बहु_सूची args)
{
  स्थिरांक अक्षर* style = flags & note ? "note" : severity_style (sever);

  जर (loc)
    location_print (*loc, मानक_त्रुटि);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "%s", grammar_file ? grammar_file : program_name);
  ख_लिखो (मानक_त्रुटि, ": ");

  जर (sever != severity_disabled)
    {
      begin_use_class (style, मानक_त्रुटि);
      ख_लिखो (मानक_त्रुटि, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, मानक_त्रुटि);
      ख_अक्षर_दो (' ', मानक_त्रुटि);
    }

  भख_लिखो (मानक_त्रुटि, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  जर (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, मानक_त्रुटि);

  माप_प्रकार l = म_माप (message);
  जर (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      अ_दो ('\n', मानक_त्रुटि);
      flush (मानक_त्रुटि);
      जर (loc && !(flags & no_caret))
        location_caret (*loc, style, मानक_त्रुटि);
    }
  flush (मानक_त्रुटि);
}

/** Raise a complaint (fatal error, error or just warning).  */

स्थिर शून्य
complains (स्थिरांक location *loc, warnings flags,
           स्थिरांक अक्षर *message, बहु_सूची args)
{
  जर ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  जर (severity_warning <= s)
    {
      जर (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  जर (flags & fatal)
    निकास (निकास_त्रुटि);
}

शून्य
complain (location स्थिरांक *loc, warnings flags, स्थिरांक अक्षर *message, ...)
{
  बहु_सूची args;
  बहु_शुरू (args, message);
  complains (loc, flags, message, args);
  बहु_पूर्ण (args);
}

शून्य
subcomplain (location स्थिरांक *loc, warnings flags, स्थिरांक अक्षर *message, ...)
{
  बहु_सूची args;
  बहु_शुरू (args, message);
  complains (loc, flags | note | silent, message, args);
  बहु_पूर्ण (args);
}

शून्य
complain_args (location स्थिरांक *loc, warnings w,
               पूर्णांक argc, अक्षर *argv[])
{
  निवड (argc)
  {
  स्थिती 1:
    complain (loc, w, "%s", _(argv[0]));
    थांबा;
  स्थिती 2:
    complain (loc, w, _(argv[0]), argv[1]);
    थांबा;
  स्थिती 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    थांबा;
  स्थिती 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    थांबा;
  स्थिती 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    थांबा;
  पूर्वनिर्धारित:
    complain (loc, fatal, "too many arguments for complains");
    थांबा;
  }
}


शून्य
bison_directive (location स्थिरांक *loc, अक्षर स्थिरांक *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

शून्य
deprecated_directive (location स्थिरांक *loc, अक्षर स्थिरांक *old, अक्षर स्थिरांक *upd)
{
  जर (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, मानक_त्रुटि);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

शून्य
duplicate_directive (अक्षर स्थिरांक *directive,
                     location first, location second)
{
  जर (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  अन्यथा
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

शून्य
duplicate_rule_directive (अक्षर स्थिरांक *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

शून्य
syntax_error (location loc,
              पूर्णांक argc, स्थिरांक अक्षर* argv[])
{
  जर (complaint_status < status_complaint)
    complaint_status = status_complaint;
  प्रतिज्ञा (argc <= 5);
  स्थिरांक अक्षर *format = रिक्त;
  निवड (argc)
    {
#define CASE(N, S)                          \
      स्थिती N:                               \
        format = S;                         \
        थांबा
    पूर्वनिर्धारित: /* Avoid compiler warnings. */
      CASE (0, _("syntax error"));
      CASE (1, _("unexpected %0$s"));
      CASE (2, _("expected %1$s before %0$s"));
      CASE (3, _("expected %1$s or %2$s before %0$s"));
      CASE (4, _("expected %1$s or %2$s or %3$s before %0$s"));
      CASE (5, _("expected %1$s or %2$s or %3$s or %4$s before %0$s"));
#undef CASE
    }
  location_print (loc, मानक_त्रुटि);
  ख_माला_दो (": ", मानक_त्रुटि);
  severity_print (severity_error, मानक_त्रुटि);

  पर्यंत (*format)
    जर (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        पूर्णांक i = format[1] - '0';
        स्थिरांक अक्षर *style = i == 0 ? "unexpected" : "expected";
        begin_use_class (style, मानक_त्रुटि);
        ख_माला_दो (argv[i], मानक_त्रुटि);
        end_use_class (style, मानक_त्रुटि);
        format += 4;
      }
    अन्यथा
      {
        ख_अक्षर_दो (*format, मानक_त्रुटि);
        ++format;
      }
  ख_अक्षर_दो ('\n', मानक_त्रुटि);
  location_caret (loc, "error", मानक_त्रुटि);
}
