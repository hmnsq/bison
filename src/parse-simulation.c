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

কাঠামো parse_state
{
  // Path of state-items the parser has traversed.
  কাঠামো si_chunk
  {
    // Elements newly added in this chunk.
    state_item_list contents;
    // Properties of the linked list this chunk represents.
    ধ্রুবক state_item *শীর্ষ_elt;
    ধ্রুবক state_item *লেজ_elt;
    size_t যোগ_আকার;
  } state_বস্তুসমূহ;
  // List of derivations of the symbols.
  কাঠামো deriv_chunk
  {
    derivation_list contents;
    ধ্রুবক derivation *শীর্ষ_elt;
    ধ্রুবক derivation *লেজ_elt;
    size_t যোগ_আকার;
  } derivs;
  কাঠামো parse_state *parent;
  পূর্ণসংখ্যা reference_গুনতি;
  // Incremented during productions, decremented during reductions.
  পূর্ণসংখ্যা depth;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


স্থির শূন্য
ps_si_prepend (parse_state *ps, ধ্রুবক state_item *si)
{
  কাঠামো si_chunk *sic = &ps->state_বস্তুসমূহ;
  gl_list_add_first (sic->contents, si);
  sic->শীর্ষ_elt = si;
  ++sic->যোগ_আকার;
  যদি (!sic->লেজ_elt)
    sic->লেজ_elt = si;
}

স্থির শূন্য
ps_si_append (parse_state *ps, ধ্রুবক state_item *si)
{
  কাঠামো si_chunk *sic = &ps->state_বস্তুসমূহ;
  gl_list_add_last (sic->contents, si);
  sic->লেজ_elt = si;
  ++sic->যোগ_আকার;
  যদি (!sic->শীর্ষ_elt)
    sic->শীর্ষ_elt = si;
}

স্থির শূন্য
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  কাঠামো deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->শীর্ষ_elt = d;
  ++dc->যোগ_আকার;
  যদি (!dc->লেজ_elt)
    dc->লেজ_elt = d;
}

স্থির শূন্য
ps_derivs_append (parse_state *ps, derivation *d)
{
  কাঠামো deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->লেজ_elt = d;
  ++dc->যোগ_আকার;
  যদি (!dc->শীর্ষ_elt)
    dc->শীর্ষ_elt = d;
}

স্থির পূর্ণসংখ্যা allocs = 0;
স্থির পূর্ণসংখ্যা frees = 0;

স্থির parse_state *
empty_parse_state (শূন্য)
{
  parse_state *res = xcalloc (1, আকার *res);
  res->state_বস্তুসমূহ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  ফেরত res;
}

parse_state *
new_parse_state (ধ্রুবক state_item *si)
{
  parse_state *res = empty_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  ফেরত res;
}

স্থির parse_state *
copy_parse_state (bool prepend, parse_state *parent)
{
  parse_state *res = xmalloc (আকার *res);
  *res = *parent;
  res->state_বস্তুসমূহ.contents
    = gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
  res->derivs.contents = derivation_list_new ();
  res->parent = parent;
  res->prepend = prepend;
  res->reference_গুনতি = 0;
  res->free_contents_early = false;
  parse_state_retain (parent);
  ++allocs;
  ফেরত res;
}

bool
parse_state_derivation_completed (ধ্রুবক parse_state *ps)
{
  ফেরত ps->derivs.যোগ_আকার == 1;
}

derivation *
parse_state_derivation (ধ্রুবক parse_state *ps)
{
  ফেরত (derivation *) ps->derivs.শীর্ষ_elt;
}

ধ্রুবক state_item *
parse_state_শীর্ষ (ধ্রুবক parse_state *ps)
{
  ফেরত ps->state_বস্তুসমূহ.শীর্ষ_elt;
}

ধ্রুবক state_item *
parse_state_লেজ (ধ্রুবক parse_state *ps)
{
  ফেরত ps->state_বস্তুসমূহ.লেজ_elt;
}

পূর্ণসংখ্যা
parse_state_দৈর্ঘ্য (ধ্রুবক parse_state *ps)
{
  ফেরত ps->state_বস্তুসমূহ.যোগ_আকার;
}

