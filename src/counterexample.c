/* Conflict counterexample generation

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ಅನ್ತರ್ಭಾವ "counterexample.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <c-strtod.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <gethrxtime.h>
#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <gl_rbtreehash_list.h>
#ಅನ್ತರ್ಭಾವ <hash.h>
#ಅನ್ತರ್ಭಾವ <mbswidth.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <textstyle.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "closure.h"
#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "derivation.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lssi.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "parse-simulation.h"


#ವ್ಯಾಖ್ಯಾನಿಸು ಸಮಯ_ಎಲ್ಲೆ_ENFORCED true
/** If set to false, only consider the states on the shortest
 *  lookahead-sensitive path when constructing a unifying counterexample. */
#ವ್ಯಾಖ್ಯಾನಿಸು EXTENDED_ಹುಡುಕು false

/* costs for making various steps in a search */
#ವ್ಯಾಖ್ಯಾನಿಸು PRODUCTION_ವೆಚ್ಚ 50
#ವ್ಯಾಖ್ಯಾನಿಸು REDUCE_ವೆಚ್ಚ 1
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರಿಸು_ವೆಚ್ಚ 1
#ವ್ಯಾಖ್ಯಾನಿಸು UNSHIFT_ವೆಚ್ಚ 1
#ವ್ಯಾಖ್ಯಾನಿಸು EXTENDED_ವೆಚ್ಚ 10000

/** The time limit before printing an assurance message to the user to
 *  indicate that the search is still running. */
#ವ್ಯಾಖ್ಯಾನಿಸು ASSURANCE_ಎಲ್ಲೆ 2.0

/* The time limit before giving up looking for unifying counterexample. */
ಸ್ಥಿರ ದ್ವಿಗುಣ ಸಮಯ_ಎಲ್ಲೆ = 5.0;

#ವ್ಯಾಖ್ಯಾನಿಸು CUMULATIVE_ಸಮಯ_ಎಲ್ಲೆ 120.0

// This is the fastest way to get the tail node from the gl_list API.
ಸ್ಥಿರ gl_list_ಗಂಟು_t
list_get_ಕೊನೆ (gl_list_t list)
{
  gl_list_ಗಂಟು_t sentinel = gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (list, ಖಾಲಿ);
  gl_list_ಗಂಟು_t res = gl_list_ಹಿಂದಿನ_ಗಂಟು (list, sentinel);
  gl_list_remove_ಗಂಟು (list, sentinel);
  ಹಿಂತಿರುಗಿ res;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  derivation *d1;
  derivation *d2;
  bool ಸರಿಸು_reduce;
  bool unifying;
  bool timeout;
} counterexample;

ಸ್ಥಿರ counterexample *
new_counterexample (derivation *d1, derivation *d2,
                    bool ಸರಿಸು_reduce,
                    bool u, bool t)
{
  counterexample *res = xmalloc (ಗಾತ್ರ *res);
  res->ಸರಿಸು_reduce = ಸರಿಸು_reduce;
  ಆದರೆ (ಸರಿಸು_reduce)
    {
      // Display the shift first.
      res->d1 = d2;
      res->d2 = d1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      res->d1 = d1;
      res->d2 = d2;
    }
  res->unifying = u;
  res->timeout = t;
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_counterexample (counterexample *cex)
{
  derivation_free (cex->d1);
  derivation_free (cex->d2);
  ಬಿಡುಗಡೆ (cex);
}

ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_print (ಸ್ಥಿರಾಂಕ counterexample *cex, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  ಸ್ಥಿರಾಂಕ bool flat = getenv ("YYFLAT");
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *example1_label
    = cex->unifying ? _("Example") : _("First example");
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *example2_label
    = cex->unifying ? _("Example") : _("Second example");
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *deriv1_label
    = cex->ಸರಿಸು_reduce ? _("Shift derivation") : _("First reduce derivation");
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *deriv2_label
    = cex->ಸರಿಸು_reduce ? _("Reduce derivation") : _("Second reduce derivation");
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ =
    max_int (max_int (mbswidth (example1_label, 0), mbswidth (example2_label, 0)),
             max_int (mbswidth (deriv1_label, 0),   mbswidth (deriv2_label, 0)));
  ಆದರೆ (flat)
    fprintf (ಹೊರಹೋಗು, "  %s%s%*s ", ಪೂರ್ವಪ್ರತ್ಯಯ,
             example1_label, ಅಗಲ - mbswidth (example1_label, 0), "");
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, "  %s%s: ", ಪೂರ್ವಪ್ರತ್ಯಯ, example1_label);
  derivation_print_leaves (cex->d1, ಹೊರಹೋಗು);
  ಆದರೆ (flat)
    fprintf (ಹೊರಹೋಗು, "  %s%s%*s ", ಪೂರ್ವಪ್ರತ್ಯಯ,
             deriv1_label, ಅಗಲ - mbswidth (deriv1_label, 0), "");
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, "  %s%s", ಪೂರ್ವಪ್ರತ್ಯಯ, deriv1_label);
  derivation_print (cex->d1, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);

  // If we output to the terminal (via stderr) and we have color
  // support, display unifying examples a second time, as color allows
  // to see the differences.
  ಆದರೆ (!cex->unifying || is_styled (stderr))
    {
      ಆದರೆ (flat)
        fprintf (ಹೊರಹೋಗು, "  %s%s%*s ", ಪೂರ್ವಪ್ರತ್ಯಯ,
                 example2_label, ಅಗಲ - mbswidth (example2_label, 0), "");
      ಇಲ್ಲದಿದ್ದರೆ
        fprintf (ಹೊರಹೋಗು, "  %s%s: ", ಪೂರ್ವಪ್ರತ್ಯಯ, example2_label);
      derivation_print_leaves (cex->d2, ಹೊರಹೋಗು);
    }
  ಆದರೆ (flat)
    fprintf (ಹೊರಹೋಗು, "  %s%s%*s ", ಪೂರ್ವಪ್ರತ್ಯಯ,
             deriv2_label, ಅಗಲ - mbswidth (deriv2_label, 0), "");
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, "  %s%s", ಪೂರ್ವಪ್ರತ್ಯಯ, deriv2_label);
  derivation_print (cex->d2, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);

  ಆದರೆ (ಹೊರಹೋಗು != stderr)
    putc ('\n', ಹೊರಹೋಗು);
}

/*
 *
 * NON UNIFYING COUNTER EXAMPLES
 *
 */

// Search node for BFS on state items
ರಚನೆ si_bfs_ಗಂಟು;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ si_bfs_ಗಂಟು
{
  state_ಅಂಶ_ಸಂಖ್ಯೆ si;
  ರಚನೆ si_bfs_ಗಂಟು *ಪೋಷಕ;
  ಪೂರ್ಣಾಂಕ reference_ಎಣಿಕೆ;
} si_bfs_ಗಂಟು;

