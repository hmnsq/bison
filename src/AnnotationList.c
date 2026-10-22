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

#समावेश <config.h>

#समावेश "AnnotationList.h"

#समावेश "system.h"

#समावेश "ielr.h"
#समावेश "lalr.h"

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
अटल AnnotationList*
AnnotationList__alloc_on_obstack (Contribution_सूचकांक contribution_गिनती,
                                  काष्ठा obstack *annotations_obstackp)
{
  AnnotationList *res;
  माप_प्रकार contributions_आकार = contribution_गिनती * माप res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       दुरत्व (AnnotationList, contributions)
                       + contributions_आकार);
  res->अग्र = शून्य;
  res->inadequacy_गाँठ = शून्य;
  वापस res;
}

/**
 * \pre
 *   - <tt>self != NULL</tt>.
 *   - <tt>0 <= ci < self->inadequacyNode->contributionCount</tt>.
 * \post
 *   - \c result = true iff contribution \c ci in \c self represents an
 *     "always" contribution.
 */
अटल bool
AnnotationList__isContributionAlways (AnnotationList स्थिर *self,
                                      Contribution_सूचकांक ci)
{
  aver (0 <= ci && ci < self->inadequacy_गाँठ->contribution_गिनती_2);
  वापस self->contributions[ci] == शून्य;
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
अटल bool
Annotation_List_जोड़ो_Into (AnnotationList *self, AnnotationList **list,
                            माप_प्रकार nitems)
{
  AnnotationList **गाँठ;
  क्रम (गाँठ = list; *गाँठ; गाँठ = &(*गाँठ)->अग्र)
    {
      पूर्णांक cmp = 0;
      अगर (self->inadequacy_गाँठ->id < (*गाँठ)->inadequacy_गाँठ->id)
        cmp = 1;
      अन्यथा अगर ((*गाँठ)->inadequacy_गाँठ->id < self->inadequacy_गाँठ->id)
        cmp = -1;
      अन्यथा
        क्रम (Contribution_सूचकांक ci = 0;
             cmp == 0 && ci < self->inadequacy_गाँठ->contribution_गिनती_2;
             ++ci)
          {
            अगर (AnnotationList__isContributionAlways (self, ci))
              {
                अगर (!AnnotationList__isContributionAlways (*गाँठ, ci))
                  cmp = -1;
              }
            अन्यथा अगर (AnnotationList__isContributionAlways (*गाँठ, ci))
              cmp = 1;
            अन्यथा
              क्रम (माप_प्रकार item = 0; cmp == 0 && item < nitems; ++item)
                {
                  अगर (!Sbitset__test (self->contributions[ci], item))
                    {
                      अगर (Sbitset__test ((*गाँठ)->contributions[ci], item))
                        cmp = -1;
                    }
                  अन्यथा अगर (!Sbitset__test ((*गाँठ)->contributions[ci], item))
                    cmp = 1;
                }
          }
      अगर (cmp < 0)
        {
          self->अग्र = *गाँठ;
          *गाँठ = self;
          अवरोध;
        }
      अन्यथा अगर (cmp == 0)
        {
          self = शून्य;
          अवरोध;
        }
    }
  अगर (!*गाँठ)
    *गाँठ = self;
  वापस self != शून्य;
}

अटल bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  पूर्णांक i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_प्रतीक (trans, i));
  वापस shift_tokens;
}

अटल bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  क्रम (पूर्णांक i = 0; i < reds->अंक; ++i)
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

  वापस conflicted_tokens;
}

अटल bool
AnnotationList__compute_lhs_contributions (state *s, स्थिर rule *the_rule,
                                           प्रतीक_संख्या conflicted_token,
                                           bitsetv follow_kernel_वस्तुएँ,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *वस्तुएँ,
                                           काष्ठा obstack
                                             *annotations_obstackp)
{
  goto_संख्या lhs_goto = map_goto (s->संख्या, the_rule->lhs->संख्या);
  अगर (bitset_test (always_follows[lhs_goto], conflicted_token))
    वापस true;
  *वस्तुएँ = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_वस्तुएँ[lhs_goto], item, 0)
      अगर (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*वस्तुएँ, item);
  }
  वापस false;
}

