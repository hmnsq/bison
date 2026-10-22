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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "counterexample.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "print-xml.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "state.h"
#અન્તર્ભાવ "symtab.h"

/* -1 stands for not specified. */
પૂર્ણાંક expected_sr_conflicts = -1;
પૂર્ણાંક expected_rr_conflicts = -1;

/* CONFLICTS[STATE-NUM] -- Whether that state has unresolved conflicts.  */
સ્થિર bool *conflicts;

સ્થિર સંરચના obstack solved_conflicts_obstack;
સ્થિર સંરચના obstack solved_conflicts_xml_obstack;

સ્થિર bitset shift_set;
સ્થિર bitset lookahead_set;

bool
has_conflicts (અચલ state *s)
{
  પાછા conflicts[s->સંખ્યા];
}



ગણના conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    ડાબું_resolution,
    જમણું_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

સ્થિર અંતઃરેખ શૂન્ય
log_resolution (rule *r, પ્રતીક_સંખ્યા token,
                ગણના conflict_resolution resolution)
{
  જો (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      પસંદગી (resolution)
        {
        સ્થિતિ shift_resolution:
        સ્થિતિ જમણું_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->સંખ્યા,
                          symbols[token]->tag);
          તોડો;

        સ્થિતિ reduce_resolution:
        સ્થિતિ ડાબું_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->સંખ્યા,
                          symbols[token]->tag);
          તોડો;

        સ્થિતિ nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->સંખ્યા,
                          symbols[token]->tag);
          તોડો;
        }

      /* The reason. */
      પસંદગી (resolution)
        {
        સ્થિતિ shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->પ્રતીક->tag,
                          symbols[token]->tag);
          તોડો;

        સ્થિતિ reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->પ્રતીક->tag);
          તોડો;

        સ્થિતિ ડાબું_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          તોડો;

        સ્થિતિ જમણું_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          તોડો;

        સ્થિતિ nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          તોડો;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  જો (xml_flag)
    {
      /* The description of the resolution. */
      પસંદગી (resolution)
        {
        સ્થિતિ shift_resolution:
        સ્થિતિ જમણું_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->સંખ્યા,
                          xml_escape (symbols[token]->tag));
          તોડો;

        સ્થિતિ reduce_resolution:
        સ્થિતિ ડાબું_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->સંખ્યા,
                          xml_escape (symbols[token]->tag));
          તોડો;

        સ્થિતિ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->સંખ્યા,
                          xml_escape (symbols[token]->tag));
          તોડો;
        }

      /* The reason. */
      પસંદગી (resolution)
        {
        સ્થિતિ shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->પ્રતીક->tag),
                          xml_escape_n (1, symbols[token]->tag));
          તોડો;

        સ્થિતિ reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->પ્રતીક->tag));
          તોડો;

        સ્થિતિ ડાબું_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          તોડો;

        સ્થિતિ જમણું_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          તોડો;

        સ્થિતિ nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      તોડો;
        }

      obstack_sgrow (&solved_conflicts_xml_obstack, "</resolution>\n");
    }
}


/*------------------------------------------------------------------.
| Turn off the shift recorded for the specified token in the        |
| specified state.  Used when we resolve a shift/reduce conflict in |
| favor of the reduction or as an error (%nonassoc).                |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
flush_shift (state *s, પૂર્ણાંક token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  માટે (પૂર્ણાંક i = 0; i < trans->અંક; ++i)
    જો (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_પ્રતીક (trans, i) == token)
      TRANSITION_DISABLE (trans, i);
}


/*--------------------------------------------------------------------.
| Turn off the reduce recorded for the specified token in the         |
| specified lookahead set.  Used when we resolve a shift/reduce       |
| conflict in favor of the shift or as an error (%nonassoc).          |
`--------------------------------------------------------------------*/

સ્થિર શૂન્ય
flush_reduce (bitset lookaheads, પૂર્ણાંક token)
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

