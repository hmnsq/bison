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

#अन्तर्भाव <config.h>

#अन्तर्भाव "AnnotationList.h"

#अन्तर्भाव "system.h"

#अन्तर्भाव "ielr.h"
#अन्तर्भाव "lalr.h"

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
स्थिर AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_गणनम्,
                                  संरचना obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_गणनम् * परिमाण res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = रिक्त;
  res->inadequacy_ग्रन्थिः = रिक्त;
  प्रत्यागम res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
स्थिर bool
AnnotationList__isContributionAlways (AnnotationList ध्रुव *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_ग्रन्थिः->contribution_गणनम्_2);
  प्रत्यागम self->contributions[ci] == रिक्त;
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
स्थिर bool
Annotation_List_निवेशय_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **ग्रन्थिः;
  प्रति (ग्रन्थिः = list; *ग्रन्थिः; ग्रन्थिः = &(*ग्रन्थिः)->next)
    {
      पूर्णाङ्क cmp = 0;
      जइ (self->inadequacy_ग्रन्थिः->id < (*ग्रन्थिः)->inadequacy_ग्रन्थिः->id)
        cmp = 1;
      अण्णहा जइ ((*ग्रन्थिः)->inadequacy_ग्रन्थिः->id < self->inadequacy_ग्रन्थिः->id)
        cmp = -1;
      अण्णहा
        प्रति (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_ग्रन्थिः->contribution_गणनम्_2;
             ++ci)
          {
            जइ (AnnotationList__isContributionAlways (self, ci))
              {
                जइ (!AnnotationList__isContributionAlways (*ग्रन्थिः, ci))
                  cmp = -1;
              }
            अण्णहा जइ (AnnotationList__isContributionAlways (*ग्रन्थिः, ci))
              cmp = 1;
            अण्णहा
              प्रति (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  जइ (!Sbitset__test (self->contributions[ci], item))
                    {
                      जइ (Sbitset__test ((*ग्रन्थिः)->contributions[ci], item))
                        cmp = -1;
                    }
                  अण्णहा जइ (!Sbitset__test ((*ग्रन्थिः)->contributions[ci], item))
                    cmp = 1;
                }
          }
      जइ (cmp < 0)
        {
          self->next = *ग्रन्थिः;
          *ग्रन्थिः = self;
          भङ्ग;
        }
      अण्णहा जइ (cmp == 0)
        {
          self = रिक्त;
          भङ्ग;
        }
    }
  जइ (!*ग्रन्थिः)
    *ग्रन्थिः = self;
  प्रत्यागम self != रिक्त;
}

स्थिर bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  पूर्णाङ्क i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_चिह्नम् (trans, i));
  प्रत्यागम shift_tokens;
}

स्थिर bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  प्रति (पूर्णाङ्क i = 0; i < reds->अङ्कः; ++i)
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

  प्रत्यागम conflicted_tokens;
}

स्थिर bool
AnnotationList__compute_lhs_contributions (state *s, ध्रुव rule *the_rule,
                                           चिह्नम्_सङ्ख्या conflicted_token,
                                           bitsetv follow_kernel_वस्तूनि,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *वस्तूनि,
                                           संरचना obstack
                                             *annotations_obstackp)
{
  goto_सङ्ख्या lhs_goto = map_goto (s->सङ्ख्या, the_rule->lhs->सङ्ख्या);
  जइ (bitset_test (always_follows[lhs_goto], conflicted_token))
    प्रत्यागम true;
  *वस्तूनि = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_वस्तूनि[lhs_goto], item, 0)
      जइ (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*वस्तूनि, item);
  }
  प्रत्यागम false;
}

