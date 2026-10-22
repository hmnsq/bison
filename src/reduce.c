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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <bitset.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"
#અન્તર્ભાવ "print-xml.h"
#અન્તર્ભાવ "reader.h"
#અન્તર્ભાવ "reduce.h"
#અન્તર્ભાવ "symtab.h"

/* Set of nonterminals whose language is not empty.  */
સ્થિર bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
સ્થિર bitset P;

/* Set of accessible symbols.  */
સ્થિર bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
સ્થિર bitset V1;

પૂર્ણાંક nuseless_productions;
પૂર્ણાંક nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  કરો {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } જ્યારે (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

સ્થિર bool
useful_production (rule_સંખ્યા r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  માટે (item_સંખ્યા *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    જો (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      પાછા false;
  પાછા true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

સ્થિર શૂન્ય
useless_nonterminals (શૂન્ય)
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

  જ્યારે (1)
    {
      bitset_copy (Np, N);
      માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
        જો (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->સંખ્યા - ntokens);
            bitset_set (P, r);
          }
      જો (bitset_equal_p (N, Np))
        તોડો;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


સ્થિર શૂન્ય
inaccessable_symbols (શૂન્ય)
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
  જો (bitset_test (N, acceptsymbol->content->સંખ્યા - ntokens))
    {
      bitset_set (V, acceptsymbol->content->સંખ્યા);

      જ્યારે (1)
        {
          bitset_copy (Vp, V);
          માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
            જો (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->સંખ્યા))
              {
                માટે (item_સંખ્યા *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  જો (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          જો (bitset_equal_p (V, Vp))
            તોડો;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->સંખ્યા);   /* end-of-input token */
  bitset_set (V, errtoken->content->સંખ્યા);   /* error token */
  bitset_set (V, undeftoken->content->સંખ્યા); /* some undefined token */

  bitset_free (P);
  P = Pp;

  પૂર્ણાંક nuseful_productions = bitset_ગણતરી (P);
  nuseless_productions = nrules - nuseful_productions;

  પૂર્ણાંક nuseful_nonterminals = 0;
  માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
    જો (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->સંખ્યા);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

સ્થિર શૂન્ય
reduce_grammar_tables (શૂન્ય)
{
  /* Report and flag useless productions.  */
  {
    માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    પૂર્ણાંક useful = 0;
    પૂર્ણાંક useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, કદ *rules_sorted);
    માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    મુક્ત (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
      {
        item_સંખ્યા *rhsp = rules[r].rhs;
        માટે (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          ચાલુ;
        *rhsp = rule_સંખ્યા_as_item_સંખ્યા (r);
        rules[r].સંખ્યા = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  માટે (rule_સંખ્યા r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

પ્રતીક_સંખ્યા *nterm_map = ખાલી;

સ્થિર શૂન્ય
nonterminals_reduce (શૂન્ય)
{
  nterm_map = xnmalloc (nnterms, કદ *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    પ્રતીક_સંખ્યા n = ntokens;
    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      જો (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      જો (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          જો (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    પ્રતીક **symbols_sorted = xnmalloc (nnterms, કદ *symbols_sorted);
    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      symbols[i]->content->સંખ્યા = nterm_map[i - ntokens];
    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    માટે (પ્રતીક_સંખ્યા i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    મુક્ત (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    માટે (rule_સંખ્યા r = 0; r < nrules; ++r)
      માટે (item_સંખ્યા *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        જો (ISVAR (*rhsp))
          *rhsp = પ્રતીક_સંખ્યા_as_item_સંખ્યા (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->સંખ્યા = nterm_map[acceptsymbol->content->સંખ્યા - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

શૂન્ય
reduce_output (FILE *out)
{
  જો (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      માટે (પૂર્ણાંક i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    માટે (પૂર્ણાંક i = 0; i < ntokens; ++i)
      જો (reduce_token_unused_in_grammar (i))
        {
          જો (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    જો (b)
      fputs ("\n\n", out);
  }

  જો (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

સ્થિર શૂન્ય
reduce_print (શૂન્ય)
{
  જો (nuseless_nonterminals)
    complain (ખાલી, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  જો (nuseless_productions)
    complain (ખાલી, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

શૂન્ય
reduce_grammar (શૂન્ય)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  જો (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      માટે (પ્રતીક_list *list = start_symbols; list; list = list->next)
        જો (!bitset_test (N, list->content.sym->content->સંખ્યા - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      જો (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      જો (nuseless_nonterminals)
        nonterminals_reduce ();
      જો (nuseless_productions)
        reduce_grammar_tables ();
    }

  જો (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (પ્રતીક_સંખ્યા i)
{
  aver (i < ntokens);
  પાછા !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (અચલ sym_content *sym)
{
  પ્રતીક_સંખ્યા n = sym->સંખ્યા;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  પાછા nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

શૂન્ય
reduce_free (શૂન્ય)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  મુક્ત (nterm_map);
  nterm_map = ખાલી;
}
