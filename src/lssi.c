/* Lookahead sensitive state item searches for counterexample generation

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

#ضمن "lssi.h"

#ضمن <gl_linked_list.h>
#ضمن <gl_xlist.h>
#ضمن <stdlib.h>

#ضمن "getargs.h"
#ضمن "nullable.h"

// Lookahead sensitive state item.
تعريفنوع بنية lssi
{
  state_شے_نمبر si;
  بنية lssi *ماپا;
  // this is the precise lookahead set (follow_L from the CupEx paper)
  bitset lookahead;
  bool free_lookahead;
} lssi;

ساكن lssi *
new_lssi (state_شے_نمبر si, lssi *p, bitset l, bool free_l)
{
  lssi *res = xmalloc (حجم *res);
  res->si = si;
  res->ماپا = p;
  res->lookahead = l;
  res->free_lookahead = free_l;
  أرجع res;
}

ساكن فراغ
lssi_free (lssi *sn)
{
  إذا (sn == عدم)
    أرجع;
  إذا (sn->free_lookahead)
    bitset_free (sn->lookahead);
  حرر (sn);
}

ساكن size_t
lssi_hasher (lssi *sn, size_t max)
{
  size_t hash = sn->si;
  bitset_iterator biter;
  علامت_نمبر syn;
  BITSET_FOR_EACH (biter, sn->lookahead, syn, 0)
    hash += syn;
  أرجع hash % max;
}

ساكن bool
lssi_comparator (lssi *s1, lssi *s2)
{
  إذا (s1->si == s2->si)
    {
      إذا (s1->lookahead == s2->lookahead)
        أرجع true;
      أرجع bitset_equal_p (s1->lookahead, s2->lookahead);
    }
  أرجع false;
}

تعريفنوع gl_list_t lssi_list;

ساكن مضمن bool
append_lssi (lssi *sn, Hash_جدول *ویکھیا, lssi_list قطار)
{
  إذا (hash_lookup (ویکھیا, sn))
    {
      sn->free_lookahead = false;
      lssi_free (sn);
      أرجع false;
    }
  hash_xinsert (ویکھیا, sn);
  gl_list_جوڑو_آخری (قطار, sn);
  أرجع true;
}

#إذا 0
ساكن فراغ
lssi_print (lssi *l)
{
  FILE *باہرلا = stderr;
  print_state_شے (&state_اشیاء[l->si], باہرلا);
  إذا (l->lookahead)
    {
      fprintf (باہرلا, "FOLLOWL = { ");
      bitset_iterator biter;
      علامت_نمبر جیب;
      BITSET_FOR_EACH (biter, l->lookahead, جیب, 0)
        fprintf (باہرلا, "%s, \n", symbols[جیب]->ٹیگ);
      fprintf (باہرلا, "}\n");
    }
}
#جے_انت

/**
 * Compute the set of state-items that can reach the given conflict item via
 * a combination of transitions or production steps.
 */
ساكن bitset
eligible_state_اشیاء (state_شے *نشانہ)
{
  bitset نتیجہ = bitset_create (nstate_اشیاء, BITSET_FIXED);
  state_شے_list قطار =
    gl_list_create (GL_LINKED_LIST, عدم, عدم, عدم, true, 1,
                    (ثابت فراغ **) &نشانہ);
  طالما (gl_list_ناپ (قطار) > 0)
    {
      state_شے *si = (state_شے *) gl_list_get_at (قطار, 0);
      gl_list_remove_at (قطار, 0);
      إذا (bitset_test (نتیجہ, si - state_اشیاء))
        استمر;
      bitset_سیٹ (نتیجہ, si - state_اشیاء);
      // search all reverse edges.
      bitset rsi = si->revs;
      bitset_iterator biter;
      state_شے_نمبر جیب;
      BITSET_FOR_EACH (biter, rsi, جیب, 0)
        gl_list_جوڑو_آخری (قطار, &state_اشیاء[جیب]);
    }
  gl_list_free (قطار);
  أرجع نتیجہ;
}