পূর্ণসংখ্যা
parse_state_depth (ধ্রুবক parse_state *ps)
{
  ফেরত ps->depth;
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
  parse_state *parent_ps = ফাঁকা;
  জন্য (parse_state *ps = original_ps; ps && free_contents; ps = parent_ps)
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
parse_state_hasher (ধ্রুবক parse_state *ps, size_t max)
{
  ধ্রুবক কাঠামো si_chunk *sis = &ps->state_বস্তুসমূহ;
  ফেরত ((state_item *) sis->শীর্ষ_elt - state_বস্তুসমূহ +
          (state_item *) sis->লেজ_elt - state_বস্তুসমূহ + sis->যোগ_আকার) % max;
}

bool
parse_state_comparator (ধ্রুবক parse_state *ps1, ধ্রুবক parse_state *ps2)
{
  ধ্রুবক কাঠামো si_chunk *sis1 = &ps1->state_বস্তুসমূহ;
  ধ্রুবক কাঠামো si_chunk *sis2 = &ps2->state_বস্তুসমূহ;
  ফেরত sis1->শীর্ষ_elt == sis2->শীর্ষ_elt
    && sis1->লেজ_elt == sis2->লেজ_elt
    && sis1->যোগ_আকার == sis2->যোগ_আকার;
}


শূন্য
parse_state_completed_steps (ধ্রুবক parse_state *ps, পূর্ণসংখ্যা *shifts, পূর্ণসংখ্যা *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ধ্রুবক parse_state *মূল_ps = ps;
  যখন (মূল_ps->parent)
    মূল_ps = মূল_ps->parent;

  state_item_list sis = মূল_ps->state_বস্তুসমূহ.contents;
  পূর্ণসংখ্যা গুনতি = 0;

  state_item *last = ফাঁকা;
  state_item *পরবর্তী = ফাঁকা;
  জন্য (gl_list_iterator_t it = gl_list_iterator (sis);
       state_item_list_পরবর্তী (&it, &পরবর্তী);
       )
    {
      যদি (last && last->state == পরবর্তী->state)
        ++গুনতি;
      last = পরবর্তী;
    }
  *productions = গুনতি;
  *shifts = মূল_ps->state_বস্তুসমূহ.যোগ_আকার - গুনতি;
}

প্রকারলেখ শূন্য (*chunk_append_fn) (gl_list_t, ধ্রুবক শূন্য *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
স্থির শূন্য
list_add_last (gl_list_t list, ধ্রুবক শূন্য *elt)
{
  gl_list_add_last (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
স্থির শূন্য
list_flatten_and_split (gl_list_t *list, gl_list_t *rets, পূর্ণসংখ্যা split, পূর্ণসংখ্যা n,
                        chunk_append_fn append_fn)
{
  পূর্ণসংখ্যা ret_সূচক = 0;
  পূর্ণসংখ্যা ret_array = 0;
  জন্য (পূর্ণসংখ্যা i = 0; i < n; ++i)
    {
      ধ্রুবক শূন্য *p = ফাঁকা;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      যখন (gl_list_iterator_পরবর্তী (&it, &p, ফাঁকা))
        যদি (p)
          {
            gl_list_t l = (gl_list_t) p;
            ধ্রুবক শূন্য *si = ফাঁকা;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            যখন (gl_list_iterator_পরবর্তী (&it2, &si, ফাঁকা))
              {
                যদি (ret_সূচক++ == split)
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
  ফেরত gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা,
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
parser_বের_করো (parse_state *ps, পূর্ণসংখ্যা deriv_সূচক,
            পূর্ণসংখ্যা si_সূচক, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  জন্য (পূর্ণসংখ্যা i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
  জন্য (parse_state *pn = ps; pn != ফাঁকা; pn = pn->parent)
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
  gl_list_t ret_chunks[4] = { ret->state_বস্তুসমূহ.contents, ফাঁকা,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_split (chunks, ret_chunks, si_সূচক, 2,
                          list_add_last);
  list_flatten_and_split (chunks + 2, ret_chunks + 2, deriv_সূচক, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_আকার = gl_list_আকার (ret->state_বস্তুসমূহ.contents);
  ret->state_বস্তুসমূহ.যোগ_আকার = s_আকার;
  যদি (s_আকার > 0)
    {
      ret->state_বস্তুসমূহ.লেজ_elt = gl_list_get_at (ret->state_বস্তুসমূহ.contents,
                                                  s_আকার - 1);
      ret->state_বস্তুসমূহ.শীর্ষ_elt =
        gl_list_get_at (ret->state_বস্তুসমূহ.contents, 0);
    }
  অন্যথা
    {
      ret->state_বস্তুসমূহ.লেজ_elt = ফাঁকা;
      ret->state_বস্তুসমূহ.শীর্ষ_elt = ফাঁকা;
    }
  size_t d_আকার = gl_list_আকার (ret->derivs.contents);
  ret->derivs.যোগ_আকার = d_আকার;
  যদি (d_আকার > 0)
    {
      ret->derivs.লেজ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_আকার - 1);
      ret->derivs.শীর্ষ_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  অন্যথা
    {
      ret->derivs.লেজ_elt = ফাঁকা;
      ret->derivs.শীর্ষ_elt = ফাঁকা;
    }
  জন্য (পূর্ণসংখ্যা i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  ফেরত popped_derivs;
}

শূন্য
parse_state_lists (parse_state *ps, state_item_list *sitems,
                   derivation_list *derivs)
{
  parse_state *অস্থায়ী = empty_parse_state ();
  size_t si_আকার = ps->state_বস্তুসমূহ.যোগ_আকার;
  size_t deriv_আকার = ps->derivs.যোগ_আকার;
  derivation_list dl = parser_বের_করো (ps, si_আকার, deriv_আকার, অস্থায়ী);
  *sitems = অস্থায়ী->state_বস্তুসমূহ.contents;
  *derivs = অস্থায়ী->derivs.contents;
  // prevent the return lists from being freed
  অস্থায়ী->state_বস্তুসমূহ.contents = ফাঁকা;
  অস্থায়ী->derivs.contents = ফাঁকা;
  free_parse_state (অস্থায়ী);
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
  state_item_সংখ্যা পূর্ব_sin = si - state_বস্তুসমূহ;
  জন্য (state_item_সংখ্যা sin = si->trans; sin != -1;
       পূর্ব_sin = sin, sin = state_বস্তুসমূহ[sin].trans)
    {
      state_item *psi = &state_বস্তুসমূহ[পূর্ব_sin];
      প্রতীক_সংখ্যা sp = item_সংখ্যা_as_প্রতীক_সংখ্যা (*psi->item);
      যদি (ISTOKEN (sp) || !nullable[sp - ntokens])
        বিরতি;

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
  ধ্রুবক state_item *si = ps->state_বস্তুসমূহ.লেজ_elt;
  প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*si->item);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list ফলাফল = parse_state_list_new ();
  state_item_সংখ্যা si_পরবর্তী = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  যদি (si_পরবর্তী < 0)
    ফেরত ফলাফল;
  parse_state *পরবর্তী_ps = copy_parse_state (false, ps);
  ps_si_append (পরবর্তী_ps, &state_বস্তুসমূহ[si_পরবর্তী]);
  ps_derivs_append (পরবর্তী_ps, derivation_new_leaf (sym));
  parse_state_list_append (ফলাফল, পরবর্তী_ps);

  nullable_closure (পরবর্তী_ps, &state_বস্তুসমূহ[si_পরবর্তী], ফলাফল);
  ফেরত ফলাফল;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
স্থির bool
compatible (প্রতীক_সংখ্যা sym1, প্রতীক_সংখ্যা sym2)
{
  যদি (sym1 == sym2)
    ফেরত true;
  যদি (ISTOKEN (sym1) && ISVAR (sym2))
    ফেরত bitset_test (FIRSTS (sym2), sym1);
  অন্যথা যদি (ISVAR (sym1) && ISTOKEN (sym2))
    ফেরত bitset_test (FIRSTS (sym1), sym2);
  অন্যথা যদি (ISVAR (sym1) && ISVAR (sym2))
    ফেরত !bitset_disjoint_p (FIRSTS (sym1), FIRSTS (sym2));
  অন্যথা
    ফেরত false;
}

parse_state_list
simulate_production (parse_state *ps, প্রতীক_সংখ্যা compat_sym)
{
  parse_state_list ফলাফল = parse_state_list_new ();
  ধ্রুবক state_item *si = parse_state_লেজ (ps);
  যদি (si->prods)
    {
      bitset_iterator biter;
      state_item_সংখ্যা sin;
      BITSET_FOR_EACH (biter, si->prods, sin, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_item *পরবর্তী = &state_বস্তুসমূহ[sin];
          item_সংখ্যা *itm1 = পরবর্তী->item;
          যদি (!compatible (*itm1, compat_sym) || !production_allowed (si, পরবর্তী))
            চালু;
          parse_state *পরবর্তী_ps = copy_parse_state (false, ps);
          ps_si_append (পরবর্তী_ps, পরবর্তী);
          parse_state_list_append (ফলাফল, পরবর্তী_ps);
          যদি (পরবর্তী_ps->depth >= 0)
            ++পরবর্তী_ps->depth;
          nullable_closure (পরবর্তী_ps, পরবর্তী, ফলাফল);
        }
    }
  ফেরত ফলাফল;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, পূর্ণসংখ্যা rule_len, bitset প্রতীক_set)
{
  parse_state_list ফলাফল = parse_state_list_new ();

  পূর্ণসংখ্যা s_আকার = ps->state_বস্তুসমূহ.যোগ_আকার;
  পূর্ণসংখ্যা d_আকার = ps->derivs.যোগ_আকার;
  যদি (ps->depth >= 0)
    d_আকার--;                   // account for dot
  parse_state *new_মূল = empty_parse_state ();
  derivation_list popped_derivs =
    parser_বের_করো (ps, d_আকার - rule_len,
                s_আকার - rule_len - 1, new_মূল);

  // update derivation
  state_item *si = (state_item *) ps->state_বস্তুসমূহ.লেজ_elt;
  ধ্রুবক rule *r = item_rule (si->item);
  প্রতীক_সংখ্যা lhs = r->lhs->সংখ্যা;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_item_rule (si));
  --new_মূল->depth;
  ps_derivs_append (new_মূল, deriv);

  যদি (s_আকার != rule_len + 1)
    {
      state_item *লেজ = (state_item *) new_মূল->state_বস্তুসমূহ.লেজ_elt;
      ps_si_append (new_মূল, &state_বস্তুসমূহ[লেজ->trans]);
      parse_state_list_append (ফলাফল, new_মূল);
    }
  অন্যথা
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ধ্রুবক state_item *শীর্ষ_3 = ps->state_বস্তুসমূহ.শীর্ষ_elt;
      state_item_list পূর্ব = lssi_reverse_production (শীর্ষ_3, প্রতীক_set);
      // TODO: better understand what causes this case.
      যদি (gl_list_আকার (পূর্ব) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_মূল);
          free_parse_state (new_মূল);
        }
      অন্যথা
        {
          state_item *psis = ফাঁকা;
          জন্য (gl_list_iterator_t it = gl_list_iterator (পূর্ব);
               state_item_list_পরবর্তী (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_মূল);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              কাঠামো si_chunk *sis = &copy->state_বস্তুসমূহ;
              ধ্রুবক state_item *লেজ = sis->লেজ_elt;
              ps_si_append (copy, &state_বস্তুসমূহ[লেজ->trans]);
              parse_state_list_append (ফলাফল, copy);
              nullable_closure (copy, (state_item *) sis->লেজ_elt, ফলাফল);
            }
        }
      gl_list_free (পূর্ব);
    }
  ফেরত ফলাফল;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ধ্রুবক state_item *শীর্ষ_3 = ps->state_বস্তুসমূহ.শীর্ষ_elt;
  প্রতীক_সংখ্যা prepend_sym =
    item_সংখ্যা_as_প্রতীক_সংখ্যা (*(শীর্ষ_3->item - 1));
  bitset_iterator biter;
  state_item_সংখ্যা sin;
  BITSET_FOR_EACH (biter, শীর্ষ_3->revs, sin, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_বস্তুসমূহ[sin]);
    যদি (SI_TRANSITION (শীর্ষ_3))
      ps_derivs_prepend (copy, derivation_new_leaf (prepend_sym));
    parse_state_list_append (res, copy);
  }
  ফেরত res;
}

শূন্য
print_parse_state (parse_state *ps)
{
  FILE *out = stderr;
  fprintf (out, "(size %zu depth %d rc %d)\n",
          ps->state_বস্তুসমূহ.যোগ_আকার, ps->depth, ps->reference_গুনতি);
  state_item_print (ps->state_বস্তুসমূহ.শীর্ষ_elt, out, "");
  state_item_print (ps->state_বস্তুসমূহ.লেজ_elt, out, "");
  যদি (ps->derivs.যোগ_আকার > 0)
    derivation_print (ps->derivs.শীর্ষ_elt, out, "");
  putc ('\n', out);
}
