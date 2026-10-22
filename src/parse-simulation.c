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

#شامل <config.h>

#شامل "parse-simulation.h"

#شامل <gl_linked_list.h>
#شامل <gl_xlist.h>
#شامل <stdlib.h>

#شامل "lssi.h"
#شامل "nullable.h"

ساختار parse_state
{
  // Path of state-items the parser has traversed.
  ساختار si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ثابت state_item *head_elt;
    ثابت state_item *tail_elt;
    size_t total_size;
  } state_اقلام;
  // List of derivations of the symbols.
  ساختار deriv_chunk
  {
    derivation_list contents;
    ثابت derivation *head_elt;
    ثابت derivation *tail_elt;
    size_t total_size;
  } derivs;
  ساختار parse_state *parent;
  صحیح reference_شمار;
  // Incremented during productions, decremented during reductions.
  صحیح depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ایستا تهی
ps_si_prepend (parse_state *ps, ثابت state_item *si)
{
  ساختار si_chunk *sic = &ps->state_اقلام;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  اگر (!sic->tail_elt)
    sic->tail_elt = si;
}

ایستا تهی
ps_si_append (parse_state *ps, ثابت state_item *si)
{
  ساختار si_chunk *sic = &ps->state_اقلام;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  اگر (!sic->head_elt)
    sic->head_elt = si;
}

ایستا تهی
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  ساختار deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  اگر (!dc->tail_elt)
    dc->tail_elt = d;
}

ایستا تهی
ps_derivs_append (parse_state *ps, derivation *d)
{
  ساختار deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  اگر (!dc->head_elt)
    dc->head_elt = d;
}

ایستا صحیح allocs = 0;
ایستا صحیح frees = 0;

ایستا parse_state *
empty_parse_state (تهی)
{
  parse_state *res = xcalloc (1, اندازه *res);
  res->state_اقلام.contents
    = gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  بازگشت res;
}

parse_state *
new_parse_state (ثابت state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  بازگشت res;
}

ایستا parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (اندازه *res);
  *res = *parent;
  res->state_اقلام.contents
    = gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_شمار = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  بازگشت res;
}

