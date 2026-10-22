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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ <bitset.h>
#অন্তর্ভাৱ <bitsetv.h>

#অন্তর্ভাৱ "complain.h"
#অন্তর্ভাৱ "conflicts.h"
#অন্তর্ভাৱ "files.h"
#অন্তর্ভাৱ "getargs.h"
#অন্তর্ভাৱ "gram.h"
#অন্তর্ভাৱ "lalr.h"
#অন্তর্ভাৱ "muscle-tab.h"
#অন্তর্ভাৱ "reader.h"
#অন্তর্ভাৱ "symtab.h"
#অন্তর্ভাৱ "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
প্রকারনির্দেশ পূর্ণাঙ্ক vector_সংখ্যা;

#যদি 0 /* Not currently used.  */
স্থির অন্তঃস্থ vector_সংখ্যা
state_সংখ্যা_to_vector_সংখ্যা (state_সংখ্যা s)
{
  ঘূৰাই s;
}
#endif

স্থির অন্তঃস্থ vector_সংখ্যা
প্ৰতীক_সংখ্যা_to_vector_সংখ্যা (প্ৰতীক_সংখ্যা sym)
{
  ঘূৰাই state_সংখ্যা_as_int (nstates) + sym - ntokens;
}

পূর্ণাঙ্ক nvectors;


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

স্থির base_সংখ্যা **froms;
স্থির base_সংখ্যা **tos;
স্থির পূর্ণাঙ্ক **conflict_tos;
স্থির size_t *tally;
স্থির base_সংখ্যা *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
প্রকারনির্দেশ পূর্ণাঙ্ক action_সংখ্যা;
#define ACTION_সংখ্যা_MINIMUM INT_MIN

স্থির action_সংখ্যা *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
স্থির vector_সংখ্যা *order;
স্থির পূর্ণাঙ্ক nentries;

base_সংখ্যা *base = রিক্ত;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_সংখ্যা base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
স্থির bitset pos_set = রিক্ত;
/* The integer denoted by bitno 0 in pos_set.  */
স্থির পূর্ণাঙ্ক pos_set_base = 0;

স্থির পূর্ণাঙ্ক *conflrow;
পূর্ণাঙ্ক *conflict_table;
পূর্ণাঙ্ক *conflict_list;
পূর্ণাঙ্ক conflict_list_cnt;
স্থির পূর্ণাঙ্ক conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
স্থির পূর্ণাঙ্ক table_size = 32768;
base_সংখ্যা *table;
base_সংখ্যা *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_সংখ্যা table_ninf = 0;
স্থির পূর্ণাঙ্ক lowzero;
পূর্ণাঙ্ক উপৰেৰ;

state_সংখ্যা *yydefgoto;
rule_সংখ্যা *yydefact;


/*----------.
| pos_set.  |
`----------*/

#যদি 0
স্থির শূন্য
pos_set_dump (শূন্য)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  পূর্ণাঙ্ক i;
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

