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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <argmatch.h>
#समावेश <c-ctype.h>
#समावेश <progname.h>
#समावेश <stdarg.h>
#समावेश <sys/stat.h>
#समावेश <textstyle.h>

#समावेश "complain.h"
#समावेश "files.h"
#समावेश "fixits.h"
#समावेश "getargs.h"
#समावेश "quote.h"

// The URL of the manual page about diagnostics.  Use the per-node
// manual, to avoid downloading repeatedly the whole manual over the
// Internet.
अटल स्थिर अक्षर *diagnostics_url
  = "https://www.gnu.org/software/bison/manual/html_node/Diagnostics.html";


err_status complaint_status = status_none;

bool warnings_are_errors = false;

/** Whether -Werror/-Wno-error was applied to a warning.  */
प्रकार क्रमागत
  {
    errority_unset = 0,     /** No explicit status.  */
    errority_disabled = 1,  /** Explicitly disabled with -Wno-error=foo.  */
    errority_enabled = 2    /** Explicitly enabled with -Werror=foo. */
  } errority;

/** For each warning type, its errority.  */
अटल errority errority_flag[warnings_आकार];

/** Diagnostics severity.  */
प्रकार क्रमागत
  {
    severity_disabled = 0, /**< Explicitly disabled via -Wno-foo.  */
    severity_unset = 1,    /**< Unspecified status.  */
    severity_warning = 2,  /**< A warning.  */
    severity_error = 3,    /**< An error (continue, but die soon).  */
    severity_fatal = 4     /**< Fatal error (die now).  */
  } severity;


/** For each warning type, its severity.  */
अटल severity warnings_flag[warnings_आकार];

styled_ostream_t errstream = शून्य;

