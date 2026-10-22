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

#ضمن <config.h>
#ضمن "getargs.h"

#ضمن "system.h"

#ضمن <argmatch.h>
#ضمن <c-strcase.h>
#ضمن <configmake.h>
#ضمن <error.h>
#ضمن <getopt.h>
#ضمن <progname.h>
#ضمن <quote.h>
#ضمن <textstyle.h>

#ضمن "complain.h"
#ضمن "files.h"
#ضمن "muscle-tab.h"
#ضمن "output.h"
#ضمن "uniqstr.h"

bool سرو_flag = false;
bool گراف_flag = false;
bool html_flag = false;
bool xml_flag = false;
bool no_سٽون_flag = false;
bool token_جدول_flag = false;
location yacc_loc = خالي_جايون_LOCATION_INIT;
bool update_flag = false; /* for -u */
bool color_debug = false;

bool nondeterministic_parser = false;
bool glr_parser = false;

صحيح feature_flag = feature_caret;
صحيح report_flag = report_none;
صحيح trace_flag = trace_none;

ساكن بنية bison_language ثابت valid_languages[] = {
  /* lang,  skeleton,       ext,     hdr,     add_tab */
  { "c",    "c-skel.m4",    ".c",    ".h",    true },
  { "c++",  "c++-skel.m4",  ".cc",   ".hh",   true },
  { "d",    "d-skel.m4",    ".d",    ".d",    false },
  { "java", "java-skel.m4", ".java", ".java", false },
  { "", "", "", "", false }
};

صحيح skeleton_prio = default_prio;
ثابت حرف *skeleton = عدم;
صحيح language_prio = default_prio;
بنية bison_language ثابت *language = &valid_languages[0];

تعريفنوع صحيح* (xargmatch_fn) (ثابت حرف *context, ثابت حرف *arg);

