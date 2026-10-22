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

#include <config.h>
#include "system.h"

#include <bitset.h>

#include "complain.h"
#include "conflicts.h"
#include "counterexample.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "lalr.h"
#include "lr0.h"
#include "print-xml.h"
#include "reader.h"
#include "state.h"
#include "symtab.h"

/* -1 stands for not specified. */
จำนวนเต็ม expected_sr_conflicts = -1;
จำนวนเต็ม expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
static bool *conflicts;

static sa_nracanaa obstack solved_conflicts_obstack;
static sa_nracanaa obstack solved_conflicts_xml_obstack;

static bitset shift_set;
static bitset lookahead_set;

bool
has_conflicts (const state *s)
{
  คืน conflicts[s->number];
}



ga_nanaa conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    left_resolution,
    right_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

static anta_hstha void
log_resolution (rule *r, symbol_number token,
                ga_nanaa conflict_resolution resolution)
{
  ถ้า (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      switch (resolution)
        {
        pak_sa shift_resolution:
        pak_sa right_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->number,
                          symbols[token]->tag);
          bha_nga;

        pak_sa reduce_resolution:
        pak_sa left_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->number,
                          symbols[token]->tag);
          bha_nga;

        pak_sa nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->number,
                          symbols[token]->tag);
          bha_nga;
        }

      /* The reason. */
      switch (resolution)
        {
        pak_sa shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->symbol->tag,
                          symbols[token]->tag);
          bha_nga;

        pak_sa reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->symbol->tag);
          bha_nga;

        pak_sa left_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          bha_nga;

        pak_sa right_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          bha_nga;

        pak_sa nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          bha_nga;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  ถ้า (xml_flag)
    {
      /* The description of the resolution. */
      switch (resolution)
        {
        pak_sa shift_resolution:
        pak_sa right_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->number,
                          xml_escape (symbols[token]->tag));
          bha_nga;

        pak_sa reduce_resolution:
        pak_sa left_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->number,
                          xml_escape (symbols[token]->tag));
          bha_nga;

        pak_sa nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->number,
                          xml_escape (symbols[token]->tag));
          bha_nga;
        }

      /* The reason. */
      switch (resolution)
        {
        pak_sa shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->symbol->tag),
                          xml_escape_n (1, symbols[token]->tag));
          bha_nga;

        pak_sa reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->symbol->tag));
          bha_nga;

        pak_sa left_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          bha_nga;

        pak_sa right_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          bha_nga;

        pak_sa nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      bha_nga;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

