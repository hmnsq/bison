/* Output the generated parsing program for Bison.

   Copyright (C) 1984, 1986, 1989, 1992, 2000-2006, 2009-2015, 2018-2022
   Free Software Foundation, Inc.

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
#ಅನ್ತರ್ಭಾವ <bitsetv.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "conflicts.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"
#ಅನ್ತರ್ಭಾವ "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ ಸದಿಶ_ಸಂಖ್ಯೆ;

#ಆದರೆ 0 /* Not currently used.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸದಿಶ_ಸಂಖ್ಯೆ
state_ಸಂಖ್ಯೆ_ಗಮ್ಯ_ಸದಿಶ_ಸಂಖ್ಯೆ (state_ಸಂಖ್ಯೆ s)
{
  ಹಿಂತಿರುಗಿ s;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸದಿಶ_ಸಂಖ್ಯೆ
ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಗಮ್ಯ_ಸದಿಶ_ಸಂಖ್ಯೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  ಹಿಂತಿರುಗಿ state_ಸಂಖ್ಯೆ_as_int (nstates) + sym - ntokens;
}

ಪೂರ್ಣಾಂಕ nvectors;


/* FROMS and TOS are indexed by vector_number.

   If VECTOR is a nonterminal, (FROMS[VECTOR], TOS[VECTOR]) form an
   array of state numbers of the non defaulted GOTO on VECTOR.

   If VECTOR is a state, TOS[VECTOR] is the array of actions to do on
   the (array of) symbols FROMS[VECTOR].

   In both cases, TALLY[VECTOR] is the size of the arrays
   FROMS[VECTOR], TOS[VECTOR]; and WIDTH[VECTOR] =
   (FROMS[VECTOR][SIZE] - FROMS[VECTOR][0] + 1) where SIZE =
   TALLY[VECTOR].

   FROMS therefore contains symbol_number and action_number,
   TOS state_number and action_number,
   TALLY sizes,
   WIDTH differences of FROMS.

   Let base_number be the type of FROMS, TOS, and WIDTH.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ತಳ_ಗರಿಷ್ಠ ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ
#ವ್ಯಾಖ್ಯಾನಿಸು ತಳ_MINIMUM INT_MIN

ಸ್ಥಿರ ತಳ_ಸಂಖ್ಯೆ **froms;
ಸ್ಥಿರ ತಳ_ಸಂಖ್ಯೆ **tos;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ **conflict_tos;
ಸ್ಥಿರ size_t *tally;
ಸ್ಥಿರ ತಳ_ಸಂಖ್ಯೆ *ಅಗಲ;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ action_ಸಂಖ್ಯೆ;
#ವ್ಯಾಖ್ಯಾನಿಸು ACTION_ಸಂಖ್ಯೆ_MINIMUM INT_MIN

ಸ್ಥಿರ action_ಸಂಖ್ಯೆ *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
ಸ್ಥಿರ ಸದಿಶ_ಸಂಖ್ಯೆ *ಕ್ರಮ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nentries;

ತಳ_ಸಂಖ್ಯೆ *ತಳ = ಖಾಲಿ;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
ತಳ_ಸಂಖ್ಯೆ ತಳ_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
ಸ್ಥಿರ bitset pos_ಗಣ = ಖಾಲಿ;
/* The integer denoted by bitno 0 in pos_set.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pos_ಗಣ_ತಳ = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *conflrow;
ಪೂರ್ಣಾಂಕ *conflict_ಕೋಷ್ಟಕ;
ಪೂರ್ಣಾಂಕ *conflict_list;
ಪೂರ್ಣಾಂಕ conflict_list_cnt;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೋಷ್ಟಕ_ಗಾತ್ರ = 32768;
ತಳ_ಸಂಖ್ಯೆ *ಕೋಷ್ಟಕ;
ತಳ_ಸಂಖ್ಯೆ *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
ತಳ_ಸಂಖ್ಯೆ ಕೋಷ್ಟಕ_ninf = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ lowzero;
ಪೂರ್ಣಾಂಕ ಮೇಲಿನ;

state_ಸಂಖ್ಯೆ *yydefgoto;
ನಿಯಮ_ಸಂಖ್ಯೆ *yydefact;


/*----------.
| pos_set.  |
`----------*/

