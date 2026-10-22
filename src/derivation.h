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

प्रकारव्याख्या gl_list_t derivation_list;
प्रकारव्याख्या संरचना derivation derivation;

स्थिर ओळीत derivation_list derivation_list_new (शून्य)
{
  परत gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
}

स्थिर ओळीत bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  स्थिरांक शून्य *p = रिक्त;
  bool res = gl_list_iterator_next (it, &p, रिक्त);
  जर (res)
    *d = (derivation *) p;
  अन्यथा
    gl_list_iterator_free (it);
  परत res;
}

शून्य derivation_list_append (derivation_list dl, derivation *d);
शून्य derivation_list_prepend (derivation_list dl, derivation *d);
शून्य derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (चिन्ह_संख्या sym, derivation_list children,
                स्थिरांक rule *r);

स्थिर ओळीत derivation *derivation_new_leaf (चिन्ह_संख्या sym)
{
  परत derivation_new (sym, रिक्त, रिक्त);
}

// Number of symbols.
माप_प्रकार derivation_size (स्थिरांक derivation *deriv);
शून्य derivation_print (स्थिरांक derivation *deriv, खाता *out, स्थिरांक अक्षर *prefix);
शून्य derivation_print_leaves (स्थिरांक derivation *deriv, खाता *out);
शून्य derivation_free (derivation *deriv);
शून्य derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (शून्य);

#endif /* DERIVATION_H */
