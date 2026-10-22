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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "AnnotationList.h"

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "ielr.h"
#ಅನ್ತರ್ಭಾವ "lalr.h"

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
ಸ್ಥಿರ AnnotationList*
AnnotationList__alloc_on_obstack (Contribution_ಸೂಚ್ಯಂಕ contribution_ಎಣಿಕೆ,
                                  ರಚನೆ obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_ಗಾತ್ರ = contribution_ಎಣಿಕೆ * ಗಾತ್ರ res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_ಗಾತ್ರ);
  res->ಮುಂದಿನ = ಖಾಲಿ;
  res->inadequacy_ಗಂಟು = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
ಸ್ಥಿರ bool
AnnotationList__isContributionAlways (AnnotationList ಸ್ಥಿರಾಂಕ *self,
                                      Contribution_ಸೂಚ್ಯಂಕ ci)
{
  aver (0 <= ci && ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2);
  ಹಿಂತಿರುಗಿ self->contributions[ci] == ಖಾಲಿ;
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
ಸ್ಥಿರ bool
Annotation_List_ಸೇರಿಸು_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **ಗಂಟು;
  ಗಾಗಿ (ಗಂಟು = list; *ಗಂಟು; ಗಂಟು = &(*ಗಂಟು)->ಮುಂದಿನ)
    {
      ಪೂರ್ಣಾಂಕ cmp = 0;
      ಆದರೆ (self->inadequacy_ಗಂಟು->id < (*ಗಂಟು)->inadequacy_ಗಂಟು->id)
        cmp = 1;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((*ಗಂಟು)->inadequacy_ಗಂಟು->id < self->inadequacy_ಗಂಟು->id)
        cmp = -1;
      ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (Contribution_ಸೂಚ್ಯಂಕ ci = 0;
             cmp == 0 && ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2;
             ++ci)
          {
            ಆದರೆ (AnnotationList__isContributionAlways (self, ci))
              {
                ಆದರೆ (!AnnotationList__isContributionAlways (*ಗಂಟು, ci))
                  cmp = -1;
              }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (AnnotationList__isContributionAlways (*ಗಂಟು, ci))
              cmp = 1;
            ಇಲ್ಲದಿದ್ದರೆ
              ಗಾಗಿ (size_t ಅಂಶ = 0; cmp == 0 && ಅಂಶ < nitems; ++ಅಂಶ)
                {
                  ಆದರೆ (!Sbitset__test (self->contributions[ci], ಅಂಶ))
                    {
                      ಆದರೆ (Sbitset__test ((*ಗಂಟು)->contributions[ci], ಅಂಶ))
                        cmp = -1;
                    }
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!Sbitset__test ((*ಗಂಟು)->contributions[ci], ಅಂಶ))
                    cmp = 1;
                }
          }
      ಆದರೆ (cmp < 0)
        {
          self->ಮುಂದಿನ = *ಗಂಟು;
          *ಗಂಟು = self;
          ನಿಲ್ಲಿಸು;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cmp == 0)
        {
          self = ಖಾಲಿ;
          ನಿಲ್ಲಿಸು;
        }
    }
  ಆದರೆ (!*ಗಂಟು)
    *ಗಂಟು = self;
  ಹಿಂತಿರುಗಿ self != ಖಾಲಿ;
}

