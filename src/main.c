/* Top level entry point of Bison.

   Copyright (C) 1984, 1986, 1989, 1992, 1995, 2000-2002, 2004-2015,
   2018-2022 Free Software Foundation, Inc.

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
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <bitset/stats.h>
#अन्तर्भाव <closeout.h>
#अन्तर्भाव <configmake.h>
#अन्तर्भाव <progname.h>
#अन्तर्भाव <quote.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <relocatable.h> /* relocate2 */
#अन्तर्भाव <timevar.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "counterexample.h"
#अन्तर्भाव "derives.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "fixits.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "glyphs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "ielr.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "nullable.h"
#अन्तर्भाव "output.h"
#अन्तर्भाव "parse-gram.h"
#अन्तर्भाव "print-graph.h"
#अन्तर्भाव "print-xml.h"
#अन्तर्भाव "print.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "scan-code.h"
#अन्तर्भाव "scan-gram.h"
#अन्तर्भाव "scan-skel.h"
#अन्तर्भाव "symtab.h"
#अन्तर्भाव "tables.h"
#अन्तर्भाव "uniqstr.h"


पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण *argv[])
{
  {
    वर्ण *cp = getenv ("BISON_PROGRAM_NAME");
    जइ (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    वर्ण *cp = रिक्त;
    वर्ण ध्रुव *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    मुक्त (cp);
  }
  textdomain ("bison");

  {
    वर्ण ध्रुव *cp = getenv ("LC_CTYPE");
    जइ (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    अण्णहा
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  जइ (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  जइ (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_प्रक्षिप (tv_reader);
  reader (grammar_file);
  timevar_निष्कासय (tv_reader);

  जइ (complaint_status == status_complaint)
    गच्छ finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_प्रक्षिप (tv_reduce);
  reduce_grammar ();
  timevar_निष्कासय (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_प्रक्षिप (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_निष्कासय (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_प्रक्षिप (tv_lr0);
  generate_states ();
  timevar_निष्कासय (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_प्रक्षिप (tv_conflicts);
  conflicts_solve ();
  जइ (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_सङ्ख्या *old_to_new = xnmalloc (nstates, परिमाण *old_to_new);
      state_सङ्ख्या nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      मुक्त (old_to_new);
    }
  जइ (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_निष्कासय (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_प्रक्षिप (tv_actions);
  tables_generate ();
  timevar_निष्कासय (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  जइ (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      जइ (report_flag)
        {
          timevar_प्रक्षिप (tv_report);
          print_results ();
          timevar_निष्कासय (tv_report);
        }

      /* Output the graph.  */
      जइ (graph_flag)
        {
          timevar_प्रक्षिप (tv_graph);
          print_graph ();
          timevar_निष्कासय (tv_graph);
        }

      /* Output xml.  */
      जइ (html_flag || xml_flag)
        {
          timevar_प्रक्षिप (tv_xml);
          print_xml ();
          timevar_निष्कासय (tv_xml);
        }

      /* Output html.  */
      जइ (html_flag)
        {
          timevar_प्रक्षिप (tv_html);
          print_html ();
          timevar_निष्कासय (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  जइ (complaint_status == status_complaint)
    गच्छ finish;

  /* Lookahead tokens are no longer needed. */
  timevar_प्रक्षिप (tv_free);
  lalr_free ();
  timevar_निष्कासय (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  जइ (generate)
    {
      timevar_प्रक्षिप (tv_parser);
      output ();
      timevar_निष्कासय (tv_parser);
    }

 finish:

  timevar_प्रक्षिप (tv_free);
  nullable_free ();
  derives_free ();
  tables_free ();
  states_free ();
  reduce_free ();
  conflicts_free ();
  grammar_free ();
  counterexample_free ();
  output_file_names_free ();

  /* The scanner and parser memory cannot be released right after
     parsing, as it contains things such as user actions, prologue,
     epilogue etc.  */
  gram_scanner_free ();
  parser_free ();

  muscle_free ();
  code_scanner_free ();
  skel_scanner_free ();
  timevar_निष्कासय (tv_free);

  जइ (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  जइ (!fixits_empty ())
    {
      जइ (update_flag)
        fixits_run ();
      अण्णहा
        complain (रिक्त, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  प्रत्यागम complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
