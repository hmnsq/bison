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

#ضمن <config.h>

#ضمن "print.h"

#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <mbswidth.h>

#ضمن "closure.h"
#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "counterexample.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "muscle-tab.h"
#ضمن "reader.h"
#ضمن "reduce.h"
#ضمن "state.h"
#ضمن "symtab.h"
#ضمن "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
ساكن bitset no_reduce_سېټ;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ساكن فراغ
max_اوږدتیا (size_t *پلنوالی, ثابت حرف *str)
{
  size_t len = mbswidth (str, 0);
  إذا (len > *پلنوالی)
    *پلنوالی = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *بهر, ثابت state *s)
{
  ثابت توکی_شاخص *sitems = s->توکي;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      تړون (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  إذا (!snritems)
    أرجع;

  fputc ('\n', بهر);

  قاعده ثابت *پخوانی_قاعده = عدم;
  لكل (size_t i = 0; i < snritems; i++)
    {
      توکی_شمېره *sp1 = ritem + sitems[i];
      قاعده ثابت *r = توکی_قاعده (sp1);
      توکی_print (sp1, پخوانی_قاعده, بهر);
      پخوانی_قاعده = r;

      /* Display the lookahead tokens?  */
      إذا (report_flag & report_lookaheads
          && توکی_شمېره_is_قاعده_شمېره (*sp1))
        state_قاعده_lookaheads_print (s, r, بهر);
      fputc ('\n', بهر);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ساكن فراغ
print_transitions (ثابت state *s, FILE *بهر, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t پلنوالی = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_ښويدنه (trans, i) == display_transitions_p)
      {
        نښه *sym = symbols[TRANSITION_نښه (trans, i)];
        max_اوږدتیا (&پلنوالی, sym->نښان);
      }

  /* Nothing to report. */
  إذا (!پلنوالی)
    أرجع;

  fputc ('\n', بهر);
  پلنوالی += 2;

  /* Report lookahead tokens and shifts.  */
  لكل (صحيح i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_ښويدنه (trans, i) == display_transitions_p)
      {
        نښه *sym = symbols[TRANSITION_نښه (trans, i)];
        ثابت حرف *نښان = sym->نښان;
        ثابت state *s1 = trans->حالتونه[i];

        fprintf (بهر, "    %s", نښان);
        لكل (صحيح j = پلنوالی - mbswidth (نښان, 0); j > 0; --j)
          fputc (' ', بهر);
        إذا (display_transitions_p)
          fprintf (بهر, _("shift, and go to state %d\n"), s1->شمېره);
        وإلا
          fprintf (بهر, _("go to state %d\n"), s1->شمېره);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *بهر, ثابت state *s)
{
  errs *errp = s->errs;
  size_t پلنوالی = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < errp->عدد; ++i)
    إذا (errp->symbols[i])
      max_اوږدتیا (&پلنوالی, errp->symbols[i]->نښان);

  /* Nothing to report. */
  إذا (!پلنوالی)
    أرجع;

  fputc ('\n', بهر);
  پلنوالی += 2;

  /* Report lookahead tokens and errors.  */
  لكل (صحيح i = 0; i < errp->عدد; ++i)
    إذا (errp->symbols[i])
      {
        ثابت حرف *نښان = errp->symbols[i]->نښان;
        fprintf (بهر, "    %s", نښان);
        لكل (صحيح j = پلنوالی - mbswidth (نښان, 0); j > 0; --j)
          fputc (' ', بهر);
        fputs (_("error (nonassociative)\n"), بهر);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
print_reduction (FILE *بهر, size_t پلنوالی,
                 ثابت حرف *lookahead,
                 قاعده *r, bool enabled)
{
  fprintf (بهر, "    %s", lookahead);
  لكل (صحيح j = پلنوالی - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', بهر);
  إذا (!enabled)
    fputc ('[', بهر);
  إذا (قاعده_is_initial (r))
    fprintf (بهر, _("accept"));
  وإلا
    fprintf (بهر, _("reduce using rule %d (%s)"), r->شمېره,
             r->lhs->نښه->نښان);
  إذا (!enabled)
    fputc (']', بهر);
  fputc ('\n', بهر);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *بهر, ثابت state *s)
{
  reductions *reds = s->reductions;
  إذا (reds->عدد == 0)
    أرجع;

  قاعده *default_reduction = عدم;
  إذا (yydefact[s->شمېره] != 0)
    default_reduction = &قواعد[yydefact[s->شمېره] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_سېټ);
  {
    صحيح i;
    FOR_EACH_ښويدنه (trans, i)
      bitset_سېټ (no_reduce_سېټ, TRANSITION_نښه (trans, i));
  }
  لكل (صحيح i = 0; i < s->errs->عدد; ++i)
    إذا (s->errs->symbols[i])
      bitset_سېټ (no_reduce_سېټ, s->errs->symbols[i]->content->شمېره);

  /* Compute the width of the lookahead token column.  */
  size_t پلنوالی = 0;
  إذا (default_reduction)
    پلنوالی = mbswidth (_("$default"), 0);

  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool شمېر = bitset_test (no_reduce_سېټ, i);

        لكل (صحيح j = 0; j < reds->عدد; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! شمېر)
                {
                  إذا (reds->قواعد[j] != default_reduction)
                    max_اوږدتیا (&پلنوالی, symbols[i]->نښان);
                  شمېر = true;
                }
              وإلا
                max_اوږدتیا (&پلنوالی, symbols[i]->نښان);
            }
      }

  /* Nothing to report. */
  إذا (!پلنوالی)
    أرجع;

  fputc ('\n', بهر);
  پلنوالی += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool شمېر = bitset_test (no_reduce_سېټ, i);
        إذا (شمېر)
          default_reduction_only = false;

        لكل (صحيح j = 0; j < reds->عدد; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! شمېر)
                {
                  إذا (reds->قواعد[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (بهر, پلنوالی,
                                       symbols[i]->نښان,
                                       reds->قواعد[j], true);
                    }
                  وإلا
                    defaulted = true;
                  شمېر = true;
                }
              وإلا
                {
                  default_reduction_only = false;
                  إذا (defaulted)
                    print_reduction (بهر, پلنوالی,
                                     symbols[i]->نښان,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (بهر, پلنوالی,
                                   symbols[i]->نښان,
                                   reds->قواعد[j], false);
                }
            }
      }

  إذا (default_reduction)
    {
      حرف *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (بهر, پلنوالی, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->عدد == 1 && قاعده_is_initial (reds->قواعد[0])));
      (فراغ) default_reduction_only;
      حرر (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *بهر, ثابت state *s)
{
  /* Print shifts.  */
  print_transitions (s, بهر, true);
  print_errs (بهر, s);
  print_reductions (بهر, s);
  /* Print gotos.  */
  print_transitions (s, بهر, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ساكن فراغ
print_state (FILE *بهر, ثابت state *s)
{
  fputs ("\n\n", بهر);
  fprintf (بهر, _("State %d"), s->شمېره);
  fputc ('\n', بهر);
  print_core (بهر, s);
  print_actions (بهر, s);
  إذا ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', بهر);
      fputs (s->solved_conflicts, بهر);
    }
  إذا (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      fputc ('\n', بهر);
      counterexample_report_state (s, بهر, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ساكن فراغ
print_terminal_symbols (FILE *بهر)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (بهر, "%s\n\n", _("Terminals, with rules where they appear"));
  لكل (صحيح i = 0; i < max_کوډ + 1; ++i)
    إذا (token_translations[i] != undeftoken->content->شمېره)
      {
        ثابت نښه *sym = symbols[token_translations[i]];
        ثابت حرف *نښان = sym->نښان;
        fprintf (بهر, "%4s%s", "", نښان);
        إذا (sym->content->type_name)
          fprintf (بهر, " <%s>", sym->content->type_name);
        fprintf (بهر, " (%d)", i);

        لكل (قاعده_شمېره r = 0; r < nrules; r++)
          لكل (توکی_شمېره *rhsp = قواعد[r].rhs; *rhsp >= 0; rhsp++)
            إذا (توکی_شمېره_as_نښه_شمېره (*rhsp) == token_translations[i])
              {
                fprintf (بهر, " %d", r);
                اكسر;
              }
        fputc ('\n', بهر);
      }
  fputs ("\n\n", بهر);
}


ساكن فراغ
print_nonterminal_symbols (FILE *بهر)
{
  fprintf (بهر, "%s\n\n", _("Nonterminals, with rules where they appear"));
  لكل (نښه_شمېره i = ntokens; i < nsyms; i++)
    {
      ثابت نښه *sym = symbols[i];
      ثابت حرف *نښان = sym->نښان;
      bool on_کیڼ = false;
      bool on_ښی = false;

      لكل (قاعده_شمېره r = 0; r < nrules; r++)
        {
          on_کیڼ |= قواعد[r].lhs->شمېره == i;
          لكل (توکی_شمېره *rhsp = قواعد[r].rhs; !on_ښی && 0 <= *rhsp; ++rhsp)
            on_ښی |= توکی_شمېره_as_نښه_شمېره (*rhsp) == i;
          إذا (on_کیڼ && on_ښی)
            اكسر;
        }

      fprintf (بهر, "%4s%s", "", نښان);
      إذا (sym->content->type_name)
        fprintf (بهر, " <%s>", sym->content->type_name);
      fprintf (بهر, " (%d)\n", i);

      إذا (on_کیڼ)
        {
          fprintf (بهر, "%8s%s", "", _("on left:"));
          لكل (قاعده_شمېره r = 0; r < nrules; r++)
            إذا (قواعد[r].lhs->شمېره == i)
              fprintf (بهر, " %d", r);
          fputc ('\n', بهر);
        }

      إذا (on_ښی)
        {
          fprintf (بهر, "%8s%s", "", _("on right:"));
          لكل (قاعده_شمېره r = 0; r < nrules; r++)
            لكل (توکی_شمېره *rhsp = قواعد[r].rhs; 0 <= *rhsp; ++rhsp)
              إذا (توکی_شمېره_as_نښه_شمېره (*rhsp) == i)
                {
                  fprintf (بهر, " %d", r);
                  اكسر;
                }
          fputc ('\n', بهر);
        }
    }
}

فراغ
print_results (فراغ)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *بهر = xfopen (spec_verbose_file, "w");

  reduce_output (بهر);
  grammar_قواعد_partial_print (بهر,
                               _("Rules useless in parser due to conflicts"),
                               قاعده_useless_دننه_parser_p);
  conflicts_output (بهر);

  grammar_قواعد_print (بهر);
  print_terminal_symbols (بهر);
  print_nonterminal_symbols (بهر);

  /* Storage for print_reductions.  */
  no_reduce_سېټ = bitset_create (ntokens, BITSET_FIXED);
  لكل (state_شمېره i = 0; i < nstates; i++)
    print_state (بهر, حالتونه[i]);
  bitset_free (no_reduce_سېټ);

  xfclose (بهر);
}
