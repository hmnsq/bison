/* Print information on generated parser, for bison,

   Copyright (C) 1984, 1986, 1989, 2000-2005, 2007, 2009-2015, 2018-2022
   Free Software Foundation, Inc.

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

#অন্তর্ভাব "print.h"

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>
#অন্তর্ভাব <mbswidth.h>

#অন্তর্ভাব "closure.h"
#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "conflicts.h"
#অন্তর্ভাব "counterexample.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lr0.h"
#অন্তর্ভাব "muscle-tab.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "state.h"
#অন্তর্ভাব "symtab.h"
#অন্তর্ভাব "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
স্থির bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

স্থির শূন্য
max_দৈর্ঘ্য (size_t *width, ধ্রুবক অক্ষর *str)
{
  size_t len = mbswidth (str, 0);
  যদি (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

স্থির শূন্য
print_core (FILE *out, ধ্রুবক state *s)
{
  ধ্রুবক item_সূচক *sitems = s->বস্তুসমূহ;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  যদি (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  যদি (!snritems)
    ফেরত;

  fputc ('\n', out);

  rule ধ্রুবক *previous_rule = ফাঁকা;
  জন্য (size_t i = 0; i < snritems; i++)
    {
      item_সংখ্যা *sp1 = ritem + sitems[i];
      rule ধ্রুবক *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      যদি (report_flag & report_lookaheads
          && item_সংখ্যা_is_rule_সংখ্যা (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

স্থির শূন্য
print_transitions (ধ্রুবক state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  জন্য (পূর্ণসংখ্যা i = 0; i < trans->অঙ্ক; i++)
    যদি (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        প্রতীক *sym = symbols[TRANSITION_প্রতীক (trans, i)];
        max_দৈর্ঘ্য (&width, sym->tag);
      }

  /* Nothing to report. */
  যদি (!width)
    ফেরত;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  জন্য (পূর্ণসংখ্যা i = 0; i < trans->অঙ্ক; i++)
    যদি (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        প্রতীক *sym = symbols[TRANSITION_প্রতীক (trans, i)];
        ধ্রুবক অক্ষর *tag = sym->tag;
        ধ্রুবক state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        জন্য (পূর্ণসংখ্যা j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        যদি (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->সংখ্যা);
        অন্যথা
          fprintf (out, _("go to state %d\n"), s1->সংখ্যা);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

স্থির শূন্য
print_errs (FILE *out, ধ্রুবক state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  জন্য (পূর্ণসংখ্যা i = 0; i < errp->অঙ্ক; ++i)
    যদি (errp->symbols[i])
      max_দৈর্ঘ্য (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  যদি (!width)
    ফেরত;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  জন্য (পূর্ণসংখ্যা i = 0; i < errp->অঙ্ক; ++i)
    যদি (errp->symbols[i])
      {
        ধ্রুবক অক্ষর *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        জন্য (পূর্ণসংখ্যা j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

স্থির শূন্য
print_reduction (FILE *out, size_t width,
                 ধ্রুবক অক্ষর *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  জন্য (পূর্ণসংখ্যা j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  যদি (!enabled)
    fputc ('[', out);
  যদি (rule_is_initial (r))
    fprintf (out, _("accept"));
  অন্যথা
    fprintf (out, _("reduce using rule %d (%s)"), r->সংখ্যা,
             r->lhs->প্রতীক->tag);
  যদি (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

স্থির শূন্য
print_reductions (FILE *out, ধ্রুবক state *s)
{
  reductions *reds = s->reductions;
  যদি (reds->অঙ্ক == 0)
    ফেরত;

  rule *default_reduction = ফাঁকা;
  যদি (yydefact[s->সংখ্যা] != 0)
    default_reduction = &rules[yydefact[s->সংখ্যা] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    পূর্ণসংখ্যা i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_প্রতীক (trans, i));
  }
  জন্য (পূর্ণসংখ্যা i = 0; i < s->errs->অঙ্ক; ++i)
    যদি (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->সংখ্যা);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  যদি (default_reduction)
    width = mbswidth (_("$default"), 0);

  যদি (reds->lookaheads)
    জন্য (পূর্ণসংখ্যা i = 0; i < ntokens; i++)
      {
        bool গুনতি = bitset_test (no_reduce_set, i);

        জন্য (পূর্ণসংখ্যা j = 0; j < reds->অঙ্ক; ++j)
          যদি (bitset_test (reds->lookaheads[j], i))
            {
              যদি (! গুনতি)
                {
                  যদি (reds->rules[j] != default_reduction)
                    max_দৈর্ঘ্য (&width, symbols[i]->tag);
                  গুনতি = true;
                }
              অন্যথা
                max_দৈর্ঘ্য (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  যদি (!width)
    ফেরত;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  যদি (reds->lookaheads)
    জন্য (পূর্ণসংখ্যা i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool গুনতি = bitset_test (no_reduce_set, i);
        যদি (গুনতি)
          default_reduction_only = false;

        জন্য (পূর্ণসংখ্যা j = 0; j < reds->অঙ্ক; ++j)
          যদি (bitset_test (reds->lookaheads[j], i))
            {
              যদি (! গুনতি)
                {
                  যদি (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  অন্যথা
                    defaulted = true;
                  গুনতি = true;
                }
              অন্যথা
                {
                  default_reduction_only = false;
                  যদি (defaulted)
                    print_reduction (out, width,
                                     symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (out, width,
                                   symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  যদি (default_reduction)
    {
      অক্ষর *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->অঙ্ক == 1 && rule_is_initial (reds->rules[0])));
      (শূন্য) default_reduction_only;
      মুক্ত (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

স্থির শূন্য
print_actions (FILE *out, ধ্রুবক state *s)
{
  /* Print shifts.  */
  print_transitions (s, out, true);
  print_errs (out, s);
  print_reductions (out, s);
  /* Print gotos.  */
  print_transitions (s, out, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

স্থির শূন্য
print_state (FILE *out, ধ্রুবক state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->সংখ্যা);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  যদি ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  যদি (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      fputc ('\n', out);
      counterexample_report_state (s, out, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

স্থির শূন্য
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  জন্য (পূর্ণসংখ্যা i = 0; i < max_code + 1; ++i)
    যদি (token_translations[i] != undeftoken->content->সংখ্যা)
      {
        ধ্রুবক প্রতীক *sym = symbols[token_translations[i]];
        ধ্রুবক অক্ষর *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        যদি (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        জন্য (rule_সংখ্যা r = 0; r < nrules; r++)
          জন্য (item_সংখ্যা *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            যদি (item_সংখ্যা_as_প্রতীক_সংখ্যা (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                বিরতি;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


স্থির শূন্য
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  জন্য (প্রতীক_সংখ্যা i = ntokens; i < nsyms; i++)
    {
      ধ্রুবক প্রতীক *sym = symbols[i];
      ধ্রুবক অক্ষর *tag = sym->tag;
      bool on_বাম = false;
      bool on_ডান = false;

      জন্য (rule_সংখ্যা r = 0; r < nrules; r++)
        {
          on_বাম |= rules[r].lhs->সংখ্যা == i;
          জন্য (item_সংখ্যা *rhsp = rules[r].rhs; !on_ডান && 0 <= *rhsp; ++rhsp)
            on_ডান |= item_সংখ্যা_as_প্রতীক_সংখ্যা (*rhsp) == i;
          যদি (on_বাম && on_ডান)
            বিরতি;
        }

      fprintf (out, "%4s%s", "", tag);
      যদি (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      যদি (on_বাম)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          জন্য (rule_সংখ্যা r = 0; r < nrules; r++)
            যদি (rules[r].lhs->সংখ্যা == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      যদি (on_ডান)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          জন্য (rule_সংখ্যা r = 0; r < nrules; r++)
            জন্য (item_সংখ্যা *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              যদি (item_সংখ্যা_as_প্রতীক_সংখ্যা (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  বিরতি;
                }
          fputc ('\n', out);
        }
    }
}

শূন্য
print_results (শূন্য)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *out = xfopen (spec_verbose_file, "w");

  reduce_output (out);
  grammar_rules_partial_print (out,
                               _("Rules useless in parser due to conflicts"),
                               rule_useless_in_parser_p);
  conflicts_output (out);

  grammar_rules_print (out);
  print_terminal_symbols (out);
  print_nonterminal_symbols (out);

  /* Storage for print_reductions.  */
  no_reduce_set = bitset_create (ntokens, BITSET_FIXED);
  জন্য (state_সংখ্যা i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
