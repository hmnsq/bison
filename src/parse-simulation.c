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
    state_شي_list contents;
    // Properties of the linked list this chunk represents.
    ثابت state_شي *سر_elt;
    ثابت state_شي *پڇ_elt;
    size_t ڪل_سائيز;
  } state_شيون;
  // List of derivations of the symbols.
  بنية deriv_chunk
  {
    derivation_list contents;
    ثابت derivation *سر_elt;
    ثابت derivation *پڇ_elt;
    size_t ڪل_سائيز;
  } derivs;
  بنية parse_state *والد;
  صحيح reference_ڳڻپ;
  // Incremented during productions, decremented during reductions.
  صحيح اونهائي;
  // Whether the contents of the chunks should be prepended or
  // appended to the list the chunks represent.
  bool prepend;
  // Causes chunk contents to be freed when the reference count is
  // one. Used when only the chunk metadata will be needed.
  bool free_contents_early;
};


ساكن فراغ
ps_si_prepend (parse_state *ps, ثابت state_شي *si)
{
  بنية si_chunk *sic = &ps->state_شيون;
  gl_list_جوڙيو_پهريون (sic->contents, si);
  sic->سر_elt = si;
  ++sic->ڪل_سائيز;
  إذا (!sic->پڇ_elt)
    sic->پڇ_elt = si;
}

ساكن فراغ
ps_si_append (parse_state *ps, ثابت state_شي *si)
{
  بنية si_chunk *sic = &ps->state_شيون;
  gl_list_جوڙيو_آخري (sic->contents, si);
  sic->پڇ_elt = si;
  ++sic->ڪل_سائيز;
  إذا (!sic->سر_elt)
    sic->سر_elt = si;
}

ساكن فراغ
ps_derivs_prepend (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_prepend (dc->contents, d);
  dc->سر_elt = d;
  ++dc->ڪل_سائيز;
  إذا (!dc->پڇ_elt)
    dc->پڇ_elt = d;
}

ساكن فراغ
ps_derivs_append (parse_state *ps, derivation *d)
{
  بنية deriv_chunk *dc = &ps->derivs;
  derivation_list_append (dc->contents, d);
  dc->پڇ_elt = d;
  ++dc->ڪل_سائيز;
  إذا (!dc->سر_elt)
    dc->سر_elt = d;
}

ساكن صحيح allocs = 0;
ساكن صحيح frees = 0;

ساكن parse_state *
خالي_جايون_parse_state (فراغ)
{
  parse_state *res = xcalloc (1, حجم *res);
  res->state_شيون.contents
    = gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  ++allocs;
  أرجع res;
}

parse_state *
new_parse_state (ثابت state_شي *si)
{
  parse_state *res = خالي_جايون_parse_state ();
  ps_si_append (res, si);
  ps_derivs_append (res, derivation_dot ());
  أرجع res;
}

ساكن parse_state *
copy_parse_state (bool prepend, parse_state *والد)
{
  parse_state *res = xmalloc (حجم *res);
  *res = *والد;
  res->state_شيون.contents
    = gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم, عدم, true);
  res->derivs.contents = derivation_list_new ();
  res->والد = والد;
  res->prepend = prepend;
  res->reference_ڳڻپ = 0;
  res->free_contents_early = false;
  parse_state_retain (والد);
  ++allocs;
  أرجع res;
}

bool
parse_state_derivation_completed (ثابت parse_state *ps)
{
  أرجع ps->derivs.ڪل_سائيز == 1;
}

derivation *
parse_state_derivation (ثابت parse_state *ps)
{
  أرجع (derivation *) ps->derivs.سر_elt;
}

ثابت state_شي *
parse_state_سر (ثابت parse_state *ps)
{
  أرجع ps->state_شيون.سر_elt;
}

ثابت state_شي *
parse_state_پڇ (ثابت parse_state *ps)
{
  أرجع ps->state_شيون.پڇ_elt;
}

صحيح
parse_state_ڊيگهائي (ثابت parse_state *ps)
{
  أرجع ps->state_شيون.ڪل_سائيز;
}

