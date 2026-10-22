/* Find and resolve or report lookahead conflicts for bison,

   Copyright (C) 1984, 1989, 1992, 2000-2015, 2018-2022 Free Software
   Foundation, Inc.

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
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "counterexample.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "print-xml.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "state.h"
#അന്തര്ഭാവ "symtab.h"

/* -1 stands for not specified. */
പൂർണ്ണസംഖ്യ expected_sr_conflicts = -1;
പൂർണ്ണസംഖ്യ expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
സ്ഥിതം bool *conflicts;

സ്ഥിതം ഘടന obstack solved_conflicts_obstack;
സ്ഥിതം ഘടന obstack solved_conflicts_xml_obstack;

സ്ഥിതം bitset shift_set;
സ്ഥിതം bitset lookahead_set;

bool
has_conflicts (സ്ഥിരം state *s)
{
  തിരികെ conflicts[s->സംഖ്യ];
}



എണ്ണൽ conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    ഇടത്_resolution,
    വലത്_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

സ്ഥിതം വരിയിൽ ശൂന്യം
log_resolution (rule *r, ചിഹ്നം_സംഖ്യ token,
                എണ്ണൽ conflict_resolution resolution)
{
  എങ്കിൽ (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      തിരഞ്ഞെടുപ്പ് (resolution)
        {
        അവസ്ഥ shift_resolution:
        അവസ്ഥ വലത്_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->സംഖ്യ,
                          symbols[token]->tag);
          നിർത്തുക;

        അവസ്ഥ reduce_resolution:
        അവസ്ഥ ഇടത്_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->സംഖ്യ,
                          symbols[token]->tag);
          നിർത്തുക;

        അവസ്ഥ nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->സംഖ്യ,
                          symbols[token]->tag);
          നിർത്തുക;
        }

      /* The reason. */
      തിരഞ്ഞെടുപ്പ് (resolution)
        {
        അവസ്ഥ shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->ചിഹ്നം->tag,
                          symbols[token]->tag);
          നിർത്തുക;

        അവസ്ഥ reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->ചിഹ്നം->tag);
          നിർത്തുക;

        അവസ്ഥ ഇടത്_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          നിർത്തുക;

        അവസ്ഥ വലത്_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          നിർത്തുക;

        അവസ്ഥ nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          നിർത്തുക;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  എങ്കിൽ (xml_flag)
    {
      /* The description of the resolution. */
      തിരഞ്ഞെടുപ്പ് (resolution)
        {
        അവസ്ഥ shift_resolution:
        അവസ്ഥ വലത്_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->സംഖ്യ,
                          xml_escape (symbols[token]->tag));
          നിർത്തുക;

        അവസ്ഥ reduce_resolution:
        അവസ്ഥ ഇടത്_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->സംഖ്യ,
                          xml_escape (symbols[token]->tag));
          നിർത്തുക;

        അവസ്ഥ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->സംഖ്യ,
                          xml_escape (symbols[token]->tag));
          നിർത്തുക;
        }

      /* The reason. */
      തിരഞ്ഞെടുപ്പ് (resolution)
        {
        അവസ്ഥ shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->ചിഹ്നം->tag),
                          xml_escape_n (1, symbols[token]->tag));
          നിർത്തുക;

        അവസ്ഥ reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->ചിഹ്നം->tag));
          നിർത്തുക;

        അവസ്ഥ ഇടത്_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          നിർത്തുക;

        അവസ്ഥ വലത്_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          നിർത്തുക;

        അവസ്ഥ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      നിർത്തുക;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
