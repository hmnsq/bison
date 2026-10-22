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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "getargs.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <argmatch.h>
#ଅନ୍ତର୍ଭାଵ <c-strcase.h>
#ଅନ୍ତର୍ଭାଵ <configmake.h>
#ଅନ୍ତର୍ଭାଵ <error.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <progname.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>
#ଅନ୍ତର୍ଭାଵ <textstyle.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "output.h"
#ଅନ୍ତର୍ଭାଵ "uniqstr.h"

bool header_flag = false;
bool graph_flag = false;
bool html_flag = false;
bool xml_flag = false;
bool no_ଧାଡ଼ିଗୁଡ଼ିକ_flag = false;
bool token_table_flag = false;
location yacc_loc = EMPTY_LOCATION_INIT;
bool update_flag = false; /* for -u */
bool color_debug = false;

bool nondeterministic_parser = false;
bool glr_parser = false;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_flag = feature_caret;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା report_flag = report_none;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା trace_flag = trace_none;

ସ୍ଥିର ସଂରଚନା bison_language ଅଚଳ valid_languages[] = {
  /* lang,  skeleton,       ext,     hdr,     add_tab */
  { "c",    "c-skel.m4",    ".c",    ".h",    true },
  { "c++",  "c++-skel.m4",  ".cc",   ".hh",   true },
  { "d",    "d-skel.m4",    ".d",    ".d",    false },
  { "java", "java-skel.m4", ".java", ".java", false },
  { "", "", "", "", false }
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା skeleton_prio = default_prio;
ଅଚଳ ଅକ୍ଷର *skeleton = ଖାଲି;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା language_prio = default_prio;
ସଂରଚନା bison_language ଅଚଳ *language = &valid_languages[0];

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା* (xargmatch_fn) (ଅଚଳ ଅକ୍ଷର *context, ଅଚଳ ଅକ୍ଷର *arg);

ଶୂନ୍ୟ
set_yacc (location loc)
{
  yacc_loc = loc;
  ଯଦି (getenv ("POSIXLY_CORRECT"))
    muscle_percent_define_ଭର୍ତ୍ତି ("posix",
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
ସ୍ଥିର ଶୂନ୍ୟ
flag_argmatch (ଅଚଳ ଅକ୍ଷର *opt, xargmatch_fn xargmatch,
               ପୂର୍ଣ୍ଣସଂଖ୍ୟା all, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags, ଅକ୍ଷର *arg, size_t no)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = *xargmatch (opt, arg + no);

  /* -rnone == -rno-all, and -rno-none == -rall.  */
  ଯଦି (!ମୂଲ୍ୟ)
    {
      ମୂଲ୍ୟ = all;
      no = !no;
    }

  ଯଦି (no)
    *flags &= ~ମୂଲ୍ୟ;
  ଅନ୍ୟଥା
    *flags |= ମୂଲ୍ୟ;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (usage_fn) (FILE *out);

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
ସ୍ଥିର ଶୂନ୍ୟ
flags_argmatch (ଅଚଳ ଅକ୍ଷର *opt,
                xargmatch_fn xargmatch,
                usage_fn usage,
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା all, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags, ଅକ୍ଷର *args)
{
  ଯଦି (!args)
    *flags |= all;
  ଅନ୍ୟଥା ଯଦି (STREQ (args, "help"))
    {
      usage (stdout);
      exit (EXIT_SUCCESS);
    }
  ଅନ୍ୟଥା
    ପାଇଁ (args = strtok (args, ","); args; args = strtok (ଖାଲି, ","))
      {
        size_t no = STRPREFIX_LIT ("no-", args) ? 3 : 0;
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
                  (xargmatch_fn*) argmatch_## FlagName ## ମୂଲ୍ୟ_2,       \
                  argmatch_ ## FlagName ## _usage,                      \
                  All, &FlagName ## _flag, Args)

/*---------------------.
| --color's handling.  |
`---------------------*/

ଗଣନା color
  {
    color_always,
    color_never,
    color_auto
  };

ARGMATCH_DEFINE_GROUP (color, ଗଣନା color)

ସ୍ଥିର ଅଚଳ argmatch_color_doc argmatch_color_docs[] =
{
  { "always",     N_("colorize the output") },
  { "never",      N_("don't colorize the output") },
  { "auto",       N_("colorize if the output device is a tty") },
  { ଖାଲି, ଖାଲି },
};

ସ୍ଥିର ଅଚଳ argmatch_color_arg argmatch_color_args[] =
{
  { "always",   color_always },
  { "yes",      color_always },
  { "never",    color_never },
  { "no",       color_never },
  { "auto",     color_auto },
  { "tty",      color_auto },
  { ଖାଲି, color_always },
};

ଅଚଳ argmatch_color_group_type argmatch_color_group =
{
  argmatch_color_args,
  argmatch_color_docs,
  /* TRANSLATORS: Use the same translation for WHEN as in the
     --color=WHEN help message.  */
  N_("WHEN can be one of the following:"),
  ଖାଲି
};


/*----------------------.
| --report's handling.  |
`----------------------*/

ARGMATCH_DEFINE_GROUP (report, ଗଣନା report)

ସ୍ଥିର ଅଚଳ argmatch_report_doc argmatch_report_docs[] =
{
  { "states",          N_("describe the states") },
  { "itemsets",        N_("complete the core item sets with their closure") },
  { "lookaheads",      N_("explicitly associate lookahead tokens to items") },
  { "solved",          N_("describe shift/reduce conflicts solving") },
  { "counterexamples", N_("generate conflict counterexamples") },
  { "all",             N_("include all the above information") },
  { "none",            N_("disable the report") },
  { ଖାଲି, ଖାଲି },
};

ସ୍ଥିର ଅଚଳ argmatch_report_arg argmatch_report_args[] =
{
  { "none",            report_none },
  { "states",          report_states },
  { "itemsets",        report_states | report_itemsets },
  { "lookaheads",      report_states | report_lookaheads },
  { "solved",          report_states | report_solved_conflicts },
  { "counterexamples", report_cex },
  { "cex",             report_cex },
  { "all",             report_all },
  { ଖାଲି, report_none },
};

ଅଚଳ argmatch_report_group_type argmatch_report_group =
{
  argmatch_report_args,
  argmatch_report_docs,
  /* TRANSLATORS: Use the same translation for THINGS as in the
     --report=THINGS help message.  */
  N_("THINGS is a list of comma separated words that can include:"),
  ଖାଲି
};

/*---------------------.
| --trace's handling.  |
`---------------------*/

ARGMATCH_DEFINE_GROUP (trace, ଗଣନା trace)

ସ୍ଥିର ଅଚଳ argmatch_trace_doc argmatch_trace_docs[] =
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
  { ଖାଲି, ଖାଲି},
};

ସ୍ଥିର ଅଚଳ argmatch_trace_arg argmatch_trace_args[] =
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
  { ଖାଲି,        trace_none},
};

ଅଚଳ argmatch_trace_group_type argmatch_trace_group =
{
  argmatch_trace_args,
  argmatch_trace_docs,
  N_("TRACES is a list of comma separated words that can include:"),
  ଖାଲି
};

/*-----------------------.
| --feature's handling.  |
`-----------------------*/

ARGMATCH_DEFINE_GROUP (feature, ଗଣନା feature)

ସ୍ଥିର ଅଚଳ argmatch_feature_doc argmatch_feature_docs[] =
{
  { "caret",       N_("show errors with carets") },
  { "fixit",       N_("show machine-readable fixes") },
  { "syntax-only", N_("do not generate any file") },
  { "all",         N_("all of the above") },
  { "none",        N_("disable all of the above") },
  { ଖାଲି, ଖାଲି }
};

ସ୍ଥିର ଅଚଳ argmatch_feature_arg argmatch_feature_args[] =
{
  { "none",                          feature_none },
  { "caret",                         feature_caret },
  { "diagnostics-show-caret",        feature_caret },
  { "fixit",                         feature_fixit },
  { "diagnostics-parseable-fixits",  feature_fixit },
  { "syntax-only",                   feature_syntax_only },
  { "all",                           feature_all },
  { ଖାଲି, feature_none}
};

ଅଚଳ argmatch_feature_group_type argmatch_feature_group =
{
  argmatch_feature_args,
  argmatch_feature_docs,
  /* TRANSLATORS: Use the same translation for FEATURES as in the
     --feature=FEATURES help message.  */
  N_("FEATURES is a list of comma separated words that can include:"),
  ଖାଲି
};

/*-------------------------------------------.
| Display the help message and exit STATUS.  |
`-------------------------------------------*/

 _Noreturn
ସ୍ଥିର ଶୂନ୍ୟ usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

ସ୍ଥିର ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != 0)
    fprintf (stderr, _("Try '%s --help' for more information.\n"),
             program_name);
  ଅନ୍ୟଥା
    {
      /* For ../build-aux/cross-options.pl to work, use the format:
                ^  -S, --long[=ARGS] (whitespace)
         A --long option is required.
         Otherwise, add exceptions to ../build-aux/cross-options.pl.  */

      printf (_("Usage: %s [OPTION]... FILE\n"), program_name);
      fputs (_("\
Generate a deterministic LR or generalized LR (GLR) parser employing\n\
LALR(1), IELR(1), or canonical LR(1) parser tables.\n\
\n\
"), stdout);

      fputs (_("\
Mandatory arguments to long options are mandatory ପାଇଁ short options too.\n\
"), stdout);
      fputs (_("\
The same is true ପାଇଁ optional arguments.\n\
"), stdout);
      putc ('\n', stdout);

      fputs (_("\
Operation Modes:\n\
  -h, --help                 display this help and exit\n\
  -V, --version              output version information and exit\n\
      --print-localedir      output directory containing locale-dependent data\n\
                             and exit\n\
      --print-datadir        output directory containing skeletons and XSLT\n\
                             and exit\n\
  -u, --update               apply fixes to the source grammar file and exit\n\
  -f, --feature[=FEATURES]   activate miscellaneous features\n\
\n\
"), stdout);

      argmatch_feature_usage (stdout);
      putc ('\n', stdout);

      fputs (_("\
Diagnostics:\n\
  -W, --warnings[=CATEGORY]  report the warnings falling in CATEGORY\n\
      --color[=WHEN]         whether to colorize the diagnostics\n\
      --style=FILE           specify the CSS FILE ପାଇଁ colorizer diagnostics\n\
\n\
"), stdout);

      warning_usage (stdout);
      putc ('\n', stdout);

      argmatch_color_usage (stdout);
      putc ('\n', stdout);

      fputs (_("\
Tuning the Parser:\n\
  -L, --language=LANGUAGE          specify the output programming language\n\
  -S, --skeleton=FILE              specify the skeleton to use\n\
  -t, --debug                      instrument the parser ପାଇଁ tracing\n\
                                   same as '-Dparse.trace'\n\
      --locations                  enable location support\n\
  -D, --define=NAME[=ମୂଲ୍ୟ_3]        similar to '%define NAME VALUE'\n\
  -F, --force-define=NAME[=ମୂଲ୍ୟ_3]  override '%define NAME VALUE'\n\
  -p, --name-prefix=PREFIX         prepend PREFIX to the external symbols\n\
                                   deprecated by '-Dapi.prefix={PREFIX}'\n\
  -l, --no-ଧାଡ଼ିଗୁଡ଼ିକ                   don't generate '#line' directives\n\
  -k, --token-table                include a table of token names\n\
  -y, --yacc                       emulate POSIX Yacc\n\
"), stdout);
      putc ('\n', stdout);

      fputs (_("\
Output Files:\n\
  -H, --header=[FILE]           also produce a header file\n\
  -d                            likewise but cannot specify FILE (ପାଇଁ POSIX Yacc)\n\
  -r, --report=THINGS           also produce details on the automaton\n\
      --report-file=FILE        ଲିଖ report to FILE\n\
  -v, --verbose                 same as '--report=state'\n\
  -b, --file-prefix=PREFIX      specify a PREFIX ପାଇଁ output files\n\
  -o, --output=FILE             leave output to FILE\n\
  -g, --graph[=FILE]            also output a graph of the automaton\n\
      --html[=FILE]             also output an HTML report of the automaton\n\
  -x, --xml[=FILE]              also output an XML report of the automaton\n\
  -M, --file-prefix-map=OLD=NEW replace prefix OLD with NEW when writing file paths\n\
                                in output files\n\
"), stdout);
      putc ('\n', stdout);

      argmatch_report_usage (stdout);
      putc ('\n', stdout);

      printf (_("Report bugs to <%s>.\n"), PACKAGE_BUGREPORT);
      printf (_("%s home page: <%s>.\n"), PACKAGE_NAME, PACKAGE_URL);
      fputs (_("General help using GNU software: "
               "<https://www.gnu.org/gethelp/>.\n"),
             stdout);

#ଯଦି (defined __GLIBC__ && __GLIBC__ >= 2) && !defined __UCLIBC__
      /* Don't output this redundant message for English locales.
         Note we still output for 'C' so that it gets included in the
         man page.  */
      ଅଚଳ ଅକ୍ଷର *lc_messages = setlocale (LC_MESSAGES, ଖାଲି);
      ଯଦି (lc_messages && !STREQ (lc_messages, "en_"))
        /* TRANSLATORS: Replace LANG_CODE in this URL with your language code to
           form one of the URLs at https://translationproject.org/team/.
           Otherwise, replace the entire URL with your translation team's
           email address.  */
        fputs (_("Report translation bugs to "
                 "<https://translationproject.org/team/>.\n"), stdout);
#endif
      fputs (_("For complete documentation, run: info bison.\n"), stdout);
    }

  exit (status);
}


/*------------------------------.
| Display the version message.  |
`------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
version (ଶୂନ୍ୟ)
{
  /* Some efforts were made to ease the translators' task, please
     continue.  */
  printf ("bison (GNU Bison) %s", VERSION);
  putc ('\n', stdout);
  fputs (_("Written by Robert Corbett and Richard Stallman.\n"), stdout);
  putc ('\n', stdout);

  fprintf (stdout,
           _("Copyright (C) %d Free Software Foundation, Inc.\n"),
           PACKAGE_COPYRIGHT_YEAR);

  fputs (_("\
This is ମୁକ୍ତ software; see the source ପାଇଁ copying conditions.  There is NO\n\
warranty; not even ପାଇଁ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
"),
         stdout);
}


/*-------------------------------------.
| --skeleton and --language handling.  |
`--------------------------------------*/

ଶୂନ୍ୟ
skeleton_arg (ଅକ୍ଷର ଅଚଳ *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio, location loc)
{
  ଯଦି (prio < skeleton_prio)
    {
      skeleton_prio = prio;
      skeleton = arg;
    }
  ଅନ୍ୟଥା ଯଦି (prio == skeleton_prio)
    complain (&loc, complaint,
              _("multiple skeleton declarations are invalid"));
}

ଶୂନ୍ୟ
language_argmatch (ଅକ୍ଷର ଅଚଳ *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio, location loc)
{
  ଅକ୍ଷର ଅଚଳ *ସନ୍ଦେଶ = ଖାଲି;

  ଯଦି (prio < language_prio)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; valid_languages[i].language[0]; ++i)
        ଯଦି (c_strcasecmp (arg, valid_languages[i].language) == 0)
          {
            language_prio = prio;
            language = &valid_languages[i];
            ଫେରନ୍ତୁ;
          }
      ସନ୍ଦେଶ = _("%s: invalid language");
    }
  ଅନ୍ୟଥା ଯଦି (language_prio == prio)
    ସନ୍ଦେଶ = _("multiple language declarations are invalid");

  ଯଦି (ସନ୍ଦେଶ)
    complain (&loc, complaint, ସନ୍ଦେଶ, quotearg_colon (arg));
}

