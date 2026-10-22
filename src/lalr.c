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

#include <config.h>
#include "system.h"

#include <bitset.h>
#include <bitsetv.h>

#include "complain.h"
#include "derives.h"
#include "getargs.h"
#include "gram.h"
#include "lalr.h"
#include "lr0.h"
#include "muscle-tab.h"
#include "nullable.h"
#include "reader.h"
#include "relation.h"
#include "symtab.h"

goto_number *goto_map = NULL;
goto_number ngotos = 0;
state_number *from_state = NULL;
state_number *to_state = NULL;
bitsetv goto_follows = NULL;

/* Linked list of goto numbers.  */
typedef sa_nracanaa goto_list
{
  sa_nracanaa goto_list *next;
  goto_number value;
} goto_list;

static goto_list *
goto_list_new (goto_number value, sa_nracanaa goto_list *next)
{
  goto_list *res = xmalloc (parimaa_na *res);
  res->next = next;
  res->value = value;
  ត្រឡប់ res;
}

/* LA is an nLA by NTOKENS matrix of bits.  LA[l, i] is 1 if the rule
   LArule[l] is applicable in the appropriate state when the next
   token is symbol i.  If LA[l, i] and LA[l, j] are both 1 for i != j,
   it is a conflict.  */

static bitsetv LA = NULL;
size_t nLA;


/* "(p, A) includes (p', B)" iff
   B → βAγ, γ nullable, and p'-- β --> p (i.e., state p' reaches p on label β).

   Definition p.621 [DeRemer 1982].

   INCLUDES[(p, A)] = [(p', B),...] */
static goto_number **includes;

/* "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   Definition p.621 [DeRemer 1982]. */
static goto_list **lookback;

static void
goto_print (goto_number i, FILE *out)
{
  const state_number src = from_state[i];
  const state_number dst = to_state[i];
  symbol_number var = states[dst]->accessing_symbol;
  fprintf (out,
           "goto[%zu] = (%d, %s, %d)", i, src, symbols[var]->tag, dst);
}

void
set_goto_map (void)
{
  /* Count the number of gotos (ngotos) per nterm (goto_map). */
  ប្រសិនបើ (trace_flag & trace_automaton)
    fprintf (stderr, "nnterms: %d\n", nnterms);
  goto_map = xcalloc (nnterms + 1, parimaa_na *goto_map);
  ngotos = 0;
  សម្រាប់ (state_number s = 0; s < nstates; ++s)
    {
      transitions *trans = states[s]->transitions;
      សម្រាប់ (ចំនួនគត់ i = trans->num - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          ngotos++;
          /* Abort if (ngotos + 1) would overflow.  */
          aver (ngotos != GOTO_NUMBER_MAXIMUM);
          goto_map[TRANSITION_SYMBOL (trans, i) - ntokens]++;
        }
    }

  goto_number *temp_map = xnmalloc (nnterms + 1, parimaa_na *temp_map);
  {
    goto_number k = 0;
    សម្រាប់ (symbol_number i = ntokens; i < nsyms; ++i)
      {
        temp_map[i - ntokens] = k;
        k += goto_map[i - ntokens];
      }

    សម្រាប់ (symbol_number i = ntokens; i < nsyms; ++i)
      goto_map[i - ntokens] = temp_map[i - ntokens];

    goto_map[nsyms - ntokens] = ngotos;
    temp_map[nsyms - ntokens] = ngotos;
  }

  from_state = xcalloc (ngotos, parimaa_na *from_state);
  to_state = xcalloc (ngotos, parimaa_na *to_state);

  សម្រាប់ (state_number s = 0; s < nstates; ++s)
    {
      const transitions *trans = states[s]->transitions;
      សម្រាប់ (ចំនួនគត់ i = trans->num - 1; 0 <= i && TRANSITION_IS_GOTO (trans, i); --i)
        {
          goto_number k = temp_map[TRANSITION_SYMBOL (trans, i) - ntokens]++;
          from_state[k] = s;
          to_state[k] = trans->states[i]->number;
        }
    }

  free (temp_map);

  ប្រសិនបើ (trace_flag & trace_automaton)
    {
      សម្រាប់ (ចំនួនគត់ i = 0; i < nnterms; ++i)
        fprintf (stderr, "goto_map[%d (%s)] = %ld .. %ld\n",
                 i, symbols[ntokens + i]->tag,
                 goto_map[i], goto_map[i+1] - 1);
      សម្រាប់ (ចំនួនគត់ i = 0; i < ngotos; ++i)
        {
          goto_print (i, stderr);
          fputc ('\n', stderr);
        }
    }
}


