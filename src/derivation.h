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

வகைவரை gl_list_t derivation_list;
வகைவரை கட்டமைப்பு derivation derivation;

நிலையான உள்வரி derivation_list derivation_list_new (வெறுமை)
{
  திரும்பு gl_list_create_empty (GL_LINKED_LIST, வெற்று, வெற்று, வெற்று, true);
}

நிலையான உள்வரி bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  மாறிலி வெறுமை *p = வெற்று;
  bool res = gl_list_iterator_next (it, &p, வெற்று);
  எனில் (res)
    *d = (derivation *) p;
  இல்லையெனில்
    gl_list_iterator_free (it);
  திரும்பு res;
}

வெறுமை derivation_list_append (derivation_list dl, derivation *d);
வெறுமை derivation_list_prepend (derivation_list dl, derivation *d);
வெறுமை derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (குறியீடு_எண் sym, derivation_list children,
                மாறிலி rule *r);

நிலையான உள்வரி derivation *derivation_new_leaf (குறியீடு_எண் sym)
{
  திரும்பு derivation_new (sym, வெற்று, வெற்று);
}

// Number of symbols.
size_t derivation_size (மாறிலி derivation *deriv);
வெறுமை derivation_print (மாறிலி derivation *deriv, FILE *out, மாறிலி எழுத்து *prefix);
வெறுமை derivation_print_leaves (மாறிலி derivation *deriv, FILE *out);
வெறுமை derivation_free (derivation *deriv);
வெறுமை derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (வெறுமை);

#endif /* DERIVATION_H */
