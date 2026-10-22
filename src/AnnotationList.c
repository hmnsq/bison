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

#include <config.h>

#include "AnnotationList.h"

#include "system.h"

#include "ielr.h"
#include "lalr.h"

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
static AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_बिलांग,
                                  struct obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_बिलांग * sizeof res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = NULL;
  res->inadequacy_बुहोल = NULL;
  return res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
static bool
AnnotationList__isContributionAlways (AnnotationList const *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_बुहोल->contribution_बिलांग_2);
  return self->contributions[ci] == NULL;
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
static bool
Annotation_List_इसिंगित_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **बुहोल;
  for (बुहोल = list; *बुहोल; बुहोल = &(*बुहोल)->next)
    {
      int cmp = 0;
      if (self->inadequacy_बुहोल->id < (*बुहोल)->inadequacy_बुहोल->id)
        cmp = 1;
      else if ((*बुहोल)->inadequacy_बुहोल->id < self->inadequacy_बुहोल->id)
        cmp = -1;
      else
        for (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_बुहोल->contribution_बिलांग_2;
             ++ci)
          {
            if (AnnotationList__isContributionAlways (self, ci))
              {
                if (!AnnotationList__isContributionAlways (*बुहोल, ci))
                  cmp = -1;
              }
            else if (AnnotationList__isContributionAlways (*बुहोल, ci))
              cmp = 1;
            else
              for (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  if (!Sbitset__test (self->contributions[ci], item))
                    {
                      if (Sbitset__test ((*बुहोल)->contributions[ci], item))
                        cmp = -1;
                    }
                  else if (!Sbitset__test ((*बुहोल)->contributions[ci], item))
                    cmp = 1;
                }
          }
      if (cmp < 0)
        {
          self->next = *बुहोल;
          *बुहोल = self;
          break;
        }
      else if (cmp == 0)
        {
          self = NULL;
          break;
        }
    }
  if (!*बुहोल)
    *बुहोल = self;
  return self != NULL;
}

static bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  int i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_सिम्बोलो (trans, i));
  return shift_tokens;
}

static bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  for (int i = 0; i < reds->नुमेरो; ++i)
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

  return conflicted_tokens;
}

static bool
AnnotationList__compute_lhs_contributions (state *s, const rule *the_rule,
                                           सिम्बोलो_नुमेरो conflicted_token,
                                           bitsetv follow_kernel_मगा_बागाय,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *मगा_बागाय,
                                           struct obstack
                                             *annotations_obstackp)
{
  goto_नुमेरो lhs_goto = map_goto (s->नुमेरो_2, the_rule->lhs->नुमेरो_2);
  if (bitset_test (always_follows[lhs_goto], conflicted_token))
    return true;
  *मगा_बागाय = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_मगा_बागाय[lhs_goto], item, 0)
      if (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*मगा_बागाय, item);
  }
  return false;
}