ಸ್ಥಿರ si_bfs_ಗಂಟು *
si_bfs_new (state_ಅಂಶ_ಸಂಖ್ಯೆ si, si_bfs_ಗಂಟು *ಪೋಷಕ)
{
  si_bfs_ಗಂಟು *res = xcalloc (1, ಗಾತ್ರ *res);
  res->si = si;
  res->ಪೋಷಕ = ಪೋಷಕ;
  res->reference_ಎಣಿಕೆ = 1;
  ಆದರೆ (ಪೋಷಕ)
    ++ಪೋಷಕ->reference_ಎಣಿಕೆ;
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ bool
si_bfs_ಒಳಗೊಂಡಿದೆ (ಸ್ಥಿರಾಂಕ si_bfs_ಗಂಟು *n, state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ)
{
  ಗಾಗಿ (ಸ್ಥಿರಾಂಕ si_bfs_ಗಂಟು *ಹುಡುಕು = n; ಹುಡುಕು != ಖಾಲಿ; ಹುಡುಕು = ಹುಡುಕು->ಪೋಷಕ)
    ಆದರೆ (ಹುಡುಕು->si == ಜ್ಯಾ)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
si_bfs_free (si_bfs_ಗಂಟು *n)
{
  ಆದರೆ (n == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  --n->reference_ಎಣಿಕೆ;
  ಆದರೆ (n->reference_ಎಣಿಕೆ == 0)
    {
      si_bfs_free (n->ಪೋಷಕ);
      ಬಿಡುಗಡೆ (n);
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t si_bfs_ಗಂಟು_list;

/**
 * start is a state_item such that conflict_sym is an element of FIRSTS of the
 * nonterminal after the dot in start. Because of this, we should be able to
 * find a production item starting with conflict_sym by only searching productions
 * of the nonterminal and shifting over nullable nonterminals
 *
 * this returns the derivation of the productions that lead to conflict_sym
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ derivation_list
expand_ಗಮ್ಯ_conflict (state_ಅಂಶ_ಸಂಖ್ಯೆ ಆರಂಭ, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ conflict_sym)
{
  si_bfs_ಗಂಟು *init = si_bfs_new (ಆರಂಭ, ಖಾಲಿ);

  si_bfs_ಗಂಟು_list ಸರತಿ
    = gl_list_create (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ,
                      (gl_listelement_dispose_fn) si_bfs_free,
                      true, 1, (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ **) &init);
  si_bfs_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;
  // breadth-first search for a path of productions to the conflict symbol
  ಆಗ (gl_list_ಗಾತ್ರ (ಸರತಿ) > 0)
    {
      ಗಂಟು = (si_bfs_ಗಂಟು *) gl_list_get_at (ಸರತಿ, 0);
      state_ಅಂಶ *silast = &state_ವಸ್ತುಗಳು[ಗಂಟು->si];
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*silast->ಅಂಶ);
      ಆದರೆ (sym == conflict_sym)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (ISVAR (sym))
        {
          // add each production to the search
          bitset_iterator biter;
          state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
          bitset sib = silast->prods;
          BITSET_FOR_EACH (biter, sib, ಜ್ಯಾ, 0)
            {
              // ignore productions already in the path
              ಆದರೆ (si_bfs_ಒಳಗೊಂಡಿದೆ (ಗಂಟು, ಜ್ಯಾ))
                ಮುಂದುವರಿ;
              si_bfs_ಗಂಟು *ಮುಂದಿನ = si_bfs_new (ಜ್ಯಾ, ಗಂಟು);
              gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, ಮುಂದಿನ);
            }
          // for nullable nonterminals, add its goto to the search
          ಆದರೆ (nullable[sym - ntokens])
            {
              si_bfs_ಗಂಟು *ಮುಂದಿನ = si_bfs_new (silast->trans, ಗಂಟು);
              gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, ಮುಂದಿನ);
            }
        }
      gl_list_remove_at (ಸರತಿ, 0);
    }
  ಆದರೆ (gl_list_ಗಾತ್ರ (ಸರತಿ) == 0)
    {
      gl_list_free (ಸರತಿ);
      fputs ("Error expanding derivation\n", stderr);
      abort ();
    }

  derivation *dinit = derivation_new_ಎಲೆ (conflict_sym);
  derivation_list ಫಲಿತಾಂಶ = derivation_list_new ();
  derivation_list_append (ಫಲಿತಾಂಶ, dinit);
  // iterate backwards through the generated path to create a derivation
  // of the conflict symbol containing derivations of each production step.

  ಗಾಗಿ (si_bfs_ಗಂಟು *n = ಗಂಟು; n != ಖಾಲಿ; n = n->ಪೋಷಕ)
    {
      state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[n->si];
      ಅಂಶ_ಸಂಖ್ಯೆ *pos = si->ಅಂಶ;
      ಆದರೆ (SI_PRODUCTION (si))
        {
          ಅಂಶ_ಸಂಖ್ಯೆ *i = ಖಾಲಿ;
          ಗಾಗಿ (i = pos + 1; !ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*i); ++i)
            derivation_list_append (ಫಲಿತಾಂಶ, derivation_new_ಎಲೆ (*i));
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs =
            ನಿಯಮಗಳು[ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (*i)].lhs->ಸಂಖ್ಯೆ;
          derivation *deriv = derivation_new (lhs, ಫಲಿತಾಂಶ,
                                              state_ಅಂಶ_ನಿಯಮ (si));
          ಫಲಿತಾಂಶ = derivation_list_new ();
          derivation_list_append (ಫಲಿತಾಂಶ, deriv);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(pos - 1));
          derivation *deriv = derivation_new_ಎಲೆ (sym);
          derivation_list_prepend (ಫಲಿತಾಂಶ, deriv);
        }
    }
  gl_list_free (ಸರತಿ);
  derivation_free ((derivation*)gl_list_get_at (ಫಲಿತಾಂಶ, 0));
  gl_list_remove_at (ಫಲಿತಾಂಶ, 0);
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/**
 * Complete derivations for any pending productions in the given
 * sequence of state-items. For example, the input could be a path
 * of states that would give us the following input:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' •
 * So to complete the derivation of Stmt, we need an output like:
 * Stmt ::= [lval ::= [VAR] '=' e ::=[ e::=['0'] '+' • e ] ';' ]
 */
ಸ್ಥಿರ derivation *
complete_diverging_example (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ conflict_sym,
                            state_ಅಂಶ_list ಮಾರ್ಗ, derivation_list derivs)
{
  // The idea is to transfer each pending symbol on the productions
  // associated with the given StateItems to the resulting derivation.
  derivation_list ಫಲಿತಾಂಶ = derivation_list_new ();
  bool lookahead_required = false;
  ಆದರೆ (!derivs)
    {
      derivs = derivation_list_new ();
      gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಫಲಿತಾಂಶ, derivation_dot ());
      lookahead_required = true;
    }

  gl_list_ಗಂಟು_t deriv = list_get_ಕೊನೆ (derivs);

  // We go backwards through the path to create the derivation tree bottom-up.
  // Effectively this loops through each production once, and generates a
  // derivation of the left hand side by appending all of the rhs symbols.
  // this becomes the derivation of the nonterminal after the dot in the
  // next production, and all of the other symbols of the rule are added as normal.
  ಗಾಗಿ (gl_list_ಗಂಟು_t state_ಗಂಟು = list_get_ಕೊನೆ (ಮಾರ್ಗ);
       state_ಗಂಟು != ಖಾಲಿ;
       state_ಗಂಟು = gl_list_ಹಿಂದಿನ_ಗಂಟು (ಮಾರ್ಗ, state_ಗಂಟು))
    {
      state_ಅಂಶ *si = (state_ಅಂಶ *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (ಮಾರ್ಗ, state_ಗಂಟು);
      ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ = si->ಅಂಶ;
      ಅಂಶ_ಸಂಖ್ಯೆ pos = *ಅಂಶ;
      // symbols after dot
      ಆದರೆ (gl_list_ಗಾತ್ರ (ಫಲಿತಾಂಶ) == 1 && !ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (pos)
          && gl_list_get_at (ಫಲಿತಾಂಶ, 0) == derivation_dot ())
        {
          derivation_list_append (ಫಲಿತಾಂಶ,
            derivation_new_ಎಲೆ (ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (pos)));
          lookahead_required = false;
        }
      ಅಂಶ_ಸಂಖ್ಯೆ *i = ಅಂಶ;
      // go through each symbol after the dot in the current rule, and
      // add each symbol to its derivation.
      ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ nsi = si - state_ವಸ್ತುಗಳು;
           !ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*i);
           ++i, nsi = state_ವಸ್ತುಗಳು[nsi].trans)
        {
          // if the item is a reduction, we could skip to the wrong rule
          // by starting at i + 1, so this continue is necessary
          ಆದರೆ (i == ಅಂಶ)
            ಮುಂದುವರಿ;
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*i);
          ಆದರೆ (!lookahead_required || sym == conflict_sym)
            {
              derivation_list_append (ಫಲಿತಾಂಶ, derivation_new_ಎಲೆ (sym));
              lookahead_required = false;
              ಮುಂದುವರಿ;
            }
          // Since PATH is a path to the conflict state-item,
          // for a reduce conflict item, we will want to have a derivation
          // that shows the conflict symbol from its lookahead set being used.
          //
          // Since reductions have the dot at the end of the item,
          // this loop will be first executed on the last item in the path
          // that's not a reduction. When that happens,
          // the symbol after the dot should be a nonterminal,
          // and we can look through successive nullable nonterminals
          // for one with the conflict symbol in its first set.
          ಆದರೆ (bitset_test (FIRSTS (sym), conflict_sym))
            {
              lookahead_required = false;
              derivation_list ಮುಂದಿನ_derivs =
                expand_ಗಮ್ಯ_conflict (nsi, conflict_sym);
              derivation *d = ಖಾಲಿ;
              ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (ಮುಂದಿನ_derivs);
                   derivation_list_ಮುಂದಿನ (&it, &d);)
                derivation_list_append (ಫಲಿತಾಂಶ, d);
              i += gl_list_ಗಾತ್ರ (ಮುಂದಿನ_derivs) - 1;
              derivation_list_free (ಮುಂದಿನ_derivs);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nullable[sym - ntokens])
            {
              derivation *d = derivation_new_ಎಲೆ (sym);
              derivation_list_append (ಫಲಿತಾಂಶ, d);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              // We found a path to the conflict item, and despite it
              // having the conflict symbol in its lookahead, no example
              // containing the symbol after the conflict item
              // can be found.
              derivation_list_append (ಫಲಿತಾಂಶ, derivation_new_ಎಲೆ (1));
              lookahead_required = false;
            }
        }
      ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = &ನಿಯಮಗಳು[ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (*i)];
      // add derivations for symbols before dot
      ಗಾಗಿ (i = ಅಂಶ - 1; !ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*i) && i >= ritem; i--)
        {
          gl_list_ಗಂಟು_t p = gl_list_ಹಿಂದಿನ_ಗಂಟು (ಮಾರ್ಗ, state_ಗಂಟು);
          ಆದರೆ (p)
            state_ಗಂಟು = p;
          ಆದರೆ (deriv)
            {
              ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *tmp_deriv = gl_list_ಗಂಟು_ಮೌಲ್ಯ (derivs, deriv);
              deriv = gl_list_ಹಿಂದಿನ_ಗಂಟು (derivs, deriv);
              derivation_list_prepend (ಫಲಿತಾಂಶ, (derivation*)tmp_deriv);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            derivation_list_prepend (ಫಲಿತಾಂಶ, derivation_new_ಎಲೆ (*i));
        }
      // completing the derivation
      derivation *new_deriv = derivation_new (r->lhs->ಸಂಖ್ಯೆ, ಫಲಿತಾಂಶ, r);
      ಫಲಿತಾಂಶ = derivation_list_new ();
      derivation_list_append (ಫಲಿತಾಂಶ, new_deriv);
    }
  derivation *res = (derivation *) gl_list_get_at (ಫಲಿತಾಂಶ, 0);
  derivation_retain (res);
  derivation_list_free (ಫಲಿತಾಂಶ);
  derivation_list_free (derivs);
  ಹಿಂತಿರುಗಿ res;
}

/* Iterate backwards through the shifts of the path in the reduce
   conflict, and find a path of shifts from the shift conflict that
   goes through the same states. */
ಸ್ಥಿರ state_ಅಂಶ_list
nonunifying_ಸರಿಸು_ಮಾರ್ಗ (state_ಅಂಶ_list reduce_ಮಾರ್ಗ, state_ಅಂಶ *ಸರಿಸು_conflict)
{
  gl_list_ಗಂಟು_t tmp = gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (reduce_ಮಾರ್ಗ, ಖಾಲಿ);
  gl_list_ಗಂಟು_t ಮುಂದಿನ_ಗಂಟು = gl_list_ಹಿಂದಿನ_ಗಂಟು (reduce_ಮಾರ್ಗ, tmp);
  gl_list_ಗಂಟು_t ಗಂಟು = gl_list_ಹಿಂದಿನ_ಗಂಟು (reduce_ಮಾರ್ಗ, ಮುಂದಿನ_ಗಂಟು);
  gl_list_remove_ಗಂಟು (reduce_ಮಾರ್ಗ, tmp);
  state_ಅಂಶ *si = ಸರಿಸು_conflict;
  state_ಅಂಶ_list ಫಲಿತಾಂಶ =
    gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
  // FIXME: bool paths_merged;
  ಗಾಗಿ (; ಗಂಟು != ಖಾಲಿ; ಮುಂದಿನ_ಗಂಟು = ಗಂಟು,
       ಗಂಟು = gl_list_ಹಿಂದಿನ_ಗಂಟು (reduce_ಮಾರ್ಗ, ಗಂಟು))
    {
      state_ಅಂಶ *refsi =
        (state_ಅಂಶ *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (reduce_ಮಾರ್ಗ, ಗಂಟು);
      state_ಅಂಶ *nextrefsi =
        (state_ಅಂಶ *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (reduce_ಮಾರ್ಗ, ಮುಂದಿನ_ಗಂಟು);
      ಆದರೆ (nextrefsi == si)
        {
          gl_list_ಕೂಡಿಸು_ಮೊದಲ (ಫಲಿತಾಂಶ, refsi);
          si = refsi;
          ಮುಂದುವರಿ;
        }
      // skip reduction items
      ಆದರೆ (nextrefsi->ಅಂಶ != refsi->ಅಂಶ + 1 && refsi->ಅಂಶ != ritem)
        ಮುಂದುವರಿ;

      // bfs to find a shift to the right state
      si_bfs_ಗಂಟು *init = si_bfs_new (si - state_ವಸ್ತುಗಳು, ಖಾಲಿ);
      si_bfs_ಗಂಟು_list ಸರತಿ
        = gl_list_create (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ,
                          (gl_listelement_dispose_fn) si_bfs_free,
                          true, 1, (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ **) &init);
      si_bfs_ಗಂಟು *sis = ಖಾಲಿ;
      state_ಅಂಶ *prevsi = ಖಾಲಿ;
      ಆಗ (gl_list_ಗಾತ್ರ (ಸರತಿ) > 0)
        {
          sis = (si_bfs_ಗಂಟು *) gl_list_get_at (ಸರತಿ, 0);
          // if we end up in the start state, the shift couldn't be found.
          ಆದರೆ (sis->si == 0)
            ನಿಲ್ಲಿಸು;

          state_ಅಂಶ *ಹುಡುಕು_si = &state_ವಸ್ತುಗಳು[sis->si];
          // if the current state-item is a production item,
          // its reverse production items get added to the queue.
          // Otherwise, look for a reverse transition to the target state.
          bitset rsi = ಹುಡುಕು_si->revs;
          bitset_iterator biter;
          state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
          BITSET_FOR_EACH (biter, rsi, ಜ್ಯಾ, 0)
            {
              prevsi = &state_ವಸ್ತುಗಳು[ಜ್ಯಾ];
              ಆದರೆ (SI_TRANSITION (ಹುಡುಕು_si))
                {
                  ಆದರೆ (prevsi->state == refsi->state)
                    ಹೋಗು ಹುಡುಕು_ಕೊನೆ;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!si_bfs_ಒಳಗೊಂಡಿದೆ (sis, ಜ್ಯಾ))
                {
                  si_bfs_ಗಂಟು *prevsis = si_bfs_new (ಜ್ಯಾ, sis);
                  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, prevsis);
                }
            }
          gl_list_remove_at (ಸರತಿ, 0);
        }
    ಹುಡುಕು_ಕೊನೆ:
      // prepend path to shift we found
      ಆದರೆ (sis)
        {
          gl_list_ಗಂಟು_t ln = gl_list_ಕೂಡಿಸು_ಮೊದಲ (ಫಲಿತಾಂಶ, &state_ವಸ್ತುಗಳು[sis->si]);
          ಗಾಗಿ (si_bfs_ಗಂಟು *n = sis->ಪೋಷಕ; n; n = n->ಪೋಷಕ)
            ln = gl_list_ಕೂಡಿಸು_after (ಫಲಿತಾಂಶ, ln, &state_ವಸ್ತುಗಳು[n->si]);

        }
      si = prevsi;
      gl_list_free (ಸರತಿ);
    }
  ಆದರೆ (trace_flag & trace_cex)
    {
      fputs ("SHIFT ITEM PATH:\n", stderr);
      state_ಅಂಶ *sip = ಖಾಲಿ;
      ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (ಫಲಿತಾಂಶ);
           state_ಅಂಶ_list_ಮುಂದಿನ (&it, &sip);
           )
        state_ಅಂಶ_print (sip, stderr, "");
    }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}


/**
 * Construct a nonunifying counterexample from the shortest
 * lookahead-sensitive path.
 */
ಸ್ಥಿರ counterexample *
example_from_ಮಾರ್ಗ (bool ಸರಿಸು_reduce,
                   state_ಅಂಶ_ಸಂಖ್ಯೆ itm2,
                   state_ಅಂಶ_list ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym)
{
  derivation *deriv1 =
    complete_diverging_example (ಮುಂದಿನ_sym, ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ, ಖಾಲಿ);
  state_ಅಂಶ_list ಮಾರ್ಗ_2
    = ಸರಿಸು_reduce
    ? nonunifying_ಸರಿಸು_ಮಾರ್ಗ (ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ, &state_ವಸ್ತುಗಳು [itm2])
    : ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ_from_ಆರಂಭ (itm2, ಮುಂದಿನ_sym);
  derivation *deriv2 = complete_diverging_example (ಮುಂದಿನ_sym, ಮಾರ್ಗ_2, ಖಾಲಿ);
  gl_list_free (ಮಾರ್ಗ_2);
  ಹಿಂತಿರುಗಿ new_counterexample (deriv1, deriv2, ಸರಿಸು_reduce, false, true);
}

/*
 *
 * UNIFYING COUNTER EXAMPLES
 *
 */

/* A search state keeps track of two parser simulations,
 * one starting at each conflict. Complexity is a metric
 * which sums different parser actions with varying weights.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  parse_state *ಸ್ಥಿತಿಗಳು[2];
  ಪೂರ್ಣಾಂಕ complexity;
} ಹುಡುಕು_state;

ಸ್ಥಿರ ಹುಡುಕು_state *
initial_ಹುಡುಕು_state (state_ಅಂಶ *conflict1, state_ಅಂಶ *conflict2)
{
  ಹುಡುಕು_state *res = xmalloc (ಗಾತ್ರ *res);
  res->ಸ್ಥಿತಿಗಳು[0] = new_parse_state (conflict1);
  res->ಸ್ಥಿತಿಗಳು[1] = new_parse_state (conflict2);
  parse_state_retain (res->ಸ್ಥಿತಿಗಳು[0]);
  parse_state_retain (res->ಸ್ಥಿತಿಗಳು[1]);
  res->complexity = 0;
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಹುಡುಕು_state *
new_ಹುಡುಕು_state (parse_state *ps1, parse_state *ps2, ಪೂರ್ಣಾಂಕ complexity)
{
  ಹುಡುಕು_state *res = xmalloc (ಗಾತ್ರ *res);
  res->ಸ್ಥಿತಿಗಳು[0] = ps1;
  res->ಸ್ಥಿತಿಗಳು[1] = ps2;
  parse_state_retain (res->ಸ್ಥಿತಿಗಳು[0]);
  parse_state_retain (res->ಸ್ಥಿತಿಗಳು[1]);
  res->complexity = complexity;
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಹುಡುಕು_state *
copy_ಹುಡುಕು_state (ಹುಡುಕು_state *ಪೋಷಕ)
{
  ಹುಡುಕು_state *res = xmalloc (ಗಾತ್ರ *res);
  *res = *ಪೋಷಕ;
  parse_state_retain (res->ಸ್ಥಿತಿಗಳು[0]);
  parse_state_retain (res->ಸ್ಥಿತಿಗಳು[1]);
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_free_children (ಹುಡುಕು_state *ss)
{
  free_parse_state (ss->ಸ್ಥಿತಿಗಳು[0]);
  free_parse_state (ss->ಸ್ಥಿತಿಗಳು[1]);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_free (ಹುಡುಕು_state *ss)
{
  ಆದರೆ (ss == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  ಹುಡುಕು_state_free_children (ss);
  ಬಿಡುಗಡೆ (ss);
}

/* For debugging traces.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_print (ಹುಡುಕು_state *ss)
{
  fputs ("CONFLICT 1 ", stderr);
  print_parse_state (ss->ಸ್ಥಿತಿಗಳು[0]);
  fputs ("CONFLICT 2 ", stderr);
  print_parse_state (ss->ಸ್ಥಿತಿಗಳು[1]);
  putc ('\n', stderr);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t ಹುಡುಕು_state_list;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಹುಡುಕು_state_list_ಮುಂದಿನ (gl_list_iterator_t *it, ಹುಡುಕು_state **ss)
{
  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p = ಖಾಲಿ;
  bool res = gl_list_iterator_ಮುಂದಿನ (it, &p, ಖಾಲಿ);
  ಆದರೆ (res)
    *ss = (ಹುಡುಕು_state*) p;
  ಇಲ್ಲದಿದ್ದರೆ
    gl_list_iterator_free (it);
  ಹಿಂತಿರುಗಿ res;
}

/*
 * When a search state is copied, this is used to
 * directly set one of the parse states
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ss_ಗಣ_parse_state (ಹುಡುಕು_state *ss, ಪೂರ್ಣಾಂಕ idx, parse_state *ps)
{
  free_parse_state (ss->ಸ್ಥಿತಿಗಳು[idx]);
  ss->ಸ್ಥಿತಿಗಳು[idx] = ps;
  parse_state_retain (ps);
}

/*
 * Construct a nonunifying example from a search state
 * which has its parse states unified at the beginning
 * but not the end of the example.
 */
ಸ್ಥಿರ counterexample *
complete_diverging_examples (ಹುಡುಕು_state *ss,
                             ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym,
                             bool ಸರಿಸು_reduce)
{
  derivation *new_derivs[2];
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; ++i)
    {
      state_ಅಂಶ_list sitems;
      derivation_list derivs;
      parse_state_lists (ss->ಸ್ಥಿತಿಗಳು[i], &sitems, &derivs);
      new_derivs[i] = complete_diverging_example (ಮುಂದಿನ_sym, sitems, derivs);
      gl_list_free (sitems);
    }
  ಹಿಂತಿರುಗಿ new_counterexample (new_derivs[0], new_derivs[1],
                             ಸರಿಸು_reduce, false, true);
}

