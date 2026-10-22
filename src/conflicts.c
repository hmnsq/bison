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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "counterexample.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "print-xml.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* -1 stands for not specified. */
ಪೂರ್ಣಾಂಕ expected_sr_conflicts = -1;
ಪೂರ್ಣಾಂಕ expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
ಸ್ಥಿರ bool *conflicts;

ಸ್ಥಿರ ರಚನೆ obstack solved_conflicts_obstack;
ಸ್ಥಿರ ರಚನೆ obstack solved_conflicts_xml_obstack;

ಸ್ಥಿರ bitset ಸರಿಸು_ಗಣ;
ಸ್ಥಿರ bitset lookahead_ಗಣ;

bool
has_conflicts (ಸ್ಥಿರಾಂಕ state *s)
{
  ಹಿಂತಿರುಗಿ conflicts[s->ಸಂಖ್ಯೆ];
}



ಎಣಿಕೆ conflict_resolution
  {
    ಸರಿಸು_resolution,
    reduce_resolution,
    ಎಡ_resolution,
    ಬಲ_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
log_resolution (ನಿಯಮ *r, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token,
                ಎಣಿಕೆ conflict_resolution resolution)
{
  ಆದರೆ (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      ಆಯ್ಕೆ (resolution)
        {
        ಸ್ಥಿತಿ ಸರಿಸು_resolution:
        ಸ್ಥಿತಿ ಬಲ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->ಸಂಖ್ಯೆ,
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ reduce_resolution:
        ಸ್ಥಿತಿ ಎಡ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->ಸಂಖ್ಯೆ,
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->ಸಂಖ್ಯೆ,
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;
        }

      /* The reason. */
      ಆಯ್ಕೆ (resolution)
        {
        ಸ್ಥಿತಿ ಸರಿಸು_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್,
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->ಟ್ಯಾಗ್,
                          r->prec->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಎಡ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಬಲ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->ಟ್ಯಾಗ್);
          ನಿಲ್ಲಿಸು;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  ಆದರೆ (xml_flag)
    {
      /* The description of the resolution. */
      ಆಯ್ಕೆ (resolution)
        {
        ಸ್ಥಿತಿ ಸರಿಸು_resolution:
        ಸ್ಥಿತಿ ಬಲ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->ಸಂಖ್ಯೆ,
                          xml_escape (symbols[token]->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ reduce_resolution:
        ಸ್ಥಿತಿ ಎಡ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->ಸಂಖ್ಯೆ,
                          xml_escape (symbols[token]->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->ಸಂಖ್ಯೆ,
                          xml_escape (symbols[token]->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;
        }

      /* The reason. */
      ಆಯ್ಕೆ (resolution)
        {
        ಸ್ಥಿತಿ ಸರಿಸು_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್),
                          xml_escape_n (1, symbols[token]->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->ಟ್ಯಾಗ್),
                          xml_escape_n (1, r->prec->ಚಿಹ್ನೆ->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಎಡ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಬಲ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->ಟ್ಯಾಗ್));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->ಟ್ಯಾಗ್));
      ನಿಲ್ಲಿಸು;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
flush_ಸರಿಸು (state *s, ಪೂರ್ಣಾಂಕ token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_ಗಣ, token);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < trans->ಅಂಕೆ; ++i)
    ಆದರೆ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_ಚಿಹ್ನೆ (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
flush_reduce (bitset lookaheads, ಪೂರ್ಣಾಂಕ token)
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

ಸ್ಥಿರ ಶೂನ್ಯ
resolve_sr_conflict (state *s, ಪೂರ್ಣಾಂಕ ruleno, ಚಿಹ್ನೆ **errors, ಪೂರ್ಣಾಂಕ *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  ನಿಯಮ *redrule = reds->ನಿಯಮಗಳು[ruleno];
  ಪೂರ್ಣಾಂಕ redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; ++i)
    ಆದರೆ (bitset_test (lookaheads, i)
        && bitset_test (lookahead_ಗಣ, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        ಆದರೆ (symbols[i]->content->prec < redprec)
          {
            ರಿಜಿಸ್ಟರ್_precedence (redrule->prec->ಸಂಖ್ಯೆ, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_ಸರಿಸು (s, i);
          }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (symbols[i]->content->prec > redprec)
          {
            ರಿಜಿಸ್ಟರ್_precedence (i, redrule->prec->ಸಂಖ್ಯೆ);
            log_resolution (redrule, i, ಸರಿಸು_resolution);
            flush_reduce (lookaheads, i);
          }
        ಇಲ್ಲದಿದ್ದರೆ
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          ಆಯ್ಕೆ (symbols[i]->content->assoc)
            {
            ಸ್ಥಿತಿ undef_assoc:
              abort ();

            ಸ್ಥಿತಿ precedence_assoc:
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ ಬಲ_assoc:
              ರಿಜಿಸ್ಟರ್_assoc (i, redrule->prec->ಸಂಖ್ಯೆ);
              log_resolution (redrule, i, ಬಲ_resolution);
              flush_reduce (lookaheads, i);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ ಎಡ_assoc:
              ರಿಜಿಸ್ಟರ್_assoc (i, redrule->prec->ಸಂಖ್ಯೆ);
              log_resolution (redrule, i, ಎಡ_resolution);
              flush_ಸರಿಸು (s, i);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ non_assoc:
              ರಿಜಿಸ್ಟರ್_assoc (i, redrule->prec->ಸಂಖ್ಯೆ);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_ಸರಿಸು (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              ನಿಲ್ಲಿಸು;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_conflicts (state *s, ಚಿಹ್ನೆ **errors)
{
  ಆದರೆ (s->consistent)
    ಹಿಂತಿರುಗಿ;

  reductions *reds = s->reductions;
  ಪೂರ್ಣಾಂಕ nerrs = 0;

  bitset_zero (lookahead_ಗಣ);

  {
    transitions *trans = s->transitions;
    ಪೂರ್ಣಾಂಕ i;
    FOR_EACH_ಸರಿಸು (trans, i)
      bitset_ಗಣ (lookahead_ಗಣ, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಆದರೆ (reds->ನಿಯಮಗಳು[i]->prec
        && reds->ನಿಯಮಗಳು[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_ಗಣ))
      resolve_sr_conflict (s, i, errors, &nerrs);

  ಆದರೆ (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_ಗಣ (s, nerrs, errors);

  ಆದರೆ (obstack_object_ಗಾತ್ರ (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  ಆದರೆ (obstack_object_ಗಾತ್ರ (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  ಆದರೆ (reds->lookaheads)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
      {
        ಆದರೆ (!bitset_disjoint_p (reds->lookaheads[i], lookahead_ಗಣ))
          conflicts[s->ಸಂಖ್ಯೆ] = true;
        bitset_or (lookahead_ಗಣ, lookahead_ಗಣ, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

ಶೂನ್ಯ
conflicts_solve (ಶೂನ್ಯ)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  ಚಿಹ್ನೆ **errors = xnmalloc (ntokens + 1, ಗಾತ್ರ *errors);

  conflicts = xcalloc (nstates, ಗಾತ್ರ *conflicts);
  ಸರಿಸು_ಗಣ = bitset_create (ntokens, BITSET_FIXED);
  lookahead_ಗಣ = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಗಣ_conflicts (ಸ್ಥಿತಿಗಳು[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      ಆದರೆ (!ಸ್ಥಿತಿಗಳು[i]->errs)
        ಸ್ಥಿತಿಗಳು[i]->errs = errs_new (0, 0);
    }

  ಬಿಡುಗಡೆ (errors);
}


ಶೂನ್ಯ
conflicts_update_state_numbers (state_ಸಂಖ್ಯೆ old_ಗಮ್ಯ_new[],
                                state_ಸಂಖ್ಯೆ nstates_old)
{
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates_old; ++i)
    ಆದರೆ (old_ಗಮ್ಯ_new[i] != nstates_old)
      conflicts[old_ಗಮ್ಯ_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_state_sr_conflicts (ಸ್ಥಿರಾಂಕ state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ಆದರೆ (!trans)
    ಹಿಂತಿರುಗಿ 0;

  bitset_zero (lookahead_ಗಣ);
  bitset_zero (ಸರಿಸು_ಗಣ);

  {
    ಪೂರ್ಣಾಂಕ i;
    FOR_EACH_ಸರಿಸು (trans, i)
      bitset_ಗಣ (ಸರಿಸು_ಗಣ, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    bitset_or (lookahead_ಗಣ, lookahead_ಗಣ, reds->lookaheads[i]);

  bitset_and (lookahead_ಗಣ, lookahead_ಗಣ, ಸರಿಸು_ಗಣ);

  ಹಿಂತಿರುಗಿ bitset_ಎಣಿಕೆ (lookahead_ಗಣ);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_sr_conflicts (ಶೂನ್ಯ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಆದರೆ (conflicts[i])
      res += ಎಣಿಕೆ_state_sr_conflicts (ಸ್ಥಿತಿಗಳು[i]);
  ಹಿಂತಿರುಗಿ res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_state_rr_conflicts (ಸ್ಥಿರಾಂಕ state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; ++i)
    {
      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < reds->ಅಂಕೆ; ++j)
        ಎಣಿಕೆ_2 += bitset_test (reds->lookaheads[j], i);
      ಆದರೆ (2 <= ಎಣಿಕೆ_2)
        res += ಎಣಿಕೆ_2-1;
    }

  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_rr_conflicts (ಶೂನ್ಯ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಆದರೆ (conflicts[i])
      res += ಎಣಿಕೆ_state_rr_conflicts (ಸ್ಥಿತಿಗಳು[i]);
  ಹಿಂತಿರುಗಿ res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_ನಿಯಮ_state_sr_conflicts (ನಿಯಮ *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಆದರೆ (reds->ನಿಯಮಗಳು[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        ಪೂರ್ಣಾಂಕ j;
        FOR_EACH_ಸರಿಸು (trans, j)
          res += bitset_test (lookaheads, TRANSITION_ಚಿಹ್ನೆ (trans, j));
      }

  ಹಿಂತಿರುಗಿ res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_ನಿಯಮ_sr_conflicts (ನಿಯಮ *r)
{
  size_t res = 0;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಆದರೆ (conflicts[i])
      res += ಎಣಿಕೆ_ನಿಯಮ_state_sr_conflicts (r, ಸ್ಥಿತಿಗಳು[i]);
  ಹಿಂತಿರುಗಿ res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_ನಿಯಮ_state_rr_conflicts (ನಿಯಮ *r, state *s)
{
  size_t res = 0;
  ಸ್ಥಿರಾಂಕ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಆದರೆ (reds->ನಿಯಮಗಳು[i] == r)
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < reds->ಅಂಕೆ; ++j)
        ಆದರೆ (reds->ನಿಯಮಗಳು[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_ಎಣಿಕೆ (lookaheads);
          }
  bitset_free (lookaheads);
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_ನಿಯಮ_rr_conflicts (ನಿಯಮ *r)
{
  size_t res = 0;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    res += ಎಣಿಕೆ_ನಿಯಮ_state_rr_conflicts (r, ಸ್ಥಿತಿಗಳು[i]);
  ಹಿಂತಿರುಗಿ res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

ಶೂನ್ಯ
conflicts_output (FILE *ಹೊರಹೋಗು)
{
  bool printed_sth = false;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಆದರೆ (conflicts[i])
      {
        ಸ್ಥಿರಾಂಕ state *s = ಸ್ಥಿತಿಗಳು[i];
        ಪೂರ್ಣಾಂಕ src = ಎಣಿಕೆ_state_sr_conflicts (s);
        ಪೂರ್ಣಾಂಕ rrc = ಎಣಿಕೆ_state_rr_conflicts (s);
        fprintf (ಹೊರಹೋಗು, _("State %d "), i);
        ಆದರೆ (src && rrc)
          fprintf (ಹೊರಹೋಗು,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src)
          fprintf (ಹೊರಹೋಗು, _("conflicts: %d shift/reduce\n"), src);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rrc)
          fprintf (ಹೊರಹೋಗು, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  ಆದರೆ (printed_sth)
    fputs ("\n\n", ಹೊರಹೋಗು);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

ಪೂರ್ಣಾಂಕ
conflicts_ಒಟ್ಟು_ಎಣಿಕೆ (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_sr_conflicts () + ಎಣಿಕೆ_rr_conflicts ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
report_counterexamples (ಶೂನ್ಯ)
{
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ sn = 0; sn < nstates; ++sn)
    ಆದರೆ (conflicts[sn])
      counterexample_report_state (ಸ್ಥಿತಿಗಳು[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
report_ನಿಯಮ_expectation_mismatches (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ i = 0; i < nrules; i += 1)
    {
      ನಿಯಮ *r = &ನಿಯಮಗಳು[i];
      ಪೂರ್ಣಾಂಕ expected_sr = r->expected_sr_conflicts;
      ಪೂರ್ಣಾಂಕ expected_rr = r->expected_rr_conflicts;

      ಆದರೆ (expected_sr != -1 || expected_rr != -1)
        {
          ಪೂರ್ಣಾಂಕ sr = ಎಣಿಕೆ_ನಿಯಮ_sr_conflicts (r);
          ಆದರೆ (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->ಸಂಕೇತ_2, sr, expected_sr);
          ಪೂರ್ಣಾಂಕ rr = ಎಣಿಕೆ_ನಿಯಮ_rr_conflicts (r);
          ಆದರೆ (rr != expected_rr && (rr != 0 || expected_rr != -1))
            complain (&r->location, complaint,
                      _("reduce/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->ಸಂಕೇತ_2, rr, expected_rr);
        }
    }
}

/*---------------------------------.
| Reporting numbers of conflicts.  |
`---------------------------------*/

ಶೂನ್ಯ
conflicts_print (ಶೂನ್ಯ)
{
  report_ನಿಯಮ_expectation_mismatches ();

  ಆದರೆ (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (ಖಾಲಿ, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = ಎಣಿಕೆ_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    ಪೂರ್ಣಾಂಕ expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    ಆದರೆ (expected != -1)
      {
        ಆದರೆ (expected != ಒಟ್ಟು)
          {
            complain (ಖಾಲಿ, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      ಒಟ್ಟು, expected);
            ಆದರೆ (ಒಟ್ಟು)
              unexpected_conflicts_warning = complaint;
          }
      }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಟ್ಟು)
      {
        complain (ಖಾಲಿ, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            ಒಟ್ಟು),
                  ಒಟ್ಟು);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = ಎಣಿಕೆ_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    ಪೂರ್ಣಾಂಕ expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    ಆದರೆ (expected != -1)
      {
        ಆದರೆ (expected != ಒಟ್ಟು)
          {
            complain (ಖಾಲಿ, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      ಒಟ್ಟು, expected);
            ಆದರೆ (ಒಟ್ಟು)
              unexpected_conflicts_warning = complaint;
          }
      }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಟ್ಟು)
      {
        complain (ಖಾಲಿ, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            ಒಟ್ಟು),
                  ಒಟ್ಟು);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  ಆದರೆ (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unexpected_conflicts_warning != Wnone)
    subcomplain (ಖಾಲಿ, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

ಶೂನ್ಯ
conflicts_free (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (conflicts);
  bitset_free (ಸರಿಸು_ಗಣ);
  bitset_free (lookahead_ಗಣ);
  obstack_free (&solved_conflicts_obstack, ಖಾಲಿ);
  obstack_free (&solved_conflicts_xml_obstack, ಖಾಲಿ);
}
