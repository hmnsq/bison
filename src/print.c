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

#समावेश <config.h>

#समावेश "print.h"

#समावेश "system.h"

#समावेश <bitset.h>
#समावेश <mbswidth.h>

#समावेश "closure.h"
#समावेश "complain.h"
#समावेश "conflicts.h"
#समावेश "counterexample.h"
#समावेश "files.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "lalr.h"
#समावेश "lr0.h"
#समावेश "muscle-tab.h"
#समावेश "reader.h"
#समावेश "reduce.h"
#समावेश "state.h"
#समावेश "symtab.h"
#समावेश "tables.h"

/* For a given state, the symbol numbers of the lookahead tokens for
   shifts and errors (i.e. not reduce).  */
अटल bitset no_reduce_set;



/*---------------------------------------.
| *WIDTH := max (*WIDTH, strlen (STR)).  |
`---------------------------------------*/

अटल व्योम
max_लंबाई (माप_प्रकार *width, स्थिर अक्षर *str)
{
  माप_प्रकार len = mbswidth (str, 0);
  अगर (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

अटल व्योम
print_core (खाता *out, स्थिर state *s)
{
  स्थिर item_सूचकांक *sitems = s->वस्तुएँ;
  माप_प्रकार snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  अगर (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  अगर (!snritems)
    वापस;

  ख_अक्षर_दो ('\n', out);

  rule स्थिर *previous_rule = शून्य;
  क्रम (माप_प्रकार i = 0; i < snritems; i++)
    {
      item_संख्या *sp1 = ritem + sitems[i];
      rule स्थिर *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      अगर (report_flag & report_lookaheads
          && item_संख्या_is_rule_संख्या (*sp1))
        state_rule_lookaheads_print (s, r, out);
      ख_अक्षर_दो ('\n', out);
    }
}


/*------------------------------------------------------------.
| Report the shifts iff DISPLAY_SHIFTS_P or the gotos of S on |
| OUT.                                                        |
`------------------------------------------------------------*/

अटल व्योम
print_transitions (स्थिर state *s, खाता *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  माप_प्रकार width = 0;

  /* Compute the width of the lookahead token column.  */
  क्रम (पूर्णांक i = 0; i < trans->अंक; i++)
    अगर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        max_लंबाई (&width, sym->tag);
      }

  /* Nothing to report. */
  अगर (!width)
    वापस;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  क्रम (पूर्णांक i = 0; i < trans->अंक; i++)
    अगर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        स्थिर अक्षर *tag = sym->tag;
        स्थिर state *s1 = trans->states[i];

        ख_लिखो (out, "    %s", tag);
        क्रम (पूर्णांक j = width - mbswidth (tag, 0); j > 0; --j)
          ख_अक्षर_दो (' ', out);
        अगर (display_transitions_p)
          ख_लिखो (out, _("shift, and go to state %d\n"), s1->संख्या);
        अन्यथा
          ख_लिखो (out, _("go to state %d\n"), s1->संख्या);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

अटल व्योम
print_errs (खाता *out, स्थिर state *s)
{
  errs *errp = s->errs;
  माप_प्रकार width = 0;

  /* Compute the width of the lookahead token column.  */
  क्रम (पूर्णांक i = 0; i < errp->अंक; ++i)
    अगर (errp->symbols[i])
      max_लंबाई (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  अगर (!width)
    वापस;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  क्रम (पूर्णांक i = 0; i < errp->अंक; ++i)
    अगर (errp->symbols[i])
      {
        स्थिर अक्षर *tag = errp->symbols[i]->tag;
        ख_लिखो (out, "    %s", tag);
        क्रम (पूर्णांक j = width - mbswidth (tag, 0); j > 0; --j)
          ख_अक्षर_दो (' ', out);
        ख_माला_दो (_("error (nonassociative)\n"), out);
      }
}


/*-------------------------------------------------------------------.
| Report a reduction of RULE on LOOKAHEAD (which can be 'default').  |
| If not ENABLED, the rule is masked by a shift or a reduce (S/R and |
| R/R conflicts).                                                    |
`-------------------------------------------------------------------*/

अटल व्योम
print_reduction (खाता *out, माप_प्रकार width,
                 स्थिर अक्षर *lookahead,
                 rule *r, bool enabled)
{
  ख_लिखो (out, "    %s", lookahead);
  क्रम (पूर्णांक j = width - mbswidth (lookahead, 0); j > 0; --j)
    ख_अक्षर_दो (' ', out);
  अगर (!enabled)
    ख_अक्षर_दो ('[', out);
  अगर (rule_is_initial (r))
    ख_लिखो (out, _("accept"));
  अन्यथा
    ख_लिखो (out, _("reduce using rule %d (%s)"), r->संख्या,
             r->lhs->प्रतीक->tag);
  अगर (!enabled)
    ख_अक्षर_दो (']', out);
  ख_अक्षर_दो ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

अटल व्योम
print_reductions (खाता *out, स्थिर state *s)
{
  reductions *reds = s->reductions;
  अगर (reds->अंक == 0)
    वापस;

  rule *default_reduction = शून्य;
  अगर (yydefact[s->संख्या] != 0)
    default_reduction = &rules[yydefact[s->संख्या] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_प्रतीक (trans, i));
  }
  क्रम (पूर्णांक i = 0; i < s->errs->अंक; ++i)
    अगर (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->संख्या);

  /* Compute the width of the lookahead token column.  */
  माप_प्रकार width = 0;
  अगर (default_reduction)
    width = mbswidth (_("$default"), 0);

  अगर (reds->lookaheads)
    क्रम (पूर्णांक i = 0; i < ntokens; i++)
      {
        bool गिनती = bitset_test (no_reduce_set, i);

        क्रम (पूर्णांक j = 0; j < reds->अंक; ++j)
          अगर (bitset_test (reds->lookaheads[j], i))
            {
              अगर (! गिनती)
                {
                  अगर (reds->rules[j] != default_reduction)
                    max_लंबाई (&width, symbols[i]->tag);
                  गिनती = true;
                }
              अन्यथा
                max_लंबाई (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  अगर (!width)
    वापस;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  अगर (reds->lookaheads)
    क्रम (पूर्णांक i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गिनती = bitset_test (no_reduce_set, i);
        अगर (गिनती)
          default_reduction_only = false;

        क्रम (पूर्णांक j = 0; j < reds->अंक; ++j)
          अगर (bitset_test (reds->lookaheads[j], i))
            {
              अगर (! गिनती)
                {
                  अगर (reds->rules[j] != default_reduction)
                    {
                      default_reduction_only = false;
                      print_reduction (out, width,
                                       symbols[i]->tag,
                                       reds->rules[j], true);
                    }
                  अन्यथा
                    defaulted = true;
                  गिनती = true;
                }
              अन्यथा
                {
                  default_reduction_only = false;
                  अगर (defaulted)
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

  अगर (default_reduction)
    {
      अक्षर *default_reductions =
        muscle_percent_define_get ("lr.default-reduction");
      print_reduction (out, width, _("$default"), default_reduction, true);
      aver (STREQ (default_reductions, "most")
            || (STREQ (default_reductions, "consistent")
                && default_reduction_only)
            || (reds->अंक == 1 && rule_is_initial (reds->rules[0])));
      (व्योम) default_reduction_only;
      मुक्त (default_reductions);
    }
}


/*--------------------------------------------------------------.
| Report on OUT all the actions (shifts, gotos, reductions, and |
| explicit errors from %nonassoc) of S.                         |
`--------------------------------------------------------------*/

अटल व्योम
print_actions (खाता *out, स्थिर state *s)
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

अटल व्योम
print_state (खाता *out, स्थिर state *s)
{
  ख_माला_दो ("\n\n", out);
  ख_लिखो (out, _("State %d"), s->संख्या);
  ख_अक्षर_दो ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  अगर ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      ख_अक्षर_दो ('\n', out);
      ख_माला_दो (s->solved_conflicts, out);
    }
  अगर (has_conflicts (s)
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

अटल व्योम
print_terminal_symbols (खाता *out)
{
  /* TERMINAL (type #) : rule #s terminal is on RHS */
  ख_लिखो (out, "%s\n\n", _("Terminals, with rules where they appear"));
  क्रम (पूर्णांक i = 0; i < max_code + 1; ++i)
    अगर (token_translations[i] != undeftoken->content->संख्या)
      {
        स्थिर प्रतीक *sym = symbols[token_translations[i]];
        स्थिर अक्षर *tag = sym->tag;
        ख_लिखो (out, "%4s%s", "", tag);
        अगर (sym->content->type_name)
          ख_लिखो (out, " <%s>", sym->content->type_name);
        ख_लिखो (out, " (%d)", i);

        क्रम (rule_संख्या r = 0; r < nrules; r++)
          क्रम (item_संख्या *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            अगर (item_संख्या_as_प्रतीक_संख्या (*rhsp) == token_translations[i])
              {
                ख_लिखो (out, " %d", r);
                अवरोध;
              }
        ख_अक्षर_दो ('\n', out);
      }
  ख_माला_दो ("\n\n", out);
}


अटल व्योम
print_nonterminal_symbols (खाता *out)
{
  ख_लिखो (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms; i++)
    {
      स्थिर प्रतीक *sym = symbols[i];
      स्थिर अक्षर *tag = sym->tag;
      bool on_बायाँ = false;
      bool on_दायाँ = false;

      क्रम (rule_संख्या r = 0; r < nrules; r++)
        {
          on_बायाँ |= rules[r].lhs->संख्या == i;
          क्रम (item_संख्या *rhsp = rules[r].rhs; !on_दायाँ && 0 <= *rhsp; ++rhsp)
            on_दायाँ |= item_संख्या_as_प्रतीक_संख्या (*rhsp) == i;
          अगर (on_बायाँ && on_दायाँ)
            अवरोध;
        }

      ख_लिखो (out, "%4s%s", "", tag);
      अगर (sym->content->type_name)
        ख_लिखो (out, " <%s>", sym->content->type_name);
      ख_लिखो (out, " (%d)\n", i);

      अगर (on_बायाँ)
        {
          ख_लिखो (out, "%8s%s", "", _("on left:"));
          क्रम (rule_संख्या r = 0; r < nrules; r++)
            अगर (rules[r].lhs->संख्या == i)
              ख_लिखो (out, " %d", r);
          ख_अक्षर_दो ('\n', out);
        }

      अगर (on_दायाँ)
        {
          ख_लिखो (out, "%8s%s", "", _("on right:"));
          क्रम (rule_संख्या r = 0; r < nrules; r++)
            क्रम (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              अगर (item_संख्या_as_प्रतीक_संख्या (*rhsp) == i)
                {
                  ख_लिखो (out, " %d", r);
                  अवरोध;
                }
          ख_अक्षर_दो ('\n', out);
        }
    }
}

व्योम
print_results (व्योम)
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
  क्रम (state_संख्या i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
