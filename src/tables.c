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
प्रकारनिर्देश पूर्णाङ्क सदिशम्_सङ्ख्या;

#यदि 0 /* Not currently used.  */
स्थिर अन्तःस्थ सदिशम्_सङ्ख्या
state_सङ्ख्या_गन्तव्यम्_सदिशम्_सङ्ख्या (state_सङ्ख्या s)
{
  प्रत्यागम s;
}
#यदि_अन्तः

स्थिर अन्तःस्थ सदिशम्_सङ्ख्या
चिह्नम्_सङ्ख्या_गन्तव्यम्_सदिशम्_सङ्ख्या (चिह्नम्_सङ्ख्या sym)
{
  प्रत्यागम state_सङ्ख्या_as_int (nstates) + sym - ntokens;
}

पूर्णाङ्क nvectors;


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
#परिभाषय आधारः_अधिकतमम् अधिकतम_पूर्णाङ्कः
#परिभाषय आधारः_MINIMUM INT_MIN

स्थिर आधारः_सङ्ख्या **froms;
स्थिर आधारः_सङ्ख्या **tos;
स्थिर पूर्णाङ्क **conflict_tos;
स्थिर size_t *tally;
स्थिर आधारः_सङ्ख्या *विस्तारः;


/* For a given state, N = ACTROW[SYMBOL]:

   If N = 0, stands for 'run the default action'.
   If N = MIN, stands for 'raise a syntax error'.
   If N > 0, stands for 'shift SYMBOL and go to n'.
   If N < 0, stands for 'reduce -N'.  */
प्रकारनिर्देश पूर्णाङ्क action_सङ्ख्या;
#परिभाषय ACTION_सङ्ख्या_MINIMUM INT_MIN

स्थिर action_सङ्ख्या *actrow;

/* FROMS and TOS are reordered to be compressed.  ORDER[VECTOR] is the
   new vector number of VECTOR.  We skip 'empty' vectors (i.e.,
   TALLY[VECTOR] = 0), and call these 'entries'.  */
स्थिर सदिशम्_सङ्ख्या *क्रमः;
स्थिर पूर्णाङ्क nentries;

आधारः_सङ्ख्या *आधारः = रिक्त;
/* A distinguished value of BASE, negative infinite.  During the
   computation equals to BASE_MINIMUM, later mapped to BASE_NINF to
   keep parser tables small.  */
आधारः_सङ्ख्या आधारः_ninf = 0;

/* Bitset representing an integer set in the range
   POS_SET_OFFSET..(POS_SET_OFFSET + SIZE).  POS_SET_OFFSET is
   nonpositive. */
स्थिर bitset pos_समुच्चयः = रिक्त;
/* The integer denoted by bitno 0 in pos_set.  */
स्थिर पूर्णाङ्क pos_समुच्चयः_आधारः = 0;

स्थिर पूर्णाङ्क *conflrow;
पूर्णाङ्क *conflict_सारणी;
पूर्णाङ्क *conflict_list;
पूर्णाङ्क conflict_list_cnt;
स्थिर पूर्णाङ्क conflict_list_free;

/* TABLE_SIZE is the allocated size of both TABLE and CHECK.  We start
   with more or less the original hard-coded value (which was
   SHRT_MAX).  */
स्थिर पूर्णाङ्क सारणी_आकार = 32768;
आधारः_सङ्ख्या *सारणी;
आधारः_सङ्ख्या *check;
/* The value used in TABLE to denote explicit syntax errors
   (%nonassoc), a negative infinite.  First defaults to ACTION_NUMBER_MINIMUM,
   but in order to keep small tables, renumbered as TABLE_ERROR, which
   is the smallest (non error) value minus 1.  */
आधारः_सङ्ख्या सारणी_ninf = 0;
स्थिर पूर्णाङ्क lowzero;
पूर्णाङ्क उच्चः;

state_सङ्ख्या *yydefgoto;
नियमः_सङ्ख्या *yydefact;


