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

#अगर_अघोषित READER_H_
# define READER_H_

# include "location.h"
# include "symlist.h"
# include "named-ref.h"

# include "parse-gram.h"

प्रकार काष्ठा merger_list
{
  काष्ठा merger_list* अग्र;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  प्रतीक *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

व्योम free_merger_functions (व्योम);
बाह्य merger_list *merge_functions;

/* List of the start symbols.  */
बाह्य प्रतीक_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
प्रतीक *switching_token (स्थिर प्रतीक *start);

व्योम grammar_start_symbols_add (प्रतीक_list *syms);

व्योम grammar_current_rule_begin (प्रतीक *lhs, location loc,
                                 named_ref *lhs_named_ref);
व्योम grammar_current_rule_end (location loc);
व्योम grammar_midrule_action (व्योम);
/* Apply %empty to the current rule.  */
व्योम grammar_current_rule_empty_set (location loc);
व्योम grammar_current_rule_prec_set (प्रतीक *precsym, location loc);
व्योम grammar_current_rule_dprec_set (पूर्णांक dprec, location loc);
व्योम grammar_current_rule_merge_set (uniqstr name, location loc);
व्योम grammar_current_rule_expect_sr (पूर्णांक गिनती, location loc);
व्योम grammar_current_rule_expect_rr (पूर्णांक गिनती, location loc);
व्योम grammar_current_rule_प्रतीक_append (प्रतीक *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
व्योम grammar_current_rule_action_append (स्थिर अक्षर *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
व्योम grammar_current_rule_predicate_append (स्थिर अक्षर *predicate, location loc);

/* Read in the grammar specification.  */
व्योम reader (स्थिर अक्षर *gram);

/* Was %union seen?  */
बाह्य bool union_seen;

/* Should rules have a default precedence?  */
बाह्य bool default_prec;

#पूर्ण_अगर /* !READER_H_ */
