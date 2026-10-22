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
    state_شے_list contents;
    // Properties of the linked list this chunk represents.
    ثابت state_شے *سر_elt;
    ثابت state_شے *پوچھ_elt;
    size_t کل_ناپ;
  } state_اشیاء;
  // List of derivations of the symbols.
  بنية deriv_chunk
  {
    derivation_list contents;
    ثابت derivation *سر_elt;
    ثابت derivation *پوچھ_elt;
    size_t کل_ناپ;
  } derivs;
  بنية parse_state *ماپا;
  صحيح reference_گنتی;
  // Incremented during productions, decremented during reductions.
  صحيح ڈونگھائی;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ساكن فراغ
ps_si_prepend (parse_state *ps, ثابت state_شے *si)
{
  بنية si_chunk *sic = &ps->state_اشیاء;
  gl_list_جوڑو_پہلا (sic->contents, si);
  sic->سر_elt = si;
  ++sic->کل_ناپ;
  إذا (!sic->پوچھ_elt)
    sic->پوچھ_elt = si;
}

ساكن فراغ
ps_si_append (parse_state *ps, ثابت state_شے *si)
{
  بنية si_chunk *sic = &ps->state_اشیاء;
  gl_list_جوڑو_آخری (sic->contents, si);
  sic->پوچھ_elt = si;
  ++sic->کل_ناپ;
  إذا (!sic->سر_elt)
    sic->سر_elt = si;
}

ساكن فراغ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->سر_elt = d;
  ++dc->کل_ناپ;
  إذا (!dc->پوچھ_elt)
    dc->پوچھ_elt = d;
}

ساكن فراغ
ps_derivs_append (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->پوچھ_elt = d;
  ++dc->کل_ناپ;
  إذا (!dc->سر_elt)
    dc->سر_elt = d;
}

ساكن صحيح allocs = 0;
ساكن صحيح frees = 0;

ساكن parse_state *
خالی_تھاواں_parse_state (فراغ)
{
  parse_state *res = xcalloc (1, حجم *res);
  res->state_اشیاء.contents
    = gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  أرجع res;
}

parse_state *
new_parse_state (ثابت state_شے *si)
{
  parse_state *res = خالی_تھاواں_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  أرجع res;
}

ساكن parse_state *
copy_parse_state (bool prepend, parse_state *ماپا)
{
  parse_state *res = xmalloc (حجم *res);
  *res = *ماپا;
  res->state_اشیاء.contents
    = gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  res->ماپا = ماپا;
  res->prepend = prepend;
  res->reference_گنتی = 0;
  res->free_contents_early = false;
  parse_state_retain (ماپا);
  ++allocs;
  أرجع res;
}

bool
parse_state_derivation_completed (ثابت parse_state *ps)
{
  أرجع ps->derivs.کل_ناپ == 1;
}

derivation *
parse_state_derivation (ثابت parse_state *ps)
{
  أرجع (derivation *) ps->derivs.سر_elt;
}

ثابت state_شے *
parse_state_سر (ثابت parse_state *ps)
{
  أرجع ps->state_اشیاء.سر_elt;
}

ثابت state_شے *
parse_state_پوچھ (ثابت parse_state *ps)
{
  أرجع ps->state_اشیاء.پوچھ_elt;
}

صحيح
parse_state_لمائی (ثابت parse_state *ps)
{
  أرجع ps->state_اشیاء.کل_ناپ;
}

صحيح
parse_state_ڈونگھائی (ثابت parse_state *ps)
{
  أرجع ps->ڈونگھائی;
}