static void
flush_shift (state *s, จำนวนเต็ม token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  สำหรับ (จำนวนเต็ม i = 0; i < trans->num; ++i)
    ถ้า (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_SYMBOL (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

static void
flush_reduce (bitset lookaheads, จำนวนเต็ม token)
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

static void
resolve_sr_conflict (state *s, จำนวนเต็ม ruleno, symbol **errors, จำนวนเต็ม *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  จำนวนเต็ม redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  สำหรับ (symbol_number i = 0; i < ntokens; ++i)
    ถ้า (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        ถ้า (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->number, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        มิฉะนั้น ถ้า (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->number);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        มิฉะนั้น
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          switch (symbols[i]->content->assoc)
            {
            pak_sa undef_assoc:
              abort ();

            pak_sa precedence_assoc:
              bha_nga;

            pak_sa right_assoc:
              register_assoc (i, redrule->prec->number);
              log_resolution (redrule, i, right_resolution);
              flush_reduce (lookaheads, i);
              bha_nga;

            pak_sa left_assoc:
              register_assoc (i, redrule->prec->number);
              log_resolution (redrule, i, left_resolution);
              flush_shift (s, i);
              bha_nga;

            pak_sa non_assoc:
              register_assoc (i, redrule->prec->number);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              bha_nga;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

static void
set_conflicts (state *s, symbol **errors)
{
  ถ้า (s->consistent)
    คืน;

  reductions *reds = s->reductions;
  จำนวนเต็ม nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    จำนวนเต็ม i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_SYMBOL (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  สำหรับ (จำนวนเต็ม i = 0; i < reds->num; ++i)
    ถ้า (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  ถ้า (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  ถ้า (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  ถ้า (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  ถ้า (reds->lookaheads)
    สำหรับ (จำนวนเต็ม i = 0; i < reds->num; ++i)
      {
        ถ้า (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->number] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

void
conflicts_solve (void)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  symbol **errors = xnmalloc (ntokens + 1, parimaa_na *errors);

  conflicts = xcalloc (nstates, parimaa_na *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  สำหรับ (state_number i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      ถ้า (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  free (errors);
}


void
conflicts_update_state_numbers (state_number old_to_new[],
                                state_number nstates_old)
{
  สำหรับ (state_number i = 0; i < nstates_old; ++i)
    ถ้า (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

static size_t
count_state_sr_conflicts (const state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ถ้า (!trans)
    คืน 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    จำนวนเต็ม i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_SYMBOL (trans, i));
  }

  สำหรับ (จำนวนเต็ม i = 0; i < reds->num; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  คืน bitset_count (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

static size_t
count_sr_conflicts (void)
{
  size_t res = 0;
  /* Conflicts by state.  */
  สำหรับ (state_number i = 0; i < nstates; ++i)
    ถ้า (conflicts[i])
      res += count_state_sr_conflicts (states[i]);
  คืน res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

static size_t
count_state_rr_conflicts (const state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  สำหรับ (symbol_number i = 0; i < ntokens; ++i)
    {
      จำนวนเต็ม count = 0;
      สำหรับ (จำนวนเต็ม j = 0; j < reds->num; ++j)
        count += bitset_test (reds->lookaheads[j], i);
      ถ้า (2 <= count)
        res += count-1;
    }

  คืน res;
}

static size_t
count_rr_conflicts (void)
{
  size_t res = 0;
  /* Conflicts by state.  */
  สำหรับ (state_number i = 0; i < nstates; ++i)
    ถ้า (conflicts[i])
      res += count_state_rr_conflicts (states[i]);
  คืน res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

static size_t
count_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  สำหรับ (จำนวนเต็ม i = 0; i < reds->num; ++i)
    ถ้า (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        จำนวนเต็ม j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_SYMBOL (trans, j));
      }

  คืน res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

static size_t
count_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  สำหรับ (state_number i = 0; i < nstates; ++i)
    ถ้า (conflicts[i])
      res += count_rule_state_sr_conflicts (r, states[i]);
  คืน res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

static size_t
count_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  const reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  สำหรับ (จำนวนเต็ม i = 0; i < reds->num; ++i)
    ถ้า (reds->rules[i] == r)
      สำหรับ (จำนวนเต็ม j = 0; j < reds->num; ++j)
        ถ้า (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_count (lookaheads);
          }
  bitset_free (lookaheads);
  คืน res;
}

static size_t
count_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  สำหรับ (state_number i = 0; i < nstates; ++i)
    res += count_rule_state_rr_conflicts (r, states[i]);
  คืน res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

void
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  สำหรับ (state_number i = 0; i < nstates; ++i)
    ถ้า (conflicts[i])
      {
        const state *s = states[i];
        จำนวนเต็ม src = count_state_sr_conflicts (s);
        จำนวนเต็ม rrc = count_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        ถ้า (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        มิฉะนั้น ถ้า (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        มิฉะนั้น ถ้า (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  ถ้า (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

จำนวนเต็ม
conflicts_total_count (void)
{
  คืน count_sr_conflicts () + count_rr_conflicts ();
}

static void
report_counterexamples (void)
{
  สำหรับ (state_number sn = 0; sn < nstates; ++sn)
    ถ้า (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

static void
report_rule_expectation_mismatches (void)
{
  สำหรับ (rule_number i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      จำนวนเต็ม expected_sr = r->expected_sr_conflicts;
      จำนวนเต็ม expected_rr = r->expected_rr_conflicts;

      ถ้า (expected_sr != -1 || expected_rr != -1)
        {
          จำนวนเต็ม sr = count_rule_sr_conflicts (r);
          ถ้า (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          จำนวนเต็ม rr = count_rule_rr_conflicts (r);
          ถ้า (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

void
conflicts_print (void)
{
  report_rule_expectation_mismatches ();

  ถ้า (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (NULL, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    จำนวนเต็ม total = count_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    จำนวนเต็ม expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    ถ้า (expected != -1)
      {
        ถ้า (expected != total)
          {
            complain (NULL, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            ถ้า (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    มิฉะนั้น ถ้า (total)
      {
        complain (NULL, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    จำนวนเต็ม total = count_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    จำนวนเต็ม expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    ถ้า (expected != -1)
      {
        ถ้า (expected != total)
          {
            complain (NULL, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            ถ้า (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    มิฉะนั้น ถ้า (total)
      {
        complain (NULL, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  ถ้า (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  มิฉะนั้น ถ้า (unexpected_conflicts_warning != Wnone)
    subcomplain (NULL, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

void
conflicts_free (void)
{
  free (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, NULL);
  obstack_free (&solved_conflicts_xml_obstack, NULL);
}
