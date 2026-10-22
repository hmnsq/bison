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
AnnotationList__alloc_on_obstack (Contribution_اشاریہ contribution_گنتی,
                                  بنية obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_ناپ = contribution_گنتی * حجم res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_ناپ);
  res->اگلا = عدم;
  res->inadequacy_گرہ = عدم;
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
                                      Contribution_اشاریہ ci)
{
  aver (0 <= ci && ci < self->inadequacy_گرہ->contribution_گنتی_2);
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
Annotation_List_داخل_کرو_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **گرہ;
  لكل (گرہ = list; *گرہ; گرہ = &(*گرہ)->اگلا)
    {
      صحيح cmp = 0;
      إذا (self->inadequacy_گرہ->id < (*گرہ)->inadequacy_گرہ->id)
        cmp = 1;
      وإلا إذا ((*گرہ)->inadequacy_گرہ->id < self->inadequacy_گرہ->id)
        cmp = -1;
      وإلا
        لكل (Contribution_اشاریہ ci = 0;
             cmp == 0 && ci < self->inadequacy_گرہ->contribution_گنتی_2;
             ++ci)
          {
            إذا (AnnotationList__isContributionAlways (self, ci))
              {
                إذا (!AnnotationList__isContributionAlways (*گرہ, ci))
                  cmp = -1;
              }
            وإلا إذا (AnnotationList__isContributionAlways (*گرہ, ci))
              cmp = 1;
            وإلا
              لكل (size_t شے = 0; cmp == 0 && شے < nitems; ++شے)
                {
                  إذا (!Sbitset__test (self->contributions[ci], شے))
                    {
                      إذا (Sbitset__test ((*گرہ)->contributions[ci], شے))
                        cmp = -1;
                    }
                  وإلا إذا (!Sbitset__test ((*گرہ)->contributions[ci], شے))
                    cmp = 1;
                }
          }
      إذا (cmp < 0)
        {
          self->اگلا = *گرہ;
          *گرہ = self;
          اكسر;
        }
      وإلا إذا (cmp == 0)
        {
          self = عدم;
          اكسر;
        }
    }
  إذا (!*گرہ)
    *گرہ = self;
  أرجع self != عدم;
}

ساكن bitset
Annotation_List_compute_کھسکاؤ_ٹوکن (transitions *trans)
{
  bitset کھسکاؤ_ٹوکن = bitset_create (ntokens, BITSET_FIXED);
  صحيح i;
  FOR_EACH_کھسکاؤ (trans, i)
    bitset_سیٹ (کھسکاؤ_ٹوکن, TRANSITION_علامت (trans, i));
  أرجع کھسکاؤ_ٹوکن;
}

ساكن bitset
Annotation_List_compute_conflicted_ٹوکن (bitset کھسکاؤ_ٹوکن,
                                           reductions *reds)
{
  bitset conflicted_ٹوکن = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_ٹوکن_قاعدہ = bitset_create (ntokens, BITSET_FIXED);
  bitset ٹوکن = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (ٹوکن, کھسکاؤ_ٹوکن);
  لكل (صحيح i = 0; i < reds->عدد; ++i)
    {
      bitset_and (conflicted_ٹوکن_قاعدہ, ٹوکن, reds->lookaheads[i]);
      bitset_or (conflicted_ٹوکن,
                 conflicted_ٹوکن, conflicted_ٹوکن_قاعدہ);
      bitset_or (ٹوکن, ٹوکن, reds->lookaheads[i]);
      /* Check that rules are sorted on rule number or the next step in
         AnnotationList__compute_from_inadequacies will misbehave.  */
      aver (i == 0 || reds->قاعدے[i-1] < reds->قاعدے[i]);
    }

  bitset_free (ٹوکن);
  bitset_free (conflicted_ٹوکن_قاعدہ);

  أرجع conflicted_ٹوکن;
}

ساكن bool
AnnotationList__compute_lhs_contributions (state *s, ثابت قاعدہ *the_قاعدہ,
                                           علامت_نمبر conflicted_token,
                                           bitsetv follow_kernel_اشیاء,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **شے_lookahead_sets,
                                           Sbitset *اشیاء,
                                           بنية obstack
                                             *annotations_obstackp)
{
  goto_نمبر lhs_goto = map_goto (s->نمبر, the_قاعدہ->lhs->نمبر);
  إذا (bitset_test (always_follows[lhs_goto], conflicted_token))
    أرجع true;
  *اشیاء = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_شے;
    bitset_bindex شے;
    BITSET_FOR_EACH (biter_شے, follow_kernel_اشیاء[lhs_goto], شے, 0)
      إذا (ielr_شے_has_lookahead (s, 0, شے, conflicted_token,
                                   predecessors, شے_lookahead_sets))
        Sbitset_سیٹ (*اشیاء, شے);
  }
  أرجع false;
}

