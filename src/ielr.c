/* IELR main implementation.

   Copyright (C) 2009-2015, 2018-2022 Free Software Foundation, Inc.

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

#ಅನ್ತರ್ಭಾವ "ielr.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>
#ಅನ್ತರ್ಭಾವ <timevar.h>

#ಅನ್ತರ್ಭಾವ "AnnotationList.h"
#ಅನ್ತರ್ಭಾವ "derives.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"
#ಅನ್ತರ್ಭಾವ "muscle-tab.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"
#ಅನ್ತರ್ಭಾವ "relation.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/** Records the value of the \%define variable lr.type.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
  {
   LR_TYPE__LR0,
   LR_TYPE__LALR,
   LR_TYPE__IELR,
   LR_TYPE__CANONICAL_LR
  } LrType;

/* The user's requested LR type.  */
ಸ್ಥಿರ LrType
lr_type_get (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ *type = muscle_percent_define_get ("lr.type");
  LrType res;
  ಆದರೆ (STREQ (type, "lr""(0)"))
    res = LR_TYPE__LR0;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (type, "lalr"))
    res = LR_TYPE__LALR;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (type, "ielr"))
    res = LR_TYPE__IELR;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STREQ (type, "canonical-lr"))
    res = LR_TYPE__CANONICAL_LR;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      aver (false);
      abort ();
    }
  ಬಿಡುಗಡೆ (type);
  ಹಿಂತಿರುಗಿ res;
}

/**
 * \post:
 *   - \c result = a new \c bitset of size \c ::nritems such that any bit \c i
 *     is set iff <tt>ritem[i]</tt> is a nonterminal after which all ritems in
 *     the same RHS are nullable nonterminals.  In other words, the follows of
 *     a goto on <tt>ritem[i]</tt> include the lookahead set of the item.
 */
ಸ್ಥಿರ bitset
ielr_compute_ritem_sees_lookahead_ಗಣ (ಶೂನ್ಯ)
{
  bitset ಫಲಿತಾಂಶ = bitset_create (nritems, BITSET_FIXED);
  ಪೂರ್ಣಾಂಕ i = nritems-1;
  ಆಗ (0 < i)
    {
      --i;
      // Walk the RHS right to left, as long as it's symbol,
      // nonterminal, nullable.
      ಆಗ (!ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[i])
             && ISVAR (ritem[i])
             && nullable [ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[i]) - ntokens])
        bitset_ಗಣ (ಫಲಿತಾಂಶ, i--);
      ಆದರೆ (!ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[i]) && ISVAR (ritem[i]))
        bitset_ಗಣ (ಫಲಿತಾಂಶ, i--);
      // Flush the remainder of the RHS.
      ಆಗ (!ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[i]) && 0 < i)
        --i;
    }
  ಆದರೆ (trace_flag & trace_ielr)
    {
      fprintf (stderr, "ritem_sees_lookahead_set (indexes of ritems): ");
      bitset_dump (stderr, ಫಲಿತಾಂಶ);
      fprintf (stderr, "\n");
    }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/**
 * \pre:
 *   - \c ritem_sees_lookahead_set was computed by
 *     \c ielr_compute_ritem_sees_lookahead_set.
 * \post:
 *   - Each of \c *edgesp and \c *edge_countsp is a new array of size
 *     \c ::ngotos.
 *   - <tt>(*edgesp)[i]</tt> points to a \c goto_number array of size
 *     <tt>(*edge_countsp)[i]+1</tt>.
 *   - In such a \c goto_number array, the last element is \c ::END_NODE.
 *   - All remaining elements are the indices of the gotos to which there is an
 *     internal follow edge from goto \c i.
 *   - There is an internal follow edge from goto \c i to goto \c j iff both:
 *     - The from states of gotos \c i and \c j are the same.
 *     - The transition nonterminal for goto \c i appears as the first RHS
 *       symbol of at least one production for which both:
 *       - The LHS is the transition symbol of goto \c j.
 *       - All other RHS symbols are nullable nonterminals.
 *     - In other words, the follows of goto \c i include the follows of
 *       goto \c j and it's an internal edge because the from states are the
 *       same.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_ಆಂತರಿಕ_follow_ಅಂಚುಗಳು (bitset ritem_sees_lookahead_ಗಣ,
                                    goto_ಸಂಖ್ಯೆ ***edgesp, ಪೂರ್ಣಾಂಕ **ಅಂಚು_countsp)
{
  *edgesp = xnmalloc (ngotos, ಗಾತ್ರ **edgesp);
  *ಅಂಚು_countsp = xnmalloc (ngotos, ಗಾತ್ರ **ಅಂಚು_countsp);
  {
    bitset sources = bitset_create (ngotos, BITSET_FIXED);
    ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
      (*ಅಂಚು_countsp)[i] = 0;
    ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
      {
        ಪೂರ್ಣಾಂಕ nsources = 0;
        {
          ಗಾಗಿ (ನಿಯಮ **rulep = derives[ಸ್ಥಿತಿಗಳು[ಗಮ್ಯ_state[i]]->accessing_ಚಿಹ್ನೆ
                                      - ntokens];
               *rulep;
               ++rulep)
            {
              /* If there is at least one RHS symbol, if the first RHS symbol
                 is a nonterminal, and if all remaining RHS symbols (if any)
                 are nullable nonterminals, create an edge from the LHS
                 symbol's goto to the first RHS symbol's goto such that the RHS
                 symbol's goto will be the source of the edge after the
                 eventual relation_transpose below.

                 Unlike in ielr_compute_always_follows, I use a bitset for
                 edges rather than an array because it is possible that
                 multiple RHS's with the same first symbol could fit and thus
                 that we could end up with redundant edges.  With the
                 possibility of redundant edges, it's hard to know ahead of
                 time how large to make such an array.  Another possible
                 redundancy is that source and destination might be the same
                 goto.  Eliminating all these possible redundancies now might
                 possibly help performance a little.  I have not proven any of
                 this, but I'm guessing the bitset shouldn't entail much of a
                 performance penalty, if any.  */
              ಆದರೆ (bitset_test (ritem_sees_lookahead_ಗಣ,
                               (*rulep)->rhs - ritem))
                {
                  goto_ಸಂಖ್ಯೆ ಮೂಲ =
                    map_goto (from_state[i],
                              ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(*rulep)->rhs));
                  ಆದರೆ (i != ಮೂಲ && !bitset_test (sources, ಮೂಲ))
                    {
                      bitset_ಗಣ (sources, ಮೂಲ);
                      ++nsources;
                      ++(*ಅಂಚು_countsp)[ಮೂಲ];
                    }
                }
            }
        }
        ಆದರೆ (nsources == 0)
          (*edgesp)[i] = ಖಾಲಿ;
        ಇಲ್ಲದಿದ್ದರೆ
          {
            (*edgesp)[i] = xnmalloc (nsources + 1, ಗಾತ್ರ *(*edgesp)[i]);
            {
              bitset_iterator biter_ಮೂಲ;
              bitset_bindex ಮೂಲ;
              ಪೂರ್ಣಾಂಕ j = 0;
              BITSET_FOR_EACH (biter_ಮೂಲ, sources, ಮೂಲ, 0)
                (*edgesp)[i][j++] = ಮೂಲ;
            }
            (*edgesp)[i][nsources] = ಕೊನೆ_ಗಂಟು;
          }
        bitset_zero (sources);
      }
    bitset_free (sources);
  }

  ಸಂಬಂಧ_transpose (edgesp, ngotos);

  ಆದರೆ (trace_flag & trace_ielr)
    ಸಂಬಂಧ_print ("internal_follow_edges", *edgesp, ngotos, ಖಾಲಿ, stderr);
}

