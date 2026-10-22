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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>
#অন্তর্ভাব <bitset/stats.h>
#অন্তর্ভাব <closeout.h>
#অন্তর্ভাব <configmake.h>
#অন্তর্ভাব <progname.h>
#অন্তর্ভাব <quote.h>
#অন্তর্ভাব <quotearg.h>
#অন্তর্ভাব <relocatable.h> /* relocate2 */
#অন্তর্ভাব <timevar.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "conflicts.h"
#অন্তর্ভাব "counterexample.h"
#অন্তর্ভাব "derives.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "fixits.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "glyphs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "ielr.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lr0.h"
#অন্তর্ভাব "muscle-tab.h"
#অন্তর্ভাব "nullable.h"
#অন্তর্ভাব "output.h"
#অন্তর্ভাব "parse-gram.h"
#অন্তর্ভাব "print-graph.h"
#অন্তর্ভাব "print-xml.h"
#অন্তর্ভাব "print.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "scan-code.h"
#অন্তর্ভাব "scan-gram.h"
#অন্তর্ভাব "scan-skel.h"
#অন্তর্ভাব "symtab.h"
#অন্তর্ভাব "tables.h"
#অন্তর্ভাব "uniqstr.h"


পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর *argv[])
{
  {
    অক্ষর *cp = getenv ("BISON_PROGRAM_NAME");
    যদি (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    অক্ষর *cp = ফাঁকা;
    অক্ষর ধ্রুবক *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    মুক্ত (cp);
  }
  textdomain ("bison");

  {
    অক্ষর ধ্রুবক *cp = getenv ("LC_CTYPE");
    যদি (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    অন্যথা
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  যদি (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_যোগ);

  যদি (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ঠেলো (tv_reader);
  reader (grammar_file);
  timevar_বের_করো (tv_reader);

  যদি (complaint_status == status_complaint)
    যাও finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_ঠেলো (tv_reduce);
  reduce_grammar ();
  timevar_বের_করো (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_ঠেলো (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_বের_করো (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_ঠেলো (tv_lr0);
  generate_states ();
  timevar_বের_করো (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_ঠেলো (tv_conflicts);
  conflicts_solve ();
  যদি (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_সংখ্যা *old_to_new = xnmalloc (nstates, আকার *old_to_new);
      state_সংখ্যা nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      মুক্ত (old_to_new);
    }
  যদি (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_বের_করো (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ঠেলো (tv_actions);
  tables_generate ();
  timevar_বের_করো (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  যদি (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      যদি (report_flag)
        {
          timevar_ঠেলো (tv_report);
          print_results ();
          timevar_বের_করো (tv_report);
        }

      /* Output the graph.  */
      যদি (graph_flag)
        {
          timevar_ঠেলো (tv_graph);
          print_graph ();
          timevar_বের_করো (tv_graph);
        }

      /* Output xml.  */
      যদি (html_flag || xml_flag)
        {
          timevar_ঠেলো (tv_xml);
          print_xml ();
          timevar_বের_করো (tv_xml);
        }

      /* Output html.  */
      যদি (html_flag)
        {
          timevar_ঠেলো (tv_html);
          print_html ();
          timevar_বের_করো (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  যদি (complaint_status == status_complaint)
    যাও finish;

  /* Lookahead tokens are no longer needed. */
  timevar_ঠেলো (tv_free);
  lalr_free ();
  timevar_বের_করো (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  যদি (generate)
    {
      timevar_ঠেলো (tv_parser);
      output ();
      timevar_বের_করো (tv_parser);
    }

 finish:

  timevar_ঠেলো (tv_free);
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
  timevar_বের_করো (tv_free);

  যদি (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_যোগ);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  যদি (!fixits_empty ())
    {
      যদি (update_flag)
        fixits_run ();
      অন্যথা
        complain (ফাঁকা, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  ফেরত complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
