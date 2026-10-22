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
AnnotationList__alloc_on_obstack (Contribution_اشاريو contribution_ڳڻپ,
                                  بنية obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_سائيز = contribution_ڳڻپ * حجم res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_سائيز);
  res->ايندڙ = عدم;
  res->inadequacy_ڳنڍ = عدم;
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
                                      Contribution_اشاريو ci)
{
  aver (0 <= ci && ci < self->inadequacy_ڳنڍ->contribution_ڳڻپ_2);
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
Annotation_List_وجهو_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **ڳنڍ;
  لكل (ڳنڍ = list; *ڳنڍ; ڳنڍ = &(*ڳنڍ)->ايندڙ)
    {
      صحيح cmp = 0;
      إذا (self->inadequacy_ڳنڍ->id < (*ڳنڍ)->inadequacy_ڳنڍ->id)
        cmp = 1;
      وإلا إذا ((*ڳنڍ)->inadequacy_ڳنڍ->id < self->inadequacy_ڳنڍ->id)
        cmp = -1;
      وإلا
        لكل (Contribution_اشاريو ci = 0;
             cmp == 0 && ci < self->inadequacy_ڳنڍ->contribution_ڳڻپ_2;
             ++ci)
          {
            إذا (AnnotationList__isContributionAlways (self, ci))
              {
                إذا (!AnnotationList__isContributionAlways (*ڳنڍ, ci))
                  cmp = -1;
              }
            وإلا إذا (AnnotationList__isContributionAlways (*ڳنڍ, ci))
              cmp = 1;
            وإلا
              لكل (size_t شي = 0; cmp == 0 && شي < nitems; ++شي)
                {
                  إذا (!Sbitset__test (self->contributions[ci], شي))
                    {
                      إذا (Sbitset__test ((*ڳنڍ)->contributions[ci], شي))
                        cmp = -1;
                    }
                  وإلا إذا (!Sbitset__test ((*ڳنڍ)->contributions[ci], شي))
                    cmp = 1;
                }
          }
      إذا (cmp < 0)
        {
          self->ايندڙ = *ڳنڍ;
          *ڳنڍ = self;
          اكسر;
        }
      وإلا إذا (cmp == 0)
        {
          self = عدم;
          اكسر;
        }
    }
  إذا (!*ڳنڍ)
    *ڳنڍ = self;
  أرجع self != عدم;
}

ساكن bitset
Annotation_List_compute_سرڪايو_ٽوڪن (transitions *trans)
{
  bitset سرڪايو_ٽوڪن = bitset_create (ntokens, BITSET_FIXED);
  صحيح i;
  FOR_EACH_سرڪايو (trans, i)
    bitset_سيٽ (سرڪايو_ٽوڪن, TRANSITION_نشان (trans, i));
  أرجع سرڪايو_ٽوڪن;
}

ساكن bitset
Annotation_List_compute_conflicted_ٽوڪن (bitset سرڪايو_ٽوڪن,
                                           reductions *reds)
{
  bitset conflicted_ٽوڪن = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_ٽوڪن_قاعدو = bitset_create (ntokens, BITSET_FIXED);
  bitset ٽوڪن = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (ٽوڪن, سرڪايو_ٽوڪن);
  لكل (صحيح i = 0; i < reds->انگ; ++i)
    {
      bitset_and (conflicted_ٽوڪن_قاعدو, ٽوڪن, reds->lookaheads[i]);
      bitset_or (conflicted_ٽوڪن,
                 conflicted_ٽوڪن, conflicted_ٽوڪن_قاعدو);
      bitset_or (ٽوڪن, ٽوڪن, reds->lookaheads[i]);
      /* Check that rules are sorted on rule number or the next step in
         AnnotationList__compute_from_inadequacies will misbehave.  */
      aver (i == 0 || reds->قاعدا[i-1] < reds->قاعدا[i]);
    }

  bitset_free (ٽوڪن);
  bitset_free (conflicted_ٽوڪن_قاعدو);

  أرجع conflicted_ٽوڪن;
}

