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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "print.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>
#અન્તર્ભાવ <mbswidth.h>

#અન્તર્ભાવ "closure.h"
#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "counterexample.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "state.h"
#અન્તર્ભાવ "symtab.h"
#અન્તર્ભાવ "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
સ્થિર bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

સ્થિર શૂન્ય
max_length (size_t *width, અચલ અક્ષર *str)
{
  size_t len = mbswidth (str, 0);
  જો (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

સ્થિર શૂન્ય
print_core (FILE *out, અચલ state *s)
{
  અચલ item_index *sitems = s->વસ્તુઓ;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  જો (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  જો (!snritems)
    પાછા;

  fputc ('\n', out);

  rule અચલ *previous_rule = ખાલી;
  માટે (size_t i = 0; i < snritems; i++)
    {
      item_સંખ્યા *sp1 = ritem + sitems[i];
      rule અચલ *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      જો (report_flag & report_lookaheads
          && item_સંખ્યા_is_rule_સંખ્યા (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

સ્થિર શૂન્ય
print_transitions (અચલ state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  માટે (પૂર્ણાંક i = 0; i < trans->અંક; i++)
    જો (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        પ્રતીક *sym = symbols[TRANSITION_પ્રતીક (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  જો (!width)
    પાછા;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  માટે (પૂર્ણાંક i = 0; i < trans->અંક; i++)
    જો (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        પ્રતીક *sym = symbols[TRANSITION_પ્રતીક (trans, i)];
        અચલ અક્ષર *tag = sym->tag;
        અચલ state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        માટે (પૂર્ણાંક j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        જો (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->સંખ્યા);
        નહીંતર
          fprintf (out, _("go to state %d\n"), s1->સંખ્યા);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

સ્થિર શૂન્ય
print_errs (FILE *out, અચલ state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  માટે (પૂર્ણાંક i = 0; i < errp->અંક; ++i)
    જો (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  જો (!width)
    પાછા;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  માટે (પૂર્ણાંક i = 0; i < errp->અંક; ++i)
    જો (errp->symbols[i])
      {
        અચલ અક્ષર *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        માટે (પૂર્ણાંક j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
print_reduction (FILE *out, size_t width,
                 અચલ અક્ષર *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  માટે (પૂર્ણાંક j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  જો (!enabled)
    fputc ('[', out);
  જો (rule_is_initial (r))
    fprintf (out, _("accept"));
  નહીંતર
    fprintf (out, _("reduce using rule %d (%s)"), r->સંખ્યા,
             r->lhs->પ્રતીક->tag);
  જો (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

સ્થિર શૂન્ય
print_reductions (FILE *out, અચલ state *s)
{
  reductions *reds = s->reductions;
  જો (reds->અંક == 0)
    પાછા;

  rule *default_reduction = ખાલી;
  જો (yydefact[s->સંખ્યા] != 0)
    default_reduction = &rules[yydefact[s->સંખ્યા] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    પૂર્ણાંક i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_પ્રતીક (trans, i));
  }
  માટે (પૂર્ણાંક i = 0; i < s->errs->અંક; ++i)
    જો (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->સંખ્યા);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  જો (default_reduction)
    width = mbswidth (_("$default"), 0);

  જો (reds->lookaheads)
    માટે (પૂર્ણાંક i = 0; i < ntokens; i++)
      {
        bool ગણતરી = bitset_test (no_reduce_set, i);

        માટે (પૂર્ણાંક j = 0; j < reds->અંક; ++j)
          જો (bitset_test (reds->lookaheads[j], i))
            {
              જો (! ગણતરી)
                {
                  જો (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  ગણતરી = true;
                }
              નહીંતર
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  જો (!width)
    પાછા;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  જો (reds->lookaheads)
    માટે (પૂર્ણાંક i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool ગણતરી = bitset_test (no_reduce_set, i);
        જો (ગણતરી)
          default_reduction_only = false;

        માટે (પૂર્ણાંક j = 0; j < reds->અંક; ++j)
          જો (bitset_test (reds->lookaheads[j], i))
            {
              જો (! ગણતરી)
                {
                  જો (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  નહીંતર
                    defaulted = true;
                  ગણતરી = true;
                }
              નહીંતર
                {
                  default_reduction_only = false;
                  જો (defaulted)
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

  જો (default_reduction)
    {
      અક્ષર *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->અંક == 1 && rule_is_initial (reds->rules[0])));
      (શૂન્ય) default_reduction_only;
      મુક્ત (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

સ્થિર શૂન્ય
print_actions (FILE *out, અચલ state *s)
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

સ્થિર શૂન્ય
print_state (FILE *out, અચલ state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->સંખ્યા);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  જો ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  જો (has_conflicts (s)
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

સ્થિર શૂન્ય
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  માટે (પૂર્ણાંક i = 0; i < max_code + 1; ++i)
    જો (token_translations[i] != undeftoken->content->સંખ્યા)
      {
        અચલ પ્રતીક *sym = symbols[token_translations[i]];
        અચલ અક્ષર *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        જો (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        માટે (rule_સંખ્યા r = 0; r < nrules; r++)
          માટે (item_સંખ્યા *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            જો (item_સંખ્યા_as_પ્રતીક_સંખ્યા (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                તોડો;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


સ્થિર શૂન્ય
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; i++)
    {
      અચલ પ્રતીક *sym = symbols[i];
      અચલ અક્ષર *tag = sym->tag;
      bool on_ડાબું = false;
      bool on_જમણું = false;

      માટે (rule_સંખ્યા r = 0; r < nrules; r++)
        {
          on_ડાબું |= rules[r].lhs->સંખ્યા == i;
          માટે (item_સંખ્યા *rhsp = rules[r].rhs; !on_જમણું && 0 <= *rhsp; ++rhsp)
            on_જમણું |= item_સંખ્યા_as_પ્રતીક_સંખ્યા (*rhsp) == i;
          જો (on_ડાબું && on_જમણું)
            તોડો;
        }

      fprintf (out, "%4s%s", "", tag);
      જો (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      જો (on_ડાબું)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          માટે (rule_સંખ્યા r = 0; r < nrules; r++)
            જો (rules[r].lhs->સંખ્યા == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      જો (on_જમણું)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          માટે (rule_સંખ્યા r = 0; r < nrules; r++)
            માટે (item_સંખ્યા *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              જો (item_સંખ્યા_as_પ્રતીક_સંખ્યા (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  તોડો;
                }
          fputc ('\n', out);
        }
    }
}

શૂન્ય
print_results (શૂન્ય)
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
  માટે (state_સંખ્યા i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
