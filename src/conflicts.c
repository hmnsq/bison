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

स्थिर bitset shift_set;
स्थिर bitset lookahead_set;

bool
has_conflicts (अचर state *s)
{
  फर्काऊ conflicts[s->सङ्ख्या];
}



गणना conflict_resolution
  {
    shift_resolution,
    reduce_resolution,
    बायाँ_resolution,
    दायाँ_resolution,
    nonassoc_resolution
  };


/*----------------------------------------------------------------.
| Explain how an SR conflict between TOKEN and RULE was resolved: |
| RESOLUTION.                                                     |
`----------------------------------------------------------------*/

स्थिर अन्तःस्थ शून्य
log_resolution (rule *r, चिह्न_सङ्ख्या token,
                गणना conflict_resolution resolution)
{
  यदि (report_flag & report_solved_conflicts)
    {
      /* The description of the resolution. */
      विकल्प (resolution)
        {
        पक्ष shift_resolution:
        पक्ष दायाँ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as shift"),
                          r->सङ्ख्या,
                          symbols[token]->tag);
          रोक;

        पक्ष reduce_resolution:
        पक्ष बायाँ_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as reduce"),
                          r->सङ्ख्या,
                          symbols[token]->tag);
          रोक;

        पक्ष nonassoc_resolution:
          obstack_sgrow (&solved_conflicts_obstack, "    ");
          obstack_printf (&solved_conflicts_obstack,
                          _("Conflict between rule %d and token %s"
                            " resolved as an error"),
                          r->सङ्ख्या,
                          symbols[token]->tag);
          रोक;
        }

      /* The reason. */
      विकल्प (resolution)
        {
        पक्ष shift_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          r->prec->चिह्न->tag,
                          symbols[token]->tag);
          रोक;

        पक्ष reduce_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%s < %s)",
                          symbols[token]->tag,
                          r->prec->चिह्न->tag);
          रोक;

        पक्ष बायाँ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%left %s)",
                          symbols[token]->tag);
          रोक;

        पक्ष दायाँ_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%right %s)",
                          symbols[token]->tag);
          रोक;

        पक्ष nonassoc_resolution:
          obstack_printf (&solved_conflicts_obstack,
                          " (%%nonassoc %s)",
                          symbols[token]->tag);
          रोक;
        }

      obstack_sgrow (&solved_conflicts_obstack, ".\n");
    }

  /* XML report */
  यदि (xml_flag)
    {
      /* The description of the resolution. */
      विकल्प (resolution)
        {
        पक्ष shift_resolution:
        पक्ष दायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"shift\">",
                          r->सङ्ख्या,
                          xml_escape (symbols[token]->tag));
          रोक;

        पक्ष reduce_resolution:
        पक्ष बायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"reduce\">",
                          r->सङ्ख्या,
                          xml_escape (symbols[token]->tag));
          रोक;

        पक्ष nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "        <resolution rule=\"%d\" symbol=\"%s\""
                          " type=\"error\">",
                          r->सङ्ख्या,
                          xml_escape (symbols[token]->tag));
          रोक;
        }

      /* The reason. */
      विकल्प (resolution)
        {
        पक्ष shift_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, r->prec->चिह्न->tag),
                          xml_escape_n (1, symbols[token]->tag));
          रोक;

        पक्ष reduce_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%s &lt; %s",
                          xml_escape_n (0, symbols[token]->tag),
                          xml_escape_n (1, r->prec->चिह्न->tag));
          रोक;

        पक्ष बायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%left %s",
                          xml_escape (symbols[token]->tag));
          रोक;

        पक्ष दायाँ_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%right %s",
                          xml_escape (symbols[token]->tag));
          रोक;

        पक्ष nonassoc_resolution:
          obstack_printf (&solved_conflicts_xml_obstack,
                          "%%nonassoc %s",
                          xml_escape (symbols[token]->tag));
      रोक;
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
flush_shift (state *s, पूर्णाङ्क token)
{
  transitions *trans = s->transitions;

  bitset_reset (lookahead_set, token);
  लागि (पूर्णाङ्क i = 0; i < trans->अङ्क; ++i)
    यदि (!TRANSITION_IS_DISABLED (trans, i)
        && TRANSITION_चिह्न (trans, i) == token)
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
resolve_sr_conflict (state *s, पूर्णाङ्क ruleno, चिह्न **errors, पूर्णाङ्क *nerrs)
{
  reductions *reds = s->reductions;
  /* Find the rule to reduce by to get precedence of reduction.  */
  rule *redrule = reds->rules[ruleno];
  पूर्णाङ्क redprec = redrule->prec->prec;
  bitset lookaheads = reds->lookaheads[ruleno];

  लागि (चिह्न_सङ्ख्या i = 0; i < ntokens; ++i)
    यदि (bitset_test (lookaheads, i)
        && bitset_test (lookahead_set, i)
        && symbols[i]->content->prec)
      {
        /* Shift/reduce conflict occurs for token number i
           and it has a precedence.
           The precedence of shifting is that of token i.  */
        यदि (symbols[i]->content->prec < redprec)
          {
            register_precedence (redrule->prec->सङ्ख्या, i);
            log_resolution (redrule, i, reduce_resolution);
            flush_shift (s, i);
          }
        अन्यथा यदि (symbols[i]->content->prec > redprec)
          {
            register_precedence (i, redrule->prec->सङ्ख्या);
            log_resolution (redrule, i, shift_resolution);
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
              पात ();

            पक्ष precedence_assoc:
              रोक;

            पक्ष दायाँ_assoc:
              register_assoc (i, redrule->prec->सङ्ख्या);
              log_resolution (redrule, i, दायाँ_resolution);
              flush_reduce (lookaheads, i);
              रोक;

            पक्ष बायाँ_assoc:
              register_assoc (i, redrule->prec->सङ्ख्या);
              log_resolution (redrule, i, बायाँ_resolution);
              flush_shift (s, i);
              रोक;

            पक्ष non_assoc:
              register_assoc (i, redrule->prec->सङ्ख्या);
              log_resolution (redrule, i, nonassoc_resolution);
              flush_shift (s, i);
              flush_reduce (lookaheads, i);
              /* Record an explicit error for this token.  */
              errors[(*nerrs)++] = symbols[i];
              रोक;
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
set_conflicts (state *s, चिह्न **errors)
{
  यदि (s->consistent)
    फर्काऊ;

  reductions *reds = s->reductions;
  पूर्णाङ्क nerrs = 0;

  bitset_zero (lookahead_set);

  {
    transitions *trans = s->transitions;
    पूर्णाङ्क i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (lookahead_set, TRANSITION_चिह्न (trans, i));
  }

  /* Loop over all rules which require lookahead in this state.  First
     check for shift/reduce conflict, and try to resolve using
     precedence.  */
  लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
    यदि (reds->rules[i]->prec
        && reds->rules[i]->prec->prec
        && !bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
      resolve_sr_conflict (s, i, errors, &nerrs);

  यदि (nerrs)
    /* Some tokens have been explicitly made errors.  Allocate a
       permanent errs structure for this state, to record them.  */
    state_errs_set (s, nerrs, errors);

  यदि (obstack_object_size (&solved_conflicts_obstack))
    s->solved_conflicts = obstack_finish0 (&solved_conflicts_obstack);
  यदि (obstack_object_size (&solved_conflicts_xml_obstack))
    s->solved_conflicts_xml = obstack_finish0 (&solved_conflicts_xml_obstack);

  /* Loop over all rules which require lookahead in this state.  Check
     for conflicts not resolved above.

     reds->lookaheads can be NULL if the LR type is LR(0).  */
  यदि (reds->lookaheads)
    लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
      {
        यदि (!bitset_disjoint_p (reds->lookaheads[i], lookahead_set))
          conflicts[s->सङ्ख्या] = true;
        bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);
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
  चिह्न **errors = xnmalloc (ntokens + 1, परिमाण *errors);

  conflicts = xcalloc (nstates, परिमाण *conflicts);
  shift_set = bitset_create (ntokens, BITSET_FIXED);
  lookahead_set = bitset_create (ntokens, BITSET_FIXED);
  obstack_init (&solved_conflicts_obstack);
  obstack_init (&solved_conflicts_xml_obstack);

  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      set_conflicts (states[i], errors);

      /* For uniformity of the code, make sure all the states have a valid
         'errs' member.  */
      यदि (!states[i]->errs)
        states[i]->errs = errs_new (0, 0);
    }

  मुक्त (errors);
}


शून्य
conflicts_update_state_numbers (state_सङ्ख्या old_to_new[],
                                state_सङ्ख्या nstates_old)
{
  लागि (state_सङ्ख्या i = 0; i < nstates_old; ++i)
    यदि (old_to_new[i] != nstates_old)
      conflicts[old_to_new[i]] = conflicts[i];
}


/*---------------------------------------------.
| Count the number of shift/reduce conflicts.  |
`---------------------------------------------*/

स्थिर माप_प्रकार
गन्ती_state_sr_conflicts (अचर state *s)
{
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  यदि (!trans)
    फर्काऊ 0;

  bitset_zero (lookahead_set);
  bitset_zero (shift_set);

  {
    पूर्णाङ्क i;
    FOR_EACH_SHIFT (trans, i)
      bitset_set (shift_set, TRANSITION_चिह्न (trans, i));
  }

  लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
    bitset_or (lookahead_set, lookahead_set, reds->lookaheads[i]);

  bitset_and (lookahead_set, lookahead_set, shift_set);

  फर्काऊ bitset_गन्ती (lookahead_set);
}

/*---------------------------------------------.
| The total number of shift/reduce conflicts.  |
`---------------------------------------------*/

स्थिर माप_प्रकार
गन्ती_sr_conflicts (शून्य)
{
  माप_प्रकार res = 0;
  /* Conflicts by state.  */
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      res += गन्ती_state_sr_conflicts (states[i]);
  फर्काऊ res;
}



/*-----------------------------------------------------------------.
| Count the number of reduce/reduce conflicts.  Count one conflict |
| for each reduction after the first for a given token.            |
`-----------------------------------------------------------------*/

स्थिर माप_प्रकार
गन्ती_state_rr_conflicts (अचर state *s)
{
  reductions *reds = s->reductions;
  माप_प्रकार res = 0;

  लागि (चिह्न_सङ्ख्या i = 0; i < ntokens; ++i)
    {
      पूर्णाङ्क गन्ती = 0;
      लागि (पूर्णाङ्क j = 0; j < reds->अङ्क; ++j)
        गन्ती += bitset_test (reds->lookaheads[j], i);
      यदि (2 <= गन्ती)
        res += गन्ती-1;
    }

  फर्काऊ res;
}

स्थिर माप_प्रकार
गन्ती_rr_conflicts (शून्य)
{
  माप_प्रकार res = 0;
  /* Conflicts by state.  */
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      res += गन्ती_state_rr_conflicts (states[i]);
  फर्काऊ res;
}


/*------------------------------------------------------------------.
| For a given rule, the number of shift/reduce conflicts in a given |
| state.                                                            |
`------------------------------------------------------------------*/

स्थिर माप_प्रकार
गन्ती_rule_state_sr_conflicts (rule *r, state *s)
{
  माप_प्रकार res = 0;
  transitions *trans = s->transitions;
  reductions *reds = s->reductions;

  लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
    यदि (reds->rules[i] == r)
      {
        bitset lookaheads = reds->lookaheads[i];
        पूर्णाङ्क j;
        FOR_EACH_SHIFT (trans, j)
          res += bitset_test (lookaheads, TRANSITION_चिह्न (trans, j));
      }

  फर्काऊ res;
}

/*----------------------------------------------------------------------.
| For a given rule, count the number of states for which it is involved |
| in shift/reduce conflicts.                                            |
`----------------------------------------------------------------------*/

स्थिर माप_प्रकार
गन्ती_rule_sr_conflicts (rule *r)
{
  माप_प्रकार res = 0;
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      res += गन्ती_rule_state_sr_conflicts (r, states[i]);
  फर्काऊ res;
}

/*-----------------------------------------------------------------.
| For a given rule, count the number of states in which it is      |
| involved in reduce/reduce conflicts.                             |
`-----------------------------------------------------------------*/

स्थिर माप_प्रकार
गन्ती_rule_state_rr_conflicts (rule *r, state *s)
{
  माप_प्रकार res = 0;
  अचर reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);

  लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
    यदि (reds->rules[i] == r)
      लागि (पूर्णाङ्क j = 0; j < reds->अङ्क; ++j)
        यदि (reds->rules[j] != r)
          {
            bitset_and (lookaheads,
                        reds->lookaheads[i],
                        reds->lookaheads[j]);
            res += bitset_गन्ती (lookaheads);
          }
  bitset_free (lookaheads);
  फर्काऊ res;
}

स्थिर माप_प्रकार
गन्ती_rule_rr_conflicts (rule *r)
{
  माप_प्रकार res = 0;
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    res += गन्ती_rule_state_rr_conflicts (r, states[i]);
  फर्काऊ res;
}

/*-----------------------------------------------------------.
| Output the detailed description of states with conflicts.  |
`-----------------------------------------------------------*/

शून्य
conflicts_output (खाता *out)
{
  bool printed_sth = false;
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (conflicts[i])
      {
        अचर state *s = states[i];
        पूर्णाङ्क src = गन्ती_state_sr_conflicts (s);
        पूर्णाङ्क rrc = गन्ती_state_rr_conflicts (s);
        ख_लिखो (out, _("State %d "), i);
        यदि (src && rrc)
          ख_लिखो (out,
                   _("conflicts: %d shift/reduce, %d reduce/reduce\n"),
                   src, rrc);
        अन्यथा यदि (src)
          ख_लिखो (out, _("conflicts: %d shift/reduce\n"), src);
        अन्यथा यदि (rrc)
          ख_लिखो (out, _("conflicts: %d reduce/reduce\n"), rrc);
        printed_sth = true;
      }
  यदि (printed_sth)
    ख_माला_दो ("\n\n", out);
}

/*--------------------------------------------.
| Total the number of S/R and R/R conflicts.  |
`--------------------------------------------*/

पूर्णाङ्क
conflicts_total_गन्ती (शून्य)
{
  फर्काऊ गन्ती_sr_conflicts () + गन्ती_rr_conflicts ();
}

स्थिर शून्य
report_counterexamples (शून्य)
{
  लागि (state_सङ्ख्या sn = 0; sn < nstates; ++sn)
    यदि (conflicts[sn])
      counterexample_report_state (states[sn], मानक_त्रुटि, "");
}

/*------------------------------------------------.
| Report per-rule %expect/%expect-rr mismatches.  |
`------------------------------------------------*/

स्थिर शून्य
report_rule_expectation_mismatches (शून्य)
{
  लागि (rule_सङ्ख्या i = 0; i < nrules; i += 1)
    {
      rule *r = &rules[i];
      पूर्णाङ्क expected_sr = r->expected_sr_conflicts;
      पूर्णाङ्क expected_rr = r->expected_rr_conflicts;

      यदि (expected_sr != -1 || expected_rr != -1)
        {
          पूर्णाङ्क sr = गन्ती_rule_sr_conflicts (r);
          यदि (sr != expected_sr && (sr != 0 || expected_sr != -1))
            complain (&r->location, complaint,
                      _("shift/reduce conflicts for rule %d:"
                        " %d found, %d expected"),
                      r->code, sr, expected_sr);
          पूर्णाङ्क rr = गन्ती_rule_rr_conflicts (r);
          यदि (rr != expected_rr && (rr != 0 || expected_rr != -1))
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

शून्य
conflicts_print (शून्य)
{
  report_rule_expectation_mismatches ();

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
    पूर्णाङ्क total = गन्ती_sr_conflicts ();
    /* If %expect is not used, but %expect-rr is, then expect 0 sr.  */
    पूर्णाङ्क expected =
      (expected_sr_conflicts == -1 && expected_rr_conflicts != -1)
      ? 0
      : expected_sr_conflicts;
    यदि (expected != -1)
      {
        यदि (expected != total)
          {
            complain (रिक्त, complaint,
                      _("shift/reduce conflicts: %d found, %d expected"),
                      total, expected);
            यदि (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा यदि (total)
      {
        complain (रिक्त, Wconflicts_sr,
                  ngettext ("%d shift/reduce conflict",
                            "%d shift/reduce conflicts",
                            total),
                  total);
        unexpected_conflicts_warning = Wconflicts_sr;
      }
  }

  {
    पूर्णाङ्क total = गन्ती_rr_conflicts ();
    /* If %expect-rr is not used, but %expect is, then expect 0 rr.  */
    पूर्णाङ्क expected =
      (expected_rr_conflicts == -1 && expected_sr_conflicts != -1)
      ? 0
      : expected_rr_conflicts;
    यदि (expected != -1)
      {
        यदि (expected != total)
          {
            complain (रिक्त, complaint,
                      _("reduce/reduce conflicts: %d found, %d expected"),
                      total, expected);
            यदि (total)
              unexpected_conflicts_warning = complaint;
          }
      }
    अन्यथा यदि (total)
      {
        complain (रिक्त, Wconflicts_rr,
                  ngettext ("%d reduce/reduce conflict",
                            "%d reduce/reduce conflicts",
                            total),
                  total);
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
  bitset_free (shift_set);
  bitset_free (lookahead_set);
  obstack_free (&solved_conflicts_obstack, रिक्त);
  obstack_free (&solved_conflicts_xml_obstack, रिक्त);
}
