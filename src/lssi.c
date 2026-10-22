/* Lookahead sensitive state item searches for counterexample generation

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "lssi.h"

#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <gl_xlist.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"

// Lookahead sensitive state item.
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ lssi
{
  state_ಅಂಶ_ಸಂಖ್ಯೆ si;
  ರಚನೆ lssi *ಪೋಷಕ;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ಸ್ಥಿರ lssi *
new_lssi (state_ಅಂಶ_ಸಂಖ್ಯೆ si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (ಗಾತ್ರ *res);
  res->si = si;
  res->ಪೋಷಕ = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
lssi_free (lssi *sn)
{
  ಆದರೆ (sn == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (sn->free_lookahead)
    bitset_free (sn->lookahead);
  ಬಿಡುಗಡೆ (sn);
}

ಸ್ಥಿರ size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  ಹಿಂತಿರುಗಿ hash % max;
}

ಸ್ಥಿರ bool
lssi_comparator (lssi *s1, lssi *s2)
{
  ಆದರೆ (s1->si == s2->si)
    {
      ಆದರೆ (s1->lookahead == s2->lookahead)
        ಹಿಂತಿರುಗಿ true;
      ಹಿಂತಿರುಗಿ bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  ಹಿಂತಿರುಗಿ false;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t lssi_list;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
append_lssi (lssi *sn, Hash_ಕೋಷ್ಟಕ *ಭೇಟಿಯಾದ, lssi_list ಸರತಿ)
{
  ಆದರೆ (hash_lookup (ಭೇಟಿಯಾದ, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      ಹಿಂತಿರುಗಿ false;
    }
  hash_xinsert (ಭೇಟಿಯಾದ, sn);
  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, sn);
  ಹಿಂತಿರುಗಿ true;
}

#ಆದರೆ 0
ಸ್ಥಿರ ಶೂನ್ಯ
lssi_print (lssi *l)
{
  FILE *ಹೊರಹೋಗು = stderr;
  print_state_ಅಂಶ (&state_ವಸ್ತುಗಳು[l->si], ಹೊರಹೋಗು);
  ಆದರೆ (l->lookahead)
    {
      fprintf (ಹೊರಹೋಗು, "FOLLOWL = { ");
      bitset_iterator biter;
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
      BITSET_FOR_EACH (biter, l->lookahead, ಜ್ಯಾ, 0)
        fprintf (ಹೊರಹೋಗು, "%s, \n", symbols[ಜ್ಯಾ]->ಟ್ಯಾಗ್);
      fprintf (ಹೊರಹೋಗು, "}\n");
    }
}
#ಆದರೆ_ಅಂತ್ಯ

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
ಸ್ಥಿರ bitset
eligible_state_ವಸ್ತುಗಳು (state_ಅಂಶ *ಗುರಿ)
{
  bitset ಫಲಿತಾಂಶ = bitset_create (nstate_ವಸ್ತುಗಳು, BITSET_FIXED);
  state_ಅಂಶ_list ಸರತಿ =
    gl_list_create (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true, 1,
                    (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ **) &ಗುರಿ);
  ಆಗ (gl_list_ಗಾತ್ರ (ಸರತಿ) > 0)
    {
      state_ಅಂಶ *si = (state_ಅಂಶ *) gl_list_get_at (ಸರತಿ, 0);
      gl_list_remove_at (ಸರತಿ, 0);
      ಆದರೆ (bitset_test (ಫಲಿತಾಂಶ, si - state_ವಸ್ತುಗಳು))
        ಮುಂದುವರಿ;
      bitset_ಗಣ (ಫಲಿತಾಂಶ, si - state_ವಸ್ತುಗಳು);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
      BITSET_FOR_EACH (biter, rsi, ಜ್ಯಾ, 0)
        gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಸರತಿ, &state_ವಸ್ತುಗಳು[ಜ್ಯಾ]);
    }
  gl_list_free (ಸರತಿ);
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_ಅಂಶ_list
ಅತಿಹ್ರಸ್ವ_ಮಾರ್ಗ_from_ಆರಂಭ (state_ಅಂಶ_ಸಂಖ್ಯೆ ಗುರಿ, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym)
{
  bitset eligible = eligible_state_ವಸ್ತುಗಳು (&state_ವಸ್ತುಗಳು[ಗುರಿ]);
  Hash_ಕೋಷ್ಟಕ *ಭೇಟಿಯಾದ = hash_initialize (32,
                                         ಖಾಲಿ,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_ದತ್ತಾಂಶ_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_ಗಣ (il, 0);
  lssi *init = new_lssi (0, ಖಾಲಿ, il, true);
  lssi_list ಸರತಿ = gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ,
                                          ಖಾಲಿ, true);
  append_lssi (init, ಭೇಟಿಯಾದ, ಸರತಿ);
  // breadth-first search
  bool ಪೂರ್ಣಗೊಂಡ = false;
  lssi *n;
  ಆಗ (gl_list_ಗಾತ್ರ (ಸರತಿ) > 0)
    {
      n = (lssi *) gl_list_get_at (ಸರತಿ, 0);
      gl_list_remove_at (ಸರತಿ, 0);
      state_ಅಂಶ_ಸಂಖ್ಯೆ ಕೊನೆಯ = n->si;
      ಆದರೆ (ಗುರಿ == ಕೊನೆಯ && bitset_test (n->lookahead, ಮುಂದಿನ_sym))
        {
          ಪೂರ್ಣಗೊಂಡ = true;
          ನಿಲ್ಲಿಸು;
        }
      state_ಅಂಶ *si = &state_ವಸ್ತುಗಳು[ಕೊನೆಯ];
      // Transitions don't change follow_L
      ಆದರೆ (si->trans >= 0)
        {
          ಆದರೆ (bitset_test (eligible, si->trans))
            {
              lssi *ಮುಂದಿನ = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (ಮುಂದಿನ, ಭೇಟಿಯಾದ, ಸರತಿ);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // nonterminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      ಆದರೆ (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          ಅಂಶ_ಸಂಖ್ಯೆ *pos = si->ಅಂಶ + 1;
          ಗಾಗಿ (; !ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*pos); ++pos)
            {
              ಅಂಶ_ಸಂಖ್ಯೆ it = *pos;
              ಆದರೆ (ISTOKEN (it))
                {
                  bitset_ಗಣ (lookahead, it);
                  ನಿಲ್ಲಿಸು;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  ಆದರೆ (!nullable[it - ntokens])
                    ನಿಲ್ಲಿಸು;
                }
            }
          ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_ಬಳಸಿದ = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_ಅಂಶ_ಸಂಖ್ಯೆ ಮುಂದಿನ_SI;
          BITSET_FOR_EACH (biter, si->prods, ಮುಂದಿನ_SI, 0)
            {
              ಆದರೆ (!bitset_test (eligible, ಮುಂದಿನ_SI))
                ಮುಂದುವರಿ;
              lssi *ಮುಂದಿನ = new_lssi (ಮುಂದಿನ_SI, n, lookahead,
                                     !lookahead_ಬಳಸಿದ);
              lookahead_ಬಳಸಿದ = append_lssi (ಮುಂದಿನ, ಭೇಟಿಯಾದ, ಸರತಿ)
                               || lookahead_ಬಳಸಿದ;
            }
          ಆದರೆ (!lookahead_ಬಳಸಿದ)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  ಆದರೆ (!ಪೂರ್ಣಗೊಂಡ)
    {
      gl_list_free (ಸರತಿ);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_ಅಂಶ_list res =
    gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
  ಗಾಗಿ (lssi *sn = n; sn != ಖಾಲಿ; sn = sn->ಪೋಷಕ)
    gl_list_ಕೂಡಿಸು_ಮೊದಲ (res, &state_ವಸ್ತುಗಳು[sn->si]);

  hash_free (ಭೇಟಿಯಾದ);
  gl_list_free (ಸರತಿ);

  ಆದರೆ (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *sip;
      ಆಗ (gl_list_iterator_ಮುಂದಿನ (&it, &sip, ಖಾಲಿ))
        state_ಅಂಶ_print ((state_ಅಂಶ *) sip, stderr, "");
    }
  ಹಿಂತಿರುಗಿ res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_ಚಿಹ್ನೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, bitset syms)
{
  ಆದರೆ (!syms)
    ಹಿಂತಿರುಗಿ true;
  bitset_iterator biter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      ಆದರೆ (sym == sn)
        ಹಿಂತಿರುಗಿ true;
      ಆದರೆ (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  ಆದರೆ (!syms || !ts)
    ಹಿಂತಿರುಗಿ true;
  bitset_iterator biter;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      ಆದರೆ (bitset_test (ts, sn))
        ಹಿಂತಿರುಗಿ true;
      ಆದರೆ (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_ಅಂಶ_list
lssi_ಹಿಮ್ಮುಖ_production (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si, bitset lookahead)
{
  state_ಅಂಶ_list ಫಲಿತಾಂಶ =
    gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
  ಆದರೆ (SI_TRANSITION (si))
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
  BITSET_FOR_EACH (biter, si->revs, ಜ್ಯಾ, 0)
  {
    state_ಅಂಶ *prevsi = &state_ವಸ್ತುಗಳು[ಜ್ಯಾ];
    ಆದರೆ (!production_allowed (prevsi, si))
      ಮುಂದುವರಿ;
    bitset prev_lookahead = prevsi->lookahead;
    ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*(prevsi->ಅಂಶ)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        ಆದರೆ (!intersect (prev_lookahead, lookahead))
          ಮುಂದುವರಿ;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        // shift item
        ಆದರೆ (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            ಗಾಗಿ (ಅಂಶ_ಸಂಖ್ಯೆ *pos = prevsi->ಅಂಶ + 1;
                 !applicable && nlable && ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*pos);
                 ++pos)
              {
                ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ ಮುಂದಿನ_sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*pos);
                ಆದರೆ (ISTOKEN (ಮುಂದಿನ_sym))
                  {
                    applicable = intersect_ಚಿಹ್ನೆ (ಮುಂದಿನ_sym, lookahead);
                    nlable = false;
                  }
                ಇಲ್ಲದಿದ್ದರೆ
                  {
                    applicable = intersect (FIRSTS (ಮುಂದಿನ_sym), lookahead);
                    ಆದರೆ (!applicable)
                      nlable = nullable[ಮುಂದಿನ_sym - ntokens];
                  }
              }
            ಆದರೆ (!applicable && !nlable)
              ಮುಂದುವರಿ;
          }
      }
    gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಫಲಿತಾಂಶ, prevsi);
  }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}