فراغ
سيٽ_yacc (location loc)
{
  yacc_loc = loc;
  إذا (getenv ("POSIXLY_CORRECT"))
    muscle_percent_define_وجهو ("posix",
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
ساكن فراغ
flag_argmatch (ثابت حرف *opt, xargmatch_fn xargmatch,
               صحيح all, صحيح *flags, حرف *arg, size_t no)
{
  صحيح قدر = *xargmatch (opt, arg + no);

  /* -rnone == -rno-all, and -rno-none == -rall.  */
  إذا (!قدر)
    {
      قدر = all;
      no = !no;
    }

  إذا (no)
    *flags &= ~قدر;
  وإلا
    *flags |= قدر;
}

تعريفنوع فراغ (usage_fn) (FILE *ٻاهريون);

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
ساكن فراغ
flags_argmatch (ثابت حرف *opt,
                xargmatch_fn xargmatch,
                usage_fn usage,
                صحيح all, صحيح *flags, حرف *دليلون)
{
  إذا (!دليلون)
    *flags |= all;
  وإلا إذا (STREQ (دليلون, "help"))
    {
      usage (stdout);
      ٻاهر_نڪرو (EXIT_SUCCESS);
    }
  وإلا
    لكل (دليلون = strtok (دليلون, ","); دليلون; دليلون = strtok (عدم, ","))
      {
        size_t no = STRPREFIX_LIT ("no-", دليلون) ? 3 : 0;
        flag_argmatch (opt, xargmatch,
                       all, flags, دليلون, no);
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
#وصف_ڪريو FLAGS_ARGMATCH(FlagName, دليلون_2, All)                             \
  flags_argmatch ("--" #FlagName,                                       \
                  (xargmatch_fn*) argmatch_## FlagName ## قدر_2,       \
                  argmatch_ ## FlagName ## _usage,                      \
                  All, &FlagName ## _flag, دليلون_2)

/*---------------------.
| --color's handling.  |
`---------------------*/

تعداد color
  {
    color_always,
    color_never,
    color_auto
  };

ARGMATCH_DEFINE_گروهه (color, تعداد color)

ساكن ثابت argmatch_color_doc argmatch_color_docs[] =
{
  { "always",     N_("colorize the output") },
  { "never",      N_("don't colorize the output") },
  { "auto",       N_("colorize if the output device is a tty") },
  { عدم, عدم },
};

ساكن ثابت argmatch_color_arg argmatch_color_دليلون[] =
{
  { "always",   color_always },
  { "yes",      color_always },
  { "never",    color_never },
  { "no",       color_never },
  { "auto",     color_auto },
  { "tty",      color_auto },
  { عدم, color_always },
};

ثابت argmatch_color_گروهه_type argmatch_color_گروهه =
{
  argmatch_color_دليلون,
  argmatch_color_docs,
  /* TRANSLATORS: Use the same translation for WHEN as in the
     --color=WHEN help message.  */
  N_("WHEN can be one of the following:"),
  عدم
};


/*----------------------.
| --report's handling.  |
`----------------------*/

ARGMATCH_DEFINE_گروهه (report, تعداد report)

ساكن ثابت argmatch_report_doc argmatch_report_docs[] =
{
  { "states",          N_("describe the states") },
  { "itemsets",        N_("complete the core item sets with their closure") },
  { "lookaheads",      N_("explicitly associate lookahead tokens to items") },
  { "solved",          N_("describe shift/reduce conflicts solving") },
  { "counterexamples", N_("generate conflict counterexamples") },
  { "all",             N_("include all the above information") },
  { "none",            N_("disable the report") },
  { عدم, عدم },
};

ساكن ثابت argmatch_report_arg argmatch_report_دليلون[] =
{
  { "none",            report_none },
  { "states",          report_حالتون },
  { "itemsets",        report_حالتون | report_itemsets },
  { "lookaheads",      report_حالتون | report_lookaheads },
  { "solved",          report_حالتون | report_solved_conflicts },
  { "counterexamples", report_cex },
  { "cex",             report_cex },
  { "all",             report_all },
  { عدم, report_none },
};

ثابت argmatch_report_گروهه_type argmatch_report_گروهه =
{
  argmatch_report_دليلون,
  argmatch_report_docs,
  /* TRANSLATORS: Use the same translation for THINGS as in the
     --report=THINGS help message.  */
  N_("THINGS is a list of comma separated words that can include:"),
  عدم
};

/*---------------------.
| --trace's handling.  |
`---------------------*/

ARGMATCH_DEFINE_گروهه (trace, تعداد trace)

ساكن ثابت argmatch_trace_doc argmatch_trace_docs[] =
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
  { عدم, عدم},
};

ساكن ثابت argmatch_trace_arg argmatch_trace_دليلون[] =
{
  { "none",      trace_none },
  { "locations", trace_locations },
  { "scan",      trace_جاچ },
  { "parse",     trace_parse },
  { "automaton", trace_automaton },
  { "bitsets",   trace_bitsets },
  { "closure",   trace_گهيرو },
  { "grammar",   trace_grammar },
  { "resource",  trace_resource },
  { "sets",      trace_sets },
  { "muscles",   trace_muscles },
  { "tools",     trace_tools },
  { "m4-early",  trace_m4_early },
  { "m4",        trace_m4 },
  { "skeleton",  trace_skeleton },
  { "time",      trace_وقت },
  { "ielr",      trace_ielr },
  { "cex",       trace_cex },
  { "all",       trace_all },
  { عدم,        trace_none},
};

ثابت argmatch_trace_گروهه_type argmatch_trace_گروهه =
{
  argmatch_trace_دليلون,
  argmatch_trace_docs,
  N_("TRACES is a list of comma separated words that can include:"),
  عدم
};

/*-----------------------.
| --feature's handling.  |
`-----------------------*/

ARGMATCH_DEFINE_گروهه (feature, تعداد feature)

ساكن ثابت argmatch_feature_doc argmatch_feature_docs[] =
{
  { "caret",       N_("show errors with carets") },
  { "fixit",       N_("show machine-readable fixes") },
  { "syntax-only", N_("do not generate any file") },
  { "all",         N_("all of the above") },
  { "none",        N_("disable all of the above") },
  { عدم, عدم }
};

ساكن ثابت argmatch_feature_arg argmatch_feature_دليلون[] =
{
  { "none",                          feature_none },
  { "caret",                         feature_caret },
  { "diagnostics-show-caret",        feature_caret },
  { "fixit",                         feature_fixit },
  { "diagnostics-parseable-fixits",  feature_fixit },
  { "syntax-only",                   feature_syntax_only },
  { "all",                           feature_all },
  { عدم, feature_none}
};

ثابت argmatch_feature_گروهه_type argmatch_feature_گروهه =
{
  argmatch_feature_دليلون,
  argmatch_feature_docs,
  /* TRANSLATORS: Use the same translation for FEATURES as in the
     --feature=FEATURES help message.  */
  N_("FEATURES is a list of comma separated words that can include:"),
  عدم
};

/*-------------------------------------------.
| Display the help message and exit STATUS.  |
`-------------------------------------------*/

 _Noreturn
ساكن فراغ usage (صحيح);

ساكن فراغ
usage (صحيح حالت)
{
  إذا (حالت != 0)
    fprintf (stderr, _("Try '%s --help' for more information.\n"),
             پروگرام_name);
  وإلا
    {
      /* For ../build-aux/cross-options.pl to work, use the format:
                ^  -S, --long[=ARGS] (whitespace)
         A --long option is required.
         Otherwise, add exceptions to ../build-aux/cross-options.pl.  */

      لکو (_("Usage: %s [OPTION]... FILE\n"), پروگرام_name);
      fputs (_("\
Generate a deterministic LR or generalized LR (GLR) parser employing\n\
LALR(1), IELR(1), or canonical LR(1) parser tables.\n\
\n\
"), stdout);

      fputs (_("\
Mandatory arguments منزل ڊگهو options are mandatory لكل ننڍو options too.\n\
"), stdout);
      fputs (_("\
The same is true لكل optional arguments.\n\
"), stdout);
      putc ('\n', stdout);

      fputs (_("\
Operation Modes:\n\
  -h, --help                 display this help and ٻاهر_نڪرو\n\
  -V, --نسخو              output نسخو information and ٻاهر_نڪرو\n\
      --print-localedir      output directory containing locale-dependent معلومات\n\
                             and ٻاهر_نڪرو\n\
      --print-datadir        output directory containing skeletons and XSLT\n\
                             and ٻاهر_نڪرو\n\
  -u, --update               apply fixes منزل the ذريعو grammar file and ٻاهر_نڪرو\n\
  -f, --feature[=FEATURES]   activate miscellaneous features\n\
\n\
"), stdout);

      argmatch_feature_usage (stdout);
      putc ('\n', stdout);

      fputs (_("\
Diagnostics:\n\
  -W, --warnings[=CATEGORY]  report the warnings falling اندريون CATEGORY\n\
      --color[=WHEN]         whether منزل colorize the diagnostics\n\
      --style=FILE           specify the CSS FILE لكل colorizer diagnostics\n\
\n\
"), stdout);

      warning_usage (stdout);
      putc ('\n', stdout);

      argmatch_color_usage (stdout);
      putc ('\n', stdout);

      fputs (_("\
Tuning the Parser:\n\
  -L, --language=LANGUAGE          specify the output programming language\n\
  -S, --skeleton=FILE              specify the skeleton منزل use\n\
  -t, --debug                      instrument the parser لكل tracing\n\
                                   same as '-Dparse.trace'\n\
      --locations                  enable location support\n\
  -D, --define=NAME[=قدر_3]        similar منزل '%define NAME VALUE'\n\
  -F, --force-define=NAME[=قدر_3]  override '%define NAME VALUE'\n\
  -p, --name-اڳياڙي=اڳياڙي_2         prepend اڳياڙي_2 منزل the external symbols\n\
                                   deprecated by '-Dapi.prefix={PREFIX}'\n\
  -l, --no-سٽون                   don't generate '#سٽ' directives\n\
  -k, --token-جدول                include a جدول جو token نالا\n\
  -y, --yacc                       emulate POSIX Yacc\n\
"), stdout);
      putc ('\n', stdout);

      fputs (_("\
Output Files:\n\
  -H, --سرو=[FILE]           also produce a سرو file\n\
  -d                            likewise but cannot specify FILE (لكل POSIX Yacc)\n\
  -r, --report=THINGS           also produce details on the automaton\n\
      --report-file=FILE        اكتب report منزل FILE\n\
  -v, --verbose                 same as '--report=state'\n\
  -b, --file-اڳياڙي=اڳياڙي_2      specify a اڳياڙي_2 لكل output files\n\
  -o, --output=FILE             leave output منزل FILE\n\
  -g, --گراف[=FILE]            also output a گراف جو the automaton\n\
      --html[=FILE]             also output an HTML report جو the automaton\n\
  -x, --xml[=FILE]              also output an XML report جو the automaton\n\
  -M, --file-اڳياڙي-map=OLD=NEW replace اڳياڙي OLD with NEW when writing file paths\n\
                                اندريون output files\n\
"), stdout);
      putc ('\n', stdout);

      argmatch_report_usage (stdout);
      putc ('\n', stdout);

      لکو (_("Report bugs to <%s>.\n"), PACKAGE_BUGREPORT);
      لکو (_("%s home page: <%s>.\n"), PACKAGE_NAME, PACKAGE_URL);
      fputs (_("General help using GNU software: "
               "<https://www.gnu.org/gethelp/>.\n"),
             stdout);

#إذا (defined __GLIBC__ && __GLIBC__ >= 2) && !defined __UCLIBC__
      /* Don't output this redundant message for English locales.
         Note we still output for 'C' so that it gets included in the
         man page.  */
      ثابت حرف *lc_messages = setlocale (LC_MESSAGES, عدم);
      إذا (lc_messages && !STREQ (lc_messages, "en_"))
        /* TRANSLATORS: Replace LANG_CODE in this URL with your language code to
           form one of the URLs at https://translationproject.org/team/.
           Otherwise, replace the entire URL with your translation team's
           email address.  */
        fputs (_("Report translation bugs to "
                 "<https://translationproject.org/team/>.\n"), stdout);
#جيڪڏهن_پورو
      fputs (_("For complete documentation, run: info bison.\n"), stdout);
    }

  ٻاهر_نڪرو (حالت);
}


/*------------------------------.
| Display the version message.  |
`------------------------------*/

ساكن فراغ
نسخو (فراغ)
{
  /* Some efforts were made to ease the translators' task, please
     continue.  */
  لکو ("bison (GNU Bison) %s", نسخو_2);
  putc ('\n', stdout);
  fputs (_("Written by Robert Corbett and Richard Stallman.\n"), stdout);
  putc ('\n', stdout);

  fprintf (stdout,
           _("Copyright (C) %d Free Software Foundation, Inc.\n"),
           PACKAGE_COPYRIGHT_YEAR);

  fputs (_("\
This is حرر software; see the ذريعو لكل copying conditions.  There is NO\n\
warranty; not even لكل MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
"),
         stdout);
}


/*-------------------------------------.
| --skeleton and --language handling.  |
`--------------------------------------*/

فراغ
skeleton_arg (حرف ثابت *arg, صحيح prio, location loc)
{
  إذا (prio < skeleton_prio)
    {
      skeleton_prio = prio;
      skeleton = arg;
    }
  وإلا إذا (prio == skeleton_prio)
    complain (&loc, complaint,
              _("multiple skeleton declarations are invalid"));
}

فراغ
language_argmatch (حرف ثابت *arg, صحيح prio, location loc)
{
  حرف ثابت *نياپو = عدم;

  إذا (prio < language_prio)
    {
      لكل (صحيح i = 0; valid_languages[i].language[0]; ++i)
        إذا (c_strcasecmp (arg, valid_languages[i].language) == 0)
          {
            language_prio = prio;
            language = &valid_languages[i];
            أرجع;
          }
      نياپو = _("%s: invalid language");
    }
  وإلا إذا (language_prio == prio)
    نياپو = _("multiple language declarations are invalid");

  إذا (نياپو)
    complain (&loc, complaint, نياپو, quotearg_colon (arg));
}

/*----------------------.
| Process the options.  |
`----------------------*/

/* Shorts options.
   Should be computed from long_options.  */
ساكن حرف ثابت short_options[] =
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
تعداد
{
  COLOR_OPTION = CHAR_MAX + 1,
  FIXED_OUTPUT_FILES_OPTION,
  HTML_OPTION,
  LOCATIONS_OPTION,
  PRINT_DATADIR_OPTION,
  PRINT_LOCALEDIR_OPTION,
  REPORT_FILE_OPTION,
  STYLE_OPTION
};

/* In the same order as in usage(), and in the documentation.  */
ساكن بنية option ثابت long_options[] =
{
  /* Operation modes. */
  { "help",            no_دليل,       0,   'h' },
  { "version",         no_دليل,       0,   'V' },
  { "print-localedir", no_دليل,       0,   PRINT_LOCALEDIR_OPTION },
  { "print-datadir",   no_دليل,       0,   PRINT_DATADIR_OPTION   },
  { "update",          no_دليل,       0,   'u' },
  { "feature",         optional_دليل, 0,   'f' },

  /* Diagnostics.  */
  { "warnings",        optional_دليل,  0, 'W' },
  { "color",           optional_دليل,  0,  COLOR_OPTION },
  { "style",           optional_دليل,  0,  STYLE_OPTION },

  /* Tuning the Parser. */
  { "language",       required_دليل,   0, 'L' },
  { "skeleton",       required_دليل,   0, 'S' },
  { "debug",          no_دليل,         0, 't' },
  { "locations",      no_دليل,         0, LOCATIONS_OPTION },
  { "define",         required_دليل,   0, 'D' },
  { "force-define",   required_دليل,   0, 'F' },
  { "name-prefix",    required_دليل,   0, 'p' },
  { "no-lines",       no_دليل,         0, 'l' },
  { "token-table",    no_دليل,         0, 'k' },
  { "yacc",           no_دليل,         0, 'y' },

  /* Output Files. */
  { "header",          optional_دليل,   0,   'H' },
  { "defines",         optional_دليل,   0,   'd' },
  { "report",          required_دليل,   0,   'r' },
  { "report-file",     required_دليل,   0,   REPORT_FILE_OPTION },
  { "verbose",         no_دليل,         0,   'v' },
  { "file-prefix",     required_دليل,   0,   'b' },
  { "output",          required_دليل,   0,   'o' },
  { "graph",           optional_دليل,   0,   'g' },
  { "html",            optional_دليل,   0,   HTML_OPTION },
  { "xml",             optional_دليل,   0,   'x' },
  { "file-prefix-map", required_دليل,   0,   'M' },

  /* Hidden. */
  { "fixed-output-files", no_دليل,       0,  FIXED_OUTPUT_FILES_OPTION },
  { "output-file",        required_دليل, 0,  'o' },
  { "trace",              optional_دليل, 0,  'T' },

  {0, 0, 0, 0}
};

/* Build a location for the current command line argument. */
ساكن location
command_سٽ_location (فراغ)
{
  location res;
  /* "<command line>" is used in GCC's messages about -D. */
  boundary_سيٽ (&res.شروعات, uniqstr_new ("<command line>"), optind - 1, -1, -1);
  res.پڇاڙي = res.شروعات;
  أرجع res;
}


/* Handle the command line options for color support.  Do it early, so
   that error messages from getargs be also colored as per the user's
   request.  This is consistent with the way GCC and Clang behave.  */

ساكن فراغ
getargs_colors (صحيح argc, حرف *argv[])
{
  لكل (صحيح i = 1; i < argc; i++)
    {
      ثابت حرف *arg = argv[i];
      إذا (STRPREFIX_LIT ("--color=", arg))
        {
          ثابت حرف *color = arg + لڙي_ڊيگهه ("--color=");
          إذا (STREQ (color, "debug"))
            color_debug = true;
          وإلا
            handle_color_option (color);
        }
      وإلا إذا (STREQ ("--color", arg))
        handle_color_option (عدم);
      وإلا إذا (STRPREFIX_LIT ("--style=", arg))
        {
          ثابت حرف *style = arg + لڙي_ڊيگهه ("--style=");
          handle_style_option (style);
        }
    }
  complain_init_color ();
}


فراغ
getargs (صحيح argc, حرف *argv[])
{
  getargs_colors (argc, argv);

  صحيح c;
  طالما ((c = getopt_long (argc, argv, short_options, long_options, عدم))
         != -1)
  {
    location loc = command_سٽ_location ();
    بدل (c)
      {
        /* ASCII Sorting for short options (i.e., upper case then
           lower case), and then long-only options.  */

      حالة 0:
        /* Certain long options cause getopt_long to return 0.  */
        اكسر;

      حالة 'D': /* -DNAME[=(VALUE|"VALUE"|{VALUE})]. */
      حالة 'F': /* -FNAME[=(VALUE|"VALUE"|{VALUE})]. */
        {
          حرف *name = optarg;
          حرف *قدر = strchr (optarg, '=');
          muscle_قسم قسم = muscle_keyword;
          إذا (قدر)
            {
              حرف *پڇاڙي = قدر + لڙي_ڊيگهه (قدر) - 1;
              *قدر++ = 0;
              إذا (*قدر == '{' && *پڇاڙي == '}')
                {
                  قسم = muscle_ڪوڊ;
                  ++قدر;
                  *پڇاڙي = 0;
                }
              وإلا إذا (*قدر == '"' && *پڇاڙي == '"')
                {
                  قسم = muscle_string;
                  ++قدر;
                  *پڇاڙي = 0;
                }
            }
          muscle_percent_define_وجهو (name, loc,
                                        قسم, قدر ? قدر : "",
                                        c == 'D' ? MUSCLE_PERCENT_DEFINE_D
                                                 : MUSCLE_PERCENT_DEFINE_F);
        }
        اكسر;

      حالة 'H':
      حالة 'd':
        سرو_flag = true;
        إذا (optarg)
          {
            حرر (spec_سرو_file);
            spec_سرو_file = xstrdup (optarg);
          }
        اكسر;

      حالة 'L':
        language_argmatch (optarg, command_سٽ_prio, loc);
        اكسر;

      حالة 'M': // -MOLDPREFIX=NEWPREFIX
        {
          حرف *newprefix = strchr (optarg, '=');
          إذا (newprefix)
            {
              *newprefix = '\0';
              جوڙيو_اڳياڙي_map (optarg, newprefix + 1);
            }
          وإلا
            {
              complain (&loc, complaint, _("invalid argument for %s: %s"),
                        quote ("--file-prefix-map"), quotearg_n (1, optarg));
            }
        }
        اكسر;

      حالة 'S':
        skeleton_arg (optarg, command_سٽ_prio, loc);
        اكسر;

      حالة 'T':
        FLAGS_ARGMATCH (trace, optarg, trace_all);
        اكسر;

      حالة 'V':
        نسخو ();
        ٻاهر_نڪرو (EXIT_SUCCESS);

      حالة 'f':
        FLAGS_ARGMATCH (feature, optarg, feature_all);
        اكسر;

      حالة 'W':
        warnings_argmatch (optarg);
        اكسر;

      حالة 'b':
        spec_file_اڳياڙي = optarg;
        اكسر;

      حالة 'g':
        گراف_flag = true;
        إذا (optarg)
          {
            حرر (spec_گراف_file);
            spec_گراف_file = xstrdup (optarg);
          }
        اكسر;

      حالة 'h':
        usage (EXIT_SUCCESS);

      حالة 'k':
        token_جدول_flag = true;
        اكسر;

      حالة 'l':
        no_سٽون_flag = true;
        اكسر;

      حالة 'o':
        spec_outfile = optarg;
        اكسر;

      حالة 'p':
        spec_name_اڳياڙي = optarg;
        اكسر;

      حالة 'r':
        FLAGS_ARGMATCH (report, optarg, report_all);
        اكسر;

      حالة 't':
        muscle_percent_define_وجهو ("parse.trace",
                                      loc,
                                      muscle_keyword, "",
                                      MUSCLE_PERCENT_DEFINE_D);
        اكسر;

      حالة 'u':
        update_flag = true;
        feature_flag |= feature_syntax_only;
        اكسر;

      حالة 'v':
        report_flag |= report_حالتون;
        اكسر;

      حالة 'x':
        xml_flag = true;
        إذا (optarg)
          {
            حرر (spec_xml_file);
            spec_xml_file = xstrdup (optarg);
          }
        اكسر;

      حالة 'y':
        warning_argmatch ("yacc", 0, 0);
        سيٽ_yacc (loc);
        اكسر;

      حالة COLOR_OPTION:
        /* Handled in getargs_colors. */
        اكسر;

      حالة HTML_OPTION:
        html_flag = true;
        xml_flag = true;
        إذا (optarg)
          {
            حرر (spec_html_file);
            spec_html_file = xstrdup (optarg);
          }
        اكسر;

      حالة FIXED_OUTPUT_FILES_OPTION:
        complain (&loc, Wdeprecated,
                  _("deprecated option: %s, use %s"),
                  quote ("--fixed-output-files"), quote_n (1, "-o y.tab.c"));
        spec_outfile = "y.tab.c";
        اكسر;

      حالة LOCATIONS_OPTION:
        muscle_percent_define_ensure ("locations", loc, true);
        اكسر;

      حالة PRINT_LOCALEDIR_OPTION:
        لکو ("%s\n", LOCALEDIR);
        ٻاهر_نڪرو (EXIT_SUCCESS);

      حالة PRINT_DATADIR_OPTION:
        لکو ("%s\n", pkgdatadir ());
        ٻاهر_نڪرو (EXIT_SUCCESS);

      حالة REPORT_FILE_OPTION:
        حرر (spec_verbose_file);
        spec_verbose_file = xstrdup (optarg);
        اكسر;

      حالة STYLE_OPTION:
        /* Handled in getargs_colors. */
        اكسر;

      افتراضي:
        usage (EXIT_FAILURE);
      }
  }

  إذا (argc - optind != 1)
    {
      إذا (argc - optind < 1)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  grammar_file = uniqstr_new (argv[optind]);
  MUSCLE_وجهو_C_STRING ("file_name", grammar_file);
}

فراغ
tr (حرف *s, حرف from, حرف منزل)
{
  لكل (; *s; ++s)
    إذا (*s == from)
      *s = منزل;
}
