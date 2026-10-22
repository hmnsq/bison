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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "conflicts.h"
#অন্তর্ভাব "counterexample.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lr0.h"
#অন্তর্ভাব "print-xml.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "state.h"
#অন্তর্ভাব "symtab.h"

/* -1 stands for not specified. */
পূর্ণসংখ্যা expected_sr_conflicts = -1;
পূর্ণসংখ্যা expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
স্থির bool *conflicts;

স্থির কাঠামো obstack solved_conflicts_obstack;
স্থির কাঠামো obstack solved_conflicts_xml_obstack;

স্থির bitset shift_set;
স্থির bitset lookahead_set;

bool
has_conflicts (ধ্রুবক state *s)
{
  ফেরত conflicts[s->সংখ্যা];
}



গণনা conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    বাম_resolution,
    ডান_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

স্থির অন্তঃরেখ শূন্য
log_resolution (rule *r, প্রতীক_সংখ্যা token,
                গণনা conflict_resolution resolution)
{
  যদি (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      নির্বাচন (resolution)
        {
        ক্ষেত্র shift_resolution:
        ক্ষেত্র ডান_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->সংখ্যা,
                          symbols[token]->tag);
          বিরতি;

        ক্ষেত্র reduce_resolution:
        ক্ষেত্র বাম_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->সংখ্যা,
                          symbols[token]->tag);
          বিরতি;

        ক্ষেত্র nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->সংখ্যা,
                          symbols[token]->tag);
          বিরতি;
        }

      /* The reason. */
      নির্বাচন (resolution)
        {
        ক্ষেত্র shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->প্রতীক->tag,
                          symbols[token]->tag);
          বিরতি;

        ক্ষেত্র reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->প্রতীক->tag);
          বিরতি;

        ক্ষেত্র বাম_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          বিরতি;

        ক্ষেত্র ডান_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          বিরতি;

        ক্ষেত্র nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          বিরতি;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  যদি (xml_flag)
    {
      /* The description of the resolution. */
      নির্বাচন (resolution)
        {
        ক্ষেত্র shift_resolution:
        ক্ষেত্র ডান_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->সংখ্যা,
                          xml_escape (symbols[token]->tag));
          বিরতি;

        ক্ষেত্র reduce_resolution:
        ক্ষেত্র বাম_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->সংখ্যা,
                          xml_escape (symbols[token]->tag));
          বিরতি;

        ক্ষেত্র nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->সংখ্যা,
                          xml_escape (symbols[token]->tag));
          বিরতি;
        }

      /* The reason. */
      নির্বাচন (resolution)
        {
        ক্ষেত্র shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->প্রতীক->tag),
                          xml_escape_n (1, symbols[token]->tag));
          বিরতি;

        ক্ষেত্র reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->প্রতীক->tag));
          বিরতি;

        ক্ষেত্র বাম_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          বিরতি;

        ক্ষেত্র ডান_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          বিরতি;

        ক্ষেত্র nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      বিরতি;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

