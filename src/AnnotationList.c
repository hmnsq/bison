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

#شامل <config.h>

#شامل "AnnotationList.h"

#شامل "system.h"

#شامل "ielr.h"
#شامل "lalr.h"

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
ایستا AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_شمار,
                                  ساختار obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_شمار * اندازه res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = پوچ;
  res->inadequacy_گره = پوچ;
  بازگشت res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
ایستا bool
AnnotationList__isContributionAlways (AnnotationList ثابت *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_گره->contribution_شمار_2);
  بازگشت self->contributions[ci] == پوچ;
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
ایستا bool
Annotation_List_درج_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **گره;
  برای (گره = list; *گره; گره = &(*گره)->next)
    {
      صحیح cmp = 0;
      اگر (self->inadequacy_گره->id < (*گره)->inadequacy_گره->id)
        cmp = 1;
      وگرنه اگر ((*گره)->inadequacy_گره->id < self->inadequacy_گره->id)
        cmp = -1;
      وگرنه
        برای (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_گره->contribution_شمار_2;
             ++ci)
          {
            اگر (AnnotationList__isContributionAlways (self, ci))
              {
                اگر (!AnnotationList__isContributionAlways (*گره, ci))
                  cmp = -1;
              }
            وگرنه اگر (AnnotationList__isContributionAlways (*گره, ci))
              cmp = 1;
            وگرنه
              برای (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  اگر (!Sbitset__test (self->contributions[ci], item))
                    {
                      اگر (Sbitset__test ((*گره)->contributions[ci], item))
                        cmp = -1;
                    }
                  وگرنه اگر (!Sbitset__test ((*گره)->contributions[ci], item))
                    cmp = 1;
                }
          }
      اگر (cmp < 0)
        {
          self->next = *گره;
          *گره = self;
          بشکن;
        }
      وگرنه اگر (cmp == 0)
        {
          self = پوچ;
          بشکن;
        }
    }
  اگر (!*گره)
    *گره = self;
  بازگشت self != پوچ;
}

ایستا bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  صحیح i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_نماد (trans, i));
  بازگشت shift_tokens;
}

ایستا bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  برای (صحیح i = 0; i < reds->رقم; ++i)
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

  بازگشت conflicted_tokens;
}

ایستا bool
AnnotationList__compute_lhs_contributions (state *s, ثابت rule *the_rule,
                                           نماد_عدد conflicted_token,
                                           bitsetv follow_kernel_اقلام,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *اقلام,
                                           ساختار obstack
                                             *annotations_obstackp)
{
  goto_عدد lhs_goto = map_goto (s->عدد, the_rule->lhs->عدد);
  اگر (bitset_test (always_follows[lhs_goto], conflicted_token))
    بازگشت true;
  *اقلام = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_اقلام[lhs_goto], item, 0)
      اگر (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*اقلام, item);
  }
  بازگشت false;
}

