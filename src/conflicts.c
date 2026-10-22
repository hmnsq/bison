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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "conflicts.h"
#ଅନ୍ତର୍ଭାଵ "counterexample.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "print-xml.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "state.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

/* -1 stands for not specified. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_sr_conflicts = -1;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
ସ୍ଥିର bool *conflicts;

ସ୍ଥିର ସଂରଚନା obstack solved_conflicts_obstack;
ସ୍ଥିର ସଂରଚନା obstack solved_conflicts_xml_obstack;

ସ୍ଥିର bitset shift_set;
ସ୍ଥିର bitset lookahead_set;

bool
has_conflicts (ଅଚଳ state *s)
{
  ଫେରନ୍ତୁ conflicts[s->ସଂଖ୍ୟା];
}



ଗଣନା conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    ବାମ_resolution,
    ଡାହାଣ_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
log_resolution (rule *r, ପ୍ରତୀକ_ସଂଖ୍ୟା token,
                ଗଣନା conflict_resolution resolution)
{
  ଯଦି (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      ବାଛ (resolution)
        {
        ସ୍ଥିତି shift_resolution:
        ସ୍ଥିତି ଡାହାଣ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->ସଂଖ୍ୟା,
                          symbols[token]->tag);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି reduce_resolution:
        ସ୍ଥିତି ବାମ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->ସଂଖ୍ୟା,
                          symbols[token]->tag);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->ସଂଖ୍ୟା,
                          symbols[token]->tag);
          ଭାଙ୍ଗ;
        }

      /* The reason. */
      ବାଛ (resolution)
        {
        ସ୍ଥିତି shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->ପ୍ରତୀକ->tag,
                          symbols[token]->tag);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->ପ୍ରତୀକ->tag);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ବାମ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ଡାହାଣ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          ଭାଙ୍ଗ;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  ଯଦି (xml_flag)
    {
      /* The description of the resolution. */
      ବାଛ (resolution)
        {
        ସ୍ଥିତି shift_resolution:
        ସ୍ଥିତି ଡାହାଣ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->ସଂଖ୍ୟା,
                          xml_escape (symbols[token]->tag));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି reduce_resolution:
        ସ୍ଥିତି ବାମ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->ସଂଖ୍ୟା,
                          xml_escape (symbols[token]->tag));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->ସଂଖ୍ୟା,
                          xml_escape (symbols[token]->tag));
          ଭାଙ୍ଗ;
        }

      /* The reason. */
      ବାଛ (resolution)
        {
        ସ୍ଥିତି shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->ପ୍ରତୀକ->tag),
                          xml_escape_n (1, symbols[token]->tag));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->ପ୍ରତୀକ->tag));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ବାମ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି ଡାହାଣ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      ଭାଙ୍ଗ;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
