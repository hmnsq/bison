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
स्थिर bitset no_reduce_समुच्चयः;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

स्थिर शून्य
max_दीर्घता (size_t *विस्तारः, ध्रुव वर्ण *str)
{
  size_t len = mbswidth (str, 0);
  यदि (len > *विस्तारः)
    *विस्तारः = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (FILE *बहिर्गमनम्, ध्रुव state *s)
{
  ध्रुव item_सूचकांक *sitems = s->वस्तूनि;
  size_t snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  यदि (report_flag & report_itemsets)
    {
      संवृतिः (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  यदि (!snritems)
    प्रत्यागम;

  fputc ('\n', बहिर्गमनम्);

  नियमः ध्रुव *previous_नियमः = रिक्त;
  प्रति (size_t i = 0; i < snritems; i++)
    {
      item_सङ्ख्या *sp1 = ritem + sitems[i];
      नियमः ध्रुव *r = item_नियमः (sp1);
      item_print (sp1, previous_नियमः, बहिर्गमनम्);
      previous_नियमः = r;

      /* Display the lookahead tokens?  */
      यदि (report_flag & report_lookaheads
          && item_सङ्ख्या_is_नियमः_सङ्ख्या (*sp1))
        state_नियमः_lookaheads_print (s, r, बहिर्गमनम्);
      fputc ('\n', बहिर्गमनम्);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

स्थिर शून्य
print_transitions (ध्रुव state *s, FILE *बहिर्गमनम्, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  size_t विस्तारः = 0;

  /* Compute the width of the lookahead token column.  */
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_सरणम् (trans, i) == display_transitions_p)
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        max_दीर्घता (&विस्तारः, sym->चिह्नकम्);
      }

  /* Nothing to report. */
  यदि (!विस्तारः)
    प्रत्यागम;

  fputc ('\n', बहिर्गमनम्);
  विस्तारः += 2;

  /* Report lookahead tokens and shifts.  */
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_सरणम् (trans, i) == display_transitions_p)
      {
        चिह्नम् *sym = symbols[TRANSITION_चिह्नम् (trans, i)];
        ध्रुव वर्ण *चिह्नकम् = sym->चिह्नकम्;
        ध्रुव state *s1 = trans->अवस्थाः[i];

        fprintf (बहिर्गमनम्, "    %s", चिह्नकम्);
        प्रति (पूर्णाङ्क j = विस्तारः - mbswidth (चिह्नकम्, 0); j > 0; --j)
          fputc (' ', बहिर्गमनम्);
        यदि (display_transitions_p)
          fprintf (बहिर्गमनम्, _("shift, and go to state %d\n"), s1->सङ्ख्या);
        अन्यथा
          fprintf (बहिर्गमनम्, _("go to state %d\n"), s1->सङ्ख्या);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

स्थिर शून्य
print_errs (FILE *बहिर्गमनम्, ध्रुव state *s)
{
  errs *errp = s->errs;
  size_t विस्तारः = 0;

  /* Compute the width of the lookahead token column.  */
  प्रति (पूर्णाङ्क i = 0; i < errp->अङ्कः; ++i)
    यदि (errp->symbols[i])
      max_दीर्घता (&विस्तारः, errp->symbols[i]->चिह्नकम्);

  /* Nothing to report. */
  यदि (!विस्तारः)
    प्रत्यागम;

  fputc ('\n', बहिर्गमनम्);
  विस्तारः += 2;

  /* Report lookahead tokens and errors.  */
  प्रति (पूर्णाङ्क i = 0; i < errp->अङ्कः; ++i)
    यदि (errp->symbols[i])
      {
        ध्रुव वर्ण *चिह्नकम् = errp->symbols[i]->चिह्नकम्;
        fprintf (बहिर्गमनम्, "    %s", चिह्नकम्);
        प्रति (पूर्णाङ्क j = विस्तारः - mbswidth (चिह्नकम्, 0); j > 0; --j)
          fputc (' ', बहिर्गमनम्);
        fputs (_("error (nonassociative)\n"), बहिर्गमनम्);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

स्थिर शून्य
print_reduction (FILE *बहिर्गमनम्, size_t विस्तारः,
                 ध्रुव वर्ण *lookahead,
                 नियमः *r, bool enabled)
{
  fprintf (बहिर्गमनम्, "    %s", lookahead);
  प्रति (पूर्णाङ्क j = विस्तारः - mbswidth (lookahead, 0); j > 0; --j)
    fputc (' ', बहिर्गमनम्);
  यदि (!enabled)
    fputc ('[', बहिर्गमनम्);
  यदि (नियमः_is_initial (r))
    fprintf (बहिर्गमनम्, _("accept"));
  अन्यथा
    fprintf (बहिर्गमनम्, _("reduce using rule %d (%s)"), r->सङ्ख्या,
             r->lhs->चिह्नम्->चिह्नकम्);
  यदि (!enabled)
    fputc (']', बहिर्गमनम्);
  fputc ('\n', बहिर्गमनम्);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

स्थिर शून्य
print_reductions (FILE *बहिर्गमनम्, ध्रुव state *s)
{
  reductions *reds = s->reductions;
  यदि (reds->अङ्कः == 0)
    प्रत्यागम;

  नियमः *default_reduction = रिक्त;
  यदि (yydefact[s->सङ्ख्या] != 0)
    default_reduction = &नियमाः[yydefact[s->सङ्ख्या] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_समुच्चयः);
  {
    पूर्णाङ्क i;
    FOR_EACH_सरणम् (trans, i)
      bitset_समुच्चयः (no_reduce_समुच्चयः, TRANSITION_चिह्नम् (trans, i));
  }
  प्रति (पूर्णाङ्क i = 0; i < s->errs->अङ्कः; ++i)
    यदि (s->errs->symbols[i])
      bitset_समुच्चयः (no_reduce_समुच्चयः, s->errs->symbols[i]->content->सङ्ख्या);

  /* Compute the width of the lookahead token column.  */
  size_t विस्तारः = 0;
  यदि (default_reduction)
    विस्तारः = mbswidth (_("$default"), 0);

  यदि (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < ntokens; i++)
      {
        bool गणनम् = bitset_test (no_reduce_समुच्चयः, i);

        प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गणनम्)
                {
                  यदि (reds->नियमाः[j] != default_reduction)
                    max_दीर्घता (&विस्तारः, symbols[i]->चिह्नकम्);
                  गणनम् = true;
                }
              अन्यथा
                max_दीर्घता (&विस्तारः, symbols[i]->चिह्नकम्);
            }
      }

  /* Nothing to report. */
  यदि (!विस्तारः)
    प्रत्यागम;

  fputc ('\n', बहिर्गमनम्);
  विस्तारः += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  यदि (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गणनम् = bitset_test (no_reduce_समुच्चयः, i);
        यदि (गणनम्)
          default_reduction_only = false;

        प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गणनम्)
                {
                  यदि (reds->नियमाः[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (बहिर्गमनम्, विस्तारः,
                                       symbols[i]->चिह्नकम्,
                                       reds->नियमाः[j], true);
                    }
                  अन्यथा
                    defaulted = true;
                  गणनम् = true;
                }
              अन्यथा
                {
                  default_reduction_only = false;
                  यदि (defaulted)
                    print_reduction (बहिर्गमनम्, विस्तारः,
                                     symbols[i]->चिह्नकम्,
                                     default_reduction, true);
                  defaulted = false;
                  print_reduction (बहिर्गमनम्, विस्तारः,
                                   symbols[i]->चिह्नकम्,
                                   reds->नियमाः[j], false);
                }
            }
      }

  यदि (default_reduction)
    {
      वर्ण *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (बहिर्गमनम्, विस्तारः, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->अङ्कः == 1 && नियमः_is_initial (reds->नियमाः[0])));
      (शून्य) default_reduction_only;
      मुक्त (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
print_actions (FILE *बहिर्गमनम्, ध्रुव state *s)
{
  /* Print shifts.  */
  print_transitions (s, बहिर्गमनम्, true);
  print_errs (बहिर्गमनम्, s);
  print_reductions (बहिर्गमनम्, s);
  /* Print gotos.  */
  print_transitions (s, बहिर्गमनम्, false);
}


/*----------------------------------.
| Report all the data on S on OUT.  |
`----------------------------------*/

स्थिर शून्य
print_state (FILE *बहिर्गमनम्, ध्रुव state *s)
{
  fputs ("\n\n", बहिर्गमनम्);
  fprintf (बहिर्गमनम्, _("State %d"), s->सङ्ख्या);
  fputc ('\n', बहिर्गमनम्);
  print_core (बहिर्गमनम्, s);
  print_actions (बहिर्गमनम्, s);
  यदि ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      fputc ('\n', बहिर्गमनम्);
      fputs (s->solved_conflicts, बहिर्गमनम्);
    }
  यदि (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      fputc ('\n', बहिर्गमनम्);
      counterexample_report_state (s, बहिर्गमनम्, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

स्थिर शून्य
print_terminal_symbols (FILE *बहिर्गमनम्)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  fprintf (बहिर्गमनम्, "%s\n\n", _("Terminals, with rules where they appear"));
  प्रति (पूर्णाङ्क i = 0; i < max_संहिता + 1; ++i)
    यदि (token_translations[i] != undeftoken->content->सङ्ख्या)
      {
        ध्रुव चिह्नम् *sym = symbols[token_translations[i]];
        ध्रुव वर्ण *चिह्नकम् = sym->चिह्नकम्;
        fprintf (बहिर्गमनम्, "%4s%s", "", चिह्नकम्);
        यदि (sym->content->type_name)
          fprintf (बहिर्गमनम्, " <%s>", sym->content->type_name);
        fprintf (बहिर्गमनम्, " (%d)", i);

        प्रति (नियमः_सङ्ख्या r = 0; r < nrules; r++)
          प्रति (item_सङ्ख्या *rhsp = नियमाः[r].rhs; *rhsp >= 0; rhsp++)
            यदि (item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp) == token_translations[i])
              {
                fprintf (बहिर्गमनम्, " %d", r);
                भङ्ग;
              }
        fputc ('\n', बहिर्गमनम्);
      }
  fputs ("\n\n", बहिर्गमनम्);
}


स्थिर शून्य
print_nonterminal_symbols (FILE *बहिर्गमनम्)
{
  fprintf (बहिर्गमनम्, "%s\n\n", _("Nonterminals, with rules where they appear"));
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; i++)
    {
      ध्रुव चिह्नम् *sym = symbols[i];
      ध्रुव वर्ण *चिह्नकम् = sym->चिह्नकम्;
      bool on_वामः = false;
      bool on_दक्षिणः = false;

      प्रति (नियमः_सङ्ख्या r = 0; r < nrules; r++)
        {
          on_वामः |= नियमाः[r].lhs->सङ्ख्या == i;
          प्रति (item_सङ्ख्या *rhsp = नियमाः[r].rhs; !on_दक्षिणः && 0 <= *rhsp; ++rhsp)
            on_दक्षिणः |= item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp) == i;
          यदि (on_वामः && on_दक्षिणः)
            भङ्ग;
        }

      fprintf (बहिर्गमनम्, "%4s%s", "", चिह्नकम्);
      यदि (sym->content->type_name)
        fprintf (बहिर्गमनम्, " <%s>", sym->content->type_name);
      fprintf (बहिर्गमनम्, " (%d)\n", i);

      यदि (on_वामः)
        {
          fprintf (बहिर्गमनम्, "%8s%s", "", _("on left:"));
          प्रति (नियमः_सङ्ख्या r = 0; r < nrules; r++)
            यदि (नियमाः[r].lhs->सङ्ख्या == i)
              fprintf (बहिर्गमनम्, " %d", r);
          fputc ('\n', बहिर्गमनम्);
        }

      यदि (on_दक्षिणः)
        {
          fprintf (बहिर्गमनम्, "%8s%s", "", _("on right:"));
          प्रति (नियमः_सङ्ख्या r = 0; r < nrules; r++)
            प्रति (item_सङ्ख्या *rhsp = नियमाः[r].rhs; 0 <= *rhsp; ++rhsp)
              यदि (item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rhsp) == i)
                {
                  fprintf (बहिर्गमनम्, " %d", r);
                  भङ्ग;
                }
          fputc ('\n', बहिर्गमनम्);
        }
    }
}

शून्य
print_results (शून्य)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  FILE *बहिर्गमनम् = xfopen (spec_verbose_file, "w");

  reduce_output (बहिर्गमनम्);
  grammar_नियमाः_partial_print (बहिर्गमनम्,
                               _("Rules useless in parser due to conflicts"),
                               नियमः_useless_अन्तर्_parser_p);
  conflicts_output (बहिर्गमनम्);

  grammar_नियमाः_print (बहिर्गमनम्);
  print_terminal_symbols (बहिर्गमनम्);
  print_nonterminal_symbols (बहिर्गमनम्);

  /* Storage for print_reductions.  */
  no_reduce_समुच्चयः = bitset_create (ntokens, BITSET_FIXED);
  प्रति (state_सङ्ख्या i = 0; i < nstates; i++)
    print_state (बहिर्गमनम्, अवस्थाः[i]);
  bitset_free (no_reduce_समुच्चयः);

  xfclose (बहिर्गमनम्);
}
