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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>
#ଅନ୍ତର୍ଭାଵ <bitsetv.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "conflicts.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"
#ଅନ୍ତର୍ଭାଵ "tables.h"

/* Several tables are indexed both by state and nonterminal numbers.
   We call such an index a 'vector'; i.e., a vector is either a state
   or a nonterminal number.

   Of course vector_number_t ought to be wide enough to contain
   state_number and symbol_number.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା vector_ସଂଖ୍ୟା;

#ଯଦି 0 /* Not currently used.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ vector_ସଂଖ୍ୟା
state_ସଂଖ୍ୟା_to_vector_ସଂଖ୍ୟା (state_ସଂଖ୍ୟା s)
{
  ଫେରନ୍ତୁ s;
}
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ vector_ସଂଖ୍ୟା
ପ୍ରତୀକ_ସଂଖ୍ୟା_to_vector_ସଂଖ୍ୟା (ପ୍ରତୀକ_ସଂଖ୍ୟା sym)
{
  ଫେରନ୍ତୁ state_ସଂଖ୍ୟା_as_int (nstates) + sym - ntokens;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvectors;


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

ସ୍ଥିର base_ସଂଖ୍ୟା **froms;
ସ୍ଥିର base_ସଂଖ୍ୟା **tos;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା **conflict_tos;
ସ୍ଥିର size_t *tally;
ସ୍ଥିର base_ସଂଖ୍ୟା *width;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା action_ସଂଖ୍ୟା;
#define ACTION_ସଂଖ୍ୟା_MINIMUM INT_MIN

ସ୍ଥିର action_ସଂଖ୍ୟା *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
ସ୍ଥିର vector_ସଂଖ୍ୟା *order;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nentries;

base_ସଂଖ୍ୟା *base = ଖାଲି;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
base_ସଂଖ୍ୟା base_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
ସ୍ଥିର bitset pos_set = ଖାଲି;
/* The integer denoted by bitno 0 in pos_set.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos_set_base = 0;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *conflrow;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା *conflict_table;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା *conflict_list;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା conflict_list_cnt;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_size = 32768;
base_ସଂଖ୍ୟା *table;
base_ସଂଖ୍ୟା *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
base_ସଂଖ୍ୟା table_ninf = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା lowzero;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର;

state_ସଂଖ୍ୟା *yydefgoto;
rule_ସଂଖ୍ୟା *yydefact;


/*----------.
| pos_set.  |
`----------*/

#ଯଦି 0
ସ୍ଥିର ଶୂନ୍ୟ
pos_set_dump (ଶୂନ୍ୟ)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_size (pos_set), pos_set_base);
  bitset_iterator biter;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
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

ସ୍ଥିର ଶୂନ୍ୟ
pos_set_set (ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bitno = pos - pos_set_base;
  ଯଦି (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = pos_set_base - pos;
      ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_size = bitset_size (pos_set);
      ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_size = old_size + delta;
      bitset_resize (pos_set, new_size);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = new_size - 1; 0 <= i ; --i)
        ଯଦି (delta <= i && bitset_test (pos_set, i - delta))
          bitset_set (pos_set, i);
        ଅନ୍ୟଥା
          bitset_reset (pos_set, i);
      pos_set_base = pos;
      bitno = 0;
    }
  ଅନ୍ୟଥା ଯଦି (bitset_size (pos_set) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_set, bitno + 1);
  bitset_set (pos_set, bitno);
}