/**
 * \pre:
 *   - \c ritem_sees_lookahead_set was computed by
 *     \c ielr_compute_ritem_sees_lookahead_set.
 *   - \c internal_follow_edges was computed by
 *     \c ielr_compute_internal_follow_edges.
 * \post:
 *   - \c *follow_kernel_itemsp is a new \c bitsetv in which the number of rows
 *     is \c ngotos and the number of columns is maximum number of kernel items
 *     in any state.
 *   - <tt>(*follow_kernel_itemsp)[i][j]</tt> is set iff the follows of goto
 *     \c i include the lookahead set of item \c j in the from state of goto
 *     \c i.
 *   - Thus, <tt>(*follow_kernel_itemsp)[i][j]</tt> is always unset if there is
 *     no item \c j in the from state of goto \c i.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_follow_kernel_ವಸ್ತುಗಳು (bitset ritem_sees_lookahead_ಗಣ,
                                  goto_ಸಂಖ್ಯೆ **ಆಂತರಿಕ_follow_ಅಂಚುಗಳು,
                                  bitsetv *follow_kernel_itemsp)
{
  {
    size_t max_nitems = 0;
    ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
      ಆದರೆ (ಸ್ಥಿತಿಗಳು[i]->nitems > max_nitems)
        max_nitems = ಸ್ಥಿತಿಗಳು[i]->nitems;
    *follow_kernel_itemsp = bitsetv_create (ngotos, max_nitems, BITSET_FIXED);
  }
  ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
    {
      size_t nitems = ಸ್ಥಿತಿಗಳು[from_state[i]]->nitems;
      ಅಂಶ_ಸೂಚ್ಯಂಕ *ವಸ್ತುಗಳು = ಸ್ಥಿತಿಗಳು[from_state[i]]->ವಸ್ತುಗಳು;
      ಗಾಗಿ (size_t j = 0; j < nitems; ++j)
        /* If this item has this goto and if all subsequent symbols in this
           RHS (if any) are nullable nonterminals, then record this item as
           one whose lookahead set is included in this goto's follows.  */
        ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[ವಸ್ತುಗಳು[j]])
            && ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (ritem[ವಸ್ತುಗಳು[j]])
            == ಸ್ಥಿತಿಗಳು[ಗಮ್ಯ_state[i]]->accessing_ಚಿಹ್ನೆ
            && bitset_test (ritem_sees_lookahead_ಗಣ, ವಸ್ತುಗಳು[j]))
          bitset_ಗಣ ((*follow_kernel_itemsp)[i], j);
    }
  ಸಂಬಂಧ_digraph (ಆಂತರಿಕ_follow_ಅಂಚುಗಳು, ngotos, *follow_kernel_itemsp);

  ಆದರೆ (trace_flag & trace_ielr)
    {
      fprintf (stderr, "follow_kernel_items:\n");
      debug_bitsetv (*follow_kernel_itemsp);
    }
}

/**
 * \pre
 *   - \c *edgesp and \c edge_counts were computed by
 *     \c ielr_compute_internal_follow_edges.
 * \post
 *   - \c *always_followsp is a new \c bitsetv with \c ngotos rows and
 *     \c ntokens columns.
 *   - <tt>(*always_followsp)[i][j]</tt> is set iff token \c j is an always
 *     follow (that is, it's computed by internal and successor edges) of goto
 *     \c i.
 *   - Rows of \c *edgesp have been realloc'ed and extended to include
 *     successor follow edges.  \c edge_counts has not been updated.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_always_follows (goto_ಸಂಖ್ಯೆ ***edgesp,
                             ಪೂರ್ಣಾಂಕ ಸ್ಥಿರಾಂಕ ಅಂಚು_counts[],
                             bitsetv *always_followsp)
{
  *always_followsp = bitsetv_create (ngotos, ntokens, BITSET_FIXED);
  {
    goto_ಸಂಖ್ಯೆ *ಅಂಚು_array = xnmalloc (ngotos, ಗಾತ್ರ *ಅಂಚು_array);
    ಗಾಗಿ (goto_ಸಂಖ್ಯೆ i = 0; i < ngotos; ++i)
      {
        goto_ಸಂಖ್ಯೆ nedges = ಅಂಚು_counts[i];
        {
          ಪೂರ್ಣಾಂಕ j;
          transitions *trans = ಸ್ಥಿತಿಗಳು[ಗಮ್ಯ_state[i]]->transitions;
          FOR_EACH_ಸರಿಸು (trans, j)
            bitset_ಗಣ ((*always_followsp)[i], TRANSITION_ಚಿಹ್ನೆ (trans, j));
          ಗಾಗಿ (; j < trans->ಅಂಕೆ; ++j)
            {
              ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = TRANSITION_ಚಿಹ್ನೆ (trans, j);
              ಆದರೆ (nullable[sym - ntokens])
                ಅಂಚು_array[nedges++] = map_goto (ಗಮ್ಯ_state[i], sym);
            }
        }
        ಆದರೆ (nedges - ಅಂಚು_counts[i])
          {
            (*edgesp)[i] =
              xnrealloc ((*edgesp)[i], nedges + 1, ಗಾತ್ರ *(*edgesp)[i]);
            ಸ್ಮರಣೆ_ನಕಲು ((*edgesp)[i] + ಅಂಚು_counts[i], ಅಂಚು_array + ಅಂಚು_counts[i],
                    (nedges - ಅಂಚು_counts[i]) * ಗಾತ್ರ *(*edgesp)[i]);
            (*edgesp)[i][nedges] = ಕೊನೆ_ಗಂಟು;
          }
      }
    ಬಿಡುಗಡೆ (ಅಂಚು_array);
  }
  ಸಂಬಂಧ_digraph (*edgesp, ngotos, *always_followsp);

  ಆದರೆ (trace_flag & trace_ielr)
    {
      ಸಂಬಂಧ_print ("always follow edges", *edgesp, ngotos, ಖಾಲಿ, stderr);
      fprintf (stderr, "always_follows:\n");
      debug_bitsetv (*always_followsp);
    }
}

/**
 * \post
 *   - \c result is a new array of size \c ::nstates.
 *   - <tt>result[i]</tt> is an array of pointers to the predecessor
 *     <tt>state</tt>'s of state \c i.
 *   - The last element of such an array is \c NULL.
 */
