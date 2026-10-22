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

#شامل <config.h>

#شامل "ielr.h"

#شامل "system.h"

#شامل <bitset.h>
#شامل <timevar.h>

#شامل "AnnotationList.h"
#شامل "derives.h"
#شامل "getargs.h"
#شامل "lalr.h"
#شامل "muscle-tab.h"
#شامل "nullable.h"
#شامل "relation.h"
#شامل "state.h"
#شامل "symtab.h"

/** Records the value of the \%define variable lr.type.  */
تعریفنوع شمارش
  {
   LR_TYPE__LR0,
   LR_TYPE__LALR,
   LR_TYPE__IELR,
   LR_TYPE__CANONICAL_LR
  } LrType;

/* The user's requested LR type.  */
ایستا LrType
lr_type_get (تهی)
{
  نویسه *type = muscle_percent_define_get ("lr.type");
  LrType res;
  اگر (STREQ (type, "lr""(0)"))
    res = LR_TYPE__LR0;
  وگرنه اگر (STREQ (type, "lalr"))
    res = LR_TYPE__LALR;
  وگرنه اگر (STREQ (type, "ielr"))
    res = LR_TYPE__IELR;
  وگرنه اگر (STREQ (type, "canonical-lr"))
    res = LR_TYPE__CANONICAL_LR;
  وگرنه
    {
      aver (false);
      abort ();
    }
  آزاد (type);
  بازگشت res;
}

/**
 * \post:
 *   - \c result = a new \c bitset of size \c ::nritems such that any bit \c i
 *     is set iff <tt>ritem[i]</tt> is a nonterminal after which all ritems in
 *     the same RHS are nullable nonterminals.  In other words, the follows of
 *     a goto on <tt>ritem[i]</tt> include the lookahead set of the item.
 */
