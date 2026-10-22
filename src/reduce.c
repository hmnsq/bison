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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <bitset.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"
#अन्तर्भाव "print-xml.h"
#अन्तर्भाव "reader.h"
#अन्तर्भाव "reduce.h"
#अन्तर्भाव "symtab.h"

/* Set of nonterminals whose language is not empty.  */
स्थिर bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
स्थिर bitset P;

/* Set of accessible symbols.  */
स्थिर bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
स्थिर bitset V1;

पूर्णाङ्क nuseless_productions;
पूर्णाङ्क nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  कुरु {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } यावत् (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

स्थिर bool
useful_production (rule_संख्या r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  प्रति (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    यदि (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      प्रत्यागम false;
  प्रत्यागम true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

स्थिर शून्य
useless_nonterminals (शून्य)
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

  यावत् (1)
    {
      bitset_copy (Np, N);
      प्रति (rule_संख्या r = 0; r < nrules; ++r)
        यदि (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->संख्या - ntokens);
            bitset_set (P, r);
          }
      यदि (bitset_equal_p (N, Np))
        भङ्ग;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


स्थिर शून्य
inaccessable_symbols (शून्य)
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
  यदि (bitset_test (N, acceptsymbol->content->संख्या - ntokens))
    {
      bitset_set (V, acceptsymbol->content->संख्या);

      यावत् (1)
        {
          bitset_copy (Vp, V);
          प्रति (rule_संख्या r = 0; r < nrules; ++r)
            यदि (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->संख्या))
              {
                प्रति (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  यदि (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          यदि (bitset_equal_p (V, Vp))
            भङ्ग;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->संख्या);   /* end-of-input token */
  bitset_set (V, errtoken->content->संख्या);   /* error token */
  bitset_set (V, undeftoken->content->संख्या); /* some undefined token */

  bitset_free (P);
  P = Pp;

  पूर्णाङ्क nuseful_productions = bitset_गिनती (P);
  nuseless_productions = nrules - nuseful_productions;

  पूर्णाङ्क nuseful_nonterminals = 0;
  प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  प्रति (rule_संख्या r = 0; r < nrules; ++r)
    यदि (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->संख्या);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

स्थिर शून्य
reduce_grammar_tables (शून्य)
{
  /* Report and flag useless productions.  */
  {
    प्रति (rule_संख्या r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    पूर्णाङ्क useful = 0;
    पूर्णाङ्क useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, परिमाण *rules_sorted);
    प्रति (rule_संख्या r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    मुक्त (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    प्रति (rule_संख्या r = 0; r < nrules; ++r)
      {
        item_संख्या *rhsp = rules[r].rhs;
        प्रति (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          अनुवर्त;
        *rhsp = rule_संख्या_as_item_संख्या (r);
        rules[r].संख्या = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  प्रति (rule_संख्या r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

प्रतीक_संख्या *nterm_map = रिक्त;

स्थिर शून्य
nonterminals_reduce (शून्य)
{
  nterm_map = xnmalloc (nnterms, परिमाण *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    प्रतीक_संख्या n = ntokens;
    प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
      यदि (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
      यदि (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          यदि (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    प्रतीक **symbols_sorted = xnmalloc (nnterms, परिमाण *symbols_sorted);
    प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
      symbols[i]->content->संख्या = nterm_map[i - ntokens];
    प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    प्रति (प्रतीक_संख्या i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    मुक्त (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    प्रति (rule_संख्या r = 0; r < nrules; ++r)
      प्रति (item_संख्या *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        यदि (ISVAR (*rhsp))
          *rhsp = प्रतीक_संख्या_as_item_संख्या (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->संख्या = nterm_map[acceptsymbol->content->संख्या - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

शून्य
reduce_output (FILE *out)
{
  यदि (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      प्रति (पूर्णाङ्क i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    प्रति (पूर्णाङ्क i = 0; i < ntokens; ++i)
      यदि (reduce_token_unused_in_grammar (i))
        {
          यदि (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    यदि (b)
      fputs ("\n\n", out);
  }

  यदि (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

स्थिर शून्य
reduce_print (शून्य)
{
  यदि (nuseless_nonterminals)
    complain (रिक्त, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  यदि (nuseless_productions)
    complain (रिक्त, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

शून्य
reduce_grammar (शून्य)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  यदि (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      प्रति (प्रतीक_list *list = start_symbols; list; list = list->next)
        यदि (!bitset_test (N, list->content.sym->content->संख्या - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      यदि (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      यदि (nuseless_nonterminals)
        nonterminals_reduce ();
      यदि (nuseless_productions)
        reduce_grammar_tables ();
    }

  यदि (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (प्रतीक_संख्या i)
{
  aver (i < ntokens);
  प्रत्यागम !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (ध्रुव sym_content *sym)
{
  प्रतीक_संख्या n = sym->संख्या;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  प्रत्यागम nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

शून्य
reduce_free (शून्य)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  मुक्त (nterm_map);
  nterm_map = रिक्त;
}
