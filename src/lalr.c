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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <bitset.h>
#അന്തര്ഭാവ <bitsetv.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "derives.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"
#അന്തര്ഭാവ "lalr.h"
#അന്തര്ഭാവ "lr0.h"
#അന്തര്ഭാവ "muscle-tab.h"
#അന്തര്ഭാവ "nullable.h"
#അന്തര്ഭാവ "reader.h"
#അന്തര്ഭാവ "relation.h"
#അന്തര്ഭാവ "symtab.h"

goto_സംഖ്യ *goto_map = ശൂന്യ;
goto_സംഖ്യ ngotos = 0;
state_സംഖ്യ *from_state = ശൂന്യ;
state_സംഖ്യ *to_state = ശൂന്യ;
bitsetv goto_follows = ശൂന്യ;

/* Linked list of goto numbers.  */
തരനിർവചനം ഘടന goto_list
{
  ഘടന goto_list *next;
  goto_സംഖ്യ മൂല്യം;
} goto_list;

സ്ഥിതം goto_list *
goto_list_new (goto_സംഖ്യ മൂല്യം, ഘടന goto_list *next)
{
  goto_list *res = xmalloc (വലുപ്പം *res);
  res->next = next;
  res->മൂല്യം = മൂല്യം;
  തിരികെ res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

സ്ഥിതം bitsetv LA = ശൂന്യ;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
സ്ഥിതം goto_സംഖ്യ **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
സ്ഥിതം goto_list **lookback;

സ്ഥിതം ശൂന്യം
goto_print (goto_സംഖ്യ i, FILE *out)
{
  സ്ഥിരം state_സംഖ്യ src = from_state[i];
  സ്ഥിരം state_സംഖ്യ dst = to_state[i];
  ചിഹ്നം_സംഖ്യ var = states[dst]->accessing_ചിഹ്നം;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

ശൂന്യം
set_goto_map (ശൂന്യം)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  എങ്കിൽ (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, വലുപ്പം *goto_map);
  ngotos = 0;
  വേണ്ടി (state_സംഖ്യ s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = trans->അക്കം - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_സംഖ്യ_MAXIMUM);
          goto_map[TRANSITION_ചിഹ്നം (trans, i) - ntokens]++;
        }
    }

  goto_സംഖ്യ *temp_map = xnmalloc (nnterms + 1, വലുപ്പം *temp_map);
  {
    goto_സംഖ്യ k = 0;
    വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    വേണ്ടി (ചിഹ്നം_സംഖ്യ i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, വലുപ്പം *from_state);
  to_state = xcalloc (ngotos, വലുപ്പം *to_state);

  വേണ്ടി (state_സംഖ്യ s = 0; s < nstates; ++s)
    {
      സ്ഥിരം transitions *trans = states[s]->transitions;
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = trans->അക്കം - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_സംഖ്യ k = temp_map[TRANSITION_ചിഹ്നം (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->സംഖ്യ;
        }
    }

  സ്വതന്ത്രം (temp_map);

  എങ്കിൽ (trace_flag & trace_automaton)
    {
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_സംഖ്യ
map_goto (state_സംഖ്യ src, ചിഹ്നം_സംഖ്യ sym)
{
  goto_സംഖ്യ താഴെയുള്ള = goto_map[sym - ntokens];
  പ്രതിജ്ഞാ (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_സംഖ്യ മുകളിലെ = goto_map[sym - ntokens + 1] - 1;

  വേണ്ടി (;;)
    {
      aver (താഴെയുള്ള <= മുകളിലെ);
      goto_സംഖ്യ നടുവ് = (താഴെയുള്ള + മുകളിലെ) / 2;
      state_സംഖ്യ s = from_state[നടുവ്];
      എങ്കിൽ (s == src)
        തിരികെ നടുവ്;
      അല്ലെങ്കിൽ എങ്കിൽ (s < src)
        താഴെയുള്ള = നടുവ് + 1;
      അല്ലെങ്കിൽ
        മുകളിലെ = നടുവ് - 1;
    }
}

/* Print FOLLOWS for debugging.  */
സ്ഥിതം ശൂന്യം
follows_print (സ്ഥിരം അക്ഷരം* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  വേണ്ടി (goto_സംഖ്യ i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      ചിഹ്നം_സംഖ്യ sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
സ്ഥിതം ശൂന്യം
initialize_goto_follows (ശൂന്യം)
{
  goto_സംഖ്യ **reads = xnmalloc (ngotos, വലുപ്പം *reads);
  goto_സംഖ്യ *edge = xnmalloc (ngotos, വലുപ്പം *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  വേണ്ടി (goto_സംഖ്യ i = 0; i < ngotos; ++i)
    {
      state_സംഖ്യ dst = to_state[i];
      സ്ഥിരം transitions *trans = states[dst]->transitions;

      പൂർണ്ണസംഖ്യ j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_ചിഹ്നം (trans, j));

      /* Gotos outgoing from DST. */
      goto_സംഖ്യ nedges = 0;
      വേണ്ടി (; j < trans->അക്കം; ++j)
        {
          ചിഹ്നം_സംഖ്യ sym = TRANSITION_ചിഹ്നം (trans, j);
          എങ്കിൽ (nullable[sym - ntokens])
            {
              പ്രതിജ്ഞാ (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      എങ്കിൽ (nedges == 0)
        reads[i] = ശൂന്യ;
      അല്ലെങ്കിൽ
        {
          reads[i] = xnmalloc (nedges + 1, വലുപ്പം reads[i][0]);
          memcpy (reads[i], edge, nedges * വലുപ്പം edge[0]);
          reads[i][nedges] = END_കണ്ണി;
        }
    }
  എങ്കിൽ (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  എങ്കിൽ (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  വേണ്ടി (goto_സംഖ്യ i = 0; i < ngotos; ++i)
    സ്വതന്ത്രം (reads[i]);
  സ്വതന്ത്രം (reads);
  സ്വതന്ത്രം (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
സ്ഥിതം സ്ഥിരം state *
lookback_find_state (പൂർണ്ണസംഖ്യ lookback_index)
{
  state *res = ശൂന്യ;
  വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nstates; ++j)
    എങ്കിൽ (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        എങ്കിൽ (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          നിർത്തുക;
        അല്ലെങ്കിൽ
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  എങ്കിൽ (!res)
    abort ();
  തിരികെ res;
}

/* Print LOOKBACK for debugging.  */
സ്ഥിതം ശൂന്യം
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nLA; ++i)
    എങ്കിൽ (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      സ്ഥിരം state *s = lookback_find_state (i);
      പൂർണ്ണസംഖ്യ rnum = i - (s->reductions->lookaheads - LA);
      സ്ഥിരം rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->സംഖ്യ);
      rule_print (r, ശൂന്യ, out);
      fputs (") ->", out);
      വേണ്ടി (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->മൂല്യം, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
സ്ഥിതം ശൂന്യം
add_lookback_edge (state *s, rule സ്ഥിരം *r, goto_സംഖ്യ gotono)
{
  പൂർണ്ണസംഖ്യ ri = state_reduction_find (s, r);
  പൂർണ്ണസംഖ്യ idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
സ്ഥിതം ശൂന്യം
build_relations (ശൂന്യം)
{
  goto_സംഖ്യ *edge = xnmalloc (ngotos, വലുപ്പം *edge);
  state_സംഖ്യ *path = xnmalloc (ritem_longest_rhs () + 1, വലുപ്പം *path);

  includes = xnmalloc (ngotos, വലുപ്പം *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  വേണ്ടി (goto_സംഖ്യ i = 0; i < ngotos; ++i)
    {
      സ്ഥിരം state_സംഖ്യ src = from_state[i];
      സ്ഥിരം state_സംഖ്യ dst = to_state[i];
      ചിഹ്നം_സംഖ്യ var = states[dst]->accessing_ചിഹ്നം;

      /* Size of EDGE.  */
      പൂർണ്ണസംഖ്യ nedges = 0;
      വേണ്ടി (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule സ്ഥിരം *r = *rulep;
          state *s = states[src];
          path[0] = s->സംഖ്യ;

          /* Length of PATH.  */
          പൂർണ്ണസംഖ്യ length = 1;
          വേണ്ടി (item_സംഖ്യ സ്ഥിരം *rp = r->rhs; 0 <= *rp; rp++)
            {
              ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->സംഖ്യ;
            }

          /* S is the end of PATH.  */
          എങ്കിൽ (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          വേണ്ടി (പൂർണ്ണസംഖ്യ p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              ചിഹ്നം_സംഖ്യ sym = item_സംഖ്യ_as_ചിഹ്നം_സംഖ്യ (r->rhs[p]);
              goto_സംഖ്യ g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                എങ്കിൽ (!found)
                  {
                    പ്രതിജ്ഞാ (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              എങ്കിൽ (!nullable[sym - ntokens])
                നിർത്തുക;
            }
        }

      എങ്കിൽ (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      എങ്കിൽ (nedges == 0)
        includes[i] = ശൂന്യ;
      അല്ലെങ്കിൽ
        {
          includes[i] = xnmalloc (nedges + 1, വലുപ്പം includes[i][0]);
          വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_കണ്ണി;
        }
    }

  സ്വതന്ത്രം (edge);
  സ്വതന്ത്രം (path);

  relation_transpose (&includes, ngotos);
  എങ്കിൽ (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
സ്ഥിതം ശൂന്യം
compute_follows (ശൂന്യം)
{
  relation_digraph (includes, ngotos, goto_follows);
  എങ്കിൽ (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  വേണ്ടി (goto_സംഖ്യ i = 0; i < ngotos; ++i)
    സ്വതന്ത്രം (includes[i]);
  സ്വതന്ത്രം (includes);
}


സ്ഥിതം ശൂന്യം
compute_lookaheads (ശൂന്യം)
{
  എങ്കിൽ (trace_flag & trace_automaton)
      lookback_print (stderr);

  വേണ്ടി (size_t i = 0; i < nLA; ++i)
    വേണ്ടി (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->മൂല്യം]);

  /* Free LOOKBACK. */
  വേണ്ടി (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  സ്വതന്ത്രം (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

സ്ഥിതം പൂർണ്ണസംഖ്യ
state_lookaheads_എണ്ണം (state *s, bool default_reduction_only_for_accept)
{
  സ്ഥിരം reductions *reds = s->reductions;
  സ്ഥിരം transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->അക്കം == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->അക്കം > 1
      || (reds->അക്കം == 1 && trans->അക്കം && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->അക്കം == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  തിരികെ s->consistent ? 0 : reds->അക്കം;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

ശൂന്യം
initialize_LA (ശൂന്യം)
{
  bool default_reduction_only_for_accept;
  {
    അക്ഷരം *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    സ്വതന്ത്രം (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    nLA += state_lookaheads_എണ്ണം (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  എങ്കിൽ (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      പൂർണ്ണസംഖ്യ എണ്ണം = state_lookaheads_എണ്ണം (states[i],
                                          default_reduction_only_for_accept);
      എങ്കിൽ (എണ്ണം)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += എണ്ണം;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

സ്ഥിതം ശൂന്യം
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  വേണ്ടി (state_സംഖ്യ i = 0; i < nstates; ++i)
    {
      സ്ഥിരം reductions *reds = states[i]->reductions;
      എങ്കിൽ (reds->അക്കം)
        {
          fprintf (out, "  State %d:\n", i);
          വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < reds->അക്കം; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->സംഖ്യ);
              എങ്കിൽ (reds->lookaheads)
              {
                bitset_iterator iter;
                പൂർണ്ണസംഖ്യ k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

ശൂന്യം
lalr (ശൂന്യം)
{
  എങ്കിൽ (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, വലുപ്പം *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  എങ്കിൽ (trace_flag & trace_sets)
    lookaheads_print (stderr);
  എങ്കിൽ (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


ശൂന്യം
lalr_update_state_numbers (state_സംഖ്യ old_to_new[], state_സംഖ്യ nstates_old)
{
  goto_സംഖ്യ ngotos_reachable = 0;
  ചിഹ്നം_സംഖ്യ nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  വേണ്ടി (goto_സംഖ്യ i = 0; i < ngotos; ++i)
    {
      ആകുമ്പോൾ (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      എങ്കിൽ (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  ആകുമ്പോൾ (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


ശൂന്യം
lalr_free (ശൂന്യം)
{
  വേണ്ടി (state_സംഖ്യ s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = ശൂന്യ;
  bitsetv_free (LA);
}
