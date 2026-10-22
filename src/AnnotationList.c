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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "AnnotationList.h"

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "ielr.h"
#અન્તર્ભાવ "lalr.h"

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
સ્થિર AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_ગણતરી,
                                  સંરચના obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_ગણતરી * કદ res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = ખાલી;
  res->inadequacy_ગાંઠ = ખાલી;
  પાછા res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
સ્થિર bool
AnnotationList__isContributionAlways (AnnotationList અચલ *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_ગાંઠ->contribution_ગણતરી_2);
  પાછા self->contributions[ci] == ખાલી;
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
સ્થિર bool
Annotation_List_ઉમેરો_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **ગાંઠ;
  માટે (ગાંઠ = list; *ગાંઠ; ગાંઠ = &(*ગાંઠ)->next)
    {
      પૂર્ણાંક cmp = 0;
      જો (self->inadequacy_ગાંઠ->id < (*ગાંઠ)->inadequacy_ગાંઠ->id)
        cmp = 1;
      નહીંતર જો ((*ગાંઠ)->inadequacy_ગાંઠ->id < self->inadequacy_ગાંઠ->id)
        cmp = -1;
      નહીંતર
        માટે (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_ગાંઠ->contribution_ગણતરી_2;
             ++ci)
          {
            જો (AnnotationList__isContributionAlways (self, ci))
              {
                જો (!AnnotationList__isContributionAlways (*ગાંઠ, ci))
                  cmp = -1;
              }
            નહીંતર જો (AnnotationList__isContributionAlways (*ગાંઠ, ci))
              cmp = 1;
            નહીંતર
              માટે (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  જો (!Sbitset__test (self->contributions[ci], item))
                    {
                      જો (Sbitset__test ((*ગાંઠ)->contributions[ci], item))
                        cmp = -1;
                    }
                  નહીંતર જો (!Sbitset__test ((*ગાંઠ)->contributions[ci], item))
                    cmp = 1;
                }
          }
      જો (cmp < 0)
        {
          self->next = *ગાંઠ;
          *ગાંઠ = self;
          તોડો;
        }
      નહીંતર જો (cmp == 0)
        {
          self = ખાલી;
          તોડો;
        }
    }
  જો (!*ગાંઠ)
    *ગાંઠ = self;
  પાછા self != ખાલી;
}

સ્થિર bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  પૂર્ણાંક i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_પ્રતીક (trans, i));
  પાછા shift_tokens;
}

સ્થિર bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  માટે (પૂર્ણાંક i = 0; i < reds->અંક; ++i)
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

  પાછા conflicted_tokens;
}

સ્થિર bool
AnnotationList__compute_lhs_contributions (state *s, અચલ rule *the_rule,
                                           પ્રતીક_સંખ્યા conflicted_token,
                                           bitsetv follow_kernel_વસ્તુઓ,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *વસ્તુઓ,
                                           સંરચના obstack
                                             *annotations_obstackp)
{
  goto_સંખ્યા lhs_goto = map_goto (s->સંખ્યા, the_rule->lhs->સંખ્યા);
  જો (bitset_test (always_follows[lhs_goto], conflicted_token))
    પાછા true;
  *વસ્તુઓ = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_વસ્તુઓ[lhs_goto], item, 0)
      જો (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*વસ્તુઓ, item);
  }
  પાછા false;
}

