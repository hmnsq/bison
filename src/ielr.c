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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "ielr.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <timevar.h>

#அந்தர்பாவ "AnnotationList.h"
#அந்தர்பாவ "derives.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "nullable.h"
#அந்தர்பாவ "relation.h"
#அந்தர்பாவ "state.h"
#அந்தர்பாவ "symtab.h"

/** Records the value of the \%define variable lr.type.  */
வகைவரை எண்ணிக்கை
  {
   LR_TYPE__LR0,
   LR_TYPE__LALR,
   LR_TYPE__IELR,
   LR_TYPE__CANONICAL_LR
  } LrType;

/* The user's requested LR type.  */
நிலையான LrType
lr_type_get (வெறுமை)
{
  எழுத்து *type = muscle_percent_define_get ("lr.type");
  LrType res;
  எனில் (STREQ (type, "lr""(0)"))
    res = LR_TYPE__LR0;
  இல்லையெனில் எனில் (STREQ (type, "lalr"))
    res = LR_TYPE__LALR;
  இல்லையெனில் எனில் (STREQ (type, "ielr"))
    res = LR_TYPE__IELR;
  இல்லையெனில் எனில் (STREQ (type, "canonical-lr"))
    res = LR_TYPE__CANONICAL_LR;
  இல்லையெனில்
    {
      aver (false);
      abort ();
    }
  விடுவி (type);
  திரும்பு res;
}

/**
 * \post:
 *   - \c result = a new \c bitset of size \c ::nritems such that any bit \c i
 *     is set iff <tt>ritem[i]</tt> is a nonterminal after which all ritems in
 *     the same RHS are nullable nonterminals.  In other words, the follows of
 *     a goto on <tt>ritem[i]</tt> include the lookahead set of the item.
 */
