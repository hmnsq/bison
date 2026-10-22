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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "AnnotationList.h"

#அந்தர்பாவ "system.h"

#அந்தர்பாவ "ielr.h"
#அந்தர்பாவ "lalr.h"

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
நிலையான AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_மொத்தம்,
                                  கட்டமைப்பு obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_மொத்தம் * அளவு res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = வெற்று;
  res->inadequacy_முனை = வெற்று;
  திரும்பு res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
நிலையான bool
AnnotationList__isContributionAlways (AnnotationList மாறிலி *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_முனை->contribution_மொத்தம்_2);
  திரும்பு self->contributions[ci] == வெற்று;
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
நிலையான bool
Annotation_List_செருகு_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **முனை;
  க்கு (முனை = list; *முனை; முனை = &(*முனை)->next)
    {
      முழு cmp = 0;
      எனில் (self->inadequacy_முனை->id < (*முனை)->inadequacy_முனை->id)
        cmp = 1;
      இல்லையெனில் எனில் ((*முனை)->inadequacy_முனை->id < self->inadequacy_முனை->id)
        cmp = -1;
      இல்லையெனில்
        க்கு (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_முனை->contribution_மொத்தம்_2;
             ++ci)
          {
            எனில் (AnnotationList__isContributionAlways (self, ci))
              {
                எனில் (!AnnotationList__isContributionAlways (*முனை, ci))
                  cmp = -1;
              }
            இல்லையெனில் எனில் (AnnotationList__isContributionAlways (*முனை, ci))
              cmp = 1;
            இல்லையெனில்
              க்கு (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  எனில் (!Sbitset__test (self->contributions[ci], item))
                    {
                      எனில் (Sbitset__test ((*முனை)->contributions[ci], item))
                        cmp = -1;
                    }
                  இல்லையெனில் எனில் (!Sbitset__test ((*முனை)->contributions[ci], item))
                    cmp = 1;
                }
          }
      எனில் (cmp < 0)
        {
          self->next = *முனை;
          *முனை = self;
          நிறுத்து;
        }
      இல்லையெனில் எனில் (cmp == 0)
        {
          self = வெற்று;
          நிறுத்து;
        }
    }
  எனில் (!*முனை)
    *முனை = self;
  திரும்பு self != வெற்று;
}

நிலையான bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  முழு i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_குறியீடு (trans, i));
  திரும்பு shift_tokens;
}

நிலையான bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  க்கு (முழு i = 0; i < reds->இலக்கம்; ++i)
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

  திரும்பு conflicted_tokens;
}

நிலையான bool
AnnotationList__compute_lhs_contributions (state *s, மாறிலி rule *the_rule,
                                           குறியீடு_எண் conflicted_token,
                                           bitsetv follow_kernel_பொருட்கள்,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *பொருட்கள்,
                                           கட்டமைப்பு obstack
                                             *annotations_obstackp)
{
  goto_எண் lhs_goto = map_goto (s->எண், the_rule->lhs->எண்);
  எனில் (bitset_test (always_follows[lhs_goto], conflicted_token))
    திரும்பு true;
  *பொருட்கள் = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_பொருட்கள்[lhs_goto], item, 0)
      எனில் (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*பொருட்கள், item);
  }
  திரும்பு false;
}