static void
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_मगा_बागाय,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  struct obstack *annotations_obstackp)
{
  for (state **predecessor = predecessors[s->नुमेरो_2]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_बुहोल =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_बुहोल->contribution_बिलांग_2, annotations_obstackp);
      annotation_बुहोल->inadequacy_बुहोल = self->inadequacy_बुहोल;
      bool potential_contribution = false;
      bitset *lookaheads = NULL;

      for (ContributionIndex ci = 0; ci < self->inadequacy_बुहोल->contribution_बिलांग_2; ++ci)
        {
          सिम्बोलो_नुमेरो contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_बुहोल, ci)
            ->content->नुमेरो_2;
          if (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_बुहोल->contributions[ci] = NULL;
              continue;
            }
          annotation_बुहोल->contributions[ci] =
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
                aver (s->मगा_बागाय[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                if (item_नुमेरो_is_rule_नुमेरो (ritem[s->मगा_बागाय[self_item] - 2]))
                  {
                    Sbitset मगा_बागाय;
                    if (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->मगा_बागाय[self_item]]),
                          contribution_token,
                          follow_kernel_मगा_बागाय, always_follows, predecessors,
                          item_lookahead_sets, &मगा_बागाय, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_बुहोल->contributions[ci]);
                        annotation_बुहोल->contributions[ci] = NULL;
                        // "Break" out of SBITSET__FOR_EACH.
                        goto after_sbitset__for_each;
                      }
                    else
                      {
                        Sbitset__or (annotation_बुहोल->contributions[ci],
                                     annotation_बुहोल->contributions[ci],
                                     मगा_बागाय, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, मगा_बागाय);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                else
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    for (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      if ((*predecessor)->मगा_बागाय[predecessor_item]
                          == s->मगा_बागाय[self_item] - 1)
                        break;
                    aver (predecessor_item != (*predecessor)->nitems);
                    if (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_बुहोल->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          if (annotation_बुहोल->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_बुहोल->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  if (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             sizeof *lookaheads);
                      for (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = NULL;
                    }
                  if (!lookaheads[i])
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
      if (potential_contribution)
        {
          if (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_बुहोल, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_बुहोल);
              annotation_बुहोल = NULL;
            }
          {
            for (size_t i = 0; i < (*predecessor)->nitems; ++i)
              if (lookaheads[i])
                bitset_free (lookaheads[i]);
            free (lookaheads);
          }
          if (annotation_बुहोल)
            {
              if (Annotation_List_इसिंगित_Into (annotation_बुहोल,
                                              &annotation_lists[(*predecessor)
                                                                ->नुमेरो_2],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->नुमेरो_2];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_बुहोल, *predecessor,
                    follow_kernel_मगा_बागाय, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              else
                obstack_free (annotations_obstackp, annotation_बुहोल);
            }
        }
      else
        obstack_free (annotations_obstackp, annotation_बुहोल);
    }
}

void
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_मगा_बागाय, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  struct obstack *annotations_obstackp,
  Inadequacy_List_बुहोल_बिलांग *inadequacy_list_बुहोल_बिलांग)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  if (s->consistent)
    return;

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
      AnnotationList *annotation_बुहोल;
      ContributionIndex contribution_बिलांग = 0;

      /* Allocate the annotation node.  */
      {
        for (int rule_i = 0; rule_i < s->reductions->नुमेरो; ++rule_i)
          if (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_बिलांग;
        if (bitset_test (shift_tokens, conflicted_token))
          ++contribution_बिलांग;
        annotation_बुहोल =
          AnnotationList__alloc_on_obstack (contribution_बिलांग,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->नुमेरो + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        int item_i = 0;
        for (int rule_i = 0; rule_i < s->reductions->नुमेरो; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            if (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                if (!item_नुमेरो_is_rule_नुमेरो (the_rule->rhs[0]))
                  {
                    annotation_बुहोल->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    while (!item_नुमेरो_is_rule_नुमेरो (ritem[s->मगा_बागाय[item_i]])
                           || item_नुमेरो_as_rule_नुमेरो (ritem[s->मगा_बागाय[item_i]]) != the_rule->नुमेरो_2)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_बुहोल->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                else if (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_मगा_बागाय,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_बुहोल->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_बुहोल->contributions[ci++] = NULL;
                    continue;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_बुहोल->contributions[ci],
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
      if (potential_contribution)
        {
          if (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->नुमेरो);
              annotation_बुहोल->contributions[contribution_बिलांग - 1] = NULL;
            }
          {
            InadequacyList *conflict_बुहोल =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_बुहोल_बिलांग);
            actions = NULL;
            annotation_बुहोल->inadequacy_बुहोल = conflict_बुहोल;
            if (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_बुहोल, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_बुहोल);
                InadequacyList__delete (conflict_बुहोल);
              }
            else
              {
                InadequacyList__prependTo (conflict_बुहोल,
                                           &inadequacy_lists[s->नुमेरो_2]);
                {
                  bool b =
                    Annotation_List_इसिंगित_Into (annotation_बुहोल,
                                                &annotation_lists[s->नुमेरो_2],
                                                s->nitems);
                  aver (b); (void) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->नुमेरो_2];
                if (contribution_बिलांग > *max_contributionsp)
                  *max_contributionsp = contribution_बिलांग;
                AnnotationList__computePredecessorAnnotations (
                  annotation_बुहोल, s,
                  follow_kernel_मगा_बागाय, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      else
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_बुहोल);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

void
AnnotationList__debug (AnnotationList const *self, size_t nitems, int spaces)
{
  AnnotationList const *a;
  AnnotationIndex ai;
  for (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_बुहोल->manifestingState->नुमेरो_2);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_बुहोल->inadequacy.conflict.actions);
      for (ContributionIndex ci = 0; ci < a->inadequacy_बुहोल->contribution_बिलांग_2; ++ci)
        {
          सिम्बोलो_नुमेरो token =
            InadequacyList__getContributionToken (a->inadequacy_बुहोल, ci)
            ->content->नुमेरो_2;
          fprintf (stderr, "%*s", spaces+2, "");
          if (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_बुहोल))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          else
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_बुहोल->manifestingState
                       ->reductions->rules[rulei]->नुमेरो_2);
              rulei =
                bitset_next (a->inadequacy_बुहोल->inadequacy.conflict.actions,
                             rulei+1);
              if (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              else
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

void
AnnotationList__computeLookaheadFilter (AnnotationList const *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  for (; self; self = self->next)
    for (ContributionIndex ci = 0; ci < self->inadequacy_बुहोल->contribution_बिलांग_2; ++ci)
      if (!AnnotationList__isContributionAlways (self, ci))
        {
          सिम्बोलो_नुमेरो token =
            InadequacyList__getContributionToken (self->inadequacy_बुहोल, ci)
            ->content->नुमेरो_2;
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
static bool
AnnotationList__stateMakesContribution (AnnotationList const *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  if (AnnotationList__isContributionAlways (self, ci))
    return true;
  if (!lookaheads)
    return false;
  {
    सिम्बोलो_नुमेरो token =
      InadequacyList__getContributionToken (self->inadequacy_बुहोल, ci)
      ->content->नुमेरो_2;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      if (lookaheads[item] && bitset_test (lookaheads[item], token))
        return true;
  }
  return false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList const *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex const ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_बुहोल);

  सिम्बोलो *token = self->inadequacy_बुहोल->inadequacy.conflict.token;

  /* S/R conflict.  */
  if (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        int shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        if (!shift_precedence)
          return ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        int actioni;
        ContributionIndex ci;
        for (ci = 0,
               actioni = bitset_first (self->inadequacy_बुहोल->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_बुहोल->contribution_बिलांग_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_बुहोल->inadequacy
                                      .conflict.actions, actioni+1))
          {
            int reduce_precedence = 0;
            if (ci == ci_shift)
              continue;
            {
              rule *r = self->inadequacy_बुहोल->manifestingState
                          ->reductions->rules[actioni];
              if (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            if (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == कानान_assoc)))
              continue;
            if (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              continue;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            if (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                if (find_stable_domination_over_shift)
                  return ContributionIndex__none;
                if (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  return ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            if (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            if (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                if (find_stable_error_action_domination)
                  return ContributionIndex__none;
                if (!require_split_stable)
                  return ci_rr_dominator;
                if (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  return ContributionIndex__none;
                if (AnnotationList__isContributionAlways (self, ci))
                  return ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      if (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        return ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      return ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  for (ContributionIndex ci = 0; ci < self->inadequacy_बुहोल->contribution_बिलांग_2; ++ci)
    if (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        if (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          return ContributionIndex__none;
        return ci;
      }
  return ContributionIndex__none;
}
