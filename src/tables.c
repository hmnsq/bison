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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>
#شامل <bitsetv.h>

#شامل "complain.h"
#شامل "conflicts.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "muscle-tab.h"
#شامل "reader.h"
#شامل "symtab.h"
#شامل "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
تعریفنوع صحیح vector_عدد;

#اگر 0 /* Not currently used.  */
ایستا درونخطی vector_عدد
state_عدد_to_vector_عدد (state_عدد s)
{
  بازگشت s;
}
#endif

ایستا درونخطی vector_عدد
نماد_عدد_to_vector_عدد (نماد_عدد sym)
{
  بازگشت state_عدد_as_int (nstates) + sym - ntokens;
}

صحیح nvectors;


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

ایستا base_عدد **froms;
ایستا base_عدد **tos;
ایستا صحیح **conflict_tos;
ایستا size_t *tally;
ایستا base_عدد *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
تعریفنوع صحیح action_عدد;
#define ACTION_عدد_MINIMUM INT_MIN

ایستا action_عدد *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
ایستا vector_عدد *order;
ایستا صحیح nentries;

base_عدد *base = پوچ;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_عدد base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
ایستا bitset pos_set = پوچ;
/* The integer denoted by bitno 0 in pos_set.  */
ایستا صحیح pos_set_base = 0;

ایستا صحیح *conflrow;
صحیح *conflict_table;
صحیح *conflict_list;
صحیح conflict_list_cnt;
ایستا صحیح conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
ایستا صحیح table_size = 32768;
base_عدد *table;
base_عدد *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_عدد table_ninf = 0;
ایستا صحیح lowzero;
صحیح بالا;

state_عدد *yydefgoto;
rule_عدد *yydefact;


/*----------.
| pos_set.  |
`----------*/

#اگر 0
ایستا تهی
pos_set_dump (تهی)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  صحیح i;
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

