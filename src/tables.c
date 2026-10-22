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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <bitsetv.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "conflicts.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "symtab.h"
#அந்தர்பாவ "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
வகைவரை முழு vector_எண்;

#எனில் 0 /* Not currently used.  */
நிலையான உள்வரி vector_எண்
state_எண்_to_vector_எண் (state_எண் s)
{
  திரும்பு s;
}
#endif

நிலையான உள்வரி vector_எண்
குறியீடு_எண்_to_vector_எண் (குறியீடு_எண் sym)
{
  திரும்பு state_எண்_as_int (nstates) + sym - ntokens;
}

முழு nvectors;


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

நிலையான base_எண் **froms;
நிலையான base_எண் **tos;
நிலையான முழு **conflict_tos;
நிலையான size_t *tally;
நிலையான base_எண் *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
வகைவரை முழு action_எண்;
#define ACTION_எண்_MINIMUM INT_MIN

நிலையான action_எண் *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
நிலையான vector_எண் *order;
நிலையான முழு nentries;

base_எண் *base = வெற்று;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_எண் base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
நிலையான bitset pos_set = வெற்று;
/* The integer denoted by bitno 0 in pos_set.  */
நிலையான முழு pos_set_base = 0;

நிலையான முழு *conflrow;
முழு *conflict_table;
முழு *conflict_list;
முழு conflict_list_cnt;
நிலையான முழு conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
நிலையான முழு table_size = 32768;
base_எண் *table;
base_எண் *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_எண் table_ninf = 0;
நிலையான முழு lowzero;
முழு உயர்வு;

state_எண் *yydefgoto;
rule_எண் *yydefact;


/*----------.
| pos_set.  |
`----------*/

#எனில் 0
நிலையான வெறுமை
pos_set_dump (வெறுமை)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  முழு i;
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