સ્થિર શૂન્ય
resolve_sr_conflict (state *s, પૂર્ણાંક ruleno, પ્રતીક **errors, પૂર્ણાંક *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  પૂર્ણાંક redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  માટે (પ્રતીક_સંખ્યા i = 0; i < ntokens; ++i)
    જો (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        જો (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->સંખ્યા, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        નહીંતર જો (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->સંખ્યા);
            log_resolution (redrule, i, shift_resolution);
            flush_reduce (lookaheads, i);
          }
        નહીંતર
          /* Matching precedence levels.
             For non-defined associativity, keep both: unexpected
             associativity conflict.
             For left associativity, keep only the reduction.
             For right associativity, keep only the shift.
             For nonassociativity, keep neither.  */

          પસંદગી (symbols[i]->content->assoc)
            {
            સ્થિતિ undef_assoc:
              abort ();

            સ્થિતિ precedence_assoc:
              તોડો;

            સ્થિતિ જમણું_assoc:
              register_assoc (i, redrule->prec->સંખ્યા);
              log_resolution (redrule, i, જમણું_resolution);
              flush_reduce (lookaheads, i);
              તોડો;

            સ્થિતિ ડાબું_assoc:
              register_assoc (i, redrule->prec->સંખ્યા);
              log_resolution (redrule, i, ડાબું_resolution);
              flush_shift (s, i);
              તોડો;

            સ્થિતિ non_assoc:
              register_assoc (i, redrule->prec->સંખ્યા);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              તોડો;
            }
      }
}


/*-------------------------------------------------------------------.
| Solve the S/R conflicts of state S using the                       |
| precedence/associativity, and flag it inconsistent if it still has |
| conflicts.  ERRORS can be used as storage to compute the list of   |
| lookahead tokens on which S raises a syntax error (%nonassoc).     |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
set_conflicts (state *s, પ્રતીક **errors)
{
  જો (s->consistent)
    પાછા;

  reductions *reds = s->reductions;
  પૂર્ણાંક nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    પૂર્ણાંક i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_પ્રતીક (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
    જો (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  જો (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  જો (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  જો (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  જો (reds->lookaheads)
    માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
      {
        જો (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->સંખ્યા] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
      }
}


/*----------------------------------------------------------------.
| Solve all the S/R conflicts using the precedence/associativity, |
| and flag as inconsistent the states that still have conflicts.  |
`----------------------------------------------------------------*/

