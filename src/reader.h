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

प्रकारनिर्देश संरचना merger_list
{
  संरचना merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  चिह्न *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

शून्य free_merger_functions (शून्य);
बाह्य merger_list *merge_functions;

/* List of the start symbols.  */
बाह्य चिह्न_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
चिह्न *switching_token (अचर चिह्न *start);

शून्य grammar_start_symbols_add (चिह्न_list *syms);

शून्य grammar_current_rule_begin (चिह्न *lhs, location loc,
                                 named_ref *lhs_named_ref);
शून्य grammar_current_rule_end (location loc);
शून्य grammar_midrule_action (शून्य);
/* Apply %empty to the current rule.  */
शून्य grammar_current_rule_empty_set (location loc);
शून्य grammar_current_rule_prec_set (चिह्न *precsym, location loc);
शून्य grammar_current_rule_dprec_set (पूर्णाङ्क dprec, location loc);
शून्य grammar_current_rule_merge_set (uniqstr name, location loc);
शून्य grammar_current_rule_expect_sr (पूर्णाङ्क गन्ती, location loc);
शून्य grammar_current_rule_expect_rr (पूर्णाङ्क गन्ती, location loc);
शून्य grammar_current_rule_चिह्न_append (चिह्न *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
शून्य grammar_current_rule_action_append (अचर वर्ण *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
शून्य grammar_current_rule_predicate_append (अचर वर्ण *predicate, location loc);

/* Read in the grammar specification.  */
शून्य reader (अचर वर्ण *gram);

/* Was %union seen?  */
बाह्य bool union_seen;

/* Should rules have a default precedence?  */
बाह्य bool default_prec;

#endif /* !READER_H_ */
