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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitset.h>

#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "counterexample.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "print-xml.h"
#ضمن "reader.h"
#ضمن "state.h"
#ضمن "symtab.h"

/* -1 stands for not specified. */
صحيح expected_sr_conflicts = -1;
صحيح expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
ساكن bool *conflicts;

ساكن بنية obstack solved_conflicts_obstack;
ساكن بنية obstack solved_conflicts_xml_obstack;

ساكن bitset سرڪايو_سيٽ;
ساكن bitset lookahead_سيٽ;

bool
has_conflicts (ثابت state *s)
{
  أرجع conflicts[s->عدد];
}



تعداد conflict_resolution
  {
    سرڪايو_resolution,
    reduce_resolution,
    کاٻو_resolution,
    ساڄو_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

ساكن مضمن فراغ
log_resolution (قاعدو *r, نشان_عدد token,
                تعداد conflict_resolution resolution)
{
  إذا (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      بدل (resolution)
        {
        حالة سرڪايو_resolution:
        حالة ساڄو_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->عدد,
                          symbols[token]->ٽيگ);
          اكسر;

        حالة reduce_resolution:
        حالة کاٻو_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->عدد,
                          symbols[token]->ٽيگ);
          اكسر;

        حالة nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->عدد,
                          symbols[token]->ٽيگ);
          اكسر;
        }

      /* The reason. */
      بدل (resolution)
        {
        حالة سرڪايو_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->نشان->ٽيگ,
                          symbols[token]->ٽيگ);
          اكسر;

        حالة reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->ٽيگ,
                          r->prec->نشان->ٽيگ);
          اكسر;

        حالة کاٻو_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->ٽيگ);
          اكسر;

        حالة ساڄو_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->ٽيگ);
          اكسر;

        حالة nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->ٽيگ);
          اكسر;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  إذا (xml_flag)
    {
      /* The description of the resolution. */
      بدل (resolution)
        {
        حالة سرڪايو_resolution:
        حالة ساڄو_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->عدد,
                          xml_escape (symbols[token]->ٽيگ));
          اكسر;

        حالة reduce_resolution:
        حالة کاٻو_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->عدد,
                          xml_escape (symbols[token]->ٽيگ));
          اكسر;

        حالة nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->عدد,
                          xml_escape (symbols[token]->ٽيگ));
          اكسر;
        }

      /* The reason. */
      بدل (resolution)
        {
        حالة سرڪايو_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->نشان->ٽيگ),
                          xml_escape_n (1, symbols[token]->ٽيگ));
          اكسر;

        حالة reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->ٽيگ),
                          xml_escape_n (1, r->prec->نشان->ٽيگ));
          اكسر;

        حالة کاٻو_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->ٽيگ));
          اكسر;

        حالة ساڄو_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->ٽيگ));
          اكسر;

        حالة nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->ٽيگ));
      اكسر;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

