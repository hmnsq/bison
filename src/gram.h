/* Data definitions for internal representation of Bison's input.

   Copyright (C) 1984, 1986, 1989, 1992, 2001-2007, 2009-2015, 2018-2022
   Free Software Foundation, Inc.

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

#ifndef GRAM_H_
# define GRAM_H_

/* Representation of the grammar rules:

   NTOKENS is the number of tokens, and NNTERMS is the number of
   nonterminals (aka variables).  NSYMS is the total number, NTOKENS +
   NNTERMS.

   Each symbol (either token or nterm) receives a symbol number.
   Numbers 0 to NTOKENS - 1 are for tokens, and NTOKENS to NSYMS - 1
   are for nterms.  Symbol number zero is the end-of-input token.
   This token is counted in ntokens.  The true number of token values
   assigned is NTOKENS reduced by one for each alias declaration.

   The rules receive rule numbers 1 to NRULES in the order they are
   written.  More precisely Bison augments the grammar with the
   initial rule, '$accept: START-SYMBOL $end', which is numbered 1,
   all the user rules are 2, 3 etc.  Each time a rule number is
   presented to the user, we subtract 1, so *displayed* rule numbers
   are 0, 1, 2...

   Internally, we cannot use the number 0 for a rule because for
   instance RITEM stores both symbols (the RHS) and rule numbers: the
   symbols are integers >= 0, and rule numbers are stored negative.
   Therefore 0 cannot be used, since it would be both the rule number
   0, and the token $end.

   Actions are accessed via the rule number.

   The rules themselves are described by several arrays: amongst which
   RITEM, and RULES.

   RULES is an array of rules, whose members are:

   RULES[R].lhs -- the symbol of the left hand side of rule R.

   RULES[R].rhs -- the beginning of the portion of RITEM for rule R.

   RULES[R].prec -- the symbol providing the precedence level of R.

   RULES[R].precsym -- the symbol attached (via %prec) to give its
   precedence to R.  Of course, if set, it is equal to 'prec', but we
   need to distinguish one from the other when reducing: a symbol used
   in a %prec is not useless.

   RULES[R].assoc -- the associativity of R.

   RULES[R].dprec -- the dynamic precedence level of R (for GLR
   parsing).

   RULES[R].merger -- index of merging function for R (for GLR
   parsing).

   RULES[R].line -- the line where R was defined.

   RULES[R].useful -- whether the rule is used.  False if thrown away
   by reduce().

   The right hand side of rules is stored as symbol numbers in a
   portion of RITEM.

   The length of the portion is one greater than the number of symbols
   in the rule's right hand side.  The last element in the portion
   contains -R, which identifies it as the end of a portion and says
   which rule it is for.

   The portions of RITEM come in order of increasing rule number.
   NRITEMS is the total length of RITEM.  Each element of RITEM is
   called an "item" of type item_number and its index in RITEM is an
   item_index.

   Item numbers are used in the finite state machine to represent
   places that parsing can get to.

   SYMBOLS[I]->prec records the precedence level of each symbol.

   Precedence levels are assigned in increasing order starting with 1
   so that numerically higher precedence values mean tighter binding
   as they ought to.  Zero as a symbol or rule's precedence means none
   is assigned.

   Associativities are recorded similarly in SYMBOLS[I]->assoc.  */

# include "system.h"

# include "location.h"
# include "symtab.h"

# define ISTOKEN(i)     ((i) < ntokens)
# define ISVAR(i)       ((i) >= ntokens)

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ nsyms;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ntokens;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ nnterms;

/* Elements of ritem. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ item_ಸಂಖ್ಯೆ;
# define ITEM_ಸಂಖ್ಯೆ_MAX INT_MAX
ಬಾಹ್ಯ item_ಸಂಖ್ಯೆ *ritem;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ nritems;

/* Indices into ritem. */
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ item_index;

