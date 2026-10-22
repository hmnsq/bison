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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>
#અન્તર્ભાવ <bitset/stats.h>
#અન્તર્ભાવ <closeout.h>
#અન્તર્ભાવ <configmake.h>
#અન્તર્ભાવ <progname.h>
#અન્તર્ભાવ <quote.h>
#અન્તર્ભાવ <quotearg.h>
#અન્તર્ભાવ <relocatable.h> /* relocate2 */
#અન્તર્ભાવ <timevar.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "counterexample.h"
#અન્તર્ભાવ "derives.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "fixits.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "glyphs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "ielr.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "nullable.h"
#અન્તર્ભાવ "output.h"
#અન્તર્ભાવ "parse-gram.h"
#અન્તર્ભાવ "print-graph.h"
#અન્તર્ભાવ "print-xml.h"
#અન્તર્ભાવ "print.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "scan-code.h"
#અન્તર્ભાવ "scan-gram.h"
#અન્તર્ભાવ "scan-skel.h"
#અન્તર્ભાવ "symtab.h"
#અન્તર્ભાવ "tables.h"
#અન્તર્ભાવ "uniqstr.h"


પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર *argv[])
{
  {
    અક્ષર *cp = getenv ("BISON_PROGRAM_NAME");
    જો (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    અક્ષર *cp = ખાલી;
    અક્ષર અચલ *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    મુક્ત (cp);
  }
  textdomain ("bison");

  {
    અક્ષર અચલ *cp = getenv ("LC_CTYPE");
    જો (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    નહીંતર
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  જો (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  જો (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ધકેલો (tv_reader);
  reader (grammar_file);
  timevar_કાઢો (tv_reader);

  જો (complaint_status == status_complaint)
    જાઓ finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_ધકેલો (tv_reduce);
  reduce_grammar ();
  timevar_કાઢો (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_ધકેલો (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_કાઢો (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_ધકેલો (tv_lr0);
  generate_states ();
  timevar_કાઢો (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_ધકેલો (tv_conflicts);
  conflicts_solve ();
  જો (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_સંખ્યા *old_to_new = xnmalloc (nstates, કદ *old_to_new);
      state_સંખ્યા nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      મુક્ત (old_to_new);
    }
  જો (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_કાઢો (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ધકેલો (tv_actions);
  tables_generate ();
  timevar_કાઢો (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  જો (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      જો (report_flag)
        {
          timevar_ધકેલો (tv_report);
          print_results ();
          timevar_કાઢો (tv_report);
        }

      /* Output the graph.  */
      જો (graph_flag)
        {
          timevar_ધકેલો (tv_graph);
          print_graph ();
          timevar_કાઢો (tv_graph);
        }

      /* Output xml.  */
      જો (html_flag || xml_flag)
        {
          timevar_ધકેલો (tv_xml);
          print_xml ();
          timevar_કાઢો (tv_xml);
        }

      /* Output html.  */
      જો (html_flag)
        {
          timevar_ધકેલો (tv_html);
          print_html ();
          timevar_કાઢો (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  જો (complaint_status == status_complaint)
    જાઓ finish;

  /* Lookahead tokens are no longer needed. */
  timevar_ધકેલો (tv_free);
  lalr_free ();
  timevar_કાઢો (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  જો (generate)
    {
      timevar_ધકેલો (tv_parser);
      output ();
      timevar_કાઢો (tv_parser);
    }

 finish:

  timevar_ધકેલો (tv_free);
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
  timevar_કાઢો (tv_free);

  જો (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  જો (!fixits_empty ())
    {
      જો (update_flag)
        fixits_run ();
      નહીંતર
        complain (ખાલી, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  પાછા complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
