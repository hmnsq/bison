/* Compute lookahead criteria for Bison.

   Copyright (C) 1984, 1986, 1989, 2000-2015, 2018-2022 Free Software
   Foundation, Inc.

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


/* Find which rules need lookahead in each state, and which lookahead
   tokens they accept.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <bitsetv.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "derives.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "lr0.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "relation.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

goto_ಸಂಖ್ಯೆ *goto_map = ಖಾಲಿ;
goto_ಸಂಖ್ಯೆ ngotos = 0;
state_ಸಂಖ್ಯೆ *from_state = ಖಾಲಿ;
state_ಸಂಖ್ಯೆ *ಗಮ್ಯ_state = ಖಾಲಿ;
bitsetv goto_follows = ಖಾಲಿ;

/* Linked list of goto numbers.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ goto_list
{
  ರಚನೆ goto_list *ಮುಂದಿನ;
  goto_ಸಂಖ್ಯೆ ಮೌಲ್ಯ;
} goto_list;

ಸ್ಥಿರ goto_list *
goto_list_new (goto_ಸಂಖ್ಯೆ ಮೌಲ್ಯ, ರಚನೆ goto_list *ಮುಂದಿನ)
{
  goto_list *res = xmalloc (ಗಾತ್ರ *res);
  res->ಮುಂದಿನ = ಮುಂದಿನ;
  res->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
  ಹಿಂತಿರುಗಿ res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

ಸ್ಥಿರ bitsetv LA = ಖಾಲಿ;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
ಸ್ಥಿರ goto_ಸಂಖ್ಯೆ **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
ಸ್ಥಿರ goto_list **lookback;

ಸ್ಥಿರ ಶೂನ್ಯ
goto_print (goto_ಸಂಖ್ಯೆ i, FILE *ಹೊರಹೋಗು)
{
  ಸ್ಥಿರಾಂಕ state_ಸಂಖ್ಯೆ src = from_state[i];
  ಸ್ಥಿರಾಂಕ state_ಸಂಖ್ಯೆ dst = ಗಮ್ಯ_state[i];
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ var = ಸ್ಥಿತಿಗಳು[dst]->accessing_ಚಿಹ್ನೆ;
  fprintf (ಹೊರಹೋಗು,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->ಟ್ಯಾಗ್, dst);
}

ಶೂನ್ಯ
ಗಣ_goto_map (ಶೂನ್ಯ)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  ಆದರೆ (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, ಗಾತ್ರ *goto_map);
  ngotos = 0;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; ++s)
    {
      transitions *trans = ಸ್ಥಿತಿಗಳು[s]->transitions;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = trans->ಅಂಕೆ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_ಸಂಖ್ಯೆ_ಗರಿಷ್ಠ);
          goto_map[TRANSITION_ಚಿಹ್ನೆ (trans, i) - ntokens]++;
        }
    }

  goto_ಸಂಖ್ಯೆ *ತಾತ್ಕಾಲಿಕ_map = xnmalloc (nnterms + 1, ಗಾತ್ರ *ತಾತ್ಕಾಲಿಕ_map);
  {
    goto_ಸಂಖ್ಯೆ k = 0;
    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      {
        ತಾತ್ಕಾಲಿಕ_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    ಗಾಗಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = ತಾತ್ಕಾಲಿಕ_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    ತಾತ್ಕಾಲಿಕ_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, ಗಾತ್ರ *from_state);
  ಗಮ್ಯ_state = xcalloc (ngotos, ಗಾತ್ರ *ಗಮ್ಯ_state);

  ಗಾಗಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; ++s)
    {
      ಸ್ಥಿರಾಂಕ transitions *trans = ಸ್ಥಿತಿಗಳು[s]->transitions;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = trans->ಅಂಕೆ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_ಸಂಖ್ಯೆ k = ತಾತ್ಕಾಲಿಕ_map[TRANSITION_ಚಿಹ್ನೆ (trans, i) - ntokens]++;
          from_state[k] = s;
          ಗಮ್ಯ_state[k] = trans->ಸ್ಥಿತಿಗಳು[i]->ಸಂಖ್ಯೆ;
        }
    }

  ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ_map);

  ಆದರೆ (trace_flag & trace_automaton)
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->ಟ್ಯಾಗ್,
                 goto_map[i], goto_map[i+1] - 1);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_ಸಂಖ್ಯೆ
map_goto (state_ಸಂಖ್ಯೆ src, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  goto_ಸಂಖ್ಯೆ ಕೆಳಗಿನ = goto_map[sym - ntokens];
  ಪ್ರತಿಜ್ಞಾ (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_ಸಂಖ್ಯೆ ಮೇಲಿನ = goto_map[sym - ntokens + 1] - 1;

  ಗಾಗಿ (;;)
    {
      aver (ಕೆಳಗಿನ <= ಮೇಲಿನ);
      goto_ಸಂಖ್ಯೆ ಮಧ್ಯ = (ಕೆಳಗಿನ + ಮೇಲಿನ) / 2;
      state_ಸಂಖ್ಯೆ s = from_state[ಮಧ್ಯ];
      ಆದರೆ (s == src)
        ಹಿಂತಿರುಗಿ ಮಧ್ಯ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s < src)
        ಕೆಳಗಿನ = ಮಧ್ಯ + 1;
      ಇಲ್ಲದಿದ್ದರೆ
        ಮೇಲಿನ = ಮಧ್ಯ - 1;
    }
}

/* Print FOLLOWS for debugging.  */
ಸ್ಥಿರ ಶೂನ್ಯ
follows_print (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* title, FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು, "%s:\n", title);
  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", ಹೊರಹೋಗು);
      goto_print (i, ಹೊರಹೋಗು);
      fputs ("] =", ಹೊರಹೋಗು);
      bitset_iterator iter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (ಹೊರಹೋಗು, " %s", symbols[sym]->ಟ್ಯಾಗ್);
      fputc ('\n', ಹೊರಹೋಗು);
    }
  fputc ('\n', ಹೊರಹೋಗು);
}

