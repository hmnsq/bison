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

#شامل <config.h>
#شامل "system.h"

#شامل <bitset.h>

#شامل "complain.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"
#شامل "print-xml.h"
#شامل "reader.h"
#شامل "reduce.h"
#شامل "symtab.h"

/* Set of nonterminals whose language is not empty.  */
ایستا bitset N;

/* Set of rules that have no useless nonterminals in their RHS.  */
ایستا bitset P;

/* Set of accessible symbols.  */
ایستا bitset V;

/* Set of symbols used to define rule precedence (so they are
   'useless', but no warning should be issued).  */
ایستا bitset V1;

صحیح nuseless_productions;
صحیح nuseless_nonterminals;

#define bitset_swap(Lhs, Rhs)                   \
  انجامبده {                                          \
    bitset lhs__ = Lhs;                         \
    Lhs = Rhs;                                  \
    Rhs = lhs__;                                \
  } وقتی (0)

/*-------------------------------------------------------------------.
| Another way to do this would be with a set for each production and |
| then do subset tests against N0, but even for the C grammar the    |
| whole reducing process takes only 2 seconds on my 8Mhz AT.         |
`-------------------------------------------------------------------*/

ایستا bool
useful_production (rule_عدد r, bitset N0)
{
  /* A production is useful if all of the nonterminals in its appear
     in the set of useful nonterminals.  */

  برای (item_عدد *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
    اگر (ISVAR (*rhsp) && !bitset_test (N0, *rhsp - ntokens))
      بازگشت false;
  بازگشت true;
}


/*-----------------------------------------------------------------.
| Compute N, the set of nonterminals whose language is not empty.  |
|                                                                  |
| Remember that rules are 1-origin, symbols are 0-origin.          |
`-----------------------------------------------------------------*/

ایستا تهی
useless_nonterminals (تهی)
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

  وقتی (1)
    {
      bitset_copy (Np, N);
      برای (rule_عدد r = 0; r < nrules; ++r)
        اگر (!bitset_test (P, r)
            && useful_production (r, N))
          {
            bitset_set (Np, rules[r].lhs->عدد - ntokens);
            bitset_set (P, r);
          }
      اگر (bitset_equal_p (N, Np))
        بشکن;
      bitset_swap (N, Np);
    }
  bitset_free (N);
  N = Np;
}


ایستا تهی
inaccessable_symbols (تهی)
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
  اگر (bitset_test (N, acceptsymbol->content->عدد - ntokens))
    {
      bitset_set (V, acceptsymbol->content->عدد);

      وقتی (1)
        {
          bitset_copy (Vp, V);
          برای (rule_عدد r = 0; r < nrules; ++r)
            اگر (!bitset_test (Pp, r)
                && bitset_test (P, r)
                && bitset_test (V, rules[r].lhs->عدد))
              {
                برای (item_عدد *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
                  اگر (ISTOKEN (*rhsp) || bitset_test (N, *rhsp - ntokens))
                    bitset_set (Vp, *rhsp);
                bitset_set (Pp, r);
              }
          اگر (bitset_equal_p (V, Vp))
            بشکن;
          bitset_swap (V, Vp);
        }
    }

  bitset_free (V);
  V = Vp;

  /* These tokens (numbered 0, 1, and 2) are internal to Bison.
     Consider them useful. */
  bitset_set (V, eoftoken->content->عدد);   /* end-of-input token */
  bitset_set (V, errtoken->content->عدد);   /* error token */
  bitset_set (V, undeftoken->content->عدد); /* some undefined token */

  bitset_free (P);
  P = Pp;

  صحیح nuseful_productions = bitset_شمار (P);
  nuseless_productions = nrules - nuseful_productions;

  صحیح nuseful_nonterminals = 0;
  برای (نماد_عدد i = ntokens; i < nsyms; ++i)
    nuseful_nonterminals += bitset_test (V, i);
  nuseless_nonterminals = nnterms - nuseful_nonterminals;

  /* A token that was used in %prec should not be warned about.  */
  برای (rule_عدد r = 0; r < nrules; ++r)
    اگر (rules[r].precsym != 0)
      bitset_set (V1, rules[r].precsym->عدد);
}


