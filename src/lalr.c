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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>
#अन्तर्भाव <bitsetv.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "derives.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "lalr.h"
#अन्तर्भाव "lr0.h"
#अन्तर्भाव "muscle-tab.h"
#अन्तर्भाव "nullable.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "relation.h"
#अन्तर्भाव "symtab.h"

goto_सङ्ख्या *goto_map = रिक्त;
goto_सङ्ख्या ngotos = 0;
state_सङ्ख्या *from_state = रिक्त;
state_सङ्ख्या *to_state = रिक्त;
bitsetv goto_follows = रिक्त;

/* Linked list of goto numbers.  */
प्रकारनिर्देश संरचना goto_list
{
  संरचना goto_list *next;
  goto_सङ्ख्या मानम्;
} goto_list;

स्थिर goto_list *
goto_list_new (goto_सङ्ख्या मानम्, संरचना goto_list *next)
{
  goto_list *res = xmalloc (परिमाण *res);
  res->next = next;
  res->मानम् = मानम्;
  प्रत्यागम res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

स्थिर bitsetv LA = रिक्त;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
स्थिर goto_सङ्ख्या **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
स्थिर goto_list **lookback;

स्थिर शून्य
goto_print (goto_सङ्ख्या i, FILE *out)
{
  ध्रुव state_सङ्ख्या src = from_state[i];
  ध्रुव state_सङ्ख्या dst = to_state[i];
  चिह्नम्_सङ्ख्या var = states[dst]->accessing_चिह्नम्;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

शून्य
set_goto_map (शून्य)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  जइ (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, परिमाण *goto_map);
  ngotos = 0;
  प्रति (state_सङ्ख्या s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      प्रति (पूर्णाङ्क i = trans->अङ्कः - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_सङ्ख्या_MAXIMUM);
          goto_map[TRANSITION_चिह्नम् (trans, i) - ntokens]++;
        }
    }

  goto_सङ्ख्या *temp_map = xnmalloc (nnterms + 1, परिमाण *temp_map);
  {
    goto_सङ्ख्या k = 0;
    प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    प्रति (चिह्नम्_सङ्ख्या i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, परिमाण *from_state);
  to_state = xcalloc (ngotos, परिमाण *to_state);

  प्रति (state_सङ्ख्या s = 0; s < nstates; ++s)
    {
      ध्रुव transitions *trans = states[s]->transitions;
      प्रति (पूर्णाङ्क i = trans->अङ्कः - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_सङ्ख्या k = temp_map[TRANSITION_चिह्नम् (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->सङ्ख्या;
        }
    }

  मुक्त (temp_map);

  जइ (trace_flag & trace_automaton)
    {
      प्रति (पूर्णाङ्क i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      प्रति (पूर्णाङ्क i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_सङ्ख्या
map_goto (state_सङ्ख्या src, चिह्नम्_सङ्ख्या sym)
{
  goto_सङ्ख्या नीचः = goto_map[sym - ntokens];
  प्रतिज्ञा (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_सङ्ख्या उच्चः = goto_map[sym - ntokens + 1] - 1;

  प्रति (;;)
    {
      aver (नीचः <= उच्चः);
      goto_सङ्ख्या मध्यः = (नीचः + उच्चः) / 2;
      state_सङ्ख्या s = from_state[मध्यः];
      जइ (s == src)
        प्रत्यागम मध्यः;
      अण्णहा जइ (s < src)
        नीचः = मध्यः + 1;
      अण्णहा
        उच्चः = मध्यः - 1;
    }
}

/* Print FOLLOWS for debugging.  */
स्थिर शून्य
follows_print (ध्रुव वर्ण* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      चिह्नम्_सङ्ख्या sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
स्थिर शून्य
initialize_goto_follows (शून्य)
{
  goto_सङ्ख्या **reads = xnmalloc (ngotos, परिमाण *reads);
  goto_सङ्ख्या *edge = xnmalloc (ngotos, परिमाण *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      state_सङ्ख्या dst = to_state[i];
      ध्रुव transitions *trans = states[dst]->transitions;

      पूर्णाङ्क j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_चिह्नम् (trans, j));

      /* Gotos outgoing from DST. */
      goto_सङ्ख्या nedges = 0;
      प्रति (; j < trans->अङ्कः; ++j)
        {
          चिह्नम्_सङ्ख्या sym = TRANSITION_चिह्नम् (trans, j);
          जइ (nullable[sym - ntokens])
            {
              प्रतिज्ञा (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      जइ (nedges == 0)
        reads[i] = रिक्त;
      अण्णहा
        {
          reads[i] = xnmalloc (nedges + 1, परिमाण reads[i][0]);
          memcpy (reads[i], edge, nedges * परिमाण edge[0]);
          reads[i][nedges] = END_ग्रन्थिः;
        }
    }
  जइ (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  जइ (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    मुक्त (reads[i]);
  मुक्त (reads);
  मुक्त (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
स्थिर ध्रुव state *
lookback_find_state (पूर्णाङ्क lookback_index)
{
  state *res = रिक्त;
  प्रति (पूर्णाङ्क j = 0; j < nstates; ++j)
    जइ (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        जइ (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          भङ्ग;
        अण्णहा
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  जइ (!res)
    abort ();
  प्रत्यागम res;
}

/* Print LOOKBACK for debugging.  */
स्थिर शून्य
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  प्रति (पूर्णाङ्क i = 0; i < nLA; ++i)
    जइ (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      ध्रुव state *s = lookback_find_state (i);
      पूर्णाङ्क rnum = i - (s->reductions->lookaheads - LA);
      ध्रुव rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->सङ्ख्या);
      rule_print (r, रिक्त, out);
      fputs (") ->", out);
      प्रति (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->मानम्, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
स्थिर शून्य
add_lookback_edge (state *s, rule ध्रुव *r, goto_सङ्ख्या gotono)
{
  पूर्णाङ्क ri = state_reduction_find (s, r);
  पूर्णाङ्क idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
स्थिर शून्य
build_relations (शून्य)
{
  goto_सङ्ख्या *edge = xnmalloc (ngotos, परिमाण *edge);
  state_सङ्ख्या *path = xnmalloc (ritem_longest_rhs () + 1, परिमाण *path);

  includes = xnmalloc (ngotos, परिमाण *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      ध्रुव state_सङ्ख्या src = from_state[i];
      ध्रुव state_सङ्ख्या dst = to_state[i];
      चिह्नम्_सङ्ख्या var = states[dst]->accessing_चिह्नम्;

      /* Size of EDGE.  */
      पूर्णाङ्क nedges = 0;
      प्रति (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule ध्रुव *r = *rulep;
          state *s = states[src];
          path[0] = s->सङ्ख्या;

          /* Length of PATH.  */
          पूर्णाङ्क length = 1;
          प्रति (item_सङ्ख्या ध्रुव *rp = r->rhs; 0 <= *rp; rp++)
            {
              चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->सङ्ख्या;
            }

          /* S is the end of PATH.  */
          जइ (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          प्रति (पूर्णाङ्क p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              चिह्नम्_सङ्ख्या sym = item_सङ्ख्या_as_चिह्नम्_सङ्ख्या (r->rhs[p]);
              goto_सङ्ख्या g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                प्रति (पूर्णाङ्क j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                जइ (!found)
                  {
                    प्रतिज्ञा (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              जइ (!nullable[sym - ntokens])
                भङ्ग;
            }
        }

      जइ (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          प्रति (पूर्णाङ्क j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      जइ (nedges == 0)
        includes[i] = रिक्त;
      अण्णहा
        {
          includes[i] = xnmalloc (nedges + 1, परिमाण includes[i][0]);
          प्रति (पूर्णाङ्क j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_ग्रन्थिः;
        }
    }

  मुक्त (edge);
  मुक्त (path);

  relation_transpose (&includes, ngotos);
  जइ (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
स्थिर शून्य
compute_follows (शून्य)
{
  relation_digraph (includes, ngotos, goto_follows);
  जइ (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    मुक्त (includes[i]);
  मुक्त (includes);
}


स्थिर शून्य
compute_lookaheads (शून्य)
{
  जइ (trace_flag & trace_automaton)
      lookback_print (stderr);

  प्रति (size_t i = 0; i < nLA; ++i)
    प्रति (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->मानम्]);

  /* Free LOOKBACK. */
  प्रति (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  मुक्त (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

स्थिर पूर्णाङ्क
state_lookaheads_गणनम् (state *s, bool default_reduction_only_for_accept)
{
  ध्रुव reductions *reds = s->reductions;
  ध्रुव transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->अङ्कः == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->अङ्कः > 1
      || (reds->अङ्कः == 1 && trans->अङ्कः && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->अङ्कः == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  प्रत्यागम s->consistent ? 0 : reds->अङ्कः;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

शून्य
initialize_LA (शून्य)
{
  bool default_reduction_only_for_accept;
  {
    वर्ण *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    मुक्त (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    nLA += state_lookaheads_गणनम् (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  जइ (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      पूर्णाङ्क गणनम् = state_lookaheads_गणनम् (states[i],
                                          default_reduction_only_for_accept);
      जइ (गणनम्)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += गणनम्;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

स्थिर शून्य
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  प्रति (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      ध्रुव reductions *reds = states[i]->reductions;
      जइ (reds->अङ्कः)
        {
          fprintf (out, "  State %d:\n", i);
          प्रति (पूर्णाङ्क j = 0; j < reds->अङ्कः; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->सङ्ख्या);
              जइ (reds->lookaheads)
              {
                bitset_iterator iter;
                पूर्णाङ्क k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

शून्य
lalr (शून्य)
{
  जइ (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, परिमाण *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  जइ (trace_flag & trace_sets)
    lookaheads_print (stderr);
  जइ (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


शून्य
lalr_update_state_numbers (state_सङ्ख्या old_to_new[], state_सङ्ख्या nstates_old)
{
  goto_सङ्ख्या ngotos_reachable = 0;
  चिह्नम्_सङ्ख्या nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  प्रति (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      जाव (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      जइ (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  जाव (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


शून्य
lalr_free (शून्य)
{
  प्रति (state_सङ्ख्या s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = रिक्त;
  bitsetv_free (LA);
}
