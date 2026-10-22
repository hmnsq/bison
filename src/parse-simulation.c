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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "parse-simulation.h"

#ଅନ୍ତର୍ଭାଵ <gl_linked_list.h>
#ଅନ୍ତର୍ଭାଵ <gl_xlist.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "lssi.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"

ସଂରଚନା parse_state
{
  // Path of state-items the parser has traversed.
  ସଂରଚନା si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ଅଚଳ state_item *head_elt;
    ଅଚଳ state_item *tail_elt;
    size_t total_size;
  } state_ବସ୍ତୁଗୁଡ଼ିକ;
  // List of derivations of the symbols.
  ସଂରଚନା deriv_chunk
  {
    derivation_list contents;
    ଅଚଳ derivation *head_elt;
    ଅଚଳ derivation *tail_elt;
    size_t total_size;
  } derivs;
  ସଂରଚନା parse_state *parent;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା reference_ଗଣନା;
  // Incremented during productions, decremented during reductions.
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ସ୍ଥିର ଶୂନ୍ୟ
ps_si_prepend (parse_state *ps, ଅଚଳ state_item *si)
{
  ସଂରଚନା si_chunk *sic = &ps->state_ବସ୍ତୁଗୁଡ଼ିକ;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  ଯଦି (!sic->tail_elt)
    sic->tail_elt = si;
}

ସ୍ଥିର ଶୂନ୍ୟ
ps_si_append (parse_state *ps, ଅଚଳ state_item *si)
{
  ସଂରଚନା si_chunk *sic = &ps->state_ବସ୍ତୁଗୁଡ଼ିକ;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  ଯଦି (!sic->head_elt)
    sic->head_elt = si;
}

ସ୍ଥିର ଶୂନ୍ୟ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  ସଂରଚନା deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  ଯଦି (!dc->tail_elt)
    dc->tail_elt = d;
}

