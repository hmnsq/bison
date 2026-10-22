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

প্রকারলেখ gl_list_t derivation_list;
প্রকারলেখ কাঠামো derivation derivation;

স্থির অন্তঃরেখ derivation_list derivation_list_new (শূন্য)
{
  ফেরত gl_list_create_empty (GL_LINKED_LIST, ফাঁকা, ফাঁকা, ফাঁকা, true);
}

স্থির অন্তঃরেখ bool
derivation_list_পরবর্তী (gl_list_iterator_t *it, derivation **d)
{
  ধ্রুবক শূন্য *p = ফাঁকা;
  bool res = gl_list_iterator_পরবর্তী (it, &p, ফাঁকা);
  যদি (res)
    *d = (derivation *) p;
  অন্যথা
    gl_list_iterator_free (it);
  ফেরত res;
}

শূন্য derivation_list_append (derivation_list dl, derivation *d);
শূন্য derivation_list_prepend (derivation_list dl, derivation *d);
শূন্য derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (প্রতীক_সংখ্যা sym, derivation_list children,
                ধ্রুবক rule *r);

স্থির অন্তঃরেখ derivation *derivation_new_leaf (প্রতীক_সংখ্যা sym)
{
  ফেরত derivation_new (sym, ফাঁকা, ফাঁকা);
}

// Number of symbols.
size_t derivation_আকার (ধ্রুবক derivation *deriv);
শূন্য derivation_print (ধ্রুবক derivation *deriv, FILE *out, ধ্রুবক অক্ষর *prefix);
শূন্য derivation_print_leaves (ধ্রুবক derivation *deriv, FILE *out);
শূন্য derivation_free (derivation *deriv);
শূন্য derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (শূন্য);

#endif /* DERIVATION_H */