ایستا bitset
ielr_compute_ritem_sees_lookahead_set (تهی)
{
  bitset result = bitset_create (nritems, BITSET_FIXED);
  صحیح i = nritems-1;
  وقتی (0 < i)
    {
      --i;
      // Walk the RHS right to left, as long as it's symbol,
      // nonterminal, nullable.
      وقتی (!item_عدد_is_rule_عدد (ritem[i])
             && ISVAR (ritem[i])
             && nullable [item_عدد_as_نماد_عدد (ritem[i]) - ntokens])
        bitset_set (result, i--);
      اگر (!item_عدد_is_rule_عدد (ritem[i]) && ISVAR (ritem[i]))
        bitset_set (result, i--);
      // Flush the remainder of the RHS.
      وقتی (!item_عدد_is_rule_عدد (ritem[i]) && 0 < i)
        --i;
    }
  اگر (trace_flag & trace_ielr)
    {
      fprintf (stderr, "ritem_sees_lookahead_set (indexes of ritems): ");
      bitset_dump (stderr, result);
      fprintf (stderr, "\n");
    }
  بازگشت result;
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
ایستا تهی
ielr_compute_internal_follow_edges (bitset ritem_sees_lookahead_set,
                                    goto_عدد ***edgesp, صحیح **edge_countsp)
{
  *edgesp = xnmalloc (ngotos, اندازه **edgesp);
  *edge_countsp = xnmalloc (ngotos, اندازه **edge_countsp);
  {
    bitset sources = bitset_create (ngotos, BITSET_FIXED);
    برای (goto_عدد i = 0; i < ngotos; ++i)
      (*edge_countsp)[i] = 0;
    برای (goto_عدد i = 0; i < ngotos; ++i)
      {
        صحیح nsources = 0;
        {
          برای (rule **rulep = derives[states[to_state[i]]->accessing_نماد
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
              اگر (bitset_test (ritem_sees_lookahead_set,
                               (*rulep)->rhs - ritem))
                {
                  goto_عدد source =
                    map_goto (from_state[i],
                              item_عدد_as_نماد_عدد (*(*rulep)->rhs));
                  اگر (i != source && !bitset_test (sources, source))
                    {
                      bitset_set (sources, source);
                      ++nsources;
                      ++(*edge_countsp)[source];
                    }
                }
            }
        }
        اگر (nsources == 0)
          (*edgesp)[i] = پوچ;
        وگرنه
          {
            (*edgesp)[i] = xnmalloc (nsources + 1, اندازه *(*edgesp)[i]);
            {
              bitset_iterator biter_source;
              bitset_bindex source;
              صحیح j = 0;
              BITSET_FOR_EACH (biter_source, sources, source, 0)
                (*edgesp)[i][j++] = source;
            }
            (*edgesp)[i][nsources] = END_گره;
          }
        bitset_zero (sources);
      }
    bitset_free (sources);
  }

  relation_transpose (edgesp, ngotos);

  اگر (trace_flag & trace_ielr)
    relation_print ("internal_follow_edges", *edgesp, ngotos, پوچ, stderr);
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
ایستا تهی
ielr_compute_follow_kernel_اقلام (bitset ritem_sees_lookahead_set,
                                  goto_عدد **internal_follow_edges,
                                  bitsetv *follow_kernel_itemsp)
{
  {
    size_t max_nitems = 0;
    برای (state_عدد i = 0; i < nstates; ++i)
      اگر (states[i]->nitems > max_nitems)
        max_nitems = states[i]->nitems;
    *follow_kernel_itemsp = bitsetv_create (ngotos, max_nitems, BITSET_FIXED);
  }
  برای (goto_عدد i = 0; i < ngotos; ++i)
    {
      size_t nitems = states[from_state[i]]->nitems;
      item_index *اقلام = states[from_state[i]]->اقلام;
      برای (size_t j = 0; j < nitems; ++j)
        /* If this item has this goto and if all subsequent symbols in this
           RHS (if any) are nullable nonterminals, then record this item as
           one whose lookahead set is included in this goto's follows.  */
        اگر (item_عدد_is_نماد_عدد (ritem[اقلام[j]])
            && item_عدد_as_نماد_عدد (ritem[اقلام[j]])
            == states[to_state[i]]->accessing_نماد
            && bitset_test (ritem_sees_lookahead_set, اقلام[j]))
          bitset_set ((*follow_kernel_itemsp)[i], j);
    }
  relation_digraph (internal_follow_edges, ngotos, *follow_kernel_itemsp);

  اگر (trace_flag & trace_ielr)
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
ایستا تهی
ielr_compute_always_follows (goto_عدد ***edgesp,
                             صحیح ثابت edge_counts[],
                             bitsetv *always_followsp)
{
  *always_followsp = bitsetv_create (ngotos, ntokens, BITSET_FIXED);
  {
    goto_عدد *edge_array = xnmalloc (ngotos, اندازه *edge_array);
    برای (goto_عدد i = 0; i < ngotos; ++i)
      {
        goto_عدد nedges = edge_counts[i];
        {
          صحیح j;
          transitions *trans = states[to_state[i]]->transitions;
          FOR_EACH_SHIFT (trans, j)
            bitset_set ((*always_followsp)[i], TRANSITION_نماد (trans, j));
          برای (; j < trans->رقم; ++j)
            {
              نماد_عدد sym = TRANSITION_نماد (trans, j);
              اگر (nullable[sym - ntokens])
                edge_array[nedges++] = map_goto (to_state[i], sym);
            }
        }
        اگر (nedges - edge_counts[i])
          {
            (*edgesp)[i] =
              xnrealloc ((*edgesp)[i], nedges + 1, اندازه *(*edgesp)[i]);
            memcpy ((*edgesp)[i] + edge_counts[i], edge_array + edge_counts[i],
                    (nedges - edge_counts[i]) * اندازه *(*edgesp)[i]);
            (*edgesp)[i][nedges] = END_گره;
          }
      }
    آزاد (edge_array);
  }
  relation_digraph (*edgesp, ngotos, *always_followsp);

  اگر (trace_flag & trace_ielr)
    {
      relation_print ("always follow edges", *edgesp, ngotos, پوچ, stderr);
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
ایستا state ***
ielr_compute_predecessors (تهی)
{
  صحیح *predecessor_counts = xnmalloc (nstates, اندازه *predecessor_counts);
  state ***res = xnmalloc (nstates, اندازه *res);
  برای (state_عدد i = 0; i < nstates; ++i)
    predecessor_counts[i] = 0;
  برای (state_عدد i = 0; i < nstates; ++i)
    برای (صحیح j = 0; j < states[i]->transitions->رقم; ++j)
      ++predecessor_counts[states[i]->transitions->states[j]->عدد];
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      res[i] = xnmalloc (predecessor_counts[i]+1, اندازه *res[i]);
      res[i][predecessor_counts[i]] = پوچ;
      predecessor_counts[i] = 0;
    }
  برای (state_عدد i = 0; i < nstates; ++i)
    برای (صحیح j = 0; j < states[i]->transitions->رقم; ++j)
      {
        state_عدد k = states[i]->transitions->states[j]->عدد;
        res[k][predecessor_counts[k]++] = states[i];
      }
  آزاد (predecessor_counts);
  بازگشت res;
}

/**
 * \post
 *   - \c *follow_kernel_itemsp and \c *always_followsp were computed by
 *     \c ielr_compute_follow_kernel_items and
 *     \c ielr_compute_always_follows.
 *   - Iff <tt>predecessorsp != NULL</tt>, then \c *predecessorsp was computed
 *     by \c ielr_compute_predecessors.
 */
ایستا تهی
ielr_compute_auxiliary_tables (bitsetv *follow_kernel_itemsp,
                               bitsetv *always_followsp,
                               state ****predecessorsp)
{
  goto_عدد **edges;
  صحیح *edge_counts;
  {
    bitset ritem_sees_lookahead_set = ielr_compute_ritem_sees_lookahead_set ();
    ielr_compute_internal_follow_edges (ritem_sees_lookahead_set,
                                        &edges, &edge_counts);
    ielr_compute_follow_kernel_اقلام (ritem_sees_lookahead_set, edges,
                                      follow_kernel_itemsp);
    bitset_free (ritem_sees_lookahead_set);
  }
  ielr_compute_always_follows (&edges, edge_counts, always_followsp);
  برای (صحیح i = 0; i < ngotos; ++i)
    آزاد (edges[i]);
  آزاد (edges);
  آزاد (edge_counts);
  اگر (predecessorsp)
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
ielr_item_has_lookahead (state *s, نماد_عدد lhs, size_t item,
                         نماد_عدد lookahead, state ***predecessors,
                         bitset **item_lookahead_sets)
{
  اگر (!item_lookahead_sets[s->عدد])
    {
      item_lookahead_sets[s->عدد] =
        xnmalloc (s->nitems, اندازه item_lookahead_sets[s->عدد][0]);
      برای (size_t i = 0; i < s->nitems; ++i)
        item_lookahead_sets[s->عدد][i] = پوچ;
    }
  اگر (!item_lookahead_sets[s->عدد][item])
    {
      item_lookahead_sets[s->عدد][item] =
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
      ثابت rule *r = item_rule (&ritem[s->اقلام[item]]);
      ثابت bool is_successor_of_initial_item
        = rule_is_initial (r) && &ritem[s->اقلام[item]] == r->rhs + 1;
      aver (!is_successor_of_initial_item);
      اگر (!is_successor_of_initial_item)
        {
          /* If the LHS symbol of this item isn't known (because this is a
             top-level invocation), go get it.  */
          اگر (!lhs)
            lhs = r->lhs->عدد;
          /* If this kernel item is next to the beginning of the RHS, then
             check all predecessors' goto follows for the LHS.  */
          اگر (item_عدد_is_rule_عدد (ritem[s->اقلام[item] - 2]))
            {
              aver (lhs != acceptsymbol->content->عدد);
              برای (state **predecessor = predecessors[s->عدد];
                   *predecessor;
                   ++predecessor)
                bitset_or (item_lookahead_sets[s->عدد][item],
                           item_lookahead_sets[s->عدد][item],
                           goto_follows[map_goto ((*predecessor)->عدد,
                                                  lhs)]);
            }
          /* If this kernel item is later in the RHS, then check all
             predecessor items' lookahead sets.  */
          وگرنه
            {
              برای (state **predecessor = predecessors[s->عدد];
                   *predecessor;
                   ++predecessor)
                {
                  size_t predecessor_item;
                  برای (predecessor_item = 0;
                       predecessor_item < (*predecessor)->nitems;
                       ++predecessor_item)
                    اگر ((*predecessor)->اقلام[predecessor_item]
                        == s->اقلام[item] - 1)
                      بشکن;
                  aver (predecessor_item != (*predecessor)->nitems);
                  ielr_item_has_lookahead (*predecessor, lhs,
                                           predecessor_item, 0 /*irrelevant*/,
                                           predecessors, item_lookahead_sets);
                  bitset_or (item_lookahead_sets[s->عدد][item],
                             item_lookahead_sets[s->عدد][item],
                             item_lookahead_sets[(*predecessor)->عدد]
                                                [predecessor_item]);
                }
            }
        }
    }
  بازگشت bitset_test (item_lookahead_sets[s->عدد][item], lookahead);
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
ایستا تهی
ielr_compute_annotation_lists (bitsetv follow_kernel_اقلام,
                               bitsetv always_follows, state ***predecessors,
                               AnnotationIndex *max_annotationsp,
                               InadequacyList ***inadequacy_listsp,
                               AnnotationList ***annotation_listsp,
                               ساختار obstack *annotations_obstackp)
{
  bitset **item_lookahead_sets =
    xnmalloc (nstates, اندازه *item_lookahead_sets);
  AnnotationIndex *annotation_counts =
    xnmalloc (nstates, اندازه *annotation_counts);
  ContributionIndex max_contributions = 0;
  صحیح total_annotations = 0;

  *inadequacy_listsp = xnmalloc (nstates, اندازه **inadequacy_listsp);
  *annotation_listsp = xnmalloc (nstates, اندازه **annotation_listsp);
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      item_lookahead_sets[i] = پوچ;
      (*inadequacy_listsp)[i] = پوچ;
      (*annotation_listsp)[i] = پوچ;
      annotation_counts[i] = 0;
    }
  {
    Inadequacy_List_گره_شمار inadequacy_list_گره_شمار = 0;
    برای (state_عدد i = 0; i < nstates; ++i)
      AnnotationList__compute_from_inadequacies (
        states[i], follow_kernel_اقلام, always_follows, predecessors,
        item_lookahead_sets, *inadequacy_listsp, *annotation_listsp,
        annotation_counts, &max_contributions, annotations_obstackp,
        &inadequacy_list_گره_شمار);
  }
  *max_annotationsp = 0;
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      اگر (annotation_counts[i] > *max_annotationsp)
        *max_annotationsp = annotation_counts[i];
      total_annotations += annotation_counts[i];
    }
  اگر (trace_flag & trace_ielr)
    {
      برای (state_عدد i = 0; i < nstates; ++i)
        {
          fprintf (stderr, "Inadequacy annotations for state %d:\n", i);
          AnnotationList__debug ((*annotation_listsp)[i],
                                 states[i]->nitems, 2);
        }
      fprintf (stderr, "Number of LR(0)/LALR(1) states: %d\n", nstates);
      fprintf (stderr, "Average number of annotations per state: %f\n",
               (اعشاری)total_annotations/nstates);
      fprintf (stderr, "Max number of annotations per state: %d\n",
               *max_annotationsp);
      fprintf (stderr, "Max number of contributions per annotation: %d\n",
               max_contributions);
    }
  برای (state_عدد i = 0; i < nstates; ++i)
    اگر (item_lookahead_sets[i])
      {
        برای (size_t j = 0; j < states[i]->nitems; ++j)
          اگر (item_lookahead_sets[i][j])
            bitset_free (item_lookahead_sets[i][j]);
        آزاد (item_lookahead_sets[i]);
      }
  آزاد (item_lookahead_sets);
  آزاد (annotation_counts);
}

تعریفنوع ساختار state_list
{
  ساختار state_list *next;
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
  ساختار state_list *lr0Isocore;
  ساختار state_list *nextIsocore;
} state_list;

MAYBE_UNUSED ایستا تهی
state_list_print_ (ثابت state_list *s, FILE *out, ثابت نویسه *sep)
{
  اگر (s)
    {
      fprintf (out, "%s%d", sep, s->state->عدد);
      state_list_print_ (s->next, out, " ");
    }
}

MAYBE_UNUSED ایستا تهی
state_list_print (ثابت state_list *s, FILE *out)
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
ایستا تهی
ielr_compute_goto_follow_set (bitsetv follow_kernel_اقلام,
                              bitsetv always_follows, state_list *s,
                              sym_content *n, bitset follow_set)
{
  goto_عدد n_goto = map_goto (s->lr0Isocore->state->عدد, n->عدد);
  bitset_copy (follow_set, always_follows[n_goto]);
  اگر (s->lookaheads)
    {
      bitset_iterator biter_item;
      bitset_bindex item;
      BITSET_FOR_EACH (biter_item, follow_kernel_اقلام[n_goto], item, 0)
        اگر (s->lookaheads[item])
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
ایستا تهی
ielr_compute_lookaheads (bitsetv follow_kernel_اقلام, bitsetv always_follows,
                         state_list *s, state *t, bitsetv lookahead_filter,
                         bitsetv lookaheads)
{
  size_t s_item = 0;
  bitsetv_zero (lookaheads);
  برای (size_t t_item = 0; t_item < t->nitems; ++t_item)
    {
      /* If this kernel item is the beginning of a RHS, it must be the
         kernel item in the start state, but t is supposed to be a successor
         state.  If, instead, this kernel item is the successor of the start
         state's kernel item, the lookahead set is empty.  This second case is
         a special case to avoid the - 2 below, but the next successor can be
         handled fine without special casing it.  */
      aver (t->اقلام[t_item] != 0);
      ثابت rule *r = item_rule (&ritem[t->اقلام[t_item]]);
      ثابت bool is_successor_of_initial_item
        = rule_is_initial (r) && &ritem[t->اقلام[t_item]] == r->rhs + 1;
      اگر (!is_successor_of_initial_item
          && !bitset_empty_p (lookahead_filter[t_item]))
        {
          /* Is this kernel item next to the beginning of the RHS?  */
          اگر (item_عدد_is_rule_عدد (ritem[t->اقلام[t_item] - 2]))
            ielr_compute_goto_follow_set (
              follow_kernel_اقلام, always_follows, s,
              r->lhs,
              lookaheads[t_item]);
          وگرنه اگر (s->lookaheads)
            {
              /* We don't have to start the s item search at the beginning
                 every time because items from both states are sorted by their
                 indices in ritem.  */
              برای (; s_item < s->state->nitems; ++s_item)
                اگر (s->state->اقلام[s_item] == t->اقلام[t_item] - 1)
                  بشکن;
              aver (s_item != s->state->nitems);
              اگر (s->lookaheads[s_item])
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
ایستا تهی
ielr_compute_state (bitsetv follow_kernel_اقلام, bitsetv always_follows,
                    AnnotationList **annotation_lists, state *t,
                    bitsetv lookaheads, state_list **last_statep,
                    ContributionIndex work1[], bitsetv work2, state **tp)
{
  state_list *lr0_isocore = t->state_list->lr0Isocore;
  state_list **this_isocorep;

  /* Determine whether there's an isocore of t with which these lookaheads can
     be merged.  */
  {
    اگر (annotation_lists)
      {
        AnnotationIndex ai;
        AnnotationList *a;
        برای (ai = 0, a = annotation_lists[lr0_isocore->state->عدد];
             a;
             ++ai, a = a->next)
        work1[ai] =
          AnnotationList__computeDominantContribution (
            a, lr0_isocore->state->nitems, lookaheads, false);
      }
    برای (this_isocorep = &t->state_list;
         this_isocorep == &t->state_list || *this_isocorep != t->state_list;
         this_isocorep = &(*this_isocorep)->nextIsocore)
      {
        اگر (!(*this_isocorep)->recomputedAsSuccessor)
          بشکن;
        اگر (annotation_lists)
          {
            AnnotationIndex ai;
            AnnotationList *a;
            برای (ai = 0, a = annotation_lists[lr0_isocore->state->عدد];
                 a;
                 ++ai, a = a->next)
              {
                اگر (work1[ai] != ContributionIndex__none)
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
                    اگر (ci != ContributionIndex__none && work1[ai] != ci)
                      بشکن;
                  }
              }
            اگر (!a)
              بشکن;
          }
        وگرنه
          {
            size_t i;
            برای (i = 0; i < t->nitems; ++i)
              {
                اگر (!(*this_isocorep)->lookaheads
                    || !(*this_isocorep)->lookaheads[i])
                  {
                    اگر (!bitset_empty_p (lookaheads[i]))
                      بشکن;
                  }
                /* bitset_equal_p uses the size of the first argument,
                   so lookaheads[i] must be the second argument.  */
                وگرنه اگر (!bitset_equal_p ((*this_isocorep)->lookaheads[i],
                                          lookaheads[i]))
                  بشکن;
              }
            اگر (i == t->nitems)
              بشکن;
          }
      }
  }

  bool has_lookaheads = false;
  برای (size_t i = 0; i < lr0_isocore->state->nitems; ++i)
    اگر (!bitset_empty_p (lookaheads[i]))
      {
        has_lookaheads = true;
        بشکن;
      }

  /* Merge with an existing isocore.  */
  اگر (this_isocorep == &t->state_list || *this_isocorep != t->state_list)
    {
      bool new_lookaheads = false;
      *tp = (*this_isocorep)->state;

      /* Merge lookaheads into the state and record whether any of them are
         actually new.  */
      اگر (has_lookaheads)
        {
          اگر (!(*this_isocorep)->lookaheads)
            {
              (*this_isocorep)->lookaheads =
                xnmalloc (t->nitems, اندازه (*this_isocorep)->lookaheads);
              برای (size_t i = 0; i < t->nitems; ++i)
                (*this_isocorep)->lookaheads[i] = پوچ;
            }
          برای (size_t i = 0; i < t->nitems; ++i)
            اگر (!bitset_empty_p (lookaheads[i]))
              {
                اگر (!(*this_isocorep)->lookaheads[i])
                  (*this_isocorep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                bitset_andn (lookaheads[i],
                             lookaheads[i], (*this_isocorep)->lookaheads[i]);
                bitset_or ((*this_isocorep)->lookaheads[i],
                           lookaheads[i], (*this_isocorep)->lookaheads[i]);
                اگر (!bitset_empty_p (lookaheads[i]))
                  new_lookaheads = true;
              }
        }

      /* If new lookaheads were merged, propagate those lookaheads to the
         successors, possibly splitting them.  If *tp is being recomputed for
         the first time, this isn't necessary because the main
         ielr_split_states loop will handle the successors later.  */
      اگر (!(*this_isocorep)->recomputedAsSuccessor)
        (*this_isocorep)->recomputedAsSuccessor = true;
      وگرنه اگر (new_lookaheads)
        {
          /* When merging demands identical lookahead sets, it is impossible to
             merge new lookaheads.  */
          aver (annotation_lists);
          برای (صحیح i = 0; i < (*tp)->transitions->رقم; ++i)
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
              اگر (!t2->state_list->recomputedAsSuccessor)
                بشکن;
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t2->state_list->lr0Isocore->state->عدد],
                t2->nitems, work2);
              ielr_compute_lookaheads (follow_kernel_اقلام, always_follows,
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
              ielr_compute_state (follow_kernel_اقلام, always_follows,
                                  annotation_lists, t2, lookaheads,
                                  last_statep, work1, work2,
                                  &(*tp)->transitions->states[i]);
            }
        }
    }

  /* Create a new isocore.  */
  وگرنه
    {
      state_list *old_isocore = *this_isocorep;
      (*last_statep)->next = *this_isocorep = xmalloc (اندازه **last_statep);
      *last_statep = *this_isocorep;
      (*last_statep)->state = *tp = state_new_isocore (t);
      (*tp)->state_list = *last_statep;
      (*last_statep)->recomputedAsSuccessor = true;
      (*last_statep)->next = پوچ;
      (*last_statep)->lookaheads = پوچ;
      اگر (has_lookaheads)
        {
          (*last_statep)->lookaheads =
            xnmalloc (t->nitems, اندازه (*last_statep)->lookaheads);
          برای (size_t i = 0; i < t->nitems; ++i)
            {
              اگر (bitset_empty_p (lookaheads[i]))
                (*last_statep)->lookaheads[i] = پوچ;
              وگرنه
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
ایستا تهی
ielr_split_states (bitsetv follow_kernel_اقلام, bitsetv always_follows,
                   AnnotationList **annotation_lists,
                   AnnotationIndex max_annotations)
{
  state_list *first_state;
  state_list *last_state;
  bitsetv lookahead_filter = پوچ;
  bitsetv lookaheads;

  /* Set up state list and some reusable bitsets.  */
  {
    size_t max_nitems = 0;
    state_list **nodep = &first_state;
    برای (state_عدد i = 0; i < nstates; ++i)
      {
        *nodep = states[i]->state_list = last_state = xmalloc (اندازه **nodep);
        (*nodep)->state = states[i];
        (*nodep)->recomputedAsSuccessor = false;
        (*nodep)->lookaheads = پوچ;
        (*nodep)->lr0Isocore = *nodep;
        (*nodep)->nextIsocore = *nodep;
        nodep = &(*nodep)->next;
        اگر (states[i]->nitems > max_nitems)
          max_nitems = states[i]->nitems;
      }
    *nodep = پوچ;
    lookahead_filter = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
    اگر (!annotation_lists)
      bitsetv_ones (lookahead_filter);
    lookaheads = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
  }

  /* Recompute states.  */
  {
    ContributionIndex *work = xnmalloc (max_annotations, اندازه *work);
    برای (state_list *this_state = first_state;
         this_state;
         this_state = this_state->next)
      {
        ثابت state *s = this_state->state;
        برای (صحیح i = 0; i < s->transitions->رقم; ++i)
          {
            state *t = s->transitions->states[i];
            اگر (annotation_lists)
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t->state_list->lr0Isocore->state->عدد],
                t->nitems, lookahead_filter);
            ielr_compute_lookaheads (follow_kernel_اقلام, always_follows,
                                     this_state, t, lookahead_filter,
                                     lookaheads);
            ielr_compute_state (follow_kernel_اقلام, always_follows,
                                annotation_lists, t, lookaheads, &last_state,
                                work, lookahead_filter,
                                &s->transitions->states[i]);
          }
      }
    آزاد (work);
  }

  bitsetv_free (lookahead_filter);
  bitsetv_free (lookaheads);

  /* Store states back in the states array.  */
  states = xnrealloc (states, nstates, اندازه *states);
  برای (state_list *گره = first_state; گره; گره = گره->next)
    states[گره->state->عدد] = گره->state;

  /* In the case of canonical LR(1), copy item lookahead sets to reduction
     lookahead sets.  */
  اگر (!annotation_lists)
    {
      timevar_افزودن (tv_ielr_phase4);
      initialize_LA ();
      برای (state_list *گره = first_state; گره; گره = گره->next)
        اگر (!گره->state->consistent)
          {
            size_t i = 0;
            item_index *itemset = گره->state->اقلام;
            برای (size_t r = 0; r < گره->state->reductions->رقم; ++r)
              {
                rule *this_rule = گره->state->reductions->rules[r];
                bitset lookahead_set =
                  گره->state->reductions->lookaheads[r];
                اگر (item_عدد_is_rule_عدد (*this_rule->rhs))
                  ielr_compute_goto_follow_set (follow_kernel_اقلام,
                                                always_follows, گره,
                                                this_rule->lhs, lookahead_set);
                وگرنه اگر (گره->lookaheads)
                  {
                    /* We don't need to start the kernel item search back at
                       i=0 because both items and reductions are sorted on rule
                       number.  */
                    وقتی (!item_عدد_is_rule_عدد (ritem[itemset[i]])
                           || item_عدد_as_rule_عدد (ritem[itemset[i]])
                              != this_rule->عدد)
                      {
                        ++i;
                        aver (i < گره->state->nitems);
                      }
                    اگر (گره->lookaheads[i])
                      bitset_copy (lookahead_set, گره->lookaheads[i]);
                  }
              }
          }
      timevar_برداشت (tv_ielr_phase4);
    }

  /* Free state list.  */
  وقتی (first_state)
    {
      state_list *گره = first_state;
      اگر (گره->lookaheads)
        {
          برای (size_t i = 0; i < گره->state->nitems; ++i)
            اگر (گره->lookaheads[i])
              bitset_free (گره->lookaheads[i]);
          آزاد (گره->lookaheads);
        }
      first_state = گره->next;
      آزاد (گره);
    }
}


تهی
ielr (تهی)
{
  LrType lr_type = lr_type_get ();

  /* Phase 0: LALR(1).  */
  انتخاب (lr_type)
    {
    حالت LR_TYPE__LR0:
      timevar_افزودن (tv_lalr);
      set_goto_map ();
      timevar_برداشت (tv_lalr);
      بازگشت;

    حالت LR_TYPE__CANONICAL_LR:
      timevar_افزودن (tv_lalr);
      set_goto_map ();
      timevar_برداشت (tv_lalr);
      بشکن;

    حالت LR_TYPE__LALR:
      timevar_افزودن (tv_lalr);
      lalr ();
      bitsetv_free (goto_follows);
      timevar_برداشت (tv_lalr);
      بازگشت;

    حالت LR_TYPE__IELR:
      timevar_افزودن (tv_lalr);
      lalr ();
      timevar_برداشت (tv_lalr);
      بشکن;
    }

  {
    bitsetv follow_kernel_اقلام;
    bitsetv always_follows;
    InadequacyList **inadequacy_lists = پوچ;
    AnnotationList **annotation_lists = پوچ;
    ساختار obstack annotations_obstack;
    AnnotationIndex max_annotations = 0;

    {
      /* Phase 1: Compute Auxiliary Tables.  */
      state ***predecessors;
      timevar_افزودن (tv_ielr_phase1);
      ielr_compute_auxiliary_tables (
        &follow_kernel_اقلام, &always_follows,
        lr_type == LR_TYPE__CANONICAL_LR ? پوچ : &predecessors);
      timevar_برداشت (tv_ielr_phase1);

      /* Phase 2: Compute Annotations.  */
      timevar_افزودن (tv_ielr_phase2);
      اگر (lr_type != LR_TYPE__CANONICAL_LR)
        {
          obstack_init (&annotations_obstack);
          ielr_compute_annotation_lists (follow_kernel_اقلام, always_follows,
                                         predecessors, &max_annotations,
                                         &inadequacy_lists, &annotation_lists,
                                         &annotations_obstack);
          برای (state_عدد i = 0; i < nstates; ++i)
            آزاد (predecessors[i]);
          آزاد (predecessors);
          bitsetv_free (goto_follows);
          lalr_free ();
        }
      timevar_برداشت (tv_ielr_phase2);
    }

    /* Phase 3: Split States.  */
    timevar_افزودن (tv_ielr_phase3);
    {
      state_عدد nstates_lr0 = nstates;
      ielr_split_states (follow_kernel_اقلام, always_follows,
                         annotation_lists, max_annotations);
      اگر (inadequacy_lists)
        برای (state_عدد i = 0; i < nstates_lr0; ++i)
          InadequacyList__delete (inadequacy_lists[i]);
    }
    آزاد (inadequacy_lists);
    اگر (annotation_lists)
      obstack_free (&annotations_obstack, پوچ);
    آزاد (annotation_lists);
    bitsetv_free (follow_kernel_اقلام);
    bitsetv_free (always_follows);
    timevar_برداشت (tv_ielr_phase3);
  }

  /* Phase 4: Compute Reduction Lookaheads.  */
  timevar_افزودن (tv_ielr_phase4);
  آزاد (goto_map);
  آزاد (from_state);
  آزاد (to_state);
  اگر (lr_type == LR_TYPE__CANONICAL_LR)
    {
      /* Reduction lookaheads are computed in ielr_split_states above
         but are timed as part of phase 4. */
      set_goto_map ();
    }
  وگرنه
    {
      lalr ();
      bitsetv_free (goto_follows);
    }
  timevar_برداشت (tv_ielr_phase4);
}