நிலையான bitset
ielr_compute_ritem_sees_lookahead_set (வெறுமை)
{
  bitset result = bitset_create (nritems, BITSET_FIXED);
  முழு i = nritems-1;
  போது (0 < i)
    {
      --i;
      // Walk the RHS right to left, as long as it's symbol,
      // nonterminal, nullable.
      போது (!item_எண்_is_rule_எண் (ritem[i])
             && ISVAR (ritem[i])
             && nullable [item_எண்_as_குறியீடு_எண் (ritem[i]) - ntokens])
        bitset_set (result, i--);
      எனில் (!item_எண்_is_rule_எண் (ritem[i]) && ISVAR (ritem[i]))
        bitset_set (result, i--);
      // Flush the remainder of the RHS.
      போது (!item_எண்_is_rule_எண் (ritem[i]) && 0 < i)
        --i;
    }
  எனில் (trace_flag & trace_ielr)
    {
      fprintf (stderr, "ritem_sees_lookahead_set (indexes of ritems): ");
      bitset_dump (stderr, result);
      fprintf (stderr, "\n");
    }
  திரும்பு result;
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
நிலையான வெறுமை
ielr_compute_internal_follow_edges (bitset ritem_sees_lookahead_set,
                                    goto_எண் ***edgesp, முழு **edge_countsp)
{
  *edgesp = xnmalloc (ngotos, அளவு **edgesp);
  *edge_countsp = xnmalloc (ngotos, அளவு **edge_countsp);
  {
    bitset sources = bitset_create (ngotos, BITSET_FIXED);
    க்கு (goto_எண் i = 0; i < ngotos; ++i)
      (*edge_countsp)[i] = 0;
    க்கு (goto_எண் i = 0; i < ngotos; ++i)
      {
        முழு nsources = 0;
        {
          க்கு (rule **rulep = derives[states[to_state[i]]->accessing_குறியீடு
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
              எனில் (bitset_test (ritem_sees_lookahead_set,
                               (*rulep)->rhs - ritem))
                {
                  goto_எண் source =
                    map_goto (from_state[i],
                              item_எண்_as_குறியீடு_எண் (*(*rulep)->rhs));
                  எனில் (i != source && !bitset_test (sources, source))
                    {
                      bitset_set (sources, source);
                      ++nsources;
                      ++(*edge_countsp)[source];
                    }
                }
            }
        }
        எனில் (nsources == 0)
          (*edgesp)[i] = வெற்று;
        இல்லையெனில்
          {
            (*edgesp)[i] = xnmalloc (nsources + 1, அளவு *(*edgesp)[i]);
            {
              bitset_iterator biter_source;
              bitset_bindex source;
              முழு j = 0;
              BITSET_FOR_EACH (biter_source, sources, source, 0)
                (*edgesp)[i][j++] = source;
            }
            (*edgesp)[i][nsources] = END_முனை;
          }
        bitset_zero (sources);
      }
    bitset_free (sources);
  }

  relation_transpose (edgesp, ngotos);

  எனில் (trace_flag & trace_ielr)
    relation_print ("internal_follow_edges", *edgesp, ngotos, வெற்று, stderr);
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
நிலையான வெறுமை
ielr_compute_follow_kernel_பொருட்கள் (bitset ritem_sees_lookahead_set,
                                  goto_எண் **internal_follow_edges,
                                  bitsetv *follow_kernel_itemsp)
{
  {
    size_t max_nitems = 0;
    க்கு (state_எண் i = 0; i < nstates; ++i)
      எனில் (states[i]->nitems > max_nitems)
        max_nitems = states[i]->nitems;
    *follow_kernel_itemsp = bitsetv_create (ngotos, max_nitems, BITSET_FIXED);
  }
  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    {
      size_t nitems = states[from_state[i]]->nitems;
      item_index *பொருட்கள் = states[from_state[i]]->பொருட்கள்;
      க்கு (size_t j = 0; j < nitems; ++j)
        /* If this item has this goto and if all subsequent symbols in this
           RHS (if any) are nullable nonterminals, then record this item as
           one whose lookahead set is included in this goto's follows.  */
        எனில் (item_எண்_is_குறியீடு_எண் (ritem[பொருட்கள்[j]])
            && item_எண்_as_குறியீடு_எண் (ritem[பொருட்கள்[j]])
            == states[to_state[i]]->accessing_குறியீடு
            && bitset_test (ritem_sees_lookahead_set, பொருட்கள்[j]))
          bitset_set ((*follow_kernel_itemsp)[i], j);
    }
  relation_digraph (internal_follow_edges, ngotos, *follow_kernel_itemsp);

  எனில் (trace_flag & trace_ielr)
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
நிலையான வெறுமை
ielr_compute_always_follows (goto_எண் ***edgesp,
                             முழு மாறிலி edge_counts[],
                             bitsetv *always_followsp)
{
  *always_followsp = bitsetv_create (ngotos, ntokens, BITSET_FIXED);
  {
    goto_எண் *edge_array = xnmalloc (ngotos, அளவு *edge_array);
    க்கு (goto_எண் i = 0; i < ngotos; ++i)
      {
        goto_எண் nedges = edge_counts[i];
        {
          முழு j;
          transitions *trans = states[to_state[i]]->transitions;
          FOR_EACH_SHIFT (trans, j)
            bitset_set ((*always_followsp)[i], TRANSITION_குறியீடு (trans, j));
          க்கு (; j < trans->இலக்கம்; ++j)
            {
              குறியீடு_எண் sym = TRANSITION_குறியீடு (trans, j);
              எனில் (nullable[sym - ntokens])
                edge_array[nedges++] = map_goto (to_state[i], sym);
            }
        }
        எனில் (nedges - edge_counts[i])
          {
            (*edgesp)[i] =
              xnrealloc ((*edgesp)[i], nedges + 1, அளவு *(*edgesp)[i]);
            memcpy ((*edgesp)[i] + edge_counts[i], edge_array + edge_counts[i],
                    (nedges - edge_counts[i]) * அளவு *(*edgesp)[i]);
            (*edgesp)[i][nedges] = END_முனை;
          }
      }
    விடுவி (edge_array);
  }
  relation_digraph (*edgesp, ngotos, *always_followsp);

  எனில் (trace_flag & trace_ielr)
    {
      relation_print ("always follow edges", *edgesp, ngotos, வெற்று, stderr);
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
நிலையான state ***
ielr_compute_predecessors (வெறுமை)
{
  முழு *predecessor_counts = xnmalloc (nstates, அளவு *predecessor_counts);
  state ***res = xnmalloc (nstates, அளவு *res);
  க்கு (state_எண் i = 0; i < nstates; ++i)
    predecessor_counts[i] = 0;
  க்கு (state_எண் i = 0; i < nstates; ++i)
    க்கு (முழு j = 0; j < states[i]->transitions->இலக்கம்; ++j)
      ++predecessor_counts[states[i]->transitions->states[j]->எண்];
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      res[i] = xnmalloc (predecessor_counts[i]+1, அளவு *res[i]);
      res[i][predecessor_counts[i]] = வெற்று;
      predecessor_counts[i] = 0;
    }
  க்கு (state_எண் i = 0; i < nstates; ++i)
    க்கு (முழு j = 0; j < states[i]->transitions->இலக்கம்; ++j)
      {
        state_எண் k = states[i]->transitions->states[j]->எண்;
        res[k][predecessor_counts[k]++] = states[i];
      }
  விடுவி (predecessor_counts);
  திரும்பு res;
}

/**
 * \post
 *   - \c *follow_kernel_itemsp and \c *always_followsp were computed by
 *     \c ielr_compute_follow_kernel_items and
 *     \c ielr_compute_always_follows.
 *   - Iff <tt>predecessorsp != NULL</tt>, then \c *predecessorsp was computed
 *     by \c ielr_compute_predecessors.
 */
நிலையான வெறுமை
ielr_compute_auxiliary_tables (bitsetv *follow_kernel_itemsp,
                               bitsetv *always_followsp,
                               state ****predecessorsp)
{
  goto_எண் **edges;
  முழு *edge_counts;
  {
    bitset ritem_sees_lookahead_set = ielr_compute_ritem_sees_lookahead_set ();
    ielr_compute_internal_follow_edges (ritem_sees_lookahead_set,
                                        &edges, &edge_counts);
    ielr_compute_follow_kernel_பொருட்கள் (ritem_sees_lookahead_set, edges,
                                      follow_kernel_itemsp);
    bitset_free (ritem_sees_lookahead_set);
  }
  ielr_compute_always_follows (&edges, edge_counts, always_followsp);
  க்கு (முழு i = 0; i < ngotos; ++i)
    விடுவி (edges[i]);
  விடுவி (edges);
  விடுவி (edge_counts);
  எனில் (predecessorsp)
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
ielr_item_has_lookahead (state *s, குறியீடு_எண் lhs, size_t item,
                         குறியீடு_எண் lookahead, state ***predecessors,
                         bitset **item_lookahead_sets)
{
  எனில் (!item_lookahead_sets[s->எண்])
    {
      item_lookahead_sets[s->எண்] =
        xnmalloc (s->nitems, அளவு item_lookahead_sets[s->எண்][0]);
      க்கு (size_t i = 0; i < s->nitems; ++i)
        item_lookahead_sets[s->எண்][i] = வெற்று;
    }
  எனில் (!item_lookahead_sets[s->எண்][item])
    {
      item_lookahead_sets[s->எண்][item] =
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
      மாறிலி rule *r = item_rule (&ritem[s->பொருட்கள்[item]]);
      மாறிலி bool is_successor_of_initial_item
        = rule_is_initial (r) && &ritem[s->பொருட்கள்[item]] == r->rhs + 1;
      aver (!is_successor_of_initial_item);
      எனில் (!is_successor_of_initial_item)
        {
          /* If the LHS symbol of this item isn't known (because this is a
             top-level invocation), go get it.  */
          எனில் (!lhs)
            lhs = r->lhs->எண்;
          /* If this kernel item is next to the beginning of the RHS, then
             check all predecessors' goto follows for the LHS.  */
          எனில் (item_எண்_is_rule_எண் (ritem[s->பொருட்கள்[item] - 2]))
            {
              aver (lhs != acceptsymbol->content->எண்);
              க்கு (state **predecessor = predecessors[s->எண்];
                   *predecessor;
                   ++predecessor)
                bitset_or (item_lookahead_sets[s->எண்][item],
                           item_lookahead_sets[s->எண்][item],
                           goto_follows[map_goto ((*predecessor)->எண்,
                                                  lhs)]);
            }
          /* If this kernel item is later in the RHS, then check all
             predecessor items' lookahead sets.  */
          இல்லையெனில்
            {
              க்கு (state **predecessor = predecessors[s->எண்];
                   *predecessor;
                   ++predecessor)
                {
                  size_t predecessor_item;
                  க்கு (predecessor_item = 0;
                       predecessor_item < (*predecessor)->nitems;
                       ++predecessor_item)
                    எனில் ((*predecessor)->பொருட்கள்[predecessor_item]
                        == s->பொருட்கள்[item] - 1)
                      நிறுத்து;
                  aver (predecessor_item != (*predecessor)->nitems);
                  ielr_item_has_lookahead (*predecessor, lhs,
                                           predecessor_item, 0 /*irrelevant*/,
                                           predecessors, item_lookahead_sets);
                  bitset_or (item_lookahead_sets[s->எண்][item],
                             item_lookahead_sets[s->எண்][item],
                             item_lookahead_sets[(*predecessor)->எண்]
                                                [predecessor_item]);
                }
            }
        }
    }
  திரும்பு bitset_test (item_lookahead_sets[s->எண்][item], lookahead);
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
நிலையான வெறுமை
ielr_compute_annotation_lists (bitsetv follow_kernel_பொருட்கள்,
                               bitsetv always_follows, state ***predecessors,
                               AnnotationIndex *max_annotationsp,
                               InadequacyList ***inadequacy_listsp,
                               AnnotationList ***annotation_listsp,
                               கட்டமைப்பு obstack *annotations_obstackp)
{
  bitset **item_lookahead_sets =
    xnmalloc (nstates, அளவு *item_lookahead_sets);
  AnnotationIndex *annotation_counts =
    xnmalloc (nstates, அளவு *annotation_counts);
  ContributionIndex max_contributions = 0;
  முழு total_annotations = 0;

  *inadequacy_listsp = xnmalloc (nstates, அளவு **inadequacy_listsp);
  *annotation_listsp = xnmalloc (nstates, அளவு **annotation_listsp);
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      item_lookahead_sets[i] = வெற்று;
      (*inadequacy_listsp)[i] = வெற்று;
      (*annotation_listsp)[i] = வெற்று;
      annotation_counts[i] = 0;
    }
  {
    Inadequacy_List_முனை_மொத்தம் inadequacy_list_முனை_மொத்தம் = 0;
    க்கு (state_எண் i = 0; i < nstates; ++i)
      AnnotationList__compute_from_inadequacies (
        states[i], follow_kernel_பொருட்கள், always_follows, predecessors,
        item_lookahead_sets, *inadequacy_listsp, *annotation_listsp,
        annotation_counts, &max_contributions, annotations_obstackp,
        &inadequacy_list_முனை_மொத்தம்);
  }
  *max_annotationsp = 0;
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      எனில் (annotation_counts[i] > *max_annotationsp)
        *max_annotationsp = annotation_counts[i];
      total_annotations += annotation_counts[i];
    }
  எனில் (trace_flag & trace_ielr)
    {
      க்கு (state_எண் i = 0; i < nstates; ++i)
        {
          fprintf (stderr, "Inadequacy annotations for state %d:\n", i);
          AnnotationList__debug ((*annotation_listsp)[i],
                                 states[i]->nitems, 2);
        }
      fprintf (stderr, "Number of LR(0)/LALR(1) states: %d\n", nstates);
      fprintf (stderr, "Average number of annotations per state: %f\n",
               (தசம)total_annotations/nstates);
      fprintf (stderr, "Max number of annotations per state: %d\n",
               *max_annotationsp);
      fprintf (stderr, "Max number of contributions per annotation: %d\n",
               max_contributions);
    }
  க்கு (state_எண் i = 0; i < nstates; ++i)
    எனில் (item_lookahead_sets[i])
      {
        க்கு (size_t j = 0; j < states[i]->nitems; ++j)
          எனில் (item_lookahead_sets[i][j])
            bitset_free (item_lookahead_sets[i][j]);
        விடுவி (item_lookahead_sets[i]);
      }
  விடுவி (item_lookahead_sets);
  விடுவி (annotation_counts);
}

வகைவரை கட்டமைப்பு state_list
{
  கட்டமைப்பு state_list *next;
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
  கட்டமைப்பு state_list *lr0Isocore;
  கட்டமைப்பு state_list *nextIsocore;
} state_list;

MAYBE_UNUSED நிலையான வெறுமை
state_list_print_ (மாறிலி state_list *s, FILE *out, மாறிலி எழுத்து *sep)
{
  எனில் (s)
    {
      fprintf (out, "%s%d", sep, s->state->எண்);
      state_list_print_ (s->next, out, " ");
    }
}

MAYBE_UNUSED நிலையான வெறுமை
state_list_print (மாறிலி state_list *s, FILE *out)
{
  fprintf (out, "{");
  state_list_print_ (s, out, "");
  fprintf (out, "}");
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
நிலையான வெறுமை
ielr_compute_goto_follow_set (bitsetv follow_kernel_பொருட்கள்,
                              bitsetv always_follows, state_list *s,
                              sym_content *n, bitset follow_set)
{
  goto_எண் n_goto = map_goto (s->lr0Isocore->state->எண், n->எண்);
  bitset_copy (follow_set, always_follows[n_goto]);
  எனில் (s->lookaheads)
    {
      bitset_iterator biter_item;
      bitset_bindex item;
      BITSET_FOR_EACH (biter_item, follow_kernel_பொருட்கள்[n_goto], item, 0)
        எனில் (s->lookaheads[item])
          bitset_or (follow_set, follow_set, s->lookaheads[item]);
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
நிலையான வெறுமை
ielr_compute_lookaheads (bitsetv follow_kernel_பொருட்கள், bitsetv always_follows,
                         state_list *s, state *t, bitsetv lookahead_filter,
                         bitsetv lookaheads)
{
  size_t s_item = 0;
  bitsetv_zero (lookaheads);
  க்கு (size_t t_item = 0; t_item < t->nitems; ++t_item)
    {
      /* If this kernel item is the beginning of a RHS, it must be the
         kernel item in the start state, but t is supposed to be a successor
         state.  If, instead, this kernel item is the successor of the start
         state's kernel item, the lookahead set is empty.  This second case is
         a special case to avoid the - 2 below, but the next successor can be
         handled fine without special casing it.  */
      aver (t->பொருட்கள்[t_item] != 0);
      மாறிலி rule *r = item_rule (&ritem[t->பொருட்கள்[t_item]]);
      மாறிலி bool is_successor_of_initial_item
        = rule_is_initial (r) && &ritem[t->பொருட்கள்[t_item]] == r->rhs + 1;
      எனில் (!is_successor_of_initial_item
          && !bitset_empty_p (lookahead_filter[t_item]))
        {
          /* Is this kernel item next to the beginning of the RHS?  */
          எனில் (item_எண்_is_rule_எண் (ritem[t->பொருட்கள்[t_item] - 2]))
            ielr_compute_goto_follow_set (
              follow_kernel_பொருட்கள், always_follows, s,
              r->lhs,
              lookaheads[t_item]);
          இல்லையெனில் எனில் (s->lookaheads)
            {
              /* We don't have to start the s item search at the beginning
                 every time because items from both states are sorted by their
                 indices in ritem.  */
              க்கு (; s_item < s->state->nitems; ++s_item)
                எனில் (s->state->பொருட்கள்[s_item] == t->பொருட்கள்[t_item] - 1)
                  நிறுத்து;
              aver (s_item != s->state->nitems);
              எனில் (s->lookaheads[s_item])
                bitset_copy (lookaheads[t_item], s->lookaheads[s_item]);
            }
          bitset_and (lookaheads[t_item],
                      lookaheads[t_item], lookahead_filter[t_item]);
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
நிலையான வெறுமை
ielr_compute_state (bitsetv follow_kernel_பொருட்கள், bitsetv always_follows,
                    AnnotationList **annotation_lists, state *t,
                    bitsetv lookaheads, state_list **last_statep,
                    ContributionIndex work1[], bitsetv work2, state **tp)
{
  state_list *lr0_isocore = t->state_list->lr0Isocore;
  state_list **this_isocorep;

  /* Determine whether there's an isocore of t with which these lookaheads can
     be merged.  */
  {
    எனில் (annotation_lists)
      {
        AnnotationIndex ai;
        AnnotationList *a;
        க்கு (ai = 0, a = annotation_lists[lr0_isocore->state->எண்];
             a;
             ++ai, a = a->next)
        work1[ai] =
          AnnotationList__computeDominantContribution (
            a, lr0_isocore->state->nitems, lookaheads, false);
      }
    க்கு (this_isocorep = &t->state_list;
         this_isocorep == &t->state_list || *this_isocorep != t->state_list;
         this_isocorep = &(*this_isocorep)->nextIsocore)
      {
        எனில் (!(*this_isocorep)->recomputedAsSuccessor)
          நிறுத்து;
        எனில் (annotation_lists)
          {
            AnnotationIndex ai;
            AnnotationList *a;
            க்கு (ai = 0, a = annotation_lists[lr0_isocore->state->எண்];
                 a;
                 ++ai, a = a->next)
              {
                எனில் (work1[ai] != ContributionIndex__none)
                  {
                    /* This isocore compatibility test depends on the fact
                       that, if the dominant contributions are the same for the
                       two isocores, then merging their lookahead sets will not
                       produce a state with a different dominant contribution.
                       */
                    ContributionIndex ci =
                      AnnotationList__computeDominantContribution (
                        a, lr0_isocore->state->nitems,
                        (*this_isocorep)->lookaheads, false);
                    எனில் (ci != ContributionIndex__none && work1[ai] != ci)
                      நிறுத்து;
                  }
              }
            எனில் (!a)
              நிறுத்து;
          }
        இல்லையெனில்
          {
            size_t i;
            க்கு (i = 0; i < t->nitems; ++i)
              {
                எனில் (!(*this_isocorep)->lookaheads
                    || !(*this_isocorep)->lookaheads[i])
                  {
                    எனில் (!bitset_empty_p (lookaheads[i]))
                      நிறுத்து;
                  }
                /* bitset_equal_p uses the size of the first argument,
                   so lookaheads[i] must be the second argument.  */
                இல்லையெனில் எனில் (!bitset_equal_p ((*this_isocorep)->lookaheads[i],
                                          lookaheads[i]))
                  நிறுத்து;
              }
            எனில் (i == t->nitems)
              நிறுத்து;
          }
      }
  }

  bool has_lookaheads = false;
  க்கு (size_t i = 0; i < lr0_isocore->state->nitems; ++i)
    எனில் (!bitset_empty_p (lookaheads[i]))
      {
        has_lookaheads = true;
        நிறுத்து;
      }

  /* Merge with an existing isocore.  */
  எனில் (this_isocorep == &t->state_list || *this_isocorep != t->state_list)
    {
      bool new_lookaheads = false;
      *tp = (*this_isocorep)->state;

      /* Merge lookaheads into the state and record whether any of them are
         actually new.  */
      எனில் (has_lookaheads)
        {
          எனில் (!(*this_isocorep)->lookaheads)
            {
              (*this_isocorep)->lookaheads =
                xnmalloc (t->nitems, அளவு (*this_isocorep)->lookaheads);
              க்கு (size_t i = 0; i < t->nitems; ++i)
                (*this_isocorep)->lookaheads[i] = வெற்று;
            }
          க்கு (size_t i = 0; i < t->nitems; ++i)
            எனில் (!bitset_empty_p (lookaheads[i]))
              {
                எனில் (!(*this_isocorep)->lookaheads[i])
                  (*this_isocorep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                bitset_andn (lookaheads[i],
                             lookaheads[i], (*this_isocorep)->lookaheads[i]);
                bitset_or ((*this_isocorep)->lookaheads[i],
                           lookaheads[i], (*this_isocorep)->lookaheads[i]);
                எனில் (!bitset_empty_p (lookaheads[i]))
                  new_lookaheads = true;
              }
        }

      /* If new lookaheads were merged, propagate those lookaheads to the
         successors, possibly splitting them.  If *tp is being recomputed for
         the first time, this isn't necessary because the main
         ielr_split_states loop will handle the successors later.  */
      எனில் (!(*this_isocorep)->recomputedAsSuccessor)
        (*this_isocorep)->recomputedAsSuccessor = true;
      இல்லையெனில் எனில் (new_lookaheads)
        {
          /* When merging demands identical lookahead sets, it is impossible to
             merge new lookaheads.  */
          aver (annotation_lists);
          க்கு (முழு i = 0; i < (*tp)->transitions->இலக்கம்; ++i)
            {
              state *t2 = (*tp)->transitions->states[i];
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
              எனில் (!t2->state_list->recomputedAsSuccessor)
                நிறுத்து;
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t2->state_list->lr0Isocore->state->எண்],
                t2->nitems, work2);
              ielr_compute_lookaheads (follow_kernel_பொருட்கள், always_follows,
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
              ielr_compute_state (follow_kernel_பொருட்கள், always_follows,
                                  annotation_lists, t2, lookaheads,
                                  last_statep, work1, work2,
                                  &(*tp)->transitions->states[i]);
            }
        }
    }

  /* Create a new isocore.  */
  இல்லையெனில்
    {
      state_list *old_isocore = *this_isocorep;
      (*last_statep)->next = *this_isocorep = xmalloc (அளவு **last_statep);
      *last_statep = *this_isocorep;
      (*last_statep)->state = *tp = state_new_isocore (t);
      (*tp)->state_list = *last_statep;
      (*last_statep)->recomputedAsSuccessor = true;
      (*last_statep)->next = வெற்று;
      (*last_statep)->lookaheads = வெற்று;
      எனில் (has_lookaheads)
        {
          (*last_statep)->lookaheads =
            xnmalloc (t->nitems, அளவு (*last_statep)->lookaheads);
          க்கு (size_t i = 0; i < t->nitems; ++i)
            {
              எனில் (bitset_empty_p (lookaheads[i]))
                (*last_statep)->lookaheads[i] = வெற்று;
              இல்லையெனில்
                {
                  (*last_statep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                  bitset_copy ((*last_statep)->lookaheads[i], lookaheads[i]);
                }
            }
        }
      (*last_statep)->lr0Isocore = lr0_isocore;
      (*last_statep)->nextIsocore = old_isocore;
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
நிலையான வெறுமை
ielr_split_states (bitsetv follow_kernel_பொருட்கள், bitsetv always_follows,
                   AnnotationList **annotation_lists,
                   AnnotationIndex max_annotations)
{
  state_list *first_state;
  state_list *last_state;
  bitsetv lookahead_filter = வெற்று;
  bitsetv lookaheads;

  /* Set up state list and some reusable bitsets.  */
  {
    size_t max_nitems = 0;
    state_list **nodep = &first_state;
    க்கு (state_எண் i = 0; i < nstates; ++i)
      {
        *nodep = states[i]->state_list = last_state = xmalloc (அளவு **nodep);
        (*nodep)->state = states[i];
        (*nodep)->recomputedAsSuccessor = false;
        (*nodep)->lookaheads = வெற்று;
        (*nodep)->lr0Isocore = *nodep;
        (*nodep)->nextIsocore = *nodep;
        nodep = &(*nodep)->next;
        எனில் (states[i]->nitems > max_nitems)
          max_nitems = states[i]->nitems;
      }
    *nodep = வெற்று;
    lookahead_filter = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
    எனில் (!annotation_lists)
      bitsetv_ones (lookahead_filter);
    lookaheads = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
  }

  /* Recompute states.  */
  {
    ContributionIndex *work = xnmalloc (max_annotations, அளவு *work);
    க்கு (state_list *this_state = first_state;
         this_state;
         this_state = this_state->next)
      {
        மாறிலி state *s = this_state->state;
        க்கு (முழு i = 0; i < s->transitions->இலக்கம்; ++i)
          {
            state *t = s->transitions->states[i];
            எனில் (annotation_lists)
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t->state_list->lr0Isocore->state->எண்],
                t->nitems, lookahead_filter);
            ielr_compute_lookaheads (follow_kernel_பொருட்கள், always_follows,
                                     this_state, t, lookahead_filter,
                                     lookaheads);
            ielr_compute_state (follow_kernel_பொருட்கள், always_follows,
                                annotation_lists, t, lookaheads, &last_state,
                                work, lookahead_filter,
                                &s->transitions->states[i]);
          }
      }
    விடுவி (work);
  }

  bitsetv_free (lookahead_filter);
  bitsetv_free (lookaheads);

  /* Store states back in the states array.  */
  states = xnrealloc (states, nstates, அளவு *states);
  க்கு (state_list *முனை = first_state; முனை; முனை = முனை->next)
    states[முனை->state->எண்] = முனை->state;

  /* In the case of canonical LR(1), copy item lookahead sets to reduction
     lookahead sets.  */
  எனில் (!annotation_lists)
    {
      timevar_தள்ளு (tv_ielr_phase4);
      initialize_LA ();
      க்கு (state_list *முனை = first_state; முனை; முனை = முனை->next)
        எனில் (!முனை->state->consistent)
          {
            size_t i = 0;
            item_index *itemset = முனை->state->பொருட்கள்;
            க்கு (size_t r = 0; r < முனை->state->reductions->இலக்கம்; ++r)
              {
                rule *this_rule = முனை->state->reductions->rules[r];
                bitset lookahead_set =
                  முனை->state->reductions->lookaheads[r];
                எனில் (item_எண்_is_rule_எண் (*this_rule->rhs))
                  ielr_compute_goto_follow_set (follow_kernel_பொருட்கள்,
                                                always_follows, முனை,
                                                this_rule->lhs, lookahead_set);
                இல்லையெனில் எனில் (முனை->lookaheads)
                  {
                    /* We don't need to start the kernel item search back at
                       i=0 because both items and reductions are sorted on rule
                       number.  */
                    போது (!item_எண்_is_rule_எண் (ritem[itemset[i]])
                           || item_எண்_as_rule_எண் (ritem[itemset[i]])
                              != this_rule->எண்)
                      {
                        ++i;
                        aver (i < முனை->state->nitems);
                      }
                    எனில் (முனை->lookaheads[i])
                      bitset_copy (lookahead_set, முனை->lookaheads[i]);
                  }
              }
          }
      timevar_எடு (tv_ielr_phase4);
    }

  /* Free state list.  */
  போது (first_state)
    {
      state_list *முனை = first_state;
      எனில் (முனை->lookaheads)
        {
          க்கு (size_t i = 0; i < முனை->state->nitems; ++i)
            எனில் (முனை->lookaheads[i])
              bitset_free (முனை->lookaheads[i]);
          விடுவி (முனை->lookaheads);
        }
      first_state = முனை->next;
      விடுவி (முனை);
    }
}


வெறுமை
ielr (வெறுமை)
{
  LrType lr_type = lr_type_get ();

  /* Phase 0: LALR(1).  */
  தேர்வு (lr_type)
    {
    நிலை LR_TYPE__LR0:
      timevar_தள்ளு (tv_lalr);
      set_goto_map ();
      timevar_எடு (tv_lalr);
      திரும்பு;

    நிலை LR_TYPE__CANONICAL_LR:
      timevar_தள்ளு (tv_lalr);
      set_goto_map ();
      timevar_எடு (tv_lalr);
      நிறுத்து;

    நிலை LR_TYPE__LALR:
      timevar_தள்ளு (tv_lalr);
      lalr ();
      bitsetv_free (goto_follows);
      timevar_எடு (tv_lalr);
      திரும்பு;

    நிலை LR_TYPE__IELR:
      timevar_தள்ளு (tv_lalr);
      lalr ();
      timevar_எடு (tv_lalr);
      நிறுத்து;
    }

  {
    bitsetv follow_kernel_பொருட்கள்;
    bitsetv always_follows;
    InadequacyList **inadequacy_lists = வெற்று;
    AnnotationList **annotation_lists = வெற்று;
    கட்டமைப்பு obstack annotations_obstack;
    AnnotationIndex max_annotations = 0;

    {
      /* Phase 1: Compute Auxiliary Tables.  */
      state ***predecessors;
      timevar_தள்ளு (tv_ielr_phase1);
      ielr_compute_auxiliary_tables (
        &follow_kernel_பொருட்கள், &always_follows,
        lr_type == LR_TYPE__CANONICAL_LR ? வெற்று : &predecessors);
      timevar_எடு (tv_ielr_phase1);

      /* Phase 2: Compute Annotations.  */
      timevar_தள்ளு (tv_ielr_phase2);
      எனில் (lr_type != LR_TYPE__CANONICAL_LR)
        {
          obstack_init (&annotations_obstack);
          ielr_compute_annotation_lists (follow_kernel_பொருட்கள், always_follows,
                                         predecessors, &max_annotations,
                                         &inadequacy_lists, &annotation_lists,
                                         &annotations_obstack);
          க்கு (state_எண் i = 0; i < nstates; ++i)
            விடுவி (predecessors[i]);
          விடுவி (predecessors);
          bitsetv_free (goto_follows);
          lalr_free ();
        }
      timevar_எடு (tv_ielr_phase2);
    }

    /* Phase 3: Split States.  */
    timevar_தள்ளு (tv_ielr_phase3);
    {
      state_எண் nstates_lr0 = nstates;
      ielr_split_states (follow_kernel_பொருட்கள், always_follows,
                         annotation_lists, max_annotations);
      எனில் (inadequacy_lists)
        க்கு (state_எண் i = 0; i < nstates_lr0; ++i)
          InadequacyList__delete (inadequacy_lists[i]);
    }
    விடுவி (inadequacy_lists);
    எனில் (annotation_lists)
      obstack_free (&annotations_obstack, வெற்று);
    விடுவி (annotation_lists);
    bitsetv_free (follow_kernel_பொருட்கள்);
    bitsetv_free (always_follows);
    timevar_எடு (tv_ielr_phase3);
  }

  /* Phase 4: Compute Reduction Lookaheads.  */
  timevar_தள்ளு (tv_ielr_phase4);
  விடுவி (goto_map);
  விடுவி (from_state);
  விடுவி (to_state);
  எனில் (lr_type == LR_TYPE__CANONICAL_LR)
    {
      /* Reduction lookaheads are computed in ielr_split_states above
         but are timed as part of phase 4. */
      set_goto_map ();
    }
  இல்லையெனில்
    {
      lalr ();
      bitsetv_free (goto_follows);
    }
  timevar_எடு (tv_ielr_phase4);
}
