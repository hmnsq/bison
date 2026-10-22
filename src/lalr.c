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

goto_ಸಂಖ್ಯೆ *goto_map = ರಿಕ್ತ;
goto_ಸಂಖ್ಯೆ ngotos = 0;
state_ಸಂಖ್ಯೆ *from_state = ರಿಕ್ತ;
state_ಸಂಖ್ಯೆ *to_state = ರಿಕ್ತ;
bitsetv goto_follows = ರಿಕ್ತ;

/* Linked list of goto numbers.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ goto_list
{
  ಸಂರಚನಾ goto_list *next;
  goto_ಸಂಖ್ಯೆ ಮೌಲ್ಯ;
} goto_list;

ಸ್ಥಿರ goto_list *
goto_list_new (goto_ಸಂಖ್ಯೆ ಮೌಲ್ಯ, ಸಂರಚನಾ goto_list *next)
{
  goto_list *res = xmalloc (ಪರಿಮಾಣ *res);
  res->next = next;
  res->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
  ಪ್ರತ್ಯಾಗಮ res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

ಸ್ಥಿರ bitsetv LA = ರಿಕ್ತ;
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
goto_print (goto_ಸಂಖ್ಯೆ i, FILE *out)
{
  ಧ್ರುವ state_ಸಂಖ್ಯೆ src = from_state[i];
  ಧ್ರುವ state_ಸಂಖ್ಯೆ dst = to_state[i];
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ var = states[dst]->accessing_ಚಿಹ್ನೆ;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

ಶೂನ್ಯ
set_goto_map (ಶೂನ್ಯ)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  ಯದಿ (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, ಪರಿಮಾಣ *goto_map);
  ngotos = 0;
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = trans->ಅಂಕೆ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_ಸಂಖ್ಯೆ_MAXIMUM);
          goto_map[TRANSITION_ಚಿಹ್ನೆ (trans, i) - ntokens]++;
        }
    }

  goto_ಸಂಖ್ಯೆ *temp_map = xnmalloc (nnterms + 1, ಪರಿಮಾಣ *temp_map);
  {
    goto_ಸಂಖ್ಯೆ k = 0;
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, ಪರಿಮಾಣ *from_state);
  to_state = xcalloc (ngotos, ಪರಿಮಾಣ *to_state);

  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; ++s)
    {
      ಧ್ರುವ transitions *trans = states[s]->transitions;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = trans->ಅಂಕೆ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_ಸಂಖ್ಯೆ k = temp_map[TRANSITION_ಚಿಹ್ನೆ (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->ಸಂಖ್ಯೆ;
        }
    }

  ಮುಕ್ತ (temp_map);

  ಯದಿ (trace_flag & trace_automaton)
    {
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ngotos; ++i)
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

  ಪ್ರತಿ (;;)
    {
      aver (ಕೆಳಗಿನ <= ಮೇಲಿನ);
      goto_ಸಂಖ್ಯೆ ಮಧ್ಯ = (ಕೆಳಗಿನ + ಮೇಲಿನ) / 2;
      state_ಸಂಖ್ಯೆ s = from_state[ಮಧ್ಯ];
      ಯದಿ (s == src)
        ಪ್ರತ್ಯಾಗಮ ಮಧ್ಯ;
      ಅನ್ಯಥಾ ಯದಿ (s < src)
        ಕೆಳಗಿನ = ಮಧ್ಯ + 1;
      ಅನ್ಯಥಾ
        ಮೇಲಿನ = ಮಧ್ಯ - 1;
    }
}

/* Print FOLLOWS for debugging.  */
ಸ್ಥಿರ ಶೂನ್ಯ
follows_print (ಧ್ರುವ ವರ್ಣ* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  ಪ್ರತಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
ಸ್ಥಿರ ಶೂನ್ಯ
initialize_goto_follows (ಶೂನ್ಯ)
{
  goto_ಸಂಖ್ಯೆ **reads = xnmalloc (ngotos, ಪರಿಮಾಣ *reads);
  goto_ಸಂಖ್ಯೆ *edge = xnmalloc (ngotos, ಪರಿಮಾಣ *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  ಪ್ರತಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      state_ಸಂಖ್ಯೆ dst = to_state[i];
      ಧ್ರುವ transitions *trans = states[dst]->transitions;

      ಪೂರ್ಣಾಙ್ಕ j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_ಚಿಹ್ನೆ (trans, j));

      /* Gotos outgoing from DST. */
      goto_ಸಂಖ್ಯೆ nedges = 0;
      ಪ್ರತಿ (; j < trans->ಅಂಕೆ; ++j)
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = TRANSITION_ಚಿಹ್ನೆ (trans, j);
          ಯದಿ (nullable[sym - ntokens])
            {
              ಪ್ರತಿಜ್ಞಾ (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      ಯದಿ (nedges == 0)
        reads[i] = ರಿಕ್ತ;
      ಅನ್ಯಥಾ
        {
          reads[i] = xnmalloc (nedges + 1, ಪರಿಮಾಣ reads[i][0]);
          memcpy (reads[i], edge, nedges * ಪರಿಮಾಣ edge[0]);
          reads[i][nedges] = END_ಗಂಟು;
        }
    }
  ಯದಿ (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  ಯದಿ (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  ಪ್ರತಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    ಮುಕ್ತ (reads[i]);
  ಮುಕ್ತ (reads);
  ಮುಕ್ತ (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
ಸ್ಥಿರ ಧ್ರುವ state *
lookback_find_state (ಪೂರ್ಣಾಙ್ಕ lookback_index)
{
  state *res = ರಿಕ್ತ;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nstates; ++j)
    ಯದಿ (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        ಯದಿ (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          ಭಙ್ಗ;
        ಅನ್ಯಥಾ
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  ಯದಿ (!res)
    abort ();
  ಪ್ರತ್ಯಾಗಮ res;
}

/* Print LOOKBACK for debugging.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nLA; ++i)
    ಯದಿ (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      ಧ್ರುವ state *s = lookback_find_state (i);
      ಪೂರ್ಣಾಙ್ಕ rnum = i - (s->reductions->lookaheads - LA);
      ಧ್ರುವ rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->ಸಂಖ್ಯೆ);
      rule_print (r, ರಿಕ್ತ, out);
      fputs (") ->", out);
      ಪ್ರತಿ (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->ಮೌಲ್ಯ, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
ಸ್ಥಿರ ಶೂನ್ಯ
add_lookback_edge (state *s, rule ಧ್ರುವ *r, goto_ಸಂಖ್ಯೆ gotono)
{
  ಪೂರ್ಣಾಙ್ಕ ri = state_reduction_find (s, r);
  ಪೂರ್ಣಾಙ್ಕ idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
ಸ್ಥಿರ ಶೂನ್ಯ
build_relations (ಶೂನ್ಯ)
{
  goto_ಸಂಖ್ಯೆ *edge = xnmalloc (ngotos, ಪರಿಮಾಣ *edge);
  state_ಸಂಖ್ಯೆ *path = xnmalloc (ritem_longest_rhs () + 1, ಪರಿಮಾಣ *path);

  includes = xnmalloc (ngotos, ಪರಿಮಾಣ *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  ಪ್ರತಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      ಧ್ರುವ state_ಸಂಖ್ಯೆ src = from_state[i];
      ಧ್ರುವ state_ಸಂಖ್ಯೆ dst = to_state[i];
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ var = states[dst]->accessing_ಚಿಹ್ನೆ;

      /* Size of EDGE.  */
      ಪೂರ್ಣಾಙ್ಕ nedges = 0;
      ಪ್ರತಿ (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule ಧ್ರುವ *r = *rulep;
          state *s = states[src];
          path[0] = s->ಸಂಖ್ಯೆ;

          /* Length of PATH.  */
          ಪೂರ್ಣಾಙ್ಕ length = 1;
          ಪ್ರತಿ (item_ಸಂಖ್ಯೆ ಧ್ರುವ *rp = r->rhs; 0 <= *rp; rp++)
            {
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->ಸಂಖ್ಯೆ;
            }

          /* S is the end of PATH.  */
          ಯದಿ (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (r->rhs[p]);
              goto_ಸಂಖ್ಯೆ g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                ಯದಿ (!found)
                  {
                    ಪ್ರತಿಜ್ಞಾ (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              ಯದಿ (!nullable[sym - ntokens])
                ಭಙ್ಗ;
            }
        }

      ಯದಿ (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      ಯದಿ (nedges == 0)
        includes[i] = ರಿಕ್ತ;
      ಅನ್ಯಥಾ
        {
          includes[i] = xnmalloc (nedges + 1, ಪರಿಮಾಣ includes[i][0]);
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_ಗಂಟು;
        }
    }

  ಮುಕ್ತ (edge);
  ಮುಕ್ತ (path);

  relation_transpose (&includes, ngotos);
  ಯದಿ (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
ಸ್ಥಿರ ಶೂನ್ಯ
compute_follows (ಶೂನ್ಯ)
{
  relation_digraph (includes, ngotos, goto_follows);
  ಯದಿ (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  ಪ್ರತಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    ಮುಕ್ತ (includes[i]);
  ಮುಕ್ತ (includes);
}


ಸ್ಥಿರ ಶೂನ್ಯ
compute_lookaheads (ಶೂನ್ಯ)
{
  ಯದಿ (trace_flag & trace_automaton)
      lookback_print (stderr);

  ಪ್ರತಿ (size_t i = 0; i < nLA; ++i)
    ಪ್ರತಿ (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->ಮೌಲ್ಯ]);

  /* Free LOOKBACK. */
  ಪ್ರತಿ (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  ಮುಕ್ತ (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
state_lookaheads_ಎಣಿಕೆ (state *s, bool default_reduction_only_for_accept)
{
  ಧ್ರುವ reductions *reds = s->reductions;
  ಧ್ರುವ transitions *trans = s->transitions;

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
      || (reds->ಅಂಕೆ == 1 && trans->ಅಂಕೆ && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->ಅಂಕೆ == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  ಪ್ರತ್ಯಾಗಮ s->consistent ? 0 : reds->ಅಂಕೆ;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

ಶೂನ್ಯ
initialize_LA (ಶೂನ್ಯ)
{
  bool default_reduction_only_for_accept;
  {
    ವರ್ಣ *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    ಮುಕ್ತ (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    nLA += state_lookaheads_ಎಣಿಕೆ (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  ಯದಿ (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = state_lookaheads_ಎಣಿಕೆ (states[i],
                                          default_reduction_only_for_accept);
      ಯದಿ (ಎಣಿಕೆ)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += ಎಣಿಕೆ;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಧ್ರುವ reductions *reds = states[i]->reductions;
      ಯದಿ (reds->ಅಂಕೆ)
        {
          fprintf (out, "  State %d:\n", i);
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < reds->ಅಂಕೆ; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->ಸಂಖ್ಯೆ);
              ಯದಿ (reds->lookaheads)
              {
                bitset_iterator iter;
                ಪೂರ್ಣಾಙ್ಕ k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

ಶೂನ್ಯ
lalr (ಶೂನ್ಯ)
{
  ಯದಿ (trace_flag & trace_automaton)
    {
      fputc ('\n', stderr);
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: begin");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
  initialize_LA ();
  set_goto_map ();
  initialize_goto_follows ();
  lookback = xcalloc (nLA, ಪರಿಮಾಣ *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  ಯದಿ (trace_flag & trace_sets)
    lookaheads_print (stderr);
  ಯದಿ (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


ಶೂನ್ಯ
lalr_update_state_numbers (state_ಸಂಖ್ಯೆ old_to_new[], state_ಸಂಖ್ಯೆ nstates_old)
{
  goto_ಸಂಖ್ಯೆ ngotos_reachable = 0;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  ಪ್ರತಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      ಯಾವತ್ (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      ಯದಿ (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  ಯಾವತ್ (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


ಶೂನ್ಯ
lalr_free (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (state_ಸಂಖ್ಯೆ s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = ರಿಕ್ತ;
  bitsetv_free (LA);
}
