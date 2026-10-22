/* IELR's inadequacy annotation list.

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

#ضمن "AnnotationList.h"

#ضمن "system.h"

#ضمن "ielr.h"
#ضمن "lalr.h"

/**
 * \pre
 *   - <tt>annotations_obstackp != NULL</tt>.
 * \post
 *   - \c result is a new \c AnnotationList with one node whose:
 *     - \c inadequacyNode member is \c NULL.
 *     - \c contributions member is allocated with \c contribution_count
 *       uninitialized elements.
 *   - All memory was allocated on \c annotations_obstackp.
 */
ساكن AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_كدي,
                                  بنية obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_كدي * حجم res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = عدم;
  res->inadequacy_كلي = عدم;
  أرجع res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
ساكن bool
AnnotationList__isContributionAlways (AnnotationList ثابت *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_كلي->contribution_كدي_2);
  أرجع self->contributions[ci] == عدم;
}

/**
 * \pre
 *   - \c self is a single node.
 *   - \c self annotates the same state as every other node in \c list, and
 *     that state has \c nitems kernel items.
 * \post
 *   - If the list \c list already contains an identical annotation to \c self,
 *     \c self was discarded, \c result is false, and the caller is responsible
 *     for the memory of \c self.
 *   - Otherwise, \c list now contains the node \c self, \c result is true, and
 *     \c list assumes responsibility for the memory of \c self.
 *   - The sort in \c list is:
 *     - Sort in reverse order on the unique ID of the associated
 *       inadequacy node.  Because these IDs are assigned in ascending
 *       order, this should mean that the insertion position within an
 *       annotation list is usually near the beginning with other
 *       annotations associated with the same inadequacy.
 *     - Next, sort on the first contribution that is different as follows:
 *       - Sort an always-contribution before a never-contribution before a
 *         potential-contribution.
 *       - Two always-contributions are identical.
 *       - Two never-contributions are identical.
 *       - For two potential-contributions, sort on the contributions' kernel
 *         item bitsets interpreted as binary numbers.
 *  - The sorting has a few effects:
 *    - It accelerates elimination of identical annotations during insertion.
 *    - It determines how the output of \c AnnotationList__debug is sorted.
 *    - Other than that, it's probably not important.
 */
ساكن bool
Annotation_List_سك_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **كلي;
  لكل (كلي = list; *كلي; كلي = &(*كلي)->next)
    {
      صحيح cmp = 0;
      إذا (self->inadequacy_كلي->id < (*كلي)->inadequacy_كلي->id)
        cmp = 1;
      وإلا إذا ((*كلي)->inadequacy_كلي->id < self->inadequacy_كلي->id)
        cmp = -1;
      وإلا
        لكل (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_كلي->contribution_كدي_2;
             ++ci)
          {
            إذا (AnnotationList__isContributionAlways (self, ci))
              {
                إذا (!AnnotationList__isContributionAlways (*كلي, ci))
                  cmp = -1;
              }
            وإلا إذا (AnnotationList__isContributionAlways (*كلي, ci))
              cmp = 1;
            وإلا
              لكل (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  إذا (!Sbitset__test (self->contributions[ci], item))
                    {
                      إذا (Sbitset__test ((*كلي)->contributions[ci], item))
                        cmp = -1;
                    }
                  وإلا إذا (!Sbitset__test ((*كلي)->contributions[ci], item))
                    cmp = 1;
                }
          }
      إذا (cmp < 0)
        {
          self->next = *كلي;
          *كلي = self;
          اكسر;
        }
      وإلا إذا (cmp == 0)
        {
          self = عدم;
          اكسر;
        }
    }
  إذا (!*كلي)
    *كلي = self;
  أرجع self != عدم;
}

ساكن bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  صحيح i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_علام (trans, i));
  أرجع shift_tokens;
}

ساكن bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  لكل (صحيح i = 0; i < reds->لمب; ++i)
    {
      bitset_and (conflicted_tokens_rule, tokens, reds->lookaheads[i]);
      bitset_or (conflicted_tokens,
                 conflicted_tokens, conflicted_tokens_rule);
      bitset_or (tokens, tokens, reds->lookaheads[i]);
      /* Check that rules are sorted on rule number or the next step in
         AnnotationList__compute_from_inadequacies will misbehave.  */
      aver (i == 0 || reds->rules[i-1] < reds->rules[i]);
    }

  bitset_free (tokens);
  bitset_free (conflicted_tokens_rule);

  أرجع conflicted_tokens;
}

