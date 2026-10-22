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
max_length (माप_प्रकार *width, ध्रुव वर्ण *str)
{
  माप_प्रकार len = mbswidth (str, 0);
  यदि (len > *width)
    *width = len;
}

/*--------------------------------.
| Report information on a state.  |
`--------------------------------*/

स्थिर शून्य
print_core (खाता *out, ध्रुव state *s)
{
  ध्रुव item_index *sitems = s->वस्तुएँ;
  माप_प्रकार snritems = s->nitems;
  /* Output all the items of a state, not only its kernel.  */
  यदि (report_flag & report_itemsets)
    {
      closure (sitems, snritems);
      sitems = itemset;
      snritems = nitemset;
    }

  यदि (!snritems)
    प्रत्यागम;

  ख_अक्षर_दो ('\n', out);

  rule ध्रुव *previous_rule = रिक्त;
  प्रति (माप_प्रकार i = 0; i < snritems; i++)
    {
      item_संख्या *sp1 = ritem + sitems[i];
      rule ध्रुव *r = item_rule (sp1);
      item_print (sp1, previous_rule, out);
      previous_rule = r;

      /* Display the lookahead tokens?  */
      यदि (report_flag & report_lookaheads
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
print_transitions (ध्रुव state *s, खाता *out, bool display_transitions_p)
{
  transitions *trans = s->transitions;
  माप_प्रकार width = 0;

  /* Compute the width of the lookahead token column.  */
  प्रति (पूर्णाङ्क i = 0; i < trans->अंक; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        max_length (&width, sym->tag);
      }

  /* Nothing to report. */
  यदि (!width)
    प्रत्यागम;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  /* Report lookahead tokens and shifts.  */
  प्रति (पूर्णाङ्क i = 0; i < trans->अंक; i++)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_IS_SHIFT (trans, i) == display_transitions_p)
      {
        प्रतीक *sym = symbols[TRANSITION_प्रतीक (trans, i)];
        ध्रुव वर्ण *tag = sym->tag;
        ध्रुव state *s1 = trans->states[i];

        ख_लिखो (out, "    %s", tag);
        प्रति (पूर्णाङ्क j = width - mbswidth (tag, 0); j > 0; --j)
          ख_अक्षर_दो (' ', out);
        यदि (display_transitions_p)
          ख_लिखो (out, _("shift, and go to state %d\n"), s1->संख्या);
        अन्यथा
          ख_लिखो (out, _("go to state %d\n"), s1->संख्या);
      }
}


/*--------------------------------------------------------.
| Report the explicit errors of S raised from %nonassoc.  |
`--------------------------------------------------------*/

स्थिर शून्य
print_errs (खाता *out, ध्रुव state *s)
{
  errs *errp = s->errs;
  माप_प्रकार width = 0;

  /* Compute the width of the lookahead token column.  */
  प्रति (पूर्णाङ्क i = 0; i < errp->अंक; ++i)
    यदि (errp->symbols[i])
      max_length (&width, errp->symbols[i]->tag);

  /* Nothing to report. */
  यदि (!width)
    प्रत्यागम;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  /* Report lookahead tokens and errors.  */
  प्रति (पूर्णाङ्क i = 0; i < errp->अंक; ++i)
    यदि (errp->symbols[i])
      {
        ध्रुव वर्ण *tag = errp->symbols[i]->tag;
        ख_लिखो (out, "    %s", tag);
        प्रति (पूर्णाङ्क j = width - mbswidth (tag, 0); j > 0; --j)
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
                 ध्रुव वर्ण *lookahead,
                 rule *r, bool enabled)
{
  ख_लिखो (out, "    %s", lookahead);
  प्रति (पूर्णाङ्क j = width - mbswidth (lookahead, 0); j > 0; --j)
    ख_अक्षर_दो (' ', out);
  यदि (!enabled)
    ख_अक्षर_दो ('[', out);
  यदि (rule_is_initial (r))
    ख_लिखो (out, _("accept"));
  अन्यथा
    ख_लिखो (out, _("reduce using rule %d (%s)"), r->संख्या,
             r->lhs->प्रतीक->tag);
  यदि (!enabled)
    ख_अक्षर_दो (']', out);
  ख_अक्षर_दो ('\n', out);
}


/*-------------------------------------------.
| Report on OUT the reduction actions of S.  |
`-------------------------------------------*/

स्थिर शून्य
print_reductions (खाता *out, ध्रुव state *s)
{
  reductions *reds = s->reductions;
  यदि (reds->अंक == 0)
    प्रत्यागम;

  rule *default_reduction = रिक्त;
  यदि (yydefact[s->संख्या] != 0)
    default_reduction = &rules[yydefact[s->संख्या] - 1];

  transitions *trans = s->transitions;

  bitset_zero (no_reduce_set);
  {
    पूर्णाङ्क i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (no_reduce_set, TRANSITION_प्रतीक (trans, i));
  }
  प्रति (पूर्णाङ्क i = 0; i < s->errs->अंक; ++i)
    यदि (s->errs->symbols[i])
      bitset_set (no_reduce_set, s->errs->symbols[i]->content->संख्या);

  /* Compute the width of the lookahead token column.  */
  माप_प्रकार width = 0;
  यदि (default_reduction)
    width = mbswidth (_("$default"), 0);

  यदि (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < ntokens; i++)
      {
        bool गिनती = bitset_test (no_reduce_set, i);

        प्रति (पूर्णाङ्क j = 0; j < reds->अंक; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गिनती)
                {
                  यदि (reds->rules[j] != default_reduction)
                    max_length (&width, symbols[i]->tag);
                  गिनती = true;
                }
              अन्यथा
                max_length (&width, symbols[i]->tag);
            }
      }

  /* Nothing to report. */
  यदि (!width)
    प्रत्यागम;

  ख_अक्षर_दो ('\n', out);
  width += 2;

  bool default_reduction_only = true;

  /* Report lookahead tokens (or $default) and reductions.  */
  यदि (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < ntokens; i++)
      {
        bool defaulted = false;
        bool गिनती = bitset_test (no_reduce_set, i);
        यदि (गिनती)
          default_reduction_only = false;

        प्रति (पूर्णाङ्क j = 0; j < reds->अंक; ++j)
          यदि (bitset_test (reds->lookaheads[j], i))
            {
              यदि (! गिनती)
                {
                  यदि (reds->rules[j] != default_reduction)
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
                  यदि (defaulted)
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

  यदि (default_reduction)
    {
      वर्ण *default_reductions =
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
print_actions (खाता *out, ध्रुव state *s)
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
print_state (खाता *out, ध्रुव state *s)
{
  ख_माला_दो ("\n\n", out);
  ख_लिखो (out, _("State %d"), s->संख्या);
  ख_अक्षर_दो ('\n', out);
  print_core (out, s);
  print_actions (out, s);
  यदि ((report_flag & report_solved_conflicts) && s->solved_conflicts)
    {
      ख_अक्षर_दो ('\n', out);
      ख_माला_दो (s->solved_conflicts, out);
    }
  यदि (has_conflicts (s)
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
  प्रति (पूर्णाङ्क i = 0; i < max_code + 1; ++i)
    यदि (token_translations[i] != undeftoken->content->संख्या)
      {
        ध्रुव प्रतीक *sym = symbols[token_translations[i]];
        ध्रुव वर्ण *tag = sym->tag;
        ख_लिखो (out, "%4s%s", "", tag);
        यदि (sym->content->type_name)
          ख_लिखो (out, " <%s>", sym->content->type_name);
        ख_लिखो (out, " (%d)", i);

        प्रति (rule_संख्या r = 0; r < nrules; r++)
          प्रति (item_संख्या *rhsp = rules[r].rhs; *rhsp >= 0; rhsp++)
            यदि (item_संख्या_as_प्रतीक_संख्या (*rhsp) == token_translations[i])
              {
                ख_लिखो (out, " %d", r);
                भङ्ग;
              }
        ख_अक्षर_दो ('\n', out);
      }
  ख_माला_दो ("\n\n", out);
}


स्थिर शून्य
print_nonterminal_symbols (खाता *out)
{
  ख_लिखो (out, "%s\n\n", _("Nonterminals, with rules where they appear"));
  प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; i++)
    {
      ध्रुव प्रतीक *sym = symbols[i];
      ध्रुव वर्ण *tag = sym->tag;
      bool on_बायाँ = false;
      bool on_दायाँ = false;

      प्रति (rule_संख्या r = 0; r < nrules; r++)
        {
          on_बायाँ |= rules[r].lhs->संख्या == i;
          प्रति (item_संख्या *rhsp = rules[r].rhs; !on_दायाँ && 0 <= *rhsp; ++rhsp)
            on_दायाँ |= item_संख्या_as_प्रतीक_संख्या (*rhsp) == i;
          यदि (on_बायाँ && on_दायाँ)
            भङ्ग;
        }

      ख_लिखो (out, "%4s%s", "", tag);
      यदि (sym->content->type_name)
        ख_लिखो (out, " <%s>", sym->content->type_name);
      ख_लिखो (out, " (%d)\n", i);

      यदि (on_बायाँ)
        {
          ख_लिखो (out, "%8s%s", "", _("on left:"));
          प्रति (rule_संख्या r = 0; r < nrules; r++)
            यदि (rules[r].lhs->संख्या == i)
              ख_लिखो (out, " %d", r);
          ख_अक्षर_दो ('\n', out);
        }

      यदि (on_दायाँ)
        {
          ख_लिखो (out, "%8s%s", "", _("on right:"));
          प्रति (rule_संख्या r = 0; r < nrules; r++)
            प्रति (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
              यदि (item_संख्या_as_प्रतीक_संख्या (*rhsp) == i)
                {
                  ख_लिखो (out, " %d", r);
                  भङ्ग;
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
  प्रति (state_संख्या i = 0; i < nstates; i++)
    print_state (out, states[i]);
  bitset_free (no_reduce_set);

  xfclose (out);
}
