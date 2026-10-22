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

goto_عدد *goto_map = عدم;
goto_عدد ngotos = 0;
state_عدد *from_state = عدم;
state_عدد *منزل_state = عدم;
bitsetv goto_follows = عدم;

/* Linked list of goto numbers.  */
تعريفنوع بنية goto_list
{
  بنية goto_list *ايندڙ;
  goto_عدد قدر;
} goto_list;

ساكن goto_list *
goto_list_new (goto_عدد قدر, بنية goto_list *ايندڙ)
{
  goto_list *res = xmalloc (حجم *res);
  res->ايندڙ = ايندڙ;
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
ساكن goto_عدد **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
ساكن goto_list **lookback;

ساكن فراغ
goto_print (goto_عدد i, FILE *ٻاهريون)
{
  ثابت state_عدد src = from_state[i];
  ثابت state_عدد dst = منزل_state[i];
  نشان_عدد var = حالتون[dst]->accessing_نشان;
  fprintf (ٻاهريون,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->ٽيگ, dst);
}

فراغ
سيٽ_goto_map (فراغ)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  إذا (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, حجم *goto_map);
  ngotos = 0;
  لكل (state_عدد s = 0; s < nstates; ++s)
    {
      transitions *trans = حالتون[s]->transitions;
      لكل (صحيح i = trans->انگ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_عدد_وڌ_کان_وڌ);
          goto_map[TRANSITION_نشان (trans, i) - ntokens]++;
        }
    }

  goto_عدد *عارضي_map = xnmalloc (nnterms + 1, حجم *عارضي_map);
  {
    goto_عدد k = 0;
    لكل (نشان_عدد i = ntokens; i < nsyms; ++i)
      {
        عارضي_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    لكل (نشان_عدد i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = عارضي_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    عارضي_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, حجم *from_state);
  منزل_state = xcalloc (ngotos, حجم *منزل_state);

  لكل (state_عدد s = 0; s < nstates; ++s)
    {
      ثابت transitions *trans = حالتون[s]->transitions;
      لكل (صحيح i = trans->انگ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_عدد k = عارضي_map[TRANSITION_نشان (trans, i) - ntokens]++;
          from_state[k] = s;
          منزل_state[k] = trans->حالتون[i]->عدد;
        }
    }

  حرر (عارضي_map);

  إذا (trace_flag & trace_automaton)
    {
      لكل (صحيح i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->ٽيگ,
                 goto_map[i], goto_map[i+1] - 1);
      لكل (صحيح i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_عدد
map_goto (state_عدد src, نشان_عدد sym)
{
  goto_عدد هيٺيون = goto_map[sym - ntokens];
  أكد (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_عدد مٿاهون = goto_map[sym - ntokens + 1] - 1;

  لكل (;;)
    {
      aver (هيٺيون <= مٿاهون);
      goto_عدد وچ = (هيٺيون + مٿاهون) / 2;
      state_عدد s = from_state[وچ];
      إذا (s == src)
        أرجع وچ;
      وإلا إذا (s < src)
        هيٺيون = وچ + 1;
      وإلا
        مٿاهون = وچ - 1;
    }
}

/* Print FOLLOWS for debugging.  */
ساكن فراغ
follows_print (ثابت حرف* title, FILE *ٻاهريون)
{
  fprintf (ٻاهريون, "%s:\n", title);
  لكل (goto_عدد i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", ٻاهريون);
      goto_print (i, ٻاهريون);
      fputs ("] =", ٻاهريون);
      bitset_iterator iter;
      نشان_عدد sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (ٻاهريون, " %s", symbols[sym]->ٽيگ);
      fputc ('\n', ٻاهريون);
    }
  fputc ('\n', ٻاهريون);
}

/* Build goto_follows. */
ساكن فراغ
initialize_goto_follows (فراغ)
{
  goto_عدد **reads = xnmalloc (ngotos, حجم *reads);
  goto_عدد *ڪنارو = xnmalloc (ngotos, حجم *ڪنارو);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  لكل (goto_عدد i = 0; i < ngotos; ++i)
    {
      state_عدد dst = منزل_state[i];
      ثابت transitions *trans = حالتون[dst]->transitions;

      صحيح j;
      FOR_EACH_سرڪايو (trans, j)
        bitset_سيٽ (goto_follows[i], TRANSITION_نشان (trans, j));

      /* Gotos outgoing from DST. */
      goto_عدد nedges = 0;
      لكل (; j < trans->انگ; ++j)
        {
          نشان_عدد sym = TRANSITION_نشان (trans, j);
          إذا (nullable[sym - ntokens])
            {
              أكد (nedges < ngotos);
              ڪنارو[nedges++] = map_goto (dst, sym);
            }
        }

      إذا (nedges == 0)
        reads[i] = عدم;
      وإلا
        {
          reads[i] = xnmalloc (nedges + 1, حجم reads[i][0]);
          يادگيري_نقل (reads[i], ڪنارو, nedges * حجم ڪنارو[0]);
          reads[i][nedges] = پڇاڙي_ڳنڍ;
        }
    }
  إذا (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      لاڳاپو_print ("reads", reads, ngotos, goto_print, stderr);
    }

  لاڳاپو_digraph (reads, ngotos, goto_follows);
  إذا (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  لكل (goto_عدد i = 0; i < ngotos; ++i)
    حرر (reads[i]);
  حرر (reads);
  حرر (ڪنارو);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
ساكن ثابت state *
lookback_ڳوليو_state (صحيح lookback_اشاريو)
{
  state *res = عدم;
  لكل (صحيح j = 0; j < nstates; ++j)
    إذا (حالتون[j]->reductions
        && حالتون[j]->reductions->lookaheads)
      {
        إذا (حالتون[j]->reductions->lookaheads - LA > lookback_اشاريو)
          /* Went too far. */
          اكسر;
        وإلا
          res = حالتون[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  إذا (!res)
    abort ();
  أرجع res;
}

/* Print LOOKBACK for debugging.  */
ساكن فراغ
lookback_print (FILE *ٻاهريون)
{
  fputs ("lookback:\n", ٻاهريون);
  لكل (صحيح i = 0; i < nLA; ++i)
    إذا (lookback[i])
    {
      fprintf (ٻاهريون, "   %3d = ", i);
      ثابت state *s = lookback_ڳوليو_state (i);
      صحيح rnum = i - (s->reductions->lookaheads - LA);
      ثابت قاعدو *r = s->reductions->قاعدا[rnum];
      fprintf (ٻاهريون, "(%3d, ", s->عدد);
      قاعدو_print (r, عدم, ٻاهريون);
      fputs (") ->", ٻاهريون);
      لكل (goto_list *sp = lookback[i]; sp; sp = sp->ايندڙ)
        {
          fputc (' ', ٻاهريون);
          goto_print (sp->قدر, ٻاهريون);
        }
      fputc ('\n', ٻاهريون);
    }
  fputc ('\n', ٻاهريون);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
ساكن فراغ
جوڙيو_lookback_ڪنارو (state *s, قاعدو ثابت *r, goto_عدد gotono)
{
  صحيح ri = state_reduction_ڳوليو (s, r);
  صحيح idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
ساكن فراغ
build_relations (فراغ)
{
  goto_عدد *ڪنارو = xnmalloc (ngotos, حجم *ڪنارو);
  state_عدد *رستو = xnmalloc (ritem_longest_rhs () + 1, حجم *رستو);

  includes = xnmalloc (ngotos, حجم *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  لكل (goto_عدد i = 0; i < ngotos; ++i)
    {
      ثابت state_عدد src = from_state[i];
      ثابت state_عدد dst = منزل_state[i];
      نشان_عدد var = حالتون[dst]->accessing_نشان;

      /* Size of EDGE.  */
      صحيح nedges = 0;
      لكل (قاعدو **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          قاعدو ثابت *r = *rulep;
          state *s = حالتون[src];
          رستو[0] = s->عدد;

          /* Length of PATH.  */
          صحيح ڊيگهائي = 1;
          لكل (شي_عدد ثابت *rp = r->rhs; 0 <= *rp; rp++)
            {
              نشان_عدد sym = شي_عدد_as_نشان_عدد (*rp);
              s = transitions_منزل (s, sym);
              رستو[ڊيگهائي++] = s->عدد;
            }

          /* S is the end of PATH.  */
          إذا (!s->consistent)
            جوڙيو_lookback_ڪنارو (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          لكل (صحيح p = ڊيگهائي - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              نشان_عدد sym = شي_عدد_as_نشان_عدد (r->rhs[p]);
              goto_عدد g = map_goto (رستو[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool مليو = false;
                لكل (صحيح j = 0; !مليو && j < nedges; ++j)
                  مليو = ڪنارو[j] == g;
                إذا (!مليو)
                  {
                    أكد (nedges < ngotos);
                    ڪنارو[nedges++] = g;
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
              goto_print (ڪنارو[j], stderr);
            }
          fputc ('\n', stderr);
        }

      إذا (nedges == 0)
        includes[i] = عدم;
      وإلا
        {
          includes[i] = xnmalloc (nedges + 1, حجم includes[i][0]);
          لكل (صحيح j = 0; j < nedges; ++j)
            includes[i][j] = ڪنارو[j];
          includes[i][nedges] = پڇاڙي_ڳنڍ;
        }
    }

  حرر (ڪنارو);
  حرر (رستو);

  لاڳاپو_transpose (&includes, ngotos);
  إذا (trace_flag & trace_automaton)
    لاڳاپو_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
ساكن فراغ
compute_follows (فراغ)
{
  لاڳاپو_digraph (includes, ngotos, goto_follows);
  إذا (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  لكل (goto_عدد i = 0; i < ngotos; ++i)
    حرر (includes[i]);
  حرر (includes);
}


ساكن فراغ
compute_lookaheads (فراغ)
{
  إذا (trace_flag & trace_automaton)
      lookback_print (stderr);

  لكل (size_t i = 0; i < nLA; ++i)
    لكل (goto_list *sp = lookback[i]; sp; sp = sp->ايندڙ)
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
state_lookaheads_ڳڻپ (state *s, bool default_reduction_only_for_accept)
{
  ثابت reductions *reds = s->reductions;
  ثابت transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->انگ == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->انگ > 1
      || (reds->انگ == 1 && trans->انگ && TRANSITION_IS_سرڪايو (trans, 0))
      || (reds->انگ == 1 && !قاعدو_is_initial (reds->قاعدا[0])
          && default_reduction_only_for_accept));

  أرجع s->consistent ? 0 : reds->انگ;
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
  لكل (state_عدد i = 0; i < nstates; ++i)
    nLA += state_lookaheads_ڳڻپ (حالتون[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  إذا (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  لكل (state_عدد i = 0; i < nstates; ++i)
    {
      صحيح ڳڻپ = state_lookaheads_ڳڻپ (حالتون[i],
                                          default_reduction_only_for_accept);
      إذا (ڳڻپ)
        {
          حالتون[i]->reductions->lookaheads = pLA;
          pLA += ڳڻپ;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

ساكن فراغ
lookaheads_print (FILE *ٻاهريون)
{
  fputs ("Lookaheads:\n", ٻاهريون);
  لكل (state_عدد i = 0; i < nstates; ++i)
    {
      ثابت reductions *reds = حالتون[i]->reductions;
      إذا (reds->انگ)
        {
          fprintf (ٻاهريون, "  State %d:\n", i);
          لكل (صحيح j = 0; j < reds->انگ; ++j)
            {
              fprintf (ٻاهريون, "    rule %d:", reds->قاعدا[j]->عدد);
              إذا (reds->lookaheads)
              {
                bitset_iterator iter;
                صحيح k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (ٻاهريون, " %s", symbols[k]->ٽيگ);
              }
              fputc ('\n', ٻاهريون);
            }
        }
    }
  fputc ('\n', ٻاهريون);
}

فراغ
lalr (فراغ)
{
  إذا (trace_flag & trace_automaton)
    {
      fputc ('\n', stderr);
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: begin");
      پڇاڙي_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
  initialize_LA ();
  سيٽ_goto_map ();
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
      پڇاڙي_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


فراغ
lalr_update_state_numbers (state_عدد old_منزل_new[], state_عدد nstates_old)
{
  goto_عدد ngotos_reachable = 0;
  نشان_عدد nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  لكل (goto_عدد i = 0; i < ngotos; ++i)
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
  لكل (state_عدد s = 0; s < nstates; ++s)
    حالتون[s]->reductions->lookaheads = عدم;
  bitsetv_free (LA);
}