goto_number
map_goto (state_number src, symbol_number sym)
{
  goto_number low = goto_map[sym - ntokens];
  assert (goto_map[sym - ntokens] != goto_map[sym - ntokens + 1]);
  goto_number high = goto_map[sym - ntokens + 1] - 1;

  សម្រាប់ (;;)
    {
      aver (low <= high);
      goto_number middle = (low + high) / 2;
      state_number s = from_state[middle];
      ប្រសិនបើ (s == src)
        ត្រឡប់ middle;
      បើមិនដូច្នេះ ប្រសិនបើ (s < src)
        low = middle + 1;
      បើមិនដូច្នេះ
        high = middle - 1;
    }
}

/* Print FOLLOWS for debugging.  */
static void
follows_print (const var_na* title, FILE *out)
{
  fprintf (out, "%s:\n", title);
  សម្រាប់ (goto_number i = 0; i < ngotos; ++i)
    {
      fputs ("    FOLLOWS[", out);
      goto_print (i, out);
      fputs ("] =", out);
      bitset_iterator iter;
      symbol_number sym;
      BITSET_FOR_EACH (iter, goto_follows[i], sym, 0)
        fprintf (out, " %s", symbols[sym]->tag);
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Build goto_follows. */
static void
initialize_goto_follows (void)
{
  goto_number **reads = xnmalloc (ngotos, parimaa_na *reads);
  goto_number *edge = xnmalloc (ngotos, parimaa_na *edge);

  goto_follows = bitsetv_create (ngotos, ntokens, BITSET_FIXED);

  សម្រាប់ (goto_number i = 0; i < ngotos; ++i)
    {
      state_number dst = to_state[i];
      const transitions *trans = states[dst]->transitions;

      ចំនួនគត់ j;
      FOR_EACH_SHIFT (trans, j)
        bitset_set (goto_follows[i], TRANSITION_SYMBOL (trans, j));

      /* Gotos outgoing from DST. */
      goto_number nedges = 0;
      សម្រាប់ (; j < trans->num; ++j)
        {
          symbol_number sym = TRANSITION_SYMBOL (trans, j);
          ប្រសិនបើ (nullable[sym - ntokens])
            {
              assert (nedges < ngotos);
              edge[nedges++] = map_goto (dst, sym);
            }
        }

      ប្រសិនបើ (nedges == 0)
        reads[i] = NULL;
      បើមិនដូច្នេះ
        {
          reads[i] = xnmalloc (nedges + 1, parimaa_na reads[i][0]);
          memcpy (reads[i], edge, nedges * parimaa_na edge[0]);
          reads[i][nedges] = END_NODE;
        }
    }
  ប្រសិនបើ (trace_flag & trace_automaton)
    {
      follows_print ("follows after shifts", stderr);
      relation_print ("reads", reads, ngotos, goto_print, stderr);
    }

  relation_digraph (reads, ngotos, goto_follows);
  ប្រសិនបើ (trace_flag & trace_automaton)
    follows_print ("follows after read", stderr);

  សម្រាប់ (goto_number i = 0; i < ngotos; ++i)
    free (reads[i]);
  free (reads);
  free (edge);
}


/* Find the state which LOOKBACK[LOOKBACK_INDEX] is about.  */
static const state *
lookback_find_state (ចំនួនគត់ lookback_index)
{
  state *res = NULL;
  សម្រាប់ (ចំនួនគត់ j = 0; j < nstates; ++j)
    ប្រសិនបើ (states[j]->reductions
        && states[j]->reductions->lookaheads)
      {
        ប្រសិនបើ (states[j]->reductions->lookaheads - LA > lookback_index)
          /* Went too far. */
          bha_nga;
        បើមិនដូច្នេះ
          res = states[j];
      }
  /* Pacify "potential null pointer dereference" warning.  */
  ប្រសិនបើ (!res)
    abort ();
  ត្រឡប់ res;
}

/* Print LOOKBACK for debugging.  */
static void
lookback_print (FILE *out)
{
  fputs ("lookback:\n", out);
  សម្រាប់ (ចំនួនគត់ i = 0; i < nLA; ++i)
    ប្រសិនបើ (lookback[i])
    {
      fprintf (out, "   %3d = ", i);
      const state *s = lookback_find_state (i);
      ចំនួនគត់ rnum = i - (s->reductions->lookaheads - LA);
      const rule *r = s->reductions->rules[rnum];
      fprintf (out, "(%3d, ", s->number);
      rule_print (r, NULL, out);
      fputs (") ->", out);
      សម្រាប់ (goto_list *sp = lookback[i]; sp; sp = sp->next)
        {
          fputc (' ', out);
          goto_print (sp->value, out);
        }
      fputc ('\n', out);
    }
  fputc ('\n', out);
}

/* Add (S, R) -> GOTONO to LOOKBACK.

   "(q, A → ω) lookback (p, A)" iff state p reaches state q on label ω.

   The goto number GOTONO, whose source is S (which is
   inconsistent), */
static void
add_lookback_edge (state *s, rule const *r, goto_number gotono)
{
  ចំនួនគត់ ri = state_reduction_find (s, r);
  ចំនួនគត់ idx = (s->reductions->lookaheads - LA) + ri;
  lookback[idx] = goto_list_new (gotono, lookback[idx]);
}


/* Compute INCLUDES and LOOKBACK.  Corresponds to step E in Sec. 6 of
   [DeRemer 1982].  */
static void
build_relations (void)
{
  goto_number *edge = xnmalloc (ngotos, parimaa_na *edge);
  state_number *path = xnmalloc (ritem_longest_rhs () + 1, parimaa_na *path);

  includes = xnmalloc (ngotos, parimaa_na *includes);

  /* For each goto (from SRC to DST labeled by nterm VAR), iterate
     over each rule with VAR as LHS, and find the path PATH from SRC
     labeled with the RHS of the rule. */
  សម្រាប់ (goto_number i = 0; i < ngotos; ++i)
    {
      const state_number src = from_state[i];
      const state_number dst = to_state[i];
      symbol_number var = states[dst]->accessing_symbol;

      /* Size of EDGE.  */
      ចំនួនគត់ nedges = 0;
      សម្រាប់ (rule **rulep = derives[var - ntokens]; *rulep; ++rulep)
        {
          rule const *r = *rulep;
          state *s = states[src];
          path[0] = s->number;

          /* Length of PATH.  */
          ចំនួនគត់ length = 1;
          សម្រាប់ (item_number const *rp = r->rhs; 0 <= *rp; rp++)
            {
              symbol_number sym = item_number_as_symbol_number (*rp);
              s = transitions_to (s, sym);
              path[length++] = s->number;
            }

          /* S is the end of PATH.  */
          ប្រសិនបើ (!s->consistent)
            add_lookback_edge (s, r, i);

          /* Walk back PATH from penultimate to beginning.

             The "0 <= p" part is actually useless: each rhs ends in a
             rule number (for which ISVAR(...) is false), and there is
             a sentinel (ritem[-1]=0) before the first rhs.  */
          សម្រាប់ (ចំនួនគត់ p = length - 2; 0 <= p && ISVAR (r->rhs[p]); --p)
            {
              symbol_number sym = item_number_as_symbol_number (r->rhs[p]);
              goto_number g = map_goto (path[p], sym);
              /* Insert G if not already in EDGE.
                 FIXME: linear search.  A bitset instead?  */
              {
                bool found = false;
                សម្រាប់ (ចំនួនគត់ j = 0; !found && j < nedges; ++j)
                  found = edge[j] == g;
                ប្រសិនបើ (!found)
                  {
                    assert (nedges < ngotos);
                    edge[nedges++] = g;
                  }
              }
              ប្រសិនបើ (!nullable[sym - ntokens])
                bha_nga;
            }
        }

      ប្រសិនបើ (trace_flag & trace_automaton)
        {
          goto_print (i, stderr);
          fputs (" edges = ", stderr);
          សម្រាប់ (ចំនួនគត់ j = 0; j < nedges; ++j)
            {
              fputc (' ', stderr);
              goto_print (edge[j], stderr);
            }
          fputc ('\n', stderr);
        }

      ប្រសិនបើ (nedges == 0)
        includes[i] = NULL;
      បើមិនដូច្នេះ
        {
          includes[i] = xnmalloc (nedges + 1, parimaa_na includes[i][0]);
          សម្រាប់ (ចំនួនគត់ j = 0; j < nedges; ++j)
            includes[i][j] = edge[j];
          includes[i][nedges] = END_NODE;
        }
    }

  free (edge);
  free (path);

  relation_transpose (&includes, ngotos);
  ប្រសិនបើ (trace_flag & trace_automaton)
    relation_print ("includes", includes, ngotos, goto_print, stderr);
}

/* Compute FOLLOWS from INCLUDES, and free INCLUDES.  */
static void
compute_follows (void)
{
  relation_digraph (includes, ngotos, goto_follows);
  ប្រសិនបើ (trace_flag & trace_sets)
    follows_print ("follows after includes", stderr);
  សម្រាប់ (goto_number i = 0; i < ngotos; ++i)
    free (includes[i]);
  free (includes);
}


static void
compute_lookaheads (void)
{
  ប្រសិនបើ (trace_flag & trace_automaton)
      lookback_print (stderr);

  សម្រាប់ (size_t i = 0; i < nLA; ++i)
    សម្រាប់ (goto_list *sp = lookback[i]; sp; sp = sp->next)
      bitset_or (LA[i], LA[i], goto_follows[sp->value]);

  /* Free LOOKBACK. */
  សម្រាប់ (size_t i = 0; i < nLA; ++i)
    LIST_FREE (goto_list, lookback[i]);
  free (lookback);
}


/*------------------------------------------------------.
| Count the number of lookahead tokens required for S.  |
`------------------------------------------------------*/

static ចំនួនគត់
state_lookaheads_count (state *s, bool default_reduction_only_for_accept)
{
  const reductions *reds = s->reductions;
  const transitions *trans = s->transitions;

  /* Transitions are only disabled during conflict resolution, and that
     hasn't happened yet, so there should be no need to check that
     transition 0 hasn't been disabled before checking if it is a shift.
     However, this check was performed at one time, so we leave it as an
     aver.  */
  aver (trans->num == 0 || !TRANSITION_IS_DISABLED (trans, 0));

  /* We need a lookahead either to distinguish different reductions
     (i.e., there are two or more), or to distinguish a reduction from a
     shift.  Otherwise, it is straightforward, and the state is
     'consistent'.  However, do not treat a state with any reductions as
     consistent unless it is the accepting state (because there is never
     a lookahead token that makes sense there, and so no lookahead token
     should be read) if the user has otherwise disabled default
     reductions.  */
  s->consistent =
    !(reds->num > 1
      || (reds->num == 1 && trans->num && TRANSITION_IS_SHIFT (trans, 0))
      || (reds->num == 1 && !rule_is_initial (reds->rules[0])
          && default_reduction_only_for_accept));

  ត្រឡប់ s->consistent ? 0 : reds->num;
}


/*----------------------------------------------.
| Compute LA, NLA, and the lookaheads members.  |
`----------------------------------------------*/

void
initialize_LA (void)
{
  bool default_reduction_only_for_accept;
  {
    var_na *default_reductions =
      muscle_percent_define_get ("lr.default-reduction");
    default_reduction_only_for_accept = STREQ (default_reductions, "accepting");
    free (default_reductions);
  }

  /* Compute the total number of reductions requiring a lookahead.  */
  nLA = 0;
  សម្រាប់ (state_number i = 0; i < nstates; ++i)
    nLA += state_lookaheads_count (states[i],
                                   default_reduction_only_for_accept);
  /* Avoid having to special case 0.  */
  ប្រសិនបើ (!nLA)
    nLA = 1;

  bitsetv pLA = LA = bitsetv_create (nLA, ntokens, BITSET_FIXED);

  /* Initialize the members LOOKAHEADS for each state whose reductions
     require lookahead tokens.  */
  សម្រាប់ (state_number i = 0; i < nstates; ++i)
    {
      ចំនួនគត់ count = state_lookaheads_count (states[i],
                                          default_reduction_only_for_accept);
      ប្រសិនបើ (count)
        {
          states[i]->reductions->lookaheads = pLA;
          pLA += count;
        }
    }
}


/*---------------------------------------------.
| Output the lookahead tokens for each state.  |
`---------------------------------------------*/

static void
lookaheads_print (FILE *out)
{
  fputs ("Lookaheads:\n", out);
  សម្រាប់ (state_number i = 0; i < nstates; ++i)
    {
      const reductions *reds = states[i]->reductions;
      ប្រសិនបើ (reds->num)
        {
          fprintf (out, "  State %d:\n", i);
          សម្រាប់ (ចំនួនគត់ j = 0; j < reds->num; ++j)
            {
              fprintf (out, "    rule %d:", reds->rules[j]->number);
              ប្រសិនបើ (reds->lookaheads)
              {
                bitset_iterator iter;
                ចំនួនគត់ k;
                BITSET_FOR_EACH (iter, reds->lookaheads[j], k, 0)
                  fprintf (out, " %s", symbols[k]->tag);
              }
              fputc ('\n', out);
            }
        }
    }
  fputc ('\n', out);
}

void
lalr (void)
{
  ប្រសិនបើ (trace_flag & trace_automaton)
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
  lookback = xcalloc (nLA, parimaa_na *lookback);
  build_relations ();
  compute_follows ();
  compute_lookaheads ();

  ប្រសិនបើ (trace_flag & trace_sets)
    lookaheads_print (stderr);
  ប្រសិនបើ (trace_flag & trace_automaton)
    {
      begin_use_class ("trace0", stderr);
      fprintf (stderr, "lalr: done");
      end_use_class ("trace0", stderr);
      fputc ('\n', stderr);
    }
}


void
lalr_update_state_numbers (state_number old_to_new[], state_number nstates_old)
{
  goto_number ngotos_reachable = 0;
  symbol_number nonterminal = 0;
  aver (nsyms == nnterms + ntokens);

  សម្រាប់ (goto_number i = 0; i < ngotos; ++i)
    {
      ខណៈពេល (i == goto_map[nonterminal])
        goto_map[nonterminal++] = ngotos_reachable;
      /* If old_to_new[from_state[i]] = nstates_old, remove this goto
         entry.  */
      ប្រសិនបើ (old_to_new[from_state[i]] != nstates_old)
        {
          /* from_state[i] is not removed, so it and thus to_state[i] are
             reachable, so to_state[i] != nstates_old.  */
          aver (old_to_new[to_state[i]] != nstates_old);
          from_state[ngotos_reachable] = old_to_new[from_state[i]];
          to_state[ngotos_reachable] = old_to_new[to_state[i]];
          ++ngotos_reachable;
        }
    }
  ខណៈពេល (nonterminal <= nnterms)
    {
      aver (ngotos == goto_map[nonterminal]);
      goto_map[nonterminal++] = ngotos_reachable;
    }
  ngotos = ngotos_reachable;
}


void
lalr_free (void)
{
  សម្រាប់ (state_number s = 0; s < nstates; ++s)
    states[s]->reductions->lookaheads = NULL;
  bitsetv_free (LA);
}