#ಆದರೆ 0
ಸ್ಥಿರ ಶೂನ್ಯ
pos_ಗಣ_dump (ಶೂನ್ಯ)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_ಗಾತ್ರ (pos_ಗಣ), pos_ಗಣ_ತಳ);
  bitset_iterator biter;
  ಪೂರ್ಣಾಂಕ i;
  BITSET_FOR_EACH (biter, pos_ಗಣ, i, 0)
    fprintf (stderr, " %d", i + pos_ಗಣ_ತಳ);
  putc ('\n', stderr);
}
#ಆದರೆ_ಅಂತ್ಯ


/* The size and base of POS_SET are not known, we need to be able to
   move the base farther "on the left", and grow "on the right".

   It would be nice to be able to predict the base accurately, but it
   seems difficult (-nstates seems to work most of the time, except
   when there are useless tokens).

   FIXME: The current approach is correct, but with poor performances.
   Bitsets need to support 'assign' and 'shift'.  And instead of
   extending POS_SET just for the out-of-range new values, we need
   something like doubling the size.
  */

ಸ್ಥಿರ ಶೂನ್ಯ
pos_ಗಣ_ಗಣ (ಪೂರ್ಣಾಂಕ pos)
{
  ಪೂರ್ಣಾಂಕ bitno = pos - pos_ಗಣ_ತಳ;
  ಆದರೆ (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ delta = pos_ಗಣ_ತಳ - pos;
      ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ old_ಗಾತ್ರ = bitset_ಗಾತ್ರ (pos_ಗಣ);
      ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ new_ಗಾತ್ರ = old_ಗಾತ್ರ + delta;
      bitset_resize (pos_ಗಣ, new_ಗಾತ್ರ);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = new_ಗಾತ್ರ - 1; 0 <= i ; --i)
        ಆದರೆ (delta <= i && bitset_test (pos_ಗಣ, i - delta))
          bitset_ಗಣ (pos_ಗಣ, i);
        ಇಲ್ಲದಿದ್ದರೆ
          bitset_reset (pos_ಗಣ, i);
      pos_ಗಣ_ತಳ = pos;
      bitno = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (bitset_ಗಾತ್ರ (pos_ಗಣ) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_ಗಣ, bitno + 1);
  bitset_ಗಣ (pos_ಗಣ, bitno);
}

ಸ್ಥಿರ bool
pos_ಗಣ_test (ಪೂರ್ಣಾಂಕ pos)
{
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ bitno = pos - pos_ಗಣ_ತಳ;
  ಹಿಂತಿರುಗಿ bitset_test (pos_ಗಣ, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೋಷ್ಟಕ_grow (ಪೂರ್ಣಾಂಕ desired)
{
  ಪೂರ್ಣಾಂಕ old_ಗಾತ್ರ = ಕೋಷ್ಟಕ_ಗಾತ್ರ;

  ಆಗ (ಕೋಷ್ಟಕ_ಗಾತ್ರ <= desired)
    ಕೋಷ್ಟಕ_ಗಾತ್ರ *= 2;

  ಆದರೆ (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_ಗಾತ್ರ, ಕೋಷ್ಟಕ_ಗಾತ್ರ);

  ಕೋಷ್ಟಕ = xnrealloc (ಕೋಷ್ಟಕ, ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಗಾತ್ರ *ಕೋಷ್ಟಕ);
  memset (ಕೋಷ್ಟಕ + old_ಗಾತ್ರ, 0,
          ಗಾತ್ರ *ಕೋಷ್ಟಕ * (ಕೋಷ್ಟಕ_ಗಾತ್ರ - old_ಗಾತ್ರ));

  conflict_ಕೋಷ್ಟಕ = xnrealloc (conflict_ಕೋಷ್ಟಕ, ಕೋಷ್ಟಕ_ಗಾತ್ರ,
                              ಗಾತ್ರ *conflict_ಕೋಷ್ಟಕ);
  memset (conflict_ಕೋಷ್ಟಕ + old_ಗಾತ್ರ, 0,
          ಗಾತ್ರ *conflict_ಕೋಷ್ಟಕ * (ಕೋಷ್ಟಕ_ಗಾತ್ರ - old_ಗಾತ್ರ));

  check = xnrealloc (check, ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಗಾತ್ರ *check);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = old_ಗಾತ್ರ; i < ಕೋಷ್ಟಕ_ಗಾತ್ರ; ++i)
    check[i] = -1;
}




/*-------------------------------------------------------------------.
| For GLR parsers, for each conflicted token in S, as indicated      |
| by non-zero entries in CONFLROW, create a list of possible         |
| reductions that are alternatives to the shift or reduction         |
| currently recorded for that token in S.  Store the alternative     |
| reductions followed by a 0 in CONFLICT_LIST, updating              |
| CONFLICT_LIST_CNT, and storing an index to the start of the list   |
| back into CONFLROW.                                                |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
conflict_ಅಡ್ಡಸಾಲು (state *s)
{
  ಆದರೆ (!nondeterministic_parser)
    ಹಿಂತಿರುಗಿ;

  ಸ್ಥಿರಾಂಕ reductions *reds = s->reductions;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ j = 0; j < ntokens; j += 1)
    ಆದರೆ (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; i += 1)
          ಆದರೆ (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != ನಿಯಮ_ಸಂಖ್ಯೆ_as_ಅಂಶ_ಸಂಖ್ಯೆ (reds->ನಿಯಮಗಳು[i]->ಸಂಖ್ಯೆ)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->ನಿಯಮಗಳು[i]->ಸಂಖ್ಯೆ + 1;
              conflict_list_cnt += 1;
              conflict_list_free -= 1;
            }

        /* Leave a 0 at the end.  */
        aver (0 < conflict_list_free);
        conflict_list[conflict_list_cnt] = 0;
        conflict_list_cnt += 1;
        conflict_list_free -= 1;
      }
}


