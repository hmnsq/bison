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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "print.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <mbswidth.h>

#அந்தர்பாவ "closure.h"
#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "counterexample.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "state.h"
#அந்தர்பாவ "symtab.h"
#அந்தர்பாவ "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
நிலையான bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

நிலையான வெறுமை
max_length (size_t *width, மாறிலி எழுத்து *str)
{
  size_t len = mbswidth (str, 0);
  எனில் (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

நிலையான வெறுமை
print_core (FILE *out, மாறிலி state *s)
{
  மாறிலி item_index *sitems = s->பொருட்கள்;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  எனில் (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  எனில் (!snritems)
    திரும்பு;

  fputc ('\n', out);

  rule மாறிலி *previous_rule = வெற்று;
  க்கு (size_t i = 0; i < snritems; i++)
    {
      item_எண் *sp1 = ritem + sitems[i];
      rule மாறிலி *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      எனில் (report_flag & report_lookaheads
          && item_எண்_is_rule_எண் (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

நிலையான வெறுமை
print_transitions (மாறிலி state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  க்கு (முழு i = 0; i < trans->இலக்கம்; i++)
    எனில் (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        குறியீடு *sym = symbols[TRANSITION_குறியீடு (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  எனில் (!width)
    திரும்பு;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  க்கு (முழு i = 0; i < trans->இலக்கம்; i++)
    எனில் (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        குறியீடு *sym = symbols[TRANSITION_குறியீடு (trans, i)];
        மாறிலி எழுத்து *tag = sym->tag;
        மாறிலி state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        க்கு (முழு j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        எனில் (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->எண்);
        இல்லையெனில்
          fprintf (out, _("go to state %d\n"), s1->எண்);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

நிலையான வெறுமை
print_errs (FILE *out, மாறிலி state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  க்கு (முழு i = 0; i < errp->இலக்கம்; ++i)
    எனில் (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  எனில் (!width)
    திரும்பு;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  க்கு (முழு i = 0; i < errp->இலக்கம்; ++i)
    எனில் (errp->symbols[i])
      {
        மாறிலி எழுத்து *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        க்கு (முழு j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
print_reduction (FILE *out, size_t width,
                 மாறிலி எழுத்து *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  க்கு (முழு j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  எனில் (!enabled)
    fputc ('[', out);
  எனில் (rule_is_initial (r))
    fprintf (out, _("accept"));
  இல்லையெனில்
    fprintf (out, _("reduce using rule %d (%s)"), r->எண்,
             r->lhs->குறியீடு->tag);
  எனில் (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

நிலையான வெறுமை
print_reductions (FILE *out, மாறிலி state *s)
{
  reductions *reds = s->reductions;
  எனில் (reds->இலக்கம் == 0)
    திரும்பு;

  rule *default_reduction = வெற்று;
  எனில் (yydefact[s->எண்] != 0)
    default_reduction = &rules[yydefact[s->எண்] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    முழு i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_குறியீடு (trans, i));
  }
  க்கு (முழு i = 0; i < s->errs->இலக்கம்; ++i)
    எனில் (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->எண்);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  எனில் (default_reduction)
    width = mbswidth (_("$default"), 0);

  எனில் (reds->lookaheads)
    க்கு (முழு i = 0; i < ntokens; i++)
      {
        bool மொத்தம் = bitset_test (no_reduce_set, i);

        க்கு (முழு j = 0; j < reds->இலக்கம்; ++j)
          எனில் (bitset_test (reds->lookaheads[j], i))
            {
              எனில் (! மொத்தம்)
                {
                  எனில் (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  மொத்தம் = true;
                }
              இல்லையெனில்
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  எனில் (!width)
    திரும்பு;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  எனில் (reds->lookaheads)
    க்கு (முழு i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool மொத்தம் = bitset_test (no_reduce_set, i);
        எனில் (மொத்தம்)
          default_reduction_only = false;

        க்கு (முழு j = 0; j < reds->இலக்கம்; ++j)
          எனில் (bitset_test (reds->lookaheads[j], i))
            {
              எனில் (! மொத்தம்)
                {
                  எனில் (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  இல்லையெனில்
                    defaulted = true;
                  மொத்தம் = true;
                }
              இல்லையெனில்
                {
                  default_reduction_only = false;
                  எனில் (defaulted)
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

  எனில் (default_reduction)
    {
      எழுத்து *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->இலக்கம் == 1 && rule_is_initial (reds->rules[0])));
      (வெறுமை) default_reduction_only;
      விடுவி (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

நிலையான வெறுமை
print_actions (FILE *out, மாறிலி state *s)
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

நிலையான வெறுமை
print_state (FILE *out, மாறிலி state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->எண்);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  எனில் ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  எனில் (has_conflicts (s)
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

நிலையான வெறுமை
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  க்கு (முழு i = 0; i < max_code + 1; ++i)
    எனில் (token_translations[i] != undeftoken->content->எண்)
      {
        மாறிலி குறியீடு *sym = symbols[token_translations[i]];
        மாறிலி எழுத்து *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        எனில் (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        க்கு (rule_எண் r = 0; r < nrules; r++)
          க்கு (item_எண் *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            எனில் (item_எண்_as_குறியீடு_எண் (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                நிறுத்து;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


நிலையான வெறுமை
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; i++)
    {
      மாறிலி குறியீடு *sym = symbols[i];
      மாறிலி எழுத்து *tag = sym->tag;
      bool on_இடது = false;
      bool on_வலது = false;

      க்கு (rule_எண் r = 0; r < nrules; r++)
        {
          on_இடது |= rules[r].lhs->எண் == i;
          க்கு (item_எண் *rhsp = rules[r].rhs; !on_வலது && 0 <= *rhsp; ++rhsp)
            on_வலது |= item_எண்_as_குறியீடு_எண் (*rhsp) == i;
          எனில் (on_இடது && on_வலது)
            நிறுத்து;
        }

      fprintf (out, "%4s%s", "", tag);
      எனில் (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      எனில் (on_இடது)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          க்கு (rule_எண் r = 0; r < nrules; r++)
            எனில் (rules[r].lhs->எண் == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      எனில் (on_வலது)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          க்கு (rule_எண் r = 0; r < nrules; r++)
            க்கு (item_எண் *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              எனில் (item_எண்_as_குறியீடு_எண் (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  நிறுத்து;
                }
          fputc ('\n', out);
        }
    }
}

வெறுமை
print_results (வெறுமை)
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
  க்கு (state_எண் i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