/* There is weird relationship between OT1H item_number and OTOH
   symbol_number and rule_number: we store the latter in
   item_number.  symbol_number values are stored as-is, while
   the negation of (rule_number + 1) is stored.

   Therefore, a symbol_number must be a valid item_number, and we
   sometimes have to perform the converse transformation.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ item_ಸಂಖ್ಯೆ
ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ_as_item_ಸಂಖ್ಯೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  ಪ್ರತ್ಯಾಗಮ sym;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ
item_ಸಂಖ್ಯೆ_as_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (item_ಸಂಖ್ಯೆ i)
{
  ಪ್ರತ್ಯಾಗಮ i;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
item_ಸಂಖ್ಯೆ_is_ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ (item_ಸಂಖ್ಯೆ i)
{
  ಪ್ರತ್ಯಾಗಮ i >= 0;
}

/* Rule numbers.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ rule_ಸಂಖ್ಯೆ;
# define RULE_ಸಂಖ್ಯೆ_MAX INT_MAX

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ item_ಸಂಖ್ಯೆ
rule_ಸಂಖ್ಯೆ_as_item_ಸಂಖ್ಯೆ (rule_ಸಂಖ್ಯೆ r)
{
  ಪ್ರತ್ಯಾಗಮ -1 - r;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ rule_ಸಂಖ್ಯೆ
item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (item_ಸಂಖ್ಯೆ i)
{
  ಪ್ರತ್ಯಾಗಮ -1 - i;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (item_ಸಂಖ್ಯೆ i)
{
  ಪ್ರತ್ಯಾಗಮ i < 0;
}


/*--------.
| Rules.  |
`--------*/

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  /* The number of the rule in the source.  It is usually the index in
     RULES too, except if there are useless rules.  */
  rule_ಸಂಖ್ಯೆ code;

  /* The index in RULES.  Usually the rule number in the source,
     except if some rules are useless.  */
  rule_ಸಂಖ್ಯೆ ಸಂಖ್ಯೆ;

  sym_content *lhs;
  item_ಸಂಖ್ಯೆ *rhs;

  /* This symbol provides both the associativity, and the precedence. */
  sym_content *prec;

  ಪೂರ್ಣಾಙ್ಕ dprec;
  ಪೂರ್ಣಾಙ್ಕ merger;

  /* This symbol was attached to the rule via %prec. */
  sym_content *precsym;

  /* Location of the rhs.  */
  location location;
  bool useful;
  bool is_predicate;

  /* Counts of the numbers of expected conflicts for this rule, or -1 if none
     given. */
  ಪೂರ್ಣಾಙ್ಕ expected_sr_conflicts;
  ಪೂರ್ಣಾಙ್ಕ expected_rr_conflicts;

  ಧ್ರುವ ವರ್ಣ *action;
  location action_loc;
} rule;

/* The used rules (size NRULES).  */
ಬಾಹ್ಯ rule *rules;
ಬಾಹ್ಯ rule_ಸಂಖ್ಯೆ nrules;

/* Get the rule associated to this item.  ITEM points inside RITEM.  */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ rule ಧ್ರುವ *
item_rule (item_ಸಂಖ್ಯೆ ಧ್ರುವ *item)
{
  item_ಸಂಖ್ಯೆ ಧ್ರುವ *sp = item;
  ಯಾವತ್ (!item_ಸಂಖ್ಯೆ_is_rule_ಸಂಖ್ಯೆ (*sp))
    ++sp;
  rule_ಸಂಖ್ಯೆ r = item_ಸಂಖ್ಯೆ_as_rule_ಸಂಖ್ಯೆ (*sp);
  ಪ್ರತ್ಯಾಗಮ &rules[r];
}

/* Pretty-print this ITEM (as in the report).  ITEM points inside
   RITEM.  PREVIOUS_RULE is used to see if the lhs is common, in which
   case LHS is factored.  Passing NULL is fine.  */
ಶೂನ್ಯ item_print (item_ಸಂಖ್ಯೆ *item, rule ಧ್ರುವ *previous_rule,
                 FILE *out);

/*--------.
| Rules.  |
`--------*/

/* A function that selects a rule.  */
ಪ್ರಕಾರನಿರ್ದೇಶ bool (*rule_filter) (rule ಧ್ರುವ *);

