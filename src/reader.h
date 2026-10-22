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

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା merger_list
{
  ସଂରଚନା merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  ପ୍ରତୀକ *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

ଶୂନ୍ୟ free_merger_functions (ଶୂନ୍ୟ);
ବାହ୍ୟ merger_list *merge_functions;

/* List of the start symbols.  */
ବାହ୍ୟ ପ୍ରତୀକ_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
ପ୍ରତୀକ *switching_token (ଅଚଳ ପ୍ରତୀକ *start);

ଶୂନ୍ୟ grammar_start_symbols_add (ପ୍ରତୀକ_list *syms);

ଶୂନ୍ୟ grammar_current_rule_begin (ପ୍ରତୀକ *lhs, location loc,
                                 named_ref *lhs_named_ref);
ଶୂନ୍ୟ grammar_current_rule_end (location loc);
ଶୂନ୍ୟ grammar_midrule_action (ଶୂନ୍ୟ);
/* Apply %empty to the current rule.  */
ଶୂନ୍ୟ grammar_current_rule_empty_set (location loc);
ଶୂନ୍ୟ grammar_current_rule_prec_set (ପ୍ରତୀକ *precsym, location loc);
ଶୂନ୍ୟ grammar_current_rule_dprec_set (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dprec, location loc);
ଶୂନ୍ୟ grammar_current_rule_merge_set (uniqstr name, location loc);
ଶୂନ୍ୟ grammar_current_rule_expect_sr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, location loc);
ଶୂନ୍ୟ grammar_current_rule_expect_rr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, location loc);
ଶୂନ୍ୟ grammar_current_rule_ପ୍ରତୀକ_append (ପ୍ରତୀକ *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
ଶୂନ୍ୟ grammar_current_rule_action_append (ଅଚଳ ଅକ୍ଷର *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
ଶୂନ୍ୟ grammar_current_rule_predicate_append (ଅଚଳ ଅକ୍ଷର *predicate, location loc);

/* Read in the grammar specification.  */
ଶୂନ୍ୟ reader (ଅଚଳ ଅକ୍ଷର *gram);

/* Was %union seen?  */
ବାହ୍ୟ bool union_seen;

/* Should rules have a default precedence?  */
ବାହ୍ୟ bool default_prec;

#endif /* !READER_H_ */
