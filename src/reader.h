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

வகைவரை கட்டமைப்பு merger_list
{
  கட்டமைப்பு merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  குறியீடு *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

வெறுமை free_merger_functions (வெறுமை);
வெளி merger_list *merge_functions;

/* List of the start symbols.  */
வெளி குறியீடு_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
குறியீடு *switching_token (மாறிலி குறியீடு *start);

வெறுமை grammar_start_symbols_add (குறியீடு_list *syms);

வெறுமை grammar_current_rule_begin (குறியீடு *lhs, location loc,
                                 named_ref *lhs_named_ref);
வெறுமை grammar_current_rule_end (location loc);
வெறுமை grammar_midrule_action (வெறுமை);
/* Apply %empty to the current rule.  */
வெறுமை grammar_current_rule_empty_set (location loc);
வெறுமை grammar_current_rule_prec_set (குறியீடு *precsym, location loc);
வெறுமை grammar_current_rule_dprec_set (முழு dprec, location loc);
வெறுமை grammar_current_rule_merge_set (uniqstr name, location loc);
வெறுமை grammar_current_rule_expect_sr (முழு மொத்தம், location loc);
வெறுமை grammar_current_rule_expect_rr (முழு மொத்தம், location loc);
வெறுமை grammar_current_rule_குறியீடு_append (குறியீடு *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
வெறுமை grammar_current_rule_action_append (மாறிலி எழுத்து *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
வெறுமை grammar_current_rule_predicate_append (மாறிலி எழுத்து *predicate, location loc);

/* Read in the grammar specification.  */
வெறுமை reader (மாறிலி எழுத்து *gram);

/* Was %union seen?  */
வெளி bool union_seen;

/* Should rules have a default precedence?  */
வெளி bool default_prec;

#endif /* !READER_H_ */
