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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>

#شامل "complain.h"
#شامل "conflicts.h"
#شامل "counterexample.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "print-xml.h"
#شامل "reader.h"
#شامل "state.h"
#شامل "symtab.h"

/* -1 stands for not specified. */
صحیح expected_sr_conflicts = -1;
صحیح expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
ایستا bool *conflicts;

ایستا ساختار obstack solved_conflicts_obstack;
ایستا ساختار obstack solved_conflicts_xml_obstack;

ایستا bitset shift_set;
ایستا bitset lookahead_set;

bool
has_conflicts (ثابت state *s)
{
  بازگشت conflicts[s->عدد];
}



شمارش conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    چپ_resolution,
    راست_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

ایستا درونخطی تهی
log_resolution (rule *r, نماد_عدد token,
                شمارش conflict_resolution resolution)
{
  اگر (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      انتخاب (resolution)
        {
        حالت shift_resolution:
        حالت راست_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->عدد,
                          symbols[token]->tag);
          بشکن;

        حالت reduce_resolution:
        حالت چپ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->عدد,
                          symbols[token]->tag);
          بشکن;

        حالت nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->عدد,
                          symbols[token]->tag);
          بشکن;
        }

      /* The reason. */
      انتخاب (resolution)
        {
        حالت shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->نماد->tag,
                          symbols[token]->tag);
          بشکن;

        حالت reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->نماد->tag);
          بشکن;

        حالت چپ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          بشکن;

        حالت راست_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          بشکن;

        حالت nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          بشکن;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  اگر (xml_flag)
    {
      /* The description of the resolution. */
      انتخاب (resolution)
        {
        حالت shift_resolution:
        حالت راست_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->عدد,
                          xml_escape (symbols[token]->tag));
          بشکن;

        حالت reduce_resolution:
        حالت چپ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->عدد,
                          xml_escape (symbols[token]->tag));
          بشکن;

        حالت nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->عدد,
                          xml_escape (symbols[token]->tag));
          بشکن;
        }

      /* The reason. */
      انتخاب (resolution)
        {
        حالت shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->نماد->tag),
                          xml_escape_n (1, symbols[token]->tag));
          بشکن;

        حالت reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->نماد->tag));
          بشکن;

        حالت چپ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          بشکن;

        حالت راست_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          بشکن;

        حالت nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      بشکن;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

