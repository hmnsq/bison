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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <bitset/stats.h>
#அந்தர்பாவ <closeout.h>
#அந்தர்பாவ <configmake.h>
#அந்தர்பாவ <progname.h>
#அந்தர்பாவ <quote.h>
#அந்தர்பாவ <quotearg.h>
#அந்தர்பாவ <relocatable.h> /* relocate2 */
#அந்தர்பாவ <timevar.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "counterexample.h"
#அந்தர்பாவ "derives.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "fixits.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "glyphs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "ielr.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "nullable.h"
#அந்தர்பாவ "output.h"
#அந்தர்பாவ "parse-gram.h"
#அந்தர்பாவ "print-graph.h"
#அந்தர்பாவ "print-xml.h"
#அந்தர்பாவ "print.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "scan-code.h"
#அந்தர்பாவ "scan-gram.h"
#அந்தர்பாவ "scan-skel.h"
#அந்தர்பாவ "symtab.h"
#அந்தர்பாவ "tables.h"
#அந்தர்பாவ "uniqstr.h"


முழு
முதன்மை (முழு argc, எழுத்து *argv[])
{
  {
    எழுத்து *cp = getenv ("BISON_PROGRAM_NAME");
    எனில் (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    எழுத்து *cp = வெற்று;
    எழுத்து மாறிலி *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    விடுவி (cp);
  }
  textdomain ("bison");

  {
    எழுத்து மாறிலி *cp = getenv ("LC_CTYPE");
    எனில் (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    இல்லையெனில்
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  எனில் (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  எனில் (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_தள்ளு (tv_reader);
  reader (grammar_file);
  timevar_எடு (tv_reader);

  எனில் (complaint_status == status_complaint)
    செல் finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_தள்ளு (tv_reduce);
  reduce_grammar ();
  timevar_எடு (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_தள்ளு (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_எடு (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_தள்ளு (tv_lr0);
  generate_states ();
  timevar_எடு (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_தள்ளு (tv_conflicts);
  conflicts_solve ();
  எனில் (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_எண் *old_to_new = xnmalloc (nstates, அளவு *old_to_new);
      state_எண் nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      விடுவி (old_to_new);
    }
  எனில் (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_எடு (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_தள்ளு (tv_actions);
  tables_generate ();
  timevar_எடு (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  எனில் (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      எனில் (report_flag)
        {
          timevar_தள்ளு (tv_report);
          print_results ();
          timevar_எடு (tv_report);
        }

      /* Output the graph.  */
      எனில் (graph_flag)
        {
          timevar_தள்ளு (tv_graph);
          print_graph ();
          timevar_எடு (tv_graph);
        }

      /* Output xml.  */
      எனில் (html_flag || xml_flag)
        {
          timevar_தள்ளு (tv_xml);
          print_xml ();
          timevar_எடு (tv_xml);
        }

      /* Output html.  */
      எனில் (html_flag)
        {
          timevar_தள்ளு (tv_html);
          print_html ();
          timevar_எடு (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  எனில் (complaint_status == status_complaint)
    செல் finish;

  /* Lookahead tokens are no longer needed. */
  timevar_தள்ளு (tv_free);
  lalr_free ();
  timevar_எடு (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  எனில் (generate)
    {
      timevar_தள்ளு (tv_parser);
      output ();
      timevar_எடு (tv_parser);
    }

 finish:

  timevar_தள்ளு (tv_free);
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
  timevar_எடு (tv_free);

  எனில் (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  எனில் (!fixits_empty ())
    {
      எனில் (update_flag)
        fixits_run ();
      இல்லையெனில்
        complain (வெற்று, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  திரும்பு complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