bool
parse_state_derivation_completed (ثابت parse_state *ps)
{
  بازگشت ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (ثابت parse_state *ps)
{
  بازگشت (derivation *) ps->derivs.head_elt;
}

ثابت state_item *
parse_state_head (ثابت parse_state *ps)
{
  بازگشت ps->state_اقلام.head_elt;
}

ثابت state_item *
parse_state_tail (ثابت parse_state *ps)
{
  بازگشت ps->state_اقلام.tail_elt;
}

صحیح
parse_state_length (ثابت parse_state *ps)
{
  بازگشت ps->state_اقلام.total_size;
}

صحیح
parse_state_depth (ثابت parse_state *ps)
{
  بازگشت ps->depth;
}

تهی
parse_state_retain (parse_state *ps)
{
  ++ps->reference_شمار;
}

تهی
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

تهی
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = پوچ;
  برای (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_شمار;
      free_contents = (ps->reference_شمار == 1 && ps->free_contents_early)
        || (ps->reference_شمار == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      اگر (free_contents)
        {
          اگر (ps->state_اقلام.contents)
            gl_list_free (ps->state_اقلام.contents);
          اگر (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      اگر (ps->reference_شمار <= 0)
        {
          آزاد (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ثابت parse_state *ps, size_t max)
{
  ثابت ساختار si_chunk *sis = &ps->state_اقلام;
  بازگشت ((state_item *) sis->head_elt - state_اقلام +
          (state_item *) sis->tail_elt - state_اقلام + sis->total_size) % max;
}

bool
parse_state_comparator (ثابت parse_state *ps1, ثابت parse_state *ps2)
{
  ثابت ساختار si_chunk *sis1 = &ps1->state_اقلام;
  ثابت ساختار si_chunk *sis2 = &ps2->state_اقلام;
  بازگشت sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


تهی
parse_state_completed_steps (ثابت parse_state *ps, صحیح *shifts, صحیح *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ثابت parse_state *ریشه_ps = ps;
  وقتی (ریشه_ps->parent)
    ریشه_ps = ریشه_ps->parent;

  state_item_list sis = ریشه_ps->state_اقلام.contents;
  صحیح شمار = 0;

  state_item *last = پوچ;
  state_item *next = پوچ;
  برای (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      اگر (last && last->state == next->state)
        ++شمار;
      last = next;
    }
  *productions = شمار;
  *shifts = ریشه_ps->state_اقلام.total_size - شمار;
}

تعریفنوع تهی (*chunk_append_fn) (gl_list_t, ثابت تهی *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ایستا تهی
list_add_last (gl_list_t list, ثابت تهی *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ایستا تهی
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, صحیح split, صحیح n,
                        chunk_append_fn append_fn)
{
  صحیح ret_index = 0;
  صحیح ret_array = 0;
  برای (صحیح i = 0; i < n; ++i)
    {
      ثابت تهی *p = پوچ;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      وقتی (gl_list_iterator_next (&it, &p, پوچ))
        اگر (p)
          {
            gl_list_t l = (gl_list_t) p;
            ثابت تهی *si = پوچ;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            وقتی (gl_list_iterator_next (&it2, &si, پوچ))
              {
                اگر (ret_index++ == split)
                  ++ret_array;
                اگر (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

ایستا parse_state_list
parse_state_list_new (تهی)
{
  بازگشت gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ایستا تهی
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ایستا derivation_list
parser_برداشت (parse_state *ps, صحیح deriv_index,
            صحیح si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  برای (صحیح i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
  برای (parse_state *pn = ps; pn != پوچ; pn = pn->parent)
    اگر (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_اقلام.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    وگرنه
      {
        gl_list_add_first (chunks[1], pn->state_اقلام.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_اقلام.contents, پوچ,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_اقلام.contents);
  ret->state_اقلام.total_size = s_size;
  اگر (s_size > 0)
    {
      ret->state_اقلام.tail_elt = gl_list_get_at (ret->state_اقلام.contents,
                                                  s_size - 1);
      ret->state_اقلام.head_elt =
        gl_list_get_at (ret->state_اقلام.contents, 0);
    }
  وگرنه
    {
      ret->state_اقلام.tail_elt = پوچ;
      ret->state_اقلام.head_elt = پوچ;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  اگر (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  وگرنه
    {
      ret->derivs.tail_elt = پوچ;
      ret->derivs.head_elt = پوچ;
    }
  برای (صحیح i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  بازگشت popped_derivs;
}

تهی
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_اقلام.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_برداشت (ps, si_size, deriv_size, temp);
  *sitems = temp->state_اقلام.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_اقلام.contents = پوچ;
  temp->derivs.contents = پوچ;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ایستا تهی
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_عدد prev_sin = si - state_اقلام;
  برای (state_item_عدد sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_اقلام[sin].trans)
    {
      state_item *psi = &state_اقلام[prev_sin];
      نماد_عدد sp = item_عدد_as_نماد_عدد (*psi->item);
      اگر (ISTOKEN (sp) || !nullable[sp - ntokens])
        بشکن;

      state_item *nsi = &state_اقلام[sin];
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
  ثابت state_item *si = ps->state_اقلام.tail_elt;
  نماد_عدد sym = item_عدد_as_نماد_عدد (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_عدد si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  اگر (si_next < 0)
    بازگشت result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_اقلام[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_اقلام[si_next], result);
  بازگشت result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ایستا bool
compatible (نماد_عدد sym1, نماد_عدد sym2)
{
  اگر (sym1 == sym2)
    بازگشت true;
  اگر (ISTOKEN (sym1) && ISVAR (sym2))
    بازگشت bitset_test (FIRSTS (sym2), sym1);
  وگرنه اگر (ISVAR (sym1) && ISTOKEN (sym2))
    بازگشت bitset_test (FIRSTS (sym1), sym2);
  وگرنه اگر (ISVAR (sym1) && ISVAR (sym2))
    بازگشت !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  وگرنه
    بازگشت false;
}

parse_state_list
simulate_production (parse_state *ps, نماد_عدد compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  ثابت state_item *si = parse_state_tail (ps);
  اگر (si->prods)
    {
      bitset_iterator biter;
      state_item_عدد sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_اقلام[sin];
          item_عدد *itm1 = next->item;
          اگر (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            ادامه;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          اگر (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  بازگشت result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, صحیح rule_len, bitset نماد_set)
{
  parse_state_list result = parse_state_list_new ();

  صحیح s_size = ps->state_اقلام.total_size;
  صحیح d_size = ps->derivs.total_size;
  اگر (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_ریشه = empty_parse_state ();
  derivation_list popped_derivs =
    parser_برداشت (ps, d_size - rule_len,
                s_size - rule_len - 1, new_ریشه);

  // update derivation
  state_item *si = (state_item *) ps->state_اقلام.tail_elt;
  ثابت rule *r = item_rule (si->item);
  نماد_عدد lhs = r->lhs->عدد;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_ریشه->depth;
  ps_derivs_append (new_ریشه, deriv);

  اگر (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_ریشه->state_اقلام.tail_elt;
      ps_si_append (new_ریشه, &state_اقلام[tail->trans]);
      parse_state_list_append (result, new_ریشه);
    }
  وگرنه
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ثابت state_item *head = ps->state_اقلام.head_elt;
      state_item_list prev = lssi_reverse_production (head, نماد_set);
      // TODO: better understand what causes this case.
      اگر (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_ریشه);
          free_parse_state (new_ریشه);
        }
      وگرنه
        {
          state_item *psis = پوچ;
          برای (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_ریشه);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              ساختار si_chunk *sis = &copy->state_اقلام;
              ثابت state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_اقلام[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  بازگشت result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ثابت state_item *head = ps->state_اقلام.head_elt;
  نماد_عدد prepend_sym =
    item_عدد_as_نماد_عدد (*(head->item - 1));
  bitset_iterator biter;
  state_item_عدد sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_اقلام[sin]);
    اگر (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  بازگشت res;
}

تهی
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_اقلام.total_size, ps->depth, ps->reference_شمار);
  state_item_print (ps->state_اقلام.head_elt, out, "");
  state_item_print (ps->state_اقلام.tail_elt, out, "");
  اگر (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