ساكن فراغ
flush_سرڪايو (state *s, صحيح token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_سيٽ, token);
  لكل (صحيح i = 0; i < trans->انگ; ++i)
    إذا (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_نشان (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

ساكن فراغ
flush_reduce (bitset lookaheads, صحيح token)
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

ساكن فراغ
resolve_sr_conflict (state *s, صحيح ruleno, نشان **errors, صحيح *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  قاعدو *redrule = reds->قاعدا[ruleno];
  صحيح redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  لكل (نشان_عدد i = 0; i < ntokens; ++i)
    إذا (bitset_test (lookaheads, i)
        && bitset_test (lookahead_سيٽ, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        إذا (symbols[i]->content->prec < redprec)
          {
            رجسٽر_precedence (redrule->prec->عدد, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_سرڪايو (s, i);
          }
        وإلا إذا (symbols[i]->content->prec > redprec)
          {
            رجسٽر_precedence (i, redrule->prec->عدد);
            log_resolution (redrule, i, سرڪايو_resolution);
            flush_reduce (lookaheads, i);
          }
        وإلا
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          بدل (symbols[i]->content->assoc)
            {
            حالة undef_assoc:
              abort ();

            حالة precedence_assoc:
              اكسر;

            حالة ساڄو_assoc:
              رجسٽر_assoc (i, redrule->prec->عدد);
              log_resolution (redrule, i, ساڄو_resolution);
              flush_reduce (lookaheads, i);
              اكسر;

            حالة کاٻو_assoc:
              رجسٽر_assoc (i, redrule->prec->عدد);
              log_resolution (redrule, i, کاٻو_resolution);
              flush_سرڪايو (s, i);
              اكسر;

            حالة non_assoc:
              رجسٽر_assoc (i, redrule->prec->عدد);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_سرڪايو (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              اكسر;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

ساكن فراغ
سيٽ_conflicts (state *s, نشان **errors)
{
  إذا (s->consistent)
    أرجع;

  reductions *reds = s->reductions;
  صحيح nerrs = 0;

  bitset_zero (lookahead_سيٽ);

  {
    transitions *trans = s->transitions;
    صحيح i;
    FOR_EACH_سرڪايو (trans, i)
      bitset_سيٽ (lookahead_سيٽ, TRANSITION_نشان (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  لكل (صحيح i = 0; i < reds->انگ; ++i)
    إذا (reds->قاعدا[i]->prec
        && reds->قاعدا[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_سيٽ))
      resolve_sr_conflict (s, i, errors, &nerrs);

  إذا (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_سيٽ (s, nerrs, errors);

  إذا (obstack_object_سائيز (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  إذا (obstack_object_سائيز (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  إذا (reds->lookaheads)
    لكل (صحيح i = 0; i < reds->انگ; ++i)
      {
        إذا (!bitset_disjoint_p (reds->lookaheads[i], lookahead_سيٽ))
          conflicts[s->عدد] = true;
        bitset_or (lookahead_سيٽ, lookahead_سيٽ, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

فراغ
conflicts_solve (فراغ)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  نشان **errors = xnmalloc (ntokens + 1, حجم *errors);

  conflicts = xcalloc (nstates, حجم *conflicts);
  سرڪايو_سيٽ = bitset_create (ntokens, BITSET_FIXED);
  lookahead_سيٽ = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  لكل (state_عدد i = 0; i < nstates; ++i)
    {
      سيٽ_conflicts (حالتون[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      إذا (!حالتون[i]->errs)
        حالتون[i]->errs = errs_new (0, 0);
    }

  حرر (errors);
}


فراغ
conflicts_update_state_numbers (state_عدد old_منزل_new[],
                                state_عدد nstates_old)
{
  لكل (state_عدد i = 0; i < nstates_old; ++i)
    إذا (old_منزل_new[i] != nstates_old)
      conflicts[old_منزل_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

ساكن size_t
ڳڻپ_state_sr_conflicts (ثابت state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  إذا (!trans)
    أرجع 0;

  bitset_zero (lookahead_سيٽ);
  bitset_zero (سرڪايو_سيٽ);

  {
    صحيح i;
    FOR_EACH_سرڪايو (trans, i)
      bitset_سيٽ (سرڪايو_سيٽ, TRANSITION_نشان (trans, i));
  }

  لكل (صحيح i = 0; i < reds->انگ; ++i)
    bitset_or (lookahead_سيٽ, lookahead_سيٽ, reds->lookaheads[i]);

  bitset_and (lookahead_سيٽ, lookahead_سيٽ, سرڪايو_سيٽ);

  أرجع bitset_ڳڻپ (lookahead_سيٽ);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

ساكن size_t
ڳڻپ_sr_conflicts (فراغ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  لكل (state_عدد i = 0; i < nstates; ++i)
    إذا (conflicts[i])
      res += ڳڻپ_state_sr_conflicts (حالتون[i]);
  أرجع res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

ساكن size_t
ڳڻپ_state_rr_conflicts (ثابت state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  لكل (نشان_عدد i = 0; i < ntokens; ++i)
    {
      صحيح ڳڻپ = 0;
      لكل (صحيح j = 0; j < reds->انگ; ++j)
        ڳڻپ += bitset_test (reds->lookaheads[j], i);
      إذا (2 <= ڳڻپ)
        res += ڳڻپ-1;
    }

  أرجع res;
}

ساكن size_t
ڳڻپ_rr_conflicts (فراغ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  لكل (state_عدد i = 0; i < nstates; ++i)
    إذا (conflicts[i])
      res += ڳڻپ_state_rr_conflicts (حالتون[i]);
  أرجع res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

ساكن size_t
ڳڻپ_قاعدو_state_sr_conflicts (قاعدو *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  لكل (صحيح i = 0; i < reds->انگ; ++i)
    إذا (reds->قاعدا[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        صحيح j;
        FOR_EACH_سرڪايو (trans, j)
          res += bitset_test (lookaheads, TRANSITION_نشان (trans, j));
      }

  أرجع res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

ساكن size_t
ڳڻپ_قاعدو_sr_conflicts (قاعدو *r)
{
  size_t res = 0;
  لكل (state_عدد i = 0; i < nstates; ++i)
    إذا (conflicts[i])
      res += ڳڻپ_قاعدو_state_sr_conflicts (r, حالتون[i]);
  أرجع res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

ساكن size_t
ڳڻپ_قاعدو_state_rr_conflicts (قاعدو *r, state *s)
{
  size_t res = 0;
  ثابت reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  لكل (صحيح i = 0; i < reds->انگ; ++i)
    إذا (reds->قاعدا[i] == r)
      لكل (صحيح j = 0; j < reds->انگ; ++j)
        إذا (reds->قاعدا[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_ڳڻپ (lookaheads);
          }
  bitset_free (lookaheads);
  أرجع res;
}

ساكن size_t
ڳڻپ_قاعدو_rr_conflicts (قاعدو *r)
{
  size_t res = 0;
  لكل (state_عدد i = 0; i < nstates; ++i)
    res += ڳڻپ_قاعدو_state_rr_conflicts (r, حالتون[i]);
  أرجع res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

فراغ
conflicts_output (FILE *ٻاهريون)
{
  bool printed_sth = false;
  لكل (state_عدد i = 0; i < nstates; ++i)
    إذا (conflicts[i])
      {
        ثابت state *s = حالتون[i];
        صحيح src = ڳڻپ_state_sr_conflicts (s);
        صحيح rrc = ڳڻپ_state_rr_conflicts (s);
        fprintf (ٻاهريون, _("State %d "), i);
        إذا (src && rrc)
          fprintf (ٻاهريون,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        وإلا إذا (src)
          fprintf (ٻاهريون, _("conflicts: %d shift/reduce\n"), src);
        وإلا إذا (rrc)
          fprintf (ٻاهريون, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  إذا (printed_sth)
    fputs ("\n\n", ٻاهريون);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

صحيح
conflicts_ڪل_ڳڻپ (فراغ)
{
  أرجع ڳڻپ_sr_conflicts () + ڳڻپ_rr_conflicts ();
}

ساكن فراغ
report_counterexamples (فراغ)
{
  لكل (state_عدد sn = 0; sn < nstates; ++sn)
    إذا (conflicts[sn])
      counterexample_report_state (حالتون[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

ساكن فراغ
report_قاعدو_expectation_mismatches (فراغ)
{
  لكل (قاعدو_عدد i = 0; i < nrules; i += 1)
    {
      قاعدو *r = &قاعدا[i];
      صحيح expected_sr = r->expected_sr_conflicts;
      صحيح expected_rr = r->expected_rr_conflicts;

      إذا (expected_sr != -1 || expected_rr != -1)
        {
          صحيح sr = ڳڻپ_قاعدو_sr_conflicts (r);
          إذا (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->ڪوڊ, sr, expected_sr);
          صحيح rr = ڳڻپ_قاعدو_rr_conflicts (r);
          إذا (rr != expected_rr && (rr != 0 || expected_rr != -1))
            complain (&r->location, complaint,
                      _("reduce/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->ڪوڊ, rr, expected_rr);
        }
    }
}

/*---------------------------------.
| Reporting numbers of conflicts.  |
`---------------------------------*/

فراغ
conflicts_print (فراغ)
{
  report_قاعدو_expectation_mismatches ();

  إذا (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (عدم, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    صحيح ڪل = ڳڻپ_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    صحيح expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    إذا (expected != -1)
      {
        إذا (expected != ڪل)
          {
            complain (عدم, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      ڪل, expected);
            إذا (ڪل)
              unexpected_conflicts_warning = complaint;
          }
      }
    وإلا إذا (ڪل)
      {
        complain (عدم, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            ڪل),
                  ڪل);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    صحيح ڪل = ڳڻپ_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    صحيح expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    إذا (expected != -1)
      {
        إذا (expected != ڪل)
          {
            complain (عدم, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      ڪل, expected);
            إذا (ڪل)
              unexpected_conflicts_warning = complaint;
          }
      }
    وإلا إذا (ڪل)
      {
        complain (عدم, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            ڪل),
                  ڪل);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  إذا (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  وإلا إذا (unexpected_conflicts_warning != Wnone)
    subcomplain (عدم, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

فراغ
conflicts_free (فراغ)
{
  حرر (conflicts);
  bitset_free (سرڪايو_سيٽ);
  bitset_free (lookahead_سيٽ);
  obstack_free (&solved_conflicts_obstack, عدم);
  obstack_free (&solved_conflicts_xml_obstack, عدم);
}