நிலையான வெறுமை
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_பொருட்கள்,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  கட்டமைப்பு obstack *annotations_obstackp)
{
  க்கு (state **predecessor = predecessors[s->எண்]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_முனை =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_முனை->contribution_மொத்தம்_2, annotations_obstackp);
      annotation_முனை->inadequacy_முனை = self->inadequacy_முனை;
      bool potential_contribution = false;
      bitset *lookaheads = வெற்று;

      க்கு (ContributionIndex ci = 0; ci < self->inadequacy_முனை->contribution_மொத்தம்_2; ++ci)
        {
          குறியீடு_எண் contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_முனை, ci)
            ->content->எண்;
          எனில் (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_முனை->contributions[ci] = வெற்று;
              தொடர்;
            }
          annotation_முனை->contributions[ci] =
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
                aver (s->பொருட்கள்[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                எனில் (item_எண்_is_rule_எண் (ritem[s->பொருட்கள்[self_item] - 2]))
                  {
                    Sbitset பொருட்கள்;
                    எனில் (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->பொருட்கள்[self_item]]),
                          contribution_token,
                          follow_kernel_பொருட்கள், always_follows, predecessors,
                          item_lookahead_sets, &பொருட்கள், annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_முனை->contributions[ci]);
                        annotation_முனை->contributions[ci] = வெற்று;
                        // "Break" out of SBITSET__FOR_EACH.
                        செல் after_sbitset__for_each;
                      }
                    இல்லையெனில்
                      {
                        Sbitset__or (annotation_முனை->contributions[ci],
                                     annotation_முனை->contributions[ci],
                                     பொருட்கள், (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, பொருட்கள்);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                இல்லையெனில்
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    க்கு (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      எனில் ((*predecessor)->பொருட்கள்[predecessor_item]
                          == s->பொருட்கள்[self_item] - 1)
                        நிறுத்து;
                    aver (predecessor_item != (*predecessor)->nitems);
                    எனில் (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_முனை->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          எனில் (annotation_முனை->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_முனை->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  எனில் (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             அளவு *lookaheads);
                      க்கு (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = வெற்று;
                    }
                  எனில் (!lookaheads[i])
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
      எனில் (potential_contribution)
        {
          எனில் (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_முனை, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_முனை);
              annotation_முனை = வெற்று;
            }
          {
            க்கு (size_t i = 0; i < (*predecessor)->nitems; ++i)
              எனில் (lookaheads[i])
                bitset_free (lookaheads[i]);
            விடுவி (lookaheads);
          }
          எனில் (annotation_முனை)
            {
              எனில் (Annotation_List_செருகு_Into (annotation_முனை,
                                              &annotation_lists[(*predecessor)
                                                                ->எண்],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->எண்];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_முனை, *predecessor,
                    follow_kernel_பொருட்கள், always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              இல்லையெனில்
                obstack_free (annotations_obstackp, annotation_முனை);
            }
        }
      இல்லையெனில்
        obstack_free (annotations_obstackp, annotation_முனை);
    }
}

வெறுமை
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_பொருட்கள், bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  கட்டமைப்பு obstack *annotations_obstackp,
  Inadequacy_List_முனை_மொத்தம் *inadequacy_list_முனை_மொத்தம்)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  எனில் (s->consistent)
    திரும்பு;

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
      AnnotationList *annotation_முனை;
      ContributionIndex contribution_மொத்தம் = 0;

      /* Allocate the annotation node.  */
      {
        க்கு (முழு rule_i = 0; rule_i < s->reductions->இலக்கம்; ++rule_i)
          எனில் (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_மொத்தம்;
        எனில் (bitset_test (shift_tokens, conflicted_token))
          ++contribution_மொத்தம்;
        annotation_முனை =
          AnnotationList__alloc_on_obstack (contribution_மொத்தம்,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->இலக்கம் + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        முழு item_i = 0;
        க்கு (முழு rule_i = 0; rule_i < s->reductions->இலக்கம்; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            எனில் (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                எனில் (!item_எண்_is_rule_எண் (the_rule->rhs[0]))
                  {
                    annotation_முனை->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    போது (!item_எண்_is_rule_எண் (ritem[s->பொருட்கள்[item_i]])
                           || item_எண்_as_rule_எண் (ritem[s->பொருட்கள்[item_i]]) != the_rule->எண்)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_முனை->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                இல்லையெனில் எனில் (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_பொருட்கள்,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_முனை->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_முனை->contributions[ci++] = வெற்று;
                    தொடர்;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_முனை->contributions[ci],
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
      எனில் (potential_contribution)
        {
          எனில் (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->இலக்கம்);
              annotation_முனை->contributions[contribution_மொத்தம் - 1] = வெற்று;
            }
          {
            InadequacyList *conflict_முனை =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_முனை_மொத்தம்);
            actions = வெற்று;
            annotation_முனை->inadequacy_முனை = conflict_முனை;
            எனில் (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_முனை, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_முனை);
                InadequacyList__delete (conflict_முனை);
              }
            இல்லையெனில்
              {
                InadequacyList__prependTo (conflict_முனை,
                                           &inadequacy_lists[s->எண்]);
                {
                  bool b =
                    Annotation_List_செருகு_Into (annotation_முனை,
                                                &annotation_lists[s->எண்],
                                                s->nitems);
                  aver (b); (வெறுமை) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->எண்];
                எனில் (contribution_மொத்தம் > *max_contributionsp)
                  *max_contributionsp = contribution_மொத்தம்;
                AnnotationList__computePredecessorAnnotations (
                  annotation_முனை, s,
                  follow_kernel_பொருட்கள், always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      இல்லையெனில்
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_முனை);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

வெறுமை
AnnotationList__debug (AnnotationList மாறிலி *self, size_t nitems, முழு spaces)
{
  AnnotationList மாறிலி *a;
  AnnotationIndex ai;
  க்கு (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_முனை->manifestingState->எண்);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_முனை->inadequacy.conflict.actions);
      க்கு (ContributionIndex ci = 0; ci < a->inadequacy_முனை->contribution_மொத்தம்_2; ++ci)
        {
          குறியீடு_எண் token =
            InadequacyList__getContributionToken (a->inadequacy_முனை, ci)
            ->content->எண்;
          fprintf (stderr, "%*s", spaces+2, "");
          எனில் (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_முனை))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          இல்லையெனில்
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_முனை->manifestingState
                       ->reductions->rules[rulei]->எண்);
              rulei =
                bitset_next (a->inadequacy_முனை->inadequacy.conflict.actions,
                             rulei+1);
              எனில் (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              இல்லையெனில்
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

வெறுமை
AnnotationList__computeLookaheadFilter (AnnotationList மாறிலி *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  க்கு (; self; self = self->next)
    க்கு (ContributionIndex ci = 0; ci < self->inadequacy_முனை->contribution_மொத்தம்_2; ++ci)
      எனில் (!AnnotationList__isContributionAlways (self, ci))
        {
          குறியீடு_எண் token =
            InadequacyList__getContributionToken (self->inadequacy_முனை, ci)
            ->content->எண்;
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
நிலையான bool
AnnotationList__stateMakesContribution (AnnotationList மாறிலி *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  எனில் (AnnotationList__isContributionAlways (self, ci))
    திரும்பு true;
  எனில் (!lookaheads)
    திரும்பு false;
  {
    குறியீடு_எண் token =
      InadequacyList__getContributionToken (self->inadequacy_முனை, ci)
      ->content->எண்;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      எனில் (lookaheads[item] && bitset_test (lookaheads[item], token))
        திரும்பு true;
  }
  திரும்பு false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList மாறிலி *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex மாறிலி ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_முனை);

  குறியீடு *token = self->inadequacy_முனை->inadequacy.conflict.token;

  /* S/R conflict.  */
  எனில் (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        முழு shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        எனில் (!shift_precedence)
          திரும்பு ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        முழு actioni;
        ContributionIndex ci;
        க்கு (ci = 0,
               actioni = bitset_first (self->inadequacy_முனை->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_முனை->contribution_மொத்தம்_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_முனை->inadequacy
                                      .conflict.actions, actioni+1))
          {
            முழு reduce_precedence = 0;
            எனில் (ci == ci_shift)
              தொடர்;
            {
              rule *r = self->inadequacy_முனை->manifestingState
                          ->reductions->rules[actioni];
              எனில் (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            எனில் (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == வலது_assoc)))
              தொடர்;
            எனில் (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              தொடர்;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            எனில் (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                எனில் (find_stable_domination_over_shift)
                  திரும்பு ContributionIndex__none;
                எனில் (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  திரும்பு ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            எனில் (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            எனில் (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                எனில் (find_stable_error_action_domination)
                  திரும்பு ContributionIndex__none;
                எனில் (!require_split_stable)
                  திரும்பு ci_rr_dominator;
                எனில் (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  திரும்பு ContributionIndex__none;
                எனில் (AnnotationList__isContributionAlways (self, ci))
                  திரும்பு ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      எனில் (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        திரும்பு ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      திரும்பு ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  க்கு (ContributionIndex ci = 0; ci < self->inadequacy_முனை->contribution_மொத்தம்_2; ++ci)
    எனில் (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        எனில் (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          திரும்பு ContributionIndex__none;
        திரும்பு ci;
      }
  திரும்பு ContributionIndex__none;
}
