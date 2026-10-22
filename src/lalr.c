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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>
#ଅନ୍ତର୍ଭାଵ <bitsetv.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "derives.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "lalr.h"
#ଅନ୍ତର୍ଭାଵ "lr0.h"
#ଅନ୍ତର୍ଭାଵ "muscle-tab.h"
#ଅନ୍ତର୍ଭାଵ "nullable.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "relation.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

goto_ସଂଖ୍ୟା *goto_map = ଖାଲି;
goto_ସଂଖ୍ୟା ngotos = 0;
state_ସଂଖ୍ୟା *from_state = ଖାଲି;
state_ସଂଖ୍ୟା *to_state = ଖାଲି;
bitsetv goto_follows = ଖାଲି;

/* Linked list of goto numbers.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା goto_list
{
  ସଂରଚନା goto_list *next;
  goto_ସଂଖ୍ୟା ମୂଲ୍ୟ;
} goto_list;

ସ୍ଥିର goto_list *
goto_list_new (goto_ସଂଖ୍ୟା ମୂଲ୍ୟ, ସଂରଚନା goto_list *next)
{
  goto_list *res = xmalloc (ଆକାର *res);
  res->next = next;
  res->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
  ଫେରନ୍ତୁ res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

ସ୍ଥିର bitsetv LA = ଖାଲି;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
ସ୍ଥିର goto_ସଂଖ୍ୟା **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
ସ୍ଥିର goto_list **lookback;

ସ୍ଥିର ଶୂନ୍ୟ
goto_print (goto_ସଂଖ୍ୟା i, FILE *out)
{
  ଅଚଳ state_ସଂଖ୍ୟା src = from_state[i];
  ଅଚଳ state_ସଂଖ୍ୟା dst = to_state[i];
  ପ୍ରତୀକ_ସଂଖ୍ୟା var = states[dst]->accessing_ପ୍ରତୀକ;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

ଶୂନ୍ୟ
set_goto_map (ଶୂନ୍ୟ)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  ଯଦି (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, ଆକାର *goto_map);
  ngotos = 0;
  ପାଇଁ (state_ସଂଖ୍ୟା s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = trans->ଅଙ୍କ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_ସଂଖ୍ୟା_MAXIMUM);
          goto_map[TRANSITION_ପ୍ରତୀକ (trans, i) - ntokens]++;
        }
    }

  goto_ସଂଖ୍ୟା *temp_map = xnmalloc (nnterms + 1, ଆକାର *temp_map);
  {
    goto_ସଂଖ୍ୟା k = 0;
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, ଆକାର *from_state);
  to_state = xcalloc (ngotos, ଆକାର *to_state);

  ପାଇଁ (state_ସଂଖ୍ୟା s = 0; s < nstates; ++s)
    {
      ଅଚଳ transitions *trans = states[s]->transitions;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = trans->ଅଙ୍କ - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_ସଂଖ୍ୟା k = temp_map[TRANSITION_ପ୍ରତୀକ (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->ସଂଖ୍ୟା;
        }
    }

  ମୁକ୍ତ (temp_map);

  ଯଦି (trace_flag & trace_automaton)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_ସଂଖ୍ୟା
map_goto (state_ସଂଖ୍ୟା src, ପ୍ରତୀକ_ସଂଖ୍ୟା sym)
{
  goto_ସଂଖ୍ୟା ତଳ = goto_map[sym - ntokens];
  ପ୍ରତିଜ୍ଞା (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_ସଂଖ୍ୟା ଉପର = goto_map[sym - ntokens + 1] - 1;

  ପାଇଁ (;;)
    {
      aver (ତଳ <= ଉପର);
      goto_ସଂଖ୍ୟା ମଧ୍ୟ = (ତଳ + ଉପର) / 2;
      state_ସଂଖ୍ୟା s = from_state[ମଧ୍ୟ];
      ଯଦି (s == src)
        ଫେରନ୍ତୁ ମଧ୍ୟ;
      ଅନ୍ୟଥା ଯଦି (s < src)
        ତଳ = ମଧ୍ୟ + 1;
      ଅନ୍ୟଥା
        ଉପର = ମଧ୍ୟ - 1;
    }
}

/* Print FOLLOWS for debugging.  */
ସ୍ଥିର ଶୂନ୍ୟ
follows_print (ଅଚଳ ଅକ୍ଷର* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  ପାଇଁ (goto_ସଂଖ୍ୟା i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      ପ୍ରତୀକ_ସଂଖ୍ୟା sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
ସ୍ଥିର ଶୂନ୍ୟ
initialize_goto_follows (ଶୂନ୍ୟ)
{
  goto_ସଂଖ୍ୟା **reads = xnmalloc (ngotos, ଆକାର *reads);
  goto_ସଂଖ୍ୟା *edge = xnmalloc (ngotos, ଆକାର *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  ପାଇଁ (goto_ସଂଖ୍ୟା i = 0; i < ngotos; ++i)
    {
      state_ସଂଖ୍ୟା dst = to_state[i];
      ଅଚଳ transitions *trans = states[dst]->transitions;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_ପ୍ରତୀକ (trans, j));

      /* Gotos outgoing from DST. */
      goto_ସଂଖ୍ୟା nedges = 0;
      ପାଇଁ (; j < trans->ଅଙ୍କ; ++j)
        {
          ପ୍ରତୀକ_ସଂଖ୍ୟା sym = TRANSITION_ପ୍ରତୀକ (trans, j);
          ଯଦି (nullable[sym - ntokens])
            {
              ପ୍ରତିଜ୍ଞା (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      ଯଦି (nedges == 0)
        reads[i] = ଖାଲି;
      ଅନ୍ୟଥା
        {
          reads[i] = xnmalloc (nedges + 1, ଆକାର reads[i][0]);
          memcpy (reads[i], edge, nedges * ଆକାର edge[0]);
          reads[i][nedges] = END_ଗଣ୍ଠି;
        }
    }
  ଯଦି (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  ଯଦି (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  ପାଇଁ (goto_ସଂଖ୍ୟା i = 0; i < ngotos; ++i)
    ମୁକ୍ତ (reads[i]);
  ମୁକ୍ତ (reads);
  ମୁକ୍ତ (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
ସ୍ଥିର ଅଚଳ state *
lookback_find_state (ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookback_index)
{
  state *res = ଖାଲି;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < nstates; ++j)
    ଯଦି (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        ଯଦି (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          ଭାଙ୍ଗ;
        ଅନ୍ୟଥା
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  ଯଦି (!res)
    abort ();
  ଫେରନ୍ତୁ res;
}

/* Print LOOKBACK for debugging.  */
ସ୍ଥିର ଶୂନ୍ୟ
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nLA; ++i)
    ଯଦି (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      ଅଚଳ state *s = lookback_find_state (i);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rnum = i - (s->reductions->lookaheads - LA);
      ଅଚଳ rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->ସଂଖ୍ୟା);
      rule_print (r, ଖାଲି, out);
      fputs (") ->", out);
      ପାଇଁ (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->ମୂଲ୍ୟ, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
ସ୍ଥିର ଶୂନ୍ୟ
add_lookback_edge (state *s, rule ଅଚଳ *r, goto_ସଂଖ୍ୟା gotono)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ri = state_reduction_find (s, r);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
ସ୍ଥିର ଶୂନ୍ୟ
build_relations (ଶୂନ୍ୟ)
{
  goto_ସଂଖ୍ୟା *edge = xnmalloc (ngotos, ଆକାର *edge);
  state_ସଂଖ୍ୟା *path = xnmalloc (ritem_longest_rhs () + 1, ଆକାର *path);

  includes = xnmalloc (ngotos, ଆକାର *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  ପାଇଁ (goto_ସଂଖ୍ୟା i = 0; i < ngotos; ++i)
    {
      ଅଚଳ state_ସଂଖ୍ୟା src = from_state[i];
      ଅଚଳ state_ସଂଖ୍ୟା dst = to_state[i];
      ପ୍ରତୀକ_ସଂଖ୍ୟା var = states[dst]->accessing_ପ୍ରତୀକ;

      /* Size of EDGE.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା nedges = 0;
      ପାଇଁ (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule ଅଚଳ *r = *rulep;
          state *s = states[src];
          path[0] = s->ସଂଖ୍ୟା;

          /* Length of PATH.  */
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = 1;
          ପାଇଁ (item_ସଂଖ୍ୟା ଅଚଳ *rp = r->rhs; 0 <= *rp; rp++)
            {
              ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->ସଂଖ୍ୟା;
            }

          /* S is the end of PATH.  */
          ଯଦି (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              ପ୍ରତୀକ_ସଂଖ୍ୟା sym = item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (r->rhs[p]);
              goto_ସଂଖ୍ୟା g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                ଯଦି (!found)
                  {
                    ପ୍ରତିଜ୍ଞା (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              ଯଦି (!nullable[sym - ntokens])
                ଭାଙ୍ଗ;
            }
        }

      ଯଦି (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      ଯଦି (nedges == 0)
        includes[i] = ଖାଲି;
      ଅନ୍ୟଥା
        {
          includes[i] = xnmalloc (nedges + 1, ଆକାର includes[i][0]);
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_ଗଣ୍ଠି;
        }
    }

  ମୁକ୍ତ (edge);
  ମୁକ୍ତ (path);

  relation_transpose (&includes, ngotos);
  ଯଦି (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
ସ୍ଥିର ଶୂନ୍ୟ
compute_follows (ଶୂନ୍ୟ)
{
  relation_digraph (includes, ngotos, goto_follows);
  ଯଦି (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  ପାଇଁ (goto_ସଂଖ୍ୟା i = 0; i < ngotos; ++i)
    ମୁକ୍ତ (includes[i]);
  ମୁକ୍ତ (includes);
}


ସ୍ଥିର ଶୂନ୍ୟ
compute_lookaheads (ଶୂନ୍ୟ)
{
  ଯଦି (trace_flag & trace_automaton)
      lookback_print (stderr);

  ପାଇଁ (size_t i = 0; i < nLA; ++i)
    ପାଇଁ (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->ମୂଲ୍ୟ]);

  /* Free LOOKBACK. */
  ପାଇଁ (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  ମୁକ୍ତ (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
state_lookaheads_ଗଣନା (state *s, bool default_reduction_only_for_accept)
{
  ଅଚଳ reductions *reds = s->reductions;
  ଅଚଳ transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->ଅଙ୍କ == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->ଅଙ୍କ > 1
      || (reds->ଅଙ୍କ == 1 && trans->ଅଙ୍କ && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->ଅଙ୍କ == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  ଫେରନ୍ତୁ s->consistent ? 0 : reds->ଅଙ୍କ;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

ଶୂନ୍ୟ
initialize_LA (ଶୂନ୍ୟ)
{
  bool default_reduction_only_for_accept;
  {
    ଅକ୍ଷର *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    ମୁକ୍ତ (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    nLA += state_lookaheads_ଗଣନା (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  ଯଦି (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = state_lookaheads_ଗଣନା (states[i],
                                          default_reduction_only_for_accept);
      ଯଦି (ଗଣନା_2)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += ଗଣନା_2;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  ପାଇଁ (state_ସଂଖ୍ୟା i = 0; i < nstates; ++i)
    {
      ଅଚଳ reductions *reds = states[i]->reductions;
      ଯଦି (reds->ଅଙ୍କ)
        {
          fprintf (out, "  State %d:\n", i);
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < reds->ଅଙ୍କ; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->ସଂଖ୍ୟା);
              ଯଦି (reds->lookaheads)
              {
                bitset_iterator iter;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

ଶୂନ୍ୟ
lalr (ଶୂନ୍ୟ)
{
  ଯଦି (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, ଆକାର *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  ଯଦି (trace_flag & trace_sets)
    lookaheads_print (stderr);
  ଯଦି (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


ଶୂନ୍ୟ
lalr_update_state_numbers (state_ସଂଖ୍ୟା old_to_new[], state_ସଂଖ୍ୟା nstates_old)
{
  goto_ସଂଖ୍ୟା ngotos_reachable = 0;
  ପ୍ରତୀକ_ସଂଖ୍ୟା nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  ପାଇଁ (goto_ସଂଖ୍ୟା i = 0; i < ngotos; ++i)
    {
      ଯେତେବେଳେ (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      ଯଦି (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  ଯେତେବେଳେ (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


ଶୂନ୍ୟ
lalr_free (ଶୂନ୍ୟ)
{
  ପାଇଁ (state_ସଂଖ୍ୟା s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = ଖାଲି;
  bitsetv_free (LA);
}