શૂન્ય
conflicts_solve (શૂન્ય)
{
  /* List of lookahead tokens on which we explicitly raise a syntax error.  */
  પ્રતીક **errors = xnmalloc (ntokens + 1, કદ *errors);

  conflicts = xcalloc (nstates, કદ *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      જો (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  મુક્ત (errors);
}


શૂન્ય
conflicts_update_state_numbers (state_સંખ્યા old_to_new[],
                                state_સંખ્યા nstates_old)
{
  માટે (state_સંખ્યા i = 0; i < nstates_old; ++i)
    જો (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

સ્થિર size_t
ગણતરી_state_sr_conflicts (અચલ state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  જો (!trans)
    પાછા 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    પૂર્ણાંક i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_પ્રતીક (trans, i));
  }

  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  પાછા bitset_ગણતરી (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

સ્થિર size_t
ગણતરી_sr_conflicts (શૂન્ય)
{
  size_t res = 0;
  /* Conflicts by state.  */
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    જો (conflicts[i])
      res += ગણતરી_state_sr_conflicts (states[i]);
  પાછા res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

સ્થિર size_t
ગણતરી_state_rr_conflicts (અચલ state *s)
{
  reductions *reds = s->reductions;
  size_t res = 0;

  માટે (પ્રતીક_સંખ્યા i = 0; i < ntokens; ++i)
    {
      પૂર્ણાંક ગણતરી = 0;
      માટે (પૂર્ણાંક j = 0; j < reds->અંક; ++j)
        ગણતરી += bitset_test (reds->lookaheads[j], i);
      જો (2 <= ગણતરી)
        res += ગણતરી-1;
    }

  પાછા res;
}

સ્થિર size_t
ગણતરી_rr_conflicts (શૂન્ય)
{
  size_t res = 0;
  /* Conflicts by state.  */
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    જો (conflicts[i])
      res += ગણતરી_state_rr_conflicts (states[i]);
  પાછા res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

સ્થિર size_t
ગણતરી_rule_state_sr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
    જો (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        પૂર્ણાંક j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_પ્રતીક (trans, j));
      }

  પાછા res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

સ્થિર size_t
ગણતરી_rule_sr_conflicts (rule *r)
{
  size_t res = 0;
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    જો (conflicts[i])
      res += ગણતરી_rule_state_sr_conflicts (r, states[i]);
  પાછા res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

સ્થિર size_t
ગણતરી_rule_state_rr_conflicts (rule *r, state *s)
{
  size_t res = 0;
  અચલ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
    જો (reds->rules[i] == r)
      માટે (પૂર્ણાંક j = 0; j < reds->અંક; ++j)
        જો (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_ગણતરી (lookaheads);
          }
  bitset_free (lookaheads);
  પાછા res;
}

સ્થિર size_t
ગણતરી_rule_rr_conflicts (rule *r)
{
  size_t res = 0;
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    res += ગણતરી_rule_state_rr_conflicts (r, states[i]);
  પાછા res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

શૂન્ય
conflicts_output (FILE *out)
{
  bool printed_sth = false;
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    જો (conflicts[i])
      {
        અચલ state *s = states[i];
        પૂર્ણાંક src = ગણતરી_state_sr_conflicts (s);
        પૂર્ણાંક rrc = ગણતરી_state_rr_conflicts (s);
        fprintf (out, _("State %d "), i);
        જો (src && rrc)
          fprintf (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        નહીંતર જો (src)
          fprintf (out, _("conflicts: %d shift/reduce\n"), src);
        નહીંતર જો (rrc)
          fprintf (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  જો (printed_sth)
    fputs ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

પૂર્ણાંક
conflicts_total_ગણતરી (શૂન્ય)
{
  પાછા ગણતરી_sr_conflicts () + ગણતરી_rr_conflicts ();
}

સ્થિર શૂન્ય
report_counterexamples (શૂન્ય)
{
  માટે (state_સંખ્યા sn = 0; sn < nstates; ++sn)
    જો (conflicts[sn])
      counterexample_report_state (states[sn], stderr, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

સ્થિર શૂન્ય
report_rule_expectation_mismatches (શૂન્ય)
{
  માટે (rule_સંખ્યા i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      પૂર્ણાંક expected_sr = r->expected_sr_conflicts;
      પૂર્ણાંક expected_rr = r->expected_rr_conflicts;

      જો (expected_sr != -1 || expected_rr != -1)
        {
          પૂર્ણાંક sr = ગણતરી_rule_sr_conflicts (r);
          જો (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          પૂર્ણાંક rr = ગણતરી_rule_rr_conflicts (r);
          જો (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

શૂન્ય
conflicts_print (શૂન્ય)
{
  report_rule_expectation_mismatches ();

  જો (! glr_parser && expected_rr_conflicts != -1)
    {
      complain (ખાલી, Wother, _("%%expect-rr applies only to GLR parsers"));
      expected_rr_conflicts = -1;
    }

  // The warning flags used to emit a diagnostic, if we did.
  warnings unexpected_conflicts_warning = Wnone;
  /* The following two blocks scream for factoring, but i18n support
     would make it ugly.  */
  {
    પૂર્ણાંક total = ગણતરી_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    પૂર્ણાંક expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    જો (expected != -1)
      {
        જો (expected != total)
          {
            complain (ખાલી, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            જો (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    નહીંતર જો (total)
      {
        complain (ખાલી, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    પૂર્ણાંક total = ગણતરી_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    પૂર્ણાંક expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    જો (expected != -1)
      {
        જો (expected != total)
          {
            complain (ખાલી, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            જો (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    નહીંતર જો (total)
      {
        complain (ખાલી, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_rr;
      }
  }

  જો (warning_is_enabled (Wcounterexamples))
    report_counterexamples ();
  નહીંતર જો (unexpected_conflicts_warning != Wnone)
    subcomplain (ખાલી, unexpected_conflicts_warning,
                 _("rerun with option '-Wcounterexamples'"
                   " to generate conflict counterexamples"));
}

શૂન્ય
conflicts_free (શૂન્ય)
{
  મુક્ત (conflicts);
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, ખાલી);
  obstack_free (&solved_conflicts_xml_obstack, ખાલી);
}
