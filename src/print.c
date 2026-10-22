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

#شامل <config.h>

#شامل "print.h"

#شامل "system.h"

#شامل <bitset.h>
#شامل <mbswidth.h>

#شامل "closure.h"
#شامل "complain.h"
#شامل "conflicts.h"
#شامل "counterexample.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "muscle-tab.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "state.h"
#شامل "symtab.h"
#شامل "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
ایستا bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ایستا تهی
max_length (size_t *width, ثابت نویسه *str)
{
  size_t len = mbswidth (str, 0);
  اگر (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ایستا تهی
print_core (FILE *out, ثابت state *s)
{
  ثابت item_index *sitems = s->اقلام;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  اگر (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  اگر (!snritems)
    بازگشت;

  fputc ('\n', out);

  rule ثابت *previous_rule = پوچ;
  برای (size_t i = 0; i < snritems; i++)
    {
      item_عدد *sp1 = ritem + sitems[i];
      rule ثابت *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      اگر (report_flag & report_lookaheads
          && item_عدد_is_rule_عدد (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ایستا تهی
print_transitions (ثابت state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  برای (صحیح i = 0; i < trans->رقم; i++)
    اگر (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        نماد *sym = symbols[TRANSITION_نماد (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  اگر (!width)
    بازگشت;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  برای (صحیح i = 0; i < trans->رقم; i++)
    اگر (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        نماد *sym = symbols[TRANSITION_نماد (trans, i)];
        ثابت نویسه *tag = sym->tag;
        ثابت state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        برای (صحیح j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        اگر (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->عدد);
        وگرنه
          fprintf (out, _("go to state %d\n"), s1->عدد);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ایستا تهی
print_errs (FILE *out, ثابت state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  برای (صحیح i = 0; i < errp->رقم; ++i)
    اگر (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  اگر (!width)
    بازگشت;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  برای (صحیح i = 0; i < errp->رقم; ++i)
    اگر (errp->symbols[i])
      {
        ثابت نویسه *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        برای (صحیح j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ایستا تهی
print_reduction (FILE *out, size_t width,
                 ثابت نویسه *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  برای (صحیح j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  اگر (!enabled)
    fputc ('[', out);
  اگر (rule_is_initial (r))
    fprintf (out, _("accept"));
  وگرنه
    fprintf (out, _("reduce using rule %d (%s)"), r->عدد,
             r->lhs->نماد->tag);
  اگر (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ایستا تهی
print_reductions (FILE *out, ثابت state *s)
{
  reductions *reds = s->reductions;
  اگر (reds->رقم == 0)
    بازگشت;

  rule *default_reduction = پوچ;
  اگر (yydefact[s->عدد] != 0)
    default_reduction = &rules[yydefact[s->عدد] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    صحیح i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_نماد (trans, i));
  }
  برای (صحیح i = 0; i < s->errs->رقم; ++i)
    اگر (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->عدد);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  اگر (default_reduction)
    width = mbswidth (_("$default"), 0);

  اگر (reds->lookaheads)
    برای (صحیح i = 0; i < ntokens; i++)
      {
        bool شمار = bitset_test (no_reduce_set, i);

        برای (صحیح j = 0; j < reds->رقم; ++j)
          اگر (bitset_test (reds->lookaheads[j], i))
            {
              اگر (! شمار)
                {
                  اگر (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  شمار = true;
                }
              وگرنه
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  اگر (!width)
    بازگشت;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  اگر (reds->lookaheads)
    برای (صحیح i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool شمار = bitset_test (no_reduce_set, i);
        اگر (شمار)
          default_reduction_only = false;

        برای (صحیح j = 0; j < reds->رقم; ++j)
          اگر (bitset_test (reds->lookaheads[j], i))
            {
              اگر (! شمار)
                {
                  اگر (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  وگرنه
                    defaulted = true;
                  شمار = true;
                }
              وگرنه
                {
                  default_reduction_only = false;
                  اگر (defaulted)
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

  اگر (default_reduction)
    {
      نویسه *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->رقم == 1 && rule_is_initial (reds->rules[0])));
      (تهی) default_reduction_only;
      آزاد (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ایستا تهی
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

ایستا تهی
print_state (FILE *out, ثابت state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->عدد);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  اگر ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  اگر (has_conflicts (s)
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

ایستا تهی
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  برای (صحیح i = 0; i < max_code + 1; ++i)
    اگر (token_translations[i] != undeftoken->content->عدد)
      {
        ثابت نماد *sym = symbols[token_translations[i]];
        ثابت نویسه *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        اگر (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        برای (rule_عدد r = 0; r < nrules; r++)
          برای (item_عدد *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            اگر (item_عدد_as_نماد_عدد (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                بشکن;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


ایستا تهی
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  برای (نماد_عدد i = ntokens; i < nsyms; i++)
    {
      ثابت نماد *sym = symbols[i];
      ثابت نویسه *tag = sym->tag;
      bool on_چپ = false;
      bool on_راست = false;

      برای (rule_عدد r = 0; r < nrules; r++)
        {
          on_چپ |= rules[r].lhs->عدد == i;
          برای (item_عدد *rhsp = rules[r].rhs; !on_راست && 0 <= *rhsp; ++rhsp)
            on_راست |= item_عدد_as_نماد_عدد (*rhsp) == i;
          اگر (on_چپ && on_راست)
            بشکن;
        }

      fprintf (out, "%4s%s", "", tag);
      اگر (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      اگر (on_چپ)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          برای (rule_عدد r = 0; r < nrules; r++)
            اگر (rules[r].lhs->عدد == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      اگر (on_راست)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          برای (rule_عدد r = 0; r < nrules; r++)
            برای (item_عدد *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              اگر (item_عدد_as_نماد_عدد (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  بشکن;
                }
          fputc ('\n', out);
        }
    }
}

تهی
print_results (تهی)
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
  برای (state_عدد i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