flush_shift (state *s, പൂർണ്ണസംഖ്യ token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < trans->അക്കം; ++i)
    എങ്കിൽ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_ചിഹ്നം (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
flush_reduce (bitset lookaheads, പൂർണ്ണസംഖ്യ token)
{
  bitset_reset (lookaheads, token);
}


/*------------------------------------------------------------------.
| Attempt to resolve shift/reduce conflict for one rule by means of |
| precedence declarations.  It has already been checked that the    |
| rule has a precedence.  A conflict is resolved by modifying the   |
| shift or reduce tables so that there is no longer a conflict.     |
|                                                                   |
| RULENO is the number of the lookahead bitset to consider.         |
|                                                                   |
| ERRORS and NERRS can be used to store discovered explicit         |
| errors.                                                           |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
resolve_sr_conflict (state *s, പൂർണ്ണസംഖ്യ ruleno, ചിഹ്നം **errors, പൂർണ്ണസംഖ്യ *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  പൂർണ്ണസംഖ്യ redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < ntokens; ++i)
    എങ്കിൽ (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        എങ്കിൽ (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->സംഖ്യ, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        അല്ലെങ്കിൽ എങ്കിൽ (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->സംഖ്യ);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        അല്ലെങ്കിൽ
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          തിരഞ്ഞെടുപ്പ് (symbols[i]->content->assoc)
            {
            അവസ്ഥ undef_assoc:
              abort ();

            അവസ്ഥ precedence_assoc:
              നിർത്തുക;

            അവസ്ഥ വലത്_assoc:
              register_assoc (i, redrule->prec->സംഖ്യ);
              log_resolution (redrule, i, വലത്_resolution);
              flush_reduce (lookaheads, i);
              നിർത്തുക;

            അവസ്ഥ ഇടത്_assoc:
              register_assoc (i, redrule->prec->സംഖ്യ);
              log_resolution (redrule, i, ഇടത്_resolution);
              flush_shift (s, i);
              നിർത്തുക;

            അവസ്ഥ non_assoc:
              register_assoc (i, redrule->prec->സംഖ്യ);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              നിർത്തുക;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
set_conflicts (state *s, ചിഹ്നം **errors)
{
  എങ്കിൽ (s->consistent)
    തിരികെ;

  reductions *reds = s->reductions;
  പൂർണ്ണസംഖ്യ nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    പൂർണ്ണസംഖ്യ i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_ചിഹ്നം (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
    എങ്കിൽ (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  എങ്കിൽ (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  എങ്കിൽ (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  എങ്കിൽ (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  എങ്കിൽ (reds->lookaheads)
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
      {
        എങ്കിൽ (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->സംഖ്യ] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

ശൂന്യം
conflicts_solve (ശൂന്യം)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  ചിഹ്നം **errors = xnmalloc (ntokens + 1, വലുപ്പം *errors);

  conflicts = xcalloc (nstates, വലുപ്പം *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      എങ്കിൽ (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  സ്വതന്ത്രം (errors);
}


ശൂന്യം
conflicts_update_state_numbers (state_സംഖ്യ old_to_new[],
                                state_സംഖ്യ nstates_old)
{
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates_old; ++i)
    എങ്കിൽ (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

സ്ഥിതം size_t
എണ്ണം_state_sr_conflicts (സ്ഥിരം state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  എങ്കിൽ (!trans)
    തിരികെ 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    പൂർണ്ണസംഖ്യ i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_ചിഹ്നം (trans, i));
  }

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  തിരികെ bitset_എണ്ണം (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

സ്ഥിതം size_t
എണ്ണം_sr_conflicts (ശൂന്യം)
{
  size_t res = 0;
  /* Conflicts by state.  */
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    എങ്കിൽ (conflicts[i])
      res += എണ്ണം_state_sr_conflicts (states[i]);
  തിരികെ res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

സ്ഥിതം size_t
എണ്ണം_state_rr_conflicts (സ്ഥിരം state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < ntokens; ++i)
    {
      പൂർണ്ണസംഖ്യ എണ്ണം = 0;
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < reds->അക്കം; ++j)
        എണ്ണം += bitset_test (reds->lookaheads[j], i);
      എങ്കിൽ (2 <= എണ്ണം)
        res += എണ്ണം-1;
    }

  തിരികെ res;
}

സ്ഥിതം size_t
എണ്ണം_rr_conflicts (ശൂന്യം)
{
  size_t res = 0;
  /* Conflicts by state.  */
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    എങ്കിൽ (conflicts[i])
      res += എണ്ണം_state_rr_conflicts (states[i]);
  തിരികെ res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

സ്ഥിതം size_t
എണ്ണം_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
    എങ്കിൽ (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        പൂർണ്ണസംഖ്യ j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_ചിഹ്നം (trans, j));
      }

  തിരികെ res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

