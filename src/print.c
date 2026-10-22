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

#अन्तर्भाव <config.h>

#अन्तर्भाव "print.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <mbswidth.h>

#अन्तर्भाव "closure.h"
#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "counterexample.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "symtab.h"
#अन्तर्भाव "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
स्थिर bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

स्थिर शून्य
max_length (size_t *width, ध्रुव वर्ण *str)
{
  size_t len = mbswidth (str, 0);
  जइ (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (FILE *out, ध्रुव state *s)
{
  ध्रुव item_index *sitems = s->वस्तूनि;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  जइ (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  जइ (!snritems)
    प्रत्यागम;

  fputc ('\n', out);

  rule ध्रुव *previous_rule = रिक्त;
  प्रति (size_t i = 0; i < snritems; i++)
    {
      item_सङ्ख्या *sp1 = ritem + sitems[i];
      rule ध्रुव *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      जइ (report_flag & report_lookaheads
          && item_सङ्ख्या_is_rule_सङ्ख्या (*sp1))
        state_rule_lookaheads_print (s, r, out);
      fputc ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

स्थिर शून्य
print_transitions (ध्रुव state *s, FILE *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; i++)
    जइ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  जइ (!width)
    प्रत्यागम;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; i++)
    जइ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        ध्रुव वर्ण *tag = sym->tag;
        ध्रुव state *s1 = trans->states[i];

        fprintf (out, "    %s", tag);
        प्रति (पूर्णाङ्क j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        जइ (display_transitions_p)
          fprintf (out, _("shift, and go to state %d\n"), s1->सङ्ख्या);
        अण्णहा
          fprintf (out, _("go to state %d\n"), s1->सङ्ख्या);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

स्थिर शून्य
print_errs (FILE *out, ध्रुव state *s)
{
  errs *errp = s->errs;
  size_t width = 0;

  /* Compute the width of the lookahead token column.  */
  प्रति (पूर्णाङ्क i = 0; i < errp->अङ्कः; ++i)
    जइ (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  जइ (!width)
    प्रत्यागम;

  fputc ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  प्रति (पूर्णाङ्क i = 0; i < errp->अङ्कः; ++i)
    जइ (errp->symbols[i])
      {
        ध्रुव वर्ण *tag = errp->symbols[i]->tag;
        fprintf (out, "    %s", tag);
        प्रति (पूर्णाङ्क j = width - mbswidth (tag, 0); j > 0; --j)
          fputc (' ', out);
        fputs (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

स्थिर शून्य
print_reduction (FILE *out, size_t width,
                 ध्रुव वर्ण *lookahead,
                 rule *r, bool enabled)
{
  fprintf (out, "    %s", lookahead);
  प्रति (पूर्णाङ्क j = width - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', out);
  जइ (!enabled)
    fputc ('[', out);
  जइ (rule_is_initial (r))
    fprintf (out, _("accept"));
  अण्णहा
    fprintf (out, _("reduce using rule %d (%s)"), r->सङ्ख्या,
             r->lhs->चिह्नम्->tag);
  जइ (!enabled)
    fputc (']', out);
  fputc ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

स्थिर शून्य
print_reductions (FILE *out, ध्रुव state *s)
{
  reductions *reds = s->reductions;
  जइ (reds->अङ्कः == 0)
    प्रत्यागम;

  rule *default_reduction = रिक्त;
  जइ (yydefact[s->सङ्ख्या] != 0)
    default_reduction = &rules[yydefact[s->सङ्ख्या] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    पूर्णाङ्क i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_चिह्नम् (trans, i));
  }
  प्रति (पूर्णाङ्क i = 0; i < s->errs->अङ्कः; ++i)
    जइ (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->सङ्ख्या);

  /* Compute the width of the lookahead token column.  */
  size_t width = 0;
  जइ (default_reduction)
    width = mbswidth (_("$default"), 0);

  जइ (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < ntokens; i++)
      {
        bool गणनम् = bitset_test (no_reduce_set, i);

        प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
          जइ (bitset_test (reds->lookaheads[j], i))
            {
              जइ (! गणनम्)
                {
                  जइ (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  गणनम् = true;
                }
              अण्णहा
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  जइ (!width)
    प्रत्यागम;

  fputc ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  जइ (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गणनम् = bitset_test (no_reduce_set, i);
        जइ (गणनम्)
          default_reduction_only = false;

        प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
          जइ (bitset_test (reds->lookaheads[j], i))
            {
              जइ (! गणनम्)
                {
                  जइ (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  अण्णहा
                    defaulted = true;
                  गणनम् = true;
                }
              अण्णहा
                {
                  default_reduction_only = false;
                  जइ (defaulted)
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

  जइ (default_reduction)
    {
      वर्ण *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->अङ्कः == 1 && rule_is_initial (reds->rules[0])));
      (शून्य) default_reduction_only;
      मुक्त (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
print_actions (FILE *out, ध्रुव state *s)
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

स्थिर शून्य
print_state (FILE *out, ध्रुव state *s)
{
  fputs ("\n\n", out);
  fprintf (out, _("State %d"), s->सङ्ख्या);
  fputc ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  जइ ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', out);
      fputs (s->solved_conflicts, out);
    }
  जइ (has_conflicts (s)
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

स्थिर शून्य
print_terminal_symbols (FILE *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (out, "%s\n\n", _("Terminals, with rules where they appear"));
  प्रति (पूर्णाङ्क i = 0; i < max_code + 1; ++i)
    जइ (token_translations[i] != undeftoken->content->सङ्ख्या)
      {
        ध्रुव चिह्नम् *sym = symbols[token_translations[i]];
        ध्रुव वर्ण *tag = sym->tag;
        fprintf (out, "%4s%s", "", tag);
        जइ (sym->content->type_name)
          fprintf (out, " <%s>", sym->content->type_name);
        fprintf (out, " (%d)", i);

        प्रति (rule_सङ्ख्या r = 0; r < nrules; r++)
          प्रति (item_सङ्ख्या *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            जइ (item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp) == token_translations[i])
              {
                fprintf (out, " %d", r);
                भङ्ग;
              }
        fputc ('\n', out);
      }
  fputs ("\n\n", out);
}


स्थिर शून्य
print_nonterminal_symbols (FILE *out)
{
  fprintf (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; i++)
    {
      ध्रुव चिह्नम् *sym = symbols[i];
      ध्रुव वर्ण *tag = sym->tag;
      bool on_वामः = false;
      bool on_दक्षिणः = false;

      प्रति (rule_सङ्ख्या r = 0; r < nrules; r++)
        {
          on_वामः |= rules[r].lhs->सङ्ख्या == i;
          प्रति (item_सङ्ख्या *rhsp = rules[r].rhs; !on_दक्षिणः && 0 <= *rhsp; ++rhsp)
            on_दक्षिणः |= item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp) == i;
          जइ (on_वामः && on_दक्षिणः)
            भङ्ग;
        }

      fprintf (out, "%4s%s", "", tag);
      जइ (sym->content->type_name)
        fprintf (out, " <%s>", sym->content->type_name);
      fprintf (out, " (%d)\n", i);

      जइ (on_वामः)
        {
          fprintf (out, "%8s%s", "", _("on left:"));
          प्रति (rule_सङ्ख्या r = 0; r < nrules; r++)
            जइ (rules[r].lhs->सङ्ख्या == i)
              fprintf (out, " %d", r);
          fputc ('\n', out);
        }

      जइ (on_दक्षिणः)
        {
          fprintf (out, "%8s%s", "", _("on right:"));
          प्रति (rule_सङ्ख्या r = 0; r < nrules; r++)
            प्रति (item_सङ्ख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              जइ (item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp) == i)
                {
                  fprintf (out, " %d", r);
                  भङ्ग;
                }
          fputc ('\n', out);
        }
    }
}

शून्य
print_results (शून्य)
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
  प्रति (state_सङ्ख्या i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
