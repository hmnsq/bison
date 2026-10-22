/* Parse command line arguments for Bison.

   Copyright (C) 1984, 1986, 1989, 1992, 2000-2015, 2018-2022 Free
   Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

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

#अन्तर्भाव <config.h>
#अन्तर्भाव "getargs.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <argmatch.h>
#अन्तर्भाव <c-strcase.h>
#अन्तर्भाव <configmake.h>
#अन्तर्भाव <error.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <progname.h>
#अन्तर्भाव <quote.h>
#अन्तर्भाव <textstyle.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "output.h"
#अन्तर्भाव "uniqstr.h"

bool header_flag = false;
bool graph_flag = false;
bool html_flag = false;
bool xml_flag = false;
bool no_ओळी_flag = false;
bool token_table_flag = false;
location yacc_loc = EMPTY_LOCATION_INIT;
bool update_flag = false; /* for -u */
bool color_debug = false;

bool nondeterministic_parser = false;
bool glr_parser = false;

पूर्णांक feature_flag = feature_caret;
पूर्णांक report_flag = report_none;
पूर्णांक trace_flag = trace_none;

स्थिर संरचना bison_language स्थिरांक valid_languages[] = {
  /* lang,  skeleton,       ext,     hdr,     add_tab */
  { "c",    "c-skel.m4",    ".c",    ".h",    true },
  { "c++",  "c++-skel.m4",  ".cc",   ".hh",   true },
  { "d",    "d-skel.m4",    ".d",    ".d",    false },
  { "java", "java-skel.m4", ".java", ".java", false },
  { "", "", "", "", false }
};

पूर्णांक skeleton_prio = default_prio;
स्थिरांक अक्षर *skeleton = रिक्त;
पूर्णांक language_prio = default_prio;
संरचना bison_language स्थिरांक *language = &valid_languages[0];

प्रकारव्याख्या पूर्णांक* (xargmatch_fn) (स्थिरांक अक्षर *context, स्थिरांक अक्षर *arg);

शून्य
set_yacc (location loc)
{
  yacc_loc = loc;
  जर (दो_पर्या ("POSIXLY_CORRECT"))
    muscle_percent_define_घाला ("posix",
                                  loc,
                                  muscle_keyword, "",
                                  MUSCLE_PERCENT_DEFINE_D);
}


/** Decode an option's key.
 *
 *  \param opt        option being decoded.
 *  \param xargmatch  matching function.
 *  \param all        the value of the argument 'all'.
 *  \param flags      the flags to update
 *  \param arg        the subarguments to decode.
 *                    If null, then activate all the flags.
 *  \param no         length of the potential "no-" prefix.
 *                    Can be 0 or 3. If 3, negate the action of the subargument.
 *
 *  If VALUE != 0 then KEY sets flags and no-KEY clears them.
 *  If VALUE == 0 then KEY clears all flags from \c all and no-KEY sets all
 *  flags from \c all.  Thus no-none = all and no-all = none.
 */
स्थिर शून्य
flag_argmatch (स्थिरांक अक्षर *opt, xargmatch_fn xargmatch,
               पूर्णांक all, पूर्णांक *flags, अक्षर *arg, माप_प्रकार no)
{
  पूर्णांक मूल्य = *xargmatch (opt, arg + no);

  /* -rnone == -rno-all, and -rno-none == -rall.  */
  जर (!मूल्य)
    {
      मूल्य = all;
      no = !no;
    }

  जर (no)
    *flags &= ~मूल्य;
  अन्यथा
    *flags |= मूल्य;
}

प्रकारव्याख्या शून्य (usage_fn) (खाता *out);

/** Decode an option's set of keys.
 *
 *  \param opt        option being decoded (e.g., --report).
 *  \param xargmatch  matching function.
 *  \param usage      function that implement --help for this option.
 *  \param all        the value of the argument 'all'.
 *  \param flags      the flags to update
 *  \param args       comma separated list of effective subarguments to decode.
 *                    If 0, then activate all the flags.
 */
