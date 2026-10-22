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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "parse-simulation.h"

#অন্তর্ভাব <gl_linked_list.h>
#অন্তর্ভাব <gl_xlist.h>
#অন্তর্ভাব <stdlib.h>

#অন্তর্ভাব "lssi.h"
#অন্তর্ভাব "nullable.h"

সংরচনা parse_state
{
  // Path of state-items the parser has traversed.
  সংরচনা si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ধ্রুব state_item *head_elt;
    ধ্রুব state_item *tail_elt;
    size_t total_size;
  } state_বস্তুসমূহ;
  // List of derivations of the symbols.
  সংরচনা deriv_chunk
  {
    derivation_list contents;
    ধ্রুব derivation *head_elt;
    ধ্রুব derivation *tail_elt;
    size_t total_size;
  } derivs;
  সংরচনা parse_state *parent;
  পূর্ণাঙ্ক reference_গুনতি;
  // Incremented during productions, decremented during reductions.
  পূর্ণাঙ্ক depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


স্থির শূন্য
ps_si_prepend (parse_state *ps, ধ্রুব state_item *si)
{
  সংরচনা si_chunk *sic = &ps->state_বস্তুসমূহ;
  gl_list_add_first (sic->contents, si);
  sic->head_elt = si;
  ++sic->total_size;
  যদি (!sic->tail_elt)
    sic->tail_elt = si;
}

স্থির শূন্য
ps_si_append (parse_state *ps, ধ্রুব state_item *si)
{
  সংরচনা si_chunk *sic = &ps->state_বস্তুসমূহ;
  gl_list_add_last (sic->contents, si);
  sic->tail_elt = si;
  ++sic->total_size;
  যদি (!sic->head_elt)
    sic->head_elt = si;
}

স্থির শূন্য
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  সংরচনা deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->head_elt = d;
  ++dc->total_size;
  যদি (!dc->tail_elt)
    dc->tail_elt = d;
}

স্থির শূন্য
ps_derivs_append (parse_state *ps, derivation *d)
{
  সংরচনা deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->tail_elt = d;
  ++dc->total_size;
  যদি (!dc->head_elt)
    dc->head_elt = d;
}

স্থির পূর্ণাঙ্ক allocs = 0;
স্থির পূর্ণাঙ্ক frees = 0;

