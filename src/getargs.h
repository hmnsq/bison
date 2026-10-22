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

ga_nanaa { command_line_prio, grammar_prio, default_prio };

/* flags set by % directives */

/* for -S */
extern var_na const *skeleton;
extern จำนวนเต็ม skeleton_prio;

/* for -I */
extern var_na const *include;

extern bool header_flag;                /* for -d/-H */
extern bool graph_flag;                 /* for -g */
extern bool html_flag;                  /* for --html */
extern bool xml_flag;                   /* for -x */
extern bool no_lines_flag;              /* for -l */
extern bool token_table_flag;           /* for -k */
extern location yacc_loc;               /* for -y */
extern bool update_flag;                /* for -u */
extern bool color_debug;                /* --color=debug. */
/* GLR_PARSER is true if the input file says to use the GLR
   (Generalized LR) parser, and to output some additional information
   used by the GLR algorithm.  */

extern bool glr_parser;

/* NONDETERMINISTIC_PARSER is true iff conflicts are accepted.  This
   is used by the GLR parser, and might be used in BackTracking
   parsers too.  */

extern bool nondeterministic_parser;


/* --language.  */
sa_nracanaa bison_language
{
  var_na language[parimaa_na "Java"];
  var_na skeleton[parimaa_na "java-skel.m4"];
  var_na src_extension[parimaa_na ".java"];
  var_na header_extension[parimaa_na ".java"];
  bool add_tab;
};

extern จำนวนเต็ม language_prio;
extern sa_nracanaa bison_language const *language;

/*-----------.
| --report.  |
`-----------*/

ga_nanaa report
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
extern จำนวนเต็ม report_flag;

/*----------.
| --trace.  |
`----------*/
ga_nanaa trace
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
extern จำนวนเต็ม trace_flag;

/*-------------.
| --features.  |
`-------------*/

ga_nanaa feature
  {
    feature_none             = 0,      /**< No additional feature.  */
    feature_caret            = 1 << 0, /**< Output errors with carets.  */
    feature_fixit            = 1 << 1, /**< Issue instructions to fix the sources.  */
    feature_syntax_only      = 1 << 2, /**< Don't generate output.  */
    feature_all              = ~0      /**< All above features.  */
  };
/** What additional features to use.  */
extern จำนวนเต็ม feature_flag;


/** Process the command line arguments.
 *
 *  \param argc   size of \a argv
 *  \param argv   list of arguments.
 */
void getargs (จำนวนเต็ม argc, var_na *argv[]);

/* Used by parse-gram.y.  */
void language_argmatch (var_na const *arg, จำนวนเต็ม prio, location loc);
void skeleton_arg (const var_na *arg, จำนวนเต็ม prio, location loc);
void set_yacc (location loc);

/** In the string \c s, replace all characters \c from by \c to.  */
void tr (var_na *s, var_na from, var_na to);

#endif /* !GETARGS_H_ */