अटल व्योम
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_वस्तुएँ,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  Annotation_सूचकांक *annotation_counts,
  काष्ठा obstack *annotations_obstackp)
{
  क्रम (state **predecessor = predecessors[s->संख्या]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_गाँठ =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_गाँठ->contribution_गिनती_2, annotations_obstackp);
      annotation_गाँठ->inadequacy_गाँठ = self->inadequacy_गाँठ;
      bool potential_contribution = false;
      bitset *lookaheads = शून्य;

      क्रम (Contribution_सूचकांक ci = 0; ci < self->inadequacy_गाँठ->contribution_गिनती_2; ++ci)
        {
          प्रतीक_संख्या contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_गाँठ, ci)
            ->content->संख्या;
          अगर (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_गाँठ->contributions[ci] = शून्य;
              जारी;
            }
          annotation_गाँठ->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            माप_प्रकार predecessor_item = 0;
            Sbitset sbiter_item;
            Sbitset_सूचकांक self_item;
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
                aver (s->वस्तुएँ[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                अगर (item_संख्या_is_rule_संख्या (ritem[s->वस्तुएँ[self_item] - 2]))
                  {
                    Sbitset वस्तुएँ;
                    अगर (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->वस्तुएँ[self_item]]),
                          contribution_token,
                          follow_kernel_वस्तुएँ, always_follows, predecessors,
                          item_lookahead_sets, &वस्तुएँ, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_गाँठ->contributions[ci]);
                        annotation_गाँठ->contributions[ci] = शून्य;
                        // "Break" out of SBITSET__FOR_EACH.
                        जाओ after_sbitset__for_each;
                      }
                    अन्यथा
                      {
                        Sbitset__or (annotation_गाँठ->contributions[ci],
                                     annotation_गाँठ->contributions[ci],
                                     वस्तुएँ, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, वस्तुएँ);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                अन्यथा
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    क्रम (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      अगर ((*predecessor)->वस्तुएँ[predecessor_item]
                          == s->वस्तुएँ[self_item] - 1)
                        अवरोध;
                    aver (predecessor_item != (*predecessor)->nitems);
                    अगर (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_गाँठ->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          अगर (annotation_गाँठ->contributions[ci])
            {
              Sbitset biter;
              Sbitset_सूचकांक i;
              SBITSET__FOR_EACH (annotation_गाँठ->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  अगर (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             माप *lookaheads);
                      क्रम (माप_प्रकार j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = शून्य;
                    }
                  अगर (!lookaheads[i])
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
      अगर (potential_contribution)
        {
          अगर (Contribution_सूचकांक_none
              != AnnotationList__computeDominantContribution (
                   annotation_गाँठ, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_गाँठ);
              annotation_गाँठ = शून्य;
            }
          {
            क्रम (माप_प्रकार i = 0; i < (*predecessor)->nitems; ++i)
              अगर (lookaheads[i])
                bitset_free (lookaheads[i]);
            मुक्त (lookaheads);
          }
          अगर (annotation_गाँठ)
            {
              अगर (Annotation_List_जोड़ो_Into (annotation_गाँठ,
                                              &annotation_lists[(*predecessor)
                                                                ->संख्या],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->संख्या];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_गाँठ, *predecessor,
                    follow_kernel_वस्तुएँ, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              अन्यथा
                obstack_free (annotations_obstackp, annotation_गाँठ);
            }
        }
      अन्यथा
        obstack_free (annotations_obstackp, annotation_गाँठ);
    }
}

व्योम
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_वस्तुएँ, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  Annotation_सूचकांक *annotation_counts,
  Contribution_सूचकांक *max_contributionsp,
  काष्ठा obstack *annotations_obstackp,
  Inadequacy_List_गाँठ_गिनती *inadequacy_list_गाँठ_गिनती)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  अगर (s->consistent)
    वापस;

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
      AnnotationList *annotation_गाँठ;
      Contribution_सूचकांक contribution_गिनती = 0;

      /* Allocate the annotation node.  */
      {
        क्रम (पूर्णांक rule_i = 0; rule_i < s->reductions->अंक; ++rule_i)
          अगर (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_गिनती;
        अगर (bitset_test (shift_tokens, conflicted_token))
          ++contribution_गिनती;
        annotation_गाँठ =
          AnnotationList__alloc_on_obstack (contribution_गिनती,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->अंक + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        Contribution_सूचकांक ci = 0;
        पूर्णांक item_i = 0;
        क्रम (पूर्णांक rule_i = 0; rule_i < s->reductions->अंक; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            अगर (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                अगर (!item_संख्या_is_rule_संख्या (the_rule->rhs[0]))
                  {
                    annotation_गाँठ->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    जबतक (!item_संख्या_is_rule_संख्या (ritem[s->वस्तुएँ[item_i]])
                           || item_संख्या_as_rule_संख्या (ritem[s->वस्तुएँ[item_i]]) != the_rule->संख्या)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_गाँठ->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                अन्यथा अगर (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_वस्तुएँ,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_गाँठ->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_गाँठ->contributions[ci++] = शून्य;
                    जारी;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_गाँठ->contributions[ci],
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
      अगर (potential_contribution)
        {
          अगर (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->अंक);
              annotation_गाँठ->contributions[contribution_गिनती - 1] = शून्य;
            }
          {
            InadequacyList *conflict_गाँठ =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_गाँठ_गिनती);
            actions = शून्य;
            annotation_गाँठ->inadequacy_गाँठ = conflict_गाँठ;
            अगर (Contribution_सूचकांक_none
                != AnnotationList__computeDominantContribution (
                     annotation_गाँठ, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_गाँठ);
                InadequacyList__delete (conflict_गाँठ);
              }
            अन्यथा
              {
                InadequacyList__prependTo (conflict_गाँठ,
                                           &inadequacy_lists[s->संख्या]);
                {
                  bool b =
                    Annotation_List_जोड़ो_Into (annotation_गाँठ,
                                                &annotation_lists[s->संख्या],
                                                s->nitems);
                  aver (b); (व्योम) b;
                }
                /* This aver makes sure the
                   AnnotationList__computeDominantContribution check above
                   does discard annotations in the simplest case of a S/R
                   conflict with no token precedence.  */
                aver (!bitset_test (shift_tokens, conflicted_token)
                      || symbols[conflicted_token]->content->prec);
                ++annotation_counts[s->संख्या];
                अगर (contribution_गिनती > *max_contributionsp)
                  *max_contributionsp = contribution_गिनती;
                AnnotationList__computePredecessorAnnotations (
                  annotation_गाँठ, s,
                  follow_kernel_वस्तुएँ, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      अन्यथा
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_गाँठ);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

व्योम
AnnotationList__debug (AnnotationList स्थिर *self, माप_प्रकार nitems, पूर्णांक spaces)
{
  AnnotationList स्थिर *a;
  Annotation_सूचकांक ai;
  क्रम (a = self, ai = 0; a; a = a->अग्र, ++ai)
    {
      ख_लिखो (मानक_त्रुटि, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_गाँठ->manifestingState->संख्या);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_गाँठ->inadequacy.conflict.actions);
      क्रम (Contribution_सूचकांक ci = 0; ci < a->inadequacy_गाँठ->contribution_गिनती_2; ++ci)
        {
          प्रतीक_संख्या token =
            InadequacyList__getContributionToken (a->inadequacy_गाँठ, ci)
            ->content->संख्या;
          ख_लिखो (मानक_त्रुटि, "%*s", spaces+2, "");
          अगर (ci == Inadequacy_List_get_Shift_Contribution_सूचकांक (
                                                               a->inadequacy_गाँठ))
            ख_लिखो (मानक_त्रुटि, "Contributes shift of token %d.\n", token);
          अन्यथा
            {
              ख_लिखो (मानक_त्रुटि, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              ख_लिखो (मानक_त्रुटि, " as lookahead, rule number %d",
                       a->inadequacy_गाँठ->manifestingState
                       ->reductions->rules[rulei]->संख्या);
              rulei =
                bitset_अग्र (a->inadequacy_गाँठ->inadequacy.conflict.actions,
                             rulei+1);
              अगर (AnnotationList__isContributionAlways (a, ci))
                ख_लिखो (मानक_त्रुटि, " always.");
              अन्यथा
                {
                  ख_लिखो (मानक_त्रुटि, ", items: ");
                  Sbitset__fprint (a->contributions[ci], nitems, मानक_त्रुटि);
                }
              ख_लिखो (मानक_त्रुटि, "\n");
            }
        }
    }
}

व्योम
AnnotationList__computeLookaheadFilter (AnnotationList स्थिर *self,
                                        माप_प्रकार nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  क्रम (; self; self = self->अग्र)
    क्रम (Contribution_सूचकांक ci = 0; ci < self->inadequacy_गाँठ->contribution_गिनती_2; ++ci)
      अगर (!AnnotationList__isContributionAlways (self, ci))
        {
          प्रतीक_संख्या token =
            InadequacyList__getContributionToken (self->inadequacy_गाँठ, ci)
            ->content->संख्या;
          Sbitset_सूचकांक item;
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
अटल bool
AnnotationList__stateMakesContribution (AnnotationList स्थिर *self,
                                        माप_प्रकार nitems, Contribution_सूचकांक ci,
                                        bitset *lookaheads)
{
  अगर (AnnotationList__isContributionAlways (self, ci))
    वापस true;
  अगर (!lookaheads)
    वापस false;
  {
    प्रतीक_संख्या token =
      InadequacyList__getContributionToken (self->inadequacy_गाँठ, ci)
      ->content->संख्या;
    Sbitset_सूचकांक item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      अगर (lookaheads[item] && bitset_test (lookaheads[item], token))
        वापस true;
  }
  वापस false;
}

Contribution_सूचकांक
AnnotationList__computeDominantContribution (AnnotationList स्थिर *self,
                                             माप_प्रकार nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  Contribution_सूचकांक स्थिर ci_shift =
    Inadequacy_List_get_Shift_Contribution_सूचकांक (self->inadequacy_गाँठ);

  प्रतीक *token = self->inadequacy_गाँठ->inadequacy.conflict.token;

  /* S/R conflict.  */
  अगर (ci_shift != Contribution_सूचकांक_none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        पूर्णांक shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        अगर (!shift_precedence)
          वापस ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        Contribution_सूचकांक ci_rr_dominator = Contribution_सूचकांक_none;
        पूर्णांक actioni;
        Contribution_सूचकांक ci;
        क्रम (ci = 0,
               actioni = bitset_first (self->inadequacy_गाँठ->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_गाँठ->contribution_गिनती_2;
             ++ci,
               actioni = bitset_अग्र (self->inadequacy_गाँठ->inadequacy
                                      .conflict.actions, actioni+1))
          {
            पूर्णांक reduce_precedence = 0;
            अगर (ci == ci_shift)
              जारी;
            {
              rule *r = self->inadequacy_गाँठ->manifestingState
                          ->reductions->rules[actioni];
              अगर (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            अगर (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == दायाँ_assoc)))
              जारी;
            अगर (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              जारी;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            अगर (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                अगर (find_stable_domination_over_shift)
                  वापस Contribution_सूचकांक_none;
                अगर (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  वापस Contribution_सूचकांक_error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            अगर (ci_rr_dominator == Contribution_सूचकांक_none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            अगर (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                अगर (find_stable_error_action_domination)
                  वापस Contribution_सूचकांक_none;
                अगर (!require_split_stable)
                  वापस ci_rr_dominator;
                अगर (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  वापस Contribution_सूचकांक_none;
                अगर (AnnotationList__isContributionAlways (self, ci))
                  वापस ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      अगर (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        वापस Contribution_सूचकांक_none;
      /* No reduce or error action domination found, so shift dominates.  */
      वापस ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  क्रम (Contribution_सूचकांक ci = 0; ci < self->inadequacy_गाँठ->contribution_गिनती_2; ++ci)
    अगर (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        अगर (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          वापस Contribution_सूचकांक_none;
        वापस ci;
      }
  वापस Contribution_सूचकांक_none;
}
