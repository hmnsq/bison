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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "counterexample.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "print-xml.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "state.h"
#அந்தர்பாவ "symtab.h"

/* -1 stands for not specified. */
முழு expected_sr_conflicts = -1;
முழு expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
நிலையான bool *conflicts;

நிலையான கட்டமைப்பு obstack solved_conflicts_obstack;
நிலையான கட்டமைப்பு obstack solved_conflicts_xml_obstack;

நிலையான bitset shift_set;
நிலையான bitset lookahead_set;

bool
has_conflicts (மாறிலி state *s)
{
  திரும்பு conflicts[s->எண்];
}



எண்ணிக்கை conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    இடது_resolution,
    வலது_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

நிலையான உள்வரி வெறுமை
log_resolution (rule *r, குறியீடு_எண் token,
                எண்ணிக்கை conflict_resolution resolution)
{
  எனில் (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      தேர்வு (resolution)
        {
        நிலை shift_resolution:
        நிலை வலது_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->எண்,
                          symbols[token]->tag);
          நிறுத்து;

        நிலை reduce_resolution:
        நிலை இடது_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->எண்,
                          symbols[token]->tag);
          நிறுத்து;

        நிலை nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->எண்,
                          symbols[token]->tag);
          நிறுத்து;
        }

      /* The reason. */
      தேர்வு (resolution)
        {
        நிலை shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->குறியீடு->tag,
                          symbols[token]->tag);
          நிறுத்து;

        நிலை reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->குறியீடு->tag);
          நிறுத்து;

        நிலை இடது_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          நிறுத்து;

        நிலை வலது_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          நிறுத்து;

        நிலை nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          நிறுத்து;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  எனில் (xml_flag)
    {
      /* The description of the resolution. */
      தேர்வு (resolution)
        {
        நிலை shift_resolution:
        நிலை வலது_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->எண்,
                          xml_escape (symbols[token]->tag));
          நிறுத்து;

        நிலை reduce_resolution:
        நிலை இடது_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->எண்,
                          xml_escape (symbols[token]->tag));
          நிறுத்து;

        நிலை nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->எண்,
                          xml_escape (symbols[token]->tag));
          நிறுத்து;
        }

      /* The reason. */
      தேர்வு (resolution)
        {
        நிலை shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->குறியீடு->tag),
                          xml_escape_n (1, symbols[token]->tag));
          நிறுத்து;

        நிலை reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->குறியீடு->tag));
          நிறுத்து;

        நிலை இடது_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          நிறுத்து;

        நிலை வலது_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          நிறுத்து;

        நிலை nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      நிறுத்து;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

