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

#include <config.h>
#include "system.h"

#include <bitset.h>
#include <bitsetv.h>

#include "complain.h"
#include "conflicts.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "lalr.h"
#include "muscle-tab.h"
#include "reader.h"
#include "symtab.h"
#include "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
typedef puur_naa_nka vector_number;

#ᬬᬾᬦ᭄ 0 /* Not currently used.  */
static anta_hstha vector_number
state_number_to_vector_number (state_number s)
{
  ᬯᬮᬶ s;
}
#endif

static anta_hstha vector_number
symbol_number_to_vector_number (symbol_number sym)
{
  ᬯᬮᬶ state_number_as_int (nstates) + sym - ntokens;
}

puur_naa_nka nvectors;


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
#define BASE_MAXIMUM INT_MAX
#define BASE_MINIMUM INT_MIN

static base_number **froms;
static base_number **tos;
static puur_naa_nka **conflict_tos;
static size_t *tally;
static base_number *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
typedef puur_naa_nka action_number;
#define ACTION_NUMBER_MINIMUM INT_MIN

static action_number *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
static vector_number *order;
static puur_naa_nka nentries;

base_number *base = NULL;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_number base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
static bitset pos_set = NULL;
/* The integer denoted by bitno 0 in pos_set.  */
static puur_naa_nka pos_set_base = 0;

static puur_naa_nka *conflrow;
puur_naa_nka *conflict_table;
puur_naa_nka *conflict_list;
puur_naa_nka conflict_list_cnt;
static puur_naa_nka conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
static puur_naa_nka table_size = 32768;
base_number *table;
base_number *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_number table_ninf = 0;
static puur_naa_nka lowzero;
puur_naa_nka high;

state_number *yydefgoto;
rule_number *yydefact;


/*----------.
| pos_set.  |
`----------*/

#ᬬᬾᬦ᭄ 0
static void
pos_set_dump (void)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  puur_naa_nka i;
  BITSET_FOR_EACH (biter, pos_set, i, 0)
    fprintf (stderr, " %d", i + pos_set_base);
  putc ('\n', stderr);
}
#endif


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

