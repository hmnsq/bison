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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "AnnotationList.h"

#অন্তর্ভাব "system.h"

#অন্তর্ভাব "ielr.h"
#অন্তর্ভাব "lalr.h"

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
স্থির AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_গুনতি,
                                  সংরচনা obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_গুনতি * পরিমাণ res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = রিক্ত;
  res->inadequacy_গ্রন্থি = রিক্ত;
  প্রত্যাগম res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
স্থির bool
AnnotationList__isContributionAlways (AnnotationList ধ্রুব *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_গ্রন্থি->contribution_গুনতি_2);
  প্রত্যাগম self->contributions[ci] == রিক্ত;
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
স্থির bool
Annotation_List_ঢোকাও_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **গ্রন্থি;
  প্রতি (গ্রন্থি = list; *গ্রন্থি; গ্রন্থি = &(*গ্রন্থি)->next)
    {
      পূর্ণাঙ্ক cmp = 0;
      যদি (self->inadequacy_গ্রন্থি->id < (*গ্রন্থি)->inadequacy_গ্রন্থি->id)
        cmp = 1;
      অন্যথা যদি ((*গ্রন্থি)->inadequacy_গ্রন্থি->id < self->inadequacy_গ্রন্থি->id)
        cmp = -1;
      অন্যথা
        প্রতি (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_গ্রন্থি->contribution_গুনতি_2;
             ++ci)
          {
            যদি (AnnotationList__isContributionAlways (self, ci))
              {
                যদি (!AnnotationList__isContributionAlways (*গ্রন্থি, ci))
                  cmp = -1;
              }
            অন্যথা যদি (AnnotationList__isContributionAlways (*গ্রন্থি, ci))
              cmp = 1;
            অন্যথা
              প্রতি (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  যদি (!Sbitset__test (self->contributions[ci], item))
                    {
                      যদি (Sbitset__test ((*গ্রন্থি)->contributions[ci], item))
                        cmp = -1;
                    }
                  অন্যথা যদি (!Sbitset__test ((*গ্রন্থি)->contributions[ci], item))
                    cmp = 1;
                }
          }
      যদি (cmp < 0)
        {
          self->next = *গ্রন্থি;
          *গ্রন্থি = self;
          ভঙ্গ;
        }
      অন্যথা যদি (cmp == 0)
        {
          self = রিক্ত;
          ভঙ্গ;
        }
    }
  যদি (!*গ্রন্থি)
    *গ্রন্থি = self;
  প্রত্যাগম self != রিক্ত;
}

স্থির bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  পূর্ণাঙ্ক i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_প্রতীক (trans, i));
  প্রত্যাগম shift_tokens;
}

স্থির bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  প্রতি (পূর্ণাঙ্ক i = 0; i < reds->অঙ্ক; ++i)
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

  প্রত্যাগম conflicted_tokens;
}

স্থির bool
AnnotationList__compute_lhs_contributions (state *s, ধ্রুব rule *the_rule,
                                           প্রতীক_সংখ্যা conflicted_token,
                                           bitsetv follow_kernel_বস্তুসমূহ,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *বস্তুসমূহ,
                                           সংরচনা obstack
                                             *annotations_obstackp)
{
  goto_সংখ্যা lhs_goto = map_goto (s->সংখ্যা, the_rule->lhs->সংখ্যা);
  যদি (bitset_test (always_follows[lhs_goto], conflicted_token))
    প্রত্যাগম true;
  *বস্তুসমূহ = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_বস্তুসমূহ[lhs_goto], item, 0)
      যদি (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*বস্তুসমূহ, item);
  }
  প্রত্যাগম false;
}