स्थिर शून्य
flags_argmatch (स्थिरांक अक्षर *opt,
                xargmatch_fn xargmatch,
                usage_fn usage,
                पूर्णांक all, पूर्णांक *flags, अक्षर *args)
{
  जर (!args)
    *flags |= all;
  अन्यथा जर (STREQ (args, "help"))
    {
      usage (मानक_निकास);
      निकास (निकास_सफल);
    }
  अन्यथा
    साठी (args = म_मोहर (args, ","); args; args = म_मोहर (रिक्त, ","))
      {
        माप_प्रकार no = STRPREFIX_LIT ("no-", args) ? 3 : 0;
        flag_argmatch (opt, xargmatch,
                       all, flags, args, no);
      }
}


/** Decode a set of sub arguments.
 *
 *  \param FlagName  the flag family to update.
 *  \param Args      the effective sub arguments to decode.
 *  \param All       the "all" value.
 *
 *  \arg FlagName_args   the list of keys.
 *  \arg FlagName_types  the list of values.
 *  \arg FlagName_flag   the flag to update.
 */
#define FLAGS_ARGMATCH(FlagName, Args, All)                             \
  flags_argmatch ("--" #FlagName,                                       \
                  (xargmatch_fn*) argmatch_## FlagName ## मूल्य_2,       \
                  argmatch_ ## FlagName ## _usage,                      \
                  All, &FlagName ## _flag, Args)

/*---------------------.
| --color's handling.  |
`---------------------*/

गणना color
  {
    color_always,
    color_never,
    color_auto
  };

ARGMATCH_DEFINE_GROUP (color, गणना color)

स्थिर स्थिरांक argmatch_color_doc argmatch_color_docs[] =
{
  { "always",     N_("colorize the output") },
  { "never",      N_("don't colorize the output") },
  { "auto",       N_("colorize if the output device is a tty") },
  { रिक्त, रिक्त },
};

स्थिर स्थिरांक argmatch_color_arg argmatch_color_args[] =
{
  { "always",   color_always },
  { "yes",      color_always },
  { "never",    color_never },
  { "no",       color_never },
  { "auto",     color_auto },
  { "tty",      color_auto },
  { रिक्त, color_always },
};

स्थिरांक argmatch_color_group_type argmatch_color_group =
{
  argmatch_color_args,
  argmatch_color_docs,
  /* TRANSLATORS: Use the same translation for WHEN as in the
     --color=WHEN help message.  */
  N_("WHEN can be one of the following:"),
  रिक्त
};


/*----------------------.
| --report's handling.  |
`----------------------*/

ARGMATCH_DEFINE_GROUP (report, गणना report)

स्थिर स्थिरांक argmatch_report_doc argmatch_report_docs[] =
{
  { "states",          N_("describe the states") },
  { "itemsets",        N_("complete the core item sets with their closure") },
  { "lookaheads",      N_("explicitly associate lookahead tokens to items") },
  { "solved",          N_("describe shift/reduce conflicts solving") },
  { "counterexamples", N_("generate conflict counterexamples") },
  { "all",             N_("include all the above information") },
  { "none",            N_("disable the report") },
  { रिक्त, रिक्त },
};

स्थिर स्थिरांक argmatch_report_arg argmatch_report_args[] =
{
  { "none",            report_none },
  { "states",          report_states },
  { "itemsets",        report_states | report_itemsets },
  { "lookaheads",      report_states | report_lookaheads },
  { "solved",          report_states | report_solved_conflicts },
  { "counterexamples", report_cex },
  { "cex",             report_cex },
  { "all",             report_all },
  { रिक्त, report_none },
};

स्थिरांक argmatch_report_group_type argmatch_report_group =
{
  argmatch_report_args,
  argmatch_report_docs,
  /* TRANSLATORS: Use the same translation for THINGS as in the
     --report=THINGS help message.  */
  N_("THINGS is a list of comma separated words that can include:"),
  रिक्त
};

/*---------------------.
| --trace's handling.  |
`---------------------*/

ARGMATCH_DEFINE_GROUP (trace, गणना trace)

स्थिर स्थिरांक argmatch_trace_doc argmatch_trace_docs[] =
{
  /* Meant for developers only, don't translate them.  */
  { "none",       "no traces" },
  { "locations",  "full display of the locations" },
  { "scan",       "grammar scanner traces" },
  { "parse",      "grammar parser traces" },
  { "automaton",  "construction of the automaton" },
  { "bitsets",    "use of bitsets" },
  { "closure",    "input/output of closure" },
  { "grammar",    "reading, reducing the grammar" },
  { "resource",   "memory consumption (where available)" },
  { "sets",       "grammar sets: firsts, nullable etc." },
  { "muscles",    "m4 definitions passed to the skeleton" },
  { "tools",      "m4 invocation" },
  { "m4-early",   "m4 traces starting from the start" },
  { "m4",         "m4 traces starting from the skeleton evaluation" },
  { "skeleton",   "skeleton postprocessing" },
  { "time",       "time consumption" },
  { "ielr",       "IELR conversion" },
  { "cex",        "counterexample generation"},
  { "all",        "all of the above" },
  { रिक्त, रिक्त},
};

स्थिर स्थिरांक argmatch_trace_arg argmatch_trace_args[] =
{
  { "none",      trace_none },
  { "locations", trace_locations },
  { "scan",      trace_scan },
  { "parse",     trace_parse },
  { "automaton", trace_automaton },
  { "bitsets",   trace_bitsets },
  { "closure",   trace_closure },
  { "grammar",   trace_grammar },
  { "resource",  trace_resource },
  { "sets",      trace_sets },
  { "muscles",   trace_muscles },
  { "tools",     trace_tools },
  { "m4-early",  trace_m4_early },
  { "m4",        trace_m4 },
  { "skeleton",  trace_skeleton },
  { "time",      trace_time },
  { "ielr",      trace_ielr },
  { "cex",       trace_cex },
  { "all",       trace_all },
  { रिक्त,        trace_none},
};

स्थिरांक argmatch_trace_group_type argmatch_trace_group =
{
  argmatch_trace_args,
  argmatch_trace_docs,
  N_("TRACES is a list of comma separated words that can include:"),
  रिक्त
};

/*-----------------------.
| --feature's handling.  |
`-----------------------*/

ARGMATCH_DEFINE_GROUP (feature, गणना feature)

स्थिर स्थिरांक argmatch_feature_doc argmatch_feature_docs[] =
{
  { "caret",       N_("show errors with carets") },
  { "fixit",       N_("show machine-readable fixes") },
  { "syntax-only", N_("do not generate any file") },
  { "all",         N_("all of the above") },
  { "none",        N_("disable all of the above") },
  { रिक्त, रिक्त }
};

स्थिर स्थिरांक argmatch_feature_arg argmatch_feature_args[] =
{
  { "none",                          feature_none },
  { "caret",                         feature_caret },
  { "diagnostics-show-caret",        feature_caret },
  { "fixit",                         feature_fixit },
  { "diagnostics-parseable-fixits",  feature_fixit },
  { "syntax-only",                   feature_syntax_only },
  { "all",                           feature_all },
  { रिक्त, feature_none}
};

स्थिरांक argmatch_feature_group_type argmatch_feature_group =
{
  argmatch_feature_args,
  argmatch_feature_docs,
  /* TRANSLATORS: Use the same translation for FEATURES as in the
     --feature=FEATURES help message.  */
  N_("FEATURES is a list of comma separated words that can include:"),
  रिक्त
};

/*-------------------------------------------.
| Display the help message and exit STATUS.  |
`-------------------------------------------*/

 _Noreturn
स्थिर शून्य usage (पूर्णांक);

स्थिर शून्य
usage (पूर्णांक status)
{
  जर (status != 0)
    ख_लिखो (मानक_त्रुटि, _("Try '%s --help' for more information.\n"),
             program_name);
  अन्यथा
    {
      /* For ../build-aux/cross-options.pl to work, use the format:
                ^  -S, --long[=ARGS] (whitespace)
         A --long option is required.
         Otherwise, add exceptions to ../build-aux/cross-options.pl.  */

      म_लिखो (_("Usage: %s [OPTION]... FILE\n"), program_name);
      ख_माला_दो (_("\
Generate a deterministic LR or generalized LR (GLR) parser employing\n\
LALR(1), IELR(1), or canonical LR(1) parser tables.\n\
\n\
"), मानक_निकास);

      ख_माला_दो (_("\
Mandatory arguments to दीर्घ options are mandatory साठी लघु options too.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
The same is true साठी optional arguments.\n\
"), मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      ख_माला_दो (_("\
Operation Modes:\n\
  -h, --help                 display this help and निकास\n\
  -V, --version              output version information and निकास\n\
      --print-localedir      output directory containing locale-dependent data\n\
                             and निकास\n\
      --print-datadir        output directory containing skeletons and XSLT\n\
                             and निकास\n\
  -u, --update               apply fixes to the source grammar file and निकास\n\
  -f, --feature[=FEATURES]   activate miscellaneous features\n\
\n\
"), मानक_निकास);

      argmatch_feature_usage (मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      ख_माला_दो (_("\
Diagnostics:\n\
  -W, --warnings[=CATEGORY]  report the warnings falling in CATEGORY\n\
      --color[=WHEN]         whether to colorize the diagnostics\n\
      --style=खाता           specify the CSS खाता साठी colorizer diagnostics\n\
\n\
"), मानक_निकास);

      warning_usage (मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      argmatch_color_usage (मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      ख_माला_दो (_("\
Tuning the Parser:\n\
  -L, --language=LANGUAGE          specify the output programming language\n\
  -S, --skeleton=खाता              specify the skeleton to use\n\
  -t, --debug                      instrument the parser साठी tracing\n\
                                   same as '-Dparse.trace'\n\
      --locations                  enable location support\n\
  -D, --define=NAME[=मूल्य_3]        similar to '%define NAME VALUE'\n\
  -F, --force-define=NAME[=मूल्य_3]  override '%define NAME VALUE'\n\
  -p, --name-prefix=PREFIX         prepend PREFIX to the external symbols\n\
                                   deprecated by '-Dapi.prefix={PREFIX}'\n\
  -l, --no-ओळी                   don't generate '#line' directives\n\
  -k, --token-table                include a table of token names\n\
  -y, --yacc                       emulate POSIX Yacc\n\
"), मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      ख_माला_दो (_("\
Output Files:\n\
  -H, --header=[खाता]           also produce a header file\n\
  -d                            likewise but cannot specify खाता (साठी POSIX Yacc)\n\
  -r, --report=THINGS           also produce details on the automaton\n\
      --report-file=खाता        लिख report to खाता\n\
  -v, --verbose                 same as '--report=state'\n\
  -b, --file-prefix=PREFIX      specify a PREFIX साठी output files\n\
  -o, --output=खाता             leave output to खाता\n\
  -g, --graph[=खाता]            also output a graph of the automaton\n\
      --html[=खाता]             also output an HTML report of the automaton\n\
  -x, --xml[=खाता]              also output an XML report of the automaton\n\
  -M, --file-prefix-map=OLD=NEW replace prefix OLD with NEW when writing file paths\n\
                                in output files\n\
"), मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      argmatch_report_usage (मानक_निकास);
      अ_दो ('\n', मानक_निकास);

      म_लिखो (_("Report bugs to <%s>.\n"), PACKAGE_BUGREPORT);
      म_लिखो (_("%s home page: <%s>.\n"), PACKAGE_NAME, PACKAGE_URL);
      ख_माला_दो (_("General help using GNU software: "
               "<https://www.gnu.org/gethelp/>.\n"),
             मानक_निकास);

#जर (defined __GLIBC__ && __GLIBC__ >= 2) && !defined __UCLIBC__
      /* Don't output this redundant message for English locales.
         Note we still output for 'C' so that it gets included in the
         man page.  */
      स्थिरांक अक्षर *lc_messages = रखो_क्षेत्र (LC_MESSAGES, रिक्त);
      जर (lc_messages && !STREQ (lc_messages, "en_"))
        /* TRANSLATORS: Replace LANG_CODE in this URL with your language code to
           form one of the URLs at https://translationproject.org/team/.
           Otherwise, replace the entire URL with your translation team's
           email address.  */
        ख_माला_दो (_("Report translation bugs to "
                 "<https://translationproject.org/team/>.\n"), मानक_निकास);
#endif
      ख_माला_दो (_("For complete documentation, run: info bison.\n"), मानक_निकास);
    }

  निकास (status);
}


/*------------------------------.
| Display the version message.  |
`------------------------------*/

स्थिर शून्य
version (शून्य)
{
  /* Some efforts were made to ease the translators' task, please
     continue.  */
  म_लिखो ("bison (GNU Bison) %s", VERSION);
  अ_दो ('\n', मानक_निकास);
  ख_माला_दो (_("Written by Robert Corbett and Richard Stallman.\n"), मानक_निकास);
  अ_दो ('\n', मानक_निकास);

  ख_लिखो (मानक_निकास,
           _("Copyright (C) %d Free Software Foundation, Inc.\n"),
           PACKAGE_COPYRIGHT_YEAR);

  ख_माला_दो (_("\
This is मुक्त software; see the source साठी copying conditions.  There is NO\n\
warranty; not even साठी MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
"),
         मानक_निकास);
}


/*-------------------------------------.
| --skeleton and --language handling.  |
`--------------------------------------*/

शून्य
skeleton_arg (अक्षर स्थिरांक *arg, पूर्णांक prio, location loc)
{
  जर (prio < skeleton_prio)
    {
      skeleton_prio = prio;
      skeleton = arg;
    }
  अन्यथा जर (prio == skeleton_prio)
    complain (&loc, complaint,
              _("multiple skeleton declarations are invalid"));
}

शून्य
language_argmatch (अक्षर स्थिरांक *arg, पूर्णांक prio, location loc)
{
  अक्षर स्थिरांक *संदेश = रिक्त;

  जर (prio < language_prio)
    {
      साठी (पूर्णांक i = 0; valid_languages[i].language[0]; ++i)
        जर (c_strcasecmp (arg, valid_languages[i].language) == 0)
          {
            language_prio = prio;
            language = &valid_languages[i];
            परत;
          }
      संदेश = _("%s: invalid language");
    }
  अन्यथा जर (language_prio == prio)
    संदेश = _("multiple language declarations are invalid");

  जर (संदेश)
    complain (&loc, complaint, संदेश, quotearg_colon (arg));
}

/*----------------------.
| Process the options.  |
`----------------------*/

/* Shorts options.
   Should be computed from long_options.  */
स्थिर अक्षर स्थिरांक short_options[] =
  "D:"
  "F:"
  "H::"
  "L:"
  "S:"
  "T::"
  "V"
  "W::"
  "b:"
  "d"
  "f::"
  "g::"
  "h"
  "k"
  "l"
  "M:"
  "o:"
  "p:"
  "r:"
  "t"
  "u"   /* --update */
  "v"
  "x::"
  "y"
  ;

/* Values for long options that do not have single-letter equivalents.  */
गणना
{
  COLOR_OPTION = अक्षर_उच्च + 1,
  FIXED_OUTPUT_FILES_OPTION,
  HTML_OPTION,
  LOCATIONS_OPTION,
  PRINT_DATADIR_OPTION,
  PRINT_LOCALEDIR_OPTION,
  REPORT_FILE_OPTION,
  STYLE_OPTION
};

/* In the same order as in usage(), and in the documentation.  */
स्थिर संरचना option स्थिरांक long_options[] =
{
  /* Operation modes. */
  { "help",            no_argument,       0,   'h' },
  { "version",         no_argument,       0,   'V' },
  { "print-localedir", no_argument,       0,   PRINT_LOCALEDIR_OPTION },
  { "print-datadir",   no_argument,       0,   PRINT_DATADIR_OPTION   },
  { "update",          no_argument,       0,   'u' },
  { "feature",         optional_argument, 0,   'f' },

  /* Diagnostics.  */
  { "warnings",        optional_argument,  0, 'W' },
  { "color",           optional_argument,  0,  COLOR_OPTION },
  { "style",           optional_argument,  0,  STYLE_OPTION },

  /* Tuning the Parser. */
  { "language",       required_argument,   0, 'L' },
  { "skeleton",       required_argument,   0, 'S' },
  { "debug",          no_argument,         0, 't' },
  { "locations",      no_argument,         0, LOCATIONS_OPTION },
  { "define",         required_argument,   0, 'D' },
  { "force-define",   required_argument,   0, 'F' },
  { "name-prefix",    required_argument,   0, 'p' },
  { "no-lines",       no_argument,         0, 'l' },
  { "token-table",    no_argument,         0, 'k' },
  { "yacc",           no_argument,         0, 'y' },

  /* Output Files. */
  { "header",          optional_argument,   0,   'H' },
  { "defines",         optional_argument,   0,   'd' },
  { "report",          required_argument,   0,   'r' },
  { "report-file",     required_argument,   0,   REPORT_FILE_OPTION },
  { "verbose",         no_argument,         0,   'v' },
  { "file-prefix",     required_argument,   0,   'b' },
  { "output",          required_argument,   0,   'o' },
  { "graph",           optional_argument,   0,   'g' },
  { "html",            optional_argument,   0,   HTML_OPTION },
  { "xml",             optional_argument,   0,   'x' },
  { "file-prefix-map", required_argument,   0,   'M' },

  /* Hidden. */
  { "fixed-output-files", no_argument,       0,  FIXED_OUTPUT_FILES_OPTION },
  { "output-file",        required_argument, 0,  'o' },
  { "trace",              optional_argument, 0,  'T' },

  {0, 0, 0, 0}
};

/* Build a location for the current command line argument. */
स्थिर location
command_ओळ_location (शून्य)
{
  location res;
  /* "<command line>" is used in GCC's messages about -D. */
  boundary_set (&res.start, uniqstr_new ("<command line>"), optind - 1, -1, -1);
  res.end = res.start;
  परत res;
}


/* Handle the command line options for color support.  Do it early, so
   that error messages from getargs be also colored as per the user's
   request.  This is consistent with the way GCC and Clang behave.  */

स्थिर शून्य
getargs_colors (पूर्णांक argc, अक्षर *argv[])
{
  साठी (पूर्णांक i = 1; i < argc; i++)
    {
      स्थिरांक अक्षर *arg = argv[i];
      जर (STRPREFIX_LIT ("--color=", arg))
        {
          स्थिरांक अक्षर *color = arg + म_माप ("--color=");
          जर (STREQ (color, "debug"))
            color_debug = true;
          अन्यथा
            handle_color_option (color);
        }
      अन्यथा जर (STREQ ("--color", arg))
        handle_color_option (रिक्त);
      अन्यथा जर (STRPREFIX_LIT ("--style=", arg))
        {
          स्थिरांक अक्षर *style = arg + म_माप ("--style=");
          handle_style_option (style);
        }
    }
  complain_init_color ();
}


शून्य
getargs (पूर्णांक argc, अक्षर *argv[])
{
  getargs_colors (argc, argv);

  पूर्णांक c;
  पर्यंत ((c = getopt_long (argc, argv, short_options, long_options, रिक्त))
         != -1)
  {
    location loc = command_ओळ_location ();
    निवड (c)
      {
        /* ASCII Sorting for short options (i.e., upper case then
           lower case), and then long-only options.  */

      स्थिती 0:
        /* Certain long options cause getopt_long to return 0.  */
        थांबा;

      स्थिती 'D': /* -DNAME[=(VALUE|"VALUE"|{VALUE})]. */
      स्थिती 'F': /* -FNAME[=(VALUE|"VALUE"|{VALUE})]. */
        {
          अक्षर *name = optarg;
          अक्षर *मूल्य = म_अक्षर (optarg, '=');
          muscle_kind kind = muscle_keyword;
          जर (मूल्य)
            {
              अक्षर *end = मूल्य + म_माप (मूल्य) - 1;
              *मूल्य++ = 0;
              जर (*मूल्य == '{' && *end == '}')
                {
                  kind = muscle_code;
                  ++मूल्य;
                  *end = 0;
                }
              अन्यथा जर (*मूल्य == '"' && *end == '"')
                {
                  kind = muscle_string;
                  ++मूल्य;
                  *end = 0;
                }
            }
          muscle_percent_define_घाला (name, loc,
                                        kind, मूल्य ? मूल्य : "",
                                        c == 'D' ? MUSCLE_PERCENT_DEFINE_D
                                                 : MUSCLE_PERCENT_DEFINE_F);
        }
        थांबा;

      स्थिती 'H':
      स्थिती 'd':
        header_flag = true;
        जर (optarg)
          {
            मुक्त (spec_header_file);
            spec_header_file = xstrdup (optarg);
          }
        थांबा;

      स्थिती 'L':
        language_argmatch (optarg, command_ओळ_prio, loc);
        थांबा;

      स्थिती 'M': // -MOLDPREFIX=NEWPREFIX
        {
          अक्षर *newprefix = म_अक्षर (optarg, '=');
          जर (newprefix)
            {
              *newprefix = '\0';
              add_prefix_map (optarg, newprefix + 1);
            }
          अन्यथा
            {
              complain (&loc, complaint, _("invalid argument for %s: %s"),
                        quote ("--file-prefix-map"), quotearg_n (1, optarg));
            }
        }
        थांबा;

      स्थिती 'S':
        skeleton_arg (optarg, command_ओळ_prio, loc);
        थांबा;

      स्थिती 'T':
        FLAGS_ARGMATCH (trace, optarg, trace_all);
        थांबा;

      स्थिती 'V':
        version ();
        निकास (निकास_सफल);

      स्थिती 'f':
        FLAGS_ARGMATCH (feature, optarg, feature_all);
        थांबा;

      स्थिती 'W':
        warnings_argmatch (optarg);
        थांबा;

      स्थिती 'b':
        spec_file_prefix = optarg;
        थांबा;

      स्थिती 'g':
        graph_flag = true;
        जर (optarg)
          {
            मुक्त (spec_graph_file);
            spec_graph_file = xstrdup (optarg);
          }
        थांबा;

      स्थिती 'h':
        usage (निकास_सफल);

      स्थिती 'k':
        token_table_flag = true;
        थांबा;

      स्थिती 'l':
        no_ओळी_flag = true;
        थांबा;

      स्थिती 'o':
        spec_outfile = optarg;
        थांबा;

      स्थिती 'p':
        spec_name_prefix = optarg;
        थांबा;

      स्थिती 'r':
        FLAGS_ARGMATCH (report, optarg, report_all);
        थांबा;

      स्थिती 't':
        muscle_percent_define_घाला ("parse.trace",
                                      loc,
                                      muscle_keyword, "",
                                      MUSCLE_PERCENT_DEFINE_D);
        थांबा;

      स्थिती 'u':
        update_flag = true;
        feature_flag |= feature_syntax_only;
        थांबा;

      स्थिती 'v':
        report_flag |= report_states;
        थांबा;

      स्थिती 'x':
        xml_flag = true;
        जर (optarg)
          {
            मुक्त (spec_xml_file);
            spec_xml_file = xstrdup (optarg);
          }
        थांबा;

      स्थिती 'y':
        warning_argmatch ("yacc", 0, 0);
        set_yacc (loc);
        थांबा;

      स्थिती COLOR_OPTION:
        /* Handled in getargs_colors. */
        थांबा;

      स्थिती HTML_OPTION:
        html_flag = true;
        xml_flag = true;
        जर (optarg)
          {
            मुक्त (spec_html_file);
            spec_html_file = xstrdup (optarg);
          }
        थांबा;

      स्थिती FIXED_OUTPUT_FILES_OPTION:
        complain (&loc, Wdeprecated,
                  _("deprecated option: %s, use %s"),
                  quote ("--fixed-output-files"), quote_n (1, "-o y.tab.c"));
        spec_outfile = "y.tab.c";
        थांबा;

      स्थिती LOCATIONS_OPTION:
        muscle_percent_define_ensure ("locations", loc, true);
        थांबा;

      स्थिती PRINT_LOCALEDIR_OPTION:
        म_लिखो ("%s\n", LOCALEDIR);
        निकास (निकास_सफल);

      स्थिती PRINT_DATADIR_OPTION:
        म_लिखो ("%s\n", pkgdatadir ());
        निकास (निकास_सफल);

      स्थिती REPORT_FILE_OPTION:
        मुक्त (spec_verbose_file);
        spec_verbose_file = xstrdup (optarg);
        थांबा;

      स्थिती STYLE_OPTION:
        /* Handled in getargs_colors. */
        थांबा;

      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }
  }

  जर (argc - optind != 1)
    {
      जर (argc - optind < 1)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  grammar_file = uniqstr_new (argv[optind]);
  MUSCLE_घाला_C_STRING ("file_name", grammar_file);
}

शून्य
tr (अक्षर *s, अक्षर from, अक्षर to)
{
  साठी (; *s; ++s)
    जर (*s == from)
      *s = to;
}