ସ୍ଥିର ଶୂନ୍ୟ
ps_derivs_append (parse_state *ps, derivation *d)
{
  ସଂରଚନା deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  ଯଦି (!dc->head_elt)
    dc->head_elt = d;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocs = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା frees = 0;

ସ୍ଥିର parse_state *
empty_parse_state (ଶୂନ୍ୟ)
{
  parse_state *res = xcalloc (1, ଆକାର *res);
  res->state_ବସ୍ତୁଗୁଡ଼ିକ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  ଫେରନ୍ତୁ res;
}

parse_state *
new_parse_state (ଅଚଳ state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (ଆକାର *res);
  *res = *parent;
  res->state_ବସ୍ତୁଗୁଡ଼ିକ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_ଗଣନା = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  ଫେରନ୍ତୁ res;
}

bool
parse_state_derivation_completed (ଅଚଳ parse_state *ps)
{
  ଫେରନ୍ତୁ ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (ଅଚଳ parse_state *ps)
{
  ଫେରନ୍ତୁ (derivation *) ps->derivs.head_elt;
}

ଅଚଳ state_item *
parse_state_head (ଅଚଳ parse_state *ps)
{
  ଫେରନ୍ତୁ ps->state_ବସ୍ତୁଗୁଡ଼ିକ.head_elt;
}

ଅଚଳ state_item *
parse_state_tail (ଅଚଳ parse_state *ps)
{
  ଫେରନ୍ତୁ ps->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_state_length (ଅଚଳ parse_state *ps)
{
  ଫେରନ୍ତୁ ps->state_ବସ୍ତୁଗୁଡ଼ିକ.total_size;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_state_depth (ଅଚଳ parse_state *ps)
{
  ଫେରନ୍ତୁ ps->depth;
}

ଶୂନ୍ୟ
parse_state_retain (parse_state *ps)
{
  ++ps->reference_ଗଣନା;
}

ଶୂନ୍ୟ
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

ଶୂନ୍ୟ
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = ଖାଲି;
  ପାଇଁ (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_ଗଣନା;
      free_contents = (ps->reference_ଗଣନା == 1 && ps->free_contents_early)
        || (ps->reference_ଗଣନା == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      ଯଦି (free_contents)
        {
          ଯଦି (ps->state_ବସ୍ତୁଗୁଡ଼ିକ.contents)
            gl_list_free (ps->state_ବସ୍ତୁଗୁଡ଼ିକ.contents);
          ଯଦି (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      ଯଦି (ps->reference_ଗଣନା <= 0)
        {
          ମୁକ୍ତ (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ଅଚଳ parse_state *ps, size_t max)
{
  ଅଚଳ ସଂରଚନା si_chunk *sis = &ps->state_ବସ୍ତୁଗୁଡ଼ିକ;
  ଫେରନ୍ତୁ ((state_item *) sis->head_elt - state_ବସ୍ତୁଗୁଡ଼ିକ +
          (state_item *) sis->tail_elt - state_ବସ୍ତୁଗୁଡ଼ିକ + sis->total_size) % max;
}

bool
parse_state_comparator (ଅଚଳ parse_state *ps1, ଅଚଳ parse_state *ps2)
{
  ଅଚଳ ସଂରଚନା si_chunk *sis1 = &ps1->state_ବସ୍ତୁଗୁଡ଼ିକ;
  ଅଚଳ ସଂରଚନା si_chunk *sis2 = &ps2->state_ବସ୍ତୁଗୁଡ଼ିକ;
  ଫେରନ୍ତୁ sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


ଶୂନ୍ୟ
parse_state_completed_steps (ଅଚଳ parse_state *ps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *shifts, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ଅଚଳ parse_state *ମୂଳ_ps = ps;
  ଯେତେବେଳେ (ମୂଳ_ps->parent)
    ମୂଳ_ps = ମୂଳ_ps->parent;

  state_item_list sis = ମୂଳ_ps->state_ବସ୍ତୁଗୁଡ଼ିକ.contents;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

  state_item *last = ଖାଲି;
  state_item *next = ଖାଲି;
  ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      ଯଦି (last && last->state == next->state)
        ++ଗଣନା_2;
      last = next;
    }
  *productions = ଗଣନା_2;
  *shifts = ମୂଳ_ps->state_ବସ୍ତୁଗୁଡ଼ିକ.total_size - ଗଣନା_2;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*chunk_append_fn) (gl_list_t, ଅଚଳ ଶୂନ୍ୟ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ସ୍ଥିର ଶୂନ୍ୟ
list_add_last (gl_list_t list, ଅଚଳ ଶୂନ୍ୟ *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ସ୍ଥିର ଶୂନ୍ୟ
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା split, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n,
                        chunk_append_fn append_fn)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_index = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_array = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; ++i)
    {
      ଅଚଳ ଶୂନ୍ୟ *p = ଖାଲି;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      ଯେତେବେଳେ (gl_list_iterator_next (&it, &p, ଖାଲି))
        ଯଦି (p)
          {
            gl_list_t l = (gl_list_t) p;
            ଅଚଳ ଶୂନ୍ୟ *si = ଖାଲି;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            ଯେତେବେଳେ (gl_list_iterator_next (&it2, &si, ଖାଲି))
              {
                ଯଦି (ret_index++ == split)
                  ++ret_array;
                ଯଦି (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

ସ୍ଥିର parse_state_list
parse_state_list_new (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ସ୍ଥିର derivation_list
parser_କାଢ଼ (parse_state *ps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା deriv_index,
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
  ପାଇଁ (parse_state *pn = ps; pn != ଖାଲି; pn = pn->parent)
    ଯଦି (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_ବସ୍ତୁଗୁଡ଼ିକ.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    ଅନ୍ୟଥା
      {
        gl_list_add_first (chunks[1], pn->state_ବସ୍ତୁଗୁଡ଼ିକ.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_ବସ୍ତୁଗୁଡ଼ିକ.contents, ଖାଲି,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_ବସ୍ତୁଗୁଡ଼ିକ.contents);
  ret->state_ବସ୍ତୁଗୁଡ଼ିକ.total_size = s_size;
  ଯଦି (s_size > 0)
    {
      ret->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt = gl_list_get_at (ret->state_ବସ୍ତୁଗୁଡ଼ିକ.contents,
                                                  s_size - 1);
      ret->state_ବସ୍ତୁଗୁଡ଼ିକ.head_elt =
        gl_list_get_at (ret->state_ବସ୍ତୁଗୁଡ଼ିକ.contents, 0);
    }
  ଅନ୍ୟଥା
    {
      ret->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt = ଖାଲି;
      ret->state_ବସ୍ତୁଗୁଡ଼ିକ.head_elt = ଖାଲି;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  ଯଦି (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  ଅନ୍ୟଥା
    {
      ret->derivs.tail_elt = ଖାଲି;
      ret->derivs.head_elt = ଖାଲି;
    }
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  ଫେରନ୍ତୁ popped_derivs;
}

ଶୂନ୍ୟ
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_ବସ୍ତୁଗୁଡ଼ିକ.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_କାଢ଼ (ps, si_size, deriv_size, temp);
  *sitems = temp->state_ବସ୍ତୁଗୁଡ଼ିକ.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_ବସ୍ତୁଗୁଡ଼ିକ.contents = ଖାଲି;
  temp->derivs.contents = ଖାଲି;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ସ୍ଥିର ଶୂନ୍ୟ
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_ସଂଖ୍ୟା prev_sin = si - state_ବସ୍ତୁଗୁଡ଼ିକ;
  ପାଇଁ (state_item_ସଂଖ୍ୟା sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_ବସ୍ତୁଗୁଡ଼ିକ[sin].trans)
    {
      state_item *psi = &state_ବସ୍ତୁଗୁଡ଼ିକ[prev_sin];
      ପ୍ରତୀକ_ସଂଖ୍ୟା sp = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*psi->item);
      ଯଦି (ISTOKEN (sp) || !nullable[sp - ntokens])
        ଭାଙ୍ଗ;

      state_item *nsi = &state_ବସ୍ତୁଗୁଡ଼ିକ[sin];
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
  ଅଚଳ state_item *si = ps->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt;
  ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_ସଂଖ୍ୟା si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  ଯଦି (si_next < 0)
    ଫେରନ୍ତୁ result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_ବସ୍ତୁଗୁଡ଼ିକ[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_ବସ୍ତୁଗୁଡ଼ିକ[si_next], result);
  ଫେରନ୍ତୁ result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ସ୍ଥିର bool
compatible (ପ୍ରତୀକ_ସଂଖ୍ୟା sym1, ପ୍ରତୀକ_ସଂଖ୍ୟା sym2)
{
  ଯଦି (sym1 == sym2)
    ଫେରନ୍ତୁ true;
  ଯଦି (ISTOKEN (sym1) && ISVAR (sym2))
    ଫେରନ୍ତୁ bitset_test (FIRSTS (sym2), sym1);
  ଅନ୍ୟଥା ଯଦି (ISVAR (sym1) && ISTOKEN (sym2))
    ଫେରନ୍ତୁ bitset_test (FIRSTS (sym1), sym2);
  ଅନ୍ୟଥା ଯଦି (ISVAR (sym1) && ISVAR (sym2))
    ଫେରନ୍ତୁ !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ false;
}

parse_state_list
simulate_production (parse_state *ps, ପ୍ରତୀକ_ସଂଖ୍ୟା compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  ଅଚଳ state_item *si = parse_state_tail (ps);
  ଯଦି (si->prods)
    {
      bitset_iterator biter;
      state_item_ସଂଖ୍ୟା sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_ବସ୍ତୁଗୁଡ଼ିକ[sin];
          item_ସଂଖ୍ୟା *itm1 = next->item;
          ଯଦି (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            ଜାରି;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          ଯଦି (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  ଫେରନ୍ତୁ result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_len, bitset ପ୍ରତୀକ_set)
{
  parse_state_list result = parse_state_list_new ();

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା s_size = ps->state_ବସ୍ତୁଗୁଡ଼ିକ.total_size;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା d_size = ps->derivs.total_size;
  ଯଦି (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_ମୂଳ = empty_parse_state ();
  derivation_list popped_derivs =
    parser_କାଢ଼ (ps, d_size - rule_len,
                s_size - rule_len - 1, new_ମୂଳ);

  // update derivation
  state_item *si = (state_item *) ps->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt;
  ଅଚଳ rule *r = item_rule (si->item);
  ପ୍ରତୀକ_ସଂଖ୍ୟା lhs = r->lhs->ସଂଖ୍ୟା;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_ମୂଳ->depth;
  ps_derivs_append (new_ମୂଳ, deriv);

  ଯଦି (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_ମୂଳ->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt;
      ps_si_append (new_ମୂଳ, &state_ବସ୍ତୁଗୁଡ଼ିକ[tail->trans]);
      parse_state_list_append (result, new_ମୂଳ);
    }
  ଅନ୍ୟଥା
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ଅଚଳ state_item *head = ps->state_ବସ୍ତୁଗୁଡ଼ିକ.head_elt;
      state_item_list prev = lssi_reverse_production (head, ପ୍ରତୀକ_set);
      // TODO: better understand what causes this case.
      ଯଦି (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_ମୂଳ);
          free_parse_state (new_ମୂଳ);
        }
      ଅନ୍ୟଥା
        {
          state_item *psis = ଖାଲି;
          ପାଇଁ (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_ମୂଳ);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              ସଂରଚନା si_chunk *sis = &copy->state_ବସ୍ତୁଗୁଡ଼ିକ;
              ଅଚଳ state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_ବସ୍ତୁଗୁଡ଼ିକ[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  ଫେରନ୍ତୁ result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ଅଚଳ state_item *head = ps->state_ବସ୍ତୁଗୁଡ଼ିକ.head_elt;
  ପ୍ରତୀକ_ସଂଖ୍ୟା prepend_sym =
    item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*(head->item - 1));
  bitset_iterator biter;
  state_item_ସଂଖ୍ୟା sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_ବସ୍ତୁଗୁଡ଼ିକ[sin]);
    ଯଦି (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_ବସ୍ତୁଗୁଡ଼ିକ.total_size, ps->depth, ps->reference_ଗଣନା);
  state_item_print (ps->state_ବସ୍ତୁଗୁଡ଼ିକ.head_elt, out, "");
  state_item_print (ps->state_ବସ୍ତୁଗୁଡ଼ିକ.tail_elt, out, "");
  ଯଦି (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
