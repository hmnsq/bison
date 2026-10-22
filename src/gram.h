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

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsyms;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ntokens;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nnterms;

/* Elements of ritem. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା item_ସଂଖ୍ୟା;
# define ITEM_ସଂଖ୍ୟା_MAX INT_MAX
ବାହ୍ୟ item_ସଂଖ୍ୟା *ritem;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nritems;

/* Indices into ritem. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା item_index;

/* There is weird relationship between OT1H item_number and OTOH
   symbol_number and rule_number: we store the latter in
   item_number.  symbol_number values are stored as-is, while
   the negation of (rule_number + 1) is stored.

   Therefore, a symbol_number must be a valid item_number, and we
   sometimes have to perform the converse transformation.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ item_ସଂଖ୍ୟା
ପ୍ରତୀକ_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (ପ୍ରତୀକ_ସଂଖ୍ୟା sym)
{
  ଫେରନ୍ତୁ sym;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପ୍ରତୀକ_ସଂଖ୍ୟା
item_ସଂଖ୍ୟା_as_ପ୍ରତୀକ_ସଂଖ୍ୟା (item_ସଂଖ୍ୟା i)
{
  ଫେରନ୍ତୁ i;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
item_ସଂଖ୍ୟା_is_ପ୍ରତୀକ_ସଂଖ୍ୟା (item_ସଂଖ୍ୟା i)
{
  ଫେରନ୍ତୁ i >= 0;
}

/* Rule numbers.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା rule_ସଂଖ୍ୟା;
# define RULE_ସଂଖ୍ୟା_MAX INT_MAX

ସ୍ଥିର ଧାଡ଼ିଭିତରେ item_ସଂଖ୍ୟା
rule_ସଂଖ୍ୟା_as_item_ସଂଖ୍ୟା (rule_ସଂଖ୍ୟା r)
{
  ଫେରନ୍ତୁ -1 - r;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ rule_ସଂଖ୍ୟା
item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (item_ସଂଖ୍ୟା i)
{
  ଫେରନ୍ତୁ -1 - i;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (item_ସଂଖ୍ୟା i)
{
  ଫେରନ୍ତୁ i < 0;
}


/*--------.
| Rules.  |
`--------*/

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  /* The number of the rule in the source.  It is usually the index in
     RULES too, except if there are useless rules.  */
  rule_ସଂଖ୍ୟା code;

  /* The index in RULES.  Usually the rule number in the source,
     except if some rules are useless.  */
  rule_ସଂଖ୍ୟା ସଂଖ୍ୟା;

  sym_content *lhs;
  item_ସଂଖ୍ୟା *rhs;

  /* This symbol provides both the associativity, and the precedence. */
  sym_content *prec;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dprec;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା merger;

  /* This symbol was attached to the rule via %prec. */
  sym_content *precsym;

  /* Location of the rhs.  */
  location location;
  bool useful;
  bool is_predicate;

  /* Counts of the numbers of expected conflicts for this rule, or -1 if none
     given. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_sr_conflicts;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା expected_rr_conflicts;

  ଅଚଳ ଅକ୍ଷର *action;
  location action_loc;
} rule;

/* The used rules (size NRULES).  */
ବାହ୍ୟ rule *rules;
ବାହ୍ୟ rule_ସଂଖ୍ୟା nrules;

/* Get the rule associated to this item.  ITEM points inside RITEM.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ rule ଅଚଳ *
item_rule (item_ସଂଖ୍ୟା ଅଚଳ *item)
{
  item_ସଂଖ୍ୟା ଅଚଳ *sp = item;
  ଯେତେବେଳେ (!item_ସଂଖ୍ୟା_is_rule_ସଂଖ୍ୟା (*sp))
    ++sp;
  rule_ସଂଖ୍ୟା r = item_ସଂଖ୍ୟା_as_rule_ସଂଖ୍ୟା (*sp);
  ଫେରନ୍ତୁ &rules[r];
}

/* Pretty-print this ITEM (as in the report).  ITEM points inside
   RITEM.  PREVIOUS_RULE is used to see if the lhs is common, in which
   case LHS is factored.  Passing NULL is fine.  */