স্থির parse_state *
empty_parse_state (শূন্য)
{
  parse_state *res = xcalloc (1, পরিমাণ *res);
  res->state_বস্তুসমূহ.contents
    = gl_list_create_empty (GL_LINKED_LIST, রিক্ত, রিক্ত, রিক্ত, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  প্রত্যাগম res;
}

parse_state *
new_parse_state (ধ্রুব state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  প্রত্যাগম res;
}

স্থির parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (পরিমাণ *res);
  *res = *parent;
  res->state_বস্তুসমূহ.contents
    = gl_list_create_empty (GL_LINKED_LIST, রিক্ত, রিক্ত, রিক্ত, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_গুনতি = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  প্রত্যাগম res;
}

bool
parse_state_derivation_completed (ধ্রুব parse_state *ps)
{
  প্রত্যাগম ps->derivs.total_size == 1;
}

derivation *
parse_state_derivation (ধ্রুব parse_state *ps)
{
  প্রত্যাগম (derivation *) ps->derivs.head_elt;
}

ধ্রুব state_item *
parse_state_head (ধ্রুব parse_state *ps)
{
  প্রত্যাগম ps->state_বস্তুসমূহ.head_elt;
}

ধ্রুব state_item *
parse_state_tail (ধ্রুব parse_state *ps)
{
  প্রত্যাগম ps->state_বস্তুসমূহ.tail_elt;
}

পূর্ণাঙ্ক
parse_state_length (ধ্রুব parse_state *ps)
{
  প্রত্যাগম ps->state_বস্তুসমূহ.total_size;
}

পূর্ণাঙ্ক
parse_state_depth (ধ্রুব parse_state *ps)
{
  প্রত্যাগম ps->depth;
}

শূন্য
parse_state_retain (parse_state *ps)
{
  ++ps->reference_গুনতি;
}

শূন্য
parse_state_free_contents_early (parse_state *ps)
{
  ps->free_contents_early = true;
}

শূন্য
free_parse_state (parse_state *original_ps)
{
  bool free_contents = true;
  parse_state *parent_ps = রিক্ত;
  প্রতি (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
    {
      --ps->reference_গুনতি;
      free_contents = (ps->reference_গুনতি == 1 && ps->free_contents_early)
        || (ps->reference_গুনতি == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      যদি (free_contents)
        {
          যদি (ps->state_বস্তুসমূহ.contents)
            gl_list_free (ps->state_বস্তুসমূহ.contents);
          যদি (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      parent_ps = ps->parent;
      যদি (ps->reference_গুনতি <= 0)
        {
          মুক্ত (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ধ্রুব parse_state *ps, size_t max)
{
  ধ্রুব সংরচনা si_chunk *sis = &ps->state_বস্তুসমূহ;
  প্রত্যাগম ((state_item *) sis->head_elt - state_বস্তুসমূহ +
          (state_item *) sis->tail_elt - state_বস্তুসমূহ + sis->total_size) % max;
}

bool
parse_state_comparator (ধ্রুব parse_state *ps1, ধ্রুব parse_state *ps2)
{
  ধ্রুব সংরচনা si_chunk *sis1 = &ps1->state_বস্তুসমূহ;
  ধ্রুব সংরচনা si_chunk *sis2 = &ps2->state_বস্তুসমূহ;
  প্রত্যাগম sis1->head_elt == sis2->head_elt
    && sis1->tail_elt == sis2->tail_elt
    && sis1->total_size == sis2->total_size;
}


শূন্য
parse_state_completed_steps (ধ্রুব parse_state *ps, পূর্ণাঙ্ক *shifts, পূর্ণাঙ্ক *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ধ্রুব parse_state *মূল_ps = ps;
  যাবত্ (মূল_ps->parent)
    মূল_ps = মূল_ps->parent;

  state_item_list sis = মূল_ps->state_বস্তুসমূহ.contents;
  পূর্ণাঙ্ক গুনতি = 0;

  state_item *last = রিক্ত;
  state_item *next = রিক্ত;
  প্রতি (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_next (&it, &next);
       )
    {
      যদি (last && last->state == next->state)
        ++গুনতি;
      last = next;
    }
  *productions = গুনতি;
  *shifts = মূল_ps->state_বস্তুসমূহ.total_size - গুনতি;
}

প্রকারনির্দেশ শূন্য (*chunk_append_fn) (gl_list_t, ধ্রুব শূন্য *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
স্থির শূন্য
list_add_last (gl_list_t list, ধ্রুব শূন্য *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
স্থির শূন্য
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, পূর্ণাঙ্ক split, পূর্ণাঙ্ক n,
                        chunk_append_fn append_fn)
{
  পূর্ণাঙ্ক ret_index = 0;
  পূর্ণাঙ্ক ret_array = 0;
  প্রতি (পূর্ণাঙ্ক i = 0; i < n; ++i)
    {
      ধ্রুব শূন্য *p = রিক্ত;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      যাবত্ (gl_list_iterator_next (&it, &p, রিক্ত))
        যদি (p)
          {
            gl_list_t l = (gl_list_t) p;
            ধ্রুব শূন্য *si = রিক্ত;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            যাবত্ (gl_list_iterator_next (&it2, &si, রিক্ত))
              {
                যদি (ret_index++ == split)
                  ++ret_array;
                যদি (rets[ret_array])
                  append_fn (rets[ret_array], si);
              }
            gl_list_iterator_free (&it2);
          }
      gl_list_iterator_free (&it);
    }
}

স্থির parse_state_list
parse_state_list_new (শূন্য)
{
  প্রত্যাগম gl_list_create_empty (GL_LINKED_LIST, রিক্ত, রিক্ত,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

স্থির শূন্য
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_add_last (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
স্থির derivation_list
parser_বের_করো (parse_state *ps, পূর্ণাঙ্ক deriv_index,
            পূর্ণাঙ্ক si_index, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  প্রতি (পূর্ণাঙ্ক i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, রিক্ত, রিক্ত, রিক্ত, true);
  প্রতি (parse_state *pn = ps; pn != রিক্ত; pn = pn->parent)
    যদি (pn->prepend)
      {
        gl_list_add_last (chunks[0], pn->state_বস্তুসমূহ.contents);
        gl_list_add_last (chunks[2], pn->derivs.contents);
      }
    অন্যথা
      {
        gl_list_add_first (chunks[1], pn->state_বস্তুসমূহ.contents);
        gl_list_add_first (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_বস্তুসমূহ.contents, রিক্ত,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_index, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_index, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_size = gl_list_size (ret->state_বস্তুসমূহ.contents);
  ret->state_বস্তুসমূহ.total_size = s_size;
  যদি (s_size > 0)
    {
      ret->state_বস্তুসমূহ.tail_elt = gl_list_get_at (ret->state_বস্তুসমূহ.contents,
                                                  s_size - 1);
      ret->state_বস্তুসমূহ.head_elt =
        gl_list_get_at (ret->state_বস্তুসমূহ.contents, 0);
    }
  অন্যথা
    {
      ret->state_বস্তুসমূহ.tail_elt = রিক্ত;
      ret->state_বস্তুসমূহ.head_elt = রিক্ত;
    }
  size_t d_size = gl_list_size (ret->derivs.contents);
  ret->derivs.total_size = d_size;
  যদি (d_size > 0)
    {
      ret->derivs.tail_elt = gl_list_get_at (ret->derivs.contents,
                                             d_size - 1);
      ret->derivs.head_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  অন্যথা
    {
      ret->derivs.tail_elt = রিক্ত;
      ret->derivs.head_elt = রিক্ত;
    }
  প্রতি (পূর্ণাঙ্ক i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  প্রত্যাগম popped_derivs;
}

শূন্য
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *temp = empty_parse_state ();
  size_t si_size = ps->state_বস্তুসমূহ.total_size;
  size_t deriv_size = ps->derivs.total_size;
  derivation_list dl = parser_বের_করো (ps, si_size, deriv_size, temp);
  *sitems = temp->state_বস্তুসমূহ.contents;
  *derivs = temp->derivs.contents;
  // prevent the return lists from being freed
  temp->state_বস্তুসমূহ.contents = রিক্ত;
  temp->derivs.contents = রিক্ত;
  free_parse_state (temp);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
স্থির শূন্য
nullable_closure (parse_state *ps, state_item *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_item_সংখ্যা prev_sin = si - state_বস্তুসমূহ;
  প্রতি (state_item_সংখ্যা sin = si->trans; sin != -1;
       prev_sin = sin, sin = state_বস্তুসমূহ[sin].trans)
    {
      state_item *psi = &state_বস্তুসমূহ[prev_sin];
      প্রতীক_সংখ্যা sp = item_সংখ্যা_as_প্রতীক_সংখ্যা (*psi->item);
      যদি (ISTOKEN (sp) || !nullable[sp - ntokens])
        ভঙ্গ;

      state_item *nsi = &state_বস্তুসমূহ[sin];
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
  ধ্রুব state_item *si = ps->state_বস্তুসমূহ.tail_elt;
  প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list result = parse_state_list_new ();
  state_item_সংখ্যা si_next = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  যদি (si_next < 0)
    প্রত্যাগম result;
  parse_state *next_ps = copy_parse_state (false, ps);
  ps_si_append (next_ps, &state_বস্তুসমূহ[si_next]);
  ps_derivs_append (next_ps, derivation_new_leaf (sym));
  parse_state_list_append (result, next_ps);

  nullable_closure (next_ps, &state_বস্তুসমূহ[si_next], result);
  প্রত্যাগম result;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
স্থির bool
compatible (প্রতীক_সংখ্যা sym1, প্রতীক_সংখ্যা sym2)
{
  যদি (sym1 == sym2)
    প্রত্যাগম true;
  যদি (ISTOKEN (sym1) && ISVAR (sym2))
    প্রত্যাগম bitset_test (FIRSTS (sym2), sym1);
  অন্যথা যদি (ISVAR (sym1) && ISTOKEN (sym2))
    প্রত্যাগম bitset_test (FIRSTS (sym1), sym2);
  অন্যথা যদি (ISVAR (sym1) && ISVAR (sym2))
    প্রত্যাগম !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  অন্যথা
    প্রত্যাগম false;
}

parse_state_list
simulate_production (parse_state *ps, প্রতীক_সংখ্যা compat_sym)
{
  parse_state_list result = parse_state_list_new ();
  ধ্রুব state_item *si = parse_state_tail (ps);
  যদি (si->prods)
    {
      bitset_iterator biter;
      state_item_সংখ্যা sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *next = &state_বস্তুসমূহ[sin];
          item_সংখ্যা *itm1 = next->item;
          যদি (!compatible (*itm1, compat_sym) || !production_allowed (si, next))
            অনুবর্ত;
          parse_state *next_ps = copy_parse_state (false, ps);
          ps_si_append (next_ps, next);
          parse_state_list_append (result, next_ps);
          যদি (next_ps->depth >= 0)
            ++next_ps->depth;
          nullable_closure (next_ps, next, result);
        }
    }
  প্রত্যাগম result;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, পূর্ণাঙ্ক rule_len, bitset প্রতীক_set)
{
  parse_state_list result = parse_state_list_new ();

  পূর্ণাঙ্ক s_size = ps->state_বস্তুসমূহ.total_size;
  পূর্ণাঙ্ক d_size = ps->derivs.total_size;
  যদি (ps->depth >= 0)
    d_size--;                   // account for dot
  parse_state *new_মূল = empty_parse_state ();
  derivation_list popped_derivs =
    parser_বের_করো (ps, d_size - rule_len,
                s_size - rule_len - 1, new_মূল);

  // update derivation
  state_item *si = (state_item *) ps->state_বস্তুসমূহ.tail_elt;
  ধ্রুব rule *r = item_rule (si->item);
  প্রতীক_সংখ্যা lhs = r->lhs->সংখ্যা;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_মূল->depth;
  ps_derivs_append (new_মূল, deriv);

  যদি (s_size != rule_len + 1)
    {
      state_item *tail = (state_item *) new_মূল->state_বস্তুসমূহ.tail_elt;
      ps_si_append (new_মূল, &state_বস্তুসমূহ[tail->trans]);
      parse_state_list_append (result, new_মূল);
    }
  অন্যথা
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ধ্রুব state_item *head = ps->state_বস্তুসমূহ.head_elt;
      state_item_list prev = lssi_reverse_production (head, প্রতীক_set);
      // TODO: better understand what causes this case.
      যদি (gl_list_size (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_মূল);
          free_parse_state (new_মূল);
        }
      অন্যথা
        {
          state_item *psis = রিক্ত;
          প্রতি (gl_list_iterator_t it = gl_list_iterator (prev);
               state_item_list_next (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_মূল);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              সংরচনা si_chunk *sis = &copy->state_বস্তুসমূহ;
              ধ্রুব state_item *tail = sis->tail_elt;
              ps_si_append (copy, &state_বস্তুসমূহ[tail->trans]);
              parse_state_list_append (result, copy);
              nullable_closure (copy, (state_item *) sis->tail_elt, result);
            }
        }
      gl_list_free (prev);
    }
  প্রত্যাগম result;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ধ্রুব state_item *head = ps->state_বস্তুসমূহ.head_elt;
  প্রতীক_সংখ্যা prepend_sym =
    item_সংখ্যা_as_প্রতীক_সংখ্যা (*(head->item - 1));
  bitset_iterator biter;
  state_item_সংখ্যা sin;
  BITSET_FOR_EACH (biter, head->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_বস্তুসমূহ[sin]);
    যদি (SI_TRANSITION (head))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  প্রত্যাগম res;
}

শূন্য
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_বস্তুসমূহ.total_size, ps->depth, ps->reference_গুনতি);
  state_item_print (ps->state_বস্তুসমূহ.head_elt, out, "");
  state_item_print (ps->state_বস্তুসমূহ.tail_elt, out, "");
  যদি (ps->derivs.total_size > 0)
    derivation_print (ps->derivs.head_elt, out, "");
  putc ('\n', out);
}