/*----------------------.
| Process the options.  |
`----------------------*/

/* Shorts options.
   Should be computed from long_options.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ short_options[] =
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
ଗଣନା
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
ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
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
ସ୍ଥିର location
command_ଧାଡ଼ି_location (ଶୂନ୍ୟ)
{
  location res;
  /* "<command line>" is used in GCC's messages about -D. */
  boundary_set (&res.start, uniqstr_new ("<command line>"), optind - 1, -1, -1);
  res.end = res.start;
  ଫେରନ୍ତୁ res;
}


/* Handle the command line options for color support.  Do it early, so
   that error messages from getargs be also colored as per the user's
   request.  This is consistent with the way GCC and Clang behave.  */

ସ୍ଥିର ଶୂନ୍ୟ
getargs_colors (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < argc; i++)
    {
      ଅଚଳ ଅକ୍ଷର *arg = argv[i];
      ଯଦି (STRPREFIX_LIT ("--color=", arg))
        {
          ଅଚଳ ଅକ୍ଷର *color = arg + strlen ("--color=");
          ଯଦି (STREQ (color, "debug"))
            color_debug = true;
          ଅନ୍ୟଥା
            handle_color_option (color);
        }
      ଅନ୍ୟଥା ଯଦି (STREQ ("--color", arg))
        handle_color_option (ଖାଲି);
      ଅନ୍ୟଥା ଯଦି (STRPREFIX_LIT ("--style=", arg))
        {
          ଅଚଳ ଅକ୍ଷର *style = arg + strlen ("--style=");
          handle_style_option (style);
        }
    }
  complain_init_color ();
}


