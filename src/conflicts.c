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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <bitset.h>

#समावेश "complain.h"
#समावेश "conflicts.h"
#समावेश "counterexample.h"
#समावेश "files.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "lalr.h"
#समावेश "lr0.h"
#समावेश "print-xml.h"
#समावेश "reader.h"
#समावेश "state.h"
#समावेश "symtab.h"

/* -1 stands for not specified. */
पूर्णांक expected_sr_conflicts = -1;
पूर्णांक expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
अटल bool *conflicts;

अटल काष्ठा obstack solved_conflicts_obstack;
अटल काष्ठा obstack solved_conflicts_xml_obstack;

अटल bitset shift_set;
अटल bitset lookahead_set;

bool
has_conflicts (स्थिर state *s)
{
  वापस conflicts[s->संख्या];
}



क्रमागत conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    बायाँ_resolution,
    दायाँ_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

अटल अंतरभूत व्योम
log_resolution (rule *r, प्रतीक_संख्या token,
                क्रमागत conflict_resolution resolution)
{
  अगर (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      चयन (resolution)
        {
        हाल shift_resolution:
        हाल दायाँ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->संख्या,
                          symbols[token]->tag);
          अवरोध;

        हाल reduce_resolution:
        हाल बायाँ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->संख्या,
                          symbols[token]->tag);
          अवरोध;

        हाल nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->संख्या,
                          symbols[token]->tag);
          अवरोध;
        }

      /* The reason. */
      चयन (resolution)
        {
        हाल shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->प्रतीक->tag,
                          symbols[token]->tag);
          अवरोध;

        हाल reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->प्रतीक->tag);
          अवरोध;

        हाल बायाँ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          अवरोध;

        हाल दायाँ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          अवरोध;

        हाल nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          अवरोध;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  अगर (xml_flag)
    {
      /* The description of the resolution. */
      चयन (resolution)
        {
        हाल shift_resolution:
        हाल दायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->संख्या,
                          xml_escape (symbols[token]->tag));
          अवरोध;

        हाल reduce_resolution:
        हाल बायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->संख्या,
                          xml_escape (symbols[token]->tag));
          अवरोध;

        हाल nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->संख्या,
                          xml_escape (symbols[token]->tag));
          अवरोध;
        }

      /* The reason. */
      चयन (resolution)
        {
        हाल shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->प्रतीक->tag),
                          xml_escape_n (1, symbols[token]->tag));
          अवरोध;

        हाल reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->प्रतीक->tag));
          अवरोध;

        हाल बायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          अवरोध;

        हाल दायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          अवरोध;

        हाल nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      अवरोध;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

