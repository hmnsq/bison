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

تعریفنوع gl_list_t derivation_list;
تعریفنوع ساختار derivation derivation;

ایستا درونخطی derivation_list derivation_list_new (تهی)
{
  بازگشت gl_list_create_empty (GL_LINKED_LIST, پوچ, پوچ, پوچ, true);
}

ایستا درونخطی bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  ثابت تهی *p = پوچ;
  bool res = gl_list_iterator_next (it, &p, پوچ);
  اگر (res)
    *d = (derivation *) p;
  وگرنه
    gl_list_iterator_free (it);
  بازگشت res;
}

تهی derivation_list_append (derivation_list dl, derivation *d);
تهی derivation_list_prepend (derivation_list dl, derivation *d);
تهی derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (نماد_عدد sym, derivation_list children,
                ثابت rule *r);

ایستا درونخطی derivation *derivation_new_leaf (نماد_عدد sym)
{
  بازگشت derivation_new (sym, پوچ, پوچ);
}

// Number of symbols.
size_t derivation_size (ثابت derivation *deriv);
تهی derivation_print (ثابت derivation *deriv, FILE *out, ثابت نویسه *prefix);
تهی derivation_print_leaves (ثابت derivation *deriv, FILE *out);
تهی derivation_free (derivation *deriv);
تهی derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (تهی);

#endif /* DERIVATION_H */
