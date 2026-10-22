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

ପ୍ରକାରବ୍ୟାଖ୍ୟା gl_list_t derivation_list;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା derivation derivation;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ derivation_list derivation_list_new (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ gl_list_create_empty (GL_LINKED_LIST, ଖାଲି, ଖାଲି, ଖାଲି, true);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  ଅଚଳ ଶୂନ୍ୟ *p = ଖାଲି;
  bool res = gl_list_iterator_next (it, &p, ଖାଲି);
  ଯଦି (res)
    *d = (derivation *) p;
  ଅନ୍ୟଥା
    gl_list_iterator_free (it);
  ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ derivation_list_append (derivation_list dl, derivation *d);
ଶୂନ୍ୟ derivation_list_prepend (derivation_list dl, derivation *d);
ଶୂନ୍ୟ derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (ପ୍ରତୀକ_ସଂଖ୍ୟା sym, derivation_list children,
                ଅଚଳ rule *r);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ derivation *derivation_new_leaf (ପ୍ରତୀକ_ସଂଖ୍ୟା sym)
{
  ଫେରନ୍ତୁ derivation_new (sym, ଖାଲି, ଖାଲି);
}

// Number of symbols.
size_t derivation_size (ଅଚଳ derivation *deriv);
ଶୂନ୍ୟ derivation_print (ଅଚଳ derivation *deriv, FILE *out, ଅଚଳ ଅକ୍ଷର *prefix);
ଶୂନ୍ୟ derivation_print_leaves (ଅଚଳ derivation *deriv, FILE *out);
ଶୂନ୍ୟ derivation_free (derivation *deriv);
ଶୂନ୍ୟ derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (ଶୂନ୍ୟ);

#endif /* DERIVATION_H */
