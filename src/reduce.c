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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <bitset.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"
#অন্তর্ভাব "print-xml.h"
#অন্তর্ভাব "reader.h"
#অন্তর্ভাব "reduce.h"
#অন্তর্ভাব "symtab.h"

/* Set of nonterminals whose language is not empty.  */
স্থির bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
স্থির bitset P;

/* Set of accessible symbols.  */
স্থির bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
স্থির bitset V1;

পূর্ণাঙ্ক nuseless_productions;
পূর্ণাঙ্ক nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  কুরু {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } যাবত্ (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

স্থির bool
useful_production (rule_সংখ্যা r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  প্রতি (item_সংখ্যা *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    যদি (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      প্রত্যাগম false;
  প্রত্যাগম true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

স্থির শূন্য
useless_nonterminals (শূন্য)
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

  যাবত্ (1)
    {
      bitset_copy (Np, N);
      প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
        যদি (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->সংখ্যা - ntokens);
            bitset_set (P, r);
          }
      যদি (bitset_equal_p (N, Np))
        ভঙ্গ;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


স্থির শূন্য
inaccessable_symbols (শূন্য)
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
  যদি (bitset_test (N, acceptsymbol->content->সংখ্যা - ntokens))
    {
      bitset_set (V, acceptsymbol->content->সংখ্যা);

      যাবত্ (1)
        {
          bitset_copy (Vp, V);
          প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
            যদি (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->সংখ্যা))
              {
                প্রতি (item_সংখ্যা *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  যদি (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          যদি (bitset_equal_p (V, Vp))
            ভঙ্গ;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->সংখ্যা);   /* end-of-input token */
  bitset_set (V, errtoken->content->সংখ্যা);   /* error token */
  bitset_set (V, undeftoken->content->সংখ্যা); /* some undefined token */

  bitset_free (P);
  P = Pp;

  পূর্ণাঙ্ক nuseful_productions = bitset_গুনতি (P);
  nuseless_productions = nrules - nuseful_productions;

  পূর্ণাঙ্ক nuseful_nonterminals = 0;
  প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
    যদি (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->সংখ্যা);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

স্থির শূন্য
reduce_grammar_tables (শূন্য)
{
  /* Report and flag useless productions.  */
  {
    প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    পূর্ণাঙ্ক useful = 0;
    পূর্ণাঙ্ক useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, পরিমাণ *rules_sorted);
    প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    মুক্ত (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
      {
        item_সংখ্যা *rhsp = rules[r].rhs;
        প্রতি (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          অনুবর্ত;
        *rhsp = rule_সংখ্যা_as_item_সংখ্যা (r);
        rules[r].সংখ্যা = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  প্রতি (rule_সংখ্যা r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

প্রতীক_সংখ্যা *nterm_map = রিক্ত;

স্থির শূন্য
nonterminals_reduce (শূন্য)
{
  nterm_map = xnmalloc (nnterms, পরিমাণ *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    প্রতীক_সংখ্যা n = ntokens;
    প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      যদি (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      যদি (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          যদি (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    প্রতীক **symbols_sorted = xnmalloc (nnterms, পরিমাণ *symbols_sorted);
    প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      symbols[i]->content->সংখ্যা = nterm_map[i - ntokens];
    প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    প্রতি (প্রতীক_সংখ্যা i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    মুক্ত (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    প্রতি (rule_সংখ্যা r = 0; r < nrules; ++r)
      প্রতি (item_সংখ্যা *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        যদি (ISVAR (*rhsp))
          *rhsp = প্রতীক_সংখ্যা_as_item_সংখ্যা (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->সংখ্যা = nterm_map[acceptsymbol->content->সংখ্যা - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

শূন্য
reduce_output (FILE *out)
{
  যদি (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      প্রতি (পূর্ণাঙ্ক i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    প্রতি (পূর্ণাঙ্ক i = 0; i < ntokens; ++i)
      যদি (reduce_token_unused_in_grammar (i))
        {
          যদি (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    যদি (b)
      fputs ("\n\n", out);
  }

  যদি (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

স্থির শূন্য
reduce_print (শূন্য)
{
  যদি (nuseless_nonterminals)
    complain (রিক্ত, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  যদি (nuseless_productions)
    complain (রিক্ত, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

শূন্য
reduce_grammar (শূন্য)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  যদি (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      প্রতি (প্রতীক_list *list = start_symbols; list; list = list->next)
        যদি (!bitset_test (N, list->content.sym->content->সংখ্যা - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      যদি (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      যদি (nuseless_nonterminals)
        nonterminals_reduce ();
      যদি (nuseless_productions)
        reduce_grammar_tables ();
    }

  যদি (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (প্রতীক_সংখ্যা i)
{
  aver (i < ntokens);
  প্রত্যাগম !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (ধ্রুব sym_content *sym)
{
  প্রতীক_সংখ্যা n = sym->সংখ্যা;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  প্রত্যাগম nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

শূন্য
reduce_free (শূন্য)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  মুক্ত (nterm_map);
  nterm_map = রিক্ত;
}
