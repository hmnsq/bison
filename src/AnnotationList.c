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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "AnnotationList.h"

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "ielr.h"
#അന്തര്ഭാവ "lalr.h"

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
സ്ഥിതം AnnotationList*
AnnotationList__alloc_on_obstack (ContributionIndex contribution_എണ്ണം,
                                  ഘടന obstack *annotations_obstackp)
{
  AnnotationList *res;
  size_t contributions_size = contribution_എണ്ണം * വലുപ്പം res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       offsetof (AnnotationList, contributions)
                       + contributions_size);
  res->next = ശൂന്യ;
  res->inadequacy_കണ്ണി = ശൂന്യ;
  തിരികെ res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
സ്ഥിതം bool
AnnotationList__isContributionAlways (AnnotationList സ്ഥിരം *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_കണ്ണി->contribution_എണ്ണം_2);
  തിരികെ self->contributions[ci] == ശൂന്യ;
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
സ്ഥിതം bool
Annotation_List_തിരുകുക_Into (AnnotationList *self, AnnotationList **list,
                            size_t nitems)
{
  AnnotationList **കണ്ണി;
  വേണ്ടി (കണ്ണി = list; *കണ്ണി; കണ്ണി = &(*കണ്ണി)->next)
    {
      പൂർണ്ണസംഖ്യ cmp = 0;
      എങ്കിൽ (self->inadequacy_കണ്ണി->id < (*കണ്ണി)->inadequacy_കണ്ണി->id)
        cmp = 1;
      അല്ലെങ്കിൽ എങ്കിൽ ((*കണ്ണി)->inadequacy_കണ്ണി->id < self->inadequacy_കണ്ണി->id)
        cmp = -1;
      അല്ലെങ്കിൽ
        വേണ്ടി (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_കണ്ണി->contribution_എണ്ണം_2;
             ++ci)
          {
            എങ്കിൽ (AnnotationList__isContributionAlways (self, ci))
              {
                എങ്കിൽ (!AnnotationList__isContributionAlways (*കണ്ണി, ci))
                  cmp = -1;
              }
            അല്ലെങ്കിൽ എങ്കിൽ (AnnotationList__isContributionAlways (*കണ്ണി, ci))
              cmp = 1;
            അല്ലെങ്കിൽ
              വേണ്ടി (size_t item = 0; cmp == 0 && item < nitems; ++item)
                {
                  എങ്കിൽ (!Sbitset__test (self->contributions[ci], item))
                    {
                      എങ്കിൽ (Sbitset__test ((*കണ്ണി)->contributions[ci], item))
                        cmp = -1;
                    }
                  അല്ലെങ്കിൽ എങ്കിൽ (!Sbitset__test ((*കണ്ണി)->contributions[ci], item))
                    cmp = 1;
                }
          }
      എങ്കിൽ (cmp < 0)
        {
          self->next = *കണ്ണി;
          *കണ്ണി = self;
          നിർത്തുക;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (cmp == 0)
        {
          self = ശൂന്യ;
          നിർത്തുക;
        }
    }
  എങ്കിൽ (!*കണ്ണി)
    *കണ്ണി = self;
  തിരികെ self != ശൂന്യ;
}

സ്ഥിതം bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  പൂർണ്ണസംഖ്യ i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_ചിഹ്നം (trans, i));
  തിരികെ shift_tokens;
}

സ്ഥിതം bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < reds->അക്കം; ++i)
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

  തിരികെ conflicted_tokens;
}

സ്ഥിതം bool
AnnotationList__compute_lhs_contributions (state *s, സ്ഥിരം rule *the_rule,
                                           ചിഹ്നം_സംഖ്യ conflicted_token,
                                           bitsetv follow_kernel_ഇനങ്ങൾ,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *ഇനങ്ങൾ,
                                           ഘടന obstack
                                             *annotations_obstackp)
{
  goto_സംഖ്യ lhs_goto = map_goto (s->സംഖ്യ, the_rule->lhs->സംഖ്യ);
  എങ്കിൽ (bitset_test (always_follows[lhs_goto], conflicted_token))
    തിരികെ true;
  *ഇനങ്ങൾ = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_ഇനങ്ങൾ[lhs_goto], item, 0)
      എങ്കിൽ (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*ഇനങ്ങൾ, item);
  }
  തിരികെ false;
}

