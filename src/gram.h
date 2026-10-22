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

#جے_بنا_تعریف GRAM_H_
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

خارجي صحيح nsyms;
خارجي صحيح ntokens;
خارجي صحيح nnterms;

/* Elements of ritem. */
تعريفنوع صحيح شے_نمبر;
# define شے_نمبر_MAX ودھ_توں_ودھ_عدد
خارجي شے_نمبر *ritem;
خارجي صحيح nritems;

/* Indices into ritem. */
تعريفنوع بنانشانی صحيح شے_اشاریہ;

/* There is weird relationship between OT1H item_number and OTOH
   symbol_number and rule_number: we store the latter in
   item_number.  symbol_number values are stored as-is, while
   the negation of (rule_number + 1) is stored.

   Therefore, a symbol_number must be a valid item_number, and we
   sometimes have to perform the converse transformation.  */

ساكن مضمن شے_نمبر
علامت_نمبر_as_شے_نمبر (علامت_نمبر sym)
{
  أرجع sym;
}

ساكن مضمن علامت_نمبر
شے_نمبر_as_علامت_نمبر (شے_نمبر i)
{
  أرجع i;
}

ساكن مضمن bool
شے_نمبر_is_علامت_نمبر (شے_نمبر i)
{
  أرجع i >= 0;
}

/* Rule numbers.  */
تعريفنوع صحيح قاعدہ_نمبر;
# define قاعدہ_نمبر_MAX ودھ_توں_ودھ_عدد

ساكن مضمن شے_نمبر
قاعدہ_نمبر_as_شے_نمبر (قاعدہ_نمبر r)
{
  أرجع -1 - r;
}

ساكن مضمن قاعدہ_نمبر
شے_نمبر_as_قاعدہ_نمبر (شے_نمبر i)
{
  أرجع -1 - i;
}

ساكن مضمن bool
شے_نمبر_is_قاعدہ_نمبر (شے_نمبر i)
{
  أرجع i < 0;
}


/*--------.
| Rules.  |
`--------*/

تعريفنوع بنية
{
  /* The number of the rule in the source.  It is usually the index in
     RULES too, except if there are useless rules.  */
  قاعدہ_نمبر کوڈ;

  /* The index in RULES.  Usually the rule number in the source,
     except if some rules are useless.  */
  قاعدہ_نمبر نمبر;

  sym_content *lhs;
  شے_نمبر *rhs;

  /* This symbol provides both the associativity, and the precedence. */
  sym_content *prec;

  صحيح dprec;
  صحيح merger;

  /* This symbol was attached to the rule via %prec. */
  sym_content *precsym;

  /* Location of the rhs.  */
  location location;
  bool useful;
  bool is_predicate;

  /* Counts of the numbers of expected conflicts for this rule, or -1 if none
     given. */
  صحيح expected_sr_conflicts;
  صحيح expected_rr_conflicts;

  ثابت حرف *action;
  location action_loc;
} قاعدہ;

/* The used rules (size NRULES).  */
خارجي قاعدہ *قاعدے;
خارجي قاعدہ_نمبر nrules;

/* Get the rule associated to this item.  ITEM points inside RITEM.  */
ساكن مضمن قاعدہ ثابت *
شے_قاعدہ (شے_نمبر ثابت *شے)
{
  شے_نمبر ثابت *sp = شے;
  طالما (!شے_نمبر_is_قاعدہ_نمبر (*sp))
    ++sp;
  قاعدہ_نمبر r = شے_نمبر_as_قاعدہ_نمبر (*sp);
  أرجع &قاعدے[r];
}

/* Pretty-print this ITEM (as in the report).  ITEM points inside
   RITEM.  PREVIOUS_RULE is used to see if the lhs is common, in which
   case LHS is factored.  Passing NULL is fine.  */
فراغ شے_print (شے_نمبر *شے, قاعدہ ثابت *پچھلا_قاعدہ,
                 FILE *باہرلا);

/*--------.
| Rules.  |
`--------*/