/*
 * Search states are stored in bundles with those that
 * share the same complexity. This is so the priority
 * queue takes less overhead.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಹುಡುಕು_state_list ಸ್ಥಿತಿಗಳು;
  ಪೂರ್ಣಾಂಕ complexity;
} ಹುಡುಕು_state_bundle;

ಸ್ಥಿರ ಶೂನ್ಯ
ssb_free (ಹುಡುಕು_state_bundle *ssb)
{
  gl_list_free (ssb->ಸ್ಥಿತಿಗಳು);
  ಬಿಡುಗಡೆ (ssb);
}

ಸ್ಥಿರ size_t
ssb_hasher (ಹುಡುಕು_state_bundle *ssb)
{
  ಹಿಂತಿರುಗಿ ssb->complexity;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ssb_comp (ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state_bundle *s1, ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state_bundle *s2)
{
  ಹಿಂತಿರುಗಿ s1->complexity - s2->complexity;
}

ಸ್ಥಿರ bool
ssb_equals (ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state_bundle *s1, ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state_bundle *s2)
{
  ಹಿಂತಿರುಗಿ s1->complexity == s2->complexity;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t ssb_list;

ಸ್ಥಿರ size_t
ಭೇಟಿಯಾದ_hasher (ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state *ss, size_t max)
{
  ಹಿಂತಿರುಗಿ (parse_state_hasher (ss->ಸ್ಥಿತಿಗಳು[0], max)
          + parse_state_hasher (ss->ಸ್ಥಿತಿಗಳು[1], max)) % max;
}

ಸ್ಥಿರ bool
ಭೇಟಿಯಾದ_comparator (ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state *ss1, ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state *ss2)
{
  ಹಿಂತಿರುಗಿ parse_state_comparator (ss1->ಸ್ಥಿತಿಗಳು[0], ss2->ಸ್ಥಿತಿಗಳು[0])
    && parse_state_comparator (ss1->ಸ್ಥಿತಿಗಳು[1], ss2->ಸ್ಥಿತಿಗಳು[1]);
}

/* Priority queue for search states with minimal complexity. */
ಸ್ಥಿರ ssb_list ssb_ಸರತಿ;
ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *ಭೇಟಿಯಾದ;
/* The set of parser states on the shortest lookahead-sensitive path. */
ಸ್ಥಿರ bitset scp_ಗಣ = ಖಾಲಿ;
/* The set of parser states used for the conflict reduction rule. */
ಸ್ಥಿರ bitset rpp_ಗಣ = ಖಾಲಿ;

