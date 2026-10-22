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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>
#અન્તર્ભાવ <bitsetv.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "conflicts.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "symtab.h"
#અન્તર્ભાવ "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
પ્રકારવ્યાખ્યા પૂર્ણાંક vector_સંખ્યા;

#જો 0 /* Not currently used.  */
સ્થિર અંતઃરેખ vector_સંખ્યા
state_સંખ્યા_to_vector_સંખ્યા (state_સંખ્યા s)
{
  પાછા s;
}
#endif

સ્થિર અંતઃરેખ vector_સંખ્યા
પ્રતીક_સંખ્યા_to_vector_સંખ્યા (પ્રતીક_સંખ્યા sym)
{
  પાછા state_સંખ્યા_as_int (nstates) + sym - ntokens;
}

પૂર્ણાંક nvectors;


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

સ્થિર base_સંખ્યા **froms;
સ્થિર base_સંખ્યા **tos;
સ્થિર પૂર્ણાંક **conflict_tos;
સ્થિર size_t *tally;
સ્થિર base_સંખ્યા *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
પ્રકારવ્યાખ્યા પૂર્ણાંક action_સંખ્યા;
#define ACTION_સંખ્યા_MINIMUM INT_MIN

સ્થિર action_સંખ્યા *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
સ્થિર vector_સંખ્યા *order;
સ્થિર પૂર્ણાંક nentries;

base_સંખ્યા *base = ખાલી;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_સંખ્યા base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
સ્થિર bitset pos_set = ખાલી;
/* The integer denoted by bitno 0 in pos_set.  */
સ્થિર પૂર્ણાંક pos_set_base = 0;

સ્થિર પૂર્ણાંક *conflrow;
પૂર્ણાંક *conflict_table;
પૂર્ણાંક *conflict_list;
પૂર્ણાંક conflict_list_cnt;
સ્થિર પૂર્ણાંક conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
સ્થિર પૂર્ણાંક table_size = 32768;
base_સંખ્યા *table;
base_સંખ્યા *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_સંખ્યા table_ninf = 0;
સ્થિર પૂર્ણાંક lowzero;
પૂર્ણાંક ઉપલું;

state_સંખ્યા *yydefgoto;
rule_સંખ્યા *yydefact;


/*----------.
| pos_set.  |
`----------*/

#જો 0
સ્થિર શૂન્ય
pos_set_dump (શૂન્ય)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  પૂર્ણાંક i;
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