സ്ഥിതം ശൂന്യം
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_ഇനങ്ങൾ,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ഘടന obstack *annotations_obstackp)
{
  വേണ്ടി (state **predecessor = predecessors[s->സംഖ്യ]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_കണ്ണി =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_കണ്ണി->contribution_എണ്ണം_2, annotations_obstackp);
      annotation_കണ്ണി->inadequacy_കണ്ണി = self->inadequacy_കണ്ണി;
      bool potential_contribution = false;
      bitset *lookaheads = ശൂന്യ;

      വേണ്ടി (ContributionIndex ci = 0; ci < self->inadequacy_കണ്ണി->contribution_എണ്ണം_2; ++ci)
        {
          ചിഹ്നം_സംഖ്യ contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_കണ്ണി, ci)
            ->content->സംഖ്യ;
          എങ്കിൽ (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_കണ്ണി->contributions[ci] = ശൂന്യ;
              തുടരുക;
            }
          annotation_കണ്ണി->contributions[ci] =
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
                aver (s->ഇനങ്ങൾ[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                എങ്കിൽ (item_സംഖ്യ_is_rule_സംഖ്യ (ritem[s->ഇനങ്ങൾ[self_item] - 2]))
                  {
                    Sbitset ഇനങ്ങൾ;
                    എങ്കിൽ (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->ഇനങ്ങൾ[self_item]]),
                          contribution_token,
                          follow_kernel_ഇനങ്ങൾ, always_follows, predecessors,
                          item_lookahead_sets, &ഇനങ്ങൾ, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_കണ്ണി->contributions[ci]);
                        annotation_കണ്ണി->contributions[ci] = ശൂന്യ;
                        // "Break" out of SBITSET__FOR_EACH.
                        പോകുക after_sbitset__for_each;
                      }
                    അല്ലെങ്കിൽ
                      {
                        Sbitset__or (annotation_കണ്ണി->contributions[ci],
                                     annotation_കണ്ണി->contributions[ci],
                                     ഇനങ്ങൾ, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, ഇനങ്ങൾ);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                അല്ലെങ്കിൽ
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    വേണ്ടി (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      എങ്കിൽ ((*predecessor)->ഇനങ്ങൾ[predecessor_item]
                          == s->ഇനങ്ങൾ[self_item] - 1)
                        നിർത്തുക;
                    aver (predecessor_item != (*predecessor)->nitems);
                    എങ്കിൽ (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_കണ്ണി->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          എങ്കിൽ (annotation_കണ്ണി->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_കണ്ണി->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  എങ്കിൽ (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             വലുപ്പം *lookaheads);
                      വേണ്ടി (size_t j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = ശൂന്യ;
                    }
                  എങ്കിൽ (!lookaheads[i])
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
      എങ്കിൽ (potential_contribution)
        {
          എങ്കിൽ (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_കണ്ണി, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_കണ്ണി);
              annotation_കണ്ണി = ശൂന്യ;
            }
          {
            വേണ്ടി (size_t i = 0; i < (*predecessor)->nitems; ++i)
              എങ്കിൽ (lookaheads[i])
                bitset_free (lookaheads[i]);
            സ്വതന്ത്രം (lookaheads);
          }
          എങ്കിൽ (annotation_കണ്ണി)
            {
              എങ്കിൽ (Annotation_List_തിരുകുക_Into (annotation_കണ്ണി,
                                              &annotation_lists[(*predecessor)
                                                                ->സംഖ്യ],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->സംഖ്യ];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_കണ്ണി, *predecessor,
                    follow_kernel_ഇനങ്ങൾ, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              അല്ലെങ്കിൽ
                obstack_free (annotations_obstackp, annotation_കണ്ണി);
            }
        }
      അല്ലെങ്കിൽ
        obstack_free (annotations_obstackp, annotation_കണ്ണി);
    }
}

ശൂന്യം
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_ഇനങ്ങൾ, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  ഘടന obstack *annotations_obstackp,
  Inadequacy_List_കണ്ണി_എണ്ണം *inadequacy_list_കണ്ണി_എണ്ണം)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  എങ്കിൽ (s->consistent)
    തിരികെ;

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
      AnnotationList *annotation_കണ്ണി;
      ContributionIndex contribution_എണ്ണം = 0;

      /* Allocate the annotation node.  */
      {
        വേണ്ടി (പൂർണ്ണസംഖ്യ rule_i = 0; rule_i < s->reductions->അക്കം; ++rule_i)
          എങ്കിൽ (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_എണ്ണം;
        എങ്കിൽ (bitset_test (shift_tokens, conflicted_token))
          ++contribution_എണ്ണം;
        annotation_കണ്ണി =
          AnnotationList__alloc_on_obstack (contribution_എണ്ണം,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->അക്കം + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        പൂർണ്ണസംഖ്യ item_i = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ rule_i = 0; rule_i < s->reductions->അക്കം; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            എങ്കിൽ (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                എങ്കിൽ (!item_സംഖ്യ_is_rule_സംഖ്യ (the_rule->rhs[0]))
                  {
                    annotation_കണ്ണി->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    ആകുമ്പോൾ (!item_സംഖ്യ_is_rule_സംഖ്യ (ritem[s->ഇനങ്ങൾ[item_i]])
                           || item_സംഖ്യ_as_rule_സംഖ്യ (ritem[s->ഇനങ്ങൾ[item_i]]) != the_rule->സംഖ്യ)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_കണ്ണി->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                അല്ലെങ്കിൽ എങ്കിൽ (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_ഇനങ്ങൾ,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_കണ്ണി->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_കണ്ണി->contributions[ci++] = ശൂന്യ;
                    തുടരുക;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_കണ്ണി->contributions[ci],
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
      എങ്കിൽ (potential_contribution)
        {
          എങ്കിൽ (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->അക്കം);
              annotation_കണ്ണി->contributions[contribution_എണ്ണം - 1] = ശൂന്യ;
            }
          {
            InadequacyList *conflict_കണ്ണി =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_കണ്ണി_എണ്ണം);
            actions = ശൂന്യ;
            annotation_കണ്ണി->inadequacy_കണ്ണി = conflict_കണ്ണി;
            എങ്കിൽ (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_കണ്ണി, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_കണ്ണി);
                InadequacyList__delete (conflict_കണ്ണി);
              }
            അല്ലെങ്കിൽ
              {
                InadequacyList__prependTo (conflict_കണ്ണി,
                                           &inadequacy_lists[s->സംഖ്യ]);
                {
                  bool b =
                    Annotation_List_തിരുകുക_Into (annotation_കണ്ണി,
                                                &annotation_lists[s->സംഖ്യ],
                                                s->nitems);
                  aver (b); (ശൂന്യം) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->സംഖ്യ];
                എങ്കിൽ (contribution_എണ്ണം > *max_contributionsp)
                  *max_contributionsp = contribution_എണ്ണം;
                AnnotationList__computePredecessorAnnotations (
                  annotation_കണ്ണി, s,
                  follow_kernel_ഇനങ്ങൾ, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      അല്ലെങ്കിൽ
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_കണ്ണി);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

ശൂന്യം
AnnotationList__debug (AnnotationList സ്ഥിരം *self, size_t nitems, പൂർണ്ണസംഖ്യ spaces)
{
  AnnotationList സ്ഥിരം *a;
  AnnotationIndex ai;
  വേണ്ടി (a = self, ai = 0; a; a = a->next, ++ai)
    {
      fprintf (stderr, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_കണ്ണി->manifestingState->സംഖ്യ);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_കണ്ണി->inadequacy.conflict.actions);
      വേണ്ടി (ContributionIndex ci = 0; ci < a->inadequacy_കണ്ണി->contribution_എണ്ണം_2; ++ci)
        {
          ചിഹ്നം_സംഖ്യ token =
            InadequacyList__getContributionToken (a->inadequacy_കണ്ണി, ci)
            ->content->സംഖ്യ;
          fprintf (stderr, "%*s", spaces+2, "");
          എങ്കിൽ (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_കണ്ണി))
            fprintf (stderr, "Contributes shift of token %d.\n", token);
          അല്ലെങ്കിൽ
            {
              fprintf (stderr, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              fprintf (stderr, " as lookahead, rule number %d",
                       a->inadequacy_കണ്ണി->manifestingState
                       ->reductions->rules[rulei]->സംഖ്യ);
              rulei =
                bitset_next (a->inadequacy_കണ്ണി->inadequacy.conflict.actions,
                             rulei+1);
              എങ്കിൽ (AnnotationList__isContributionAlways (a, ci))
                fprintf (stderr, " always.");
              അല്ലെങ്കിൽ
                {
                  fprintf (stderr, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, stderr);
                }
              fprintf (stderr, "\n");
            }
        }
    }
}

ശൂന്യം
AnnotationList__computeLookaheadFilter (AnnotationList സ്ഥിരം *self,
                                        size_t nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  വേണ്ടി (; self; self = self->next)
    വേണ്ടി (ContributionIndex ci = 0; ci < self->inadequacy_കണ്ണി->contribution_എണ്ണം_2; ++ci)
      എങ്കിൽ (!AnnotationList__isContributionAlways (self, ci))
        {
          ചിഹ്നം_സംഖ്യ token =
            InadequacyList__getContributionToken (self->inadequacy_കണ്ണി, ci)
            ->content->സംഖ്യ;
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
സ്ഥിതം bool
AnnotationList__stateMakesContribution (AnnotationList സ്ഥിരം *self,
                                        size_t nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  എങ്കിൽ (AnnotationList__isContributionAlways (self, ci))
    തിരികെ true;
  എങ്കിൽ (!lookaheads)
    തിരികെ false;
  {
    ചിഹ്നം_സംഖ്യ token =
      InadequacyList__getContributionToken (self->inadequacy_കണ്ണി, ci)
      ->content->സംഖ്യ;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      എങ്കിൽ (lookaheads[item] && bitset_test (lookaheads[item], token))
        തിരികെ true;
  }
  തിരികെ false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList സ്ഥിരം *self,
                                             size_t nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex സ്ഥിരം ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_കണ്ണി);

  ചിഹ്നം *token = self->inadequacy_കണ്ണി->inadequacy.conflict.token;

  /* S/R conflict.  */
  എങ്കിൽ (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        പൂർണ്ണസംഖ്യ shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        എങ്കിൽ (!shift_precedence)
          തിരികെ ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        പൂർണ്ണസംഖ്യ actioni;
        ContributionIndex ci;
        വേണ്ടി (ci = 0,
               actioni = bitset_first (self->inadequacy_കണ്ണി->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_കണ്ണി->contribution_എണ്ണം_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_കണ്ണി->inadequacy
                                      .conflict.actions, actioni+1))
          {
            പൂർണ്ണസംഖ്യ reduce_precedence = 0;
            എങ്കിൽ (ci == ci_shift)
              തുടരുക;
            {
              rule *r = self->inadequacy_കണ്ണി->manifestingState
                          ->reductions->rules[actioni];
              എങ്കിൽ (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            എങ്കിൽ (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == വലത്_assoc)))
              തുടരുക;
            എങ്കിൽ (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              തുടരുക;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            എങ്കിൽ (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                എങ്കിൽ (find_stable_domination_over_shift)
                  തിരികെ ContributionIndex__none;
                എങ്കിൽ (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  തിരികെ ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            എങ്കിൽ (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            എങ്കിൽ (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                എങ്കിൽ (find_stable_error_action_domination)
                  തിരികെ ContributionIndex__none;
                എങ്കിൽ (!require_split_stable)
                  തിരികെ ci_rr_dominator;
                എങ്കിൽ (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  തിരികെ ContributionIndex__none;
                എങ്കിൽ (AnnotationList__isContributionAlways (self, ci))
                  തിരികെ ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      എങ്കിൽ (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        തിരികെ ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      തിരികെ ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  വേണ്ടി (ContributionIndex ci = 0; ci < self->inadequacy_കണ്ണി->contribution_എണ്ണം_2; ++ci)
    എങ്കിൽ (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        എങ്കിൽ (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          തിരികെ ContributionIndex__none;
        തിരികെ ci;
      }
  തിരികെ ContributionIndex__none;
}