/*----------.
| pos_set.  |
`----------*/

#यदि 0
स्थिर शून्य
pos_समुच्चयः_dump (शून्य)
{
  fprintf (stderr, "pos_set (%ld, %d) =", bitset_आकार (pos_समुच्चयः), pos_समुच्चयः_आधारः);
  bitset_iterator biter;
  पूर्णाङ्क i;
  BITSET_FOR_EACH (biter, pos_समुच्चयः, i, 0)
    fprintf (stderr, " %d", i + pos_समुच्चयः_आधारः);
  putc ('\n', stderr);
}
#यदि_अन्तः


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
pos_समुच्चयः_समुच्चयः (पूर्णाङ्क pos)
{
  पूर्णाङ्क bitno = pos - pos_समुच्चयः_आधारः;
  यदि (bitno < 0)
    {
      // Need more room on the left.
      // DELTA is positive.  Run 'pos_set >> delta'.
      ध्रुव पूर्णाङ्क delta = pos_समुच्चयः_आधारः - pos;
      ध्रुव पूर्णाङ्क old_आकार = bitset_आकार (pos_समुच्चयः);
      ध्रुव पूर्णाङ्क new_आकार = old_आकार + delta;
      bitset_resize (pos_समुच्चयः, new_आकार);
      // Right-shift all the bits by DELTA.  Be sure to reset the new
      // bits on the left.
      //
      // FIXME: add bitset_assign, and bitset_shift?
      प्रति (पूर्णाङ्क i = new_आकार - 1; 0 <= i ; --i)
        यदि (delta <= i && bitset_test (pos_समुच्चयः, i - delta))
          bitset_समुच्चयः (pos_समुच्चयः, i);
        अन्यथा
          bitset_reset (pos_समुच्चयः, i);
      pos_समुच्चयः_आधारः = pos;
      bitno = 0;
    }
  अन्यथा यदि (bitset_आकार (pos_समुच्चयः) <= bitno)
    // Need more room on the right.
    bitset_resize (pos_समुच्चयः, bitno + 1);
  bitset_समुच्चयः (pos_समुच्चयः, bitno);
}

स्थिर bool
pos_समुच्चयः_test (पूर्णाङ्क pos)
{
  ध्रुव पूर्णाङ्क bitno = pos - pos_समुच्चयः_आधारः;
  प्रत्यागम bitset_test (pos_समुच्चयः, bitno);
}


/*-------------------------------------------------------------------.
| If TABLE, CONFLICT_TABLE, and CHECK are too small to be addressed  |
| at DESIRED, grow them.  TABLE[DESIRED] can be used, so the desired |
| size is at least DESIRED + 1.                                      |
`-------------------------------------------------------------------*/

