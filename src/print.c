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
ساكن bitset no_reduce_سيٽ;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ساكن فراغ
max_ڊيگهائي (size_t *ويڪر, ثابت حرف *str)
{
  size_t len = mbswidth (str, 0);
  إذا (len > *ويڪر)
    *ويڪر = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *ٻاهريون, ثابت state *s)
{
  ثابت شي_اشاريو *sitems = s->شيون;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      گهيرو (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  إذا (!snritems)
    أرجع;

  fputc ('\n', ٻاهريون);

  قاعدو ثابت *اڳيون_قاعدو = عدم;
  لكل (size_t i = 0; i < snritems; i++)
    {
      شي_عدد *sp1 = ritem + sitems[i];
      قاعدو ثابت *r = شي_قاعدو (sp1);
      شي_print (sp1, اڳيون_قاعدو, ٻاهريون);
      اڳيون_قاعدو = r;

      /* Display the lookahead tokens?  */
      إذا (report_flag & report_lookaheads
          && شي_عدد_is_قاعدو_عدد (*sp1))
        state_قاعدو_lookaheads_print (s, r, ٻاهريون);
      fputc ('\n', ٻاهريون);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ساكن فراغ
print_transitions (ثابت state *s, FILE *ٻاهريون, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t ويڪر = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < trans->انگ; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_سرڪايو (trans, i) == display_transitions_p)
      {
        نشان *sym = symbols[TRANSITION_نشان (trans, i)];
        max_ڊيگهائي (&ويڪر, sym->ٽيگ);
      }

  /* Nothing to report. */
  إذا (!ويڪر)
    أرجع;

  fputc ('\n', ٻاهريون);
  ويڪر += 2;

  /* Report lookahead tokens and shifts.  */
  لكل (صحيح i = 0; i < trans->انگ; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_سرڪايو (trans, i) == display_transitions_p)
      {
        نشان *sym = symbols[TRANSITION_نشان (trans, i)];
        ثابت حرف *ٽيگ = sym->ٽيگ;
        ثابت state *s1 = trans->حالتون[i];

        fprintf (ٻاهريون, "    %s", ٽيگ);
        لكل (صحيح j = ويڪر - mbswidth (ٽيگ, 0); j > 0; --j)
          fputc (' ', ٻاهريون);
        إذا (display_transitions_p)
          fprintf (ٻاهريون, _("shift, and go to state %d\n"), s1->عدد);
        وإلا
          fprintf (ٻاهريون, _("go to state %d\n"), s1->عدد);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *ٻاهريون, ثابت state *s)
{
  errs *errp = s->errs;
  size_t ويڪر = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < errp->انگ; ++i)
    إذا (errp->symbols[i])
      max_ڊيگهائي (&ويڪر, errp->symbols[i]->ٽيگ);

  /* Nothing to report. */
  إذا (!ويڪر)
    أرجع;

  fputc ('\n', ٻاهريون);
  ويڪر += 2;

  /* Report lookahead tokens and errors.  */
  لكل (صحيح i = 0; i < errp->انگ; ++i)
    إذا (errp->symbols[i])
      {
        ثابت حرف *ٽيگ = errp->symbols[i]->ٽيگ;
        fprintf (ٻاهريون, "    %s", ٽيگ);
        لكل (صحيح j = ويڪر - mbswidth (ٽيگ, 0); j > 0; --j)
          fputc (' ', ٻاهريون);
        fputs (_("error (nonassociative)\n"), ٻاهريون);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
print_reduction (FILE *ٻاهريون, size_t ويڪر,
                 ثابت حرف *lookahead,
                 قاعدو *r, bool enabled)
{
  fprintf (ٻاهريون, "    %s", lookahead);
  لكل (صحيح j = ويڪر - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', ٻاهريون);
  إذا (!enabled)
    fputc ('[', ٻاهريون);
  إذا (قاعدو_is_initial (r))
    fprintf (ٻاهريون, _("accept"));
  وإلا
    fprintf (ٻاهريون, _("reduce using rule %d (%s)"), r->عدد,
             r->lhs->نشان->ٽيگ);
  إذا (!enabled)
    fputc (']', ٻاهريون);
  fputc ('\n', ٻاهريون);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *ٻاهريون, ثابت state *s)
{
  reductions *reds = s->reductions;
  إذا (reds->انگ == 0)
    أرجع;

  قاعدو *default_reduction = عدم;
  إذا (yydefact[s->عدد] != 0)
    default_reduction = &قاعدا[yydefact[s->عدد] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_سيٽ);
  {
    صحيح i;
    FOR_EACH_سرڪايو (trans, i)
      bitset_سيٽ (no_reduce_سيٽ, TRANSITION_نشان (trans, i));
  }
  لكل (صحيح i = 0; i < s->errs->انگ; ++i)
    إذا (s->errs->symbols[i])
      bitset_سيٽ (no_reduce_سيٽ, s->errs->symbols[i]->content->عدد);

  /* Compute the width of the lookahead token column.  */
  size_t ويڪر = 0;
  إذا (default_reduction)
    ويڪر = mbswidth (_("$default"), 0);

  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool ڳڻپ = bitset_test (no_reduce_سيٽ, i);

        لكل (صحيح j = 0; j < reds->انگ; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! ڳڻپ)
                {
                  إذا (reds->قاعدا[j] != default_reduction)
                    max_ڊيگهائي (&ويڪر, symbols[i]->ٽيگ);
                  ڳڻپ = true;
                }
              وإلا
                max_ڊيگهائي (&ويڪر, symbols[i]->ٽيگ);
            }
      }

  /* Nothing to report. */
  إذا (!ويڪر)
    أرجع;

  fputc ('\n', ٻاهريون);
  ويڪر += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ڳڻپ = bitset_test (no_reduce_سيٽ, i);
        إذا (ڳڻپ)
          default_reduction_only = false;

        لكل (صحيح j = 0; j < reds->انگ; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! ڳڻپ)
                {
                  إذا (reds->قاعدا[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (ٻاهريون, ويڪر,
                                       symbols[i]->ٽيگ,
                                       reds->قاعدا[j], true);
                    }
                  وإلا
                    defaulted = true;
                  ڳڻپ = true;
                }
              وإلا
                {
                  default_reduction_only = false;
                  إذا (defaulted)
                    print_reduction (ٻاهريون, ويڪر,
                                     symbols[i]->ٽيگ,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (ٻاهريون, ويڪر,
                                   symbols[i]->ٽيگ,
                                   reds->قاعدا[j], false);
                }
            }
      }

  إذا (default_reduction)
    {
      حرف *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (ٻاهريون, ويڪر, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->انگ == 1 && قاعدو_is_initial (reds->قاعدا[0])));
      (فراغ) default_reduction_only;
      حرر (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *ٻاهريون, ثابت state *s)
{
  /* Print shifts.  */
  print_transitions (s, ٻاهريون, true);
  print_errs (ٻاهريون, s);
  print_reductions (ٻاهريون, s);
  /* Print gotos.  */
  print_transitions (s, ٻاهريون, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ساكن فراغ
print_state (FILE *ٻاهريون, ثابت state *s)
{
  fputs ("\n\n", ٻاهريون);
  fprintf (ٻاهريون, _("State %d"), s->عدد);
  fputc ('\n', ٻاهريون);
  print_core (ٻاهريون, s);
  print_actions (ٻاهريون, s);
  إذا ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', ٻاهريون);
      fputs (s->solved_conflicts, ٻاهريون);
    }
  إذا (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      fputc ('\n', ٻاهريون);
      counterexample_report_state (s, ٻاهريون, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ساكن فراغ
print_terminal_symbols (FILE *ٻاهريون)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (ٻاهريون, "%s\n\n", _("Terminals, with rules where they appear"));
  لكل (صحيح i = 0; i < max_ڪوڊ + 1; ++i)
    إذا (token_translations[i] != undeftoken->content->عدد)
      {
        ثابت نشان *sym = symbols[token_translations[i]];
        ثابت حرف *ٽيگ = sym->ٽيگ;
        fprintf (ٻاهريون, "%4s%s", "", ٽيگ);
        إذا (sym->content->type_name)
          fprintf (ٻاهريون, " <%s>", sym->content->type_name);
        fprintf (ٻاهريون, " (%d)", i);

        لكل (قاعدو_عدد r = 0; r < nrules; r++)
          لكل (شي_عدد *rhsp = قاعدا[r].rhs; *rhsp >= 0; rhsp++)
            إذا (شي_عدد_as_نشان_عدد (*rhsp) == token_translations[i])
              {
                fprintf (ٻاهريون, " %d", r);
                اكسر;
              }
        fputc ('\n', ٻاهريون);
      }
  fputs ("\n\n", ٻاهريون);
}


ساكن فراغ
print_nonterminal_symbols (FILE *ٻاهريون)
{
  fprintf (ٻاهريون, "%s\n\n", _("Nonterminals, with rules where they appear"));
  لكل (نشان_عدد i = ntokens; i < nsyms; i++)
    {
      ثابت نشان *sym = symbols[i];
      ثابت حرف *ٽيگ = sym->ٽيگ;
      bool on_کاٻو = false;
      bool on_ساڄو = false;

      لكل (قاعدو_عدد r = 0; r < nrules; r++)
        {
          on_کاٻو |= قاعدا[r].lhs->عدد == i;
          لكل (شي_عدد *rhsp = قاعدا[r].rhs; !on_ساڄو && 0 <= *rhsp; ++rhsp)
            on_ساڄو |= شي_عدد_as_نشان_عدد (*rhsp) == i;
          إذا (on_کاٻو && on_ساڄو)
            اكسر;
        }

      fprintf (ٻاهريون, "%4s%s", "", ٽيگ);
      إذا (sym->content->type_name)
        fprintf (ٻاهريون, " <%s>", sym->content->type_name);
      fprintf (ٻاهريون, " (%d)\n", i);

      إذا (on_کاٻو)
        {
          fprintf (ٻاهريون, "%8s%s", "", _("on left:"));
          لكل (قاعدو_عدد r = 0; r < nrules; r++)
            إذا (قاعدا[r].lhs->عدد == i)
              fprintf (ٻاهريون, " %d", r);
          fputc ('\n', ٻاهريون);
        }

      إذا (on_ساڄو)
        {
          fprintf (ٻاهريون, "%8s%s", "", _("on right:"));
          لكل (قاعدو_عدد r = 0; r < nrules; r++)
            لكل (شي_عدد *rhsp = قاعدا[r].rhs; 0 <= *rhsp; ++rhsp)
              إذا (شي_عدد_as_نشان_عدد (*rhsp) == i)
                {
                  fprintf (ٻاهريون, " %d", r);
                  اكسر;
                }
          fputc ('\n', ٻاهريون);
        }
    }
}

فراغ
print_results (فراغ)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *ٻاهريون = xfopen (spec_verbose_file, "w");

  reduce_output (ٻاهريون);
  grammar_قاعدا_partial_print (ٻاهريون,
                               _("Rules useless in parser due to conflicts"),
                               قاعدو_useless_اندريون_parser_p);
  conflicts_output (ٻاهريون);

  grammar_قاعدا_print (ٻاهريون);
  print_terminal_symbols (ٻاهريون);
  print_nonterminal_symbols (ٻاهريون);

  /* Storage for print_reductions.  */
  no_reduce_سيٽ = bitset_create (ntokens, BITSET_FIXED);
  لكل (state_عدد i = 0; i < nstates; i++)
    print_state (ٻاهريون, حالتون[i]);
  bitset_free (no_reduce_سيٽ);

  xfclose (ٻاهريون);
}
