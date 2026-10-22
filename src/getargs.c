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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "getargs.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <argmatch.h>
#ಅನ್ತರ್ಭಾವ <c-strcase.h>
#ಅನ್ತರ್ಭಾವ <configmake.h>
#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <progname.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <textstyle.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "output.h"
#ಅನ್ತರ್ಭಾವ "uniqstr.h"

bool ಶೀರ್ಷಿಕೆ_flag = false;
bool ನಕ್ಷೆ_flag = false;
bool html_flag = false;
bool xml_flag = false;
bool no_ಸಾಲುಗಳು_flag = false;
bool token_ಕೋಷ್ಟಕ_flag = false;
location yacc_loc = ಖಾಲಿಗಳು_LOCATION_INIT;
bool update_flag = false; /* for -u */
bool color_debug = false;

bool nondeterministic_parser = false;
bool glr_parser = false;

ಪೂರ್ಣಾಂಕ feature_flag = feature_caret;
ಪೂರ್ಣಾಂಕ report_flag = report_none;
ಪೂರ್ಣಾಂಕ trace_flag = trace_none;

ಸ್ಥಿರ ರಚನೆ bison_language ಸ್ಥಿರಾಂಕ valid_languages[] = {
  /* lang,  skeleton,       ext,     hdr,     add_tab */
  { "c",    "c-skel.m4",    ".c",    ".h",    true },
  { "c++",  "c++-skel.m4",  ".cc",   ".hh",   true },
  { "d",    "d-skel.m4",    ".d",    ".d",    false },
  { "java", "java-skel.m4", ".java", ".java", false },
  { "", "", "", "", false }
};

ಪೂರ್ಣಾಂಕ skeleton_prio = default_prio;
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *skeleton = ಖಾಲಿ;
ಪೂರ್ಣಾಂಕ language_prio = default_prio;
ರಚನೆ bison_language ಸ್ಥಿರಾಂಕ *language = &valid_languages[0];

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ* (xargmatch_fn) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg);

