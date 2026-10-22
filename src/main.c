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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <bitset/stats.h>
#ಅನ್ತರ್ಭಾವ <closeout.h>
#ಅನ್ತರ್ಭಾವ <configmake.h>
#ಅನ್ತರ್ಭಾವ <progname.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <relocatable.h> /* relocate2 */
#ಅನ್ತರ್ಭಾವ <timevar.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "counterexample.h"
#ಅನ್ತರ್ಭಾವ "derives.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "fixits.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "glyphs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "ielr.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "output.h"
#ಅನ್ತರ್ಭಾವ "parse-gram.h"
#ಅನ್ತರ್ಭಾವ "print-graph.h"
#ಅನ್ತರ್ಭಾವ "print-xml.h"
#ಅನ್ತರ್ಭಾವ "print.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "scan-code.h"
#ಅನ್ತರ್ಭಾವ "scan-gram.h"
#ಅನ್ತರ್ಭಾವ "scan-skel.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "tables.h"
#ಅನ್ತರ್ಭಾವ "uniqstr.h"


ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ *argv[])
{
  {
    ವರ್ಣ *cp = getenv ("BISON_PROGRAM_NAME");
    ಯದಿ (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    ವರ್ಣ *cp = ರಿಕ್ತ;
    ವರ್ಣ ಧ್ರುವ *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    ಮುಕ್ತ (cp);
  }
  textdomain ("bison");

  {
    ವರ್ಣ ಧ್ರುವ *cp = getenv ("LC_CTYPE");
    ಯದಿ (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    ಅನ್ಯಥಾ
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  ಯದಿ (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  ಯದಿ (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ತಳ್ಳು (tv_reader);
  reader (grammar_file);
  timevar_ತೆಗೆ (tv_reader);

  ಯದಿ (complaint_status == status_complaint)
    ಗಚ್ಛ finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_ತಳ್ಳು (tv_reduce);
  reduce_grammar ();
  timevar_ತೆಗೆ (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_ತಳ್ಳು (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_ತೆಗೆ (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_ತಳ್ಳು (tv_lr0);
  generate_states ();
  timevar_ತೆಗೆ (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_ತಳ್ಳು (tv_conflicts);
  conflicts_solve ();
  ಯದಿ (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_ಸಂಖ್ಯೆ *old_to_new = xnmalloc (nstates, ಪರಿಮಾಣ *old_to_new);
      state_ಸಂಖ್ಯೆ nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      ಮುಕ್ತ (old_to_new);
    }
  ಯದಿ (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_ತೆಗೆ (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ತಳ್ಳು (tv_actions);
  tables_generate ();
  timevar_ತೆಗೆ (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  ಯದಿ (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      ಯದಿ (report_flag)
        {
          timevar_ತಳ್ಳು (tv_report);
          print_results ();
          timevar_ತೆಗೆ (tv_report);
        }

      /* Output the graph.  */
      ಯದಿ (graph_flag)
        {
          timevar_ತಳ್ಳು (tv_graph);
          print_graph ();
          timevar_ತೆಗೆ (tv_graph);
        }

      /* Output xml.  */
      ಯದಿ (html_flag || xml_flag)
        {
          timevar_ತಳ್ಳು (tv_xml);
          print_xml ();
          timevar_ತೆಗೆ (tv_xml);
        }

      /* Output html.  */
      ಯದಿ (html_flag)
        {
          timevar_ತಳ್ಳು (tv_html);
          print_html ();
          timevar_ತೆಗೆ (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  ಯದಿ (complaint_status == status_complaint)
    ಗಚ್ಛ finish;

  /* Lookahead tokens are no longer needed. */
  timevar_ತಳ್ಳು (tv_free);
  lalr_free ();
  timevar_ತೆಗೆ (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  ಯದಿ (generate)
    {
      timevar_ತಳ್ಳು (tv_parser);
      output ();
      timevar_ತೆಗೆ (tv_parser);
    }

 finish:

  timevar_ತಳ್ಳು (tv_free);
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
  timevar_ತೆಗೆ (tv_free);

  ಯದಿ (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  ಯದಿ (!fixits_empty ())
    {
      ಯದಿ (update_flag)
        fixits_run ();
      ಅನ್ಯಥಾ
        complain (ರಿಕ್ತ, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  ಪ್ರತ್ಯಾಗಮ complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
