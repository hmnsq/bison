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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "AnnotationList.h"

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "ielr.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"

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
ସ୍ଥିର AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_ଗଣନା,
                                  ସଂରଚନା obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_ଗଣନା * ଆକାର res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = ଖାଲି;
  res->inadequacy_ଗଣ୍ଠି = ଖାଲି;
  ଫେରନ୍ତୁ res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
ସ୍ଥିର bool
AnnotationList__isContributionAlways (AnnotationList ଅଚଳ *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2);
  ଫେରନ୍ତୁ self->contributions[ci] == ଖାଲି;
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
ସ୍ଥିର bool
Annotation_List_ଭର୍ତ୍ତି_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **ଗଣ୍ଠି;
  ପାଇଁ (ଗଣ୍ଠି = list; *ଗଣ୍ଠି; ଗଣ୍ଠି = &(*ଗଣ୍ଠି)->next)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = 0;
      ଯଦି (self->inadequacy_ଗଣ୍ଠି->id < (*ଗଣ୍ଠି)->inadequacy_ଗଣ୍ଠି->id)
        cmp = 1;
      ଅନ୍ୟଥା ଯଦି ((*ଗଣ୍ଠି)->inadequacy_ଗଣ୍ଠି->id < self->inadequacy_ଗଣ୍ଠି->id)
        cmp = -1;
      ଅନ୍ୟଥା
        ପାଇଁ (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2;
             ++ci)
          {
            ଯଦି (AnnotationList__isContributionAlways (self, ci))
              {
                ଯଦି (!AnnotationList__isContributionAlways (*ଗଣ୍ଠି, ci))
                  cmp = -1;
              }
            ଅନ୍ୟଥା ଯଦି (AnnotationList__isContributionAlways (*ଗଣ୍ଠି, ci))
              cmp = 1;
            ଅନ୍ୟଥା
              ପାଇଁ (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  ଯଦି (!Sbitset__test (self->contributions[ci], item))
                    {
                      ଯଦି (Sbitset__test ((*ଗଣ୍ଠି)->contributions[ci], item))
                        cmp = -1;
                    }
                  ଅନ୍ୟଥା ଯଦି (!Sbitset__test ((*ଗଣ୍ଠି)->contributions[ci], item))
                    cmp = 1;
                }
          }
      ଯଦି (cmp < 0)
        {
          self->next = *ଗଣ୍ଠି;
          *ଗଣ୍ଠି = self;
          ଭାଙ୍ଗ;
        }
      ଅନ୍ୟଥା ଯଦି (cmp == 0)
        {
          self = ଖାଲି;
          ଭାଙ୍ଗ;
        }
    }
  ଯଦି (!*ଗଣ୍ଠି)
    *ଗଣ୍ଠି = self;
  ଫେରନ୍ତୁ self != ଖାଲି;
}

ସ୍ଥିର bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_ପ୍ରତୀକ (trans, i));
  ଫେରନ୍ତୁ shift_tokens;
}

ସ୍ଥିର bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < reds->ଅଙ୍କ; ++i)
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

  ଫେରନ୍ତୁ conflicted_tokens;
}

