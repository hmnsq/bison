/* Lookahead sensitive state item searches for counterexample generation

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#अन्तर्भाव "lssi.h"

#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "getargs.h"
#अन्तर्भाव "nullable.h"

// Lookahead sensitive state item.
प्रकारव्याख्या संरचना lssi
{
  state_item_संख्या si;
  संरचना lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

स्थिर lssi *
new_lssi (state_item_संख्या si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (आकार *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  परत res;
}

स्थिर शून्य
lssi_free (lssi *sn)
{
  जर (sn == रिक्त)
    परत;
  जर (sn->free_lookahead)
    bitset_free (sn->lookahead);
  मुक्त (sn);
}

स्थिर माप_प्रकार
lssi_hasher (lssi *sn, माप_प्रकार max)
{
  माप_प्रकार hash = sn->si;
  bitset_iterator biter;
  चिन्ह_संख्या syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  परत hash % max;
}

स्थिर bool
lssi_comparator (lssi *s1, lssi *s2)
{
  जर (s1->si == s2->si)
    {
      जर (s1->lookahead == s2->lookahead)
        परत true;
      परत bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  परत false;
}

प्रकारव्याख्या gl_list_t lssi_list;

स्थिर ओळीत bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  जर (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      परत false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  परत true;
}

#जर 0
स्थिर शून्य
lssi_print (lssi *l)
{
  खाता *out = मानक_त्रुटि;
  print_state_item (&state_वस्तुसंच[l->si], out);
  जर (l->lookahead)
    {
      ख_लिखो (out, "FOLLOWL = { ");
      bitset_iterator biter;
      चिन्ह_संख्या sin;
      BITSET_FOR_EACH (biter, l->lookahead, sin, 0)
        ख_लिखो (out, "%s, \n", symbols[sin]->tag);
      ख_लिखो (out, "}\n");
    }
}
#endif

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
स्थिर bitset
eligible_state_वस्तुसंच (state_item *target)
{
  bitset result = bitset_create (nstate_वस्तुसंच, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true, 1,
                    (स्थिरांक शून्य **) &target);
  पर्यंत (gl_list_size (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      जर (bitset_test (result, si - state_वस्तुसंच))
        सुरूठेवा;
      bitset_set (result, si - state_वस्तुसंच);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_वस्तुसंच[sin]);
    }
  gl_list_free (queue);
  परत result;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_संख्या target, चिन्ह_संख्या next_sym)
{
  bitset eligible = eligible_state_वस्तुसंच (&state_वस्तुसंच[target]);
  Hash_table *visited = hash_initialize (32,
                                         रिक्त,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_data_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, रिक्त, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त,
                                          रिक्त, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  पर्यंत (gl_list_size (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_संख्या last = n->si;
      जर (target == last && bitset_test (n->lookahead, next_sym))
        {
          finished = true;
          थांबा;
        }
      state_item *si = &state_वस्तुसंच[last];
      // Transitions don't change follow_L
      जर (si->trans >= 0)
        {
          जर (bitset_test (eligible, si->trans))
            {
              lssi *next = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (next, visited, queue);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // nonterminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      जर (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_संख्या *pos = si->item + 1;
          साठी (; !item_संख्या_is_rule_संख्या (*pos); ++pos)
            {
              item_संख्या it = *pos;
              जर (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  थांबा;
                }
              अन्यथा
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  जर (!nullable[it - ntokens])
                    थांबा;
                }
            }
          जर (item_संख्या_is_rule_संख्या (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_संख्या nextSI;
          BITSET_FOR_EACH (biter, si->prods, nextSI, 0)
            {
              जर (!bitset_test (eligible, nextSI))
                सुरूठेवा;
              lssi *next = new_lssi (nextSI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (next, visited, queue)
                               || lookahead_used;
            }
          जर (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  जर (!finished)
    {
      gl_list_free (queue);
      ख_माला_दो ("Cannot find shortest path to conflict state.", मानक_त्रुटि);
      पात ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  साठी (lssi *sn = n; sn != रिक्त; sn = sn->parent)
    gl_list_add_first (res, &state_वस्तुसंच[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  जर (trace_flag & trace_cex)
    {
      ख_माला_दो ("REDUCE ITEM PATH:\n", मानक_त्रुटि);
      gl_list_iterator_t it = gl_list_iterator (res);
      स्थिरांक शून्य *sip;
      पर्यंत (gl_list_iterator_next (&it, &sip, रिक्त))
        state_item_print ((state_item *) sip, मानक_त्रुटि, "");
    }
  परत res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_चिन्ह (चिन्ह_संख्या sym, bitset syms)
{
  जर (!syms)
    परत true;
  bitset_iterator biter;
  चिन्ह_संख्या sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      जर (sym == sn)
        परत true;
      जर (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        परत true;
    }
  परत false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  जर (!syms || !ts)
    परत true;
  bitset_iterator biter;
  चिन्ह_संख्या sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      जर (bitset_test (ts, sn))
        परत true;
      जर (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        परत true;
    }
  परत false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (स्थिरांक state_item *si, bitset lookahead)
{
  state_item_list result =
    gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  जर (SI_TRANSITION (si))
    परत result;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_संख्या sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_वस्तुसंच[sin];
    जर (!production_allowed (prevsi, si))
      सुरूठेवा;
    bitset prev_lookahead = prevsi->lookahead;
    जर (item_संख्या_is_rule_संख्या (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        जर (!intersect (prev_lookahead, lookahead))
          सुरूठेवा;
      }
    अन्यथा
      {
        // shift item
        जर (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            साठी (item_संख्या *pos = prevsi->item + 1;
                 !applicable && nlable && item_संख्या_is_चिन्ह_संख्या (*pos);
                 ++pos)
              {
                चिन्ह_संख्या next_sym = item_संख्या_as_चिन्ह_संख्या (*pos);
                जर (ISTOKEN (next_sym))
                  {
                    applicable = intersect_चिन्ह (next_sym, lookahead);
                    nlable = false;
                  }
                अन्यथा
                  {
                    applicable = intersect (FIRSTS (next_sym), lookahead);
                    जर (!applicable)
                      nlable = nullable[next_sym - ntokens];
                  }
              }
            जर (!applicable && !nlable)
              सुरूठेवा;
          }
      }
    gl_list_add_last (result, prevsi);
  }
  परत result;
}
