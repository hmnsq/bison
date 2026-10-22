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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <bitsetv.h>

#ضمن "complain.h"
#ضمن "conflicts.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "muscle-tab.h"
#ضمن "reader.h"
#ضمن "symtab.h"
#ضمن "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
تعريفنوع صحيح vector_نمبر;

#إذا 0 /* Not currently used.  */
ساكن مضمن vector_نمبر
state_نمبر_to_vector_نمبر (state_نمبر s)
{
  أرجع s;
}
#endif

ساكن مضمن vector_نمبر
علامت_نمبر_to_vector_نمبر (علامت_نمبر sym)
{
  أرجع state_نمبر_as_int (nstates) + sym - ntokens;
}

صحيح nvectors;


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

ساكن base_نمبر **froms;
ساكن base_نمبر **tos;
ساكن صحيح **conflict_tos;
ساكن size_t *tally;
ساكن base_نمبر *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
تعريفنوع صحيح action_نمبر;
#define ACTION_نمبر_MINIMUM INT_MIN

ساكن action_نمبر *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
ساكن vector_نمبر *order;
ساكن صحيح nentries;

base_نمبر *base = عدم;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_نمبر base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
ساكن bitset pos_set = عدم;
/* The integer denoted by bitno 0 in pos_set.  */
ساكن صحيح pos_set_base = 0;

ساكن صحيح *conflrow;
صحيح *conflict_table;
صحيح *conflict_list;
صحيح conflict_list_cnt;
ساكن صحيح conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
ساكن صحيح table_size = 32768;
base_نمبر *table;
base_نمبر *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_نمبر table_ninf = 0;
ساكن صحيح lowzero;
صحيح بلند;

state_نمبر *yydefgoto;
rule_نمبر *yydefact;


/*----------.
| pos_set.  |
`----------*/

#إذا 0
ساكن فراغ
pos_set_dump (فراغ)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  صحيح i;
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