व्योम
begin_use_class (स्थिर अक्षर *s, खाता *out)
{
  अगर (out == मानक_त्रुटि)
    {
      अगर (color_debug)
        ख_लिखो (out, "<%s>", s);
      अन्यथा
        {
          styled_ostream_begin_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

व्योम
end_use_class (स्थिर अक्षर *s, खाता *out)
{
  अगर (out == मानक_त्रुटि)
    {
      अगर (color_debug)
        ख_लिखो (out, "</%s>", s);
      अन्यथा
        {
          styled_ostream_end_use_class (errstream, s);
          styled_ostream_flush_to_current_style (errstream);
        }
    }
}

अटल व्योम
begin_hyperlink (खाता *out, स्थिर अक्षर *ref)
{
  अगर (out == मानक_त्रुटि)
    styled_ostream_set_hyperlink (errstream, ref, शून्य);
}

अटल व्योम
end_hyperlink (खाता *out)
{
  अगर (out == मानक_त्रुटि)
    styled_ostream_set_hyperlink (errstream, शून्य, शून्य);
}

व्योम
flush (खाता *out)
{
  अगर (out == मानक_त्रुटि)
    ostream_flush (errstream, FLUSH_THIS_STREAM);
  ख_साफ (out);
}

bool
is_styled (खाता *out)
{
  अगर (out != मानक_त्रुटि)
    वापस false;
  अगर (color_debug)
    वापस true;
#अगर HAVE_LIBTEXTSTYLE
  वापस (color_mode == color_yes
          || color_mode == color_html
          || (color_mode == color_tty && isatty (STDERR_FILENO)));
#अन्यथा
  वापस false;
#पूर्ण_अगर
}


/*------------------------.
| --warnings's handling.  |
`------------------------*/

ARGMATCH_DEFINE_GROUP (warning, warnings)

अटल स्थिर argmatch_warning_doc argmatch_warning_docs[] =
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
  { शून्य, शून्य }
};

अटल स्थिर argmatch_warning_arg argmatch_warning_args[] =
{
  { "all",             Wall },
  { "conflicts-rr",    Wconflicts_rr },
  { "conflicts-sr",    Wconflicts_sr },
  { "counterexamples", Wcounterexamples }, { "cex", Wcounterexamples }, // Show cex second.
  { "dangling-alias",  Wdangling_alias },
  { "deprecated",      Wdeprecated },
  { "empty-rule",      Wempty_rule },
  { "everything",      Weverything },
  { "midrule-values",  Wmidrule_मूल्यावली },
  { "none",            Wnone },
  { "other",           Wother },
  { "precedence",      Wprecedence },
  { "yacc",            Wyacc },
  { शून्य, Wnone }
};

स्थिर argmatch_warning_group_type argmatch_warning_group =
{
  argmatch_warning_args,
  argmatch_warning_docs,
  N_("Warning categories include:"),
  शून्य
};

व्योम
warning_usage (खाता *out)
{
  argmatch_warning_usage (out);
}

व्योम
warning_argmatch (अक्षर स्थिर *arg, माप_प्रकार no, माप_प्रकार err)
{
  पूर्णांक मूल्य = *argmatch_warning_मूल्य ("--warning", arg + no + err);

  /* -Wnone == -Wno-everything, and -Wno-none == -Weverything.  */
  अगर (!मूल्य)
    {
      मूल्य = Weverything;
      no = !no;
    }

  क्रम (माप_प्रकार b = 0; b < warnings_आकार; ++b)
    अगर (मूल्य & 1 << b)
      {
        अगर (err && no)
          /* -Wno-error=foo.  */
          errority_flag[b] = errority_disabled;
        अन्यथा अगर (err && !no)
          {
            /* -Werror=foo: enables -Wfoo. */
            errority_flag[b] = errority_enabled;
            warnings_flag[b] = severity_warning;
          }
        अन्यथा अगर (no)
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

व्योम
warnings_argmatch (अक्षर *args)
{
  अगर (!args)
    warning_argmatch ("all", 0, 0);
  अन्यथा अगर (STREQ (args, "help"))
    {
      warning_usage (मानक_निकास);
      निकास (निकास_सफल);
    }
  अन्यथा
    क्रम (args = म_मोहर (args, ","); args; args = म_मोहर (शून्य, ","))
      अगर (STREQ (args, "error"))
        warnings_are_errors = true;
      अन्यथा अगर (STREQ (args, "no-error"))
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
अटल स्थिर अक्षर*
severity_style (severity s)
{
  चयन (s)
    {
    हाल severity_disabled:
    हाल severity_unset:
      वापस "note";
    हाल severity_warning:
      वापस "warning";
    हाल severity_error:
    हाल severity_fatal:
      वापस "error";
    }
  पात ();
}

/* Prefix for this type of message.  */
अटल स्थिर अक्षर*
severity_prefix (severity s)
{
  चयन (s)
    {
    हाल severity_disabled:
    हाल severity_unset:
      वापस "";
    हाल severity_warning:
      वापस _("warning");
    हाल severity_error:
      वापस  _("error");
    हाल severity_fatal:
      वापस _("fatal error");
    }
  पात ();
}


अटल व्योम
severity_print (severity s, खाता *out)
{
  अगर (s != severity_disabled)
    {
      स्थिर अक्षर* style = severity_style (s);
      begin_use_class (style, out);
      ख_लिखो (out, "%s:", severity_prefix (s));
      end_use_class (style, out);
      ख_अक्षर_दो (' ', out);
    }
}


/*-----------.
| complain.  |
`-----------*/

व्योम
complain_init_color (व्योम)
{
#अगर HAVE_LIBTEXTSTYLE
  अगर (is_styled (मानक_त्रुटि))
    {
      style_file_prepare ("BISON_STYLE", "BISON_STYLEDIR", pkgdatadir (),
                          "bison-default.css");
      /* As a fallback, use the default in the current directory.  */
      काष्ठा stat statbuf;
      अगर ((style_file_name == शून्य || stat (style_file_name, &statbuf) < 0)
          && stat ("bison-default.css", &statbuf) == 0)
        style_file_name = "bison-default.css";
    }
  अन्यथा
    /* No styling.  */
    style_file_name = शून्य;
#पूर्ण_अगर

  /* Workaround clang's warning (starting at Clang 3.5) about the stub
     code of html_styled_ostream_create:

     | src/complain.c:274:7: error: code will never be executed [-Werror,-Wunreachable-code]
     |     ? html_styled_ostream_create (file_ostream_create (stderr),
     |       ^~~~~~~~~~~~~~~~~~~~~~~~~~ */
#अगर defined __clang__
# pragma clang diagnostic धकेलो
# pragma clang diagnostic ignored "-Wunreachable-code"
#पूर्ण_अगर
  errstream =
    color_mode == color_html
    ? html_styled_ostream_create (file_ostream_create (मानक_त्रुटि),
                                  style_file_name)
    : styled_ostream_create (STDERR_FILENO, "(stderr)", TTYCTL_AUTO,
                             style_file_name);
#अगर defined __clang__
# pragma clang diagnostic निकालो
#पूर्ण_अगर
}

व्योम
complain_init (व्योम)
{
  caret_init ();

  warnings warnings_default =
    Wconflicts_sr | Wconflicts_rr | Wdeprecated | Wother;

  क्रम (माप_प्रकार b = 0; b < warnings_आकार; ++b)
    {
      warnings_flag[b] = (1 << b & warnings_default
                          ? severity_warning
                          : severity_unset);
      errority_flag[b] = errority_unset;
    }
}

व्योम
complain_free (व्योम)
{
  caret_free ();
  styled_ostream_free (errstream);
}

/* A diagnostic with FLAGS is about to be issued.  With what severity?
   (severity_fatal, severity_error, severity_disabled, or
   severity_warning.) */

अटल severity
warning_severity (warnings flags)
{
  अगर (flags & fatal)
    /* Diagnostics about fatal errors.  */
    वापस severity_fatal;
  अन्यथा अगर (flags & complaint)
    /* Diagnostics about errors.  */
    वापस severity_error;
  अन्यथा
    {
      /* Diagnostics about warnings.  */
      severity res = severity_disabled;
      क्रम (माप_प्रकार b = 0; b < warnings_आकार; ++b)
        अगर (flags & 1 << b)
          {
            res = res < warnings_flag[b] ? warnings_flag[b] : res;
            /* If the diagnostic is enabled, and -Werror is enabled,
               and -Wno-error=foo was not explicitly requested, this
               is an error. */
            अगर (res == severity_warning
                && (errority_flag[b] == errority_enabled
                    || (warnings_are_errors
                        && errority_flag[b] != errority_disabled)))
              res = severity_error;
          }
      वापस res;
    }
}

bool
warning_is_unset (warnings flags)
{
  क्रम (माप_प्रकार b = 0; b < warnings_आकार; ++b)
    अगर (flags & 1 << b && warnings_flag[b] != severity_unset)
      वापस false;
  वापस true;
}

bool
warning_is_enabled (warnings flags)
{
  वापस severity_warning <= warning_severity (flags);
}

/** Display a "[-Wyacc]" like message on \a out.  */

अटल व्योम
warnings_print_categories (warnings warn_flags, खाता *out)
{
  क्रम (पूर्णांक wbit = 0; wbit < warnings_आकार; ++wbit)
    अगर (warn_flags & (1 << wbit))
      {
        warnings w = 1 << wbit;
        severity s = warning_severity (w);
        स्थिर अक्षर* style = severity_style (s);
        ख_माला_दो (" [", out);
        begin_use_class (style, out);
        // E.g., "counterexamples".
        स्थिर अक्षर *warning = argmatch_warning_argument (&w);
        अक्षर ref[200];
        snprintf (ref, माप ref,
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
        वापस;
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
अटल
व्योम
error_message (स्थिर location *loc, warnings flags,
               severity sever, स्थिर अक्षर *message, बहु_सूची args)
{
  स्थिर अक्षर* style = flags & note ? "note" : severity_style (sever);

  अगर (loc)
    location_print (*loc, मानक_त्रुटि);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "%s", grammar_file ? grammar_file : program_name);
  ख_लिखो (मानक_त्रुटि, ": ");

  अगर (sever != severity_disabled)
    {
      begin_use_class (style, मानक_त्रुटि);
      ख_लिखो (मानक_त्रुटि, "%s:", flags & note ? _("note") : severity_prefix (sever));
      end_use_class (style, मानक_त्रुटि);
      ख_अक्षर_दो (' ', मानक_त्रुटि);
    }

  भख_लिखो (मानक_त्रुटि, message, args);
  /* Print the type of warning, only if this is not a sub message
     (in which case the prefix is null).  */
  अगर (! (flags & silent) && sever != severity_disabled)
    warnings_print_categories (flags, मानक_त्रुटि);

  माप_प्रकार l = म_माप (message);
  अगर (l < 2 || message[l - 2] != ':' || message[l - 1] != ' ')
    {
      अ_दो ('\n', मानक_त्रुटि);
      flush (मानक_त्रुटि);
      अगर (loc && !(flags & no_caret))
        location_caret (*loc, style, मानक_त्रुटि);
    }
  flush (मानक_त्रुटि);
}

/** Raise a complaint (fatal error, error or just warning).  */

अटल व्योम
complains (स्थिर location *loc, warnings flags,
           स्थिर अक्षर *message, बहु_सूची args)
{
  अगर ((flags & complaint) && complaint_status < status_complaint)
    complaint_status = status_complaint;

  severity s = warning_severity (flags);
  अगर (severity_warning <= s)
    {
      अगर (severity_error <= s && ! complaint_status)
        complaint_status = status_warning_as_error;
      error_message (loc, flags, s, message, args);
    }

  अगर (flags & fatal)
    निकास (निकास_त्रुटि);
}

व्योम
complain (location स्थिर *loc, warnings flags, स्थिर अक्षर *message, ...)
{
  बहु_सूची args;
  बहु_शुरू (args, message);
  complains (loc, flags, message, args);
  बहु_पूर्ण (args);
}

व्योम
subcomplain (location स्थिर *loc, warnings flags, स्थिर अक्षर *message, ...)
{
  बहु_सूची args;
  बहु_शुरू (args, message);
  complains (loc, flags | note | silent, message, args);
  बहु_पूर्ण (args);
}

व्योम
complain_args (location स्थिर *loc, warnings w,
               पूर्णांक argc, अक्षर *argv[])
{
  चयन (argc)
  {
  हाल 1:
    complain (loc, w, "%s", _(argv[0]));
    अवरोध;
  हाल 2:
    complain (loc, w, _(argv[0]), argv[1]);
    अवरोध;
  हाल 3:
    complain (loc, w, _(argv[0]), argv[1], argv[2]);
    अवरोध;
  हाल 4:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3]);
    अवरोध;
  हाल 5:
    complain (loc, w, _(argv[0]), argv[1], argv[2], argv[3], argv[4]);
    अवरोध;
  शेष:
    complain (loc, fatal, "too many arguments for complains");
    अवरोध;
  }
}


व्योम
bison_directive (location स्थिर *loc, अक्षर स्थिर *directive)
{
  complain (loc, Wyacc,
            _("POSIX Yacc does not support %s"), directive);
}

व्योम
deprecated_directive (location स्थिर *loc, अक्षर स्थिर *old, अक्षर स्थिर *upd)
{
  अगर (warning_is_enabled (Wdeprecated))
    {
      complain (loc, Wdeprecated,
                _("deprecated directive: %s, use %s"),
                quote (old), quote_n (1, upd));
      location_caret_suggestion (*loc, upd, मानक_त्रुटि);
      /* Register updates only if -Wdeprecated is enabled.  */
      fixits_register (loc, upd);
    }
}

व्योम
duplicate_directive (अक्षर स्थिर *directive,
                     location first, location second)
{
  अगर (feature_flag & feature_caret)
    complain (&second, Wother, _("duplicate directive"));
  अन्यथा
    complain (&second, Wother, _("duplicate directive: %s"), quote (directive));
  subcomplain (&first, Wother, _("previous declaration"));
  fixits_register (&second, "");
}

व्योम
duplicate_rule_directive (अक्षर स्थिर *directive,
                          location first, location second)
{
  complain (&second, complaint, _("only one %s allowed per rule"), directive);
  subcomplain (&first, complaint, _("previous declaration"));
  fixits_register (&second, "");
}

व्योम
syntax_error (location loc,
              पूर्णांक argc, स्थिर अक्षर* argv[])
{
  अगर (complaint_status < status_complaint)
    complaint_status = status_complaint;
  निश्चित (argc <= 5);
  स्थिर अक्षर *format = शून्य;
  चयन (argc)
    {
#घोषणा CASE(N, S)                          \
      हाल N:                               \
        format = S;                         \
        अवरोध
    शेष: /* Avoid compiler warnings. */
      CASE (0, _("syntax error"));
      CASE (1, _("unexpected %0$s"));
      CASE (2, _("expected %1$s before %0$s"));
      CASE (3, _("expected %1$s or %2$s before %0$s"));
      CASE (4, _("expected %1$s or %2$s or %3$s before %0$s"));
      CASE (5, _("expected %1$s or %2$s or %3$s or %4$s before %0$s"));
#अघोषित CASE
    }
  location_print (loc, मानक_त्रुटि);
  ख_माला_दो (": ", मानक_त्रुटि);
  severity_print (severity_error, मानक_त्रुटि);

  जबतक (*format)
    अगर (format[0] == '%'
        && c_isdigit (format[1])
        && format[2] == '$'
        && format[3] == 's'
        && (format[1] - '0') < argc)
      {
        पूर्णांक i = format[1] - '0';
        स्थिर अक्षर *style = i == 0 ? "unexpected" : "expected";
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