ایستا تهی
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_اقلام,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ساختار obstack *annotations_obstackp)
{
  برای (state **predecessor = predecessors[s->عدد]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_گره =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_گره->contribution_شمار_2, annotations_obstackp);
      annotation_گره->inadequacy_گره = self->inadequacy_گره;
      bool potential_contribution = false;
      bitset *lookaheads = پوچ;

      برای (ContributionIndex ci = 0; ci < self->inadequacy_گره->contribution_شمار_2; ++ci)
        {
          نماد_عدد contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_گره, ci)
            ->content->عدد;
          اگر (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_گره->contributions[ci] = پوچ;
              ادامه;
            }
          annotation_گره->contributions[ci] =
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
                aver (s->اقلام[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                اگر (item_عدد_is_rule_عدد (ritem[s->اقلام[self_item] - 2]))
                  {
                    Sbitset اقلام;
                    اگر (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->اقلام[self_item]]),
                          contribution_token,
                          follow_kernel_اقلام, always_follows, predecessors,
                          item_lookahead_sets, &اقلام, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_گره->contributions[ci]);
                        annotation_گره->contributions[ci] = پوچ;
                        // "Break" out of SBITSET__FOR_EACH.
                        برو after_sbitset__for_each;
                      }
                    وگرنه
                      {
                        Sbitset__or (annotation_گره->contributions[ci],
                                     annotation_گره->contributions[ci],
                                     اقلام, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, اقلام);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                وگرنه
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    برای (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      اگر ((*predecessor)->اقلام[predecessor_item]
                          == s->اقلام[self_item] - 1)
                        بشکن;
                    aver (predecessor_item != (*predecessor)->nitems);
                    اگر (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_گره->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          اگر (annotation_گره->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_گره->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  اگر (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             اندازه *lookaheads);
                      برای (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = پوچ;
                    }
                  اگر (!lookaheads[i])
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
      اگر (potential_contribution)
        {
          اگر (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_گره, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_گره);
              annotation_گره = پوچ;
            }
          {
            برای (size_t i = 0; i < (*predecessor)->nitems; ++i)
              اگر (lookaheads[i])
                bitset_free (lookaheads[i]);
            آزاد (lookaheads);
          }
          اگر (annotation_گره)
            {
              اگر (Annotation_List_درج_Into (annotation_گره,
                                              &annotation_lists[(*predecessor)
                                                                ->عدد],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->عدد];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_گره, *predecessor,
                    follow_kernel_اقلام, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              وگرنه
                obstack_free (annotations_obstackp, annotation_گره);
            }
        }
      وگرنه
        obstack_free (annotations_obstackp, annotation_گره);
    }
}

تهی
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_اقلام, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  ساختار obstack *annotations_obstackp,
  Inadequacy_List_گره_شمار *inadequacy_list_گره_شمار)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  اگر (s->consistent)
    بازگشت;

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
      AnnotationList *annotation_گره;
      ContributionIndex contribution_شمار = 0;

      /* Allocate the annotation node.  */
      {
        برای (صحیح rule_i = 0; rule_i < s->reductions->رقم; ++rule_i)
          اگر (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_شمار;
        اگر (bitset_test (shift_tokens, conflicted_token))
          ++contribution_شمار;
        annotation_گره =
          AnnotationList__alloc_on_obstack (contribution_شمار,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->رقم + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        صحیح item_i = 0;
        برای (صحیح rule_i = 0; rule_i < s->reductions->رقم; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            اگر (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                اگر (!item_عدد_is_rule_عدد (the_rule->rhs[0]))
                  {
                    annotation_گره->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    وقتی (!item_عدد_is_rule_عدد (ritem[s->اقلام[item_i]])
                           || item_عدد_as_rule_عدد (ritem[s->اقلام[item_i]]) != the_rule->عدد)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_گره->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                وگرنه اگر (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_اقلام,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_گره->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_گره->contributions[ci++] = پوچ;
                    ادامه;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_گره->contributions[ci],
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
      اگر (potential_contribution)
        {
          اگر (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->رقم);
              annotation_گره->contributions[contribution_شمار - 1] = پوچ;
            }
          {
            InadequacyList *conflict_گره =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_گره_شمار);
            actions = پوچ;
            annotation_گره->inadequacy_گره = conflict_گره;
            اگر (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_گره, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_گره);
                InadequacyList__delete (conflict_گره);
              }
            وگرنه
              {
                InadequacyList__prependTo (conflict_گره,
                                           &inadequacy_lists[s->عدد]);
                {
                  bool b =
                    Annotation_List_درج_Into (annotation_گره,
                                                &annotation_lists[s->عدد],
                                                s->nitems);
                  aver (b); (تهی) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->عدد];
                اگر (contribution_شمار > *max_contributionsp)
                  *max_contributionsp = contribution_شمار;
                AnnotationList__computePredecessorAnnotations (
                  annotation_گره, s,
                  follow_kernel_اقلام, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      وگرنه
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_گره);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

تهی
AnnotationList__debug (AnnotationList ثابت *self, size_t nitems, صحیح spaces)
{
  AnnotationList ثابت *a;
  AnnotationIndex ai;
  برای (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_گره->manifestingState->عدد);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_گره->inadequacy.conflict.actions);
      برای (ContributionIndex ci = 0; ci < a->inadequacy_گره->contribution_شمار_2; ++ci)
        {
          نماد_عدد token =
            InadequacyList__getContributionToken (a->inadequacy_گره, ci)
            ->content->عدد;
          fprintf (stderr, "%*s", spaces+2, "");
          اگر (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_گره))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          وگرنه
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_گره->manifestingState
                       ->reductions->rules[rulei]->عدد);
              rulei =
                bitset_next (a->inadequacy_گره->inadequacy.conflict.actions,
                             rulei+1);
              اگر (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              وگرنه
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

تهی
AnnotationList__computeLookaheadFilter (AnnotationList ثابت *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  برای (; self; self = self->next)
    برای (ContributionIndex ci = 0; ci < self->inadequacy_گره->contribution_شمار_2; ++ci)
      اگر (!AnnotationList__isContributionAlways (self, ci))
        {
          نماد_عدد token =
            InadequacyList__getContributionToken (self->inadequacy_گره, ci)
            ->content->عدد;
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
ایستا bool
AnnotationList__stateMakesContribution (AnnotationList ثابت *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  اگر (AnnotationList__isContributionAlways (self, ci))
    بازگشت true;
  اگر (!lookaheads)
    بازگشت false;
  {
    نماد_عدد token =
      InadequacyList__getContributionToken (self->inadequacy_گره, ci)
      ->content->عدد;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      اگر (lookaheads[item] && bitset_test (lookaheads[item], token))
        بازگشت true;
  }
  بازگشت false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList ثابت *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex ثابت ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_گره);

  نماد *token = self->inadequacy_گره->inadequacy.conflict.token;

  /* S/R conflict.  */
  اگر (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        صحیح shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        اگر (!shift_precedence)
          بازگشت ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        صحیح actioni;
        ContributionIndex ci;
        برای (ci = 0,
               actioni = bitset_first (self->inadequacy_گره->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_گره->contribution_شمار_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_گره->inadequacy
                                      .conflict.actions, actioni+1))
          {
            صحیح reduce_precedence = 0;
            اگر (ci == ci_shift)
              ادامه;
            {
              rule *r = self->inadequacy_گره->manifestingState
                          ->reductions->rules[actioni];
              اگر (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            اگر (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == راست_assoc)))
              ادامه;
            اگر (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              ادامه;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            اگر (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                اگر (find_stable_domination_over_shift)
                  بازگشت ContributionIndex__none;
                اگر (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  بازگشت ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            اگر (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            اگر (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                اگر (find_stable_error_action_domination)
                  بازگشت ContributionIndex__none;
                اگر (!require_split_stable)
                  بازگشت ci_rr_dominator;
                اگر (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  بازگشت ContributionIndex__none;
                اگر (AnnotationList__isContributionAlways (self, ci))
                  بازگشت ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      اگر (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        بازگشت ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      بازگشت ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  برای (ContributionIndex ci = 0; ci < self->inadequacy_گره->contribution_شمار_2; ++ci)
    اگر (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        اگر (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          بازگشت ContributionIndex__none;
        بازگشت ci;
      }
  بازگشت ContributionIndex__none;
}
