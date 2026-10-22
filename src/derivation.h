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

प्रकारनिर्देश gl_list_t derivation_list;
प्रकारनिर्देश संरचना derivation derivation;

स्थिर अन्तःस्थ derivation_list derivation_list_new (शून्य)
{
  प्रत्यागम gl_list_create_empty (GL_LINKED_LIST, रिक्त, रिक्त, रिक्त, true);
}

स्थिर अन्तःस्थ bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  ध्रुव शून्य *p = रिक्त;
  bool res = gl_list_iterator_next (it, &p, रिक्त);
  जइ (res)
    *d = (derivation *) p;
  अण्णहा
    gl_list_iterator_free (it);
  प्रत्यागम res;
}

शून्य derivation_list_append (derivation_list dl, derivation *d);
शून्य derivation_list_prepend (derivation_list dl, derivation *d);
शून्य derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (चिह्नम्_सङ्ख्या sym, derivation_list children,
                ध्रुव rule *r);

स्थिर अन्तःस्थ derivation *derivation_new_leaf (चिह्नम्_सङ्ख्या sym)
{
  प्रत्यागम derivation_new (sym, रिक्त, रिक्त);
}

// Number of symbols.
size_t derivation_size (ध्रुव derivation *deriv);
शून्य derivation_print (ध्रुव derivation *deriv, FILE *out, ध्रुव वर्ण *prefix);
शून्य derivation_print_leaves (ध्रुव derivation *deriv, FILE *out);
शून्य derivation_free (derivation *deriv);
शून्य derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (शून्य);

#endif /* DERIVATION_H */
