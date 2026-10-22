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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <bitset.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"
#ଅନ୍ତର୍ଭାଵ "print-xml.h"
#ଅନ୍ତର୍ଭାଵ "reader.h"
#ଅନ୍ତର୍ଭାଵ "reduce.h"
#ଅନ୍ତର୍ଭାଵ "symtab.h"

/* Set of nonterminals whose language is not empty.  */
ସ୍ଥିର bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
ସ୍ଥିର bitset P;

/* Set of accessible symbols.  */
ସ୍ଥିର bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
ସ୍ଥିର bitset V1;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା nuseless_productions;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  କର {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } ଯେତେବେଳେ (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

ସ୍ଥିର bool
useful_production (rule_ସଂଖ୍ୟା r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    ଯଦି (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      ଫେରନ୍ତୁ false;
  ଫେରନ୍ତୁ true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
useless_nonterminals (ଶୂନ୍ୟ)
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

  ଯେତେବେଳେ (1)
    {
      bitset_copy (Np, N);
      ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
        ଯଦି (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->ସଂଖ୍ୟା - ntokens);
            bitset_set (P, r);
          }
      ଯଦି (bitset_equal_p (N, Np))
        ଭାଙ୍ଗ;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


ସ୍ଥିର ଶୂନ୍ୟ
inaccessable_symbols (ଶୂନ୍ୟ)
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
  ଯଦି (bitset_test (N, acceptsymbol->content->ସଂଖ୍ୟା - ntokens))
    {
      bitset_set (V, acceptsymbol->content->ସଂଖ୍ୟା);

      ଯେତେବେଳେ (1)
        {
          bitset_copy (Vp, V);
          ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
            ଯଦି (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->ସଂଖ୍ୟା))
              {
                ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  ଯଦି (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          ଯଦି (bitset_equal_p (V, Vp))
            ଭାଙ୍ଗ;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->ସଂଖ୍ୟା);   /* end-of-input token */
  bitset_set (V, errtoken->content->ସଂଖ୍ୟା);   /* error token */
  bitset_set (V, undeftoken->content->ସଂଖ୍ୟା); /* some undefined token */

  bitset_free (P);
  P = Pp;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nuseful_productions = bitset_ଗଣନା (P);
  nuseless_productions = nrules - nuseful_productions;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nuseful_nonterminals = 0;
  ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
    ଯଦି (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->ସଂଖ୍ୟା);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
reduce_grammar_tables (ଶୂନ୍ୟ)
{
  /* Report and flag useless productions.  */
  {
    ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା useful = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, ଆକାର *rules_sorted);
    ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    ମୁକ୍ତ (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
      {
        item_ସଂଖ୍ୟା *rhsp = rules[r].rhs;
        ପାଇଁ (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          ଜାରି;
        *rhsp = rule_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (r);
        rules[r].ସଂଖ୍ୟା = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  ପାଇଁ (rule_ସଂଖ୍ୟା r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

ପ୍ରତୀକ_ସଂଖ୍ୟା *nterm_map = ଖାଲି;

ସ୍ଥିର ଶୂନ୍ୟ
nonterminals_reduce (ଶୂନ୍ୟ)
{
  nterm_map = xnmalloc (nnterms, ଆକାର *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    ପ୍ରତୀକ_ସଂଖ୍ୟା n = ntokens;
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      ଯଦି (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      ଯଦି (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          ଯଦି (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    ପ୍ରତୀକ **symbols_sorted = xnmalloc (nnterms, ଆକାର *symbols_sorted);
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      symbols[i]->content->ସଂଖ୍ୟା = nterm_map[i - ntokens];
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    ପାଇଁ (ପ୍ରତୀକ_ସଂଖ୍ୟା i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    ମୁକ୍ତ (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    ପାଇଁ (rule_ସଂଖ୍ୟା r = 0; r < nrules; ++r)
      ପାଇଁ (item_ସଂଖ୍ୟା *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        ଯଦି (ISVAR (*rhsp))
          *rhsp = ପ୍ରତୀକ_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->ସଂଖ୍ୟା = nterm_map[acceptsymbol->content->ସଂଖ୍ୟା - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

ଶୂନ୍ୟ
reduce_output (FILE *out)
{
  ଯଦି (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ntokens; ++i)
      ଯଦି (reduce_token_unused_in_grammar (i))
        {
          ଯଦି (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    ଯଦି (b)
      fputs ("\n\n", out);
  }

  ଯଦି (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
reduce_print (ଶୂନ୍ୟ)
{
  ଯଦି (nuseless_nonterminals)
    complain (ଖାଲି, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  ଯଦି (nuseless_productions)
    complain (ଖାଲି, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

ଶୂନ୍ୟ
reduce_grammar (ଶୂନ୍ୟ)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  ଯଦି (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      ପାଇଁ (ପ୍ରତୀକ_list *list = start_symbols; list; list = list->next)
        ଯଦି (!bitset_test (N, list->content.sym->content->ସଂଖ୍ୟା - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      ଯଦି (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      ଯଦି (nuseless_nonterminals)
        nonterminals_reduce ();
      ଯଦି (nuseless_productions)
        reduce_grammar_tables ();
    }

  ଯଦି (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (ପ୍ରତୀକ_ସଂଖ୍ୟା i)
{
  aver (i < ntokens);
  ଫେରନ୍ତୁ !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (ଅଚଳ sym_content *sym)
{
  ପ୍ରତୀକ_ସଂଖ୍ୟା n = sym->ସଂଖ୍ୟା;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  ଫେରନ୍ତୁ nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

ଶୂନ୍ୟ
reduce_free (ଶୂନ୍ୟ)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  ମୁକ୍ତ (nterm_map);
  nterm_map = ଖାଲି;
}