/*-------------------------------------------------------------------.
| Put the useless productions at the end of RULES, and adjust NRULES |
| accordingly.                                                       |
`-------------------------------------------------------------------*/

ایستا تهی
reduce_grammar_tables (تهی)
{
  /* Report and flag useless productions.  */
  {
    برای (rule_عدد r = 0; r < nrules; ++r)
      rules[r].useful = bitset_test (P, r);
    grammar_rules_useless_report (_("rule useless in grammar"));
  }

  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    صحیح useful = 0;
    صحیح useless = nrules - nuseless_productions;
    rule *rules_sorted = xnmalloc (nrules, اندازه *rules_sorted);
    برای (rule_عدد r = 0; r < nrules; ++r)
      rules_sorted[rules[r].useful ? useful++ : useless++] = rules[r];
    آزاد (rules);
    rules = rules_sorted;

    /* Renumber the rules markers in RITEMS.  */
    برای (rule_عدد r = 0; r < nrules; ++r)
      {
        item_عدد *rhsp = rules[r].rhs;
        برای (/* Nothing. */; 0 <= *rhsp; ++rhsp)
          ادامه;
        *rhsp = rule_عدد_as_item_عدد (r);
        rules[r].عدد = r;
      }
    nrules -= nuseless_productions;
  }

  /* Adjust NRITEMS.  */
  برای (rule_عدد r = nrules; r < nrules + nuseless_productions; ++r)
    nritems -= rule_rhs_length (&rules[r]) + 1;
}


/*------------------------------.
| Remove useless nonterminals.  |
`------------------------------*/

نماد_عدد *nterm_map = پوچ;

ایستا تهی
nonterminals_reduce (تهی)
{
  nterm_map = xnmalloc (nnterms, اندازه *nterm_map);
  /* Map the nonterminals to their new index: useful first, useless
     afterwards.  Kept for later report.  */
  {
    نماد_عدد n = ntokens;
    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      اگر (bitset_test (V, i))
        nterm_map[i - ntokens] = n++;
    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      اگر (!bitset_test (V, i))
        {
          nterm_map[i - ntokens] = n++;
          اگر (symbols[i]->content->status != used
              && symbols[i] != acceptsymbol)
            complain (&symbols[i]->location, Wother,
                      _("nonterminal useless in grammar: %s"),
                      symbols[i]->tag);
        }
  }

  /* Shuffle elements of tables indexed by symbol number.  */
  {
    نماد **symbols_sorted = xnmalloc (nnterms, اندازه *symbols_sorted);
    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      symbols[i]->content->عدد = nterm_map[i - ntokens];
    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      symbols_sorted[nterm_map[i - ntokens] - ntokens] = symbols[i];
    برای (نماد_عدد i = ntokens; i < nsyms; ++i)
      symbols[i] = symbols_sorted[i - ntokens];
    آزاد (symbols_sorted);
  }

  /* Update nonterminal numbers in the RHS of the rules.  LHS are
     pointers to the symbol structure, they don't need renumbering. */
  {
    برای (rule_عدد r = 0; r < nrules; ++r)
      برای (item_عدد *rhsp = rules[r].rhs; 0 <= *rhsp; ++rhsp)
        اگر (ISVAR (*rhsp))
          *rhsp = نماد_عدد_as_item_عدد (nterm_map[*rhsp - ntokens]);
    acceptsymbol->content->عدد = nterm_map[acceptsymbol->content->عدد - ntokens];
  }

  nsyms -= nuseless_nonterminals;
  nnterms -= nuseless_nonterminals;
}


/*------------------------------------------------------------------.
| Output the detailed results of the reductions.  For FILE.output.  |
`------------------------------------------------------------------*/