/* A function that selects a rule.  */
تعريفنوع bool (*قاعدہ_filter) (قاعدہ ثابت *);

/* Whether is an accepting rule (i.e., its reduction terminates
   parsing with success). */
ساكن مضمن bool
قاعدہ_is_initial (قاعدہ ثابت *r)
{
  /* In the case of multistart, we need to check whether the LHS is
     $accept.  In the case of "unistart", it would suffice to
     check whether this is rule number 0.  */
  أرجع r->lhs == acceptsymbol->content;
}

/* Whether the rule has a 'number' smaller than NRULES.  That is, it
   is useful in the grammar.  */
bool قاعدہ_useful_وچ_grammar_p (قاعدہ ثابت *r);

/* Whether the rule has a 'number' higher than NRULES.  That is, it is
   useless in the grammar.  */
bool قاعدہ_useless_وچ_grammar_p (قاعدہ ثابت *r);

/* Whether the rule is not flagged as useful but is useful in the
   grammar.  In other words, it was discarded because of conflicts.  */
bool قاعدہ_useless_وچ_parser_p (قاعدہ ثابت *r);

/* Whether the rule has a single RHS, and no user action. */
bool قاعدہ_useless_chain_p (قاعدہ ثابت *r);

/* Print this rule's number and lhs on OUT.  If a PREVIOUS_LHS was
   already displayed (by a previous call for another rule), avoid
   useless repetitions.  */
فراغ قاعدہ_lhs_print (قاعدہ ثابت *r, sym_content ثابت *پچھلا_lhs,
                     FILE *باہرلا);
فراغ قاعدہ_lhs_print_xml (قاعدہ ثابت *r, FILE *باہرلا, صحيح level);

/* The length of the RHS.  */
size_t قاعدہ_rhs_لمائی (قاعدہ ثابت *r);

/* Print this rule's RHS on OUT.  */
فراغ قاعدہ_rhs_print (قاعدہ ثابت *r, FILE *باہرلا);

/* Print this rule on OUT.  If a PREVIOUS_RULE was already displayed,
   avoid useless repetitions of their LHS. */
فراغ قاعدہ_print (قاعدہ ثابت *r, قاعدہ ثابت *prev_قاعدہ, FILE *باہرلا);



/* Table of the symbols, indexed by the symbol number. */
خارجي علامت **symbols;

/* TOKEN_TRANSLATION -- a table indexed by a token number as returned
   by the user's yylex routine, it yields the internal token number
   used by the parser and throughout bison.  */
خارجي علامت_نمبر *token_translations;
خارجي صحيح max_کوڈ;



/* Dump RITEM for traces. */
فراغ ritem_print (FILE *باہرلا);

/* The size of the longest rule RHS.  */
size_t ritem_longest_rhs (فراغ);

/* Print the grammar's rules that match FILTER on OUT under TITLE.  */
فراغ grammar_قاعدے_partial_print (FILE *باہرلا, ثابت حرف *title,
                                  قاعدہ_filter filter);

/* Print the grammar's useful rules on OUT.  */
فراغ grammar_قاعدے_print (FILE *باہرلا);
/* Print all of the grammar's rules with a "usefulness" attribute.  */
فراغ grammar_قاعدے_print_xml (FILE *باہرلا, صحيح level);

/* Dump the grammar. */
فراغ grammar_dump (FILE *باہرلا, ثابت حرف *title);

/* Report on STDERR the rules that are not flagged USEFUL, using the
   MESSAGE (which can be 'rule useless in grammar' when invoked after grammar
   reduction, or 'rule useless in parser due to conflicts' after conflicts
   were taken into account).  */
فراغ grammar_قاعدے_useless_report (ثابت حرف *سنیہا);

/* Free the packed grammar. */
فراغ grammar_free (فراغ);

/* The version %required by the grammar file, as an int (100 * major +
   minor).  0 if unspecified.  */
خارجي صحيح required_نسخہ;

#جے_انت /* !GRAM_H_ */
