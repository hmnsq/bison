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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>
#অন্তর্ভাব <bitsetv.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "derives.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "lalr.h"
#অন্তর্ভাব "lr0.h"
#অন্তর্ভাব "muscle-tab.h"
#অন্তর্ভাব "nullable.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "relation.h"
#অন্তর্ভাব "symtab.h"

goto_সংখ্যা *goto_map = ফাঁকা;
goto_সংখ্যা ngotos = 0;
state_সংখ্যা *from_state = ফাঁকা;
state_সংখ্যা *to_state = ফাঁকা;
bitsetv goto_follows = ফাঁকা;

/* Linked list of goto numbers.  */
প্রকারলেখ কাঠামো goto_list
{
  কাঠামো goto_list *পরবর্তী;
  goto_সংখ্যা মান;
} goto_list;

স্থির goto_list *
goto_list_new (goto_সংখ্যা মান, কাঠামো goto_list *পরবর্তী)
{
  goto_list *res = xmalloc (আকার *res);
  res->পরবর্তী = পরবর্তী;
  res->মান = মান;
  ফেরত res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

স্থির bitsetv LA = ফাঁকা;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
স্থির goto_সংখ্যা **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
স্থির goto_list **lookback;

স্থির শূন্য
goto_print (goto_সংখ্যা i, FILE *out)
{
  ধ্রুবক state_সংখ্যা src = from_state[i];
  ধ্রুবক state_সংখ্যা dst = to_state[i];
  প্রতীক_সংখ্যা var = states[dst]->accessing_প্রতীক;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

শূন্য
set_goto_map (শূন্য)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  যদি (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, আকার *goto_map);
  ngotos = 0;
  জন্য (state_সংখ্যা s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      জন্য (পূর্ণসংখ্যা i = trans->অঙ্ক - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_সংখ্যা_MAXIMUM);
          goto_map[TRANSITION_প্রতীক (trans, i) - ntokens]++;
        }
    }

  goto_সংখ্যা *অস্থায়ী_map = xnmalloc (nnterms + 1, আকার *অস্থায়ী_map);
  {
    goto_সংখ্যা k = 0;
    জন্য (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      {
        অস্থায়ী_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    জন্য (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = অস্থায়ী_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    অস্থায়ী_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, আকার *from_state);
  to_state = xcalloc (ngotos, আকার *to_state);

  জন্য (state_সংখ্যা s = 0; s < nstates; ++s)
    {
      ধ্রুবক transitions *trans = states[s]->transitions;
      জন্য (পূর্ণসংখ্যা i = trans->অঙ্ক - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_সংখ্যা k = অস্থায়ী_map[TRANSITION_প্রতীক (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->সংখ্যা;
        }
    }

  মুক্ত (অস্থায়ী_map);

  যদি (trace_flag & trace_automaton)
    {
      জন্য (পূর্ণসংখ্যা i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      জন্য (পূর্ণসংখ্যা i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_সংখ্যা
map_goto (state_সংখ্যা src, প্রতীক_সংখ্যা sym)
{
  goto_সংখ্যা নিচের = goto_map[sym - ntokens];
  প্রতিজ্ঞা (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_সংখ্যা উপরের = goto_map[sym - ntokens + 1] - 1;

  জন্য (;;)
    {
      aver (নিচের <= উপরের);
      goto_সংখ্যা মধ্য = (নিচের + উপরের) / 2;
      state_সংখ্যা s = from_state[মধ্য];
      যদি (s == src)
        ফেরত মধ্য;
      অন্যথা যদি (s < src)
        নিচের = মধ্য + 1;
      অন্যথা
        উপরের = মধ্য - 1;
    }
}

/* Print FOLLOWS for debugging.  */
স্থির শূন্য
follows_print (ধ্রুবক অক্ষর* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  জন্য (goto_সংখ্যা i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      প্রতীক_সংখ্যা sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
স্থির শূন্য
initialize_goto_follows (শূন্য)
{
  goto_সংখ্যা **reads = xnmalloc (ngotos, আকার *reads);
  goto_সংখ্যা *edge = xnmalloc (ngotos, আকার *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  জন্য (goto_সংখ্যা i = 0; i < ngotos; ++i)
    {
      state_সংখ্যা dst = to_state[i];
      ধ্রুবক transitions *trans = states[dst]->transitions;

      পূর্ণসংখ্যা j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_প্রতীক (trans, j));

      /* Gotos outgoing from DST. */
      goto_সংখ্যা nedges = 0;
      জন্য (; j < trans->অঙ্ক; ++j)
        {
          প্রতীক_সংখ্যা sym = TRANSITION_প্রতীক (trans, j);
          যদি (nullable[sym - ntokens])
            {
              প্রতিজ্ঞা (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      যদি (nedges == 0)
        reads[i] = ফাঁকা;
      অন্যথা
        {
          reads[i] = xnmalloc (nedges + 1, আকার reads[i][0]);
          memcpy (reads[i], edge, nedges * আকার edge[0]);
          reads[i][nedges] = END_গ্রন্থি;
        }
    }
  যদি (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  যদি (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  জন্য (goto_সংখ্যা i = 0; i < ngotos; ++i)
    মুক্ত (reads[i]);
  মুক্ত (reads);
  মুক্ত (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
স্থির ধ্রুবক state *
lookback_find_state (পূর্ণসংখ্যা lookback_সূচক)
{
  state *res = ফাঁকা;
  জন্য (পূর্ণসংখ্যা j = 0; j < nstates; ++j)
    যদি (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        যদি (states[j]->reductions->lookaheads - LA > lookback_সূচক)
          /* Went too far. */
          বিরতি;
        অন্যথা
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  যদি (!res)
    abort ();
  ফেরত res;
}

/* Print LOOKBACK for debugging.  */
স্থির শূন্য
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  জন্য (পূর্ণসংখ্যা i = 0; i < nLA; ++i)
    যদি (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      ধ্রুবক state *s = lookback_find_state (i);
      পূর্ণসংখ্যা rnum = i - (s->reductions->lookaheads - LA);
      ধ্রুবক rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->সংখ্যা);
      rule_print (r, ফাঁকা, out);
      fputs (") ->", out);
      জন্য (goto_list *sp = lookback[i]; sp; sp = sp->পরবর্তী)
        {
          fputc (' ', out);
          goto_print (sp->মান, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
স্থির শূন্য
add_lookback_edge (state *s, rule ধ্রুবক *r, goto_সংখ্যা gotono)
{
  পূর্ণসংখ্যা ri = state_reduction_find (s, r);
  পূর্ণসংখ্যা idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
স্থির শূন্য
build_relations (শূন্য)
{
  goto_সংখ্যা *edge = xnmalloc (ngotos, আকার *edge);
  state_সংখ্যা *path = xnmalloc (ritem_longest_rhs () + 1, আকার *path);

  includes = xnmalloc (ngotos, আকার *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  জন্য (goto_সংখ্যা i = 0; i < ngotos; ++i)
    {
      ধ্রুবক state_সংখ্যা src = from_state[i];
      ধ্রুবক state_সংখ্যা dst = to_state[i];
      প্রতীক_সংখ্যা var = states[dst]->accessing_প্রতীক;

      /* Size of EDGE.  */
      পূর্ণসংখ্যা nedges = 0;
      জন্য (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule ধ্রুবক *r = *rulep;
          state *s = states[src];
          path[0] = s->সংখ্যা;

          /* Length of PATH.  */
          পূর্ণসংখ্যা দৈর্ঘ্য = 1;
          জন্য (item_সংখ্যা ধ্রুবক *rp = r->rhs; 0 <= *rp; rp++)
            {
              প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (*rp);
              s = transitions_to (s, sym);
              path[দৈর্ঘ্য++] = s->সংখ্যা;
            }

          /* S is the end of PATH.  */
          যদি (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          জন্য (পূর্ণসংখ্যা p = দৈর্ঘ্য - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              প্রতীক_সংখ্যা sym = item_সংখ্যা_as_প্রতীক_সংখ্যা (r->rhs[p]);
              goto_সংখ্যা g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                জন্য (পূর্ণসংখ্যা j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                যদি (!found)
                  {
                    প্রতিজ্ঞা (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              যদি (!nullable[sym - ntokens])
                বিরতি;
            }
        }

      যদি (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          জন্য (পূর্ণসংখ্যা j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      যদি (nedges == 0)
        includes[i] = ফাঁকা;
      অন্যথা
        {
          includes[i] = xnmalloc (nedges + 1, আকার includes[i][0]);
          জন্য (পূর্ণসংখ্যা j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_গ্রন্থি;
        }
    }

  মুক্ত (edge);
  মুক্ত (path);

  relation_transpose (&includes, ngotos);
  যদি (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
স্থির শূন্য
compute_follows (শূন্য)
{
  relation_digraph (includes, ngotos, goto_follows);
  যদি (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  জন্য (goto_সংখ্যা i = 0; i < ngotos; ++i)
    মুক্ত (includes[i]);
  মুক্ত (includes);
}


স্থির শূন্য
compute_lookaheads (শূন্য)
{
  যদি (trace_flag & trace_automaton)
      lookback_print (stderr);

  জন্য (size_t i = 0; i < nLA; ++i)
    জন্য (goto_list *sp = lookback[i]; sp; sp = sp->পরবর্তী)
      bitset_or (LA[i], LA[i], goto_follows[sp->মান]);

  /* Free LOOKBACK. */
  জন্য (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  মুক্ত (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

স্থির পূর্ণসংখ্যা
state_lookaheads_গুনতি (state *s, bool default_reduction_only_for_accept)
{
  ধ্রুবক reductions *reds = s->reductions;
  ধ্রুবক transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->অঙ্ক == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->অঙ্ক > 1
      || (reds->অঙ্ক == 1 && trans->অঙ্ক && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->অঙ্ক == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  ফেরত s->consistent ? 0 : reds->অঙ্ক;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

শূন্য
initialize_LA (শূন্য)
{
  bool default_reduction_only_for_accept;
  {
    অক্ষর *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    মুক্ত (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    nLA += state_lookaheads_গুনতি (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  যদি (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      পূর্ণসংখ্যা গুনতি = state_lookaheads_গুনতি (states[i],
                                          default_reduction_only_for_accept);
      যদি (গুনতি)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += গুনতি;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

স্থির শূন্য
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  জন্য (state_সংখ্যা i = 0; i < nstates; ++i)
    {
      ধ্রুবক reductions *reds = states[i]->reductions;
      যদি (reds->অঙ্ক)
        {
          fprintf (out, "  State %d:\n", i);
          জন্য (পূর্ণসংখ্যা j = 0; j < reds->অঙ্ক; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->সংখ্যা);
              যদি (reds->lookaheads)
              {
                bitset_iterator iter;
                পূর্ণসংখ্যা k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

শূন্য
lalr (শূন্য)
{
  যদি (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, আকার *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  যদি (trace_flag & trace_sets)
    lookaheads_print (stderr);
  যদি (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


শূন্য
lalr_update_state_numbers (state_সংখ্যা old_to_new[], state_সংখ্যা nstates_old)
{
  goto_সংখ্যা ngotos_reachable = 0;
  প্রতীক_সংখ্যা nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  জন্য (goto_সংখ্যা i = 0; i < ngotos; ++i)
    {
      যখন (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      যদি (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  যখন (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


শূন্য
lalr_free (শূন্য)
{
  জন্য (state_সংখ্যা s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = ফাঁকা;
  bitsetv_free (LA);
}
