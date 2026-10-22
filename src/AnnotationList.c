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
AnnotationList__alloc_on_obstack (Contribution_شاخص contribution_شمېر,
                                  بنية obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_اندازه = contribution_شمېر * حجم res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_اندازه);
  res->بل = عدم;
  res->inadequacy_غوټه = عدم;
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
                                      Contribution_شاخص ci)
{
  aver (0 <= ci && ci < self->inadequacy_غوټه->contribution_شمېر_2);
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
Annotation_List_ورننباسه_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **غوټه;
  لكل (غوټه = list; *غوټه; غوټه = &(*غوټه)->بل)
    {
      صحيح cmp = 0;
      إذا (self->inadequacy_غوټه->id < (*غوټه)->inadequacy_غوټه->id)
        cmp = 1;
      وإلا إذا ((*غوټه)->inadequacy_غوټه->id < self->inadequacy_غوټه->id)
        cmp = -1;
      وإلا
        لكل (Contribution_شاخص ci = 0;
             cmp == 0 && ci < self->inadequacy_غوټه->contribution_شمېر_2;
             ++ci)
          {
            إذا (AnnotationList__isContributionAlways (self, ci))
              {
                إذا (!AnnotationList__isContributionAlways (*غوټه, ci))
                  cmp = -1;
              }
            وإلا إذا (AnnotationList__isContributionAlways (*غوټه, ci))
              cmp = 1;
            وإلا
              لكل (size_t توکی = 0; cmp == 0 && توکی < nitems; ++توکی)
                {
                  إذا (!Sbitset__test (self->contributions[ci], توکی))
                    {
                      إذا (Sbitset__test ((*غوټه)->contributions[ci], توکی))
                        cmp = -1;
                    }
                  وإلا إذا (!Sbitset__test ((*غوټه)->contributions[ci], توکی))
                    cmp = 1;
                }
          }
      إذا (cmp < 0)
        {
          self->بل = *غوټه;
          *غوټه = self;
          اكسر;
        }
      وإلا إذا (cmp == 0)
        {
          self = عدم;
          اكسر;
        }
    }
  إذا (!*غوټه)
    *غوټه = self;
  أرجع self != عدم;
}

ساكن bitset
Annotation_List_compute_ښويدنه_نښې (transitions *trans)
{
  bitset ښويدنه_نښې = bitset_create (ntokens, BITSET_FIXED);
  صحيح i;
  FOR_EACH_ښويدنه (trans, i)
    bitset_سېټ (ښويدنه_نښې, TRANSITION_نښه (trans, i));
  أرجع ښويدنه_نښې;
}

ساكن bitset
Annotation_List_compute_conflicted_نښې (bitset ښويدنه_نښې,
                                           reductions *reds)
{
  bitset conflicted_نښې = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_نښې_قاعده = bitset_create (ntokens, BITSET_FIXED);
  bitset نښې = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (نښې, ښويدنه_نښې);
  لكل (صحيح i = 0; i < reds->عدد; ++i)
    {
      bitset_and (conflicted_نښې_قاعده, نښې, reds->lookaheads[i]);
      bitset_or (conflicted_نښې,
                 conflicted_نښې, conflicted_نښې_قاعده);
      bitset_or (نښې, نښې, reds->lookaheads[i]);
      /* Check that rules are sorted on rule number or the next step in
         AnnotationList__compute_from_inadequacies will misbehave.  */
      aver (i == 0 || reds->قواعد[i-1] < reds->قواعد[i]);
    }

  bitset_free (نښې);
  bitset_free (conflicted_نښې_قاعده);

  أرجع conflicted_نښې;
}

ساكن bool
AnnotationList__compute_lhs_contributions (state *s, ثابت قاعده *the_قاعده,
                                           نښه_شمېره conflicted_token,
                                           bitsetv follow_kernel_توکي,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **توکی_lookahead_sets,
                                           Sbitset *توکي,
                                           بنية obstack
                                             *annotations_obstackp)
{
  goto_شمېره lhs_goto = map_goto (s->شمېره, the_قاعده->lhs->شمېره);
  إذا (bitset_test (always_follows[lhs_goto], conflicted_token))
    أرجع true;
  *توکي = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_توکی;
    bitset_bindex توکی;
    BITSET_FOR_EACH (biter_توکی, follow_kernel_توکي[lhs_goto], توکی, 0)
      إذا (ielr_توکی_has_lookahead (s, 0, توکی, conflicted_token,
                                   predecessors, توکی_lookahead_sets))
        Sbitset_سېټ (*توکي, توکی);
  }
  أرجع false;
}