ಸ್ಥಿರ state ***
ielr_compute_predecessors (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ *predecessor_counts = xnmalloc (nstates, ಗಾತ್ರ *predecessor_counts);
  state ***res = xnmalloc (nstates, ಗಾತ್ರ *res);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    predecessor_counts[i] = 0;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಸ್ಥಿತಿಗಳು[i]->transitions->ಅಂಕೆ; ++j)
      ++predecessor_counts[ಸ್ಥಿತಿಗಳು[i]->transitions->ಸ್ಥಿತಿಗಳು[j]->ಸಂಖ್ಯೆ];
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      res[i] = xnmalloc (predecessor_counts[i]+1, ಗಾತ್ರ *res[i]);
      res[i][predecessor_counts[i]] = ಖಾಲಿ;
      predecessor_counts[i] = 0;
    }
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಸ್ಥಿತಿಗಳು[i]->transitions->ಅಂಕೆ; ++j)
      {
        state_ಸಂಖ್ಯೆ k = ಸ್ಥಿತಿಗಳು[i]->transitions->ಸ್ಥಿತಿಗಳು[j]->ಸಂಖ್ಯೆ;
        res[k][predecessor_counts[k]++] = ಸ್ಥಿತಿಗಳು[i];
      }
  ಬಿಡುಗಡೆ (predecessor_counts);
  ಹಿಂತಿರುಗಿ res;
}

/**
 * \post
 *   - \c *follow_kernel_itemsp and \c *always_followsp were computed by
 *     \c ielr_compute_follow_kernel_items and
 *     \c ielr_compute_always_follows.
 *   - Iff <tt>predecessorsp != NULL</tt>, then \c *predecessorsp was computed
 *     by \c ielr_compute_predecessors.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_auxiliary_tables (bitsetv *follow_kernel_itemsp,
                               bitsetv *always_followsp,
                               state ****predecessorsp)
{
  goto_ಸಂಖ್ಯೆ **ಅಂಚುಗಳು;
  ಪೂರ್ಣಾಂಕ *ಅಂಚು_counts;
  {
    bitset ritem_sees_lookahead_ಗಣ = ielr_compute_ritem_sees_lookahead_ಗಣ ();
    ielr_compute_ಆಂತರಿಕ_follow_ಅಂಚುಗಳು (ritem_sees_lookahead_ಗಣ,
                                        &ಅಂಚುಗಳು, &ಅಂಚು_counts);
    ielr_compute_follow_kernel_ವಸ್ತುಗಳು (ritem_sees_lookahead_ಗಣ, ಅಂಚುಗಳು,
                                      follow_kernel_itemsp);
    bitset_free (ritem_sees_lookahead_ಗಣ);
  }
  ielr_compute_always_follows (&ಅಂಚುಗಳು, ಅಂಚು_counts, always_followsp);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ngotos; ++i)
    ಬಿಡುಗಡೆ (ಅಂಚುಗಳು[i]);
  ಬಿಡುಗಡೆ (ಅಂಚುಗಳು);
  ಬಿಡುಗಡೆ (ಅಂಚು_counts);
  ಆದರೆ (predecessorsp)
    *predecessorsp = ielr_compute_predecessors ();
}

/**
 * \note
 *   - FIXME: It might be an interesting experiment to compare the space and
 *     time efficiency of computing \c item_lookahead_sets either:
 *     - Fully up front.
 *     - Just-in-time, as implemented below.
 *     - Not at all.  That is, just let annotations continue even when
 *       unnecessary.
 */
bool
ielr_ಅಂಶ_has_lookahead (state *s, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs, size_t ಅಂಶ,
                         ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lookahead, state ***predecessors,
                         bitset **ಅಂಶ_lookahead_sets)
{
  ಆದರೆ (!ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ])
    {
      ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ] =
        xnmalloc (s->nitems, ಗಾತ್ರ ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][0]);
      ಗಾಗಿ (size_t i = 0; i < s->nitems; ++i)
        ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][i] = ಖಾಲಿ;
    }
  ಆದರೆ (!ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ])
    {
      ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ] =
        bitset_create (ntokens, BITSET_FIXED);
      /* If this kernel item is the beginning of a RHS, it must be the kernel
         item in the start state, and so its LHS has no follows and no goto to
         check.  If, instead, this kernel item is the successor of the start
         state's kernel item, there are still no follows and no goto.  This
         situation is fortunate because we want to avoid the - 2 below in both
         cases.

         Actually, IELR(1) should never invoke this function for either of
         those cases because (1) follow_kernel_items will never reference a
         kernel item for this RHS because the end token blocks sight of the
         lookahead set from the RHS's only nonterminal, and (2) no reduction
         has a lookback dependency on this lookahead set.  Nevertheless, I
         didn't change this test to an aver just in case the usage of this
         function evolves to need those two cases.  In both cases, the current
         implementation returns the right result.  */
      ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = ಅಂಶ_ನಿಯಮ (&ritem[s->ವಸ್ತುಗಳು[ಅಂಶ]]);
      ಸ್ಥಿರಾಂಕ bool is_successor_ದ_initial_ಅಂಶ
        = ನಿಯಮ_is_initial (r) && &ritem[s->ವಸ್ತುಗಳು[ಅಂಶ]] == r->rhs + 1;
      aver (!is_successor_ದ_initial_ಅಂಶ);
      ಆದರೆ (!is_successor_ದ_initial_ಅಂಶ)
        {
          /* If the LHS symbol of this item isn't known (because this is a
             top-level invocation), go get it.  */
          ಆದರೆ (!lhs)
            lhs = r->lhs->ಸಂಖ್ಯೆ;
          /* If this kernel item is next to the beginning of the RHS, then
             check all predecessors' goto follows for the LHS.  */
          ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[ಅಂಶ] - 2]))
            {
              aver (lhs != acceptsymbol->content->ಸಂಖ್ಯೆ);
              ಗಾಗಿ (state **predecessor = predecessors[s->ಸಂಖ್ಯೆ];
                   *predecessor;
                   ++predecessor)
                bitset_or (ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ],
                           ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ],
                           goto_follows[map_goto ((*predecessor)->ಸಂಖ್ಯೆ,
                                                  lhs)]);
            }
          /* If this kernel item is later in the RHS, then check all
             predecessor items' lookahead sets.  */
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಗಾಗಿ (state **predecessor = predecessors[s->ಸಂಖ್ಯೆ];
                   *predecessor;
                   ++predecessor)
                {
                  size_t predecessor_ಅಂಶ;
                  ಗಾಗಿ (predecessor_ಅಂಶ = 0;
                       predecessor_ಅಂಶ < (*predecessor)->nitems;
                       ++predecessor_ಅಂಶ)
                    ಆದರೆ ((*predecessor)->ವಸ್ತುಗಳು[predecessor_ಅಂಶ]
                        == s->ವಸ್ತುಗಳು[ಅಂಶ] - 1)
                      ನಿಲ್ಲಿಸು;
                  aver (predecessor_ಅಂಶ != (*predecessor)->nitems);
                  ielr_ಅಂಶ_has_lookahead (*predecessor, lhs,
                                           predecessor_ಅಂಶ, 0 /*irrelevant*/,
                                           predecessors, ಅಂಶ_lookahead_sets);
                  bitset_or (ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ],
                             ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ],
                             ಅಂಶ_lookahead_sets[(*predecessor)->ಸಂಖ್ಯೆ]
                                                [predecessor_ಅಂಶ]);
                }
            }
        }
    }
  ಹಿಂತಿರುಗಿ bitset_test (ಅಂಶ_lookahead_sets[s->ಸಂಖ್ಯೆ][ಅಂಶ], lookahead);
}

