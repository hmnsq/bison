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
  goto_सङ्ख्या मान;
} goto_list;

स्थिर goto_list *
goto_list_new (goto_सङ्ख्या मान, संरचना goto_list *next)
{
  goto_list *res = xmalloc (परिमाण *res);
  res->next = next;
  res->मान = मान;
  फर्काऊ res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

स्थिर bitsetv LA = रिक्त;
माप_प्रकार nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
स्थिर goto_सङ्ख्या **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
स्थिर goto_list **lookback;

स्थिर शून्य
goto_print (goto_सङ्ख्या i, खाता *out)
{
  अचर state_सङ्ख्या src = from_state[i];
  अचर state_सङ्ख्या dst = to_state[i];
  चिह्न_सङ्ख्या var = states[dst]->accessing_चिह्न;
  ख_लिखो (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

शून्य
set_goto_map (शून्य)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  यदि (trace_flag & trace_automaton)
    ख_लिखो (मानक_त्रुटि, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, परिमाण *goto_map);
  ngotos = 0;
  लागि (state_सङ्ख्या s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      लागि (पूर्णाङ्क i = trans->अङ्क - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_सङ्ख्या_MAXIMUM);
          goto_map[TRANSITION_चिह्न (trans, i) - ntokens]++;
        }
    }

  goto_सङ्ख्या *temp_map = xnmalloc (nnterms + 1, परिमाण *temp_map);
  {
    goto_सङ्ख्या k = 0;
    लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    लागि (चिह्न_सङ्ख्या i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, परिमाण *from_state);
  to_state = xcalloc (ngotos, परिमाण *to_state);

  लागि (state_सङ्ख्या s = 0; s < nstates; ++s)
    {
      अचर transitions *trans = states[s]->transitions;
      लागि (पूर्णाङ्क i = trans->अङ्क - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_सङ्ख्या k = temp_map[TRANSITION_चिह्न (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->सङ्ख्या;
        }
    }

  मुक्त (temp_map);

  यदि (trace_flag & trace_automaton)
    {
      लागि (पूर्णाङ्क i = 0; i < nnterms; ++i)
        ख_लिखो (मानक_त्रुटि, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      लागि (पूर्णाङ्क i = 0; i < ngotos; ++i)
        {
          goto_print (i, मानक_त्रुटि);
          ख_अक्षर_दो ('\n', मानक_त्रुटि);
        }
    }
}


goto_सङ्ख्या
map_goto (state_सङ्ख्या src, चिह्न_सङ्ख्या sym)
{
  goto_सङ्ख्या तल्लो = goto_map[sym - ntokens];
  प्रतिज्ञा (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_सङ्ख्या माथिल्लो = goto_map[sym - ntokens + 1] - 1;

  लागि (;;)
    {
      aver (तल्लो <= माथिल्लो);
      goto_सङ्ख्या बीच = (तल्लो + माथिल्लो) / 2;
      state_सङ्ख्या s = from_state[बीच];
      यदि (s == src)
        फर्काऊ बीच;
      अन्यथा यदि (s < src)
        तल्लो = बीच + 1;
      अन्यथा
        माथिल्लो = बीच - 1;
    }
}

/* Print FOLLOWS for debugging.  */
स्थिर शून्य
follows_print (अचर वर्ण* title, खाता *out)
{
  ख_लिखो (out, "%s:\n", title);
  लागि (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      ख_माला_दो ("    FOLLOWS[", out);
      goto_print (i, out);
      ख_माला_दो ("] =", out);
      bitset_iterator iter;
      चिह्न_सङ्ख्या sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        ख_लिखो (out, " %s", symbols[sym]->tag);
      ख_अक्षर_दो ('\n', out);
    }
  ख_अक्षर_दो ('\n', out);
}

/* Build goto_follows. */
स्थिर शून्य
initialize_goto_follows (शून्य)
{
  goto_सङ्ख्या **reads = xnmalloc (ngotos, परिमाण *reads);
  goto_सङ्ख्या *edge = xnmalloc (ngotos, परिमाण *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  लागि (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      state_सङ्ख्या dst = to_state[i];
      अचर transitions *trans = states[dst]->transitions;

      पूर्णाङ्क j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_चिह्न (trans, j));

      /* Gotos outgoing from DST. */
      goto_सङ्ख्या nedges = 0;
      लागि (; j < trans->अङ्क; ++j)
        {
          चिह्न_सङ्ख्या sym = TRANSITION_चिह्न (trans, j);
          यदि (nullable[sym - ntokens])
            {
              प्रतिज्ञा (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      यदि (nedges == 0)
        reads[i] = रिक्त;
      अन्यथा
        {
          reads[i] = xnmalloc (nedges + 1, परिमाण reads[i][0]);
          स_नकल (reads[i], edge, nedges * परिमाण edge[0]);
          reads[i][nedges] = END_गाँठो;
        }
    }
  यदि (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", मानक_त्रुटि);
      relation_print ("reads", reads, ngotos, goto_print, मानक_त्रुटि);
    }

  relation_digraph (reads, ngotos, goto_follows);
  यदि (trace_flag & trace_automaton)
    follows_print ("follows after read", मानक_त्रुटि);

  लागि (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    मुक्त (reads[i]);
  मुक्त (reads);
  मुक्त (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
स्थिर अचर state *
lookback_find_state (पूर्णाङ्क lookback_index)
{
  state *res = रिक्त;
  लागि (पूर्णाङ्क j = 0; j < nstates; ++j)
    यदि (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        यदि (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          रोक;
        अन्यथा
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  यदि (!res)
    पात ();
  फर्काऊ res;
}

/* Print LOOKBACK for debugging.  */
स्थिर शून्य
lookback_print (खाता *out)
{
  ख_माला_दो ("lookback:\n", out);
  लागि (पूर्णाङ्क i = 0; i < nLA; ++i)
    यदि (lookback[i])
    {
      ख_लिखो (out, "   %3d = ", i);
      अचर state *s = lookback_find_state (i);
      पूर्णाङ्क rnum = i - (s->reductions->lookaheads - LA);
      अचर rule *r = s->reductions->rules[rnum];
      ख_लिखो (out, "(%3d, ", s->सङ्ख्या);
      rule_print (r, रिक्त, out);
      ख_माला_दो (") ->", out);
      लागि (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          ख_अक्षर_दो (' ', out);
          goto_print (sp->मान, out);
        }
      ख_अक्षर_दो ('\n', out);
    }
  ख_अक्षर_दो ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
स्थिर शून्य
add_lookback_edge (state *s, rule अचर *r, goto_सङ्ख्या gotono)
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
  लागि (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      अचर state_सङ्ख्या src = from_state[i];
      अचर state_सङ्ख्या dst = to_state[i];
      चिह्न_सङ्ख्या var = states[dst]->accessing_चिह्न;

      /* Size of EDGE.  */
      पूर्णाङ्क nedges = 0;
      लागि (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule अचर *r = *rulep;
          state *s = states[src];
          path[0] = s->सङ्ख्या;

          /* Length of PATH.  */
          पूर्णाङ्क length = 1;
          लागि (item_सङ्ख्या अचर *rp = r->rhs; 0 <= *rp; rp++)
            {
              चिह्न_सङ्ख्या sym = item_सङ्ख्या_as_चिह्न_सङ्ख्या (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->सङ्ख्या;
            }

          /* S is the end of PATH.  */
          यदि (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          लागि (पूर्णाङ्क p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              चिह्न_सङ्ख्या sym = item_सङ्ख्या_as_चिह्न_सङ्ख्या (r->rhs[p]);
              goto_सङ्ख्या g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                लागि (पूर्णाङ्क j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                यदि (!found)
                  {
                    प्रतिज्ञा (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              यदि (!nullable[sym - ntokens])
                रोक;
            }
        }

      यदि (trace_flag & trace_automaton)
        {
          goto_print (i, मानक_त्रुटि);
          ख_माला_दो (" edges = ", मानक_त्रुटि);
          लागि (पूर्णाङ्क j = 0; j < nedges; ++j)
            {
              ख_अक्षर_दो (' ', मानक_त्रुटि);
              goto_print (edge[j], मानक_त्रुटि);
            }
          ख_अक्षर_दो ('\n', मानक_त्रुटि);
        }

      यदि (nedges == 0)
        includes[i] = रिक्त;
      अन्यथा
        {
          includes[i] = xnmalloc (nedges + 1, परिमाण includes[i][0]);
          लागि (पूर्णाङ्क j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_गाँठो;
        }
    }

  मुक्त (edge);
  मुक्त (path);

  relation_transpose (&includes, ngotos);
  यदि (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, मानक_त्रुटि);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
स्थिर शून्य
compute_follows (शून्य)
{
  relation_digraph (includes, ngotos, goto_follows);
  यदि (trace_flag & trace_sets)
    follows_print ("follows after includes", मानक_त्रुटि);
  लागि (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    मुक्त (includes[i]);
  मुक्त (includes);
}


स्थिर शून्य
compute_lookaheads (शून्य)
{
  यदि (trace_flag & trace_automaton)
      lookback_print (मानक_त्रुटि);

  लागि (माप_प्रकार i = 0; i < nLA; ++i)
    लागि (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->मान]);

  /* Free LOOKBACK. */
  लागि (माप_प्रकार i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  मुक्त (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

स्थिर पूर्णाङ्क
state_lookaheads_गन्ती (state *s, bool default_reduction_only_for_accept)
{
  अचर reductions *reds = s->reductions;
  अचर transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->अङ्क == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->अङ्क > 1
      || (reds->अङ्क == 1 && trans->अङ्क && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->अङ्क == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  फर्काऊ s->consistent ? 0 : reds->अङ्क;
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
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    nLA += state_lookaheads_गन्ती (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  यदि (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      पूर्णाङ्क गन्ती = state_lookaheads_गन्ती (states[i],
                                          default_reduction_only_for_accept);
      यदि (गन्ती)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += गन्ती;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

स्थिर शून्य
lookaheads_print (खाता *out)
{
  ख_माला_दो ("Lookaheads:\n", out);
  लागि (state_सङ्ख्या i = 0; i < nstates; ++i)
    {
      अचर reductions *reds = states[i]->reductions;
      यदि (reds->अङ्क)
        {
          ख_लिखो (out, "  State %d:\n", i);
          लागि (पूर्णाङ्क j = 0; j < reds->अङ्क; ++j)
            {
              ख_लिखो (out, "    rule %d:", reds->rules[j]->सङ्ख्या);
              यदि (reds->lookaheads)
              {
                bitset_iterator iter;
                पूर्णाङ्क k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  ख_लिखो (out, " %s", symbols[k]->tag);
              }
              ख_अक्षर_दो ('\n', out);
            }
        }
    }
  ख_अक्षर_दो ('\n', out);
}

शून्य
lalr (शून्य)
{
  यदि (trace_flag & trace_automaton)
    {
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
      begin_use_class ("trace0", मानक_त्रुटि);
      ख_लिखो (मानक_त्रुटि, "lalr: begin");
      end_use_class ("trace0", मानक_त्रुटि);
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
    }
  initialize_LA ();
  set_goto_map ();
  initialize_goto_follows ();
  lookback = xcalloc (nLA, परिमाण *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  यदि (trace_flag & trace_sets)
    lookaheads_print (मानक_त्रुटि);
  यदि (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", मानक_त्रुटि);
      ख_लिखो (मानक_त्रुटि, "lalr: done");
      end_use_class ("trace0", मानक_त्रुटि);
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
    }
}


शून्य
lalr_update_state_numbers (state_सङ्ख्या old_to_new[], state_सङ्ख्या nstates_old)
{
  goto_सङ्ख्या ngotos_reachable = 0;
  चिह्न_सङ्ख्या nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  लागि (goto_सङ्ख्या i = 0; i < ngotos; ++i)
    {
      जबसम्म (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      यदि (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  जबसम्म (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


शून्य
lalr_free (शून्य)
{
  लागि (state_सङ्ख्या s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = रिक्त;
  bitsetv_free (LA);
}
