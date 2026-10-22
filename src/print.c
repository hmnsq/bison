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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "print.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "counterexample.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
ಸ್ಥಿರ bitset no_reduce_ಗಣ;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
max_ಉದ್ದಳತೆ (size_t *ಅಗಲ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
  size_t len = mbswidth (str, 0);
  ಆದರೆ (len > *ಅಗಲ)
    *ಅಗಲ = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_core (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ state *s)
{
  ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸೂಚ್ಯಂಕ *sitems = s->ವಸ್ತುಗಳು;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  ಆದರೆ (report_flag & report_itemsets)
    {
      ಆವರಣ (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  ಆದರೆ (!snritems)
    ಹಿಂತಿರುಗಿ;

  fputc ('\n', ಹೊರಹೋಗು);

  ನಿಯಮ ಸ್ಥಿರಾಂಕ *ಹಿಂದಿನ_ನಿಯಮ = ಖಾಲಿ;
  ಗಾಗಿ (size_t i = 0; i < snritems; i++)
    {
      ಅಂಶ_ಸಂಖ್ಯೆ *sp1 = ritem + sitems[i];
      ನಿಯಮ ಸ್ಥಿರಾಂಕ *r = ಅಂಶ_ನಿಯಮ (sp1);
      ಅಂಶ_print (sp1, ಹಿಂದಿನ_ನಿಯಮ, ಹೊರಹೋಗು);
      ಹಿಂದಿನ_ನಿಯಮ = r;

      /* Display the lookahead tokens?  */
      ಆದರೆ (report_flag & report_lookaheads
          && ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*sp1))
        state_ನಿಯಮ_lookaheads_print (s, r, ಹೊರಹೋಗು);
      fputc ('\n', ಹೊರಹೋಗು);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_transitions (ಸ್ಥಿರಾಂಕ state *s, FILE *ಹೊರಹೋಗು, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t ಅಗಲ = 0;

  /* Compute the width of the lookahead token column.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < trans->ಅಂಕೆ; i++)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_ಸರಿಸು (trans, i) == display_transitions_p)
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        max_ಉದ್ದಳತೆ (&ಅಗಲ, sym->ಟ್ಯಾಗ್);
      }

  /* Nothing to report. */
  ಆದರೆ (!ಅಗಲ)
    ಹಿಂತಿರುಗಿ;

  fputc ('\n', ಹೊರಹೋಗು);
  ಅಗಲ += 2;

  /* Report lookahead tokens and shifts.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < trans->ಅಂಕೆ; i++)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_ಸರಿಸು (trans, i) == display_transitions_p)
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
        ಸ್ಥಿರಾಂಕ state *s1 = trans->ಸ್ಥಿತಿಗಳು[i];

        fprintf (ಹೊರಹೋಗು, "    %s", ಟ್ಯಾಗ್);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = ಅಗಲ - mbswidth (ಟ್ಯಾಗ್, 0); j > 0; --j)
          fputc (' ', ಹೊರಹೋಗು);
        ಆದರೆ (display_transitions_p)
          fprintf (ಹೊರಹೋಗು, _("shift, and go to state %d\n"), s1->ಸಂಖ್ಯೆ);
        ಇಲ್ಲದಿದ್ದರೆ
          fprintf (ಹೊರಹೋಗು, _("go to state %d\n"), s1->ಸಂಖ್ಯೆ);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_errs (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ state *s)
{
  errs *errp = s->errs;
  size_t ಅಗಲ = 0;

  /* Compute the width of the lookahead token column.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < errp->ಅಂಕೆ; ++i)
    ಆದರೆ (errp->symbols[i])
      max_ಉದ್ದಳತೆ (&ಅಗಲ, errp->symbols[i]->ಟ್ಯಾಗ್);

  /* Nothing to report. */
  ಆದರೆ (!ಅಗಲ)
    ಹಿಂತಿರುಗಿ;

  fputc ('\n', ಹೊರಹೋಗು);
  ಅಗಲ += 2;

  /* Report lookahead tokens and errors.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < errp->ಅಂಕೆ; ++i)
    ಆದರೆ (errp->symbols[i])
      {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = errp->symbols[i]->ಟ್ಯಾಗ್;
        fprintf (ಹೊರಹೋಗು, "    %s", ಟ್ಯಾಗ್);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = ಅಗಲ - mbswidth (ಟ್ಯಾಗ್, 0); j > 0; --j)
          fputc (' ', ಹೊರಹೋಗು);
        fputs (_("error (nonassociative)\n"), ಹೊರಹೋಗು);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reduction (FILE *ಹೊರಹೋಗು, size_t ಅಗಲ,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lookahead,
                 ನಿಯಮ *r, bool enabled)
{
  fprintf (ಹೊರಹೋಗು, "    %s", lookahead);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = ಅಗಲ - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', ಹೊರಹೋಗು);
  ಆದರೆ (!enabled)
    fputc ('[', ಹೊರಹೋಗು);
  ಆದರೆ (ನಿಯಮ_is_initial (r))
    fprintf (ಹೊರಹೋಗು, _("accept"));
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, _("reduce using rule %d (%s)"), r->ಸಂಖ್ಯೆ,
             r->lhs->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
  ಆದರೆ (!enabled)
    fputc (']', ಹೊರಹೋಗು);
  fputc ('\n', ಹೊರಹೋಗು);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reductions (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ state *s)
{
  reductions *reds = s->reductions;
  ಆದರೆ (reds->ಅಂಕೆ == 0)
    ಹಿಂತಿರುಗಿ;

  ನಿಯಮ *default_reduction = ಖಾಲಿ;
  ಆದರೆ (yydefact[s->ಸಂಖ್ಯೆ] != 0)
    default_reduction = &ನಿಯಮಗಳು[yydefact[s->ಸಂಖ್ಯೆ] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_ಗಣ);
  {
    ಪೂರ್ಣಾಂಕ i;
    FOR_EACH_ಸರಿಸು (trans, i)
      bitset_ಗಣ (no_reduce_ಗಣ, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  }
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < s->errs->ಅಂಕೆ; ++i)
    ಆದರೆ (s->errs->symbols[i])
      bitset_ಗಣ (no_reduce_ಗಣ, s->errs->symbols[i]->content->ಸಂಖ್ಯೆ);

  /* Compute the width of the lookahead token column.  */
  size_t ಅಗಲ = 0;
  ಆದರೆ (default_reduction)
    ಅಗಲ = mbswidth (_("$default"), 0);

  ಆದರೆ (reds->lookaheads)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; i++)
      {
        bool ಎಣಿಕೆ_2 = bitset_test (no_reduce_ಗಣ, i);

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < reds->ಅಂಕೆ; ++j)
          ಆದರೆ (bitset_test (reds->lookaheads[j], i))
            {
              ಆದರೆ (! ಎಣಿಕೆ_2)
                {
                  ಆದರೆ (reds->ನಿಯಮಗಳು[j] != default_reduction)
                    max_ಉದ್ದಳತೆ (&ಅಗಲ, symbols[i]->ಟ್ಯಾಗ್);
                  ಎಣಿಕೆ_2 = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                max_ಉದ್ದಳತೆ (&ಅಗಲ, symbols[i]->ಟ್ಯಾಗ್);
            }
      }

  /* Nothing to report. */
  ಆದರೆ (!ಅಗಲ)
    ಹಿಂತಿರುಗಿ;

  fputc ('\n', ಹೊರಹೋಗು);
  ಅಗಲ += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  ಆದರೆ (reds->lookaheads)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ಎಣಿಕೆ_2 = bitset_test (no_reduce_ಗಣ, i);
        ಆದರೆ (ಎಣಿಕೆ_2)
          default_reduction_only = false;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < reds->ಅಂಕೆ; ++j)
          ಆದರೆ (bitset_test (reds->lookaheads[j], i))
            {
              ಆದರೆ (! ಎಣಿಕೆ_2)
                {
                  ಆದರೆ (reds->ನಿಯಮಗಳು[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (ಹೊರಹೋಗು, ಅಗಲ,
                                       symbols[i]->ಟ್ಯಾಗ್,
                                       reds->ನಿಯಮಗಳು[j], true);
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    defaulted = true;
                  ಎಣಿಕೆ_2 = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  default_reduction_only = false;
                  ಆದರೆ (defaulted)
                    print_reduction (ಹೊರಹೋಗು, ಅಗಲ,
                                     symbols[i]->ಟ್ಯಾಗ್,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (ಹೊರಹೋಗು, ಅಗಲ,
                                   symbols[i]->ಟ್ಯಾಗ್,
                                   reds->ನಿಯಮಗಳು[j], false);
                }
            }
      }

  ಆದರೆ (default_reduction)
    {
      ಅಕ್ಷರ *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (ಹೊರಹೋಗು, ಅಗಲ, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->ಅಂಕೆ == 1 && ನಿಯಮ_is_initial (reds->ನಿಯಮಗಳು[0])));
      (ಶೂನ್ಯ) default_reduction_only;
      ಬಿಡುಗಡೆ (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_actions (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ state *s)
{
  /* Print shifts.  */
  print_transitions (s, ಹೊರಹೋಗು, true);
  print_errs (ಹೊರಹೋಗು, s);
  print_reductions (ಹೊರಹೋಗು, s);
  /* Print gotos.  */
  print_transitions (s, ಹೊರಹೋಗು, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_state (FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ state *s)
{
  fputs ("\n\n", ಹೊರಹೋಗು);
  fprintf (ಹೊರಹೋಗು, _("State %d"), s->ಸಂಖ್ಯೆ);
  fputc ('\n', ಹೊರಹೋಗು);
  print_core (ಹೊರಹೋಗು, s);
  print_actions (ಹೊರಹೋಗು, s);
  ಆದರೆ ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', ಹೊರಹೋಗು);
      fputs (s->solved_conflicts, ಹೊರಹೋಗು);
    }
  ಆದರೆ (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      fputc ('\n', ಹೊರಹೋಗು);
      counterexample_report_state (s, ಹೊರಹೋಗು, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_terminal_symbols (FILE *ಹೊರಹೋಗು)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (ಹೊರಹೋಗು, "%s\n\n", _("Terminals, with rules where they appear"));
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < max_ಸಂಕೇತ + 1; ++i)
    ಆದರೆ (token_translations[i] != undeftoken->content->ಸಂಖ್ಯೆ)
      {
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[token_translations[i]];
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
        fprintf (ಹೊರಹೋಗು, "%4s%s", "", ಟ್ಯಾಗ್);
        ಆದರೆ (sym->content->type_name)
          fprintf (ಹೊರಹೋಗು, " <%s>", sym->content->type_name);
        fprintf (ಹೊರಹೋಗು, " (%d)", i);

        ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
          ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = ನಿಯಮಗಳು[r].rhs; *rhsp >= 0; rhsp++)
            ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp) == token_translations[i])
              {
                fprintf (ಹೊರಹೋಗು, " %d", r);
                ನಿಲ್ಲಿಸು;
              }
        fputc ('\n', ಹೊರಹೋಗು);
      }
  fputs ("\n\n", ಹೊರಹೋಗು);
}


ಸ್ಥಿರ ಶೂನ್ಯ
print_nonterminal_symbols (FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು, "%s\n\n", _("Nonterminals, with rules where they appear"));
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; i++)
    {
      ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *sym = symbols[i];
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = sym->ಟ್ಯಾಗ್;
      bool on_ಎಡ = false;
      bool on_ಬಲ = false;

      ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
        {
          on_ಎಡ |= ನಿಯಮಗಳು[r].lhs->ಸಂಖ್ಯೆ == i;
          ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = ನಿಯಮಗಳು[r].rhs; !on_ಬಲ && 0 <= *rhsp; ++rhsp)
            on_ಬಲ |= ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp) == i;
          ಆದರೆ (on_ಎಡ && on_ಬಲ)
            ನಿಲ್ಲಿಸು;
        }

      fprintf (ಹೊರಹೋಗು, "%4s%s", "", ಟ್ಯಾಗ್);
      ಆದರೆ (sym->content->type_name)
        fprintf (ಹೊರಹೋಗು, " <%s>", sym->content->type_name);
      fprintf (ಹೊರಹೋಗು, " (%d)\n", i);

      ಆದರೆ (on_ಎಡ)
        {
          fprintf (ಹೊರಹೋಗು, "%8s%s", "", _("on left:"));
          ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
            ಆದರೆ (ನಿಯಮಗಳು[r].lhs->ಸಂಖ್ಯೆ == i)
              fprintf (ಹೊರಹೋಗು, " %d", r);
          fputc ('\n', ಹೊರಹೋಗು);
        }

      ಆದರೆ (on_ಬಲ)
        {
          fprintf (ಹೊರಹೋಗು, "%8s%s", "", _("on right:"));
          ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
            ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *rhsp = ನಿಯಮಗಳು[r].rhs; 0 <= *rhsp; ++rhsp)
              ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp) == i)
                {
                  fprintf (ಹೊರಹೋಗು, " %d", r);
                  ನಿಲ್ಲಿಸು;
                }
          fputc ('\n', ಹೊರಹೋಗು);
        }
    }
}

ಶೂನ್ಯ
print_results (ಶೂನ್ಯ)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *ಹೊರಹೋಗು = xfopen (spec_verbose_file, "w");

  reduce_output (ಹೊರಹೋಗು);
  grammar_ನಿಯಮಗಳು_partial_print (ಹೊರಹೋಗು,
                               _("Rules useless in parser due to conflicts"),
                               ನಿಯಮ_useless_ಒಳ_parser_p);
  conflicts_output (ಹೊರಹೋಗು);

  grammar_ನಿಯಮಗಳು_print (ಹೊರಹೋಗು);
  print_terminal_symbols (ಹೊರಹೋಗು);
  print_nonterminal_symbols (ಹೊರಹೋಗು);

  /* Storage for print_reductions.  */
  no_reduce_ಗಣ = bitset_create (ntokens, BITSET_FIXED);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; i++)
    print_state (ಹೊರಹೋಗು, ಸ್ಥಿತಿಗಳು[i]);
  bitset_free (no_reduce_ಗಣ);

  xfclose (ಹೊರಹೋಗು);
}