/**
 * \pre
 *   - \c follow_kernel_items, \c always_follows, and \c predecessors
 *     were computed by \c ielr_compute_auxiliary_tables.
 * \post
 *   - Each of <tt>*inadequacy_listsp</tt> and <tt>*annotation_listsp</tt>
 *     points to a new array of size \c ::nstates.
 *   - For <tt>0 <= i < ::nstates</tt>:
 *     - <tt>(*inadequacy_listsp)[i]</tt> contains the \c InadequacyList head
 *       node for <tt>states[i]</tt>.
 *     - <tt>(*annotation_listsp)[i]</tt> contains the \c AnnotationList head
 *       node for <tt>states[i]</tt>.
 *   - <tt>*max_annotationsp</tt> is the maximum number of annotations per
 *     state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_annotation_lists (bitsetv follow_kernel_ವಸ್ತುಗಳು,
                               bitsetv always_follows, state ***predecessors,
                               Annotation_ಸೂಚ್ಯಂಕ *max_annotationsp,
                               InadequacyList ***inadequacy_listsp,
                               AnnotationList ***annotation_listsp,
                               ರಚನೆ obstack *annotations_obstackp)
{
  bitset **ಅಂಶ_lookahead_sets =
    xnmalloc (nstates, ಗಾತ್ರ *ಅಂಶ_lookahead_sets);
  Annotation_ಸೂಚ್ಯಂಕ *annotation_counts =
    xnmalloc (nstates, ಗಾತ್ರ *annotation_counts);
  Contribution_ಸೂಚ್ಯಂಕ max_contributions = 0;
  ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_annotations = 0;

  *inadequacy_listsp = xnmalloc (nstates, ಗಾತ್ರ **inadequacy_listsp);
  *annotation_listsp = xnmalloc (nstates, ಗಾತ್ರ **annotation_listsp);
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಅಂಶ_lookahead_sets[i] = ಖಾಲಿ;
      (*inadequacy_listsp)[i] = ಖಾಲಿ;
      (*annotation_listsp)[i] = ಖಾಲಿ;
      annotation_counts[i] = 0;
    }
  {
    Inadequacy_List_ಗಂಟು_ಎಣಿಕೆ inadequacy_list_ಗಂಟು_ಎಣಿಕೆ = 0;
    ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
      AnnotationList__compute_from_inadequacies (
        ಸ್ಥಿತಿಗಳು[i], follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
        ಅಂಶ_lookahead_sets, *inadequacy_listsp, *annotation_listsp,
        annotation_counts, &max_contributions, annotations_obstackp,
        &inadequacy_list_ಗಂಟು_ಎಣಿಕೆ);
  }
  *max_annotationsp = 0;
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    {
      ಆದರೆ (annotation_counts[i] > *max_annotationsp)
        *max_annotationsp = annotation_counts[i];
      ಒಟ್ಟು_annotations += annotation_counts[i];
    }
  ಆದರೆ (trace_flag & trace_ielr)
    {
      ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
        {
          fprintf (stderr, "Inadequacy annotations for state %d:\n", i);
          AnnotationList__debug ((*annotation_listsp)[i],
                                 ಸ್ಥಿತಿಗಳು[i]->nitems, 2);
        }
      fprintf (stderr, "Number of LR(0)/LALR(1) states: %d\n", nstates);
      fprintf (stderr, "Average number of annotations per state: %f\n",
               (ದಶಮಾಂಶ)ಒಟ್ಟು_annotations/nstates);
      fprintf (stderr, "Max number of annotations per state: %d\n",
               *max_annotationsp);
      fprintf (stderr, "Max number of contributions per annotation: %d\n",
               max_contributions);
    }
  ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
    ಆದರೆ (ಅಂಶ_lookahead_sets[i])
      {
        ಗಾಗಿ (size_t j = 0; j < ಸ್ಥಿತಿಗಳು[i]->nitems; ++j)
          ಆದರೆ (ಅಂಶ_lookahead_sets[i][j])
            bitset_free (ಅಂಶ_lookahead_sets[i][j]);
        ಬಿಡುಗಡೆ (ಅಂಶ_lookahead_sets[i]);
      }
  ಬಿಡುಗಡೆ (ಅಂಶ_lookahead_sets);
  ಬಿಡುಗಡೆ (annotation_counts);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ state_list
{
  ರಚನೆ state_list *ಮುಂದಿನ;
  state *state;
  /** Has this state been recomputed as a successor of another state?  */
  bool recomputedAsSuccessor;
  /**
   * \c NULL iff all lookahead sets are empty.  <tt>lookaheads[i] = NULL</tt>
   * iff the lookahead set on item \c i is empty.
   */
  bitset *lookaheads;
  /**
   * nextIsocore is the next state in a circularly linked-list of all states
   * with the same core.  The one originally computed by generate_states in
   * lr0.c is lr0Isocore.
   */
  ರಚನೆ state_list *lr0Isocore;
  ರಚನೆ state_list *ಮುಂದಿನ_Isocore;
} state_list;

MAYBE_ಬಳಸದ ಸ್ಥಿರ ಶೂನ್ಯ
state_list_print_ (ಸ್ಥಿರಾಂಕ state_list *s, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sep)
{
  ಆದರೆ (s)
    {
      fprintf (ಹೊರಹೋಗು, "%s%d", sep, s->state->ಸಂಖ್ಯೆ);
      state_list_print_ (s->ಮುಂದಿನ, ಹೊರಹೋಗು, " ");
    }
}

MAYBE_ಬಳಸದ ಸ್ಥಿರ ಶೂನ್ಯ
state_list_print (ಸ್ಥಿರಾಂಕ state_list *s, FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು, "{");
  state_list_print_ (s, ಹೊರಹೋಗು, "");
  fprintf (ಹೊರಹೋಗು, "}");
}

