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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <bitsetv.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "conflicts.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "symtab.h"
#अन्तर्भाव "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
प्रकारव्याख्या पूर्णांक vector_संख्या;

#जर 0 /* Not currently used.  */
स्थिर ओळीत vector_संख्या
state_संख्या_to_vector_संख्या (state_संख्या s)
{
  परत s;
}
#endif

स्थिर ओळीत vector_संख्या
चिन्ह_संख्या_to_vector_संख्या (चिन्ह_संख्या sym)
{
  परत state_संख्या_as_int (nstates) + sym - ntokens;
}

पूर्णांक nvectors;


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
#define BASE_MAXIMUM पूर्णांक_उच्च
#define BASE_MINIMUM पूर्णांक_न्यून

स्थिर base_संख्या **froms;
स्थिर base_संख्या **tos;
स्थिर पूर्णांक **conflict_tos;
स्थिर माप_प्रकार *tally;
स्थिर base_संख्या *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
प्रकारव्याख्या पूर्णांक action_संख्या;
#define ACTION_संख्या_MINIMUM पूर्णांक_न्यून

स्थिर action_संख्या *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
स्थिर vector_संख्या *order;
स्थिर पूर्णांक nentries;

base_संख्या *base = रिक्त;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_संख्या base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
स्थिर bitset pos_set = रिक्त;
/* The integer denoted by bitno 0 in pos_set.  */
स्थिर पूर्णांक pos_set_base = 0;

स्थिर पूर्णांक *conflrow;
पूर्णांक *conflict_table;
पूर्णांक *conflict_list;
पूर्णांक conflict_list_cnt;
स्थिर पूर्णांक conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
स्थिर पूर्णांक table_size = 32768;
base_संख्या *table;
base_संख्या *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_संख्या table_ninf = 0;
स्थिर पूर्णांक lowzero;
पूर्णांक वरचा;

state_संख्या *yydefgoto;
rule_संख्या *yydefact;


/*----------.
| pos_set.  |
`----------*/

