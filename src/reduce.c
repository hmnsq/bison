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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <bitset.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"
#அந்தர்பாவ "print-xml.h"
#அந்தர்பாவ "reader.h"
#அந்தர்பாவ "reduce.h"
#அந்தர்பாவ "symtab.h"

/* Set of nonterminals whose language is not empty.  */
நிலையான bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
நிலையான bitset P;

/* Set of accessible symbols.  */
நிலையான bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
நிலையான bitset V1;

முழு nuseless_productions;
முழு nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  செய் {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } போது (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

நிலையான bool
useful_production (rule_எண் r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  க்கு (item_எண் *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    எனில் (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      திரும்பு false;
  திரும்பு true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

நிலையான வெறுமை
useless_nonterminals (வெறுமை)
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

  போது (1)
    {
      bitset_copy (Np, N);
      க்கு (rule_எண் r = 0; r < nrules; ++r)
        எனில் (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->எண் - ntokens);
            bitset_set (P, r);
          }
      எனில் (bitset_equal_p (N, Np))
        நிறுத்து;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


நிலையான வெறுமை
inaccessable_symbols (வெறுமை)
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
  எனில் (bitset_test (N, acceptsymbol->content->எண் - ntokens))
    {
      bitset_set (V, acceptsymbol->content->எண்);

      போது (1)
        {
          bitset_copy (Vp, V);
          க்கு (rule_எண் r = 0; r < nrules; ++r)
            எனில் (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->எண்))
              {
                க்கு (item_எண் *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  எனில் (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          எனில் (bitset_equal_p (V, Vp))
            நிறுத்து;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->எண்);   /* end-of-input token */
  bitset_set (V, errtoken->content->எண்);   /* error token */
  bitset_set (V, undeftoken->content->எண்); /* some undefined token */

  bitset_free (P);
  P = Pp;

  முழு nuseful_productions = bitset_மொத்தம் (P);
  nuseless_productions = nrules - nuseful_productions;

  முழு nuseful_nonterminals = 0;
  க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  க்கு (rule_எண் r = 0; r < nrules; ++r)
    எனில் (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->எண்);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

நிலையான வெறுமை
reduce_grammar_tables (வெறுமை)
{
  /* Report and flag useless productions.  */
  {
    க்கு (rule_எண் r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    முழு useful = 0;
    முழு useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, அளவு *rules_sorted);
    க்கு (rule_எண் r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    விடுவி (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    க்கு (rule_எண் r = 0; r < nrules; ++r)
      {
        item_எண் *rhsp = rules[r].rhs;
        க்கு (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          தொடர்;
        *rhsp = rule_எண்_as_item_எண் (r);
        rules[r].எண் = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  க்கு (rule_எண் r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

குறியீடு_எண் *nterm_map = வெற்று;

நிலையான வெறுமை
nonterminals_reduce (வெறுமை)
{
  nterm_map = xnmalloc (nnterms, அளவு *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    குறியீடு_எண் n = ntokens;
    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      எனில் (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      எனில் (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          எனில் (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    குறியீடு **symbols_sorted = xnmalloc (nnterms, அளவு *symbols_sorted);
    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      symbols[i]->content->எண் = nterm_map[i - ntokens];
    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    க்கு (குறியீடு_எண் i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    விடுவி (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    க்கு (rule_எண் r = 0; r < nrules; ++r)
      க்கு (item_எண் *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        எனில் (ISVAR (*rhsp))
          *rhsp = குறியீடு_எண்_as_item_எண் (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->எண் = nterm_map[acceptsymbol->content->எண் - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

வெறுமை
reduce_output (FILE *out)
{
  எனில் (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      க்கு (முழு i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    க்கு (முழு i = 0; i < ntokens; ++i)
      எனில் (reduce_token_unused_in_grammar (i))
        {
          எனில் (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    எனில் (b)
      fputs ("\n\n", out);
  }

  எனில் (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

நிலையான வெறுமை
reduce_print (வெறுமை)
{
  எனில் (nuseless_nonterminals)
    complain (வெற்று, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  எனில் (nuseless_productions)
    complain (வெற்று, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

வெறுமை
reduce_grammar (வெறுமை)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  எனில் (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      க்கு (குறியீடு_list *list = start_symbols; list; list = list->next)
        எனில் (!bitset_test (N, list->content.sym->content->எண் - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      எனில் (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      எனில் (nuseless_nonterminals)
        nonterminals_reduce ();
      எனில் (nuseless_productions)
        reduce_grammar_tables ();
    }

  எனில் (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (குறியீடு_எண் i)
{
  aver (i < ntokens);
  திரும்பு !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (மாறிலி sym_content *sym)
{
  குறியீடு_எண் n = sym->எண்;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  திரும்பு nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

வெறுமை
reduce_free (வெறுமை)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  விடுவி (nterm_map);
  nterm_map = வெற்று;
}