/**
 * \pre
 *   - \c follow_kernel_items and \c always_follows were computed by
 *     \c ielr_compute_auxiliary_tables.
 *   - <tt>n->class = nterm_sym</tt>.
 * \post
 *   - \c follow_set contains the follow set for the goto on nonterminal \c n
 *     in state \c s based on the lookaheads stored in <tt>s->lookaheads</tt>.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_goto_follow_ಗಣ (bitsetv follow_kernel_ವಸ್ತುಗಳು,
                              bitsetv always_follows, state_list *s,
                              sym_content *n, bitset follow_ಗಣ)
{
  goto_ಸಂಖ್ಯೆ n_goto = map_goto (s->lr0Isocore->state->ಸಂಖ್ಯೆ, n->ಸಂಖ್ಯೆ);
  bitset_copy (follow_ಗಣ, always_follows[n_goto]);
  ಆದರೆ (s->lookaheads)
    {
      bitset_iterator biter_ಅಂಶ;
      bitset_bindex ಅಂಶ;
      BITSET_FOR_EACH (biter_ಅಂಶ, follow_kernel_ವಸ್ತುಗಳು[n_goto], ಅಂಶ, 0)
        ಆದರೆ (s->lookaheads[ಅಂಶ])
          bitset_or (follow_ಗಣ, follow_ಗಣ, s->lookaheads[ಅಂಶ]);
    }
}

/**
 * \pre
 *   - \c follow_kernel_items and \c always_follows were computed by
 *     \c ielr_compute_auxiliary_tables.
 *   - \c lookahead_filter was computed by
 *     \c AnnotationList__computeLookaheadFilter for the original LR(0) isocore
 *     of \c t.
 *   - The number of rows in \c lookaheads is at least the number of items in
 *     \c t, and the number of columns is \c ::ntokens.
 * \post
 *   - <tt>lookaheads[i][j]</tt> is set iff both:
 *     - <tt>lookahead_filter[i][j]</tt> is set.
 *     - The isocore of \c t that will be the transition successor of \c s will
 *       inherit from \c s token \c j into the lookahead set of item \c i.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_lookaheads (bitsetv follow_kernel_ವಸ್ತುಗಳು, bitsetv always_follows,
                         state_list *s, state *t, bitsetv lookahead_filter,
                         bitsetv lookaheads)
{
  size_t s_ಅಂಶ = 0;
  bitsetv_zero (lookaheads);
  ಗಾಗಿ (size_t t_ಅಂಶ = 0; t_ಅಂಶ < t->nitems; ++t_ಅಂಶ)
    {
      /* If this kernel item is the beginning of a RHS, it must be the
         kernel item in the start state, but t is supposed to be a successor
         state.  If, instead, this kernel item is the successor of the start
         state's kernel item, the lookahead set is empty.  This second case is
         a special case to avoid the - 2 below, but the next successor can be
         handled fine without special casing it.  */
      aver (t->ವಸ್ತುಗಳು[t_ಅಂಶ] != 0);
      ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = ಅಂಶ_ನಿಯಮ (&ritem[t->ವಸ್ತುಗಳು[t_ಅಂಶ]]);
      ಸ್ಥಿರಾಂಕ bool is_successor_ದ_initial_ಅಂಶ
        = ನಿಯಮ_is_initial (r) && &ritem[t->ವಸ್ತುಗಳು[t_ಅಂಶ]] == r->rhs + 1;
      ಆದರೆ (!is_successor_ದ_initial_ಅಂಶ
          && !bitset_ಖಾಲಿಗಳು_p (lookahead_filter[t_ಅಂಶ]))
        {
          /* Is this kernel item next to the beginning of the RHS?  */
          ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[t->ವಸ್ತುಗಳು[t_ಅಂಶ] - 2]))
            ielr_compute_goto_follow_ಗಣ (
              follow_kernel_ವಸ್ತುಗಳು, always_follows, s,
              r->lhs,
              lookaheads[t_ಅಂಶ]);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s->lookaheads)
            {
              /* We don't have to start the s item search at the beginning
                 every time because items from both states are sorted by their
                 indices in ritem.  */
              ಗಾಗಿ (; s_ಅಂಶ < s->state->nitems; ++s_ಅಂಶ)
                ಆದರೆ (s->state->ವಸ್ತುಗಳು[s_ಅಂಶ] == t->ವಸ್ತುಗಳು[t_ಅಂಶ] - 1)
                  ನಿಲ್ಲಿಸು;
              aver (s_ಅಂಶ != s->state->nitems);
              ಆದರೆ (s->lookaheads[s_ಅಂಶ])
                bitset_copy (lookaheads[t_ಅಂಶ], s->lookaheads[s_ಅಂಶ]);
            }
          bitset_and (lookaheads[t_ಅಂಶ],
                      lookaheads[t_ಅಂಶ], lookahead_filter[t_ಅಂಶ]);
        }
    }
}