ଶୂନ୍ୟ item_print (item_ସଂଖ୍ୟା *item, rule ଅଚଳ *previous_rule,
                 FILE *out);

/*--------.
| Rules.  |
`--------*/

/* A function that selects a rule.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା bool (*rule_filter) (rule ଅଚଳ *);

/* Whether is an accepting rule (i.e., its reduction terminates
   parsing with success). */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
rule_is_initial (rule ଅଚଳ *r)
{
  /* In the case of multistart, we need to check whether the LHS is
     $accept.  In the case of "unistart", it would suffice to
     check whether this is rule number 0.  */
  ଫେରନ୍ତୁ r->lhs == acceptsymbol->content;
}

/* Whether the rule has a 'number' smaller than NRULES.  That is, it
   is useful in the grammar.  */
bool rule_useful_in_grammar_p (rule ଅଚଳ *r);

/* Whether the rule has a 'number' higher than NRULES.  That is, it is
   useless in the grammar.  */
bool rule_useless_in_grammar_p (rule ଅଚଳ *r);

/* Whether the rule is not flagged as useful but is useful in the
   grammar.  In other words, it was discarded because of conflicts.  */
bool rule_useless_in_parser_p (rule ଅଚଳ *r);

/* Whether the rule has a single RHS, and no user action. */
bool rule_useless_chain_p (rule ଅଚଳ *r);

/* Print this rule's number and lhs on OUT.  If a PREVIOUS_LHS was
   already displayed (by a previous call for another rule), avoid
   useless repetitions.  */
ଶୂନ୍ୟ rule_lhs_print (rule ଅଚଳ *r, sym_content ଅଚଳ *previous_lhs,
                     FILE *out);
ଶୂନ୍ୟ rule_lhs_print_xml (rule ଅଚଳ *r, FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level);

/* The length of the RHS.  */
size_t rule_rhs_length (rule ଅଚଳ *r);

/* Print this rule's RHS on OUT.  */
ଶୂନ୍ୟ rule_rhs_print (rule ଅଚଳ *r, FILE *out);

/* Print this rule on OUT.  If a PREVIOUS_RULE was already displayed,
   avoid useless repetitions of their LHS. */
ଶୂନ୍ୟ rule_print (rule ଅଚଳ *r, rule ଅଚଳ *prev_rule, FILE *out);



/* Table of the symbols, indexed by the symbol number. */
ବାହ୍ୟ ପ୍ରତୀକ **symbols;

/* TOKEN_TRANSLATION -- a table indexed by a token number as returned
   by the user's yylex routine, it yields the internal token number
   used by the parser and throughout bison.  */
ବାହ୍ୟ ପ୍ରତୀକ_ସଂଖ୍ୟା *token_translations;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_code;



/* Dump RITEM for traces. */
ଶୂନ୍ୟ ritem_print (FILE *out);

/* The size of the longest rule RHS.  */
size_t ritem_longest_rhs (ଶୂନ୍ୟ);

/* Print the grammar's rules that match FILTER on OUT under TITLE.  */
ଶୂନ୍ୟ grammar_rules_partial_print (FILE *out, ଅଚଳ ଅକ୍ଷର *title,
                                  rule_filter filter);

/* Print the grammar's useful rules on OUT.  */
ଶୂନ୍ୟ grammar_rules_print (FILE *out);
/* Print all of the grammar's rules with a "usefulness" attribute.  */
ଶୂନ୍ୟ grammar_rules_print_xml (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level);

/* Dump the grammar. */
ଶୂନ୍ୟ grammar_dump (FILE *out, ଅଚଳ ଅକ୍ଷର *title);

/* Report on STDERR the rules that are not flagged USEFUL, using the
   MESSAGE (which can be 'rule useless in grammar' when invoked after grammar
   reduction, or 'rule useless in parser due to conflicts' after conflicts
   were taken into account).  */
ଶୂନ୍ୟ grammar_rules_useless_report (ଅଚଳ ଅକ୍ଷର *message);

/* Free the packed grammar. */
ଶୂନ୍ୟ grammar_free (ଶୂନ୍ୟ);

/* The version %required by the grammar file, as an int (100 * major +
   minor).  0 if unspecified.  */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା required_version;

#endif /* !GRAM_H_ */
