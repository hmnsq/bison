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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "parse-simulation.h"

#അന്തര്ഭാവ <gl_linked_list.h>
#അന്തര്ഭാവ <gl_xlist.h>
#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "lssi.h"
#അന്തര്ഭാവ "nullable.h"

ഘടന parse_state
{
  // Path of state-items the parser has traversed.
  ഘടന si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    സ്ഥിരം state_item *head_elt;
    സ്ഥിരം state_item *tail_elt;
    size_t total_size;
  } state_ഇനങ്ങൾ;
  // List of derivations of the symbols.
  ഘടന deriv_chunk
  {
    derivation_list contents;
    സ്ഥിരം derivation *head_elt;
    സ്ഥിരം derivation *tail_elt;
    size_t total_size;
  } derivs;
  ഘടന parse_state *parent;
  പൂർണ്ണസംഖ്യ reference_എണ്ണം;
  // Incremented during productions, decremented during reductions.
  പൂർണ്ണസംഖ്യ depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


സ്ഥിതം ശൂന്യം
ps_si_prepend (parse_state *ps, സ്ഥിരം state_item *si)
{
  ഘടന si_chunk *sic = &ps->state_ഇനങ്ങൾ;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  എങ്കിൽ (!sic->tail_elt)
    sic->tail_elt = si;
}

സ്ഥിതം ശൂന്യം
ps_si_append (parse_state *ps, സ്ഥിരം state_item *si)
{
  ഘടന si_chunk *sic = &ps->state_ഇനങ്ങൾ;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  എങ്കിൽ (!sic->head_elt)
    sic->head_elt = si;
}

സ്ഥിതം ശൂന്യം
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  ഘടന deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  എങ്കിൽ (!dc->tail_elt)
    dc->tail_elt = d;
}

സ്ഥിതം ശൂന്യം
ps_derivs_append (parse_state *ps, derivation *d)
{
  ഘടന deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  എങ്കിൽ (!dc->head_elt)
    dc->head_elt = d;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ allocs = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ frees = 0;

സ്ഥിതം parse_state *
empty_parse_state (ശൂന്യം)
{
  parse_state *res = xcalloc (1, വലുപ്പം *res);
  res->state_ഇനങ്ങൾ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  തിരികെ res;
}

parse_state *
new_parse_state (സ്ഥിരം state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  തിരികെ res;
}

സ്ഥിതം parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (വലുപ്പം *res);
  *res = *parent;
  res->state_ഇനങ്ങൾ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_എണ്ണം = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  തിരികെ res;
}