/**
 * \pre
 *   - \c follow_kernel_items and \c always_follows were computed by
 *     \c ielr_compute_auxiliary_tables.
 *   - Either:
 *     - <tt>annotation_lists = NULL</tt> and all bits in work2 are set.
 *     - \c annotation_lists was computed by \c ielr_compute_annotation_lists.
 *   - The number of rows in each of \c lookaheads and \c work2 is the maximum
 *     number of items in any state.  The number of columns in each is
 *     \c ::ntokens.
 *   - \c lookaheads was computed by \c ielr_compute_lookaheads for \c t.
 *   - \c ::nstates is the total number of states, some not yet fully computed,
 *     in the list ending at \c *last_statep.  It is at least the number of
 *     original LR(0) states.
 *   - The size of \c work1 is at least the number of annotations for the LR(0)
 *     isocore of \c t.
 * \post
 *   - Either:
 *     - In the case that <tt>annotation_lists != NULL</tt>,
 *       <tt>lookaheads \@pre</tt> was merged with some isocore of \c t if
 *       permitted by the annotations for the original LR(0) isocore of \c t.
 *       If this changed the lookaheads in that isocore, those changes were
 *       propagated to all already computed transition successors recursively
 *       possibly resulting in the splitting of some of those successors.
 *     - In the case that <tt>annotation_lists = NULL</tt>,
 *       <tt>lookaheads \@pre</tt> was merged with some isocore of \c t if the
 *       isocore's lookahead sets were identical to those specified by
 *       <tt>lookaheads \@pre</tt>.
 *     - If no such merge was permitted, a new isocore of \c t containing
 *       <tt>lookaheads \@pre</tt> was appended to the state list whose
 *       previous tail was <tt>*last_statep \@pre</tt> and \c ::nstates was
 *       incremented.  It was also appended to \c t's isocore list.
 *   - <tt>*tp</tt> = the isocore of \c t into which
 *     <tt>lookaheads \@pre</tt> was placed/merged.
 *   - \c lookaheads, \c work1, and \c work2 may have been altered.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_compute_state (bitsetv follow_kernel_ವಸ್ತುಗಳು, bitsetv always_follows,
                    AnnotationList **annotation_lists, state *t,
                    bitsetv lookaheads, state_list **ಕೊನೆಯ_statep,
                    Contribution_ಸೂಚ್ಯಂಕ work1[], bitsetv work2, state **tp)
{
  state_list *lr0_isocore = t->state_list->lr0Isocore;
  state_list **this_isocorep;

  /* Determine whether there's an isocore of t with which these lookaheads can
     be merged.  */
  {
    ಆದರೆ (annotation_lists)
      {
        Annotation_ಸೂಚ್ಯಂಕ ai;
        AnnotationList *a;
        ಗಾಗಿ (ai = 0, a = annotation_lists[lr0_isocore->state->ಸಂಖ್ಯೆ];
             a;
             ++ai, a = a->ಮುಂದಿನ)
        work1[ai] =
          AnnotationList__computeDominantContribution (
            a, lr0_isocore->state->nitems, lookaheads, false);
      }
    ಗಾಗಿ (this_isocorep = &t->state_list;
         this_isocorep == &t->state_list || *this_isocorep != t->state_list;
         this_isocorep = &(*this_isocorep)->ಮುಂದಿನ_Isocore)
      {
        ಆದರೆ (!(*this_isocorep)->recomputedAsSuccessor)
          ನಿಲ್ಲಿಸು;
        ಆದರೆ (annotation_lists)
          {
            Annotation_ಸೂಚ್ಯಂಕ ai;
            AnnotationList *a;
            ಗಾಗಿ (ai = 0, a = annotation_lists[lr0_isocore->state->ಸಂಖ್ಯೆ];
                 a;
                 ++ai, a = a->ಮುಂದಿನ)
              {
                ಆದರೆ (work1[ai] != Contribution_ಸೂಚ್ಯಂಕ_none)
                  {
                    /* This isocore compatibility test depends on the fact
                       that, if the dominant contributions are the same for the
                       two isocores, then merging their lookahead sets will not
                       produce a state with a different dominant contribution.
                       */
                    Contribution_ಸೂಚ್ಯಂಕ ci =
                      AnnotationList__computeDominantContribution (
                        a, lr0_isocore->state->nitems,
                        (*this_isocorep)->lookaheads, false);
                    ಆದರೆ (ci != Contribution_ಸೂಚ್ಯಂಕ_none && work1[ai] != ci)
                      ನಿಲ್ಲಿಸು;
                  }
              }
            ಆದರೆ (!a)
              ನಿಲ್ಲಿಸು;
          }
        ಇಲ್ಲದಿದ್ದರೆ
          {
            size_t i;
            ಗಾಗಿ (i = 0; i < t->nitems; ++i)
              {
                ಆದರೆ (!(*this_isocorep)->lookaheads
                    || !(*this_isocorep)->lookaheads[i])
                  {
                    ಆದರೆ (!bitset_ಖಾಲಿಗಳು_p (lookaheads[i]))
                      ನಿಲ್ಲಿಸು;
                  }
                /* bitset_equal_p uses the size of the first argument,
                   so lookaheads[i] must be the second argument.  */
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!bitset_equal_p ((*this_isocorep)->lookaheads[i],
                                          lookaheads[i]))
                  ನಿಲ್ಲಿಸು;
              }
            ಆದರೆ (i == t->nitems)
              ನಿಲ್ಲಿಸು;
          }
      }
  }

  bool has_lookaheads = false;
  ಗಾಗಿ (size_t i = 0; i < lr0_isocore->state->nitems; ++i)
    ಆದರೆ (!bitset_ಖಾಲಿಗಳು_p (lookaheads[i]))
      {
        has_lookaheads = true;
        ನಿಲ್ಲಿಸು;
      }

  /* Merge with an existing isocore.  */
  ಆದರೆ (this_isocorep == &t->state_list || *this_isocorep != t->state_list)
    {
      bool new_lookaheads = false;
      *tp = (*this_isocorep)->state;

      /* Merge lookaheads into the state and record whether any of them are
         actually new.  */
      ಆದರೆ (has_lookaheads)
        {
          ಆದರೆ (!(*this_isocorep)->lookaheads)
            {
              (*this_isocorep)->lookaheads =
                xnmalloc (t->nitems, ಗಾತ್ರ (*this_isocorep)->lookaheads);
              ಗಾಗಿ (size_t i = 0; i < t->nitems; ++i)
                (*this_isocorep)->lookaheads[i] = ಖಾಲಿ;
            }
          ಗಾಗಿ (size_t i = 0; i < t->nitems; ++i)
            ಆದರೆ (!bitset_ಖಾಲಿಗಳು_p (lookaheads[i]))
              {
                ಆದರೆ (!(*this_isocorep)->lookaheads[i])
                  (*this_isocorep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                bitset_andn (lookaheads[i],
                             lookaheads[i], (*this_isocorep)->lookaheads[i]);
                bitset_or ((*this_isocorep)->lookaheads[i],
                           lookaheads[i], (*this_isocorep)->lookaheads[i]);
                ಆದರೆ (!bitset_ಖಾಲಿಗಳು_p (lookaheads[i]))
                  new_lookaheads = true;
              }
        }

      /* If new lookaheads were merged, propagate those lookaheads to the
         successors, possibly splitting them.  If *tp is being recomputed for
         the first time, this isn't necessary because the main
         ielr_split_states loop will handle the successors later.  */
      ಆದರೆ (!(*this_isocorep)->recomputedAsSuccessor)
        (*this_isocorep)->recomputedAsSuccessor = true;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (new_lookaheads)
        {
          /* When merging demands identical lookahead sets, it is impossible to
             merge new lookaheads.  */
          aver (annotation_lists);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (*tp)->transitions->ಅಂಕೆ; ++i)
            {
              state *t2 = (*tp)->transitions->ಸ್ಥಿತಿಗಳು[i];
              /* At any time, there's at most one state for which we have so
                 far initially recomputed only some of its successors in the
                 main ielr_split_states loop.  Because we recompute successors
                 in order, we can just stop at the first such successor.  Of
                 course, *tp might be a state some of whose successors have
                 been recomputed as successors of other states rather than as
                 successors of *tp.  It's fine if we go ahead and propagate to
                 some of those.  We'll propagate to them again (but stop when
                 we see nothing changes) and to the others when we reach *tp in
                 the main ielr_split_states loop later.  */
              ಆದರೆ (!t2->state_list->recomputedAsSuccessor)
                ನಿಲ್ಲಿಸು;
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t2->state_list->lr0Isocore->state->ಸಂಖ್ಯೆ],
                t2->nitems, work2);
              ielr_compute_lookaheads (follow_kernel_ವಸ್ತುಗಳು, always_follows,
                                       (*this_isocorep), t2, work2,
                                       lookaheads);
              /* FIXME: If splitting t2 here, it's possible that lookaheads
                 that had already propagated from *tp to t2 will be left in t2
                 after *tp has been removed as t2's predecessor:
                 - We're going to recompute all lookaheads in phase 4, so these
                   extra lookaheads won't appear in the final parser table.
                 - If t2 has just one annotation, then these extra lookaheads
                   cannot alter the dominating contribution to the associated
                   inadequacy and thus cannot needlessly prevent a future merge
                   of some new state with t2.  We can be sure of this because:
                   - The fact that we're splitting t2 now means that some
                     predecessors (at least one) other than *tp must be
                     propagating contributions to t2.
                   - The fact that t2 was merged in the first place means that,
                     if *tp propagated any contributions, the dominating
                     contribution must be the same as that from those other
                     predecessors.
                   - Thus, if some new state to be merged with t2 in the future
                     proves to be compatible with the contributions propagated
                     by the other predecessors, it will also be compatible with
                     the contributions made by the extra lookaheads left behind
                     by *tp.
                 - However, if t2 has more than one annotation and these extra
                   lookaheads contribute to one of their inadequacies, it's
                   possible these extra lookaheads may needlessly prevent a
                   future merge with t2.  For example:
                   - Let's say there's an inadequacy A that makes the split
                     necessary as follows:
                     - There's currently just one other predecessor and it
                       propagates to t2 some contributions to inadequacy A.
                     - The new lookaheads that we were attempting to propagate
                       from *tp to t2 made contributions to inadequacy A with a
                       different dominating contribution than those from that
                       other predecessor.
                     - The extra lookaheads either make no contribution to
                       inadequacy A or have the same dominating contribution as
                       the contributions from the other predecessor.  Either
                       way, as explained above, they can't prevent a future
                       merge.
                   - Let's say there's an inadequacy B that causes the trouble
                     with future merges as follows:
                     - The extra lookaheads make contributions to inadequacy B.
                     - Those extra contributions did not prevent the original
                       merge to create t2 because the other predecessor
                       propagates to t2 no contributions to inadequacy B.
                     - Thus, those extra contributions may prevent a future
                       merge with t2 even though the merge would be fine if *tp
                       had not left them behind.
                 - Is the latter case common enough to worry about?
                 - Perhaps we should track all predecessors and iterate them
                   now to recreate t2 without those extra lookaheads.  */
              ielr_compute_state (follow_kernel_ವಸ್ತುಗಳು, always_follows,
                                  annotation_lists, t2, lookaheads,
                                  ಕೊನೆಯ_statep, work1, work2,
                                  &(*tp)->transitions->ಸ್ಥಿತಿಗಳು[i]);
            }
        }
    }

  /* Create a new isocore.  */
  ಇಲ್ಲದಿದ್ದರೆ
    {
      state_list *old_isocore = *this_isocorep;
      (*ಕೊನೆಯ_statep)->ಮುಂದಿನ = *this_isocorep = xmalloc (ಗಾತ್ರ **ಕೊನೆಯ_statep);
      *ಕೊನೆಯ_statep = *this_isocorep;
      (*ಕೊನೆಯ_statep)->state = *tp = state_new_isocore (t);
      (*tp)->state_list = *ಕೊನೆಯ_statep;
      (*ಕೊನೆಯ_statep)->recomputedAsSuccessor = true;
      (*ಕೊನೆಯ_statep)->ಮುಂದಿನ = ಖಾಲಿ;
      (*ಕೊನೆಯ_statep)->lookaheads = ಖಾಲಿ;
      ಆದರೆ (has_lookaheads)
        {
          (*ಕೊನೆಯ_statep)->lookaheads =
            xnmalloc (t->nitems, ಗಾತ್ರ (*ಕೊನೆಯ_statep)->lookaheads);
          ಗಾಗಿ (size_t i = 0; i < t->nitems; ++i)
            {
              ಆದರೆ (bitset_ಖಾಲಿಗಳು_p (lookaheads[i]))
                (*ಕೊನೆಯ_statep)->lookaheads[i] = ಖಾಲಿ;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  (*ಕೊನೆಯ_statep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                  bitset_copy ((*ಕೊನೆಯ_statep)->lookaheads[i], lookaheads[i]);
                }
            }
        }
      (*ಕೊನೆಯ_statep)->lr0Isocore = lr0_isocore;
      (*ಕೊನೆಯ_statep)->ಮುಂದಿನ_Isocore = old_isocore;
    }
}

