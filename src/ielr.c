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

#ضمن <config.h>

#ضمن "ielr.h"

#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <timevar.h>

#ضمن "AnnotationList.h"
#ضمن "derives.h"
#ضمن "getargs.h"
#ضمن "lalr.h"
#ضمن "muscle-tab.h"
#ضمن "nullable.h"
#ضمن "relation.h"
#ضمن "state.h"
#ضمن "symtab.h"

/** Records the value of the \%define variable lr.type.  */
تعريفنوع تعداد
  {
   LR_TYPE__LR0,
   LR_TYPE__LALR,
   LR_TYPE__IELR,
   LR_TYPE__CANONICAL_LR
  } LrType;

/* The user's requested LR type.  */
ساكن LrType
lr_type_get (فراغ)
{
  حرف *type = muscle_percent_define_get ("lr.type");
  LrType res;
  إذا (STREQ (type, "lr""(0)"))
    res = LR_TYPE__LR0;
  وإلا إذا (STREQ (type, "lalr"))
    res = LR_TYPE__LALR;
  وإلا إذا (STREQ (type, "ielr"))
    res = LR_TYPE__IELR;
  وإلا إذا (STREQ (type, "canonical-lr"))
    res = LR_TYPE__CANONICAL_LR;
  وإلا
    {
      aver (false);
      abort ();
    }
  حرر (type);
  أرجع res;
}

/**
 * \post:
 *   - \c result = a new \c bitset of size \c ::nritems such that any bit \c i
 *     is set iff <tt>ritem[i]</tt> is a nonterminal after which all ritems in
 *     the same RHS are nullable nonterminals.  In other words, the follows of
 *     a goto on <tt>ritem[i]</tt> include the lookahead set of the item.
 */
