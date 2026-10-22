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

#अगर_अघोषित GRAM_H_
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

बाह्य पूर्णांक nsyms;
बाह्य पूर्णांक ntokens;
बाह्य पूर्णांक nnterms;

/* Elements of ritem. */
प्रकार पूर्णांक item_संख्या;
# define ITEM_संख्या_MAX पूर्णांक_उच्च
बाह्य item_संख्या *ritem;
बाह्य पूर्णांक nritems;

/* Indices into ritem. */
प्रकार अचिन्हित पूर्णांक item_सूचकांक;

/* There is weird relationship between OT1H item_number and OTOH
   symbol_number and rule_number: we store the latter in
   item_number.  symbol_number values are stored as-is, while
   the negation of (rule_number + 1) is stored.

   Therefore, a symbol_number must be a valid item_number, and we
   sometimes have to perform the converse transformation.  */

अटल अंतरभूत item_संख्या
प्रतीक_संख्या_as_item_संख्या (प्रतीक_संख्या sym)
{
  वापस sym;
}

अटल अंतरभूत प्रतीक_संख्या
item_संख्या_as_प्रतीक_संख्या (item_संख्या i)
{
  वापस i;
}

अटल अंतरभूत bool
item_संख्या_is_प्रतीक_संख्या (item_संख्या i)
{
  वापस i >= 0;
}

/* Rule numbers.  */
प्रकार पूर्णांक rule_संख्या;
# define RULE_संख्या_MAX पूर्णांक_उच्च

अटल अंतरभूत item_संख्या
rule_संख्या_as_item_संख्या (rule_संख्या r)
{
  वापस -1 - r;
}

अटल अंतरभूत rule_संख्या
item_संख्या_as_rule_संख्या (item_संख्या i)
{
  वापस -1 - i;
}

अटल अंतरभूत bool
item_संख्या_is_rule_संख्या (item_संख्या i)
{
  वापस i < 0;
}


/*--------.
| Rules.  |
`--------*/

प्रकार काष्ठा
{
  /* The number of the rule in the source.  It is usually the index in
     RULES too, except if there are useless rules.  */
  rule_संख्या code;

  /* The index in RULES.  Usually the rule number in the source,
     except if some rules are useless.  */
  rule_संख्या संख्या;

  sym_content *lhs;
  item_संख्या *rhs;

  /* This symbol provides both the associativity, and the precedence. */
  sym_content *prec;

  पूर्णांक dprec;
  पूर्णांक merger;

  /* This symbol was attached to the rule via %prec. */
  sym_content *precsym;

  /* Location of the rhs.  */
  location location;
  bool useful;
  bool is_predicate;

  /* Counts of the numbers of expected conflicts for this rule, or -1 if none
     given. */
  पूर्णांक expected_sr_conflicts;
  पूर्णांक expected_rr_conflicts;

  स्थिर अक्षर *action;
  location action_loc;
} rule;

/* The used rules (size NRULES).  */
बाह्य rule *rules;
बाह्य rule_संख्या nrules;

/* Get the rule associated to this item.  ITEM points inside RITEM.  */
अटल अंतरभूत rule स्थिर *
item_rule (item_संख्या स्थिर *item)
{
  item_संख्या स्थिर *sp = item;
  जबतक (!item_संख्या_is_rule_संख्या (*sp))
    ++sp;
  rule_संख्या r = item_संख्या_as_rule_संख्या (*sp);
  वापस &rules[r];
}

/* Pretty-print this ITEM (as in the report).  ITEM points inside
   RITEM.  PREVIOUS_RULE is used to see if the lhs is common, in which
   case LHS is factored.  Passing NULL is fine.  */
व्योम item_print (item_संख्या *item, rule स्थिर *previous_rule,
                 खाता *out);

/*--------.
| Rules.  |
`--------*/

/* A function that selects a rule.  */
प्रकार bool (*rule_filter) (rule स्थिर *);

/* Whether is an accepting rule (i.e., its reduction terminates
   parsing with success). */
अटल अंतरभूत bool
rule_is_initial (rule स्थिर *r)
{
  /* In the case of multistart, we need to check whether the LHS is
     $accept.  In the case of "unistart", it would suffice to
     check whether this is rule number 0.  */
  वापस r->lhs == acceptsymbol->content;
}

/* Whether the rule has a 'number' smaller than NRULES.  That is, it
   is useful in the grammar.  */
bool rule_useful_in_grammar_p (rule स्थिर *r);

/* Whether the rule has a 'number' higher than NRULES.  That is, it is
   useless in the grammar.  */
bool rule_useless_in_grammar_p (rule स्थिर *r);

/* Whether the rule is not flagged as useful but is useful in the
   grammar.  In other words, it was discarded because of conflicts.  */
bool rule_useless_in_parser_p (rule स्थिर *r);

/* Whether the rule has a single RHS, and no user action. */
bool rule_useless_chain_p (rule स्थिर *r);

/* Print this rule's number and lhs on OUT.  If a PREVIOUS_LHS was
   already displayed (by a previous call for another rule), avoid
   useless repetitions.  */
व्योम rule_lhs_print (rule स्थिर *r, sym_content स्थिर *previous_lhs,
                     खाता *out);
व्योम rule_lhs_print_xml (rule स्थिर *r, खाता *out, पूर्णांक level);

/* The length of the RHS.  */
माप_प्रकार rule_rhs_लंबाई (rule स्थिर *r);

/* Print this rule's RHS on OUT.  */
व्योम rule_rhs_print (rule स्थिर *r, खाता *out);

/* Print this rule on OUT.  If a PREVIOUS_RULE was already displayed,
   avoid useless repetitions of their LHS. */
व्योम rule_print (rule स्थिर *r, rule स्थिर *पूर्व_rule, खाता *out);



/* Table of the symbols, indexed by the symbol number. */
बाह्य प्रतीक **symbols;

/* TOKEN_TRANSLATION -- a table indexed by a token number as returned
   by the user's yylex routine, it yields the internal token number
   used by the parser and throughout bison.  */
बाह्य प्रतीक_संख्या *token_translations;
बाह्य पूर्णांक max_code;



/* Dump RITEM for traces. */
व्योम ritem_print (खाता *out);

/* The size of the longest rule RHS.  */
माप_प्रकार ritem_longest_rhs (व्योम);

/* Print the grammar's rules that match FILTER on OUT under TITLE.  */
व्योम grammar_rules_partial_print (खाता *out, स्थिर अक्षर *title,
                                  rule_filter filter);

/* Print the grammar's useful rules on OUT.  */
व्योम grammar_rules_print (खाता *out);
/* Print all of the grammar's rules with a "usefulness" attribute.  */
व्योम grammar_rules_print_xml (खाता *out, पूर्णांक level);

/* Dump the grammar. */
व्योम grammar_dump (खाता *out, स्थिर अक्षर *title);

/* Report on STDERR the rules that are not flagged USEFUL, using the
   MESSAGE (which can be 'rule useless in grammar' when invoked after grammar
   reduction, or 'rule useless in parser due to conflicts' after conflicts
   were taken into account).  */
व्योम grammar_rules_useless_report (स्थिर अक्षर *message);

/* Free the packed grammar. */
व्योम grammar_free (व्योम);

/* The version %required by the grammar file, as an int (100 * major +
   minor).  0 if unspecified.  */
बाह्य पूर्णांक required_version;

#पूर्ण_अगर /* !GRAM_H_ */
