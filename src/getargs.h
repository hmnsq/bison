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

تعداد { command_هێڵ_prio, grammar_prio, default_prio };

/* flags set by % directives */

/* for -S */
خارجي حرف ثابت *skeleton;
خارجي صحيح skeleton_prio;

/* for -I */
خارجي حرف ثابت *include;

خارجي bool header_flag;                /* for -d/-H */
خارجي bool graph_flag;                 /* for -g */
خارجي bool html_flag;                  /* for --html */
خارجي bool xml_flag;                   /* for -x */
خارجي bool no_هێڵەکان_flag;              /* for -l */
خارجي bool token_table_flag;           /* for -k */
خارجي location yacc_loc;               /* for -y */
خارجي bool update_flag;                /* for -u */
خارجي bool color_debug;                /* --color=debug. */
/* GLR_PARSER is true if the input file says to use the GLR
   (Generalized LR) parser, and to output some additional information
   used by the GLR algorithm.  */

خارجي bool glr_parser;

/* NONDETERMINISTIC_PARSER is true iff conflicts are accepted.  This
   is used by the GLR parser, and might be used in BackTracking
   parsers too.  */

خارجي bool nondeterministic_parser;


/* --language.  */
بنية bison_language
{
  حرف language[حجم "Java"];
  حرف skeleton[حجم "java-skel.m4"];
  حرف src_extension[حجم ".java"];
  حرف header_extension[حجم ".java"];
  bool add_tab;
};

خارجي صحيح language_prio;
خارجي بنية bison_language ثابت *language;

/*-----------.
| --report.  |
`-----------*/

تعداد report
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
خارجي صحيح report_flag;

/*----------.
| --trace.  |
`----------*/
تعداد trace
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
خارجي صحيح trace_flag;

/*-------------.
| --features.  |
`-------------*/

تعداد feature
  {
    feature_none             = 0,      /**< No additional feature.  */
    feature_caret            = 1 << 0, /**< Output errors with carets.  */
    feature_fixit            = 1 << 1, /**< Issue instructions to fix the sources.  */
    feature_syntax_only      = 1 << 2, /**< Don't generate output.  */
    feature_all              = ~0      /**< All above features.  */
  };
/** What additional features to use.  */
خارجي صحيح feature_flag;


/** Process the command line arguments.
 *
 *  \param argc   size of \a argv
 *  \param argv   list of arguments.
 */
فراغ getargs (صحيح argc, حرف *argv[]);

/* Used by parse-gram.y.  */
فراغ language_argmatch (حرف ثابت *arg, صحيح prio, location loc);
فراغ skeleton_arg (ثابت حرف *arg, صحيح prio, location loc);
فراغ set_yacc (location loc);

/** In the string \c s, replace all characters \c from by \c to.  */
فراغ tr (حرف *s, حرف from, حرف to);

#endif /* !GETARGS_H_ */
