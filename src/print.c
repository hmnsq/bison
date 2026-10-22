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
max_length (माप_प्रकार *width, स्थिरांक अक्षर *str)
{
  माप_प्रकार len = mbswidth (str, 0);
  जर (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (खाता *out, स्थिरांक state *s)
{
  स्थिरांक item_index *sitems = s->वस्तुसंच;
  माप_प्रकार snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  जर (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  जर (!snritems)
    परत;

  ख_अक्षर_दो ('\n', out);

  rule स्थिरांक *previous_rule = रिक्त;
  साठी (माप_प्रकार i = 0; i < snritems; i++)
    {
      item_संख्या *sp1 = ritem + sitems[i];
      rule स्थिरांक *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      जर (report_flag & report_lookaheads
          && item_संख्या_is_rule_संख्या (*sp1))
        state_rule_lookaheads_print (s, r, out);
      ख_अक्षर_दो ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

स्थिर शून्य
print_transitions (स्थिरांक state *s, खाता *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  माप_प्रकार width = 0;

  /* Compute the width of the lookahead token column.  */
  साठी (पूर्णांक i = 0; i < trans->अंक; i++)
    जर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        चिन्ह *sym = symbols[TRANSITION_चिन्ह (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  जर (!width)
    परत;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  साठी (पूर्णांक i = 0; i < trans->अंक; i++)
    जर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        चिन्ह *sym = symbols[TRANSITION_चिन्ह (trans, i)];
        स्थिरांक अक्षर *tag = sym->tag;
        स्थिरांक state *s1 = trans->states[i];

        ख_लिखो (out, "    %s", tag);
        साठी (पूर्णांक j = width - mbswidth (tag, 0); j > 0; --j)
          ख_अक्षर_दो (' ', out);
        जर (display_transitions_p)
          ख_लिखो (out, _("shift, and go to state %d\n"), s1->संख्या);
        अन्यथा
          ख_लिखो (out, _("go to state %d\n"), s1->संख्या);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

स्थिर शून्य
print_errs (खाता *out, स्थिरांक state *s)
{
  errs *errp = s->errs;
  माप_प्रकार width = 0;

  /* Compute the width of the lookahead token column.  */
  साठी (पूर्णांक i = 0; i < errp->अंक; ++i)
    जर (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  जर (!width)
    परत;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  साठी (पूर्णांक i = 0; i < errp->अंक; ++i)
    जर (errp->symbols[i])
      {
        स्थिरांक अक्षर *tag = errp->symbols[i]->tag;
        ख_लिखो (out, "    %s", tag);
        साठी (पूर्णांक j = width - mbswidth (tag, 0); j > 0; --j)
          ख_अक्षर_दो (' ', out);
        ख_माला_दो (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

स्थिर शून्य
print_reduction (खाता *out, माप_प्रकार width,
                 स्थिरांक अक्षर *lookahead,
                 rule *r, bool enabled)
{
  ख_लिखो (out, "    %s", lookahead);
  साठी (पूर्णांक j = width - mbswidth (lookahead, 0); j > 0; --j)
    ख_अक्षर_दो (' ', out);
  जर (!enabled)
    ख_अक्षर_दो ('[', out);
  जर (rule_is_initial (r))
    ख_लिखो (out, _("accept"));
  अन्यथा
    ख_लिखो (out, _("reduce using rule %d (%s)"), r->संख्या,
             r->lhs->चिन्ह->tag);
  जर (!enabled)
    ख_अक्षर_दो (']', out);
  ख_अक्षर_दो ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

स्थिर शून्य
print_reductions (खाता *out, स्थिरांक state *s)
{
  reductions *reds = s->reductions;
  जर (reds->अंक == 0)
    परत;

  rule *default_reduction = रिक्त;
  जर (yydefact[s->संख्या] != 0)
    default_reduction = &rules[yydefact[s->संख्या] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_चिन्ह (trans, i));
  }
  साठी (पूर्णांक i = 0; i < s->errs->अंक; ++i)
    जर (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->संख्या);

  /* Compute the width of the lookahead token column.  */
  माप_प्रकार width = 0;
  जर (default_reduction)
    width = mbswidth (_("$default"), 0);

  जर (reds->lookaheads)
    साठी (पूर्णांक i = 0; i < ntokens; i++)
      {
        bool मोजणी = bitset_test (no_reduce_set, i);

        साठी (पूर्णांक j = 0; j < reds->अंक; ++j)
          जर (bitset_test (reds->lookaheads[j], i))
            {
              जर (! मोजणी)
                {
                  जर (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  मोजणी = true;
                }
              अन्यथा
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  जर (!width)
    परत;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  जर (reds->lookaheads)
    साठी (पूर्णांक i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool मोजणी = bitset_test (no_reduce_set, i);
        जर (मोजणी)
          default_reduction_only = false;

        साठी (पूर्णांक j = 0; j < reds->अंक; ++j)
          जर (bitset_test (reds->lookaheads[j], i))
            {
              जर (! मोजणी)
                {
                  जर (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  अन्यथा
                    defaulted = true;
                  मोजणी = true;
                }
              अन्यथा
                {
                  default_reduction_only = false;
                  जर (defaulted)
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

  जर (default_reduction)
    {
      अक्षर *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->अंक == 1 && rule_is_initial (reds->rules[0])));
      (शून्य) default_reduction_only;
      मुक्त (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

स्थिर शून्य
print_actions (खाता *out, स्थिरांक state *s)
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
print_state (खाता *out, स्थिरांक state *s)
{
  ख_माला_दो ("\n\n", out);
  ख_लिखो (out, _("State %d"), s->संख्या);
  ख_अक्षर_दो ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  जर ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      ख_अक्षर_दो ('\n', out);
      ख_माला_दो (s->solved_conflicts, out);
    }
  जर (has_conflicts (s)
      && (report_flag & report_cex
          || warning_is_enabled (Wcounterexamples)))
    {
      ख_अक्षर_दो ('\n', out);
      counterexample_report_state (s, out, "    ");
    }
}

/*-----------------------------------------.
| Print information on the whole grammar.  |
`-----------------------------------------*/

स्थिर शून्य
print_terminal_symbols (खाता *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  ख_लिखो (out, "%s\n\n", _("Terminals, with rules where they appear"));
  साठी (पूर्णांक i = 0; i < max_code + 1; ++i)
    जर (token_translations[i] != undeftoken->content->संख्या)
      {
        स्थिरांक चिन्ह *sym = symbols[token_translations[i]];
        स्थिरांक अक्षर *tag = sym->tag;
        ख_लिखो (out, "%4s%s", "", tag);
        जर (sym->content->type_name)
          ख_लिखो (out, " <%s>", sym->content->type_name);
        ख_लिखो (out, " (%d)", i);

        साठी (rule_संख्या r = 0; r < nrules; r++)
          साठी (item_संख्या *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            जर (item_संख्या_as_चिन्ह_संख्या (*rhsp) == token_translations[i])
              {
                ख_लिखो (out, " %d", r);
                थांबा;
              }
        ख_अक्षर_दो ('\n', out);
      }
  ख_माला_दो ("\n\n", out);
}


स्थिर शून्य
print_nonterminal_symbols (खाता *out)
{
  ख_लिखो (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  साठी (चिन्ह_संख्या i = ntokens; i < nsyms; i++)
    {
      स्थिरांक चिन्ह *sym = symbols[i];
      स्थिरांक अक्षर *tag = sym->tag;
      bool on_डावा = false;
      bool on_उजवा = false;

      साठी (rule_संख्या r = 0; r < nrules; r++)
        {
          on_डावा |= rules[r].lhs->संख्या == i;
          साठी (item_संख्या *rhsp = rules[r].rhs; !on_उजवा && 0 <= *rhsp; ++rhsp)
            on_उजवा |= item_संख्या_as_चिन्ह_संख्या (*rhsp) == i;
          जर (on_डावा && on_उजवा)
            थांबा;
        }

      ख_लिखो (out, "%4s%s", "", tag);
      जर (sym->content->type_name)
        ख_लिखो (out, " <%s>", sym->content->type_name);
      ख_लिखो (out, " (%d)\n", i);

      जर (on_डावा)
        {
          ख_लिखो (out, "%8s%s", "", _("on left:"));
          साठी (rule_संख्या r = 0; r < nrules; r++)
            जर (rules[r].lhs->संख्या == i)
              ख_लिखो (out, " %d", r);
          ख_अक्षर_दो ('\n', out);
        }

      जर (on_उजवा)
        {
          ख_लिखो (out, "%8s%s", "", _("on right:"));
          साठी (rule_संख्या r = 0; r < nrules; r++)
            साठी (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              जर (item_संख्या_as_चिन्ह_संख्या (*rhsp) == i)
                {
                  ख_लिखो (out, " %d", r);
                  थांबा;
                }
          ख_अक्षर_दो ('\n', out);
        }
    }
}

शून्य
print_results (शून्य)
{
  /* We used to use just .out if SPEC_NAME_PREFIX (-p) was used, but
     that conflicts with Posix.  */
  खाता *out = xfopen (spec_verbose_file, "w");

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
  साठी (state_संख्या i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