ಶೂನ್ಯ
ಗಣ_yacc (location loc)
{
  yacc_loc = loc;
  ಆದರೆ (getenv ("POSIXLY_CORRECT"))
    muscle_percent_define_ಸೇರಿಸು ("posix",
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
ಸ್ಥಿರ ಶೂನ್ಯ
flag_argmatch (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt, xargmatch_fn xargmatch,
               ಪೂರ್ಣಾಂಕ all, ಪೂರ್ಣಾಂಕ *flags, ಅಕ್ಷರ *arg, size_t no)
{
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = *xargmatch (opt, arg + no);

  /* -rnone == -rno-all, and -rno-none == -rall.  */
  ಆದರೆ (!ಮೌಲ್ಯ)
    {
      ಮೌಲ್ಯ = all;
      no = !no;
    }

  ಆದರೆ (no)
    *flags &= ~ಮೌಲ್ಯ;
  ಇಲ್ಲದಿದ್ದರೆ
    *flags |= ಮೌಲ್ಯ;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (usage_fn) (FILE *ಹೊರಹೋಗು);

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
ಸ್ಥಿರ ಶೂನ್ಯ
flags_argmatch (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt,
                xargmatch_fn xargmatch,
                usage_fn usage,
                ಪೂರ್ಣಾಂಕ all, ಪೂರ್ಣಾಂಕ *flags, ಅಕ್ಷರ *ವಾದಗಳು)
{
  ಆದರೆ (!ವಾದಗಳು)
    *flags |= all;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (ವಾದಗಳು, "help"))
    {
      usage (stdout);
      ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (ವಾದಗಳು = strtok (ವಾದಗಳು, ","); ವಾದಗಳು; ವಾದಗಳು = strtok (ಖಾಲಿ, ","))
      {
        size_t no = STRPREFIX_LIT ("no-", ವಾದಗಳು) ? 3 : 0;
        flag_argmatch (opt, xargmatch,
                       all, flags, ವಾದಗಳು, no);
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
#ವ್ಯಾಖ್ಯಾನಿಸು FLAGS_ARGMATCH(FlagName, ವಾದಗಳು_2, All)                             \
  flags_argmatch ("--" #FlagName,                                       \
                  (xargmatch_fn*) argmatch_## FlagName ## ಮೌಲ್ಯ_2,       \
                  argmatch_ ## FlagName ## _usage,                      \
                  All, &FlagName ## _flag, ವಾದಗಳು_2)

/*---------------------.
| --color's handling.  |
`---------------------*/

ಎಣಿಕೆ color
  {
    color_always,
    color_never,
    color_auto
  };

ARGMATCH_DEFINE_ಗುಂಪು (color, ಎಣಿಕೆ color)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_color_doc argmatch_color_docs[] =
{
  { "always",     N_("colorize the output") },
  { "never",      N_("don't colorize the output") },
  { "auto",       N_("colorize if the output device is a tty") },
  { ಖಾಲಿ, ಖಾಲಿ },
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_color_arg argmatch_color_ವಾದಗಳು[] =
{
  { "always",   color_always },
  { "yes",      color_always },
  { "never",    color_never },
  { "no",       color_never },
  { "auto",     color_auto },
  { "tty",      color_auto },
  { ಖಾಲಿ, color_always },
};

ಸ್ಥಿರಾಂಕ argmatch_color_ಗುಂಪು_type argmatch_color_ಗುಂಪು =
{
  argmatch_color_ವಾದಗಳು,
  argmatch_color_docs,
  /* TRANSLATORS: Use the same translation for WHEN as in the
     --color=WHEN help message.  */
  N_("WHEN can be one of the following:"),
  ಖಾಲಿ
};


/*----------------------.
| --report's handling.  |
`----------------------*/

ARGMATCH_DEFINE_ಗುಂಪು (report, ಎಣಿಕೆ report)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_report_doc argmatch_report_docs[] =
{
  { "states",          N_("describe the states") },
  { "itemsets",        N_("complete the core item sets with their closure") },
  { "lookaheads",      N_("explicitly associate lookahead tokens to items") },
  { "solved",          N_("describe shift/reduce conflicts solving") },
  { "counterexamples", N_("generate conflict counterexamples") },
  { "all",             N_("include all the above information") },
  { "none",            N_("disable the report") },
  { ಖಾಲಿ, ಖಾಲಿ },
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_report_arg argmatch_report_ವಾದಗಳು[] =
{
  { "none",            report_none },
  { "states",          report_ಸ್ಥಿತಿಗಳು },
  { "itemsets",        report_ಸ್ಥಿತಿಗಳು | report_itemsets },
  { "lookaheads",      report_ಸ್ಥಿತಿಗಳು | report_lookaheads },
  { "solved",          report_ಸ್ಥಿತಿಗಳು | report_solved_conflicts },
  { "counterexamples", report_cex },
  { "cex",             report_cex },
  { "all",             report_all },
  { ಖಾಲಿ, report_none },
};

ಸ್ಥಿರಾಂಕ argmatch_report_ಗುಂಪು_type argmatch_report_ಗುಂಪು =
{
  argmatch_report_ವಾದಗಳು,
  argmatch_report_docs,
  /* TRANSLATORS: Use the same translation for THINGS as in the
     --report=THINGS help message.  */
  N_("THINGS is a list of comma separated words that can include:"),
  ಖಾಲಿ
};

/*---------------------.
| --trace's handling.  |
`---------------------*/

ARGMATCH_DEFINE_ಗುಂಪು (trace, ಎಣಿಕೆ trace)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_trace_doc argmatch_trace_docs[] =
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
  { ಖಾಲಿ, ಖಾಲಿ},
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_trace_arg argmatch_trace_ವಾದಗಳು[] =
{
  { "none",      trace_none },
  { "locations", trace_locations },
  { "scan",      trace_ಪರಿಶೀಲನೆ },
  { "parse",     trace_parse },
  { "automaton", trace_automaton },
  { "bitsets",   trace_bitsets },
  { "closure",   trace_ಆವರಣ },
  { "grammar",   trace_grammar },
  { "resource",  trace_resource },
  { "sets",      trace_sets },
  { "muscles",   trace_muscles },
  { "tools",     trace_tools },
  { "m4-early",  trace_m4_early },
  { "m4",        trace_m4 },
  { "skeleton",  trace_skeleton },
  { "time",      trace_ಸಮಯ },
  { "ielr",      trace_ielr },
  { "cex",       trace_cex },
  { "all",       trace_all },
  { ಖಾಲಿ,        trace_none},
};

ಸ್ಥಿರಾಂಕ argmatch_trace_ಗುಂಪು_type argmatch_trace_ಗುಂಪು =
{
  argmatch_trace_ವಾದಗಳು,
  argmatch_trace_docs,
  N_("TRACES is a list of comma separated words that can include:"),
  ಖಾಲಿ
};

/*-----------------------.
| --feature's handling.  |
`-----------------------*/

ARGMATCH_DEFINE_ಗುಂಪು (feature, ಎಣಿಕೆ feature)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_feature_doc argmatch_feature_docs[] =
{
  { "caret",       N_("show errors with carets") },
  { "fixit",       N_("show machine-readable fixes") },
  { "syntax-only", N_("do not generate any file") },
  { "all",         N_("all of the above") },
  { "none",        N_("disable all of the above") },
  { ಖಾಲಿ, ಖಾಲಿ }
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ argmatch_feature_arg argmatch_feature_ವಾದಗಳು[] =
{
  { "none",                          feature_none },
  { "caret",                         feature_caret },
  { "diagnostics-show-caret",        feature_caret },
  { "fixit",                         feature_fixit },
  { "diagnostics-parseable-fixits",  feature_fixit },
  { "syntax-only",                   feature_syntax_only },
  { "all",                           feature_all },
  { ಖಾಲಿ, feature_none}
};

ಸ್ಥಿರಾಂಕ argmatch_feature_ಗುಂಪು_type argmatch_feature_ಗುಂಪು =
{
  argmatch_feature_ವಾದಗಳು,
  argmatch_feature_docs,
  /* TRANSLATORS: Use the same translation for FEATURES as in the
     --feature=FEATURES help message.  */
  N_("FEATURES is a list of comma separated words that can include:"),
  ಖಾಲಿ
};

/*-------------------------------------------.
| Display the help message and exit STATUS.  |
`-------------------------------------------*/

 _Noreturn
ಸ್ಥಿರ ಶೂನ್ಯ usage (ಪೂರ್ಣಾಂಕ);

ಸ್ಥಿರ ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != 0)
    fprintf (stderr, _("Try '%s --help' for more information.\n"),
             ಕಾರ್ಯಕ್ರಮ_name);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* For ../build-aux/cross-options.pl to work, use the format:
                ^  -S, --long[=ARGS] (whitespace)
         A --long option is required.
         Otherwise, add exceptions to ../build-aux/cross-options.pl.  */

      ಬರೆ (_("Usage: %s [OPTION]... FILE\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Generate a deterministic LR or generalized LR (GLR) parser employing\n\
LALR(1), IELR(1), or canonical LR(1) parser tables.\n\
\n\
"), stdout);

      fputs (_("\
Mandatory arguments ಗಮ್ಯ ದೀರ್ಘ options are mandatory ಗಾಗಿ ಗಿಡ್ಡ options too.\n\
"), stdout);
      fputs (_("\
The same is true ಗಾಗಿ optional arguments.\n\
"), stdout);
      putc ('\n', stdout);

      fputs (_("\
Operation Modes:\n\
  -h, --help                 display this help and ನಿರ್ಗಮಿಸು\n\
  -V, --ಆವೃತ್ತಿ              output ಆವೃತ್ತಿ information and ನಿರ್ಗಮಿಸು\n\
      --print-localedir      output directory containing locale-dependent ದತ್ತಾಂಶ\n\
                             and ನಿರ್ಗಮಿಸು\n\
      --print-datadir        output directory containing skeletons and XSLT\n\
                             and ನಿರ್ಗಮಿಸು\n\
  -u, --update               apply fixes ಗಮ್ಯ the ಮೂಲ grammar file and ನಿರ್ಗಮಿಸು\n\
  -f, --feature[=FEATURES]   activate miscellaneous features\n\
\n\
"), stdout);

      argmatch_feature_usage (stdout);
      putc ('\n', stdout);

      fputs (_("\
Diagnostics:\n\
  -W, --warnings[=CATEGORY]  report the warnings falling ಒಳ CATEGORY\n\
      --color[=WHEN]         whether ಗಮ್ಯ colorize the diagnostics\n\
      --style=FILE           specify the CSS FILE ಗಾಗಿ colorizer diagnostics\n\
\n\
"), stdout);

      warning_usage (stdout);
      putc ('\n', stdout);

      argmatch_color_usage (stdout);
      putc ('\n', stdout);

      fputs (_("\
Tuning the Parser:\n\
  -L, --language=LANGUAGE          specify the output programming language\n\
  -S, --skeleton=FILE              specify the skeleton ಗಮ್ಯ use\n\
  -t, --debug                      instrument the parser ಗಾಗಿ tracing\n\
                                   same as '-Dparse.trace'\n\
      --locations                  enable location support\n\
  -D, --define=NAME[=ಮೌಲ್ಯ_3]        similar ಗಮ್ಯ '%define NAME VALUE'\n\
  -F, --force-define=NAME[=ಮೌಲ್ಯ_3]  override '%define NAME VALUE'\n\
  -p, --name-ಪೂರ್ವಪ್ರತ್ಯಯ=ಪೂರ್ವಪ್ರತ್ಯಯ_2         prepend ಪೂರ್ವಪ್ರತ್ಯಯ_2 ಗಮ್ಯ the external symbols\n\
                                   deprecated by '-Dapi.prefix={PREFIX}'\n\
  -l, --no-ಸಾಲುಗಳು                   don't generate '#ಸಾಲು' directives\n\
  -k, --token-ಕೋಷ್ಟಕ                include a ಕೋಷ್ಟಕ ದ token ಹೆಸರುಗಳು\n\
  -y, --yacc                       emulate POSIX Yacc\n\
"), stdout);
      putc ('\n', stdout);

      fputs (_("\
Output Files:\n\
  -H, --ಶೀರ್ಷಿಕೆ=[FILE]           also produce a ಶೀರ್ಷಿಕೆ file\n\
  -d                            likewise but cannot specify FILE (ಗಾಗಿ POSIX Yacc)\n\
  -r, --report=THINGS           also produce details on the automaton\n\
      --report-file=FILE        ಲಿಖ report ಗಮ್ಯ FILE\n\
  -v, --verbose                 same as '--report=state'\n\
  -b, --file-ಪೂರ್ವಪ್ರತ್ಯಯ=ಪೂರ್ವಪ್ರತ್ಯಯ_2      specify a ಪೂರ್ವಪ್ರತ್ಯಯ_2 ಗಾಗಿ output files\n\
  -o, --output=FILE             leave output ಗಮ್ಯ FILE\n\
  -g, --ನಕ್ಷೆ[=FILE]            also output a ನಕ್ಷೆ ದ the automaton\n\
      --html[=FILE]             also output an HTML report ದ the automaton\n\
  -x, --xml[=FILE]              also output an XML report ದ the automaton\n\
  -M, --file-ಪೂರ್ವಪ್ರತ್ಯಯ-map=OLD=NEW replace ಪೂರ್ವಪ್ರತ್ಯಯ OLD with NEW when writing file paths\n\
                                ಒಳ output files\n\
"), stdout);
      putc ('\n', stdout);

      argmatch_report_usage (stdout);
      putc ('\n', stdout);

      ಬರೆ (_("Report bugs to <%s>.\n"), PACKAGE_BUGREPORT);
      ಬರೆ (_("%s home page: <%s>.\n"), PACKAGE_NAME, PACKAGE_URL);
      fputs (_("General help using GNU software: "
               "<https://www.gnu.org/gethelp/>.\n"),
             stdout);

#ಆದರೆ (defined __GLIBC__ && __GLIBC__ >= 2) && !defined __UCLIBC__
      /* Don't output this redundant message for English locales.
         Note we still output for 'C' so that it gets included in the
         man page.  */
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lc_messages = setlocale (LC_MESSAGES, ಖಾಲಿ);
      ಆದರೆ (lc_messages && !STREQ (lc_messages, "en_"))
        /* TRANSLATORS: Replace LANG_CODE in this URL with your language code to
           form one of the URLs at https://translationproject.org/team/.
           Otherwise, replace the entire URL with your translation team's
           email address.  */
        fputs (_("Report translation bugs to "
                 "<https://translationproject.org/team/>.\n"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("For complete documentation, run: info bison.\n"), stdout);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}


/*------------------------------.
| Display the version message.  |
`------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಆವೃತ್ತಿ (ಶೂನ್ಯ)
{
  /* Some efforts were made to ease the translators' task, please
     continue.  */
  ಬರೆ ("bison (GNU Bison) %s", ಆವೃತ್ತಿ_2);
  putc ('\n', stdout);
  fputs (_("Written by Robert Corbett and Richard Stallman.\n"), stdout);
  putc ('\n', stdout);

  fprintf (stdout,
           _("Copyright (C) %d Free Software Foundation, Inc.\n"),
           PACKAGE_COPYRIGHT_YEAR);

  fputs (_("\
This is ಬಿಡುಗಡೆ software; see the ಮೂಲ ಗಾಗಿ copying conditions.  There is NO\n\
warranty; not even ಗಾಗಿ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
"),
         stdout);
}


/*-------------------------------------.
| --skeleton and --language handling.  |
`--------------------------------------*/

ಶೂನ್ಯ
skeleton_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, ಪೂರ್ಣಾಂಕ prio, location loc)
{
  ಆದರೆ (prio < skeleton_prio)
    {
      skeleton_prio = prio;
      skeleton = arg;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prio == skeleton_prio)
    complain (&loc, complaint,
              _("multiple skeleton declarations are invalid"));
}

ಶೂನ್ಯ
language_argmatch (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, ಪೂರ್ಣಾಂಕ prio, location loc)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2 = ಖಾಲಿ;

  ಆದರೆ (prio < language_prio)
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; valid_languages[i].language[0]; ++i)
        ಆದರೆ (c_strcasecmp (arg, valid_languages[i].language) == 0)
          {
            language_prio = prio;
            language = &valid_languages[i];
            ಹಿಂತಿರುಗಿ;
          }
      ಸಂದೇಶ_2 = _("%s: invalid language");
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (language_prio == prio)
    ಸಂದೇಶ_2 = _("multiple language declarations are invalid");

  ಆದರೆ (ಸಂದೇಶ_2)
    complain (&loc, complaint, ಸಂದೇಶ_2, quotearg_colon (arg));
}

/*----------------------.
| Process the options.  |
`----------------------*/

/* Shorts options.
   Should be computed from long_options.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ short_options[] =
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
ಎಣಿಕೆ
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
ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  /* Operation modes. */
  { "help",            no_ವಾದ,       0,   'h' },
  { "version",         no_ವಾದ,       0,   'V' },
  { "print-localedir", no_ವಾದ,       0,   PRINT_LOCALEDIR_OPTION },
  { "print-datadir",   no_ವಾದ,       0,   PRINT_DATADIR_OPTION   },
  { "update",          no_ವಾದ,       0,   'u' },
  { "feature",         optional_ವಾದ, 0,   'f' },

  /* Diagnostics.  */
  { "warnings",        optional_ವಾದ,  0, 'W' },
  { "color",           optional_ವಾದ,  0,  COLOR_OPTION },
  { "style",           optional_ವಾದ,  0,  STYLE_OPTION },

  /* Tuning the Parser. */
  { "language",       required_ವಾದ,   0, 'L' },
  { "skeleton",       required_ವಾದ,   0, 'S' },
  { "debug",          no_ವಾದ,         0, 't' },
  { "locations",      no_ವಾದ,         0, LOCATIONS_OPTION },
  { "define",         required_ವಾದ,   0, 'D' },
  { "force-define",   required_ವಾದ,   0, 'F' },
  { "name-prefix",    required_ವಾದ,   0, 'p' },
  { "no-lines",       no_ವಾದ,         0, 'l' },
  { "token-table",    no_ವಾದ,         0, 'k' },
  { "yacc",           no_ವಾದ,         0, 'y' },

  /* Output Files. */
  { "header",          optional_ವಾದ,   0,   'H' },
  { "defines",         optional_ವಾದ,   0,   'd' },
  { "report",          required_ವಾದ,   0,   'r' },
  { "report-file",     required_ವಾದ,   0,   REPORT_FILE_OPTION },
  { "verbose",         no_ವಾದ,         0,   'v' },
  { "file-prefix",     required_ವಾದ,   0,   'b' },
  { "output",          required_ವಾದ,   0,   'o' },
  { "graph",           optional_ವಾದ,   0,   'g' },
  { "html",            optional_ವಾದ,   0,   HTML_OPTION },
  { "xml",             optional_ವಾದ,   0,   'x' },
  { "file-prefix-map", required_ವಾದ,   0,   'M' },

  /* Hidden. */
  { "fixed-output-files", no_ವಾದ,       0,  FIXED_OUTPUT_FILES_OPTION },
  { "output-file",        required_ವಾದ, 0,  'o' },
  { "trace",              optional_ವಾದ, 0,  'T' },

  {0, 0, 0, 0}
};

/* Build a location for the current command line argument. */
ಸ್ಥಿರ location
command_ಸಾಲು_location (ಶೂನ್ಯ)
{
  location res;
  /* "<command line>" is used in GCC's messages about -D. */
  boundary_ಗಣ (&res.ಆರಂಭ, uniqstr_new ("<command line>"), optind - 1, -1, -1);
  res.ಕೊನೆ = res.ಆರಂಭ;
  ಹಿಂತಿರುಗಿ res;
}


/* Handle the command line options for color support.  Do it early, so
   that error messages from getargs be also colored as per the user's
   request.  This is consistent with the way GCC and Clang behave.  */

ಸ್ಥಿರ ಶೂನ್ಯ
getargs_colors (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
      ಆದರೆ (STRPREFIX_LIT ("--color=", arg))
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *color = arg + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("--color=");
          ಆದರೆ (STREQ (color, "debug"))
            color_debug = true;
          ಇಲ್ಲದಿದ್ದರೆ
            handle_color_option (color);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ ("--color", arg))
        handle_color_option (ಖಾಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STRPREFIX_LIT ("--style=", arg))
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *style = arg + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("--style=");
          handle_style_option (style);
        }
    }
  complain_init_color ();
}


ಶೂನ್ಯ
getargs (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
  getargs_colors (argc, argv);

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, short_options, long_options, ಖಾಲಿ))
         != -1)
  {
    location loc = command_ಸಾಲು_location ();
    ಆಯ್ಕೆ (c)
      {
        /* ASCII Sorting for short options (i.e., upper case then
           lower case), and then long-only options.  */

      ಸ್ಥಿತಿ 0:
        /* Certain long options cause getopt_long to return 0.  */
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'D': /* -DNAME[=(VALUE|"VALUE"|{VALUE})]. */
      ಸ್ಥಿತಿ 'F': /* -FNAME[=(VALUE|"VALUE"|{VALUE})]. */
        {
          ಅಕ್ಷರ *name = optarg;
          ಅಕ್ಷರ *ಮೌಲ್ಯ = strchr (optarg, '=');
          muscle_ಬಗೆ ಬಗೆ = muscle_keyword;
          ಆದರೆ (ಮೌಲ್ಯ)
            {
              ಅಕ್ಷರ *ಕೊನೆ = ಮೌಲ್ಯ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೌಲ್ಯ) - 1;
              *ಮೌಲ್ಯ++ = 0;
              ಆದರೆ (*ಮೌಲ್ಯ == '{' && *ಕೊನೆ == '}')
                {
                  ಬಗೆ = muscle_ಸಂಕೇತ;
                  ++ಮೌಲ್ಯ;
                  *ಕೊನೆ = 0;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಮೌಲ್ಯ == '"' && *ಕೊನೆ == '"')
                {
                  ಬಗೆ = muscle_string;
                  ++ಮೌಲ್ಯ;
                  *ಕೊನೆ = 0;
                }
            }
          muscle_percent_define_ಸೇರಿಸು (name, loc,
                                        ಬಗೆ, ಮೌಲ್ಯ ? ಮೌಲ್ಯ : "",
                                        c == 'D' ? MUSCLE_PERCENT_DEFINE_D
                                                 : MUSCLE_PERCENT_DEFINE_F);
        }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'H':
      ಸ್ಥಿತಿ 'd':
        ಶೀರ್ಷಿಕೆ_flag = true;
        ಆದರೆ (optarg)
          {
            ಬಿಡುಗಡೆ (spec_ಶೀರ್ಷಿಕೆ_file);
            spec_ಶೀರ್ಷಿಕೆ_file = xstrdup (optarg);
          }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'L':
        language_argmatch (optarg, command_ಸಾಲು_prio, loc);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'M': // -MOLDPREFIX=NEWPREFIX
        {
          ಅಕ್ಷರ *newprefix = strchr (optarg, '=');
          ಆದರೆ (newprefix)
            {
              *newprefix = '\0';
              ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ_map (optarg, newprefix + 1);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              complain (&loc, complaint, _("invalid argument for %s: %s"),
                        quote ("--file-prefix-map"), quotearg_n (1, optarg));
            }
        }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'S':
        skeleton_arg (optarg, command_ಸಾಲು_prio, loc);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'T':
        FLAGS_ARGMATCH (trace, optarg, trace_all);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'V':
        ಆವೃತ್ತಿ ();
        ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);

      ಸ್ಥಿತಿ 'f':
        FLAGS_ARGMATCH (feature, optarg, feature_all);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'W':
        warnings_argmatch (optarg);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'b':
        spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'g':
        ನಕ್ಷೆ_flag = true;
        ಆದರೆ (optarg)
          {
            ಬಿಡುಗಡೆ (spec_ನಕ್ಷೆ_file);
            spec_ನಕ್ಷೆ_file = xstrdup (optarg);
          }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'h':
        usage (EXIT_SUCCESS);

      ಸ್ಥಿತಿ 'k':
        token_ಕೋಷ್ಟಕ_flag = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'l':
        no_ಸಾಲುಗಳು_flag = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'o':
        spec_outfile = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'p':
        spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'r':
        FLAGS_ARGMATCH (report, optarg, report_all);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 't':
        muscle_percent_define_ಸೇರಿಸು ("parse.trace",
                                      loc,
                                      muscle_keyword, "",
                                      MUSCLE_PERCENT_DEFINE_D);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'u':
        update_flag = true;
        feature_flag |= feature_syntax_only;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'v':
        report_flag |= report_ಸ್ಥಿತಿಗಳು;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'x':
        xml_flag = true;
        ಆದರೆ (optarg)
          {
            ಬಿಡುಗಡೆ (spec_xml_file);
            spec_xml_file = xstrdup (optarg);
          }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'y':
        warning_argmatch ("yacc", 0, 0);
        ಗಣ_yacc (loc);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ COLOR_OPTION:
        /* Handled in getargs_colors. */
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ HTML_OPTION:
        html_flag = true;
        xml_flag = true;
        ಆದರೆ (optarg)
          {
            ಬಿಡುಗಡೆ (spec_html_file);
            spec_html_file = xstrdup (optarg);
          }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ FIXED_OUTPUT_FILES_OPTION:
        complain (&loc, Wdeprecated,
                  _("deprecated option: %s, use %s"),
                  quote ("--fixed-output-files"), quote_n (1, "-o y.tab.c"));
        spec_outfile = "y.tab.c";
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ LOCATIONS_OPTION:
        muscle_percent_define_ensure ("locations", loc, true);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ PRINT_LOCALEDIR_OPTION:
        ಬರೆ ("%s\n", LOCALEDIR);
        ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);

      ಸ್ಥಿತಿ PRINT_DATADIR_OPTION:
        ಬರೆ ("%s\n", pkgdatadir ());
        ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);

      ಸ್ಥಿತಿ REPORT_FILE_OPTION:
        ಬಿಡುಗಡೆ (spec_verbose_file);
        spec_verbose_file = xstrdup (optarg);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ STYLE_OPTION:
        /* Handled in getargs_colors. */
        ನಿಲ್ಲಿಸು;

      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }
  }

  ಆದರೆ (argc - optind != 1)
    {
      ಆದರೆ (argc - optind < 1)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  grammar_file = uniqstr_new (argv[optind]);
  MUSCLE_ಸೇರಿಸು_C_STRING ("file_name", grammar_file);
}

ಶೂನ್ಯ
tr (ಅಕ್ಷರ *s, ಅಕ್ಷರ from, ಅಕ್ಷರ ಗಮ್ಯ)
{
  ಗಾಗಿ (; *s; ++s)
    ಆದರೆ (*s == from)
      *s = ಗಮ್ಯ;
}
