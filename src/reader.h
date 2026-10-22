/* Input parser for Bison

   Copyright (C) 2000-2003, 2005-2007, 2009-2015, 2018-2022 Free
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

#ifndef READER_H_
# define READER_H_

# include "location.h"
# include "symlist.h"
# include "named-ref.h"

# include "parse-gram.h"

തരനിർവചനം ഘടന merger_list
{
  ഘടന merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  ചിഹ്നം *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

ശൂന്യം free_merger_functions (ശൂന്യം);
ബാഹ്യ merger_list *merge_functions;

/* List of the start symbols.  */
ബാഹ്യ ചിഹ്നം_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
ചിഹ്നം *switching_token (സ്ഥിരം ചിഹ്നം *start);

ശൂന്യം grammar_start_symbols_add (ചിഹ്നം_list *syms);

ശൂന്യം grammar_current_rule_begin (ചിഹ്നം *lhs, location loc,
                                 named_ref *lhs_named_ref);
ശൂന്യം grammar_current_rule_end (location loc);
ശൂന്യം grammar_midrule_action (ശൂന്യം);
/* Apply %empty to the current rule.  */
ശൂന്യം grammar_current_rule_empty_set (location loc);
ശൂന്യം grammar_current_rule_prec_set (ചിഹ്നം *precsym, location loc);
ശൂന്യം grammar_current_rule_dprec_set (പൂർണ്ണസംഖ്യ dprec, location loc);
ശൂന്യം grammar_current_rule_merge_set (uniqstr name, location loc);
ശൂന്യം grammar_current_rule_expect_sr (പൂർണ്ണസംഖ്യ എണ്ണം, location loc);
ശൂന്യം grammar_current_rule_expect_rr (പൂർണ്ണസംഖ്യ എണ്ണം, location loc);
ശൂന്യം grammar_current_rule_ചിഹ്നം_append (ചിഹ്നം *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
ശൂന്യം grammar_current_rule_action_append (സ്ഥിരം അക്ഷരം *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
ശൂന്യം grammar_current_rule_predicate_append (സ്ഥിരം അക്ഷരം *predicate, location loc);

/* Read in the grammar specification.  */
ശൂന്യം reader (സ്ഥിരം അക്ഷരം *gram);

/* Was %union seen?  */
ബാഹ്യ bool union_seen;

/* Should rules have a default precedence?  */
ബാഹ്യ bool default_prec;

#endif /* !READER_H_ */