ಸ್ಥಿರ bitset
Annotation_List_compute_ಸರಿಸು_ಟೋಕನುಗಳು (transitions *trans)
{
  bitset ಸರಿಸು_ಟೋಕನುಗಳು = bitset_create (ntokens, BITSET_FIXED);
  ಪೂರ್ಣಾಂಕ i;
  FOR_EACH_ಸರಿಸು (trans, i)
    bitset_ಗಣ (ಸರಿಸು_ಟೋಕನುಗಳು, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  ಹಿಂತಿರುಗಿ ಸರಿಸು_ಟೋಕನುಗಳು;
}

ಸ್ಥಿರ bitset
Annotation_List_compute_conflicted_ಟೋಕನುಗಳು (bitset ಸರಿಸು_ಟೋಕನುಗಳು,
                                           reductions *reds)
{
  bitset conflicted_ಟೋಕನುಗಳು = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_ಟೋಕನುಗಳು_ನಿಯಮ = bitset_create (ntokens, BITSET_FIXED);
  bitset ಟೋಕನುಗಳು = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (ಟೋಕನುಗಳು, ಸರಿಸು_ಟೋಕನುಗಳು);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < reds->ಅಂಕೆ; ++i)
    {
      bitset_and (conflicted_ಟೋಕನುಗಳು_ನಿಯಮ, ಟೋಕನುಗಳು, reds->lookaheads[i]);
      bitset_or (conflicted_ಟೋಕನುಗಳು,
                 conflicted_ಟೋಕನುಗಳು, conflicted_ಟೋಕನುಗಳು_ನಿಯಮ);
      bitset_or (ಟೋಕನುಗಳು, ಟೋಕನುಗಳು, reds->lookaheads[i]);
      /* Check that rules are sorted on rule number or the next step in
         AnnotationList__compute_from_inadequacies will misbehave.  */
      aver (i == 0 || reds->ನಿಯಮಗಳು[i-1] < reds->ನಿಯಮಗಳು[i]);
    }

  bitset_free (ಟೋಕನುಗಳು);
  bitset_free (conflicted_ಟೋಕನುಗಳು_ನಿಯಮ);

  ಹಿಂತಿರುಗಿ conflicted_ಟೋಕನುಗಳು;
}