સ્થિર શૂન્ય
pos_set_set (પૂર્ણાંક pos)
{
  પૂર્ણાંક bitno = pos - pos_set_base;
  જો (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      અચલ પૂર્ણાંક delta = pos_set_base - pos;
      અચલ પૂર્ણાંક old_size = bitset_size (pos_set);
      અચલ પૂર્ણાંક new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      માટે (પૂર્ણાંક i = new_size - 1; 0 <= i ; --i)
        જો (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        નહીંતર
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  નહીંતર જો (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

સ્થિર bool
pos_set_test (પૂર્ણાંક pos)
{
  અચલ પૂર્ણાંક bitno = pos - pos_set_base;
  પાછા bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
table_grow (પૂર્ણાંક desired)
{
  પૂર્ણાંક old_size = table_size;

  જ્યારે (table_size <= desired)
    table_size *= 2;

  જો (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, કદ *table);
  memset (table + old_size, 0,
          કદ *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              કદ *conflict_table);
  memset (conflict_table + old_size, 0,
          કદ *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, કદ *check);
  માટે (પૂર્ણાંક i = old_size; i < table_size; ++i)
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

સ્થિર શૂન્ય
conflict_row (state *s)
{
  જો (!nondeterministic_parser)
    પાછા;

  અચલ reductions *reds = s->reductions;
  માટે (state_સંખ્યા j = 0; j < ntokens; j += 1)
    જો (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        માટે (પૂર્ણાંક i = 0; i < reds->અંક; i += 1)
          જો (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_સંખ્યા_as_item_સંખ્યા (reds->rules[i]->સંખ્યા)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->સંખ્યા + 1;
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

સ્થિર rule *
action_row (state *s)
{
  માટે (state_સંખ્યા i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  જો (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    માટે (પૂર્ણાંક i = reds->અંક - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        પૂર્ણાંક j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            જો (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_સંખ્યા_as_item_સંખ્યા (reds->rules[i]->સંખ્યા);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    પૂર્ણાંક i;
    FOR_EACH_SHIFT (trans, i)
      {
        પ્રતીક_સંખ્યા sym = TRANSITION_પ્રતીક (trans, i);
        state *shift_state = trans->states[i];

        જો (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_સંખ્યા_as_int (shift_state->સંખ્યા);

        /* Do not use any default reduction if there is a shift for
           error */
        જો (sym == errtoken->content->સંખ્યા)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  માટે (પૂર્ણાંક i = 0; i < errp->અંક; i++)
    {
      પ્રતીક *sym = errp->symbols[i];
      actrow[sym->content->સંખ્યા] = ACTION_સંખ્યા_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    અક્ષર *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    જો (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    મુક્ત (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = ખાલી;
  જો (reds->અંક >= 1 && !nodefault)
    {
      જો (s->consistent)
        default_reduction = reds->rules[0];
      નહીંતર
        {
          પૂર્ણાંક max = 0;
          માટે (પૂર્ણાંક i = 0; i < reds->અંક; i++)
            {
              પૂર્ણાંક ગણતરી = 0;
              rule *r = reds->rules[i];
              માટે (પ્રતીક_સંખ્યા j = 0; j < ntokens; j++)
                જો (actrow[j] == rule_સંખ્યા_as_item_સંખ્યા (r->સંખ્યા))
                  ગણતરી++;

              જો (ગણતરી > max)
                {
                  max = ગણતરી;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          જો (0 < max)
            માટે (પ્રતીક_સંખ્યા j = 0; j < ntokens; j++)
              જો (actrow[j]
                  == rule_સંખ્યા_as_item_સંખ્યા (default_reduction->સંખ્યા)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  જો (!default_reduction)
    માટે (પ્રતીક_સંખ્યા i = 0; i < ntokens; i++)
      જો (actrow[i] == ACTION_સંખ્યા_MINIMUM)
        actrow[i] = 0;

  જો (conflicted)
    conflict_row (s);

  પાછા default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

સ્થિર શૂન્ય
save_row (state_સંખ્યા s)
{
  /* Number of non default actions in S.  */
  size_t ગણતરી = 0;
  માટે (પ્રતીક_સંખ્યા i = 0; i < ntokens; i++)
    જો (actrow[i] != 0)
      ગણતરી++;

  જો (ગણતરી)
    {
      /* Allocate non defaulted actions.  */
      base_સંખ્યા *sp1 = froms[s] = xnmalloc (ગણતરી, કદ *sp1);
      base_સંખ્યા *sp2 = tos[s] = xnmalloc (ગણતરી, કદ *sp2);
      પૂર્ણાંક *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (ગણતરી, કદ *sp3) : ખાલી;

      /* Store non defaulted actions.  */
      માટે (પ્રતીક_સંખ્યા i = 0; i < ntokens; i++)
        જો (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            જો (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = ગણતરી;
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

સ્થિર શૂન્ય
token_actions (શૂન્ય)
{
  પૂર્ણાંક nconflict = nondeterministic_parser ? conflicts_total_ગણતરી () : 0;

  yydefact = xnmalloc (nstates, કદ *yydefact);

  actrow = xnmalloc (ntokens, કદ *actrow);
  conflrow = xnmalloc (ntokens, કદ *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, કદ *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  જો (!nondeterministic_parser)
    માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
      rules[r].useful = false;

  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->સંખ્યા + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      જો (!nondeterministic_parser)
        {
          માટે (પ્રતીક_સંખ્યા j = 0; j < ntokens; ++j)
            જો (actrow[j] < 0 && actrow[j] != ACTION_સંખ્યા_MINIMUM)
              rules[item_સંખ્યા_as_rule_સંખ્યા (actrow[j])].useful = true;
          જો (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  મુક્ત (actrow);
  મુક્ત (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
save_column (પ્રતીક_સંખ્યા sym, state_સંખ્યા default_state)
{
  અચલ goto_સંખ્યા begin = goto_map[sym - ntokens];
  અચલ goto_સંખ્યા end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t ગણતરી = 0;
  માટે (goto_સંખ્યા i = begin; i < end; i++)
    જો (to_state[i] != default_state)
      ગણતરી++;

  જો (ગણતરી)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_સંખ્યા symno = પ્રતીક_સંખ્યા_to_vector_સંખ્યા (sym);
      base_સંખ્યા *sp1 = froms[symno] = xnmalloc (ગણતરી, કદ *sp1);
      base_સંખ્યા *sp2 = tos[symno] = xnmalloc (ગણતરી, કદ *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      માટે (goto_સંખ્યા i = begin; i < end; i++)
        જો (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = ગણતરી;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

સ્થિર state_સંખ્યા
default_goto (પ્રતીક_સંખ્યા sym, size_t state_ગણતરી[])
{
  અચલ goto_સંખ્યા begin = goto_map[sym - ntokens];
  અચલ goto_સંખ્યા end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_સંખ્યા res = 0;

  જો (begin != end)
    {
      માટે (state_સંખ્યા s = 0; s < nstates; s++)
        state_ગણતરી[s] = 0;

      માટે (goto_સંખ્યા i = begin; i < end; i++)
        state_ગણતરી[to_state[i]]++;

      size_t max = 0;
      માટે (state_સંખ્યા s = 0; s < nstates; s++)
        જો (max < state_ગણતરી[s])
          {
            max = state_ગણતરી[s];
            res = s;
          }
    }
  પાછા res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
goto_actions (શૂન્ય)
{
  size_t *state_ગણતરી = xnmalloc (nstates, કદ *state_ગણતરી);
  yydefgoto = xnmalloc (nnterms, કદ *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    {
      state_સંખ્યા default_state = default_goto (i, state_ગણતરી);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  મુક્ત (state_ગણતરી);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

સ્થિર શૂન્ય
sort_actions (શૂન્ય)
{
  nentries = 0;

  માટે (પૂર્ણાંક i = 0; i < nvectors; i++)
    જો (0 < tally[i])
      {
        અચલ size_t t = tally[i];
        અચલ પૂર્ણાંક w = width[i];
        પૂર્ણાંક j = nentries - 1;

        જ્યારે (0 <= j && width[order[j]] < w)
          j--;

        જ્યારે (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        માટે (પૂર્ણાંક k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

સ્થિર state_સંખ્યા
matching_state (vector_સંખ્યા vector)
{
  vector_સંખ્યા i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  જો (i < nstates)
    {
      size_t t = tally[i];
      પૂર્ણાંક w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      જો (conflict_tos[i] != ખાલી)
        માટે (પૂર્ણાંક j = 0; j < t; j += 1)
          જો (conflict_tos[i][j] != 0)
            પાછા -1;

      માટે (પૂર્ણાંક prev = vector - 1; 0 <= prev; prev--)
        {
          vector_સંખ્યા j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          જો (width[j] != w || tally[j] != t)
            પાછા -1;
          નહીંતર
            {
              bool match = true;
              માટે (પૂર્ણાંક k = 0; match && k < t; k++)
                જો (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != ખાલી && conflict_tos[j][k] != 0))
                  match = false;
              જો (match)
                પાછા j;
            }
        }
    }
  પાછા -1;
}


સ્થિર base_સંખ્યા
pack_vector (vector_સંખ્યા vector)
{
  vector_સંખ્યા i = order[vector];
  size_t t = tally[i];
  base_સંખ્યા *from = froms[i];
  base_સંખ્યા *to = tos[i];
  પૂર્ણાંક *conflict_to = conflict_tos[i];

  aver (t != 0);

  માટે (base_સંખ્યા res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        માટે (પૂર્ણાંક k = 0; ok && k < t; k++)
          {
            પૂર્ણાંક loc = res + state_સંખ્યા_as_int (from[k]);
            જો (table_size <= loc)
              table_grow (loc);

            જો (table[loc] != 0)
              ok = false;
          }

        જો (ok && pos_set_test (res))
          ok = false;
      }

      જો (ok)
        {
          પૂર્ણાંક loc PACIFY_CC (= -1);
          માટે (પૂર્ણાંક k = 0; k < t; k++)
            {
              loc = res + state_સંખ્યા_as_int (from[k]);
              table[loc] = to[k];
              જો (nondeterministic_parser && conflict_to != ખાલી)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          જ્યારે (table[lowzero] != 0)
            lowzero++;

          જો (ઉપલું < loc)
            ઉપલું = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          પાછા res;
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

સ્થિર base_સંખ્યા
table_ninf_remap (base_સંખ્યા tab[], પૂર્ણાંક size, base_સંખ્યા ninf)
{
  base_સંખ્યા res = 0;

  માટે (પૂર્ણાંક i = 0; i < size; i++)
    જો (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  માટે (પૂર્ણાંક i = 0; i < size; i++)
    જો (tab[i] == ninf)
      tab[i] = res;

  પાછા res;
}

સ્થિર શૂન્ય
pack_table (શૂન્ય)
{
  base = xnmalloc (nvectors, કદ *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, કદ *table);
  conflict_table = xcalloc (table_size, કદ *conflict_table);
  check = xnmalloc (table_size, કદ *check);

  lowzero = 0;
  ઉપલું = 0;

  માટે (પૂર્ણાંક i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  માટે (પૂર્ણાંક i = 0; i < table_size; i++)
    check[i] = -1;

  માટે (પૂર્ણાંક i = 0; i < nentries; i++)
    {
      state_સંખ્યા s = matching_state (i);
      base_સંખ્યા place;

      જો (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      નહીંતર
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, ઉપલું + 1, ACTION_સંખ્યા_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

શૂન્ય
tables_generate (શૂન્ય)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (કદ nstates <= કદ nvectors);
  verify (કદ nnterms <= કદ nvectors);

  nvectors = state_સંખ્યા_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, કદ *froms);
  tos = xcalloc (nvectors, કદ *tos);
  conflict_tos = xcalloc (nvectors, કદ *conflict_tos);
  tally = xcalloc (nvectors, કદ *tally);
  width = xnmalloc (nvectors, કદ *width);

  token_actions ();

  goto_actions ();
  મુક્ત (goto_map);
  મુક્ત (from_state);
  મુક્ત (to_state);

  order = xcalloc (nvectors, કદ *order);
  sort_actions ();
  pack_table ();
  મુક્ત (order);

  મુક્ત (tally);
  મુક્ત (width);

  માટે (પૂર્ણાંક i = 0; i < nvectors; i++)
    {
      મુક્ત (froms[i]);
      મુક્ત (tos[i]);
      મુક્ત (conflict_tos[i]);
    }

  મુક્ત (froms);
  મુક્ત (tos);
  મુક્ત (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

શૂન્ય
tables_free (શૂન્ય)
{
  મુક્ત (base);
  મુક્ત (conflict_table);
  મુક્ત (conflict_list);
  મુક્ત (table);
  મુક્ત (check);
  મુક્ત (yydefgoto);
  મુક્ત (yydefact);
}
