/* Counterexample derivation trees

   Copyright (C) 2020-2022 Free Software Foundation, Inc.

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

#ifndef DERIVATION_H
# define DERIVATION_H

# include <gl_linked_list.h>
# include <gl_xlist.h>

# include "gram.h"

/* Derivations are trees of symbols such that each nonterminal's
   children are symbols that produce that nonterminal if they are
   relevant to the counterexample.  The leaves of a derivation form a
   counterexample when printed.  */

ಪ್ರಕಾರನಿರ್ದೇಶ gl_list_t derivation_list;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ derivation derivation;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ derivation_list derivation_list_new (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ gl_list_create_empty (GL_LINKED_LIST, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, true);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  ಧ್ರುವ ಶೂನ್ಯ *p = ರಿಕ್ತ;
  bool res = gl_list_iterator_next (it, &p, ರಿಕ್ತ);
  ಯದಿ (res)
    *d = (derivation *) p;
  ಅನ್ಯಥಾ
    gl_list_iterator_free (it);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ derivation_list_append (derivation_list dl, derivation *d);
ಶೂನ್ಯ derivation_list_prepend (derivation_list dl, derivation *d);
ಶೂನ್ಯ derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, derivation_list children,
                ಧ್ರುವ rule *r);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ derivation *derivation_new_leaf (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  ಪ್ರತ್ಯಾಗಮ derivation_new (sym, ರಿಕ್ತ, ರಿಕ್ತ);
}

// Number of symbols.
size_t derivation_size (ಧ್ರುವ derivation *deriv);
ಶೂನ್ಯ derivation_print (ಧ್ರುವ derivation *deriv, FILE *out, ಧ್ರುವ ವರ್ಣ *prefix);
ಶೂನ್ಯ derivation_print_leaves (ಧ್ರುವ derivation *deriv, FILE *out);
ಶೂನ್ಯ derivation_free (derivation *deriv);
ಶೂನ್ಯ derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (ಶೂನ್ಯ);

#endif /* DERIVATION_H */