تهی
reduce_output (FILE *out)
{
  اگر (nuseless_nonterminals)
    {
      fprintf (out, "%s\n\n", _("Nonterminals useless in grammar"));
      برای (صحیح i = 0; i < nuseless_nonterminals; ++i)
        fprintf (out, "    %s\n", symbols[nsyms + i]->tag);
      fputs ("\n\n", out);
    }

  {
    bool b = false;
    برای (صحیح i = 0; i < ntokens; ++i)
      اگر (reduce_token_unused_in_grammar (i))
        {
          اگر (!b)
            fprintf (out, "%s\n\n", _("Terminals unused in grammar"));
          b = true;
          fprintf (out, "    %s\n", symbols[i]->tag);
        }
    اگر (b)
      fputs ("\n\n", out);
  }

  اگر (nuseless_productions)
    grammar_rules_partial_print (out, _("Rules useless in grammar"),
                                 rule_useless_in_grammar_p);
}


/*-------------------------------.
| Report the results to STDERR.  |
`-------------------------------*/

ایستا تهی
reduce_print (تهی)
{
  اگر (nuseless_nonterminals)
    complain (پوچ, Wother, ngettext ("%d nonterminal useless in grammar",
                                      "%d nonterminals useless in grammar",
                                      nuseless_nonterminals),
              nuseless_nonterminals);
  اگر (nuseless_productions)
    complain (پوچ, Wother, ngettext ("%d rule useless in grammar",
                                      "%d rules useless in grammar",
                                      nuseless_productions),
              nuseless_productions);
}

تهی
reduce_grammar (تهی)
{
  /* Allocate the global sets used to compute the reduced grammar */

  N = bitset_create (nnterms, BITSET_FIXED);
  P =  bitset_create (nrules, BITSET_FIXED);
  V = bitset_create (nsyms, BITSET_FIXED);
  V1 = bitset_create (nsyms, BITSET_FIXED);

  useless_nonterminals ();
  inaccessable_symbols ();

  /* Did we reduce something? */
  اگر (nuseless_nonterminals || nuseless_productions)
    {
      reduce_print ();

      // Check that start symbols have non-empty languages.
      bool failure = false;
      برای (نماد_list *list = start_symbols; list; list = list->next)
        اگر (!bitset_test (N, list->content.sym->content->عدد - ntokens))
          {
            failure = true;
            complain (&list->sym_loc, complaint,
                      _("start symbol %s does not derive any sentence"),
                      list->content.sym->tag);
          }
      اگر (failure)
        exit (EXIT_FAILURE);

      /* First reduce the nonterminals, as they renumber themselves in the
         whole grammar.  If you change the order, nonterms would be
         renumbered only in the reduced grammar.  */
      اگر (nuseless_nonterminals)
        nonterminals_reduce ();
      اگر (nuseless_productions)
        reduce_grammar_tables ();
    }

  اگر (trace_flag & trace_grammar)
    {
      grammar_dump (stderr, "Reduced Grammar");

      fprintf (stderr, "reduced %s defines %d terminals, %d nonterminals"
               ", and %d productions.\n",
               grammar_file, ntokens, nnterms, nrules);
    }
}

bool
reduce_token_unused_in_grammar (نماد_عدد i)
{
  aver (i < ntokens);
  بازگشت !bitset_test (V, i) && !bitset_test (V1, i);
}

bool
reduce_nonterminal_useless_in_grammar (ثابت sym_content *sym)
{
  نماد_عدد n = sym->عدد;
  aver (ntokens <= n && n < nsyms + nuseless_nonterminals);
  بازگشت nsyms <= n;
}

/*-----------------------------------------------------------.
| Free the global sets used to compute the reduced grammar.  |
`-----------------------------------------------------------*/

تهی
reduce_free (تهی)
{
  bitset_free (N);
  bitset_free (V);
  bitset_free (V1);
  bitset_free (P);
  آزاد (nterm_map);
  nterm_map = پوچ;
}
