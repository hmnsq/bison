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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "print.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>
#ଅନ୍ତର୍ଭାଵ <mbswidth.h>

#ଅନ୍ତର୍ଭାଵ "closure.h"
#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "conflicts.h"
#ଅନ୍ତର୍ଭାଵ "counterexample.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "state.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"
#ଅନ୍ତର୍ଭାଵ "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
ସ୍ଥିର bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
max_length (size_t *width, ଅଚଳ ଅକ୍ଷର *str)
{
  size_t len = mbswidth (str, 0);
  ଯଦି (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_core (FILE *out, ଅଚଳ state *s)
{
  ଅଚଳ item_index *sitems = s->ବସ୍ତୁଗୁଡ଼ିକ;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  ଯଦି (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  ଯଦି (!snritems)
    ଫେରନ୍ତୁ;

  fputc ('\n', out);

  rule ଅଚଳ *previous_rule = ଖାଲି;
  ପାଇଁ (size_t i = 0; i < snritems; i++)
    {
      item_ସଂଖ୍ୟା *sp1 = ritem + sitems[i];
      rule ଅଚଳ *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      ଯଦି (report_flag & report_lookaheads
          && item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_transitions (ଅଚଳ state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < trans->ଅଙ୍କ; i++)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        ପ୍ରତୀକ *sym = symbols[TRANSITION_ପ୍ରତୀକ (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  ଯଦି (!width)
    ଫେରନ୍ତୁ;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < trans->ଅଙ୍କ; i++)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        ପ୍ରତୀକ *sym = symbols[TRANSITION_ପ୍ରତୀକ (trans, i)];
        ଅଚଳ ଅକ୍ଷର *tag = sym->tag;
        ଅଚଳ state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        ଯଦି (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->ସଂଖ୍ୟା);
        ଅନ୍ୟଥା
          fprintf (out, _("go to state %d\n"), s1->ସଂଖ୍ୟା);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_errs (FILE *out, ଅଚଳ state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < errp->ଅଙ୍କ; ++i)
    ଯଦି (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  ଯଦି (!width)
    ଫେରନ୍ତୁ;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < errp->ଅଙ୍କ; ++i)
    ଯଦି (errp->symbols[i])
      {
        ଅଚଳ ଅକ୍ଷର *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_reduction (FILE *out, size_t width,
                 ଅଚଳ ଅକ୍ଷର *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  ଯଦି (!enabled)
    fputc ('[', out);
  ଯଦି (rule_is_initial (r))
    fprintf (out, _("accept"));
  ଅନ୍ୟଥା
    fprintf (out, _("reduce using rule %d (%s)"), r->ସଂଖ୍ୟା,
             r->lhs->ପ୍ରତୀକ->tag);
  ଯଦି (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_reductions (FILE *out, ଅଚଳ state *s)
{
  reductions *reds = s->reductions;
  ଯଦି (reds->ଅଙ୍କ == 0)
    ଫେରନ୍ତୁ;

  rule *default_reduction = ଖାଲି;
  ଯଦି (yydefact[s->ସଂଖ୍ୟା] != 0)
    default_reduction = &rules[yydefact[s->ସଂଖ୍ୟା] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_ପ୍ରତୀକ (trans, i));
  }
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < s->errs->ଅଙ୍କ; ++i)
    ଯଦି (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->ସଂଖ୍ୟା);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  ଯଦି (default_reduction)
    width = mbswidth (_("$default"), 0);

  ଯଦି (reds->lookaheads)
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; i++)
      {
        bool ଗଣନା_2 = bitset_test (no_reduce_set, i);

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < reds->ଅଙ୍କ; ++j)
          ଯଦି (bitset_test (reds->lookaheads[j], i))
            {
              ଯଦି (! ଗଣନା_2)
                {
                  ଯଦି (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  ଗଣନା_2 = true;
                }
              ଅନ୍ୟଥା
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  ଯଦି (!width)
    ଫେରନ୍ତୁ;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  ଯଦି (reds->lookaheads)
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ଗଣନା_2 = bitset_test (no_reduce_set, i);
        ଯଦି (ଗଣନା_2)
          default_reduction_only = false;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < reds->ଅଙ୍କ; ++j)
          ଯଦି (bitset_test (reds->lookaheads[j], i))
            {
              ଯଦି (! ଗଣନା_2)
                {
                  ଯଦି (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  ଅନ୍ୟଥା
                    defaulted = true;
                  ଗଣନା_2 = true;
                }
              ଅନ୍ୟଥା
                {
                  default_reduction_only = false;
                  ଯଦି (defaulted)
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

  ଯଦି (default_reduction)
    {
      ଅକ୍ଷର *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->ଅଙ୍କ == 1 && rule_is_initial (reds->rules[0])));
      (ଶୂନ୍ୟ) default_reduction_only;
      ମୁକ୍ତ (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
print_actions (FILE *out, ଅଚଳ state *s)
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

ସ୍ଥିର ଶୂନ୍ୟ
print_state (FILE *out, ଅଚଳ state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->ସଂଖ୍ୟା);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  ଯଦି ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  ଯଦି (has_conflicts (s)
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

ସ୍ଥିର ଶୂନ୍ୟ
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < max_code + 1; ++i)
    ଯଦି (token_translations[i] != undeftoken->content->ସଂଖ୍ୟା)
      {
        ଅଚଳ ପ୍ରତୀକ *sym = symbols[token_translations[i]];
        ଅଚଳ ଅକ୍ଷର *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        ଯଦି (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; r++)
          ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            ଯଦି (item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                ଭାଙ୍ଗ;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


ସ୍ଥିର ଶୂନ୍ୟ
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; i++)
    {
      ଅଚଳ ପ୍ରତୀକ *sym = symbols[i];
      ଅଚଳ ଅକ୍ଷର *tag = sym->tag;
      bool on_ବାମ = false;
      bool on_ଡାହାଣ = false;

      ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; r++)
        {
          on_ବାମ |= rules[r].lhs->ସଂଖ୍ୟା == i;
          ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; !on_ଡାହାଣ && 0 <= *rhsp; ++rhsp)
            on_ଡାହାଣ |= item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*rhsp) == i;
          ଯଦି (on_ବାମ && on_ଡାହାଣ)
            ଭାଙ୍ଗ;
        }

      fprintf (out, "%4s%s", "", tag);
      ଯଦି (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      ଯଦି (on_ବାମ)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; r++)
            ଯଦି (rules[r].lhs->ସଂଖ୍ୟା == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      ଯଦି (on_ଡାହାଣ)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; r++)
            ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              ଯଦି (item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  ଭାଙ୍ଗ;
                }
          fputc ('\n', out);
        }
    }
}

ଶୂନ୍ୟ
print_results (ଶୂନ୍ୟ)
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
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
