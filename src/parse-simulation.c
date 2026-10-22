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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "parse-simulation.h"

#ಅನ್ತರ್ಭಾವ <gl_linked_list.h>
#ಅನ್ತರ್ಭಾವ <gl_xlist.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "lssi.h"
#ಅನ್ತರ್ಭಾವ "nullable.h"

ಸಂರಚನಾ parse_state
{
  // Path of state-items the parser has traversed.
  ಸಂರಚನಾ si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ಧ್ರುವ state_item *head_elt;
    ಧ್ರುವ state_item *tail_elt;
    size_t total_size;
  } state_ವಸ್ತುಗಳು;
  // List of derivations of the symbols.
  ಸಂರಚನಾ deriv_chunk
  {
    derivation_list contents;
    ಧ್ರುವ derivation *head_elt;
    ಧ್ರುವ derivation *tail_elt;
    size_t total_size;
  } derivs;
  ಸಂರಚನಾ parse_state *parent;
  ಪೂರ್ಣಾಙ್ಕ reference_ಎಣಿಕೆ;
  // Incremented during productions, decremented during reductions.
  ಪೂರ್ಣಾಙ್ಕ depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ಸ್ಥಿರ ಶೂನ್ಯ
ps_si_prepend (parse_state *ps, ಧ್ರುವ state_item *si)
{
  ಸಂರಚನಾ si_chunk *sic = &ps->state_ವಸ್ತುಗಳು;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  ಯದಿ (!sic->tail_elt)
    sic->tail_elt = si;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ps_si_append (parse_state *ps, ಧ್ರುವ state_item *si)
{
  ಸಂರಚನಾ si_chunk *sic = &ps->state_ವಸ್ತುಗಳು;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  ಯದಿ (!sic->head_elt)
    sic->head_elt = si;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  ಸಂರಚನಾ deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  ಯದಿ (!dc->tail_elt)
    dc->tail_elt = d;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ps_derivs_append (parse_state *ps, derivation *d)
{
  ಸಂರಚನಾ deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  ಯದಿ (!dc->head_elt)
    dc->head_elt = d;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ allocs = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ frees = 0;

ಸ್ಥಿರ parse_state *
empty_parse_state (ಶೂನ್ಯ)
{
  parse_state *res = xcalloc (1, ಪರಿಮಾಣ *res);
  res->state_ವಸ್ತುಗಳು.contents
    = gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  ಪ್ರತ್ಯಾಗಮ res;
}

parse_state *
new_parse_state (ಧ್ರುವ state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (ಪರಿಮಾಣ *res);
  *res = *parent;
  res->state_ವಸ್ತುಗಳು.contents
    = gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_ಎಣಿಕೆ = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  ಪ್ರತ್ಯಾಗಮ res;
}

bool
parse_state_derivation_completed (ಧ್ರುವ parse_state *ps)
{
  ಪ್ರತ್ಯಾಗಮ ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (ಧ್ರುವ parse_state *ps)
{
  ಪ್ರತ್ಯಾಗಮ (derivation *) ps->derivs.head_elt;
}

ಧ್ರುವ state_item *
parse_state_head (ಧ್ರುವ parse_state *ps)
{
  ಪ್ರತ್ಯಾಗಮ ps->state_ವಸ್ತುಗಳು.head_elt;
}

ಧ್ರುವ state_item *
parse_state_tail (ಧ್ರುವ parse_state *ps)
{
  ಪ್ರತ್ಯಾಗಮ ps->state_ವಸ್ತುಗಳು.tail_elt;
}

ಪೂರ್ಣಾಙ್ಕ
parse_state_length (ಧ್ರುವ parse_state *ps)
{
  ಪ್ರತ್ಯಾಗಮ ps->state_ವಸ್ತುಗಳು.total_size;
}

ಪೂರ್ಣಾಙ್ಕ
parse_state_depth (ಧ್ರುವ parse_state *ps)
{
  ಪ್ರತ್ಯಾಗಮ ps->depth;
}

ಶೂನ್ಯ
parse_state_retain (parse_state *ps)
{
  ++ps->reference_ಎಣಿಕೆ;
}

ಶೂನ್ಯ
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

ಶೂನ್ಯ
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = ರಿಕ್ತ;
  ಪ್ರತಿ (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_ಎಣಿಕೆ;
      free_contents = (ps->reference_ಎಣಿಕೆ == 1 && ps->free_contents_early)
        || (ps->reference_ಎಣಿಕೆ == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      ಯದಿ (free_contents)
        {
          ಯದಿ (ps->state_ವಸ್ತುಗಳು.contents)
            gl_list_free (ps->state_ವಸ್ತುಗಳು.contents);
          ಯದಿ (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      ಯದಿ (ps->reference_ಎಣಿಕೆ <= 0)
        {
          ಮುಕ್ತ (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ಧ್ರುವ parse_state *ps, size_t max)
{
  ಧ್ರುವ ಸಂರಚನಾ si_chunk *sis = &ps->state_ವಸ್ತುಗಳು;
  ಪ್ರತ್ಯಾಗಮ ((state_item *) sis->head_elt - state_ವಸ್ತುಗಳು +
          (state_item *) sis->tail_elt - state_ವಸ್ತುಗಳು + sis->total_size) % max;
}

bool
parse_state_comparator (ಧ್ರುವ parse_state *ps1, ಧ್ರುವ parse_state *ps2)
{
  ಧ್ರುವ ಸಂರಚನಾ si_chunk *sis1 = &ps1->state_ವಸ್ತುಗಳು;
  ಧ್ರುವ ಸಂರಚನಾ si_chunk *sis2 = &ps2->state_ವಸ್ತುಗಳು;
  ಪ್ರತ್ಯಾಗಮ sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


ಶೂನ್ಯ
parse_state_completed_steps (ಧ್ರುವ parse_state *ps, ಪೂರ್ಣಾಙ್ಕ *shifts, ಪೂರ್ಣಾಙ್ಕ *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ಧ್ರುವ parse_state *ಬೇರು_ps = ps;
  ಯಾವತ್ (ಬೇರು_ps->parent)
    ಬೇರು_ps = ಬೇರು_ps->parent;

  state_item_list sis = ಬೇರು_ps->state_ವಸ್ತುಗಳು.contents;
  ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;

  state_item *last = ರಿಕ್ತ;
  state_item *next = ರಿಕ್ತ;
  ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      ಯದಿ (last && last->state == next->state)
        ++ಎಣಿಕೆ;
      last = next;
    }
  *productions = ಎಣಿಕೆ;
  *shifts = ಬೇರು_ps->state_ವಸ್ತುಗಳು.total_size - ಎಣಿಕೆ;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*chunk_append_fn) (gl_list_t, ಧ್ರುವ ಶೂನ್ಯ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ಸ್ಥಿರ ಶೂನ್ಯ
list_add_last (gl_list_t list, ಧ್ರುವ ಶೂನ್ಯ *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ಸ್ಥಿರ ಶೂನ್ಯ
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, ಪೂರ್ಣಾಙ್ಕ split, ಪೂರ್ಣಾಙ್ಕ n,
                        chunk_append_fn append_fn)
{
  ಪೂರ್ಣಾಙ್ಕ ret_index = 0;
  ಪೂರ್ಣಾಙ್ಕ ret_array = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; ++i)
    {
      ಧ್ರುವ ಶೂನ್ಯ *p = ರಿಕ್ತ;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      ಯಾವತ್ (gl_list_iterator_next (&it, &p, ರಿಕ್ತ))
        ಯದಿ (p)
          {
            gl_list_t l = (gl_list_t) p;
            ಧ್ರುವ ಶೂನ್ಯ *si = ರಿಕ್ತ;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            ಯಾವತ್ (gl_list_iterator_next (&it2, &si, ರಿಕ್ತ))
              {
                ಯದಿ (ret_index++ == split)
                  ++ret_array;
                ಯದಿ (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

ಸ್ಥಿರ parse_state_list
parse_state_list_new (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ಸ್ಥಿರ derivation_list
parser_ತೆಗೆ (parse_state *ps, ಪೂರ್ಣಾಙ್ಕ deriv_index,
            ಪೂರ್ಣಾಙ್ಕ si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
  ಪ್ರತಿ (parse_state *pn = ps; pn != ರಿಕ್ತ; pn = pn->parent)
    ಯದಿ (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_ವಸ್ತುಗಳು.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    ಅನ್ಯಥಾ
      {
        gl_list_add_first (chunks[1], pn->state_ವಸ್ತುಗಳು.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_ವಸ್ತುಗಳು.contents, ರಿಕ್ತ,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_ವಸ್ತುಗಳು.contents);
  ret->state_ವಸ್ತುಗಳು.total_size = s_size;
  ಯದಿ (s_size > 0)
    {
      ret->state_ವಸ್ತುಗಳು.tail_elt = gl_list_get_at (ret->state_ವಸ್ತುಗಳು.contents,
                                                  s_size - 1);
      ret->state_ವಸ್ತುಗಳು.head_elt =
        gl_list_get_at (ret->state_ವಸ್ತುಗಳು.contents, 0);
    }
  ಅನ್ಯಥಾ
    {
      ret->state_ವಸ್ತುಗಳು.tail_elt = ರಿಕ್ತ;
      ret->state_ವಸ್ತುಗಳು.head_elt = ರಿಕ್ತ;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  ಯದಿ (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  ಅನ್ಯಥಾ
    {
      ret->derivs.tail_elt = ರಿಕ್ತ;
      ret->derivs.head_elt = ರಿಕ್ತ;
    }
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  ಪ್ರತ್ಯಾಗಮ popped_derivs;
}

ಶೂನ್ಯ
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_ವಸ್ತುಗಳು.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_ತೆಗೆ (ps, si_size, deriv_size, temp);
  *sitems = temp->state_ವಸ್ತುಗಳು.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_ವಸ್ತುಗಳು.contents = ರಿಕ್ತ;
  temp->derivs.contents = ರಿಕ್ತ;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_ಸಂಖ್ಯೆ prev_sin = si - state_ವಸ್ತುಗಳು;
  ಪ್ರತಿ (state_item_ಸಂಖ್ಯೆ sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_ವಸ್ತುಗಳು[sin].trans)
    {
      state_item *psi = &state_ವಸ್ತುಗಳು[prev_sin];
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sp = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*psi->item);
      ಯದಿ (ISTOKEN (sp) || !nullable[sp - ntokens])
        ಭಙ್ಗ;

      state_item *nsi = &state_ವಸ್ತುಗಳು[sin];
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
  ಧ್ರುವ state_item *si = ps->state_ವಸ್ತುಗಳು.tail_elt;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_ಸಂಖ್ಯೆ si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  ಯದಿ (si_next < 0)
    ಪ್ರತ್ಯಾಗಮ result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_ವಸ್ತುಗಳು[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_ವಸ್ತುಗಳು[si_next], result);
  ಪ್ರತ್ಯಾಗಮ result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ಸ್ಥಿರ bool
compatible (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym1, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym2)
{
  ಯದಿ (sym1 == sym2)
    ಪ್ರತ್ಯಾಗಮ true;
  ಯದಿ (ISTOKEN (sym1) && ISVAR (sym2))
    ಪ್ರತ್ಯಾಗಮ bitset_test (FIRSTS (sym2), sym1);
  ಅನ್ಯಥಾ ಯದಿ (ISVAR (sym1) && ISTOKEN (sym2))
    ಪ್ರತ್ಯಾಗಮ bitset_test (FIRSTS (sym1), sym2);
  ಅನ್ಯಥಾ ಯದಿ (ISVAR (sym1) && ISVAR (sym2))
    ಪ್ರತ್ಯಾಗಮ !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ false;
}

parse_state_list
simulate_production (parse_state *ps, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  ಧ್ರುವ state_item *si = parse_state_tail (ps);
  ಯದಿ (si->prods)
    {
      bitset_iterator biter;
      state_item_ಸಂಖ್ಯೆ sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_ವಸ್ತುಗಳು[sin];
          item_ಸಂಖ್ಯೆ *itm1 = next->item;
          ಯದಿ (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            ಅನುವರ್ತ;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          ಯದಿ (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  ಪ್ರತ್ಯಾಗಮ result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, ಪೂರ್ಣಾಙ್ಕ rule_len, bitset ಚಿಹ್ನೆ_set)
{
  parse_state_list result = parse_state_list_new ();

  ಪೂರ್ಣಾಙ್ಕ s_size = ps->state_ವಸ್ತುಗಳು.total_size;
  ಪೂರ್ಣಾಙ್ಕ d_size = ps->derivs.total_size;
  ಯದಿ (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_ಬೇರು = empty_parse_state ();
  derivation_list popped_derivs =
    parser_ತೆಗೆ (ps, d_size - rule_len,
                s_size - rule_len - 1, new_ಬೇರು);

  // update derivation
  state_item *si = (state_item *) ps->state_ವಸ್ತುಗಳು.tail_elt;
  ಧ್ರುವ rule *r = item_rule (si->item);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = r->lhs->ಸಂಖ್ಯೆ;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_ಬೇರು->depth;
  ps_derivs_append (new_ಬೇರು, deriv);

  ಯದಿ (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_ಬೇರು->state_ವಸ್ತುಗಳು.tail_elt;
      ps_si_append (new_ಬೇರು, &state_ವಸ್ತುಗಳು[tail->trans]);
      parse_state_list_append (result, new_ಬೇರು);
    }
  ಅನ್ಯಥಾ
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ಧ್ರುವ state_item *head = ps->state_ವಸ್ತುಗಳು.head_elt;
      state_item_list prev = lssi_reverse_production (head, ಚಿಹ್ನೆ_set);
      // TODO: better understand what causes this case.
      ಯದಿ (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_ಬೇರು);
          free_parse_state (new_ಬೇರು);
        }
      ಅನ್ಯಥಾ
        {
          state_item *psis = ರಿಕ್ತ;
          ಪ್ರತಿ (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_ಬೇರು);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              ಸಂರಚನಾ si_chunk *sis = &copy->state_ವಸ್ತುಗಳು;
              ಧ್ರುವ state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_ವಸ್ತುಗಳು[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  ಪ್ರತ್ಯಾಗಮ result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ಧ್ರುವ state_item *head = ps->state_ವಸ್ತುಗಳು.head_elt;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ prepend_sym =
    item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(head->item - 1));
  bitset_iterator biter;
  state_item_ಸಂಖ್ಯೆ sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_ವಸ್ತುಗಳು[sin]);
    ಯದಿ (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_ವಸ್ತುಗಳು.total_size, ps->depth, ps->reference_ಎಣಿಕೆ);
  state_item_print (ps->state_ವಸ್ತುಗಳು.head_elt, out, "");
  state_item_print (ps->state_ವಸ್ತುಗಳು.tail_elt, out, "");
  ಯದಿ (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
