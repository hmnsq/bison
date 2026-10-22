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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>
#અન્તર્ભાવ <bitsetv.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "derives.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "lalr.h"
#અન્તર્ભાવ "lr0.h"
#અન્તર્ભાવ "muscle-tab.h"
#અન્તર્ભાવ "nullable.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "relation.h"
#અન્તર્ભાવ "symtab.h"

goto_સંખ્યા *goto_map = ખાલી;
goto_સંખ્યા ngotos = 0;
state_સંખ્યા *from_state = ખાલી;
state_સંખ્યા *to_state = ખાલી;
bitsetv goto_follows = ખાલી;

/* Linked list of goto numbers.  */
પ્રકારવ્યાખ્યા સંરચના goto_list
{
  સંરચના goto_list *next;
  goto_સંખ્યા કિંમત;
} goto_list;

સ્થિર goto_list *
goto_list_new (goto_સંખ્યા કિંમત, સંરચના goto_list *next)
{
  goto_list *res = xmalloc (કદ *res);
  res->next = next;
  res->કિંમત = કિંમત;
  પાછા res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

સ્થિર bitsetv LA = ખાલી;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
સ્થિર goto_સંખ્યા **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
સ્થિર goto_list **lookback;

સ્થિર શૂન્ય
goto_print (goto_સંખ્યા i, FILE *out)
{
  અચલ state_સંખ્યા src = from_state[i];
  અચલ state_સંખ્યા dst = to_state[i];
  પ્રતીક_સંખ્યા var = states[dst]->accessing_પ્રતીક;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

શૂન્ય
set_goto_map (શૂન્ય)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  જો (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, કદ *goto_map);
  ngotos = 0;
  માટે (state_સંખ્યા s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      માટે (પૂર્ણાંક i = trans->અંક - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_સંખ્યા_MAXIMUM);
          goto_map[TRANSITION_પ્રતીક (trans, i) - ntokens]++;
        }
    }

  goto_સંખ્યા *temp_map = xnmalloc (nnterms + 1, કદ *temp_map);
  {
    goto_સંખ્યા k = 0;
    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, કદ *from_state);
  to_state = xcalloc (ngotos, કદ *to_state);

  માટે (state_સંખ્યા s = 0; s < nstates; ++s)
    {
      અચલ transitions *trans = states[s]->transitions;
      માટે (પૂર્ણાંક i = trans->અંક - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_સંખ્યા k = temp_map[TRANSITION_પ્રતીક (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->સંખ્યા;
        }
    }

  મુક્ત (temp_map);

  જો (trace_flag & trace_automaton)
    {
      માટે (પૂર્ણાંક i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      માટે (પૂર્ણાંક i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_સંખ્યા
map_goto (state_સંખ્યા src, પ્રતીક_સંખ્યા sym)
{
  goto_સંખ્યા નીચલું = goto_map[sym - ntokens];
  પ્રતિજ્ઞા (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_સંખ્યા ઉપલું = goto_map[sym - ntokens + 1] - 1;

  માટે (;;)
    {
      aver (નીચલું <= ઉપલું);
      goto_સંખ્યા મધ્ય = (નીચલું + ઉપલું) / 2;
      state_સંખ્યા s = from_state[મધ્ય];
      જો (s == src)
        પાછા મધ્ય;
      નહીંતર જો (s < src)
        નીચલું = મધ્ય + 1;
      નહીંતર
        ઉપલું = મધ્ય - 1;
    }
}

/* Print FOLLOWS for debugging.  */
સ્થિર શૂન્ય
follows_print (અચલ અક્ષર* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  માટે (goto_સંખ્યા i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      પ્રતીક_સંખ્યા sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
સ્થિર શૂન્ય
initialize_goto_follows (શૂન્ય)
{
  goto_સંખ્યા **reads = xnmalloc (ngotos, કદ *reads);
  goto_સંખ્યા *edge = xnmalloc (ngotos, કદ *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  માટે (goto_સંખ્યા i = 0; i < ngotos; ++i)
    {
      state_સંખ્યા dst = to_state[i];
      અચલ transitions *trans = states[dst]->transitions;

      પૂર્ણાંક j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_પ્રતીક (trans, j));

      /* Gotos outgoing from DST. */
      goto_સંખ્યા nedges = 0;
      માટે (; j < trans->અંક; ++j)
        {
          પ્રતીક_સંખ્યા sym = TRANSITION_પ્રતીક (trans, j);
          જો (nullable[sym - ntokens])
            {
              પ્રતિજ્ઞા (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      જો (nedges == 0)
        reads[i] = ખાલી;
      નહીંતર
        {
          reads[i] = xnmalloc (nedges + 1, કદ reads[i][0]);
          memcpy (reads[i], edge, nedges * કદ edge[0]);
          reads[i][nedges] = END_ગાંઠ;
        }
    }
  જો (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  જો (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  માટે (goto_સંખ્યા i = 0; i < ngotos; ++i)
    મુક્ત (reads[i]);
  મુક્ત (reads);
  મુક્ત (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
સ્થિર અચલ state *
lookback_find_state (પૂર્ણાંક lookback_index)
{
  state *res = ખાલી;
  માટે (પૂર્ણાંક j = 0; j < nstates; ++j)
    જો (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        જો (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          તોડો;
        નહીંતર
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  જો (!res)
    abort ();
  પાછા res;
}

/* Print LOOKBACK for debugging.  */
સ્થિર શૂન્ય
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  માટે (પૂર્ણાંક i = 0; i < nLA; ++i)
    જો (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      અચલ state *s = lookback_find_state (i);
      પૂર્ણાંક rnum = i - (s->reductions->lookaheads - LA);
      અચલ rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->સંખ્યા);
      rule_print (r, ખાલી, out);
      fputs (") ->", out);
      માટે (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->કિંમત, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
સ્થિર શૂન્ય
add_lookback_edge (state *s, rule અચલ *r, goto_સંખ્યા gotono)
{
  પૂર્ણાંક ri = state_reduction_find (s, r);
  પૂર્ણાંક idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
સ્થિર શૂન્ય
build_relations (શૂન્ય)
{
  goto_સંખ્યા *edge = xnmalloc (ngotos, કદ *edge);
  state_સંખ્યા *path = xnmalloc (ritem_longest_rhs () + 1, કદ *path);

  includes = xnmalloc (ngotos, કદ *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  માટે (goto_સંખ્યા i = 0; i < ngotos; ++i)
    {
      અચલ state_સંખ્યા src = from_state[i];
      અચલ state_સંખ્યા dst = to_state[i];
      પ્રતીક_સંખ્યા var = states[dst]->accessing_પ્રતીક;

      /* Size of EDGE.  */
      પૂર્ણાંક nedges = 0;
      માટે (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule અચલ *r = *rulep;
          state *s = states[src];
          path[0] = s->સંખ્યા;

          /* Length of PATH.  */
          પૂર્ણાંક length = 1;
          માટે (item_સંખ્યા અચલ *rp = r->rhs; 0 <= *rp; rp++)
            {
              પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->સંખ્યા;
            }

          /* S is the end of PATH.  */
          જો (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          માટે (પૂર્ણાંક p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              પ્રતીક_સંખ્યા sym = item_સંખ્યા_as_પ્રતીક_સંખ્યા (r->rhs[p]);
              goto_સંખ્યા g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                માટે (પૂર્ણાંક j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                જો (!found)
                  {
                    પ્રતિજ્ઞા (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              જો (!nullable[sym - ntokens])
                તોડો;
            }
        }

      જો (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          માટે (પૂર્ણાંક j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      જો (nedges == 0)
        includes[i] = ખાલી;
      નહીંતર
        {
          includes[i] = xnmalloc (nedges + 1, કદ includes[i][0]);
          માટે (પૂર્ણાંક j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_ગાંઠ;
        }
    }

  મુક્ત (edge);
  મુક્ત (path);

  relation_transpose (&includes, ngotos);
  જો (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
સ્થિર શૂન્ય
compute_follows (શૂન્ય)
{
  relation_digraph (includes, ngotos, goto_follows);
  જો (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  માટે (goto_સંખ્યા i = 0; i < ngotos; ++i)
    મુક્ત (includes[i]);
  મુક્ત (includes);
}


સ્થિર શૂન્ય
compute_lookaheads (શૂન્ય)
{
  જો (trace_flag & trace_automaton)
      lookback_print (stderr);

  માટે (size_t i = 0; i < nLA; ++i)
    માટે (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->કિંમત]);

  /* Free LOOKBACK. */
  માટે (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  મુક્ત (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

સ્થિર પૂર્ણાંક
state_lookaheads_ગણતરી (state *s, bool default_reduction_only_for_accept)
{
  અચલ reductions *reds = s->reductions;
  અચલ transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->અંક == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->અંક > 1
      || (reds->અંક == 1 && trans->અંક && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->અંક == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  પાછા s->consistent ? 0 : reds->અંક;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

શૂન્ય
initialize_LA (શૂન્ય)
{
  bool default_reduction_only_for_accept;
  {
    અક્ષર *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    મુક્ત (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    nLA += state_lookaheads_ગણતરી (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  જો (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      પૂર્ણાંક ગણતરી = state_lookaheads_ગણતરી (states[i],
                                          default_reduction_only_for_accept);
      જો (ગણતરી)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += ગણતરી;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

સ્થિર શૂન્ય
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  માટે (state_સંખ્યા i = 0; i < nstates; ++i)
    {
      અચલ reductions *reds = states[i]->reductions;
      જો (reds->અંક)
        {
          fprintf (out, "  State %d:\n", i);
          માટે (પૂર્ણાંક j = 0; j < reds->અંક; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->સંખ્યા);
              જો (reds->lookaheads)
              {
                bitset_iterator iter;
                પૂર્ણાંક k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

શૂન્ય
lalr (શૂન્ય)
{
  જો (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, કદ *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  જો (trace_flag & trace_sets)
    lookaheads_print (stderr);
  જો (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


શૂન્ય
lalr_update_state_numbers (state_સંખ્યા old_to_new[], state_સંખ્યા nstates_old)
{
  goto_સંખ્યા ngotos_reachable = 0;
  પ્રતીક_સંખ્યા nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  માટે (goto_સંખ્યા i = 0; i < ngotos; ++i)
    {
      જ્યારે (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      જો (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  જ્યારે (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


શૂન્ય
lalr_free (શૂન્ય)
{
  માટે (state_સંખ્યા s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = ખાલી;
  bitsetv_free (LA);
}
