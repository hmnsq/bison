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
AnnotationList__alloc_on_obstack (ContributionIndex contribution_गन्ती,
                                  संरचना obstack *annotations_obstackp)
{
  AnnotationList *res;
  माप_प्रकार contributions_size = contribution_गन्ती * परिमाण res->contributions[0];
  res = obstack_alloc (annotations_obstackp,
                       दुरत्व (AnnotationList, contributions)
                       + contributions_size);
  res->next = रिक्त;
  res->inadequacy_गाँठो = रिक्त;
  फर्काऊ res;
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
AnnotationList__isContributionAlways (AnnotationList अचर *self,
                                      ContributionIndex ci)
{
  aver (0 <= ci && ci < self->inadequacy_गाँठो->contribution_गन्ती_2);
  फर्काऊ self->contributions[ci] == रिक्त;
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
Annotation_List_घुसाउ_Into (AnnotationList *self, AnnotationList **list,
                            माप_प्रकार nitems)
{
  AnnotationList **गाँठो;
  लागि (गाँठो = list; *गाँठो; गाँठो = &(*गाँठो)->next)
    {
      पूर्णाङ्क cmp = 0;
      यदि (self->inadequacy_गाँठो->id < (*गाँठो)->inadequacy_गाँठो->id)
        cmp = 1;
      अन्यथा यदि ((*गाँठो)->inadequacy_गाँठो->id < self->inadequacy_गाँठो->id)
        cmp = -1;
      अन्यथा
        लागि (ContributionIndex ci = 0;
             cmp == 0 && ci < self->inadequacy_गाँठो->contribution_गन्ती_2;
             ++ci)
          {
            यदि (AnnotationList__isContributionAlways (self, ci))
              {
                यदि (!AnnotationList__isContributionAlways (*गाँठो, ci))
                  cmp = -1;
              }
            अन्यथा यदि (AnnotationList__isContributionAlways (*गाँठो, ci))
              cmp = 1;
            अन्यथा
              लागि (माप_प्रकार item = 0; cmp == 0 && item < nitems; ++item)
                {
                  यदि (!Sbitset__test (self->contributions[ci], item))
                    {
                      यदि (Sbitset__test ((*गाँठो)->contributions[ci], item))
                        cmp = -1;
                    }
                  अन्यथा यदि (!Sbitset__test ((*गाँठो)->contributions[ci], item))
                    cmp = 1;
                }
          }
      यदि (cmp < 0)
        {
          self->next = *गाँठो;
          *गाँठो = self;
          रोक;
        }
      अन्यथा यदि (cmp == 0)
        {
          self = रिक्त;
          रोक;
        }
    }
  यदि (!*गाँठो)
    *गाँठो = self;
  फर्काऊ self != रिक्त;
}

स्थिर bitset
AnnotationList__compute_shift_tokens (transitions *trans)
{
  bitset shift_tokens = bitset_create (ntokens, BITSET_FIXED);
  पूर्णाङ्क i;
  FOR_EACH_SHIFT (trans, i)
    bitset_set (shift_tokens, TRANSITION_चिह्न (trans, i));
  फर्काऊ shift_tokens;
}

स्थिर bitset
AnnotationList__compute_conflicted_tokens (bitset shift_tokens,
                                           reductions *reds)
{
  bitset conflicted_tokens = bitset_create (ntokens, BITSET_FIXED);
  bitset conflicted_tokens_rule = bitset_create (ntokens, BITSET_FIXED);
  bitset tokens = bitset_create (ntokens, BITSET_FIXED);

  bitset_copy (tokens, shift_tokens);
  लागि (पूर्णाङ्क i = 0; i < reds->अङ्क; ++i)
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

  फर्काऊ conflicted_tokens;
}

स्थिर bool
AnnotationList__compute_lhs_contributions (state *s, अचर rule *the_rule,
                                           चिह्न_सङ्ख्या conflicted_token,
                                           bitsetv follow_kernel_वस्तुहरू,
                                           bitsetv always_follows,
                                           state ***predecessors,
                                           bitset **item_lookahead_sets,
                                           Sbitset *वस्तुहरू,
                                           संरचना obstack
                                             *annotations_obstackp)
{
  goto_सङ्ख्या lhs_goto = map_goto (s->सङ्ख्या, the_rule->lhs->सङ्ख्या);
  यदि (bitset_test (always_follows[lhs_goto], conflicted_token))
    फर्काऊ true;
  *वस्तुहरू = Sbitset__new_on_obstack (s->nitems, annotations_obstackp);
  {
    bitset_iterator biter_item;
    bitset_bindex item;
    BITSET_FOR_EACH (biter_item, follow_kernel_वस्तुहरू[lhs_goto], item, 0)
      यदि (ielr_item_has_lookahead (s, 0, item, conflicted_token,
                                   predecessors, item_lookahead_sets))
        Sbitset__set (*वस्तुहरू, item);
  }
  फर्काऊ false;
}

स्थिर शून्य
AnnotationList__computePredecessorAnnotations (
  AnnotationList *self, state *s,
  bitsetv follow_kernel_वस्तुहरू,
  bitsetv always_follows,
  state ***predecessors,
  bitset **item_lookahead_sets,
  AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  संरचना obstack *annotations_obstackp)
{
  लागि (state **predecessor = predecessors[s->सङ्ख्या]; *predecessor; ++predecessor)
    {
      AnnotationList *annotation_गाँठो =
        AnnotationList__alloc_on_obstack (
          self->inadequacy_गाँठो->contribution_गन्ती_2, annotations_obstackp);
      annotation_गाँठो->inadequacy_गाँठो = self->inadequacy_गाँठो;
      bool potential_contribution = false;
      bitset *lookaheads = रिक्त;

      लागि (ContributionIndex ci = 0; ci < self->inadequacy_गाँठो->contribution_गन्ती_2; ++ci)
        {
          चिह्न_सङ्ख्या contribution_token =
            InadequacyList__getContributionToken (self->inadequacy_गाँठो, ci)
            ->content->सङ्ख्या;
          यदि (AnnotationList__isContributionAlways (self, ci))
            {
              annotation_गाँठो->contributions[ci] = रिक्त;
              जारी;
            }
          annotation_गाँठो->contributions[ci] =
            Sbitset__new_on_obstack ((*predecessor)->nitems,
                                     annotations_obstackp);
          {
            माप_प्रकार predecessor_item = 0;
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
                aver (s->वस्तुहरू[self_item] > 1);
                /* If this kernel item is next to the beginning of the RHS,
                   then check all of the predecessor's goto follows for the
                   LHS.  */
                यदि (item_सङ्ख्या_is_rule_सङ्ख्या (ritem[s->वस्तुहरू[self_item] - 2]))
                  {
                    Sbitset वस्तुहरू;
                    यदि (AnnotationList__compute_lhs_contributions (
                          *predecessor,
                          item_rule (&ritem[s->वस्तुहरू[self_item]]),
                          contribution_token,
                          follow_kernel_वस्तुहरू, always_follows, predecessors,
                          item_lookahead_sets, &वस्तुहरू, annotations_obstackp))
                      {
                        obstack_free (annotations_obstackp,
                                      annotation_गाँठो->contributions[ci]);
                        annotation_गाँठो->contributions[ci] = रिक्त;
                        // "Break" out of SBITSET__FOR_EACH.
                        गच्छ after_sbitset__for_each;
                      }
                    अन्यथा
                      {
                        Sbitset__or (annotation_गाँठो->contributions[ci],
                                     annotation_गाँठो->contributions[ci],
                                     वस्तुहरू, (*predecessor)->nitems);
                        obstack_free (annotations_obstackp, वस्तुहरू);
                      }
                  }
                /* If this kernel item is later in the RHS, then check the
                   predecessor item's lookahead set.  */
                अन्यथा
                  {
                    /* We don't have to start the predecessor item search at
                       the beginning every time because items from both
                       states are sorted by their indices in ritem.  */
                    लागि (;
                         predecessor_item < (*predecessor)->nitems;
                         ++predecessor_item)
                      यदि ((*predecessor)->वस्तुहरू[predecessor_item]
                          == s->वस्तुहरू[self_item] - 1)
                        रोक;
                    aver (predecessor_item != (*predecessor)->nitems);
                    यदि (ielr_item_has_lookahead (*predecessor, 0,
                                                 predecessor_item,
                                                 contribution_token,
                                                 predecessors,
                                                 item_lookahead_sets))
                      Sbitset__set (annotation_गाँठो->contributions[ci],
                                    predecessor_item);
                  }
              }
          after_sbitset__for_each:;
          }
          यदि (annotation_गाँठो->contributions[ci])
            {
              Sbitset biter;
              Sbitset__Index i;
              SBITSET__FOR_EACH (annotation_गाँठो->contributions[ci],
                                 (*predecessor)->nitems, biter, i)
                {
                  potential_contribution = true;
                  यदि (!lookaheads)
                    {
                      lookaheads = xnmalloc ((*predecessor)->nitems,
                                             परिमाण *lookaheads);
                      लागि (माप_प्रकार j = 0; j < (*predecessor)->nitems; ++j)
                        lookaheads[j] = रिक्त;
                    }
                  यदि (!lookaheads[i])
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
      यदि (potential_contribution)
        {
          यदि (ContributionIndex__none
              != AnnotationList__computeDominantContribution (
                   annotation_गाँठो, (*predecessor)->nitems, lookaheads, true))
            {
              obstack_free (annotations_obstackp, annotation_गाँठो);
              annotation_गाँठो = रिक्त;
            }
          {
            लागि (माप_प्रकार i = 0; i < (*predecessor)->nitems; ++i)
              यदि (lookaheads[i])
                bitset_free (lookaheads[i]);
            मुक्त (lookaheads);
          }
          यदि (annotation_गाँठो)
            {
              यदि (Annotation_List_घुसाउ_Into (annotation_गाँठो,
                                              &annotation_lists[(*predecessor)
                                                                ->सङ्ख्या],
                                              (*predecessor)->nitems))
                {
                  ++annotation_counts[(*predecessor)->सङ्ख्या];
                  AnnotationList__computePredecessorAnnotations (
                    annotation_गाँठो, *predecessor,
                    follow_kernel_वस्तुहरू, always_follows, predecessors,
                    item_lookahead_sets, annotation_lists, annotation_counts,
                    annotations_obstackp);
                }
              अन्यथा
                obstack_free (annotations_obstackp, annotation_गाँठो);
            }
        }
      अन्यथा
        obstack_free (annotations_obstackp, annotation_गाँठो);
    }
}

शून्य
AnnotationList__compute_from_inadequacies (
  state *s, bitsetv follow_kernel_वस्तुहरू, bitsetv always_follows,
  state ***predecessors, bitset **item_lookahead_sets,
  InadequacyList **inadequacy_lists, AnnotationList **annotation_lists,
  AnnotationIndex *annotation_counts,
  ContributionIndex *max_contributionsp,
  संरचना obstack *annotations_obstackp,
  Inadequacy_List_गाँठो_गन्ती *inadequacy_list_गाँठो_गन्ती)
{
  /* Return an empty list if s->lookaheads = NULL.  */
  यदि (s->consistent)
    फर्काऊ;

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
      AnnotationList *annotation_गाँठो;
      ContributionIndex contribution_गन्ती = 0;

      /* Allocate the annotation node.  */
      {
        लागि (पूर्णाङ्क rule_i = 0; rule_i < s->reductions->अङ्क; ++rule_i)
          यदि (bitset_test (s->reductions->lookaheads[rule_i],
                           conflicted_token))
            ++contribution_गन्ती;
        यदि (bitset_test (shift_tokens, conflicted_token))
          ++contribution_गन्ती;
        annotation_गाँठो =
          AnnotationList__alloc_on_obstack (contribution_गन्ती,
                                            annotations_obstackp);
      }

      /* FIXME: Would a BITSET_FRUGAL or BITEST_SPARSE be more efficient?  Now
         or convert it inside InadequacyList__new_conflict?  */
      bitset actions = bitset_create (s->reductions->अङ्क + 1, BITSET_FIXED);
      bool potential_contribution = false;

      /* Add a contribution for each reduction that has conflicted_token as a
         lookahead.  */
      {
        ContributionIndex ci = 0;
        पूर्णाङ्क item_i = 0;
        लागि (पूर्णाङ्क rule_i = 0; rule_i < s->reductions->अङ्क; ++rule_i)
          {
            rule *the_rule = s->reductions->rules[rule_i];
            यदि (bitset_test (s->reductions->lookaheads[rule_i],
                             conflicted_token))
              {
                bitset_set (actions, rule_i);
                /* If this reduction is on a kernel item, just add it.  */
                यदि (!item_सङ्ख्या_is_rule_सङ्ख्या (the_rule->rhs[0]))
                  {
                    annotation_गाँठो->contributions[ci] =
                      Sbitset__new_on_obstack (s->nitems,
                                               annotations_obstackp);
                    /* Catch item_i up to rule_i.  This works because both are
                       sorted on rule number.  */
                    जबसम्म (!item_सङ्ख्या_is_rule_सङ्ख्या (ritem[s->वस्तुहरू[item_i]])
                           || item_सङ्ख्या_as_rule_सङ्ख्या (ritem[s->वस्तुहरू[item_i]]) != the_rule->सङ्ख्या)
                      {
                        ++item_i;
                        aver (item_i < s->nitems);
                      }
                    Sbitset__set (annotation_गाँठो->contributions[ci], item_i);
                  }
                /* Otherwise, add the kernel items whose lookahead sets
                   contribute the conflicted token to this reduction's
                   lookahead set.  */
                अन्यथा यदि (AnnotationList__compute_lhs_contributions (
                           s, the_rule, conflicted_token, follow_kernel_वस्तुहरू,
                           always_follows, predecessors, item_lookahead_sets,
                           &annotation_गाँठो->contributions[ci],
                           annotations_obstackp))
                  {
                    annotation_गाँठो->contributions[ci++] = रिक्त;
                    जारी;
                  }
                /* The lookahead token has to come from somewhere.  */
                aver (!Sbitset__isEmpty (annotation_गाँठो->contributions[ci],
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
      यदि (potential_contribution)
        {
          यदि (bitset_test (shift_tokens, conflicted_token))
            {
              bitset_set (actions, s->reductions->अङ्क);
              annotation_गाँठो->contributions[contribution_गन्ती - 1] = रिक्त;
            }
          {
            InadequacyList *conflict_गाँठो =
              InadequacyList__new_conflict (
                s, symbols[conflicted_token], actions,
                inadequacy_list_गाँठो_गन्ती);
            actions = रिक्त;
            annotation_गाँठो->inadequacy_गाँठो = conflict_गाँठो;
            यदि (ContributionIndex__none
                != AnnotationList__computeDominantContribution (
                     annotation_गाँठो, s->nitems, all_lookaheads, true))
              {
                obstack_free (annotations_obstackp, annotation_गाँठो);
                InadequacyList__delete (conflict_गाँठो);
              }
            अन्यथा
              {
                InadequacyList__prependTo (conflict_गाँठो,
                                           &inadequacy_lists[s->सङ्ख्या]);
                {
                  bool b =
                    Annotation_List_घुसाउ_Into (annotation_गाँठो,
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
                यदि (contribution_गन्ती > *max_contributionsp)
                  *max_contributionsp = contribution_गन्ती;
                AnnotationList__computePredecessorAnnotations (
                  annotation_गाँठो, s,
                  follow_kernel_वस्तुहरू, always_follows, predecessors,
                  item_lookahead_sets, annotation_lists, annotation_counts,
                  annotations_obstackp);
              }
          }
        }
      अन्यथा
        {
          bitset_free (actions);
          obstack_free (annotations_obstackp, annotation_गाँठो);
        }
    }

  bitsetv_free (all_lookaheads);
  bitset_free (shift_tokens);
  bitset_free (conflicted_tokens);
}

शून्य
AnnotationList__debug (AnnotationList अचर *self, माप_प्रकार nitems, पूर्णाङ्क spaces)
{
  AnnotationList अचर *a;
  AnnotationIndex ai;
  लागि (a = self, ai = 0; a; a = a->next, ++ai)
    {
      ख_लिखो (मानक_त्रुटि, "%*sAnnotation %d (manifesting state %d):\n",
               spaces, "",
               ai, a->inadequacy_गाँठो->manifestingState->सङ्ख्या);
      bitset_bindex rulei
        = bitset_first (a->inadequacy_गाँठो->inadequacy.conflict.actions);
      लागि (ContributionIndex ci = 0; ci < a->inadequacy_गाँठो->contribution_गन्ती_2; ++ci)
        {
          चिह्न_सङ्ख्या token =
            InadequacyList__getContributionToken (a->inadequacy_गाँठो, ci)
            ->content->सङ्ख्या;
          ख_लिखो (मानक_त्रुटि, "%*s", spaces+2, "");
          यदि (ci == InadequacyList__getShiftContributionIndex (
                                                               a->inadequacy_गाँठो))
            ख_लिखो (मानक_त्रुटि, "Contributes shift of token %d.\n", token);
          अन्यथा
            {
              ख_लिखो (मानक_त्रुटि, "Contributes token %d", token);
              aver (rulei != BITSET_BINDEX_MAX);
              ख_लिखो (मानक_त्रुटि, " as lookahead, rule number %d",
                       a->inadequacy_गाँठो->manifestingState
                       ->reductions->rules[rulei]->सङ्ख्या);
              rulei =
                bitset_next (a->inadequacy_गाँठो->inadequacy.conflict.actions,
                             rulei+1);
              यदि (AnnotationList__isContributionAlways (a, ci))
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

शून्य
AnnotationList__computeLookaheadFilter (AnnotationList अचर *self,
                                        माप_प्रकार nitems,
                                        bitsetv lookahead_filter)
{
  bitsetv_zero (lookahead_filter);
  लागि (; self; self = self->next)
    लागि (ContributionIndex ci = 0; ci < self->inadequacy_गाँठो->contribution_गन्ती_2; ++ci)
      यदि (!AnnotationList__isContributionAlways (self, ci))
        {
          चिह्न_सङ्ख्या token =
            InadequacyList__getContributionToken (self->inadequacy_गाँठो, ci)
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
AnnotationList__stateMakesContribution (AnnotationList अचर *self,
                                        माप_प्रकार nitems, ContributionIndex ci,
                                        bitset *lookaheads)
{
  यदि (AnnotationList__isContributionAlways (self, ci))
    फर्काऊ true;
  यदि (!lookaheads)
    फर्काऊ false;
  {
    चिह्न_सङ्ख्या token =
      InadequacyList__getContributionToken (self->inadequacy_गाँठो, ci)
      ->content->सङ्ख्या;
    Sbitset__Index item;
    Sbitset biter;
    SBITSET__FOR_EACH (self->contributions[ci], nitems, biter, item)
      यदि (lookaheads[item] && bitset_test (lookaheads[item], token))
        फर्काऊ true;
  }
  फर्काऊ false;
}

ContributionIndex
AnnotationList__computeDominantContribution (AnnotationList अचर *self,
                                             माप_प्रकार nitems, bitset *lookaheads,
                                             bool require_split_stable)
{
  ContributionIndex अचर ci_shift =
    InadequacyList__getShiftContributionIndex (self->inadequacy_गाँठो);

  चिह्न *token = self->inadequacy_गाँठो->inadequacy.conflict.token;

  /* S/R conflict.  */
  यदि (ci_shift != ContributionIndex__none)
    {
      bool find_stable_domination_over_shift = false;
      bool find_stable_error_action_domination = false;
      {
        पूर्णाङ्क shift_precedence = token->content->prec;

        /* If the token has no precedence set, shift is always chosen.  */
        यदि (!shift_precedence)
          फर्काऊ ci_shift;

        /* Figure out which reductions contribute, which of those would
           dominate in a R/R comparison, and whether any reduction dominates
           the shift so that the R/R comparison is actually needed.  */
        ContributionIndex ci_rr_dominator = ContributionIndex__none;
        पूर्णाङ्क actioni;
        ContributionIndex ci;
        लागि (ci = 0,
               actioni = bitset_first (self->inadequacy_गाँठो->inadequacy
                                       .conflict.actions);
             ci < self->inadequacy_गाँठो->contribution_गन्ती_2;
             ++ci,
               actioni = bitset_next (self->inadequacy_गाँठो->inadequacy
                                      .conflict.actions, actioni+1))
          {
            पूर्णाङ्क reduce_precedence = 0;
            यदि (ci == ci_shift)
              जारी;
            {
              rule *r = self->inadequacy_गाँठो->manifestingState
                          ->reductions->rules[actioni];
              यदि (r->prec)
                reduce_precedence = r->prec->prec;
            }
            /* If there's no need to check whether this reduction actually
               contributes because the shift eliminates it from the R/R
               comparison anyway, continue to the next reduction.  */
            यदि (reduce_precedence
                && (reduce_precedence < shift_precedence
                    || (reduce_precedence == shift_precedence
                        && token->content->assoc == दायाँ_assoc)))
              जारी;
            यदि (!AnnotationList__stateMakesContribution (self, nitems, ci,
                                                         lookaheads))
              जारी;
            /* This uneliminated reduction contributes, so see if it can cause
               an error action.  */
            यदि (reduce_precedence == shift_precedence
                 && token->content->assoc == non_assoc)
              {
                /* It's not possible to find split-stable domination over
                   shift after a potential %nonassoc.  */
                यदि (find_stable_domination_over_shift)
                  फर्काऊ ContributionIndex__none;
                यदि (!require_split_stable
                    || AnnotationList__isContributionAlways (self, ci))
                  फर्काऊ ContributionIndex__error_action;
                find_stable_error_action_domination = true;
              }
            /* Consider this uneliminated contributing reduction in the R/R
               comparison.  */
            यदि (ci_rr_dominator == ContributionIndex__none)
              ci_rr_dominator = ci;
            /* If precedence is set for this uneliminated contributing
               reduction, it dominates the shift, so try to figure out which
               reduction dominates the R/R comparison.  */
            यदि (reduce_precedence)
              {
                /* It's not possible to find split-stable error action
                   domination after a potential reduction.  */
                यदि (find_stable_error_action_domination)
                  फर्काऊ ContributionIndex__none;
                यदि (!require_split_stable)
                  फर्काऊ ci_rr_dominator;
                यदि (!AnnotationList__isContributionAlways (self,
                                                           ci_rr_dominator))
                  फर्काऊ ContributionIndex__none;
                यदि (AnnotationList__isContributionAlways (self, ci))
                  फर्काऊ ci_rr_dominator;
                find_stable_domination_over_shift = true;
              }
          }
      }
      यदि (find_stable_domination_over_shift
          || find_stable_error_action_domination)
        फर्काऊ ContributionIndex__none;
      /* No reduce or error action domination found, so shift dominates.  */
      फर्काऊ ci_shift;
    }

  /* R/R conflict, so the reduction with the lowest rule number dominates.
     Fortunately, contributions are sorted by rule number.  */
  लागि (ContributionIndex ci = 0; ci < self->inadequacy_गाँठो->contribution_गन्ती_2; ++ci)
    यदि (AnnotationList__stateMakesContribution (self, nitems, ci, lookaheads))
      {
        यदि (require_split_stable
            && !AnnotationList__isContributionAlways (self, ci))
          फर्काऊ ContributionIndex__none;
        फर्काऊ ci;
      }
  फर्काऊ ContributionIndex__none;
}
