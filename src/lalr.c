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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>
#شامل <bitsetv.h>

#شامل "complain.h"
#شامل "derives.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "lalr.h"
#شامل "lr0.h"
#شامل "muscle-tab.h"
#شامل "nullable.h"
#شامل "reader.h"
#شامل "relation.h"
#شامل "symtab.h"

goto_عدد *goto_map = پوچ;
goto_عدد ngotos = 0;
state_عدد *from_state = پوچ;
state_عدد *to_state = پوچ;
bitsetv goto_follows = پوچ;

/* Linked list of goto numbers.  */
تعریفنوع ساختار goto_list
{
  ساختار goto_list *next;
  goto_عدد مقدار;
} goto_list;

ایستا goto_list *
goto_list_new (goto_عدد مقدار, ساختار goto_list *next)
{
  goto_list *res = xmalloc (اندازه *res);
  res->next = next;
  res->مقدار = مقدار;
  بازگشت res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

ایستا bitsetv LA = پوچ;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
ایستا goto_عدد **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
ایستا goto_list **lookback;

ایستا تهی
goto_print (goto_عدد i, FILE *out)
{
  ثابت state_عدد src = from_state[i];
  ثابت state_عدد dst = to_state[i];
  نماد_عدد var = states[dst]->accessing_نماد;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

تهی
set_goto_map (تهی)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  اگر (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, اندازه *goto_map);
  ngotos = 0;
  برای (state_عدد s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      برای (صحیح i = trans->رقم - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_عدد_MAXIMUM);
          goto_map[TRANSITION_نماد (trans, i) - ntokens]++;
        }
    }

  goto_عدد *temp_map = xnmalloc (nnterms + 1, اندازه *temp_map);
  {
    goto_عدد k = 0;
    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, اندازه *from_state);
  to_state = xcalloc (ngotos, اندازه *to_state);

  برای (state_عدد s = 0; s < nstates; ++s)
    {
      ثابت transitions *trans = states[s]->transitions;
      برای (صحیح i = trans->رقم - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_عدد k = temp_map[TRANSITION_نماد (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->عدد;
        }
    }

  آزاد (temp_map);

  اگر (trace_flag & trace_automaton)
    {
      برای (صحیح i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      برای (صحیح i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_عدد
map_goto (state_عدد src, نماد_عدد sym)
{
  goto_عدد کم = goto_map[sym - ntokens];
  أكد (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_عدد بالا = goto_map[sym - ntokens + 1] - 1;

  برای (;;)
    {
      aver (کم <= بالا);
      goto_عدد میانه = (کم + بالا) / 2;
      state_عدد s = from_state[میانه];
      اگر (s == src)
        بازگشت میانه;
      وگرنه اگر (s < src)
        کم = میانه + 1;
      وگرنه
        بالا = میانه - 1;
    }
}

/* Print FOLLOWS for debugging.  */
ایستا تهی
follows_print (ثابت نویسه* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  برای (goto_عدد i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      نماد_عدد sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
ایستا تهی
initialize_goto_follows (تهی)
{
  goto_عدد **reads = xnmalloc (ngotos, اندازه *reads);
  goto_عدد *edge = xnmalloc (ngotos, اندازه *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  برای (goto_عدد i = 0; i < ngotos; ++i)
    {
      state_عدد dst = to_state[i];
      ثابت transitions *trans = states[dst]->transitions;

      صحیح j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_نماد (trans, j));

      /* Gotos outgoing from DST. */
      goto_عدد nedges = 0;
      برای (; j < trans->رقم; ++j)
        {
          نماد_عدد sym = TRANSITION_نماد (trans, j);
          اگر (nullable[sym - ntokens])
            {
              أكد (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      اگر (nedges == 0)
        reads[i] = پوچ;
      وگرنه
        {
          reads[i] = xnmalloc (nedges + 1, اندازه reads[i][0]);
          memcpy (reads[i], edge, nedges * اندازه edge[0]);
          reads[i][nedges] = END_گره;
        }
    }
  اگر (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  اگر (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  برای (goto_عدد i = 0; i < ngotos; ++i)
    آزاد (reads[i]);
  آزاد (reads);
  آزاد (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
ایستا ثابت state *
lookback_find_state (صحیح lookback_index)
{
  state *res = پوچ;
  برای (صحیح j = 0; j < nstates; ++j)
    اگر (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        اگر (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          بشکن;
        وگرنه
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  اگر (!res)
    abort ();
  بازگشت res;
}

/* Print LOOKBACK for debugging.  */
ایستا تهی
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  برای (صحیح i = 0; i < nLA; ++i)
    اگر (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      ثابت state *s = lookback_find_state (i);
      صحیح rnum = i - (s->reductions->lookaheads - LA);
      ثابت rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->عدد);
      rule_print (r, پوچ, out);
      fputs (") ->", out);
      برای (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->مقدار, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
ایستا تهی
add_lookback_edge (state *s, rule ثابت *r, goto_عدد gotono)
{
  صحیح ri = state_reduction_find (s, r);
  صحیح idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
ایستا تهی
build_relations (تهی)
{
  goto_عدد *edge = xnmalloc (ngotos, اندازه *edge);
  state_عدد *path = xnmalloc (ritem_longest_rhs () + 1, اندازه *path);

  includes = xnmalloc (ngotos, اندازه *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  برای (goto_عدد i = 0; i < ngotos; ++i)
    {
      ثابت state_عدد src = from_state[i];
      ثابت state_عدد dst = to_state[i];
      نماد_عدد var = states[dst]->accessing_نماد;

      /* Size of EDGE.  */
      صحیح nedges = 0;
      برای (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule ثابت *r = *rulep;
          state *s = states[src];
          path[0] = s->عدد;

          /* Length of PATH.  */
          صحیح length = 1;
          برای (item_عدد ثابت *rp = r->rhs; 0 <= *rp; rp++)
            {
              نماد_عدد sym = item_عدد_as_نماد_عدد (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->عدد;
            }

          /* S is the end of PATH.  */
          اگر (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          برای (صحیح p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              نماد_عدد sym = item_عدد_as_نماد_عدد (r->rhs[p]);
              goto_عدد g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                برای (صحیح j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                اگر (!found)
                  {
                    أكد (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              اگر (!nullable[sym - ntokens])
                بشکن;
            }
        }

      اگر (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          برای (صحیح j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      اگر (nedges == 0)
        includes[i] = پوچ;
      وگرنه
        {
          includes[i] = xnmalloc (nedges + 1, اندازه includes[i][0]);
          برای (صحیح j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_گره;
        }
    }

  آزاد (edge);
  آزاد (path);

  relation_transpose (&includes, ngotos);
  اگر (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
ایستا تهی
compute_follows (تهی)
{
  relation_digraph (includes, ngotos, goto_follows);
  اگر (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  برای (goto_عدد i = 0; i < ngotos; ++i)
    آزاد (includes[i]);
  آزاد (includes);
}


ایستا تهی
compute_lookaheads (تهی)
{
  اگر (trace_flag & trace_automaton)
      lookback_print (stderr);

  برای (size_t i = 0; i < nLA; ++i)
    برای (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->مقدار]);

  /* Free LOOKBACK. */
  برای (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  آزاد (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

ایستا صحیح
state_lookaheads_شمار (state *s, bool default_reduction_only_for_accept)
{
  ثابت reductions *reds = s->reductions;
  ثابت transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->رقم == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->رقم > 1
      || (reds->رقم == 1 && trans->رقم && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->رقم == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  بازگشت s->consistent ? 0 : reds->رقم;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

تهی
initialize_LA (تهی)
{
  bool default_reduction_only_for_accept;
  {
    نویسه *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    آزاد (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  برای (state_عدد i = 0; i < nstates; ++i)
    nLA += state_lookaheads_شمار (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  اگر (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      صحیح شمار = state_lookaheads_شمار (states[i],
                                          default_reduction_only_for_accept);
      اگر (شمار)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += شمار;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

ایستا تهی
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  برای (state_عدد i = 0; i < nstates; ++i)
    {
      ثابت reductions *reds = states[i]->reductions;
      اگر (reds->رقم)
        {
          fprintf (out, "  State %d:\n", i);
          برای (صحیح j = 0; j < reds->رقم; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->عدد);
              اگر (reds->lookaheads)
              {
                bitset_iterator iter;
                صحیح k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

تهی
lalr (تهی)
{
  اگر (trace_flag & trace_automaton)
    {
      fputc ('\n', stderr);
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: begin");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
  initialize_LA ();
  set_goto_map ();
  initialize_goto_follows ();
  lookback = xcalloc (nLA, اندازه *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  اگر (trace_flag & trace_sets)
    lookaheads_print (stderr);
  اگر (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


تهی
lalr_update_state_numbers (state_عدد old_to_new[], state_عدد nstates_old)
{
  goto_عدد ngotos_reachable = 0;
  نماد_عدد nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  برای (goto_عدد i = 0; i < ngotos; ++i)
    {
      وقتی (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      اگر (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  وقتی (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


تهی
lalr_free (تهی)
{
  برای (state_عدد s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = پوچ;
  bitsetv_free (LA);
}