صحيح
parse_state_اونهائي (ثابت parse_state *ps)
{
  أرجع ps->اونهائي;
}

فراغ
parse_state_retain (parse_state *ps)
{
  ++ps->reference_ڳڻپ;
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
  parse_state *والد_ps = عدم;
  لكل (parse_state *ps = original_ps; ps && free_contents; ps = والد_ps)
    {
      --ps->reference_ڳڻپ;
      free_contents = (ps->reference_ڳڻپ == 1 && ps->free_contents_early)
        || (ps->reference_ڳڻپ == 0 && !ps->free_contents_early);
      // need to keep the parse state around for visited hash set,
      // but its contents and parent can be freed
      إذا (free_contents)
        {
          إذا (ps->state_شيون.contents)
            gl_list_free (ps->state_شيون.contents);
          إذا (ps->derivs.contents)
            derivation_list_free (ps->derivs.contents);
        }
      والد_ps = ps->والد;
      إذا (ps->reference_ڳڻپ <= 0)
        {
          حرر (ps);
          ++frees;
        }
    }
}

size_t
parse_state_hasher (ثابت parse_state *ps, size_t max)
{
  ثابت بنية si_chunk *sis = &ps->state_شيون;
  أرجع ((state_شي *) sis->سر_elt - state_شيون +
          (state_شي *) sis->پڇ_elt - state_شيون + sis->ڪل_سائيز) % max;
}

bool
parse_state_comparator (ثابت parse_state *ps1, ثابت parse_state *ps2)
{
  ثابت بنية si_chunk *sis1 = &ps1->state_شيون;
  ثابت بنية si_chunk *sis2 = &ps2->state_شيون;
  أرجع sis1->سر_elt == sis2->سر_elt
    && sis1->پڇ_elt == sis2->پڇ_elt
    && sis1->ڪل_سائيز == sis2->ڪل_سائيز;
}


فراغ
parse_state_completed_قدمن (ثابت parse_state *ps, صحيح *shifts, صحيح *productions)
{
  // traverse to the root parse_state,
  // which will have a list of all completed productions.
  ثابت parse_state *پاڙ_ps = ps;
  طالما (پاڙ_ps->والد)
    پاڙ_ps = پاڙ_ps->والد;

  state_شي_list sis = پاڙ_ps->state_شيون.contents;
  صحيح ڳڻپ = 0;

  state_شي *آخري = عدم;
  state_شي *ايندڙ = عدم;
  لكل (gl_list_iterator_t it = gl_list_iterator (sis);
       state_شي_list_ايندڙ (&it, &ايندڙ);
       )
    {
      إذا (آخري && آخري->state == ايندڙ->state)
        ++ڳڻپ;
      آخري = ايندڙ;
    }
  *productions = ڳڻپ;
  *shifts = پاڙ_ps->state_شيون.ڪل_سائيز - ڳڻپ;
}

تعريفنوع فراغ (*chunk_append_fn) (gl_list_t, ثابت فراغ *);

// A version of gl_list_add_last which has the chunk_append_fn
// signature.
ساكن فراغ
list_جوڙيو_آخري (gl_list_t list, ثابت فراغ *elt)
{
  gl_list_جوڙيو_آخري (list, elt);
}

