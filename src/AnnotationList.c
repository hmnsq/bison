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
AnnotationList__alloc_on_obstack (ContributionIndex contribution_ಎಣಿಕೆ,
                                  ಸಂರಚನಾ obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_ಎಣಿಕೆ * ಪರಿಮಾಣ res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = ರಿಕ್ತ;
  res->inadequacy_ಗಂಟು = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ res;
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
AnnotationList__isContributionAlways (AnnotationList ಧ್ರುವ *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2);
  ಪ್ರತ್ಯಾಗಮ self->contributions[ci] == ರಿಕ್ತ;
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
  ಪ್ರತಿ (ಗಂಟು = list; *ಗಂಟು; ಗಂಟು = &(*ಗಂಟು)->next)
    {
      ಪೂರ್ಣಾಙ್ಕ cmp = 0;
      ಯದಿ (self->inadequacy_ಗಂಟು->id < (*ಗಂಟು)->inadequacy_ಗಂಟು->id)
        cmp = 1;
      ಅನ್ಯಥಾ ಯದಿ ((*ಗಂಟು)->inadequacy_ಗಂಟು->id < self->inadequacy_ಗಂಟು->id)
        cmp = -1;
      ಅನ್ಯಥಾ
        ಪ್ರತಿ (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2;
             ++ci)
          {
            ಯದಿ (AnnotationList__isContributionAlways (self, ci))
              {
                ಯದಿ (!AnnotationList__isContributionAlways (*ಗಂಟು, ci))
                  cmp = -1;
              }
            ಅನ್ಯಥಾ ಯದಿ (AnnotationList__isContributionAlways (*ಗಂಟು, ci))
              cmp = 1;
            ಅನ್ಯಥಾ
              ಪ್ರತಿ (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  ಯದಿ (!Sbitset__test (self->contributions[ci], item))
                    {
                      ಯದಿ (Sbitset__test ((*ಗಂಟು)->contributions[ci], item))
                        cmp = -1;
                    }
                  ಅನ್ಯಥಾ ಯದಿ (!Sbitset__test ((*ಗಂಟು)->contributions[ci], item))
                    cmp = 1;
                }
          }
      ಯದಿ (cmp < 0)
        {
          self->next = *ಗಂಟು;
          *ಗಂಟು = self;
          ಭಙ್ಗ;
        }
      ಅನ್ಯಥಾ ಯದಿ (cmp == 0)
        {
          self = ರಿಕ್ತ;
          ಭಙ್ಗ;
        }
    }
  ಯದಿ (!*ಗಂಟು)
    *ಗಂಟು = self;
  ಪ್ರತ್ಯಾಗಮ self != ರಿಕ್ತ;
}

ಸ್ಥಿರ bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  ಪೂರ್ಣಾಙ್ಕ i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_ಚಿಹ್ನೆ (trans, i));
  ಪ್ರತ್ಯಾಗಮ shift_tokens;
}

ಸ್ಥಿರ bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < reds->ಅಂಕೆ; ++i)
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

  ಪ್ರತ್ಯಾಗಮ conflicted_tokens;
}

