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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <bitset.h>
#समावेश <bitsetv.h>

#समावेश "complain.h"
#समावेश "conflicts.h"
#समावेश "files.h"
#समावेश "getargs.h"
#समावेश "gram.h"
#समावेश "lalr.h"
#समावेश "muscle-tab.h"
#समावेश "reader.h"
#समावेश "symtab.h"
#समावेश "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
प्रकार पूर्णांक vector_संख्या;

#अगर 0 /* Not currently used.  */
अटल अंतरभूत vector_संख्या
state_संख्या_to_vector_संख्या (state_संख्या s)
{
  वापस s;
}
#पूर्ण_अगर

अटल अंतरभूत vector_संख्या
प्रतीक_संख्या_to_vector_संख्या (प्रतीक_संख्या sym)
{
  वापस state_संख्या_as_int (nstates) + sym - ntokens;
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
#घोषणा BASE_MAXIMUM पूर्णांक_उच्च
#घोषणा BASE_MINIMUM पूर्णांक_न्यून

अटल base_संख्या **froms;
अटल base_संख्या **tos;
अटल पूर्णांक **conflict_tos;
अटल माप_प्रकार *tally;
अटल base_संख्या *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
प्रकार पूर्णांक action_संख्या;
#घोषणा ACTION_संख्या_MINIMUM पूर्णांक_न्यून

अटल action_संख्या *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
अटल vector_संख्या *order;
अटल पूर्णांक nentries;

base_संख्या *base = शून्य;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_संख्या base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
अटल bitset pos_set = शून्य;
/* The integer denoted by bitno 0 in pos_set.  */
अटल पूर्णांक pos_set_base = 0;

अटल पूर्णांक *conflrow;
पूर्णांक *conflict_table;
पूर्णांक *conflict_list;
पूर्णांक conflict_list_cnt;
अटल पूर्णांक conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
अटल पूर्णांक table_आकार = 32768;
base_संख्या *table;
base_संख्या *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_संख्या table_ninf = 0;
अटल पूर्णांक lowzero;
पूर्णांक ऊपरी;

state_संख्या *yydefgoto;
rule_संख्या *yydefact;


/*----------.
| pos_set.  |
`----------*/

#अगर 0
अटल व्योम
pos_set_dump (व्योम)
{
  ख_लिखो (मानक_त्रुटि, "pos_set (%ld, %d) =", bitset_आकार (pos_set), pos_set_base);
  bitset_iterator biter;
  पूर्णांक i;
  BITSET_FOR_EACH (biter, pos_set, i, 0)
    ख_लिखो (मानक_त्रुटि, " %d", i + pos_set_base);
  अ_दो ('\n', मानक_त्रुटि);
}
#पूर्ण_अगर


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

अटल व्योम
pos_set_set (पूर्णांक pos)
{
  पूर्णांक bitno = pos - pos_set_base;
  अगर (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      स्थिर पूर्णांक delta = pos_set_base - pos;
      स्थिर पूर्णांक old_आकार = bitset_आकार (pos_set);
      स्थिर पूर्णांक new_आकार = old_आकार + delta;
      bitset_resize (pos_set, new_आकार);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      क्रम (पूर्णांक i = new_आकार - 1; 0 <= i ; --i)
        अगर (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        अन्यथा
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  अन्यथा अगर (bitset_आकार (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

अटल bool
pos_set_test (पूर्णांक pos)
{
  स्थिर पूर्णांक bitno = pos - pos_set_base;
  वापस bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

अटल व्योम
table_grow (पूर्णांक desired)
{
  पूर्णांक old_आकार = table_आकार;

  जबतक (table_आकार <= desired)
    table_आकार *= 2;

  अगर (trace_flag & trace_resource)
    ख_लिखो (मानक_त्रुटि, "growing tables from %d to %d\n",
             old_आकार, table_आकार);

  table = xnrealloc (table, table_आकार, माप *table);
  स_रखो (table + old_आकार, 0,
          माप *table * (table_आकार - old_आकार));

  conflict_table = xnrealloc (conflict_table, table_आकार,
                              माप *conflict_table);
  स_रखो (conflict_table + old_आकार, 0,
          माप *conflict_table * (table_आकार - old_आकार));

  check = xnrealloc (check, table_आकार, माप *check);
  क्रम (पूर्णांक i = old_आकार; i < table_आकार; ++i)
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

अटल व्योम
conflict_row (state *s)
{
  अगर (!nondeterministic_parser)
    वापस;

  स्थिर reductions *reds = s->reductions;
  क्रम (state_संख्या j = 0; j < ntokens; j += 1)
    अगर (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        क्रम (पूर्णांक i = 0; i < reds->अंक; i += 1)
          अगर (bitset_test (reds->lookaheads[i], j)
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

अटल rule *
action_row (state *s)
{
  क्रम (state_संख्या i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  अगर (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    क्रम (पूर्णांक i = reds->अंक - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        पूर्णांक j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            अगर (actrow[j] != 0)
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
        प्रतीक_संख्या sym = TRANSITION_प्रतीक (trans, i);
        state *shift_state = trans->states[i];

        अगर (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_संख्या_as_int (shift_state->संख्या);

        /* Do not use any default reduction if there is a shift for
           error */
        अगर (sym == errtoken->content->संख्या)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  क्रम (पूर्णांक i = 0; i < errp->अंक; i++)
    {
      प्रतीक *sym = errp->symbols[i];
      actrow[sym->content->संख्या] = ACTION_संख्या_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    अक्षर *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    अगर (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    मुक्त (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = शून्य;
  अगर (reds->अंक >= 1 && !nodefault)
    {
      अगर (s->consistent)
        default_reduction = reds->rules[0];
      अन्यथा
        {
          पूर्णांक max = 0;
          क्रम (पूर्णांक i = 0; i < reds->अंक; i++)
            {
              पूर्णांक गिनती = 0;
              rule *r = reds->rules[i];
              क्रम (प्रतीक_संख्या j = 0; j < ntokens; j++)
                अगर (actrow[j] == rule_संख्या_as_item_संख्या (r->संख्या))
                  गिनती++;

              अगर (गिनती > max)
                {
                  max = गिनती;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          अगर (0 < max)
            क्रम (प्रतीक_संख्या j = 0; j < ntokens; j++)
              अगर (actrow[j]
                  == rule_संख्या_as_item_संख्या (default_reduction->संख्या)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  अगर (!default_reduction)
    क्रम (प्रतीक_संख्या i = 0; i < ntokens; i++)
      अगर (actrow[i] == ACTION_संख्या_MINIMUM)
        actrow[i] = 0;

  अगर (conflicted)
    conflict_row (s);

  वापस default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

अटल व्योम
save_row (state_संख्या s)
{
  /* Number of non default actions in S.  */
  माप_प्रकार गिनती = 0;
  क्रम (प्रतीक_संख्या i = 0; i < ntokens; i++)
    अगर (actrow[i] != 0)
      गिनती++;

  अगर (गिनती)
    {
      /* Allocate non defaulted actions.  */
      base_संख्या *sp1 = froms[s] = xnmalloc (गिनती, माप *sp1);
      base_संख्या *sp2 = tos[s] = xnmalloc (गिनती, माप *sp2);
      पूर्णांक *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (गिनती, माप *sp3) : शून्य;

      /* Store non defaulted actions.  */
      क्रम (प्रतीक_संख्या i = 0; i < ntokens; i++)
        अगर (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            अगर (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = गिनती;
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

अटल व्योम
token_actions (व्योम)
{
  पूर्णांक nconflict = nondeterministic_parser ? conflicts_योग_गिनती () : 0;

  yydefact = xnmalloc (nstates, माप *yydefact);

  actrow = xnmalloc (ntokens, माप *actrow);
  conflrow = xnmalloc (ntokens, माप *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, माप *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  अगर (!nondeterministic_parser)
    क्रम (rule_संख्या r = 0; r < nrules; ++r)
      rules[r].useful = false;

  क्रम (state_संख्या i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->संख्या + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      अगर (!nondeterministic_parser)
        {
          क्रम (प्रतीक_संख्या j = 0; j < ntokens; ++j)
            अगर (actrow[j] < 0 && actrow[j] != ACTION_संख्या_MINIMUM)
              rules[item_संख्या_as_rule_संख्या (actrow[j])].useful = true;
          अगर (yydefact[i])
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

अटल व्योम
save_column (प्रतीक_संख्या sym, state_संख्या default_state)
{
  स्थिर goto_संख्या begin = goto_map[sym - ntokens];
  स्थिर goto_संख्या end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  माप_प्रकार गिनती = 0;
  क्रम (goto_संख्या i = begin; i < end; i++)
    अगर (to_state[i] != default_state)
      गिनती++;

  अगर (गिनती)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_संख्या symno = प्रतीक_संख्या_to_vector_संख्या (sym);
      base_संख्या *sp1 = froms[symno] = xnmalloc (गिनती, माप *sp1);
      base_संख्या *sp2 = tos[symno] = xnmalloc (गिनती, माप *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      क्रम (goto_संख्या i = begin; i < end; i++)
        अगर (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = गिनती;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

अटल state_संख्या
default_goto (प्रतीक_संख्या sym, माप_प्रकार state_गिनती[])
{
  स्थिर goto_संख्या begin = goto_map[sym - ntokens];
  स्थिर goto_संख्या end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_संख्या res = 0;

  अगर (begin != end)
    {
      क्रम (state_संख्या s = 0; s < nstates; s++)
        state_गिनती[s] = 0;

      क्रम (goto_संख्या i = begin; i < end; i++)
        state_गिनती[to_state[i]]++;

      माप_प्रकार max = 0;
      क्रम (state_संख्या s = 0; s < nstates; s++)
        अगर (max < state_गिनती[s])
          {
            max = state_गिनती[s];
            res = s;
          }
    }
  वापस res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

अटल व्योम
goto_actions (व्योम)
{
  माप_प्रकार *state_गिनती = xnmalloc (nstates, माप *state_गिनती);
  yydefgoto = xnmalloc (nnterms, माप *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  क्रम (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
    {
      state_संख्या default_state = default_goto (i, state_गिनती);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  मुक्त (state_गिनती);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

अटल व्योम
sort_actions (व्योम)
{
  nentries = 0;

  क्रम (पूर्णांक i = 0; i < nvectors; i++)
    अगर (0 < tally[i])
      {
        स्थिर माप_प्रकार t = tally[i];
        स्थिर पूर्णांक w = width[i];
        पूर्णांक j = nentries - 1;

        जबतक (0 <= j && width[order[j]] < w)
          j--;

        जबतक (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        क्रम (पूर्णांक k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

अटल state_संख्या
matching_state (vector_संख्या vector)
{
  vector_संख्या i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  अगर (i < nstates)
    {
      माप_प्रकार t = tally[i];
      पूर्णांक w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      अगर (conflict_tos[i] != शून्य)
        क्रम (पूर्णांक j = 0; j < t; j += 1)
          अगर (conflict_tos[i][j] != 0)
            वापस -1;

      क्रम (पूर्णांक पूर्व = vector - 1; 0 <= पूर्व; पूर्व--)
        {
          vector_संख्या j = order[पूर्व];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          अगर (width[j] != w || tally[j] != t)
            वापस -1;
          अन्यथा
            {
              bool match = true;
              क्रम (पूर्णांक k = 0; match && k < t; k++)
                अगर (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != शून्य && conflict_tos[j][k] != 0))
                  match = false;
              अगर (match)
                वापस j;
            }
        }
    }
  वापस -1;
}


अटल base_संख्या
pack_vector (vector_संख्या vector)
{
  vector_संख्या i = order[vector];
  माप_प्रकार t = tally[i];
  base_संख्या *from = froms[i];
  base_संख्या *to = tos[i];
  पूर्णांक *conflict_to = conflict_tos[i];

  aver (t != 0);

  क्रम (base_संख्या res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_आकार);
      {
        क्रम (पूर्णांक k = 0; ok && k < t; k++)
          {
            पूर्णांक loc = res + state_संख्या_as_int (from[k]);
            अगर (table_आकार <= loc)
              table_grow (loc);

            अगर (table[loc] != 0)
              ok = false;
          }

        अगर (ok && pos_set_test (res))
          ok = false;
      }

      अगर (ok)
        {
          पूर्णांक loc PACIFY_CC (= -1);
          क्रम (पूर्णांक k = 0; k < t; k++)
            {
              loc = res + state_संख्या_as_int (from[k]);
              table[loc] = to[k];
              अगर (nondeterministic_parser && conflict_to != शून्य)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          जबतक (table[lowzero] != 0)
            lowzero++;

          अगर (ऊपरी < loc)
            ऊपरी = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          वापस res;
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

अटल base_संख्या
table_ninf_remap (base_संख्या tab[], पूर्णांक आकार, base_संख्या ninf)
{
  base_संख्या res = 0;

  क्रम (पूर्णांक i = 0; i < आकार; i++)
    अगर (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  क्रम (पूर्णांक i = 0; i < आकार; i++)
    अगर (tab[i] == ninf)
      tab[i] = res;

  वापस res;
}

अटल व्योम
pack_table (व्योम)
{
  base = xnmalloc (nvectors, माप *base);
  pos_set = bitset_create (table_आकार + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_आकार, माप *table);
  conflict_table = xcalloc (table_आकार, माप *conflict_table);
  check = xnmalloc (table_आकार, माप *check);

  lowzero = 0;
  ऊपरी = 0;

  क्रम (पूर्णांक i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  क्रम (पूर्णांक i = 0; i < table_आकार; i++)
    check[i] = -1;

  क्रम (पूर्णांक i = 0; i < nentries; i++)
    {
      state_संख्या s = matching_state (i);
      base_संख्या place;

      अगर (s < 0)
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
  table_ninf = table_ninf_remap (table, ऊपरी + 1, ACTION_संख्या_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

व्योम
tables_generate (व्योम)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (माप nstates <= माप nvectors);
  verify (माप nnterms <= माप nvectors);

  nvectors = state_संख्या_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, माप *froms);
  tos = xcalloc (nvectors, माप *tos);
  conflict_tos = xcalloc (nvectors, माप *conflict_tos);
  tally = xcalloc (nvectors, माप *tally);
  width = xnmalloc (nvectors, माप *width);

  token_actions ();

  goto_actions ();
  मुक्त (goto_map);
  मुक्त (from_state);
  मुक्त (to_state);

  order = xcalloc (nvectors, माप *order);
  sort_actions ();
  pack_table ();
  मुक्त (order);

  मुक्त (tally);
  मुक्त (width);

  क्रम (पूर्णांक i = 0; i < nvectors; i++)
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

व्योम
tables_free (व्योम)
{
  मुक्त (base);
  मुक्त (conflict_table);
  मुक्त (conflict_list);
  मुक्त (table);
  मुक्त (check);
  मुक्त (yydefgoto);
  मुक्त (yydefact);
}
