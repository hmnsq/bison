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

#समावेश <config.h>

#समावेश "lssi.h"

#समावेश <gl_linked_list.h>
#समावेश <gl_xlist.h>
#समावेश <stdlib.h>

#समावेश "getargs.h"
#समावेश "nullable.h"

// Lookahead sensitive state item.
प्रकार काष्ठा lssi
{
  state_item_संख्या si;
  काष्ठा lssi *parent;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

अटल lssi *
new_lssi (state_item_संख्या si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (माप *res);
  res->si = si;
  res->parent = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  वापस res;
}

अटल व्योम
lssi_free (lssi *sn)
{
  अगर (sn == शून्य)
    वापस;
  अगर (sn->free_lookahead)
    bitset_free (sn->lookahead);
  मुक्त (sn);
}

अटल माप_प्रकार
lssi_hasher (lssi *sn, माप_प्रकार max)
{
  माप_प्रकार hash = sn->si;
  bitset_iterator biter;
  प्रतीक_संख्या syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  वापस hash % max;
}

अटल bool
lssi_comparator (lssi *s1, lssi *s2)
{
  अगर (s1->si == s2->si)
    {
      अगर (s1->lookahead == s2->lookahead)
        वापस true;
      वापस bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  वापस false;
}

प्रकार gl_list_t lssi_list;

अटल अंतरभूत bool
append_lssi (lssi *sn, Hash_table *visited, lssi_list queue)
{
  अगर (hash_lookup (visited, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      वापस false;
    }
  hash_xinsert (visited, sn);
  gl_list_add_last (queue, sn);
  वापस true;
}

#अगर 0
अटल व्योम
lssi_print (lssi *l)
{
  खाता *out = मानक_त्रुटि;
  print_state_item (&state_वस्तुएँ[l->si], out);
  अगर (l->lookahead)
    {
      ख_लिखो (out, "FOLLOWL = { ");
      bitset_iterator biter;
      प्रतीक_संख्या sin;
      BITSET_FOR_EACH (biter, l->lookahead, sin, 0)
        ख_लिखो (out, "%s, \n", symbols[sin]->tag);
      ख_लिखो (out, "}\n");
    }
}
#पूर्ण_अगर

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
अटल bitset
eligible_state_वस्तुएँ (state_item *target)
{
  bitset परिणाम = bitset_create (nstate_वस्तुएँ, BITSET_FIXED);
  state_item_list queue =
    gl_list_create (GL_LINKED_LIST, शून्य, शून्य, शून्य, true, 1,
                    (स्थिर व्योम **) &target);
  जबतक (gl_list_आकार (queue) > 0)
    {
      state_item *si = (state_item *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      अगर (bitset_test (परिणाम, si - state_वस्तुएँ))
        जारी;
      bitset_set (परिणाम, si - state_वस्तुएँ);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, rsi, sin, 0)
        gl_list_add_last (queue, &state_वस्तुएँ[sin]);
    }
  gl_list_free (queue);
  वापस परिणाम;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_item_list
shortest_path_from_start (state_item_संख्या target, प्रतीक_संख्या अग्र_sym)
{
  bitset eligible = eligible_state_वस्तुएँ (&state_वस्तुएँ[target]);
  Hash_table *visited = hash_initialize (32,
                                         शून्य,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_तथ्य_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_set (il, 0);
  lssi *init = new_lssi (0, शून्य, il, true);
  lssi_list queue = gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य,
                                          शून्य, true);
  append_lssi (init, visited, queue);
  // breadth-first search
  bool finished = false;
  lssi *n;
  जबतक (gl_list_आकार (queue) > 0)
    {
      n = (lssi *) gl_list_get_at (queue, 0);
      gl_list_remove_at (queue, 0);
      state_item_संख्या last = n->si;
      अगर (target == last && bitset_test (n->lookahead, अग्र_sym))
        {
          finished = true;
          अवरोध;
        }
      state_item *si = &state_वस्तुएँ[last];
      // Transitions don't change follow_L
      अगर (si->trans >= 0)
        {
          अगर (bitset_test (eligible, si->trans))
            {
              lssi *अग्र = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (अग्र, visited, queue);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // nonterminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      अगर (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          item_संख्या *pos = si->item + 1;
          क्रम (; !item_संख्या_is_rule_संख्या (*pos); ++pos)
            {
              item_संख्या it = *pos;
              अगर (ISTOKEN (it))
                {
                  bitset_set (lookahead, it);
                  अवरोध;
                }
              अन्यथा
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  अगर (!nullable[it - ntokens])
                    अवरोध;
                }
            }
          अगर (item_संख्या_is_rule_संख्या (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_used = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_item_संख्या अग्र_SI;
          BITSET_FOR_EACH (biter, si->prods, अग्र_SI, 0)
            {
              अगर (!bitset_test (eligible, अग्र_SI))
                जारी;
              lssi *अग्र = new_lssi (अग्र_SI, n, lookahead,
                                     !lookahead_used);
              lookahead_used = append_lssi (अग्र, visited, queue)
                               || lookahead_used;
            }
          अगर (!lookahead_used)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  अगर (!finished)
    {
      gl_list_free (queue);
      ख_माला_दो ("Cannot find shortest path to conflict state.", मानक_त्रुटि);
      पात ();
    }
  state_item_list res =
    gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
  क्रम (lssi *sn = n; sn != शून्य; sn = sn->parent)
    gl_list_add_first (res, &state_वस्तुएँ[sn->si]);

  hash_free (visited);
  gl_list_free (queue);

  अगर (trace_flag & trace_cex)
    {
      ख_माला_दो ("REDUCE ITEM PATH:\n", मानक_त्रुटि);
      gl_list_iterator_t it = gl_list_iterator (res);
      स्थिर व्योम *sip;
      जबतक (gl_list_iterator_अग्र (&it, &sip, शून्य))
        state_item_print ((state_item *) sip, मानक_त्रुटि, "");
    }
  वापस res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_प्रतीक (प्रतीक_संख्या sym, bitset syms)
{
  अगर (!syms)
    वापस true;
  bitset_iterator biter;
  प्रतीक_संख्या sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      अगर (sym == sn)
        वापस true;
      अगर (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        वापस true;
    }
  वापस false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  अगर (!syms || !ts)
    वापस true;
  bitset_iterator biter;
  प्रतीक_संख्या sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      अगर (bitset_test (ts, sn))
        वापस true;
      अगर (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        वापस true;
    }
  वापस false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_item_list
lssi_reverse_production (स्थिर state_item *si, bitset lookahead)
{
  state_item_list परिणाम =
    gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
  अगर (SI_TRANSITION (si))
    वापस परिणाम;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_item_संख्या sin;
  BITSET_FOR_EACH (biter, si->revs, sin, 0)
  {
    state_item *prevsi = &state_वस्तुएँ[sin];
    अगर (!production_allowed (prevsi, si))
      जारी;
    bitset पूर्व_lookahead = prevsi->lookahead;
    अगर (item_संख्या_is_rule_संख्या (*(prevsi->item)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        अगर (!intersect (पूर्व_lookahead, lookahead))
          जारी;
      }
    अन्यथा
      {
        // shift item
        अगर (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            क्रम (item_संख्या *pos = prevsi->item + 1;
                 !applicable && nlable && item_संख्या_is_प्रतीक_संख्या (*pos);
                 ++pos)
              {
                प्रतीक_संख्या अग्र_sym = item_संख्या_as_प्रतीक_संख्या (*pos);
                अगर (ISTOKEN (अग्र_sym))
                  {
                    applicable = intersect_प्रतीक (अग्र_sym, lookahead);
                    nlable = false;
                  }
                अन्यथा
                  {
                    applicable = intersect (FIRSTS (अग्र_sym), lookahead);
                    अगर (!applicable)
                      nlable = nullable[अग्र_sym - ntokens];
                  }
              }
            अगर (!applicable && !nlable)
              जारी;
          }
      }
    gl_list_add_last (परिणाम, prevsi);
  }
  वापस परिणाम;
}
