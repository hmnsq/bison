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
ಸ್ಥಿರ bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
max_length (size_t *width, ಧ್ರುವ ವರ್ಣ *str)
{
  size_t len = mbswidth (str, 0);
  ಯದಿ (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_core (FILE *out, ಧ್ರುವ state *s)
{
  ಧ್ರುವ item_index *sitems = s->ವಸ್ತುಗಳು;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  ಯದಿ (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  ಯದಿ (!snritems)
    ಪ್ರತ್ಯಾಗಮ;

  fputc ('\n', out);

  rule ಧ್ರುವ *previous_rule = ರಿಕ್ತ;
  ಪ್ರತಿ (size_t i = 0; i < snritems; i++)
    {
      item_ಸಂಖ್ಯೆ *sp1 = ritem + sitems[i];
      rule ಧ್ರುವ *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      ಯದಿ (report_flag & report_lookaheads
          && item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_transitions (ಧ್ರುವ state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < trans->ಅಂಕೆ; i++)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  ಯದಿ (!width)
    ಪ್ರತ್ಯಾಗಮ;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < trans->ಅಂಕೆ; i++)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        ಚಿಹ್ನೆ *sym = symbols[TRANSITION_ಚಿಹ್ನೆ (trans, i)];
        ಧ್ರುವ ವರ್ಣ *tag = sym->tag;
        ಧ್ರುವ state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        ಯದಿ (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->ಸಂಖ್ಯೆ);
        ಅನ್ಯಥಾ
          fprintf (out, _("go to state %d\n"), s1->ಸಂಖ್ಯೆ);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_errs (FILE *out, ಧ್ರುವ state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < errp->ಅಂಕೆ; ++i)
    ಯದಿ (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  ಯದಿ (!width)
    ಪ್ರತ್ಯಾಗಮ;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < errp->ಅಂಕೆ; ++i)
    ಯದಿ (errp->symbols[i])
      {
        ಧ್ರುವ ವರ್ಣ *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reduction (FILE *out, size_t width,
                 ಧ್ರುವ ವರ್ಣ *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  ಯದಿ (!enabled)
    fputc ('[', out);
  ಯದಿ (rule_is_initial (r))
    fprintf (out, _("accept"));
  ಅನ್ಯಥಾ
    fprintf (out, _("reduce using rule %d (%s)"), r->ಸಂಖ್ಯೆ,
             r->lhs->ಚಿಹ್ನೆ->tag);
  ಯದಿ (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_reductions (FILE *out, ಧ್ರುವ state *s)
{
  reductions *reds = s->reductions;
  ಯದಿ (reds->ಅಂಕೆ == 0)
    ಪ್ರತ್ಯಾಗಮ;

  rule *default_reduction = ರಿಕ್ತ;
  ಯದಿ (yydefact[s->ಸಂಖ್ಯೆ] != 0)
    default_reduction = &rules[yydefact[s->ಸಂಖ್ಯೆ] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    ಪೂರ್ಣಾಙ್ಕ i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  }
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < s->errs->ಅಂಕೆ; ++i)
    ಯದಿ (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->ಸಂಖ್ಯೆ);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  ಯದಿ (default_reduction)
    width = mbswidth (_("$default"), 0);

  ಯದಿ (reds->lookaheads)
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; i++)
      {
        bool ಎಣಿಕೆ = bitset_test (no_reduce_set, i);

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < reds->ಅಂಕೆ; ++j)
          ಯದಿ (bitset_test (reds->lookaheads[j], i))
            {
              ಯದಿ (! ಎಣಿಕೆ)
                {
                  ಯದಿ (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  ಎಣಿಕೆ = true;
                }
              ಅನ್ಯಥಾ
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  ಯದಿ (!width)
    ಪ್ರತ್ಯಾಗಮ;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  ಯದಿ (reds->lookaheads)
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ಎಣಿಕೆ = bitset_test (no_reduce_set, i);
        ಯದಿ (ಎಣಿಕೆ)
          default_reduction_only = false;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < reds->ಅಂಕೆ; ++j)
          ಯದಿ (bitset_test (reds->lookaheads[j], i))
            {
              ಯದಿ (! ಎಣಿಕೆ)
                {
                  ಯದಿ (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  ಅನ್ಯಥಾ
                    defaulted = true;
                  ಎಣಿಕೆ = true;
                }
              ಅನ್ಯಥಾ
                {
                  default_reduction_only = false;
                  ಯದಿ (defaulted)
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

  ಯದಿ (default_reduction)
    {
      ವರ್ಣ *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->ಅಂಕೆ == 1 && rule_is_initial (reds->rules[0])));
      (ಶೂನ್ಯ) default_reduction_only;
      ಮುಕ್ತ (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
print_actions (FILE *out, ಧ್ರುವ state *s)
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

ಸ್ಥಿರ ಶೂನ್ಯ
print_state (FILE *out, ಧ್ರುವ state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->ಸಂಖ್ಯೆ);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  ಯದಿ ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  ಯದಿ (has_conflicts (s)
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

ಸ್ಥಿರ ಶೂನ್ಯ
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < max_code + 1; ++i)
    ಯದಿ (token_translations[i] != undeftoken->content->ಸಂಖ್ಯೆ)
      {
        ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[token_translations[i]];
        ಧ್ರುವ ವರ್ಣ *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        ಯದಿ (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
          ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            ಯದಿ (item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                ಭಙ್ಗ;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


ಸ್ಥಿರ ಶೂನ್ಯ
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; i++)
    {
      ಧ್ರುವ ಚಿಹ್ನೆ *sym = symbols[i];
      ಧ್ರುವ ವರ್ಣ *tag = sym->tag;
      bool on_ಎಡ = false;
      bool on_ಬಲ = false;

      ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
        {
          on_ಎಡ |= rules[r].lhs->ಸಂಖ್ಯೆ == i;
          ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; !on_ಬಲ && 0 <= *rhsp; ++rhsp)
            on_ಬಲ |= item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp) == i;
          ಯದಿ (on_ಎಡ && on_ಬಲ)
            ಭಙ್ಗ;
        }

      fprintf (out, "%4s%s", "", tag);
      ಯದಿ (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      ಯದಿ (on_ಎಡ)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
            ಯದಿ (rules[r].lhs->ಸಂಖ್ಯೆ == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      ಯದಿ (on_ಬಲ)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; r++)
            ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              ಯದಿ (item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  ಭಙ್ಗ;
                }
          fputc ('\n', out);
        }
    }
}

ಶೂನ್ಯ
print_results (ಶೂನ್ಯ)
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
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