فراغ
parse_state_retain (parse_state *ps)
{
  ++ps->reference_گنتی;
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
  parse_state *ماپا_ps = عدم;
  لكل (parse_state *ps = original_ps; ps && free_contents; ps = ماپا_ps)
    {
      --ps->reference_گنتی;
      free_contents = (ps->reference_گنتی == 1 && ps->free_contents_early)
        || (ps->reference_گنتی == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      إذا (free_contents)
        {
          إذا (ps->state_اشیاء.contents)
            gl_list_free (ps->state_اشیاء.contents);
          إذا (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      ماپا_ps = ps->ماپا;
      إذا (ps->reference_گنتی <= 0)
        {
          حرر (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ثابت parse_state *ps, size_t max)
{
  ثابت بنية si_chunk *sis = &ps->state_اشیاء;
  أرجع ((state_شے *) sis->سر_elt - state_اشیاء +
          (state_شے *) sis->پوچھ_elt - state_اشیاء + sis->کل_ناپ) % max;
}

bool
parse_state_comparator (ثابت parse_state *ps1, ثابت parse_state *ps2)
{
  ثابت بنية si_chunk *sis1 = &ps1->state_اشیاء;
  ثابت بنية si_chunk *sis2 = &ps2->state_اشیاء;
  أرجع sis1->سر_elt == sis2->سر_elt
    && sis1->پوچھ_elt == sis2->پوچھ_elt
    && sis1->کل_ناپ == sis2->کل_ناپ;
}


فراغ
parse_state_completed_قدماں (ثابت parse_state *ps, صحيح *shifts, صحيح *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ثابت parse_state *جڑ_ps = ps;
  طالما (جڑ_ps->ماپا)
    جڑ_ps = جڑ_ps->ماپا;

  state_شے_list sis = جڑ_ps->state_اشیاء.contents;
  صحيح گنتی = 0;

  state_شے *آخری = عدم;
  state_شے *اگلا = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (sis);
       state_شے_list_اگلا (&it, &اگلا);
       )
    {
      إذا (آخری && آخری->state == اگلا->state)
        ++گنتی;
      آخری = اگلا;
    }
  *productions = گنتی;
  *shifts = جڑ_ps->state_اشیاء.کل_ناپ - گنتی;
}

تعريفنوع فراغ (*chunk_append_fn) (gl_list_t, ثابت فراغ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ساكن فراغ
list_جوڑو_آخری (gl_list_t list, ثابت فراغ *elt)
{
  gl_list_جوڑو_آخری (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ساكن فراغ
list_flatten_and_ونڈو (gl_list_t *list, gl_list_t *rets, صحيح ونڈو, صحيح n,
                        chunk_append_fn append_fn)
{
  صحيح ret_اشاریہ = 0;
  صحيح ret_array = 0;
  لكل (صحيح i = 0; i < n; ++i)
    {
      ثابت فراغ *p = عدم;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      طالما (gl_list_iterator_اگلا (&it, &p, عدم))
        إذا (p)
          {
            gl_list_t l = (gl_list_t) p;
            ثابت فراغ *si = عدم;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            طالما (gl_list_iterator_اگلا (&it2, &si, عدم))
              {
                إذا (ret_اشاریہ++ == ونڈو)
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
  أرجع gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ساكن فراغ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_جوڑو_آخری (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ساكن derivation_list
parser_نکالو (parse_state *ps, صحيح deriv_اشاریہ,
            صحيح si_اشاریہ, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  لكل (صحيح i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (parse_state *pn = ps; pn != عدم; pn = pn->ماپا)
    إذا (pn->prepend)
      {
        gl_list_جوڑو_آخری (chunks[0], pn->state_اشیاء.contents);
        gl_list_جوڑو_آخری (chunks[2], pn->derivs.contents);
      }
    وإلا
      {
        gl_list_جوڑو_پہلا (chunks[1], pn->state_اشیاء.contents);
        gl_list_جوڑو_پہلا (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_اشیاء.contents, عدم,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_ونڈو (chunks, ret_chunks, si_اشاریہ, 2,
                          list_جوڑو_آخری);
  list_flatten_and_ونڈو (chunks + 2, ret_chunks + 2, deriv_اشاریہ, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_ناپ = gl_list_ناپ (ret->state_اشیاء.contents);
  ret->state_اشیاء.کل_ناپ = s_ناپ;
  إذا (s_ناپ > 0)
    {
      ret->state_اشیاء.پوچھ_elt = gl_list_get_at (ret->state_اشیاء.contents,
                                                  s_ناپ - 1);
      ret->state_اشیاء.سر_elt =
        gl_list_get_at (ret->state_اشیاء.contents, 0);
    }
  وإلا
    {
      ret->state_اشیاء.پوچھ_elt = عدم;
      ret->state_اشیاء.سر_elt = عدم;
    }
  size_t d_ناپ = gl_list_ناپ (ret->derivs.contents);
  ret->derivs.کل_ناپ = d_ناپ;
  إذا (d_ناپ > 0)
    {
      ret->derivs.پوچھ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_ناپ - 1);
      ret->derivs.سر_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  وإلا
    {
      ret->derivs.پوچھ_elt = عدم;
      ret->derivs.سر_elt = عدم;
    }
  لكل (صحيح i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  أرجع popped_derivs;
}

فراغ
parse_state_lists (parse_state *ps, state_شے_list *sitems,
                   derivation_list *derivs)
{
  parse_state *عارضی = خالی_تھاواں_parse_state ();
  size_t si_ناپ = ps->state_اشیاء.کل_ناپ;
  size_t deriv_ناپ = ps->derivs.کل_ناپ;
  derivation_list dl = parser_نکالو (ps, si_ناپ, deriv_ناپ, عارضی);
  *sitems = عارضی->state_اشیاء.contents;
  *derivs = عارضی->derivs.contents;
  // prevent the return lists from being freed
  عارضی->state_اشیاء.contents = عدم;
  عارضی->derivs.contents = عدم;
  free_parse_state (عارضی);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ساكن فراغ
nullable_گھیرا (parse_state *ps, state_شے *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_شے_نمبر prev_sin = si - state_اشیاء;
  لكل (state_شے_نمبر جیب = si->trans; جیب != -1;
       prev_sin = جیب, جیب = state_اشیاء[جیب].trans)
    {
      state_شے *psi = &state_اشیاء[prev_sin];
      علامت_نمبر sp = شے_نمبر_as_علامت_نمبر (*psi->شے);
      إذا (ISTOKEN (sp) || !nullable[sp - ntokens])
        اكسر;

      state_شے *nsi = &state_اشیاء[جیب];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_شے_قاعدہ (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  ثابت state_شے *si = ps->state_اشیاء.پوچھ_elt;
  علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (*si->شے);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list نتیجہ = parse_state_list_new ();
  state_شے_نمبر si_اگلا = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  إذا (si_اگلا < 0)
    أرجع نتیجہ;
  parse_state *اگلا_ps = copy_parse_state (false, ps);
  ps_si_append (اگلا_ps, &state_اشیاء[si_اگلا]);
  ps_derivs_append (اگلا_ps, derivation_new_پتی (sym));
  parse_state_list_append (نتیجہ, اگلا_ps);

  nullable_گھیرا (اگلا_ps, &state_اشیاء[si_اگلا], نتیجہ);
  أرجع نتیجہ;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ساكن bool
compatible (علامت_نمبر sym1, علامت_نمبر sym2)
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
simulate_production (parse_state *ps, علامت_نمبر compat_sym)
{
  parse_state_list نتیجہ = parse_state_list_new ();
  ثابت state_شے *si = parse_state_پوچھ (ps);
  إذا (si->prods)
    {
      bitset_iterator biter;
      state_شے_نمبر جیب;
      BITSET_FOR_EACH (biter, si->prods, جیب, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_شے *اگلا = &state_اشیاء[جیب];
          شے_نمبر *itm1 = اگلا->شے;
          إذا (!compatible (*itm1, compat_sym) || !production_allowed (si, اگلا))
            استمر;
          parse_state *اگلا_ps = copy_parse_state (false, ps);
          ps_si_append (اگلا_ps, اگلا);
          parse_state_list_append (نتیجہ, اگلا_ps);
          إذا (اگلا_ps->ڈونگھائی >= 0)
            ++اگلا_ps->ڈونگھائی;
          nullable_گھیرا (اگلا_ps, اگلا, نتیجہ);
        }
    }
  أرجع نتیجہ;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, صحيح قاعدہ_len, bitset علامت_سیٹ)
{
  parse_state_list نتیجہ = parse_state_list_new ();

  صحيح s_ناپ = ps->state_اشیاء.کل_ناپ;
  صحيح d_ناپ = ps->derivs.کل_ناپ;
  إذا (ps->ڈونگھائی >= 0)
    d_ناپ--;                   // account for dot
  parse_state *new_جڑ = خالی_تھاواں_parse_state ();
  derivation_list popped_derivs =
    parser_نکالو (ps, d_ناپ - قاعدہ_len,
                s_ناپ - قاعدہ_len - 1, new_جڑ);

  // update derivation
  state_شے *si = (state_شے *) ps->state_اشیاء.پوچھ_elt;
  ثابت قاعدہ *r = شے_قاعدہ (si->شے);
  علامت_نمبر lhs = r->lhs->نمبر;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_شے_قاعدہ (si));
  --new_جڑ->ڈونگھائی;
  ps_derivs_append (new_جڑ, deriv);

  إذا (s_ناپ != قاعدہ_len + 1)
    {
      state_شے *پوچھ = (state_شے *) new_جڑ->state_اشیاء.پوچھ_elt;
      ps_si_append (new_جڑ, &state_اشیاء[پوچھ->trans]);
      parse_state_list_append (نتیجہ, new_جڑ);
    }
  وإلا
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ثابت state_شے *سر = ps->state_اشیاء.سر_elt;
      state_شے_list prev = lssi_الٹ_production (سر, علامت_سیٹ);
      // TODO: better understand what causes this case.
      إذا (gl_list_ناپ (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_جڑ);
          free_parse_state (new_جڑ);
        }
      وإلا
        {
          state_شے *psis = عدم;
          لكل (gl_list_iterator_t it = gl_list_iterator (prev);
               state_شے_list_اگلا (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_جڑ);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              بنية si_chunk *sis = &copy->state_اشیاء;
              ثابت state_شے *پوچھ = sis->پوچھ_elt;
              ps_si_append (copy, &state_اشیاء[پوچھ->trans]);
              parse_state_list_append (نتیجہ, copy);
              nullable_گھیرا (copy, (state_شے *) sis->پوچھ_elt, نتیجہ);
            }
        }
      gl_list_free (prev);
    }
  أرجع نتیجہ;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ثابت state_شے *سر = ps->state_اشیاء.سر_elt;
  علامت_نمبر prepend_sym =
    شے_نمبر_as_علامت_نمبر (*(سر->شے - 1));
  bitset_iterator biter;
  state_شے_نمبر جیب;
  BITSET_FOR_EACH (biter, سر->revs, جیب, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_اشیاء[جیب]);
    إذا (SI_TRANSITION (سر))
      ps_derivs_prepend (copy, derivation_new_پتی (prepend_sym));
    parse_state_list_append (res, copy);
  }
  أرجع res;
}

فراغ
print_parse_state (parse_state *ps)
{
  FILE *باہرلا = stderr;
  fprintf (باہرلا, "(size %zu depth %d rc %d)\n",
          ps->state_اشیاء.کل_ناپ, ps->ڈونگھائی, ps->reference_گنتی);
  state_شے_print (ps->state_اشیاء.سر_elt, باہرلا, "");
  state_شے_print (ps->state_اشیاء.پوچھ_elt, باہرلا, "");
  إذا (ps->derivs.کل_ناپ > 0)
    derivation_print (ps->derivs.سر_elt, باہرلا, "");
  putc ('\n', باہرلا);
}
