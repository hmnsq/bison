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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "parse-simulation.h"

#அந்தர்பாவ <gl_linked_list.h>
#அந்தர்பாவ <gl_xlist.h>
#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "lssi.h"
#அந்தர்பாவ "nullable.h"

கட்டமைப்பு parse_state
{
  // Path of state-items the parser has traversed.
  கட்டமைப்பு si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    மாறிலி state_item *head_elt;
    மாறிலி state_item *tail_elt;
    size_t total_size;
  } state_பொருட்கள்;
  // List of derivations of the symbols.
  கட்டமைப்பு deriv_chunk
  {
    derivation_list contents;
    மாறிலி derivation *head_elt;
    மாறிலி derivation *tail_elt;
    size_t total_size;
  } derivs;
  கட்டமைப்பு parse_state *parent;
  முழு reference_மொத்தம்;
  // Incremented during productions, decremented during reductions.
  முழு depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


நிலையான வெறுமை
ps_si_prepend (parse_state *ps, மாறிலி state_item *si)
{
  கட்டமைப்பு si_chunk *sic = &ps->state_பொருட்கள்;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  எனில் (!sic->tail_elt)
    sic->tail_elt = si;
}

நிலையான வெறுமை
ps_si_append (parse_state *ps, மாறிலி state_item *si)
{
  கட்டமைப்பு si_chunk *sic = &ps->state_பொருட்கள்;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  எனில் (!sic->head_elt)
    sic->head_elt = si;
}

நிலையான வெறுமை
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  கட்டமைப்பு deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  எனில் (!dc->tail_elt)
    dc->tail_elt = d;
}

நிலையான வெறுமை
ps_derivs_append (parse_state *ps, derivation *d)
{
  கட்டமைப்பு deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  எனில் (!dc->head_elt)
    dc->head_elt = d;
}

நிலையான முழு allocs = 0;
நிலையான முழு frees = 0;

நிலையான parse_state *
empty_parse_state (வெறுமை)
{
  parse_state *res = xcalloc (1, அளவு *res);
  res->state_பொருட்கள்.contents
    = gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  திரும்பு res;
}

parse_state *
new_parse_state (மாறிலி state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  திரும்பு res;
}

நிலையான parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (அளவு *res);
  *res = *parent;
  res->state_பொருட்கள்.contents
    = gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_மொத்தம் = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  திரும்பு res;
}

