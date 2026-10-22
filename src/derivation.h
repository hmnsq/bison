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

પ્રકારવ્યાખ્યા gl_list_t derivation_list;
પ્રકારવ્યાખ્યા સંરચના derivation derivation;

સ્થિર અંતઃરેખ derivation_list derivation_list_new (શૂન્ય)
{
  પાછા gl_list_create_empty (GL_LINKED_LIST, ખાલી, ખાલી, ખાલી, true);
}

સ્થિર અંતઃરેખ bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  અચલ શૂન્ય *p = ખાલી;
  bool res = gl_list_iterator_next (it, &p, ખાલી);
  જો (res)
    *d = (derivation *) p;
  નહીંતર
    gl_list_iterator_free (it);
  પાછા res;
}

શૂન્ય derivation_list_append (derivation_list dl, derivation *d);
શૂન્ય derivation_list_prepend (derivation_list dl, derivation *d);
શૂન્ય derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (પ્રતીક_સંખ્યા sym, derivation_list children,
                અચલ rule *r);

સ્થિર અંતઃરેખ derivation *derivation_new_leaf (પ્રતીક_સંખ્યા sym)
{
  પાછા derivation_new (sym, ખાલી, ખાલી);
}

// Number of symbols.
size_t derivation_size (અચલ derivation *deriv);
શૂન્ય derivation_print (અચલ derivation *deriv, FILE *out, અચલ અક્ષર *prefix);
શૂન્ય derivation_print_leaves (અચલ derivation *deriv, FILE *out);
શૂન્ય derivation_free (derivation *deriv);
શૂન્ય derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (શૂન્ય);

#endif /* DERIVATION_H */
