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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "counterexample.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "print-xml.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "symtab.h"

/* -1 stands for not specified. */
पूर्णांक expected_sr_conflicts = -1;
पूर्णांक expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
स्थिर bool *conflicts;

स्थिर संरचना obstack solved_conflicts_obstack;
स्थिर संरचना obstack solved_conflicts_xml_obstack;

स्थिर bitset shift_set;
स्थिर bitset lookahead_set;

bool
has_conflicts (स्थिरांक state *s)
{
  परत conflicts[s->संख्या];
}



गणना conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    डावा_resolution,
    उजवा_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

स्थिर ओळीत शून्य
log_resolution (rule *r, चिन्ह_संख्या token,
                गणना conflict_resolution resolution)
{
  जर (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      निवड (resolution)
        {
        स्थिती shift_resolution:
        स्थिती उजवा_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->संख्या,
                          symbols[token]->tag);
          थांबा;

        स्थिती reduce_resolution:
        स्थिती डावा_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->संख्या,
                          symbols[token]->tag);
          थांबा;

        स्थिती nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->संख्या,
                          symbols[token]->tag);
          थांबा;
        }

      /* The reason. */
      निवड (resolution)
        {
        स्थिती shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->चिन्ह->tag,
                          symbols[token]->tag);
          थांबा;

        स्थिती reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->चिन्ह->tag);
          थांबा;

        स्थिती डावा_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          थांबा;

        स्थिती उजवा_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          थांबा;

        स्थिती nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          थांबा;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  जर (xml_flag)
    {
      /* The description of the resolution. */
      निवड (resolution)
        {
        स्थिती shift_resolution:
        स्थिती उजवा_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->संख्या,
                          xml_escape (symbols[token]->tag));
          थांबा;

        स्थिती reduce_resolution:
        स्थिती डावा_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->संख्या,
                          xml_escape (symbols[token]->tag));
          थांबा;

        स्थिती nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->संख्या,
                          xml_escape (symbols[token]->tag));
          थांबा;
        }

      /* The reason. */
      निवड (resolution)
        {
        स्थिती shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->चिन्ह->tag),
                          xml_escape_n (1, symbols[token]->tag));
          थांबा;

        स्थिती reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->चिन्ह->tag));
          थांबा;

        स्थिती डावा_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          थांबा;

        स्थिती उजवा_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          थांबा;

        स्थिती nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      थांबा;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

