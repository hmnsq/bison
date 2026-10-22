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

#अन्तर्भाव <config.h>

#अन्तर्भाव "ielr.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <timevar.h>

#अन्तर्भाव "AnnotationList.h"
#अन्तर्भाव "derives.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "nullable.h"
#अन्तर्भाव "relation.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "symtab.h"

/** Records the value of the \%define variable lr.type.  */
प्रकारनिर्देश गणना
  {
   LR_TYPE__LR0,
   LR_TYPE__LALR,
   LR_TYPE__IELR,
   LR_TYPE__CANONICAL_LR
  } LrType;

/* The user's requested LR type.  */
स्थिर LrType
lr_type_get (शून्य)
{
  वर्ण *type = muscle_percent_define_get ("lr.type");
  LrType res;
  यदि (STREQ (type, "lr""(0)"))
    res = LR_TYPE__LR0;
  अन्यथा यदि (STREQ (type, "lalr"))
    res = LR_TYPE__LALR;
  अन्यथा यदि (STREQ (type, "ielr"))
    res = LR_TYPE__IELR;
  अन्यथा यदि (STREQ (type, "canonical-lr"))
    res = LR_TYPE__CANONICAL_LR;
  अन्यथा
    {
      aver (false);
      abort ();
    }
  मुक्त (type);
  प्रत्यागम res;
}

/**
 * \post:
 *   - \c result = a new \c bitset of size \c ::nritems such that any bit \c i
 *     is set iff <tt>ritem[i]</tt> is a nonterminal after which all ritems in
 *     the same RHS are nullable nonterminals.  In other words, the follows of
 *     a goto on <tt>ritem[i]</tt> include the lookahead set of the item.
 */
