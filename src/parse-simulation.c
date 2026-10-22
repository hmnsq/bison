/* Parser simulator for unifying counterexample search

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

#अन्तर्भाव "parse-simulation.h"

#अन्तर्भाव <gl_linked_list.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "lssi.h"
#अन्तर्भाव "nullable.h"

संरचना parse_state
{
  // Path of state-items the parser has traversed.
  संरचना si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ध्रुव state_item *head_elt;
    ध्रुव state_item *tail_elt;
    माप_प्रकार total_size;
  } state_वस्तुएँ;
  // List of derivations of the symbols.
  संरचना deriv_chunk
  {
    derivation_list contents;
    ध्रुव derivation *head_elt;
    ध्रुव derivation *tail_elt;
    माप_प्रकार total_size;
  } derivs;
  संरचना parse_state *parent;
  पूर्णाङ्क reference_गिनती;
  // Incremented during productions, decremented during reductions.
  पूर्णाङ्क depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


स्थिर शून्य
ps_si_prepend (parse_state *ps, ध्रुव state_item *si)
{
  संरचना si_chunk *sic = &ps->state_वस्तुएँ;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  यदि (!sic->tail_elt)
    sic->tail_elt = si;
}

स्थिर शून्य
ps_si_append (parse_state *ps, ध्रुव state_item *si)
{
  संरचना si_chunk *sic = &ps->state_वस्तुएँ;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  यदि (!sic->head_elt)
    sic->head_elt = si;
}

स्थिर शून्य
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  संरचना deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  यदि (!dc->tail_elt)
    dc->tail_elt = d;
}

स्थिर शून्य
ps_derivs_append (parse_state *ps, derivation *d)
{
  संरचना deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  यदि (!dc->head_elt)
    dc->head_elt = d;
}

स्थिर पूर्णाङ्क allocs = 0;
स्थिर पूर्णाङ्क frees = 0;

स्थिर parse_state *
empty_parse_state (शून्य)
{
  parse_state *res = xcalloc (1, परिमाण *res);
  res->state_वस्तुएँ.contents
    = gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  प्रत्यागम res;
}

parse_state *
new_parse_state (ध्रुव state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  प्रत्यागम res;
}

स्थिर parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (परिमाण *res);
  *res = *parent;
  res->state_वस्तुएँ.contents
    = gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_गिनती = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  प्रत्यागम res;
}

bool
parse_state_derivation_completed (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (ध्रुव parse_state *ps)
{
  प्रत्यागम (derivation *) ps->derivs.head_elt;
}

ध्रुव state_item *
parse_state_head (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->state_वस्तुएँ.head_elt;
}

ध्रुव state_item *
parse_state_tail (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->state_वस्तुएँ.tail_elt;
}

पूर्णाङ्क
parse_state_length (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->state_वस्तुएँ.total_size;
}

पूर्णाङ्क
parse_state_depth (ध्रुव parse_state *ps)
{
  प्रत्यागम ps->depth;
}

शून्य
parse_state_retain (parse_state *ps)
{
  ++ps->reference_गिनती;
}

शून्य
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

शून्य
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = रिक्त;
  प्रति (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_गिनती;
      free_contents = (ps->reference_गिनती == 1 && ps->free_contents_early)
        || (ps->reference_गिनती == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      यदि (free_contents)
        {
          यदि (ps->state_वस्तुएँ.contents)
            gl_list_free (ps->state_वस्तुएँ.contents);
          यदि (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      यदि (ps->reference_गिनती <= 0)
        {
          मुक्त (ps);
          ++frees;
        }
    }
}

माप_प्रकार
parse_state_hasher (ध्रुव parse_state *ps, माप_प्रकार max)
{
  ध्रुव संरचना si_chunk *sis = &ps->state_वस्तुएँ;
  प्रत्यागम ((state_item *) sis->head_elt - state_वस्तुएँ +
          (state_item *) sis->tail_elt - state_वस्तुएँ + sis->total_size) % max;
}

bool
parse_state_comparator (ध्रुव parse_state *ps1, ध्रुव parse_state *ps2)
{
  ध्रुव संरचना si_chunk *sis1 = &ps1->state_वस्तुएँ;
  ध्रुव संरचना si_chunk *sis2 = &ps2->state_वस्तुएँ;
  प्रत्यागम sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


शून्य
parse_state_completed_steps (ध्रुव parse_state *ps, पूर्णाङ्क *shifts, पूर्णाङ्क *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ध्रुव parse_state *मूल_ps = ps;
  यावत् (मूल_ps->parent)
    मूल_ps = मूल_ps->parent;

  state_item_list sis = मूल_ps->state_वस्तुएँ.contents;
  पूर्णाङ्क गिनती = 0;

  state_item *last = रिक्त;
  state_item *next = रिक्त;
  प्रति (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      यदि (last && last->state == next->state)
        ++गिनती;
      last = next;
    }
  *productions = गिनती;
  *shifts = मूल_ps->state_वस्तुएँ.total_size - गिनती;
}

प्रकारनिर्देश शून्य (*chunk_append_fn) (gl_list_t, ध्रुव शून्य *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
स्थिर शून्य
list_add_last (gl_list_t list, ध्रुव शून्य *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
स्थिर शून्य
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, पूर्णाङ्क split, पूर्णाङ्क n,
                        chunk_append_fn append_fn)
{
  पूर्णाङ्क ret_index = 0;
  पूर्णाङ्क ret_array = 0;
  प्रति (पूर्णाङ्क i = 0; i < n; ++i)
    {
      ध्रुव शून्य *p = रिक्त;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      यावत् (gl_list_iterator_next (&it, &p, रिक्त))
        यदि (p)
          {
            gl_list_t l = (gl_list_t) p;
            ध्रुव शून्य *si = रिक्त;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            यावत् (gl_list_iterator_next (&it2, &si, रिक्त))
              {
                यदि (ret_index++ == split)
                  ++ret_array;
                यदि (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

स्थिर parse_state_list
parse_state_list_new (शून्य)
{
  प्रत्यागम gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

स्थिर शून्य
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
स्थिर derivation_list
parser_निकालो (parse_state *ps, पूर्णाङ्क deriv_index,
            पूर्णाङ्क si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  प्रति (पूर्णाङ्क i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
  प्रति (parse_state *pn = ps; pn != रिक्त; pn = pn->parent)
    यदि (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_वस्तुएँ.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    अन्यथा
      {
        gl_list_add_first (chunks[1], pn->state_वस्तुएँ.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_वस्तुएँ.contents, रिक्त,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  माप_प्रकार s_size = gl_list_size (ret->state_वस्तुएँ.contents);
  ret->state_वस्तुएँ.total_size = s_size;
  यदि (s_size > 0)
    {
      ret->state_वस्तुएँ.tail_elt = gl_list_get_at (ret->state_वस्तुएँ.contents,
                                                  s_size - 1);
      ret->state_वस्तुएँ.head_elt =
        gl_list_get_at (ret->state_वस्तुएँ.contents, 0);
    }
  अन्यथा
    {
      ret->state_वस्तुएँ.tail_elt = रिक्त;
      ret->state_वस्तुएँ.head_elt = रिक्त;
    }
  माप_प्रकार d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  यदि (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  अन्यथा
    {
      ret->derivs.tail_elt = रिक्त;
      ret->derivs.head_elt = रिक्त;
    }
  प्रति (पूर्णाङ्क i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  प्रत्यागम popped_derivs;
}

शून्य
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  माप_प्रकार si_size = ps->state_वस्तुएँ.total_size;
  माप_प्रकार deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_निकालो (ps, si_size, deriv_size, temp);
  *sitems = temp->state_वस्तुएँ.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_वस्तुएँ.contents = रिक्त;
  temp->derivs.contents = रिक्त;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
स्थिर शून्य
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_संख्या prev_sin = si - state_वस्तुएँ;
  प्रति (state_item_संख्या sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_वस्तुएँ[sin].trans)
    {
      state_item *psi = &state_वस्तुएँ[prev_sin];
      प्रतीक_संख्या sp = item_संख्या_as_प्रतीक_संख्या (*psi->item);
      यदि (ISTOKEN (sp) || !nullable[sp - ntokens])
        भङ्ग;

      state_item *nsi = &state_वस्तुएँ[sin];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_item_rule (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  ध्रुव state_item *si = ps->state_वस्तुएँ.tail_elt;
  प्रतीक_संख्या sym = item_संख्या_as_प्रतीक_संख्या (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_संख्या si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  यदि (si_next < 0)
    प्रत्यागम result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_वस्तुएँ[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_वस्तुएँ[si_next], result);
  प्रत्यागम result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
स्थिर bool
compatible (प्रतीक_संख्या sym1, प्रतीक_संख्या sym2)
{
  यदि (sym1 == sym2)
    प्रत्यागम true;
  यदि (ISTOKEN (sym1) && ISVAR (sym2))
    प्रत्यागम bitset_test (FIRSTS (sym2), sym1);
  अन्यथा यदि (ISVAR (sym1) && ISTOKEN (sym2))
    प्रत्यागम bitset_test (FIRSTS (sym1), sym2);
  अन्यथा यदि (ISVAR (sym1) && ISVAR (sym2))
    प्रत्यागम !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  अन्यथा
    प्रत्यागम false;
}

parse_state_list
simulate_production (parse_state *ps, प्रतीक_संख्या compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  ध्रुव state_item *si = parse_state_tail (ps);
  यदि (si->prods)
    {
      bitset_iterator biter;
      state_item_संख्या sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_वस्तुएँ[sin];
          item_संख्या *itm1 = next->item;
          यदि (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            अनुवर्त;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          यदि (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  प्रत्यागम result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, पूर्णाङ्क rule_len, bitset प्रतीक_set)
{
  parse_state_list result = parse_state_list_new ();

  पूर्णाङ्क s_size = ps->state_वस्तुएँ.total_size;
  पूर्णाङ्क d_size = ps->derivs.total_size;
  यदि (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_मूल = empty_parse_state ();
  derivation_list popped_derivs =
    parser_निकालो (ps, d_size - rule_len,
                s_size - rule_len - 1, new_मूल);

  // update derivation
  state_item *si = (state_item *) ps->state_वस्तुएँ.tail_elt;
  ध्रुव rule *r = item_rule (si->item);
  प्रतीक_संख्या lhs = r->lhs->संख्या;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_मूल->depth;
  ps_derivs_append (new_मूल, deriv);

  यदि (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_मूल->state_वस्तुएँ.tail_elt;
      ps_si_append (new_मूल, &state_वस्तुएँ[tail->trans]);
      parse_state_list_append (result, new_मूल);
    }
  अन्यथा
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ध्रुव state_item *head = ps->state_वस्तुएँ.head_elt;
      state_item_list prev = lssi_reverse_production (head, प्रतीक_set);
      // TODO: better understand what causes this case.
      यदि (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_मूल);
          free_parse_state (new_मूल);
        }
      अन्यथा
        {
          state_item *psis = रिक्त;
          प्रति (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_मूल);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              संरचना si_chunk *sis = &copy->state_वस्तुएँ;
              ध्रुव state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_वस्तुएँ[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  प्रत्यागम result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ध्रुव state_item *head = ps->state_वस्तुएँ.head_elt;
  प्रतीक_संख्या prepend_sym =
    item_संख्या_as_प्रतीक_संख्या (*(head->item - 1));
  bitset_iterator biter;
  state_item_संख्या sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_वस्तुएँ[sin]);
    यदि (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  प्रत्यागम res;
}

शून्य
print_parse_state (parse_state *ps)
{
  खाता *out = मानक_त्रुटि;
  ख_लिखो (out, "(size %zu depth %d rc %d)\n",
          ps->state_वस्तुएँ.total_size, ps->depth, ps->reference_गिनती);
  state_item_print (ps->state_वस्तुएँ.head_elt, out, "");
  state_item_print (ps->state_वस्तुएँ.tail_elt, out, "");
  यदि (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  अ_दो ('\n', out);
}