/**
 * \pre
 *   - \c follow_kernel_items and \c always_follows were computed by
 *     \c ielr_compute_auxiliary_tables.
 *   - Either:
 *     - <tt>annotation_lists = NULL</tt> and <tt>max_annotations=0</tt>.
 *     - \c annotation_lists and \c max_annotations were computed by
 *       \c ielr_compute_annotation_lists.
 * \post
 *   - \c ::states is of size \c ::nstates (which might be greater than
 *     <tt>::nstates \@pre</tt>) and no longer contains any LR(1)-relative
 *     inadequacy.  \c annotation_lists was used to determine state
 *     compatibility or, if <tt>annotation_lists = NULL</tt>, the canonical
 *     LR(1) state compatibility test was used.
 *   - If <tt>annotation_lists = NULL</tt>, reduction lookahead sets were
 *     computed in all states.  tv_ielr_phase4 was pushed while they were
 *     computed from item lookahead sets.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ielr_ವಿಭಜಿಸು_ಸ್ಥಿತಿಗಳು (bitsetv follow_kernel_ವಸ್ತುಗಳು, bitsetv always_follows,
                   AnnotationList **annotation_lists,
                   Annotation_ಸೂಚ್ಯಂಕ max_annotations)
{
  state_list *ಮೊದಲ_state;
  state_list *ಕೊನೆಯ_state;
  bitsetv lookahead_filter = ಖಾಲಿ;
  bitsetv lookaheads;

  /* Set up state list and some reusable bitsets.  */
  {
    size_t max_nitems = 0;
    state_list **nodep = &ಮೊದಲ_state;
    ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
      {
        *nodep = ಸ್ಥಿತಿಗಳು[i]->state_list = ಕೊನೆಯ_state = xmalloc (ಗಾತ್ರ **nodep);
        (*nodep)->state = ಸ್ಥಿತಿಗಳು[i];
        (*nodep)->recomputedAsSuccessor = false;
        (*nodep)->lookaheads = ಖಾಲಿ;
        (*nodep)->lr0Isocore = *nodep;
        (*nodep)->ಮುಂದಿನ_Isocore = *nodep;
        nodep = &(*nodep)->ಮುಂದಿನ;
        ಆದರೆ (ಸ್ಥಿತಿಗಳು[i]->nitems > max_nitems)
          max_nitems = ಸ್ಥಿತಿಗಳು[i]->nitems;
      }
    *nodep = ಖಾಲಿ;
    lookahead_filter = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
    ಆದರೆ (!annotation_lists)
      bitsetv_ones (lookahead_filter);
    lookaheads = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
  }

  /* Recompute states.  */
  {
    Contribution_ಸೂಚ್ಯಂಕ *work = xnmalloc (max_annotations, ಗಾತ್ರ *work);
    ಗಾಗಿ (state_list *this_state = ಮೊದಲ_state;
         this_state;
         this_state = this_state->ಮುಂದಿನ)
      {
        ಸ್ಥಿರಾಂಕ state *s = this_state->state;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < s->transitions->ಅಂಕೆ; ++i)
          {
            state *t = s->transitions->ಸ್ಥಿತಿಗಳು[i];
            ಆದರೆ (annotation_lists)
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t->state_list->lr0Isocore->state->ಸಂಖ್ಯೆ],
                t->nitems, lookahead_filter);
            ielr_compute_lookaheads (follow_kernel_ವಸ್ತುಗಳು, always_follows,
                                     this_state, t, lookahead_filter,
                                     lookaheads);
            ielr_compute_state (follow_kernel_ವಸ್ತುಗಳು, always_follows,
                                annotation_lists, t, lookaheads, &ಕೊನೆಯ_state,
                                work, lookahead_filter,
                                &s->transitions->ಸ್ಥಿತಿಗಳು[i]);
          }
      }
    ಬಿಡುಗಡೆ (work);
  }

  bitsetv_free (lookahead_filter);
  bitsetv_free (lookaheads);

  /* Store states back in the states array.  */
  ಸ್ಥಿತಿಗಳು = xnrealloc (ಸ್ಥಿತಿಗಳು, nstates, ಗಾತ್ರ *ಸ್ಥಿತಿಗಳು);
  ಗಾಗಿ (state_list *ಗಂಟು = ಮೊದಲ_state; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ)
    ಸ್ಥಿತಿಗಳು[ಗಂಟು->state->ಸಂಖ್ಯೆ] = ಗಂಟು->state;

  /* In the case of canonical LR(1), copy item lookahead sets to reduction
     lookahead sets.  */
  ಆದರೆ (!annotation_lists)
    {
      timevar_ತಳ್ಳು (tv_ielr_phase4);
      initialize_LA ();
      ಗಾಗಿ (state_list *ಗಂಟು = ಮೊದಲ_state; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ)
        ಆದರೆ (!ಗಂಟು->state->consistent)
          {
            size_t i = 0;
            ಅಂಶ_ಸೂಚ್ಯಂಕ *itemset = ಗಂಟು->state->ವಸ್ತುಗಳು;
            ಗಾಗಿ (size_t r = 0; r < ಗಂಟು->state->reductions->ಅಂಕೆ; ++r)
              {
                ನಿಯಮ *this_ನಿಯಮ = ಗಂಟು->state->reductions->ನಿಯಮಗಳು[r];
                bitset lookahead_ಗಣ =
                  ಗಂಟು->state->reductions->lookaheads[r];
                ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (*this_ನಿಯಮ->rhs))
                  ielr_compute_goto_follow_ಗಣ (follow_kernel_ವಸ್ತುಗಳು,
                                                always_follows, ಗಂಟು,
                                                this_ನಿಯಮ->lhs, lookahead_ಗಣ);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->lookaheads)
                  {
                    /* We don't need to start the kernel item search back at
                       i=0 because both items and reductions are sorted on rule
                       number.  */
                    ಆಗ (!ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[itemset[i]])
                           || ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[itemset[i]])
                              != this_ನಿಯಮ->ಸಂಖ್ಯೆ)
                      {
                        ++i;
                        aver (i < ಗಂಟು->state->nitems);
                      }
                    ಆದರೆ (ಗಂಟು->lookaheads[i])
                      bitset_copy (lookahead_ಗಣ, ಗಂಟು->lookaheads[i]);
                  }
              }
          }
      timevar_ತೆಗೆ (tv_ielr_phase4);
    }

  /* Free state list.  */
  ಆಗ (ಮೊದಲ_state)
    {
      state_list *ಗಂಟು = ಮೊದಲ_state;
      ಆದರೆ (ಗಂಟು->lookaheads)
        {
          ಗಾಗಿ (size_t i = 0; i < ಗಂಟು->state->nitems; ++i)
            ಆದರೆ (ಗಂಟು->lookaheads[i])
              bitset_free (ಗಂಟು->lookaheads[i]);
          ಬಿಡುಗಡೆ (ಗಂಟು->lookaheads);
        }
      ಮೊದಲ_state = ಗಂಟು->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (ಗಂಟು);
    }
}


