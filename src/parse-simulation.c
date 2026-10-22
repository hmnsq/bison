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
    state_توکی_list contents;
    // Properties of the linked list this chunk represents.
    ثابت state_توکی *سر_elt;
    ثابت state_توکی *لکۍ_elt;
    size_t ټول_اندازه;
  } state_توکي;
  // List of derivations of the symbols.
  بنية deriv_chunk
  {
    derivation_list contents;
    ثابت derivation *سر_elt;
    ثابت derivation *لکۍ_elt;
    size_t ټول_اندازه;
  } derivs;
  بنية parse_state *پلار;
  صحيح reference_شمېر;
  // Incremented during productions, decremented during reductions.
  صحيح ژوروالی;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ساكن فراغ
ps_si_prepend (parse_state *ps, ثابت state_توکی *si)
{
  بنية si_chunk *sic = &ps->state_توکي;
  gl_list_جمع_کړه_لومړی (sic->contents, si);
  sic->سر_elt = si;
  ++sic->ټول_اندازه;
  إذا (!sic->لکۍ_elt)
    sic->لکۍ_elt = si;
}

ساكن فراغ
ps_si_append (parse_state *ps, ثابت state_توکی *si)
{
  بنية si_chunk *sic = &ps->state_توکي;
  gl_list_جمع_کړه_وروستی (sic->contents, si);
  sic->لکۍ_elt = si;
  ++sic->ټول_اندازه;
  إذا (!sic->سر_elt)
    sic->سر_elt = si;
}

ساكن فراغ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->سر_elt = d;
  ++dc->ټول_اندازه;
  إذا (!dc->لکۍ_elt)
    dc->لکۍ_elt = d;
}

ساكن فراغ
ps_derivs_append (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->لکۍ_elt = d;
  ++dc->ټول_اندازه;
  إذا (!dc->سر_elt)
    dc->سر_elt = d;
}

ساكن صحيح allocs = 0;
ساكن صحيح frees = 0;

ساكن parse_state *
تشه_parse_state (فراغ)
{
  parse_state *res = xcalloc (1, حجم *res);
  res->state_توکي.contents
    = gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  أرجع res;
}

parse_state *
new_parse_state (ثابت state_توکی *si)
{
  parse_state *res = تشه_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  أرجع res;
}

ساكن parse_state *
copy_parse_state (bool prepend, parse_state *پلار)
{
  parse_state *res = xmalloc (حجم *res);
  *res = *پلار;
  res->state_توکي.contents
    = gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  res->پلار = پلار;
  res->prepend = prepend;
  res->reference_شمېر = 0;
  res->free_contents_early = false;
  parse_state_retain (پلار);
  ++allocs;
  أرجع res;
}

bool
parse_state_derivation_completed (ثابت parse_state *ps)
{
  أرجع ps->derivs.ټول_اندازه == 1;
}

derivation *
parse_state_derivation (ثابت parse_state *ps)
{
  أرجع (derivation *) ps->derivs.سر_elt;
}

ثابت state_توکی *
parse_state_سر (ثابت parse_state *ps)
{
  أرجع ps->state_توکي.سر_elt;
}

ثابت state_توکی *
parse_state_لکۍ (ثابت parse_state *ps)
{
  أرجع ps->state_توکي.لکۍ_elt;
}

صحيح
parse_state_اوږدتیا (ثابت parse_state *ps)
{
  أرجع ps->state_توکي.ټول_اندازه;
}

صحيح
parse_state_ژوروالی (ثابت parse_state *ps)
{
  أرجع ps->ژوروالی;
}