ಸ್ಥಿರ ಶೂನ್ಯ
ssb_append (ಹುಡುಕು_state *ss)
{
  ಆದರೆ (hash_lookup (ಭೇಟಿಯಾದ, ss))
    {
      ಹುಡುಕು_state_free (ss);
      ಹಿಂತಿರುಗಿ;
    }
  hash_xinsert (ಭೇಟಿಯಾದ, ss);
  // if states are only referenced by the visited set,
  // their contents should be freed as we only need
  // the metadata necessary to compute a hash.
  parse_state_free_contents_early (ss->ಸ್ಥಿತಿಗಳು[0]);
  parse_state_free_contents_early (ss->ಸ್ಥಿತಿಗಳು[1]);
  parse_state_retain (ss->ಸ್ಥಿತಿಗಳು[0]);
  parse_state_retain (ss->ಸ್ಥಿತಿಗಳು[1]);
  ಹುಡುಕು_state_bundle *ssb = xmalloc (ಗಾತ್ರ *ssb);
  ssb->complexity = ss->complexity;
  gl_list_ಗಂಟು_t n = gl_list_ಹುಡುಕು (ssb_ಸರತಿ, ssb);
  ಆದರೆ (!n)
    {
      ssb->ಸ್ಥಿತಿಗಳು =
        gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ,
                              (gl_listelement_dispose_fn)ಹುಡುಕು_state_free_children,
                              true);
      gl_sortedlist_ಕೂಡಿಸು (ssb_ಸರತಿ, (gl_listelement_compar_fn) ssb_comp, ssb);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬಿಡುಗಡೆ (ssb);
      ssb = (ಹುಡುಕು_state_bundle *) gl_list_ಗಂಟು_ಮೌಲ್ಯ (ssb_ಸರತಿ, n);
    }
  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ssb->ಸ್ಥಿತಿಗಳು, ss);
}