स्थिर शून्य
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_वस्तूनि,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  संरचना obstack *annotations_obstackp)
{
  प्रति (state **predecessor = predecessors[s->सङ्ख्या]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_ग्रन्थिः =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_ग्रन्थिः->contribution_गणनम्_2, annotations_obstackp);
      annotation_ग्रन्थिः->inadequacy_ग्रन्थिः = self->inadequacy_ग्रन्थिः;
      bool potential_contribution = false;
      bitset *lookaheads = रिक्त;

      प्रति (ContributionIndex ci = 0; ci < self->inadequacy_ग्रन्थिः->contribution_गणनम्_2; ++ci)
        {
          चिह्नम्_सङ्ख्या contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_ग्रन्थिः, ci)
            ->content->सङ्ख्या;
          जइ (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_ग्रन्थिः->contributions[ci] = रिक्त;
              अनुवर्त;
            }
          annotation_ग्रन्थिः->contributions[ci] =
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
                aver (s->वस्तूनि[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                जइ (item_सङ्ख्या_is_rule_सङ्ख्या (ritem[s->वस्तूनि[self_item] - 2]))
                  {
                    Sbitset वस्तूनि;
                    जइ (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->वस्तूनि[self_item]]),
                          contribution_token,
                          follow_kernel_वस्तूनि, always_follows, predecessors,
                          item_lookahead_sets, &वस्तूनि, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_ग्रन्थिः->contributions[ci]);
                        annotation_ग्रन्थिः->contributions[ci] = रिक्त;
                        // "Break" out of SBITSET__FOR_EACH.
                        गच्छ after_sbitset__for_each;
                      }
                    अण्णहा
                      {
                        Sbitset__or (annotation_ग्रन्थिः->contributions[ci],
                                     annotation_ग्रन्थिः->contributions[ci],
                                     वस्तूनि, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, वस्तूनि);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                अण्णहा
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    प्रति (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      जइ ((*predecessor)->वस्तूनि[predecessor_item]
                          == s->वस्तूनि[self_item] - 1)
                        भङ्ग;
                    aver (predecessor_item != (*predecessor)->nitems);
                    जइ (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_ग्रन्थिः->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          जइ (annotation_ग्रन्थिः->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_ग्रन्थिः->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  जइ (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             परिमाण *lookaheads);
                      प्रति (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = रिक्त;
                    }
                  जइ (!lookaheads[i])
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
      जइ (potential_contribution)
        {
          जइ (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_ग्रन्थिः, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_ग्रन्थिः);
              annotation_ग्रन्थिः = रिक्त;
            }
          {
            प्रति (size_t i = 0; i < (*predecessor)->nitems; ++i)
              जइ (lookaheads[i])
                bitset_free (lookaheads[i]);
            मुक्त (lookaheads);
          }
          जइ (annotation_ग्रन्थिः)
            {
              जइ (Annotation_List_निवेशय_Into (annotation_ग्रन्थिः,
                                              &annotation_lists[(*predecessor)
                                                                ->सङ्ख्या],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->सङ्ख्या];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_ग्रन्थिः, *predecessor,
                    follow_kernel_वस्तूनि, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              अण्णहा
                obstack_free (annotations_obstackp, annotation_ग्रन्थिः);
            }
        }
      अण्णहा
        obstack_free (annotations_obstackp, annotation_ग्रन्थिः);
    }
}

शून्य
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_वस्तूनि, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  संरचना obstack *annotations_obstackp,
  Inadequacy_List_ग्रन्थिः_गणनम् *inadequacy_list_ग्रन्थिः_गणनम्)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  जइ (s->consistent)
    प्रत्यागम;

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
      AnnotationList *annotation_ग्रन्थिः;
      ContributionIndex contribution_गणनम् = 0;

      /* Allocate the annotation node.  */
      {
        प्रति (पूर्णाङ्क rule_i = 0; rule_i < s->reductions->अङ्कः; ++rule_i)
          जइ (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_गणनम्;
        जइ (bitset_test (shift_tokens, conflicted_token))
          ++contribution_गणनम्;
        annotation_ग्रन्थिः =
          AnnotationList__alloc_on_obstack (contribution_गणनम्,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->अङ्कः + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        पूर्णाङ्क item_i = 0;
        प्रति (पूर्णाङ्क rule_i = 0; rule_i < s->reductions->अङ्कः; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            जइ (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                जइ (!item_सङ्ख्या_is_rule_सङ्ख्या (the_rule->rhs[0]))
                  {
                    annotation_ग्रन्थिः->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    जाव (!item_सङ्ख्या_is_rule_सङ्ख्या (ritem[s->वस्तूनि[item_i]])
                           || item_सङ्ख्या_as_rule_सङ्ख्या (ritem[s->वस्तूनि[item_i]]) != the_rule->सङ्ख्या)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_ग्रन्थिः->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                अण्णहा जइ (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_वस्तूनि,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_ग्रन्थिः->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_ग्रन्थिः->contributions[ci++] = रिक्त;
                    अनुवर्त;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_ग्रन्थिः->contributions[ci],
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
      जइ (potential_contribution)
        {
          जइ (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->अङ्कः);
              annotation_ग्रन्थिः->contributions[contribution_गणनम् - 1] = रिक्त;
            }
          {
            InadequacyList *conflict_ग्रन्थिः =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_ग्रन्थिः_गणनम्);
            actions = रिक्त;
            annotation_ग्रन्थिः->inadequacy_ग्रन्थिः = conflict_ग्रन्थिः;
            जइ (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_ग्रन्थिः, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_ग्रन्थिः);
                InadequacyList__delete (conflict_ग्रन्थिः);
              }
            अण्णहा
              {
                InadequacyList__prependTo (conflict_ग्रन्थिः,
                                           &inadequacy_lists[s->सङ्ख्या]);
                {
                  bool b =
                    Annotation_List_निवेशय_Into (annotation_ग्रन्थिः,
                                                &annotation_lists[s->सङ्ख्या],
                                                s->nitems);
                  aver (b); (शून्य) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->सङ्ख्या];
                जइ (contribution_गणनम् > *max_contributionsp)
                  *max_contributionsp = contribution_गणनम्;
                AnnotationList__computePredecessorAnnotations (
                  annotation_ग्रन्थिः, s,
                  follow_kernel_वस्तूनि, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      अण्णहा
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_ग्रन्थिः);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

शून्य
AnnotationList__debug (AnnotationList ध्रुव *self, size_t nitems, पूर्णाङ्क spaces)
{
  AnnotationList ध्रुव *a;
  AnnotationIndex ai;
  प्रति (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_ग्रन्थिः->manifestingState->सङ्ख्या);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_ग्रन्थिः->inadequacy.conflict.actions);
      प्रति (ContributionIndex ci = 0; ci < a->inadequacy_ग्रन्थिः->contribution_गणनम्_2; ++ci)
        {
          चिह्नम्_सङ्ख्या token =
            InadequacyList__getContributionToken (a->inadequacy_ग्रन्थिः, ci)
            ->content->सङ्ख्या;
          fprintf (stderr, "%*s", spaces+2, "");
          जइ (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_ग्रन्थिः))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          अण्णहा
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_ग्रन्थिः->manifestingState
                       ->reductions->rules[rulei]->सङ्ख्या);
              rulei =
                bitset_next (a->inadequacy_ग्रन्थिः->inadequacy.conflict.actions,
                             rulei+1);
              जइ (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              अण्णहा
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

शून्य
AnnotationList__computeLookaheadFilter (AnnotationList ध्रुव *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  प्रति (; self; self = self->next)
    प्रति (ContributionIndex ci = 0; ci < self->inadequacy_ग्रन्थिः->contribution_गणनम्_2; ++ci)
      जइ (!AnnotationList__isContributionAlways (self, ci))
        {
          चिह्नम्_सङ्ख्या token =
            InadequacyList__getContributionToken (self->inadequacy_ग्रन्थिः, ci)
            ->content->सङ्ख्या;
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
स्थिर bool
AnnotationList__stateMakesContribution (AnnotationList ध्रुव *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  जइ (AnnotationList__isContributionAlways (self, ci))
    प्रत्यागम true;
  जइ (!lookaheads)
    प्रत्यागम false;
  {
    चिह्नम्_सङ्ख्या token =
      InadequacyList__getContributionToken (self->inadequacy_ग्रन्थिः, ci)
      ->content->सङ्ख्या;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      जइ (lookaheads[item] && bitset_test (lookaheads[item], token))
        प्रत्यागम true;
  }
  प्रत्यागम false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList ध्रुव *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex ध्रुव ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_ग्रन्थिः);

  चिह्नम् *token = self->inadequacy_ग्रन्थिः->inadequacy.conflict.token;

  /* S/R conflict.  */
  जइ (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        पूर्णाङ्क shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        जइ (!shift_precedence)
          प्रत्यागम ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        पूर्णाङ्क actioni;
        ContributionIndex ci;
        प्रति (ci = 0,
               actioni = bitset_first (self->inadequacy_ग्रन्थिः->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_ग्रन्थिः->contribution_गणनम्_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_ग्रन्थिः->inadequacy
                                      .conflict.actions, actioni+1))
          {
            पूर्णाङ्क reduce_precedence = 0;
            जइ (ci == ci_shift)
              अनुवर्त;
            {
              rule *r = self->inadequacy_ग्रन्थिः->manifestingState
                          ->reductions->rules[actioni];
              जइ (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            जइ (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == दक्षिणः_assoc)))
              अनुवर्त;
            जइ (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              अनुवर्त;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            जइ (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                जइ (find_stable_domination_over_shift)
                  प्रत्यागम ContributionIndex__none;
                जइ (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  प्रत्यागम ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            जइ (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            जइ (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                जइ (find_stable_error_action_domination)
                  प्रत्यागम ContributionIndex__none;
                जइ (!require_split_stable)
                  प्रत्यागम ci_rr_dominator;
                जइ (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  प्रत्यागम ContributionIndex__none;
                जइ (AnnotationList__isContributionAlways (self, ci))
                  प्रत्यागम ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      जइ (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        प्रत्यागम ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      प्रत्यागम ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  प्रति (ContributionIndex ci = 0; ci < self->inadequacy_ग्रन्थिः->contribution_गणनम्_2; ++ci)
    जइ (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        जइ (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          प्रत्यागम ContributionIndex__none;
        प्रत्यागम ci;
      }
  प्रत्यागम ContributionIndex__none;
}