/* Build goto_follows. */
ಸ್ಥಿರ ಶೂನ್ಯ
initialize_goto_follows (ಶೂನ್ಯ)
{
  goto_ಸಂಖ್ಯೆ **reads = xnmalloc (ngotos, ಗಾತ್ರ *reads);
  goto_ಸಂಖ್ಯೆ *ಅಂಚು = xnmalloc (ngotos, ಗಾತ್ರ *ಅಂಚು);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      state_ಸಂಖ್ಯೆ dst = ಗಮ್ಯ_state[i];
      ಸ್ಥಿರಾಂಕ transitions *trans = ಸ್ಥಿತಿಗಳು[dst]->transitions;

      ಪೂರ್ಣಾಂಕ j;
      FOR_EACH_ಸರಿಸು (trans, j)
        bitset_ಗಣ (goto_follows[i], TRANSITION_ಚಿಹ್ನೆ (trans, j));

      /* Gotos outgoing from DST. */
      goto_ಸಂಖ್ಯೆ nedges = 0;
      ಗಾಗಿ (; j < trans->ಅಂಕೆ; ++j)
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = TRANSITION_ಚಿಹ್ನೆ (trans, j);
          ಆದರೆ (nullable[sym - ntokens])
            {
              ಪ್ರತಿಜ್ಞಾ (nedges < ngotos);
              ಅಂಚು[nedges++] = map_goto (dst, sym);
            }
        }

      ಆದರೆ (nedges == 0)
        reads[i] = ಖಾಲಿ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          reads[i] = xnmalloc (nedges + 1, ಗಾತ್ರ reads[i][0]);
          ಸ್ಮರಣೆ_ನಕಲು (reads[i], ಅಂಚು, nedges * ಗಾತ್ರ ಅಂಚು[0]);
          reads[i][nedges] = ಕೊನೆ_ಗಂಟು;
        }
    }
  ಆದರೆ (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      ಸಂಬಂಧ_print ("reads", reads, ngotos, goto_print, stderr);
    }

  ಸಂಬಂಧ_digraph (reads, ngotos, goto_follows);
  ಆದರೆ (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    ಬಿಡುಗಡೆ (reads[i]);
  ಬಿಡುಗಡೆ (reads);
  ಬಿಡುಗಡೆ (ಅಂಚು);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ state *
lookback_ಕಂಡುಹಿಡಿ_state (ಪೂರ್ಣಾಂಕ lookback_ಸೂಚ್ಯಂಕ)
{
  state *res = ಖಾಲಿ;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nstates; ++j)
    ಆದರೆ (ಸ್ಥಿತಿಗಳು[j]->reductions
        && ಸ್ಥಿತಿಗಳು[j]->reductions->lookaheads)
      {
        ಆದರೆ (ಸ್ಥಿತಿಗಳು[j]->reductions->lookaheads - LA > lookback_ಸೂಚ್ಯಂಕ)
          /* Went too far. */
          ನಿಲ್ಲಿಸು;
        ಇಲ್ಲದಿದ್ದರೆ
          res = ಸ್ಥಿತಿಗಳು[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  ಆದರೆ (!res)
    abort ();
  ಹಿಂತಿರುಗಿ res;
}

/* Print LOOKBACK for debugging.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lookback_print (FILE *ಹೊರಹೋಗು)
{
  fputs ("lookback:\n", ಹೊರಹೋಗು);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nLA; ++i)
    ಆದರೆ (lookback[i])
    {
      fprintf (ಹೊರಹೋಗು, "   %3d = ", i);
      ಸ್ಥಿರಾಂಕ state *s = lookback_ಕಂಡುಹಿಡಿ_state (i);
      ಪೂರ್ಣಾಂಕ rnum = i - (s->reductions->lookaheads - LA);
      ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = s->reductions->ನಿಯಮಗಳು[rnum];
      fprintf (ಹೊರಹೋಗು, "(%3d, ", s->ಸಂಖ್ಯೆ);
      ನಿಯಮ_print (r, ಖಾಲಿ, ಹೊರಹೋಗು);
      fputs (") ->", ಹೊರಹೋಗು);
      ಗಾಗಿ (goto_list *sp = lookback[i]; sp; sp = sp->ಮುಂದಿನ)
        {
          fputc (' ', ಹೊರಹೋಗು);
          goto_print (sp->ಮೌಲ್ಯ, ಹೊರಹೋಗು);
        }
      fputc ('\n', ಹೊರಹೋಗು);
    }
  fputc ('\n', ಹೊರಹೋಗು);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_lookback_ಅಂಚು (state *s, ನಿಯಮ ಸ್ಥಿರಾಂಕ *r, goto_ಸಂಖ್ಯೆ gotono)
{
  ಪೂರ್ಣಾಂಕ ri = state_reduction_ಕಂಡುಹಿಡಿ (s, r);
  ಪೂರ್ಣಾಂಕ idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
ಸ್ಥಿರ ಶೂನ್ಯ
build_relations (ಶೂನ್ಯ)
{
  goto_ಸಂಖ್ಯೆ *ಅಂಚು = xnmalloc (ngotos, ಗಾತ್ರ *ಅಂಚು);
  state_ಸಂಖ್ಯೆ *ಮಾರ್ಗ = xnmalloc (ritem_longest_rhs () + 1, ಗಾತ್ರ *ಮಾರ್ಗ);

  includes = xnmalloc (ngotos, ಗಾತ್ರ *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      ಸ್ಥಿರಾಂಕ state_ಸಂಖ್ಯೆ src = from_state[i];
      ಸ್ಥಿರಾಂಕ state_ಸಂಖ್ಯೆ dst = ಗಮ್ಯ_state[i];
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ var = ಸ್ಥಿತಿಗಳು[dst]->accessing_ಚಿಹ್ನೆ;

      /* Size of EDGE.  */
      ಪೂರ್ಣಾಂಕ nedges = 0;
      ಗಾಗಿ (ನಿಯಮ **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          ನಿಯಮ ಸ್ಥಿರಾಂಕ *r = *rulep;
          state *s = ಸ್ಥಿತಿಗಳು[src];
          ಮಾರ್ಗ[0] = s->ಸಂಖ್ಯೆ;

          /* Length of PATH.  */
          ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = 1;
          ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ ಸ್ಥಿರಾಂಕ *rp = r->rhs; 0 <= *rp; rp++)
            {
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rp);
              s = transitions_ಗಮ್ಯ (s, sym);
              ಮಾರ್ಗ[ಉದ್ದಳತೆ++] = s->ಸಂಖ್ಯೆ;
            }

          /* S is the end of PATH.  */
          ಆದರೆ (!s->consistent)
            ಕೂಡಿಸು_lookback_ಅಂಚು (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ p = ಉದ್ದಳತೆ - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (r->rhs[p]);
              goto_ಸಂಖ್ಯೆ g = map_goto (ಮಾರ್ಗ[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool ಸಿಕ್ಕಿತು = false;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; !ಸಿಕ್ಕಿತು && j < nedges; ++j)
                  ಸಿಕ್ಕಿತು = ಅಂಚು[j] == g;
                ಆದರೆ (!ಸಿಕ್ಕಿತು)
                  {
                    ಪ್ರತಿಜ್ಞಾ (nedges < ngotos);
                    ಅಂಚು[nedges++] = g;
                  }
              }
              ಆದರೆ (!nullable[sym - ntokens])
                ನಿಲ್ಲಿಸು;
            }
        }

      ಆದರೆ (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (ಅಂಚು[j], stderr);
            }
          fputc ('\n', stderr);
        }

      ಆದರೆ (nedges == 0)
        includes[i] = ಖಾಲಿ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          includes[i] = xnmalloc (nedges + 1, ಗಾತ್ರ includes[i][0]);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < nedges; ++j)
            includes[i][j] = ಅಂಚು[j];
          includes[i][nedges] = ಕೊನೆ_ಗಂಟು;
        }
    }

  ಬಿಡುಗಡೆ (ಅಂಚು);
  ಬಿಡುಗಡೆ (ಮಾರ್ಗ);

  ಸಂಬಂಧ_transpose (&includes, ngotos);
  ಆದರೆ (trace_flag & trace_automaton)
    ಸಂಬಂಧ_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
