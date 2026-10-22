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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>
#ଅନ୍ତର୍ଭାଵ <bitset/stats.h>
#ଅନ୍ତର୍ଭାଵ <closeout.h>
#ଅନ୍ତର୍ଭାଵ <configmake.h>
#ଅନ୍ତର୍ଭାଵ <progname.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>
#ଅନ୍ତର୍ଭାଵ <quotearg.h>
#ଅନ୍ତର୍ଭାଵ <relocatable.h> /* relocate2 */
#ଅନ୍ତର୍ଭାଵ <timevar.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "conflicts.h"
#ଅନ୍ତର୍ଭାଵ "counterexample.h"
#ଅନ୍ତର୍ଭାଵ "derives.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "fixits.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "glyphs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "ielr.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"
#ଅନ୍ତର୍ଭାଵ "output.h"
#ଅନ୍ତର୍ଭାଵ "parse-gram.h"
#ଅନ୍ତର୍ଭାଵ "print-graph.h"
#ଅନ୍ତର୍ଭାଵ "print-xml.h"
#ଅନ୍ତର୍ଭାଵ "print.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "scan-code.h"
#ଅନ୍ତର୍ଭାଵ "scan-gram.h"
#ଅନ୍ତର୍ଭାଵ "scan-skel.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"
#ଅନ୍ତର୍ଭାଵ "tables.h"
#ଅନ୍ତର୍ଭାଵ "uniqstr.h"


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
  {
    ଅକ୍ଷର *cp = getenv ("BISON_PROGRAM_NAME");
    ଯଦି (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    ଅକ୍ଷର *cp = ଖାଲି;
    ଅକ୍ଷର ଅଚଳ *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    ମୁକ୍ତ (cp);
  }
  textdomain ("bison");

  {
    ଅକ୍ଷର ଅଚଳ *cp = getenv ("LC_CTYPE");
    ଯଦି (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    ଅନ୍ୟଥା
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  ଯଦି (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  ଯଦି (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ଠେଲ (tv_reader);
  reader (grammar_file);
  timevar_କାଢ଼ (tv_reader);

  ଯଦି (complaint_status == status_complaint)
    ଯାଅ finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_ଠେଲ (tv_reduce);
  reduce_grammar ();
  timevar_କାଢ଼ (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_ଠେଲ (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_କାଢ଼ (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_ଠେଲ (tv_lr0);
  generate_states ();
  timevar_କାଢ଼ (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_ଠେଲ (tv_conflicts);
  conflicts_solve ();
  ଯଦି (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_ସଂଖ୍ୟା *old_to_new = xnmalloc (nstates, ଆକାର *old_to_new);
      state_ସଂଖ୍ୟା nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      ମୁକ୍ତ (old_to_new);
    }
  ଯଦି (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_କାଢ଼ (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ଠେଲ (tv_actions);
  tables_generate ();
  timevar_କାଢ଼ (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  ଯଦି (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      ଯଦି (report_flag)
        {
          timevar_ଠେଲ (tv_report);
          print_results ();
          timevar_କାଢ଼ (tv_report);
        }

      /* Output the graph.  */
      ଯଦି (graph_flag)
        {
          timevar_ଠେଲ (tv_graph);
          print_graph ();
          timevar_କାଢ଼ (tv_graph);
        }

      /* Output xml.  */
      ଯଦି (html_flag || xml_flag)
        {
          timevar_ଠେଲ (tv_xml);
          print_xml ();
          timevar_କାଢ଼ (tv_xml);
        }

      /* Output html.  */
      ଯଦି (html_flag)
        {
          timevar_ଠେଲ (tv_html);
          print_html ();
          timevar_କାଢ଼ (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  ଯଦି (complaint_status == status_complaint)
    ଯାଅ finish;

  /* Lookahead tokens are no longer needed. */
  timevar_ଠେଲ (tv_free);
  lalr_free ();
  timevar_କାଢ଼ (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  ଯଦି (generate)
    {
      timevar_ଠେଲ (tv_parser);
      output ();
      timevar_କାଢ଼ (tv_parser);
    }

 finish:

  timevar_ଠେଲ (tv_free);
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
  timevar_କାଢ଼ (tv_free);

  ଯଦି (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  ଯଦି (!fixits_empty ())
    {
      ଯଦି (update_flag)
        fixits_run ();
      ଅନ୍ୟଥା
        complain (ଖାଲି, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  ଫେରନ୍ତୁ complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
