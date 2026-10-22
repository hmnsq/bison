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
اصلي (صحيح argc, حرف *argv[])
{
  {
    حرف *cp = getenv ("BISON_PROGRAM_NAME");
    إذا (cp)
      argv[0] = cp;
  }

#تعریف_کړه DEPENDS_ON_LIBINTL 1
  سېټ_پروګرام_name (argv[0]);
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
      سېټ_custom_quoting (&quote_quoting_options, "'", "'");
    وإلا
      سېټ_quoting_style (&quote_quoting_options, locale_quoting_style);
  }

  atexit (close_stdout);

  glyphs_init ();
  uniqstrs_new ();
  muscle_init ();
  complain_init ();
  کوډ_scanner_init ();

  getargs (argc, argv);

  إذا (trace_flag)
    fprintf (stderr, "bison (GNU Bison) %s\n", نسخه_2);

  timevar_enabled = trace_flag & trace_وخت;
  timevar_init ();
  timevar_پیل (tv_ټول);

  إذا (trace_flag & trace_bitsets)
    bitset_stats_enable ();

  /* Read the input.  Copy some parts of it to FGUARD, FACTION, FTABLE
     and FATTRS.  In file reader.c.  The other parts are recorded in
     the grammar; see gram.h.  */

  timevar_ټېل (tv_reader);
  reader (grammar_file);
  timevar_راوباسه (tv_reader);

  إذا (complaint_حالت == حالت_complaint)
    اذهب پای;

  /* Find useless nonterminals and productions and reduce the grammar. */
  timevar_ټېل (tv_reduce);
  reduce_grammar ();
  timevar_راوباسه (tv_reduce);

  /* Record other info about the grammar.  In files derives and
     nullable.  */
  timevar_ټېل (tv_sets);
  derives_compute ();
  nullable_compute ();
  timevar_راوباسه (tv_sets);

  /* Compute LR(0) parser states.  See state.h for more info.  */
  timevar_ټېل (tv_lr0);
  generate_حالتونه ();
  timevar_راوباسه (tv_lr0);

  /* Add lookahead sets to parser states.  Except when LALR(1) is
     requested, split states to eliminate LR(1)-relative
     inadequacies.  */
  ielr ();

  /* Find and record any conflicts: places where one token of
     lookahead is not enough to disambiguate the parsing.  In file
     conflicts.  Also resolve s/r conflicts based on precedence
     declarations.  */
  timevar_ټېل (tv_conflicts);
  conflicts_solve ();
  إذا (!muscle_percent_define_flag_if ("lr.keep-unreachable-state"))
    {
      state_شمېره *old_منزل_new = xnmalloc (nstates, حجم *old_منزل_new);
      state_شمېره nstates_old = nstates;
      state_remove_unreachable_حالتونه (old_منزل_new);
      lalr_update_state_numbers (old_منزل_new, nstates_old);
      conflicts_update_state_numbers (old_منزل_new, nstates_old);
      حرر (old_منزل_new);
    }
  إذا (report_flag & report_cex
      || warning_is_enabled (Wcounterexamples))
    counterexample_init ();
  conflicts_print ();
  timevar_راوباسه (tv_conflicts);

  /* Compute the parser tables.  */
  timevar_ټېل (tv_actions);
  tables_generate ();
  timevar_راوباسه (tv_actions);

  grammar_قواعد_useless_report (_("rule useless in parser due to conflicts"));

  print_precedence_warnings ();

  /* Whether to generate output files.  */
  bool generate = !(feature_flag & feature_syntax_only);

  إذا (generate)
    {
      /* Output file names. */
      compute_output_file_نومونه ();

      /* Output the detailed report on the grammar.  */
      إذا (report_flag)
        {
          timevar_ټېل (tv_report);
          print_results ();
          timevar_راوباسه (tv_report);
        }

      /* Output the graph.  */
      إذا (ګراف_flag)
        {
          timevar_ټېل (tv_ګراف);
          print_ګراف ();
          timevar_راوباسه (tv_ګراف);
        }

      /* Output xml.  */
      إذا (html_flag || xml_flag)
        {
          timevar_ټېل (tv_xml);
          print_xml ();
          timevar_راوباسه (tv_xml);
        }

      /* Output html.  */
      إذا (html_flag)
        {
          timevar_ټېل (tv_html);
          print_html ();
          timevar_راوباسه (tv_html);
        }
    }

  /* Stop if there were errors, to avoid trashing previous output
     files.  */
  إذا (complaint_حالت == حالت_complaint)
    اذهب پای;

  /* Lookahead tokens are no longer needed. */
  timevar_ټېل (tv_free);
  lalr_free ();
  timevar_راوباسه (tv_free);

  /* Output the tables and the parser to ftable.  In file output.  */
  إذا (generate)
    {
      timevar_ټېل (tv_parser);
      output ();
      timevar_راوباسه (tv_parser);
    }

 پای:

  timevar_ټېل (tv_free);
  nullable_free ();
  derives_free ();
  tables_free ();
  حالتونه_free ();
  reduce_free ();
  conflicts_free ();
  grammar_free ();
  counterexample_free ();
  output_file_نومونه_free ();

  /* The scanner and parser memory cannot be released right after
     parsing, as it contains things such as user actions, prologue,
     epilogue etc.  */
  gram_scanner_free ();
  parser_free ();

  muscle_free ();
  کوډ_scanner_free ();
  skel_scanner_free ();
  timevar_راوباسه (tv_free);

  إذا (trace_flag & trace_bitsets)
    bitset_stats_dump (stderr);

  /* Stop timing and print the times.  */
  timevar_stop (tv_ټول);
  timevar_print (stderr);

  /* Fix input file now, even if there are errors: that's less
     warnings in the following runs.  */
  إذا (!fixits_تشه ())
    {
      إذا (update_flag)
        fixits_پرلپسې ();
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
