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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ merger_list
{
  ಸಂರಚನಾ merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  ಚಿಹ್ನೆ *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

ಶೂನ್ಯ free_merger_functions (ಶೂನ್ಯ);
ಬಾಹ್ಯ merger_list *merge_functions;

/* List of the start symbols.  */
ಬಾಹ್ಯ ಚಿಹ್ನೆ_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
ಚಿಹ್ನೆ *switching_token (ಧ್ರುವ ಚಿಹ್ನೆ *start);

ಶೂನ್ಯ grammar_start_symbols_add (ಚಿಹ್ನೆ_list *syms);

ಶೂನ್ಯ grammar_current_rule_begin (ಚಿಹ್ನೆ *lhs, location loc,
                                 named_ref *lhs_named_ref);
ಶೂನ್ಯ grammar_current_rule_end (location loc);
ಶೂನ್ಯ grammar_midrule_action (ಶೂನ್ಯ);
/* Apply %empty to the current rule.  */
ಶೂನ್ಯ grammar_current_rule_empty_set (location loc);
ಶೂನ್ಯ grammar_current_rule_prec_set (ಚಿಹ್ನೆ *precsym, location loc);
ಶೂನ್ಯ grammar_current_rule_dprec_set (ಪೂರ್ಣಾಙ್ಕ dprec, location loc);
ಶೂನ್ಯ grammar_current_rule_merge_set (uniqstr name, location loc);
ಶೂನ್ಯ grammar_current_rule_expect_sr (ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, location loc);
ಶೂನ್ಯ grammar_current_rule_expect_rr (ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, location loc);
ಶೂನ್ಯ grammar_current_rule_ಚಿಹ್ನೆ_append (ಚಿಹ್ನೆ *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
ಶೂನ್ಯ grammar_current_rule_action_append (ಧ್ರುವ ವರ್ಣ *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
ಶೂನ್ಯ grammar_current_rule_predicate_append (ಧ್ರುವ ವರ್ಣ *predicate, location loc);

/* Read in the grammar specification.  */
ಶೂನ್ಯ reader (ಧ್ರುವ ವರ್ಣ *gram);

/* Was %union seen?  */
ಬಾಹ್ಯ bool union_seen;

/* Should rules have a default precedence?  */
ಬಾಹ್ಯ bool default_prec;

#endif /* !READER_H_ */