bool
parse_state_derivation_completed (மாறிலி parse_state *ps)
{
  திரும்பு ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (மாறிலி parse_state *ps)
{
  திரும்பு (derivation *) ps->derivs.head_elt;
}

மாறிலி state_item *
parse_state_head (மாறிலி parse_state *ps)
{
  திரும்பு ps->state_பொருட்கள்.head_elt;
}

மாறிலி state_item *
parse_state_tail (மாறிலி parse_state *ps)
{
  திரும்பு ps->state_பொருட்கள்.tail_elt;
}

முழு
parse_state_length (மாறிலி parse_state *ps)
{
  திரும்பு ps->state_பொருட்கள்.total_size;
}

முழு
parse_state_depth (மாறிலி parse_state *ps)
{
  திரும்பு ps->depth;
}

வெறுமை
parse_state_retain (parse_state *ps)
{
  ++ps->reference_மொத்தம்;
}

வெறுமை
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

வெறுமை
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = வெற்று;
  க்கு (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_மொத்தம்;
      free_contents = (ps->reference_மொத்தம் == 1 && ps->free_contents_early)
        || (ps->reference_மொத்தம் == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      எனில் (free_contents)
        {
          எனில் (ps->state_பொருட்கள்.contents)
            gl_list_free (ps->state_பொருட்கள்.contents);
          எனில் (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      எனில் (ps->reference_மொத்தம் <= 0)
        {
          விடுவி (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (மாறிலி parse_state *ps, size_t max)
{
  மாறிலி கட்டமைப்பு si_chunk *sis = &ps->state_பொருட்கள்;
  திரும்பு ((state_item *) sis->head_elt - state_பொருட்கள் +
          (state_item *) sis->tail_elt - state_பொருட்கள் + sis->total_size) % max;
}

bool
parse_state_comparator (மாறிலி parse_state *ps1, மாறிலி parse_state *ps2)
{
  மாறிலி கட்டமைப்பு si_chunk *sis1 = &ps1->state_பொருட்கள்;
  மாறிலி கட்டமைப்பு si_chunk *sis2 = &ps2->state_பொருட்கள்;
  திரும்பு sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


வெறுமை
parse_state_completed_steps (மாறிலி parse_state *ps, முழு *shifts, முழு *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  மாறிலி parse_state *வேர்_ps = ps;
  போது (வேர்_ps->parent)
    வேர்_ps = வேர்_ps->parent;

  state_item_list sis = வேர்_ps->state_பொருட்கள்.contents;
  முழு மொத்தம் = 0;

  state_item *last = வெற்று;
  state_item *next = வெற்று;
  க்கு (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      எனில் (last && last->state == next->state)
        ++மொத்தம்;
      last = next;
    }
  *productions = மொத்தம்;
  *shifts = வேர்_ps->state_பொருட்கள்.total_size - மொத்தம்;
}

வகைவரை வெறுமை (*chunk_append_fn) (gl_list_t, மாறிலி வெறுமை *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
நிலையான வெறுமை
list_add_last (gl_list_t list, மாறிலி வெறுமை *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
நிலையான வெறுமை
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, முழு split, முழு n,
                        chunk_append_fn append_fn)
{
  முழு ret_index = 0;
  முழு ret_array = 0;
  க்கு (முழு i = 0; i < n; ++i)
    {
      மாறிலி வெறுமை *p = வெற்று;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      போது (gl_list_iterator_next (&it, &p, வெற்று))
        எனில் (p)
          {
            gl_list_t l = (gl_list_t) p;
            மாறிலி வெறுமை *si = வெற்று;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            போது (gl_list_iterator_next (&it2, &si, வெற்று))
              {
                எனில் (ret_index++ == split)
                  ++ret_array;
                எனில் (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

நிலையான parse_state_list
parse_state_list_new (வெறுமை)
{
  திரும்பு gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

நிலையான வெறுமை
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
நிலையான derivation_list
parser_எடு (parse_state *ps, முழு deriv_index,
            முழு si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  க்கு (முழு i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
  க்கு (parse_state *pn = ps; pn != வெற்று; pn = pn->parent)
    எனில் (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_பொருட்கள்.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    இல்லையெனில்
      {
        gl_list_add_first (chunks[1], pn->state_பொருட்கள்.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_பொருட்கள்.contents, வெற்று,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_பொருட்கள்.contents);
  ret->state_பொருட்கள்.total_size = s_size;
  எனில் (s_size > 0)
    {
      ret->state_பொருட்கள்.tail_elt = gl_list_get_at (ret->state_பொருட்கள்.contents,
                                                  s_size - 1);
      ret->state_பொருட்கள்.head_elt =
        gl_list_get_at (ret->state_பொருட்கள்.contents, 0);
    }
  இல்லையெனில்
    {
      ret->state_பொருட்கள்.tail_elt = வெற்று;
      ret->state_பொருட்கள்.head_elt = வெற்று;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  எனில் (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  இல்லையெனில்
    {
      ret->derivs.tail_elt = வெற்று;
      ret->derivs.head_elt = வெற்று;
    }
  க்கு (முழு i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  திரும்பு popped_derivs;
}

வெறுமை
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_பொருட்கள்.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_எடு (ps, si_size, deriv_size, temp);
  *sitems = temp->state_பொருட்கள்.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_பொருட்கள்.contents = வெற்று;
  temp->derivs.contents = வெற்று;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
நிலையான வெறுமை
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_எண் prev_sin = si - state_பொருட்கள்;
  க்கு (state_item_எண் sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_பொருட்கள்[sin].trans)
    {
      state_item *psi = &state_பொருட்கள்[prev_sin];
      குறியீடு_எண் sp = item_எண்_as_குறியீடு_எண் (*psi->item);
      எனில் (ISTOKEN (sp) || !nullable[sp - ntokens])
        நிறுத்து;

      state_item *nsi = &state_பொருட்கள்[sin];
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
  மாறிலி state_item *si = ps->state_பொருட்கள்.tail_elt;
  குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_எண் si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  எனில் (si_next < 0)
    திரும்பு result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_பொருட்கள்[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_பொருட்கள்[si_next], result);
  திரும்பு result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
நிலையான bool
compatible (குறியீடு_எண் sym1, குறியீடு_எண் sym2)
{
  எனில் (sym1 == sym2)
    திரும்பு true;
  எனில் (ISTOKEN (sym1) && ISVAR (sym2))
    திரும்பு bitset_test (FIRSTS (sym2), sym1);
  இல்லையெனில் எனில் (ISVAR (sym1) && ISTOKEN (sym2))
    திரும்பு bitset_test (FIRSTS (sym1), sym2);
  இல்லையெனில் எனில் (ISVAR (sym1) && ISVAR (sym2))
    திரும்பு !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  இல்லையெனில்
    திரும்பு false;
}

parse_state_list
simulate_production (parse_state *ps, குறியீடு_எண் compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  மாறிலி state_item *si = parse_state_tail (ps);
  எனில் (si->prods)
    {
      bitset_iterator biter;
      state_item_எண் sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_பொருட்கள்[sin];
          item_எண் *itm1 = next->item;
          எனில் (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            தொடர்;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          எனில் (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  திரும்பு result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, முழு rule_len, bitset குறியீடு_set)
{
  parse_state_list result = parse_state_list_new ();

  முழு s_size = ps->state_பொருட்கள்.total_size;
  முழு d_size = ps->derivs.total_size;
  எனில் (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_வேர் = empty_parse_state ();
  derivation_list popped_derivs =
    parser_எடு (ps, d_size - rule_len,
                s_size - rule_len - 1, new_வேர்);

  // update derivation
  state_item *si = (state_item *) ps->state_பொருட்கள்.tail_elt;
  மாறிலி rule *r = item_rule (si->item);
  குறியீடு_எண் lhs = r->lhs->எண்;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_வேர்->depth;
  ps_derivs_append (new_வேர், deriv);

  எனில் (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_வேர்->state_பொருட்கள்.tail_elt;
      ps_si_append (new_வேர், &state_பொருட்கள்[tail->trans]);
      parse_state_list_append (result, new_வேர்);
    }
  இல்லையெனில்
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      மாறிலி state_item *head = ps->state_பொருட்கள்.head_elt;
      state_item_list prev = lssi_reverse_production (head, குறியீடு_set);
      // TODO: better understand what causes this case.
      எனில் (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_வேர்);
          free_parse_state (new_வேர்);
        }
      இல்லையெனில்
        {
          state_item *psis = வெற்று;
          க்கு (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_வேர்);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              கட்டமைப்பு si_chunk *sis = &copy->state_பொருட்கள்;
              மாறிலி state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_பொருட்கள்[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  திரும்பு result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  மாறிலி state_item *head = ps->state_பொருட்கள்.head_elt;
  குறியீடு_எண் prepend_sym =
    item_எண்_as_குறியீடு_எண் (*(head->item - 1));
  bitset_iterator biter;
  state_item_எண் sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_பொருட்கள்[sin]);
    எனில் (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  திரும்பு res;
}

வெறுமை
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_பொருட்கள்.total_size, ps->depth, ps->reference_மொத்தம்);
  state_item_print (ps->state_பொருட்கள்.head_elt, out, "");
  state_item_print (ps->state_பொருட்கள்.tail_elt, out, "");
  எனில் (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
