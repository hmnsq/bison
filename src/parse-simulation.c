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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "parse-simulation.h"

#અન્તર્ભાવ <gl_linked_list.h>
#અન્તર્ભાવ <gl_xlist.h>
#અન્તર્ભાવ <stdlib.h>

#અન્તર્ભાવ "lssi.h"
#અન્તર્ભાવ "nullable.h"

સંરચના parse_state
{
  // Path of state-items the parser has traversed.
  સંરચના si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    અચલ state_item *head_elt;
    અચલ state_item *tail_elt;
    size_t total_size;
  } state_વસ્તુઓ;
  // List of derivations of the symbols.
  સંરચના deriv_chunk
  {
    derivation_list contents;
    અચલ derivation *head_elt;
    અચલ derivation *tail_elt;
    size_t total_size;
  } derivs;
  સંરચના parse_state *parent;
  પૂર્ણાંક reference_ગણતરી;
  // Incremented during productions, decremented during reductions.
  પૂર્ણાંક depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


સ્થિર શૂન્ય
ps_si_prepend (parse_state *ps, અચલ state_item *si)
{
  સંરચના si_chunk *sic = &ps->state_વસ્તુઓ;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  જો (!sic->tail_elt)
    sic->tail_elt = si;
}

સ્થિર શૂન્ય
ps_si_append (parse_state *ps, અચલ state_item *si)
{
  સંરચના si_chunk *sic = &ps->state_વસ્તુઓ;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  જો (!sic->head_elt)
    sic->head_elt = si;
}

સ્થિર શૂન્ય
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  સંરચના deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  જો (!dc->tail_elt)
    dc->tail_elt = d;
}

સ્થિર શૂન્ય
ps_derivs_append (parse_state *ps, derivation *d)
{
  સંરચના deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  જો (!dc->head_elt)
    dc->head_elt = d;
}

સ્થિર પૂર્ણાંક allocs = 0;
સ્થિર પૂર્ણાંક frees = 0;

સ્થિર parse_state *
empty_parse_state (શૂન્ય)
{
  parse_state *res = xcalloc (1, કદ *res);
  res->state_વસ્તુઓ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  પાછા res;
}

parse_state *
new_parse_state (અચલ state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  પાછા res;
}

સ્થિર parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (કદ *res);
  *res = *parent;
  res->state_વસ્તુઓ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_ગણતરી = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  પાછા res;
}

