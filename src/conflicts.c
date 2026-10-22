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
ಪೂರ್ಣಾಙ್ಕ expected_sr_conflicts = -1;
ಪೂರ್ಣಾಙ್ಕ expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
ಸ್ಥಿರ bool *conflicts;

ಸ್ಥಿರ ಸಂರಚನಾ obstack solved_conflicts_obstack;
ಸ್ಥಿರ ಸಂರಚನಾ obstack solved_conflicts_xml_obstack;

ಸ್ಥಿರ bitset shift_set;
ಸ್ಥಿರ bitset lookahead_set;

bool
has_conflicts (ಧ್ರುವ state *s)
{
  ಪ್ರತ್ಯಾಗಮ conflicts[s->ಸಂಖ್ಯೆ];
}



ಗಣನಾ conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    ಎಡ_resolution,
    ಬಲ_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
log_resolution (rule *r, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token,
                ಗಣನಾ conflict_resolution resolution)
{
  ಯದಿ (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      ವಿಕಲ್ಪ (resolution)
        {
        ಪಕ್ಷ shift_resolution:
        ಪಕ್ಷ ಬಲ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->ಸಂಖ್ಯೆ,
                          symbols[token]->tag);
          ಭಙ್ಗ;

        ಪಕ್ಷ reduce_resolution:
        ಪಕ್ಷ ಎಡ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->ಸಂಖ್ಯೆ,
                          symbols[token]->tag);
          ಭಙ್ಗ;

        ಪಕ್ಷ nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->ಸಂಖ್ಯೆ,
                          symbols[token]->tag);
          ಭಙ್ಗ;
        }

      /* The reason. */
      ವಿಕಲ್ಪ (resolution)
        {
        ಪಕ್ಷ shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->ಚಿಹ್ನೆ->tag,
                          symbols[token]->tag);
          ಭಙ್ಗ;

        ಪಕ್ಷ reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->ಚಿಹ್ನೆ->tag);
          ಭಙ್ಗ;

        ಪಕ್ಷ ಎಡ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          ಭಙ್ಗ;

        ಪಕ್ಷ ಬಲ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          ಭಙ್ಗ;

        ಪಕ್ಷ nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          ಭಙ್ಗ;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  ಯದಿ (xml_flag)
    {
      /* The description of the resolution. */
      ವಿಕಲ್ಪ (resolution)
        {
        ಪಕ್ಷ shift_resolution:
        ಪಕ್ಷ ಬಲ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->ಸಂಖ್ಯೆ,
                          xml_escape (symbols[token]->tag));
          ಭಙ್ಗ;

        ಪಕ್ಷ reduce_resolution:
        ಪಕ್ಷ ಎಡ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->ಸಂಖ್ಯೆ,
                          xml_escape (symbols[token]->tag));
          ಭಙ್ಗ;

        ಪಕ್ಷ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->ಸಂಖ್ಯೆ,
                          xml_escape (symbols[token]->tag));
          ಭಙ್ಗ;
        }

      /* The reason. */
      ವಿಕಲ್ಪ (resolution)
        {
        ಪಕ್ಷ shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->ಚಿಹ್ನೆ->tag),
                          xml_escape_n (1, symbols[token]->tag));
          ಭಙ್ಗ;

        ಪಕ್ಷ reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->ಚಿಹ್ನೆ->tag));
          ಭಙ್ಗ;

        ಪಕ್ಷ ಎಡ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          ಭಙ್ಗ;

        ಪಕ್ಷ ಬಲ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          ಭಙ್ಗ;

        ಪಕ್ಷ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      ಭಙ್ಗ;
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
flush_shift (state *s, ಪೂರ್ಣಾಙ್ಕ token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < trans->ಅಂಕೆ; ++i)
    ಯದಿ (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_ಚಿಹ್ನೆ (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
flush_reduce (bitset lookaheads, ಪೂರ್ಣಾಙ್ಕ token)
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
resolve_sr_conflict (state *s, ಪೂರ್ಣಾಙ್ಕ ruleno, ಚಿಹ್ನೆ **errors, ಪೂರ್ಣಾಙ್ಕ *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  ಪೂರ್ಣಾಙ್ಕ redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; ++i)
    ಯದಿ (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        ಯದಿ (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->ಸಂಖ್ಯೆ, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        ಅನ್ಯಥಾ ಯದಿ (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->ಸಂಖ್ಯೆ);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        ಅನ್ಯಥಾ
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          ವಿಕಲ್ಪ (symbols[i]->content->assoc)
            {
            ಪಕ್ಷ undef_assoc:
              abort ();

            ಪಕ್ಷ precedence_assoc:
              ಭಙ್ಗ;

            ಪಕ್ಷ ಬಲ_assoc:
              register_assoc (i, redrule->prec->ಸಂಖ್ಯೆ);
              log_resolution (redrule, i, ಬಲ_resolution);
              flush_reduce (lookaheads, i);
              ಭಙ್ಗ;

            ಪಕ್ಷ ಎಡ_assoc:
              register_assoc (i, redrule->prec->ಸಂಖ್ಯೆ);
              log_resolution (redrule, i, ಎಡ_resolution);
              flush_shift (s, i);
              ಭಙ್ಗ;

            ಪಕ್ಷ non_assoc:
              register_assoc (i, redrule->prec->ಸಂಖ್ಯೆ);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              ಭಙ್ಗ;
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
set_conflicts (state *s, ಚಿಹ್ನೆ **errors)
{
  ಯದಿ (s->consistent)
    ಪ್ರತ್ಯಾಗಮ;

  reductions *reds = s->reductions;
  ಪೂರ್ಣಾಙ್ಕ nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    ಪೂರ್ಣಾಙ್ಕ i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಯದಿ (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  ಯದಿ (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  ಯದಿ (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  ಯದಿ (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  ಯದಿ (reds->lookaheads)
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
      {
        ಯದಿ (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->ಸಂಖ್ಯೆ] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
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
  ಚಿಹ್ನೆ **errors = xnmalloc (ntokens + 1, ಪರಿಮಾಣ *errors);

  conflicts = xcalloc (nstates, ಪರಿಮಾಣ *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      ಯದಿ (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  ಮುಕ್ತ (errors);
}


ಶೂನ್ಯ
conflicts_update_state_numbers (state_ಸಂಖ್ಯೆ old_to_new[],
                                state_ಸಂಖ್ಯೆ nstates_old)
{
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates_old; ++i)
    ಯದಿ (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_state_sr_conflicts (ಧ್ರುವ state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ಯದಿ (!trans)
    ಪ್ರತ್ಯಾಗಮ 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    ಪೂರ್ಣಾಙ್ಕ i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  ಪ್ರತ್ಯಾಗಮ bitset_ಎಣಿಕೆ (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_sr_conflicts (ಶೂನ್ಯ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಯದಿ (conflicts[i])
      res += ಎಣಿಕೆ_state_sr_conflicts (states[i]);
  ಪ್ರತ್ಯಾಗಮ res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_state_rr_conflicts (ಧ್ರುವ state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; ++i)
    {
      ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < reds->ಅಂಕೆ; ++j)
        ಎಣಿಕೆ += bitset_test (reds->lookaheads[j], i);
      ಯದಿ (2 <= ಎಣಿಕೆ)
        res += ಎಣಿಕೆ-1;
    }

  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_rr_conflicts (ಶೂನ್ಯ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಯದಿ (conflicts[i])
      res += ಎಣಿಕೆ_state_rr_conflicts (states[i]);
  ಪ್ರತ್ಯಾಗಮ res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಯದಿ (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        ಪೂರ್ಣಾಙ್ಕ j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_ಚಿಹ್ನೆ (trans, j));
      }

  ಪ್ರತ್ಯಾಗಮ res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಯದಿ (conflicts[i])
      res += ಎಣಿಕೆ_rule_state_sr_conflicts (r, states[i]);
  ಪ್ರತ್ಯಾಗಮ res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  ಧ್ರುವ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    ಯದಿ (reds->rules[i] == r)
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < reds->ಅಂಕೆ; ++j)
        ಯದಿ (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_ಎಣಿಕೆ (lookaheads);
          }
  bitset_free (lookaheads);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ size_t
ಎಣಿಕೆ_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    res += ಎಣಿಕೆ_rule_state_rr_conflicts (r, states[i]);
  ಪ್ರತ್ಯಾಗಮ res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

ಶೂನ್ಯ
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಯದಿ (conflicts[i])
      {
        ಧ್ರುವ state *s = states[i];
        ಪೂರ್ಣಾಙ್ಕ src = ಎಣಿಕೆ_state_sr_conflicts (s);
        ಪೂರ್ಣಾಙ್ಕ rrc = ಎಣಿಕೆ_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        ಯದಿ (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        ಅನ್ಯಥಾ ಯದಿ (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        ಅನ್ಯಥಾ ಯದಿ (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  ಯದಿ (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

ಪೂರ್ಣಾಙ್ಕ
conflicts_total_ಎಣಿಕೆ (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ_sr_conflicts () + ಎಣಿಕೆ_rr_conflicts ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
report_counterexamples (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ sn = 0; sn < nstates; ++sn)
    ಯದಿ (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
report_rule_expectation_mismatches (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      ಪೂರ್ಣಾಙ್ಕ expected_sr = r->expected_sr_conflicts;
      ಪೂರ್ಣಾಙ್ಕ expected_rr = r->expected_rr_conflicts;

      ಯದಿ (expected_sr != -1 || expected_rr != -1)
        {
          ಪೂರ್ಣಾಙ್ಕ sr = ಎಣಿಕೆ_rule_sr_conflicts (r);
          ಯದಿ (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          ಪೂರ್ಣಾಙ್ಕ rr = ಎಣಿಕೆ_rule_rr_conflicts (r);
          ಯದಿ (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

ಶೂನ್ಯ
conflicts_print (ಶೂನ್ಯ)
{
  report_rule_expectation_mismatches ();

  ಯದಿ (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (ರಿಕ್ತ, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    ಪೂರ್ಣಾಙ್ಕ total = ಎಣಿಕೆ_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    ಪೂರ್ಣಾಙ್ಕ expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    ಯದಿ (expected != -1)
      {
        ಯದಿ (expected != total)
          {
            complain (ರಿಕ್ತ, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            ಯದಿ (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    ಅನ್ಯಥಾ ಯದಿ (total)
      {
        complain (ರಿಕ್ತ, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    ಪೂರ್ಣಾಙ್ಕ total = ಎಣಿಕೆ_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    ಪೂರ್ಣಾಙ್ಕ expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    ಯದಿ (expected != -1)
      {
        ಯದಿ (expected != total)
          {
            complain (ರಿಕ್ತ, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            ಯದಿ (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    ಅನ್ಯಥಾ ಯದಿ (total)
      {
        complain (ರಿಕ್ತ, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  ಯದಿ (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  ಅನ್ಯಥಾ ಯದಿ (unexpected_conflicts_warning != Wnone)
    subcomplain (ರಿಕ್ತ, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

ಶೂನ್ಯ
conflicts_free (ಶೂನ್ಯ)
{
  ಮುಕ್ತ (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, ರಿಕ್ತ);
  obstack_free (&solved_conflicts_xml_obstack, ರಿಕ್ತ);
}
