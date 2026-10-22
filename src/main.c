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


ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
  {
    ಅಕ್ಷರ *cp = getenv ("BISON_PROGRAM_NAME");
    ಆದರೆ (cp)
      argv[0] = cp;
  }

#ವ್ಯಾಖ್ಯಾನಿಸು DEPENDS_ON_LIBINTL 1
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    ಅಕ್ಷರ *cp = ಖಾಲಿ;
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    ಬಿಡುಗಡೆ (cp);
  }
  textdomain ("bison");

  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp = getenv ("LC_CTYPE");
    ಆದರೆ (cp && STREQ (cp, "C"))
      ಗಣ_custom_quoting (&quote_quoting_options, "'", "'");
    ಇಲ್ಲದಿದ್ದರೆ
      ಗಣ_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  ಸಂಕೇತ_scanner_init ();

  getargs (argc, argv);

  ಆದರೆ (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", ಆವೃತ್ತಿ_2);

  timevar_enabled = trace_flag & trace_ಸಮಯ;
  timevar_init ();
  timevar_ಆರಂಭ (tv_ಒಟ್ಟು);

  ಆದರೆ (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ತಳ್ಳು (tv_reader);
  reader (grammar_file);
  timevar_ತೆಗೆ (tv_reader);

  ಆದರೆ (complaint_ಸ್ಥಿತಿ == ಸ್ಥಿತಿ_complaint)
    ಹೋಗು ಮುಕ್ತಾಯ;

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
  generate_ಸ್ಥಿತಿಗಳು ();
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
  ಆದರೆ (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_ಸಂಖ್ಯೆ *old_ಗಮ್ಯ_new = xnmalloc (nstates, ಗಾತ್ರ *old_ಗಮ್ಯ_new);
      state_ಸಂಖ್ಯೆ nstates_old = nstates;
      state_remove_unreachable_ಸ್ಥಿತಿಗಳು (old_ಗಮ್ಯ_new);
      lalr_update_state_numbers (old_ಗಮ್ಯ_new, nstates_old);
      conflicts_update_state_numbers (old_ಗಮ್ಯ_new, nstates_old);
      ಬಿಡುಗಡೆ (old_ಗಮ್ಯ_new);
    }
  ಆದರೆ (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_ತೆಗೆ (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ತಳ್ಳು (tv_actions);
  tables_generate ();
  timevar_ತೆಗೆ (tv_actions);

  grammar_ನಿಯಮಗಳು_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  ಆದರೆ (generate)
    {
      /* Output file names. */
      compute_output_file_ಹೆಸರುಗಳು ();

      /* Output the detailed report on the grammar.  */
      ಆದರೆ (report_flag)
        {
          timevar_ತಳ್ಳು (tv_report);
          print_results ();
          timevar_ತೆಗೆ (tv_report);
        }

      /* Output the graph.  */
      ಆದರೆ (ನಕ್ಷೆ_flag)
        {
          timevar_ತಳ್ಳು (tv_ನಕ್ಷೆ);
          print_ನಕ್ಷೆ ();
          timevar_ತೆಗೆ (tv_ನಕ್ಷೆ);
        }

      /* Output xml.  */
      ಆದರೆ (html_flag || xml_flag)
        {
          timevar_ತಳ್ಳು (tv_xml);
          print_xml ();
          timevar_ತೆಗೆ (tv_xml);
        }

      /* Output html.  */
      ಆದರೆ (html_flag)
        {
          timevar_ತಳ್ಳು (tv_html);
          print_html ();
          timevar_ತೆಗೆ (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  ಆದರೆ (complaint_ಸ್ಥಿತಿ == ಸ್ಥಿತಿ_complaint)
    ಹೋಗು ಮುಕ್ತಾಯ;

  /* Lookahead tokens are no longer needed. */
  timevar_ತಳ್ಳು (tv_free);
  lalr_free ();
  timevar_ತೆಗೆ (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  ಆದರೆ (generate)
    {
      timevar_ತಳ್ಳು (tv_parser);
      output ();
      timevar_ತೆಗೆ (tv_parser);
    }

 ಮುಕ್ತಾಯ:

  timevar_ತಳ್ಳು (tv_free);
  nullable_free ();
  derives_free ();
  tables_free ();
  ಸ್ಥಿತಿಗಳು_free ();
  reduce_free ();
  conflicts_free ();
  grammar_free ();
  counterexample_free ();
  output_file_ಹೆಸರುಗಳು_free ();

  /* The scanner and parser memory cannot be released right after
     parsing, as it contains things such as user actions, prologue,
     epilogue etc.  */
  gram_scanner_free ();
  parser_free ();

  muscle_free ();
  ಸಂಕೇತ_scanner_free ();
  skel_scanner_free ();
  timevar_ತೆಗೆ (tv_free);

  ಆದರೆ (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_ಒಟ್ಟು);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  ಆದರೆ (!fixits_ಖಾಲಿಗಳು ())
    {
      ಆದರೆ (update_flag)
        fixits_ಸತತ ();
      ಇಲ್ಲದಿದ್ದರೆ
        complain (ಖಾಲಿ, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  ಹಿಂತಿರುಗಿ complaint_ಸ್ಥಿತಿ ? EXIT_FAILURE : EXIT_SUCCESS;
}