ساكن فراغ
pos_set_set (صحيح pos)
{
  صحيح bitno = pos - pos_set_base;
  إذا (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      ثابت صحيح delta = pos_set_base - pos;
      ثابت صحيح old_size = bitset_size (pos_set);
      ثابت صحيح new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      لكل (صحيح i = new_size - 1; 0 <= i ; --i)
        إذا (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        وإلا
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  وإلا إذا (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

ساكن bool
pos_set_test (صحيح pos)
{
  ثابت صحيح bitno = pos - pos_set_base;
  أرجع bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

ساكن فراغ
table_grow (صحيح desired)
{
  صحيح old_size = table_size;

  طالما (table_size <= desired)
    table_size *= 2;

  إذا (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, حجم *table);
  memset (table + old_size, 0,
          حجم *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              حجم *conflict_table);
  memset (conflict_table + old_size, 0,
          حجم *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, حجم *check);
  لكل (صحيح i = old_size; i < table_size; ++i)
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

ساكن فراغ
conflict_row (state *s)
{
  إذا (!nondeterministic_parser)
    أرجع;

  ثابت reductions *reds = s->reductions;
  لكل (state_نمبر j = 0; j < ntokens; j += 1)
    إذا (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        لكل (صحيح i = 0; i < reds->عدد; i += 1)
          إذا (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_نمبر_as_item_نمبر (reds->rules[i]->نمبر)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->نمبر + 1;
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

ساكن rule *
action_row (state *s)
{
  لكل (state_نمبر i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  إذا (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    لكل (صحيح i = reds->عدد - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        صحيح j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            إذا (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_نمبر_as_item_نمبر (reds->rules[i]->نمبر);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    صحيح i;
    FOR_EACH_SHIFT (trans, i)
      {
        علامت_نمبر sym = TRANSITION_علامت (trans, i);
        state *shift_state = trans->states[i];

        إذا (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_نمبر_as_int (shift_state->نمبر);

        /* Do not use any default reduction if there is a shift for
           error */
        إذا (sym == errtoken->content->نمبر)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  لكل (صحيح i = 0; i < errp->عدد; i++)
    {
      علامت *sym = errp->symbols[i];
      actrow[sym->content->نمبر] = ACTION_نمبر_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    حرف *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    إذا (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    حرر (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = عدم;
  إذا (reds->عدد >= 1 && !nodefault)
    {
      إذا (s->consistent)
        default_reduction = reds->rules[0];
      وإلا
        {
          صحيح max = 0;
          لكل (صحيح i = 0; i < reds->عدد; i++)
            {
              صحيح گنتی = 0;
              rule *r = reds->rules[i];
              لكل (علامت_نمبر j = 0; j < ntokens; j++)
                إذا (actrow[j] == rule_نمبر_as_item_نمبر (r->نمبر))
                  گنتی++;

              إذا (گنتی > max)
                {
                  max = گنتی;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          إذا (0 < max)
            لكل (علامت_نمبر j = 0; j < ntokens; j++)
              إذا (actrow[j]
                  == rule_نمبر_as_item_نمبر (default_reduction->نمبر)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  إذا (!default_reduction)
    لكل (علامت_نمبر i = 0; i < ntokens; i++)
      إذا (actrow[i] == ACTION_نمبر_MINIMUM)
        actrow[i] = 0;

  إذا (conflicted)
    conflict_row (s);

  أرجع default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

ساكن فراغ
save_row (state_نمبر s)
{
  /* Number of non default actions in S.  */
  size_t گنتی = 0;
  لكل (علامت_نمبر i = 0; i < ntokens; i++)
    إذا (actrow[i] != 0)
      گنتی++;

  إذا (گنتی)
    {
      /* Allocate non defaulted actions.  */
      base_نمبر *sp1 = froms[s] = xnmalloc (گنتی, حجم *sp1);
      base_نمبر *sp2 = tos[s] = xnmalloc (گنتی, حجم *sp2);
      صحيح *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (گنتی, حجم *sp3) : عدم;

      /* Store non defaulted actions.  */
      لكل (علامت_نمبر i = 0; i < ntokens; i++)
        إذا (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            إذا (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = گنتی;
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

ساكن فراغ
token_actions (فراغ)
{
  صحيح nconflict = nondeterministic_parser ? conflicts_total_گنتی () : 0;

  yydefact = xnmalloc (nstates, حجم *yydefact);

  actrow = xnmalloc (ntokens, حجم *actrow);
  conflrow = xnmalloc (ntokens, حجم *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, حجم *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  إذا (!nondeterministic_parser)
    لكل (rule_نمبر r = 0; r < nrules; ++r)
      rules[r].useful = false;

  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->نمبر + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      إذا (!nondeterministic_parser)
        {
          لكل (علامت_نمبر j = 0; j < ntokens; ++j)
            إذا (actrow[j] < 0 && actrow[j] != ACTION_نمبر_MINIMUM)
              rules[item_نمبر_as_rule_نمبر (actrow[j])].useful = true;
          إذا (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  حرر (actrow);
  حرر (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

ساكن فراغ
save_column (علامت_نمبر sym, state_نمبر default_state)
{
  ثابت goto_نمبر begin = goto_map[sym - ntokens];
  ثابت goto_نمبر end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t گنتی = 0;
  لكل (goto_نمبر i = begin; i < end; i++)
    إذا (to_state[i] != default_state)
      گنتی++;

  إذا (گنتی)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_نمبر symno = علامت_نمبر_to_vector_نمبر (sym);
      base_نمبر *sp1 = froms[symno] = xnmalloc (گنتی, حجم *sp1);
      base_نمبر *sp2 = tos[symno] = xnmalloc (گنتی, حجم *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      لكل (goto_نمبر i = begin; i < end; i++)
        إذا (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = گنتی;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

ساكن state_نمبر
default_goto (علامت_نمبر sym, size_t state_گنتی[])
{
  ثابت goto_نمبر begin = goto_map[sym - ntokens];
  ثابت goto_نمبر end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_نمبر res = 0;

  إذا (begin != end)
    {
      لكل (state_نمبر s = 0; s < nstates; s++)
        state_گنتی[s] = 0;

      لكل (goto_نمبر i = begin; i < end; i++)
        state_گنتی[to_state[i]]++;

      size_t max = 0;
      لكل (state_نمبر s = 0; s < nstates; s++)
        إذا (max < state_گنتی[s])
          {
            max = state_گنتی[s];
            res = s;
          }
    }
  أرجع res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

ساكن فراغ
goto_actions (فراغ)
{
  size_t *state_گنتی = xnmalloc (nstates, حجم *state_گنتی);
  yydefgoto = xnmalloc (nnterms, حجم *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  لكل (علامت_نمبر i = ntokens; i < nsyms; ++i)
    {
      state_نمبر default_state = default_goto (i, state_گنتی);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  حرر (state_گنتی);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

ساكن فراغ
sort_actions (فراغ)
{
  nentries = 0;

  لكل (صحيح i = 0; i < nvectors; i++)
    إذا (0 < tally[i])
      {
        ثابت size_t t = tally[i];
        ثابت صحيح w = width[i];
        صحيح j = nentries - 1;

        طالما (0 <= j && width[order[j]] < w)
          j--;

        طالما (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        لكل (صحيح k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

ساكن state_نمبر
matching_state (vector_نمبر vector)
{
  vector_نمبر i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  إذا (i < nstates)
    {
      size_t t = tally[i];
      صحيح w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      إذا (conflict_tos[i] != عدم)
        لكل (صحيح j = 0; j < t; j += 1)
          إذا (conflict_tos[i][j] != 0)
            أرجع -1;

      لكل (صحيح prev = vector - 1; 0 <= prev; prev--)
        {
          vector_نمبر j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          إذا (width[j] != w || tally[j] != t)
            أرجع -1;
          وإلا
            {
              bool match = true;
              لكل (صحيح k = 0; match && k < t; k++)
                إذا (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != عدم && conflict_tos[j][k] != 0))
                  match = false;
              إذا (match)
                أرجع j;
            }
        }
    }
  أرجع -1;
}


ساكن base_نمبر
pack_vector (vector_نمبر vector)
{
  vector_نمبر i = order[vector];
  size_t t = tally[i];
  base_نمبر *from = froms[i];
  base_نمبر *to = tos[i];
  صحيح *conflict_to = conflict_tos[i];

  aver (t != 0);

  لكل (base_نمبر res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        لكل (صحيح k = 0; ok && k < t; k++)
          {
            صحيح loc = res + state_نمبر_as_int (from[k]);
            إذا (table_size <= loc)
              table_grow (loc);

            إذا (table[loc] != 0)
              ok = false;
          }

        إذا (ok && pos_set_test (res))
          ok = false;
      }

      إذا (ok)
        {
          صحيح loc PACIFY_CC (= -1);
          لكل (صحيح k = 0; k < t; k++)
            {
              loc = res + state_نمبر_as_int (from[k]);
              table[loc] = to[k];
              إذا (nondeterministic_parser && conflict_to != عدم)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          طالما (table[lowzero] != 0)
            lowzero++;

          إذا (بلند < loc)
            بلند = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          أرجع res;
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

ساكن base_نمبر
table_ninf_remap (base_نمبر tab[], صحيح size, base_نمبر ninf)
{
  base_نمبر res = 0;

  لكل (صحيح i = 0; i < size; i++)
    إذا (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  لكل (صحيح i = 0; i < size; i++)
    إذا (tab[i] == ninf)
      tab[i] = res;

  أرجع res;
}

ساكن فراغ
pack_table (فراغ)
{
  base = xnmalloc (nvectors, حجم *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, حجم *table);
  conflict_table = xcalloc (table_size, حجم *conflict_table);
  check = xnmalloc (table_size, حجم *check);

  lowzero = 0;
  بلند = 0;

  لكل (صحيح i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  لكل (صحيح i = 0; i < table_size; i++)
    check[i] = -1;

  لكل (صحيح i = 0; i < nentries; i++)
    {
      state_نمبر s = matching_state (i);
      base_نمبر place;

      إذا (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      وإلا
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, بلند + 1, ACTION_نمبر_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

فراغ
tables_generate (فراغ)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (حجم nstates <= حجم nvectors);
  verify (حجم nnterms <= حجم nvectors);

  nvectors = state_نمبر_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, حجم *froms);
  tos = xcalloc (nvectors, حجم *tos);
  conflict_tos = xcalloc (nvectors, حجم *conflict_tos);
  tally = xcalloc (nvectors, حجم *tally);
  width = xnmalloc (nvectors, حجم *width);

  token_actions ();

  goto_actions ();
  حرر (goto_map);
  حرر (from_state);
  حرر (to_state);

  order = xcalloc (nvectors, حجم *order);
  sort_actions ();
  pack_table ();
  حرر (order);

  حرر (tally);
  حرر (width);

  لكل (صحيح i = 0; i < nvectors; i++)
    {
      حرر (froms[i]);
      حرر (tos[i]);
      حرر (conflict_tos[i]);
    }

  حرر (froms);
  حرر (tos);
  حرر (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

فراغ
tables_free (فراغ)
{
  حرر (base);
  حرر (conflict_table);
  حرر (conflict_list);
  حرر (table);
  حرر (check);
  حرر (yydefgoto);
  حرر (yydefact);
}