ساكن فراغ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_توکي,
  bitsetv always_follows,
  state ***predecessors,
  bitset **توکی_lookahead_sets,
  AnnotationList **annotation_lists,
  Annotation_شاخص *annotation_counts,
  بنية obstack *annotations_obstackp)
{
  لكل (state **predecessor = predecessors[s->شمېره]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_غوټه =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_غوټه->contribution_شمېر_2, annotations_obstackp);
      annotation_غوټه->inadequacy_غوټه = self->inadequacy_غوټه;
      bool potential_contribution = false;
      bitset *lookaheads = عدم;

      لكل (Contribution_شاخص ci = 0; ci < self->inadequacy_غوټه->contribution_شمېر_2; ++ci)
        {
          نښه_شمېره contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_غوټه, ci)
            ->content->شمېره;
          إذا (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_غوټه->contributions[ci] = عدم;
              استمر;
            }
          annotation_غوټه->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            size_t predecessor_توکی = 0;
            Sbitset sbiter_توکی;
            Sbitset_شاخص self_توکی;
            SBITSET__FOR_EACH (self->contributions[ci], s->nitems,
                               sbiter_توکی, self_توکی)
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
                aver (s->توکي[self_توکی] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                إذا (توکی_شمېره_is_قاعده_شمېره (ritem[s->توکي[self_توکی] - 2]))
                  {
                    Sbitset توکي;
                    إذا (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          توکی_قاعده (&ritem[s->توکي[self_توکی]]),
                          contribution_token,
                          follow_kernel_توکي, always_follows, predecessors,
                          توکی_lookahead_sets, &توکي, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_غوټه->contributions[ci]);
                        annotation_غوټه->contributions[ci] = عدم;
                        // "Break" out of SBITSET__FOR_EACH.
                        اذهب after_sbitset__for_each;
                      }
                    وإلا
                      {
                        Sbitset__or (annotation_غوټه->contributions[ci],
                                     annotation_غوټه->contributions[ci],
                                     توکي, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, توکي);
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
                         predecessor_توکی < (*predecessor)->nitems;
                         ++predecessor_توکی)
                      إذا ((*predecessor)->توکي[predecessor_توکی]
                          == s->توکي[self_توکی] - 1)
                        اكسر;
                    aver (predecessor_توکی != (*predecessor)->nitems);
                    إذا (ielr_توکی_has_lookahead (*predecessor, 0,
                                                 predecessor_توکی,
                                                 contribution_token,
                                                 predecessors,
                                                 توکی_lookahead_sets))
                      Sbitset_سېټ (annotation_غوټه->contributions[ci],
                                    predecessor_توکی);
                  }
              }
          after_sbitset__for_each:;
          }
          إذا (annotation_غوټه->contributions[ci])
            {
              Sbitset biter;
              Sbitset_شاخص i;
              SBITSET__FOR_EACH (annotation_غوټه->contributions[ci],
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
                  bitset_سېټ (lookaheads[i], contribution_token);
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
          إذا (Contribution_شاخص_none
              != AnnotationList__computeDominantContribution (
                   annotation_غوټه, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_غوټه);
              annotation_غوټه = عدم;
            }
          {
            لكل (size_t i = 0; i < (*predecessor)->nitems; ++i)
              إذا (lookaheads[i])
                bitset_free (lookaheads[i]);
            حرر (lookaheads);
          }
          إذا (annotation_غوټه)
            {
              إذا (Annotation_List_ورننباسه_Into (annotation_غوټه,
                                              &annotation_lists[(*predecessor)
                                                                ->شمېره],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->شمېره];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_غوټه, *predecessor,
                    follow_kernel_توکي, always_follows, predecessors,
                    توکی_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              وإلا
                obstack_free (annotations_obstackp, annotation_غوټه);
            }
        }
      وإلا
        obstack_free (annotations_obstackp, annotation_غوټه);
    }
}