ಸ್ಥಿರ bool
AnnotationList__compute_lhs_contributions (state *s, ಸ್ಥಿರಾಂಕ ನಿಯಮ *the_ನಿಯಮ,
                                           ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ conflicted_token,
                                           bitsetv follow_kernel_ವಸ್ತುಗಳು,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **ಅಂಶ_lookahead_sets,
                                           Sbitset *ವಸ್ತುಗಳು,
                                           ರಚನೆ obstack
                                             *annotations_obstackp)
{
  goto_ಸಂಖ್ಯೆ lhs_goto = map_goto (s->ಸಂಖ್ಯೆ, the_ನಿಯಮ->lhs->ಸಂಖ್ಯೆ);
  ಆದರೆ (bitset_test (always_follows[lhs_goto], conflicted_token))
    ಹಿಂತಿರುಗಿ true;
  *ವಸ್ತುಗಳು = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_ಅಂಶ;
    bitset_bindex ಅಂಶ;
    BITSET_FOR_EACH (biter_ಅಂಶ, follow_kernel_ವಸ್ತುಗಳು[lhs_goto], ಅಂಶ, 0)
      ಆದರೆ (ielr_ಅಂಶ_has_lookahead (s, 0, ಅಂಶ, conflicted_token,
                                   predecessors, ಅಂಶ_lookahead_sets))
        Sbitset_ಗಣ (*ವಸ್ತುಗಳು, ಅಂಶ);
  }
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_ವಸ್ತುಗಳು,
  bitsetv always_follows,
  state ***predecessors,
  bitset **ಅಂಶ_lookahead_sets,
  AnnotationList **annotation_lists,
  Annotation_ಸೂಚ್ಯಂಕ *annotation_counts,
  ರಚನೆ obstack *annotations_obstackp)
{
  ಗಾಗಿ (state **predecessor = predecessors[s->ಸಂಖ್ಯೆ]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_ಗಂಟು =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2, annotations_obstackp);
      annotation_ಗಂಟು->inadequacy_ಗಂಟು = self->inadequacy_ಗಂಟು;
      bool potential_contribution = false;
      bitset *lookaheads = ಖಾಲಿ;

      ಗಾಗಿ (Contribution_ಸೂಚ್ಯಂಕ ci = 0; ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_ಗಂಟು, ci)
            ->content->ಸಂಖ್ಯೆ;
          ಆದರೆ (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_ಗಂಟು->contributions[ci] = ಖಾಲಿ;
              ಮುಂದುವರಿ;
            }
          annotation_ಗಂಟು->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            size_t predecessor_ಅಂಶ = 0;
            Sbitset sbiter_ಅಂಶ;
            Sbitset_ಸೂಚ್ಯಂಕ self_ಅಂಶ;
            SBITSET__FOR_EACH (self->contributions[ci], s->nitems,
                               sbiter_ಅಂಶ, self_ಅಂಶ)
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
                aver (s->ವಸ್ತುಗಳು[self_ಅಂಶ] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                ಆದರೆ (ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[self_ಅಂಶ] - 2]))
                  {
                    Sbitset ವಸ್ತುಗಳು;
                    ಆದರೆ (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          ಅಂಶ_ನಿಯಮ (&ritem[s->ವಸ್ತುಗಳು[self_ಅಂಶ]]),
                          contribution_token,
                          follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
                          ಅಂಶ_lookahead_sets, &ವಸ್ತುಗಳು, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_ಗಂಟು->contributions[ci]);
                        annotation_ಗಂಟು->contributions[ci] = ಖಾಲಿ;
                        // "Break" out of SBITSET__FOR_EACH.
                        ಹೋಗು after_sbitset__for_each;
                      }
                    ಇಲ್ಲದಿದ್ದರೆ
                      {
                        Sbitset__or (annotation_ಗಂಟು->contributions[ci],
                                     annotation_ಗಂಟು->contributions[ci],
                                     ವಸ್ತುಗಳು, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, ವಸ್ತುಗಳು);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                ಇಲ್ಲದಿದ್ದರೆ
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    ಗಾಗಿ (;
                         predecessor_ಅಂಶ < (*predecessor)->nitems;
                         ++predecessor_ಅಂಶ)
                      ಆದರೆ ((*predecessor)->ವಸ್ತುಗಳು[predecessor_ಅಂಶ]
                          == s->ವಸ್ತುಗಳು[self_ಅಂಶ] - 1)
                        ನಿಲ್ಲಿಸು;
                    aver (predecessor_ಅಂಶ != (*predecessor)->nitems);
                    ಆದರೆ (ielr_ಅಂಶ_has_lookahead (*predecessor, 0,
                                                 predecessor_ಅಂಶ,
                                                 contribution_token,
                                                 predecessors,
                                                 ಅಂಶ_lookahead_sets))
                      Sbitset_ಗಣ (annotation_ಗಂಟು->contributions[ci],
                                    predecessor_ಅಂಶ);
                  }
              }
          after_sbitset__for_each:;
          }
          ಆದರೆ (annotation_ಗಂಟು->contributions[ci])
            {
              Sbitset biter;
              Sbitset_ಸೂಚ್ಯಂಕ i;
              SBITSET__FOR_EACH (annotation_ಗಂಟು->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  ಆದರೆ (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             ಗಾತ್ರ *lookaheads);
                      ಗಾಗಿ (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = ಖಾಲಿ;
                    }
                  ಆದರೆ (!lookaheads[i])
                    lookaheads[i] = bitset_create (ntokens, BITSET_FIXED);
                  bitset_ಗಣ (lookaheads[i], contribution_token);
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
      ಆದರೆ (potential_contribution)
        {
          ಆದರೆ (Contribution_ಸೂಚ್ಯಂಕ_none
              != AnnotationList__computeDominantContribution (
                   annotation_ಗಂಟು, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_ಗಂಟು);
              annotation_ಗಂಟು = ಖಾಲಿ;
            }
          {
            ಗಾಗಿ (size_t i = 0; i < (*predecessor)->nitems; ++i)
              ಆದರೆ (lookaheads[i])
                bitset_free (lookaheads[i]);
            ಬಿಡುಗಡೆ (lookaheads);
          }
          ಆದರೆ (annotation_ಗಂಟು)
            {
              ಆದರೆ (Annotation_List_ಸೇರಿಸು_Into (annotation_ಗಂಟು,
                                              &annotation_lists[(*predecessor)
                                                                ->ಸಂಖ್ಯೆ],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->ಸಂಖ್ಯೆ];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_ಗಂಟು, *predecessor,
                    follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
                    ಅಂಶ_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                obstack_free (annotations_obstackp, annotation_ಗಂಟು);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        obstack_free (annotations_obstackp, annotation_ಗಂಟು);
    }
}

ಶೂನ್ಯ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_ವಸ್ತುಗಳು, bitsetv always_follows,
  state ***predecessors, bitset **ಅಂಶ_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  Annotation_ಸೂಚ್ಯಂಕ *annotation_counts,
  Contribution_ಸೂಚ್ಯಂಕ *max_contributionsp,
  ರಚನೆ obstack *annotations_obstackp,
  Inadequacy_List_ಗಂಟು_ಎಣಿಕೆ *inadequacy_list_ಗಂಟು_ಎಣಿಕೆ)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  ಆದರೆ (s->consistent)
    ಹಿಂತಿರುಗಿ;

  bitsetv all_lookaheads = bitsetv_create (s->nitems, ntokens, BITSET_FIXED);
  bitsetv_ones (all_lookaheads);
  bitset ಸರಿಸು_ಟೋಕನುಗಳು = Annotation_List_compute_ಸರಿಸು_ಟೋಕನುಗಳು (s->transitions);
  bitset conflicted_ಟೋಕನುಗಳು =
    Annotation_List_compute_conflicted_ಟೋಕನುಗಳು (ಸರಿಸು_ಟೋಕನುಗಳು, s->reductions);

  /* Add an inadequacy annotation for each conflicted_token.  */
  bitset_iterator biter_conflict;
  bitset_bindex conflicted_token;
  BITSET_FOR_EACH (biter_conflict, conflicted_ಟೋಕನುಗಳು, conflicted_token, 0)
    {
      AnnotationList *annotation_ಗಂಟು;
      Contribution_ಸೂಚ್ಯಂಕ contribution_ಎಣಿಕೆ = 0;

      /* Allocate the annotation node.  */
      {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ನಿಯಮ_i = 0; ನಿಯಮ_i < s->reductions->ಅಂಕೆ; ++ನಿಯಮ_i)
          ಆದರೆ (bitset_test (s->reductions->lookaheads[ನಿಯಮ_i],
                           conflicted_token))
            ++contribution_ಎಣಿಕೆ;
        ಆದರೆ (bitset_test (ಸರಿಸು_ಟೋಕನುಗಳು, conflicted_token))
          ++contribution_ಎಣಿಕೆ;
        annotation_ಗಂಟು =
          AnnotationList__alloc_on_obstack (contribution_ಎಣಿಕೆ,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->ಅಂಕೆ + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        Contribution_ಸೂಚ್ಯಂಕ ci = 0;
        ಪೂರ್ಣಾಂಕ ಅಂಶ_i = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ನಿಯಮ_i = 0; ನಿಯಮ_i < s->reductions->ಅಂಕೆ; ++ನಿಯಮ_i)
          {
            ನಿಯಮ *the_ನಿಯಮ = s->reductions->ನಿಯಮಗಳು[ನಿಯಮ_i];
            ಆದರೆ (bitset_test (s->reductions->lookaheads[ನಿಯಮ_i],
                             conflicted_token))
              {
                bitset_ಗಣ (actions, ನಿಯಮ_i);
                /* If this reduction is on a kernel item, just add it.  */
                ಆದರೆ (!ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (the_ನಿಯಮ->rhs[0]))
                  {
                    annotation_ಗಂಟು->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    ಆಗ (!ಅಂಶ_ಸಂಖ್ಯೆ_is_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[ಅಂಶ_i]])
                           || ಅಂಶ_ಸಂಖ್ಯೆ_as_ನಿಯಮ_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[ಅಂಶ_i]]) != the_ನಿಯಮ->ಸಂಖ್ಯೆ)
                      {
                        ++ಅಂಶ_i;
                        aver (ಅಂಶ_i < s->nitems);
                      }
                    Sbitset_ಗಣ (annotation_ಗಂಟು->contributions[ci], ಅಂಶ_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (AnnotationList__compute_lhs_contributions (
                           s, the_ನಿಯಮ, conflicted_token, follow_kernel_ವಸ್ತುಗಳು,
                           always_follows, predecessors, ಅಂಶ_lookahead_sets,
                           &annotation_ಗಂಟು->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_ಗಂಟು->contributions[ci++] = ಖಾಲಿ;
                    ಮುಂದುವರಿ;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset_is_ಖಾಲಿಗಳು (annotation_ಗಂಟು->contributions[ci],
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
      ಆದರೆ (potential_contribution)
        {
          ಆದರೆ (bitset_test (ಸರಿಸು_ಟೋಕನುಗಳು, conflicted_token))
            {
              bitset_ಗಣ (actions, s->reductions->ಅಂಕೆ);
              annotation_ಗಂಟು->contributions[contribution_ಎಣಿಕೆ - 1] = ಖಾಲಿ;
            }
          {
            InadequacyList *conflict_ಗಂಟು =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_ಗಂಟು_ಎಣಿಕೆ);
            actions = ಖಾಲಿ;
            annotation_ಗಂಟು->inadequacy_ಗಂಟು = conflict_ಗಂಟು;
            ಆದರೆ (Contribution_ಸೂಚ್ಯಂಕ_none
                != AnnotationList__computeDominantContribution (
                     annotation_ಗಂಟು, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_ಗಂಟು);
                InadequacyList__delete (conflict_ಗಂಟು);
              }
            ಇಲ್ಲದಿದ್ದರೆ
              {
                Inadequacy_List_prepend_ಗಮ್ಯ (conflict_ಗಂಟು,
                                           &inadequacy_lists[s->ಸಂಖ್ಯೆ]);
                {
                  bool b =
                    Annotation_List_ಸೇರಿಸು_Into (annotation_ಗಂಟು,
                                                &annotation_lists[s->ಸಂಖ್ಯೆ],
                                                s->nitems);
                  aver (b); (ಶೂನ್ಯ) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (ಸರಿಸು_ಟೋಕನುಗಳು, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->ಸಂಖ್ಯೆ];
                ಆದರೆ (contribution_ಎಣಿಕೆ > *max_contributionsp)
                  *max_contributionsp = contribution_ಎಣಿಕೆ;
                AnnotationList__computePredecessorAnnotations (
                  annotation_ಗಂಟು, s,
                  follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
                  ಅಂಶ_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_ಗಂಟು);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (ಸರಿಸು_ಟೋಕನುಗಳು);
  bitset_free (conflicted_ಟೋಕನುಗಳು);
}

ಶೂನ್ಯ
AnnotationList__debug (AnnotationList ಸ್ಥಿರಾಂಕ *self, size_t nitems, ಪೂರ್ಣಾಂಕ spaces)
{
  AnnotationList ಸ್ಥಿರಾಂಕ *a;
  Annotation_ಸೂಚ್ಯಂಕ ai;
  ಗಾಗಿ (a = self, ai = 0; a; a = a->ಮುಂದಿನ, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_ಗಂಟು->manifestingState->ಸಂಖ್ಯೆ);
      bitset_bindex rulei
        = bitset_ಮೊದಲ (a->inadequacy_ಗಂಟು->inadequacy.conflict.actions);
      ಗಾಗಿ (Contribution_ಸೂಚ್ಯಂಕ ci = 0; ci < a->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token =
            InadequacyList__getContributionToken (a->inadequacy_ಗಂಟು, ci)
            ->content->ಸಂಖ್ಯೆ;
          fprintf (stderr, "%*s", spaces+2, "");
          ಆದರೆ (ci == Inadequacy_List_get_ಸರಿಸು_Contribution_ಸೂಚ್ಯಂಕ (
                                                               a->inadequacy_ಗಂಟು))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_ಗಂಟು->manifestingState
                       ->reductions->ನಿಯಮಗಳು[rulei]->ಸಂಖ್ಯೆ);
              rulei =
                bitset_ಮುಂದಿನ (a->inadequacy_ಗಂಟು->inadequacy.conflict.actions,
                             rulei+1);
              ಆದರೆ (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

ಶೂನ್ಯ
AnnotationList__computeLookaheadFilter (AnnotationList ಸ್ಥಿರಾಂಕ *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  ಗಾಗಿ (; self; self = self->ಮುಂದಿನ)
    ಗಾಗಿ (Contribution_ಸೂಚ್ಯಂಕ ci = 0; ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
      ಆದರೆ (!AnnotationList__isContributionAlways (self, ci))
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token =
            InadequacyList__getContributionToken (self->inadequacy_ಗಂಟು, ci)
            ->content->ಸಂಖ್ಯೆ;
          Sbitset_ಸೂಚ್ಯಂಕ ಅಂಶ;
          Sbitset biter;
          SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, ಅಂಶ)
            bitset_ಗಣ (lookahead_filter[ಅಂಶ], token);
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
ಸ್ಥಿರ bool
AnnotationList__stateMakesContribution (AnnotationList ಸ್ಥಿರಾಂಕ *self,
                                        size_t nitems, Contribution_ಸೂಚ್ಯಂಕ ci,
                                        bitset *lookaheads)
{
  ಆದರೆ (AnnotationList__isContributionAlways (self, ci))
    ಹಿಂತಿರುಗಿ true;
  ಆದರೆ (!lookaheads)
    ಹಿಂತಿರುಗಿ false;
  {
    ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token =
      InadequacyList__getContributionToken (self->inadequacy_ಗಂಟು, ci)
      ->content->ಸಂಖ್ಯೆ;
    Sbitset_ಸೂಚ್ಯಂಕ ಅಂಶ;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, ಅಂಶ)
      ಆದರೆ (lookaheads[ಅಂಶ] && bitset_test (lookaheads[ಅಂಶ], token))
        ಹಿಂತಿರುಗಿ true;
  }
  ಹಿಂತಿರುಗಿ false;
}

Contribution_ಸೂಚ್ಯಂಕ
AnnotationList__computeDominantContribution (AnnotationList ಸ್ಥಿರಾಂಕ *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_ವಿಭಜಿಸು_stable)
{
  Contribution_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ ci_ಸರಿಸು =
    Inadequacy_List_get_ಸರಿಸು_Contribution_ಸೂಚ್ಯಂಕ (self->inadequacy_ಗಂಟು);

  ಚಿಹ್ನೆ *token = self->inadequacy_ಗಂಟು->inadequacy.conflict.token;

  /* S/R conflict.  */
  ಆದರೆ (ci_ಸರಿಸು != Contribution_ಸೂಚ್ಯಂಕ_none)
    {
      bool ಕಂಡುಹಿಡಿ_stable_domination_over_ಸರಿಸು = false;
      bool ಕಂಡುಹಿಡಿ_stable_error_action_domination = false;
      {
        ಪೂರ್ಣಾಂಕ ಸರಿಸು_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        ಆದರೆ (!ಸರಿಸು_precedence)
          ಹಿಂತಿರುಗಿ ci_ಸರಿಸು;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        Contribution_ಸೂಚ್ಯಂಕ ci_rr_dominator = Contribution_ಸೂಚ್ಯಂಕ_none;
        ಪೂರ್ಣಾಂಕ actioni;
        Contribution_ಸೂಚ್ಯಂಕ ci;
        ಗಾಗಿ (ci = 0,
               actioni = bitset_ಮೊದಲ (self->inadequacy_ಗಂಟು->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2;
             ++ci,
               actioni = bitset_ಮುಂದಿನ (self->inadequacy_ಗಂಟು->inadequacy
                                      .conflict.actions, actioni+1))
          {
            ಪೂರ್ಣಾಂಕ reduce_precedence = 0;
            ಆದರೆ (ci == ci_ಸರಿಸು)
              ಮುಂದುವರಿ;
            {
              ನಿಯಮ *r = self->inadequacy_ಗಂಟು->manifestingState
                          ->reductions->ನಿಯಮಗಳು[actioni];
              ಆದರೆ (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            ಆದರೆ (reduce_precedence
                && (reduce_precedence < ಸರಿಸು_precedence
                    || (reduce_precedence == ಸರಿಸು_precedence
                        && token->content->assoc == ಬಲ_assoc)))
              ಮುಂದುವರಿ;
            ಆದರೆ (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              ಮುಂದುವರಿ;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            ಆದರೆ (reduce_precedence == ಸರಿಸು_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                ಆದರೆ (ಕಂಡುಹಿಡಿ_stable_domination_over_ಸರಿಸು)
                  ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
                ಆದರೆ (!require_ವಿಭಜಿಸು_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_error_action;
                ಕಂಡುಹಿಡಿ_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            ಆದರೆ (ci_rr_dominator == Contribution_ಸೂಚ್ಯಂಕ_none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            ಆದರೆ (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                ಆದರೆ (ಕಂಡುಹಿಡಿ_stable_error_action_domination)
                  ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
                ಆದರೆ (!require_ವಿಭಜಿಸು_stable)
                  ಹಿಂತಿರುಗಿ ci_rr_dominator;
                ಆದರೆ (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
                ಆದರೆ (AnnotationList__isContributionAlways (self, ci))
                  ಹಿಂತಿರುಗಿ ci_rr_dominator;
                ಕಂಡುಹಿಡಿ_stable_domination_over_ಸರಿಸು = true;
              }
          }
      }
      ಆದರೆ (ಕಂಡುಹಿಡಿ_stable_domination_over_ಸರಿಸು
          || ಕಂಡುಹಿಡಿ_stable_error_action_domination)
        ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
      /* No reduce or error action domination found, so shift dominates.  */
      ಹಿಂತಿರುಗಿ ci_ಸರಿಸು;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  ಗಾಗಿ (Contribution_ಸೂಚ್ಯಂಕ ci = 0; ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
    ಆದರೆ (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        ಆದರೆ (require_ವಿಭಜಿಸು_stable
            && !AnnotationList__isContributionAlways (self, ci))
          ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
        ಹಿಂತಿರುಗಿ ci;
      }
  ಹಿಂತಿರುಗಿ Contribution_ಸೂಚ್ಯಂಕ_none;
}
