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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DERIVATION_H
# define DERIVATION_H

# include <gl_linked_list.h>
# include <gl_xlist.h>

# include "gram.h"

/* Derivations are trees of symbols such that each nonterminal's
   children are symbols that produce that nonterminal if they are
   relevant to the counterexample.  The leaves of a derivation form a
   counterexample when printed.  */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ gl_list_t derivation_list;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ derivation derivation;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ derivation_list derivation_list_new (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ gl_list_create_ಖಾಲಿಗಳು (GL_LINKED_LIST, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, true);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
derivation_list_ಮುಂದಿನ (gl_list_iterator_t *it, derivation **d)
{
  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p = ಖಾಲಿ;
  bool res = gl_list_iterator_ಮುಂದಿನ (it, &p, ಖಾಲಿ);
  ಆದರೆ (res)
    *d = (derivation *) p;
  ಇಲ್ಲದಿದ್ದರೆ
    gl_list_iterator_free (it);
  ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ derivation_list_append (derivation_list dl, derivation *d);
ಶೂನ್ಯ derivation_list_prepend (derivation_list dl, derivation *d);
ಶೂನ್ಯ derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym, derivation_list children,
                ಸ್ಥಿರಾಂಕ ನಿಯಮ *r);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ derivation *derivation_new_ಎಲೆ (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ sym)
{
  ಹಿಂತಿರುಗಿ derivation_new (sym, ಖಾಲಿ, ಖಾಲಿ);
}

// Number of symbols.
size_t derivation_ಗಾತ್ರ (ಸ್ಥಿರಾಂಕ derivation *deriv);
ಶೂನ್ಯ derivation_print (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ);
ಶೂನ್ಯ derivation_print_leaves (ಸ್ಥಿರಾಂಕ derivation *deriv, FILE *ಹೊರಹೋಗು);
ಶೂನ್ಯ derivation_free (derivation *deriv);
ಶೂನ್ಯ derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (ಶೂನ್ಯ);

#ಆದರೆ_ಅಂತ್ಯ /* DERIVATION_H */
