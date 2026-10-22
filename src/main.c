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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <bitset/stats.h>
#ضمن <closeout.h>
#ضمن <configmake.h>
#ضمن <progname.h>
#ضمن <quote.h>
#ضمن <quotearg.h>
#ضمن <relocatable.h> /* relocate2 */
#ضمن <timevar.h>

#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "counterexample.h"
#ضمن "derives.h"
#ضمن "files.h"
#ضمن "fixits.h"
#ضمن "getargs.h"
#ضمن "glyphs.h"
#ضمن "gram.h"
#ضمن "ielr.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "muscle-tab.h"
#ضمن "nullable.h"
#ضمن "output.h"
#ضمن "parse-gram.h"
#ضمن "print-graph.h"
#ضمن "print-xml.h"
#ضمن "print.h"
#ضمن "reader.h"
#ضمن "reduce.h"
#ضمن "scan-code.h"
#ضمن "scan-gram.h"
#ضمن "scan-skel.h"
#ضمن "symtab.h"
#ضمن "tables.h"
#ضمن "uniqstr.h"


صحيح
مکيه (صحيح argc, حرف *argv[])
{
  {
    حرف *cp = getenv ("BISON_PROGRAM_NAME");
    إذا (cp)
      argv[0] = cp;
  }

#وصف_ڪريو DEPENDS_ON_LIBINTL 1
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  {
    حرف *cp = عدم;
    حرف ثابت *localedir = relocate2 (LOCALEDIR, &cp);
    bindtextdomain ("bison", localedir);
    bindtextdomain ("bison-gnulib", localedir);
    bindtextdomain ("bison-runtime", localedir);
    حرر (cp);
  }
  textdomain ("bison");

  {
    حرف ثابت *cp = getenv ("LC_CTYPE");
    إذا (cp && STREQ (cp, "C"))
      سيٽ_custom_quoting (&quote_quoting_options, "'", "'");
    وإلا
      سيٽ_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  ڪوڊ_scanner_init ();

  getargs (argc, argv);

  إذا (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", نسخو_2);

  timevar_enabled = trace_flag & trace_وقت;
  timevar_init ();
  timevar_شروعات (tv_ڪل);

  إذا (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ڌڪو (tv_reader);
  reader (grammar_file);
  timevar_ڪڍو (tv_reader);

  إذا (complaint_حالت == حالت_complaint)
    اذهب پڄاڻي;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_ڌڪو (tv_reduce);
  reduce_grammar ();
  timevar_ڪڍو (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_ڌڪو (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_ڪڍو (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_ڌڪو (tv_lr0);
  generate_حالتون ();
  timevar_ڪڍو (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_ڌڪو (tv_conflicts);
  conflicts_solve ();
  إذا (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_عدد *old_منزل_new = xnmalloc (nstates, حجم *old_منزل_new);
      state_عدد nstates_old = nstates;
      state_remove_unreachable_حالتون (old_منزل_new);
      lalr_update_state_numbers (old_منزل_new, nstates_old);
      conflicts_update_state_numbers (old_منزل_new, nstates_old);
      حرر (old_منزل_new);
    }
  إذا (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_ڪڍو (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ڌڪو (tv_actions);
  tables_generate ();
  timevar_ڪڍو (tv_actions);

  grammar_قاعدا_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  إذا (generate)
    {
      /* Output file names. */
      compute_output_file_نالا ();

      /* Output the detailed report on the grammar.  */
      إذا (report_flag)
        {
          timevar_ڌڪو (tv_report);
          print_results ();
          timevar_ڪڍو (tv_report);
        }

      /* Output the graph.  */
      إذا (گراف_flag)
        {
          timevar_ڌڪو (tv_گراف);
          print_گراف ();
          timevar_ڪڍو (tv_گراف);
        }

      /* Output xml.  */
      إذا (html_flag || xml_flag)
        {
          timevar_ڌڪو (tv_xml);
          print_xml ();
          timevar_ڪڍو (tv_xml);
        }

      /* Output html.  */
      إذا (html_flag)
        {
          timevar_ڌڪو (tv_html);
          print_html ();
          timevar_ڪڍو (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  إذا (complaint_حالت == حالت_complaint)
    اذهب پڄاڻي;

  /* Lookahead tokens are no longer needed. */
  timevar_ڌڪو (tv_free);
  lalr_free ();
  timevar_ڪڍو (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  إذا (generate)
    {
      timevar_ڌڪو (tv_parser);
      output ();
      timevar_ڪڍو (tv_parser);
    }

 پڄاڻي:

  timevar_ڌڪو (tv_free);
  nullable_free ();
  derives_free ();
  tables_free ();
  حالتون_free ();
  reduce_free ();
  conflicts_free ();
  grammar_free ();
  counterexample_free ();
  output_file_نالا_free ();

  /* The scanner and parser memory cannot be released right after
     parsing, as it contains things such as user actions, prologue,
     epilogue etc.  */
  gram_scanner_free ();
  parser_free ();

  muscle_free ();
  ڪوڊ_scanner_free ();
  skel_scanner_free ();
  timevar_ڪڍو (tv_free);

  إذا (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_ڪل);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  إذا (!fixits_خالي_جايون ())
    {
      إذا (update_flag)
        fixits_لاڳيتو ();
      وإلا
        complain (عدم, Wother,
                  _("fix-its can be applied.  Rerun with option '--update'."));
      fixits_free ();
    }
  uniqstrs_free ();

  complain_free ();
  quotearg_free ();

  أرجع complaint_حالت ? EXIT_FAILURE : EXIT_SUCCESS;
}