bool
parse_state_derivation_completed (અચલ parse_state *ps)
{
  પાછા ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (અચલ parse_state *ps)
{
  પાછા (derivation *) ps->derivs.head_elt;
}

અચલ state_item *
parse_state_head (અચલ parse_state *ps)
{
  પાછા ps->state_વસ્તુઓ.head_elt;
}

અચલ state_item *
parse_state_tail (અચલ parse_state *ps)
{
  પાછા ps->state_વસ્તુઓ.tail_elt;
}

પૂર્ણાંક
parse_state_length (અચલ parse_state *ps)
{
  પાછા ps->state_વસ્તુઓ.total_size;
}

પૂર્ણાંક
parse_state_depth (અચલ parse_state *ps)
{
  પાછા ps->depth;
}

શૂન્ય
parse_state_retain (parse_state *ps)
{
  ++ps->reference_ગણતરી;
}

શૂન્ય
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

શૂન્ય
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = ખાલી;
  માટે (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_ગણતરી;
      free_contents = (ps->reference_ગણતરી == 1 && ps->free_contents_early)
        || (ps->reference_ગણતરી == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      જો (free_contents)
        {
          જો (ps->state_વસ્તુઓ.contents)
            gl_list_free (ps->state_વસ્તુઓ.contents);
          જો (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      જો (ps->reference_ગણતરી <= 0)
        {
          મુક્ત (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (અચલ parse_state *ps, size_t max)
{
  અચલ સંરચના si_chunk *sis = &ps->state_વસ્તુઓ;
  પાછા ((state_item *) sis->head_elt - state_વસ્તુઓ +
          (state_item *) sis->tail_elt - state_વસ્તુઓ + sis->total_size) % max;
}

bool
parse_state_comparator (અચલ parse_state *ps1, અચલ parse_state *ps2)
{
  અચલ સંરચના si_chunk *sis1 = &ps1->state_વસ્તુઓ;
  અચલ સંરચના si_chunk *sis2 = &ps2->state_વસ્તુઓ;
  પાછા sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


શૂન્ય
parse_state_completed_steps (અચલ parse_state *ps, પૂર્ણાંક *shifts, પૂર્ણાંક *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  અચલ parse_state *મૂળ_ps = ps;
  જ્યારે (મૂળ_ps->parent)
    મૂળ_ps = મૂળ_ps->parent;

  state_item_list sis = મૂળ_ps->state_વસ્તુઓ.contents;
  પૂર્ણાંક ગણતરી = 0;

  state_item *last = ખાલી;
  state_item *next = ખાલી;
  માટે (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      જો (last && last->state == next->state)
        ++ગણતરી;
      last = next;
    }
  *productions = ગણતરી;
  *shifts = મૂળ_ps->state_વસ્તુઓ.total_size - ગણતરી;
}

પ્રકારવ્યાખ્યા શૂન્ય (*chunk_append_fn) (gl_list_t, અચલ શૂન્ય *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
સ્થિર શૂન્ય
list_add_last (gl_list_t list, અચલ શૂન્ય *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
સ્થિર શૂન્ય
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, પૂર્ણાંક split, પૂર્ણાંક n,
                        chunk_append_fn append_fn)
{
  પૂર્ણાંક ret_index = 0;
  પૂર્ણાંક ret_array = 0;
  માટે (પૂર્ણાંક i = 0; i < n; ++i)
    {
      અચલ શૂન્ય *p = ખાલી;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      જ્યારે (gl_list_iterator_next (&it, &p, ખાલી))
        જો (p)
          {
            gl_list_t l = (gl_list_t) p;
            અચલ શૂન્ય *si = ખાલી;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            જ્યારે (gl_list_iterator_next (&it2, &si, ખાલી))
              {
                જો (ret_index++ == split)
                  ++ret_array;
                જો (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

સ્થિર parse_state_list
parse_state_list_new (શૂન્ય)
{
  પાછા gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

સ્થિર શૂન્ય
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
સ્થિર derivation_list
parser_કાઢો (parse_state *ps, પૂર્ણાંક deriv_index,
            પૂર્ણાંક si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  માટે (પૂર્ણાંક i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
  માટે (parse_state *pn = ps; pn != ખાલી; pn = pn->parent)
    જો (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_વસ્તુઓ.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    નહીંતર
      {
        gl_list_add_first (chunks[1], pn->state_વસ્તુઓ.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_વસ્તુઓ.contents, ખાલી,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_વસ્તુઓ.contents);
  ret->state_વસ્તુઓ.total_size = s_size;
  જો (s_size > 0)
    {
      ret->state_વસ્તુઓ.tail_elt = gl_list_get_at (ret->state_વસ્તુઓ.contents,
                                                  s_size - 1);
      ret->state_વસ્તુઓ.head_elt =
        gl_list_get_at (ret->state_વસ્તુઓ.contents, 0);
    }
  નહીંતર
    {
      ret->state_વસ્તુઓ.tail_elt = ખાલી;
      ret->state_વસ્તુઓ.head_elt = ખાલી;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  જો (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  નહીંતર
    {
      ret->derivs.tail_elt = ખાલી;
      ret->derivs.head_elt = ખાલી;
    }
  માટે (પૂર્ણાંક i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  પાછા popped_derivs;
}

શૂન્ય
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_વસ્તુઓ.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_કાઢો (ps, si_size, deriv_size, temp);
  *sitems = temp->state_વસ્તુઓ.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_વસ્તુઓ.contents = ખાલી;
  temp->derivs.contents = ખાલી;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
સ્થિર શૂન્ય
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_સંખ્યા prev_sin = si - state_વસ્તુઓ;
  માટે (state_item_સંખ્યા sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_વસ્તુઓ[sin].trans)
    {
      state_item *psi = &state_વસ્તુઓ[prev_sin];
      પ્રતીક_સંખ્યા sp = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*psi->item);
      જો (ISTOKEN (sp) || !nullable[sp - ntokens])
        તોડો;

      state_item *nsi = &state_વસ્તુઓ[sin];
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
  અચલ state_item *si = ps->state_વસ્તુઓ.tail_elt;
  પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_સંખ્યા si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  જો (si_next < 0)
    પાછા result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_વસ્તુઓ[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_વસ્તુઓ[si_next], result);
  પાછા result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
સ્થિર bool
compatible (પ્રતીક_સંખ્યા sym1, પ્રતીક_સંખ્યા sym2)
{
  જો (sym1 == sym2)
    પાછા true;
  જો (ISTOKEN (sym1) && ISVAR (sym2))
    પાછા bitset_test (FIRSTS (sym2), sym1);
  નહીંતર જો (ISVAR (sym1) && ISTOKEN (sym2))
    પાછા bitset_test (FIRSTS (sym1), sym2);
  નહીંતર જો (ISVAR (sym1) && ISVAR (sym2))
    પાછા !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  નહીંતર
    પાછા false;
}

parse_state_list
simulate_production (parse_state *ps, પ્રતીક_સંખ્યા compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  અચલ state_item *si = parse_state_tail (ps);
  જો (si->prods)
    {
      bitset_iterator biter;
      state_item_સંખ્યા sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_વસ્તુઓ[sin];
          item_સંખ્યા *itm1 = next->item;
          જો (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            ચાલુ;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          જો (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  પાછા result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, પૂર્ણાંક rule_len, bitset પ્રતીક_set)
{
  parse_state_list result = parse_state_list_new ();

  પૂર્ણાંક s_size = ps->state_વસ્તુઓ.total_size;
  પૂર્ણાંક d_size = ps->derivs.total_size;
  જો (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_મૂળ = empty_parse_state ();
  derivation_list popped_derivs =
    parser_કાઢો (ps, d_size - rule_len,
                s_size - rule_len - 1, new_મૂળ);

  // update derivation
  state_item *si = (state_item *) ps->state_વસ્તુઓ.tail_elt;
  અચલ rule *r = item_rule (si->item);
  પ્રતીક_સંખ્યા lhs = r->lhs->સંખ્યા;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_મૂળ->depth;
  ps_derivs_append (new_મૂળ, deriv);

  જો (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_મૂળ->state_વસ્તુઓ.tail_elt;
      ps_si_append (new_મૂળ, &state_વસ્તુઓ[tail->trans]);
      parse_state_list_append (result, new_મૂળ);
    }
  નહીંતર
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      અચલ state_item *head = ps->state_વસ્તુઓ.head_elt;
      state_item_list prev = lssi_reverse_production (head, પ્રતીક_set);
      // TODO: better understand what causes this case.
      જો (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_મૂળ);
          free_parse_state (new_મૂળ);
        }
      નહીંતર
        {
          state_item *psis = ખાલી;
          માટે (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_મૂળ);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              સંરચના si_chunk *sis = &copy->state_વસ્તુઓ;
              અચલ state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_વસ્તુઓ[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  પાછા result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  અચલ state_item *head = ps->state_વસ્તુઓ.head_elt;
  પ્રતીક_સંખ્યા prepend_sym =
    item_સંખ્યા_as_પ્રતીક_સંખ્યા (*(head->item - 1));
  bitset_iterator biter;
  state_item_સંખ્યા sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_વસ્તુઓ[sin]);
    જો (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  પાછા res;
}

શૂન્ય
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_વસ્તુઓ.total_size, ps->depth, ps->reference_ગણતરી);
  state_item_print (ps->state_વસ્તુઓ.head_elt, out, "");
  state_item_print (ps->state_વસ્તુઓ.tail_elt, out, "");
  જો (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