فراغ
parse_state_retain (parse_state *ps)
{
  ++ps->reference_شمېر;
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
  parse_state *پلار_ps = عدم;
  لكل (parse_state *ps = original_ps; ps && free_contents; ps = پلار_ps)
    {
      --ps->reference_شمېر;
      free_contents = (ps->reference_شمېر == 1 && ps->free_contents_early)
        || (ps->reference_شمېر == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      إذا (free_contents)
        {
          إذا (ps->state_توکي.contents)
            gl_list_free (ps->state_توکي.contents);
          إذا (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      پلار_ps = ps->پلار;
      إذا (ps->reference_شمېر <= 0)
        {
          حرر (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ثابت parse_state *ps, size_t max)
{
  ثابت بنية si_chunk *sis = &ps->state_توکي;
  أرجع ((state_توکی *) sis->سر_elt - state_توکي +
          (state_توکی *) sis->لکۍ_elt - state_توکي + sis->ټول_اندازه) % max;
}

bool
parse_state_comparator (ثابت parse_state *ps1, ثابت parse_state *ps2)
{
  ثابت بنية si_chunk *sis1 = &ps1->state_توکي;
  ثابت بنية si_chunk *sis2 = &ps2->state_توکي;
  أرجع sis1->سر_elt == sis2->سر_elt
    && sis1->لکۍ_elt == sis2->لکۍ_elt
    && sis1->ټول_اندازه == sis2->ټول_اندازه;
}


فراغ
parse_state_completed_ګامونه (ثابت parse_state *ps, صحيح *shifts, صحيح *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ثابت parse_state *ریښه_ps = ps;
  طالما (ریښه_ps->پلار)
    ریښه_ps = ریښه_ps->پلار;

  state_توکی_list sis = ریښه_ps->state_توکي.contents;
  صحيح شمېر = 0;

  state_توکی *وروستی = عدم;
  state_توکی *بل = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (sis);
       state_توکی_list_بل (&it, &بل);
       )
    {
      إذا (وروستی && وروستی->state == بل->state)
        ++شمېر;
      وروستی = بل;
    }
  *productions = شمېر;
  *shifts = ریښه_ps->state_توکي.ټول_اندازه - شمېر;
}

تعريفنوع فراغ (*chunk_append_fn) (gl_list_t, ثابت فراغ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ساكن فراغ
list_جمع_کړه_وروستی (gl_list_t list, ثابت فراغ *elt)
{
  gl_list_جمع_کړه_وروستی (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ساكن فراغ
list_flatten_and_وېشل (gl_list_t *list, gl_list_t *rets, صحيح وېشل, صحيح n,
                        chunk_append_fn append_fn)
{
  صحيح ret_شاخص = 0;
  صحيح ret_array = 0;
  لكل (صحيح i = 0; i < n; ++i)
    {
      ثابت فراغ *p = عدم;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      طالما (gl_list_iterator_بل (&it, &p, عدم))
        إذا (p)
          {
            gl_list_t l = (gl_list_t) p;
            ثابت فراغ *si = عدم;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            طالما (gl_list_iterator_بل (&it2, &si, عدم))
              {
                إذا (ret_شاخص++ == وېشل)
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
  أرجع gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ساكن فراغ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_جمع_کړه_وروستی (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ساكن derivation_list
parser_راوباسه (parse_state *ps, صحيح deriv_شاخص,
            صحيح si_شاخص, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  لكل (صحيح i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (parse_state *pn = ps; pn != عدم; pn = pn->پلار)
    إذا (pn->prepend)
      {
        gl_list_جمع_کړه_وروستی (chunks[0], pn->state_توکي.contents);
        gl_list_جمع_کړه_وروستی (chunks[2], pn->derivs.contents);
      }
    وإلا
      {
        gl_list_جمع_کړه_لومړی (chunks[1], pn->state_توکي.contents);
        gl_list_جمع_کړه_لومړی (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_توکي.contents, عدم,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_وېشل (chunks, ret_chunks, si_شاخص, 2,
                          list_جمع_کړه_وروستی);
  list_flatten_and_وېشل (chunks + 2, ret_chunks + 2, deriv_شاخص, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_اندازه = gl_list_اندازه (ret->state_توکي.contents);
  ret->state_توکي.ټول_اندازه = s_اندازه;
  إذا (s_اندازه > 0)
    {
      ret->state_توکي.لکۍ_elt = gl_list_get_at (ret->state_توکي.contents,
                                                  s_اندازه - 1);
      ret->state_توکي.سر_elt =
        gl_list_get_at (ret->state_توکي.contents, 0);
    }
  وإلا
    {
      ret->state_توکي.لکۍ_elt = عدم;
      ret->state_توکي.سر_elt = عدم;
    }
  size_t d_اندازه = gl_list_اندازه (ret->derivs.contents);
  ret->derivs.ټول_اندازه = d_اندازه;
  إذا (d_اندازه > 0)
    {
      ret->derivs.لکۍ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_اندازه - 1);
      ret->derivs.سر_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  وإلا
    {
      ret->derivs.لکۍ_elt = عدم;
      ret->derivs.سر_elt = عدم;
    }
  لكل (صحيح i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  أرجع popped_derivs;
}

فراغ
parse_state_lists (parse_state *ps, state_توکی_list *sitems,
                   derivation_list *derivs)
{
  parse_state *لنډمهاله = تشه_parse_state ();
  size_t si_اندازه = ps->state_توکي.ټول_اندازه;
  size_t deriv_اندازه = ps->derivs.ټول_اندازه;
  derivation_list dl = parser_راوباسه (ps, si_اندازه, deriv_اندازه, لنډمهاله);
  *sitems = لنډمهاله->state_توکي.contents;
  *derivs = لنډمهاله->derivs.contents;
  // prevent the return lists from being freed
  لنډمهاله->state_توکي.contents = عدم;
  لنډمهاله->derivs.contents = عدم;
  free_parse_state (لنډمهاله);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ساكن فراغ
nullable_تړون (parse_state *ps, state_توکی *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_توکی_شمېره prev_sin = si - state_توکي;
  لكل (state_توکی_شمېره ساین = si->trans; ساین != -1;
       prev_sin = ساین, ساین = state_توکي[ساین].trans)
    {
      state_توکی *psi = &state_توکي[prev_sin];
      نښه_شمېره sp = توکی_شمېره_as_نښه_شمېره (*psi->توکی);
      إذا (ISTOKEN (sp) || !nullable[sp - ntokens])
        اكسر;

      state_توکی *nsi = &state_توکي[ساین];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_توکی_قاعده (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  ثابت state_توکی *si = ps->state_توکي.لکۍ_elt;
  نښه_شمېره sym = توکی_شمېره_as_نښه_شمېره (*si->توکی);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list پایله = parse_state_list_new ();
  state_توکی_شمېره si_بل = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  إذا (si_بل < 0)
    أرجع پایله;
  parse_state *بل_ps = copy_parse_state (false, ps);
  ps_si_append (بل_ps, &state_توکي[si_بل]);
  ps_derivs_append (بل_ps, derivation_new_ونې_پاڼه (sym));
  parse_state_list_append (پایله, بل_ps);

  nullable_تړون (بل_ps, &state_توکي[si_بل], پایله);
  أرجع پایله;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ساكن bool
compatible (نښه_شمېره sym1, نښه_شمېره sym2)
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
simulate_production (parse_state *ps, نښه_شمېره compat_sym)
{
  parse_state_list پایله = parse_state_list_new ();
  ثابت state_توکی *si = parse_state_لکۍ (ps);
  إذا (si->prods)
    {
      bitset_iterator biter;
      state_توکی_شمېره ساین;
      BITSET_FOR_EACH (biter, si->prods, ساین, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_توکی *بل = &state_توکي[ساین];
          توکی_شمېره *itm1 = بل->توکی;
          إذا (!compatible (*itm1, compat_sym) || !production_allowed (si, بل))
            استمر;
          parse_state *بل_ps = copy_parse_state (false, ps);
          ps_si_append (بل_ps, بل);
          parse_state_list_append (پایله, بل_ps);
          إذا (بل_ps->ژوروالی >= 0)
            ++بل_ps->ژوروالی;
          nullable_تړون (بل_ps, بل, پایله);
        }
    }
  أرجع پایله;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, صحيح قاعده_len, bitset نښه_سېټ)
{
  parse_state_list پایله = parse_state_list_new ();

  صحيح s_اندازه = ps->state_توکي.ټول_اندازه;
  صحيح d_اندازه = ps->derivs.ټول_اندازه;
  إذا (ps->ژوروالی >= 0)
    d_اندازه--;                   // account for dot
  parse_state *new_ریښه = تشه_parse_state ();
  derivation_list popped_derivs =
    parser_راوباسه (ps, d_اندازه - قاعده_len,
                s_اندازه - قاعده_len - 1, new_ریښه);

  // update derivation
  state_توکی *si = (state_توکی *) ps->state_توکي.لکۍ_elt;
  ثابت قاعده *r = توکی_قاعده (si->توکی);
  نښه_شمېره lhs = r->lhs->شمېره;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_توکی_قاعده (si));
  --new_ریښه->ژوروالی;
  ps_derivs_append (new_ریښه, deriv);

  إذا (s_اندازه != قاعده_len + 1)
    {
      state_توکی *لکۍ = (state_توکی *) new_ریښه->state_توکي.لکۍ_elt;
      ps_si_append (new_ریښه, &state_توکي[لکۍ->trans]);
      parse_state_list_append (پایله, new_ریښه);
    }
  وإلا
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ثابت state_توکی *سر_3 = ps->state_توکي.سر_elt;
      state_توکی_list prev = lssi_سرچپه_production (سر_3, نښه_سېټ);
      // TODO: better understand what causes this case.
      إذا (gl_list_اندازه (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_ریښه);
          free_parse_state (new_ریښه);
        }
      وإلا
        {
          state_توکی *psis = عدم;
          لكل (gl_list_iterator_t it = gl_list_iterator (prev);
               state_توکی_list_بل (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_ریښه);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              بنية si_chunk *sis = &copy->state_توکي;
              ثابت state_توکی *لکۍ = sis->لکۍ_elt;
              ps_si_append (copy, &state_توکي[لکۍ->trans]);
              parse_state_list_append (پایله, copy);
              nullable_تړون (copy, (state_توکی *) sis->لکۍ_elt, پایله);
            }
        }
      gl_list_free (prev);
    }
  أرجع پایله;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ثابت state_توکی *سر_3 = ps->state_توکي.سر_elt;
  نښه_شمېره prepend_sym =
    توکی_شمېره_as_نښه_شمېره (*(سر_3->توکی - 1));
  bitset_iterator biter;
  state_توکی_شمېره ساین;
  BITSET_FOR_EACH (biter, سر_3->revs, ساین, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_توکي[ساین]);
    إذا (SI_TRANSITION (سر_3))
      ps_derivs_prepend (copy, derivation_new_ونې_پاڼه (prepend_sym));
    parse_state_list_append (res, copy);
  }
  أرجع res;
}

فراغ
print_parse_state (parse_state *ps)
{
  FILE *بهر = stderr;
  fprintf (بهر, "(size %zu depth %d rc %d)\n",
          ps->state_توکي.ټول_اندازه, ps->ژوروالی, ps->reference_شمېر);
  state_توکی_print (ps->state_توکي.سر_elt, بهر, "");
  state_توکی_print (ps->state_توکي.لکۍ_elt, بهر, "");
  إذا (ps->derivs.ټول_اندازه > 0)
    derivation_print (ps->derivs.سر_elt, بهر, "");
  putc ('\n', بهر);
}