// takes an array of n gl_lists and flattens them into two list
// based off of the index split
ساكن فراغ
list_flatten_and_ورهايو (gl_list_t *list, gl_list_t *rets, صحيح ورهايو, صحيح n,
                        chunk_append_fn append_fn)
{
  صحيح ret_اشاريو = 0;
  صحيح ret_array = 0;
  لكل (صحيح i = 0; i < n; ++i)
    {
      ثابت فراغ *p = عدم;
      gl_list_iterator_t it = gl_list_iterator (list[i]);
      طالما (gl_list_iterator_ايندڙ (&it, &p, عدم))
        إذا (p)
          {
            gl_list_t l = (gl_list_t) p;
            ثابت فراغ *si = عدم;
            gl_list_iterator_t it2 = gl_list_iterator (l);
            طالما (gl_list_iterator_ايندڙ (&it2, &si, عدم))
              {
                إذا (ret_اشاريو++ == ورهايو)
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
  أرجع gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم,
                               (gl_listelement_dispose_fn)free_parse_state,
                               true);
}

ساكن فراغ
parse_state_list_append (parse_state_list pl, parse_state *ps)
{
  parse_state_retain (ps);
  gl_list_جوڙيو_آخري (pl, ps);
}

// Emulates a reduction on a parse state by popping some amount of
// derivations and state_items off of the parse_state and returning
// the result in ret. Returns the derivation of what's popped.
ساكن derivation_list
parser_ڪڍو (parse_state *ps, صحيح deriv_اشاريو,
            صحيح si_اشاريو, parse_state *ret)
{
  // prepend sis, append sis, prepend derivs, append derivs
  gl_list_t chunks[4];
  لكل (صحيح i = 0; i < 4; ++i)
    chunks[i] = gl_list_create_خالي_جايون (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (parse_state *pn = ps; pn != عدم; pn = pn->والد)
    إذا (pn->prepend)
      {
        gl_list_جوڙيو_آخري (chunks[0], pn->state_شيون.contents);
        gl_list_جوڙيو_آخري (chunks[2], pn->derivs.contents);
      }
    وإلا
      {
        gl_list_جوڙيو_پهريون (chunks[1], pn->state_شيون.contents);
        gl_list_جوڙيو_پهريون (chunks[3], pn->derivs.contents);
      }
  derivation_list popped_derivs = derivation_list_new ();
  gl_list_t ret_chunks[4] = { ret->state_شيون.contents, عدم,
    ret->derivs.contents, popped_derivs
  };
  list_flatten_and_ورهايو (chunks, ret_chunks, si_اشاريو, 2,
                          list_جوڙيو_آخري);
  list_flatten_and_ورهايو (chunks + 2, ret_chunks + 2, deriv_اشاريو, 2,
                          (chunk_append_fn)derivation_list_append);
  size_t s_سائيز = gl_list_سائيز (ret->state_شيون.contents);
  ret->state_شيون.ڪل_سائيز = s_سائيز;
  إذا (s_سائيز > 0)
    {
      ret->state_شيون.پڇ_elt = gl_list_get_at (ret->state_شيون.contents,
                                                  s_سائيز - 1);
      ret->state_شيون.سر_elt =
        gl_list_get_at (ret->state_شيون.contents, 0);
    }
  وإلا
    {
      ret->state_شيون.پڇ_elt = عدم;
      ret->state_شيون.سر_elt = عدم;
    }
  size_t d_سائيز = gl_list_سائيز (ret->derivs.contents);
  ret->derivs.ڪل_سائيز = d_سائيز;
  إذا (d_سائيز > 0)
    {
      ret->derivs.پڇ_elt = gl_list_get_at (ret->derivs.contents,
                                             d_سائيز - 1);
      ret->derivs.سر_elt = gl_list_get_at (ret->derivs.contents, 0);
    }
  وإلا
    {
      ret->derivs.پڇ_elt = عدم;
      ret->derivs.سر_elt = عدم;
    }
  لكل (صحيح i = 0; i < 4; ++i)
    gl_list_free (chunks[i]);
  أرجع popped_derivs;
}

فراغ
parse_state_lists (parse_state *ps, state_شي_list *sitems,
                   derivation_list *derivs)
{
  parse_state *عارضي = خالي_جايون_parse_state ();
  size_t si_سائيز = ps->state_شيون.ڪل_سائيز;
  size_t deriv_سائيز = ps->derivs.ڪل_سائيز;
  derivation_list dl = parser_ڪڍو (ps, si_سائيز, deriv_سائيز, عارضي);
  *sitems = عارضي->state_شيون.contents;
  *derivs = عارضي->derivs.contents;
  // prevent the return lists from being freed
  عارضي->state_شيون.contents = عدم;
  عارضي->derivs.contents = عدم;
  free_parse_state (عارضي);
  derivation_list_free (dl);
}

/**
 * Compute the parse states that result from taking a transition on
 * nullable symbols whenever possible from the given state_item.
 */
ساكن فراغ
nullable_گهيرو (parse_state *ps, state_شي *si, parse_state_list state_list)
{
  parse_state *current_ps = ps;
  state_شي_عدد prev_sin = si - state_شيون;
  لكل (state_شي_عدد جيب = si->trans; جيب != -1;
       prev_sin = جيب, جيب = state_شيون[جيب].trans)
    {
      state_شي *psi = &state_شيون[prev_sin];
      نشان_عدد sp = شي_عدد_as_نشان_عدد (*psi->شي);
      إذا (ISTOKEN (sp) || !nullable[sp - ntokens])
        اكسر;

      state_شي *nsi = &state_شيون[جيب];
      current_ps = copy_parse_state (false, current_ps);
      ps_si_append (current_ps, nsi);
      ps_derivs_append (current_ps,
                        derivation_new (sp, derivation_list_new (),
                                        state_شي_قاعدو (nsi)));
      parse_state_list_append (state_list, current_ps);
    }
}

parse_state_list
simulate_transition (parse_state *ps)
{
  ثابت state_شي *si = ps->state_شيون.پڇ_elt;
  نشان_عدد sym = شي_عدد_as_نشان_عدد (*si->شي);
  // Transition on the same next symbol, taking nullable
  // symbols into account.
  parse_state_list نتيجو = parse_state_list_new ();
  state_شي_عدد si_ايندڙ = si->trans;
  // Check for disabled transition, shouldn't happen as any
  // state_items that lead to these should be disabled.
  إذا (si_ايندڙ < 0)
    أرجع نتيجو;
  parse_state *ايندڙ_ps = copy_parse_state (false, ps);
  ps_si_append (ايندڙ_ps, &state_شيون[si_ايندڙ]);
  ps_derivs_append (ايندڙ_ps, derivation_new_پن (sym));
  parse_state_list_append (نتيجو, ايندڙ_ps);

  nullable_گهيرو (ايندڙ_ps, &state_شيون[si_ايندڙ], نتيجو);
  أرجع نتيجو;
}

/**
 * Determine if the given symbols are equal or their first sets
 * intersect.
 */
ساكن bool
compatible (نشان_عدد sym1, نشان_عدد sym2)
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
simulate_production (parse_state *ps, نشان_عدد compat_sym)
{
  parse_state_list نتيجو = parse_state_list_new ();
  ثابت state_شي *si = parse_state_پڇ (ps);
  إذا (si->prods)
    {
      bitset_iterator biter;
      state_شي_عدد جيب;
      BITSET_FOR_EACH (biter, si->prods, جيب, 0)
        {
          // Take production step only if lhs is not nullable and
          // if first rhs symbol is compatible with compat_sym
          state_شي *ايندڙ = &state_شيون[جيب];
          شي_عدد *itm1 = ايندڙ->شي;
          إذا (!compatible (*itm1, compat_sym) || !production_allowed (si, ايندڙ))
            استمر;
          parse_state *ايندڙ_ps = copy_parse_state (false, ps);
          ps_si_append (ايندڙ_ps, ايندڙ);
          parse_state_list_append (نتيجو, ايندڙ_ps);
          إذا (ايندڙ_ps->اونهائي >= 0)
            ++ايندڙ_ps->اونهائي;
          nullable_گهيرو (ايندڙ_ps, ايندڙ, نتيجو);
        }
    }
  أرجع نتيجو;
}

// simulates a reduction on the given parse state, conflict_item is the
// item associated with ps's conflict. symbol_set is a lookahead set this
// reduction must be compatible with
parse_state_list
simulate_reduction (parse_state *ps, صحيح قاعدو_len, bitset نشان_سيٽ)
{
  parse_state_list نتيجو = parse_state_list_new ();

  صحيح s_سائيز = ps->state_شيون.ڪل_سائيز;
  صحيح d_سائيز = ps->derivs.ڪل_سائيز;
  إذا (ps->اونهائي >= 0)
    d_سائيز--;                   // account for dot
  parse_state *new_پاڙ = خالي_جايون_parse_state ();
  derivation_list popped_derivs =
    parser_ڪڍو (ps, d_سائيز - قاعدو_len,
                s_سائيز - قاعدو_len - 1, new_پاڙ);

  // update derivation
  state_شي *si = (state_شي *) ps->state_شيون.پڇ_elt;
  ثابت قاعدو *r = شي_قاعدو (si->شي);
  نشان_عدد lhs = r->lhs->عدد;
  derivation *deriv = derivation_new (lhs, popped_derivs, state_شي_قاعدو (si));
  --new_پاڙ->اونهائي;
  ps_derivs_append (new_پاڙ, deriv);

  إذا (s_سائيز != قاعدو_len + 1)
    {
      state_شي *پڇ = (state_شي *) new_پاڙ->state_شيون.پڇ_elt;
      ps_si_append (new_پاڙ, &state_شيون[پڇ->trans]);
      parse_state_list_append (نتيجو, new_پاڙ);
    }
  وإلا
    {
      // The head state_item is a production item, so we need to prepend
      // with possible source state-items.
      ثابت state_شي *سر = ps->state_شيون.سر_elt;
      state_شي_list prev = lssi_ابتو_production (سر, نشان_سيٽ);
      // TODO: better understand what causes this case.
      إذا (gl_list_سائيز (prev) == 0)
        {
          // new_root needs to have an RC of 1 to be freed correctly here.
          parse_state_retain (new_پاڙ);
          free_parse_state (new_پاڙ);
        }
      وإلا
        {
          state_شي *psis = عدم;
          لكل (gl_list_iterator_t it = gl_list_iterator (prev);
               state_شي_list_ايندڙ (&it, &psis);
               )
            {
              // Prepend the result from the reverse production.
              parse_state *copy = copy_parse_state (true, new_پاڙ);
              ps_si_prepend (copy, psis);

              // Append the left hand side to the end of the parser state
              copy = copy_parse_state (false, copy);
              بنية si_chunk *sis = &copy->state_شيون;
              ثابت state_شي *پڇ = sis->پڇ_elt;
              ps_si_append (copy, &state_شيون[پڇ->trans]);
              parse_state_list_append (نتيجو, copy);
              nullable_گهيرو (copy, (state_شي *) sis->پڇ_elt, نتيجو);
            }
        }
      gl_list_free (prev);
    }
  أرجع نتيجو;
}

parse_state_list
parser_prepend (parse_state *ps)
{
  parse_state_list res = parse_state_list_new ();
  ثابت state_شي *سر = ps->state_شيون.سر_elt;
  نشان_عدد prepend_sym =
    شي_عدد_as_نشان_عدد (*(سر->شي - 1));
  bitset_iterator biter;
  state_شي_عدد جيب;
  BITSET_FOR_EACH (biter, سر->revs, جيب, 0)
  {
    parse_state *copy = copy_parse_state (true, ps);
    ps_si_prepend (copy, &state_شيون[جيب]);
    إذا (SI_TRANSITION (سر))
      ps_derivs_prepend (copy, derivation_new_پن (prepend_sym));
    parse_state_list_append (res, copy);
  }
  أرجع res;
}

فراغ
print_parse_state (parse_state *ps)
{
  FILE *ٻاهريون = stderr;
  fprintf (ٻاهريون, "(size %zu depth %d rc %d)\n",
          ps->state_شيون.ڪل_سائيز, ps->اونهائي, ps->reference_ڳڻپ);
  state_شي_print (ps->state_شيون.سر_elt, ٻاهريون, "");
  state_شي_print (ps->state_شيون.پڇ_elt, ٻاهريون, "");
  إذا (ps->derivs.ڪل_سائيز > 0)
    derivation_print (ps->derivs.سر_elt, ٻاهريون, "");
  putc ('\n', ٻاهريون);
}