ساكن bitset
ielr_compute_ritem_sees_lookahead_سیٹ (فراغ)
{
  bitset نتیجہ = bitset_create (nritems, BITSET_FIXED);
  صحيح i = nritems-1;
  طالما (0 < i)
    {
      --i;
      // Walk the RHS right to left, as long as it's symbol,
      // nonterminal, nullable.
      طالما (!شے_نمبر_is_قاعدہ_نمبر (ritem[i])
             && ISVAR (ritem[i])
             && nullable [شے_نمبر_as_علامت_نمبر (ritem[i]) - ntokens])
        bitset_سیٹ (نتیجہ, i--);
      إذا (!شے_نمبر_is_قاعدہ_نمبر (ritem[i]) && ISVAR (ritem[i]))
        bitset_سیٹ (نتیجہ, i--);
      // Flush the remainder of the RHS.
      طالما (!شے_نمبر_is_قاعدہ_نمبر (ritem[i]) && 0 < i)
        --i;
    }
  إذا (trace_flag & trace_ielr)
    {
      fprintf (stderr, "ritem_sees_lookahead_set (indexes of ritems): ");
      bitset_dump (stderr, نتیجہ);
      fprintf (stderr, "\n");
    }
  أرجع نتیجہ;
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
ساكن فراغ
ielr_compute_اندرونی_follow_کنارے (bitset ritem_sees_lookahead_سیٹ,
                                    goto_نمبر ***edgesp, صحيح **کنارا_countsp)
{
  *edgesp = xnmalloc (ngotos, حجم **edgesp);
  *کنارا_countsp = xnmalloc (ngotos, حجم **کنارا_countsp);
  {
    bitset sources = bitset_create (ngotos, BITSET_FIXED);
    لكل (goto_نمبر i = 0; i < ngotos; ++i)
      (*کنارا_countsp)[i] = 0;
    لكل (goto_نمبر i = 0; i < ngotos; ++i)
      {
        صحيح nsources = 0;
        {
          لكل (قاعدہ **rulep = derives[حالتاں[منزل_state[i]]->accessing_علامت
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
              إذا (bitset_test (ritem_sees_lookahead_سیٹ,
                               (*rulep)->rhs - ritem))
                {
                  goto_نمبر سروت =
                    map_goto (from_state[i],
                              شے_نمبر_as_علامت_نمبر (*(*rulep)->rhs));
                  إذا (i != سروت && !bitset_test (sources, سروت))
                    {
                      bitset_سیٹ (sources, سروت);
                      ++nsources;
                      ++(*کنارا_countsp)[سروت];
                    }
                }
            }
        }
        إذا (nsources == 0)
          (*edgesp)[i] = عدم;
        وإلا
          {
            (*edgesp)[i] = xnmalloc (nsources + 1, حجم *(*edgesp)[i]);
            {
              bitset_iterator biter_سروت;
              bitset_bindex سروت;
              صحيح j = 0;
              BITSET_FOR_EACH (biter_سروت, sources, سروت, 0)
                (*edgesp)[i][j++] = سروت;
            }
            (*edgesp)[i][nsources] = سرا_گرہ;
          }
        bitset_zero (sources);
      }
    bitset_free (sources);
  }

  سمبندھ_transpose (edgesp, ngotos);

  إذا (trace_flag & trace_ielr)
    سمبندھ_print ("internal_follow_edges", *edgesp, ngotos, عدم, stderr);
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
ساكن فراغ
ielr_compute_follow_kernel_اشیاء (bitset ritem_sees_lookahead_سیٹ,
                                  goto_نمبر **اندرونی_follow_کنارے,
                                  bitsetv *follow_kernel_itemsp)
{
  {
    size_t max_nitems = 0;
    لكل (state_نمبر i = 0; i < nstates; ++i)
      إذا (حالتاں[i]->nitems > max_nitems)
        max_nitems = حالتاں[i]->nitems;
    *follow_kernel_itemsp = bitsetv_create (ngotos, max_nitems, BITSET_FIXED);
  }
  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    {
      size_t nitems = حالتاں[from_state[i]]->nitems;
      شے_اشاریہ *اشیاء = حالتاں[from_state[i]]->اشیاء;
      لكل (size_t j = 0; j < nitems; ++j)
        /* If this item has this goto and if all subsequent symbols in this
           RHS (if any) are nullable nonterminals, then record this item as
           one whose lookahead set is included in this goto's follows.  */
        إذا (شے_نمبر_is_علامت_نمبر (ritem[اشیاء[j]])
            && شے_نمبر_as_علامت_نمبر (ritem[اشیاء[j]])
            == حالتاں[منزل_state[i]]->accessing_علامت
            && bitset_test (ritem_sees_lookahead_سیٹ, اشیاء[j]))
          bitset_سیٹ ((*follow_kernel_itemsp)[i], j);
    }
  سمبندھ_digraph (اندرونی_follow_کنارے, ngotos, *follow_kernel_itemsp);

  إذا (trace_flag & trace_ielr)
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
ساكن فراغ
ielr_compute_always_follows (goto_نمبر ***edgesp,
                             صحيح ثابت کنارا_counts[],
                             bitsetv *always_followsp)
{
  *always_followsp = bitsetv_create (ngotos, ntokens, BITSET_FIXED);
  {
    goto_نمبر *کنارا_array = xnmalloc (ngotos, حجم *کنارا_array);
    لكل (goto_نمبر i = 0; i < ngotos; ++i)
      {
        goto_نمبر nedges = کنارا_counts[i];
        {
          صحيح j;
          transitions *trans = حالتاں[منزل_state[i]]->transitions;
          FOR_EACH_کھسکاؤ (trans, j)
            bitset_سیٹ ((*always_followsp)[i], TRANSITION_علامت (trans, j));
          لكل (; j < trans->عدد; ++j)
            {
              علامت_نمبر sym = TRANSITION_علامت (trans, j);
              إذا (nullable[sym - ntokens])
                کنارا_array[nedges++] = map_goto (منزل_state[i], sym);
            }
        }
        إذا (nedges - کنارا_counts[i])
          {
            (*edgesp)[i] =
              xnrealloc ((*edgesp)[i], nedges + 1, حجم *(*edgesp)[i]);
            حافظہ_نقل ((*edgesp)[i] + کنارا_counts[i], کنارا_array + کنارا_counts[i],
                    (nedges - کنارا_counts[i]) * حجم *(*edgesp)[i]);
            (*edgesp)[i][nedges] = سرا_گرہ;
          }
      }
    حرر (کنارا_array);
  }
  سمبندھ_digraph (*edgesp, ngotos, *always_followsp);

  إذا (trace_flag & trace_ielr)
    {
      سمبندھ_print ("always follow edges", *edgesp, ngotos, عدم, stderr);
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
ساكن state ***
ielr_compute_predecessors (فراغ)
{
  صحيح *predecessor_counts = xnmalloc (nstates, حجم *predecessor_counts);
  state ***res = xnmalloc (nstates, حجم *res);
  لكل (state_نمبر i = 0; i < nstates; ++i)
    predecessor_counts[i] = 0;
  لكل (state_نمبر i = 0; i < nstates; ++i)
    لكل (صحيح j = 0; j < حالتاں[i]->transitions->عدد; ++j)
      ++predecessor_counts[حالتاں[i]->transitions->حالتاں[j]->نمبر];
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      res[i] = xnmalloc (predecessor_counts[i]+1, حجم *res[i]);
      res[i][predecessor_counts[i]] = عدم;
      predecessor_counts[i] = 0;
    }
  لكل (state_نمبر i = 0; i < nstates; ++i)
    لكل (صحيح j = 0; j < حالتاں[i]->transitions->عدد; ++j)
      {
        state_نمبر k = حالتاں[i]->transitions->حالتاں[j]->نمبر;
        res[k][predecessor_counts[k]++] = حالتاں[i];
      }
  حرر (predecessor_counts);
  أرجع res;
}

/**
 * \post
 *   - \c *follow_kernel_itemsp and \c *always_followsp were computed by
 *     \c ielr_compute_follow_kernel_items and
 *     \c ielr_compute_always_follows.
 *   - Iff <tt>predecessorsp != NULL</tt>, then \c *predecessorsp was computed
 *     by \c ielr_compute_predecessors.
 */
ساكن فراغ
ielr_compute_auxiliary_tables (bitsetv *follow_kernel_itemsp,
                               bitsetv *always_followsp,
                               state ****predecessorsp)
{
  goto_نمبر **کنارے;
  صحيح *کنارا_counts;
  {
    bitset ritem_sees_lookahead_سیٹ = ielr_compute_ritem_sees_lookahead_سیٹ ();
    ielr_compute_اندرونی_follow_کنارے (ritem_sees_lookahead_سیٹ,
                                        &کنارے, &کنارا_counts);
    ielr_compute_follow_kernel_اشیاء (ritem_sees_lookahead_سیٹ, کنارے,
                                      follow_kernel_itemsp);
    bitset_free (ritem_sees_lookahead_سیٹ);
  }
  ielr_compute_always_follows (&کنارے, کنارا_counts, always_followsp);
  لكل (صحيح i = 0; i < ngotos; ++i)
    حرر (کنارے[i]);
  حرر (کنارے);
  حرر (کنارا_counts);
  إذا (predecessorsp)
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
ielr_شے_has_lookahead (state *s, علامت_نمبر lhs, size_t شے,
                         علامت_نمبر lookahead, state ***predecessors,
                         bitset **شے_lookahead_sets)
{
  إذا (!شے_lookahead_sets[s->نمبر])
    {
      شے_lookahead_sets[s->نمبر] =
        xnmalloc (s->nitems, حجم شے_lookahead_sets[s->نمبر][0]);
      لكل (size_t i = 0; i < s->nitems; ++i)
        شے_lookahead_sets[s->نمبر][i] = عدم;
    }
  إذا (!شے_lookahead_sets[s->نمبر][شے])
    {
      شے_lookahead_sets[s->نمبر][شے] =
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
      ثابت قاعدہ *r = شے_قاعدہ (&ritem[s->اشیاء[شے]]);
      ثابت bool is_successor_دا_initial_شے
        = قاعدہ_is_initial (r) && &ritem[s->اشیاء[شے]] == r->rhs + 1;
      aver (!is_successor_دا_initial_شے);
      إذا (!is_successor_دا_initial_شے)
        {
          /* If the LHS symbol of this item isn't known (because this is a
             top-level invocation), go get it.  */
          إذا (!lhs)
            lhs = r->lhs->نمبر;
          /* If this kernel item is next to the beginning of the RHS, then
             check all predecessors' goto follows for the LHS.  */
          إذا (شے_نمبر_is_قاعدہ_نمبر (ritem[s->اشیاء[شے] - 2]))
            {
              aver (lhs != acceptsymbol->content->نمبر);
              لكل (state **predecessor = predecessors[s->نمبر];
                   *predecessor;
                   ++predecessor)
                bitset_or (شے_lookahead_sets[s->نمبر][شے],
                           شے_lookahead_sets[s->نمبر][شے],
                           goto_follows[map_goto ((*predecessor)->نمبر,
                                                  lhs)]);
            }
          /* If this kernel item is later in the RHS, then check all
             predecessor items' lookahead sets.  */
          وإلا
            {
              لكل (state **predecessor = predecessors[s->نمبر];
                   *predecessor;
                   ++predecessor)
                {
                  size_t predecessor_شے;
                  لكل (predecessor_شے = 0;
                       predecessor_شے < (*predecessor)->nitems;
                       ++predecessor_شے)
                    إذا ((*predecessor)->اشیاء[predecessor_شے]
                        == s->اشیاء[شے] - 1)
                      اكسر;
                  aver (predecessor_شے != (*predecessor)->nitems);
                  ielr_شے_has_lookahead (*predecessor, lhs,
                                           predecessor_شے, 0 /*irrelevant*/,
                                           predecessors, شے_lookahead_sets);
                  bitset_or (شے_lookahead_sets[s->نمبر][شے],
                             شے_lookahead_sets[s->نمبر][شے],
                             شے_lookahead_sets[(*predecessor)->نمبر]
                                                [predecessor_شے]);
                }
            }
        }
    }
  أرجع bitset_test (شے_lookahead_sets[s->نمبر][شے], lookahead);
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
ساكن فراغ
ielr_compute_annotation_lists (bitsetv follow_kernel_اشیاء,
                               bitsetv always_follows, state ***predecessors,
                               Annotation_اشاریہ *max_annotationsp,
                               InadequacyList ***inadequacy_listsp,
                               AnnotationList ***annotation_listsp,
                               بنية obstack *annotations_obstackp)
{
  bitset **شے_lookahead_sets =
    xnmalloc (nstates, حجم *شے_lookahead_sets);
  Annotation_اشاریہ *annotation_counts =
    xnmalloc (nstates, حجم *annotation_counts);
  Contribution_اشاریہ max_contributions = 0;
  صحيح کل_annotations = 0;

  *inadequacy_listsp = xnmalloc (nstates, حجم **inadequacy_listsp);
  *annotation_listsp = xnmalloc (nstates, حجم **annotation_listsp);
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      شے_lookahead_sets[i] = عدم;
      (*inadequacy_listsp)[i] = عدم;
      (*annotation_listsp)[i] = عدم;
      annotation_counts[i] = 0;
    }
  {
    Inadequacy_List_گرہ_گنتی inadequacy_list_گرہ_گنتی = 0;
    لكل (state_نمبر i = 0; i < nstates; ++i)
      AnnotationList__compute_from_inadequacies (
        حالتاں[i], follow_kernel_اشیاء, always_follows, predecessors,
        شے_lookahead_sets, *inadequacy_listsp, *annotation_listsp,
        annotation_counts, &max_contributions, annotations_obstackp,
        &inadequacy_list_گرہ_گنتی);
  }
  *max_annotationsp = 0;
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      إذا (annotation_counts[i] > *max_annotationsp)
        *max_annotationsp = annotation_counts[i];
      کل_annotations += annotation_counts[i];
    }
  إذا (trace_flag & trace_ielr)
    {
      لكل (state_نمبر i = 0; i < nstates; ++i)
        {
          fprintf (stderr, "Inadequacy annotations for state %d:\n", i);
          AnnotationList__debug ((*annotation_listsp)[i],
                                 حالتاں[i]->nitems, 2);
        }
      fprintf (stderr, "Number of LR(0)/LALR(1) states: %d\n", nstates);
      fprintf (stderr, "Average number of annotations per state: %f\n",
               (عائم)کل_annotations/nstates);
      fprintf (stderr, "Max number of annotations per state: %d\n",
               *max_annotationsp);
      fprintf (stderr, "Max number of contributions per annotation: %d\n",
               max_contributions);
    }
  لكل (state_نمبر i = 0; i < nstates; ++i)
    إذا (شے_lookahead_sets[i])
      {
        لكل (size_t j = 0; j < حالتاں[i]->nitems; ++j)
          إذا (شے_lookahead_sets[i][j])
            bitset_free (شے_lookahead_sets[i][j]);
        حرر (شے_lookahead_sets[i]);
      }
  حرر (شے_lookahead_sets);
  حرر (annotation_counts);
}

تعريفنوع بنية state_list
{
  بنية state_list *اگلا;
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
  بنية state_list *lr0Isocore;
  بنية state_list *اگلا_Isocore;
} state_list;

MAYBE_بن_ورتیا ساكن فراغ
state_list_print_ (ثابت state_list *s, FILE *باہرلا, ثابت حرف *sep)
{
  إذا (s)
    {
      fprintf (باہرلا, "%s%d", sep, s->state->نمبر);
      state_list_print_ (s->اگلا, باہرلا, " ");
    }
}

MAYBE_بن_ورتیا ساكن فراغ
state_list_print (ثابت state_list *s, FILE *باہرلا)
{
  fprintf (باہرلا, "{");
  state_list_print_ (s, باہرلا, "");
  fprintf (باہرلا, "}");
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
ساكن فراغ
ielr_compute_goto_follow_سیٹ (bitsetv follow_kernel_اشیاء,
                              bitsetv always_follows, state_list *s,
                              sym_content *n, bitset follow_سیٹ)
{
  goto_نمبر n_goto = map_goto (s->lr0Isocore->state->نمبر, n->نمبر);
  bitset_copy (follow_سیٹ, always_follows[n_goto]);
  إذا (s->lookaheads)
    {
      bitset_iterator biter_شے;
      bitset_bindex شے;
      BITSET_FOR_EACH (biter_شے, follow_kernel_اشیاء[n_goto], شے, 0)
        إذا (s->lookaheads[شے])
          bitset_or (follow_سیٹ, follow_سیٹ, s->lookaheads[شے]);
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
ساكن فراغ
ielr_compute_lookaheads (bitsetv follow_kernel_اشیاء, bitsetv always_follows,
                         state_list *s, state *t, bitsetv lookahead_filter,
                         bitsetv lookaheads)
{
  size_t s_شے = 0;
  bitsetv_zero (lookaheads);
  لكل (size_t t_شے = 0; t_شے < t->nitems; ++t_شے)
    {
      /* If this kernel item is the beginning of a RHS, it must be the
         kernel item in the start state, but t is supposed to be a successor
         state.  If, instead, this kernel item is the successor of the start
         state's kernel item, the lookahead set is empty.  This second case is
         a special case to avoid the - 2 below, but the next successor can be
         handled fine without special casing it.  */
      aver (t->اشیاء[t_شے] != 0);
      ثابت قاعدہ *r = شے_قاعدہ (&ritem[t->اشیاء[t_شے]]);
      ثابت bool is_successor_دا_initial_شے
        = قاعدہ_is_initial (r) && &ritem[t->اشیاء[t_شے]] == r->rhs + 1;
      إذا (!is_successor_دا_initial_شے
          && !bitset_خالی_تھاواں_p (lookahead_filter[t_شے]))
        {
          /* Is this kernel item next to the beginning of the RHS?  */
          إذا (شے_نمبر_is_قاعدہ_نمبر (ritem[t->اشیاء[t_شے] - 2]))
            ielr_compute_goto_follow_سیٹ (
              follow_kernel_اشیاء, always_follows, s,
              r->lhs,
              lookaheads[t_شے]);
          وإلا إذا (s->lookaheads)
            {
              /* We don't have to start the s item search at the beginning
                 every time because items from both states are sorted by their
                 indices in ritem.  */
              لكل (; s_شے < s->state->nitems; ++s_شے)
                إذا (s->state->اشیاء[s_شے] == t->اشیاء[t_شے] - 1)
                  اكسر;
              aver (s_شے != s->state->nitems);
              إذا (s->lookaheads[s_شے])
                bitset_copy (lookaheads[t_شے], s->lookaheads[s_شے]);
            }
          bitset_and (lookaheads[t_شے],
                      lookaheads[t_شے], lookahead_filter[t_شے]);
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
ساكن فراغ
ielr_compute_state (bitsetv follow_kernel_اشیاء, bitsetv always_follows,
                    AnnotationList **annotation_lists, state *t,
                    bitsetv lookaheads, state_list **آخری_statep,
                    Contribution_اشاریہ work1[], bitsetv work2, state **tp)
{
  state_list *lr0_isocore = t->state_list->lr0Isocore;
  state_list **this_isocorep;

  /* Determine whether there's an isocore of t with which these lookaheads can
     be merged.  */
  {
    إذا (annotation_lists)
      {
        Annotation_اشاریہ ai;
        AnnotationList *a;
        لكل (ai = 0, a = annotation_lists[lr0_isocore->state->نمبر];
             a;
             ++ai, a = a->اگلا)
        work1[ai] =
          AnnotationList__computeDominantContribution (
            a, lr0_isocore->state->nitems, lookaheads, false);
      }
    لكل (this_isocorep = &t->state_list;
         this_isocorep == &t->state_list || *this_isocorep != t->state_list;
         this_isocorep = &(*this_isocorep)->اگلا_Isocore)
      {
        إذا (!(*this_isocorep)->recomputedAsSuccessor)
          اكسر;
        إذا (annotation_lists)
          {
            Annotation_اشاریہ ai;
            AnnotationList *a;
            لكل (ai = 0, a = annotation_lists[lr0_isocore->state->نمبر];
                 a;
                 ++ai, a = a->اگلا)
              {
                إذا (work1[ai] != Contribution_اشاریہ_none)
                  {
                    /* This isocore compatibility test depends on the fact
                       that, if the dominant contributions are the same for the
                       two isocores, then merging their lookahead sets will not
                       produce a state with a different dominant contribution.
                       */
                    Contribution_اشاریہ ci =
                      AnnotationList__computeDominantContribution (
                        a, lr0_isocore->state->nitems,
                        (*this_isocorep)->lookaheads, false);
                    إذا (ci != Contribution_اشاریہ_none && work1[ai] != ci)
                      اكسر;
                  }
              }
            إذا (!a)
              اكسر;
          }
        وإلا
          {
            size_t i;
            لكل (i = 0; i < t->nitems; ++i)
              {
                إذا (!(*this_isocorep)->lookaheads
                    || !(*this_isocorep)->lookaheads[i])
                  {
                    إذا (!bitset_خالی_تھاواں_p (lookaheads[i]))
                      اكسر;
                  }
                /* bitset_equal_p uses the size of the first argument,
                   so lookaheads[i] must be the second argument.  */
                وإلا إذا (!bitset_equal_p ((*this_isocorep)->lookaheads[i],
                                          lookaheads[i]))
                  اكسر;
              }
            إذا (i == t->nitems)
              اكسر;
          }
      }
  }

  bool has_lookaheads = false;
  لكل (size_t i = 0; i < lr0_isocore->state->nitems; ++i)
    إذا (!bitset_خالی_تھاواں_p (lookaheads[i]))
      {
        has_lookaheads = true;
        اكسر;
      }

  /* Merge with an existing isocore.  */
  إذا (this_isocorep == &t->state_list || *this_isocorep != t->state_list)
    {
      bool new_lookaheads = false;
      *tp = (*this_isocorep)->state;

      /* Merge lookaheads into the state and record whether any of them are
         actually new.  */
      إذا (has_lookaheads)
        {
          إذا (!(*this_isocorep)->lookaheads)
            {
              (*this_isocorep)->lookaheads =
                xnmalloc (t->nitems, حجم (*this_isocorep)->lookaheads);
              لكل (size_t i = 0; i < t->nitems; ++i)
                (*this_isocorep)->lookaheads[i] = عدم;
            }
          لكل (size_t i = 0; i < t->nitems; ++i)
            إذا (!bitset_خالی_تھاواں_p (lookaheads[i]))
              {
                إذا (!(*this_isocorep)->lookaheads[i])
                  (*this_isocorep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                bitset_andn (lookaheads[i],
                             lookaheads[i], (*this_isocorep)->lookaheads[i]);
                bitset_or ((*this_isocorep)->lookaheads[i],
                           lookaheads[i], (*this_isocorep)->lookaheads[i]);
                إذا (!bitset_خالی_تھاواں_p (lookaheads[i]))
                  new_lookaheads = true;
              }
        }

      /* If new lookaheads were merged, propagate those lookaheads to the
         successors, possibly splitting them.  If *tp is being recomputed for
         the first time, this isn't necessary because the main
         ielr_split_states loop will handle the successors later.  */
      إذا (!(*this_isocorep)->recomputedAsSuccessor)
        (*this_isocorep)->recomputedAsSuccessor = true;
      وإلا إذا (new_lookaheads)
        {
          /* When merging demands identical lookahead sets, it is impossible to
             merge new lookaheads.  */
          aver (annotation_lists);
          لكل (صحيح i = 0; i < (*tp)->transitions->عدد; ++i)
            {
              state *t2 = (*tp)->transitions->حالتاں[i];
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
              إذا (!t2->state_list->recomputedAsSuccessor)
                اكسر;
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t2->state_list->lr0Isocore->state->نمبر],
                t2->nitems, work2);
              ielr_compute_lookaheads (follow_kernel_اشیاء, always_follows,
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
              ielr_compute_state (follow_kernel_اشیاء, always_follows,
                                  annotation_lists, t2, lookaheads,
                                  آخری_statep, work1, work2,
                                  &(*tp)->transitions->حالتاں[i]);
            }
        }
    }

  /* Create a new isocore.  */
  وإلا
    {
      state_list *old_isocore = *this_isocorep;
      (*آخری_statep)->اگلا = *this_isocorep = xmalloc (حجم **آخری_statep);
      *آخری_statep = *this_isocorep;
      (*آخری_statep)->state = *tp = state_new_isocore (t);
      (*tp)->state_list = *آخری_statep;
      (*آخری_statep)->recomputedAsSuccessor = true;
      (*آخری_statep)->اگلا = عدم;
      (*آخری_statep)->lookaheads = عدم;
      إذا (has_lookaheads)
        {
          (*آخری_statep)->lookaheads =
            xnmalloc (t->nitems, حجم (*آخری_statep)->lookaheads);
          لكل (size_t i = 0; i < t->nitems; ++i)
            {
              إذا (bitset_خالی_تھاواں_p (lookaheads[i]))
                (*آخری_statep)->lookaheads[i] = عدم;
              وإلا
                {
                  (*آخری_statep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                  bitset_copy ((*آخری_statep)->lookaheads[i], lookaheads[i]);
                }
            }
        }
      (*آخری_statep)->lr0Isocore = lr0_isocore;
      (*آخری_statep)->اگلا_Isocore = old_isocore;
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
ساكن فراغ
ielr_ونڈو_حالتاں (bitsetv follow_kernel_اشیاء, bitsetv always_follows,
                   AnnotationList **annotation_lists,
                   Annotation_اشاریہ max_annotations)
{
  state_list *پہلا_state;
  state_list *آخری_state;
  bitsetv lookahead_filter = عدم;
  bitsetv lookaheads;

  /* Set up state list and some reusable bitsets.  */
  {
    size_t max_nitems = 0;
    state_list **nodep = &پہلا_state;
    لكل (state_نمبر i = 0; i < nstates; ++i)
      {
        *nodep = حالتاں[i]->state_list = آخری_state = xmalloc (حجم **nodep);
        (*nodep)->state = حالتاں[i];
        (*nodep)->recomputedAsSuccessor = false;
        (*nodep)->lookaheads = عدم;
        (*nodep)->lr0Isocore = *nodep;
        (*nodep)->اگلا_Isocore = *nodep;
        nodep = &(*nodep)->اگلا;
        إذا (حالتاں[i]->nitems > max_nitems)
          max_nitems = حالتاں[i]->nitems;
      }
    *nodep = عدم;
    lookahead_filter = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
    إذا (!annotation_lists)
      bitsetv_ones (lookahead_filter);
    lookaheads = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
  }

  /* Recompute states.  */
  {
    Contribution_اشاریہ *work = xnmalloc (max_annotations, حجم *work);
    لكل (state_list *this_state = پہلا_state;
         this_state;
         this_state = this_state->اگلا)
      {
        ثابت state *s = this_state->state;
        لكل (صحيح i = 0; i < s->transitions->عدد; ++i)
          {
            state *t = s->transitions->حالتاں[i];
            إذا (annotation_lists)
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t->state_list->lr0Isocore->state->نمبر],
                t->nitems, lookahead_filter);
            ielr_compute_lookaheads (follow_kernel_اشیاء, always_follows,
                                     this_state, t, lookahead_filter,
                                     lookaheads);
            ielr_compute_state (follow_kernel_اشیاء, always_follows,
                                annotation_lists, t, lookaheads, &آخری_state,
                                work, lookahead_filter,
                                &s->transitions->حالتاں[i]);
          }
      }
    حرر (work);
  }

  bitsetv_free (lookahead_filter);
  bitsetv_free (lookaheads);

  /* Store states back in the states array.  */
  حالتاں = xnrealloc (حالتاں, nstates, حجم *حالتاں);
  لكل (state_list *گرہ = پہلا_state; گرہ; گرہ = گرہ->اگلا)
    حالتاں[گرہ->state->نمبر] = گرہ->state;

  /* In the case of canonical LR(1), copy item lookahead sets to reduction
     lookahead sets.  */
  إذا (!annotation_lists)
    {
      timevar_ڈالو (tv_ielr_phase4);
      initialize_LA ();
      لكل (state_list *گرہ = پہلا_state; گرہ; گرہ = گرہ->اگلا)
        إذا (!گرہ->state->consistent)
          {
            size_t i = 0;
            شے_اشاریہ *itemset = گرہ->state->اشیاء;
            لكل (size_t r = 0; r < گرہ->state->reductions->عدد; ++r)
              {
                قاعدہ *this_قاعدہ = گرہ->state->reductions->قاعدے[r];
                bitset lookahead_سیٹ =
                  گرہ->state->reductions->lookaheads[r];
                إذا (شے_نمبر_is_قاعدہ_نمبر (*this_قاعدہ->rhs))
                  ielr_compute_goto_follow_سیٹ (follow_kernel_اشیاء,
                                                always_follows, گرہ,
                                                this_قاعدہ->lhs, lookahead_سیٹ);
                وإلا إذا (گرہ->lookaheads)
                  {
                    /* We don't need to start the kernel item search back at
                       i=0 because both items and reductions are sorted on rule
                       number.  */
                    طالما (!شے_نمبر_is_قاعدہ_نمبر (ritem[itemset[i]])
                           || شے_نمبر_as_قاعدہ_نمبر (ritem[itemset[i]])
                              != this_قاعدہ->نمبر)
                      {
                        ++i;
                        aver (i < گرہ->state->nitems);
                      }
                    إذا (گرہ->lookaheads[i])
                      bitset_copy (lookahead_سیٹ, گرہ->lookaheads[i]);
                  }
              }
          }
      timevar_نکالو (tv_ielr_phase4);
    }

  /* Free state list.  */
  طالما (پہلا_state)
    {
      state_list *گرہ = پہلا_state;
      إذا (گرہ->lookaheads)
        {
          لكل (size_t i = 0; i < گرہ->state->nitems; ++i)
            إذا (گرہ->lookaheads[i])
              bitset_free (گرہ->lookaheads[i]);
          حرر (گرہ->lookaheads);
        }
      پہلا_state = گرہ->اگلا;
      حرر (گرہ);
    }
}


فراغ
ielr (فراغ)
{
  LrType lr_type = lr_type_get ();

  /* Phase 0: LALR(1).  */
  بدل (lr_type)
    {
    حالة LR_TYPE__LR0:
      timevar_ڈالو (tv_lalr);
      سیٹ_goto_map ();
      timevar_نکالو (tv_lalr);
      أرجع;

    حالة LR_TYPE__CANONICAL_LR:
      timevar_ڈالو (tv_lalr);
      سیٹ_goto_map ();
      timevar_نکالو (tv_lalr);
      اكسر;

    حالة LR_TYPE__LALR:
      timevar_ڈالو (tv_lalr);
      lalr ();
      bitsetv_free (goto_follows);
      timevar_نکالو (tv_lalr);
      أرجع;

    حالة LR_TYPE__IELR:
      timevar_ڈالو (tv_lalr);
      lalr ();
      timevar_نکالو (tv_lalr);
      اكسر;
    }

  {
    bitsetv follow_kernel_اشیاء;
    bitsetv always_follows;
    InadequacyList **inadequacy_lists = عدم;
    AnnotationList **annotation_lists = عدم;
    بنية obstack annotations_obstack;
    Annotation_اشاریہ max_annotations = 0;

    {
      /* Phase 1: Compute Auxiliary Tables.  */
      state ***predecessors;
      timevar_ڈالو (tv_ielr_phase1);
      ielr_compute_auxiliary_tables (
        &follow_kernel_اشیاء, &always_follows,
        lr_type == LR_TYPE__CANONICAL_LR ? عدم : &predecessors);
      timevar_نکالو (tv_ielr_phase1);

      /* Phase 2: Compute Annotations.  */
      timevar_ڈالو (tv_ielr_phase2);
      إذا (lr_type != LR_TYPE__CANONICAL_LR)
        {
          obstack_init (&annotations_obstack);
          ielr_compute_annotation_lists (follow_kernel_اشیاء, always_follows,
                                         predecessors, &max_annotations,
                                         &inadequacy_lists, &annotation_lists,
                                         &annotations_obstack);
          لكل (state_نمبر i = 0; i < nstates; ++i)
            حرر (predecessors[i]);
          حرر (predecessors);
          bitsetv_free (goto_follows);
          lalr_free ();
        }
      timevar_نکالو (tv_ielr_phase2);
    }

    /* Phase 3: Split States.  */
    timevar_ڈالو (tv_ielr_phase3);
    {
      state_نمبر nstates_lr0 = nstates;
      ielr_ونڈو_حالتاں (follow_kernel_اشیاء, always_follows,
                         annotation_lists, max_annotations);
      إذا (inadequacy_lists)
        لكل (state_نمبر i = 0; i < nstates_lr0; ++i)
          InadequacyList__delete (inadequacy_lists[i]);
    }
    حرر (inadequacy_lists);
    إذا (annotation_lists)
      obstack_free (&annotations_obstack, عدم);
    حرر (annotation_lists);
    bitsetv_free (follow_kernel_اشیاء);
    bitsetv_free (always_follows);
    timevar_نکالو (tv_ielr_phase3);
  }

  /* Phase 4: Compute Reduction Lookaheads.  */
  timevar_ڈالو (tv_ielr_phase4);
  حرر (goto_map);
  حرر (from_state);
  حرر (منزل_state);
  إذا (lr_type == LR_TYPE__CANONICAL_LR)
    {
      /* Reduction lookaheads are computed in ielr_split_states above
         but are timed as part of phase 4. */
      سیٹ_goto_map ();
    }
  وإلا
    {
      lalr ();
      bitsetv_free (goto_follows);
    }
  timevar_نکالو (tv_ielr_phase4);
}