flush_shift (state *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < trans->ଅଙ୍କ; ++i)
    ଯଦି (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_ପ୍ରତୀକ (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
flush_reduce (bitset lookaheads, ପୂର୍ଣ୍ଣସଂଖ୍ୟା token)
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

ସ୍ଥିର ଶୂନ୍ୟ
resolve_sr_conflict (state *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ruleno, ପ୍ରତୀକ **errors, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < ntokens; ++i)
    ଯଦି (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        ଯଦି (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->ସଂଖ୍ୟା, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        ଅନ୍ୟଥା ଯଦି (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->ସଂଖ୍ୟା);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        ଅନ୍ୟଥା
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          ବାଛ (symbols[i]->content->assoc)
            {
            ସ୍ଥିତି undef_assoc:
              abort ();

            ସ୍ଥିତି precedence_assoc:
              ଭାଙ୍ଗ;

            ସ୍ଥିତି ଡାହାଣ_assoc:
              register_assoc (i, redrule->prec->ସଂଖ୍ୟା);
              log_resolution (redrule, i, ଡାହାଣ_resolution);
              flush_reduce (lookaheads, i);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି ବାମ_assoc:
              register_assoc (i, redrule->prec->ସଂଖ୍ୟା);
              log_resolution (redrule, i, ବାମ_resolution);
              flush_shift (s, i);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି non_assoc:
              register_assoc (i, redrule->prec->ସଂଖ୍ୟା);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              ଭାଙ୍ଗ;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
set_conflicts (state *s, ପ୍ରତୀକ **errors)
{
  ଯଦି (s->consistent)
    ଫେରନ୍ତୁ;

  reductions *reds = s->reductions;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_ପ୍ରତୀକ (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
    ଯଦି (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  ଯଦି (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  ଯଦି (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  ଯଦି (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  ଯଦି (reds->lookaheads)
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
      {
        ଯଦି (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->ସଂଖ୍ୟା] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

ଶୂନ୍ୟ
conflicts_solve (ଶୂନ୍ୟ)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  ପ୍ରତୀକ **errors = xnmalloc (ntokens + 1, ଆକାର *errors);

  conflicts = xcalloc (nstates, ଆକାର *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      ଯଦି (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  ମୁକ୍ତ (errors);
}


ଶୂନ୍ୟ
conflicts_update_state_numbers (state_ସଂଖ୍ୟା old_to_new[],
                                state_ସଂଖ୍ୟା nstates_old)
{
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates_old; ++i)
    ଯଦି (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

ସ୍ଥିର size_t
ଗଣନା_state_sr_conflicts (ଅଚଳ state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ଯଦି (!trans)
    ଫେରନ୍ତୁ 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_ପ୍ରତୀକ (trans, i));
  }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  ଫେରନ୍ତୁ bitset_ଗଣନା (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

ସ୍ଥିର size_t
ଗଣନା_sr_conflicts (ଶୂନ୍ୟ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    ଯଦି (conflicts[i])
      res += ଗଣନା_state_sr_conflicts (states[i]);
  ଫେରନ୍ତୁ res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

ସ୍ଥିର size_t
ଗଣନା_state_rr_conflicts (ଅଚଳ state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < ntokens; ++i)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < reds->ଅଙ୍କ; ++j)
        ଗଣନା_2 += bitset_test (reds->lookaheads[j], i);
      ଯଦି (2 <= ଗଣନା_2)
        res += ଗଣନା_2-1;
    }

  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର size_t
ଗଣନା_rr_conflicts (ଶୂନ୍ୟ)
{
  size_t res = 0;
  /* Conflicts by state.  */
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    ଯଦି (conflicts[i])
      res += ଗଣନା_state_rr_conflicts (states[i]);
  ଫେରନ୍ତୁ res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

ସ୍ଥିର size_t
ଗଣନା_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
    ଯଦି (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_ପ୍ରତୀକ (trans, j));
      }

  ଫେରନ୍ତୁ res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

ସ୍ଥିର size_t
ଗଣନା_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    ଯଦି (conflicts[i])
      res += ଗଣନା_rule_state_sr_conflicts (r, states[i]);
  ଫେରନ୍ତୁ res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

ସ୍ଥିର size_t
ଗଣନା_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  ଅଚଳ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
    ଯଦି (reds->rules[i] == r)
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < reds->ଅଙ୍କ; ++j)
        ଯଦି (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_ଗଣନା (lookaheads);
          }
  bitset_free (lookaheads);
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର size_t
ଗଣନା_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    res += ଗଣନା_rule_state_rr_conflicts (r, states[i]);
  ଫେରନ୍ତୁ res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

ଶୂନ୍ୟ
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    ଯଦି (conflicts[i])
      {
        ଅଚଳ state *s = states[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା src = ଗଣନା_state_sr_conflicts (s);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା rrc = ଗଣନା_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        ଯଦି (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        ଅନ୍ୟଥା ଯଦି (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        ଅନ୍ୟଥା ଯଦି (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  ଯଦି (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
conflicts_total_ଗଣନା (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ ଗଣନା_sr_conflicts () + ଗଣନା_rr_conflicts ();
}

ସ୍ଥିର ଶୂନ୍ୟ
report_counterexamples (ଶୂନ୍ୟ)
{
  ପାଇଁ (state_ସଂଖ୍ୟା sn = 0; sn < nstates; ++sn)
    ଯଦି (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
report_rule_expectation_mismatches (ଶୂନ୍ୟ)
{
  ପାଇଁ (rule_ସଂଖ୍ୟା i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_sr = r->expected_sr_conflicts;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_rr = r->expected_rr_conflicts;

      ଯଦି (expected_sr != -1 || expected_rr != -1)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା sr = ଗଣନା_rule_sr_conflicts (r);
          ଯଦି (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rr = ଗଣନା_rule_rr_conflicts (r);
          ଯଦି (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

ଶୂନ୍ୟ
conflicts_print (ଶୂନ୍ୟ)
{
  report_rule_expectation_mismatches ();

  ଯଦି (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (ଖାଲି, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = ଗଣନା_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    ଯଦି (expected != -1)
      {
        ଯଦି (expected != total)
          {
            complain (ଖାଲି, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            ଯଦି (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    ଅନ୍ୟଥା ଯଦି (total)
      {
        complain (ଖାଲି, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = ଗଣନା_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    ଯଦି (expected != -1)
      {
        ଯଦି (expected != total)
          {
            complain (ଖାଲି, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            ଯଦି (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    ଅନ୍ୟଥା ଯଦି (total)
      {
        complain (ଖାଲି, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  ଯଦି (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  ଅନ୍ୟଥା ଯଦି (unexpected_conflicts_warning != Wnone)
    subcomplain (ଖାଲି, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

ଶୂନ୍ୟ
conflicts_free (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, ଖାଲି);
  obstack_free (&solved_conflicts_xml_obstack, ଖାଲି);
}