ସ୍ଥିର bool
AnnotationList__compute_lhs_contributions (state *s, ଅଚଳ rule *the_rule,
                                           ପ୍ରତୀକ_ସଂଖ୍ୟା conflicted_token,
                                           bitsetv follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *ବସ୍ତୁଗୁଡ଼ିକ,
                                           ସଂରଚନା obstack
                                             *annotations_obstackp)
{
  goto_ସଂଖ୍ୟା lhs_goto = map_goto (s->ସଂଖ୍ୟା, the_rule->lhs->ସଂଖ୍ୟା);
  ଯଦି (bitset_test (always_follows[lhs_goto], conflicted_token))
    ଫେରନ୍ତୁ true;
  *ବସ୍ତୁଗୁଡ଼ିକ = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ[lhs_goto], item, 0)
      ଯଦି (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*ବସ୍ତୁଗୁଡ଼ିକ, item);
  }
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ସଂରଚନା obstack *annotations_obstackp)
{
  ପାଇଁ (state **predecessor = predecessors[s->ସଂଖ୍ୟା]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_ଗଣ୍ଠି =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2, annotations_obstackp);
      annotation_ଗଣ୍ଠି->inadequacy_ଗଣ୍ଠି = self->inadequacy_ଗଣ୍ଠି;
      bool potential_contribution = false;
      bitset *lookaheads = ଖାଲି;

      ପାଇଁ (ContributionIndex ci = 0; ci < self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2; ++ci)
        {
          ପ୍ରତୀକ_ସଂଖ୍ୟା contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_ଗଣ୍ଠି, ci)
            ->content->ସଂଖ୍ୟା;
          ଯଦି (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_ଗଣ୍ଠି->contributions[ci] = ଖାଲି;
              ଜାରି;
            }
          annotation_ଗଣ୍ଠି->contributions[ci] =
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
                aver (s->ବସ୍ତୁଗୁଡ଼ିକ[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                ଯଦି (item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (ritem[s->ବସ୍ତୁଗୁଡ଼ିକ[self_item] - 2]))
                  {
                    Sbitset ବସ୍ତୁଗୁଡ଼ିକ;
                    ଯଦି (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->ବସ୍ତୁଗୁଡ଼ିକ[self_item]]),
                          contribution_token,
                          follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ, always_follows, predecessors,
                          item_lookahead_sets, &ବସ୍ତୁଗୁଡ଼ିକ, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_ଗଣ୍ଠି->contributions[ci]);
                        annotation_ଗଣ୍ଠି->contributions[ci] = ଖାଲି;
                        // "Break" out of SBITSET__FOR_EACH.
                        ଯାଅ after_sbitset__for_each;
                      }
                    ଅନ୍ୟଥା
                      {
                        Sbitset__or (annotation_ଗଣ୍ଠି->contributions[ci],
                                     annotation_ଗଣ୍ଠି->contributions[ci],
                                     ବସ୍ତୁଗୁଡ଼ିକ, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, ବସ୍ତୁଗୁଡ଼ିକ);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                ଅନ୍ୟଥା
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    ପାଇଁ (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      ଯଦି ((*predecessor)->ବସ୍ତୁଗୁଡ଼ିକ[predecessor_item]
                          == s->ବସ୍ତୁଗୁଡ଼ିକ[self_item] - 1)
                        ଭାଙ୍ଗ;
                    aver (predecessor_item != (*predecessor)->nitems);
                    ଯଦି (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_ଗଣ୍ଠି->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          ଯଦି (annotation_ଗଣ୍ଠି->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_ଗଣ୍ଠି->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  ଯଦି (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             ଆକାର *lookaheads);
                      ପାଇଁ (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = ଖାଲି;
                    }
                  ଯଦି (!lookaheads[i])
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
      ଯଦି (potential_contribution)
        {
          ଯଦି (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_ଗଣ୍ଠି, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_ଗଣ୍ଠି);
              annotation_ଗଣ୍ଠି = ଖାଲି;
            }
          {
            ପାଇଁ (size_t i = 0; i < (*predecessor)->nitems; ++i)
              ଯଦି (lookaheads[i])
                bitset_free (lookaheads[i]);
            ମୁକ୍ତ (lookaheads);
          }
          ଯଦି (annotation_ଗଣ୍ଠି)
            {
              ଯଦି (Annotation_List_ଭର୍ତ୍ତି_Into (annotation_ଗଣ୍ଠି,
                                              &annotation_lists[(*predecessor)
                                                                ->ସଂଖ୍ୟା],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->ସଂଖ୍ୟା];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_ଗଣ୍ଠି, *predecessor,
                    follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              ଅନ୍ୟଥା
                obstack_free (annotations_obstackp, annotation_ଗଣ୍ଠି);
            }
        }
      ଅନ୍ୟଥା
        obstack_free (annotations_obstackp, annotation_ଗଣ୍ଠି);
    }
}

ଶୂନ୍ୟ
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  ସଂରଚନା obstack *annotations_obstackp,
  Inadequacy_List_ଗଣ୍ଠି_ଗଣନା *inadequacy_list_ଗଣ୍ଠି_ଗଣନା)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  ଯଦି (s->consistent)
    ଫେରନ୍ତୁ;

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
      AnnotationList *annotation_ଗଣ୍ଠି;
      ContributionIndex contribution_ଗଣନା = 0;

      /* Allocate the annotation node.  */
      {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_i = 0; rule_i < s->reductions->ଅଙ୍କ; ++rule_i)
          ଯଦି (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_ଗଣନା;
        ଯଦି (bitset_test (shift_tokens, conflicted_token))
          ++contribution_ଗଣନା;
        annotation_ଗଣ୍ଠି =
          AnnotationList__alloc_on_obstack (contribution_ଗଣନା,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->ଅଙ୍କ + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା item_i = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_i = 0; rule_i < s->reductions->ଅଙ୍କ; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            ଯଦି (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                ଯଦି (!item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (the_rule->rhs[0]))
                  {
                    annotation_ଗଣ୍ଠି->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    ଯେତେବେଳେ (!item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (ritem[s->ବସ୍ତୁଗୁଡ଼ିକ[item_i]])
                           || item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (ritem[s->ବସ୍ତୁଗୁଡ଼ିକ[item_i]]) != the_rule->ସଂଖ୍ୟା)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_ଗଣ୍ଠି->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                ଅନ୍ୟଥା ଯଦି (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_ଗଣ୍ଠି->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_ଗଣ୍ଠି->contributions[ci++] = ଖାଲି;
                    ଜାରି;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_ଗଣ୍ଠି->contributions[ci],
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
      ଯଦି (potential_contribution)
        {
          ଯଦି (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->ଅଙ୍କ);
              annotation_ଗଣ୍ଠି->contributions[contribution_ଗଣନା - 1] = ଖାଲି;
            }
          {
            InadequacyList *conflict_ଗଣ୍ଠି =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_ଗଣ୍ଠି_ଗଣନା);
            actions = ଖାଲି;
            annotation_ଗଣ୍ଠି->inadequacy_ଗଣ୍ଠି = conflict_ଗଣ୍ଠି;
            ଯଦି (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_ଗଣ୍ଠି, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_ଗଣ୍ଠି);
                InadequacyList__delete (conflict_ଗଣ୍ଠି);
              }
            ଅନ୍ୟଥା
              {
                InadequacyList__prependTo (conflict_ଗଣ୍ଠି,
                                           &inadequacy_lists[s->ସଂଖ୍ୟା]);
                {
                  bool b =
                    Annotation_List_ଭର୍ତ୍ତି_Into (annotation_ଗଣ୍ଠି,
                                                &annotation_lists[s->ସଂଖ୍ୟା],
                                                s->nitems);
                  aver (b); (ଶୂନ୍ୟ) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->ସଂଖ୍ୟା];
                ଯଦି (contribution_ଗଣନା > *max_contributionsp)
                  *max_contributionsp = contribution_ଗଣନା;
                AnnotationList__computePredecessorAnnotations (
                  annotation_ଗଣ୍ଠି, s,
                  follow_kernel_ବସ୍ତୁଗୁଡ଼ିକ, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      ଅନ୍ୟଥା
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_ଗଣ୍ଠି);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

ଶୂନ୍ୟ
AnnotationList__debug (AnnotationList ଅଚଳ *self, size_t nitems, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spaces)
{
  AnnotationList ଅଚଳ *a;
  AnnotationIndex ai;
  ପାଇଁ (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_ଗଣ୍ଠି->manifestingState->ସଂଖ୍ୟା);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_ଗଣ୍ଠି->inadequacy.conflict.actions);
      ପାଇଁ (ContributionIndex ci = 0; ci < a->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2; ++ci)
        {
          ପ୍ରତୀକ_ସଂଖ୍ୟା token =
            InadequacyList__getContributionToken (a->inadequacy_ଗଣ୍ଠି, ci)
            ->content->ସଂଖ୍ୟା;
          fprintf (stderr, "%*s", spaces+2, "");
          ଯଦି (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_ଗଣ୍ଠି))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          ଅନ୍ୟଥା
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_ଗଣ୍ଠି->manifestingState
                       ->reductions->rules[rulei]->ସଂଖ୍ୟା);
              rulei =
                bitset_next (a->inadequacy_ଗଣ୍ଠି->inadequacy.conflict.actions,
                             rulei+1);
              ଯଦି (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              ଅନ୍ୟଥା
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

ଶୂନ୍ୟ
AnnotationList__computeLookaheadFilter (AnnotationList ଅଚଳ *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  ପାଇଁ (; self; self = self->next)
    ପାଇଁ (ContributionIndex ci = 0; ci < self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2; ++ci)
      ଯଦି (!AnnotationList__isContributionAlways (self, ci))
        {
          ପ୍ରତୀକ_ସଂଖ୍ୟା token =
            InadequacyList__getContributionToken (self->inadequacy_ଗଣ୍ଠି, ci)
            ->content->ସଂଖ୍ୟା;
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
ସ୍ଥିର bool
AnnotationList__stateMakesContribution (AnnotationList ଅଚଳ *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  ଯଦି (AnnotationList__isContributionAlways (self, ci))
    ଫେରନ୍ତୁ true;
  ଯଦି (!lookaheads)
    ଫେରନ୍ତୁ false;
  {
    ପ୍ରତୀକ_ସଂଖ୍ୟା token =
      InadequacyList__getContributionToken (self->inadequacy_ଗଣ୍ଠି, ci)
      ->content->ସଂଖ୍ୟା;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      ଯଦି (lookaheads[item] && bitset_test (lookaheads[item], token))
        ଫେରନ୍ତୁ true;
  }
  ଫେରନ୍ତୁ false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList ଅଚଳ *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex ଅଚଳ ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_ଗଣ୍ଠି);

  ପ୍ରତୀକ *token = self->inadequacy_ଗଣ୍ଠି->inadequacy.conflict.token;

  /* S/R conflict.  */
  ଯଦି (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        ଯଦି (!shift_precedence)
          ଫେରନ୍ତୁ ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା actioni;
        ContributionIndex ci;
        ପାଇଁ (ci = 0,
               actioni = bitset_first (self->inadequacy_ଗଣ୍ଠି->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_ଗଣ୍ଠି->inadequacy
                                      .conflict.actions, actioni+1))
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା reduce_precedence = 0;
            ଯଦି (ci == ci_shift)
              ଜାରି;
            {
              rule *r = self->inadequacy_ଗଣ୍ଠି->manifestingState
                          ->reductions->rules[actioni];
              ଯଦି (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            ଯଦି (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == ଡାହାଣ_assoc)))
              ଜାରି;
            ଯଦି (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              ଜାରି;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            ଯଦି (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                ଯଦି (find_stable_domination_over_shift)
                  ଫେରନ୍ତୁ ContributionIndex__none;
                ଯଦି (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  ଫେରନ୍ତୁ ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            ଯଦି (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            ଯଦି (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                ଯଦି (find_stable_error_action_domination)
                  ଫେରନ୍ତୁ ContributionIndex__none;
                ଯଦି (!require_split_stable)
                  ଫେରନ୍ତୁ ci_rr_dominator;
                ଯଦି (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  ଫେରନ୍ତୁ ContributionIndex__none;
                ଯଦି (AnnotationList__isContributionAlways (self, ci))
                  ଫେରନ୍ତୁ ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      ଯଦି (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        ଫେରନ୍ତୁ ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      ଫେରନ୍ତୁ ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  ପାଇଁ (ContributionIndex ci = 0; ci < self->inadequacy_ଗଣ୍ଠି->contribution_ଗଣନା_2; ++ci)
    ଯଦି (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        ଯଦି (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          ଫେରନ୍ତୁ ContributionIndex__none;
        ଫେରନ୍ତୁ ci;
      }
  ଫେରନ୍ତୁ ContributionIndex__none;
}
