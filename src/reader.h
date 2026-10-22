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

#جيڪڏهن_وصف_نه_ٿيل READER_H_
# define READER_H_

# include "location.h"
# include "symlist.h"
# include "named-ref.h"

# include "parse-gram.h"

تعريفنوع بنية merger_list
{
  بنية merger_list* ايندڙ;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  نشان *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

فراغ free_merger_functions (فراغ);
خارجي merger_list *ملايو_functions;

/* List of the start symbols.  */
خارجي نشان_list *شروعات_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
نشان *switching_token (ثابت نشان *شروعات);

فراغ grammar_شروعات_symbols_جوڙيو (نشان_list *syms);

فراغ grammar_current_قاعدو_begin (نشان *lhs, location loc,
                                 named_ref *lhs_named_ref);
فراغ grammar_current_قاعدو_پڇاڙي (location loc);
فراغ grammar_midrule_action (فراغ);
/* Apply %empty to the current rule.  */
فراغ grammar_current_قاعدو_خالي_جايون_سيٽ (location loc);
فراغ grammar_current_قاعدو_prec_سيٽ (نشان *precsym, location loc);
فراغ grammar_current_قاعدو_dprec_سيٽ (صحيح dprec, location loc);
فراغ grammar_current_قاعدو_ملايو_سيٽ (uniqstr name, location loc);
فراغ grammar_current_قاعدو_expect_sr (صحيح ڳڻپ, location loc);
فراغ grammar_current_قاعدو_expect_rr (صحيح ڳڻپ, location loc);
فراغ grammar_current_قاعدو_نشان_append (نشان *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
فراغ grammar_current_قاعدو_action_append (ثابت حرف *action, location loc,
                                         named_ref *nref, uniqstr ٽيگ);
/* Attach a PREDICATE to the current rule.  */
فراغ grammar_current_قاعدو_predicate_append (ثابت حرف *predicate, location loc);

/* Read in the grammar specification.  */
فراغ reader (ثابت حرف *gram);

/* Was %union seen?  */
خارجي bool union_ڏٺل;

/* Should rules have a default precedence?  */
خارجي bool default_prec;

#جيڪڏهن_پورو /* !READER_H_ */