/*
 * The following functions perform various actions on parse states
 * and assign complexities to the newly generated search states.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
production_ಹೆಜ್ಜೆ (ಹುಡುಕು_state *ss, ಪೂರ್ಣಾಂಕ parser_state)
{
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *other_si = parse_state_ಬಾಲ (ss->ಸ್ಥಿತಿಗಳು[1 - parser_state]);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ other_sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*other_si->ಅಂಶ);
  parse_state_list prods =
    simulate_production (ss->ಸ್ಥಿತಿಗಳು[parser_state], other_sym);
  ಪೂರ್ಣಾಂಕ complexity = ss->complexity + PRODUCTION_ವೆಚ್ಚ;

  parse_state *ps = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (prods);
       parse_state_list_ಮುಂದಿನ (&it, &ps);
       )
    {
      ಹುಡುಕು_state *copy = copy_ಹುಡುಕು_state (ss);
      ss_ಗಣ_parse_state (copy, parser_state, ps);
      copy->complexity = complexity;
      ssb_append (copy);
    }
  gl_list_free (prods);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
reduction_ವೆಚ್ಚ (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಪೂರ್ಣಾಂಕ shifts;
  ಪೂರ್ಣಾಂಕ productions;
  parse_state_completed_ಹೆಜ್ಜೆಗಳು (ps, &shifts, &productions);
  ಹಿಂತಿರುಗಿ ಸರಿಸು_ವೆಚ್ಚ * shifts + PRODUCTION_ವೆಚ್ಚ * productions;
}

ಸ್ಥಿರ ಹುಡುಕು_state_list
reduction_ಹೆಜ್ಜೆ (ಹುಡುಕು_state *ss, ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *conflict_ಅಂಶ,
                ಪೂರ್ಣಾಂಕ parser_state, ಪೂರ್ಣಾಂಕ ನಿಯಮ_len)
{
  (ಶೂನ್ಯ) conflict_ಅಂಶ; // FIXME: Unused
  ಹುಡುಕು_state_list ಫಲಿತಾಂಶ =
    gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, 1);

  parse_state *ps = ss->ಸ್ಥಿತಿಗಳು[parser_state];
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si = parse_state_ಬಾಲ (ps);
  bitset ಚಿಹ್ನೆ_ಗಣ = si->lookahead;
  parse_state *other = ss->ಸ್ಥಿತಿಗಳು[1 - parser_state];
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *other_si = parse_state_ಬಾಲ (other);
  // if the other state can transition on a symbol,
  // the reduction needs to have that symbol in its lookahead
  ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*other_si->ಅಂಶ))
    {
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ other_sym =
        ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*other_si->ಅಂಶ);
      ಆದರೆ (!intersect_ಚಿಹ್ನೆ (other_sym, ಚಿಹ್ನೆ_ಗಣ))
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
      ಚಿಹ್ನೆ_ಗಣ = bitset_create (nsyms, BITSET_FIXED);
      bitset_ಗಣ (ಚಿಹ್ನೆ_ಗಣ, other_sym);
    }

  // FIXME: search_state *new_root = copy_search_state (ss);
  parse_state_list reduced =
    simulate_reduction (ps, ನಿಯಮ_len, ಚಿಹ್ನೆ_ಗಣ);
  parse_state *reduced_ps = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (reduced);
       parse_state_list_ಮುಂದಿನ (&it, &reduced_ps);
       )
    {
      ಹುಡುಕು_state *copy = copy_ಹುಡುಕು_state (ss);
      ss_ಗಣ_parse_state (copy, parser_state, reduced_ps);
      ಪೂರ್ಣಾಂಕ r_ವೆಚ್ಚ = reduction_ವೆಚ್ಚ (reduced_ps);
      copy->complexity += r_ವೆಚ್ಚ + PRODUCTION_ವೆಚ್ಚ + 2 * ಸರಿಸು_ವೆಚ್ಚ;
      gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಫಲಿತಾಂಶ, copy);
    }
  gl_list_free (reduced);
  ಆದರೆ (ಚಿಹ್ನೆ_ಗಣ != si->lookahead)
    bitset_free (ಚಿಹ್ನೆ_ಗಣ);
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/**
 * Attempt to prepend the given symbol to this search state, respecting
 * the given subsequent next symbol on each path. If a reverse transition
 * cannot be made on both states, possible reverse productions are prepended
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ಹುಡುಕು_state_prepend (ಹುಡುಕು_state *ss, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, bitset guide)
{
  (ಶೂನ್ಯ) sym; // FIXME: Unused.
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si1src = parse_state_ತಲೆ (ss->ಸ್ಥಿತಿಗಳು[0]);
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si2src = parse_state_ತಲೆ (ss->ಸ್ಥಿತಿಗಳು[1]);

  bool prod1 = SI_PRODUCTION (si1src);
  // If one can make a reverse transition and the other can't, only apply
  // the reverse productions that the other state can make in an attempt to
  // make progress.
  ಆದರೆ (prod1 != SI_PRODUCTION (si2src))
    {
      ಪೂರ್ಣಾಂಕ prod_state = prod1 ? 0 : 1;
      parse_state_list prev = parser_prepend (ss->ಸ್ಥಿತಿಗಳು[prod_state]);
      parse_state *ps = ಖಾಲಿ;
      ಗಾಗಿ (gl_list_iterator_t iter = gl_list_iterator (prev);
           parse_state_list_ಮುಂದಿನ (&iter, &ps);
           )
        {
          ಸ್ಥಿರಾಂಕ state_ಅಂಶ *psi = parse_state_ತಲೆ (ps);
          bool guided = bitset_test (guide, psi->state->ಸಂಖ್ಯೆ);
          ಆದರೆ (!guided && !EXTENDED_ಹುಡುಕು)
            ಮುಂದುವರಿ;

          ಹುಡುಕು_state *copy = copy_ಹುಡುಕು_state (ss);
          ss_ಗಣ_parse_state (copy, prod_state, ps);
          copy->complexity += PRODUCTION_ವೆಚ್ಚ;
          ಆದರೆ (!guided)
            copy->complexity += EXTENDED_ವೆಚ್ಚ;
          ssb_append (copy);
        }
      gl_list_free (prev);
      ಹಿಂತಿರುಗಿ;
    }

  // The parse state heads are either both production items or both
  // transition items. So all prepend options will either be
  // reverse transitions or reverse productions
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ complexity_ವೆಚ್ಚ = 2 * (prod1 ? PRODUCTION_ವೆಚ್ಚ : UNSHIFT_ವೆಚ್ಚ);

  parse_state_list prev1 = parser_prepend (ss->ಸ್ಥಿತಿಗಳು[0]);
  parse_state_list prev2 = parser_prepend (ss->ಸ್ಥಿತಿಗಳು[1]);

  // loop through each pair of possible prepend states and append search
  // states for each pair where the parser states correspond to the same
  // parsed input.
  parse_state *ps1 = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t iter1 = gl_list_iterator (prev1);
       parse_state_list_ಮುಂದಿನ (&iter1, &ps1);
       )
    {
      ಸ್ಥಿರಾಂಕ state_ಅಂಶ *psi1 = parse_state_ತಲೆ (ps1);
      bool guided1 = bitset_test (guide, psi1->state->ಸಂಖ್ಯೆ);
      ಆದರೆ (!guided1 && !EXTENDED_ಹುಡುಕು)
        ಮುಂದುವರಿ;

      parse_state *ps2 = ಖಾಲಿ;
      ಗಾಗಿ (gl_list_iterator_t iter2 = gl_list_iterator (prev2);
           parse_state_list_ಮುಂದಿನ (&iter2, &ps2);
           )
        {
          ಸ್ಥಿರಾಂಕ state_ಅಂಶ *psi2 = parse_state_ತಲೆ (ps2);

          bool guided2 = bitset_test (guide, psi2->state->ಸಂಖ್ಯೆ);
          ಆದರೆ (!guided2 && !EXTENDED_ಹುಡುಕು)
            ಮುಂದುವರಿ;
          // Only consider prepend state items that share the same state.
          ಆದರೆ (psi1->state != psi2->state)
            ಮುಂದುವರಿ;

          ಪೂರ್ಣಾಂಕ complexity = ss->complexity + complexity_ವೆಚ್ಚ;
          // penalty for not being along the guide path
          ಆದರೆ (!guided1 || !guided2)
            complexity += EXTENDED_ವೆಚ್ಚ;
          ssb_append (new_ಹುಡುಕು_state (ps1, ps2, complexity));
        }
    }
  gl_list_free (prev1);
  gl_list_free (prev2);
}

/**
 * Determine if the productions associated with the given parser items have
 * the same prefix up to the dot.
 */