स्थिर शून्य
flush_shift (state *s, पूर्णांक token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  साठी (पूर्णांक i = 0; i < trans->अंक; ++i)
    जर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_चिन्ह (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

स्थिर शून्य
flush_reduce (bitset lookaheads, पूर्णांक token)
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

स्थिर शून्य
resolve_sr_conflict (state *s, पूर्णांक ruleno, चिन्ह **errors, पूर्णांक *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  पूर्णांक redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  साठी (चिन्ह_संख्या i = 0; i < ntokens; ++i)
    जर (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        जर (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->संख्या, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        अन्यथा जर (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->संख्या);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        अन्यथा
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          निवड (symbols[i]->content->assoc)
            {
            स्थिती undef_assoc:
              पात ();

            स्थिती precedence_assoc:
              थांबा;

            स्थिती उजवा_assoc:
              register_assoc (i, redrule->prec->संख्या);
              log_resolution (redrule, i, उजवा_resolution);
              flush_reduce (lookaheads, i);
              थांबा;

            स्थिती डावा_assoc:
              register_assoc (i, redrule->prec->संख्या);
              log_resolution (redrule, i, डावा_resolution);
              flush_shift (s, i);
              थांबा;

            स्थिती non_assoc:
              register_assoc (i, redrule->prec->संख्या);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              थांबा;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

स्थिर शून्य
set_conflicts (state *s, चिन्ह **errors)
{
  जर (s->consistent)
    परत;

  reductions *reds = s->reductions;
  पूर्णांक nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_चिन्ह (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  साठी (पूर्णांक i = 0; i < reds->अंक; ++i)
    जर (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  जर (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  जर (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  जर (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  जर (reds->lookaheads)
    साठी (पूर्णांक i = 0; i < reds->अंक; ++i)
      {
        जर (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->संख्या] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

शून्य
conflicts_solve (शून्य)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  चिन्ह **errors = xnmalloc (ntokens + 1, आकार *errors);

  conflicts = xcalloc (nstates, आकार *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  साठी (state_संख्या i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      जर (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  मुक्त (errors);
}


शून्य
conflicts_update_state_numbers (state_संख्या old_to_new[],
                                state_संख्या nstates_old)
{
  साठी (state_संख्या i = 0; i < nstates_old; ++i)
    जर (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

स्थिर माप_प्रकार
मोजणी_state_sr_conflicts (स्थिरांक state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  जर (!trans)
    परत 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_चिन्ह (trans, i));
  }

  साठी (पूर्णांक i = 0; i < reds->अंक; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  परत bitset_मोजणी (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

स्थिर माप_प्रकार
मोजणी_sr_conflicts (शून्य)
{
  माप_प्रकार res = 0;
  /* Conflicts by state.  */
  साठी (state_संख्या i = 0; i < nstates; ++i)
    जर (conflicts[i])
      res += मोजणी_state_sr_conflicts (states[i]);
  परत res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

स्थिर माप_प्रकार
मोजणी_state_rr_conflicts (स्थिरांक state *s)
{
  reductions *reds = s->reductions;
  माप_प्रकार res = 0;

  साठी (चिन्ह_संख्या i = 0; i < ntokens; ++i)
    {
      पूर्णांक मोजणी = 0;
      साठी (पूर्णांक j = 0; j < reds->अंक; ++j)
        मोजणी += bitset_test (reds->lookaheads[j], i);
      जर (2 <= मोजणी)
        res += मोजणी-1;
    }

  परत res;
}

स्थिर माप_प्रकार
मोजणी_rr_conflicts (शून्य)
{
  माप_प्रकार res = 0;
  /* Conflicts by state.  */
  साठी (state_संख्या i = 0; i < nstates; ++i)
    जर (conflicts[i])
      res += मोजणी_state_rr_conflicts (states[i]);
  परत res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

स्थिर माप_प्रकार
मोजणी_rule_state_sr_conflicts (rule *r, state *s)
{
  माप_प्रकार res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  साठी (पूर्णांक i = 0; i < reds->अंक; ++i)
    जर (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        पूर्णांक j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_चिन्ह (trans, j));
      }

  परत res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

स्थिर माप_प्रकार
मोजणी_rule_sr_conflicts (rule *r)
{
  माप_प्रकार res = 0;
  साठी (state_संख्या i = 0; i < nstates; ++i)
    जर (conflicts[i])
      res += मोजणी_rule_state_sr_conflicts (r, states[i]);
  परत res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

स्थिर माप_प्रकार
मोजणी_rule_state_rr_conflicts (rule *r, state *s)
{
  माप_प्रकार res = 0;
  स्थिरांक reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  साठी (पूर्णांक i = 0; i < reds->अंक; ++i)
    जर (reds->rules[i] == r)
      साठी (पूर्णांक j = 0; j < reds->अंक; ++j)
        जर (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_मोजणी (lookaheads);
          }
  bitset_free (lookaheads);
  परत res;
}

स्थिर माप_प्रकार
मोजणी_rule_rr_conflicts (rule *r)
{
  माप_प्रकार res = 0;
  साठी (state_संख्या i = 0; i < nstates; ++i)
    res += मोजणी_rule_state_rr_conflicts (r, states[i]);
  परत res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

शून्य
conflicts_output (खाता *out)
{
  bool printed_sth = false;
  साठी (state_संख्या i = 0; i < nstates; ++i)
    जर (conflicts[i])
      {
        स्थिरांक state *s = states[i];
        पूर्णांक src = मोजणी_state_sr_conflicts (s);
        पूर्णांक rrc = मोजणी_state_rr_conflicts (s);
        ख_लिखो (out, _("State %d "), i);
        जर (src && rrc)
          ख_लिखो (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        अन्यथा जर (src)
          ख_लिखो (out, _("conflicts: %d shift/reduce\n"), src);
        अन्यथा जर (rrc)
          ख_लिखो (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  जर (printed_sth)
    ख_माला_दो ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

पूर्णांक
conflicts_total_मोजणी (शून्य)
{
  परत मोजणी_sr_conflicts () + मोजणी_rr_conflicts ();
}

स्थिर शून्य
report_counterexamples (शून्य)
{
  साठी (state_संख्या sn = 0; sn < nstates; ++sn)
    जर (conflicts[sn])
      counterexample_report_state (states[sn], मानक_त्रुटि, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

स्थिर शून्य
report_rule_expectation_mismatches (शून्य)
{
  साठी (rule_संख्या i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      पूर्णांक expected_sr = r->expected_sr_conflicts;
      पूर्णांक expected_rr = r->expected_rr_conflicts;

      जर (expected_sr != -1 || expected_rr != -1)
        {
          पूर्णांक sr = मोजणी_rule_sr_conflicts (r);
          जर (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          पूर्णांक rr = मोजणी_rule_rr_conflicts (r);
          जर (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

शून्य
conflicts_print (शून्य)
{
  report_rule_expectation_mismatches ();

  जर (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (रिक्त, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    पूर्णांक total = मोजणी_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    पूर्णांक expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    जर (expected != -1)
      {
        जर (expected != total)
          {
            complain (रिक्त, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            जर (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा जर (total)
      {
        complain (रिक्त, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    पूर्णांक total = मोजणी_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    पूर्णांक expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    जर (expected != -1)
      {
        जर (expected != total)
          {
            complain (रिक्त, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            जर (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा जर (total)
      {
        complain (रिक्त, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  जर (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  अन्यथा जर (unexpected_conflicts_warning != Wnone)
    subcomplain (रिक्त, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

शून्य
conflicts_free (शून्य)
{
  मुक्त (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, रिक्त);
  obstack_free (&solved_conflicts_xml_obstack, रिक्त);
}
