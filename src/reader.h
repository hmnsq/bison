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

تعريفنوع بنية merger_list
{
  بنية merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  علام *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

فراغ free_merger_functions (فراغ);
خارجي merger_list *merge_functions;

/* List of the start symbols.  */
خارجي علام_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
علام *switching_token (ثابت علام *start);

فراغ grammar_start_symbols_add (علام_list *syms);

فراغ grammar_current_rule_begin (علام *lhs, location loc,
                                 named_ref *lhs_named_ref);
فراغ grammar_current_rule_end (location loc);
فراغ grammar_midrule_action (فراغ);
/* Apply %empty to the current rule.  */
فراغ grammar_current_rule_empty_set (location loc);
فراغ grammar_current_rule_prec_set (علام *precsym, location loc);
فراغ grammar_current_rule_dprec_set (صحيح dprec, location loc);
فراغ grammar_current_rule_merge_set (uniqstr name, location loc);
فراغ grammar_current_rule_expect_sr (صحيح كدي, location loc);
فراغ grammar_current_rule_expect_rr (صحيح كدي, location loc);
فراغ grammar_current_rule_علام_append (علام *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
فراغ grammar_current_rule_action_append (ثابت حرف *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
فراغ grammar_current_rule_predicate_append (ثابت حرف *predicate, location loc);

/* Read in the grammar specification.  */
فراغ reader (ثابت حرف *gram);

/* Was %union seen?  */
خارجي bool union_seen;

/* Should rules have a default precedence?  */
خارجي bool default_prec;

#endif /* !READER_H_ */
