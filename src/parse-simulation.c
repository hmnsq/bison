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

ರಚನೆ parse_state
{
  // Path of state-items the parser has traversed.
  ರಚನೆ si_chunk
  {
    // Elements newly added in this chunk.
    state_ಅಂಶ_list contents;
    // Properties of the linked list this chunk represents.
    ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ತಲೆ_elt;
    ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ಬಾಲ_elt;
    size_t ಒಟ್ಟು_ಗಾತ್ರ;
  } state_ವಸ್ತುಗಳು;
  // List of derivations of the symbols.
  ರಚನೆ deriv_chunk
  {
    derivation_list contents;
    ಸ್ಥಿರಾಂಕ derivation *ತಲೆ_elt;
    ಸ್ಥಿರಾಂಕ derivation *ಬಾಲ_elt;
    size_t ಒಟ್ಟು_ಗಾತ್ರ;
  } derivs;
  ರಚನೆ parse_state *ಪೋಷಕ;
  ಪೂರ್ಣಾಂಕ reference_ಎಣಿಕೆ;
  // Incremented during productions, decremented during reductions.
  ಪೂರ್ಣಾಂಕ ಆಳ;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ಸ್ಥಿರ ಶೂನ್ಯ
ps_si_prepend (parse_state *ps, ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si)
{
  ರಚನೆ si_chunk *sic = &ps->state_ವಸ್ತುಗಳು;
  gl_list_ಕೂಡಿಸು_ಮೊದಲ (sic->contents, si);
  sic->ತಲೆ_elt = si;
  ++sic->ಒಟ್ಟು_ಗಾತ್ರ;
  ಆದರೆ (!sic->ಬಾಲ_elt)
    sic->ಬಾಲ_elt = si;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ps_si_append (parse_state *ps, ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si)
{
  ರಚನೆ si_chunk *sic = &ps->state_ವಸ್ತುಗಳು;
  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (sic->contents, si);
  sic->ಬಾಲ_elt = si;
  ++sic->ಒಟ್ಟು_ಗಾತ್ರ;
  ಆದರೆ (!sic->ತಲೆ_elt)
    sic->ತಲೆ_elt = si;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  ರಚನೆ deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->ತಲೆ_elt = d;
  ++dc->ಒಟ್ಟು_ಗಾತ್ರ;
  ಆದರೆ (!dc->ಬಾಲ_elt)
    dc->ಬಾಲ_elt = d;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ps_derivs_append (parse_state *ps, derivation *d)
{
  ರಚನೆ deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->ಬಾಲ_elt = d;
  ++dc->ಒಟ್ಟು_ಗಾತ್ರ;
  ಆದರೆ (!dc->ತಲೆ_elt)
    dc->ತಲೆ_elt = d;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ allocs = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ frees = 0;

ಸ್ಥಿರ parse_state *
ಖಾಲಿಗಳು_parse_state (ಶೂನ್ಯ)
{
  parse_state *res = xcalloc (1, ಗಾತ್ರ *res);
  res->state_ವಸ್ತುಗಳು.contents
    = gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  ಹಿಂತಿರುಗಿ res;
}

parse_state *
new_parse_state (ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si)
{
  parse_state *res = ಖಾಲಿಗಳು_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ parse_state *
copy_parse_state (bool prepend, parse_state *ಪೋಷಕ)
{
  parse_state *res = xmalloc (ಗಾತ್ರ *res);
  *res = *ಪೋಷಕ;
  res->state_ವಸ್ತುಗಳು.contents
    = gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
  res->derivs.contents = derivation_list_new ();
  res->ಪೋಷಕ = ಪೋಷಕ;
  res->prepend = prepend;
  res->reference_ಎಣಿಕೆ = 0;
  res->free_contents_early = false;
  parse_state_retain (ಪೋಷಕ);
  ++allocs;
  ಹಿಂತಿರುಗಿ res;
}

bool
parse_state_derivation_completed (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಹಿಂತಿರುಗಿ ps->derivs.ಒಟ್ಟು_ಗಾತ್ರ == 1;
}

derivation *
parse_state_derivation (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಹಿಂತಿರುಗಿ (derivation *) ps->derivs.ತಲೆ_elt;
}

ಸ್ಥಿರಾಂಕ state_ಅಂಶ *
parse_state_ತಲೆ (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಹಿಂತಿರುಗಿ ps->state_ವಸ್ತುಗಳು.ತಲೆ_elt;
}

ಸ್ಥಿರಾಂಕ state_ಅಂಶ *
parse_state_ಬಾಲ (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಹಿಂತಿರುಗಿ ps->state_ವಸ್ತುಗಳು.ಬಾಲ_elt;
}

ಪೂರ್ಣಾಂಕ
parse_state_ಉದ್ದಳತೆ (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಹಿಂತಿರುಗಿ ps->state_ವಸ್ತುಗಳು.ಒಟ್ಟು_ಗಾತ್ರ;
}

ಪೂರ್ಣಾಂಕ
parse_state_ಆಳ (ಸ್ಥಿರಾಂಕ parse_state *ps)
{
  ಹಿಂತಿರುಗಿ ps->ಆಳ;
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
  parse_state *ಪೋಷಕ_ps = ಖಾಲಿ;
  ಗಾಗಿ (parse_state *ps = original_ps; ps && free_contents; ps = ಪೋಷಕ_ps)
    {
      --ps->reference_ಎಣಿಕೆ;
      free_contents = (ps->reference_ಎಣಿಕೆ == 1 && ps->free_contents_early)
        || (ps->reference_ಎಣಿಕೆ == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      ಆದರೆ (free_contents)
        {
          ಆದರೆ (ps->state_ವಸ್ತುಗಳು.contents)
            gl_list_free (ps->state_ವಸ್ತುಗಳು.contents);
          ಆದರೆ (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      ಪೋಷಕ_ps = ps->ಪೋಷಕ;
      ಆದರೆ (ps->reference_ಎಣಿಕೆ <= 0)
        {
          ಬಿಡುಗಡೆ (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ಸ್ಥಿರಾಂಕ parse_state *ps, size_t max)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ si_chunk *sis = &ps->state_ವಸ್ತುಗಳು;
  ಹಿಂತಿರುಗಿ ((state_ಅಂಶ *) sis->ತಲೆ_elt - state_ವಸ್ತುಗಳು +
          (state_ಅಂಶ *) sis->ಬಾಲ_elt - state_ವಸ್ತುಗಳು + sis->ಒಟ್ಟು_ಗಾತ್ರ) % max;
}

bool
parse_state_comparator (ಸ್ಥಿರಾಂಕ parse_state *ps1, ಸ್ಥಿರಾಂಕ parse_state *ps2)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ si_chunk *sis1 = &ps1->state_ವಸ್ತುಗಳು;
  ಸ್ಥಿರಾಂಕ ರಚನೆ si_chunk *sis2 = &ps2->state_ವಸ್ತುಗಳು;
  ಹಿಂತಿರುಗಿ sis1->ತಲೆ_elt == sis2->ತಲೆ_elt
    && sis1->ಬಾಲ_elt == sis2->ಬಾಲ_elt
    && sis1->ಒಟ್ಟು_ಗಾತ್ರ == sis2->ಒಟ್ಟು_ಗಾತ್ರ;
}


ಶೂನ್ಯ
parse_state_completed_ಹೆಜ್ಜೆಗಳು (ಸ್ಥಿರಾಂಕ parse_state *ps, ಪೂರ್ಣಾಂಕ *shifts, ಪೂರ್ಣಾಂಕ *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ಸ್ಥಿರಾಂಕ parse_state *ಬೇರು_ps = ps;
  ಆಗ (ಬೇರು_ps->ಪೋಷಕ)
    ಬೇರು_ps = ಬೇರು_ps->ಪೋಷಕ;

  state_ಅಂಶ_list sis = ಬೇರು_ps->state_ವಸ್ತುಗಳು.contents;
  ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

  state_ಅಂಶ *ಕೊನೆಯ = ಖಾಲಿ;
  state_ಅಂಶ *ಮುಂದಿನ = ಖಾಲಿ;
  ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (sis);
       state_ಅಂಶ_list_ಮುಂದಿನ (&it, &ಮುಂದಿನ);
       )
    {
      ಆದರೆ (ಕೊನೆಯ && ಕೊನೆಯ->state == ಮುಂದಿನ->state)
        ++ಎಣಿಕೆ_2;
      ಕೊನೆಯ = ಮುಂದಿನ;
    }
  *productions = ಎಣಿಕೆ_2;
  *shifts = ಬೇರು_ps->state_ವಸ್ತುಗಳು.ಒಟ್ಟು_ಗಾತ್ರ - ಎಣಿಕೆ_2;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*chunk_append_fn) (gl_list_t, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ಸ್ಥಿರ ಶೂನ್ಯ
list_ಕೂಡಿಸು_ಕೊನೆಯ (gl_list_t list, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *elt)
{
  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ಸ್ಥಿರ ಶೂನ್ಯ
list_flatten_and_ವಿಭಜಿಸು (gl_list_t *list, gl_list_t *rets, ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು, ಪೂರ್ಣಾಂಕ n,
                        chunk_append_fn append_fn)
{
  ಪೂರ್ಣಾಂಕ ret_ಸೂಚ್ಯಂಕ = 0;
  ಪೂರ್ಣಾಂಕ ret_array = 0;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; ++i)
    {
      ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p = ಖಾಲಿ;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      ಆಗ (gl_list_iterator_ಮುಂದಿನ (&it, &p, ಖಾಲಿ))
        ಆದರೆ (p)
          {
            gl_list_t l = (gl_list_t) p;
            ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *si = ಖಾಲಿ;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            ಆಗ (gl_list_iterator_ಮುಂದಿನ (&it2, &si, ಖಾಲಿ))
              {
                ಆದರೆ (ret_ಸೂಚ್ಯಂಕ++ == ವಿಭಜಿಸು)
                  ++ret_array;
                ಆದರೆ (rets[ret_array])
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
  ಹಿಂತಿರುಗಿ gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ಸ್ಥಿರ derivation_list
parser_ತೆಗೆ (parse_state *ps, ಪೂರ್ಣಾಂಕ deriv_ಸೂಚ್ಯಂಕ,
            ಪೂರ್ಣಾಂಕ si_ಸೂಚ್ಯಂಕ, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
  ಗಾಗಿ (parse_state *pn = ps; pn != ಖಾಲಿ; pn = pn->ಪೋಷಕ)
    ಆದರೆ (pn->prepend)
      {
        gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (chunks[0], pn->state_ವಸ್ತುಗಳು.contents);
        gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (chunks[2], pn->derivs.contents);
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        gl_list_ಕೂಡಿಸು_ಮೊದಲ (chunks[1], pn->state_ವಸ್ತುಗಳು.contents);
        gl_list_ಕೂಡಿಸು_ಮೊದಲ (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_ವಸ್ತುಗಳು.contents, ಖಾಲಿ,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_ವಿಭಜಿಸು (chunks, ret_chunks, si_ಸೂಚ್ಯಂಕ, 2,
                          list_ಕೂಡಿಸು_ಕೊನೆಯ);
  list_flatten_and_ವಿಭಜಿಸು (chunks + 2, ret_chunks + 2, deriv_ಸೂಚ್ಯಂಕ, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_ಗಾತ್ರ = gl_list_ಗಾತ್ರ (ret->state_ವಸ್ತುಗಳು.contents);
  ret->state_ವಸ್ತುಗಳು.ಒಟ್ಟು_ಗಾತ್ರ = s_ಗಾತ್ರ;
  ಆದರೆ (s_ಗಾತ್ರ > 0)
    {
      ret->state_ವಸ್ತುಗಳು.ಬಾಲ_elt = gl_list_get_at (ret->state_ವಸ್ತುಗಳು.contents,
                                                  s_ಗಾತ್ರ - 1);
      ret->state_ವಸ್ತುಗಳು.ತಲೆ_elt =
        gl_list_get_at (ret->state_ವಸ್ತುಗಳು.contents, 0);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret->state_ವಸ್ತುಗಳು.ಬಾಲ_elt = ಖಾಲಿ;
      ret->state_ವಸ್ತುಗಳು.ತಲೆ_elt = ಖಾಲಿ;
    }
  size_t d_ಗಾತ್ರ = gl_list_ಗಾತ್ರ (ret->derivs.contents);
  ret->derivs.ಒಟ್ಟು_ಗಾತ್ರ = d_ಗಾತ್ರ;
  ಆದರೆ (d_ಗಾತ್ರ > 0)
    {
      ret->derivs.ಬಾಲ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_ಗಾತ್ರ - 1);
      ret->derivs.ತಲೆ_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret->derivs.ಬಾಲ_elt = ಖಾಲಿ;
      ret->derivs.ತಲೆ_elt = ಖಾಲಿ;
    }
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  ಹಿಂತಿರುಗಿ popped_derivs;
}

ಶೂನ್ಯ
parse_state_lists (parse_state *ps, state_ಅಂಶ_list *sitems,
                   derivation_list *derivs)
{
  parse_state *ತಾತ್ಕಾಲಿಕ = ಖಾಲಿಗಳು_parse_state ();
  size_t si_ಗಾತ್ರ = ps->state_ವಸ್ತುಗಳು.ಒಟ್ಟು_ಗಾತ್ರ;
  size_t deriv_ಗಾತ್ರ = ps->derivs.ಒಟ್ಟು_ಗಾತ್ರ;
  derivation_list dl = parser_ತೆಗೆ (ps, si_ಗಾತ್ರ, deriv_ಗಾತ್ರ, ತಾತ್ಕಾಲಿಕ);
  *sitems = ತಾತ್ಕಾಲಿಕ->state_ವಸ್ತುಗಳು.contents;
  *derivs = ತಾತ್ಕಾಲಿಕ->derivs.contents;
  // prevent the return lists from being freed
  ತಾತ್ಕಾಲಿಕ->state_ವಸ್ತುಗಳು.contents = ಖಾಲಿ;
  ತಾತ್ಕಾಲಿಕ->derivs.contents = ಖಾಲಿ;
  free_parse_state (ತಾತ್ಕಾಲಿಕ);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
nullable_ಆವರಣ (parse_state *ps, state_ಅಂಶ *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_ಅಂಶ_ಸಂಖ್ಯೆ prev_sin = si - state_ವಸ್ತುಗಳು;
  ಗಾಗಿ (state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ = si->trans; ಜ್ಯಾ != -1;
       prev_sin = ಜ್ಯಾ, ಜ್ಯಾ = state_ವಸ್ತುಗಳು[ಜ್ಯಾ].trans)
    {
      state_ಅಂಶ *psi = &state_ವಸ್ತುಗಳು[prev_sin];
      ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sp = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*psi->ಅಂಶ);
      ಆದರೆ (ISTOKEN (sp) || !nullable[sp - ntokens])
        ನಿಲ್ಲಿಸು;

      state_ಅಂಶ *nsi = &state_ವಸ್ತುಗಳು[ಜ್ಯಾ];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_ಅಂಶ_ನಿಯಮ (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si = ps->state_ವಸ್ತುಗಳು.ಬಾಲ_elt;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym = ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*si->ಅಂಶ);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list ಫಲಿತಾಂಶ = parse_state_list_new ();
  state_ಅಂಶ_ಸಂಖ್ಯೆ si_ಮುಂದಿನ = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  ಆದರೆ (si_ಮುಂದಿನ < 0)
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  parse_state *ಮುಂದಿನ_ps = copy_parse_state (false, ps);
  ps_si_append (ಮುಂದಿನ_ps, &state_ವಸ್ತುಗಳು[si_ಮುಂದಿನ]);
  ps_derivs_append (ಮುಂದಿನ_ps, derivation_new_ಎಲೆ (sym));
  parse_state_list_append (ಫಲಿತಾಂಶ, ಮುಂದಿನ_ps);

  nullable_ಆವರಣ (ಮುಂದಿನ_ps, &state_ವಸ್ತುಗಳು[si_ಮುಂದಿನ], ಫಲಿತಾಂಶ);
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ಸ್ಥಿರ bool
compatible (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym1, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym2)
{
  ಆದರೆ (sym1 == sym2)
    ಹಿಂತಿರುಗಿ true;
  ಆದರೆ (ISTOKEN (sym1) && ISVAR (sym2))
    ಹಿಂತಿರುಗಿ bitset_test (FIRSTS (sym2), sym1);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ISVAR (sym1) && ISTOKEN (sym2))
    ಹಿಂತಿರುಗಿ bitset_test (FIRSTS (sym1), sym2);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ISVAR (sym1) && ISVAR (sym2))
    ಹಿಂತಿರುಗಿ !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ false;
}

parse_state_list
simulate_production (parse_state *ps, ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ compat_sym)
{
  parse_state_list ಫಲಿತಾಂಶ = parse_state_list_new ();
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *si = parse_state_ಬಾಲ (ps);
  ಆದರೆ (si->prods)
    {
      bitset_iterator biter;
      state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
      BITSET_FOR_EACH (biter, si->prods, ಜ್ಯಾ, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_ಅಂಶ *ಮುಂದಿನ = &state_ವಸ್ತುಗಳು[ಜ್ಯಾ];
          ಅಂಶ_ಸಂಖ್ಯೆ *itm1 = ಮುಂದಿನ->ಅಂಶ;
          ಆದರೆ (!compatible (*itm1, compat_sym) || !production_allowed (si, ಮುಂದಿನ))
            ಮುಂದುವರಿ;
          parse_state *ಮುಂದಿನ_ps = copy_parse_state (false, ps);
          ps_si_append (ಮುಂದಿನ_ps, ಮುಂದಿನ);
          parse_state_list_append (ಫಲಿತಾಂಶ, ಮುಂದಿನ_ps);
          ಆದರೆ (ಮುಂದಿನ_ps->ಆಳ >= 0)
            ++ಮುಂದಿನ_ps->ಆಳ;
          nullable_ಆವರಣ (ಮುಂದಿನ_ps, ಮುಂದಿನ, ಫಲಿತಾಂಶ);
        }
    }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, ಪೂರ್ಣಾಂಕ ನಿಯಮ_len, bitset ಚಿಹ್ನೆ_ಗಣ)
{
  parse_state_list ಫಲಿತಾಂಶ = parse_state_list_new ();

  ಪೂರ್ಣಾಂಕ s_ಗಾತ್ರ = ps->state_ವಸ್ತುಗಳು.ಒಟ್ಟು_ಗಾತ್ರ;
  ಪೂರ್ಣಾಂಕ d_ಗಾತ್ರ = ps->derivs.ಒಟ್ಟು_ಗಾತ್ರ;
  ಆದರೆ (ps->ಆಳ >= 0)
    d_ಗಾತ್ರ--;                   // account for dot
  parse_state *new_ಬೇರು = ಖಾಲಿಗಳು_parse_state ();
  derivation_list popped_derivs =
    parser_ತೆಗೆ (ps, d_ಗಾತ್ರ - ನಿಯಮ_len,
                s_ಗಾತ್ರ - ನಿಯಮ_len - 1, new_ಬೇರು);

  // update derivation
  state_ಅಂಶ *si = (state_ಅಂಶ *) ps->state_ವಸ್ತುಗಳು.ಬಾಲ_elt;
  ಸ್ಥಿರಾಂಕ ನಿಯಮ *r = ಅಂಶ_ನಿಯಮ (si->ಅಂಶ);
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ lhs = r->lhs->ಸಂಖ್ಯೆ;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_ಅಂಶ_ನಿಯಮ (si));
  --new_ಬೇರು->ಆಳ;
  ps_derivs_append (new_ಬೇರು, deriv);

  ಆದರೆ (s_ಗಾತ್ರ != ನಿಯಮ_len + 1)
    {
      state_ಅಂಶ *ಬಾಲ = (state_ಅಂಶ *) new_ಬೇರು->state_ವಸ್ತುಗಳು.ಬಾಲ_elt;
      ps_si_append (new_ಬೇರು, &state_ವಸ್ತುಗಳು[ಬಾಲ->trans]);
      parse_state_list_append (ಫಲಿತಾಂಶ, new_ಬೇರು);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ತಲೆ = ps->state_ವಸ್ತುಗಳು.ತಲೆ_elt;
      state_ಅಂಶ_list prev = lssi_ಹಿಮ್ಮುಖ_production (ತಲೆ, ಚಿಹ್ನೆ_ಗಣ);
      // TODO: better understand what causes this case.
      ಆದರೆ (gl_list_ಗಾತ್ರ (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_ಬೇರು);
          free_parse_state (new_ಬೇರು);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          state_ಅಂಶ *psis = ಖಾಲಿ;
          ಗಾಗಿ (gl_list_iterator_t it = gl_list_iterator (prev);
               state_ಅಂಶ_list_ಮುಂದಿನ (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_ಬೇರು);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              ರಚನೆ si_chunk *sis = &copy->state_ವಸ್ತುಗಳು;
              ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ಬಾಲ = sis->ಬಾಲ_elt;
              ps_si_append (copy, &state_ವಸ್ತುಗಳು[ಬಾಲ->trans]);
              parse_state_list_append (ಫಲಿತಾಂಶ, copy);
              nullable_ಆವರಣ (copy, (state_ಅಂಶ *) sis->ಬಾಲ_elt, ಫಲಿತಾಂಶ);
            }
        }
      gl_list_free (prev);
    }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ಸ್ಥಿರಾಂಕ state_ಅಂಶ *ತಲೆ = ps->state_ವಸ್ತುಗಳು.ತಲೆ_elt;
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ prepend_sym =
    ಅಂಶ_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (*(ತಲೆ->ಅಂಶ - 1));
  bitset_iterator biter;
  state_ಅಂಶ_ಸಂಖ್ಯೆ ಜ್ಯಾ;
  BITSET_FOR_EACH (biter, ತಲೆ->revs, ಜ್ಯಾ, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_ವಸ್ತುಗಳು[ಜ್ಯಾ]);
    ಆದರೆ (SI_TRANSITION (ತಲೆ))
      ps_derivs_prepend (copy, derivation_new_ಎಲೆ (prepend_sym));
    parse_state_list_append (res, copy);
  }
  ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ
print_parse_state (parse_state *ps)
{
  FILE *ಹೊರಹೋಗು = stderr;
  fprintf (ಹೊರಹೋಗು, "(size %zu depth %d rc %d)\n",
          ps->state_ವಸ್ತುಗಳು.ಒಟ್ಟು_ಗಾತ್ರ, ps->ಆಳ, ps->reference_ಎಣಿಕೆ);
  state_ಅಂಶ_print (ps->state_ವಸ್ತುಗಳು.ತಲೆ_elt, ಹೊರಹೋಗು, "");
  state_ಅಂಶ_print (ps->state_ವಸ್ತುಗಳು.ಬಾಲ_elt, ಹೊರಹೋಗು, "");
  ಆದರೆ (ps->derivs.ಒಟ್ಟು_ಗಾತ್ರ > 0)
    derivation_print (ps->derivs.ತಲೆ_elt, ಹೊರಹೋಗು, "");
  putc ('\n', ಹೊರಹೋಗು);
}
