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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>
#شامل <bitset/stats.h>
#شامل <closeout.h>
#شامل <configmake.h>
#شامل <progname.h>
#شامل <quote.h>
#شامل <quotearg.h>
#شامل <relocatable.h> /* relocate2 */
#شامل <timevar.h>

#شامل "complain.h"
#شامل "conflicts.h"
#شامل "counterexample.h"
#شامل "derives.h"
#شامل "files.h"
#شامل "fixits.h"
#شامل "getargs.h"
#شامل "glyphs.h"
#شامل "gram.h"
#شامل "ielr.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "muscle-tab.h"
#شامل "nullable.h"
#شامل "output.h"
#شامل "parse-gram.h"
#شامل "print-graph.h"
#شامل "print-xml.h"
#شامل "print.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "scan-code.h"
#شامل "scan-gram.h"
#شامل "scan-skel.h"
#شامل "symtab.h"
#شامل "tables.h"
#شامل "uniqstr.h"


صحیح
اصلی (صحیح argc, نویسه *argv[])
{
  {
    نویسه *cp = getenv ("BISON_PROGRAM_NAME");
    اگر (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    نویسه *cp = پوچ;
    نویسه ثابت *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    آزاد (cp);
  }
  textdomain ("bison");

  {
    نویسه ثابت *cp = getenv ("LC_CTYPE");
    اگر (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    وگرنه
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  اگر (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  اگر (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_افزودن (tv_reader);
  reader (grammar_file);
  timevar_برداشت (tv_reader);

  اگر (complaint_status == status_complaint)
    برو finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_افزودن (tv_reduce);
  reduce_grammar ();
  timevar_برداشت (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_افزودن (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_برداشت (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_افزودن (tv_lr0);
  generate_states ();
  timevar_برداشت (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_افزودن (tv_conflicts);
  conflicts_solve ();
  اگر (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_عدد *old_to_new = xnmalloc (nstates, اندازه *old_to_new);
      state_عدد nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      آزاد (old_to_new);
    }
  اگر (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_برداشت (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_افزودن (tv_actions);
  tables_generate ();
  timevar_برداشت (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  اگر (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      اگر (report_flag)
        {
          timevar_افزودن (tv_report);
          print_results ();
          timevar_برداشت (tv_report);
        }

      /* Output the graph.  */
      اگر (graph_flag)
        {
          timevar_افزودن (tv_graph);
          print_graph ();
          timevar_برداشت (tv_graph);
        }

      /* Output xml.  */
      اگر (html_flag || xml_flag)
        {
          timevar_افزودن (tv_xml);
          print_xml ();
          timevar_برداشت (tv_xml);
        }

      /* Output html.  */
      اگر (html_flag)
        {
          timevar_افزودن (tv_html);
          print_html ();
          timevar_برداشت (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  اگر (complaint_status == status_complaint)
    برو finish;

  /* Lookahead tokens are no longer needed. */
  timevar_افزودن (tv_free);
  lalr_free ();
  timevar_برداشت (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  اگر (generate)
    {
      timevar_افزودن (tv_parser);
      output ();
      timevar_برداشت (tv_parser);
    }

 finish:

  timevar_افزودن (tv_free);
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
  timevar_برداشت (tv_free);

  اگر (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  اگر (!fixits_empty ())
    {
      اگر (update_flag)
        fixits_run ();
      وگرنه
        complain (پوچ, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  بازگشت complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
