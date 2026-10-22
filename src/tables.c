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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>
#അന്തര്ഭാവ <bitsetv.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "conflicts.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "symtab.h"
#അന്തര്ഭാവ "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
തരനിർവചനം പൂർണ്ണസംഖ്യ vector_സംഖ്യ;

#എങ്കിൽ 0 /* Not currently used.  */
സ്ഥിതം വരിയിൽ vector_സംഖ്യ
state_സംഖ്യ_to_vector_സംഖ്യ (state_സംഖ്യ s)
{
  തിരികെ s;
}
#endif

സ്ഥിതം വരിയിൽ vector_സംഖ്യ
ചിഹ്നം_സംഖ്യ_to_vector_സംഖ്യ (ചിഹ്നം_സംഖ്യ sym)
{
  തിരികെ state_സംഖ്യ_as_int (nstates) + sym - ntokens;
}

പൂർണ്ണസംഖ്യ nvectors;


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

സ്ഥിതം base_സംഖ്യ **froms;
സ്ഥിതം base_സംഖ്യ **tos;
സ്ഥിതം പൂർണ്ണസംഖ്യ **conflict_tos;
സ്ഥിതം size_t *tally;
സ്ഥിതം base_സംഖ്യ *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
തരനിർവചനം പൂർണ്ണസംഖ്യ action_സംഖ്യ;
#define ACTION_സംഖ്യ_MINIMUM INT_MIN

സ്ഥിതം action_സംഖ്യ *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
സ്ഥിതം vector_സംഖ്യ *order;
സ്ഥിതം പൂർണ്ണസംഖ്യ nentries;

base_സംഖ്യ *base = ശൂന്യ;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_സംഖ്യ base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
സ്ഥിതം bitset pos_set = ശൂന്യ;
/* The integer denoted by bitno 0 in pos_set.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ pos_set_base = 0;

സ്ഥിതം പൂർണ്ണസംഖ്യ *conflrow;
പൂർണ്ണസംഖ്യ *conflict_table;
പൂർണ്ണസംഖ്യ *conflict_list;
പൂർണ്ണസംഖ്യ conflict_list_cnt;
സ്ഥിതം പൂർണ്ണസംഖ്യ conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
സ്ഥിതം പൂർണ്ണസംഖ്യ table_size = 32768;
base_സംഖ്യ *table;
base_സംഖ്യ *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_സംഖ്യ table_ninf = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ lowzero;
പൂർണ്ണസംഖ്യ മുകളിലെ;

state_സംഖ്യ *yydefgoto;
rule_സംഖ്യ *yydefact;


/*----------.
| pos_set.  |
`----------*/

#എങ്കിൽ 0
സ്ഥിതം ശൂന്യം
pos_set_dump (ശൂന്യം)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  പൂർണ്ണസംഖ്യ i;
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

