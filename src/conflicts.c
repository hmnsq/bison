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
पूर्णाङ्क expected_sr_conflicts = -1;
पूर्णाङ्क expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
स्थिर bool *conflicts;

स्थिर संरचना obstack solved_conflicts_obstack;
स्थिर संरचना obstack solved_conflicts_xml_obstack;

स्थिर bitset सरणम्_समुच्चयः;
स्थिर bitset lookahead_समुच्चयः;

bool
has_conflicts (ध्रुव state *s)
{
  प्रत्यागम conflicts[s->सङ्ख्या];
}



गणना conflict_resolution
  {
    सरणम्_resolution,
    reduce_resolution,
    वामः_resolution,
    दक्षिणः_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

स्थिर अन्तःस्थ शून्य
log_resolution (नियमः *r, चिह्नम्_सङ्ख्या token,
                गणना conflict_resolution resolution)
{
  यदि (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      विकल्प (resolution)
        {
        पक्ष सरणम्_resolution:
        पक्ष दक्षिणः_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->सङ्ख्या,
                          symbols[token]->चिह्नकम्);
          भङ्ग;

        पक्ष reduce_resolution:
        पक्ष वामः_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->सङ्ख्या,
                          symbols[token]->चिह्नकम्);
          भङ्ग;

        पक्ष nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->सङ्ख्या,
                          symbols[token]->चिह्नकम्);
          भङ्ग;
        }

      /* The reason. */
      विकल्प (resolution)
        {
        पक्ष सरणम्_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->चिह्नम्->चिह्नकम्,
                          symbols[token]->चिह्नकम्);
          भङ्ग;

        पक्ष reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->चिह्नकम्,
                          r->prec->चिह्नम्->चिह्नकम्);
          भङ्ग;

        पक्ष वामः_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->चिह्नकम्);
          भङ्ग;

        पक्ष दक्षिणः_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->चिह्नकम्);
          भङ्ग;

        पक्ष nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->चिह्नकम्);
          भङ्ग;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  यदि (xml_flag)
    {
      /* The description of the resolution. */
      विकल्प (resolution)
        {
        पक्ष सरणम्_resolution:
        पक्ष दक्षिणः_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->सङ्ख्या,
                          xml_escape (symbols[token]->चिह्नकम्));
          भङ्ग;

        पक्ष reduce_resolution:
        पक्ष वामः_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->सङ्ख्या,
                          xml_escape (symbols[token]->चिह्नकम्));
          भङ्ग;

        पक्ष nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->सङ्ख्या,
                          xml_escape (symbols[token]->चिह्नकम्));
          भङ्ग;
        }

      /* The reason. */
      विकल्प (resolution)
        {
        पक्ष सरणम्_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->चिह्नम्->चिह्नकम्),
                          xml_escape_n (1, symbols[token]->चिह्नकम्));
          भङ्ग;

        पक्ष reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->चिह्नकम्),
                          xml_escape_n (1, r->prec->चिह्नम्->चिह्नकम्));
          भङ्ग;

        पक्ष वामः_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->चिह्नकम्));
          भङ्ग;

        पक्ष दक्षिणः_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->चिह्नकम्));
          भङ्ग;

        पक्ष nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->चिह्नकम्));
      भङ्ग;
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
flush_सरणम् (state *s, पूर्णाङ्क token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_समुच्चयः, token);
  प्रति (पूर्णाङ्क i = 0; i < trans->अङ्कः; ++i)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_चिह्नम् (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

स्थिर शून्य
flush_reduce (bitset lookaheads, पूर्णाङ्क token)
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
resolve_sr_conflict (state *s, पूर्णाङ्क ruleno, चिह्नम् **errors, पूर्णाङ्क *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  नियमः *redrule = reds->नियमाः[ruleno];
  पूर्णाङ्क redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; ++i)
    यदि (bitset_test (lookaheads, i)
        && bitset_test (lookahead_समुच्चयः, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        यदि (symbols[i]->content->prec < redprec)
          {
            पञ्जिकाकोशः_precedence (redrule->prec->सङ्ख्या, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_सरणम् (s, i);
          }
        अन्यथा यदि (symbols[i]->content->prec > redprec)
          {
            पञ्जिकाकोशः_precedence (i, redrule->prec->सङ्ख्या);
            log_resolution (redrule, i, सरणम्_resolution);
            flush_reduce (lookaheads, i);
          }
        अन्यथा
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          विकल्प (symbols[i]->content->assoc)
            {
            पक्ष undef_assoc:
              abort ();

            पक्ष precedence_assoc:
              भङ्ग;

            पक्ष दक्षिणः_assoc:
              पञ्जिकाकोशः_assoc (i, redrule->prec->सङ्ख्या);
              log_resolution (redrule, i, दक्षिणः_resolution);
              flush_reduce (lookaheads, i);
              भङ्ग;

            पक्ष वामः_assoc:
              पञ्जिकाकोशः_assoc (i, redrule->prec->सङ्ख्या);
              log_resolution (redrule, i, वामः_resolution);
              flush_सरणम् (s, i);
              भङ्ग;

            पक्ष non_assoc:
              पञ्जिकाकोशः_assoc (i, redrule->prec->सङ्ख्या);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_सरणम् (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              भङ्ग;
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
समुच्चयः_conflicts (state *s, चिह्नम् **errors)
{
  यदि (s->consistent)
    प्रत्यागम;

  reductions *reds = s->reductions;
  पूर्णाङ्क nerrs = 0;

  bitset_zero (lookahead_समुच्चयः);

  {
    transitions *trans = s->transitions;
    पूर्णाङ्क i;
    FOR_EACH_सरणम् (trans, i)
      bitset_समुच्चयः (lookahead_समुच्चयः, TRANSITION_चिह्नम् (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    यदि (reds->नियमाः[i]->prec
        && reds->नियमाः[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_समुच्चयः))
      resolve_sr_conflict (s, i, errors, &nerrs);

  यदि (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_समुच्चयः (s, nerrs, errors);

  यदि (obstack_object_आकार (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  यदि (obstack_object_आकार (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  यदि (reds->lookaheads)
    प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
      {
        यदि (!bitset_disjoint_p (reds->lookaheads[i], lookahead_समुच्चयः))
          conflicts[s->सङ्ख्या] = true;
        bitset_or (lookahead_समुच्चयः, lookahead_समुच्चयः, reds->lookaheads[i]);
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
  चिह्नम् **errors = xnmalloc (ntokens + 1, परिमाण *errors);

  conflicts = xcalloc (nstates, परिमाण *conflicts);
  सरणम्_समुच्चयः = bitset_create (ntokens, BITSET_FIXED);
  lookahead_समुच्चयः = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      समुच्चयः_conflicts (अवस्थाः[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      यदि (!अवस्थाः[i]->errs)
        अवस्थाः[i]->errs = errs_new (0, 0);
    }

  मुक्त (errors);
}


शून्य
conflicts_update_state_numbers (state_सङ्ख्या old_गन्तव्यम्_new[],
                                state_सङ्ख्या nstates_old)
{
  प्रति (state_सङ्ख्या i = 0; i < nstates_old; ++i)
    यदि (old_गन्तव्यम्_new[i] != nstates_old)
      conflicts[old_गन्तव्यम्_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

स्थिर size_t
गणनम्_state_sr_conflicts (ध्रुव state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  यदि (!trans)
    प्रत्यागम 0;

  bitset_zero (lookahead_समुच्चयः);
  bitset_zero (सरणम्_समुच्चयः);

  {
    पूर्णाङ्क i;
    FOR_EACH_सरणम् (trans, i)
      bitset_समुच्चयः (सरणम्_समुच्चयः, TRANSITION_चिह्नम् (trans, i));
  }

  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    bitset_or (lookahead_समुच्चयः, lookahead_समुच्चयः, reds->lookaheads[i]);

  bitset_and (lookahead_समुच्चयः, lookahead_समुच्चयः, सरणम्_समुच्चयः);

  प्रत्यागम bitset_गणनम् (lookahead_समुच्चयः);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

स्थिर size_t
गणनम्_sr_conflicts (शून्य)
{
  size_t res = 0;
  /* Conflicts by state.  */
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      res += गणनम्_state_sr_conflicts (अवस्थाः[i]);
  प्रत्यागम res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

स्थिर size_t
गणनम्_state_rr_conflicts (ध्रुव state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; ++i)
    {
      पूर्णाङ्क गणनम् = 0;
      प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
        गणनम् += bitset_test (reds->lookaheads[j], i);
      यदि (2 <= गणनम्)
        res += गणनम्-1;
    }

  प्रत्यागम res;
}

स्थिर size_t
गणनम्_rr_conflicts (शून्य)
{
  size_t res = 0;
  /* Conflicts by state.  */
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      res += गणनम्_state_rr_conflicts (अवस्थाः[i]);
  प्रत्यागम res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

स्थिर size_t
गणनम्_नियमः_state_sr_conflicts (नियमः *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    यदि (reds->नियमाः[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        पूर्णाङ्क j;
        FOR_EACH_सरणम् (trans, j)
          res += bitset_test (lookaheads, TRANSITION_चिह्नम् (trans, j));
      }

  प्रत्यागम res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

स्थिर size_t
गणनम्_नियमः_sr_conflicts (नियमः *r)
{
  size_t res = 0;
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      res += गणनम्_नियमः_state_sr_conflicts (r, अवस्थाः[i]);
  प्रत्यागम res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

स्थिर size_t
गणनम्_नियमः_state_rr_conflicts (नियमः *r, state *s)
{
  size_t res = 0;
  ध्रुव reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
    यदि (reds->नियमाः[i] == r)
      प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
        यदि (reds->नियमाः[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_गणनम् (lookaheads);
          }
  bitset_free (lookaheads);
  प्रत्यागम res;
}

स्थिर size_t
गणनम्_नियमः_rr_conflicts (नियमः *r)
{
  size_t res = 0;
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    res += गणनम्_नियमः_state_rr_conflicts (r, अवस्थाः[i]);
  प्रत्यागम res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

शून्य
conflicts_output (FILE *बहिर्गमनम्)
{
  bool printed_sth = false;
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      {
        ध्रुव state *s = अवस्थाः[i];
        पूर्णाङ्क src = गणनम्_state_sr_conflicts (s);
        पूर्णाङ्क rrc = गणनम्_state_rr_conflicts (s);
        fprintf (बहिर्गमनम्, _("State %d "), i);
        यदि (src && rrc)
          fprintf (बहिर्गमनम्,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        अन्यथा यदि (src)
          fprintf (बहिर्गमनम्, _("conflicts: %d shift/reduce\n"), src);
        अन्यथा यदि (rrc)
          fprintf (बहिर्गमनम्, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  यदि (printed_sth)
    fputs ("\n\n", बहिर्गमनम्);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

पूर्णाङ्क
conflicts_योग_गणनम् (शून्य)
{
  प्रत्यागम गणनम्_sr_conflicts () + गणनम्_rr_conflicts ();
}

स्थिर शून्य
report_counterexamples (शून्य)
{
  प्रति (state_सङ्ख्या sn = 0; sn < nstates; ++sn)
    यदि (conflicts[sn])
      counterexample_report_state (अवस्थाः[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

स्थिर शून्य
report_नियमः_expectation_mismatches (शून्य)
{
  प्रति (नियमः_सङ्ख्या i = 0; i < nrules; i += 1)
    {
      नियमः *r = &नियमाः[i];
      पूर्णाङ्क expected_sr = r->expected_sr_conflicts;
      पूर्णाङ्क expected_rr = r->expected_rr_conflicts;

      यदि (expected_sr != -1 || expected_rr != -1)
        {
          पूर्णाङ्क sr = गणनम्_नियमः_sr_conflicts (r);
          यदि (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->संहिता, sr, expected_sr);
          पूर्णाङ्क rr = गणनम्_नियमः_rr_conflicts (r);
          यदि (rr != expected_rr && (rr != 0 || expected_rr != -1))
            complain (&r->location, complaint,
                      _("reduce/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->संहिता, rr, expected_rr);
        }
    }
}

/*---------------------------------.
| Reporting numbers of conflicts.  |
`---------------------------------*/

शून्य
conflicts_print (शून्य)
{
  report_नियमः_expectation_mismatches ();

  यदि (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (रिक्त, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    पूर्णाङ्क योग = गणनम्_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    पूर्णाङ्क expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    यदि (expected != -1)
      {
        यदि (expected != योग)
          {
            complain (रिक्त, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      योग, expected);
            यदि (योग)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा यदि (योग)
      {
        complain (रिक्त, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            योग),
                  योग);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    पूर्णाङ्क योग = गणनम्_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    पूर्णाङ्क expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    यदि (expected != -1)
      {
        यदि (expected != योग)
          {
            complain (रिक्त, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      योग, expected);
            यदि (योग)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा यदि (योग)
      {
        complain (रिक्त, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            योग),
                  योग);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  यदि (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  अन्यथा यदि (unexpected_conflicts_warning != Wnone)
    subcomplain (रिक्त, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

शून्य
conflicts_free (शून्य)
{
  मुक्त (conflicts);
  bitset_free (सरणम्_समुच्चयः);
  bitset_free (lookahead_समुच्चयः);
  obstack_free (&solved_conflicts_obstack, रिक्त);
  obstack_free (&solved_conflicts_xml_obstack, रिक्त);
}