நிலையான வெறுமை
pos_set_set (முழு pos)
{
  முழு bitno = pos - pos_set_base;
  எனில் (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      மாறிலி முழு delta = pos_set_base - pos;
      மாறிலி முழு old_size = bitset_size (pos_set);
      மாறிலி முழு new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      க்கு (முழு i = new_size - 1; 0 <= i ; --i)
        எனில் (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        இல்லையெனில்
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  இல்லையெனில் எனில் (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

நிலையான bool
pos_set_test (முழு pos)
{
  மாறிலி முழு bitno = pos - pos_set_base;
  திரும்பு bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
table_grow (முழு desired)
{
  முழு old_size = table_size;

  போது (table_size <= desired)
    table_size *= 2;

  எனில் (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, அளவு *table);
  memset (table + old_size, 0,
          அளவு *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              அளவு *conflict_table);
  memset (conflict_table + old_size, 0,
          அளவு *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, அளவு *check);
  க்கு (முழு i = old_size; i < table_size; ++i)
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

நிலையான வெறுமை
conflict_row (state *s)
{
  எனில் (!nondeterministic_parser)
    திரும்பு;

  மாறிலி reductions *reds = s->reductions;
  க்கு (state_எண் j = 0; j < ntokens; j += 1)
    எனில் (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        க்கு (முழு i = 0; i < reds->இலக்கம்; i += 1)
          எனில் (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_எண்_as_item_எண் (reds->rules[i]->எண்)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->எண் + 1;
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

நிலையான rule *
action_row (state *s)
{
  க்கு (state_எண் i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  எனில் (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    க்கு (முழு i = reds->இலக்கம் - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        முழு j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            எனில் (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_எண்_as_item_எண் (reds->rules[i]->எண்);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    முழு i;
    FOR_EACH_SHIFT (trans, i)
      {
        குறியீடு_எண் sym = TRANSITION_குறியீடு (trans, i);
        state *shift_state = trans->states[i];

        எனில் (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_எண்_as_int (shift_state->எண்);

        /* Do not use any default reduction if there is a shift for
           error */
        எனில் (sym == errtoken->content->எண்)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  க்கு (முழு i = 0; i < errp->இலக்கம்; i++)
    {
      குறியீடு *sym = errp->symbols[i];
      actrow[sym->content->எண்] = ACTION_எண்_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    எழுத்து *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    எனில் (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    விடுவி (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = வெற்று;
  எனில் (reds->இலக்கம் >= 1 && !nodefault)
    {
      எனில் (s->consistent)
        default_reduction = reds->rules[0];
      இல்லையெனில்
        {
          முழு max = 0;
          க்கு (முழு i = 0; i < reds->இலக்கம்; i++)
            {
              முழு மொத்தம் = 0;
              rule *r = reds->rules[i];
              க்கு (குறியீடு_எண் j = 0; j < ntokens; j++)
                எனில் (actrow[j] == rule_எண்_as_item_எண் (r->எண்))
                  மொத்தம்++;

              எனில் (மொத்தம் > max)
                {
                  max = மொத்தம்;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          எனில் (0 < max)
            க்கு (குறியீடு_எண் j = 0; j < ntokens; j++)
              எனில் (actrow[j]
                  == rule_எண்_as_item_எண் (default_reduction->எண்)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  எனில் (!default_reduction)
    க்கு (குறியீடு_எண் i = 0; i < ntokens; i++)
      எனில் (actrow[i] == ACTION_எண்_MINIMUM)
        actrow[i] = 0;

  எனில் (conflicted)
    conflict_row (s);

  திரும்பு default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

நிலையான வெறுமை
save_row (state_எண் s)
{
  /* Number of non default actions in S.  */
  size_t மொத்தம் = 0;
  க்கு (குறியீடு_எண் i = 0; i < ntokens; i++)
    எனில் (actrow[i] != 0)
      மொத்தம்++;

  எனில் (மொத்தம்)
    {
      /* Allocate non defaulted actions.  */
      base_எண் *sp1 = froms[s] = xnmalloc (மொத்தம், அளவு *sp1);
      base_எண் *sp2 = tos[s] = xnmalloc (மொத்தம், அளவு *sp2);
      முழு *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (மொத்தம், அளவு *sp3) : வெற்று;

      /* Store non defaulted actions.  */
      க்கு (குறியீடு_எண் i = 0; i < ntokens; i++)
        எனில் (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            எனில் (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = மொத்தம்;
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

நிலையான வெறுமை
token_actions (வெறுமை)
{
  முழு nconflict = nondeterministic_parser ? conflicts_total_மொத்தம் () : 0;

  yydefact = xnmalloc (nstates, அளவு *yydefact);

  actrow = xnmalloc (ntokens, அளவு *actrow);
  conflrow = xnmalloc (ntokens, அளவு *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, அளவு *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  எனில் (!nondeterministic_parser)
    க்கு (rule_எண் r = 0; r < nrules; ++r)
      rules[r].useful = false;

  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->எண் + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      எனில் (!nondeterministic_parser)
        {
          க்கு (குறியீடு_எண் j = 0; j < ntokens; ++j)
            எனில் (actrow[j] < 0 && actrow[j] != ACTION_எண்_MINIMUM)
              rules[item_எண்_as_rule_எண் (actrow[j])].useful = true;
          எனில் (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  விடுவி (actrow);
  விடுவி (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

நிலையான வெறுமை
save_column (குறியீடு_எண் sym, state_எண் default_state)
{
  மாறிலி goto_எண் begin = goto_map[sym - ntokens];
  மாறிலி goto_எண் end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t மொத்தம் = 0;
  க்கு (goto_எண் i = begin; i < end; i++)
    எனில் (to_state[i] != default_state)
      மொத்தம்++;

  எனில் (மொத்தம்)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_எண் symno = குறியீடு_எண்_to_vector_எண் (sym);
      base_எண் *sp1 = froms[symno] = xnmalloc (மொத்தம், அளவு *sp1);
      base_எண் *sp2 = tos[symno] = xnmalloc (மொத்தம், அளவு *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      க்கு (goto_எண் i = begin; i < end; i++)
        எனில் (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = மொத்தம்;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

நிலையான state_எண்
default_goto (குறியீடு_எண் sym, size_t state_மொத்தம்[])
{
  மாறிலி goto_எண் begin = goto_map[sym - ntokens];
  மாறிலி goto_எண் end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_எண் res = 0;

  எனில் (begin != end)
    {
      க்கு (state_எண் s = 0; s < nstates; s++)
        state_மொத்தம்[s] = 0;

      க்கு (goto_எண் i = begin; i < end; i++)
        state_மொத்தம்[to_state[i]]++;

      size_t max = 0;
      க்கு (state_எண் s = 0; s < nstates; s++)
        எனில் (max < state_மொத்தம்[s])
          {
            max = state_மொத்தம்[s];
            res = s;
          }
    }
  திரும்பு res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
goto_actions (வெறுமை)
{
  size_t *state_மொத்தம் = xnmalloc (nstates, அளவு *state_மொத்தம்);
  yydefgoto = xnmalloc (nnterms, அளவு *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    {
      state_எண் default_state = default_goto (i, state_மொத்தம்);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  விடுவி (state_மொத்தம்);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

நிலையான வெறுமை
sort_actions (வெறுமை)
{
  nentries = 0;

  க்கு (முழு i = 0; i < nvectors; i++)
    எனில் (0 < tally[i])
      {
        மாறிலி size_t t = tally[i];
        மாறிலி முழு w = width[i];
        முழு j = nentries - 1;

        போது (0 <= j && width[order[j]] < w)
          j--;

        போது (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        க்கு (முழு k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

நிலையான state_எண்
matching_state (vector_எண் vector)
{
  vector_எண் i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  எனில் (i < nstates)
    {
      size_t t = tally[i];
      முழு w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      எனில் (conflict_tos[i] != வெற்று)
        க்கு (முழு j = 0; j < t; j += 1)
          எனில் (conflict_tos[i][j] != 0)
            திரும்பு -1;

      க்கு (முழு prev = vector - 1; 0 <= prev; prev--)
        {
          vector_எண் j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          எனில் (width[j] != w || tally[j] != t)
            திரும்பு -1;
          இல்லையெனில்
            {
              bool match = true;
              க்கு (முழு k = 0; match && k < t; k++)
                எனில் (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != வெற்று && conflict_tos[j][k] != 0))
                  match = false;
              எனில் (match)
                திரும்பு j;
            }
        }
    }
  திரும்பு -1;
}


நிலையான base_எண்
pack_vector (vector_எண் vector)
{
  vector_எண் i = order[vector];
  size_t t = tally[i];
  base_எண் *from = froms[i];
  base_எண் *to = tos[i];
  முழு *conflict_to = conflict_tos[i];

  aver (t != 0);

  க்கு (base_எண் res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        க்கு (முழு k = 0; ok && k < t; k++)
          {
            முழு loc = res + state_எண்_as_int (from[k]);
            எனில் (table_size <= loc)
              table_grow (loc);

            எனில் (table[loc] != 0)
              ok = false;
          }

        எனில் (ok && pos_set_test (res))
          ok = false;
      }

      எனில் (ok)
        {
          முழு loc PACIFY_CC (= -1);
          க்கு (முழு k = 0; k < t; k++)
            {
              loc = res + state_எண்_as_int (from[k]);
              table[loc] = to[k];
              எனில் (nondeterministic_parser && conflict_to != வெற்று)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          போது (table[lowzero] != 0)
            lowzero++;

          எனில் (உயர்வு < loc)
            உயர்வு = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          திரும்பு res;
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

நிலையான base_எண்
table_ninf_remap (base_எண் tab[], முழு size, base_எண் ninf)
{
  base_எண் res = 0;

  க்கு (முழு i = 0; i < size; i++)
    எனில் (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  க்கு (முழு i = 0; i < size; i++)
    எனில் (tab[i] == ninf)
      tab[i] = res;

  திரும்பு res;
}

நிலையான வெறுமை
pack_table (வெறுமை)
{
  base = xnmalloc (nvectors, அளவு *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, அளவு *table);
  conflict_table = xcalloc (table_size, அளவு *conflict_table);
  check = xnmalloc (table_size, அளவு *check);

  lowzero = 0;
  உயர்வு = 0;

  க்கு (முழு i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  க்கு (முழு i = 0; i < table_size; i++)
    check[i] = -1;

  க்கு (முழு i = 0; i < nentries; i++)
    {
      state_எண் s = matching_state (i);
      base_எண் place;

      எனில் (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      இல்லையெனில்
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, உயர்வு + 1, ACTION_எண்_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

வெறுமை
tables_generate (வெறுமை)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (அளவு nstates <= அளவு nvectors);
  verify (அளவு nnterms <= அளவு nvectors);

  nvectors = state_எண்_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, அளவு *froms);
  tos = xcalloc (nvectors, அளவு *tos);
  conflict_tos = xcalloc (nvectors, அளவு *conflict_tos);
  tally = xcalloc (nvectors, அளவு *tally);
  width = xnmalloc (nvectors, அளவு *width);

  token_actions ();

  goto_actions ();
  விடுவி (goto_map);
  விடுவி (from_state);
  விடுவி (to_state);

  order = xcalloc (nvectors, அளவு *order);
  sort_actions ();
  pack_table ();
  விடுவி (order);

  விடுவி (tally);
  விடுவி (width);

  க்கு (முழு i = 0; i < nvectors; i++)
    {
      விடுவி (froms[i]);
      விடுவி (tos[i]);
      விடுவி (conflict_tos[i]);
    }

  விடுவி (froms);
  விடுவி (tos);
  விடுவி (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

வெறுமை
tables_free (வெறுமை)
{
  விடுவி (base);
  விடுவி (conflict_table);
  விடுவி (conflict_list);
  விடுவி (table);
  விடுவி (check);
  விடுவி (yydefgoto);
  விடுவி (yydefact);
}