अटल व्योम
flush_shift (state *s, पूर्णांक token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  क्रम (पूर्णांक i = 0; i < trans->अंक; ++i)
    अगर (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_प्रतीक (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

अटल व्योम
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

अटल व्योम
resolve_sr_conflict (state *s, पूर्णांक ruleno, प्रतीक **errors, पूर्णांक *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  पूर्णांक redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  क्रम (प्रतीक_संख्या i = 0; i < ntokens; ++i)
    अगर (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        अगर (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->संख्या, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        अन्यथा अगर (symbols[i]->content->prec > redprec)
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

          चयन (symbols[i]->content->assoc)
            {
            हाल undef_assoc:
              पात ();

            हाल precedence_assoc:
              अवरोध;

            हाल दायाँ_assoc:
              register_assoc (i, redrule->prec->संख्या);
              log_resolution (redrule, i, दायाँ_resolution);
              flush_reduce (lookaheads, i);
              अवरोध;

            हाल बायाँ_assoc:
              register_assoc (i, redrule->prec->संख्या);
              log_resolution (redrule, i, बायाँ_resolution);
              flush_shift (s, i);
              अवरोध;

            हाल non_assoc:
              register_assoc (i, redrule->prec->संख्या);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              अवरोध;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

अटल व्योम
set_conflicts (state *s, प्रतीक **errors)
{
  अगर (s->consistent)
    वापस;

  reductions *reds = s->reductions;
  पूर्णांक nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_प्रतीक (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
    अगर (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  अगर (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  अगर (obstack_object_आकार (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  अगर (obstack_object_आकार (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  अगर (reds->lookaheads)
    क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
      {
        अगर (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->संख्या] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

व्योम
conflicts_solve (व्योम)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  प्रतीक **errors = xnmalloc (ntokens + 1, माप *errors);

  conflicts = xcalloc (nstates, माप *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  क्रम (state_संख्या i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      अगर (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  मुक्त (errors);
}


व्योम
conflicts_update_state_numbers (state_संख्या old_to_new[],
                                state_संख्या nstates_old)
{
  क्रम (state_संख्या i = 0; i < nstates_old; ++i)
    अगर (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

अटल माप_प्रकार
गिनती_state_sr_conflicts (स्थिर state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  अगर (!trans)
    वापस 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_प्रतीक (trans, i));
  }

  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  वापस bitset_गिनती (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

अटल माप_प्रकार
गिनती_sr_conflicts (व्योम)
{
  माप_प्रकार res = 0;
  /* Conflicts by state.  */
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    अगर (conflicts[i])
      res += गिनती_state_sr_conflicts (states[i]);
  वापस res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

अटल माप_प्रकार
गिनती_state_rr_conflicts (स्थिर state *s)
{
  reductions *reds = s->reductions;
  माप_प्रकार res = 0;

  क्रम (प्रतीक_संख्या i = 0; i < ntokens; ++i)
    {
      पूर्णांक गिनती = 0;
      क्रम (पूर्णांक j = 0; j < reds->अंक; ++j)
        गिनती += bitset_test (reds->lookaheads[j], i);
      अगर (2 <= गिनती)
        res += गिनती-1;
    }

  वापस res;
}

अटल माप_प्रकार
गिनती_rr_conflicts (व्योम)
{
  माप_प्रकार res = 0;
  /* Conflicts by state.  */
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    अगर (conflicts[i])
      res += गिनती_state_rr_conflicts (states[i]);
  वापस res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

अटल माप_प्रकार
गिनती_rule_state_sr_conflicts (rule *r, state *s)
{
  माप_प्रकार res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
    अगर (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        पूर्णांक j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_प्रतीक (trans, j));
      }

  वापस res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

अटल माप_प्रकार
गिनती_rule_sr_conflicts (rule *r)
{
  माप_प्रकार res = 0;
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    अगर (conflicts[i])
      res += गिनती_rule_state_sr_conflicts (r, states[i]);
  वापस res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

अटल माप_प्रकार
गिनती_rule_state_rr_conflicts (rule *r, state *s)
{
  माप_प्रकार res = 0;
  स्थिर reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
    अगर (reds->rules[i] == r)
      क्रम (पूर्णांक j = 0; j < reds->अंक; ++j)
        अगर (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_गिनती (lookaheads);
          }
  bitset_free (lookaheads);
  वापस res;
}

अटल माप_प्रकार
गिनती_rule_rr_conflicts (rule *r)
{
  माप_प्रकार res = 0;
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    res += गिनती_rule_state_rr_conflicts (r, states[i]);
  वापस res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

व्योम
conflicts_output (खाता *out)
{
  bool printed_sth = false;
  क्रम (state_संख्या i = 0; i < nstates; ++i)
    अगर (conflicts[i])
      {
        स्थिर state *s = states[i];
        पूर्णांक src = गिनती_state_sr_conflicts (s);
        पूर्णांक rrc = गिनती_state_rr_conflicts (s);
        ख_लिखो (out, _("State %d "), i);
        अगर (src && rrc)
          ख_लिखो (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        अन्यथा अगर (src)
          ख_लिखो (out, _("conflicts: %d shift/reduce\n"), src);
        अन्यथा अगर (rrc)
          ख_लिखो (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  अगर (printed_sth)
    ख_माला_दो ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

पूर्णांक
conflicts_योग_गिनती (व्योम)
{
  वापस गिनती_sr_conflicts () + गिनती_rr_conflicts ();
}

अटल व्योम
report_counterexamples (व्योम)
{
  क्रम (state_संख्या sn = 0; sn < nstates; ++sn)
    अगर (conflicts[sn])
      counterexample_report_state (states[sn], मानक_त्रुटि, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

अटल व्योम
report_rule_expectation_mismatches (व्योम)
{
  क्रम (rule_संख्या i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      पूर्णांक expected_sr = r->expected_sr_conflicts;
      पूर्णांक expected_rr = r->expected_rr_conflicts;

      अगर (expected_sr != -1 || expected_rr != -1)
        {
          पूर्णांक sr = गिनती_rule_sr_conflicts (r);
          अगर (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          पूर्णांक rr = गिनती_rule_rr_conflicts (r);
          अगर (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

व्योम
conflicts_print (व्योम)
{
  report_rule_expectation_mismatches ();

  अगर (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (शून्य, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    पूर्णांक योग = गिनती_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    पूर्णांक expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    अगर (expected != -1)
      {
        अगर (expected != योग)
          {
            complain (शून्य, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      योग, expected);
            अगर (योग)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा अगर (योग)
      {
        complain (शून्य, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            योग),
                  योग);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    पूर्णांक योग = गिनती_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    पूर्णांक expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    अगर (expected != -1)
      {
        अगर (expected != योग)
          {
            complain (शून्य, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      योग, expected);
            अगर (योग)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा अगर (योग)
      {
        complain (शून्य, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            योग),
                  योग);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  अगर (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  अन्यथा अगर (unexpected_conflicts_warning != Wnone)
    subcomplain (शून्य, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

व्योम
conflicts_free (व्योम)
{
  मुक्त (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, शून्य);
  obstack_free (&solved_conflicts_xml_obstack, शून्य);
}
