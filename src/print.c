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
  ако (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

static void
print_core (FILE *out, const state *s)
{
  const item_index *sitems = s->items;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  ако (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  ако (!snritems)
    върни;

  fputc ('\n', out);

  rule const *previous_rule = NULL;
  за (size_t i = 0; i < snritems; i++)
    {
      item_number *sp1 = ritem + sitems[i];
      rule const *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      ако (report_flag & report_lookaheads
          && item_number_is_rule_number (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

static void
print_transitions (const state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  за (цяло i = 0; i < trans->num; i++)
    ако (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        symbol *sym = symbols[TRANSITION_SYMBOL (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  ако (!width)
    върни;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  за (цяло i = 0; i < trans->num; i++)
    ако (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        symbol *sym = symbols[TRANSITION_SYMBOL (trans, i)];
        const char *tag = sym->tag;
        const state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        за (цяло j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        ако (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->number);
        иначе
          fprintf (out, _("go to state %d\n"), s1->number);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

static void
print_errs (FILE *out, const state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  за (цяло i = 0; i < errp->num; ++i)
    ако (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  ако (!width)
    върни;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  за (цяло i = 0; i < errp->num; ++i)
    ако (errp->symbols[i])
      {
        const char *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        за (цяло j = width - mbswidth (tag, 0); j > 0; --j)
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
print_reduction (FILE *out, size_t width,
                 const char *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  за (цяло j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  ако (!enabled)
    fputc ('[', out);
  ако (rule_is_initial (r))
    fprintf (out, _("accept"));
  иначе
    fprintf (out, _("reduce using rule %d (%s)"), r->number,
             r->lhs->symbol->tag);
  ако (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

static void
print_reductions (FILE *out, const state *s)
{
  reductions *reds = s->reductions;
  ако (reds->num == 0)
    върни;

  rule *default_reduction = NULL;
  ако (yydefact[s->number] != 0)
    default_reduction = &rules[yydefact[s->number] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    цяло i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_SYMBOL (trans, i));
  }
  за (цяло i = 0; i < s->errs->num; ++i)
    ако (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->number);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  ако (default_reduction)
    width = mbswidth (_("$default"), 0);

  ако (reds->lookaheads)
    за (цяло i = 0; i < ntokens; i++)
      {
        bool count = bitset_test (no_reduce_set, i);

        за (цяло j = 0; j < reds->num; ++j)
          ако (bitset_test (reds->lookaheads[j], i))
            {
              ако (! count)
                {
                  ако (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  count = true;
                }
              иначе
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  ако (!width)
    върни;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  ако (reds->lookaheads)
    за (цяло i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool count = bitset_test (no_reduce_set, i);
        ако (count)
          default_reduction_only = false;

        за (цяло j = 0; j < reds->num; ++j)
          ако (bitset_test (reds->lookaheads[j], i))
            {
              ако (! count)
                {
                  ако (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  иначе
                    defaulted = true;
                  count = true;
                }
              иначе
                {
                  default_reduction_only = false;
                  ако (defaulted)
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

  ако (default_reduction)
    {
      char *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->num == 1 && rule_is_initial (reds->rules[0])));
      (void) default_reduction_only;
      free (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

static void
print_actions (FILE *out, const state *s)
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

static void
print_state (FILE *out, const state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->number);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  ако ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  ако (has_conflicts (s)
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
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  за (цяло i = 0; i < max_code + 1; ++i)
    ако (token_translations[i] != undeftoken->content->number)
      {
        const symbol *sym = symbols[token_translations[i]];
        const char *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        ако (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        за (rule_number r = 0; r < nrules; r++)
          за (item_number *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            ако (item_number_as_symbol_number (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                break;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


static void
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  за (symbol_number i = ntokens; i < nsyms; i++)
    {
      const symbol *sym = symbols[i];
      const char *tag = sym->tag;
      bool on_left = false;
      bool on_right = false;

      за (rule_number r = 0; r < nrules; r++)
        {
          on_left |= rules[r].lhs->number == i;
          за (item_number *rhsp = rules[r].rhs; !on_right && 0 <= *rhsp; ++rhsp)
            on_right |= item_number_as_symbol_number (*rhsp) == i;
          ако (on_left && on_right)
            break;
        }

      fprintf (out, "%4s%s", "", tag);
      ако (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      ако (on_left)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          за (rule_number r = 0; r < nrules; r++)
            ако (rules[r].lhs->number == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      ако (on_right)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          за (rule_number r = 0; r < nrules; r++)
            за (item_number *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              ако (item_number_as_symbol_number (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  break;
                }
          fputc ('\n', out);
        }
    }
}

void
print_results (void)
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
  за (state_number i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
