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

પ્રકારવ્યાખ્યા સંરચના merger_list
{
  સંરચના merger_list* next;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  પ્રતીક *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

શૂન્ય free_merger_functions (શૂન્ય);
બાહ્ય merger_list *merge_functions;

/* List of the start symbols.  */
બાહ્ય પ્રતીક_list *start_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
પ્રતીક *switching_token (અચલ પ્રતીક *start);

શૂન્ય grammar_start_symbols_add (પ્રતીક_list *syms);

શૂન્ય grammar_current_rule_begin (પ્રતીક *lhs, location loc,
                                 named_ref *lhs_named_ref);
શૂન્ય grammar_current_rule_end (location loc);
શૂન્ય grammar_midrule_action (શૂન્ય);
/* Apply %empty to the current rule.  */
શૂન્ય grammar_current_rule_empty_set (location loc);
શૂન્ય grammar_current_rule_prec_set (પ્રતીક *precsym, location loc);
શૂન્ય grammar_current_rule_dprec_set (પૂર્ણાંક dprec, location loc);
શૂન્ય grammar_current_rule_merge_set (uniqstr name, location loc);
શૂન્ય grammar_current_rule_expect_sr (પૂર્ણાંક ગણતરી, location loc);
શૂન્ય grammar_current_rule_expect_rr (પૂર્ણાંક ગણતરી, location loc);
શૂન્ય grammar_current_rule_પ્રતીક_append (પ્રતીક *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
શૂન્ય grammar_current_rule_action_append (અચલ અક્ષર *action, location loc,
                                         named_ref *nref, uniqstr tag);
/* Attach a PREDICATE to the current rule.  */
શૂન્ય grammar_current_rule_predicate_append (અચલ અક્ષર *predicate, location loc);

/* Read in the grammar specification.  */
શૂન્ય reader (અચલ અક્ષર *gram);

/* Was %union seen?  */
બાહ્ય bool union_seen;

/* Should rules have a default precedence?  */
બાહ્ય bool default_prec;

#endif /* !READER_H_ */