સ્થિર શૂન્ય
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_વસ્તુઓ,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  સંરચના obstack *annotations_obstackp)
{
  માટે (state **predecessor = predecessors[s->સંખ્યા]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_ગાંઠ =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_ગાંઠ->contribution_ગણતરી_2, annotations_obstackp);
      annotation_ગાંઠ->inadequacy_ગાંઠ = self->inadequacy_ગાંઠ;
      bool potential_contribution = false;
      bitset *lookaheads = ખાલી;

      માટે (ContributionIndex ci = 0; ci < self->inadequacy_ગાંઠ->contribution_ગણતરી_2; ++ci)
        {
          પ્રતીક_સંખ્યા contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_ગાંઠ, ci)
            ->content->સંખ્યા;
          જો (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_ગાંઠ->contributions[ci] = ખાલી;
              ચાલુ;
            }
          annotation_ગાંઠ->contributions[ci] =
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
                aver (s->વસ્તુઓ[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                જો (item_સંખ્યા_is_rule_સંખ્યા (ritem[s->વસ્તુઓ[self_item] - 2]))
                  {
                    Sbitset વસ્તુઓ;
                    જો (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->વસ્તુઓ[self_item]]),
                          contribution_token,
                          follow_kernel_વસ્તુઓ, always_follows, predecessors,
                          item_lookahead_sets, &વસ્તુઓ, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_ગાંઠ->contributions[ci]);
                        annotation_ગાંઠ->contributions[ci] = ખાલી;
                        // "Break" out of SBITSET__FOR_EACH.
                        જાઓ after_sbitset__for_each;
                      }
                    નહીંતર
                      {
                        Sbitset__or (annotation_ગાંઠ->contributions[ci],
                                     annotation_ગાંઠ->contributions[ci],
                                     વસ્તુઓ, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, વસ્તુઓ);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                નહીંતર
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    માટે (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      જો ((*predecessor)->વસ્તુઓ[predecessor_item]
                          == s->વસ્તુઓ[self_item] - 1)
                        તોડો;
                    aver (predecessor_item != (*predecessor)->nitems);
                    જો (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_ગાંઠ->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          જો (annotation_ગાંઠ->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_ગાંઠ->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  જો (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             કદ *lookaheads);
                      માટે (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = ખાલી;
                    }
                  જો (!lookaheads[i])
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
      જો (potential_contribution)
        {
          જો (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_ગાંઠ, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_ગાંઠ);
              annotation_ગાંઠ = ખાલી;
            }
          {
            માટે (size_t i = 0; i < (*predecessor)->nitems; ++i)
              જો (lookaheads[i])
                bitset_free (lookaheads[i]);
            મુક્ત (lookaheads);
          }
          જો (annotation_ગાંઠ)
            {
              જો (Annotation_List_ઉમેરો_Into (annotation_ગાંઠ,
                                              &annotation_lists[(*predecessor)
                                                                ->સંખ્યા],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->સંખ્યા];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_ગાંઠ, *predecessor,
                    follow_kernel_વસ્તુઓ, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              નહીંતર
                obstack_free (annotations_obstackp, annotation_ગાંઠ);
            }
        }
      નહીંતર
        obstack_free (annotations_obstackp, annotation_ગાંઠ);
    }
}

શૂન્ય
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_વસ્તુઓ, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  સંરચના obstack *annotations_obstackp,
  Inadequacy_List_ગાંઠ_ગણતરી *inadequacy_list_ગાંઠ_ગણતરી)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  જો (s->consistent)
    પાછા;

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
      AnnotationList *annotation_ગાંઠ;
      ContributionIndex contribution_ગણતરી = 0;

      /* Allocate the annotation node.  */
      {
        માટે (પૂર્ણાંક rule_i = 0; rule_i < s->reductions->અંક; ++rule_i)
          જો (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_ગણતરી;
        જો (bitset_test (shift_tokens, conflicted_token))
          ++contribution_ગણતરી;
        annotation_ગાંઠ =
          AnnotationList__alloc_on_obstack (contribution_ગણતરી,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->અંક + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        પૂર્ણાંક item_i = 0;
        માટે (પૂર્ણાંક rule_i = 0; rule_i < s->reductions->અંક; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            જો (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                જો (!item_સંખ્યા_is_rule_સંખ્યા (the_rule->rhs[0]))
                  {
                    annotation_ગાંઠ->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    જ્યારે (!item_સંખ્યા_is_rule_સંખ્યા (ritem[s->વસ્તુઓ[item_i]])
                           || item_સંખ્યા_as_rule_સંખ્યા (ritem[s->વસ્તુઓ[item_i]]) != the_rule->સંખ્યા)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_ગાંઠ->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                નહીંતર જો (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_વસ્તુઓ,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_ગાંઠ->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_ગાંઠ->contributions[ci++] = ખાલી;
                    ચાલુ;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_ગાંઠ->contributions[ci],
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
      જો (potential_contribution)
        {
          જો (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->અંક);
              annotation_ગાંઠ->contributions[contribution_ગણતરી - 1] = ખાલી;
            }
          {
            InadequacyList *conflict_ગાંઠ =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_ગાંઠ_ગણતરી);
            actions = ખાલી;
            annotation_ગાંઠ->inadequacy_ગાંઠ = conflict_ગાંઠ;
            જો (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_ગાંઠ, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_ગાંઠ);
                InadequacyList__delete (conflict_ગાંઠ);
              }
            નહીંતર
              {
                InadequacyList__prependTo (conflict_ગાંઠ,
                                           &inadequacy_lists[s->સંખ્યા]);
                {
                  bool b =
                    Annotation_List_ઉમેરો_Into (annotation_ગાંઠ,
                                                &annotation_lists[s->સંખ્યા],
                                                s->nitems);
                  aver (b); (શૂન્ય) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->સંખ્યા];
                જો (contribution_ગણતરી > *max_contributionsp)
                  *max_contributionsp = contribution_ગણતરી;
                AnnotationList__computePredecessorAnnotations (
                  annotation_ગાંઠ, s,
                  follow_kernel_વસ્તુઓ, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      નહીંતર
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_ગાંઠ);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

શૂન્ય
AnnotationList__debug (AnnotationList અચલ *self, size_t nitems, પૂર્ણાંક spaces)
{
  AnnotationList અચલ *a;
  AnnotationIndex ai;
  માટે (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_ગાંઠ->manifestingState->સંખ્યા);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_ગાંઠ->inadequacy.conflict.actions);
      માટે (ContributionIndex ci = 0; ci < a->inadequacy_ગાંઠ->contribution_ગણતરી_2; ++ci)
        {
          પ્રતીક_સંખ્યા token =
            InadequacyList__getContributionToken (a->inadequacy_ગાંઠ, ci)
            ->content->સંખ્યા;
          fprintf (stderr, "%*s", spaces+2, "");
          જો (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_ગાંઠ))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          નહીંતર
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_ગાંઠ->manifestingState
                       ->reductions->rules[rulei]->સંખ્યા);
              rulei =
                bitset_next (a->inadequacy_ગાંઠ->inadequacy.conflict.actions,
                             rulei+1);
              જો (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              નહીંતર
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

શૂન્ય
AnnotationList__computeLookaheadFilter (AnnotationList અચલ *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  માટે (; self; self = self->next)
    માટે (ContributionIndex ci = 0; ci < self->inadequacy_ગાંઠ->contribution_ગણતરી_2; ++ci)
      જો (!AnnotationList__isContributionAlways (self, ci))
        {
          પ્રતીક_સંખ્યા token =
            InadequacyList__getContributionToken (self->inadequacy_ગાંઠ, ci)
            ->content->સંખ્યા;
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
સ્થિર bool
AnnotationList__stateMakesContribution (AnnotationList અચલ *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  જો (AnnotationList__isContributionAlways (self, ci))
    પાછા true;
  જો (!lookaheads)
    પાછા false;
  {
    પ્રતીક_સંખ્યા token =
      InadequacyList__getContributionToken (self->inadequacy_ગાંઠ, ci)
      ->content->સંખ્યા;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      જો (lookaheads[item] && bitset_test (lookaheads[item], token))
        પાછા true;
  }
  પાછા false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList અચલ *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex અચલ ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_ગાંઠ);

  પ્રતીક *token = self->inadequacy_ગાંઠ->inadequacy.conflict.token;

  /* S/R conflict.  */
  જો (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        પૂર્ણાંક shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        જો (!shift_precedence)
          પાછા ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        પૂર્ણાંક actioni;
        ContributionIndex ci;
        માટે (ci = 0,
               actioni = bitset_first (self->inadequacy_ગાંઠ->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_ગાંઠ->contribution_ગણતરી_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_ગાંઠ->inadequacy
                                      .conflict.actions, actioni+1))
          {
            પૂર્ણાંક reduce_precedence = 0;
            જો (ci == ci_shift)
              ચાલુ;
            {
              rule *r = self->inadequacy_ગાંઠ->manifestingState
                          ->reductions->rules[actioni];
              જો (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            જો (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == જમણું_assoc)))
              ચાલુ;
            જો (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              ચાલુ;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            જો (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                જો (find_stable_domination_over_shift)
                  પાછા ContributionIndex__none;
                જો (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  પાછા ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            જો (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            જો (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                જો (find_stable_error_action_domination)
                  પાછા ContributionIndex__none;
                જો (!require_split_stable)
                  પાછા ci_rr_dominator;
                જો (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  પાછા ContributionIndex__none;
                જો (AnnotationList__isContributionAlways (self, ci))
                  પાછા ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      જો (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        પાછા ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      પાછા ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  માટે (ContributionIndex ci = 0; ci < self->inadequacy_ગાંઠ->contribution_ગણતરી_2; ++ci)
    જો (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        જો (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          પાછા ContributionIndex__none;
        પાછા ci;
      }
  પાછા ContributionIndex__none;
}
