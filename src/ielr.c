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
ielr_compute_ritem_sees_lookahead_سېټ (فراغ)
{
  bitset پایله = bitset_create (nritems, BITSET_FIXED);
  صحيح i = nritems-1;
  طالما (0 < i)
    {
      --i;
      // Walk the RHS right to left, as long as it's symbol,
      // nonterminal, nullable.
      طالما (!توکی_شمېره_is_قاعده_شمېره (ritem[i])
             && ISVAR (ritem[i])
             && nullable [توکی_شمېره_as_نښه_شمېره (ritem[i]) - ntokens])
        bitset_سېټ (پایله, i--);
      إذا (!توکی_شمېره_is_قاعده_شمېره (ritem[i]) && ISVAR (ritem[i]))
        bitset_سېټ (پایله, i--);
      // Flush the remainder of the RHS.
      طالما (!توکی_شمېره_is_قاعده_شمېره (ritem[i]) && 0 < i)
        --i;
    }
  إذا (trace_flag & trace_ielr)
    {
      fprintf (stderr, "ritem_sees_lookahead_set (indexes of ritems): ");
      bitset_dump (stderr, پایله);
      fprintf (stderr, "\n");
    }
  أرجع پایله;
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
ielr_compute_دننی_follow_څنډې (bitset ritem_sees_lookahead_سېټ,
                                    goto_شمېره ***edgesp, صحيح **څنډه_countsp)
{
  *edgesp = xnmalloc (ngotos, حجم **edgesp);
  *څنډه_countsp = xnmalloc (ngotos, حجم **څنډه_countsp);
  {
    bitset sources = bitset_create (ngotos, BITSET_FIXED);
    لكل (goto_شمېره i = 0; i < ngotos; ++i)
      (*څنډه_countsp)[i] = 0;
    لكل (goto_شمېره i = 0; i < ngotos; ++i)
      {
        صحيح nsources = 0;
        {
          لكل (قاعده **rulep = derives[حالتونه[منزل_state[i]]->accessing_نښه
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
              إذا (bitset_test (ritem_sees_lookahead_سېټ,
                               (*rulep)->rhs - ritem))
                {
                  goto_شمېره سرچینه =
                    map_goto (from_state[i],
                              توکی_شمېره_as_نښه_شمېره (*(*rulep)->rhs));
                  إذا (i != سرچینه && !bitset_test (sources, سرچینه))
                    {
                      bitset_سېټ (sources, سرچینه);
                      ++nsources;
                      ++(*څنډه_countsp)[سرچینه];
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
              bitset_iterator biter_سرچینه;
              bitset_bindex سرچینه;
              صحيح j = 0;
              BITSET_FOR_EACH (biter_سرچینه, sources, سرچینه, 0)
                (*edgesp)[i][j++] = سرچینه;
            }
            (*edgesp)[i][nsources] = پای_ټکی_غوټه;
          }
        bitset_zero (sources);
      }
    bitset_free (sources);
  }

  اړیکه_transpose (edgesp, ngotos);

  إذا (trace_flag & trace_ielr)
    اړیکه_print ("internal_follow_edges", *edgesp, ngotos, عدم, stderr);
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
ielr_compute_follow_kernel_توکي (bitset ritem_sees_lookahead_سېټ,
                                  goto_شمېره **دننی_follow_څنډې,
                                  bitsetv *follow_kernel_itemsp)
{
  {
    size_t max_nitems = 0;
    لكل (state_شمېره i = 0; i < nstates; ++i)
      إذا (حالتونه[i]->nitems > max_nitems)
        max_nitems = حالتونه[i]->nitems;
    *follow_kernel_itemsp = bitsetv_create (ngotos, max_nitems, BITSET_FIXED);
  }
  لكل (goto_شمېره i = 0; i < ngotos; ++i)
    {
      size_t nitems = حالتونه[from_state[i]]->nitems;
      توکی_شاخص *توکي = حالتونه[from_state[i]]->توکي;
      لكل (size_t j = 0; j < nitems; ++j)
        /* If this item has this goto and if all subsequent symbols in this
           RHS (if any) are nullable nonterminals, then record this item as
           one whose lookahead set is included in this goto's follows.  */
        إذا (توکی_شمېره_is_نښه_شمېره (ritem[توکي[j]])
            && توکی_شمېره_as_نښه_شمېره (ritem[توکي[j]])
            == حالتونه[منزل_state[i]]->accessing_نښه
            && bitset_test (ritem_sees_lookahead_سېټ, توکي[j]))
          bitset_سېټ ((*follow_kernel_itemsp)[i], j);
    }
  اړیکه_digraph (دننی_follow_څنډې, ngotos, *follow_kernel_itemsp);

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
ielr_compute_always_follows (goto_شمېره ***edgesp,
                             صحيح ثابت څنډه_counts[],
                             bitsetv *always_followsp)
{
  *always_followsp = bitsetv_create (ngotos, ntokens, BITSET_FIXED);
  {
    goto_شمېره *څنډه_array = xnmalloc (ngotos, حجم *څنډه_array);
    لكل (goto_شمېره i = 0; i < ngotos; ++i)
      {
        goto_شمېره nedges = څنډه_counts[i];
        {
          صحيح j;
          transitions *trans = حالتونه[منزل_state[i]]->transitions;
          FOR_EACH_ښويدنه (trans, j)
            bitset_سېټ ((*always_followsp)[i], TRANSITION_نښه (trans, j));
          لكل (; j < trans->عدد; ++j)
            {
              نښه_شمېره sym = TRANSITION_نښه (trans, j);
              إذا (nullable[sym - ntokens])
                څنډه_array[nedges++] = map_goto (منزل_state[i], sym);
            }
        }
        إذا (nedges - څنډه_counts[i])
          {
            (*edgesp)[i] =
              xnrealloc ((*edgesp)[i], nedges + 1, حجم *(*edgesp)[i]);
            حافظه_نقل ((*edgesp)[i] + څنډه_counts[i], څنډه_array + څنډه_counts[i],
                    (nedges - څنډه_counts[i]) * حجم *(*edgesp)[i]);
            (*edgesp)[i][nedges] = پای_ټکی_غوټه;
          }
      }
    حرر (څنډه_array);
  }
  اړیکه_digraph (*edgesp, ngotos, *always_followsp);

  إذا (trace_flag & trace_ielr)
    {
      اړیکه_print ("always follow edges", *edgesp, ngotos, عدم, stderr);
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
  لكل (state_شمېره i = 0; i < nstates; ++i)
    predecessor_counts[i] = 0;
  لكل (state_شمېره i = 0; i < nstates; ++i)
    لكل (صحيح j = 0; j < حالتونه[i]->transitions->عدد; ++j)
      ++predecessor_counts[حالتونه[i]->transitions->حالتونه[j]->شمېره];
  لكل (state_شمېره i = 0; i < nstates; ++i)
    {
      res[i] = xnmalloc (predecessor_counts[i]+1, حجم *res[i]);
      res[i][predecessor_counts[i]] = عدم;
      predecessor_counts[i] = 0;
    }
  لكل (state_شمېره i = 0; i < nstates; ++i)
    لكل (صحيح j = 0; j < حالتونه[i]->transitions->عدد; ++j)
      {
        state_شمېره k = حالتونه[i]->transitions->حالتونه[j]->شمېره;
        res[k][predecessor_counts[k]++] = حالتونه[i];
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
  goto_شمېره **څنډې;
  صحيح *څنډه_counts;
  {
    bitset ritem_sees_lookahead_سېټ = ielr_compute_ritem_sees_lookahead_سېټ ();
    ielr_compute_دننی_follow_څنډې (ritem_sees_lookahead_سېټ,
                                        &څنډې, &څنډه_counts);
    ielr_compute_follow_kernel_توکي (ritem_sees_lookahead_سېټ, څنډې,
                                      follow_kernel_itemsp);
    bitset_free (ritem_sees_lookahead_سېټ);
  }
  ielr_compute_always_follows (&څنډې, څنډه_counts, always_followsp);
  لكل (صحيح i = 0; i < ngotos; ++i)
    حرر (څنډې[i]);
  حرر (څنډې);
  حرر (څنډه_counts);
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
ielr_توکی_has_lookahead (state *s, نښه_شمېره lhs, size_t توکی,
                         نښه_شمېره lookahead, state ***predecessors,
                         bitset **توکی_lookahead_sets)
{
  إذا (!توکی_lookahead_sets[s->شمېره])
    {
      توکی_lookahead_sets[s->شمېره] =
        xnmalloc (s->nitems, حجم توکی_lookahead_sets[s->شمېره][0]);
      لكل (size_t i = 0; i < s->nitems; ++i)
        توکی_lookahead_sets[s->شمېره][i] = عدم;
    }
  إذا (!توکی_lookahead_sets[s->شمېره][توکی])
    {
      توکی_lookahead_sets[s->شمېره][توکی] =
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
      ثابت قاعده *r = توکی_قاعده (&ritem[s->توکي[توکی]]);
      ثابت bool is_successor_د_initial_توکی
        = قاعده_is_initial (r) && &ritem[s->توکي[توکی]] == r->rhs + 1;
      aver (!is_successor_د_initial_توکی);
      إذا (!is_successor_د_initial_توکی)
        {
          /* If the LHS symbol of this item isn't known (because this is a
             top-level invocation), go get it.  */
          إذا (!lhs)
            lhs = r->lhs->شمېره;
          /* If this kernel item is next to the beginning of the RHS, then
             check all predecessors' goto follows for the LHS.  */
          إذا (توکی_شمېره_is_قاعده_شمېره (ritem[s->توکي[توکی] - 2]))
            {
              aver (lhs != acceptsymbol->content->شمېره);
              لكل (state **predecessor = predecessors[s->شمېره];
                   *predecessor;
                   ++predecessor)
                bitset_or (توکی_lookahead_sets[s->شمېره][توکی],
                           توکی_lookahead_sets[s->شمېره][توکی],
                           goto_follows[map_goto ((*predecessor)->شمېره,
                                                  lhs)]);
            }
          /* If this kernel item is later in the RHS, then check all
             predecessor items' lookahead sets.  */
          وإلا
            {
              لكل (state **predecessor = predecessors[s->شمېره];
                   *predecessor;
                   ++predecessor)
                {
                  size_t predecessor_توکی;
                  لكل (predecessor_توکی = 0;
                       predecessor_توکی < (*predecessor)->nitems;
                       ++predecessor_توکی)
                    إذا ((*predecessor)->توکي[predecessor_توکی]
                        == s->توکي[توکی] - 1)
                      اكسر;
                  aver (predecessor_توکی != (*predecessor)->nitems);
                  ielr_توکی_has_lookahead (*predecessor, lhs,
                                           predecessor_توکی, 0 /*irrelevant*/,
                                           predecessors, توکی_lookahead_sets);
                  bitset_or (توکی_lookahead_sets[s->شمېره][توکی],
                             توکی_lookahead_sets[s->شمېره][توکی],
                             توکی_lookahead_sets[(*predecessor)->شمېره]
                                                [predecessor_توکی]);
                }
            }
        }
    }
  أرجع bitset_test (توکی_lookahead_sets[s->شمېره][توکی], lookahead);
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
ielr_compute_annotation_lists (bitsetv follow_kernel_توکي,
                               bitsetv always_follows, state ***predecessors,
                               Annotation_شاخص *max_annotationsp,
                               InadequacyList ***inadequacy_listsp,
                               AnnotationList ***annotation_listsp,
                               بنية obstack *annotations_obstackp)
{
  bitset **توکی_lookahead_sets =
    xnmalloc (nstates, حجم *توکی_lookahead_sets);
  Annotation_شاخص *annotation_counts =
    xnmalloc (nstates, حجم *annotation_counts);
  Contribution_شاخص max_contributions = 0;
  صحيح ټول_annotations = 0;

  *inadequacy_listsp = xnmalloc (nstates, حجم **inadequacy_listsp);
  *annotation_listsp = xnmalloc (nstates, حجم **annotation_listsp);
  لكل (state_شمېره i = 0; i < nstates; ++i)
    {
      توکی_lookahead_sets[i] = عدم;
      (*inadequacy_listsp)[i] = عدم;
      (*annotation_listsp)[i] = عدم;
      annotation_counts[i] = 0;
    }
  {
    Inadequacy_List_غوټه_شمېر inadequacy_list_غوټه_شمېر = 0;
    لكل (state_شمېره i = 0; i < nstates; ++i)
      AnnotationList__compute_from_inadequacies (
        حالتونه[i], follow_kernel_توکي, always_follows, predecessors,
        توکی_lookahead_sets, *inadequacy_listsp, *annotation_listsp,
        annotation_counts, &max_contributions, annotations_obstackp,
        &inadequacy_list_غوټه_شمېر);
  }
  *max_annotationsp = 0;
  لكل (state_شمېره i = 0; i < nstates; ++i)
    {
      إذا (annotation_counts[i] > *max_annotationsp)
        *max_annotationsp = annotation_counts[i];
      ټول_annotations += annotation_counts[i];
    }
  إذا (trace_flag & trace_ielr)
    {
      لكل (state_شمېره i = 0; i < nstates; ++i)
        {
          fprintf (stderr, "Inadequacy annotations for state %d:\n", i);
          AnnotationList__debug ((*annotation_listsp)[i],
                                 حالتونه[i]->nitems, 2);
        }
      fprintf (stderr, "Number of LR(0)/LALR(1) states: %d\n", nstates);
      fprintf (stderr, "Average number of annotations per state: %f\n",
               (عائم)ټول_annotations/nstates);
      fprintf (stderr, "Max number of annotations per state: %d\n",
               *max_annotationsp);
      fprintf (stderr, "Max number of contributions per annotation: %d\n",
               max_contributions);
    }
  لكل (state_شمېره i = 0; i < nstates; ++i)
    إذا (توکی_lookahead_sets[i])
      {
        لكل (size_t j = 0; j < حالتونه[i]->nitems; ++j)
          إذا (توکی_lookahead_sets[i][j])
            bitset_free (توکی_lookahead_sets[i][j]);
        حرر (توکی_lookahead_sets[i]);
      }
  حرر (توکی_lookahead_sets);
  حرر (annotation_counts);
}

تعريفنوع بنية state_list
{
  بنية state_list *بل;
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
  بنية state_list *بل_Isocore;
} state_list;

MAYBE_ناکارول_شوی ساكن فراغ
state_list_print_ (ثابت state_list *s, FILE *بهر, ثابت حرف *sep)
{
  إذا (s)
    {
      fprintf (بهر, "%s%d", sep, s->state->شمېره);
      state_list_print_ (s->بل, بهر, " ");
    }
}

MAYBE_ناکارول_شوی ساكن فراغ
state_list_print (ثابت state_list *s, FILE *بهر)
{
  fprintf (بهر, "{");
  state_list_print_ (s, بهر, "");
  fprintf (بهر, "}");
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
ielr_compute_goto_follow_سېټ (bitsetv follow_kernel_توکي,
                              bitsetv always_follows, state_list *s,
                              sym_content *n, bitset follow_سېټ)
{
  goto_شمېره n_goto = map_goto (s->lr0Isocore->state->شمېره, n->شمېره);
  bitset_copy (follow_سېټ, always_follows[n_goto]);
  إذا (s->lookaheads)
    {
      bitset_iterator biter_توکی;
      bitset_bindex توکی;
      BITSET_FOR_EACH (biter_توکی, follow_kernel_توکي[n_goto], توکی, 0)
        إذا (s->lookaheads[توکی])
          bitset_or (follow_سېټ, follow_سېټ, s->lookaheads[توکی]);
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
ielr_compute_lookaheads (bitsetv follow_kernel_توکي, bitsetv always_follows,
                         state_list *s, state *t, bitsetv lookahead_filter,
                         bitsetv lookaheads)
{
  size_t s_توکی = 0;
  bitsetv_zero (lookaheads);
  لكل (size_t t_توکی = 0; t_توکی < t->nitems; ++t_توکی)
    {
      /* If this kernel item is the beginning of a RHS, it must be the
         kernel item in the start state, but t is supposed to be a successor
         state.  If, instead, this kernel item is the successor of the start
         state's kernel item, the lookahead set is empty.  This second case is
         a special case to avoid the - 2 below, but the next successor can be
         handled fine without special casing it.  */
      aver (t->توکي[t_توکی] != 0);
      ثابت قاعده *r = توکی_قاعده (&ritem[t->توکي[t_توکی]]);
      ثابت bool is_successor_د_initial_توکی
        = قاعده_is_initial (r) && &ritem[t->توکي[t_توکی]] == r->rhs + 1;
      إذا (!is_successor_د_initial_توکی
          && !bitset_تشه_p (lookahead_filter[t_توکی]))
        {
          /* Is this kernel item next to the beginning of the RHS?  */
          إذا (توکی_شمېره_is_قاعده_شمېره (ritem[t->توکي[t_توکی] - 2]))
            ielr_compute_goto_follow_سېټ (
              follow_kernel_توکي, always_follows, s,
              r->lhs,
              lookaheads[t_توکی]);
          وإلا إذا (s->lookaheads)
            {
              /* We don't have to start the s item search at the beginning
                 every time because items from both states are sorted by their
                 indices in ritem.  */
              لكل (; s_توکی < s->state->nitems; ++s_توکی)
                إذا (s->state->توکي[s_توکی] == t->توکي[t_توکی] - 1)
                  اكسر;
              aver (s_توکی != s->state->nitems);
              إذا (s->lookaheads[s_توکی])
                bitset_copy (lookaheads[t_توکی], s->lookaheads[s_توکی]);
            }
          bitset_and (lookaheads[t_توکی],
                      lookaheads[t_توکی], lookahead_filter[t_توکی]);
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
ielr_compute_state (bitsetv follow_kernel_توکي, bitsetv always_follows,
                    AnnotationList **annotation_lists, state *t,
                    bitsetv lookaheads, state_list **وروستی_statep,
                    Contribution_شاخص work1[], bitsetv work2, state **tp)
{
  state_list *lr0_isocore = t->state_list->lr0Isocore;
  state_list **this_isocorep;

  /* Determine whether there's an isocore of t with which these lookaheads can
     be merged.  */
  {
    إذا (annotation_lists)
      {
        Annotation_شاخص ai;
        AnnotationList *a;
        لكل (ai = 0, a = annotation_lists[lr0_isocore->state->شمېره];
             a;
             ++ai, a = a->بل)
        work1[ai] =
          AnnotationList__computeDominantContribution (
            a, lr0_isocore->state->nitems, lookaheads, false);
      }
    لكل (this_isocorep = &t->state_list;
         this_isocorep == &t->state_list || *this_isocorep != t->state_list;
         this_isocorep = &(*this_isocorep)->بل_Isocore)
      {
        إذا (!(*this_isocorep)->recomputedAsSuccessor)
          اكسر;
        إذا (annotation_lists)
          {
            Annotation_شاخص ai;
            AnnotationList *a;
            لكل (ai = 0, a = annotation_lists[lr0_isocore->state->شمېره];
                 a;
                 ++ai, a = a->بل)
              {
                إذا (work1[ai] != Contribution_شاخص_none)
                  {
                    /* This isocore compatibility test depends on the fact
                       that, if the dominant contributions are the same for the
                       two isocores, then merging their lookahead sets will not
                       produce a state with a different dominant contribution.
                       */
                    Contribution_شاخص ci =
                      AnnotationList__computeDominantContribution (
                        a, lr0_isocore->state->nitems,
                        (*this_isocorep)->lookaheads, false);
                    إذا (ci != Contribution_شاخص_none && work1[ai] != ci)
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
                    إذا (!bitset_تشه_p (lookaheads[i]))
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
    إذا (!bitset_تشه_p (lookaheads[i]))
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
            إذا (!bitset_تشه_p (lookaheads[i]))
              {
                إذا (!(*this_isocorep)->lookaheads[i])
                  (*this_isocorep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                bitset_andn (lookaheads[i],
                             lookaheads[i], (*this_isocorep)->lookaheads[i]);
                bitset_or ((*this_isocorep)->lookaheads[i],
                           lookaheads[i], (*this_isocorep)->lookaheads[i]);
                إذا (!bitset_تشه_p (lookaheads[i]))
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
              state *t2 = (*tp)->transitions->حالتونه[i];
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
                annotation_lists[t2->state_list->lr0Isocore->state->شمېره],
                t2->nitems, work2);
              ielr_compute_lookaheads (follow_kernel_توکي, always_follows,
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
              ielr_compute_state (follow_kernel_توکي, always_follows,
                                  annotation_lists, t2, lookaheads,
                                  وروستی_statep, work1, work2,
                                  &(*tp)->transitions->حالتونه[i]);
            }
        }
    }

  /* Create a new isocore.  */
  وإلا
    {
      state_list *old_isocore = *this_isocorep;
      (*وروستی_statep)->بل = *this_isocorep = xmalloc (حجم **وروستی_statep);
      *وروستی_statep = *this_isocorep;
      (*وروستی_statep)->state = *tp = state_new_isocore (t);
      (*tp)->state_list = *وروستی_statep;
      (*وروستی_statep)->recomputedAsSuccessor = true;
      (*وروستی_statep)->بل = عدم;
      (*وروستی_statep)->lookaheads = عدم;
      إذا (has_lookaheads)
        {
          (*وروستی_statep)->lookaheads =
            xnmalloc (t->nitems, حجم (*وروستی_statep)->lookaheads);
          لكل (size_t i = 0; i < t->nitems; ++i)
            {
              إذا (bitset_تشه_p (lookaheads[i]))
                (*وروستی_statep)->lookaheads[i] = عدم;
              وإلا
                {
                  (*وروستی_statep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                  bitset_copy ((*وروستی_statep)->lookaheads[i], lookaheads[i]);
                }
            }
        }
      (*وروستی_statep)->lr0Isocore = lr0_isocore;
      (*وروستی_statep)->بل_Isocore = old_isocore;
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
ielr_وېشل_حالتونه (bitsetv follow_kernel_توکي, bitsetv always_follows,
                   AnnotationList **annotation_lists,
                   Annotation_شاخص max_annotations)
{
  state_list *لومړی_state;
  state_list *وروستی_state;
  bitsetv lookahead_filter = عدم;
  bitsetv lookaheads;

  /* Set up state list and some reusable bitsets.  */
  {
    size_t max_nitems = 0;
    state_list **nodep = &لومړی_state;
    لكل (state_شمېره i = 0; i < nstates; ++i)
      {
        *nodep = حالتونه[i]->state_list = وروستی_state = xmalloc (حجم **nodep);
        (*nodep)->state = حالتونه[i];
        (*nodep)->recomputedAsSuccessor = false;
        (*nodep)->lookaheads = عدم;
        (*nodep)->lr0Isocore = *nodep;
        (*nodep)->بل_Isocore = *nodep;
        nodep = &(*nodep)->بل;
        إذا (حالتونه[i]->nitems > max_nitems)
          max_nitems = حالتونه[i]->nitems;
      }
    *nodep = عدم;
    lookahead_filter = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
    إذا (!annotation_lists)
      bitsetv_ones (lookahead_filter);
    lookaheads = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
  }

  /* Recompute states.  */
  {
    Contribution_شاخص *work = xnmalloc (max_annotations, حجم *work);
    لكل (state_list *this_state = لومړی_state;
         this_state;
         this_state = this_state->بل)
      {
        ثابت state *s = this_state->state;
        لكل (صحيح i = 0; i < s->transitions->عدد; ++i)
          {
            state *t = s->transitions->حالتونه[i];
            إذا (annotation_lists)
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t->state_list->lr0Isocore->state->شمېره],
                t->nitems, lookahead_filter);
            ielr_compute_lookaheads (follow_kernel_توکي, always_follows,
                                     this_state, t, lookahead_filter,
                                     lookaheads);
            ielr_compute_state (follow_kernel_توکي, always_follows,
                                annotation_lists, t, lookaheads, &وروستی_state,
                                work, lookahead_filter,
                                &s->transitions->حالتونه[i]);
          }
      }
    حرر (work);
  }

  bitsetv_free (lookahead_filter);
  bitsetv_free (lookaheads);

  /* Store states back in the states array.  */
  حالتونه = xnrealloc (حالتونه, nstates, حجم *حالتونه);
  لكل (state_list *غوټه = لومړی_state; غوټه; غوټه = غوټه->بل)
    حالتونه[غوټه->state->شمېره] = غوټه->state;

  /* In the case of canonical LR(1), copy item lookahead sets to reduction
     lookahead sets.  */
  إذا (!annotation_lists)
    {
      timevar_ټېل (tv_ielr_phase4);
      initialize_LA ();
      لكل (state_list *غوټه = لومړی_state; غوټه; غوټه = غوټه->بل)
        إذا (!غوټه->state->consistent)
          {
            size_t i = 0;
            توکی_شاخص *itemset = غوټه->state->توکي;
            لكل (size_t r = 0; r < غوټه->state->reductions->عدد; ++r)
              {
                قاعده *this_قاعده = غوټه->state->reductions->قواعد[r];
                bitset lookahead_سېټ =
                  غوټه->state->reductions->lookaheads[r];
                إذا (توکی_شمېره_is_قاعده_شمېره (*this_قاعده->rhs))
                  ielr_compute_goto_follow_سېټ (follow_kernel_توکي,
                                                always_follows, غوټه,
                                                this_قاعده->lhs, lookahead_سېټ);
                وإلا إذا (غوټه->lookaheads)
                  {
                    /* We don't need to start the kernel item search back at
                       i=0 because both items and reductions are sorted on rule
                       number.  */
                    طالما (!توکی_شمېره_is_قاعده_شمېره (ritem[itemset[i]])
                           || توکی_شمېره_as_قاعده_شمېره (ritem[itemset[i]])
                              != this_قاعده->شمېره)
                      {
                        ++i;
                        aver (i < غوټه->state->nitems);
                      }
                    إذا (غوټه->lookaheads[i])
                      bitset_copy (lookahead_سېټ, غوټه->lookaheads[i]);
                  }
              }
          }
      timevar_راوباسه (tv_ielr_phase4);
    }

  /* Free state list.  */
  طالما (لومړی_state)
    {
      state_list *غوټه = لومړی_state;
      إذا (غوټه->lookaheads)
        {
          لكل (size_t i = 0; i < غوټه->state->nitems; ++i)
            إذا (غوټه->lookaheads[i])
              bitset_free (غوټه->lookaheads[i]);
          حرر (غوټه->lookaheads);
        }
      لومړی_state = غوټه->بل;
      حرر (غوټه);
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
      timevar_ټېل (tv_lalr);
      سېټ_goto_map ();
      timevar_راوباسه (tv_lalr);
      أرجع;

    حالة LR_TYPE__CANONICAL_LR:
      timevar_ټېل (tv_lalr);
      سېټ_goto_map ();
      timevar_راوباسه (tv_lalr);
      اكسر;

    حالة LR_TYPE__LALR:
      timevar_ټېل (tv_lalr);
      lalr ();
      bitsetv_free (goto_follows);
      timevar_راوباسه (tv_lalr);
      أرجع;

    حالة LR_TYPE__IELR:
      timevar_ټېل (tv_lalr);
      lalr ();
      timevar_راوباسه (tv_lalr);
      اكسر;
    }

  {
    bitsetv follow_kernel_توکي;
    bitsetv always_follows;
    InadequacyList **inadequacy_lists = عدم;
    AnnotationList **annotation_lists = عدم;
    بنية obstack annotations_obstack;
    Annotation_شاخص max_annotations = 0;

    {
      /* Phase 1: Compute Auxiliary Tables.  */
      state ***predecessors;
      timevar_ټېل (tv_ielr_phase1);
      ielr_compute_auxiliary_tables (
        &follow_kernel_توکي, &always_follows,
        lr_type == LR_TYPE__CANONICAL_LR ? عدم : &predecessors);
      timevar_راوباسه (tv_ielr_phase1);

      /* Phase 2: Compute Annotations.  */
      timevar_ټېل (tv_ielr_phase2);
      إذا (lr_type != LR_TYPE__CANONICAL_LR)
        {
          obstack_init (&annotations_obstack);
          ielr_compute_annotation_lists (follow_kernel_توکي, always_follows,
                                         predecessors, &max_annotations,
                                         &inadequacy_lists, &annotation_lists,
                                         &annotations_obstack);
          لكل (state_شمېره i = 0; i < nstates; ++i)
            حرر (predecessors[i]);
          حرر (predecessors);
          bitsetv_free (goto_follows);
          lalr_free ();
        }
      timevar_راوباسه (tv_ielr_phase2);
    }

    /* Phase 3: Split States.  */
    timevar_ټېل (tv_ielr_phase3);
    {
      state_شمېره nstates_lr0 = nstates;
      ielr_وېشل_حالتونه (follow_kernel_توکي, always_follows,
                         annotation_lists, max_annotations);
      إذا (inadequacy_lists)
        لكل (state_شمېره i = 0; i < nstates_lr0; ++i)
          InadequacyList__delete (inadequacy_lists[i]);
    }
    حرر (inadequacy_lists);
    إذا (annotation_lists)
      obstack_free (&annotations_obstack, عدم);
    حرر (annotation_lists);
    bitsetv_free (follow_kernel_توکي);
    bitsetv_free (always_follows);
    timevar_راوباسه (tv_ielr_phase3);
  }

  /* Phase 4: Compute Reduction Lookaheads.  */
  timevar_ټېل (tv_ielr_phase4);
  حرر (goto_map);
  حرر (from_state);
  حرر (منزل_state);
  إذا (lr_type == LR_TYPE__CANONICAL_LR)
    {
      /* Reduction lookaheads are computed in ielr_split_states above
         but are timed as part of phase 4. */
      سېټ_goto_map ();
    }
  وإلا
    {
      lalr ();
      bitsetv_free (goto_follows);
    }
  timevar_راوباسه (tv_ielr_phase4);
}