ساكن فراغ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_اشیاء,
  bitsetv always_follows,
  state ***predecessors,
  bitset **شے_lookahead_sets,
  AnnotationList **annotation_lists,
  Annotation_اشاریہ *annotation_counts,
  بنية obstack *annotations_obstackp)
{
  لكل (state **predecessor = predecessors[s->نمبر]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_گرہ =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_گرہ->contribution_گنتی_2, annotations_obstackp);
      annotation_گرہ->inadequacy_گرہ = self->inadequacy_گرہ;
      bool potential_contribution = false;
      bitset *lookaheads = عدم;

      لكل (Contribution_اشاریہ ci = 0; ci < self->inadequacy_گرہ->contribution_گنتی_2; ++ci)
        {
          علامت_نمبر contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_گرہ, ci)
            ->content->نمبر;
          إذا (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_گرہ->contributions[ci] = عدم;
              استمر;
            }
          annotation_گرہ->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            size_t predecessor_شے = 0;
            Sbitset sbiter_شے;
            Sbitset_اشاریہ self_شے;
            SBITSET__FOR_EACH (self->contributions[ci], s->nitems,
                               sbiter_شے, self_شے)
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
                aver (s->اشیاء[self_شے] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                إذا (شے_نمبر_is_قاعدہ_نمبر (ritem[s->اشیاء[self_شے] - 2]))
                  {
                    Sbitset اشیاء;
                    إذا (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          شے_قاعدہ (&ritem[s->اشیاء[self_شے]]),
                          contribution_token,
                          follow_kernel_اشیاء, always_follows, predecessors,
                          شے_lookahead_sets, &اشیاء, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_گرہ->contributions[ci]);
                        annotation_گرہ->contributions[ci] = عدم;
                        // "Break" out of SBITSET__FOR_EACH.
                        اذهب after_sbitset__for_each;
                      }
                    وإلا
                      {
                        Sbitset__or (annotation_گرہ->contributions[ci],
                                     annotation_گرہ->contributions[ci],
                                     اشیاء, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, اشیاء);
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
                         predecessor_شے < (*predecessor)->nitems;
                         ++predecessor_شے)
                      إذا ((*predecessor)->اشیاء[predecessor_شے]
                          == s->اشیاء[self_شے] - 1)
                        اكسر;
                    aver (predecessor_شے != (*predecessor)->nitems);
                    إذا (ielr_شے_has_lookahead (*predecessor, 0,
                                                 predecessor_شے,
                                                 contribution_token,
                                                 predecessors,
                                                 شے_lookahead_sets))
                      Sbitset_سیٹ (annotation_گرہ->contributions[ci],
                                    predecessor_شے);
                  }
              }
          after_sbitset__for_each:;
          }
          إذا (annotation_گرہ->contributions[ci])
            {
              Sbitset biter;
              Sbitset_اشاریہ i;
              SBITSET__FOR_EACH (annotation_گرہ->contributions[ci],
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
                  bitset_سیٹ (lookaheads[i], contribution_token);
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
          إذا (Contribution_اشاریہ_none
              != AnnotationList__computeDominantContribution (
                   annotation_گرہ, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_گرہ);
              annotation_گرہ = عدم;
            }
          {
            لكل (size_t i = 0; i < (*predecessor)->nitems; ++i)
              إذا (lookaheads[i])
                bitset_free (lookaheads[i]);
            حرر (lookaheads);
          }
          إذا (annotation_گرہ)
            {
              إذا (Annotation_List_داخل_کرو_Into (annotation_گرہ,
                                              &annotation_lists[(*predecessor)
                                                                ->نمبر],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->نمبر];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_گرہ, *predecessor,
                    follow_kernel_اشیاء, always_follows, predecessors,
                    شے_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              وإلا
                obstack_free (annotations_obstackp, annotation_گرہ);
            }
        }
      وإلا
        obstack_free (annotations_obstackp, annotation_گرہ);
    }
}

