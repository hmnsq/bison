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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>
#அந்தர்பாவ <bitsetv.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "derives.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "lalr.h"
#அந்தர்பாவ "lr0.h"
#அந்தர்பாவ "muscle-tab.h"
#அந்தர்பாவ "nullable.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "relation.h"
#அந்தர்பாவ "symtab.h"

goto_எண் *goto_map = வெற்று;
goto_எண் ngotos = 0;
state_எண் *from_state = வெற்று;
state_எண் *to_state = வெற்று;
bitsetv goto_follows = வெற்று;

/* Linked list of goto numbers.  */
வகைவரை கட்டமைப்பு goto_list
{
  கட்டமைப்பு goto_list *next;
  goto_எண் மதிப்பு;
} goto_list;

நிலையான goto_list *
goto_list_new (goto_எண் மதிப்பு, கட்டமைப்பு goto_list *next)
{
  goto_list *res = xmalloc (அளவு *res);
  res->next = next;
  res->மதிப்பு = மதிப்பு;
  திரும்பு res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

நிலையான bitsetv LA = வெற்று;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
நிலையான goto_எண் **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
நிலையான goto_list **lookback;

நிலையான வெறுமை
goto_print (goto_எண் i, FILE *out)
{
  மாறிலி state_எண் src = from_state[i];
  மாறிலி state_எண் dst = to_state[i];
  குறியீடு_எண் var = states[dst]->accessing_குறியீடு;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

வெறுமை
set_goto_map (வெறுமை)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  எனில் (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, அளவு *goto_map);
  ngotos = 0;
  க்கு (state_எண் s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      க்கு (முழு i = trans->இலக்கம் - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_எண்_MAXIMUM);
          goto_map[TRANSITION_குறியீடு (trans, i) - ntokens]++;
        }
    }

  goto_எண் *temp_map = xnmalloc (nnterms + 1, அளவு *temp_map);
  {
    goto_எண் k = 0;
    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, அளவு *from_state);
  to_state = xcalloc (ngotos, அளவு *to_state);

  க்கு (state_எண் s = 0; s < nstates; ++s)
    {
      மாறிலி transitions *trans = states[s]->transitions;
      க்கு (முழு i = trans->இலக்கம் - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_எண் k = temp_map[TRANSITION_குறியீடு (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->எண்;
        }
    }

  விடுவி (temp_map);

  எனில் (trace_flag & trace_automaton)
    {
      க்கு (முழு i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      க்கு (முழு i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_எண்
map_goto (state_எண் src, குறியீடு_எண் sym)
{
  goto_எண் கீழ் = goto_map[sym - ntokens];
  ப்ரதிஜ்ஞா (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_எண் உயர்வு = goto_map[sym - ntokens + 1] - 1;

  க்கு (;;)
    {
      aver (கீழ் <= உயர்வு);
      goto_எண் நடு = (கீழ் + உயர்வு) / 2;
      state_எண் s = from_state[நடு];
      எனில் (s == src)
        திரும்பு நடு;
      இல்லையெனில் எனில் (s < src)
        கீழ் = நடு + 1;
      இல்லையெனில்
        உயர்வு = நடு - 1;
    }
}

/* Print FOLLOWS for debugging.  */
நிலையான வெறுமை
follows_print (மாறிலி எழுத்து* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      குறியீடு_எண் sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
நிலையான வெறுமை
initialize_goto_follows (வெறுமை)
{
  goto_எண் **reads = xnmalloc (ngotos, அளவு *reads);
  goto_எண் *edge = xnmalloc (ngotos, அளவு *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    {
      state_எண் dst = to_state[i];
      மாறிலி transitions *trans = states[dst]->transitions;

      முழு j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_குறியீடு (trans, j));

      /* Gotos outgoing from DST. */
      goto_எண் nedges = 0;
      க்கு (; j < trans->இலக்கம்; ++j)
        {
          குறியீடு_எண் sym = TRANSITION_குறியீடு (trans, j);
          எனில் (nullable[sym - ntokens])
            {
              ப்ரதிஜ்ஞா (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      எனில் (nedges == 0)
        reads[i] = வெற்று;
      இல்லையெனில்
        {
          reads[i] = xnmalloc (nedges + 1, அளவு reads[i][0]);
          memcpy (reads[i], edge, nedges * அளவு edge[0]);
          reads[i][nedges] = END_முனை;
        }
    }
  எனில் (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  எனில் (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    விடுவி (reads[i]);
  விடுவி (reads);
  விடுவி (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
நிலையான மாறிலி state *
lookback_find_state (முழு lookback_index)
{
  state *res = வெற்று;
  க்கு (முழு j = 0; j < nstates; ++j)
    எனில் (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        எனில் (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          நிறுத்து;
        இல்லையெனில்
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  எனில் (!res)
    abort ();
  திரும்பு res;
}

/* Print LOOKBACK for debugging.  */
நிலையான வெறுமை
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  க்கு (முழு i = 0; i < nLA; ++i)
    எனில் (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      மாறிலி state *s = lookback_find_state (i);
      முழு rnum = i - (s->reductions->lookaheads - LA);
      மாறிலி rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->எண்);
      rule_print (r, வெற்று, out);
      fputs (") ->", out);
      க்கு (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->மதிப்பு, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
நிலையான வெறுமை
add_lookback_edge (state *s, rule மாறிலி *r, goto_எண் gotono)
{
  முழு ri = state_reduction_find (s, r);
  முழு idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
நிலையான வெறுமை
build_relations (வெறுமை)
{
  goto_எண் *edge = xnmalloc (ngotos, அளவு *edge);
  state_எண் *path = xnmalloc (ritem_longest_rhs () + 1, அளவு *path);

  includes = xnmalloc (ngotos, அளவு *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    {
      மாறிலி state_எண் src = from_state[i];
      மாறிலி state_எண் dst = to_state[i];
      குறியீடு_எண் var = states[dst]->accessing_குறியீடு;

      /* Size of EDGE.  */
      முழு nedges = 0;
      க்கு (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule மாறிலி *r = *rulep;
          state *s = states[src];
          path[0] = s->எண்;

          /* Length of PATH.  */
          முழு length = 1;
          க்கு (item_எண் மாறிலி *rp = r->rhs; 0 <= *rp; rp++)
            {
              குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->எண்;
            }

          /* S is the end of PATH.  */
          எனில் (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          க்கு (முழு p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              குறியீடு_எண் sym = item_எண்_as_குறியீடு_எண் (r->rhs[p]);
              goto_எண் g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                க்கு (முழு j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                எனில் (!found)
                  {
                    ப்ரதிஜ்ஞா (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              எனில் (!nullable[sym - ntokens])
                நிறுத்து;
            }
        }

      எனில் (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          க்கு (முழு j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      எனில் (nedges == 0)
        includes[i] = வெற்று;
      இல்லையெனில்
        {
          includes[i] = xnmalloc (nedges + 1, அளவு includes[i][0]);
          க்கு (முழு j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_முனை;
        }
    }

  விடுவி (edge);
  விடுவி (path);

  relation_transpose (&includes, ngotos);
  எனில் (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
நிலையான வெறுமை
compute_follows (வெறுமை)
{
  relation_digraph (includes, ngotos, goto_follows);
  எனில் (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    விடுவி (includes[i]);
  விடுவி (includes);
}


நிலையான வெறுமை
compute_lookaheads (வெறுமை)
{
  எனில் (trace_flag & trace_automaton)
      lookback_print (stderr);

  க்கு (size_t i = 0; i < nLA; ++i)
    க்கு (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->மதிப்பு]);

  /* Free LOOKBACK. */
  க்கு (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  விடுவி (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

நிலையான முழு
state_lookaheads_மொத்தம் (state *s, bool default_reduction_only_for_accept)
{
  மாறிலி reductions *reds = s->reductions;
  மாறிலி transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->இலக்கம் == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->இலக்கம் > 1
      || (reds->இலக்கம் == 1 && trans->இலக்கம் && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->இலக்கம் == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  திரும்பு s->consistent ? 0 : reds->இலக்கம்;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

வெறுமை
initialize_LA (வெறுமை)
{
  bool default_reduction_only_for_accept;
  {
    எழுத்து *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    விடுவி (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  க்கு (state_எண் i = 0; i < nstates; ++i)
    nLA += state_lookaheads_மொத்தம் (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  எனில் (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      முழு மொத்தம் = state_lookaheads_மொத்தம் (states[i],
                                          default_reduction_only_for_accept);
      எனில் (மொத்தம்)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += மொத்தம்;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

நிலையான வெறுமை
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  க்கு (state_எண் i = 0; i < nstates; ++i)
    {
      மாறிலி reductions *reds = states[i]->reductions;
      எனில் (reds->இலக்கம்)
        {
          fprintf (out, "  State %d:\n", i);
          க்கு (முழு j = 0; j < reds->இலக்கம்; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->எண்);
              எனில் (reds->lookaheads)
              {
                bitset_iterator iter;
                முழு k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

வெறுமை
lalr (வெறுமை)
{
  எனில் (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, அளவு *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  எனில் (trace_flag & trace_sets)
    lookaheads_print (stderr);
  எனில் (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


வெறுமை
lalr_update_state_numbers (state_எண் old_to_new[], state_எண் nstates_old)
{
  goto_எண் ngotos_reachable = 0;
  குறியீடு_எண் nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  க்கு (goto_எண் i = 0; i < ngotos; ++i)
    {
      போது (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      எனில் (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  போது (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


வெறுமை
lalr_free (வெறுமை)
{
  க்கு (state_எண் s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = வெற்று;
  bitsetv_free (LA);
}
