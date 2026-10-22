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
ساكن bitset no_reduce_سیٹ;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ساكن فراغ
max_لمائی (size_t *چوڑائی, ثابت حرف *str)
{
  size_t len = mbswidth (str, 0);
  إذا (len > *چوڑائی)
    *چوڑائی = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *باہرلا, ثابت state *s)
{
  ثابت شے_اشاریہ *sitems = s->اشیاء;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      گھیرا (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  إذا (!snritems)
    أرجع;

  fputc ('\n', باہرلا);

  قاعدہ ثابت *پچھلا_قاعدہ = عدم;
  لكل (size_t i = 0; i < snritems; i++)
    {
      شے_نمبر *sp1 = ritem + sitems[i];
      قاعدہ ثابت *r = شے_قاعدہ (sp1);
      شے_print (sp1, پچھلا_قاعدہ, باہرلا);
      پچھلا_قاعدہ = r;

      /* Display the lookahead tokens?  */
      إذا (report_flag & report_lookaheads
          && شے_نمبر_is_قاعدہ_نمبر (*sp1))
        state_قاعدہ_lookaheads_print (s, r, باہرلا);
      fputc ('\n', باہرلا);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ساكن فراغ
print_transitions (ثابت state *s, FILE *باہرلا, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t چوڑائی = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_کھسکاؤ (trans, i) == display_transitions_p)
      {
        علامت *sym = symbols[TRANSITION_علامت (trans, i)];
        max_لمائی (&چوڑائی, sym->ٹیگ);
      }

  /* Nothing to report. */
  إذا (!چوڑائی)
    أرجع;

  fputc ('\n', باہرلا);
  چوڑائی += 2;

  /* Report lookahead tokens and shifts.  */
  لكل (صحيح i = 0; i < trans->عدد; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_کھسکاؤ (trans, i) == display_transitions_p)
      {
        علامت *sym = symbols[TRANSITION_علامت (trans, i)];
        ثابت حرف *ٹیگ = sym->ٹیگ;
        ثابت state *s1 = trans->حالتاں[i];

        fprintf (باہرلا, "    %s", ٹیگ);
        لكل (صحيح j = چوڑائی - mbswidth (ٹیگ, 0); j > 0; --j)
          fputc (' ', باہرلا);
        إذا (display_transitions_p)
          fprintf (باہرلا, _("shift, and go to state %d\n"), s1->نمبر);
        وإلا
          fprintf (باہرلا, _("go to state %d\n"), s1->نمبر);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *باہرلا, ثابت state *s)
{
  errs *errp = s->errs;
  size_t چوڑائی = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < errp->عدد; ++i)
    إذا (errp->symbols[i])
      max_لمائی (&چوڑائی, errp->symbols[i]->ٹیگ);

  /* Nothing to report. */
  إذا (!چوڑائی)
    أرجع;

  fputc ('\n', باہرلا);
  چوڑائی += 2;

  /* Report lookahead tokens and errors.  */
  لكل (صحيح i = 0; i < errp->عدد; ++i)
    إذا (errp->symbols[i])
      {
        ثابت حرف *ٹیگ = errp->symbols[i]->ٹیگ;
        fprintf (باہرلا, "    %s", ٹیگ);
        لكل (صحيح j = چوڑائی - mbswidth (ٹیگ, 0); j > 0; --j)
          fputc (' ', باہرلا);
        fputs (_("error (nonassociative)\n"), باہرلا);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
print_reduction (FILE *باہرلا, size_t چوڑائی,
                 ثابت حرف *lookahead,
                 قاعدہ *r, bool enabled)
{
  fprintf (باہرلا, "    %s", lookahead);
  لكل (صحيح j = چوڑائی - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', باہرلا);
  إذا (!enabled)
    fputc ('[', باہرلا);
  إذا (قاعدہ_is_initial (r))
    fprintf (باہرلا, _("accept"));
  وإلا
    fprintf (باہرلا, _("reduce using rule %d (%s)"), r->نمبر,
             r->lhs->علامت->ٹیگ);
  إذا (!enabled)
    fputc (']', باہرلا);
  fputc ('\n', باہرلا);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *باہرلا, ثابت state *s)
{
  reductions *reds = s->reductions;
  إذا (reds->عدد == 0)
    أرجع;

  قاعدہ *default_reduction = عدم;
  إذا (yydefact[s->نمبر] != 0)
    default_reduction = &قاعدے[yydefact[s->نمبر] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_سیٹ);
  {
    صحيح i;
    FOR_EACH_کھسکاؤ (trans, i)
      bitset_سیٹ (no_reduce_سیٹ, TRANSITION_علامت (trans, i));
  }
  لكل (صحيح i = 0; i < s->errs->عدد; ++i)
    إذا (s->errs->symbols[i])
      bitset_سیٹ (no_reduce_سیٹ, s->errs->symbols[i]->content->نمبر);

  /* Compute the width of the lookahead token column.  */
  size_t چوڑائی = 0;
  إذا (default_reduction)
    چوڑائی = mbswidth (_("$default"), 0);

  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool گنتی = bitset_test (no_reduce_سیٹ, i);

        لكل (صحيح j = 0; j < reds->عدد; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! گنتی)
                {
                  إذا (reds->قاعدے[j] != default_reduction)
                    max_لمائی (&چوڑائی, symbols[i]->ٹیگ);
                  گنتی = true;
                }
              وإلا
                max_لمائی (&چوڑائی, symbols[i]->ٹیگ);
            }
      }

  /* Nothing to report. */
  إذا (!چوڑائی)
    أرجع;

  fputc ('\n', باہرلا);
  چوڑائی += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool گنتی = bitset_test (no_reduce_سیٹ, i);
        إذا (گنتی)
          default_reduction_only = false;

        لكل (صحيح j = 0; j < reds->عدد; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! گنتی)
                {
                  إذا (reds->قاعدے[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (باہرلا, چوڑائی,
                                       symbols[i]->ٹیگ,
                                       reds->قاعدے[j], true);
                    }
                  وإلا
                    defaulted = true;
                  گنتی = true;
                }
              وإلا
                {
                  default_reduction_only = false;
                  إذا (defaulted)
                    print_reduction (باہرلا, چوڑائی,
                                     symbols[i]->ٹیگ,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (باہرلا, چوڑائی,
                                   symbols[i]->ٹیگ,
                                   reds->قاعدے[j], false);
                }
            }
      }

  إذا (default_reduction)
    {
      حرف *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (باہرلا, چوڑائی, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->عدد == 1 && قاعدہ_is_initial (reds->قاعدے[0])));
      (فراغ) default_reduction_only;
      حرر (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *باہرلا, ثابت state *s)
{
  /* Print shifts.  */
  print_transitions (s, باہرلا, true);
  print_errs (باہرلا, s);
  print_reductions (باہرلا, s);
  /* Print gotos.  */
  print_transitions (s, باہرلا, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ساكن فراغ
print_state (FILE *باہرلا, ثابت state *s)
{
  fputs ("\n\n", باہرلا);
  fprintf (باہرلا, _("State %d"), s->نمبر);
  fputc ('\n', باہرلا);
  print_core (باہرلا, s);
  print_actions (باہرلا, s);
  إذا ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', باہرلا);
      fputs (s->solved_conflicts, باہرلا);
    }
  إذا (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      fputc ('\n', باہرلا);
      counterexample_report_state (s, باہرلا, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ساكن فراغ
print_terminal_symbols (FILE *باہرلا)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (باہرلا, "%s\n\n", _("Terminals, with rules where they appear"));
  لكل (صحيح i = 0; i < max_کوڈ + 1; ++i)
    إذا (token_translations[i] != undeftoken->content->نمبر)
      {
        ثابت علامت *sym = symbols[token_translations[i]];
        ثابت حرف *ٹیگ = sym->ٹیگ;
        fprintf (باہرلا, "%4s%s", "", ٹیگ);
        إذا (sym->content->type_name)
          fprintf (باہرلا, " <%s>", sym->content->type_name);
        fprintf (باہرلا, " (%d)", i);

        لكل (قاعدہ_نمبر r = 0; r < nrules; r++)
          لكل (شے_نمبر *rhsp = قاعدے[r].rhs; *rhsp >= 0; rhsp++)
            إذا (شے_نمبر_as_علامت_نمبر (*rhsp) == token_translations[i])
              {
                fprintf (باہرلا, " %d", r);
                اكسر;
              }
        fputc ('\n', باہرلا);
      }
  fputs ("\n\n", باہرلا);
}


ساكن فراغ
print_nonterminal_symbols (FILE *باہرلا)
{
  fprintf (باہرلا, "%s\n\n", _("Nonterminals, with rules where they appear"));
  لكل (علامت_نمبر i = ntokens; i < nsyms; i++)
    {
      ثابت علامت *sym = symbols[i];
      ثابت حرف *ٹیگ = sym->ٹیگ;
      bool on_بایاں = false;
      bool on_دایاں = false;

      لكل (قاعدہ_نمبر r = 0; r < nrules; r++)
        {
          on_بایاں |= قاعدے[r].lhs->نمبر == i;
          لكل (شے_نمبر *rhsp = قاعدے[r].rhs; !on_دایاں && 0 <= *rhsp; ++rhsp)
            on_دایاں |= شے_نمبر_as_علامت_نمبر (*rhsp) == i;
          إذا (on_بایاں && on_دایاں)
            اكسر;
        }

      fprintf (باہرلا, "%4s%s", "", ٹیگ);
      إذا (sym->content->type_name)
        fprintf (باہرلا, " <%s>", sym->content->type_name);
      fprintf (باہرلا, " (%d)\n", i);

      إذا (on_بایاں)
        {
          fprintf (باہرلا, "%8s%s", "", _("on left:"));
          لكل (قاعدہ_نمبر r = 0; r < nrules; r++)
            إذا (قاعدے[r].lhs->نمبر == i)
              fprintf (باہرلا, " %d", r);
          fputc ('\n', باہرلا);
        }

      إذا (on_دایاں)
        {
          fprintf (باہرلا, "%8s%s", "", _("on right:"));
          لكل (قاعدہ_نمبر r = 0; r < nrules; r++)
            لكل (شے_نمبر *rhsp = قاعدے[r].rhs; 0 <= *rhsp; ++rhsp)
              إذا (شے_نمبر_as_علامت_نمبر (*rhsp) == i)
                {
                  fprintf (باہرلا, " %d", r);
                  اكسر;
                }
          fputc ('\n', باہرلا);
        }
    }
}

فراغ
print_results (فراغ)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *باہرلا = xfopen (spec_verbose_file, "w");

  reduce_output (باہرلا);
  grammar_قاعدے_partial_print (باہرلا,
                               _("Rules useless in parser due to conflicts"),
                               قاعدہ_useless_وچ_parser_p);
  conflicts_output (باہرلا);

  grammar_قاعدے_print (باہرلا);
  print_terminal_symbols (باہرلا);
  print_nonterminal_symbols (باہرلا);

  /* Storage for print_reductions.  */
  no_reduce_سیٹ = bitset_create (ntokens, BITSET_FIXED);
  لكل (state_نمبر i = 0; i < nstates; i++)
    print_state (باہرلا, حالتاں[i]);
  bitset_free (no_reduce_سیٹ);

  xfclose (باہرلا);
}
