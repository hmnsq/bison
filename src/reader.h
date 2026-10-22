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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ READER_H_
# define READER_H_

# include "location.h"
# include "symlist.h"
# include "named-ref.h"

# include "parse-gram.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ merger_list
{
  ರಚನೆ merger_list* ಮುಂದಿನ;
  uniqstr name;
  /* One symbol whose type is the one used by all the symbols on which
     this merging function is used.  */
  ಚಿಹ್ನೆ *sym;
  /* Where SYM was bound to this merging function.  */
  location type_declaration_loc;
} merger_list;

ಶೂನ್ಯ free_merger_functions (ಶೂನ್ಯ);
ಬಾಹ್ಯ merger_list *ವಿಲೀನ_functions;

/* List of the start symbols.  */
ಬಾಹ್ಯ ಚಿಹ್ನೆ_list *ಆರಂಭ_symbols;

/* Fetch (or create) a token "YY_PARSE_foo" for start symbol "foo".

   We don't use the simple "YY_FOO" because (i) we might get clashes
   with some of our symbols (e.g., cast => YY_CAST), and (ii) upcasing
   introduces possible clashes between terminal FOO and nonterminal
   foo.  */
ಚಿಹ್ನೆ *switching_token (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆ *ಆರಂಭ);

ಶೂನ್ಯ grammar_ಆರಂಭ_symbols_ಕೂಡಿಸು (ಚಿಹ್ನೆ_list *syms);

ಶೂನ್ಯ grammar_current_ನಿಯಮ_begin (ಚಿಹ್ನೆ *lhs, location loc,
                                 named_ref *lhs_named_ref);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_ಕೊನೆ (location loc);
ಶೂನ್ಯ grammar_midrule_action (ಶೂನ್ಯ);
/* Apply %empty to the current rule.  */
ಶೂನ್ಯ grammar_current_ನಿಯಮ_ಖಾಲಿಗಳು_ಗಣ (location loc);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_prec_ಗಣ (ಚಿಹ್ನೆ *precsym, location loc);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_dprec_ಗಣ (ಪೂರ್ಣಾಂಕ dprec, location loc);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_ವಿಲೀನ_ಗಣ (uniqstr name, location loc);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_expect_sr (ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, location loc);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_expect_rr (ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, location loc);
ಶೂನ್ಯ grammar_current_ನಿಯಮ_ಚಿಹ್ನೆ_append (ಚಿಹ್ನೆ *sym, location loc,
                                         named_ref *nref);
/* Attach an ACTION to the current rule.  */
ಶೂನ್ಯ grammar_current_ನಿಯಮ_action_append (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action, location loc,
                                         named_ref *nref, uniqstr ಟ್ಯಾಗ್);
/* Attach a PREDICATE to the current rule.  */
ಶೂನ್ಯ grammar_current_ನಿಯಮ_predicate_append (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *predicate, location loc);

/* Read in the grammar specification.  */
ಶೂನ್ಯ reader (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gram);

/* Was %union seen?  */
ಬಾಹ್ಯ bool union_ನೋಡಿದ;

/* Should rules have a default precedence?  */
ಬಾಹ್ಯ bool default_prec;

#ಆದರೆ_ಅಂತ್ಯ /* !READER_H_ */
