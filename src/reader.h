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

#यदि_अपरिभाषितम् READER_H_
# define READER_H_

# include "location.h"
# include "symlist.h"
# include "named-ref.h"

# include "parse-gram.h"

प्रकारनिर्देश संरचना merger_list
{
  संरचना merger_list* अग्र;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  चिह्नम् *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

शून्य free_merger_functions (शून्य);
बाह्य merger_list *merge_functions;

/* List of the start symbols.  */
बाह्य चिह्नम्_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
चिह्नम् *switching_token (ध्रुव चिह्नम् *start);

शून्य grammar_start_symbols_सङ्कलय (चिह्नम्_list *syms);

शून्य grammar_current_नियमः_begin (चिह्नम् *lhs, location loc,
                                 named_ref *lhs_named_ref);
शून्य grammar_current_नियमः_अन्तः (location loc);
शून्य grammar_midrule_action (शून्य);
/* Apply %empty to the current rule.  */
शून्य grammar_current_नियमः_रिक्तानि_समुच्चयः (location loc);
शून्य grammar_current_नियमः_prec_समुच्चयः (चिह्नम् *precsym, location loc);
शून्य grammar_current_नियमः_dprec_समुच्चयः (पूर्णाङ्क dprec, location loc);
शून्य grammar_current_नियमः_merge_समुच्चयः (uniqstr name, location loc);
शून्य grammar_current_नियमः_expect_sr (पूर्णाङ्क गणनम्, location loc);
शून्य grammar_current_नियमः_expect_rr (पूर्णाङ्क गणनम्, location loc);
शून्य grammar_current_नियमः_चिह्नम्_append (चिह्नम् *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
शून्य grammar_current_नियमः_action_append (ध्रुव वर्ण *action, location loc,
                                         named_ref *nref, uniqstr चिह्नकम्);
/* Attach a PREDICATE to the current rule.  */
शून्य grammar_current_नियमः_predicate_append (ध्रुव वर्ण *predicate, location loc);

/* Read in the grammar specification.  */
शून्य reader (ध्रुव वर्ण *gram);

/* Was %union seen?  */
बाह्य bool union_आलोकितम्;

/* Should rules have a default precedence?  */
बाह्य bool default_prec;

#यदि_अन्तः /* !READER_H_ */