স্থির শূন্য
flush_shift (state *s, পূর্ণসংখ্যা token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  জন্য (পূর্ণসংখ্যা i = 0; i < trans->অঙ্ক; ++i)
    যদি (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_প্রতীক (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

স্থির শূন্য
flush_reduce (bitset lookaheads, পূর্ণসংখ্যা token)
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

স্থির শূন্য
resolve_sr_conflict (state *s, পূর্ণসংখ্যা ruleno, প্রতীক **errors, পূর্ণসংখ্যা *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  পূর্ণসংখ্যা redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  জন্য (প্রতীক_সংখ্যা i = 0; i < ntokens; ++i)
    যদি (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        যদি (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->সংখ্যা, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        অন্যথা যদি (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->সংখ্যা);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        অন্যথা
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          নির্বাচন (symbols[i]->content->assoc)
            {
            ক্ষেত্র undef_assoc:
              abort ();

            ক্ষেত্র precedence_assoc:
              বিরতি;

            ক্ষেত্র ডান_assoc:
              register_assoc (i, redrule->prec->সংখ্যা);
              log_resolution (redrule, i, ডান_resolution);
              flush_reduce (lookaheads, i);
              বিরতি;

            ক্ষেত্র বাম_assoc:
              register_assoc (i, redrule->prec->সংখ্যা);
              log_resolution (redrule, i, বাম_resolution);
              flush_shift (s, i);
              বিরতি;

            ক্ষেত্র non_assoc:
              register_assoc (i, redrule->prec->সংখ্যা);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              বিরতি;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

স্থির শূন্য
set_conflicts (state *s, প্রতীক **errors)
{
  যদি (s->consistent)
    ফেরত;

  reductions *reds = s->reductions;
  পূর্ণসংখ্যা nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    পূর্ণসংখ্যা i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_প্রতীক (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
    যদি (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  যদি (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  যদি (obstack_object_আকার (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  যদি (obstack_object_আকার (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  যদি (reds->lookaheads)
    জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
      {
        যদি (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->সংখ্যা] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

শূন্য
conflicts_solve (শূন্য)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  প্রতীক **errors = xnmalloc (ntokens + 1, আকার *errors);

  conflicts = xcalloc (nstates, আকার *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      যদি (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  মুক্ত (errors);
}


শূন্য
conflicts_update_state_numbers (state_সংখ্যা old_to_new[],
                                state_সংখ্যা nstates_old)
{
  জন্য (state_সংখ্যা i = 0; i < nstates_old; ++i)
    যদি (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

স্থির size_t
গুনতি_state_sr_conflicts (ধ্রুবক state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  যদি (!trans)
    ফেরত 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    পূর্ণসংখ্যা i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_প্রতীক (trans, i));
  }

  জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  ফেরত bitset_গুনতি (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

স্থির size_t
গুনতি_sr_conflicts (শূন্য)
{
  size_t res = 0;
  /* Conflicts by state.  */
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    যদি (conflicts[i])
      res += গুনতি_state_sr_conflicts (states[i]);
  ফেরত res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

স্থির size_t
গুনতি_state_rr_conflicts (ধ্রুবক state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  জন্য (প্রতীক_সংখ্যা i = 0; i < ntokens; ++i)
    {
      পূর্ণসংখ্যা গুনতি = 0;
      জন্য (পূর্ণসংখ্যা j = 0; j < reds->অঙ্ক; ++j)
        গুনতি += bitset_test (reds->lookaheads[j], i);
      যদি (2 <= গুনতি)
        res += গুনতি-1;
    }

  ফেরত res;
}

স্থির size_t
গুনতি_rr_conflicts (শূন্য)
{
  size_t res = 0;
  /* Conflicts by state.  */
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    যদি (conflicts[i])
      res += গুনতি_state_rr_conflicts (states[i]);
  ফেরত res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

স্থির size_t
গুনতি_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
    যদি (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        পূর্ণসংখ্যা j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_প্রতীক (trans, j));
      }

  ফেরত res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

স্থির size_t
গুনতি_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    যদি (conflicts[i])
      res += গুনতি_rule_state_sr_conflicts (r, states[i]);
  ফেরত res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

স্থির size_t
গুনতি_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  ধ্রুবক reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  জন্য (পূর্ণসংখ্যা i = 0; i < reds->অঙ্ক; ++i)
    যদি (reds->rules[i] == r)
      জন্য (পূর্ণসংখ্যা j = 0; j < reds->অঙ্ক; ++j)
        যদি (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_গুনতি (lookaheads);
          }
  bitset_free (lookaheads);
  ফেরত res;
}

স্থির size_t
গুনতি_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    res += গুনতি_rule_state_rr_conflicts (r, states[i]);
  ফেরত res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

শূন্য
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    যদি (conflicts[i])
      {
        ধ্রুবক state *s = states[i];
        পূর্ণসংখ্যা src = গুনতি_state_sr_conflicts (s);
        পূর্ণসংখ্যা rrc = গুনতি_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        যদি (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        অন্যথা যদি (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        অন্যথা যদি (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  যদি (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

পূর্ণসংখ্যা
conflicts_যোগ_গুনতি (শূন্য)
{
  ফেরত গুনতি_sr_conflicts () + গুনতি_rr_conflicts ();
}

স্থির শূন্য
report_counterexamples (শূন্য)
{
  জন্য (state_সংখ্যা sn = 0; sn < nstates; ++sn)
    যদি (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

স্থির শূন্য
report_rule_expectation_mismatches (শূন্য)
{
  জন্য (rule_সংখ্যা i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      পূর্ণসংখ্যা expected_sr = r->expected_sr_conflicts;
      পূর্ণসংখ্যা expected_rr = r->expected_rr_conflicts;

      যদি (expected_sr != -1 || expected_rr != -1)
        {
          পূর্ণসংখ্যা sr = গুনতি_rule_sr_conflicts (r);
          যদি (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          পূর্ণসংখ্যা rr = গুনতি_rule_rr_conflicts (r);
          যদি (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

শূন্য
conflicts_print (শূন্য)
{
  report_rule_expectation_mismatches ();

  যদি (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (ফাঁকা, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    পূর্ণসংখ্যা যোগ = গুনতি_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    পূর্ণসংখ্যা expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    যদি (expected != -1)
      {
        যদি (expected != যোগ)
          {
            complain (ফাঁকা, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      যোগ, expected);
            যদি (যোগ)
              unexpected_conflicts_warning = complaint;
          }
      }
    অন্যথা যদি (যোগ)
      {
        complain (ফাঁকা, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            যোগ),
                  যোগ);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    পূর্ণসংখ্যা যোগ = গুনতি_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    পূর্ণসংখ্যা expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    যদি (expected != -1)
      {
        যদি (expected != যোগ)
          {
            complain (ফাঁকা, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      যোগ, expected);
            যদি (যোগ)
              unexpected_conflicts_warning = complaint;
          }
      }
    অন্যথা যদি (যোগ)
      {
        complain (ফাঁকা, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            যোগ),
                  যোগ);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  যদি (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  অন্যথা যদি (unexpected_conflicts_warning != Wnone)
    subcomplain (ফাঁকা, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

শূন্য
conflicts_free (শূন্য)
{
  মুক্ত (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, ফাঁকা);
  obstack_free (&solved_conflicts_xml_obstack, ফাঁকা);
}