സ്ഥിതം size_t
എണ്ണം_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    എങ്കിൽ (conflicts[i])
      res += എണ്ണം_rule_state_sr_conflicts (r, states[i]);
  തിരികെ res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

സ്ഥിതം size_t
എണ്ണം_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  സ്ഥിരം reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
    എങ്കിൽ (reds->rules[i] == r)
      വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < reds->അക്കം; ++j)
        എങ്കിൽ (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_എണ്ണം (lookaheads);
          }
  bitset_free (lookaheads);
  തിരികെ res;
}

സ്ഥിതം size_t
എണ്ണം_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    res += എണ്ണം_rule_state_rr_conflicts (r, states[i]);
  തിരികെ res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

ശൂന്യം
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    എങ്കിൽ (conflicts[i])
      {
        സ്ഥിരം state *s = states[i];
        പൂർണ്ണസംഖ്യ src = എണ്ണം_state_sr_conflicts (s);
        പൂർണ്ണസംഖ്യ rrc = എണ്ണം_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        എങ്കിൽ (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        അല്ലെങ്കിൽ എങ്കിൽ (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        അല്ലെങ്കിൽ എങ്കിൽ (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  എങ്കിൽ (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

പൂർണ്ണസംഖ്യ
conflicts_total_എണ്ണം (ശൂന്യം)
{
  തിരികെ എണ്ണം_sr_conflicts () + എണ്ണം_rr_conflicts ();
}

സ്ഥിതം ശൂന്യം
report_counterexamples (ശൂന്യം)
{
  വേണ്ടി (state_സംഖ്യ sn = 0; sn < nstates; ++sn)
    എങ്കിൽ (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

സ്ഥിതം ശൂന്യം
report_rule_expectation_mismatches (ശൂന്യം)
{
  വേണ്ടി (rule_സംഖ്യ i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      പൂർണ്ണസംഖ്യ expected_sr = r->expected_sr_conflicts;
      പൂർണ്ണസംഖ്യ expected_rr = r->expected_rr_conflicts;

      എങ്കിൽ (expected_sr != -1 || expected_rr != -1)
        {
          പൂർണ്ണസംഖ്യ sr = എണ്ണം_rule_sr_conflicts (r);
          എങ്കിൽ (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          പൂർണ്ണസംഖ്യ rr = എണ്ണം_rule_rr_conflicts (r);
          എങ്കിൽ (rr != expected_rr && (rr != 0 || expected_rr != -1))
            complain (&r->location, complaint,
                      _("reduce/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, rr, expected_rr);
        }
    }
}

/*---------------------------------.
| Reporting numbers of conflicts.  |
`---------------------------------*/

ശൂന്യം
conflicts_print (ശൂന്യം)
{
  report_rule_expectation_mismatches ();

  എങ്കിൽ (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (ശൂന്യ, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    പൂർണ്ണസംഖ്യ total = എണ്ണം_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    പൂർണ്ണസംഖ്യ expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    എങ്കിൽ (expected != -1)
      {
        എങ്കിൽ (expected != total)
          {
            complain (ശൂന്യ, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            എങ്കിൽ (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    അല്ലെങ്കിൽ എങ്കിൽ (total)
      {
        complain (ശൂന്യ, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    പൂർണ്ണസംഖ്യ total = എണ്ണം_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    പൂർണ്ണസംഖ്യ expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    എങ്കിൽ (expected != -1)
      {
        എങ്കിൽ (expected != total)
          {
            complain (ശൂന്യ, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            എങ്കിൽ (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    അല്ലെങ്കിൽ എങ്കിൽ (total)
      {
        complain (ശൂന്യ, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  എങ്കിൽ (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  അല്ലെങ്കിൽ എങ്കിൽ (unexpected_conflicts_warning != Wnone)
    subcomplain (ശൂന്യ, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

ശൂന്യം
conflicts_free (ശൂന്യം)
{
  സ്വതന്ത്രം (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, ശൂന്യ);
  obstack_free (&solved_conflicts_xml_obstack, ശൂന്യ);
}
