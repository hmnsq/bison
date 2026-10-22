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

#که_نه_تعریف_شوی GRAM_H_
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
تعريفنوع صحيح توکی_شمېره;
# define توکی_شمېره_MAX لوی_عدد
خارجي توکی_شمېره *ritem;
خارجي صحيح nritems;

/* Indices into ritem. */
تعريفنوع بینښه صحيح توکی_شاخص;

/* There is weird relationship between OT1H item_number and OTOH
   symbol_number and rule_number: we store the latter in
   item_number.  symbol_number values are stored as-is, while
   the negation of (rule_number + 1) is stored.

   Therefore, a symbol_number must be a valid item_number, and we
   sometimes have to perform the converse transformation.  */

ساكن مضمن توکی_شمېره
نښه_شمېره_as_توکی_شمېره (نښه_شمېره sym)
{
  أرجع sym;
}

ساكن مضمن نښه_شمېره
توکی_شمېره_as_نښه_شمېره (توکی_شمېره i)
{
  أرجع i;
}

ساكن مضمن bool
توکی_شمېره_is_نښه_شمېره (توکی_شمېره i)
{
  أرجع i >= 0;
}

/* Rule numbers.  */
تعريفنوع صحيح قاعده_شمېره;
# define قاعده_شمېره_MAX لوی_عدد

ساكن مضمن توکی_شمېره
قاعده_شمېره_as_توکی_شمېره (قاعده_شمېره r)
{
  أرجع -1 - r;
}

ساكن مضمن قاعده_شمېره
توکی_شمېره_as_قاعده_شمېره (توکی_شمېره i)
{
  أرجع -1 - i;
}

ساكن مضمن bool
توکی_شمېره_is_قاعده_شمېره (توکی_شمېره i)
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
  قاعده_شمېره کوډ;

  /* The index in RULES.  Usually the rule number in the source,
     except if some rules are useless.  */
  قاعده_شمېره شمېره;

  sym_content *lhs;
  توکی_شمېره *rhs;

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
} قاعده;

/* The used rules (size NRULES).  */
خارجي قاعده *قواعد;
خارجي قاعده_شمېره nrules;

/* Get the rule associated to this item.  ITEM points inside RITEM.  */
ساكن مضمن قاعده ثابت *
توکی_قاعده (توکی_شمېره ثابت *توکی)
{
  توکی_شمېره ثابت *sp = توکی;
  طالما (!توکی_شمېره_is_قاعده_شمېره (*sp))
    ++sp;
  قاعده_شمېره r = توکی_شمېره_as_قاعده_شمېره (*sp);
  أرجع &قواعد[r];
}

/* Pretty-print this ITEM (as in the report).  ITEM points inside
   RITEM.  PREVIOUS_RULE is used to see if the lhs is common, in which
   case LHS is factored.  Passing NULL is fine.  */
فراغ توکی_print (توکی_شمېره *توکی, قاعده ثابت *پخوانی_قاعده,
                 FILE *بهر);

/*--------.
| Rules.  |
`--------*/

/* A function that selects a rule.  */
تعريفنوع bool (*قاعده_filter) (قاعده ثابت *);

/* Whether is an accepting rule (i.e., its reduction terminates
   parsing with success). */
ساكن مضمن bool
قاعده_is_initial (قاعده ثابت *r)
{
  /* In the case of multistart, we need to check whether the LHS is
     $accept.  In the case of "unistart", it would suffice to
     check whether this is rule number 0.  */
  أرجع r->lhs == acceptsymbol->content;
}

/* Whether the rule has a 'number' smaller than NRULES.  That is, it
   is useful in the grammar.  */
bool قاعده_useful_دننه_grammar_p (قاعده ثابت *r);

/* Whether the rule has a 'number' higher than NRULES.  That is, it is
   useless in the grammar.  */
bool قاعده_useless_دننه_grammar_p (قاعده ثابت *r);

/* Whether the rule is not flagged as useful but is useful in the
   grammar.  In other words, it was discarded because of conflicts.  */
bool قاعده_useless_دننه_parser_p (قاعده ثابت *r);

/* Whether the rule has a single RHS, and no user action. */
bool قاعده_useless_chain_p (قاعده ثابت *r);

/* Print this rule's number and lhs on OUT.  If a PREVIOUS_LHS was
   already displayed (by a previous call for another rule), avoid
   useless repetitions.  */
فراغ قاعده_lhs_print (قاعده ثابت *r, sym_content ثابت *پخوانی_lhs,
                     FILE *بهر);
فراغ قاعده_lhs_print_xml (قاعده ثابت *r, FILE *بهر, صحيح level);

/* The length of the RHS.  */
size_t قاعده_rhs_اوږدتیا (قاعده ثابت *r);

/* Print this rule's RHS on OUT.  */
فراغ قاعده_rhs_print (قاعده ثابت *r, FILE *بهر);

/* Print this rule on OUT.  If a PREVIOUS_RULE was already displayed,
   avoid useless repetitions of their LHS. */
فراغ قاعده_print (قاعده ثابت *r, قاعده ثابت *prev_قاعده, FILE *بهر);



/* Table of the symbols, indexed by the symbol number. */
خارجي نښه **symbols;

/* TOKEN_TRANSLATION -- a table indexed by a token number as returned
   by the user's yylex routine, it yields the internal token number
   used by the parser and throughout bison.  */
خارجي نښه_شمېره *token_translations;
خارجي صحيح max_کوډ;



/* Dump RITEM for traces. */
فراغ ritem_print (FILE *بهر);

/* The size of the longest rule RHS.  */
size_t ritem_longest_rhs (فراغ);

/* Print the grammar's rules that match FILTER on OUT under TITLE.  */
فراغ grammar_قواعد_partial_print (FILE *بهر, ثابت حرف *title,
                                  قاعده_filter filter);

/* Print the grammar's useful rules on OUT.  */
فراغ grammar_قواعد_print (FILE *بهر);
/* Print all of the grammar's rules with a "usefulness" attribute.  */
فراغ grammar_قواعد_print_xml (FILE *بهر, صحيح level);

/* Dump the grammar. */
فراغ grammar_dump (FILE *بهر, ثابت حرف *title);

/* Report on STDERR the rules that are not flagged USEFUL, using the
   MESSAGE (which can be 'rule useless in grammar' when invoked after grammar
   reduction, or 'rule useless in parser due to conflicts' after conflicts
   were taken into account).  */
فراغ grammar_قواعد_useless_report (ثابت حرف *پیغام);

/* Free the packed grammar. */
فراغ grammar_free (فراغ);

/* The version %required by the grammar file, as an int (100 * major +
   minor).  0 if unspecified.  */
خارجي صحيح required_نسخه;

#د_که_پای /* !GRAM_H_ */
