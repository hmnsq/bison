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

প্রকারনির্দেশ সংরচনা merger_list
{
  সংরচনা merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  প্রতীক *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

শূন্য free_merger_functions (শূন্য);
বাহ্য merger_list *merge_functions;

/* List of the start symbols.  */
বাহ্য প্রতীক_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
প্রতীক *switching_token (ধ্রুব প্রতীক *start);

শূন্য grammar_start_symbols_add (প্রতীক_list *syms);

শূন্য grammar_current_rule_begin (প্রতীক *lhs, location loc,
                                 named_ref *lhs_named_ref);
শূন্য grammar_current_rule_end (location loc);
শূন্য grammar_midrule_action (শূন্য);
/* Apply %empty to the current rule.  */
শূন্য grammar_current_rule_empty_set (location loc);
শূন্য grammar_current_rule_prec_set (প্রতীক *precsym, location loc);
শূন্য grammar_current_rule_dprec_set (পূর্ণাঙ্ক dprec, location loc);
শূন্য grammar_current_rule_merge_set (uniqstr name, location loc);
শূন্য grammar_current_rule_expect_sr (পূর্ণাঙ্ক গুনতি, location loc);
শূন্য grammar_current_rule_expect_rr (পূর্ণাঙ্ক গুনতি, location loc);
শূন্য grammar_current_rule_প্রতীক_append (প্রতীক *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
শূন্য grammar_current_rule_action_append (ধ্রুব বর্ণ *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
শূন্য grammar_current_rule_predicate_append (ধ্রুব বর্ণ *predicate, location loc);

/* Read in the grammar specification.  */
শূন্য reader (ধ্রুব বর্ণ *gram);

/* Was %union seen?  */
বাহ্য bool union_seen;

/* Should rules have a default precedence?  */
বাহ্য bool default_prec;

#endif /* !READER_H_ */