ساكن bool
AnnotationList__compute_lhs_contributions (state *s, ثابت قاعدو *the_قاعدو,
                                           نشان_عدد conflicted_token,
                                           bitsetv follow_kernel_شيون,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **شي_lookahead_sets,
                                           Sbitset *شيون,
                                           بنية obstack
                                             *annotations_obstackp)
{
  goto_عدد lhs_goto = map_goto (s->عدد, the_قاعدو->lhs->عدد);
  إذا (bitset_test (always_follows[lhs_goto], conflicted_token))
    أرجع true;
  *شيون = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_شي;
    bitset_bindex شي;
    BITSET_FOR_EACH (biter_شي, follow_kernel_شيون[lhs_goto], شي, 0)
      إذا (ielr_شي_has_lookahead (s, 0, شي, conflicted_token,
                                   predecessors, شي_lookahead_sets))
        Sbitset_سيٽ (*شيون, شي);
  }
  أرجع false;
}

ساكن فراغ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_شيون,
  bitsetv always_follows,
  state ***predecessors,
  bitset **شي_lookahead_sets,
  AnnotationList **annotation_lists,
  Annotation_اشاريو *annotation_counts,
  بنية obstack *annotations_obstackp)
{
  لكل (state **predecessor = predecessors[s->عدد]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_ڳنڍ =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_ڳنڍ->contribution_ڳڻپ_2, annotations_obstackp);
      annotation_ڳنڍ->inadequacy_ڳنڍ = self->inadequacy_ڳنڍ;
      bool potential_contribution = false;
      bitset *lookaheads = عدم;

      لكل (Contribution_اشاريو ci = 0; ci < self->inadequacy_ڳنڍ->contribution_ڳڻپ_2; ++ci)
        {
          نشان_عدد contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_ڳنڍ, ci)
            ->content->عدد;
          إذا (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_ڳنڍ->contributions[ci] = عدم;
              استمر;
            }
          annotation_ڳنڍ->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            size_t predecessor_شي = 0;
            Sbitset sbiter_شي;
            Sbitset_اشاريو self_شي;
            SBITSET__FOR_EACH (self->contributions[ci], s->nitems,
                               sbiter_شي, self_شي)
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
                aver (s->شيون[self_شي] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                إذا (شي_عدد_is_قاعدو_عدد (ritem[s->شيون[self_شي] - 2]))
                  {
                    Sbitset شيون;
                    إذا (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          شي_قاعدو (&ritem[s->شيون[self_شي]]),
                          contribution_token,
                          follow_kernel_شيون, always_follows, predecessors,
                          شي_lookahead_sets, &شيون, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_ڳنڍ->contributions[ci]);
                        annotation_ڳنڍ->contributions[ci] = عدم;
                        // "Break" out of SBITSET__FOR_EACH.
                        اذهب after_sbitset__for_each;
                      }
                    وإلا
                      {
                        Sbitset__or (annotation_ڳنڍ->contributions[ci],
                                     annotation_ڳنڍ->contributions[ci],
                                     شيون, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, شيون);
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
                         predecessor_شي < (*predecessor)->nitems;
                         ++predecessor_شي)
                      إذا ((*predecessor)->شيون[predecessor_شي]
                          == s->شيون[self_شي] - 1)
                        اكسر;
                    aver (predecessor_شي != (*predecessor)->nitems);
                    إذا (ielr_شي_has_lookahead (*predecessor, 0,
                                                 predecessor_شي,
                                                 contribution_token,
                                                 predecessors,
                                                 شي_lookahead_sets))
                      Sbitset_سيٽ (annotation_ڳنڍ->contributions[ci],
                                    predecessor_شي);
                  }
              }
          after_sbitset__for_each:;
          }
          إذا (annotation_ڳنڍ->contributions[ci])
            {
              Sbitset biter;
              Sbitset_اشاريو i;
              SBITSET__FOR_EACH (annotation_ڳنڍ->contributions[ci],
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
                  bitset_سيٽ (lookaheads[i], contribution_token);
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
          إذا (Contribution_اشاريو_none
              != AnnotationList__computeDominantContribution (
                   annotation_ڳنڍ, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_ڳنڍ);
              annotation_ڳنڍ = عدم;
            }
          {
            لكل (size_t i = 0; i < (*predecessor)->nitems; ++i)
              إذا (lookaheads[i])
                bitset_free (lookaheads[i]);
            حرر (lookaheads);
          }
          إذا (annotation_ڳنڍ)
            {
              إذا (Annotation_List_وجهو_Into (annotation_ڳنڍ,
                                              &annotation_lists[(*predecessor)
                                                                ->عدد],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->عدد];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_ڳنڍ, *predecessor,
                    follow_kernel_شيون, always_follows, predecessors,
                    شي_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              وإلا
                obstack_free (annotations_obstackp, annotation_ڳنڍ);
            }
        }
      وإلا
        obstack_free (annotations_obstackp, annotation_ڳنڍ);
    }
}