/* Whether is an accepting rule (i.e., its reduction terminates
   parsing with success). */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
rule_is_initial (rule ಧ್ರುವ *r)
{
  /* In the case of multistart, we need to check whether the LHS is
     $accept.  In the case of "unistart", it would suffice to
     check whether this is rule number 0.  */
  ಪ್ರತ್ಯಾಗಮ r->lhs == acceptsymbol->content;
}

/* Whether the rule has a 'number' smaller than NRULES.  That is, it
   is useful in the grammar.  */
bool rule_useful_in_grammar_p (rule ಧ್ರುವ *r);

/* Whether the rule has a 'number' higher than NRULES.  That is, it is
   useless in the grammar.  */
bool rule_useless_in_grammar_p (rule ಧ್ರುವ *r);

/* Whether the rule is not flagged as useful but is useful in the
   grammar.  In other words, it was discarded because of conflicts.  */
bool rule_useless_in_parser_p (rule ಧ್ರುವ *r);

/* Whether the rule has a single RHS, and no user action. */
bool rule_useless_chain_p (rule ಧ್ರುವ *r);

/* Print this rule's number and lhs on OUT.  If a PREVIOUS_LHS was
   already displayed (by a previous call for another rule), avoid
   useless repetitions.  */
ಶೂನ್ಯ rule_lhs_print (rule ಧ್ರುವ *r, sym_content ಧ್ರುವ *previous_lhs,
                     FILE *out);
ಶೂನ್ಯ rule_lhs_print_xml (rule ಧ್ರುವ *r, FILE *out, ಪೂರ್ಣಾಙ್ಕ level);

/* The length of the RHS.  */
size_t rule_rhs_length (rule ಧ್ರುವ *r);

/* Print this rule's RHS on OUT.  */
ಶೂನ್ಯ rule_rhs_print (rule ಧ್ರುವ *r, FILE *out);

/* Print this rule on OUT.  If a PREVIOUS_RULE was already displayed,
   avoid useless repetitions of their LHS. */
ಶೂನ್ಯ rule_print (rule ಧ್ರುವ *r, rule ಧ್ರುವ *prev_rule, FILE *out);



/* Table of the symbols, indexed by the symbol number. */
ಬಾಹ್ಯ ಚಿಹ್ನೆ **symbols;

/* TOKEN_TRANSLATION -- a table indexed by a token number as returned
   by the user's yylex routine, it yields the internal token number
   used by the parser and throughout bison.  */
ಬಾಹ್ಯ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *token_translations;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ max_code;



/* Dump RITEM for traces. */
ಶೂನ್ಯ ritem_print (FILE *out);

/* The size of the longest rule RHS.  */
size_t ritem_longest_rhs (ಶೂನ್ಯ);

/* Print the grammar's rules that match FILTER on OUT under TITLE.  */
ಶೂನ್ಯ grammar_rules_partial_print (FILE *out, ಧ್ರುವ ವರ್ಣ *title,
                                  rule_filter filter);

/* Print the grammar's useful rules on OUT.  */
ಶೂನ್ಯ grammar_rules_print (FILE *out);
/* Print all of the grammar's rules with a "usefulness" attribute.  */
ಶೂನ್ಯ grammar_rules_print_xml (FILE *out, ಪೂರ್ಣಾಙ್ಕ level);

/* Dump the grammar. */
ಶೂನ್ಯ grammar_dump (FILE *out, ಧ್ರುವ ವರ್ಣ *title);

/* Report on STDERR the rules that are not flagged USEFUL, using the
   MESSAGE (which can be 'rule useless in grammar' when invoked after grammar
   reduction, or 'rule useless in parser due to conflicts' after conflicts
   were taken into account).  */
ಶೂನ್ಯ grammar_rules_useless_report (ಧ್ರುವ ವರ್ಣ *message);

/* Free the packed grammar. */
ಶೂನ್ಯ grammar_free (ಶೂನ್ಯ);

/* The version %required by the grammar file, as an int (100 * major +
   minor).  0 if unspecified.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ required_version;

#endif /* !GRAM_H_ */