/**
 * Compute the shortest lookahead-sensitive path from the start state to
 * this conflict. If optimized is true, only consider parser states
 * that can reach the conflict state.
 */
state_شے_list
سبھ_توں_تھوڑا_رستہ_from_شروع (state_شے_نمبر نشانہ, علامت_نمبر اگلا_sym)
{
  bitset eligible = eligible_state_اشیاء (&state_اشیاء[نشانہ]);
  Hash_جدول *ویکھیا = hash_initialize (32,
                                         عدم,
                                         (Hash_hasher) lssi_hasher,
                                         (Hash_comparator) lssi_comparator,
                                         (Hash_انکڑے_freer) lssi_free);
  bitset il = bitset_create (nsyms, BITSET_FIXED);
  bitset_سیٹ (il, 0);
  lssi *init = new_lssi (0, عدم, il, true);
  lssi_list قطار = gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم,
                                          عدم, true);
  append_lssi (init, ویکھیا, قطار);
  // breadth-first search
  bool مکمل = false;
  lssi *n;
  طالما (gl_list_ناپ (قطار) > 0)
    {
      n = (lssi *) gl_list_get_at (قطار, 0);
      gl_list_remove_at (قطار, 0);
      state_شے_نمبر آخری = n->si;
      إذا (نشانہ == آخری && bitset_test (n->lookahead, اگلا_sym))
        {
          مکمل = true;
          اكسر;
        }
      state_شے *si = &state_اشیاء[آخری];
      // Transitions don't change follow_L
      إذا (si->trans >= 0)
        {
          إذا (bitset_test (eligible, si->trans))
            {
              lssi *اگلا = new_lssi (si->trans, n, n->lookahead, false);
              append_lssi (اگلا, ویکھیا, قطار);
            }
        }
      // For production steps, follow_L is based on the symbol after the
      // nonterminal being produced.
      // if no such symbol exists, follow_L is unchanged
      // if the symbol is a terminal, follow_L only contains that terminal
      // if the symbol is not nullable, follow_L is its FIRSTS set
      // if the symbol is nullable, follow_L is its FIRSTS set unioned with
      // this logic applied to the next symbol in the rule
      إذا (si->prods)
        {
          // Compute follow_L as above
          bitset lookahead = bitset_create (nsyms, BITSET_FIXED);
          شے_نمبر *pos = si->شے + 1;
          لكل (; !شے_نمبر_is_قاعدہ_نمبر (*pos); ++pos)
            {
              شے_نمبر it = *pos;
              إذا (ISTOKEN (it))
                {
                  bitset_سیٹ (lookahead, it);
                  اكسر;
                }
              وإلا
                {
                  bitset_union (lookahead, lookahead, FIRSTS (it));
                  إذا (!nullable[it - ntokens])
                    اكسر;
                }
            }
          إذا (شے_نمبر_is_قاعدہ_نمبر (*pos))
            bitset_union (lookahead, n->lookahead, lookahead);

          bool lookahead_ورتیا = false;
          // Try all possible production steps within this parser state.
          bitset_iterator biter;
          state_شے_نمبر اگلا_SI;
          BITSET_FOR_EACH (biter, si->prods, اگلا_SI, 0)
            {
              إذا (!bitset_test (eligible, اگلا_SI))
                استمر;
              lssi *اگلا = new_lssi (اگلا_SI, n, lookahead,
                                     !lookahead_ورتیا);
              lookahead_ورتیا = append_lssi (اگلا, ویکھیا, قطار)
                               || lookahead_ورتیا;
            }
          إذا (!lookahead_ورتیا)
            bitset_free (lookahead);
        }
    }

  bitset_free (eligible);
  إذا (!مکمل)
    {
      gl_list_free (قطار);
      fputs ("Cannot find shortest path to conflict state.", stderr);
      abort ();
    }
  state_شے_list res =
    gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, true);
  لكل (lssi *sn = n; sn != عدم; sn = sn->ماپا)
    gl_list_جوڑو_پہلا (res, &state_اشیاء[sn->si]);

  hash_free (ویکھیا);
  gl_list_free (قطار);

  إذا (trace_flag & trace_cex)
    {
      fputs ("REDUCE ITEM PATH:\n", stderr);
      gl_list_iterator_t it = gl_list_iterator (res);
      ثابت فراغ *sip;
      طالما (gl_list_iterator_اگلا (&it, &sip, عدم))
        state_شے_print ((state_شے *) sip, stderr, "");
    }
  أرجع res;
}

