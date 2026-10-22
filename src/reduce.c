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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <bitset.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"
#ಅನ್ತರ್ಭಾವ "print-xml.h"
#ಅನ್ತರ್ಭಾವ "reader.h"
#ಅನ್ತರ್ಭಾವ "reduce.h"
#ಅನ್ತರ್ಭಾವ "symtab.h"

/* Set of nonterminals whose language is not empty.  */
ಸ್ಥಿರ bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
ಸ್ಥಿರ bitset P;

/* Set of accessible symbols.  */
ಸ್ಥಿರ bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
ಸ್ಥಿರ bitset V1;

ಪೂರ್ಣಾಙ್ಕ nuseless_productions;
ಪೂರ್ಣಾಙ್ಕ nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  ಕುರು {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } ಯಾವತ್ (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ bool
useful_production (rule_ಸಂಖ್ಯೆ r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    ಯದಿ (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತ್ಯಾಗಮ true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
useless_nonterminals (ಶೂನ್ಯ)
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

  ಯಾವತ್ (1)
    {
      bitset_copy (Np, N);
      ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
        ಯದಿ (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->ಸಂಖ್ಯೆ - ntokens);
            bitset_set (P, r);
          }
      ಯದಿ (bitset_equal_p (N, Np))
        ಭಙ್ಗ;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


ಸ್ಥಿರ ಶೂನ್ಯ
inaccessable_symbols (ಶೂನ್ಯ)
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
  ಯದಿ (bitset_test (N, acceptsymbol->content->ಸಂಖ್ಯೆ - ntokens))
    {
      bitset_set (V, acceptsymbol->content->ಸಂಖ್ಯೆ);

      ಯಾವತ್ (1)
        {
          bitset_copy (Vp, V);
          ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
            ಯದಿ (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->ಸಂಖ್ಯೆ))
              {
                ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  ಯದಿ (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          ಯದಿ (bitset_equal_p (V, Vp))
            ಭಙ್ಗ;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->ಸಂಖ್ಯೆ);   /* end-of-input token */
  bitset_set (V, errtoken->content->ಸಂಖ್ಯೆ);   /* error token */
  bitset_set (V, undeftoken->content->ಸಂಖ್ಯೆ); /* some undefined token */

  bitset_free (P);
  P = Pp;

  ಪೂರ್ಣಾಙ್ಕ nuseful_productions = bitset_ಎಣಿಕೆ (P);
  nuseless_productions = nrules - nuseful_productions;

  ಪೂರ್ಣಾಙ್ಕ nuseful_nonterminals = 0;
  ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
    ಯದಿ (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->ಸಂಖ್ಯೆ);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
reduce_grammar_tables (ಶೂನ್ಯ)
{
  /* Report and flag useless productions.  */
  {
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    ಪೂರ್ಣಾಙ್ಕ useful = 0;
    ಪೂರ್ಣಾಙ್ಕ useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, ಪರಿಮಾಣ *rules_sorted);
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    ಮುಕ್ತ (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
      {
        item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs;
        ಪ್ರತಿ (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          ಅನುವರ್ತ;
        *rhsp = rule_ಸಂಖ್ಯೆ_as_item_ಸಂಖ್ಯೆ (r);
        rules[r].ಸಂಖ್ಯೆ = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *nterm_map = ರಿಕ್ತ;

ಸ್ಥಿರ ಶೂನ್ಯ
nonterminals_reduce (ಶೂನ್ಯ)
{
  nterm_map = xnmalloc (nnterms, ಪರಿಮಾಣ *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ n = ntokens;
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      ಯದಿ (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      ಯದಿ (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          ಯದಿ (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    ಚಿಹ್ನೆ **symbols_sorted = xnmalloc (nnterms, ಪರಿಮಾಣ *symbols_sorted);
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      symbols[i]->content->ಸಂಖ್ಯೆ = nterm_map[i - ntokens];
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    ಪ್ರತಿ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    ಮುಕ್ತ (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    ಪ್ರತಿ (rule_ಸಂಖ್ಯೆ r = 0; r < nrules; ++r)
      ಪ್ರತಿ (item_ಸಂಖ್ಯೆ *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        ಯದಿ (ISVAR (*rhsp))
          *rhsp = ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_as_item_ಸಂಖ್ಯೆ (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->ಸಂಖ್ಯೆ = nterm_map[acceptsymbol->content->ಸಂಖ್ಯೆ - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

ಶೂನ್ಯ
reduce_output (FILE *out)
{
  ಯದಿ (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ntokens; ++i)
      ಯದಿ (reduce_token_unused_in_grammar (i))
        {
          ಯದಿ (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    ಯದಿ (b)
      fputs ("\n\n", out);
  }

  ಯದಿ (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
reduce_print (ಶೂನ್ಯ)
{
  ಯದಿ (nuseless_nonterminals)
    complain (ರಿಕ್ತ, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  ಯದಿ (nuseless_productions)
    complain (ರಿಕ್ತ, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

ಶೂನ್ಯ
reduce_grammar (ಶೂನ್ಯ)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  ಯದಿ (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      ಪ್ರತಿ (ಚಿಹ್ನೆ_list *list = start_symbols; list; list = list->next)
        ಯದಿ (!bitset_test (N, list->content.sym->content->ಸಂಖ್ಯೆ - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      ಯದಿ (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      ಯದಿ (nuseless_nonterminals)
        nonterminals_reduce ();
      ಯದಿ (nuseless_productions)
        reduce_grammar_tables ();
    }

  ಯದಿ (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i)
{
  aver (i < ntokens);
  ಪ್ರತ್ಯಾಗಮ !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (ಧ್ರುವ sym_content *sym)
{
  ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ n = sym->ಸಂಖ್ಯೆ;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  ಪ್ರತ್ಯಾಗಮ nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

ಶೂನ್ಯ
reduce_free (ಶೂನ್ಯ)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  ಮುಕ್ತ (nterm_map);
  nterm_map = ರಿಕ್ತ;
}