ساكن bool
AnnotationList__compute_lhs_contributions (state *s, ثابت rule *the_rule,
                                           علام_لمب conflicted_token,
                                           bitsetv follow_kernel_ابوبو,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *ابوبو,
                                           بنية obstack
                                             *annotations_obstackp)
{
  goto_لمب lhs_goto = map_goto (s->لمب_2, the_rule->lhs->لمب_2);
  إذا (bitset_test (always_follows[lhs_goto], conflicted_token))
    أرجع true;
  *ابوبو = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_ابوبو[lhs_goto], item, 0)
      إذا (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*ابوبو, item);
  }
  أرجع false;
}

ساكن فراغ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_ابوبو,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  بنية obstack *annotations_obstackp)
{
  لكل (state **predecessor = predecessors[s->لمب_2]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_كلي =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_كلي->contribution_كدي_2, annotations_obstackp);
      annotation_كلي->inadequacy_كلي = self->inadequacy_كلي;
      bool potential_contribution = false;
      bitset *lookaheads = عدم;

      لكل (ContributionIndex ci = 0; ci < self->inadequacy_كلي->contribution_كدي_2; ++ci)
        {
          علام_لمب contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_كلي, ci)
            ->content->لمب_2;
          إذا (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_كلي->contributions[ci] = عدم;
              استمر;
            }
          annotation_كلي->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            size_t predecessor_item = 0;
            Sbitset sbiter_item;
            Sbitset__Index self_item;
            SBITSET__FOR_EACH (self->contributions[ci], s->nitems,
                               sbiter_item, self_item)
              {
                /* If this kernel item is the beginning of a RHS, it must be
                   the kernel item in the start state, and so it has an empty
                   lookahead set.  Thus, it can't contribute to inadequacies,
                   and so it should never have been identified as a
                   contribution.  If, instead, this kernel item is the
                   successor of the start state's kernel item, the lookahead
                   set is still empty, and so it also should never have been
                   identified as a contribution.  This situation is fortunate
                   because we want to avoid the - 2 below in both cases.  */
                aver (s->ابوبو[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                إذا (item_لمب_is_rule_لمب (ritem[s->ابوبو[self_item] - 2]))
                  {
                    Sbitset ابوبو;
                    إذا (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->ابوبو[self_item]]),
                          contribution_token,
                          follow_kernel_ابوبو, always_follows, predecessors,
                          item_lookahead_sets, &ابوبو, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_كلي->contributions[ci]);
                        annotation_كلي->contributions[ci] = عدم;
                        // "Break" out of SBITSET__FOR_EACH.
                        اذهب after_sbitset__for_each;
                      }
                    وإلا
                      {
                        Sbitset__or (annotation_كلي->contributions[ci],
                                     annotation_كلي->contributions[ci],
                                     ابوبو, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, ابوبو);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                وإلا
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    لكل (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      إذا ((*predecessor)->ابوبو[predecessor_item]
                          == s->ابوبو[self_item] - 1)
                        اكسر;
                    aver (predecessor_item != (*predecessor)->nitems);
                    إذا (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_كلي->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          إذا (annotation_كلي->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_كلي->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  إذا (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             حجم *lookaheads);
                      لكل (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = عدم;
                    }
                  إذا (!lookaheads[i])
                    lookaheads[i] = bitset_create (ntokens, BITSET_FIXED);
                  bitset_set (lookaheads[i], contribution_token);
                }
            }
        }

      /* If the predecessor has any contributions besides just "always" and
         "never" contributions:
         - If the dominant contribution is split-stable, the annotation could
           not affect merging on this predecessor state or its eventual
           predecessor states.  Moreover, all contributions that affect
           whether the dominant contribution remains dominant must be "always"
           or "never" contributions in order for the dominant contribution to
           be split-stable.  Thus, the dominant contribution computation result
           in eventual successor states will not be affected by lookaheads
           tracked for this predecessor state.  (Also, as in the isocore
           compatibility test, we depend on the fact that isocores with equal
           dominant contributions will have the same dominant contribution when
           merged.  Otherwise, we might have to worry that the presence of a
           potential contribution might somehow be the culprit of that behavior
           and thus need to be tracked regardless of the split stability of the
           dominant contribution.)  Thus, go ahead and discard the annotation
           to save space now plus time during state splitting.
         - Otherwise, record the annotation, and compute any resulting
           annotations needed on predecessor states.  */
      إذا (potential_contribution)
        {
          إذا (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_كلي, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_كلي);
              annotation_كلي = عدم;
            }
          {
            لكل (size_t i = 0; i < (*predecessor)->nitems; ++i)
              إذا (lookaheads[i])
                bitset_free (lookaheads[i]);
            حرر (lookaheads);
          }
          إذا (annotation_كلي)
            {
              إذا (Annotation_List_سك_Into (annotation_كلي,
                                              &annotation_lists[(*predecessor)
                                                                ->لمب_2],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->لمب_2];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_كلي, *predecessor,
                    follow_kernel_ابوبو, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              وإلا
                obstack_free (annotations_obstackp, annotation_كلي);
            }
        }
      وإلا
        obstack_free (annotations_obstackp, annotation_كلي);
    }
}

فراغ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_ابوبو, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  بنية obstack *annotations_obstackp,
  Inadequacy_List_كلي_كدي *inadequacy_list_كلي_كدي)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  إذا (s->consistent)
    أرجع;

  bitsetv all_lookaheads = bitsetv_create (s->nitems, ntokens, BITSET_FIXED);
  bitsetv_ones (all_lookaheads);
  bitset shift_tokens = AnnotationList__compute_shift_tokens (s->transitions);
  bitset conflicted_tokens =
    AnnotationList__compute_conflicted_tokens (shift_tokens, s->reductions);

  /* Add an inadequacy annotation for each conflicted_token.  */
  bitset_iterator biter_conflict;
  bitset_bindex conflicted_token;
  BITSET_FOR_EACH (biter_conflict, conflicted_tokens, conflicted_token, 0)
    {
      AnnotationList *annotation_كلي;
      ContributionIndex contribution_كدي = 0;

      /* Allocate the annotation node.  */
      {
        لكل (صحيح rule_i = 0; rule_i < s->reductions->لمب; ++rule_i)
          إذا (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_كدي;
        إذا (bitset_test (shift_tokens, conflicted_token))
          ++contribution_كدي;
        annotation_كلي =
          AnnotationList__alloc_on_obstack (contribution_كدي,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->لمب + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        صحيح item_i = 0;
        لكل (صحيح rule_i = 0; rule_i < s->reductions->لمب; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            إذا (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                إذا (!item_لمب_is_rule_لمب (the_rule->rhs[0]))
                  {
                    annotation_كلي->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    طالما (!item_لمب_is_rule_لمب (ritem[s->ابوبو[item_i]])
                           || item_لمب_as_rule_لمب (ritem[s->ابوبو[item_i]]) != the_rule->لمب_2)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_كلي->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                وإلا إذا (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_ابوبو,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_كلي->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_كلي->contributions[ci++] = عدم;
                    استمر;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_كلي->contributions[ci],
                                         s->nitems));
                ++ci;
                potential_contribution = true;
              }
          }
      }

      /* If there are any contributions besides just "always" contributions:
         - If there's also a shift contribution, record it.
         - If the dominant contribution is split-stable, then the annotation
           could not affect merging, so go ahead and discard the annotation and
           the inadequacy to save space now plus time during state splitting.
         - Otherwise, record the annotation and the inadequacy, and compute any
           resulting annotations needed on predecessor states.  */
      إذا (potential_contribution)
        {
          إذا (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->لمب);
              annotation_كلي->contributions[contribution_كدي - 1] = عدم;
            }
          {
            InadequacyList *conflict_كلي =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_كلي_كدي);
            actions = عدم;
            annotation_كلي->inadequacy_كلي = conflict_كلي;
            إذا (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_كلي, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_كلي);
                InadequacyList__delete (conflict_كلي);
              }
            وإلا
              {
                InadequacyList__prependTo (conflict_كلي,
                                           &inadequacy_lists[s->لمب_2]);
                {
                  bool b =
                    Annotation_List_سك_Into (annotation_كلي,
                                                &annotation_lists[s->لمب_2],
                                                s->nitems);
                  aver (b); (فراغ) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->لمب_2];
                إذا (contribution_كدي > *max_contributionsp)
                  *max_contributionsp = contribution_كدي;
                AnnotationList__computePredecessorAnnotations (
                  annotation_كلي, s,
                  follow_kernel_ابوبو, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      وإلا
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_كلي);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

فراغ
AnnotationList__debug (AnnotationList ثابت *self, size_t nitems, صحيح spaces)
{
  AnnotationList ثابت *a;
  AnnotationIndex ai;
  لكل (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_كلي->manifestingState->لمب_2);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_كلي->inadequacy.conflict.actions);
      لكل (ContributionIndex ci = 0; ci < a->inadequacy_كلي->contribution_كدي_2; ++ci)
        {
          علام_لمب token =
            InadequacyList__getContributionToken (a->inadequacy_كلي, ci)
            ->content->لمب_2;
          fprintf (stderr, "%*s", spaces+2, "");
          إذا (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_كلي))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          وإلا
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_كلي->manifestingState
                       ->reductions->rules[rulei]->لمب_2);
              rulei =
                bitset_next (a->inadequacy_كلي->inadequacy.conflict.actions,
                             rulei+1);
              إذا (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              وإلا
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

فراغ
AnnotationList__computeLookaheadFilter (AnnotationList ثابت *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  لكل (; self; self = self->next)
    لكل (ContributionIndex ci = 0; ci < self->inadequacy_كلي->contribution_كدي_2; ++ci)
      إذا (!AnnotationList__isContributionAlways (self, ci))
        {
          علام_لمب token =
            InadequacyList__getContributionToken (self->inadequacy_كلي, ci)
            ->content->لمب_2;
          Sbitset__Index item;
          Sbitset biter;
          SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
            bitset_set (lookahead_filter[item], token);
        }
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - \c nitems is the number of kernel items in the LR(0) state that \c self
 *     annotates.
 *   - \c lookaheads describes the lookahead sets on the kernel items of some
 *     isocore of the LR(0) state that \c self annotates.  Either:
 *     - <tt>lookaheads = NULL</tt> only if the lookahead set on every kernel
 *       item is empty.
 *     - For any <tt>0 <= i < nitems</tt>, <tt>lookaheads[i]</tt> is either:
 *       - \c NULL only if the lookahead set on kernel item \c i is empty.
 *       - The (possibly empty) lookahead set on kernel item \c i.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self is made by the state
 *     described by \c lookaheads.
 */
ساكن bool
AnnotationList__stateMakesContribution (AnnotationList ثابت *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  إذا (AnnotationList__isContributionAlways (self, ci))
    أرجع true;
  إذا (!lookaheads)
    أرجع false;
  {
    علام_لمب token =
      InadequacyList__getContributionToken (self->inadequacy_كلي, ci)
      ->content->لمب_2;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      إذا (lookaheads[item] && bitset_test (lookaheads[item], token))
        أرجع true;
  }
  أرجع false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList ثابت *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex ثابت ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_كلي);

  علام *token = self->inadequacy_كلي->inadequacy.conflict.token;

  /* S/R conflict.  */
  إذا (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        صحيح shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        إذا (!shift_precedence)
          أرجع ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        صحيح actioni;
        ContributionIndex ci;
        لكل (ci = 0,
               actioni = bitset_first (self->inadequacy_كلي->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_كلي->contribution_كدي_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_كلي->inadequacy
                                      .conflict.actions, actioni+1))
          {
            صحيح reduce_precedence = 0;
            إذا (ci == ci_shift)
              استمر;
            {
              rule *r = self->inadequacy_كلي->manifestingState
                          ->reductions->rules[actioni];
              إذا (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            إذا (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == دام_assoc)))
              استمر;
            إذا (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              استمر;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            إذا (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                إذا (find_stable_domination_over_shift)
                  أرجع ContributionIndex__none;
                إذا (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  أرجع ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            إذا (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            إذا (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                إذا (find_stable_error_action_domination)
                  أرجع ContributionIndex__none;
                إذا (!require_split_stable)
                  أرجع ci_rr_dominator;
                إذا (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  أرجع ContributionIndex__none;
                إذا (AnnotationList__isContributionAlways (self, ci))
                  أرجع ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      إذا (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        أرجع ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      أرجع ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  لكل (ContributionIndex ci = 0; ci < self->inadequacy_كلي->contribution_كدي_2; ++ci)
    إذا (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        إذا (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          أرجع ContributionIndex__none;
        أرجع ci;
      }
  أرجع ContributionIndex__none;
}