நிலையான வெறுமை
flush_shift (state *s, முழு token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  க்கு (முழு i = 0; i < trans->இலக்கம்; ++i)
    எனில் (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_குறியீடு (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

நிலையான வெறுமை
flush_reduce (bitset lookaheads, முழு token)
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

நிலையான வெறுமை
resolve_sr_conflict (state *s, முழு ruleno, குறியீடு **errors, முழு *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  முழு redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  க்கு (குறியீடு_எண் i = 0; i < ntokens; ++i)
    எனில் (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        எனில் (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->எண், i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        இல்லையெனில் எனில் (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->எண்);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        இல்லையெனில்
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          தேர்வு (symbols[i]->content->assoc)
            {
            நிலை undef_assoc:
              abort ();

            நிலை precedence_assoc:
              நிறுத்து;

            நிலை வலது_assoc:
              register_assoc (i, redrule->prec->எண்);
              log_resolution (redrule, i, வலது_resolution);
              flush_reduce (lookaheads, i);
              நிறுத்து;

            நிலை இடது_assoc:
              register_assoc (i, redrule->prec->எண்);
              log_resolution (redrule, i, இடது_resolution);
              flush_shift (s, i);
              நிறுத்து;

            நிலை non_assoc:
              register_assoc (i, redrule->prec->எண்);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              நிறுத்து;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
set_conflicts (state *s, குறியீடு **errors)
{
  எனில் (s->consistent)
    திரும்பு;

  reductions *reds = s->reductions;
  முழு nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    முழு i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_குறியீடு (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
    எனில் (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  எனில் (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  எனில் (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  எனில் (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  எனில் (reds->lookaheads)
    க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
      {
        எனில் (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->எண்] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

வெறுமை
conflicts_solve (வெறுமை)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  குறியீடு **errors = xnmalloc (ntokens + 1, அளவு *errors);

  conflicts = xcalloc (nstates, அளவு *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      எனில் (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  விடுவி (errors);
}


வெறுமை
conflicts_update_state_numbers (state_எண் old_to_new[],
                                state_எண் nstates_old)
{
  க்கு (state_எண் i = 0; i < nstates_old; ++i)
    எனில் (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

நிலையான size_t
மொத்தம்_state_sr_conflicts (மாறிலி state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  எனில் (!trans)
    திரும்பு 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    முழு i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_குறியீடு (trans, i));
  }

  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  திரும்பு bitset_மொத்தம் (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

நிலையான size_t
மொத்தம்_sr_conflicts (வெறுமை)
{
  size_t res = 0;
  /* Conflicts by state.  */
  க்கு (state_எண் i = 0; i < nstates; ++i)
    எனில் (conflicts[i])
      res += மொத்தம்_state_sr_conflicts (states[i]);
  திரும்பு res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

நிலையான size_t
மொத்தம்_state_rr_conflicts (மாறிலி state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  க்கு (குறியீடு_எண் i = 0; i < ntokens; ++i)
    {
      முழு மொத்தம் = 0;
      க்கு (முழு j = 0; j < reds->இலக்கம்; ++j)
        மொத்தம் += bitset_test (reds->lookaheads[j], i);
      எனில் (2 <= மொத்தம்)
        res += மொத்தம்-1;
    }

  திரும்பு res;
}

நிலையான size_t
மொத்தம்_rr_conflicts (வெறுமை)
{
  size_t res = 0;
  /* Conflicts by state.  */
  க்கு (state_எண் i = 0; i < nstates; ++i)
    எனில் (conflicts[i])
      res += மொத்தம்_state_rr_conflicts (states[i]);
  திரும்பு res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

நிலையான size_t
மொத்தம்_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
    எனில் (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        முழு j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_குறியீடு (trans, j));
      }

  திரும்பு res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

நிலையான size_t
மொத்தம்_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  க்கு (state_எண் i = 0; i < nstates; ++i)
    எனில் (conflicts[i])
      res += மொத்தம்_rule_state_sr_conflicts (r, states[i]);
  திரும்பு res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

நிலையான size_t
மொத்தம்_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  மாறிலி reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
    எனில் (reds->rules[i] == r)
      க்கு (முழு j = 0; j < reds->இலக்கம்; ++j)
        எனில் (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_மொத்தம் (lookaheads);
          }
  bitset_free (lookaheads);
  திரும்பு res;
}

நிலையான size_t
மொத்தம்_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  க்கு (state_எண் i = 0; i < nstates; ++i)
    res += மொத்தம்_rule_state_rr_conflicts (r, states[i]);
  திரும்பு res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

வெறுமை
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  க்கு (state_எண் i = 0; i < nstates; ++i)
    எனில் (conflicts[i])
      {
        மாறிலி state *s = states[i];
        முழு src = மொத்தம்_state_sr_conflicts (s);
        முழு rrc = மொத்தம்_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        எனில் (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        இல்லையெனில் எனில் (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        இல்லையெனில் எனில் (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  எனில் (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

முழு
conflicts_total_மொத்தம் (வெறுமை)
{
  திரும்பு மொத்தம்_sr_conflicts () + மொத்தம்_rr_conflicts ();
}

நிலையான வெறுமை
report_counterexamples (வெறுமை)
{
  க்கு (state_எண் sn = 0; sn < nstates; ++sn)
    எனில் (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

நிலையான வெறுமை
report_rule_expectation_mismatches (வெறுமை)
{
  க்கு (rule_எண் i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      முழு expected_sr = r->expected_sr_conflicts;
      முழு expected_rr = r->expected_rr_conflicts;

      எனில் (expected_sr != -1 || expected_rr != -1)
        {
          முழு sr = மொத்தம்_rule_sr_conflicts (r);
          எனில் (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          முழு rr = மொத்தம்_rule_rr_conflicts (r);
          எனில் (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

வெறுமை
conflicts_print (வெறுமை)
{
  report_rule_expectation_mismatches ();

  எனில் (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (வெற்று, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    முழு total = மொத்தம்_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    முழு expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    எனில் (expected != -1)
      {
        எனில் (expected != total)
          {
            complain (வெற்று, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            எனில் (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    இல்லையெனில் எனில் (total)
      {
        complain (வெற்று, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    முழு total = மொத்தம்_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    முழு expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    எனில் (expected != -1)
      {
        எனில் (expected != total)
          {
            complain (வெற்று, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            எனில் (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    இல்லையெனில் எனில் (total)
      {
        complain (வெற்று, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  எனில் (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  இல்லையெனில் எனில் (unexpected_conflicts_warning != Wnone)
    subcomplain (வெற்று, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

வெறுமை
conflicts_free (வெறுமை)
{
  விடுவி (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, வெற்று);
  obstack_free (&solved_conflicts_xml_obstack, வெற்று);
}