ایستا تهی
flush_shift (state *s, صحیح token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  برای (صحیح i = 0; i < trans->رقم; ++i)
    اگر (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_نماد (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

ایستا تهی
flush_reduce (bitset lookaheads, صحیح token)
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

ایستا تهی
resolve_sr_conflict (state *s, صحیح ruleno, نماد **errors, صحیح *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  صحیح redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  برای (نماد_عدد i = 0; i < ntokens; ++i)
    اگر (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        اگر (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->عدد, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        وگرنه اگر (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->عدد);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        وگرنه
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          انتخاب (symbols[i]->content->assoc)
            {
            حالت undef_assoc:
              abort ();

            حالت precedence_assoc:
              بشکن;

            حالت راست_assoc:
              register_assoc (i, redrule->prec->عدد);
              log_resolution (redrule, i, راست_resolution);
              flush_reduce (lookaheads, i);
              بشکن;

            حالت چپ_assoc:
              register_assoc (i, redrule->prec->عدد);
              log_resolution (redrule, i, چپ_resolution);
              flush_shift (s, i);
              بشکن;

            حالت non_assoc:
              register_assoc (i, redrule->prec->عدد);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              بشکن;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

ایستا تهی
set_conflicts (state *s, نماد **errors)
{
  اگر (s->consistent)
    بازگشت;

  reductions *reds = s->reductions;
  صحیح nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    صحیح i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_نماد (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  برای (صحیح i = 0; i < reds->رقم; ++i)
    اگر (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  اگر (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  اگر (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  اگر (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  اگر (reds->lookaheads)
    برای (صحیح i = 0; i < reds->رقم; ++i)
      {
        اگر (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->عدد] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

تهی
conflicts_solve (تهی)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  نماد **errors = xnmalloc (ntokens + 1, اندازه *errors);

  conflicts = xcalloc (nstates, اندازه *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  برای (state_عدد i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      اگر (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  آزاد (errors);
}


تهی
conflicts_update_state_numbers (state_عدد old_to_new[],
                                state_عدد nstates_old)
{
  برای (state_عدد i = 0; i < nstates_old; ++i)
    اگر (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

ایستا size_t
شمار_state_sr_conflicts (ثابت state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  اگر (!trans)
    بازگشت 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    صحیح i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_نماد (trans, i));
  }

  برای (صحیح i = 0; i < reds->رقم; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  بازگشت bitset_شمار (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

ایستا size_t
شمار_sr_conflicts (تهی)
{
  size_t res = 0;
  /* Conflicts by state.  */
  برای (state_عدد i = 0; i < nstates; ++i)
    اگر (conflicts[i])
      res += شمار_state_sr_conflicts (states[i]);
  بازگشت res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

ایستا size_t
شمار_state_rr_conflicts (ثابت state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  برای (نماد_عدد i = 0; i < ntokens; ++i)
    {
      صحیح شمار = 0;
      برای (صحیح j = 0; j < reds->رقم; ++j)
        شمار += bitset_test (reds->lookaheads[j], i);
      اگر (2 <= شمار)
        res += شمار-1;
    }

  بازگشت res;
}

ایستا size_t
شمار_rr_conflicts (تهی)
{
  size_t res = 0;
  /* Conflicts by state.  */
  برای (state_عدد i = 0; i < nstates; ++i)
    اگر (conflicts[i])
      res += شمار_state_rr_conflicts (states[i]);
  بازگشت res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

ایستا size_t
شمار_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  برای (صحیح i = 0; i < reds->رقم; ++i)
    اگر (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        صحیح j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_نماد (trans, j));
      }

  بازگشت res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

ایستا size_t
شمار_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  برای (state_عدد i = 0; i < nstates; ++i)
    اگر (conflicts[i])
      res += شمار_rule_state_sr_conflicts (r, states[i]);
  بازگشت res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

ایستا size_t
شمار_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  برای (صحیح i = 0; i < reds->رقم; ++i)
    اگر (reds->rules[i] == r)
      برای (صحیح j = 0; j < reds->رقم; ++j)
        اگر (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_شمار (lookaheads);
          }
  bitset_free (lookaheads);
  بازگشت res;
}

ایستا size_t
شمار_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  برای (state_عدد i = 0; i < nstates; ++i)
    res += شمار_rule_state_rr_conflicts (r, states[i]);
  بازگشت res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

تهی
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  برای (state_عدد i = 0; i < nstates; ++i)
    اگر (conflicts[i])
      {
        ثابت state *s = states[i];
        صحیح src = شمار_state_sr_conflicts (s);
        صحیح rrc = شمار_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        اگر (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        وگرنه اگر (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        وگرنه اگر (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  اگر (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

صحیح
conflicts_total_شمار (تهی)
{
  بازگشت شمار_sr_conflicts () + شمار_rr_conflicts ();
}

ایستا تهی
report_counterexamples (تهی)
{
  برای (state_عدد sn = 0; sn < nstates; ++sn)
    اگر (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

ایستا تهی
report_rule_expectation_mismatches (تهی)
{
  برای (rule_عدد i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      صحیح expected_sr = r->expected_sr_conflicts;
      صحیح expected_rr = r->expected_rr_conflicts;

      اگر (expected_sr != -1 || expected_rr != -1)
        {
          صحیح sr = شمار_rule_sr_conflicts (r);
          اگر (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          صحیح rr = شمار_rule_rr_conflicts (r);
          اگر (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

تهی
conflicts_print (تهی)
{
  report_rule_expectation_mismatches ();

  اگر (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (پوچ, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    صحیح total = شمار_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    صحیح expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    اگر (expected != -1)
      {
        اگر (expected != total)
          {
            complain (پوچ, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            اگر (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    وگرنه اگر (total)
      {
        complain (پوچ, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    صحیح total = شمار_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    صحیح expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    اگر (expected != -1)
      {
        اگر (expected != total)
          {
            complain (پوچ, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            اگر (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    وگرنه اگر (total)
      {
        complain (پوچ, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  اگر (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  وگرنه اگر (unexpected_conflicts_warning != Wnone)
    subcomplain (پوچ, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

تهی
conflicts_free (تهی)
{
  آزاد (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, پوچ);
  obstack_free (&solved_conflicts_xml_obstack, پوچ);
}
