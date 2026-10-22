/* Parse command line arguments for bison.

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

#ifndef GETARGS_H_
# define GETARGS_H_

# include "location.h"

গণনা { command_সারি_prio, grammar_prio, default_prio };

/* flags set by % directives */

/* for -S */
বাহ্য বর্ণ ধ্রুব *skeleton;
বাহ্য পূর্ণাঙ্ক skeleton_prio;

/* for -I */
বাহ্য বর্ণ ধ্রুব *include;

বাহ্য bool header_flag;                /* for -d/-H */
বাহ্য bool graph_flag;                 /* for -g */
বাহ্য bool html_flag;                  /* for --html */
বাহ্য bool xml_flag;                   /* for -x */
বাহ্য bool no_সারিগুলি_flag;              /* for -l */
বাহ্য bool token_table_flag;           /* for -k */
বাহ্য location yacc_loc;               /* for -y */
বাহ্য bool update_flag;                /* for -u */
বাহ্য bool color_debug;                /* --color=debug. */
/* GLR_PARSER is true if the input file says to use the GLR
   (Generalized LR) parser, and to output some additional information
   used by the GLR algorithm.  */

বাহ্য bool glr_parser;

/* NONDETERMINISTIC_PARSER is true iff conflicts are accepted.  This
   is used by the GLR parser, and might be used in BackTracking
   parsers too.  */

বাহ্য bool nondeterministic_parser;


/* --language.  */
সংরচনা bison_language
{
  বর্ণ language[পরিমাণ "Java"];
  বর্ণ skeleton[পরিমাণ "java-skel.m4"];
  বর্ণ src_extension[পরিমাণ ".java"];
  বর্ণ header_extension[পরিমাণ ".java"];
  bool add_tab;
};

বাহ্য পূর্ণাঙ্ক language_prio;
বাহ্য সংরচনা bison_language ধ্রুব *language;

/*-----------.
| --report.  |
`-----------*/

গণনা report
  {
    report_none             = 0,
    report_states           = 1 << 0,
    report_itemsets         = 1 << 1,
    report_lookaheads       = 1 << 2,
    report_solved_conflicts = 1 << 3,
    report_cex              = 1 << 4,
    report_all              = ~0
  };
/** What appears in the *.output file.  */
বাহ্য পূর্ণাঙ্ক report_flag;

/*----------.
| --trace.  |
`----------*/
গণনা trace
  {
    trace_none      = 0,       /**< No traces. */
    trace_scan      = 1 << 0,  /**< Grammar scanner traces. */
    trace_parse     = 1 << 1,  /**< Grammar parser traces. */
    trace_resource  = 1 << 2,  /**< Memory allocation. */
    trace_sets      = 1 << 3,  /**< Grammar sets: firsts, nullable etc. */
    trace_bitsets   = 1 << 4,  /**< Use of bitsets. */
    trace_tools     = 1 << 5,  /**< m4 invocation. */
    trace_automaton = 1 << 6,  /**< Construction of the automaton. */
    trace_grammar   = 1 << 7,  /**< Reading, reducing the grammar. */
    trace_time      = 1 << 8,  /**< Time consumption. */
    trace_skeleton  = 1 << 9,  /**< Skeleton postprocessing. */
    trace_m4_early  = 1 << 10, /**< M4 early traces. */
    trace_m4        = 1 << 11, /**< M4 traces. */
    trace_muscles   = 1 << 12, /**< M4 definitions of the muscles. */
    trace_ielr      = 1 << 13, /**< IELR conversion. */
    trace_closure   = 1 << 14, /**< Input/output of closure(). */
    trace_locations = 1 << 15, /**< Full display of locations. */
    trace_cex       = 1 << 16, /**< Counterexample generation */
    trace_all       = ~0       /**< All of the above.  */
  };
/** What debug items bison displays during its run.  */
বাহ্য পূর্ণাঙ্ক trace_flag;

/*-------------.
| --features.  |
`-------------*/

গণনা feature
  {
    feature_none             = 0,      /**< No additional feature.  */
    feature_caret            = 1 << 0, /**< Output errors with carets.  */
    feature_fixit            = 1 << 1, /**< Issue instructions to fix the sources.  */
    feature_syntax_only      = 1 << 2, /**< Don't generate output.  */
    feature_all              = ~0      /**< All above features.  */
  };
/** What additional features to use.  */
বাহ্য পূর্ণাঙ্ক feature_flag;


/** Process the command line arguments.
 *
 *  \param argc   size of \a argv
 *  \param argv   list of arguments.
 */
শূন্য getargs (পূর্ণাঙ্ক argc, বর্ণ *argv[]);

/* Used by parse-gram.y.  */
শূন্য language_argmatch (বর্ণ ধ্রুব *arg, পূর্ণাঙ্ক prio, location loc);
শূন্য skeleton_arg (ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক prio, location loc);
শূন্য set_yacc (location loc);

/** In the string \c s, replace all characters \c from by \c to.  */
শূন্য tr (বর্ণ *s, বর্ণ from, বর্ণ to);

#endif /* !GETARGS_H_ */