ಶೂನ್ಯ
ielr (ಶೂನ್ಯ)
{
  LrType lr_type = lr_type_get ();

  /* Phase 0: LALR(1).  */
  ಆಯ್ಕೆ (lr_type)
    {
    ಸ್ಥಿತಿ LR_TYPE__LR0:
      timevar_ತಳ್ಳು (tv_lalr);
      ಗಣ_goto_map ();
      timevar_ತೆಗೆ (tv_lalr);
      ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿತಿ LR_TYPE__CANONICAL_LR:
      timevar_ತಳ್ಳು (tv_lalr);
      ಗಣ_goto_map ();
      timevar_ತೆಗೆ (tv_lalr);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ LR_TYPE__LALR:
      timevar_ತಳ್ಳು (tv_lalr);
      lalr ();
      bitsetv_free (goto_follows);
      timevar_ತೆಗೆ (tv_lalr);
      ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿತಿ LR_TYPE__IELR:
      timevar_ತಳ್ಳು (tv_lalr);
      lalr ();
      timevar_ತೆಗೆ (tv_lalr);
      ನಿಲ್ಲಿಸು;
    }

  {
    bitsetv follow_kernel_ವಸ್ತುಗಳು;
    bitsetv always_follows;
    InadequacyList **inadequacy_lists = ಖಾಲಿ;
    AnnotationList **annotation_lists = ಖಾಲಿ;
    ರಚನೆ obstack annotations_obstack;
    Annotation_ಸೂಚ್ಯಂಕ max_annotations = 0;

    {
      /* Phase 1: Compute Auxiliary Tables.  */
      state ***predecessors;
      timevar_ತಳ್ಳು (tv_ielr_phase1);
      ielr_compute_auxiliary_tables (
        &follow_kernel_ವಸ್ತುಗಳು, &always_follows,
        lr_type == LR_TYPE__CANONICAL_LR ? ಖಾಲಿ : &predecessors);
      timevar_ತೆಗೆ (tv_ielr_phase1);

      /* Phase 2: Compute Annotations.  */
      timevar_ತಳ್ಳು (tv_ielr_phase2);
      ಆದರೆ (lr_type != LR_TYPE__CANONICAL_LR)
        {
          obstack_init (&annotations_obstack);
          ielr_compute_annotation_lists (follow_kernel_ವಸ್ತುಗಳು, always_follows,
                                         predecessors, &max_annotations,
                                         &inadequacy_lists, &annotation_lists,
                                         &annotations_obstack);
          ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates; ++i)
            ಬಿಡುಗಡೆ (predecessors[i]);
          ಬಿಡುಗಡೆ (predecessors);
          bitsetv_free (goto_follows);
          lalr_free ();
        }
      timevar_ತೆಗೆ (tv_ielr_phase2);
    }

    /* Phase 3: Split States.  */
    timevar_ತಳ್ಳು (tv_ielr_phase3);
    {
      state_ಸಂಖ್ಯೆ nstates_lr0 = nstates;
      ielr_ವಿಭಜಿಸು_ಸ್ಥಿತಿಗಳು (follow_kernel_ವಸ್ತುಗಳು, always_follows,
                         annotation_lists, max_annotations);
      ಆದರೆ (inadequacy_lists)
        ಗಾಗಿ (state_ಸಂಖ್ಯೆ i = 0; i < nstates_lr0; ++i)
          InadequacyList__delete (inadequacy_lists[i]);
    }
    ಬಿಡುಗಡೆ (inadequacy_lists);
    ಆದರೆ (annotation_lists)
      obstack_free (&annotations_obstack, ಖಾಲಿ);
    ಬಿಡುಗಡೆ (annotation_lists);
    bitsetv_free (follow_kernel_ವಸ್ತುಗಳು);
    bitsetv_free (always_follows);
    timevar_ತೆಗೆ (tv_ielr_phase3);
  }

  /* Phase 4: Compute Reduction Lookaheads.  */
  timevar_ತಳ್ಳು (tv_ielr_phase4);
  ಬಿಡುಗಡೆ (goto_map);
  ಬಿಡುಗಡೆ (from_state);
  ಬಿಡುಗಡೆ (ಗಮ್ಯ_state);
  ಆದರೆ (lr_type == LR_TYPE__CANONICAL_LR)
    {
      /* Reduction lookaheads are computed in ielr_split_states above
         but are timed as part of phase 4. */
      ಗಣ_goto_map ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      lalr ();
      bitsetv_free (goto_follows);
    }
  timevar_ತೆಗೆ (tv_ielr_phase4);
}