فراغ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_توکي, bitsetv always_follows,
  state ***predecessors, bitset **توکی_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  Annotation_شاخص *annotation_counts,
  Contribution_شاخص *max_contributionsp,
  بنية obstack *annotations_obstackp,
  Inadequacy_List_غوټه_شمېر *inadequacy_list_غوټه_شمېر)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  إذا (s->consistent)
    أرجع;

  bitsetv all_lookaheads = bitsetv_create (s->nitems, ntokens, BITSET_FIXED);
  bitsetv_ones (all_lookaheads);
  bitset ښويدنه_نښې = Annotation_List_compute_ښويدنه_نښې (s->transitions);
  bitset conflicted_نښې =
    Annotation_List_compute_conflicted_نښې (ښويدنه_نښې, s->reductions);

  /* Add an inadequacy annotation for each conflicted_token.  */
  bitset_iterator biter_conflict;
  bitset_bindex conflicted_token;
  BITSET_FOR_EACH (biter_conflict, conflicted_نښې, conflicted_token, 0)
    {
      AnnotationList *annotation_غوټه;
      Contribution_شاخص contribution_شمېر = 0;

      /* Allocate the annotation node.  */
      {
        لكل (صحيح قاعده_i = 0; قاعده_i < s->reductions->عدد; ++قاعده_i)
          إذا (bitset_test (s->reductions->lookaheads[قاعده_i],
                           conflicted_token))
            ++contribution_شمېر;
        إذا (bitset_test (ښويدنه_نښې, conflicted_token))
          ++contribution_شمېر;
        annotation_غوټه =
          AnnotationList__alloc_on_obstack (contribution_شمېر,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->عدد + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        Contribution_شاخص ci = 0;
        صحيح توکی_i = 0;
        لكل (صحيح قاعده_i = 0; قاعده_i < s->reductions->عدد; ++قاعده_i)
          {
            قاعده *the_قاعده = s->reductions->قواعد[قاعده_i];
            إذا (bitset_test (s->reductions->lookaheads[قاعده_i],
                             conflicted_token))
              {
                bitset_سېټ (actions, قاعده_i);
                /* If this reduction is on a kernel item, just add it.  */
                إذا (!توکی_شمېره_is_قاعده_شمېره (the_قاعده->rhs[0]))
                  {
                    annotation_غوټه->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    طالما (!توکی_شمېره_is_قاعده_شمېره (ritem[s->توکي[توکی_i]])
                           || توکی_شمېره_as_قاعده_شمېره (ritem[s->توکي[توکی_i]]) != the_قاعده->شمېره)
                      {
                        ++توکی_i;
                        aver (توکی_i < s->nitems);
                      }
                    Sbitset_سېټ (annotation_غوټه->contributions[ci], توکی_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                وإلا إذا (AnnotationList__compute_lhs_contributions (
                           s, the_قاعده, conflicted_token, follow_kernel_توکي,
                           always_follows, predecessors, توکی_lookahead_sets,
                           &annotation_غوټه->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_غوټه->contributions[ci++] = عدم;
                    استمر;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset_is_تشه (annotation_غوټه->contributions[ci],
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
          إذا (bitset_test (ښويدنه_نښې, conflicted_token))
            {
              bitset_سېټ (actions, s->reductions->عدد);
              annotation_غوټه->contributions[contribution_شمېر - 1] = عدم;
            }
          {
            InadequacyList *conflict_غوټه =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_غوټه_شمېر);
            actions = عدم;
            annotation_غوټه->inadequacy_غوټه = conflict_غوټه;
            إذا (Contribution_شاخص_none
                != AnnotationList__computeDominantContribution (
                     annotation_غوټه, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_غوټه);
                InadequacyList__delete (conflict_غوټه);
              }
            وإلا
              {
                Inadequacy_List_prepend_منزل (conflict_غوټه,
                                           &inadequacy_lists[s->شمېره]);
                {
                  bool b =
                    Annotation_List_ورننباسه_Into (annotation_غوټه,
                                                &annotation_lists[s->شمېره],
                                                s->nitems);
                  aver (b); (فراغ) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (ښويدنه_نښې, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->شمېره];
                إذا (contribution_شمېر > *max_contributionsp)
                  *max_contributionsp = contribution_شمېر;
                AnnotationList__computePredecessorAnnotations (
                  annotation_غوټه, s,
                  follow_kernel_توکي, always_follows, predecessors,
                  توکی_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      وإلا
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_غوټه);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (ښويدنه_نښې);
  bitset_free (conflicted_نښې);
}

فراغ
AnnotationList__debug (AnnotationList ثابت *self, size_t nitems, صحيح spaces)
{
  AnnotationList ثابت *a;
  Annotation_شاخص ai;
  لكل (a = self, ai = 0; a; a = a->بل, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_غوټه->manifestingState->شمېره);
      bitset_bindex rulei
        = bitset_لومړی (a->inadequacy_غوټه->inadequacy.conflict.actions);
      لكل (Contribution_شاخص ci = 0; ci < a->inadequacy_غوټه->contribution_شمېر_2; ++ci)
        {
          نښه_شمېره token =
            InadequacyList__getContributionToken (a->inadequacy_غوټه, ci)
            ->content->شمېره;
          fprintf (stderr, "%*s", spaces+2, "");
          إذا (ci == Inadequacy_List_get_ښويدنه_Contribution_شاخص (
                                                               a->inadequacy_غوټه))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          وإلا
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_غوټه->manifestingState
                       ->reductions->قواعد[rulei]->شمېره);
              rulei =
                bitset_بل (a->inadequacy_غوټه->inadequacy.conflict.actions,
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
  لكل (; self; self = self->بل)
    لكل (Contribution_شاخص ci = 0; ci < self->inadequacy_غوټه->contribution_شمېر_2; ++ci)
      إذا (!AnnotationList__isContributionAlways (self, ci))
        {
          نښه_شمېره token =
            InadequacyList__getContributionToken (self->inadequacy_غوټه, ci)
            ->content->شمېره;
          Sbitset_شاخص توکی;
          Sbitset biter;
          SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, توکی)
            bitset_سېټ (lookahead_filter[توکی], token);
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
                                        size_t nitems, Contribution_شاخص ci,
                                        bitset *lookaheads)
{
  إذا (AnnotationList__isContributionAlways (self, ci))
    أرجع true;
  إذا (!lookaheads)
    أرجع false;
  {
    نښه_شمېره token =
      InadequacyList__getContributionToken (self->inadequacy_غوټه, ci)
      ->content->شمېره;
    Sbitset_شاخص توکی;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, توکی)
      إذا (lookaheads[توکی] && bitset_test (lookaheads[توکی], token))
        أرجع true;
  }
  أرجع false;
}

Contribution_شاخص
AnnotationList__computeDominantContribution (AnnotationList ثابت *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_وېشل_stable)
{
  Contribution_شاخص ثابت ci_ښويدنه =
    Inadequacy_List_get_ښويدنه_Contribution_شاخص (self->inadequacy_غوټه);

  نښه *token = self->inadequacy_غوټه->inadequacy.conflict.token;

  /* S/R conflict.  */
  إذا (ci_ښويدنه != Contribution_شاخص_none)
    {
      bool پیدا_کړه_stable_domination_over_ښويدنه = false;
      bool پیدا_کړه_stable_error_action_domination = false;
      {
        صحيح ښويدنه_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        إذا (!ښويدنه_precedence)
          أرجع ci_ښويدنه;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        Contribution_شاخص ci_rr_dominator = Contribution_شاخص_none;
        صحيح actioni;
        Contribution_شاخص ci;
        لكل (ci = 0,
               actioni = bitset_لومړی (self->inadequacy_غوټه->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_غوټه->contribution_شمېر_2;
             ++ci,
               actioni = bitset_بل (self->inadequacy_غوټه->inadequacy
                                      .conflict.actions, actioni+1))
          {
            صحيح reduce_precedence = 0;
            إذا (ci == ci_ښويدنه)
              استمر;
            {
              قاعده *r = self->inadequacy_غوټه->manifestingState
                          ->reductions->قواعد[actioni];
              إذا (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            إذا (reduce_precedence
                && (reduce_precedence < ښويدنه_precedence
                    || (reduce_precedence == ښويدنه_precedence
                        && token->content->assoc == ښی_assoc)))
              استمر;
            إذا (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              استمر;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            إذا (reduce_precedence == ښويدنه_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                إذا (پیدا_کړه_stable_domination_over_ښويدنه)
                  أرجع Contribution_شاخص_none;
                إذا (!require_وېشل_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  أرجع Contribution_شاخص_error_action;
                پیدا_کړه_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            إذا (ci_rr_dominator == Contribution_شاخص_none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            إذا (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                إذا (پیدا_کړه_stable_error_action_domination)
                  أرجع Contribution_شاخص_none;
                إذا (!require_وېشل_stable)
                  أرجع ci_rr_dominator;
                إذا (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  أرجع Contribution_شاخص_none;
                إذا (AnnotationList__isContributionAlways (self, ci))
                  أرجع ci_rr_dominator;
                پیدا_کړه_stable_domination_over_ښويدنه = true;
              }
          }
      }
      إذا (پیدا_کړه_stable_domination_over_ښويدنه
          || پیدا_کړه_stable_error_action_domination)
        أرجع Contribution_شاخص_none;
      /* No reduce or error action domination found, so shift dominates.  */
      أرجع ci_ښويدنه;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  لكل (Contribution_شاخص ci = 0; ci < self->inadequacy_غوټه->contribution_شمېر_2; ++ci)
    إذا (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        إذا (require_وېشل_stable
            && !AnnotationList__isContributionAlways (self, ci))
          أرجع Contribution_شاخص_none;
        أرجع ci;
      }
  أرجع Contribution_شاخص_none;
}