ಸ್ಥಿರ bool
have_common_ಪೂರ್ವಪ್ರತ್ಯಯ (ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *itm1, ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *itm2)
{
  ಪೂರ್ಣಾಂಕ i = 0;
  ಗಾಗಿ (; !ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (itm1[i]); ++i)
    ಆದರೆ (itm1[i] != itm2[i])
      ಹಿಂತಿರುಗಿ false;
  ಹಿಂತಿರುಗಿ ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (itm2[i]);
}

/*
 * The start and end locations of an item in ritem.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *
ಅಂಶ_ನಿಯಮ_ಆರಂಭ (ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ)
{
  ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *res = ಖಾಲಿ;
  ಗಾಗಿ (res = ಅಂಶ;
       ritem < res && ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(res - 1));
       --res)
    ಮುಂದುವರಿ;
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *
ಅಂಶ_ನಿಯಮ_ಕೊನೆ (ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *ಅಂಶ)
{
  ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *res = ಖಾಲಿ;
  ಗಾಗಿ (res = ಅಂಶ; ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*res); ++res)
    ಮುಂದುವರಿ;
  ಹಿಂತಿರುಗಿ res;
}

/*
 * Perform the appropriate possible parser actions
 * on a search state and add the results to the
 * search state priority queue.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
generate_ಮುಂದಿನ_ಸ್ಥಿತಿಗಳು (ಹುಡುಕು_state *ss, state_ಅಂಶ *conflict1,
                      state_ಅಂಶ *conflict2)
{
  // Compute the successor configurations.
  parse_state *ps1 = ss->ಸ್ಥಿತಿಗಳು[0];
  parse_state *ps2 = ss->ಸ್ಥಿತಿಗಳು[1];
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si1 = parse_state_ಬಾಲ (ps1);
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si2 = parse_state_ಬಾಲ (ps2);
  bool si1reduce = ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*si1->ಅಂಶ);
  bool si2reduce = ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*si2->ಅಂಶ);
  ಆದರೆ (!si1reduce && !si2reduce)
    {
      // Transition if both paths end at the same symbol
      ಆದರೆ (*si1->ಅಂಶ == *si2->ಅಂಶ)
        {
          ಪೂರ್ಣಾಂಕ complexity = ss->complexity + 2 * ಸರಿಸು_ವೆಚ್ಚ;
          parse_state_list trans1 = simulate_transition (ps1);
          parse_state_list trans2 = simulate_transition (ps2);
          parse_state *tps1 = ಖಾಲಿ;
          parse_state *tps2 = ಖಾಲಿ;
          ಗಾಗಿ (gl_list_iterator_t it1 = gl_list_iterator (trans1);
               parse_state_list_ಮುಂದಿನ (&it1, &tps1);
               )
            ಗಾಗಿ (gl_list_iterator_t it2 = gl_list_iterator (trans2);
                 parse_state_list_ಮುಂದಿನ (&it2, &tps2);
                 )
              ssb_append (new_ಹುಡುಕು_state (tps1, tps2, complexity));
          gl_list_free (trans1);
          gl_list_free (trans2);
        }

      // Take production steps if possible.
      production_ಹೆಜ್ಜೆ (ss, 0);
      production_ಹೆಜ್ಜೆ (ss, 1);
    }
  // One of the states requires a reduction
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *rhs1 = ಅಂಶ_ನಿಯಮ_ಆರಂಭ (si1->ಅಂಶ);
      ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *rhe1 = ಅಂಶ_ನಿಯಮ_ಕೊನೆ (si1->ಅಂಶ);
      ಪೂರ್ಣಾಂಕ len1 = rhe1 - rhs1;
      ಪೂರ್ಣಾಂಕ size1 = parse_state_ಉದ್ದಳತೆ (ps1);
      bool ready1 = si1reduce && len1 < size1;

      ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *rhs2 = ಅಂಶ_ನಿಯಮ_ಆರಂಭ (si2->ಅಂಶ);
      ಸ್ಥಿರಾಂಕ ಅಂಶ_ಸಂಖ್ಯೆ *rhe2 = ಅಂಶ_ನಿಯಮ_ಕೊನೆ (si2->ಅಂಶ);
      ಪೂರ್ಣಾಂಕ len2 = rhe2 - rhs2;
      ಪೂರ್ಣಾಂಕ size2 = parse_state_ಉದ್ದಳತೆ (ps2);
      bool ready2 = si2reduce && len2 < size2;
      // If there is a path ready for reduction without being
      // prepended further, reduce.
      ಆದರೆ (ready1 && ready2)
        {
          ಹುಡುಕು_state_list reduced1 = reduction_ಹೆಜ್ಜೆ (ss, conflict1->ಅಂಶ, 0, len1);
          gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (reduced1, ss);
          ಹುಡುಕು_state *red1 = ಖಾಲಿ;
          ಗಾಗಿ (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ಹುಡುಕು_state_list_ಮುಂದಿನ (&iter, &red1);
               )
            {
              ಹುಡುಕು_state_list reduced2 =
                reduction_ಹೆಜ್ಜೆ (red1, conflict2->ಅಂಶ, 1, len2);
              ಹುಡುಕು_state *red2 = ಖಾಲಿ;
              ಗಾಗಿ (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
                   ಹುಡುಕು_state_list_ಮುಂದಿನ (&iter2, &red2);
                   )
                ssb_append (red2);
              // Avoid duplicates.
              ಆದರೆ (red1 != ss)
                ssb_append (red1);
              gl_list_free (reduced2);
            }
          gl_list_free (reduced1);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ready1)
        {
          ಹುಡುಕು_state_list reduced1 = reduction_ಹೆಜ್ಜೆ (ss, conflict1->ಅಂಶ, 0, len1);
          ಹುಡುಕು_state *red1 = ಖಾಲಿ;
          ಗಾಗಿ (gl_list_iterator_t iter = gl_list_iterator (reduced1);
               ಹುಡುಕು_state_list_ಮುಂದಿನ (&iter, &red1);
               )
            ssb_append (red1);
          gl_list_free (reduced1);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ready2)
        {
          ಹುಡುಕು_state_list reduced2 = reduction_ಹೆಜ್ಜೆ (ss, conflict2->ಅಂಶ, 1, len2);
          ಹುಡುಕು_state *red2 = ಖಾಲಿ;
          ಗಾಗಿ (gl_list_iterator_t iter2 = gl_list_iterator (reduced2);
               ಹುಡುಕು_state_list_ಮುಂದಿನ (&iter2, &red2);
               )
            ssb_append (red2);
          gl_list_free (reduced2);
        }
      /* Both states end with a reduction, yet they don't have enough symbols
       * to reduce. This means symbols are missing from the beginning of the
       * rule, so we must prepend */
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym
            = si1reduce && !ready1
            ? *(rhe1 - size1)
            : *(rhe2 - size2);
          ಹುಡುಕು_state_prepend (ss, sym,
                                parse_state_ಆಳ (ss->ಸ್ಥಿತಿಗಳು[0]) >= 0
                                ? rpp_ಗಣ : scp_ಗಣ);
        }
    }
}