ସ୍ଥିର bool
pos_set_test (ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos)
{
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା bitno = pos - pos_set_base;
  ଫେରନ୍ତୁ bitset_test (pos_set, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
table_grow (ପୂର୍ଣ୍ଣସଂଖ୍ୟା desired)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_size = table_size;

  ଯେତେବେଳେ (table_size <= desired)
    table_size *= 2;

  ଯଦି (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_size, table_size);

  table = xnrealloc (table, table_size, ଆକାର *table);
  memset (table + old_size, 0,
          ଆକାର *table * (table_size - old_size));

  conflict_table = xnrealloc (conflict_table, table_size,
                              ଆକାର *conflict_table);
  memset (conflict_table + old_size, 0,
          ଆକାର *conflict_table * (table_size - old_size));

  check = xnrealloc (check, table_size, ଆକାର *check);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = old_size; i < table_size; ++i)
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

ସ୍ଥିର ଶୂନ୍ୟ
conflict_row (state *s)
{
  ଯଦି (!nondeterministic_parser)
    ଫେରନ୍ତୁ;

  ଅଚଳ reductions *reds = s->reductions;
  ପାଇଁ (state_ସଂଖ୍ୟା j = 0; j < ntokens; j += 1)
    ଯଦି (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; i += 1)
          ଯଦି (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != rule_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (reds->rules[i]->ସଂଖ୍ୟା)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->rules[i]->ସଂଖ୍ୟା + 1;
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

ସ୍ଥିର rule *
action_row (state *s)
{
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  ଯଦି (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = reds->ଅଙ୍କ - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            ଯଦି (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = rule_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (reds->rules[i]->ସଂଖ୍ୟା);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    FOR_EACH_SHIFT (trans, i)
      {
        ପ୍ରତୀକ_ସଂଖ୍ୟା sym = TRANSITION_ପ୍ରତୀକ (trans, i);
        state *shift_state = trans->states[i];

        ଯଦି (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_ସଂଖ୍ୟା_as_int (shift_state->ସଂଖ୍ୟା);

        /* Do not use any default reduction if there is a shift for
           error */
        ଯଦି (sym == errtoken->content->ସଂଖ୍ୟା)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < errp->ଅଙ୍କ; i++)
    {
      ପ୍ରତୀକ *sym = errp->symbols[i];
      actrow[sym->content->ସଂଖ୍ୟା] = ACTION_ସଂଖ୍ୟା_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    ଅକ୍ଷର *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    ଯଦି (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    ମୁକ୍ତ (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  rule *default_reduction = ଖାଲି;
  ଯଦି (reds->ଅଙ୍କ >= 1 && !nodefault)
    {
      ଯଦି (s->consistent)
        default_reduction = reds->rules[0];
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା max = 0;
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; i++)
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
              rule *r = reds->rules[i];
              ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା j = 0; j < ntokens; j++)
                ଯଦି (actrow[j] == rule_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (r->ସଂଖ୍ୟା))
                  ଗଣନା_2++;

              ଯଦି (ଗଣନା_2 > max)
                {
                  max = ଗଣନା_2;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          ଯଦି (0 < max)
            ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା j = 0; j < ntokens; j++)
              ଯଦି (actrow[j]
                  == rule_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (default_reduction->ସଂଖ୍ୟା)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  ଯଦି (!default_reduction)
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < ntokens; i++)
      ଯଦି (actrow[i] == ACTION_ସଂଖ୍ୟା_MINIMUM)
        actrow[i] = 0;

  ଯଦି (conflicted)
    conflict_row (s);

  ଫେରନ୍ତୁ default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
save_row (state_ସଂଖ୍ୟା s)
{
  /* Number of non default actions in S.  */
  size_t ଗଣନା_2 = 0;
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < ntokens; i++)
    ଯଦି (actrow[i] != 0)
      ଗଣନା_2++;

  ଯଦି (ଗଣନା_2)
    {
      /* Allocate non defaulted actions.  */
      base_ସଂଖ୍ୟା *sp1 = froms[s] = xnmalloc (ଗଣନା_2, ଆକାର *sp1);
      base_ସଂଖ୍ୟା *sp2 = tos[s] = xnmalloc (ଗଣନା_2, ଆକାର *sp2);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (ଗଣନା_2, ଆକାର *sp3) : ଖାଲି;

      /* Store non defaulted actions.  */
      ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = 0; i < ntokens; i++)
        ଯଦି (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            ଯଦି (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = ଗଣନା_2;
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

ସ୍ଥିର ଶୂନ୍ୟ
token_actions (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nconflict = nondeterministic_parser ? conflicts_total_ଗଣନା () : 0;

  yydefact = xnmalloc (nstates, ଆକାର *yydefact);

  actrow = xnmalloc (ntokens, ଆକାର *actrow);
  conflrow = xnmalloc (ntokens, ଆକାର *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, ଆକାର *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  ଯଦି (!nondeterministic_parser)
    ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
      rules[r].useful = false;

  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      rule *default_reduction = action_row (states[i]);
      yydefact[i] = default_reduction ? default_reduction->ସଂଖ୍ୟା + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      ଯଦି (!nondeterministic_parser)
        {
          ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା j = 0; j < ntokens; ++j)
            ଯଦି (actrow[j] < 0 && actrow[j] != ACTION_ସଂଖ୍ୟା_MINIMUM)
              rules[item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (actrow[j])].useful = true;
          ଯଦି (yydefact[i])
            rules[yydefact[i] - 1].useful = true;
        }
    }
  ମୁକ୍ତ (actrow);
  ମୁକ୍ତ (conflrow);
}


/*------------------------------------------------------------------.
| Compute FROMS[VECTOR], TOS[VECTOR], TALLY[VECTOR], WIDTH[VECTOR], |
| i.e., the information related to non defaulted GOTO on the nterm  |
| SYM.                                                              |
|                                                                   |
| DEFAULT_STATE is the principal destination on SYM, i.e., the      |
| default GOTO destination on SYM.                                  |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
save_column (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, state_ସଂଖ୍ୟା default_state)
{
  ଅଚଳ goto_ସଂଖ୍ୟା begin = goto_map[sym - ntokens];
  ଅଚଳ goto_ସଂଖ୍ୟା end = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t ଗଣନା_2 = 0;
  ପାଇଁ (goto_ସଂଖ୍ୟା i = begin; i < end; i++)
    ଯଦି (to_state[i] != default_state)
      ଗଣନା_2++;

  ଯଦି (ଗଣନା_2)
    {
      /* Allocate room for non defaulted gotos.  */
      vector_ସଂଖ୍ୟା symno = ପ୍ରତୀକ_ସଂଖ୍ୟା_to_vector_ସଂଖ୍ୟା (sym);
      base_ସଂଖ୍ୟା *sp1 = froms[symno] = xnmalloc (ଗଣନା_2, ଆକାର *sp1);
      base_ସଂଖ୍ୟା *sp2 = tos[symno] = xnmalloc (ଗଣନା_2, ଆକାର *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      ପାଇଁ (goto_ସଂଖ୍ୟା i = begin; i < end; i++)
        ଯଦି (to_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = to_state[i];
          }

      tally[symno] = ଗଣନା_2;
      width[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

ସ୍ଥିର state_ସଂଖ୍ୟା
default_goto (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, size_t state_ଗଣନା[])
{
  ଅଚଳ goto_ସଂଖ୍ୟା begin = goto_map[sym - ntokens];
  ଅଚଳ goto_ସଂଖ୍ୟା end = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_ସଂଖ୍ୟା res = 0;

  ଯଦି (begin != end)
    {
      ପାଇଁ (state_ସଂଖ୍ୟା s = 0; s < nstates; s++)
        state_ଗଣନା[s] = 0;

      ପାଇଁ (goto_ସଂଖ୍ୟା i = begin; i < end; i++)
        state_ଗଣନା[to_state[i]]++;

      size_t max = 0;
      ପାଇଁ (state_ସଂଖ୍ୟା s = 0; s < nstates; s++)
        ଯଦି (max < state_ଗଣନା[s])
          {
            max = state_ଗଣନା[s];
            res = s;
          }
    }
  ଫେରନ୍ତୁ res;
}


/*-------------------------------------------------------------------.
| Figure out what to do after reducing with each rule, depending on  |
| the saved state from before the beginning of parsing the data that |
| matched this rule.                                                 |
|                                                                    |
| The YYDEFGOTO table is output now.  The detailed info is saved for |
| putting into YYTABLE later.                                        |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
goto_actions (ଶୂନ୍ୟ)
{
  size_t *state_ଗଣନା = xnmalloc (nstates, ଆକାର *state_ଗଣନା);
  yydefgoto = xnmalloc (nnterms, ଆକାର *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    {
      state_ସଂଖ୍ୟା default_state = default_goto (i, state_ଗଣନା);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  ମୁକ୍ତ (state_ଗଣନା);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
sort_actions (ଶୂନ୍ୟ)
{
  nentries = 0;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvectors; i++)
    ଯଦି (0 < tally[i])
      {
        ଅଚଳ size_t t = tally[i];
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = width[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = nentries - 1;

        ଯେତେବେଳେ (0 <= j && width[order[j]] < w)
          j--;

        ଯେତେବେଳେ (0 <= j && width[order[j]] == w && tally[order[j]] < t)
          j--;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = nentries - 1; k > j; k--)
          order[k + 1] = order[k];

        order[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

ସ୍ଥିର state_ସଂଖ୍ୟା
matching_state (vector_ସଂଖ୍ୟା vector)
{
  vector_ସଂଖ୍ୟା i = order[vector];
  /* If VECTOR is a nterm, return -1.  */
  ଯଦି (i < nstates)
    {
      size_t t = tally[i];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = width[i];

      /* If VECTOR has GLR conflicts, return -1 */
      ଯଦି (conflict_tos[i] != ଖାଲି)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < t; j += 1)
          ଯଦି (conflict_tos[i][j] != 0)
            ଫେରନ୍ତୁ -1;

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev = vector - 1; 0 <= prev; prev--)
        {
          vector_ସଂଖ୍ୟା j = order[prev];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          ଯଦି (width[j] != w || tally[j] != t)
            ଫେରନ୍ତୁ -1;
          ଅନ୍ୟଥା
            {
              bool match = true;
              ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; match && k < t; k++)
                ଯଦି (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != ଖାଲି && conflict_tos[j][k] != 0))
                  match = false;
              ଯଦି (match)
                ଫେରନ୍ତୁ j;
            }
        }
    }
  ଫେରନ୍ତୁ -1;
}


ସ୍ଥିର base_ସଂଖ୍ୟା
pack_vector (vector_ସଂଖ୍ୟା vector)
{
  vector_ସଂଖ୍ୟା i = order[vector];
  size_t t = tally[i];
  base_ସଂଖ୍ୟା *from = froms[i];
  base_ସଂଖ୍ୟା *to = tos[i];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *conflict_to = conflict_tos[i];

  aver (t != 0);

  ପାଇଁ (base_ସଂଖ୍ୟା res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < table_size);
      {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; ok && k < t; k++)
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc = res + state_ସଂଖ୍ୟା_as_int (from[k]);
            ଯଦି (table_size <= loc)
              table_grow (loc);

            ଯଦି (table[loc] != 0)
              ok = false;
          }

        ଯଦି (ok && pos_set_test (res))
          ok = false;
      }

      ଯଦି (ok)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc PACIFY_CC (= -1);
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < t; k++)
            {
              loc = res + state_ସଂଖ୍ୟା_as_int (from[k]);
              table[loc] = to[k];
              ଯଦି (nondeterministic_parser && conflict_to != ଖାଲି)
                conflict_table[loc] = conflict_to[k];
              check[loc] = from[k];
            }

          ଯେତେବେଳେ (table[lowzero] != 0)
            lowzero++;

          ଯଦି (ଉପର < loc)
            ଉପର = loc;

          aver (BASE_MINIMUM <= res && res <= BASE_MAXIMUM);
          ଫେରନ୍ତୁ res;
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

ସ୍ଥିର base_ସଂଖ୍ୟା
table_ninf_remap (base_ସଂଖ୍ୟା tab[], ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, base_ସଂଖ୍ୟା ninf)
{
  base_ସଂଖ୍ୟା res = 0;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size; i++)
    ଯଦି (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size; i++)
    ଯଦି (tab[i] == ninf)
      tab[i] = res;

  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
pack_table (ଶୂନ୍ୟ)
{
  base = xnmalloc (nvectors, ଆକାର *base);
  pos_set = bitset_create (table_size + nstates, BITSET_FRUGAL);
  pos_set_base = -nstates;
  table = xcalloc (table_size, ଆକାର *table);
  conflict_table = xcalloc (table_size, ଆକାର *conflict_table);
  check = xnmalloc (table_size, ଆକାର *check);

  lowzero = 0;
  ଉପର = 0;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvectors; i++)
    base[i] = BASE_MINIMUM;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < table_size; i++)
    check[i] = -1;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nentries; i++)
    {
      state_ସଂଖ୍ୟା s = matching_state (i);
      base_ସଂଖ୍ୟା place;

      ଯଦି (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_vector (i);
      ଅନ୍ୟଥା
        /* Action of I were already coded for S.  */
        place = base[s];

      pos_set_set (place);
      base[order[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  base_ninf = table_ninf_remap (base, nvectors, BASE_MINIMUM);
  table_ninf = table_ninf_remap (table, ଉପର + 1, ACTION_ସଂଖ୍ୟା_MINIMUM);

  bitset_free (pos_set);
}



/*-----------------------------------------------------------------.
| Compute and output yydefact, yydefgoto, yypact, yypgoto, yytable |
| and yycheck.                                                     |
`-----------------------------------------------------------------*/

ଶୂନ୍ୟ
tables_generate (ଶୂନ୍ୟ)
{
  /* This is a poor way to make sure the sizes are properly
     correlated.  In particular the signedness is not taken into
     account.  But it's not useless.  */
  verify (ଆକାର nstates <= ଆକାର nvectors);
  verify (ଆକାର nnterms <= ଆକାର nvectors);

  nvectors = state_ସଂଖ୍ୟା_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, ଆକାର *froms);
  tos = xcalloc (nvectors, ଆକାର *tos);
  conflict_tos = xcalloc (nvectors, ଆକାର *conflict_tos);
  tally = xcalloc (nvectors, ଆକାର *tally);
  width = xnmalloc (nvectors, ଆକାର *width);

  token_actions ();

  goto_actions ();
  ମୁକ୍ତ (goto_map);
  ମୁକ୍ତ (from_state);
  ମୁକ୍ତ (to_state);

  order = xcalloc (nvectors, ଆକାର *order);
  sort_actions ();
  pack_table ();
  ମୁକ୍ତ (order);

  ମୁକ୍ତ (tally);
  ମୁକ୍ତ (width);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvectors; i++)
    {
      ମୁକ୍ତ (froms[i]);
      ମୁକ୍ତ (tos[i]);
      ମୁକ୍ତ (conflict_tos[i]);
    }

  ମୁକ୍ତ (froms);
  ମୁକ୍ତ (tos);
  ମୁକ୍ତ (conflict_tos);
}


/*-------------------------.
| Free the parser tables.  |
`-------------------------*/

ଶୂନ୍ୟ
tables_free (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (base);
  ମୁକ୍ତ (conflict_table);
  ମୁକ୍ତ (conflict_list);
  ମୁକ୍ତ (table);
  ମୁକ୍ତ (check);
  ମୁକ୍ତ (yydefgoto);
  ମୁକ୍ତ (yydefact);
}
