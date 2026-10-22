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

#ضمن <config.h>

#ضمن "parse-simulation.h"

#ضمن <gl_linked_list.h>
#ضمن <gl_xlist.h>
#ضمن <stdlib.h>

#ضمن "lssi.h"
#ضمن "nullable.h"

بنية parse_state
{
  // Path of state-items the parser has traversed.
  بنية si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ثابت state_item *head_elt;
    ثابت state_item *tail_elt;
    size_t total_size;
  } state_عناصر;
  // List of derivations of the symbols.
  بنية deriv_chunk
  {
    derivation_list contents;
    ثابت derivation *head_elt;
    ثابت derivation *tail_elt;
    size_t total_size;
  } derivs;
  بنية parse_state *parent;
  صحيح reference_عدد;
  // Incremented during productions, decremented during reductions.
  صحيح depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ساكن فراغ
ps_si_prepend (parse_state *ps, ثابت state_item *si)
{
  بنية si_chunk *sic = &ps->state_عناصر;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  إذا (!sic->tail_elt)
    sic->tail_elt = si;
}

ساكن فراغ
ps_si_append (parse_state *ps, ثابت state_item *si)
{
  بنية si_chunk *sic = &ps->state_عناصر;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  إذا (!sic->head_elt)
    sic->head_elt = si;
}

ساكن فراغ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  إذا (!dc->tail_elt)
    dc->tail_elt = d;
}

ساكن فراغ
ps_derivs_append (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  إذا (!dc->head_elt)
    dc->head_elt = d;
}

ساكن صحيح allocs = 0;
ساكن صحيح frees = 0;

ساكن parse_state *
empty_parse_state (فراغ)
{
  parse_state *res = xcalloc (1, حجم *res);
  res->state_عناصر.contents
    = gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  أرجع res;
}

parse_state *
new_parse_state (ثابت state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  أرجع res;
}

ساكن parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (حجم *res);
  *res = *parent;
  res->state_عناصر.contents
    = gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_عدد = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  أرجع res;
}