ಸ್ಥಿರ ಶೂನ್ಯ
compute_follows (ಶೂನ್ಯ)
{
  ಸಂಬಂಧ_digraph (includes, ngotos, goto_follows);
  ಆದರೆ (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    ಬಿಡುಗಡೆ (includes[i]);
  ಬಿಡುಗಡೆ (includes);
}


ಸ್ಥಿರ ಶೂನ್ಯ
compute_lookaheads (ಶೂನ್ಯ)
{
  ಆದರೆ (trace_flag & trace_automaton)
      lookback_print (stderr);

  ಗಾಗಿ (size_t i = 0; i < nLA; ++i)
    ಗಾಗಿ (goto_list *sp = lookback[i]; sp; sp = sp->ಮುಂದಿನ)
      bitset_or (LA[i], LA[i], goto_follows[sp->ಮೌಲ್ಯ]);

  /* Free LOOKBACK. */
  ಗಾಗಿ (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  ಬಿಡುಗಡೆ (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
state_lookaheads_ಎಣಿಕೆ (state *s, bool default_reduction_only_for_accept)
{
  ಸ್ಥಿರಾಂಕ reductions *reds = s->reductions;
  ಸ್ಥಿರಾಂಕ transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->ಅಂಕೆ == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->ಅಂಕೆ > 1
      || (reds->ಅಂಕೆ == 1 && trans->ಅಂಕೆ && TRANSITION_IS_ಸರಿಸು (trans, 0))
      || (reds->ಅಂಕೆ == 1 && !ನಿಯಮ_is_initial (reds->ನಿಯಮಗಳು[0])
          && default_reduction_only_for_accept));

  ಹಿಂತಿರುಗಿ s->consistent ? 0 : reds->ಅಂಕೆ;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

ಶೂನ್ಯ
initialize_LA (ಶೂನ್ಯ)
{
  bool default_reduction_only_for_accept;
  {
    ಅಕ್ಷರ *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    ಬಿಡುಗಡೆ (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    nLA += state_lookaheads_ಎಣಿಕೆ (ಸ್ಥಿತಿಗಳು[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  ಆದರೆ (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = state_lookaheads_ಎಣಿಕೆ (ಸ್ಥಿತಿಗಳು[i],
                                          default_reduction_only_for_accept);
      ಆದರೆ (ಎಣಿಕೆ_2)
        {
          ಸ್ಥಿತಿಗಳು[i]->reductions->lookaheads = pLA;
          pLA += ಎಣಿಕೆ_2;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
lookaheads_print (FILE *ಹೊರಹೋಗು)
{
  fputs ("Lookaheads:\n", ಹೊರಹೋಗು);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಸ್ಥಿರಾಂಕ reductions *reds = ಸ್ಥಿತಿಗಳು[i]->reductions;
      ಆದರೆ (reds->ಅಂಕೆ)
        {
          fprintf (ಹೊರಹೋಗು, "  State %d:\n", i);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < reds->ಅಂಕೆ; ++j)
            {
              fprintf (ಹೊರಹೋಗು, "    rule %d:", reds->ನಿಯಮಗಳು[j]->ಸಂಖ್ಯೆ);
              ಆದರೆ (reds->lookaheads)
              {
                bitset_iterator iter;
                ಪೂರ್ಣಾಂಕ k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (ಹೊರಹೋಗು, " %s", symbols[k]->ಟ್ಯಾಗ್);
              }
              fputc ('\n', ಹೊರಹೋಗು);
            }
        }
    }
  fputc ('\n', ಹೊರಹೋಗು);
}

ಶೂನ್ಯ
lalr (ಶೂನ್ಯ)
{
  ಆದರೆ (trace_flag & trace_automaton)
    {
      fputc ('\n', stderr);
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: begin");
      ಕೊನೆ_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
  initialize_LA ();
  ಗಣ_goto_map ();
  initialize_goto_follows ();
  lookback = xcalloc (nLA, ಗಾತ್ರ *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  ಆದರೆ (trace_flag & trace_sets)
    lookaheads_print (stderr);
  ಆದರೆ (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      ಕೊನೆ_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


ಶೂನ್ಯ
lalr_update_state_numbers (state_ಸಂಖ್ಯೆ old_ಗಮ್ಯ_new[], state_ಸಂಖ್ಯೆ nstates_old)
{
  goto_ಸಂಖ್ಯೆ ngotos_reachable = 0;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      ಆಗ (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      ಆದರೆ (old_ಗಮ್ಯ_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_ಗಮ್ಯ_new[ಗಮ್ಯ_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_ಗಮ್ಯ_new[from_state[i]];
          ಗಮ್ಯ_state[ngotos_reachable] = old_ಗಮ್ಯ_new[ಗಮ್ಯ_state[i]];
          ++ngotos_reachable;
        }
    }
  ಆಗ (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


ಶೂನ್ಯ
lalr_free (ಶೂನ್ಯ)
{
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; ++s)
    ಸ್ಥಿತಿಗಳು[s]->reductions->lookaheads = ಖಾಲಿ;
  bitsetv_free (LA);
}