ಸ್ಥಿರ bool
AnnotationList__compute_lhs_contributions (state *s, ಧ್ರುವ rule *the_rule,
                                           ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ conflicted_token,
                                           bitsetv follow_kernel_ವಸ್ತುಗಳು,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *ವಸ್ತುಗಳು,
                                           ಸಂರಚನಾ obstack
                                             *annotations_obstackp)
{
  goto_ಸಂಖ್ಯೆ lhs_goto = map_goto (s->ಸಂಖ್ಯೆ, the_rule->lhs->ಸಂಖ್ಯೆ);
  ಯದಿ (bitset_test (always_follows[lhs_goto], conflicted_token))
    ಪ್ರತ್ಯಾಗಮ true;
  *ವಸ್ತುಗಳು = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_ವಸ್ತುಗಳು[lhs_goto], item, 0)
      ಯದಿ (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*ವಸ್ತುಗಳು, item);
  }
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_ವಸ್ತುಗಳು,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ಸಂರಚನಾ obstack *annotations_obstackp)
{
  ಪ್ರತಿ (state **predecessor = predecessors[s->ಸಂಖ್ಯೆ]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_ಗಂಟು =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2, annotations_obstackp);
      annotation_ಗಂಟು->inadequacy_ಗಂಟು = self->inadequacy_ಗಂಟು;
      bool potential_contribution = false;
      bitset *lookaheads = ರಿಕ್ತ;

      ಪ್ರತಿ (ContributionIndex ci = 0; ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_ಗಂಟು, ci)
            ->content->ಸಂಖ್ಯೆ;
          ಯದಿ (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_ಗಂಟು->contributions[ci] = ರಿಕ್ತ;
              ಅನುವರ್ತ;
            }
          annotation_ಗಂಟು->contributions[ci] =
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
                aver (s->ವಸ್ತುಗಳು[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                ಯದಿ (item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[self_item] - 2]))
                  {
                    Sbitset ವಸ್ತುಗಳು;
                    ಯದಿ (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->ವಸ್ತುಗಳು[self_item]]),
                          contribution_token,
                          follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
                          item_lookahead_sets, &ವಸ್ತುಗಳು, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_ಗಂಟು->contributions[ci]);
                        annotation_ಗಂಟು->contributions[ci] = ರಿಕ್ತ;
                        // "Break" out of SBITSET__FOR_EACH.
                        ಗಚ್ಛ after_sbitset__for_each;
                      }
                    ಅನ್ಯಥಾ
                      {
                        Sbitset__or (annotation_ಗಂಟು->contributions[ci],
                                     annotation_ಗಂಟು->contributions[ci],
                                     ವಸ್ತುಗಳು, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, ವಸ್ತುಗಳು);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                ಅನ್ಯಥಾ
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    ಪ್ರತಿ (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      ಯದಿ ((*predecessor)->ವಸ್ತುಗಳು[predecessor_item]
                          == s->ವಸ್ತುಗಳು[self_item] - 1)
                        ಭಙ್ಗ;
                    aver (predecessor_item != (*predecessor)->nitems);
                    ಯದಿ (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_ಗಂಟು->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          ಯದಿ (annotation_ಗಂಟು->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_ಗಂಟು->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  ಯದಿ (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             ಪರಿಮಾಣ *lookaheads);
                      ಪ್ರತಿ (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = ರಿಕ್ತ;
                    }
                  ಯದಿ (!lookaheads[i])
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
      ಯದಿ (potential_contribution)
        {
          ಯದಿ (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_ಗಂಟು, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_ಗಂಟು);
              annotation_ಗಂಟು = ರಿಕ್ತ;
            }
          {
            ಪ್ರತಿ (size_t i = 0; i < (*predecessor)->nitems; ++i)
              ಯದಿ (lookaheads[i])
                bitset_free (lookaheads[i]);
            ಮುಕ್ತ (lookaheads);
          }
          ಯದಿ (annotation_ಗಂಟು)
            {
              ಯದಿ (Annotation_List_ಸೇರಿಸು_Into (annotation_ಗಂಟು,
                                              &annotation_lists[(*predecessor)
                                                                ->ಸಂಖ್ಯೆ],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->ಸಂಖ್ಯೆ];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_ಗಂಟು, *predecessor,
                    follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              ಅನ್ಯಥಾ
                obstack_free (annotations_obstackp, annotation_ಗಂಟು);
            }
        }
      ಅನ್ಯಥಾ
        obstack_free (annotations_obstackp, annotation_ಗಂಟು);
    }
}

ಶೂನ್ಯ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_ವಸ್ತುಗಳು, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  ಸಂರಚನಾ obstack *annotations_obstackp,
  Inadequacy_List_ಗಂಟು_ಎಣಿಕೆ *inadequacy_list_ಗಂಟು_ಎಣಿಕೆ)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  ಯದಿ (s->consistent)
    ಪ್ರತ್ಯಾಗಮ;

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
      AnnotationList *annotation_ಗಂಟು;
      ContributionIndex contribution_ಎಣಿಕೆ = 0;

      /* Allocate the annotation node.  */
      {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ rule_i = 0; rule_i < s->reductions->ಅಂಕೆ; ++rule_i)
          ಯದಿ (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_ಎಣಿಕೆ;
        ಯದಿ (bitset_test (shift_tokens, conflicted_token))
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
        ContributionIndex ci = 0;
        ಪೂರ್ಣಾಙ್ಕ item_i = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ rule_i = 0; rule_i < s->reductions->ಅಂಕೆ; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            ಯದಿ (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                ಯದಿ (!item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (the_rule->rhs[0]))
                  {
                    annotation_ಗಂಟು->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    ಯಾವತ್ (!item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[item_i]])
                           || item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (ritem[s->ವಸ್ತುಗಳು[item_i]]) != the_rule->ಸಂಖ್ಯೆ)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_ಗಂಟು->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                ಅನ್ಯಥಾ ಯದಿ (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_ವಸ್ತುಗಳು,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_ಗಂಟು->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_ಗಂಟು->contributions[ci++] = ರಿಕ್ತ;
                    ಅನುವರ್ತ;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_ಗಂಟು->contributions[ci],
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
      ಯದಿ (potential_contribution)
        {
          ಯದಿ (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->ಅಂಕೆ);
              annotation_ಗಂಟು->contributions[contribution_ಎಣಿಕೆ - 1] = ರಿಕ್ತ;
            }
          {
            InadequacyList *conflict_ಗಂಟು =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_ಗಂಟು_ಎಣಿಕೆ);
            actions = ರಿಕ್ತ;
            annotation_ಗಂಟು->inadequacy_ಗಂಟು = conflict_ಗಂಟು;
            ಯದಿ (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_ಗಂಟು, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_ಗಂಟು);
                InadequacyList__delete (conflict_ಗಂಟು);
              }
            ಅನ್ಯಥಾ
              {
                InadequacyList__prependTo (conflict_ಗಂಟು,
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
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->ಸಂಖ್ಯೆ];
                ಯದಿ (contribution_ಎಣಿಕೆ > *max_contributionsp)
                  *max_contributionsp = contribution_ಎಣಿಕೆ;
                AnnotationList__computePredecessorAnnotations (
                  annotation_ಗಂಟು, s,
                  follow_kernel_ವಸ್ತುಗಳು, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      ಅನ್ಯಥಾ
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_ಗಂಟು);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

ಶೂನ್ಯ
AnnotationList__debug (AnnotationList ಧ್ರುವ *self, size_t nitems, ಪೂರ್ಣಾಙ್ಕ spaces)
{
  AnnotationList ಧ್ರುವ *a;
  AnnotationIndex ai;
  ಪ್ರತಿ (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_ಗಂಟು->manifestingState->ಸಂಖ್ಯೆ);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_ಗಂಟು->inadequacy.conflict.actions);
      ಪ್ರತಿ (ContributionIndex ci = 0; ci < a->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token =
            InadequacyList__getContributionToken (a->inadequacy_ಗಂಟು, ci)
            ->content->ಸಂಖ್ಯೆ;
          fprintf (stderr, "%*s", spaces+2, "");
          ಯದಿ (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_ಗಂಟು))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          ಅನ್ಯಥಾ
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_ಗಂಟು->manifestingState
                       ->reductions->rules[rulei]->ಸಂಖ್ಯೆ);
              rulei =
                bitset_next (a->inadequacy_ಗಂಟು->inadequacy.conflict.actions,
                             rulei+1);
              ಯದಿ (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              ಅನ್ಯಥಾ
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
AnnotationList__computeLookaheadFilter (AnnotationList ಧ್ರುವ *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  ಪ್ರತಿ (; self; self = self->next)
    ಪ್ರತಿ (ContributionIndex ci = 0; ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
      ಯದಿ (!AnnotationList__isContributionAlways (self, ci))
        {
          ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token =
            InadequacyList__getContributionToken (self->inadequacy_ಗಂಟು, ci)
            ->content->ಸಂಖ್ಯೆ;
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
ಸ್ಥಿರ bool
AnnotationList__stateMakesContribution (AnnotationList ಧ್ರುವ *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  ಯದಿ (AnnotationList__isContributionAlways (self, ci))
    ಪ್ರತ್ಯಾಗಮ true;
  ಯದಿ (!lookaheads)
    ಪ್ರತ್ಯಾಗಮ false;
  {
    ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ token =
      InadequacyList__getContributionToken (self->inadequacy_ಗಂಟು, ci)
      ->content->ಸಂಖ್ಯೆ;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      ಯದಿ (lookaheads[item] && bitset_test (lookaheads[item], token))
        ಪ್ರತ್ಯಾಗಮ true;
  }
  ಪ್ರತ್ಯಾಗಮ false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList ಧ್ರುವ *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex ಧ್ರುವ ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_ಗಂಟು);

  ಚಿಹ್ನೆ *token = self->inadequacy_ಗಂಟು->inadequacy.conflict.token;

  /* S/R conflict.  */
  ಯದಿ (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        ಪೂರ್ಣಾಙ್ಕ shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        ಯದಿ (!shift_precedence)
          ಪ್ರತ್ಯಾಗಮ ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        ಪೂರ್ಣಾಙ್ಕ actioni;
        ContributionIndex ci;
        ಪ್ರತಿ (ci = 0,
               actioni = bitset_first (self->inadequacy_ಗಂಟು->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_ಗಂಟು->inadequacy
                                      .conflict.actions, actioni+1))
          {
            ಪೂರ್ಣಾಙ್ಕ reduce_precedence = 0;
            ಯದಿ (ci == ci_shift)
              ಅನುವರ್ತ;
            {
              rule *r = self->inadequacy_ಗಂಟು->manifestingState
                          ->reductions->rules[actioni];
              ಯದಿ (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            ಯದಿ (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == ಬಲ_assoc)))
              ಅನುವರ್ತ;
            ಯದಿ (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              ಅನುವರ್ತ;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            ಯದಿ (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                ಯದಿ (find_stable_domination_over_shift)
                  ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
                ಯದಿ (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  ಪ್ರತ್ಯಾಗಮ ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            ಯದಿ (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            ಯದಿ (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                ಯದಿ (find_stable_error_action_domination)
                  ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
                ಯದಿ (!require_split_stable)
                  ಪ್ರತ್ಯಾಗಮ ci_rr_dominator;
                ಯದಿ (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
                ಯದಿ (AnnotationList__isContributionAlways (self, ci))
                  ಪ್ರತ್ಯಾಗಮ ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      ಯದಿ (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      ಪ್ರತ್ಯಾಗಮ ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  ಪ್ರತಿ (ContributionIndex ci = 0; ci < self->inadequacy_ಗಂಟು->contribution_ಎಣಿಕೆ_2; ++ci)
    ಯದಿ (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        ಯದಿ (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
        ಪ್ರತ್ಯಾಗಮ ci;
      }
  ಪ್ರತ್ಯಾಗಮ ContributionIndex__none;
}