/*
 * Perform the actual counterexample search,
 * keeps track of what stage of the search algorithm
 * we are at and gives the appropriate counterexample
 * type based off of time constraints.
 */
ಸ್ಥಿರ counterexample *
unifying_example (state_ಅಂಶ_ಸಂಖ್ಯೆ itm1,
                  state_ಅಂಶ_ಸಂಖ್ಯೆ itm2,
                  bool ಸರಿಸು_reduce,
                  state_ಅಂಶ_list reduce_ಮಾರ್ಗ, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym)
{
  state_ಅಂಶ *conflict1 = &state_ವಸ್ತುಗಳು[itm1];
  state_ಅಂಶ *conflict2 = &state_ವಸ್ತುಗಳು[itm2];
  ಹುಡುಕು_state *initial = initial_ಹುಡುಕು_state (conflict1, conflict2);
  ssb_ಸರತಿ = gl_list_create_ಖಾಲಿಗಳು (GL_RBTREEHASH_LIST,
                                    (gl_listelement_equals_fn) ssb_equals,
                                    (gl_listelement_hashcode_fn) ssb_hasher,
                                    (gl_listelement_dispose_fn) ssb_free,
                                    false);
  ಭೇಟಿಯಾದ =
    hash_initialize (32, ಖಾಲಿ, (Hash_hasher) ಭೇಟಿಯಾದ_hasher,
                     (Hash_comparator) ಭೇಟಿಯಾದ_comparator,
                     (Hash_ದತ್ತಾಂಶ_freer) ಹುಡುಕು_state_free);
  ssb_append (initial);
  xtime_t ಆರಂಭ = gethrxtime ();
  bool assurance_printed = false;
  ಹುಡುಕು_state *stage3result = ಖಾಲಿ;
  counterexample *cex = ಖಾಲಿ;
  ಆಗ (gl_list_ಗಾತ್ರ (ssb_ಸರತಿ) > 0)
    {
      ಸ್ಥಿರಾಂಕ ಹುಡುಕು_state_bundle *ssb = gl_list_get_at (ssb_ಸರತಿ, 0);

      ಹುಡುಕು_state *ss = ಖಾಲಿ;
      ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (ssb->ಸ್ಥಿತಿಗಳು);
           ಹುಡುಕು_state_list_ಮುಂದಿನ (&it, &ss);
           )
        {
          ಆದರೆ (trace_flag & trace_cex)
            ಹುಡುಕು_state_print (ss);
          // Stage 1/2 completing the rules containing the conflicts
          parse_state *ps1 = ss->ಸ್ಥಿತಿಗಳು[0];
          parse_state *ps2 = ss->ಸ್ಥಿತಿಗಳು[1];
          ಆದರೆ (parse_state_ಆಳ (ps1) < 0 && parse_state_ಆಳ (ps2) < 0)
            {
              // Stage 3: reduce and shift conflict items completed.
              ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si1src = parse_state_ತಲೆ (ps1);
              ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si2src = parse_state_ತಲೆ (ps2);
              ಆದರೆ (ಅಂಶ_ನಿಯಮ (si1src->ಅಂಶ)->lhs == ಅಂಶ_ನಿಯಮ (si2src->ಅಂಶ)->lhs
                  && have_common_ಪೂರ್ವಪ್ರತ್ಯಯ (si1src->ಅಂಶ, si2src->ಅಂಶ))
                {
                  // Stage 4: both paths share a prefix
                  derivation *d1 = parse_state_derivation (ps1);
                  derivation *d2 = parse_state_derivation (ps2);
                  ಆದರೆ (parse_state_derivation_completed (ps1)
                      && parse_state_derivation_completed (ps2))
                    {
                      // Once we have two derivations for the same symbol,
                      // we've found a unifying counterexample.
                      cex = new_counterexample (d1, d2, ಸರಿಸು_reduce, true, false);
                      derivation_retain (d1);
                      derivation_retain (d2);
                      ಹೋಗು cex_ಹುಡುಕು_ಕೊನೆ;
                    }
                  ಆದರೆ (!stage3result)
                    stage3result = copy_ಹುಡುಕು_state (ss);
                }
            }
          ಆದರೆ (ಸಮಯ_ಎಲ್ಲೆ_ENFORCED)
            {
              ದ್ವಿಗುಣ ಸಮಯ_passed = (gethrxtime () - ಆರಂಭ) / 1e9;
              ಆದರೆ (!assurance_printed && ಸಮಯ_passed > ASSURANCE_ಎಲ್ಲೆ
                  && stage3result)
                {
                  fputs ("Productions leading up to the conflict state found.  "
                         "Still searching for a possible unifying counterexample...",
                         stderr);
                  assurance_printed = true;
                }
              ಆದರೆ (ಸಮಯ_passed > ಸಮಯ_ಎಲ್ಲೆ)
                {
                  fprintf (stderr, "time limit exceeded: %f\n", ಸಮಯ_passed);
                  ಹೋಗು cex_ಹುಡುಕು_ಕೊನೆ;
                }
            }
          generate_ಮುಂದಿನ_ಸ್ಥಿತಿಗಳು (ss, conflict1, conflict2);
        }
      gl_sortedlist_remove (ssb_ಸರತಿ,
                            (gl_listelement_compar_fn) ssb_comp, ssb);
    }
cex_ಹುಡುಕು_ಕೊನೆ:;
  ಆದರೆ (!cex)
    {
      // No unifying counterexamples
      // If a search state from Stage 3 is available, use it
      // to construct a more compact nonunifying counterexample.
      ಆದರೆ (stage3result)
        cex = complete_diverging_examples (stage3result, ಮುಂದಿನ_sym, ಸರಿಸು_reduce);
      // Otherwise, construct a nonunifying counterexample that
      // begins from the start state using the shortest
      // lookahead-sensitive path to the reduce item.
      ಇಲ್ಲದಿದ್ದರೆ
        cex = example_from_ಮಾರ್ಗ (ಸರಿಸು_reduce, itm2, reduce_ಮಾರ್ಗ, ಮುಂದಿನ_sym);
    }
  gl_list_free (ssb_ಸರತಿ);
  hash_free (ಭೇಟಿಯಾದ);
  ಆದರೆ (stage3result)
    ಹುಡುಕು_state_free (stage3result);
  ಹಿಂತಿರುಗಿ cex;
}

ಸ್ಥಿರ xtime_t cumulative_ಸಮಯ;

ಶೂನ್ಯ
counterexample_init (ಶೂನ್ಯ)
{
  // Check cex.timeout.
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *variable = "cex.timeout";
    ಅಕ್ಷರ *cp = muscle_percent_define_get (variable);
    ಆದರೆ (*cp != '\0')
      {
        ಅಕ್ಷರ *ಕೊನೆ = ಖಾಲಿ;
        ದ್ವಿಗುಣ v = c_strtod (cp, &ಕೊನೆ);
        ಆದರೆ (*ಕೊನೆ == '\0' && errno == 0 && v >= 0)
          ಸಮಯ_ಎಲ್ಲೆ = v;
        ಇಲ್ಲದಿದ್ದರೆ
          {
            location loc = muscle_percent_define_get_loc (variable);
            complain (&loc, complaint,
                      _("invalid value for %%define variable %s: %s"),
                      quote (variable), quote_n (1, cp));
          }
      }
    ಬಿಡುಗಡೆ (cp);
  }
  cumulative_ಸಮಯ = gethrxtime ();
  scp_ಗಣ = bitset_create (nstates, BITSET_FIXED);
  rpp_ಗಣ = bitset_create (nstates, BITSET_FIXED);
  state_ವಸ್ತುಗಳು_init ();
}


ಶೂನ್ಯ
counterexample_free (ಶೂನ್ಯ)
{
  ಆದರೆ (scp_ಗಣ)
    {
      bitset_free (scp_ಗಣ);
      bitset_free (rpp_ಗಣ);
      state_ವಸ್ತುಗಳು_free ();
    }
}