स्थिर शून्य
सारणी_grow (पूर्णाङ्क desired)
{
  पूर्णाङ्क old_आकार = सारणी_आकार;

  यावत् (सारणी_आकार <= desired)
    सारणी_आकार *= 2;

  यदि (trace_flag & trace_resource)
    fprintf (stderr, "growing tables from %d to %d\n",
             old_आकार, सारणी_आकार);

  सारणी = xnrealloc (सारणी, सारणी_आकार, परिमाण *सारणी);
  स्मृतिं_पूरय (सारणी + old_आकार, 0,
          परिमाण *सारणी * (सारणी_आकार - old_आकार));

  conflict_सारणी = xnrealloc (conflict_सारणी, सारणी_आकार,
                              परिमाण *conflict_सारणी);
  स्मृतिं_पूरय (conflict_सारणी + old_आकार, 0,
          परिमाण *conflict_सारणी * (सारणी_आकार - old_आकार));

  check = xnrealloc (check, सारणी_आकार, परिमाण *check);
  प्रति (पूर्णाङ्क i = old_आकार; i < सारणी_आकार; ++i)
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
  यदि (!nondeterministic_parser)
    प्रत्यागम;

  ध्रुव reductions *reds = s->reductions;
  प्रति (state_सङ्ख्या j = 0; j < ntokens; j += 1)
    यदि (conflrow[j])
      {
        conflrow[j] = conflict_list_cnt;

        /* Find all reductions for token J, and record all that do not
           match ACTROW[J].  */
        प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; i += 1)
          यदि (bitset_test (reds->lookaheads[i], j)
              && (actrow[j]
                  != नियमः_सङ्ख्या_as_item_सङ्ख्या (reds->नियमाः[i]->सङ्ख्या)))
            {
              aver (0 < conflict_list_free);
              conflict_list[conflict_list_cnt] = reds->नियमाः[i]->सङ्ख्या + 1;
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

स्थिर नियमः *
action_row (state *s)
{
  प्रति (state_सङ्ख्या i = 0; i < ntokens; i++)
    actrow[i] = conflrow[i] = 0;

  reductions *reds = s->reductions;
  bool conflicted = false;
  यदि (reds->lookaheads)
    /* loop over all the rules available here which require
       lookahead (in reverse order to give precedence to the first
       rule) */
    प्रति (पूर्णाङ्क i = reds->अङ्कः - 1; 0 <= i; --i)
      /* and find each token which the rule finds acceptable
         to come next */
      {
        bitset_iterator biter;
        पूर्णाङ्क j;
        BITSET_FOR_EACH (biter, reds->lookaheads[i], j, 0)
          {
            /* and record this rule as the rule to use if that
               token follows.  */
            यदि (actrow[j] != 0)
              {
                conflicted = true;
                conflrow[j] = 1;
              }
            actrow[j] = नियमः_सङ्ख्या_as_item_सङ्ख्या (reds->नियमाः[i]->सङ्ख्या);
          }
      }

  /* Now see which tokens are allowed for shifts in this state.  For
     them, record the shift as the thing to do.  So shift is preferred
     to reduce.  */
  transitions *trans = s->transitions;
  /* Set to nonzero to inhibit having any default reduction.  */
  bool nodefault = false;
  {
    पूर्णाङ्क i;
    FOR_EACH_सरणम् (trans, i)
      {
        चिह्नम्_सङ्ख्या sym = TRANSITION_चिह्नम् (trans, i);
        state *सरणम्_state = trans->अवस्थाः[i];

        यदि (actrow[sym] != 0)
          {
            conflicted = true;
            conflrow[sym] = 1;
          }
        actrow[sym] = state_सङ्ख्या_as_int (सरणम्_state->सङ्ख्या);

        /* Do not use any default reduction if there is a shift for
           error */
        यदि (sym == errtoken->content->सङ्ख्या)
          nodefault = true;
      }
  }

  /* See which tokens are an explicit error in this state (due to
     %nonassoc).  For them, record ACTION_NUMBER_MINIMUM as the
     action.  */
  errs *errp = s->errs;
  प्रति (पूर्णाङ्क i = 0; i < errp->अङ्कः; i++)
    {
      चिह्नम् *sym = errp->symbols[i];
      actrow[sym->content->सङ्ख्या] = ACTION_सङ्ख्या_MINIMUM;
    }

  /* Turn off default reductions where requested by the user.  See
     state_lookaheads_count in lalr.c to understand when states are
     labeled as consistent.  */
  {
    वर्ण *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    यदि (STRNEQ (default_reductions, "most") && !s->consistent)
      nodefault = true;
    मुक्त (default_reductions);
  }

  /* Now find the most common reduction and make it the default action
     for this state.  */
  नियमः *default_reduction = रिक्त;
  यदि (reds->अङ्कः >= 1 && !nodefault)
    {
      यदि (s->consistent)
        default_reduction = reds->नियमाः[0];
      अन्यथा
        {
          पूर्णाङ्क max = 0;
          प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; i++)
            {
              पूर्णाङ्क गणनम् = 0;
              नियमः *r = reds->नियमाः[i];
              प्रति (चिह्नम्_सङ्ख्या j = 0; j < ntokens; j++)
                यदि (actrow[j] == नियमः_सङ्ख्या_as_item_सङ्ख्या (r->सङ्ख्या))
                  गणनम्++;

              यदि (गणनम् > max)
                {
                  max = गणनम्;
                  default_reduction = r;
                }
            }

          /* GLR parsers need space for conflict lists, so we can't
             default conflicted entries.  For non-conflicted entries
             or as long as we are not building a GLR parser,
             actions that match the default are replaced with zero,
             which means "use the default". */

          यदि (0 < max)
            प्रति (चिह्नम्_सङ्ख्या j = 0; j < ntokens; j++)
              यदि (actrow[j]
                  == नियमः_सङ्ख्या_as_item_सङ्ख्या (default_reduction->सङ्ख्या)
                  && ! (nondeterministic_parser && conflrow[j]))
                actrow[j] = 0;
        }
    }

  /* If have no default reduction, the default is an error.
     So replace any action which says "error" with "use default".  */

  यदि (!default_reduction)
    प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; i++)
      यदि (actrow[i] == ACTION_सङ्ख्या_MINIMUM)
        actrow[i] = 0;

  यदि (conflicted)
    conflict_row (s);

  प्रत्यागम default_reduction;
}


