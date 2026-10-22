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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>
#അന്തര്ഭാവ <bitset/stats.h>
#അന്തര്ഭാവ <closeout.h>
#അന്തര്ഭാവ <configmake.h>
#അന്തര്ഭാവ <progname.h>
#അന്തര്ഭാവ <quote.h>
#അന്തര്ഭാവ <quotearg.h>
#അന്തര്ഭാവ <relocatable.h> /* relocate2 */
#അന്തര്ഭാവ <timevar.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "counterexample.h"
#അന്തര്ഭാവ "derives.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "fixits.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "glyphs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "ielr.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "nullable.h"
#അന്തര്ഭാവ "output.h"
#അന്തര്ഭാവ "parse-gram.h"
#അന്തര്ഭാവ "print-graph.h"
#അന്തര്ഭാവ "print-xml.h"
#അന്തര്ഭാവ "print.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "scan-code.h"
#അന്തര്ഭാവ "scan-gram.h"
#അന്തര്ഭാവ "scan-skel.h"
#അന്തര്ഭാവ "symtab.h"
#അന്തര്ഭാവ "tables.h"
#അന്തര്ഭാവ "uniqstr.h"


പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[])
{
  {
    അക്ഷരം *cp = getenv ("BISON_PROGRAM_NAME");
    എങ്കിൽ (cp)
      argv[0] = cp;
  }

#define DEPENDS_ON_LIBINTL 1
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    അക്ഷരം *cp = ശൂന്യ;
    അക്ഷരം സ്ഥിരം *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    സ്വതന്ത്രം (cp);
  }
  textdomain ("bison");

  {
    അക്ഷരം സ്ഥിരം *cp = getenv ("LC_CTYPE");
    എങ്കിൽ (cp && STREQ (cp, "C"))
      set_custom_quoting (&quote_quoting_options, "'", "'");
    അല്ലെങ്കിൽ
      set_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  code_scanner_init ();

  getargs (argc, argv);

  എങ്കിൽ (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", VERSION);

  timevar_enabled = trace_flag & trace_time;
  timevar_init ();
  timevar_start (tv_total);

  എങ്കിൽ (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_തള്ളുക (tv_reader);
  reader (grammar_file);
  timevar_എടുക്കുക (tv_reader);

  എങ്കിൽ (complaint_status == status_complaint)
    പോകുക finish;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_തള്ളുക (tv_reduce);
  reduce_grammar ();
  timevar_എടുക്കുക (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_തള്ളുക (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_എടുക്കുക (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_തള്ളുക (tv_lr0);
  generate_states ();
  timevar_എടുക്കുക (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_തള്ളുക (tv_conflicts);
  conflicts_solve ();
  എങ്കിൽ (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_സംഖ്യ *old_to_new = xnmalloc (nstates, വലുപ്പം *old_to_new);
      state_സംഖ്യ nstates_old = nstates;
      state_remove_unreachable_states (old_to_new);
      lalr_update_state_numbers (old_to_new, nstates_old);
      conflicts_update_state_numbers (old_to_new, nstates_old);
      സ്വതന്ത്രം (old_to_new);
    }
  എങ്കിൽ (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_എടുക്കുക (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_തള്ളുക (tv_actions);
  tables_generate ();
  timevar_എടുക്കുക (tv_actions);

  grammar_rules_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  എങ്കിൽ (generate)
    {
      /* Output file names. */
      compute_output_file_names ();

      /* Output the detailed report on the grammar.  */
      എങ്കിൽ (report_flag)
        {
          timevar_തള്ളുക (tv_report);
          print_results ();
          timevar_എടുക്കുക (tv_report);
        }

      /* Output the graph.  */
      എങ്കിൽ (graph_flag)
        {
          timevar_തള്ളുക (tv_graph);
          print_graph ();
          timevar_എടുക്കുക (tv_graph);
        }

      /* Output xml.  */
      എങ്കിൽ (html_flag || xml_flag)
        {
          timevar_തള്ളുക (tv_xml);
          print_xml ();
          timevar_എടുക്കുക (tv_xml);
        }

      /* Output html.  */
      എങ്കിൽ (html_flag)
        {
          timevar_തള്ളുക (tv_html);
          print_html ();
          timevar_എടുക്കുക (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  എങ്കിൽ (complaint_status == status_complaint)
    പോകുക finish;

  /* Lookahead tokens are no longer needed. */
  timevar_തള്ളുക (tv_free);
  lalr_free ();
  timevar_എടുക്കുക (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  എങ്കിൽ (generate)
    {
      timevar_തള്ളുക (tv_parser);
      output ();
      timevar_എടുക്കുക (tv_parser);
    }

 finish:

  timevar_തള്ളുക (tv_free);
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
  timevar_എടുക്കുക (tv_free);

  എങ്കിൽ (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_total);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  എങ്കിൽ (!fixits_empty ())
    {
      എങ്കിൽ (update_flag)
        fixits_run ();
      അല്ലെങ്കിൽ
        complain (ശൂന്യ, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  തിരികെ complaint_status ? EXIT_FAILURE : EXIT_SUCCESS;
}
