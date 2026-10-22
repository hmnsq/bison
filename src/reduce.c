/* Grammar reduction for Bison.

   Copyright (C) 1988-1989, 2000-2003, 2005-2015, 2018-2022 Free
   Software Foundation, Inc.

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


/* Reduce the grammar: Find and eliminate unreachable terminals,
   nonterminals, and productions.  David S. Bakin.  */

/* Don't eliminate unreachable terminals: They may be used by the
   user's parser.  */

#include <config.h>
#include "system.h"

#include <bitset.h>

#include "complain.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"
#include "print-xml.h"
#include "reader.h"
#include "reduce.h"
#include "symtab.h"

/* Set of nonterminals whose language is not empty.  */
static bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
static bitset P;

/* Set of accessible symbols.  */
static bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
static bitset V1;

පූර්ණ_සංඛ්‍යා nuseless_productions;
පූර්ණ_සංඛ්‍යා nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  do {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } අතරතුර (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

static bool
useful_production (rule_number r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  සඳහා (item_number *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    නම් (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      ආපසු false;
  ආපසු true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

static void
useless_nonterminals (void)
{
  /* N is set as built.  Np is set being built this iteration. P is
     set of all productions which have a RHS all in N.  */

  bitset Np = bitset_create (nnterms, BITSET_FIXED);

  /* The set being computed is a set of nonterminals which can derive
     the empty string or strings consisting of all terminals. At each
     iteration a nonterminal is added to the set if there is a
     production with that nonterminal as its LHS for which all the
     nonterminals in its RHS are already in the set.  Iterate until
     the set being computed remains unchanged.  Any nonterminals not
     in the set at that point are useless in that they will never be
     used in deriving a sentence of the language.

     This iteration doesn't use any special traversal over the
     productions.  A set is kept of all productions for which all the
     nonterminals in the RHS are in useful.  Only productions not in
     this set are scanned on each iteration.  At the end, this set is
     saved to be used when finding useful productions: only
     productions in this set will appear in the final grammar.  */

  අතරතුර (1)
    {
      bitset_copy (Np, N);
      සඳහා (rule_number r = 0; r < nrules; ++r)
        නම් (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->number - ntokens);
            bitset_set (P, r);
          }
      නම් (bitset_equal_p (N, Np))
        bha_nga;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


static void
inaccessable_symbols (void)
{
  /* Find out which productions are reachable and which symbols are
     used.  Starting with an empty set of productions and a set of
     symbols which only has the start symbol in it, iterate over all
     productions until the set of productions remains unchanged for an
     iteration.  For each production which has a LHS in the set of
     reachable symbols, add the production to the set of reachable
     productions, and add all of the nonterminals in the RHS of the
     production to the set of reachable symbols.

     Consider only the (partially) reduced grammar which has only
     nonterminals in N and productions in P.

     The result is the set P of productions in the reduced grammar,
     and the set V of symbols in the reduced grammar.

     Although this algorithm also computes the set of terminals which
     are reachable, no terminal will be deleted from the grammar. Some
     terminals might not be in the grammar but might be generated by
     semantic routines, and so the user might want them available with
     specified numbers.  (Is this true?)  However, the nonreachable
     terminals are printed (if running in verbose mode) so that the
     user can know.  */

  bitset Vp = bitset_create (nsyms, BITSET_FIXED);
  bitset Pp = bitset_create (nrules, BITSET_FIXED);

  /* If the start symbol isn't useful, then nothing will be useful. */
  නම් (bitset_test (N, acceptsymbol->content->number - ntokens))
    {
      bitset_set (V, acceptsymbol->content->number);

      අතරතුර (1)
        {
          bitset_copy (Vp, V);
          සඳහා (rule_number r = 0; r < nrules; ++r)
            නම් (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->number))
              {
                සඳහා (item_number *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  නම් (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          නම් (bitset_equal_p (V, Vp))
            bha_nga;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->number);   /* end-of-input token */
  bitset_set (V, errtoken->content->number);   /* error token */
  bitset_set (V, undeftoken->content->number); /* some undefined token */

  bitset_free (P);
  P = Pp;

  පූර්ණ_සංඛ්‍යා nuseful_productions = bitset_count (P);
  nuseless_productions = nrules - nuseful_productions;

  පූර්ණ_සංඛ්‍යා nuseful_nonterminals = 0;
  සඳහා (symbol_number i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  සඳහා (rule_number r = 0; r < nrules; ++r)
    නම් (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->number);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

static void
reduce_grammar_tables (void)
{
  /* Report and flag useless productions.  */
  {
    සඳහා (rule_number r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    පූර්ණ_සංඛ්‍යා useful = 0;
    පූර්ණ_සංඛ්‍යා useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, parimaa_na *rules_sorted);
    සඳහා (rule_number r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    free (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    සඳහා (rule_number r = 0; r < nrules; ++r)
      {
        item_number *rhsp = rules[r].rhs;
        සඳහා (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          continue;
        *rhsp = rule_number_as_item_number (r);
        rules[r].number = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  සඳහා (rule_number r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

symbol_number *nterm_map = NULL;

static void
nonterminals_reduce (void)
{
  nterm_map = xnmalloc (nnterms, parimaa_na *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    symbol_number n = ntokens;
    සඳහා (symbol_number i = ntokens; i < nsyms; ++i)
      නම් (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    සඳහා (symbol_number i = ntokens; i < nsyms; ++i)
      නම් (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          නම් (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    symbol **symbols_sorted = xnmalloc (nnterms, parimaa_na *symbols_sorted);
    සඳහා (symbol_number i = ntokens; i < nsyms; ++i)
      symbols[i]->content->number = nterm_map[i - ntokens];
    සඳහා (symbol_number i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    සඳහා (symbol_number i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    free (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    සඳහා (rule_number r = 0; r < nrules; ++r)
      සඳහා (item_number *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        නම් (ISVAR (*rhsp))
          *rhsp = symbol_number_as_item_number (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->number = nterm_map[acceptsymbol->content->number - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

void
reduce_output (FILE *out)
{
  නම් (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < ntokens; ++i)
      නම් (reduce_token_unused_in_grammar (i))
        {
          නම් (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    නම් (b)
      fputs ("\n\n", out);
  }

  නම් (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

static void
reduce_print (void)
{
  නම් (nuseless_nonterminals)
    complain (NULL, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  නම් (nuseless_productions)
    complain (NULL, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

void
reduce_grammar (void)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  නම් (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      සඳහා (symbol_list *list = start_symbols; list; list = list->next)
        නම් (!bitset_test (N, list->content.sym->content->number - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      නම් (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      නම් (nuseless_nonterminals)
        nonterminals_reduce ();
      නම් (nuseless_productions)
        reduce_grammar_tables ();
    }

  නම් (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (symbol_number i)
{
  aver (i < ntokens);
  ආපසු !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (const sym_content *sym)
{
  symbol_number n = sym->number;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  ආපසු nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

void
reduce_free (void)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  free (nterm_map);
  nterm_map = NULL;
}