فراغ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_شيون, bitsetv always_follows,
  state ***predecessors, bitset **شي_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  Annotation_اشاريو *annotation_counts,
  Contribution_اشاريو *max_contributionsp,
  بنية obstack *annotations_obstackp,
  Inadequacy_List_ڳنڍ_ڳڻپ *inadequacy_list_ڳنڍ_ڳڻپ)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  إذا (s->consistent)
    أرجع;

  bitsetv all_lookaheads = bitsetv_create (s->nitems, ntokens, BITSET_FIXED);
  bitsetv_ones (all_lookaheads);
  bitset سرڪايو_ٽوڪن = Annotation_List_compute_سرڪايو_ٽوڪن (s->transitions);
  bitset conflicted_ٽوڪن =
    Annotation_List_compute_conflicted_ٽوڪن (سرڪايو_ٽوڪن, s->reductions);

  /* Add an inadequacy annotation for each conflicted_token.  */
  bitset_iterator biter_conflict;
  bitset_bindex conflicted_token;
  BITSET_FOR_EACH (biter_conflict, conflicted_ٽوڪن, conflicted_token, 0)
    {
      AnnotationList *annotation_ڳنڍ;
      Contribution_اشاريو contribution_ڳڻپ = 0;

      /* Allocate the annotation node.  */
      {
        لكل (صحيح قاعدو_i = 0; قاعدو_i < s->reductions->انگ; ++قاعدو_i)
          إذا (bitset_test (s->reductions->lookaheads[قاعدو_i],
                           conflicted_token))
            ++contribution_ڳڻپ;
        إذا (bitset_test (سرڪايو_ٽوڪن, conflicted_token))
          ++contribution_ڳڻپ;
        annotation_ڳنڍ =
          AnnotationList__alloc_on_obstack (contribution_ڳڻپ,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->انگ + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        Contribution_اشاريو ci = 0;
        صحيح شي_i = 0;
        لكل (صحيح قاعدو_i = 0; قاعدو_i < s->reductions->انگ; ++قاعدو_i)
          {
            قاعدو *the_قاعدو = s->reductions->قاعدا[قاعدو_i];
            إذا (bitset_test (s->reductions->lookaheads[قاعدو_i],
                             conflicted_token))
              {
                bitset_سيٽ (actions, قاعدو_i);
                /* If this reduction is on a kernel item, just add it.  */
                إذا (!شي_عدد_is_قاعدو_عدد (the_قاعدو->rhs[0]))
                  {
                    annotation_ڳنڍ->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    طالما (!شي_عدد_is_قاعدو_عدد (ritem[s->شيون[شي_i]])
                           || شي_عدد_as_قاعدو_عدد (ritem[s->شيون[شي_i]]) != the_قاعدو->عدد)
                      {
                        ++شي_i;
                        aver (شي_i < s->nitems);
                      }
                    Sbitset_سيٽ (annotation_ڳنڍ->contributions[ci], شي_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                وإلا إذا (AnnotationList__compute_lhs_contributions (
                           s, the_قاعدو, conflicted_token, follow_kernel_شيون,
                           always_follows, predecessors, شي_lookahead_sets,
                           &annotation_ڳنڍ->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_ڳنڍ->contributions[ci++] = عدم;
                    استمر;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset_is_خالي_جايون (annotation_ڳنڍ->contributions[ci],
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
          إذا (bitset_test (سرڪايو_ٽوڪن, conflicted_token))
            {
              bitset_سيٽ (actions, s->reductions->انگ);
              annotation_ڳنڍ->contributions[contribution_ڳڻپ - 1] = عدم;
            }
          {
            InadequacyList *conflict_ڳنڍ =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_ڳنڍ_ڳڻپ);
            actions = عدم;
            annotation_ڳنڍ->inadequacy_ڳنڍ = conflict_ڳنڍ;
            إذا (Contribution_اشاريو_none
                != AnnotationList__computeDominantContribution (
                     annotation_ڳنڍ, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_ڳنڍ);
                InadequacyList__delete (conflict_ڳنڍ);
              }
            وإلا
              {
                Inadequacy_List_prepend_منزل (conflict_ڳنڍ,
                                           &inadequacy_lists[s->عدد]);
                {
                  bool b =
                    Annotation_List_وجهو_Into (annotation_ڳنڍ,
                                                &annotation_lists[s->عدد],
                                                s->nitems);
                  aver (b); (فراغ) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (سرڪايو_ٽوڪن, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->عدد];
                إذا (contribution_ڳڻپ > *max_contributionsp)
                  *max_contributionsp = contribution_ڳڻپ;
                AnnotationList__computePredecessorAnnotations (
                  annotation_ڳنڍ, s,
                  follow_kernel_شيون, always_follows, predecessors,
                  شي_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      وإلا
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_ڳنڍ);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (سرڪايو_ٽوڪن);
  bitset_free (conflicted_ٽوڪن);
}

فراغ
AnnotationList__debug (AnnotationList ثابت *self, size_t nitems, صحيح spaces)
{
  AnnotationList ثابت *a;
  Annotation_اشاريو ai;
  لكل (a = self, ai = 0; a; a = a->ايندڙ, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_ڳنڍ->manifestingState->عدد);
      bitset_bindex rulei
        = bitset_پهريون (a->inadequacy_ڳنڍ->inadequacy.conflict.actions);
      لكل (Contribution_اشاريو ci = 0; ci < a->inadequacy_ڳنڍ->contribution_ڳڻپ_2; ++ci)
        {
          نشان_عدد token =
            InadequacyList__getContributionToken (a->inadequacy_ڳنڍ, ci)
            ->content->عدد;
          fprintf (stderr, "%*s", spaces+2, "");
          إذا (ci == Inadequacy_List_get_سرڪايو_Contribution_اشاريو (
                                                               a->inadequacy_ڳنڍ))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          وإلا
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_ڳنڍ->manifestingState
                       ->reductions->قاعدا[rulei]->عدد);
              rulei =
                bitset_ايندڙ (a->inadequacy_ڳنڍ->inadequacy.conflict.actions,
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
  لكل (; self; self = self->ايندڙ)
    لكل (Contribution_اشاريو ci = 0; ci < self->inadequacy_ڳنڍ->contribution_ڳڻپ_2; ++ci)
      إذا (!AnnotationList__isContributionAlways (self, ci))
        {
          نشان_عدد token =
            InadequacyList__getContributionToken (self->inadequacy_ڳنڍ, ci)
            ->content->عدد;
          Sbitset_اشاريو شي;
          Sbitset biter;
          SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, شي)
            bitset_سيٽ (lookahead_filter[شي], token);
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
                                        size_t nitems, Contribution_اشاريو ci,
                                        bitset *lookaheads)
{
  إذا (AnnotationList__isContributionAlways (self, ci))
    أرجع true;
  إذا (!lookaheads)
    أرجع false;
  {
    نشان_عدد token =
      InadequacyList__getContributionToken (self->inadequacy_ڳنڍ, ci)
      ->content->عدد;
    Sbitset_اشاريو شي;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, شي)
      إذا (lookaheads[شي] && bitset_test (lookaheads[شي], token))
        أرجع true;
  }
  أرجع false;
}

Contribution_اشاريو
AnnotationList__computeDominantContribution (AnnotationList ثابت *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_ورهايو_stable)
{
  Contribution_اشاريو ثابت ci_سرڪايو =
    Inadequacy_List_get_سرڪايو_Contribution_اشاريو (self->inadequacy_ڳنڍ);

  نشان *token = self->inadequacy_ڳنڍ->inadequacy.conflict.token;

  /* S/R conflict.  */
  إذا (ci_سرڪايو != Contribution_اشاريو_none)
    {
      bool ڳوليو_stable_domination_over_سرڪايو = false;
      bool ڳوليو_stable_error_action_domination = false;
      {
        صحيح سرڪايو_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        إذا (!سرڪايو_precedence)
          أرجع ci_سرڪايو;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        Contribution_اشاريو ci_rr_dominator = Contribution_اشاريو_none;
        صحيح actioni;
        Contribution_اشاريو ci;
        لكل (ci = 0,
               actioni = bitset_پهريون (self->inadequacy_ڳنڍ->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_ڳنڍ->contribution_ڳڻپ_2;
             ++ci,
               actioni = bitset_ايندڙ (self->inadequacy_ڳنڍ->inadequacy
                                      .conflict.actions, actioni+1))
          {
            صحيح reduce_precedence = 0;
            إذا (ci == ci_سرڪايو)
              استمر;
            {
              قاعدو *r = self->inadequacy_ڳنڍ->manifestingState
                          ->reductions->قاعدا[actioni];
              إذا (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            إذا (reduce_precedence
                && (reduce_precedence < سرڪايو_precedence
                    || (reduce_precedence == سرڪايو_precedence
                        && token->content->assoc == ساڄو_assoc)))
              استمر;
            إذا (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              استمر;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            إذا (reduce_precedence == سرڪايو_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                إذا (ڳوليو_stable_domination_over_سرڪايو)
                  أرجع Contribution_اشاريو_none;
                إذا (!require_ورهايو_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  أرجع Contribution_اشاريو_error_action;
                ڳوليو_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            إذا (ci_rr_dominator == Contribution_اشاريو_none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            إذا (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                إذا (ڳوليو_stable_error_action_domination)
                  أرجع Contribution_اشاريو_none;
                إذا (!require_ورهايو_stable)
                  أرجع ci_rr_dominator;
                إذا (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  أرجع Contribution_اشاريو_none;
                إذا (AnnotationList__isContributionAlways (self, ci))
                  أرجع ci_rr_dominator;
                ڳوليو_stable_domination_over_سرڪايو = true;
              }
          }
      }
      إذا (ڳوليو_stable_domination_over_سرڪايو
          || ڳوليو_stable_error_action_domination)
        أرجع Contribution_اشاريو_none;
      /* No reduce or error action domination found, so shift dominates.  */
      أرجع ci_سرڪايو;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  لكل (Contribution_اشاريو ci = 0; ci < self->inadequacy_ڳنڍ->contribution_ڳڻپ_2; ++ci)
    إذا (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        إذا (require_ورهايو_stable
            && !AnnotationList__isContributionAlways (self, ci))
          أرجع Contribution_اشاريو_none;
        أرجع ci;
      }
  أرجع Contribution_اشاريو_none;
}