/*------------------------------------------------------------------.
| Decide what to do for each type of token if seen as the           |
| lookahead in specified state.  The value returned is used as the  |
| default action (yydefact) for the state.  In addition, ACTROW is  |
| filled with what to do for each kind of token, index by symbol    |
| number, with zero meaning do the default action.  The value       |
| ACTION_NUMBER_MINIMUM, a very negative number, means this         |
| situation is an error.  The parser recognizes this value          |
| specially.                                                        |
|                                                                   |
| This is where conflicts are resolved.  The loop over lookahead    |
| rules considered lower-numbered rules last, and the last rule     |
| considered that likes a token gets to handle it.                  |
|                                                                   |
| For GLR parsers, also sets CONFLROW[SYM] to an index into         |
| CONFLICT_LIST iff there is an unresolved conflict (s/r or r/r)    |
| with symbol SYM. The default reduction is not used for a symbol   |
| that has any such conflicts.                                      |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ನಿಯಮ *
action_ಅಡ್ಡಸಾಲು (state *s)
{
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  ಆದರೆ (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = reds->ಅಂಕೆ - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        ಪೂರ್ಣಾಂಕ j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            ಆದರೆ (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = ನಿಯಮ_ಸಂಖ್ಯೆ_as_ಅಂಶ_ಸಂಖ್ಯೆ (reds->ನಿಯಮಗಳು[i]->ಸಂಖ್ಯೆ);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    ಪೂರ್ಣಾಂಕ i;
    FOR_EACH_ಸರಿಸು (trans, i)
      {
        ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = TRANSITION_ಚಿಹ್ನೆ (trans, i);
        state *ಸರಿಸು_state = trans->ಸ್ಥಿತಿಗಳು[i];

        ಆದರೆ (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_ಸಂಖ್ಯೆ_as_int (ಸರಿಸು_state->ಸಂಖ್ಯೆ);

        /* Do not use any default reduction if there is a shift for
           error */
        ಆದರೆ (sym == errtoken->content->ಸಂಖ್ಯೆ)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < errp->ಅಂಕೆ; i++)
    {
      ಚಿಹ್ನೆ *sym = errp->symbols[i];
      actrow[sym->content->ಸಂಖ್ಯೆ] = ACTION_ಸಂಖ್ಯೆ_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    ಅಕ್ಷರ *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    ಆದರೆ (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    ಬಿಡುಗಡೆ (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  ನಿಯಮ *default_reduction = ಖಾಲಿ;
  ಆದರೆ (reds->ಅಂಕೆ >= 1 && !nodefault)
    {
      ಆದರೆ (s->consistent)
        default_reduction = reds->ನಿಯಮಗಳು[0];
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ max = 0;
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; i++)
            {
              ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
              ನಿಯಮ *r = reds->ನಿಯಮಗಳು[i];
              ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = 0; j < ntokens; j++)
                ಆದರೆ (actrow[j] == ನಿಯಮ_ಸಂಖ್ಯೆ_as_ಅಂಶ_ಸಂಖ್ಯೆ (r->ಸಂಖ್ಯೆ))
                  ಎಣಿಕೆ_2++;

              ಆದರೆ (ಎಣಿಕೆ_2 > max)
                {
                  max = ಎಣಿಕೆ_2;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          ಆದರೆ (0 < max)
            ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = 0; j < ntokens; j++)
              ಆದರೆ (actrow[j]
                  == ನಿಯಮ_ಸಂಖ್ಯೆ_as_ಅಂಶ_ಸಂಖ್ಯೆ (default_reduction->ಸಂಖ್ಯೆ)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  ಆದರೆ (!default_reduction)
    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; i++)
      ಆದರೆ (actrow[i] == ACTION_ಸಂಖ್ಯೆ_MINIMUM)
        actrow[i] = 0;

  ಆದರೆ (conflicted)
    conflict_ಅಡ್ಡಸಾಲು (s);

  ಹಿಂತಿರುಗಿ default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
save_ಅಡ್ಡಸಾಲು (state_ಸಂಖ್ಯೆ s)
{
  /* Number of non default actions in S.  */
  size_t ಎಣಿಕೆ_2 = 0;
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; i++)
    ಆದರೆ (actrow[i] != 0)
      ಎಣಿಕೆ_2++;

  ಆದರೆ (ಎಣಿಕೆ_2)
    {
      /* Allocate non defaulted actions.  */
      ತಳ_ಸಂಖ್ಯೆ *sp1 = froms[s] = xnmalloc (ಎಣಿಕೆ_2, ಗಾತ್ರ *sp1);
      ತಳ_ಸಂಖ್ಯೆ *sp2 = tos[s] = xnmalloc (ಎಣಿಕೆ_2, ಗಾತ್ರ *sp2);
      ಪೂರ್ಣಾಂಕ *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (ಎಣಿಕೆ_2, ಗಾತ್ರ *sp3) : ಖಾಲಿ;

      /* Store non defaulted actions.  */
      ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = 0; i < ntokens; i++)
        ಆದರೆ (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            ಆದರೆ (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = ಎಣಿಕೆ_2;
      ಅಗಲ[s] = sp1[-1] - froms[s][0] + 1;
    }
}


/*------------------------------------------------------------------.
| Figure out the actions for the specified state, indexed by        |
| lookahead token kind.                                             |
|                                                                   |
| The YYDEFACT table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                       |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
token_actions (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ nconflict = nondeterministic_parser ? conflicts_ಒಟ್ಟು_ಎಣಿಕೆ () : 0;

  yydefact = xnmalloc (nstates, ಗಾತ್ರ *yydefact);

  actrow = xnmalloc (ntokens, ಗಾತ್ರ *actrow);
  conflrow = xnmalloc (ntokens, ಗಾತ್ರ *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, ಗಾತ್ರ *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  ಆದರೆ (!nondeterministic_parser)
    ಗಾಗಿ (ನಿಯಮ_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
      ನಿಯಮಗಳು[r].useful = false;

  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ನಿಯಮ *default_reduction = action_ಅಡ್ಡಸಾಲು (ಸ್ಥಿತಿಗಳು[i]);
      yydefact[i] = default_reduction ? default_reduction->ಸಂಖ್ಯೆ + 1 : 0;
      save_ಅಡ್ಡಸಾಲು (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      ಆದರೆ (!nondeterministic_parser)
        {
          ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ j = 0; j < ntokens; ++j)
            ಆದರೆ (actrow[j] < 0 && actrow[j] != ACTION_ಸಂಖ್ಯೆ_MINIMUM)
              ನಿಯಮಗಳು[ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (actrow[j])].useful = true;
          ಆದರೆ (yydefact[i])
            ನಿಯಮಗಳು[yydefact[i] - 1].useful = true;
        }
    }
  ಬಿಡುಗಡೆ (actrow);
  ಬಿಡುಗಡೆ (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
save_ಸ್ತಂಭ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, state_ಸಂಖ್ಯೆ default_state)
{
  ಸ್ಥಿರಾಂಕ goto_ಸಂಖ್ಯೆ begin = goto_map[sym - ntokens];
  ಸ್ಥಿರಾಂಕ goto_ಸಂಖ್ಯೆ ಕೊನೆ = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t ಎಣಿಕೆ_2 = 0;
  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = begin; i < ಕೊನೆ; i++)
    ಆದರೆ (ಗಮ್ಯ_state[i] != default_state)
      ಎಣಿಕೆ_2++;

  ಆದರೆ (ಎಣಿಕೆ_2)
    {
      /* Allocate room for non defaulted gotos.  */
      ಸದಿಶ_ಸಂಖ್ಯೆ symno = ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_ಗಮ್ಯ_ಸದಿಶ_ಸಂಖ್ಯೆ (sym);
      ತಳ_ಸಂಖ್ಯೆ *sp1 = froms[symno] = xnmalloc (ಎಣಿಕೆ_2, ಗಾತ್ರ *sp1);
      ತಳ_ಸಂಖ್ಯೆ *sp2 = tos[symno] = xnmalloc (ಎಣಿಕೆ_2, ಗಾತ್ರ *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = begin; i < ಕೊನೆ; i++)
        ಆದರೆ (ಗಮ್ಯ_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = ಗಮ್ಯ_state[i];
          }

      tally[symno] = ಎಣಿಕೆ_2;
      ಅಗಲ[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

ಸ್ಥಿರ state_ಸಂಖ್ಯೆ
default_goto (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, size_t state_ಎಣಿಕೆ[])
{
  ಸ್ಥಿರಾಂಕ goto_ಸಂಖ್ಯೆ begin = goto_map[sym - ntokens];
  ಸ್ಥಿರಾಂಕ goto_ಸಂಖ್ಯೆ ಕೊನೆ = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_ಸಂಖ್ಯೆ res = 0;

  ಆದರೆ (begin != ಕೊನೆ)
    {
      ಗಾಗಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; s++)
        state_ಎಣಿಕೆ[s] = 0;

      ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = begin; i < ಕೊನೆ; i++)
        state_ಎಣಿಕೆ[ಗಮ್ಯ_state[i]]++;

      size_t max = 0;
      ಗಾಗಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; s++)
        ಆದರೆ (max < state_ಎಣಿಕೆ[s])
          {
            max = state_ಎಣಿಕೆ[s];
            res = s;
          }
    }
  ಹಿಂತಿರುಗಿ res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
goto_actions (ಶೂನ್ಯ)
{
  size_t *state_ಎಣಿಕೆ = xnmalloc (nstates, ಗಾತ್ರ *state_ಎಣಿಕೆ);
  yydefgoto = xnmalloc (nnterms, ಗಾತ್ರ *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    {
      state_ಸಂಖ್ಯೆ default_state = default_goto (i, state_ಎಣಿಕೆ);
      save_ಸ್ತಂಭ (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  ಬಿಡುಗಡೆ (state_ಎಣಿಕೆ);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಂಗಡಿಸು_actions (ಶೂನ್ಯ)
{
  nentries = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nvectors; i++)
    ಆದರೆ (0 < tally[i])
      {
        ಸ್ಥಿರಾಂಕ size_t t = tally[i];
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ w = ಅಗಲ[i];
        ಪೂರ್ಣಾಂಕ j = nentries - 1;

        ಆಗ (0 <= j && ಅಗಲ[ಕ್ರಮ[j]] < w)
          j--;

        ಆಗ (0 <= j && ಅಗಲ[ಕ್ರಮ[j]] == w && tally[ಕ್ರಮ[j]] < t)
          j--;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = nentries - 1; k > j; k--)
          ಕ್ರಮ[k + 1] = ಕ್ರಮ[k];

        ಕ್ರಮ[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

ಸ್ಥಿರ state_ಸಂಖ್ಯೆ
matching_state (ಸದಿಶ_ಸಂಖ್ಯೆ ಸದಿಶ)
{
  ಸದಿಶ_ಸಂಖ್ಯೆ i = ಕ್ರಮ[ಸದಿಶ];
  /* If VECTOR is a nterm, return -1.  */
  ಆದರೆ (i < nstates)
    {
      size_t t = tally[i];
      ಪೂರ್ಣಾಂಕ w = ಅಗಲ[i];

      /* If VECTOR has GLR conflicts, return -1 */
      ಆದರೆ (conflict_tos[i] != ಖಾಲಿ)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < t; j += 1)
          ಆದರೆ (conflict_tos[i][j] != 0)
            ಹಿಂತಿರುಗಿ -1;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ prev = ಸದಿಶ - 1; 0 <= prev; prev--)
        {
          ಸದಿಶ_ಸಂಖ್ಯೆ j = ಕ್ರಮ[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          ಆದರೆ (ಅಗಲ[j] != w || tally[j] != t)
            ಹಿಂತಿರುಗಿ -1;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              bool ಹೊಂದಿಕೆ = true;
              ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; ಹೊಂದಿಕೆ && k < t; k++)
                ಆದರೆ (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != ಖಾಲಿ && conflict_tos[j][k] != 0))
                  ಹೊಂದಿಕೆ = false;
              ಆದರೆ (ಹೊಂದಿಕೆ)
                ಹಿಂತಿರುಗಿ j;
            }
        }
    }
  ಹಿಂತಿರುಗಿ -1;
}


ಸ್ಥಿರ ತಳ_ಸಂಖ್ಯೆ
pack_ಸದಿಶ (ಸದಿಶ_ಸಂಖ್ಯೆ ಸದಿಶ)
{
  ಸದಿಶ_ಸಂಖ್ಯೆ i = ಕ್ರಮ[ಸದಿಶ];
  size_t t = tally[i];
  ತಳ_ಸಂಖ್ಯೆ *from = froms[i];
  ತಳ_ಸಂಖ್ಯೆ *ಗಮ್ಯ = tos[i];
  ಪೂರ್ಣಾಂಕ *conflict_ಗಮ್ಯ = conflict_tos[i];

  aver (t != 0);

  ಗಾಗಿ (ತಳ_ಸಂಖ್ಯೆ res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < ಕೋಷ್ಟಕ_ಗಾತ್ರ);
      {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; ok && k < t; k++)
          {
            ಪೂರ್ಣಾಂಕ loc = res + state_ಸಂಖ್ಯೆ_as_int (from[k]);
            ಆದರೆ (ಕೋಷ್ಟಕ_ಗಾತ್ರ <= loc)
              ಕೋಷ್ಟಕ_grow (loc);

            ಆದರೆ (ಕೋಷ್ಟಕ[loc] != 0)
              ok = false;
          }

        ಆದರೆ (ok && pos_ಗಣ_test (res))
          ok = false;
      }

      ಆದರೆ (ok)
        {
          ಪೂರ್ಣಾಂಕ loc PACIFY_CC (= -1);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < t; k++)
            {
              loc = res + state_ಸಂಖ್ಯೆ_as_int (from[k]);
              ಕೋಷ್ಟಕ[loc] = ಗಮ್ಯ[k];
              ಆದರೆ (nondeterministic_parser && conflict_ಗಮ್ಯ != ಖಾಲಿ)
                conflict_ಕೋಷ್ಟಕ[loc] = conflict_ಗಮ್ಯ[k];
              check[loc] = from[k];
            }

          ಆಗ (ಕೋಷ್ಟಕ[lowzero] != 0)
            lowzero++;

          ಆದರೆ (ಮೇಲಿನ < loc)
            ಮೇಲಿನ = loc;

          aver (ತಳ_MINIMUM <= res && res <= ತಳ_ಗರಿಷ್ಠ);
          ಹಿಂತಿರುಗಿ res;
        }
    }
}