ایستا تهی
pos_set_set (صحیح pos)
{
  صحیح bitno = pos - pos_set_base;
  اگر (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      ثابت صحیح delta = pos_set_base - pos;
      ثابت صحیح old_size = bitset_size (pos_set);
      ثابت صحیح new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      برای (صحیح i = new_size - 1; 0 <= i ; --i)
        اگر (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        وگرنه
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  وگرنه اگر (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

ایستا bool
pos_set_test (صحیح pos)
{
  ثابت صحیح bitno = pos - pos_set_base;
  بازگشت bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

ایستا تهی
table_grow (صحیح desired)
{
  صحیح old_size = table_size;

  وقتی (table_size <= desired)
    table_size *= 2;

  اگر (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, اندازه *table);
  memset (table + old_size, 0,
          اندازه *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              اندازه *conflict_table);
  memset (conflict_table + old_size, 0,
          اندازه *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, اندازه *check);
  برای (صحیح i = old_size; i < table_size; ++i)
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

ایستا تهی
conflict_row (state *s)
{
  اگر (!nondeterministic_parser)
    بازگشت;

  ثابت reductions *reds = s->reductions;
  برای (state_عدد j = 0; j < ntokens; j += 1)
    اگر (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        برای (صحیح i = 0; i < reds->رقم; i += 1)
          اگر (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_عدد_as_item_عدد (reds->rules[i]->عدد)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->عدد + 1;
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

ایستا rule *
action_row (state *s)
{
  برای (state_عدد i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  اگر (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    برای (صحیح i = reds->رقم - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        صحیح j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            اگر (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_عدد_as_item_عدد (reds->rules[i]->عدد);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    صحیح i;
    FOR_EACH_SHIFT (trans, i)
      {
        نماد_عدد sym = TRANSITION_نماد (trans, i);
        state *shift_state = trans->states[i];

        اگر (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_عدد_as_int (shift_state->عدد);

        /* Do not use any default reduction if there is a shift for
           error */
        اگر (sym == errtoken->content->عدد)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  برای (صحیح i = 0; i < errp->رقم; i++)
    {
      نماد *sym = errp->symbols[i];
      actrow[sym->content->عدد] = ACTION_عدد_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    نویسه *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    اگر (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    آزاد (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = پوچ;
  اگر (reds->رقم >= 1 && !nodefault)
    {
      اگر (s->consistent)
        default_reduction = reds->rules[0];
      وگرنه
        {
          صحیح max = 0;
          برای (صحیح i = 0; i < reds->رقم; i++)
            {
              صحیح شمار = 0;
              rule *r = reds->rules[i];
              برای (نماد_عدد j = 0; j < ntokens; j++)
                اگر (actrow[j] == rule_عدد_as_item_عدد (r->عدد))
                  شمار++;

              اگر (شمار > max)
                {
                  max = شمار;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          اگر (0 < max)
            برای (نماد_عدد j = 0; j < ntokens; j++)
              اگر (actrow[j]
                  == rule_عدد_as_item_عدد (default_reduction->عدد)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  اگر (!default_reduction)
    برای (نماد_عدد i = 0; i < ntokens; i++)
      اگر (actrow[i] == ACTION_عدد_MINIMUM)
        actrow[i] = 0;

  اگر (conflicted)
    conflict_row (s);

  بازگشت default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

ایستا تهی
save_row (state_عدد s)
{
  /* Number of non default actions in S.  */
  size_t شمار = 0;
  برای (نماد_عدد i = 0; i < ntokens; i++)
    اگر (actrow[i] != 0)
      شمار++;

  اگر (شمار)
    {
      /* Allocate non defaulted actions.  */
      base_عدد *sp1 = froms[s] = xnmalloc (شمار, اندازه *sp1);
      base_عدد *sp2 = tos[s] = xnmalloc (شمار, اندازه *sp2);
      صحیح *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (شمار, اندازه *sp3) : پوچ;

      /* Store non defaulted actions.  */
      برای (نماد_عدد i = 0; i < ntokens; i++)
        اگر (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            اگر (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = شمار;
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

ایستا تهی
token_actions (تهی)
{
  صحیح nconflict = nondeterministic_parser ? conflicts_total_شمار () : 0;

  yydefact = xnmalloc (nstates, اندازه *yydefact);

  actrow = xnmalloc (ntokens, اندازه *actrow);
  conflrow = xnmalloc (ntokens, اندازه *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, اندازه *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  اگر (!nondeterministic_parser)
    برای (rule_عدد r = 0; r < nrules; ++r)
      rules[r].useful = false;

  برای (state_عدد i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->عدد + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      اگر (!nondeterministic_parser)
        {
          برای (نماد_عدد j = 0; j < ntokens; ++j)
            اگر (actrow[j] < 0 && actrow[j] != ACTION_عدد_MINIMUM)
              rules[item_عدد_as_rule_عدد (actrow[j])].useful = true;
          اگر (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  آزاد (actrow);
  آزاد (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

ایستا تهی
save_column (نماد_عدد sym, state_عدد default_state)
{
  ثابت goto_عدد begin = goto_map[sym - ntokens];
  ثابت goto_عدد end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t شمار = 0;
  برای (goto_عدد i = begin; i < end; i++)
    اگر (to_state[i] != default_state)
      شمار++;

  اگر (شمار)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_عدد symno = نماد_عدد_to_vector_عدد (sym);
      base_عدد *sp1 = froms[symno] = xnmalloc (شمار, اندازه *sp1);
      base_عدد *sp2 = tos[symno] = xnmalloc (شمار, اندازه *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      برای (goto_عدد i = begin; i < end; i++)
        اگر (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = شمار;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

ایستا state_عدد
default_goto (نماد_عدد sym, size_t state_شمار[])
{
  ثابت goto_عدد begin = goto_map[sym - ntokens];
  ثابت goto_عدد end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_عدد res = 0;

  اگر (begin != end)
    {
      برای (state_عدد s = 0; s < nstates; s++)
        state_شمار[s] = 0;

      برای (goto_عدد i = begin; i < end; i++)
        state_شمار[to_state[i]]++;

      size_t max = 0;
      برای (state_عدد s = 0; s < nstates; s++)
        اگر (max < state_شمار[s])
          {
            max = state_شمار[s];
            res = s;
          }
    }
  بازگشت res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

ایستا تهی
goto_actions (تهی)
{
  size_t *state_شمار = xnmalloc (nstates, اندازه *state_شمار);
  yydefgoto = xnmalloc (nnterms, اندازه *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    {
      state_عدد default_state = default_goto (i, state_شمار);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  آزاد (state_شمار);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

ایستا تهی
sort_actions (تهی)
{
  nentries = 0;

  برای (صحیح i = 0; i < nvectors; i++)
    اگر (0 < tally[i])
      {
        ثابت size_t t = tally[i];
        ثابت صحیح w = width[i];
        صحیح j = nentries - 1;

        وقتی (0 <= j && width[order[j]] < w)
          j--;

        وقتی (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        برای (صحیح k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

ایستا state_عدد
matching_state (vector_عدد vector)
{
  vector_عدد i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  اگر (i < nstates)
    {
      size_t t = tally[i];
      صحیح w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      اگر (conflict_tos[i] != پوچ)
        برای (صحیح j = 0; j < t; j += 1)
          اگر (conflict_tos[i][j] != 0)
            بازگشت -1;

      برای (صحیح prev = vector - 1; 0 <= prev; prev--)
        {
          vector_عدد j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          اگر (width[j] != w || tally[j] != t)
            بازگشت -1;
          وگرنه
            {
              bool match = true;
              برای (صحیح k = 0; match && k < t; k++)
                اگر (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != پوچ && conflict_tos[j][k] != 0))
                  match = false;
              اگر (match)
                بازگشت j;
            }
        }
    }
  بازگشت -1;
}


ایستا base_عدد
pack_vector (vector_عدد vector)
{
  vector_عدد i = order[vector];
  size_t t = tally[i];
  base_عدد *from = froms[i];
  base_عدد *to = tos[i];
  صحیح *conflict_to = conflict_tos[i];

  aver (t != 0);

  برای (base_عدد res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        برای (صحیح k = 0; ok && k < t; k++)
          {
            صحیح loc = res + state_عدد_as_int (from[k]);
            اگر (table_size <= loc)
              table_grow (loc);

            اگر (table[loc] != 0)
              ok = false;
          }

        اگر (ok && pos_set_test (res))
          ok = false;
      }

      اگر (ok)
        {
          صحیح loc PACIFY_CC (= -1);
          برای (صحیح k = 0; k < t; k++)
            {
              loc = res + state_عدد_as_int (from[k]);
              table[loc] = to[k];
              اگر (nondeterministic_parser && conflict_to != پوچ)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          وقتی (table[lowzero] != 0)
            lowzero++;

          اگر (بالا < loc)
            بالا = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          بازگشت res;
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

ایستا base_عدد
table_ninf_remap (base_عدد tab[], صحیح size, base_عدد ninf)
{
  base_عدد res = 0;

  برای (صحیح i = 0; i < size; i++)
    اگر (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  برای (صحیح i = 0; i < size; i++)
    اگر (tab[i] == ninf)
      tab[i] = res;

  بازگشت res;
}

ایستا تهی
pack_table (تهی)
{
  base = xnmalloc (nvectors, اندازه *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, اندازه *table);
  conflict_table = xcalloc (table_size, اندازه *conflict_table);
  check = xnmalloc (table_size, اندازه *check);

  lowzero = 0;
  بالا = 0;

  برای (صحیح i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  برای (صحیح i = 0; i < table_size; i++)
    check[i] = -1;

  برای (صحیح i = 0; i < nentries; i++)
    {
      state_عدد s = matching_state (i);
      base_عدد place;

      اگر (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      وگرنه
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, بالا + 1, ACTION_عدد_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

تهی
tables_generate (تهی)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (اندازه nstates <= اندازه nvectors);
  verify (اندازه nnterms <= اندازه nvectors);

  nvectors = state_عدد_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, اندازه *froms);
  tos = xcalloc (nvectors, اندازه *tos);
  conflict_tos = xcalloc (nvectors, اندازه *conflict_tos);
  tally = xcalloc (nvectors, اندازه *tally);
  width = xnmalloc (nvectors, اندازه *width);

  token_actions ();

  goto_actions ();
  آزاد (goto_map);
  آزاد (from_state);
  آزاد (to_state);

  order = xcalloc (nvectors, اندازه *order);
  sort_actions ();
  pack_table ();
  آزاد (order);

  آزاد (tally);
  آزاد (width);

  برای (صحیح i = 0; i < nvectors; i++)
    {
      آزاد (froms[i]);
      آزاد (tos[i]);
      آزاد (conflict_tos[i]);
    }

  آزاد (froms);
  آزاد (tos);
  آزاد (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

تهی
tables_free (تهی)
{
  آزاد (base);
  آزاد (conflict_table);
  آزاد (conflict_list);
  آزاد (table);
  آزاد (check);
  آزاد (yydefgoto);
  آزاد (yydefact);
}
