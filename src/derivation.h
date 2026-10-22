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

#که_نه_تعریف_شوی DERIVATION_H
# define DERIVATION_H

# include <gl_linked_list.h>
# include <gl_xlist.h>

# include "gram.h"

/* Derivations are trees of symbols such that each nonterminal's
   children are symbols that produce that nonterminal if they are
   relevant to the counterexample.  The leaves of a derivation form a
   counterexample when printed.  */

تعريفنوع gl_list_t derivation_list;
تعريفنوع بنية derivation derivation;

ساكن مضمن derivation_list derivation_list_new (فراغ)
{
  أرجع gl_list_create_تشه (GL_LINKED_LIST, عدم, عدم, عدم, true);
}

ساكن مضمن bool
derivation_list_بل (gl_list_iterator_t *it, derivation **d)
{
  ثابت فراغ *p = عدم;
  bool res = gl_list_iterator_بل (it, &p, عدم);
  إذا (res)
    *d = (derivation *) p;
  وإلا
    gl_list_iterator_free (it);
  أرجع res;
}

فراغ derivation_list_append (derivation_list dl, derivation *d);
فراغ derivation_list_prepend (derivation_list dl, derivation *d);
فراغ derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (نښه_شمېره sym, derivation_list children,
                ثابت قاعده *r);

ساكن مضمن derivation *derivation_new_ونې_پاڼه (نښه_شمېره sym)
{
  أرجع derivation_new (sym, عدم, عدم);
}

// Number of symbols.
size_t derivation_اندازه (ثابت derivation *deriv);
فراغ derivation_print (ثابت derivation *deriv, FILE *بهر, ثابت حرف *مختاړی);
فراغ derivation_print_leaves (ثابت derivation *deriv, FILE *بهر);
فراغ derivation_free (derivation *deriv);
فراغ derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (فراغ);

#د_که_پای /* DERIVATION_H */