bool
parse_state_derivation_completed (സ്ഥിരം parse_state *ps)
{
  തിരികെ ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (സ്ഥിരം parse_state *ps)
{
  തിരികെ (derivation *) ps->derivs.head_elt;
}

സ്ഥിരം state_item *
parse_state_head (സ്ഥിരം parse_state *ps)
{
  തിരികെ ps->state_ഇനങ്ങൾ.head_elt;
}

സ്ഥിരം state_item *
parse_state_tail (സ്ഥിരം parse_state *ps)
{
  തിരികെ ps->state_ഇനങ്ങൾ.tail_elt;
}

പൂർണ്ണസംഖ്യ
parse_state_length (സ്ഥിരം parse_state *ps)
{
  തിരികെ ps->state_ഇനങ്ങൾ.total_size;
}

പൂർണ്ണസംഖ്യ
parse_state_depth (സ്ഥിരം parse_state *ps)
{
  തിരികെ ps->depth;
}

ശൂന്യം
parse_state_retain (parse_state *ps)
{
  ++ps->reference_എണ്ണം;
}

ശൂന്യം
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

ശൂന്യം
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = ശൂന്യ;
  വേണ്ടി (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_എണ്ണം;
      free_contents = (ps->reference_എണ്ണം == 1 && ps->free_contents_early)
        || (ps->reference_എണ്ണം == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      എങ്കിൽ (free_contents)
        {
          എങ്കിൽ (ps->state_ഇനങ്ങൾ.contents)
            gl_list_free (ps->state_ഇനങ്ങൾ.contents);
          എങ്കിൽ (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      എങ്കിൽ (ps->reference_എണ്ണം <= 0)
        {
          സ്വതന്ത്രം (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (സ്ഥിരം parse_state *ps, size_t max)
{
  സ്ഥിരം ഘടന si_chunk *sis = &ps->state_ഇനങ്ങൾ;
  തിരികെ ((state_item *) sis->head_elt - state_ഇനങ്ങൾ +
          (state_item *) sis->tail_elt - state_ഇനങ്ങൾ + sis->total_size) % max;
}

bool
parse_state_comparator (സ്ഥിരം parse_state *ps1, സ്ഥിരം parse_state *ps2)
{
  സ്ഥിരം ഘടന si_chunk *sis1 = &ps1->state_ഇനങ്ങൾ;
  സ്ഥിരം ഘടന si_chunk *sis2 = &ps2->state_ഇനങ്ങൾ;
  തിരികെ sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


ശൂന്യം
parse_state_completed_steps (സ്ഥിരം parse_state *ps, പൂർണ്ണസംഖ്യ *shifts, പൂർണ്ണസംഖ്യ *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  സ്ഥിരം parse_state *വേര്_ps = ps;
  ആകുമ്പോൾ (വേര്_ps->parent)
    വേര്_ps = വേര്_ps->parent;

  state_item_list sis = വേര്_ps->state_ഇനങ്ങൾ.contents;
  പൂർണ്ണസംഖ്യ എണ്ണം = 0;

  state_item *last = ശൂന്യ;
  state_item *next = ശൂന്യ;
  വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      എങ്കിൽ (last && last->state == next->state)
        ++എണ്ണം;
      last = next;
    }
  *productions = എണ്ണം;
  *shifts = വേര്_ps->state_ഇനങ്ങൾ.total_size - എണ്ണം;
}

തരനിർവചനം ശൂന്യം (*chunk_append_fn) (gl_list_t, സ്ഥിരം ശൂന്യം *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
സ്ഥിതം ശൂന്യം
list_add_last (gl_list_t list, സ്ഥിരം ശൂന്യം *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
സ്ഥിതം ശൂന്യം
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, പൂർണ്ണസംഖ്യ split, പൂർണ്ണസംഖ്യ n,
                        chunk_append_fn append_fn)
{
  പൂർണ്ണസംഖ്യ ret_index = 0;
  പൂർണ്ണസംഖ്യ ret_array = 0;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; ++i)
    {
      സ്ഥിരം ശൂന്യം *p = ശൂന്യ;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      ആകുമ്പോൾ (gl_list_iterator_next (&it, &p, ശൂന്യ))
        എങ്കിൽ (p)
          {
            gl_list_t l = (gl_list_t) p;
            സ്ഥിരം ശൂന്യം *si = ശൂന്യ;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            ആകുമ്പോൾ (gl_list_iterator_next (&it2, &si, ശൂന്യ))
              {
                എങ്കിൽ (ret_index++ == split)
                  ++ret_array;
                എങ്കിൽ (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

സ്ഥിതം parse_state_list
parse_state_list_new (ശൂന്യം)
{
  തിരികെ gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

സ്ഥിതം ശൂന്യം
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
സ്ഥിതം derivation_list
parser_എടുക്കുക (parse_state *ps, പൂർണ്ണസംഖ്യ deriv_index,
            പൂർണ്ണസംഖ്യ si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
  വേണ്ടി (parse_state *pn = ps; pn != ശൂന്യ; pn = pn->parent)
    എങ്കിൽ (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_ഇനങ്ങൾ.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    അല്ലെങ്കിൽ
      {
        gl_list_add_first (chunks[1], pn->state_ഇനങ്ങൾ.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_ഇനങ്ങൾ.contents, ശൂന്യ,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_ഇനങ്ങൾ.contents);
  ret->state_ഇനങ്ങൾ.total_size = s_size;
  എങ്കിൽ (s_size > 0)
    {
      ret->state_ഇനങ്ങൾ.tail_elt = gl_list_get_at (ret->state_ഇനങ്ങൾ.contents,
                                                  s_size - 1);
      ret->state_ഇനങ്ങൾ.head_elt =
        gl_list_get_at (ret->state_ഇനങ്ങൾ.contents, 0);
    }
  അല്ലെങ്കിൽ
    {
      ret->state_ഇനങ്ങൾ.tail_elt = ശൂന്യ;
      ret->state_ഇനങ്ങൾ.head_elt = ശൂന്യ;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  എങ്കിൽ (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  അല്ലെങ്കിൽ
    {
      ret->derivs.tail_elt = ശൂന്യ;
      ret->derivs.head_elt = ശൂന്യ;
    }
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  തിരികെ popped_derivs;
}

ശൂന്യം
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_ഇനങ്ങൾ.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_എടുക്കുക (ps, si_size, deriv_size, temp);
  *sitems = temp->state_ഇനങ്ങൾ.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_ഇനങ്ങൾ.contents = ശൂന്യ;
  temp->derivs.contents = ശൂന്യ;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
സ്ഥിതം ശൂന്യം
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_സംഖ്യ prev_sin = si - state_ഇനങ്ങൾ;
  വേണ്ടി (state_item_സംഖ്യ sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_ഇനങ്ങൾ[sin].trans)
    {
      state_item *psi = &state_ഇനങ്ങൾ[prev_sin];
      ചിഹ്നം_സംഖ്യ sp = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*psi->item);
      എങ്കിൽ (ISTOKEN (sp) || !nullable[sp - ntokens])
        നിർത്തുക;

      state_item *nsi = &state_ഇനങ്ങൾ[sin];
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
  സ്ഥിരം state_item *si = ps->state_ഇനങ്ങൾ.tail_elt;
  ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_സംഖ്യ si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  എങ്കിൽ (si_next < 0)
    തിരികെ result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_ഇനങ്ങൾ[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_ഇനങ്ങൾ[si_next], result);
  തിരികെ result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
സ്ഥിതം bool
compatible (ചിഹ്നം_സംഖ്യ sym1, ചിഹ്നം_സംഖ്യ sym2)
{
  എങ്കിൽ (sym1 == sym2)
    തിരികെ true;
  എങ്കിൽ (ISTOKEN (sym1) && ISVAR (sym2))
    തിരികെ bitset_test (FIRSTS (sym2), sym1);
  അല്ലെങ്കിൽ എങ്കിൽ (ISVAR (sym1) && ISTOKEN (sym2))
    തിരികെ bitset_test (FIRSTS (sym1), sym2);
  അല്ലെങ്കിൽ എങ്കിൽ (ISVAR (sym1) && ISVAR (sym2))
    തിരികെ !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  അല്ലെങ്കിൽ
    തിരികെ false;
}

parse_state_list
simulate_production (parse_state *ps, ചിഹ്നം_സംഖ്യ compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  സ്ഥിരം state_item *si = parse_state_tail (ps);
  എങ്കിൽ (si->prods)
    {
      bitset_iterator biter;
      state_item_സംഖ്യ sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_ഇനങ്ങൾ[sin];
          item_സംഖ്യ *itm1 = next->item;
          എങ്കിൽ (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            തുടരുക;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          എങ്കിൽ (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  തിരികെ result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, പൂർണ്ണസംഖ്യ rule_len, bitset ചിഹ്നം_set)
{
  parse_state_list result = parse_state_list_new ();

  പൂർണ്ണസംഖ്യ s_size = ps->state_ഇനങ്ങൾ.total_size;
  പൂർണ്ണസംഖ്യ d_size = ps->derivs.total_size;
  എങ്കിൽ (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_വേര് = empty_parse_state ();
  derivation_list popped_derivs =
    parser_എടുക്കുക (ps, d_size - rule_len,
                s_size - rule_len - 1, new_വേര്);

  // update derivation
  state_item *si = (state_item *) ps->state_ഇനങ്ങൾ.tail_elt;
  സ്ഥിരം rule *r = item_rule (si->item);
  ചിഹ്നം_സംഖ്യ lhs = r->lhs->സംഖ്യ;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_വേര്->depth;
  ps_derivs_append (new_വേര്, deriv);

  എങ്കിൽ (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_വേര്->state_ഇനങ്ങൾ.tail_elt;
      ps_si_append (new_വേര്, &state_ഇനങ്ങൾ[tail->trans]);
      parse_state_list_append (result, new_വേര്);
    }
  അല്ലെങ്കിൽ
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      സ്ഥിരം state_item *head = ps->state_ഇനങ്ങൾ.head_elt;
      state_item_list prev = lssi_reverse_production (head, ചിഹ്നം_set);
      // TODO: better understand what causes this case.
      എങ്കിൽ (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_വേര്);
          free_parse_state (new_വേര്);
        }
      അല്ലെങ്കിൽ
        {
          state_item *psis = ശൂന്യ;
          വേണ്ടി (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_വേര്);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              ഘടന si_chunk *sis = &copy->state_ഇനങ്ങൾ;
              സ്ഥിരം state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_ഇനങ്ങൾ[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  തിരികെ result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  സ്ഥിരം state_item *head = ps->state_ഇനങ്ങൾ.head_elt;
  ചിഹ്നം_സംഖ്യ prepend_sym =
    item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*(head->item - 1));
  bitset_iterator biter;
  state_item_സംഖ്യ sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_ഇനങ്ങൾ[sin]);
    എങ്കിൽ (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  തിരികെ res;
}

ശൂന്യം
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_ഇനങ്ങൾ.total_size, ps->depth, ps->reference_എണ്ണം);
  state_item_print (ps->state_ഇനങ്ങൾ.head_elt, out, "");
  state_item_print (ps->state_ഇനങ്ങൾ.tail_elt, out, "");
  എങ്കിൽ (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