স্থির শূন্য
pos_set_set (পূর্ণাঙ্ক pos)
{
  পূর্ণাঙ্ক bitno = pos - pos_set_base;
  যদি (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      ধ্রুৱ পূর্ণাঙ্ক delta = pos_set_base - pos;
      ধ্রুৱ পূর্ণাঙ্ক old_size = bitset_size (pos_set);
      ধ্রুৱ পূর্ণাঙ্ক new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      বাবে (পূর্ণাঙ্ক i = new_size - 1; 0 <= i ; --i)
        যদি (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        অন্যথা
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  অন্যথা যদি (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

স্থির bool
pos_set_test (পূর্ণাঙ্ক pos)
{
  ধ্রুৱ পূর্ণাঙ্ক bitno = pos - pos_set_base;
  ঘূৰাই bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

স্থির শূন্য
table_grow (পূর্ণাঙ্ক desired)
{
  পূর্ণাঙ্ক old_size = table_size;

  যেতিয়ালৈ (table_size <= desired)
    table_size *= 2;

  যদি (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, পরিমাণ *table);
  memset (table + old_size, 0,
          পরিমাণ *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              পরিমাণ *conflict_table);
  memset (conflict_table + old_size, 0,
          পরিমাণ *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, পরিমাণ *check);
  বাবে (পূর্ণাঙ্ক i = old_size; i < table_size; ++i)
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

স্থির শূন্য
conflict_row (state *s)
{
  যদি (!nondeterministic_parser)
    ঘূৰাই;

  ধ্রুৱ reductions *reds = s->reductions;
  বাবে (state_সংখ্যা j = 0; j < ntokens; j += 1)
    যদি (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        বাবে (পূর্ণাঙ্ক i = 0; i < reds->অঙ্ক; i += 1)
          যদি (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_সংখ্যা_as_item_সংখ্যা (reds->rules[i]->সংখ্যা)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->সংখ্যা + 1;
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

স্থির rule *
action_row (state *s)
{
  বাবে (state_সংখ্যা i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  যদি (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    বাবে (পূর্ণাঙ্ক i = reds->অঙ্ক - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        পূর্ণাঙ্ক j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            যদি (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_সংখ্যা_as_item_সংখ্যা (reds->rules[i]->সংখ্যা);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    পূর্ণাঙ্ক i;
    FOR_EACH_SHIFT (trans, i)
      {
        প্ৰতীক_সংখ্যা sym = TRANSITION_প্ৰতীক (trans, i);
        state *shift_state = trans->states[i];

        যদি (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_সংখ্যা_as_int (shift_state->সংখ্যা);

        /* Do not use any default reduction if there is a shift for
           error */
        যদি (sym == errtoken->content->সংখ্যা)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  বাবে (পূর্ণাঙ্ক i = 0; i < errp->অঙ্ক; i++)
    {
      প্ৰতীক *sym = errp->symbols[i];
      actrow[sym->content->সংখ্যা] = ACTION_সংখ্যা_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    ৱর্ণ *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    যদি (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    মুক্ত (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = রিক্ত;
  যদি (reds->অঙ্ক >= 1 && !nodefault)
    {
      যদি (s->consistent)
        default_reduction = reds->rules[0];
      অন্যথা
        {
          পূর্ণাঙ্ক max = 0;
          বাবে (পূর্ণাঙ্ক i = 0; i < reds->অঙ্ক; i++)
            {
              পূর্ণাঙ্ক গুনতি = 0;
              rule *r = reds->rules[i];
              বাবে (প্ৰতীক_সংখ্যা j = 0; j < ntokens; j++)
                যদি (actrow[j] == rule_সংখ্যা_as_item_সংখ্যা (r->সংখ্যা))
                  গুনতি++;

              যদি (গুনতি > max)
                {
                  max = গুনতি;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          যদি (0 < max)
            বাবে (প্ৰতীক_সংখ্যা j = 0; j < ntokens; j++)
              যদি (actrow[j]
                  == rule_সংখ্যা_as_item_সংখ্যা (default_reduction->সংখ্যা)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  যদি (!default_reduction)
    বাবে (প্ৰতীক_সংখ্যা i = 0; i < ntokens; i++)
      যদি (actrow[i] == ACTION_সংখ্যা_MINIMUM)
        actrow[i] = 0;

  যদি (conflicted)
    conflict_row (s);

  ঘূৰাই default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

স্থির শূন্য
save_row (state_সংখ্যা s)
{
  /* Number of non default actions in S.  */
  size_t গুনতি = 0;
  বাবে (প্ৰতীক_সংখ্যা i = 0; i < ntokens; i++)
    যদি (actrow[i] != 0)
      গুনতি++;

  যদি (গুনতি)
    {
      /* Allocate non defaulted actions.  */
      base_সংখ্যা *sp1 = froms[s] = xnmalloc (গুনতি, পরিমাণ *sp1);
      base_সংখ্যা *sp2 = tos[s] = xnmalloc (গুনতি, পরিমাণ *sp2);
      পূর্ণাঙ্ক *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (গুনতি, পরিমাণ *sp3) : রিক্ত;

      /* Store non defaulted actions.  */
      বাবে (প্ৰতীক_সংখ্যা i = 0; i < ntokens; i++)
        যদি (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            যদি (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = গুনতি;
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

স্থির শূন্য
token_actions (শূন্য)
{
  পূর্ণাঙ্ক nconflict = nondeterministic_parser ? conflicts_total_গুনতি () : 0;

  yydefact = xnmalloc (nstates, পরিমাণ *yydefact);

  actrow = xnmalloc (ntokens, পরিমাণ *actrow);
  conflrow = xnmalloc (ntokens, পরিমাণ *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, পরিমাণ *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  যদি (!nondeterministic_parser)
    বাবে (rule_সংখ্যা r = 0; r < nrules; ++r)
      rules[r].useful = false;

  বাবে (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->সংখ্যা + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      যদি (!nondeterministic_parser)
        {
          বাবে (প্ৰতীক_সংখ্যা j = 0; j < ntokens; ++j)
            যদি (actrow[j] < 0 && actrow[j] != ACTION_সংখ্যা_MINIMUM)
              rules[item_সংখ্যা_as_rule_সংখ্যা (actrow[j])].useful = true;
          যদি (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  মুক্ত (actrow);
  মুক্ত (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

স্থির শূন্য
save_column (প্ৰতীক_সংখ্যা sym, state_সংখ্যা default_state)
{
  ধ্রুৱ goto_সংখ্যা begin = goto_map[sym - ntokens];
  ধ্রুৱ goto_সংখ্যা end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t গুনতি = 0;
  বাবে (goto_সংখ্যা i = begin; i < end; i++)
    যদি (to_state[i] != default_state)
      গুনতি++;

  যদি (গুনতি)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_সংখ্যা symno = প্ৰতীক_সংখ্যা_to_vector_সংখ্যা (sym);
      base_সংখ্যা *sp1 = froms[symno] = xnmalloc (গুনতি, পরিমাণ *sp1);
      base_সংখ্যা *sp2 = tos[symno] = xnmalloc (গুনতি, পরিমাণ *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      বাবে (goto_সংখ্যা i = begin; i < end; i++)
        যদি (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = গুনতি;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

স্থির state_সংখ্যা
default_goto (প্ৰতীক_সংখ্যা sym, size_t state_গুনতি[])
{
  ধ্রুৱ goto_সংখ্যা begin = goto_map[sym - ntokens];
  ধ্রুৱ goto_সংখ্যা end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_সংখ্যা res = 0;

  যদি (begin != end)
    {
      বাবে (state_সংখ্যা s = 0; s < nstates; s++)
        state_গুনতি[s] = 0;

      বাবে (goto_সংখ্যা i = begin; i < end; i++)
        state_গুনতি[to_state[i]]++;

      size_t max = 0;
      বাবে (state_সংখ্যা s = 0; s < nstates; s++)
        যদি (max < state_গুনতি[s])
          {
            max = state_গুনতি[s];
            res = s;
          }
    }
  ঘূৰাই res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

স্থির শূন্য
goto_actions (শূন্য)
{
  size_t *state_গুনতি = xnmalloc (nstates, পরিমাণ *state_গুনতি);
  yydefgoto = xnmalloc (nnterms, পরিমাণ *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  বাবে (প্ৰতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    {
      state_সংখ্যা default_state = default_goto (i, state_গুনতি);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  মুক্ত (state_গুনতি);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

স্থির শূন্য
sort_actions (শূন্য)
{
  nentries = 0;

  বাবে (পূর্ণাঙ্ক i = 0; i < nvectors; i++)
    যদি (0 < tally[i])
      {
        ধ্রুৱ size_t t = tally[i];
        ধ্রুৱ পূর্ণাঙ্ক w = width[i];
        পূর্ণাঙ্ক j = nentries - 1;

        যেতিয়ালৈ (0 <= j && width[order[j]] < w)
          j--;

        যেতিয়ালৈ (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        বাবে (পূর্ণাঙ্ক k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

স্থির state_সংখ্যা
matching_state (vector_সংখ্যা vector)
{
  vector_সংখ্যা i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  যদি (i < nstates)
    {
      size_t t = tally[i];
      পূর্ণাঙ্ক w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      যদি (conflict_tos[i] != রিক্ত)
        বাবে (পূর্ণাঙ্ক j = 0; j < t; j += 1)
          যদি (conflict_tos[i][j] != 0)
            ঘূৰাই -1;

      বাবে (পূর্ণাঙ্ক prev = vector - 1; 0 <= prev; prev--)
        {
          vector_সংখ্যা j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          যদি (width[j] != w || tally[j] != t)
            ঘূৰাই -1;
          অন্যথা
            {
              bool match = true;
              বাবে (পূর্ণাঙ্ক k = 0; match && k < t; k++)
                যদি (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != রিক্ত && conflict_tos[j][k] != 0))
                  match = false;
              যদি (match)
                ঘূৰাই j;
            }
        }
    }
  ঘূৰাই -1;
}


স্থির base_সংখ্যা
pack_vector (vector_সংখ্যা vector)
{
  vector_সংখ্যা i = order[vector];
  size_t t = tally[i];
  base_সংখ্যা *from = froms[i];
  base_সংখ্যা *to = tos[i];
  পূর্ণাঙ্ক *conflict_to = conflict_tos[i];

  aver (t != 0);

  বাবে (base_সংখ্যা res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        বাবে (পূর্ণাঙ্ক k = 0; ok && k < t; k++)
          {
            পূর্ণাঙ্ক loc = res + state_সংখ্যা_as_int (from[k]);
            যদি (table_size <= loc)
              table_grow (loc);

            যদি (table[loc] != 0)
              ok = false;
          }

        যদি (ok && pos_set_test (res))
          ok = false;
      }

      যদি (ok)
        {
          পূর্ণাঙ্ক loc PACIFY_CC (= -1);
          বাবে (পূর্ণাঙ্ক k = 0; k < t; k++)
            {
              loc = res + state_সংখ্যা_as_int (from[k]);
              table[loc] = to[k];
              যদি (nondeterministic_parser && conflict_to != রিক্ত)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          যেতিয়ালৈ (table[lowzero] != 0)
            lowzero++;

          যদি (উপৰেৰ < loc)
            উপৰেৰ = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          ঘূৰাই res;
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

স্থির base_সংখ্যা
table_ninf_remap (base_সংখ্যা tab[], পূর্ণাঙ্ক size, base_সংখ্যা ninf)
{
  base_সংখ্যা res = 0;

  বাবে (পূর্ণাঙ্ক i = 0; i < size; i++)
    যদি (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  বাবে (পূর্ণাঙ্ক i = 0; i < size; i++)
    যদি (tab[i] == ninf)
      tab[i] = res;

  ঘূৰাই res;
}

স্থির শূন্য
pack_table (শূন্য)
{
  base = xnmalloc (nvectors, পরিমাণ *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, পরিমাণ *table);
  conflict_table = xcalloc (table_size, পরিমাণ *conflict_table);
  check = xnmalloc (table_size, পরিমাণ *check);

  lowzero = 0;
  উপৰেৰ = 0;

  বাবে (পূর্ণাঙ্ক i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  বাবে (পূর্ণাঙ্ক i = 0; i < table_size; i++)
    check[i] = -1;

  বাবে (পূর্ণাঙ্ক i = 0; i < nentries; i++)
    {
      state_সংখ্যা s = matching_state (i);
      base_সংখ্যা place;

      যদি (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      অন্যথা
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, উপৰেৰ + 1, ACTION_সংখ্যা_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

শূন্য
tables_generate (শূন্য)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (পরিমাণ nstates <= পরিমাণ nvectors);
  verify (পরিমাণ nnterms <= পরিমাণ nvectors);

  nvectors = state_সংখ্যা_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, পরিমাণ *froms);
  tos = xcalloc (nvectors, পরিমাণ *tos);
  conflict_tos = xcalloc (nvectors, পরিমাণ *conflict_tos);
  tally = xcalloc (nvectors, পরিমাণ *tally);
  width = xnmalloc (nvectors, পরিমাণ *width);

  token_actions ();

  goto_actions ();
  মুক্ত (goto_map);
  মুক্ত (from_state);
  মুক্ত (to_state);

  order = xcalloc (nvectors, পরিমাণ *order);
  sort_actions ();
  pack_table ();
  মুক্ত (order);

  মুক্ত (tally);
  মুক্ত (width);

  বাবে (পূর্ণাঙ্ক i = 0; i < nvectors; i++)
    {
      মুক্ত (froms[i]);
      মুক্ত (tos[i]);
      মুক্ত (conflict_tos[i]);
    }

  মুক্ত (froms);
  মুক্ত (tos);
  মুক্ত (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

শূন্য
tables_free (শূন্য)
{
  মুক্ত (base);
  মুক্ত (conflict_table);
  মুক্ত (conflict_list);
  মুক্ত (table);
  মুক্ত (check);
  মুক্ত (yydefgoto);
  মুক্ত (yydefact);
}