/*----------------------------------------.
| Set FROMS, TOS, TALLY and WIDTH for S.  |
`----------------------------------------*/

स्थिर शून्य
save_row (state_सङ्ख्या s)
{
  /* Number of non default actions in S.  */
  size_t गणनम् = 0;
  प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; i++)
    यदि (actrow[i] != 0)
      गणनम्++;

  यदि (गणनम्)
    {
      /* Allocate non defaulted actions.  */
      आधारः_सङ्ख्या *sp1 = froms[s] = xnmalloc (गणनम्, परिमाण *sp1);
      आधारः_सङ्ख्या *sp2 = tos[s] = xnmalloc (गणनम्, परिमाण *sp2);
      पूर्णाङ्क *sp3 = conflict_tos[s] =
        nondeterministic_parser ? xnmalloc (गणनम्, परिमाण *sp3) : रिक्त;

      /* Store non defaulted actions.  */
      प्रति (चिह्नम्_सङ्ख्या i = 0; i < ntokens; i++)
        यदि (actrow[i] != 0)
          {
            *sp1++ = i;
            *sp2++ = actrow[i];
            यदि (nondeterministic_parser)
              *sp3++ = conflrow[i];
          }

      tally[s] = गणनम्;
      विस्तारः[s] = sp1[-1] - froms[s][0] + 1;
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
  पूर्णाङ्क nconflict = nondeterministic_parser ? conflicts_योग_गणनम् () : 0;

  yydefact = xnmalloc (nstates, परिमाण *yydefact);

  actrow = xnmalloc (ntokens, परिमाण *actrow);
  conflrow = xnmalloc (ntokens, परिमाण *conflrow);

  conflict_list = xnmalloc (1 + 2 * nconflict, परिमाण *conflict_list);
  conflict_list_free = 2 * nconflict;
  conflict_list_cnt = 1;

  /* Find the rules which are reduced.  */
  यदि (!nondeterministic_parser)
    प्रति (नियमः_सङ्ख्या r = 0; r < nrules; ++r)
      नियमाः[r].useful = false;

  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      नियमः *default_reduction = action_row (अवस्थाः[i]);
      yydefact[i] = default_reduction ? default_reduction->सङ्ख्या + 1 : 0;
      save_row (i);

      /* Now that the parser was computed, we can find which rules are
         really reduced, and which are not because of SR or RR
         conflicts.  */
      यदि (!nondeterministic_parser)
        {
          प्रति (चिह्नम्_सङ्ख्या j = 0; j < ntokens; ++j)
            यदि (actrow[j] < 0 && actrow[j] != ACTION_सङ्ख्या_MINIMUM)
              नियमाः[item_सङ्ख्या_as_नियमः_सङ्ख्या (actrow[j])].useful = true;
          यदि (yydefact[i])
            नियमाः[yydefact[i] - 1].useful = true;
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
save_column (चिह्नम्_सङ्ख्या sym, state_सङ्ख्या default_state)
{
  ध्रुव goto_सङ्ख्या begin = goto_map[sym - ntokens];
  ध्रुव goto_सङ्ख्या अन्तः = goto_map[sym - ntokens + 1];

  /* Number of non default GOTO.  */
  size_t गणनम् = 0;
  प्रति (goto_सङ्ख्या i = begin; i < अन्तः; i++)
    यदि (गन्तव्यम्_state[i] != default_state)
      गणनम्++;

  यदि (गणनम्)
    {
      /* Allocate room for non defaulted gotos.  */
      सदिशम्_सङ्ख्या symno = चिह्नम्_सङ्ख्या_गन्तव्यम्_सदिशम्_सङ्ख्या (sym);
      आधारः_सङ्ख्या *sp1 = froms[symno] = xnmalloc (गणनम्, परिमाण *sp1);
      आधारः_सङ्ख्या *sp2 = tos[symno] = xnmalloc (गणनम्, परिमाण *sp2);

      /* Store the state numbers of the non defaulted gotos.  */
      प्रति (goto_सङ्ख्या i = begin; i < अन्तः; i++)
        यदि (गन्तव्यम्_state[i] != default_state)
          {
            *sp1++ = from_state[i];
            *sp2++ = गन्तव्यम्_state[i];
          }

      tally[symno] = गणनम्;
      विस्तारः[symno] = sp1[-1] - froms[symno][0] + 1;
    }
}


/*----------------------------------------------------------------.
| The default state for SYM: the state which is 'the' most common |
| GOTO destination on SYM (an nterm).                             |
`----------------------------------------------------------------*/

स्थिर state_सङ्ख्या
default_goto (चिह्नम्_सङ्ख्या sym, size_t state_गणनम्[])
{
  ध्रुव goto_सङ्ख्या begin = goto_map[sym - ntokens];
  ध्रुव goto_सङ्ख्या अन्तः = goto_map[sym - ntokens + 1];

  /* In the case this symbol is never reduced to ($accept), use state
     0.  We used to use -1, but as a result the yydefgoto table must
     be signed, which (1) might trigger compiler warnings when storing
     a value from yydefgoto into a state number (nonnegative), and (2)
     wastes bits which might result in using a int16 where a uint8
     suffices. */
  state_सङ्ख्या res = 0;

  यदि (begin != अन्तः)
    {
      प्रति (state_सङ्ख्या s = 0; s < nstates; s++)
        state_गणनम्[s] = 0;

      प्रति (goto_सङ्ख्या i = begin; i < अन्तः; i++)
        state_गणनम्[गन्तव्यम्_state[i]]++;

      size_t max = 0;
      प्रति (state_सङ्ख्या s = 0; s < nstates; s++)
        यदि (max < state_गणनम्[s])
          {
            max = state_गणनम्[s];
            res = s;
          }
    }
  प्रत्यागम res;
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
  size_t *state_गणनम् = xnmalloc (nstates, परिमाण *state_गणनम्);
  yydefgoto = xnmalloc (nnterms, परिमाण *yydefgoto);

  /* For a given nterm I, STATE_COUNT[S] is the number of times there
     is a GOTO to S on I.  */
  प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
    {
      state_सङ्ख्या default_state = default_goto (i, state_गणनम्);
      save_column (i, default_state);
      yydefgoto[i - ntokens] = default_state;
    }
  मुक्त (state_गणनम्);
}


/*------------------------------------------------------------------.
| Compute ORDER, a reordering of vectors, in order to decide how to |
| pack the actions and gotos information into yytable.              |
`------------------------------------------------------------------*/

स्थिर शून्य
क्रमय_actions (शून्य)
{
  nentries = 0;

  प्रति (पूर्णाङ्क i = 0; i < nvectors; i++)
    यदि (0 < tally[i])
      {
        ध्रुव size_t t = tally[i];
        ध्रुव पूर्णाङ्क w = विस्तारः[i];
        पूर्णाङ्क j = nentries - 1;

        यावत् (0 <= j && विस्तारः[क्रमः[j]] < w)
          j--;

        यावत् (0 <= j && विस्तारः[क्रमः[j]] == w && tally[क्रमः[j]] < t)
          j--;

        प्रति (पूर्णाङ्क k = nentries - 1; k > j; k--)
          क्रमः[k + 1] = क्रमः[k];

        क्रमः[j + 1] = i;
        nentries++;
      }
}


/* If VECTOR is a state whose actions (reflected by FROMS, TOS, TALLY
   and WIDTH of VECTOR) are common to a previous state, return this
   state number.

   In any other case, return -1.  */

स्थिर state_सङ्ख्या
matching_state (सदिशम्_सङ्ख्या सदिशम्)
{
  सदिशम्_सङ्ख्या i = क्रमः[सदिशम्];
  /* If VECTOR is a nterm, return -1.  */
  यदि (i < nstates)
    {
      size_t t = tally[i];
      पूर्णाङ्क w = विस्तारः[i];

      /* If VECTOR has GLR conflicts, return -1 */
      यदि (conflict_tos[i] != रिक्त)
        प्रति (पूर्णाङ्क j = 0; j < t; j += 1)
          यदि (conflict_tos[i][j] != 0)
            प्रत्यागम -1;

      प्रति (पूर्णाङ्क पूर्व = सदिशम् - 1; 0 <= पूर्व; पूर्व--)
        {
          सदिशम्_सङ्ख्या j = क्रमः[पूर्व];
          /* Given how ORDER was computed, if the WIDTH or TALLY is
             different, there cannot be a matching state.  */
          यदि (विस्तारः[j] != w || tally[j] != t)
            प्रत्यागम -1;
          अन्यथा
            {
              bool मेलनम् = true;
              प्रति (पूर्णाङ्क k = 0; मेलनम् && k < t; k++)
                यदि (tos[j][k] != tos[i][k]
                    || froms[j][k] != froms[i][k]
                    || (conflict_tos[j] != रिक्त && conflict_tos[j][k] != 0))
                  मेलनम् = false;
              यदि (मेलनम्)
                प्रत्यागम j;
            }
        }
    }
  प्रत्यागम -1;
}


स्थिर आधारः_सङ्ख्या
pack_सदिशम् (सदिशम्_सङ्ख्या सदिशम्)
{
  सदिशम्_सङ्ख्या i = क्रमः[सदिशम्];
  size_t t = tally[i];
  आधारः_सङ्ख्या *from = froms[i];
  आधारः_सङ्ख्या *गन्तव्यम् = tos[i];
  पूर्णाङ्क *conflict_गन्तव्यम् = conflict_tos[i];

  aver (t != 0);

  प्रति (आधारः_सङ्ख्या res = lowzero - from[0]; ; res++)
    {
      bool ok = true;
      aver (res < सारणी_आकार);
      {
        प्रति (पूर्णाङ्क k = 0; ok && k < t; k++)
          {
            पूर्णाङ्क loc = res + state_सङ्ख्या_as_int (from[k]);
            यदि (सारणी_आकार <= loc)
              सारणी_grow (loc);

            यदि (सारणी[loc] != 0)
              ok = false;
          }

        यदि (ok && pos_समुच्चयः_test (res))
          ok = false;
      }

      यदि (ok)
        {
          पूर्णाङ्क loc PACIFY_CC (= -1);
          प्रति (पूर्णाङ्क k = 0; k < t; k++)
            {
              loc = res + state_सङ्ख्या_as_int (from[k]);
              सारणी[loc] = गन्तव्यम्[k];
              यदि (nondeterministic_parser && conflict_गन्तव्यम् != रिक्त)
                conflict_सारणी[loc] = conflict_गन्तव्यम्[k];
              check[loc] = from[k];
            }

          यावत् (सारणी[lowzero] != 0)
            lowzero++;

          यदि (उच्चः < loc)
            उच्चः = loc;

          aver (आधारः_MINIMUM <= res && res <= आधारः_अधिकतमम्);
          प्रत्यागम res;
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

स्थिर आधारः_सङ्ख्या
सारणी_ninf_remap (आधारः_सङ्ख्या tab[], पूर्णाङ्क आकार, आधारः_सङ्ख्या ninf)
{
  आधारः_सङ्ख्या res = 0;

  प्रति (पूर्णाङ्क i = 0; i < आकार; i++)
    यदि (tab[i] < res && tab[i] != ninf)
      res = tab[i];

  --res;

  प्रति (पूर्णाङ्क i = 0; i < आकार; i++)
    यदि (tab[i] == ninf)
      tab[i] = res;

  प्रत्यागम res;
}

स्थिर शून्य
pack_सारणी (शून्य)
{
  आधारः = xnmalloc (nvectors, परिमाण *आधारः);
  pos_समुच्चयः = bitset_create (सारणी_आकार + nstates, BITSET_FRUGAL);
  pos_समुच्चयः_आधारः = -nstates;
  सारणी = xcalloc (सारणी_आकार, परिमाण *सारणी);
  conflict_सारणी = xcalloc (सारणी_आकार, परिमाण *conflict_सारणी);
  check = xnmalloc (सारणी_आकार, परिमाण *check);

  lowzero = 0;
  उच्चः = 0;

  प्रति (पूर्णाङ्क i = 0; i < nvectors; i++)
    आधारः[i] = आधारः_MINIMUM;

  प्रति (पूर्णाङ्क i = 0; i < सारणी_आकार; i++)
    check[i] = -1;

  प्रति (पूर्णाङ्क i = 0; i < nentries; i++)
    {
      state_सङ्ख्या s = matching_state (i);
      आधारः_सङ्ख्या place;

      यदि (s < 0)
        /* A new set of state actions, or a nonterminal.  */
        place = pack_सदिशम् (i);
      अन्यथा
        /* Action of I were already coded for S.  */
        place = आधारः[s];

      pos_समुच्चयः_समुच्चयः (place);
      आधारः[क्रमः[i]] = place;
    }

  /* Use the greatest possible negative infinites.  */
  आधारः_ninf = सारणी_ninf_remap (आधारः, nvectors, आधारः_MINIMUM);
  सारणी_ninf = सारणी_ninf_remap (सारणी, उच्चः + 1, ACTION_सङ्ख्या_MINIMUM);

  bitset_free (pos_समुच्चयः);
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
  verify (परिमाण nstates <= परिमाण nvectors);
  verify (परिमाण nnterms <= परिमाण nvectors);

  nvectors = state_सङ्ख्या_as_int (nstates) + nnterms;

  froms = xcalloc (nvectors, परिमाण *froms);
  tos = xcalloc (nvectors, परिमाण *tos);
  conflict_tos = xcalloc (nvectors, परिमाण *conflict_tos);
  tally = xcalloc (nvectors, परिमाण *tally);
  विस्तारः = xnmalloc (nvectors, परिमाण *विस्तारः);

  token_actions ();

  goto_actions ();
  मुक्त (goto_map);
  मुक्त (from_state);
  मुक्त (गन्तव्यम्_state);

  क्रमः = xcalloc (nvectors, परिमाण *क्रमः);
  क्रमय_actions ();
  pack_सारणी ();
  मुक्त (क्रमः);

  मुक्त (tally);
  मुक्त (विस्तारः);

  प्रति (पूर्णाङ्क i = 0; i < nvectors; i++)
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
  मुक्त (आधारः);
  मुक्त (conflict_सारणी);
  मुक्त (conflict_list);
  मुक्त (सारणी);
  मुक्त (check);
  मुक्त (yydefgoto);
  मुक्त (yydefact);
}