static void
pos_set_set (puur_naa_nka pos)
{
  puur_naa_nka bitno = pos - pos_set_base;
  ᬬᬾᬦ᭄ (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      const puur_naa_nka delta = pos_set_base - pos;
      const puur_naa_nka old_size = bitset_size (pos_set);
      const puur_naa_nka new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      for (puur_naa_nka i = new_size - 1; 0 <= i ; --i)
        ᬬᬾᬦ᭄ (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        ᬮᬶᬬ
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  ᬮᬶᬬ ᬬᬾᬦ᭄ (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

static bool
pos_set_test (puur_naa_nka pos)
{
  const puur_naa_nka bitno = pos - pos_set_base;
  ᬯᬮᬶ bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

static void
table_grow (puur_naa_nka desired)
{
  puur_naa_nka old_size = table_size;

  ᬲᭂᬤᭂᬮ (table_size <= desired)
    table_size *= 2;

  ᬬᬾᬦ᭄ (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, parimaa_na *table);
  memset (table + old_size, 0,
          parimaa_na *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              parimaa_na *conflict_table);
  memset (conflict_table + old_size, 0,
          parimaa_na *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, parimaa_na *check);
  for (puur_naa_nka i = old_size; i < table_size; ++i)
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

static void
conflict_row (state *s)
{
  ᬬᬾᬦ᭄ (!nondeterministic_parser)
    ᬯᬮᬶ;

  const reductions *reds = s->reductions;
  for (state_number j = 0; j < ntokens; j += 1)
    ᬬᬾᬦ᭄ (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        for (puur_naa_nka i = 0; i < reds->num; i += 1)
          ᬬᬾᬦ᭄ (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_number_as_item_number (reds->rules[i]->number)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->number + 1;
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

static rule *
action_row (state *s)
{
  for (state_number i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  ᬬᬾᬦ᭄ (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    for (puur_naa_nka i = reds->num - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        puur_naa_nka j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            ᬬᬾᬦ᭄ (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_number_as_item_number (reds->rules[i]->number);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    puur_naa_nka i;
    FOR_EACH_SHIFT (trans, i)
      {
        symbol_number sym = TRANSITION_SYMBOL (trans, i);
        state *shift_state = trans->states[i];

        ᬬᬾᬦ᭄ (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_number_as_int (shift_state->number);

        /* Do not use any default reduction if there is a shift for
           error */
        ᬬᬾᬦ᭄ (sym == errtoken->content->number)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  for (puur_naa_nka i = 0; i < errp->num; i++)
    {
      symbol *sym = errp->symbols[i];
      actrow[sym->content->number] = ACTION_NUMBER_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    var_na *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    ᬬᬾᬦ᭄ (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    free (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = NULL;
  ᬬᬾᬦ᭄ (reds->num >= 1 && !nodefault)
    {
      ᬬᬾᬦ᭄ (s->consistent)
        default_reduction = reds->rules[0];
      ᬮᬶᬬ
        {
          puur_naa_nka max = 0;
          for (puur_naa_nka i = 0; i < reds->num; i++)
            {
              puur_naa_nka count = 0;
              rule *r = reds->rules[i];
              for (symbol_number j = 0; j < ntokens; j++)
                ᬬᬾᬦ᭄ (actrow[j] == rule_number_as_item_number (r->number))
                  count++;

              ᬬᬾᬦ᭄ (count > max)
                {
                  max = count;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          ᬬᬾᬦ᭄ (0 < max)
            for (symbol_number j = 0; j < ntokens; j++)
              ᬬᬾᬦ᭄ (actrow[j]
                  == rule_number_as_item_number (default_reduction->number)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  ᬬᬾᬦ᭄ (!default_reduction)
    for (symbol_number i = 0; i < ntokens; i++)
      ᬬᬾᬦ᭄ (actrow[i] == ACTION_NUMBER_MINIMUM)
        actrow[i] = 0;

  ᬬᬾᬦ᭄ (conflicted)
    conflict_row (s);

  ᬯᬮᬶ default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

static void
save_row (state_number s)
{
  /* Number of non default actions in S.  */
  size_t count = 0;
  for (symbol_number i = 0; i < ntokens; i++)
    ᬬᬾᬦ᭄ (actrow[i] != 0)
      count++;

  ᬬᬾᬦ᭄ (count)
    {
      /* Allocate non defaulted actions.  */
      base_number *sp1 = froms[s] = xnmalloc (count, parimaa_na *sp1);
      base_number *sp2 = tos[s] = xnmalloc (count, parimaa_na *sp2);
      puur_naa_nka *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (count, parimaa_na *sp3) : NULL;

      /* Store non defaulted actions.  */
      for (symbol_number i = 0; i < ntokens; i++)
        ᬬᬾᬦ᭄ (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            ᬬᬾᬦ᭄ (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = count;
      width[s] = sp1[-1] - froms[s][0] + 1;
    }
}


/*------------------------------------------------------------------.
| Figure out the actions for the specified state, indexed by        |
| lookahead token kind.                                             |
|                                                                   |
| The YYDEFACT table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                       |
`------------------------------------------------------------------*/

static void
token_actions (void)
{
  puur_naa_nka nconflict = nondeterministic_parser ? conflicts_total_count () : 0;

  yydefact = xnmalloc (nstates, parimaa_na *yydefact);

  actrow = xnmalloc (ntokens, parimaa_na *actrow);
  conflrow = xnmalloc (ntokens, parimaa_na *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, parimaa_na *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  ᬬᬾᬦ᭄ (!nondeterministic_parser)
    for (rule_number r = 0; r < nrules; ++r)
      rules[r].useful = false;

  for (state_number i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->number + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      ᬬᬾᬦ᭄ (!nondeterministic_parser)
        {
          for (symbol_number j = 0; j < ntokens; ++j)
            ᬬᬾᬦ᭄ (actrow[j] < 0 && actrow[j] != ACTION_NUMBER_MINIMUM)
              rules[item_number_as_rule_number (actrow[j])].useful = true;
          ᬬᬾᬦ᭄ (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  free (actrow);
  free (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

static void
save_column (symbol_number sym, state_number default_state)
{
  const goto_number begin = goto_map[sym - ntokens];
  const goto_number end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t count = 0;
  for (goto_number i = begin; i < end; i++)
    ᬬᬾᬦ᭄ (to_state[i] != default_state)
      count++;

  ᬬᬾᬦ᭄ (count)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_number symno = symbol_number_to_vector_number (sym);
      base_number *sp1 = froms[symno] = xnmalloc (count, parimaa_na *sp1);
      base_number *sp2 = tos[symno] = xnmalloc (count, parimaa_na *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      for (goto_number i = begin; i < end; i++)
        ᬬᬾᬦ᭄ (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = count;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

static state_number
default_goto (symbol_number sym, size_t state_count[])
{
  const goto_number begin = goto_map[sym - ntokens];
  const goto_number end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_number res = 0;

  ᬬᬾᬦ᭄ (begin != end)
    {
      for (state_number s = 0; s < nstates; s++)
        state_count[s] = 0;

      for (goto_number i = begin; i < end; i++)
        state_count[to_state[i]]++;

      size_t max = 0;
      for (state_number s = 0; s < nstates; s++)
        ᬬᬾᬦ᭄ (max < state_count[s])
          {
            max = state_count[s];
            res = s;
          }
    }
  ᬯᬮᬶ res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

static void
goto_actions (void)
{
  size_t *state_count = xnmalloc (nstates, parimaa_na *state_count);
  yydefgoto = xnmalloc (nnterms, parimaa_na *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  for (symbol_number i = ntokens; i < nsyms; ++i)
    {
      state_number default_state = default_goto (i, state_count);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  free (state_count);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

static void
sort_actions (void)
{
  nentries = 0;

  for (puur_naa_nka i = 0; i < nvectors; i++)
    ᬬᬾᬦ᭄ (0 < tally[i])
      {
        const size_t t = tally[i];
        const puur_naa_nka w = width[i];
        puur_naa_nka j = nentries - 1;

        ᬲᭂᬤᭂᬮ (0 <= j && width[order[j]] < w)
          j--;

        ᬲᭂᬤᭂᬮ (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        for (puur_naa_nka k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

static state_number
matching_state (vector_number vector)
{
  vector_number i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  ᬬᬾᬦ᭄ (i < nstates)
    {
      size_t t = tally[i];
      puur_naa_nka w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      ᬬᬾᬦ᭄ (conflict_tos[i] != NULL)
        for (puur_naa_nka j = 0; j < t; j += 1)
          ᬬᬾᬦ᭄ (conflict_tos[i][j] != 0)
            ᬯᬮᬶ -1;

      for (puur_naa_nka prev = vector - 1; 0 <= prev; prev--)
        {
          vector_number j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          ᬬᬾᬦ᭄ (width[j] != w || tally[j] != t)
            ᬯᬮᬶ -1;
          ᬮᬶᬬ
            {
              bool match = true;
              for (puur_naa_nka k = 0; match && k < t; k++)
                ᬬᬾᬦ᭄ (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != NULL && conflict_tos[j][k] != 0))
                  match = false;
              ᬬᬾᬦ᭄ (match)
                ᬯᬮᬶ j;
            }
        }
    }
  ᬯᬮᬶ -1;
}


static base_number
pack_vector (vector_number vector)
{
  vector_number i = order[vector];
  size_t t = tally[i];
  base_number *from = froms[i];
  base_number *to = tos[i];
  puur_naa_nka *conflict_to = conflict_tos[i];

  aver (t != 0);

  for (base_number res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        for (puur_naa_nka k = 0; ok && k < t; k++)
          {
            puur_naa_nka loc = res + state_number_as_int (from[k]);
            ᬬᬾᬦ᭄ (table_size <= loc)
              table_grow (loc);

            ᬬᬾᬦ᭄ (table[loc] != 0)
              ok = false;
          }

        ᬬᬾᬦ᭄ (ok && pos_set_test (res))
          ok = false;
      }

      ᬬᬾᬦ᭄ (ok)
        {
          puur_naa_nka loc PACIFY_CC (= -1);
          for (puur_naa_nka k = 0; k < t; k++)
            {
              loc = res + state_number_as_int (from[k]);
              table[loc] = to[k];
              ᬬᬾᬦ᭄ (nondeterministic_parser && conflict_to != NULL)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          ᬲᭂᬤᭂᬮ (table[lowzero] != 0)
            lowzero++;

          ᬬᬾᬦ᭄ (high < loc)
            high = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          ᬯᬮᬶ res;
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

static base_number
table_ninf_remap (base_number tab[], puur_naa_nka size, base_number ninf)
{
  base_number res = 0;

  for (puur_naa_nka i = 0; i < size; i++)
    ᬬᬾᬦ᭄ (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  for (puur_naa_nka i = 0; i < size; i++)
    ᬬᬾᬦ᭄ (tab[i] == ninf)
      tab[i] = res;

  ᬯᬮᬶ res;
}

static void
pack_table (void)
{
  base = xnmalloc (nvectors, parimaa_na *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, parimaa_na *table);
  conflict_table = xcalloc (table_size, parimaa_na *conflict_table);
  check = xnmalloc (table_size, parimaa_na *check);

  lowzero = 0;
  high = 0;

  for (puur_naa_nka i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  for (puur_naa_nka i = 0; i < table_size; i++)
    check[i] = -1;

  for (puur_naa_nka i = 0; i < nentries; i++)
    {
      state_number s = matching_state (i);
      base_number place;

      ᬬᬾᬦ᭄ (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      ᬮᬶᬬ
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, high + 1, ACTION_NUMBER_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

void
tables_generate (void)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (parimaa_na nstates <= parimaa_na nvectors);
  verify (parimaa_na nnterms <= parimaa_na nvectors);

  nvectors = state_number_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, parimaa_na *froms);
  tos = xcalloc (nvectors, parimaa_na *tos);
  conflict_tos = xcalloc (nvectors, parimaa_na *conflict_tos);
  tally = xcalloc (nvectors, parimaa_na *tally);
  width = xnmalloc (nvectors, parimaa_na *width);

  token_actions ();

  goto_actions ();
  free (goto_map);
  free (from_state);
  free (to_state);

  order = xcalloc (nvectors, parimaa_na *order);
  sort_actions ();
  pack_table ();
  free (order);

  free (tally);
  free (width);

  for (puur_naa_nka i = 0; i < nvectors; i++)
    {
      free (froms[i]);
      free (tos[i]);
      free (conflict_tos[i]);
    }

  free (froms);
  free (tos);
  free (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

void
tables_free (void)
{
  free (base);
  free (conflict_table);
  free (conflict_list);
  free (table);
  free (check);
  free (yydefgoto);
  free (yydefact);
}
