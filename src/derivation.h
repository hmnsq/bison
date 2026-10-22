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

প্রকারনির্দেশ gl_list_t derivation_list;
প্রকারনির্দেশ সংরচনা derivation derivation;

স্থির অন্তঃস্থ derivation_list derivation_list_new (শূন্য)
{
  প্রত্যাগম gl_list_create_empty (GL_LINKED_LIST, রিক্ত, রিক্ত, রিক্ত, true);
}

স্থির অন্তঃস্থ bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  ধ্রুব শূন্য *p = রিক্ত;
  bool res = gl_list_iterator_next (it, &p, রিক্ত);
  যদি (res)
    *d = (derivation *) p;
  অন্যথা
    gl_list_iterator_free (it);
  প্রত্যাগম res;
}

শূন্য derivation_list_append (derivation_list dl, derivation *d);
শূন্য derivation_list_prepend (derivation_list dl, derivation *d);
শূন্য derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (প্রতীক_সংখ্যা sym, derivation_list children,
                ধ্রুব rule *r);

স্থির অন্তঃস্থ derivation *derivation_new_leaf (প্রতীক_সংখ্যা sym)
{
  প্রত্যাগম derivation_new (sym, রিক্ত, রিক্ত);
}

// Number of symbols.
size_t derivation_size (ধ্রুব derivation *deriv);
শূন্য derivation_print (ধ্রুব derivation *deriv, FILE *out, ধ্রুব বর্ণ *prefix);
শূন্য derivation_print_leaves (ধ্রুব derivation *deriv, FILE *out);
শূন্য derivation_free (derivation *deriv);
শূন্য derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (শূন্য);

#endif /* DERIVATION_H */