स्थिर bitset
ielr_compute_ritem_sees_lookahead_समुच्चयः (शून्य)
{
  bitset परिणाम = bitset_create (nritems, BITSET_FIXED);
  पूर्णाङ्क i = nritems-1;
  यावत् (0 < i)
    {
      --i;
      // Walk the RHS right to left, as long as it's symbol,
      // nonterminal, nullable.
      यावत् (!item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[i])
             && ISVAR (ritem[i])
             && nullable [item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (ritem[i]) - ntokens])
        bitset_समुच्चयः (परिणाम, i--);
      यदि (!item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[i]) && ISVAR (ritem[i]))
        bitset_समुच्चयः (परिणाम, i--);
      // Flush the remainder of the RHS.
      यावत् (!item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[i]) && 0 < i)
        --i;
    }
  यदि (trace_flag & trace_ielr)
    {
      fprintf (stderr, "ritem_sees_lookahead_set (indexes of ritems): ");
      bitset_dump (stderr, परिणाम);
      fprintf (stderr, "\n");
    }
  प्रत्यागम परिणाम;
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
स्थिर शून्य
ielr_compute_आन्तरिकम्_follow_धाराः (bitset ritem_sees_lookahead_समुच्चयः,
                                    goto_सङ्ख्या ***edgesp, पूर्णाङ्क **edge_countsp)
{
  *edgesp = xnmalloc (ngotos, परिमाण **edgesp);
  *edge_countsp = xnmalloc (ngotos, परिमाण **edge_countsp);
  {
    bitset sources = bitset_create (ngotos, BITSET_FIXED);
    प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
      (*edge_countsp)[i] = 0;
    प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
      {
        पूर्णाङ्क nsources = 0;
        {
          प्रति (नियमः **rulep = derives[अवस्थाः[गन्तव्यम्_state[i]]->accessing_चिह्नम्
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
              यदि (bitset_test (ritem_sees_lookahead_समुच्चयः,
                               (*rulep)->rhs - ritem))
                {
                  goto_सङ्ख्या source =
                    map_goto (from_state[i],
                              item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*(*rulep)->rhs));
                  यदि (i != source && !bitset_test (sources, source))
                    {
                      bitset_समुच्चयः (sources, source);
                      ++nsources;
                      ++(*edge_countsp)[source];
                    }
                }
            }
        }
        यदि (nsources == 0)
          (*edgesp)[i] = रिक्त;
        अन्यथा
          {
            (*edgesp)[i] = xnmalloc (nsources + 1, परिमाण *(*edgesp)[i]);
            {
              bitset_iterator biter_source;
              bitset_bindex source;
              पूर्णाङ्क j = 0;
              BITSET_FOR_EACH (biter_source, sources, source, 0)
                (*edgesp)[i][j++] = source;
            }
            (*edgesp)[i][nsources] = अन्तः_ग्रन्थिः;
          }
        bitset_zero (sources);
      }
    bitset_free (sources);
  }

  सम्बन्धः_transpose (edgesp, ngotos);

  यदि (trace_flag & trace_ielr)
    सम्बन्धः_print ("internal_follow_edges", *edgesp, ngotos, रिक्त, stderr);
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
स्थिर शून्य
ielr_compute_follow_kernel_वस्तूनि (bitset ritem_sees_lookahead_समुच्चयः,
                                  goto_सङ्ख्या **आन्तरिकम्_follow_धाराः,
                                  bitsetv *follow_kernel_itemsp)
{
  {
    size_t max_nitems = 0;
    प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
      यदि (अवस्थाः[i]->nitems > max_nitems)
        max_nitems = अवस्थाः[i]->nitems;
    *follow_kernel_itemsp = bitsetv_create (ngotos, max_nitems, BITSET_FIXED);
  }
  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      size_t nitems = अवस्थाः[from_state[i]]->nitems;
      item_सूचकांक *वस्तूनि = अवस्थाः[from_state[i]]->वस्तूनि;
      प्रति (size_t j = 0; j < nitems; ++j)
        /* If this item has this goto and if all subsequent symbols in this
           RHS (if any) are nullable nonterminals, then record this item as
           one whose lookahead set is included in this goto's follows.  */
        यदि (item_सङ्ख्या_is_चिह्नम्_सङ्ख्या (ritem[वस्तूनि[j]])
            && item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (ritem[वस्तूनि[j]])
            == अवस्थाः[गन्तव्यम्_state[i]]->accessing_चिह्नम्
            && bitset_test (ritem_sees_lookahead_समुच्चयः, वस्तूनि[j]))
          bitset_समुच्चयः ((*follow_kernel_itemsp)[i], j);
    }
  सम्बन्धः_digraph (आन्तरिकम्_follow_धाराः, ngotos, *follow_kernel_itemsp);

  यदि (trace_flag & trace_ielr)
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
स्थिर शून्य
ielr_compute_always_follows (goto_सङ्ख्या ***edgesp,
                             पूर्णाङ्क ध्रुव edge_counts[],
                             bitsetv *always_followsp)
{
  *always_followsp = bitsetv_create (ngotos, ntokens, BITSET_FIXED);
  {
    goto_सङ्ख्या *edge_array = xnmalloc (ngotos, परिमाण *edge_array);
    प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
      {
        goto_सङ्ख्या nedges = edge_counts[i];
        {
          पूर्णाङ्क j;
          transitions *trans = अवस्थाः[गन्तव्यम्_state[i]]->transitions;
          FOR_EACH_सरणम् (trans, j)
            bitset_समुच्चयः ((*always_followsp)[i], TRANSITION_चिह्नम् (trans, j));
          प्रति (; j < trans->अङ्कः; ++j)
            {
              चिह्नम्_सङ्ख्या sym = TRANSITION_चिह्नम् (trans, j);
              यदि (nullable[sym - ntokens])
                edge_array[nedges++] = map_goto (गन्तव्यम्_state[i], sym);
            }
        }
        यदि (nedges - edge_counts[i])
          {
            (*edgesp)[i] =
              xnrealloc ((*edgesp)[i], nedges + 1, परिमाण *(*edgesp)[i]);
            स्मृतिं_प्रतिलिख ((*edgesp)[i] + edge_counts[i], edge_array + edge_counts[i],
                    (nedges - edge_counts[i]) * परिमाण *(*edgesp)[i]);
            (*edgesp)[i][nedges] = अन्तः_ग्रन्थिः;
          }
      }
    मुक्त (edge_array);
  }
  सम्बन्धः_digraph (*edgesp, ngotos, *always_followsp);

  यदि (trace_flag & trace_ielr)
    {
      सम्बन्धः_print ("always follow edges", *edgesp, ngotos, रिक्त, stderr);
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
स्थिर state ***
ielr_compute_predecessors (शून्य)
{
  पूर्णाङ्क *predecessor_counts = xnmalloc (nstates, परिमाण *predecessor_counts);
  state ***res = xnmalloc (nstates, परिमाण *res);
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    predecessor_counts[i] = 0;
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    प्रति (पूर्णाङ्क j = 0; j < अवस्थाः[i]->transitions->अङ्कः; ++j)
      ++predecessor_counts[अवस्थाः[i]->transitions->अवस्थाः[j]->सङ्ख्या];
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      res[i] = xnmalloc (predecessor_counts[i]+1, परिमाण *res[i]);
      res[i][predecessor_counts[i]] = रिक्त;
      predecessor_counts[i] = 0;
    }
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    प्रति (पूर्णाङ्क j = 0; j < अवस्थाः[i]->transitions->अङ्कः; ++j)
      {
        state_सङ्ख्या k = अवस्थाः[i]->transitions->अवस्थाः[j]->सङ्ख्या;
        res[k][predecessor_counts[k]++] = अवस्थाः[i];
      }
  मुक्त (predecessor_counts);
  प्रत्यागम res;
}

/**
 * \post
 *   - \c *follow_kernel_itemsp and \c *always_followsp were computed by
 *     \c ielr_compute_follow_kernel_items and
 *     \c ielr_compute_always_follows.
 *   - Iff <tt>predecessorsp != NULL</tt>, then \c *predecessorsp was computed
 *     by \c ielr_compute_predecessors.
 */
स्थिर शून्य
ielr_compute_auxiliary_tables (bitsetv *follow_kernel_itemsp,
                               bitsetv *always_followsp,
                               state ****predecessorsp)
{
  goto_सङ्ख्या **धाराः;
  पूर्णाङ्क *edge_counts;
  {
    bitset ritem_sees_lookahead_समुच्चयः = ielr_compute_ritem_sees_lookahead_समुच्चयः ();
    ielr_compute_आन्तरिकम्_follow_धाराः (ritem_sees_lookahead_समुच्चयः,
                                        &धाराः, &edge_counts);
    ielr_compute_follow_kernel_वस्तूनि (ritem_sees_lookahead_समुच्चयः, धाराः,
                                      follow_kernel_itemsp);
    bitset_free (ritem_sees_lookahead_समुच्चयः);
  }
  ielr_compute_always_follows (&धाराः, edge_counts, always_followsp);
  प्रति (पूर्णाङ्क i = 0; i < ngotos; ++i)
    मुक्त (धाराः[i]);
  मुक्त (धाराः);
  मुक्त (edge_counts);
  यदि (predecessorsp)
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
ielr_item_has_lookahead (state *s, चिह्नम्_सङ्ख्या lhs, size_t item,
                         चिह्नम्_सङ्ख्या lookahead, state ***predecessors,
                         bitset **item_lookahead_sets)
{
  यदि (!item_lookahead_sets[s->सङ्ख्या])
    {
      item_lookahead_sets[s->सङ्ख्या] =
        xnmalloc (s->nitems, परिमाण item_lookahead_sets[s->सङ्ख्या][0]);
      प्रति (size_t i = 0; i < s->nitems; ++i)
        item_lookahead_sets[s->सङ्ख्या][i] = रिक्त;
    }
  यदि (!item_lookahead_sets[s->सङ्ख्या][item])
    {
      item_lookahead_sets[s->सङ्ख्या][item] =
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
      ध्रुव नियमः *r = item_नियमः (&ritem[s->वस्तूनि[item]]);
      ध्रुव bool is_successor_सम्बन्धि_initial_item
        = नियमः_is_initial (r) && &ritem[s->वस्तूनि[item]] == r->rhs + 1;
      aver (!is_successor_सम्बन्धि_initial_item);
      यदि (!is_successor_सम्बन्धि_initial_item)
        {
          /* If the LHS symbol of this item isn't known (because this is a
             top-level invocation), go get it.  */
          यदि (!lhs)
            lhs = r->lhs->सङ्ख्या;
          /* If this kernel item is next to the beginning of the RHS, then
             check all predecessors' goto follows for the LHS.  */
          यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[s->वस्तूनि[item] - 2]))
            {
              aver (lhs != acceptsymbol->content->सङ्ख्या);
              प्रति (state **predecessor = predecessors[s->सङ्ख्या];
                   *predecessor;
                   ++predecessor)
                bitset_or (item_lookahead_sets[s->सङ्ख्या][item],
                           item_lookahead_sets[s->सङ्ख्या][item],
                           goto_follows[map_goto ((*predecessor)->सङ्ख्या,
                                                  lhs)]);
            }
          /* If this kernel item is later in the RHS, then check all
             predecessor items' lookahead sets.  */
          अन्यथा
            {
              प्रति (state **predecessor = predecessors[s->सङ्ख्या];
                   *predecessor;
                   ++predecessor)
                {
                  size_t predecessor_item;
                  प्रति (predecessor_item = 0;
                       predecessor_item < (*predecessor)->nitems;
                       ++predecessor_item)
                    यदि ((*predecessor)->वस्तूनि[predecessor_item]
                        == s->वस्तूनि[item] - 1)
                      भङ्ग;
                  aver (predecessor_item != (*predecessor)->nitems);
                  ielr_item_has_lookahead (*predecessor, lhs,
                                           predecessor_item, 0 /*irrelevant*/,
                                           predecessors, item_lookahead_sets);
                  bitset_or (item_lookahead_sets[s->सङ्ख्या][item],
                             item_lookahead_sets[s->सङ्ख्या][item],
                             item_lookahead_sets[(*predecessor)->सङ्ख्या]
                                                [predecessor_item]);
                }
            }
        }
    }
  प्रत्यागम bitset_test (item_lookahead_sets[s->सङ्ख्या][item], lookahead);
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
स्थिर शून्य
ielr_compute_annotation_lists (bitsetv follow_kernel_वस्तूनि,
                               bitsetv always_follows, state ***predecessors,
                               Annotation_सूचकांक *max_annotationsp,
                               InadequacyList ***inadequacy_listsp,
                               AnnotationList ***annotation_listsp,
                               संरचना obstack *annotations_obstackp)
{
  bitset **item_lookahead_sets =
    xnmalloc (nstates, परिमाण *item_lookahead_sets);
  Annotation_सूचकांक *annotation_counts =
    xnmalloc (nstates, परिमाण *annotation_counts);
  Contribution_सूचकांक max_contributions = 0;
  पूर्णाङ्क योग_annotations = 0;

  *inadequacy_listsp = xnmalloc (nstates, परिमाण **inadequacy_listsp);
  *annotation_listsp = xnmalloc (nstates, परिमाण **annotation_listsp);
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      item_lookahead_sets[i] = रिक्त;
      (*inadequacy_listsp)[i] = रिक्त;
      (*annotation_listsp)[i] = रिक्त;
      annotation_counts[i] = 0;
    }
  {
    Inadequacy_List_ग्रन्थिः_गणनम् inadequacy_list_ग्रन्थिः_गणनम् = 0;
    प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
      AnnotationList__compute_from_inadequacies (
        अवस्थाः[i], follow_kernel_वस्तूनि, always_follows, predecessors,
        item_lookahead_sets, *inadequacy_listsp, *annotation_listsp,
        annotation_counts, &max_contributions, annotations_obstackp,
        &inadequacy_list_ग्रन्थिः_गणनम्);
  }
  *max_annotationsp = 0;
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      यदि (annotation_counts[i] > *max_annotationsp)
        *max_annotationsp = annotation_counts[i];
      योग_annotations += annotation_counts[i];
    }
  यदि (trace_flag & trace_ielr)
    {
      प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
        {
          fprintf (stderr, "Inadequacy annotations for state %d:\n", i);
          AnnotationList__debug ((*annotation_listsp)[i],
                                 अवस्थाः[i]->nitems, 2);
        }
      fprintf (stderr, "Number of LR(0)/LALR(1) states: %d\n", nstates);
      fprintf (stderr, "Average number of annotations per state: %f\n",
               (भिन्न)योग_annotations/nstates);
      fprintf (stderr, "Max number of annotations per state: %d\n",
               *max_annotationsp);
      fprintf (stderr, "Max number of contributions per annotation: %d\n",
               max_contributions);
    }
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    यदि (item_lookahead_sets[i])
      {
        प्रति (size_t j = 0; j < अवस्थाः[i]->nitems; ++j)
          यदि (item_lookahead_sets[i][j])
            bitset_free (item_lookahead_sets[i][j]);
        मुक्त (item_lookahead_sets[i]);
      }
  मुक्त (item_lookahead_sets);
  मुक्त (annotation_counts);
}

