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

#include <config.h>

#include "print.h"

#include "system.h"

#include <bitset.h>
#include <mbswidth.h>

#include "closure.h"
#include "complain.h"
#include "conflicts.h"
#include "counterexample.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "lalr.h"
#include "lr0.h"
#include "muscle-tab.h"
#include "reader.h"
#include "reduce.h"
#include "state.h"
#include "symtab.h"
#include "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
static bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

static void
max_length (size_t *width, const char *str)
{
  size_t len = mbswidth (str, 0);
  if (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

static void
इलिम्बाग_core (FILE *out, const state *s)
{
  const item_index *sitems = s->मगा_बागाय;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  if (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  if (!snritems)
    return;

  fputc ('\n', out);

  rule const *previous_rule = NULL;
  for (size_t i = 0; i < snritems; i++)
    {
      item_नुमेरो *sp1 = ritem + sitems[i];
      rule const *r = item_rule (sp1);
      item_इलिम्बाग (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      if (report_flag & report_lookaheads
          && item_नुमेरो_is_rule_नुमेरो (*sp1))
        state_rule_lookaheads_इलिम्बाग (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

static void
इलिम्बाग_transitions (const state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  for (int i = 0; i < trans->नुमेरो; i++)
    if (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        सिम्बोलो *sym = symbols[TRANSITION_सिम्बोलो (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  if (!width)
    return;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  for (int i = 0; i < trans->नुमेरो; i++)
    if (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        सिम्बोलो *sym = symbols[TRANSITION_सिम्बोलो (trans, i)];
        const char *tag = sym->tag;
        const state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        for (int j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        if (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->नुमेरो_2);
        else
          fprintf (out, _("go to state %d\n"), s1->नुमेरो_2);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

static void
इलिम्बाग_errs (FILE *out, const state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  for (int i = 0; i < errp->नुमेरो; ++i)
    if (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  if (!width)
    return;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  for (int i = 0; i < errp->नुमेरो; ++i)
    if (errp->symbols[i])
      {
        const char *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        for (int j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

static void
इलिम्बाग_reduction (FILE *out, size_t width,
                 const char *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  for (int j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  if (!enabled)
    fputc ('[', out);
  if (rule_is_initial (r))
    fprintf (out, _("accept"));
  else
    fprintf (out, _("reduce using rule %d (%s)"), r->नुमेरो_2,
             r->lhs->सिम्बोलो->tag);
  if (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

static void
इलिम्बाग_reductions (FILE *out, const state *s)
{
  reductions *reds = s->reductions;
  if (reds->नुमेरो == 0)
    return;

  rule *default_reduction = NULL;
  if (yydefact[s->नुमेरो_2] != 0)
    default_reduction = &rules[yydefact[s->नुमेरो_2] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    int i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_सिम्बोलो (trans, i));
  }
  for (int i = 0; i < s->errs->नुमेरो; ++i)
    if (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->नुमेरो_2);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  if (default_reduction)
    width = mbswidth (_("$default"), 0);

  if (reds->lookaheads)
    for (int i = 0; i < ntokens; i++)
      {
        bool बिलांग = bitset_test (no_reduce_set, i);

        for (int j = 0; j < reds->नुमेरो; ++j)
          if (bitset_test (reds->lookaheads[j], i))
            {
              if (! बिलांग)
                {
                  if (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  बिलांग = true;
                }
              else
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  if (!width)
    return;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  if (reds->lookaheads)
    for (int i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool बिलांग = bitset_test (no_reduce_set, i);
        if (बिलांग)
          default_reduction_only = false;

        for (int j = 0; j < reds->नुमेरो; ++j)
          if (bitset_test (reds->lookaheads[j], i))
            {
              if (! बिलांग)
                {
                  if (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      इलिम्बाग_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  else
                    defaulted = true;
                  बिलांग = true;
                }
              else
                {
                  default_reduction_only = false;
                  if (defaulted)
                    इलिम्बाग_reduction (out, width,
                                     symbols[i]->tag,
                                     default_reduction, true);
                  defaulted = false;
                  इलिम्बाग_reduction (out, width,
                                   symbols[i]->tag,
                                   reds->rules[j], false);
                }
            }
      }

  if (default_reduction)
    {
      char *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      इलिम्बाग_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->नुमेरो == 1 && rule_is_initial (reds->rules[0])));
      (void) default_reduction_only;
      free (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

static void
इलिम्बाग_actions (FILE *out, const state *s)
{
  /* Print shifts.  */
  इलिम्बाग_transitions (s, out, true);
  इलिम्बाग_errs (out, s);
  इलिम्बाग_reductions (out, s);
  /* Print gotos.  */
  इलिम्बाग_transitions (s, out, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

static void
इलिम्बाग_state (FILE *out, const state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->नुमेरो_2);
  fputc ('\n', out);
  इलिम्बाग_core (out, s);
  इलिम्बाग_actions (out, s);
  if ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  if (has_conflicts (s)
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

static void
इलिम्बाग_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  for (int i = 0; i < max_code + 1; ++i)
    if (token_translations[i] != undeftoken->content->नुमेरो_2)
      {
        const सिम्बोलो *sym = symbols[token_translations[i]];
        const char *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        if (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        for (rule_नुमेरो r = 0; r < nrules; r++)
          for (item_नुमेरो *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            if (item_नुमेरो_as_सिम्बोलो_नुमेरो (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                break;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


static void
इलिम्बाग_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  for (सिम्बोलो_नुमेरो i = ntokens; i < nsyms; i++)
    {
      const सिम्बोलो *sym = symbols[i];
      const char *tag = sym->tag;
      bool on_कालिवा = false;
      bool on_कानान = false;

      for (rule_नुमेरो r = 0; r < nrules; r++)
        {
          on_कालिवा |= rules[r].lhs->नुमेरो_2 == i;
          for (item_नुमेरो *rhsp = rules[r].rhs; !on_कानान && 0 <= *rhsp; ++rhsp)
            on_कानान |= item_नुमेरो_as_सिम्बोलो_नुमेरो (*rhsp) == i;
          if (on_कालिवा && on_कानान)
            break;
        }

      fprintf (out, "%4s%s", "", tag);
      if (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      if (on_कालिवा)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          for (rule_नुमेरो r = 0; r < nrules; r++)
            if (rules[r].lhs->नुमेरो_2 == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      if (on_कानान)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          for (rule_नुमेरो r = 0; r < nrules; r++)
            for (item_नुमेरो *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              if (item_नुमेरो_as_सिम्बोलो_नुमेरो (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  break;
                }
          fputc ('\n', out);
        }
    }
}

void
इलिम्बाग_results (void)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *out = xfopen (spec_verbose_file, "w");

  reduce_output (out);
  grammar_rules_partial_इलिम्बाग (out,
                               _("Rules useless in parser due to conflicts"),
                               rule_useless_in_parser_p);
  conflicts_output (out);

  grammar_rules_इलिम्बाग (out);
  इलिम्बाग_terminal_symbols (out);
  इलिम्बाग_nonterminal_symbols (out);

  /* Storage for print_reductions.  */
  no_reduce_set = bitset_create (ntokens, BITSET_FIXED);
  for (state_नुमेरो i = 0; i < nstates; i++)
    इलिम्बाग_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