സ്ഥിതം ശൂന്യം
pos_set_set (പൂർണ്ണസംഖ്യ pos)
{
  പൂർണ്ണസംഖ്യ bitno = pos - pos_set_base;
  എങ്കിൽ (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      സ്ഥിരം പൂർണ്ണസംഖ്യ delta = pos_set_base - pos;
      സ്ഥിരം പൂർണ്ണസംഖ്യ old_size = bitset_size (pos_set);
      സ്ഥിരം പൂർണ്ണസംഖ്യ new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = new_size - 1; 0 <= i ; --i)
        എങ്കിൽ (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        അല്ലെങ്കിൽ
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

സ്ഥിതം bool
pos_set_test (പൂർണ്ണസംഖ്യ pos)
{
  സ്ഥിരം പൂർണ്ണസംഖ്യ bitno = pos - pos_set_base;
  തിരികെ bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
table_grow (പൂർണ്ണസംഖ്യ desired)
{
  പൂർണ്ണസംഖ്യ old_size = table_size;

  ആകുമ്പോൾ (table_size <= desired)
    table_size *= 2;

  എങ്കിൽ (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, വലുപ്പം *table);
  memset (table + old_size, 0,
          വലുപ്പം *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              വലുപ്പം *conflict_table);
  memset (conflict_table + old_size, 0,
          വലുപ്പം *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, വലുപ്പം *check);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = old_size; i < table_size; ++i)
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

സ്ഥിതം ശൂന്യം
conflict_row (state *s)
{
  എങ്കിൽ (!nondeterministic_parser)
    തിരികെ;

  സ്ഥിരം reductions *reds = s->reductions;
  വേണ്ടി (state_സംഖ്യ j = 0; j < ntokens; j += 1)
    എങ്കിൽ (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; i += 1)
          എങ്കിൽ (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_സംഖ്യ_as_item_സംഖ്യ (reds->rules[i]->സംഖ്യ)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->സംഖ്യ + 1;
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

സ്ഥിതം rule *
action_row (state *s)
{
  വേണ്ടി (state_സംഖ്യ i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  എങ്കിൽ (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = reds->അക്കം - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        പൂർണ്ണസംഖ്യ j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            എങ്കിൽ (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_സംഖ്യ_as_item_സംഖ്യ (reds->rules[i]->സംഖ്യ);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    പൂർണ്ണസംഖ്യ i;
    FOR_EACH_SHIFT (trans, i)
      {
        ചിഹ്നം_സംഖ്യ sym = TRANSITION_ചിഹ്നം (trans, i);
        state *shift_state = trans->states[i];

        എങ്കിൽ (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_സംഖ്യ_as_int (shift_state->സംഖ്യ);

        /* Do not use any default reduction if there is a shift for
           error */
        എങ്കിൽ (sym == errtoken->content->സംഖ്യ)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < errp->അക്കം; i++)
    {
      ചിഹ്നം *sym = errp->symbols[i];
      actrow[sym->content->സംഖ്യ] = ACTION_സംഖ്യ_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    അക്ഷരം *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    എങ്കിൽ (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    സ്വതന്ത്രം (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = ശൂന്യ;
  എങ്കിൽ (reds->അക്കം >= 1 && !nodefault)
    {
      എങ്കിൽ (s->consistent)
        default_reduction = reds->rules[0];
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ max = 0;
          വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; i++)
            {
              പൂർണ്ണസംഖ്യ എണ്ണം = 0;
              rule *r = reds->rules[i];
              വേണ്ടി (ചിഹ്നം_സംഖ്യ j = 0; j < ntokens; j++)
                എങ്കിൽ (actrow[j] == rule_സംഖ്യ_as_item_സംഖ്യ (r->സംഖ്യ))
                  എണ്ണം++;

              എങ്കിൽ (എണ്ണം > max)
                {
                  max = എണ്ണം;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          എങ്കിൽ (0 < max)
            വേണ്ടി (ചിഹ്നം_സംഖ്യ j = 0; j < ntokens; j++)
              എങ്കിൽ (actrow[j]
                  == rule_സംഖ്യ_as_item_സംഖ്യ (default_reduction->സംഖ്യ)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  എങ്കിൽ (!default_reduction)
    വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < ntokens; i++)
      എങ്കിൽ (actrow[i] == ACTION_സംഖ്യ_MINIMUM)
        actrow[i] = 0;

  എങ്കിൽ (conflicted)
    conflict_row (s);

  തിരികെ default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

സ്ഥിതം ശൂന്യം
save_row (state_സംഖ്യ s)
{
  /* Number of non default actions in S.  */
  size_t എണ്ണം = 0;
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < ntokens; i++)
    എങ്കിൽ (actrow[i] != 0)
      എണ്ണം++;

  എങ്കിൽ (എണ്ണം)
    {
      /* Allocate non defaulted actions.  */
      base_സംഖ്യ *sp1 = froms[s] = xnmalloc (എണ്ണം, വലുപ്പം *sp1);
      base_സംഖ്യ *sp2 = tos[s] = xnmalloc (എണ്ണം, വലുപ്പം *sp2);
      പൂർണ്ണസംഖ്യ *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (എണ്ണം, വലുപ്പം *sp3) : ശൂന്യ;

      /* Store non defaulted actions.  */
      വേണ്ടി (ചിഹ്നം_സംഖ്യ i = 0; i < ntokens; i++)
        എങ്കിൽ (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            എങ്കിൽ (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = എണ്ണം;
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

സ്ഥിതം ശൂന്യം
token_actions (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ nconflict = nondeterministic_parser ? conflicts_total_എണ്ണം () : 0;

  yydefact = xnmalloc (nstates, വലുപ്പം *yydefact);

  actrow = xnmalloc (ntokens, വലുപ്പം *actrow);
  conflrow = xnmalloc (ntokens, വലുപ്പം *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, വലുപ്പം *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  എങ്കിൽ (!nondeterministic_parser)
    വേണ്ടി (rule_സംഖ്യ r = 0; r < nrules; ++r)
      rules[r].useful = false;

  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->സംഖ്യ + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      എങ്കിൽ (!nondeterministic_parser)
        {
          വേണ്ടി (ചിഹ്നം_സംഖ്യ j = 0; j < ntokens; ++j)
            എങ്കിൽ (actrow[j] < 0 && actrow[j] != ACTION_സംഖ്യ_MINIMUM)
              rules[item_സംഖ്യ_as_rule_സംഖ്യ (actrow[j])].useful = true;
          എങ്കിൽ (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  സ്വതന്ത്രം (actrow);
  സ്വതന്ത്രം (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
save_column (ചിഹ്നം_സംഖ്യ sym, state_സംഖ്യ default_state)
{
  സ്ഥിരം goto_സംഖ്യ begin = goto_map[sym - ntokens];
  സ്ഥിരം goto_സംഖ്യ end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t എണ്ണം = 0;
  വേണ്ടി (goto_സംഖ്യ i = begin; i < end; i++)
    എങ്കിൽ (to_state[i] != default_state)
      എണ്ണം++;

  എങ്കിൽ (എണ്ണം)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_സംഖ്യ symno = ചിഹ്നം_സംഖ്യ_to_vector_സംഖ്യ (sym);
      base_സംഖ്യ *sp1 = froms[symno] = xnmalloc (എണ്ണം, വലുപ്പം *sp1);
      base_സംഖ്യ *sp2 = tos[symno] = xnmalloc (എണ്ണം, വലുപ്പം *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      വേണ്ടി (goto_സംഖ്യ i = begin; i < end; i++)
        എങ്കിൽ (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = എണ്ണം;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

സ്ഥിതം state_സംഖ്യ
default_goto (ചിഹ്നം_സംഖ്യ sym, size_t state_എണ്ണം[])
{
  സ്ഥിരം goto_സംഖ്യ begin = goto_map[sym - ntokens];
  സ്ഥിരം goto_സംഖ്യ end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_സംഖ്യ res = 0;

  എങ്കിൽ (begin != end)
    {
      വേണ്ടി (state_സംഖ്യ s = 0; s < nstates; s++)
        state_എണ്ണം[s] = 0;

      വേണ്ടി (goto_സംഖ്യ i = begin; i < end; i++)
        state_എണ്ണം[to_state[i]]++;

      size_t max = 0;
      വേണ്ടി (state_സംഖ്യ s = 0; s < nstates; s++)
        എങ്കിൽ (max < state_എണ്ണം[s])
          {
            max = state_എണ്ണം[s];
            res = s;
          }
    }
  തിരികെ res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
goto_actions (ശൂന്യം)
{
  size_t *state_എണ്ണം = xnmalloc (nstates, വലുപ്പം *state_എണ്ണം);
  yydefgoto = xnmalloc (nnterms, വലുപ്പം *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
    {
      state_സംഖ്യ default_state = default_goto (i, state_എണ്ണം);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  സ്വതന്ത്രം (state_എണ്ണം);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

സ്ഥിതം ശൂന്യം
sort_actions (ശൂന്യം)
{
  nentries = 0;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nvectors; i++)
    എങ്കിൽ (0 < tally[i])
      {
        സ്ഥിരം size_t t = tally[i];
        സ്ഥിരം പൂർണ്ണസംഖ്യ w = width[i];
        പൂർണ്ണസംഖ്യ j = nentries - 1;

        ആകുമ്പോൾ (0 <= j && width[order[j]] < w)
          j--;

        ആകുമ്പോൾ (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        വേണ്ടി (പൂർണ്ണസംഖ്യ k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

സ്ഥിതം state_സംഖ്യ
matching_state (vector_സംഖ്യ vector)
{
  vector_സംഖ്യ i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  എങ്കിൽ (i < nstates)
    {
      size_t t = tally[i];
      പൂർണ്ണസംഖ്യ w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      എങ്കിൽ (conflict_tos[i] != ശൂന്യ)
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < t; j += 1)
          എങ്കിൽ (conflict_tos[i][j] != 0)
            തിരികെ -1;

      വേണ്ടി (പൂർണ്ണസംഖ്യ prev = vector - 1; 0 <= prev; prev--)
        {
          vector_സംഖ്യ j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          എങ്കിൽ (width[j] != w || tally[j] != t)
            തിരികെ -1;
          അല്ലെങ്കിൽ
            {
              bool match = true;
              വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; match && k < t; k++)
                എങ്കിൽ (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != ശൂന്യ && conflict_tos[j][k] != 0))
                  match = false;
              എങ്കിൽ (match)
                തിരികെ j;
            }
        }
    }
  തിരികെ -1;
}


സ്ഥിതം base_സംഖ്യ
pack_vector (vector_സംഖ്യ vector)
{
  vector_സംഖ്യ i = order[vector];
  size_t t = tally[i];
  base_സംഖ്യ *from = froms[i];
  base_സംഖ്യ *to = tos[i];
  പൂർണ്ണസംഖ്യ *conflict_to = conflict_tos[i];

  aver (t != 0);

  വേണ്ടി (base_സംഖ്യ res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; ok && k < t; k++)
          {
            പൂർണ്ണസംഖ്യ loc = res + state_സംഖ്യ_as_int (from[k]);
            എങ്കിൽ (table_size <= loc)
              table_grow (loc);

            എങ്കിൽ (table[loc] != 0)
              ok = false;
          }

        എങ്കിൽ (ok && pos_set_test (res))
          ok = false;
      }

      എങ്കിൽ (ok)
        {
          പൂർണ്ണസംഖ്യ loc PACIFY_CC (= -1);
          വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < t; k++)
            {
              loc = res + state_സംഖ്യ_as_int (from[k]);
              table[loc] = to[k];
              എങ്കിൽ (nondeterministic_parser && conflict_to != ശൂന്യ)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          ആകുമ്പോൾ (table[lowzero] != 0)
            lowzero++;

          എങ്കിൽ (മുകളിലെ < loc)
            മുകളിലെ = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          തിരികെ res;
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

സ്ഥിതം base_സംഖ്യ
table_ninf_remap (base_സംഖ്യ tab[], പൂർണ്ണസംഖ്യ size, base_സംഖ്യ ninf)
{
  base_സംഖ്യ res = 0;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < size; i++)
    എങ്കിൽ (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < size; i++)
    എങ്കിൽ (tab[i] == ninf)
      tab[i] = res;

  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
pack_table (ശൂന്യം)
{
  base = xnmalloc (nvectors, വലുപ്പം *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, വലുപ്പം *table);
  conflict_table = xcalloc (table_size, വലുപ്പം *conflict_table);
  check = xnmalloc (table_size, വലുപ്പം *check);

  lowzero = 0;
  മുകളിലെ = 0;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < table_size; i++)
    check[i] = -1;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nentries; i++)
    {
      state_സംഖ്യ s = matching_state (i);
      base_സംഖ്യ place;

      എങ്കിൽ (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      അല്ലെങ്കിൽ
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, മുകളിലെ + 1, ACTION_സംഖ്യ_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

ശൂന്യം
tables_generate (ശൂന്യം)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (വലുപ്പം nstates <= വലുപ്പം nvectors);
  verify (വലുപ്പം nnterms <= വലുപ്പം nvectors);

  nvectors = state_സംഖ്യ_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, വലുപ്പം *froms);
  tos = xcalloc (nvectors, വലുപ്പം *tos);
  conflict_tos = xcalloc (nvectors, വലുപ്പം *conflict_tos);
  tally = xcalloc (nvectors, വലുപ്പം *tally);
  width = xnmalloc (nvectors, വലുപ്പം *width);

  token_actions ();

  goto_actions ();
  സ്വതന്ത്രം (goto_map);
  സ്വതന്ത്രം (from_state);
  സ്വതന്ത്രം (to_state);

  order = xcalloc (nvectors, വലുപ്പം *order);
  sort_actions ();
  pack_table ();
  സ്വതന്ത്രം (order);

  സ്വതന്ത്രം (tally);
  സ്വതന്ത്രം (width);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nvectors; i++)
    {
      സ്വതന്ത്രം (froms[i]);
      സ്വതന്ത്രം (tos[i]);
      സ്വതന്ത്രം (conflict_tos[i]);
    }

  സ്വതന്ത്രം (froms);
  സ്വതന്ത്രം (tos);
  സ്വതന്ത്രം (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

ശൂന്യം
tables_free (ശൂന്യം)
{
  സ്വതന്ത്രം (base);
  സ്വതന്ത്രം (conflict_table);
  സ്വതന്ത്രം (conflict_list);
  സ്വതന്ത്രം (table);
  സ്വതന്ത്രം (check);
  സ്വതന്ത്രം (yydefgoto);
  സ്വതന്ത്രം (yydefact);
}