স্থির শূন্য
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_বস্তুসমূহ,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  সংরচনা obstack *annotations_obstackp)
{
  প্রতি (state **predecessor = predecessors[s->সংখ্যা]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_গ্রন্থি =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_গ্রন্থি->contribution_গুনতি_2, annotations_obstackp);
      annotation_গ্রন্থি->inadequacy_গ্রন্থি = self->inadequacy_গ্রন্থি;
      bool potential_contribution = false;
      bitset *lookaheads = রিক্ত;

      প্রতি (ContributionIndex ci = 0; ci < self->inadequacy_গ্রন্থি->contribution_গুনতি_2; ++ci)
        {
          প্রতীক_সংখ্যা contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_গ্রন্থি, ci)
            ->content->সংখ্যা;
          যদি (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_গ্রন্থি->contributions[ci] = রিক্ত;
              অনুবর্ত;
            }
          annotation_গ্রন্থি->contributions[ci] =
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
                aver (s->বস্তুসমূহ[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                যদি (item_সংখ্যা_is_rule_সংখ্যা (ritem[s->বস্তুসমূহ[self_item] - 2]))
                  {
                    Sbitset বস্তুসমূহ;
                    যদি (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->বস্তুসমূহ[self_item]]),
                          contribution_token,
                          follow_kernel_বস্তুসমূহ, always_follows, predecessors,
                          item_lookahead_sets, &বস্তুসমূহ, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_গ্রন্থি->contributions[ci]);
                        annotation_গ্রন্থি->contributions[ci] = রিক্ত;
                        // "Break" out of SBITSET__FOR_EACH.
                        গচ্ছ after_sbitset__for_each;
                      }
                    অন্যথা
                      {
                        Sbitset__or (annotation_গ্রন্থি->contributions[ci],
                                     annotation_গ্রন্থি->contributions[ci],
                                     বস্তুসমূহ, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, বস্তুসমূহ);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                অন্যথা
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    প্রতি (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      যদি ((*predecessor)->বস্তুসমূহ[predecessor_item]
                          == s->বস্তুসমূহ[self_item] - 1)
                        ভঙ্গ;
                    aver (predecessor_item != (*predecessor)->nitems);
                    যদি (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_গ্রন্থি->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          যদি (annotation_গ্রন্থি->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_গ্রন্থি->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  যদি (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             পরিমাণ *lookaheads);
                      প্রতি (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = রিক্ত;
                    }
                  যদি (!lookaheads[i])
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
      যদি (potential_contribution)
        {
          যদি (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_গ্রন্থি, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_গ্রন্থি);
              annotation_গ্রন্থি = রিক্ত;
            }
          {
            প্রতি (size_t i = 0; i < (*predecessor)->nitems; ++i)
              যদি (lookaheads[i])
                bitset_free (lookaheads[i]);
            মুক্ত (lookaheads);
          }
          যদি (annotation_গ্রন্থি)
            {
              যদি (Annotation_List_ঢোকাও_Into (annotation_গ্রন্থি,
                                              &annotation_lists[(*predecessor)
                                                                ->সংখ্যা],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->সংখ্যা];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_গ্রন্থি, *predecessor,
                    follow_kernel_বস্তুসমূহ, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              অন্যথা
                obstack_free (annotations_obstackp, annotation_গ্রন্থি);
            }
        }
      অন্যথা
        obstack_free (annotations_obstackp, annotation_গ্রন্থি);
    }
}

শূন্য
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_বস্তুসমূহ, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  সংরচনা obstack *annotations_obstackp,
  Inadequacy_List_গ্রন্থি_গুনতি *inadequacy_list_গ্রন্থি_গুনতি)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  যদি (s->consistent)
    প্রত্যাগম;

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
      AnnotationList *annotation_গ্রন্থি;
      ContributionIndex contribution_গুনতি = 0;

      /* Allocate the annotation node.  */
      {
        প্রতি (পূর্ণাঙ্ক rule_i = 0; rule_i < s->reductions->অঙ্ক; ++rule_i)
          যদি (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_গুনতি;
        যদি (bitset_test (shift_tokens, conflicted_token))
          ++contribution_গুনতি;
        annotation_গ্রন্থি =
          AnnotationList__alloc_on_obstack (contribution_গুনতি,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->অঙ্ক + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        পূর্ণাঙ্ক item_i = 0;
        প্রতি (পূর্ণাঙ্ক rule_i = 0; rule_i < s->reductions->অঙ্ক; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            যদি (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                যদি (!item_সংখ্যা_is_rule_সংখ্যা (the_rule->rhs[0]))
                  {
                    annotation_গ্রন্থি->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    যাবত্ (!item_সংখ্যা_is_rule_সংখ্যা (ritem[s->বস্তুসমূহ[item_i]])
                           || item_সংখ্যা_as_rule_সংখ্যা (ritem[s->বস্তুসমূহ[item_i]]) != the_rule->সংখ্যা)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_গ্রন্থি->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                অন্যথা যদি (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_বস্তুসমূহ,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_গ্রন্থি->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_গ্রন্থি->contributions[ci++] = রিক্ত;
                    অনুবর্ত;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_গ্রন্থি->contributions[ci],
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
      যদি (potential_contribution)
        {
          যদি (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->অঙ্ক);
              annotation_গ্রন্থি->contributions[contribution_গুনতি - 1] = রিক্ত;
            }
          {
            InadequacyList *conflict_গ্রন্থি =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_গ্রন্থি_গুনতি);
            actions = রিক্ত;
            annotation_গ্রন্থি->inadequacy_গ্রন্থি = conflict_গ্রন্থি;
            যদি (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_গ্রন্থি, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_গ্রন্থি);
                InadequacyList__delete (conflict_গ্রন্থি);
              }
            অন্যথা
              {
                InadequacyList__prependTo (conflict_গ্রন্থি,
                                           &inadequacy_lists[s->সংখ্যা]);
                {
                  bool b =
                    Annotation_List_ঢোকাও_Into (annotation_গ্রন্থি,
                                                &annotation_lists[s->সংখ্যা],
                                                s->nitems);
                  aver (b); (শূন্য) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->সংখ্যা];
                যদি (contribution_গুনতি > *max_contributionsp)
                  *max_contributionsp = contribution_গুনতি;
                AnnotationList__computePredecessorAnnotations (
                  annotation_গ্রন্থি, s,
                  follow_kernel_বস্তুসমূহ, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      অন্যথা
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_গ্রন্থি);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

শূন্য
AnnotationList__debug (AnnotationList ধ্রুব *self, size_t nitems, পূর্ণাঙ্ক spaces)
{
  AnnotationList ধ্রুব *a;
  AnnotationIndex ai;
  প্রতি (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_গ্রন্থি->manifestingState->সংখ্যা);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_গ্রন্থি->inadequacy.conflict.actions);
      প্রতি (ContributionIndex ci = 0; ci < a->inadequacy_গ্রন্থি->contribution_গুনতি_2; ++ci)
        {
          প্রতীক_সংখ্যা token =
            InadequacyList__getContributionToken (a->inadequacy_গ্রন্থি, ci)
            ->content->সংখ্যা;
          fprintf (stderr, "%*s", spaces+2, "");
          যদি (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_গ্রন্থি))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          অন্যথা
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_গ্রন্থি->manifestingState
                       ->reductions->rules[rulei]->সংখ্যা);
              rulei =
                bitset_next (a->inadequacy_গ্রন্থি->inadequacy.conflict.actions,
                             rulei+1);
              যদি (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              অন্যথা
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

শূন্য
AnnotationList__computeLookaheadFilter (AnnotationList ধ্রুব *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  প্রতি (; self; self = self->next)
    প্রতি (ContributionIndex ci = 0; ci < self->inadequacy_গ্রন্থি->contribution_গুনতি_2; ++ci)
      যদি (!AnnotationList__isContributionAlways (self, ci))
        {
          প্রতীক_সংখ্যা token =
            InadequacyList__getContributionToken (self->inadequacy_গ্রন্থি, ci)
            ->content->সংখ্যা;
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
স্থির bool
AnnotationList__stateMakesContribution (AnnotationList ধ্রুব *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  যদি (AnnotationList__isContributionAlways (self, ci))
    প্রত্যাগম true;
  যদি (!lookaheads)
    প্রত্যাগম false;
  {
    প্রতীক_সংখ্যা token =
      InadequacyList__getContributionToken (self->inadequacy_গ্রন্থি, ci)
      ->content->সংখ্যা;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      যদি (lookaheads[item] && bitset_test (lookaheads[item], token))
        প্রত্যাগম true;
  }
  প্রত্যাগম false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList ধ্রুব *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex ধ্রুব ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_গ্রন্থি);

  প্রতীক *token = self->inadequacy_গ্রন্থি->inadequacy.conflict.token;

  /* S/R conflict.  */
  যদি (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        পূর্ণাঙ্ক shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        যদি (!shift_precedence)
          প্রত্যাগম ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        পূর্ণাঙ্ক actioni;
        ContributionIndex ci;
        প্রতি (ci = 0,
               actioni = bitset_first (self->inadequacy_গ্রন্থি->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_গ্রন্থি->contribution_গুনতি_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_গ্রন্থি->inadequacy
                                      .conflict.actions, actioni+1))
          {
            পূর্ণাঙ্ক reduce_precedence = 0;
            যদি (ci == ci_shift)
              অনুবর্ত;
            {
              rule *r = self->inadequacy_গ্রন্থি->manifestingState
                          ->reductions->rules[actioni];
              যদি (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            যদি (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == ডান_assoc)))
              অনুবর্ত;
            যদি (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              অনুবর্ত;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            যদি (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                যদি (find_stable_domination_over_shift)
                  প্রত্যাগম ContributionIndex__none;
                যদি (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  প্রত্যাগম ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            যদি (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            যদি (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                যদি (find_stable_error_action_domination)
                  প্রত্যাগম ContributionIndex__none;
                যদি (!require_split_stable)
                  প্রত্যাগম ci_rr_dominator;
                যদি (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  প্রত্যাগম ContributionIndex__none;
                যদি (AnnotationList__isContributionAlways (self, ci))
                  প্রত্যাগম ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      যদি (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        প্রত্যাগম ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      প্রত্যাগম ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  প্রতি (ContributionIndex ci = 0; ci < self->inadequacy_গ্রন্থি->contribution_গুনতি_2; ++ci)
    যদি (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        যদি (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          প্রত্যাগম ContributionIndex__none;
        প্রত্যাগম ci;
      }
  প্রত্যাগম ContributionIndex__none;
}
