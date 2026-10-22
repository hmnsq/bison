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
ساكن bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ساكن فراغ
max_length (size_t *width, ثابت حرف *str)
{
  size_t len = mbswidth (str, 0);
  إذا (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ساكن فراغ
print_core (FILE *out, ثابت state *s)
{
  ثابت item_index *sitems = s->کاڵاکان;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  إذا (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  إذا (!snritems)
    أرجع;

  fputc ('\n', out);

  rule ثابت *previous_rule = عدم;
  لكل (size_t i = 0; i < snritems; i++)
    {
      item_ژمار *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      إذا (report_flag & report_lookaheads
          && item_ژمار_is_rule_ژمار (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ساكن فراغ
print_transitions (ثابت state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < trans->ژمارە; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        هێما *sym = symbols[TRANSITION_هێما (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  إذا (!width)
    أرجع;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  لكل (صحيح i = 0; i < trans->ژمارە; i++)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        هێما *sym = symbols[TRANSITION_هێما (trans, i)];
        ثابت حرف *tag = sym->tag;
        ثابت state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        لكل (صحيح j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        إذا (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->ژمار);
        وإلا
          fprintf (out, _("go to state %d\n"), s1->ژمار);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ساكن فراغ
print_errs (FILE *out, ثابت state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  لكل (صحيح i = 0; i < errp->ژمارە; ++i)
    إذا (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  إذا (!width)
    أرجع;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  لكل (صحيح i = 0; i < errp->ژمارە; ++i)
    إذا (errp->symbols[i])
      {
        ثابت حرف *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        لكل (صحيح j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ساكن فراغ
print_reduction (FILE *out, size_t width,
                 ثابت حرف *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  لكل (صحيح j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  إذا (!enabled)
    fputc ('[', out);
  إذا (rule_is_initial (r))
    fprintf (out, _("accept"));
  وإلا
    fprintf (out, _("reduce using rule %d (%s)"), r->ژمار,
             r->lhs->هێما->tag);
  إذا (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ساكن فراغ
print_reductions (FILE *out, ثابت state *s)
{
  reductions *reds = s->reductions;
  إذا (reds->ژمارە == 0)
    أرجع;

  rule *default_reduction = عدم;
  إذا (yydefact[s->ژمار] != 0)
    default_reduction = &rules[yydefact[s->ژمار] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    صحيح i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_هێما (trans, i));
  }
  لكل (صحيح i = 0; i < s->errs->ژمارە; ++i)
    إذا (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->ژمار);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  إذا (default_reduction)
    width = mbswidth (_("$default"), 0);

  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool ژماردن = bitset_test (no_reduce_set, i);

        لكل (صحيح j = 0; j < reds->ژمارە; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! ژماردن)
                {
                  إذا (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  ژماردن = true;
                }
              وإلا
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  إذا (!width)
    أرجع;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ژماردن = bitset_test (no_reduce_set, i);
        إذا (ژماردن)
          default_reduction_only = false;

        لكل (صحيح j = 0; j < reds->ژمارە; ++j)
          إذا (bitset_test (reds->lookaheads[j], i))
            {
              إذا (! ژماردن)
                {
                  إذا (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  وإلا
                    defaulted = true;
                  ژماردن = true;
                }
              وإلا
                {
                  default_reduction_only = false;
                  إذا (defaulted)
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

  إذا (default_reduction)
    {
      حرف *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->ژمارە == 1 && rule_is_initial (reds->rules[0])));
      (فراغ) default_reduction_only;
      حرر (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ساكن فراغ
print_actions (FILE *out, ثابت state *s)
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

ساكن فراغ
print_state (FILE *out, ثابت state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->ژمار);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  إذا ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  إذا (has_conflicts (s)
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

ساكن فراغ
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  لكل (صحيح i = 0; i < max_code + 1; ++i)
    إذا (token_translations[i] != undeftoken->content->ژمار)
      {
        ثابت هێما *sym = symbols[token_translations[i]];
        ثابت حرف *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        إذا (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        لكل (rule_ژمار r = 0; r < nrules; r++)
          لكل (item_ژمار *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            إذا (item_ژمار_as_هێما_ژمار (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                اكسر;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


ساكن فراغ
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  لكل (هێما_ژمار i = ntokens; i < nsyms; i++)
    {
      ثابت هێما *sym = symbols[i];
      ثابت حرف *tag = sym->tag;
      bool on_چەپ = false;
      bool on_ڕاست = false;

      لكل (rule_ژمار r = 0; r < nrules; r++)
        {
          on_چەپ |= rules[r].lhs->ژمار == i;
          لكل (item_ژمار *rhsp = rules[r].rhs; !on_ڕاست && 0 <= *rhsp; ++rhsp)
            on_ڕاست |= item_ژمار_as_هێما_ژمار (*rhsp) == i;
          إذا (on_چەپ && on_ڕاست)
            اكسر;
        }

      fprintf (out, "%4s%s", "", tag);
      إذا (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      إذا (on_چەپ)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          لكل (rule_ژمار r = 0; r < nrules; r++)
            إذا (rules[r].lhs->ژمار == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      إذا (on_ڕاست)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          لكل (rule_ژمار r = 0; r < nrules; r++)
            لكل (item_ژمار *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              إذا (item_ژمار_as_هێما_ژمار (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  اكسر;
                }
          fputc ('\n', out);
        }
    }
}

فراغ
print_results (فراغ)
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
  لكل (state_ژمار i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