प्रकारनिर्देश संरचना state_list
{
  संरचना state_list *अग्र;
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
  संरचना state_list *lr0Isocore;
  संरचना state_list *अग्र_Isocore;
} state_list;

MAYBE_अप्रयुक्तम् स्थिर शून्य
state_list_print_ (ध्रुव state_list *s, FILE *बहिर्गमनम्, ध्रुव वर्ण *sep)
{
  यदि (s)
    {
      fprintf (बहिर्गमनम्, "%s%d", sep, s->state->सङ्ख्या);
      state_list_print_ (s->अग्र, बहिर्गमनम्, " ");
    }
}

MAYBE_अप्रयुक्तम् स्थिर शून्य
state_list_print (ध्रुव state_list *s, FILE *बहिर्गमनम्)
{
  fprintf (बहिर्गमनम्, "{");
  state_list_print_ (s, बहिर्गमनम्, "");
  fprintf (बहिर्गमनम्, "}");
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
स्थिर शून्य
ielr_compute_goto_follow_समुच्चयः (bitsetv follow_kernel_वस्तूनि,
                              bitsetv always_follows, state_list *s,
                              sym_content *n, bitset follow_समुच्चयः)
{
  goto_सङ्ख्या n_goto = map_goto (s->lr0Isocore->state->सङ्ख्या, n->सङ्ख्या);
  bitset_copy (follow_समुच्चयः, always_follows[n_goto]);
  यदि (s->lookaheads)
    {
      bitset_iterator biter_item;
      bitset_bindex item;
      BITSET_FOR_EACH (biter_item, follow_kernel_वस्तूनि[n_goto], item, 0)
        यदि (s->lookaheads[item])
          bitset_or (follow_समुच्चयः, follow_समुच्चयः, s->lookaheads[item]);
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
स्थिर शून्य
ielr_compute_lookaheads (bitsetv follow_kernel_वस्तूनि, bitsetv always_follows,
                         state_list *s, state *t, bitsetv lookahead_filter,
                         bitsetv lookaheads)
{
  size_t s_item = 0;
  bitsetv_zero (lookaheads);
  प्रति (size_t t_item = 0; t_item < t->nitems; ++t_item)
    {
      /* If this kernel item is the beginning of a RHS, it must be the
         kernel item in the start state, but t is supposed to be a successor
         state.  If, instead, this kernel item is the successor of the start
         state's kernel item, the lookahead set is empty.  This second case is
         a special case to avoid the - 2 below, but the next successor can be
         handled fine without special casing it.  */
      aver (t->वस्तूनि[t_item] != 0);
      ध्रुव नियमः *r = item_नियमः (&ritem[t->वस्तूनि[t_item]]);
      ध्रुव bool is_successor_सम्बन्धि_initial_item
        = नियमः_is_initial (r) && &ritem[t->वस्तूनि[t_item]] == r->rhs + 1;
      यदि (!is_successor_सम्बन्धि_initial_item
          && !bitset_रिक्तानि_p (lookahead_filter[t_item]))
        {
          /* Is this kernel item next to the beginning of the RHS?  */
          यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[t->वस्तूनि[t_item] - 2]))
            ielr_compute_goto_follow_समुच्चयः (
              follow_kernel_वस्तूनि, always_follows, s,
              r->lhs,
              lookaheads[t_item]);
          अन्यथा यदि (s->lookaheads)
            {
              /* We don't have to start the s item search at the beginning
                 every time because items from both states are sorted by their
                 indices in ritem.  */
              प्रति (; s_item < s->state->nitems; ++s_item)
                यदि (s->state->वस्तूनि[s_item] == t->वस्तूनि[t_item] - 1)
                  भङ्ग;
              aver (s_item != s->state->nitems);
              यदि (s->lookaheads[s_item])
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
स्थिर शून्य
ielr_compute_state (bitsetv follow_kernel_वस्तूनि, bitsetv always_follows,
                    AnnotationList **annotation_lists, state *t,
                    bitsetv lookaheads, state_list **चरमः_statep,
                    Contribution_सूचकांक work1[], bitsetv work2, state **tp)
{
  state_list *lr0_isocore = t->state_list->lr0Isocore;
  state_list **this_isocorep;

  /* Determine whether there's an isocore of t with which these lookaheads can
     be merged.  */
  {
    यदि (annotation_lists)
      {
        Annotation_सूचकांक ai;
        AnnotationList *a;
        प्रति (ai = 0, a = annotation_lists[lr0_isocore->state->सङ्ख्या];
             a;
             ++ai, a = a->अग्र)
        work1[ai] =
          AnnotationList__computeDominantContribution (
            a, lr0_isocore->state->nitems, lookaheads, false);
      }
    प्रति (this_isocorep = &t->state_list;
         this_isocorep == &t->state_list || *this_isocorep != t->state_list;
         this_isocorep = &(*this_isocorep)->अग्र_Isocore)
      {
        यदि (!(*this_isocorep)->recomputedAsSuccessor)
          भङ्ग;
        यदि (annotation_lists)
          {
            Annotation_सूचकांक ai;
            AnnotationList *a;
            प्रति (ai = 0, a = annotation_lists[lr0_isocore->state->सङ्ख्या];
                 a;
                 ++ai, a = a->अग्र)
              {
                यदि (work1[ai] != Contribution_सूचकांक_none)
                  {
                    /* This isocore compatibility test depends on the fact
                       that, if the dominant contributions are the same for the
                       two isocores, then merging their lookahead sets will not
                       produce a state with a different dominant contribution.
                       */
                    Contribution_सूचकांक ci =
                      AnnotationList__computeDominantContribution (
                        a, lr0_isocore->state->nitems,
                        (*this_isocorep)->lookaheads, false);
                    यदि (ci != Contribution_सूचकांक_none && work1[ai] != ci)
                      भङ्ग;
                  }
              }
            यदि (!a)
              भङ्ग;
          }
        अन्यथा
          {
            size_t i;
            प्रति (i = 0; i < t->nitems; ++i)
              {
                यदि (!(*this_isocorep)->lookaheads
                    || !(*this_isocorep)->lookaheads[i])
                  {
                    यदि (!bitset_रिक्तानि_p (lookaheads[i]))
                      भङ्ग;
                  }
                /* bitset_equal_p uses the size of the first argument,
                   so lookaheads[i] must be the second argument.  */
                अन्यथा यदि (!bitset_equal_p ((*this_isocorep)->lookaheads[i],
                                          lookaheads[i]))
                  भङ्ग;
              }
            यदि (i == t->nitems)
              भङ्ग;
          }
      }
  }

  bool has_lookaheads = false;
  प्रति (size_t i = 0; i < lr0_isocore->state->nitems; ++i)
    यदि (!bitset_रिक्तानि_p (lookaheads[i]))
      {
        has_lookaheads = true;
        भङ्ग;
      }

  /* Merge with an existing isocore.  */
  यदि (this_isocorep == &t->state_list || *this_isocorep != t->state_list)
    {
      bool new_lookaheads = false;
      *tp = (*this_isocorep)->state;

      /* Merge lookaheads into the state and record whether any of them are
         actually new.  */
      यदि (has_lookaheads)
        {
          यदि (!(*this_isocorep)->lookaheads)
            {
              (*this_isocorep)->lookaheads =
                xnmalloc (t->nitems, परिमाण (*this_isocorep)->lookaheads);
              प्रति (size_t i = 0; i < t->nitems; ++i)
                (*this_isocorep)->lookaheads[i] = रिक्त;
            }
          प्रति (size_t i = 0; i < t->nitems; ++i)
            यदि (!bitset_रिक्तानि_p (lookaheads[i]))
              {
                यदि (!(*this_isocorep)->lookaheads[i])
                  (*this_isocorep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                bitset_andn (lookaheads[i],
                             lookaheads[i], (*this_isocorep)->lookaheads[i]);
                bitset_or ((*this_isocorep)->lookaheads[i],
                           lookaheads[i], (*this_isocorep)->lookaheads[i]);
                यदि (!bitset_रिक्तानि_p (lookaheads[i]))
                  new_lookaheads = true;
              }
        }

      /* If new lookaheads were merged, propagate those lookaheads to the
         successors, possibly splitting them.  If *tp is being recomputed for
         the first time, this isn't necessary because the main
         ielr_split_states loop will handle the successors later.  */
      यदि (!(*this_isocorep)->recomputedAsSuccessor)
        (*this_isocorep)->recomputedAsSuccessor = true;
      अन्यथा यदि (new_lookaheads)
        {
          /* When merging demands identical lookahead sets, it is impossible to
             merge new lookaheads.  */
          aver (annotation_lists);
          प्रति (पूर्णाङ्क i = 0; i < (*tp)->transitions->अङ्कः; ++i)
            {
              state *t2 = (*tp)->transitions->अवस्थाः[i];
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
              यदि (!t2->state_list->recomputedAsSuccessor)
                भङ्ग;
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t2->state_list->lr0Isocore->state->सङ्ख्या],
                t2->nitems, work2);
              ielr_compute_lookaheads (follow_kernel_वस्तूनि, always_follows,
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
              ielr_compute_state (follow_kernel_वस्तूनि, always_follows,
                                  annotation_lists, t2, lookaheads,
                                  चरमः_statep, work1, work2,
                                  &(*tp)->transitions->अवस्थाः[i]);
            }
        }
    }

  /* Create a new isocore.  */
  अन्यथा
    {
      state_list *old_isocore = *this_isocorep;
      (*चरमः_statep)->अग्र = *this_isocorep = xmalloc (परिमाण **चरमः_statep);
      *चरमः_statep = *this_isocorep;
      (*चरमः_statep)->state = *tp = state_new_isocore (t);
      (*tp)->state_list = *चरमः_statep;
      (*चरमः_statep)->recomputedAsSuccessor = true;
      (*चरमः_statep)->अग्र = रिक्त;
      (*चरमः_statep)->lookaheads = रिक्त;
      यदि (has_lookaheads)
        {
          (*चरमः_statep)->lookaheads =
            xnmalloc (t->nitems, परिमाण (*चरमः_statep)->lookaheads);
          प्रति (size_t i = 0; i < t->nitems; ++i)
            {
              यदि (bitset_रिक्तानि_p (lookaheads[i]))
                (*चरमः_statep)->lookaheads[i] = रिक्त;
              अन्यथा
                {
                  (*चरमः_statep)->lookaheads[i] =
                    bitset_create (ntokens, BITSET_FIXED);
                  bitset_copy ((*चरमः_statep)->lookaheads[i], lookaheads[i]);
                }
            }
        }
      (*चरमः_statep)->lr0Isocore = lr0_isocore;
      (*चरमः_statep)->अग्र_Isocore = old_isocore;
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
स्थिर शून्य
ielr_विभज_अवस्थाः (bitsetv follow_kernel_वस्तूनि, bitsetv always_follows,
                   AnnotationList **annotation_lists,
                   Annotation_सूचकांक max_annotations)
{
  state_list *first_state;
  state_list *चरमः_state;
  bitsetv lookahead_filter = रिक्त;
  bitsetv lookaheads;

  /* Set up state list and some reusable bitsets.  */
  {
    size_t max_nitems = 0;
    state_list **nodep = &first_state;
    प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
      {
        *nodep = अवस्थाः[i]->state_list = चरमः_state = xmalloc (परिमाण **nodep);
        (*nodep)->state = अवस्थाः[i];
        (*nodep)->recomputedAsSuccessor = false;
        (*nodep)->lookaheads = रिक्त;
        (*nodep)->lr0Isocore = *nodep;
        (*nodep)->अग्र_Isocore = *nodep;
        nodep = &(*nodep)->अग्र;
        यदि (अवस्थाः[i]->nitems > max_nitems)
          max_nitems = अवस्थाः[i]->nitems;
      }
    *nodep = रिक्त;
    lookahead_filter = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
    यदि (!annotation_lists)
      bitsetv_ones (lookahead_filter);
    lookaheads = bitsetv_create (max_nitems, ntokens, BITSET_FIXED);
  }

  /* Recompute states.  */
  {
    Contribution_सूचकांक *work = xnmalloc (max_annotations, परिमाण *work);
    प्रति (state_list *this_state = first_state;
         this_state;
         this_state = this_state->अग्र)
      {
        ध्रुव state *s = this_state->state;
        प्रति (पूर्णाङ्क i = 0; i < s->transitions->अङ्कः; ++i)
          {
            state *t = s->transitions->अवस्थाः[i];
            यदि (annotation_lists)
              AnnotationList__computeLookaheadFilter (
                annotation_lists[t->state_list->lr0Isocore->state->सङ्ख्या],
                t->nitems, lookahead_filter);
            ielr_compute_lookaheads (follow_kernel_वस्तूनि, always_follows,
                                     this_state, t, lookahead_filter,
                                     lookaheads);
            ielr_compute_state (follow_kernel_वस्तूनि, always_follows,
                                annotation_lists, t, lookaheads, &चरमः_state,
                                work, lookahead_filter,
                                &s->transitions->अवस्थाः[i]);
          }
      }
    मुक्त (work);
  }

  bitsetv_free (lookahead_filter);
  bitsetv_free (lookaheads);

  /* Store states back in the states array.  */
  अवस्थाः = xnrealloc (अवस्थाः, nstates, परिमाण *अवस्थाः);
  प्रति (state_list *ग्रन्थिः = first_state; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र)
    अवस्थाः[ग्रन्थिः->state->सङ्ख्या] = ग्रन्थिः->state;

  /* In the case of canonical LR(1), copy item lookahead sets to reduction
     lookahead sets.  */
  यदि (!annotation_lists)
    {
      timevar_प्रक्षिप (tv_ielr_phase4);
      initialize_LA ();
      प्रति (state_list *ग्रन्थिः = first_state; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र)
        यदि (!ग्रन्थिः->state->consistent)
          {
            size_t i = 0;
            item_सूचकांक *itemset = ग्रन्थिः->state->वस्तूनि;
            प्रति (size_t r = 0; r < ग्रन्थिः->state->reductions->अङ्कः; ++r)
              {
                नियमः *this_नियमः = ग्रन्थिः->state->reductions->नियमाः[r];
                bitset lookahead_समुच्चयः =
                  ग्रन्थिः->state->reductions->lookaheads[r];
                यदि (item_सङ्ख्या_is_नियमः_सङ्ख्या (*this_नियमः->rhs))
                  ielr_compute_goto_follow_समुच्चयः (follow_kernel_वस्तूनि,
                                                always_follows, ग्रन्थिः,
                                                this_नियमः->lhs, lookahead_समुच्चयः);
                अन्यथा यदि (ग्रन्थिः->lookaheads)
                  {
                    /* We don't need to start the kernel item search back at
                       i=0 because both items and reductions are sorted on rule
                       number.  */
                    यावत् (!item_सङ्ख्या_is_नियमः_सङ्ख्या (ritem[itemset[i]])
                           || item_सङ्ख्या_as_नियमः_सङ्ख्या (ritem[itemset[i]])
                              != this_नियमः->सङ्ख्या)
                      {
                        ++i;
                        aver (i < ग्रन्थिः->state->nitems);
                      }
                    यदि (ग्रन्थिः->lookaheads[i])
                      bitset_copy (lookahead_समुच्चयः, ग्रन्थिः->lookaheads[i]);
                  }
              }
          }
      timevar_निष्कासय (tv_ielr_phase4);
    }

  /* Free state list.  */
  यावत् (first_state)
    {
      state_list *ग्रन्थिः = first_state;
      यदि (ग्रन्थिः->lookaheads)
        {
          प्रति (size_t i = 0; i < ग्रन्थिः->state->nitems; ++i)
            यदि (ग्रन्थिः->lookaheads[i])
              bitset_free (ग्रन्थिः->lookaheads[i]);
          मुक्त (ग्रन्थिः->lookaheads);
        }
      first_state = ग्रन्थिः->अग्र;
      मुक्त (ग्रन्थिः);
    }
}


शून्य
ielr (शून्य)
{
  LrType lr_type = lr_type_get ();

  /* Phase 0: LALR(1).  */
  विकल्प (lr_type)
    {
    पक्ष LR_TYPE__LR0:
      timevar_प्रक्षिप (tv_lalr);
      समुच्चयः_goto_map ();
      timevar_निष्कासय (tv_lalr);
      प्रत्यागम;

    पक्ष LR_TYPE__CANONICAL_LR:
      timevar_प्रक्षिप (tv_lalr);
      समुच्चयः_goto_map ();
      timevar_निष्कासय (tv_lalr);
      भङ्ग;

    पक्ष LR_TYPE__LALR:
      timevar_प्रक्षिप (tv_lalr);
      lalr ();
      bitsetv_free (goto_follows);
      timevar_निष्कासय (tv_lalr);
      प्रत्यागम;

    पक्ष LR_TYPE__IELR:
      timevar_प्रक्षिप (tv_lalr);
      lalr ();
      timevar_निष्कासय (tv_lalr);
      भङ्ग;
    }

  {
    bitsetv follow_kernel_वस्तूनि;
    bitsetv always_follows;
    InadequacyList **inadequacy_lists = रिक्त;
    AnnotationList **annotation_lists = रिक्त;
    संरचना obstack annotations_obstack;
    Annotation_सूचकांक max_annotations = 0;

    {
      /* Phase 1: Compute Auxiliary Tables.  */
      state ***predecessors;
      timevar_प्रक्षिप (tv_ielr_phase1);
      ielr_compute_auxiliary_tables (
        &follow_kernel_वस्तूनि, &always_follows,
        lr_type == LR_TYPE__CANONICAL_LR ? रिक्त : &predecessors);
      timevar_निष्कासय (tv_ielr_phase1);

      /* Phase 2: Compute Annotations.  */
      timevar_प्रक्षिप (tv_ielr_phase2);
      यदि (lr_type != LR_TYPE__CANONICAL_LR)
        {
          obstack_init (&annotations_obstack);
          ielr_compute_annotation_lists (follow_kernel_वस्तूनि, always_follows,
                                         predecessors, &max_annotations,
                                         &inadequacy_lists, &annotation_lists,
                                         &annotations_obstack);
          प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
            मुक्त (predecessors[i]);
          मुक्त (predecessors);
          bitsetv_free (goto_follows);
          lalr_free ();
        }
      timevar_निष्कासय (tv_ielr_phase2);
    }

    /* Phase 3: Split States.  */
    timevar_प्रक्षिप (tv_ielr_phase3);
    {
      state_सङ्ख्या nstates_lr0 = nstates;
      ielr_विभज_अवस्थाः (follow_kernel_वस्तूनि, always_follows,
                         annotation_lists, max_annotations);
      यदि (inadequacy_lists)
        प्रति (state_सङ्ख्या i = 0; i < nstates_lr0; ++i)
          InadequacyList__delete (inadequacy_lists[i]);
    }
    मुक्त (inadequacy_lists);
    यदि (annotation_lists)
      obstack_free (&annotations_obstack, रिक्त);
    मुक्त (annotation_lists);
    bitsetv_free (follow_kernel_वस्तूनि);
    bitsetv_free (always_follows);
    timevar_निष्कासय (tv_ielr_phase3);
  }

  /* Phase 4: Compute Reduction Lookaheads.  */
  timevar_प्रक्षिप (tv_ielr_phase4);
  मुक्त (goto_map);
  मुक्त (from_state);
  मुक्त (गन्तव्यम्_state);
  यदि (lr_type == LR_TYPE__CANONICAL_LR)
    {
      /* Reduction lookaheads are computed in ielr_split_states above
         but are timed as part of phase 4. */
      समुच्चयः_goto_map ();
    }
  अन्यथा
    {
      lalr ();
      bitsetv_free (goto_follows);
    }
  timevar_निष्कासय (tv_ielr_phase4);
}