ଶୂନ୍ୟ
getargs (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
  getargs_colors (argc, argv);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ଯେତେବେଳେ ((c = getopt_long (argc, argv, short_options, long_options, ଖାଲି))
         != -1)
  {
    location loc = command_ଧାଡ଼ି_location ();
    ବାଛ (c)
      {
        /* ASCII Sorting for short options (i.e., upper case then
           lower case), and then long-only options.  */

      ସ୍ଥିତି 0:
        /* Certain long options cause getopt_long to return 0.  */
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'D': /* -DNAME[=(VALUE|"VALUE"|{VALUE})]. */
      ସ୍ଥିତି 'F': /* -FNAME[=(VALUE|"VALUE"|{VALUE})]. */
        {
          ଅକ୍ଷର *name = optarg;
          ଅକ୍ଷର *ମୂଲ୍ୟ = strchr (optarg, '=');
          muscle_kind kind = muscle_keyword;
          ଯଦି (ମୂଲ୍ୟ)
            {
              ଅକ୍ଷର *end = ମୂଲ୍ୟ + strlen (ମୂଲ୍ୟ) - 1;
              *ମୂଲ୍ୟ++ = 0;
              ଯଦି (*ମୂଲ୍ୟ == '{' && *end == '}')
                {
                  kind = muscle_code;
                  ++ମୂଲ୍ୟ;
                  *end = 0;
                }
              ଅନ୍ୟଥା ଯଦି (*ମୂଲ୍ୟ == '"' && *end == '"')
                {
                  kind = muscle_string;
                  ++ମୂଲ୍ୟ;
                  *end = 0;
                }
            }
          muscle_percent_define_ଭର୍ତ୍ତି (name, loc,
                                        kind, ମୂଲ୍ୟ ? ମୂଲ୍ୟ : "",
                                        c == 'D' ? MUSCLE_PERCENT_DEFINE_D
                                                 : MUSCLE_PERCENT_DEFINE_F);
        }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'H':
      ସ୍ଥିତି 'd':
        header_flag = true;
        ଯଦି (optarg)
          {
            ମୁକ୍ତ (spec_header_file);
            spec_header_file = xstrdup (optarg);
          }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'L':
        language_argmatch (optarg, command_ଧାଡ଼ି_prio, loc);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'M': // -MOLDPREFIX=NEWPREFIX
        {
          ଅକ୍ଷର *newprefix = strchr (optarg, '=');
          ଯଦି (newprefix)
            {
              *newprefix = '\0';
              add_prefix_map (optarg, newprefix + 1);
            }
          ଅନ୍ୟଥା
            {
              complain (&loc, complaint, _("invalid argument for %s: %s"),
                        quote ("--file-prefix-map"), quotearg_n (1, optarg));
            }
        }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'S':
        skeleton_arg (optarg, command_ଧାଡ଼ି_prio, loc);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'T':
        FLAGS_ARGMATCH (trace, optarg, trace_all);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'V':
        version ();
        exit (EXIT_SUCCESS);

      ସ୍ଥିତି 'f':
        FLAGS_ARGMATCH (feature, optarg, feature_all);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'W':
        warnings_argmatch (optarg);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'b':
        spec_file_prefix = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'g':
        graph_flag = true;
        ଯଦି (optarg)
          {
            ମୁକ୍ତ (spec_graph_file);
            spec_graph_file = xstrdup (optarg);
          }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'h':
        usage (EXIT_SUCCESS);

      ସ୍ଥିତି 'k':
        token_table_flag = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'l':
        no_ଧାଡ଼ିଗୁଡ଼ିକ_flag = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'o':
        spec_outfile = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'p':
        spec_name_prefix = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'r':
        FLAGS_ARGMATCH (report, optarg, report_all);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 't':
        muscle_percent_define_ଭର୍ତ୍ତି ("parse.trace",
                                      loc,
                                      muscle_keyword, "",
                                      MUSCLE_PERCENT_DEFINE_D);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'u':
        update_flag = true;
        feature_flag |= feature_syntax_only;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'v':
        report_flag |= report_states;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'x':
        xml_flag = true;
        ଯଦି (optarg)
          {
            ମୁକ୍ତ (spec_xml_file);
            spec_xml_file = xstrdup (optarg);
          }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'y':
        warning_argmatch ("yacc", 0, 0);
        set_yacc (loc);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି COLOR_OPTION:
        /* Handled in getargs_colors. */
        ଭାଙ୍ଗ;

      ସ୍ଥିତି HTML_OPTION:
        html_flag = true;
        xml_flag = true;
        ଯଦି (optarg)
          {
            ମୁକ୍ତ (spec_html_file);
            spec_html_file = xstrdup (optarg);
          }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି FIXED_OUTPUT_FILES_OPTION:
        complain (&loc, Wdeprecated,
                  _("deprecated option: %s, use %s"),
                  quote ("--fixed-output-files"), quote_n (1, "-o y.tab.c"));
        spec_outfile = "y.tab.c";
        ଭାଙ୍ଗ;

      ସ୍ଥିତି LOCATIONS_OPTION:
        muscle_percent_define_ensure ("locations", loc, true);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି PRINT_LOCALEDIR_OPTION:
        printf ("%s\n", LOCALEDIR);
        exit (EXIT_SUCCESS);

      ସ୍ଥିତି PRINT_DATADIR_OPTION:
        printf ("%s\n", pkgdatadir ());
        exit (EXIT_SUCCESS);

      ସ୍ଥିତି REPORT_FILE_OPTION:
        ମୁକ୍ତ (spec_verbose_file);
        spec_verbose_file = xstrdup (optarg);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି STYLE_OPTION:
        /* Handled in getargs_colors. */
        ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }
  }

  ଯଦି (argc - optind != 1)
    {
      ଯଦି (argc - optind < 1)
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  grammar_file = uniqstr_new (argv[optind]);
  MUSCLE_ଭର୍ତ୍ତି_C_STRING ("file_name", grammar_file);
}

ଶୂନ୍ୟ
tr (ଅକ୍ଷର *s, ଅକ୍ଷର from, ଅକ୍ଷର to)
{
  ପାଇଁ (; *s; ++s)
    ଯଦି (*s == from)
      *s = to;
}
