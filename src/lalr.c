/* Compute lookahead criteria for Bison.

   Copyright (C) 1984, 1986, 1989, 2000-2015, 2018-2022 Free Software
   Foundation, Inc.

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


/* Find which rules need lookahead in each state, and which lookahead
   tokens they accept.  */

#ضمن <config.h>
#ضمن "system.h"

#ضمن <bitset.h>
#ضمن <bitsetv.h>

#ضمن "complain.h"
#ضمن "derives.h"
#ضمن "getargs.h"
#ضمن "gram.h"
#ضمن "lalr.h"
#ضمن "lr0.h"
#ضمن "muscle-tab.h"
#ضمن "nullable.h"
#ضمن "reader.h"
#ضمن "relation.h"
#ضمن "symtab.h"

goto_نمبر *goto_map = عدم;
goto_نمبر ngotos = 0;
state_نمبر *from_state = عدم;
state_نمبر *منزل_state = عدم;
bitsetv goto_follows = عدم;

/* Linked list of goto numbers.  */
تعريفنوع بنية goto_list
{
  بنية goto_list *اگلا;
  goto_نمبر قدر;
} goto_list;

ساكن goto_list *
goto_list_new (goto_نمبر قدر, بنية goto_list *اگلا)
{
  goto_list *res = xmalloc (حجم *res);
  res->اگلا = اگلا;
  res->قدر = قدر;
  أرجع res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

ساكن bitsetv LA = عدم;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
ساكن goto_نمبر **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
ساكن goto_list **lookback;

ساكن فراغ
goto_print (goto_نمبر i, FILE *باہرلا)
{
  ثابت state_نمبر src = from_state[i];
  ثابت state_نمبر dst = منزل_state[i];
  علامت_نمبر var = حالتاں[dst]->accessing_علامت;
  fprintf (باہرلا,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->ٹیگ, dst);
}

فراغ
سیٹ_goto_map (فراغ)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, حجم *goto_map);
  ngotos = 0;
  لكل (state_نمبر s = 0; s < nstates; ++s)
    {
      transitions *trans = حالتاں[s]->transitions;
      لكل (صحيح i = trans->عدد - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_نمبر_ودھ_توں_ودھ);
          goto_map[TRANSITION_علامت (trans, i) - ntokens]++;
        }
    }

  goto_نمبر *عارضی_map = xnmalloc (nnterms + 1, حجم *عارضی_map);
  {
    goto_نمبر k = 0;
    لكل (علامت_نمبر i = ntokens; i < nsyms; ++i)
      {
        عارضی_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    لكل (علامت_نمبر i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = عارضی_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    عارضی_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, حجم *from_state);
  منزل_state = xcalloc (ngotos, حجم *منزل_state);

  لكل (state_نمبر s = 0; s < nstates; ++s)
    {
      ثابت transitions *trans = حالتاں[s]->transitions;
      لكل (صحيح i = trans->عدد - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_نمبر k = عارضی_map[TRANSITION_علامت (trans, i) - ntokens]++;
          from_state[k] = s;
          منزل_state[k] = trans->حالتاں[i]->نمبر;
        }
    }

  حرر (عارضی_map);

  إذا (trace_flag & trace_automaton)
    {
      لكل (صحيح i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->ٹیگ,
                 goto_map[i], goto_map[i+1] - 1);
      لكل (صحيح i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_نمبر
map_goto (state_نمبر src, علامت_نمبر sym)
{
  goto_نمبر کم = goto_map[sym - ntokens];
  أكد (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_نمبر بلند = goto_map[sym - ntokens + 1] - 1;

  لكل (;;)
    {
      aver (کم <= بلند);
      goto_نمبر درمیان = (کم + بلند) / 2;
      state_نمبر s = from_state[درمیان];
      إذا (s == src)
        أرجع درمیان;
      وإلا إذا (s < src)
        کم = درمیان + 1;
      وإلا
        بلند = درمیان - 1;
    }
}

/* Print FOLLOWS for debugging.  */
ساكن فراغ
follows_print (ثابت حرف* title, FILE *باہرلا)
{
  fprintf (باہرلا, "%s:\n", title);
  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", باہرلا);
      goto_print (i, باہرلا);
      fputs ("] =", باہرلا);
      bitset_iterator iter;
      علامت_نمبر sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (باہرلا, " %s", symbols[sym]->ٹیگ);
      fputc ('\n', باہرلا);
    }
  fputc ('\n', باہرلا);
}

/* Build goto_follows. */
ساكن فراغ
initialize_goto_follows (فراغ)
{
  goto_نمبر **reads = xnmalloc (ngotos, حجم *reads);
  goto_نمبر *کنارا = xnmalloc (ngotos, حجم *کنارا);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    {
      state_نمبر dst = منزل_state[i];
      ثابت transitions *trans = حالتاں[dst]->transitions;

      صحيح j;
      FOR_EACH_کھسکاؤ (trans, j)
        bitset_سیٹ (goto_follows[i], TRANSITION_علامت (trans, j));

      /* Gotos outgoing from DST. */
      goto_نمبر nedges = 0;
      لكل (; j < trans->عدد; ++j)
        {
          علامت_نمبر sym = TRANSITION_علامت (trans, j);
          إذا (nullable[sym - ntokens])
            {
              أكد (nedges < ngotos);
              کنارا[nedges++] = map_goto (dst, sym);
            }
        }

      إذا (nedges == 0)
        reads[i] = عدم;
      وإلا
        {
          reads[i] = xnmalloc (nedges + 1, حجم reads[i][0]);
          حافظہ_نقل (reads[i], کنارا, nedges * حجم کنارا[0]);
          reads[i][nedges] = سرا_گرہ;
        }
    }
  إذا (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      سمبندھ_print ("reads", reads, ngotos, goto_print, stderr);
    }

  سمبندھ_digraph (reads, ngotos, goto_follows);
  إذا (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    حرر (reads[i]);
  حرر (reads);
  حرر (کنارا);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
ساكن ثابت state *
lookback_لبھو_state (صحيح lookback_اشاریہ)
{
  state *res = عدم;
  لكل (صحيح j = 0; j < nstates; ++j)
    إذا (حالتاں[j]->reductions
        && حالتاں[j]->reductions->lookaheads)
      {
        إذا (حالتاں[j]->reductions->lookaheads - LA > lookback_اشاریہ)
          /* Went too far. */
          اكسر;
        وإلا
          res = حالتاں[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  إذا (!res)
    abort ();
  أرجع res;
}

/* Print LOOKBACK for debugging.  */
ساكن فراغ
lookback_print (FILE *باہرلا)
{
  fputs ("lookback:\n", باہرلا);
  لكل (صحيح i = 0; i < nLA; ++i)
    إذا (lookback[i])
    {
      fprintf (باہرلا, "   %3d = ", i);
      ثابت state *s = lookback_لبھو_state (i);
      صحيح rnum = i - (s->reductions->lookaheads - LA);
      ثابت قاعدہ *r = s->reductions->قاعدے[rnum];
      fprintf (باہرلا, "(%3d, ", s->نمبر);
      قاعدہ_print (r, عدم, باہرلا);
      fputs (") ->", باہرلا);
      لكل (goto_list *sp = lookback[i]; sp; sp = sp->اگلا)
        {
          fputc (' ', باہرلا);
          goto_print (sp->قدر, باہرلا);
        }
      fputc ('\n', باہرلا);
    }
  fputc ('\n', باہرلا);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
ساكن فراغ
جوڑو_lookback_کنارا (state *s, قاعدہ ثابت *r, goto_نمبر gotono)
{
  صحيح ri = state_reduction_لبھو (s, r);
  صحيح idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
ساكن فراغ
build_relations (فراغ)
{
  goto_نمبر *کنارا = xnmalloc (ngotos, حجم *کنارا);
  state_نمبر *رستہ = xnmalloc (ritem_longest_rhs () + 1, حجم *رستہ);

  includes = xnmalloc (ngotos, حجم *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    {
      ثابت state_نمبر src = from_state[i];
      ثابت state_نمبر dst = منزل_state[i];
      علامت_نمبر var = حالتاں[dst]->accessing_علامت;

      /* Size of EDGE.  */
      صحيح nedges = 0;
      لكل (قاعدہ **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          قاعدہ ثابت *r = *rulep;
          state *s = حالتاں[src];
          رستہ[0] = s->نمبر;

          /* Length of PATH.  */
          صحيح لمائی = 1;
          لكل (شے_نمبر ثابت *rp = r->rhs; 0 <= *rp; rp++)
            {
              علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (*rp);
              s = transitions_منزل (s, sym);
              رستہ[لمائی++] = s->نمبر;
            }

          /* S is the end of PATH.  */
          إذا (!s->consistent)
            جوڑو_lookback_کنارا (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          لكل (صحيح p = لمائی - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              علامت_نمبر sym = شے_نمبر_as_علامت_نمبر (r->rhs[p]);
              goto_نمبر g = map_goto (رستہ[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool لبھیا = false;
                لكل (صحيح j = 0; !لبھیا && j < nedges; ++j)
                  لبھیا = کنارا[j] == g;
                إذا (!لبھیا)
                  {
                    أكد (nedges < ngotos);
                    کنارا[nedges++] = g;
                  }
              }
              إذا (!nullable[sym - ntokens])
                اكسر;
            }
        }

      إذا (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          لكل (صحيح j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (کنارا[j], stderr);
            }
          fputc ('\n', stderr);
        }

      إذا (nedges == 0)
        includes[i] = عدم;
      وإلا
        {
          includes[i] = xnmalloc (nedges + 1, حجم includes[i][0]);
          لكل (صحيح j = 0; j < nedges; ++j)
            includes[i][j] = کنارا[j];
          includes[i][nedges] = سرا_گرہ;
        }
    }

  حرر (کنارا);
  حرر (رستہ);

  سمبندھ_transpose (&includes, ngotos);
  إذا (trace_flag & trace_automaton)
    سمبندھ_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
ساكن فراغ
compute_follows (فراغ)
{
  سمبندھ_digraph (includes, ngotos, goto_follows);
  إذا (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    حرر (includes[i]);
  حرر (includes);
}


ساكن فراغ
compute_lookaheads (فراغ)
{
  إذا (trace_flag & trace_automaton)
      lookback_print (stderr);

  لكل (size_t i = 0; i < nLA; ++i)
    لكل (goto_list *sp = lookback[i]; sp; sp = sp->اگلا)
      bitset_or (LA[i], LA[i], goto_follows[sp->قدر]);

  /* Free LOOKBACK. */
  لكل (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  حرر (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

ساكن صحيح
state_lookaheads_گنتی (state *s, bool default_reduction_only_for_accept)
{
  ثابت reductions *reds = s->reductions;
  ثابت transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->عدد == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->عدد > 1
      || (reds->عدد == 1 && trans->عدد && TRANSITION_IS_کھسکاؤ (trans, 0))
      || (reds->عدد == 1 && !قاعدہ_is_initial (reds->قاعدے[0])
          && default_reduction_only_for_accept));

  أرجع s->consistent ? 0 : reds->عدد;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

فراغ
initialize_LA (فراغ)
{
  bool default_reduction_only_for_accept;
  {
    حرف *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    حرر (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  لكل (state_نمبر i = 0; i < nstates; ++i)
    nLA += state_lookaheads_گنتی (حالتاں[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  إذا (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      صحيح گنتی = state_lookaheads_گنتی (حالتاں[i],
                                          default_reduction_only_for_accept);
      إذا (گنتی)
        {
          حالتاں[i]->reductions->lookaheads = pLA;
          pLA += گنتی;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

ساكن فراغ
lookaheads_print (FILE *باہرلا)
{
  fputs ("Lookaheads:\n", باہرلا);
  لكل (state_نمبر i = 0; i < nstates; ++i)
    {
      ثابت reductions *reds = حالتاں[i]->reductions;
      إذا (reds->عدد)
        {
          fprintf (باہرلا, "  State %d:\n", i);
          لكل (صحيح j = 0; j < reds->عدد; ++j)
            {
              fprintf (باہرلا, "    rule %d:", reds->قاعدے[j]->نمبر);
              إذا (reds->lookaheads)
              {
                bitset_iterator iter;
                صحيح k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (باہرلا, " %s", symbols[k]->ٹیگ);
              }
              fputc ('\n', باہرلا);
            }
        }
    }
  fputc ('\n', باہرلا);
}

فراغ
lalr (فراغ)
{
  إذا (trace_flag & trace_automaton)
    {
      fputc ('\n', stderr);
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: begin");
      سرا_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
  initialize_LA ();
  سیٹ_goto_map ();
  initialize_goto_follows ();
  lookback = xcalloc (nLA, حجم *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  إذا (trace_flag & trace_sets)
    lookaheads_print (stderr);
  إذا (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      سرا_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


فراغ
lalr_update_state_numbers (state_نمبر old_منزل_new[], state_نمبر nstates_old)
{
  goto_نمبر ngotos_reachable = 0;
  علامت_نمبر nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  لكل (goto_نمبر i = 0; i < ngotos; ++i)
    {
      طالما (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      إذا (old_منزل_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_منزل_new[منزل_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_منزل_new[from_state[i]];
          منزل_state[ngotos_reachable] = old_منزل_new[منزل_state[i]];
          ++ngotos_reachable;
        }
    }
  طالما (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


فراغ
lalr_free (فراغ)
{
  لكل (state_نمبر s = 0; s < nstates; ++s)
    حالتاں[s]->reductions->lookaheads = عدم;
  bitsetv_free (LA);
}
