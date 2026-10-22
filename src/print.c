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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "print.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>
#അന്തര്ഭാവ <mbswidth.h>

#അന്തര്ഭാവ "closure.h"
#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "counterexample.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "reduce.h"
#അന്തര്ഭാവ "state.h"
#അന്തര്ഭാവ "symtab.h"
#അന്തര്ഭാവ "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
സ്ഥിതം bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

സ്ഥിതം ശൂന്യം
max_length (size_t *width, സ്ഥിരം അക്ഷരം *str)
{
  size_t len = mbswidth (str, 0);
  എങ്കിൽ (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

സ്ഥിതം ശൂന്യം
print_core (FILE *out, സ്ഥിരം state *s)
{
  സ്ഥിരം item_index *sitems = s->ഇനങ്ങൾ;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  എങ്കിൽ (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  എങ്കിൽ (!snritems)
    തിരികെ;

  fputc ('\n', out);

  rule സ്ഥിരം *previous_rule = ശൂന്യ;
  വേണ്ടി (size_t i = 0; i < snritems; i++)
    {
      item_സംഖ്യ *sp1 = ritem + sitems[i];
      rule സ്ഥിരം *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      എങ്കിൽ (report_flag & report_lookaheads
          && item_സംഖ്യ_is_rule_സംഖ്യ (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_transitions (സ്ഥിരം state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < trans->അക്കം; i++)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        ചിഹ്നം *sym = symbols[TRANSITION_ചിഹ്നം (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  എങ്കിൽ (!width)
    തിരികെ;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < trans->അക്കം; i++)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        ചിഹ്നം *sym = symbols[TRANSITION_ചിഹ്നം (trans, i)];
        സ്ഥിരം അക്ഷരം *tag = sym->tag;
        സ്ഥിരം state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        എങ്കിൽ (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->സംഖ്യ);
        അല്ലെങ്കിൽ
          fprintf (out, _("go to state %d\n"), s1->സംഖ്യ);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_errs (FILE *out, സ്ഥിരം state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < errp->അക്കം; ++i)
    എങ്കിൽ (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  എങ്കിൽ (!width)
    തിരികെ;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < errp->അക്കം; ++i)
    എങ്കിൽ (errp->symbols[i])
      {
        സ്ഥിരം അക്ഷരം *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_reduction (FILE *out, size_t width,
                 സ്ഥിരം അക്ഷരം *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  വേണ്ടി (പൂർണ്ണസംഖ്യ j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  എങ്കിൽ (!enabled)
    fputc ('[', out);
  എങ്കിൽ (rule_is_initial (r))
    fprintf (out, _("accept"));
  അല്ലെങ്കിൽ
    fprintf (out, _("reduce using rule %d (%s)"), r->സംഖ്യ,
             r->lhs->ചിഹ്നം->tag);
  എങ്കിൽ (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_reductions (FILE *out, സ്ഥിരം state *s)
{
  reductions *reds = s->reductions;
  എങ്കിൽ (reds->അക്കം == 0)
    തിരികെ;

  rule *default_reduction = ശൂന്യ;
  എങ്കിൽ (yydefact[s->സംഖ്യ] != 0)
    default_reduction = &rules[yydefact[s->സംഖ്യ] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    പൂർണ്ണസംഖ്യ i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_ചിഹ്നം (trans, i));
  }
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < s->errs->അക്കം; ++i)
    എങ്കിൽ (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->സംഖ്യ);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  എങ്കിൽ (default_reduction)
    width = mbswidth (_("$default"), 0);

  എങ്കിൽ (reds->lookaheads)
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; i++)
      {
        bool എണ്ണം = bitset_test (no_reduce_set, i);

        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < reds->അക്കം; ++j)
          എങ്കിൽ (bitset_test (reds->lookaheads[j], i))
            {
              എങ്കിൽ (! എണ്ണം)
                {
                  എങ്കിൽ (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  എണ്ണം = true;
                }
              അല്ലെങ്കിൽ
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  എങ്കിൽ (!width)
    തിരികെ;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  എങ്കിൽ (reds->lookaheads)
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool എണ്ണം = bitset_test (no_reduce_set, i);
        എങ്കിൽ (എണ്ണം)
          default_reduction_only = false;

        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < reds->അക്കം; ++j)
          എങ്കിൽ (bitset_test (reds->lookaheads[j], i))
            {
              എങ്കിൽ (! എണ്ണം)
                {
                  എങ്കിൽ (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  അല്ലെങ്കിൽ
                    defaulted = true;
                  എണ്ണം = true;
                }
              അല്ലെങ്കിൽ
                {
                  default_reduction_only = false;
                  എങ്കിൽ (defaulted)
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

  എങ്കിൽ (default_reduction)
    {
      അക്ഷരം *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->അക്കം == 1 && rule_is_initial (reds->rules[0])));
      (ശൂന്യം) default_reduction_only;
      സ്വതന്ത്രം (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
print_actions (FILE *out, സ്ഥിരം state *s)
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

സ്ഥിതം ശൂന്യം
print_state (FILE *out, സ്ഥിരം state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->സംഖ്യ);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  എങ്കിൽ ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  എങ്കിൽ (has_conflicts (s)
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

സ്ഥിതം ശൂന്യം
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < max_code + 1; ++i)
    എങ്കിൽ (token_translations[i] != undeftoken->content->സംഖ്യ)
      {
        സ്ഥിരം ചിഹ്നം *sym = symbols[token_translations[i]];
        സ്ഥിരം അക്ഷരം *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        എങ്കിൽ (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; r++)
          വേണ്ടി (item_സംഖ്യ *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            എങ്കിൽ (item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                നിർത്തുക;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


സ്ഥിതം ശൂന്യം
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; i++)
    {
      സ്ഥിരം ചിഹ്നം *sym = symbols[i];
      സ്ഥിരം അക്ഷരം *tag = sym->tag;
      bool on_ഇടത് = false;
      bool on_വലത് = false;

      വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; r++)
        {
          on_ഇടത് |= rules[r].lhs->സംഖ്യ == i;
          വേണ്ടി (item_സംഖ്യ *rhsp = rules[r].rhs; !on_വലത് && 0 <= *rhsp; ++rhsp)
            on_വലത് |= item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*rhsp) == i;
          എങ്കിൽ (on_ഇടത് && on_വലത്)
            നിർത്തുക;
        }

      fprintf (out, "%4s%s", "", tag);
      എങ്കിൽ (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      എങ്കിൽ (on_ഇടത്)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; r++)
            എങ്കിൽ (rules[r].lhs->സംഖ്യ == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      എങ്കിൽ (on_വലത്)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; r++)
            വേണ്ടി (item_സംഖ്യ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              എങ്കിൽ (item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  നിർത്തുക;
                }
          fputc ('\n', out);
        }
    }
}

ശൂന്യം
print_results (ശൂന്യം)
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
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