#जर 0
स्थिर शून्य
pos_set_dump (शून्य)
{
  ख_लिखो (मानक_त्रुटि, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  पूर्णांक i;
  BITSET_FOR_EACH (biter, pos_set, i, 0)
    ख_लिखो (मानक_त्रुटि, " %d", i + pos_set_base);
  अ_दो ('\n', मानक_त्रुटि);
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

स्थिर शून्य
pos_set_set (पूर्णांक pos)
{
  पूर्णांक bitno = pos - pos_set_base;
  जर (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      स्थिरांक पूर्णांक delta = pos_set_base - pos;
      स्थिरांक पूर्णांक old_size = bitset_size (pos_set);
      स्थिरांक पूर्णांक new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      साठी (पूर्णांक i = new_size - 1; 0 <= i ; --i)
        जर (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        अन्यथा
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  अन्यथा जर (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

स्थिर bool
pos_set_test (पूर्णांक pos)
{
  स्थिरांक पूर्णांक bitno = pos - pos_set_base;
  परत bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

स्थिर शून्य
table_grow (पूर्णांक desired)
{
  पूर्णांक old_size = table_size;

  पर्यंत (table_size <= desired)
    table_size *= 2;

  जर (trace_flag & trace_resource)
    ख_लिखो (मानक_त्रुटि, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, आकार *table);
  स_रखो (table + old_size, 0,
          आकार *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              आकार *conflict_table);
  स_रखो (conflict_table + old_size, 0,
          आकार *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, आकार *check);
  साठी (पूर्णांक i = old_size; i < table_size; ++i)
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

स्थिर शून्य
conflict_row (state *s)
{
  जर (!nondeterministic_parser)
    परत;

  स्थिरांक reductions *reds = s->reductions;
  साठी (state_संख्या j = 0; j < ntokens; j += 1)
    जर (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        साठी (पूर्णांक i = 0; i < reds->अंक; i += 1)
          जर (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_संख्या_as_item_संख्या (reds->rules[i]->संख्या)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->संख्या + 1;
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

स्थिर rule *
action_row (state *s)
{
  साठी (state_संख्या i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  जर (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    साठी (पूर्णांक i = reds->अंक - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        पूर्णांक j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            जर (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_संख्या_as_item_संख्या (reds->rules[i]->संख्या);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    पूर्णांक i;
    FOR_EACH_SHIFT (trans, i)
      {
        चिन्ह_संख्या sym = TRANSITION_चिन्ह (trans, i);
        state *shift_state = trans->states[i];

        जर (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_संख्या_as_int (shift_state->संख्या);

        /* Do not use any default reduction if there is a shift for
           error */
        जर (sym == errtoken->content->संख्या)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  साठी (पूर्णांक i = 0; i < errp->अंक; i++)
    {
      चिन्ह *sym = errp->symbols[i];
      actrow[sym->content->संख्या] = ACTION_संख्या_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    अक्षर *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    जर (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    मुक्त (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = रिक्त;
  जर (reds->अंक >= 1 && !nodefault)
    {
      जर (s->consistent)
        default_reduction = reds->rules[0];
      अन्यथा
        {
          पूर्णांक max = 0;
          साठी (पूर्णांक i = 0; i < reds->अंक; i++)
            {
              पूर्णांक मोजणी = 0;
              rule *r = reds->rules[i];
              साठी (चिन्ह_संख्या j = 0; j < ntokens; j++)
                जर (actrow[j] == rule_संख्या_as_item_संख्या (r->संख्या))
                  मोजणी++;

              जर (मोजणी > max)
                {
                  max = मोजणी;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          जर (0 < max)
            साठी (चिन्ह_संख्या j = 0; j < ntokens; j++)
              जर (actrow[j]
                  == rule_संख्या_as_item_संख्या (default_reduction->संख्या)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  जर (!default_reduction)
    साठी (चिन्ह_संख्या i = 0; i < ntokens; i++)
      जर (actrow[i] == ACTION_संख्या_MINIMUM)
        actrow[i] = 0;

  जर (conflicted)
    conflict_row (s);

  परत default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

स्थिर शून्य
save_row (state_संख्या s)
{
  /* Number of non default actions in S.  */
  माप_प्रकार मोजणी = 0;
  साठी (चिन्ह_संख्या i = 0; i < ntokens; i++)
    जर (actrow[i] != 0)
      मोजणी++;

  जर (मोजणी)
    {
      /* Allocate non defaulted actions.  */
      base_संख्या *sp1 = froms[s] = xnmalloc (मोजणी, आकार *sp1);
      base_संख्या *sp2 = tos[s] = xnmalloc (मोजणी, आकार *sp2);
      पूर्णांक *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (मोजणी, आकार *sp3) : रिक्त;

      /* Store non defaulted actions.  */
      साठी (चिन्ह_संख्या i = 0; i < ntokens; i++)
        जर (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            जर (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = मोजणी;
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

स्थिर शून्य
token_actions (शून्य)
{
  पूर्णांक nconflict = nondeterministic_parser ? conflicts_total_मोजणी () : 0;

  yydefact = xnmalloc (nstates, आकार *yydefact);

  actrow = xnmalloc (ntokens, आकार *actrow);
  conflrow = xnmalloc (ntokens, आकार *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, आकार *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  जर (!nondeterministic_parser)
    साठी (rule_संख्या r = 0; r < nrules; ++r)
      rules[r].useful = false;

  साठी (state_संख्या i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->संख्या + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      जर (!nondeterministic_parser)
        {
          साठी (चिन्ह_संख्या j = 0; j < ntokens; ++j)
            जर (actrow[j] < 0 && actrow[j] != ACTION_संख्या_MINIMUM)
              rules[item_संख्या_as_rule_संख्या (actrow[j])].useful = true;
          जर (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  मुक्त (actrow);
  मुक्त (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

स्थिर शून्य
save_column (चिन्ह_संख्या sym, state_संख्या default_state)
{
  स्थिरांक goto_संख्या begin = goto_map[sym - ntokens];
  स्थिरांक goto_संख्या end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  माप_प्रकार मोजणी = 0;
  साठी (goto_संख्या i = begin; i < end; i++)
    जर (to_state[i] != default_state)
      मोजणी++;

  जर (मोजणी)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_संख्या symno = चिन्ह_संख्या_to_vector_संख्या (sym);
      base_संख्या *sp1 = froms[symno] = xnmalloc (मोजणी, आकार *sp1);
      base_संख्या *sp2 = tos[symno] = xnmalloc (मोजणी, आकार *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      साठी (goto_संख्या i = begin; i < end; i++)
        जर (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = मोजणी;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

स्थिर state_संख्या
default_goto (चिन्ह_संख्या sym, माप_प्रकार state_मोजणी[])
{
  स्थिरांक goto_संख्या begin = goto_map[sym - ntokens];
  स्थिरांक goto_संख्या end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_संख्या res = 0;

  जर (begin != end)
    {
      साठी (state_संख्या s = 0; s < nstates; s++)
        state_मोजणी[s] = 0;

      साठी (goto_संख्या i = begin; i < end; i++)
        state_मोजणी[to_state[i]]++;

      माप_प्रकार max = 0;
      साठी (state_संख्या s = 0; s < nstates; s++)
        जर (max < state_मोजणी[s])
          {
            max = state_मोजणी[s];
            res = s;
          }
    }
  परत res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

स्थिर शून्य
goto_actions (शून्य)
{
  माप_प्रकार *state_मोजणी = xnmalloc (nstates, आकार *state_मोजणी);
  yydefgoto = xnmalloc (nnterms, आकार *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  साठी (चिन्ह_संख्या i = ntokens; i < nsyms; ++i)
    {
      state_संख्या default_state = default_goto (i, state_मोजणी);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  मुक्त (state_मोजणी);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

स्थिर शून्य
sort_actions (शून्य)
{
  nentries = 0;

  साठी (पूर्णांक i = 0; i < nvectors; i++)
    जर (0 < tally[i])
      {
        स्थिरांक माप_प्रकार t = tally[i];
        स्थिरांक पूर्णांक w = width[i];
        पूर्णांक j = nentries - 1;

        पर्यंत (0 <= j && width[order[j]] < w)
          j--;

        पर्यंत (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        साठी (पूर्णांक k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

स्थिर state_संख्या
matching_state (vector_संख्या vector)
{
  vector_संख्या i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  जर (i < nstates)
    {
      माप_प्रकार t = tally[i];
      पूर्णांक w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      जर (conflict_tos[i] != रिक्त)
        साठी (पूर्णांक j = 0; j < t; j += 1)
          जर (conflict_tos[i][j] != 0)
            परत -1;

      साठी (पूर्णांक prev = vector - 1; 0 <= prev; prev--)
        {
          vector_संख्या j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          जर (width[j] != w || tally[j] != t)
            परत -1;
          अन्यथा
            {
              bool match = true;
              साठी (पूर्णांक k = 0; match && k < t; k++)
                जर (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != रिक्त && conflict_tos[j][k] != 0))
                  match = false;
              जर (match)
                परत j;
            }
        }
    }
  परत -1;
}


स्थिर base_संख्या
pack_vector (vector_संख्या vector)
{
  vector_संख्या i = order[vector];
  माप_प्रकार t = tally[i];
  base_संख्या *from = froms[i];
  base_संख्या *to = tos[i];
  पूर्णांक *conflict_to = conflict_tos[i];

  aver (t != 0);

  साठी (base_संख्या res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        साठी (पूर्णांक k = 0; ok && k < t; k++)
          {
            पूर्णांक loc = res + state_संख्या_as_int (from[k]);
            जर (table_size <= loc)
              table_grow (loc);

            जर (table[loc] != 0)
              ok = false;
          }

        जर (ok && pos_set_test (res))
          ok = false;
      }

      जर (ok)
        {
          पूर्णांक loc PACIFY_CC (= -1);
          साठी (पूर्णांक k = 0; k < t; k++)
            {
              loc = res + state_संख्या_as_int (from[k]);
              table[loc] = to[k];
              जर (nondeterministic_parser && conflict_to != रिक्त)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          पर्यंत (table[lowzero] != 0)
            lowzero++;

          जर (वरचा < loc)
            वरचा = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          परत res;
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

स्थिर base_संख्या
table_ninf_remap (base_संख्या tab[], पूर्णांक size, base_संख्या ninf)
{
  base_संख्या res = 0;

  साठी (पूर्णांक i = 0; i < size; i++)
    जर (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  साठी (पूर्णांक i = 0; i < size; i++)
    जर (tab[i] == ninf)
      tab[i] = res;

  परत res;
}

स्थिर शून्य
pack_table (शून्य)
{
  base = xnmalloc (nvectors, आकार *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, आकार *table);
  conflict_table = xcalloc (table_size, आकार *conflict_table);
  check = xnmalloc (table_size, आकार *check);

  lowzero = 0;
  वरचा = 0;

  साठी (पूर्णांक i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  साठी (पूर्णांक i = 0; i < table_size; i++)
    check[i] = -1;

  साठी (पूर्णांक i = 0; i < nentries; i++)
    {
      state_संख्या s = matching_state (i);
      base_संख्या place;

      जर (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      अन्यथा
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, वरचा + 1, ACTION_संख्या_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

शून्य
tables_generate (शून्य)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (आकार nstates <= आकार nvectors);
  verify (आकार nnterms <= आकार nvectors);

  nvectors = state_संख्या_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, आकार *froms);
  tos = xcalloc (nvectors, आकार *tos);
  conflict_tos = xcalloc (nvectors, आकार *conflict_tos);
  tally = xcalloc (nvectors, आकार *tally);
  width = xnmalloc (nvectors, आकार *width);

  token_actions ();

  goto_actions ();
  मुक्त (goto_map);
  मुक्त (from_state);
  मुक्त (to_state);

  order = xcalloc (nvectors, आकार *order);
  sort_actions ();
  pack_table ();
  मुक्त (order);

  मुक्त (tally);
  मुक्त (width);

  साठी (पूर्णांक i = 0; i < nvectors; i++)
    {
      मुक्त (froms[i]);
      मुक्त (tos[i]);
      मुक्त (conflict_tos[i]);
    }

  मुक्त (froms);
  मुक्त (tos);
  मुक्त (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

शून्य
tables_free (शून्य)
{
  मुक्त (base);
  मुक्त (conflict_table);
  मुक्त (conflict_list);
  मुक्त (table);
  मुक्त (check);
  मुक्त (yydefgoto);
  मुक्त (yydefact);
}