/*-------------------------------------------------------------.
| Remap the negative infinite in TAB from NINF to the greatest |
| possible smallest value.  Return it.                         |
|                                                              |
| In most case this allows us to use shorts instead of ints in |
| parsers.                                                     |
`-------------------------------------------------------------*/

ಸ್ಥಿರ ತಳ_ಸಂಖ್ಯೆ
ಕೋಷ್ಟಕ_ninf_remap (ತಳ_ಸಂಖ್ಯೆ tab[], ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ತಳ_ಸಂಖ್ಯೆ ninf)
{
  ತಳ_ಸಂಖ್ಯೆ res = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ_2; i++)
    ಆದರೆ (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ_2; i++)
    ಆದರೆ (tab[i] == ninf)
      tab[i] = res;

  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
pack_ಕೋಷ್ಟಕ (ಶೂನ್ಯ)
{
  ತಳ = xnmalloc (nvectors, ಗಾತ್ರ *ತಳ);
  pos_ಗಣ = bitset_create (ಕೋಷ್ಟಕ_ಗಾತ್ರ + nstates, BITSET_FRUGAL);
  pos_ಗಣ_ತಳ = -nstates;
  ಕೋಷ್ಟಕ = xcalloc (ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಗಾತ್ರ *ಕೋಷ್ಟಕ);
  conflict_ಕೋಷ್ಟಕ = xcalloc (ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಗಾತ್ರ *conflict_ಕೋಷ್ಟಕ);
  check = xnmalloc (ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಗಾತ್ರ *check);

  lowzero = 0;
  ಮೇಲಿನ = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nvectors; i++)
    ತಳ[i] = ತಳ_MINIMUM;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಕೋಷ್ಟಕ_ಗಾತ್ರ; i++)
    check[i] = -1;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nentries; i++)
    {
      state_ಸಂಖ್ಯೆ s = matching_state (i);
      ತಳ_ಸಂಖ್ಯೆ ಇರಿಸು;

      ಆದರೆ (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        ಇರಿಸು = pack_ಸದಿಶ (i);
      ಇಲ್ಲದಿದ್ದರೆ
        /* Action of I were already coded for S.  */
        ಇರಿಸು = ತಳ[s];

      pos_ಗಣ_ಗಣ (ಇರಿಸು);
      ತಳ[ಕ್ರಮ[i]] = ಇರಿಸು;
    }

  /* Use the greatest possible negative infinites.  */
  ತಳ_ninf = ಕೋಷ್ಟಕ_ninf_remap (ತಳ, nvectors, ತಳ_MINIMUM);
  ಕೋಷ್ಟಕ_ninf = ಕೋಷ್ಟಕ_ninf_remap (ಕೋಷ್ಟಕ, ಮೇಲಿನ + 1, ACTION_ಸಂಖ್ಯೆ_MINIMUM);

  bitset_free (pos_ಗಣ);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

ಶೂನ್ಯ
tables_generate (ಶೂನ್ಯ)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (ಗಾತ್ರ nstates <= ಗಾತ್ರ nvectors);
  verify (ಗಾತ್ರ nnterms <= ಗಾತ್ರ nvectors);

  nvectors = state_ಸಂಖ್ಯೆ_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, ಗಾತ್ರ *froms);
  tos = xcalloc (nvectors, ಗಾತ್ರ *tos);
  conflict_tos = xcalloc (nvectors, ಗಾತ್ರ *conflict_tos);
  tally = xcalloc (nvectors, ಗಾತ್ರ *tally);
  ಅಗಲ = xnmalloc (nvectors, ಗಾತ್ರ *ಅಗಲ);

  token_actions ();

  goto_actions ();
  ಬಿಡುಗಡೆ (goto_map);
  ಬಿಡುಗಡೆ (from_state);
  ಬಿಡುಗಡೆ (ಗಮ್ಯ_state);

  ಕ್ರಮ = xcalloc (nvectors, ಗಾತ್ರ *ಕ್ರಮ);
  ವಿಂಗಡಿಸು_actions ();
  pack_ಕೋಷ್ಟಕ ();
  ಬಿಡುಗಡೆ (ಕ್ರಮ);

  ಬಿಡುಗಡೆ (tally);
  ಬಿಡುಗಡೆ (ಅಗಲ);

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nvectors; i++)
    {
      ಬಿಡುಗಡೆ (froms[i]);
      ಬಿಡುಗಡೆ (tos[i]);
      ಬಿಡುಗಡೆ (conflict_tos[i]);
    }

  ಬಿಡುಗಡೆ (froms);
  ಬಿಡುಗಡೆ (tos);
  ಬಿಡುಗಡೆ (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

ಶೂನ್ಯ
tables_free (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (ತಳ);
  ಬಿಡುಗಡೆ (conflict_ಕೋಷ್ಟಕ);
  ಬಿಡುಗಡೆ (conflict_list);
  ಬಿಡುಗಡೆ (ಕೋಷ್ಟಕ);
  ಬಿಡುಗಡೆ (check);
  ಬಿಡುಗಡೆ (yydefgoto);
  ಬಿಡುಗಡೆ (yydefact);
}