bool
parse_state_derivation_completed (ثابت parse_state *ps)
{
  أرجع ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (ثابت parse_state *ps)
{
  أرجع (derivation *) ps->derivs.head_elt;
}

ثابت state_item *
parse_state_head (ثابت parse_state *ps)
{
  أرجع ps->state_عناصر.head_elt;
}

ثابت state_item *
parse_state_tail (ثابت parse_state *ps)
{
  أرجع ps->state_عناصر.tail_elt;
}

صحيح
parse_state_length (ثابت parse_state *ps)
{
  أرجع ps->state_عناصر.total_size;
}

صحيح
parse_state_depth (ثابت parse_state *ps)
{
  أرجع ps->depth;
}

فراغ
parse_state_retain (parse_state *ps)
{
  ++ps->reference_عدد;
}

فراغ
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

فراغ
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = عدم;
  لكل (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_عدد;
      free_contents = (ps->reference_عدد == 1 && ps->free_contents_early)
        || (ps->reference_عدد == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      إذا (free_contents)
        {
          إذا (ps->state_عناصر.contents)
            gl_list_free (ps->state_عناصر.contents);
          إذا (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      إذا (ps->reference_عدد <= 0)
        {
          حرر (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ثابت parse_state *ps, size_t max)
{
  ثابت بنية si_chunk *sis = &ps->state_عناصر;
  أرجع ((state_item *) sis->head_elt - state_عناصر +
          (state_item *) sis->tail_elt - state_عناصر + sis->total_size) % max;
}

bool
parse_state_comparator (ثابت parse_state *ps1, ثابت parse_state *ps2)
{
  ثابت بنية si_chunk *sis1 = &ps1->state_عناصر;
  ثابت بنية si_chunk *sis2 = &ps2->state_عناصر;
  أرجع sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


فراغ
parse_state_completed_steps (ثابت parse_state *ps, صحيح *shifts, صحيح *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ثابت parse_state *جذر_ps = ps;
  طالما (جذر_ps->parent)
    جذر_ps = جذر_ps->parent;

  state_item_list sis = جذر_ps->state_عناصر.contents;
  صحيح عدد = 0;

  state_item *last = عدم;
  state_item *next = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      إذا (last && last->state == next->state)
        ++عدد;
      last = next;
    }
  *productions = عدد;
  *shifts = جذر_ps->state_عناصر.total_size - عدد;
}

تعريفنوع فراغ (*chunk_append_fn) (gl_list_t, ثابت فراغ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ساكن فراغ
list_add_last (gl_list_t list, ثابت فراغ *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ساكن فراغ
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, صحيح split, صحيح n,
                        chunk_append_fn append_fn)
{
  صحيح ret_index = 0;
  صحيح ret_array = 0;
  لكل (صحيح i = 0; i < n; ++i)
    {
      ثابت فراغ *p = عدم;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      طالما (gl_list_iterator_next (&it, &p, عدم))
        إذا (p)
          {
            gl_list_t l = (gl_list_t) p;
            ثابت فراغ *si = عدم;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            طالما (gl_list_iterator_next (&it2, &si, عدم))
              {
                إذا (ret_index++ == split)
                  ++ret_array;
                إذا (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

ساكن parse_state_list
parse_state_list_new (فراغ)
{
  أرجع gl_list_create_empty (GL_LINKED_LIST, عدم, عدم,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ساكن فراغ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ساكن derivation_list
parser_اسحب (parse_state *ps, صحيح deriv_index,
            صحيح si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  لكل (صحيح i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (parse_state *pn = ps; pn != عدم; pn = pn->parent)
    إذا (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_عناصر.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    وإلا
      {
        gl_list_add_first (chunks[1], pn->state_عناصر.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_عناصر.contents, عدم,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_عناصر.contents);
  ret->state_عناصر.total_size = s_size;
  إذا (s_size > 0)
    {
      ret->state_عناصر.tail_elt = gl_list_get_at (ret->state_عناصر.contents,
                                                  s_size - 1);
      ret->state_عناصر.head_elt =
        gl_list_get_at (ret->state_عناصر.contents, 0);
    }
  وإلا
    {
      ret->state_عناصر.tail_elt = عدم;
      ret->state_عناصر.head_elt = عدم;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  إذا (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  وإلا
    {
      ret->derivs.tail_elt = عدم;
      ret->derivs.head_elt = عدم;
    }
  لكل (صحيح i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  أرجع popped_derivs;
}

فراغ
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_عناصر.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_اسحب (ps, si_size, deriv_size, temp);
  *sitems = temp->state_عناصر.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_عناصر.contents = عدم;
  temp->derivs.contents = عدم;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ساكن فراغ
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_رقم prev_sin = si - state_عناصر;
  لكل (state_item_رقم sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_عناصر[sin].trans)
    {
      state_item *psi = &state_عناصر[prev_sin];
      رمز_رقم sp = item_رقم_as_رمز_رقم (*psi->item);
      إذا (ISTOKEN (sp) || !nullable[sp - ntokens])
        اكسر;

      state_item *nsi = &state_عناصر[sin];
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
  ثابت state_item *si = ps->state_عناصر.tail_elt;
  رمز_رقم sym = item_رقم_as_رمز_رقم (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_رقم si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  إذا (si_next < 0)
    أرجع result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_عناصر[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_عناصر[si_next], result);
  أرجع result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ساكن bool
compatible (رمز_رقم sym1, رمز_رقم sym2)
{
  إذا (sym1 == sym2)
    أرجع true;
  إذا (ISTOKEN (sym1) && ISVAR (sym2))
    أرجع bitset_test (FIRSTS (sym2), sym1);
  وإلا إذا (ISVAR (sym1) && ISTOKEN (sym2))
    أرجع bitset_test (FIRSTS (sym1), sym2);
  وإلا إذا (ISVAR (sym1) && ISVAR (sym2))
    أرجع !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  وإلا
    أرجع false;
}

parse_state_list
simulate_production (parse_state *ps, رمز_رقم compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  ثابت state_item *si = parse_state_tail (ps);
  إذا (si->prods)
    {
      bitset_iterator biter;
      state_item_رقم sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_عناصر[sin];
          item_رقم *itm1 = next->item;
          إذا (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            استمر;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          إذا (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  أرجع result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, صحيح rule_len, bitset رمز_set)
{
  parse_state_list result = parse_state_list_new ();

  صحيح s_size = ps->state_عناصر.total_size;
  صحيح d_size = ps->derivs.total_size;
  إذا (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_جذر = empty_parse_state ();
  derivation_list popped_derivs =
    parser_اسحب (ps, d_size - rule_len,
                s_size - rule_len - 1, new_جذر);

  // update derivation
  state_item *si = (state_item *) ps->state_عناصر.tail_elt;
  ثابت rule *r = item_rule (si->item);
  رمز_رقم lhs = r->lhs->رقم_2;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_جذر->depth;
  ps_derivs_append (new_جذر, deriv);

  إذا (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_جذر->state_عناصر.tail_elt;
      ps_si_append (new_جذر, &state_عناصر[tail->trans]);
      parse_state_list_append (result, new_جذر);
    }
  وإلا
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ثابت state_item *head = ps->state_عناصر.head_elt;
      state_item_list prev = lssi_reverse_production (head, رمز_set);
      // TODO: better understand what causes this case.
      إذا (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_جذر);
          free_parse_state (new_جذر);
        }
      وإلا
        {
          state_item *psis = عدم;
          لكل (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_جذر);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              بنية si_chunk *sis = &copy->state_عناصر;
              ثابت state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_عناصر[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  أرجع result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ثابت state_item *head = ps->state_عناصر.head_elt;
  رمز_رقم prepend_sym =
    item_رقم_as_رمز_رقم (*(head->item - 1));
  bitset_iterator biter;
  state_item_رقم sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_عناصر[sin]);
    إذا (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  أرجع res;
}

فراغ
اطبع_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_عناصر.total_size, ps->depth, ps->reference_عدد);
  state_item_اطبع (ps->state_عناصر.head_elt, out, "");
  state_item_اطبع (ps->state_عناصر.tail_elt, out, "");
  إذا (ps->derivs.total_size > 0)
    derivation_اطبع (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
