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

ಗಣನಾ { command_ಸಾಲು_prio, grammar_prio, default_prio };

/* flags set by % directives */

/* for -S */
ಬಾಹ್ಯ ವರ್ಣ ಧ್ರುವ *skeleton;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ skeleton_prio;

/* for -I */
ಬಾಹ್ಯ ವರ್ಣ ಧ್ರುವ *include;

ಬಾಹ್ಯ bool header_flag;                /* for -d/-H */
ಬಾಹ್ಯ bool graph_flag;                 /* for -g */
ಬಾಹ್ಯ bool html_flag;                  /* for --html */
ಬಾಹ್ಯ bool xml_flag;                   /* for -x */
ಬಾಹ್ಯ bool no_ಸಾಲುಗಳು_flag;              /* for -l */
ಬಾಹ್ಯ bool token_table_flag;           /* for -k */
ಬಾಹ್ಯ location yacc_loc;               /* for -y */
ಬಾಹ್ಯ bool update_flag;                /* for -u */
ಬಾಹ್ಯ bool color_debug;                /* --color=debug. */
/* GLR_PARSER is true if the input file says to use the GLR
   (Generalized LR) parser, and to output some additional information
   used by the GLR algorithm.  */

ಬಾಹ್ಯ bool glr_parser;

/* NONDETERMINISTIC_PARSER is true iff conflicts are accepted.  This
   is used by the GLR parser, and might be used in BackTracking
   parsers too.  */

ಬಾಹ್ಯ bool nondeterministic_parser;


/* --language.  */
ಸಂರಚನಾ bison_language
{
  ವರ್ಣ language[ಪರಿಮಾಣ "Java"];
  ವರ್ಣ skeleton[ಪರಿಮಾಣ "java-skel.m4"];
  ವರ್ಣ src_extension[ಪರಿಮಾಣ ".java"];
  ವರ್ಣ header_extension[ಪರಿಮಾಣ ".java"];
  bool add_tab;
};

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ language_prio;
ಬಾಹ್ಯ ಸಂರಚನಾ bison_language ಧ್ರುವ *language;

/*-----------.
| --report.  |
`-----------*/

ಗಣನಾ report
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
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ report_flag;

/*----------.
| --trace.  |
`----------*/
ಗಣನಾ trace
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
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ trace_flag;

/*-------------.
| --features.  |
`-------------*/

ಗಣನಾ feature
  {
    feature_none             = 0,      /**< No additional feature.  */
    feature_caret            = 1 << 0, /**< Output errors with carets.  */
    feature_fixit            = 1 << 1, /**< Issue instructions to fix the sources.  */
    feature_syntax_only      = 1 << 2, /**< Don't generate output.  */
    feature_all              = ~0      /**< All above features.  */
  };
/** What additional features to use.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ feature_flag;


/** Process the command line arguments.
 *
 *  \param argc   size of \a argv
 *  \param argv   list of arguments.
 */
ಶೂನ್ಯ getargs (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[]);

/* Used by parse-gram.y.  */
ಶೂನ್ಯ language_argmatch (ವರ್ಣ ಧ್ರುವ *arg, ಪೂರ್ಣಾಙ್ಕ prio, location loc);
ಶೂನ್ಯ skeleton_arg (ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ prio, location loc);
ಶೂನ್ಯ set_yacc (location loc);

/** In the string \c s, replace all characters \c from by \c to.  */
ಶೂನ್ಯ tr (ವರ್ಣ *s, ವರ್ಣ from, ವರ್ಣ to);

#endif /* !GETARGS_H_ */