فراغ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_اشیاء, bitsetv always_follows,
  state ***predecessors, bitset **شے_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  Annotation_اشاریہ *annotation_counts,
  Contribution_اشاریہ *max_contributionsp,
  بنية obstack *annotations_obstackp,
  Inadequacy_List_گرہ_گنتی *inadequacy_list_گرہ_گنتی)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  إذا (s->consistent)
    أرجع;

  bitsetv all_lookaheads = bitsetv_create (s->nitems, ntokens, BITSET_FIXED);
  bitsetv_ones (all_lookaheads);
  bitset کھسکاؤ_ٹوکن = Annotation_List_compute_کھسکاؤ_ٹوکن (s->transitions);
  bitset conflicted_ٹوکن =
    Annotation_List_compute_conflicted_ٹوکن (کھسکاؤ_ٹوکن, s->reductions);

  /* Add an inadequacy annotation for each conflicted_token.  */
  bitset_iterator biter_conflict;
  bitset_bindex conflicted_token;
  BITSET_FOR_EACH (biter_conflict, conflicted_ٹوکن, conflicted_token, 0)
    {
      AnnotationList *annotation_گرہ;
      Contribution_اشاریہ contribution_گنتی = 0;

      /* Allocate the annotation node.  */
      {
        لكل (صحيح قاعدہ_i = 0; قاعدہ_i < s->reductions->عدد; ++قاعدہ_i)
          إذا (bitset_test (s->reductions->lookaheads[قاعدہ_i],
                           conflicted_token))
            ++contribution_گنتی;
        إذا (bitset_test (کھسکاؤ_ٹوکن, conflicted_token))
          ++contribution_گنتی;
        annotation_گرہ =
          AnnotationList__alloc_on_obstack (contribution_گنتی,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->عدد + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        Contribution_اشاریہ ci = 0;
        صحيح شے_i = 0;
        لكل (صحيح قاعدہ_i = 0; قاعدہ_i < s->reductions->عدد; ++قاعدہ_i)
          {
            قاعدہ *the_قاعدہ = s->reductions->قاعدے[قاعدہ_i];
            إذا (bitset_test (s->reductions->lookaheads[قاعدہ_i],
                             conflicted_token))
              {
                bitset_سیٹ (actions, قاعدہ_i);
                /* If this reduction is on a kernel item, just add it.  */
                إذا (!شے_نمبر_is_قاعدہ_نمبر (the_قاعدہ->rhs[0]))
                  {
                    annotation_گرہ->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    طالما (!شے_نمبر_is_قاعدہ_نمبر (ritem[s->اشیاء[شے_i]])
                           || شے_نمبر_as_قاعدہ_نمبر (ritem[s->اشیاء[شے_i]]) != the_قاعدہ->نمبر)
                      {
                        ++شے_i;
                        aver (شے_i < s->nitems);
                      }
                    Sbitset_سیٹ (annotation_گرہ->contributions[ci], شے_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                وإلا إذا (AnnotationList__compute_lhs_contributions (
                           s, the_قاعدہ, conflicted_token, follow_kernel_اشیاء,
                           always_follows, predecessors, شے_lookahead_sets,
                           &annotation_گرہ->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_گرہ->contributions[ci++] = عدم;
                    استمر;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset_is_خالی_تھاواں (annotation_گرہ->contributions[ci],
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
          إذا (bitset_test (کھسکاؤ_ٹوکن, conflicted_token))
            {
              bitset_سیٹ (actions, s->reductions->عدد);
              annotation_گرہ->contributions[contribution_گنتی - 1] = عدم;
            }
          {
            InadequacyList *conflict_گرہ =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_گرہ_گنتی);
            actions = عدم;
            annotation_گرہ->inadequacy_گرہ = conflict_گرہ;
            إذا (Contribution_اشاریہ_none
                != AnnotationList__computeDominantContribution (
                     annotation_گرہ, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_گرہ);
                InadequacyList__delete (conflict_گرہ);
              }
            وإلا
              {
                Inadequacy_List_prepend_منزل (conflict_گرہ,
                                           &inadequacy_lists[s->نمبر]);
                {
                  bool b =
                    Annotation_List_داخل_کرو_Into (annotation_گرہ,
                                                &annotation_lists[s->نمبر],
                                                s->nitems);
                  aver (b); (فراغ) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (کھسکاؤ_ٹوکن, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->نمبر];
                إذا (contribution_گنتی > *max_contributionsp)
                  *max_contributionsp = contribution_گنتی;
                AnnotationList__computePredecessorAnnotations (
                  annotation_گرہ, s,
                  follow_kernel_اشیاء, always_follows, predecessors,
                  شے_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      وإلا
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_گرہ);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (کھسکاؤ_ٹوکن);
  bitset_free (conflicted_ٹوکن);
}

فراغ
AnnotationList__debug (AnnotationList ثابت *self, size_t nitems, صحيح spaces)
{
  AnnotationList ثابت *a;
  Annotation_اشاریہ ai;
  لكل (a = self, ai = 0; a; a = a->اگلا, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_گرہ->manifestingState->نمبر);
      bitset_bindex rulei
        = bitset_پہلا (a->inadequacy_گرہ->inadequacy.conflict.actions);
      لكل (Contribution_اشاریہ ci = 0; ci < a->inadequacy_گرہ->contribution_گنتی_2; ++ci)
        {
          علامت_نمبر token =
            InadequacyList__getContributionToken (a->inadequacy_گرہ, ci)
            ->content->نمبر;
          fprintf (stderr, "%*s", spaces+2, "");
          إذا (ci == Inadequacy_List_get_کھسکاؤ_Contribution_اشاریہ (
                                                               a->inadequacy_گرہ))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          وإلا
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_گرہ->manifestingState
                       ->reductions->قاعدے[rulei]->نمبر);
              rulei =
                bitset_اگلا (a->inadequacy_گرہ->inadequacy.conflict.actions,
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
  لكل (; self; self = self->اگلا)
    لكل (Contribution_اشاریہ ci = 0; ci < self->inadequacy_گرہ->contribution_گنتی_2; ++ci)
      إذا (!AnnotationList__isContributionAlways (self, ci))
        {
          علامت_نمبر token =
            InadequacyList__getContributionToken (self->inadequacy_گرہ, ci)
            ->content->نمبر;
          Sbitset_اشاریہ شے;
          Sbitset biter;
          SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, شے)
            bitset_سیٹ (lookahead_filter[شے], token);
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
                                        size_t nitems, Contribution_اشاریہ ci,
                                        bitset *lookaheads)
{
  إذا (AnnotationList__isContributionAlways (self, ci))
    أرجع true;
  إذا (!lookaheads)
    أرجع false;
  {
    علامت_نمبر token =
      InadequacyList__getContributionToken (self->inadequacy_گرہ, ci)
      ->content->نمبر;
    Sbitset_اشاریہ شے;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, شے)
      إذا (lookaheads[شے] && bitset_test (lookaheads[شے], token))
        أرجع true;
  }
  أرجع false;
}

Contribution_اشاریہ
AnnotationList__computeDominantContribution (AnnotationList ثابت *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_ونڈو_stable)
{
  Contribution_اشاریہ ثابت ci_کھسکاؤ =
    Inadequacy_List_get_کھسکاؤ_Contribution_اشاریہ (self->inadequacy_گرہ);

  علامت *token = self->inadequacy_گرہ->inadequacy.conflict.token;

  /* S/R conflict.  */
  إذا (ci_کھسکاؤ != Contribution_اشاریہ_none)
    {
      bool لبھو_stable_domination_over_کھسکاؤ = false;
      bool لبھو_stable_error_action_domination = false;
      {
        صحيح کھسکاؤ_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        إذا (!کھسکاؤ_precedence)
          أرجع ci_کھسکاؤ;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        Contribution_اشاریہ ci_rr_dominator = Contribution_اشاریہ_none;
        صحيح actioni;
        Contribution_اشاریہ ci;
        لكل (ci = 0,
               actioni = bitset_پہلا (self->inadequacy_گرہ->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_گرہ->contribution_گنتی_2;
             ++ci,
               actioni = bitset_اگلا (self->inadequacy_گرہ->inadequacy
                                      .conflict.actions, actioni+1))
          {
            صحيح reduce_precedence = 0;
            إذا (ci == ci_کھسکاؤ)
              استمر;
            {
              قاعدہ *r = self->inadequacy_گرہ->manifestingState
                          ->reductions->قاعدے[actioni];
              إذا (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            إذا (reduce_precedence
                && (reduce_precedence < کھسکاؤ_precedence
                    || (reduce_precedence == کھسکاؤ_precedence
                        && token->content->assoc == دایاں_assoc)))
              استمر;
            إذا (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              استمر;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            إذا (reduce_precedence == کھسکاؤ_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                إذا (لبھو_stable_domination_over_کھسکاؤ)
                  أرجع Contribution_اشاریہ_none;
                إذا (!require_ونڈو_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  أرجع Contribution_اشاریہ_error_action;
                لبھو_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            إذا (ci_rr_dominator == Contribution_اشاریہ_none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            إذا (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                إذا (لبھو_stable_error_action_domination)
                  أرجع Contribution_اشاریہ_none;
                إذا (!require_ونڈو_stable)
                  أرجع ci_rr_dominator;
                إذا (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  أرجع Contribution_اشاریہ_none;
                إذا (AnnotationList__isContributionAlways (self, ci))
                  أرجع ci_rr_dominator;
                لبھو_stable_domination_over_کھسکاؤ = true;
              }
          }
      }
      إذا (لبھو_stable_domination_over_کھسکاؤ
          || لبھو_stable_error_action_domination)
        أرجع Contribution_اشاریہ_none;
      /* No reduce or error action domination found, so shift dominates.  */
      أرجع ci_کھسکاؤ;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  لكل (Contribution_اشاریہ ci = 0; ci < self->inadequacy_گرہ->contribution_گنتی_2; ++ci)
    إذا (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        إذا (require_ونڈو_stable
            && !AnnotationList__isContributionAlways (self, ci))
          أرجع Contribution_اشاریہ_none;
        أرجع ci;
      }
  أرجع Contribution_اشاریہ_none;
}