/**
 * Determine if the given terminal is in the given symbol set or can begin
 * a nonterminal in the given symbol set.
 */
bool
intersect_علامت (علامت_نمبر sym, bitset syms)
{
  إذا (!syms)
    أرجع true;
  bitset_iterator biter;
  علامت_نمبر sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      إذا (sym == sn)
        أرجع true;
      إذا (ISVAR (sn) && bitset_test (FIRSTS (sn), sym))
        أرجع true;
    }
  أرجع false;
}

/**
 * Determine if any symbol in ts is in syms
 * or can begin a nonterminal syms.
 */
bool
intersect (bitset ts, bitset syms)
{
  إذا (!syms || !ts)
    أرجع true;
  bitset_iterator biter;
  علامت_نمبر sn;
  BITSET_FOR_EACH (biter, syms, sn, 0)
    {
      إذا (bitset_test (ts, sn))
        أرجع true;
      إذا (ISVAR (sn) && !bitset_disjoint_p (ts, FIRSTS (sn)))
        أرجع true;
    }
  أرجع false;
}


/**
 * Compute a list of state_items that have a production to n with respect
 * to its lookahead
 */
state_شے_list
lssi_الٹ_production (ثابت state_شے *si, bitset lookahead)
{
  state_شے_list نتیجہ =
    gl_list_create_خالی_تھاواں (GL_LINKED_LIST, عدم, عدم, عدم, true);
  إذا (SI_TRANSITION (si))
    أرجع نتیجہ;
  // A production step was made to the current lalr_item.
  // Check that the next symbol in the parent lalr_item is
  // compatible with the lookahead.
  bitset_iterator biter;
  state_شے_نمبر جیب;
  BITSET_FOR_EACH (biter, si->revs, جیب, 0)
  {
    state_شے *prevsi = &state_اشیاء[جیب];
    إذا (!production_allowed (prevsi, si))
      استمر;
    bitset prev_lookahead = prevsi->lookahead;
    إذا (شے_نمبر_is_قاعدہ_نمبر (*(prevsi->شے)))
      {
        // reduce item
        // Check that some lookaheads can be preserved.
        إذا (!intersect (prev_lookahead, lookahead))
          استمر;
      }
    وإلا
      {
        // shift item
        إذا (lookahead)
          {
            // Check that lookahead is compatible with the first
            // possible symbols in the rest of the production.
            // Alternatively, if the rest of the production is
            // nullable, the lookahead must be compatible with
            // the lookahead of the corresponding item.
            bool applicable = false;
            bool nlable = true;
            لكل (شے_نمبر *pos = prevsi->شے + 1;
                 !applicable && nlable && شے_نمبر_is_علامت_نمبر (*pos);
                 ++pos)
              {
                علامت_نمبر اگلا_sym = شے_نمبر_as_علامت_نمبر (*pos);
                إذا (ISTOKEN (اگلا_sym))
                  {
                    applicable = intersect_علامت (اگلا_sym, lookahead);
                    nlable = false;
                  }
                وإلا
                  {
                    applicable = intersect (FIRSTS (اگلا_sym), lookahead);
                    إذا (!applicable)
                      nlable = nullable[اگلا_sym - ntokens];
                  }
              }
            إذا (!applicable && !nlable)
              استمر;
          }
      }
    gl_list_جوڑو_آخری (نتیجہ, prevsi);
  }
  أرجع نتیجہ;
}