/**
 * Report a counterexample for conflict on symbol next_sym
 * between the given state-items
 */
ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_report (state_ಅಂಶ_ಸಂಖ್ಯೆ itm1, state_ಅಂಶ_ಸಂಖ್ಯೆ itm2,
                       ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym, bool ಸರಿಸು_reduce,
                       FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  // Compute the shortest lookahead-sensitive path and associated sets of
  // parser states.
  state_ಅಂಶ_list ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ = ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ_from_ಆರಂಭ (itm1, ಮುಂದಿನ_sym);
  bool reduce_prod_reached = false;
  ಸ್ಥಿರಾಂಕ ನಿಯಮ *reduce_ನಿಯಮ = ಅಂಶ_ನಿಯಮ (state_ವಸ್ತುಗಳು[itm1].ಅಂಶ);

  bitset_zero (scp_ಗಣ);
  bitset_zero (rpp_ಗಣ);

  state_ಅಂಶ *si = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ);
       state_ಅಂಶ_list_ಮುಂದಿನ (&it, &si);
       )
    {
      bitset_ಗಣ (scp_ಗಣ, si->state->ಸಂಖ್ಯೆ);
      reduce_prod_reached = reduce_prod_reached
                          || ಅಂಶ_ನಿಯಮ (si->ಅಂಶ) == reduce_ನಿಯಮ;
      ಆದರೆ (reduce_prod_reached)
        bitset_ಗಣ (rpp_ಗಣ, si->state->ಸಂಖ್ಯೆ);
    }
  counterexample *cex
    = (gethrxtime () - cumulative_ಸಮಯ) / 1e9 < CUMULATIVE_ಸಮಯ_ಎಲ್ಲೆ
    ? unifying_example (itm1, itm2, ಸರಿಸು_reduce, ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ, ಮುಂದಿನ_sym)
    : example_from_ಮಾರ್ಗ (ಸರಿಸು_reduce, itm2, ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ, ಮುಂದಿನ_sym);

  gl_list_free (ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ);
  counterexample_print (cex, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
  free_counterexample (cex);
}


// ITM1 denotes a shift, ITM2 a reduce.
ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_report_ಸರಿಸು_reduce (state_ಅಂಶ_ಸಂಖ್ಯೆ itm1, state_ಅಂಶ_ಸಂಖ್ಯೆ itm2,
                                    ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym,
                                    FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  ಆದರೆ (ಹೊರಹೋಗು == stderr)
    complain (ಖಾಲಿ, Wcounterexamples,
              _("shift/reduce conflict on token %s"), symbols[ಮುಂದಿನ_sym]->ಟ್ಯಾಗ್);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
      fprintf (ಹೊರಹೋಗು, _("shift/reduce conflict on token %s"), symbols[ಮುಂದಿನ_sym]->ಟ್ಯಾಗ್);
      fprintf (ಹೊರಹೋಗು, "%s\n", _(":"));
    }
  // In the report, print the items.
  ಆದರೆ (ಹೊರಹೋಗು != stderr || trace_flag & trace_cex)
    {
      state_ಅಂಶ_print (&state_ವಸ್ತುಗಳು[itm1], ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
      state_ಅಂಶ_print (&state_ವಸ್ತುಗಳು[itm2], ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
    }
  counterexample_report (itm1, itm2, ಮುಂದಿನ_sym, true, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
counterexample_report_reduce_reduce (state_ಅಂಶ_ಸಂಖ್ಯೆ itm1, state_ಅಂಶ_ಸಂಖ್ಯೆ itm2,
                                     bitset conflict_syms,
                                     FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  {
    ರಚನೆ obstack obstack;
    obstack_init (&obstack);
    bitset_iterator biter;
    state_ಅಂಶ_ಸಂಖ್ಯೆ sym;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sep = "";
    BITSET_FOR_EACH (biter, conflict_syms, sym, 0)
      {
        obstack_printf (&obstack, "%s%s", sep, symbols[sym]->ಟ್ಯಾಗ್);
        sep = ", ";
      }
    ಅಕ್ಷರ *ಟೋಕನುಗಳು = obstack_finish0 (&obstack);
    ಆದರೆ (ಹೊರಹೋಗು == stderr)
      complain (ಖಾಲಿ, Wcounterexamples,
                ngettext ("reduce/reduce conflict on token %s",
                          "reduce/reduce conflict on tokens %s",
                          bitset_ಎಣಿಕೆ (conflict_syms)),
                ಟೋಕನುಗಳು);
    ಇಲ್ಲದಿದ್ದರೆ
      {
        fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, ಹೊರಹೋಗು);
        fprintf (ಹೊರಹೋಗು,
                 ngettext ("reduce/reduce conflict on token %s",
                           "reduce/reduce conflict on tokens %s",
                           bitset_ಎಣಿಕೆ (conflict_syms)),
                 ಟೋಕನುಗಳು);
        fprintf (ಹೊರಹೋಗು, "%s\n", _(":"));
      }
    obstack_free (&obstack, ಖಾಲಿ);
  }
  // In the report, print the items.
  ಆದರೆ (ಹೊರಹೋಗು != stderr || trace_flag & trace_cex)
    {
      state_ಅಂಶ_print (&state_ವಸ್ತುಗಳು[itm1], ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
      state_ಅಂಶ_print (&state_ವಸ್ತುಗಳು[itm2], ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
    }
  counterexample_report (itm1, itm2, bitset_ಮೊದಲ (conflict_syms),
                         false, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
}

ಸ್ಥಿರ state_ಅಂಶ_ಸಂಖ್ಯೆ
ಕಂಡುಹಿಡಿ_state_ಅಂಶ_ಸಂಖ್ಯೆ (ಸ್ಥಿರಾಂಕ ನಿಯಮ *r, state_ಸಂಖ್ಯೆ sn)
{
  ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ i = state_ಅಂಶ_map[sn]; i < state_ಅಂಶ_map[sn + 1]; ++i)
    ಆದರೆ (!SI_DISABLED (i)
        && ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (*state_ವಸ್ತುಗಳು[i].ಅಂಶ) == r->ಸಂಖ್ಯೆ)
      ಹಿಂತಿರುಗಿ i;
  abort ();
}

ಶೂನ್ಯ
counterexample_report_state (ಸ್ಥಿರಾಂಕ state *s, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
  ಸ್ಥಿರಾಂಕ state_ಸಂಖ್ಯೆ sn = s->ಸಂಖ್ಯೆ;
  ಸ್ಥಿರಾಂಕ reductions *reds = s->reductions;
  bitset lookaheads = bitset_create (ntokens, BITSET_FIXED);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    {
      ಸ್ಥಿರಾಂಕ ನಿಯಮ *r1 = reds->ನಿಯಮಗಳು[i];
      ಸ್ಥಿರಾಂಕ state_ಅಂಶ_ಸಂಖ್ಯೆ c1 = ಕಂಡುಹಿಡಿ_state_ಅಂಶ_ಸಂಖ್ಯೆ (r1, sn);
      ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ c2 = state_ಅಂಶ_map[sn]; c2 < state_ಅಂಶ_map[sn + 1]; ++c2)
        ಆದರೆ (!SI_DISABLED (c2))
          {
            ಅಂಶ_ಸಂಖ್ಯೆ conf = *state_ವಸ್ತುಗಳು[c2].ಅಂಶ;
            ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (conf)
                && bitset_test (reds->lookaheads[i], conf))
              counterexample_report_ಸರಿಸು_reduce (c1, c2, conf, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
          }
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = i+1; j < reds->ಅಂಕೆ; ++j)
        {
          ಸ್ಥಿರಾಂಕ ನಿಯಮ *r2 = reds->ನಿಯಮಗಳು[j];
          // Conflicts: common lookaheads.
          bitset_intersection (lookaheads,
                               reds->lookaheads[i],
                               reds->lookaheads[j]);
          ಆದರೆ (!bitset_ಖಾಲಿಗಳು_p (lookaheads))
            ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ c2 = state_ಅಂಶ_map[sn]; c2 < state_ಅಂಶ_map[sn + 1]; ++c2)
              ಆದರೆ (!SI_DISABLED (c2)
                  && ಅಂಶ_ನಿಯಮ (state_ವಸ್ತುಗಳು[c2].ಅಂಶ) == r2)
                {
                  counterexample_report_reduce_reduce (c1, c2, lookaheads, ಹೊರಹೋಗು, ಪೂರ್ವಪ್ರತ್ಯಯ);
                  ನಿಲ್ಲಿಸು;
                }
        }
    }
  bitset_free (lookaheads);
}
