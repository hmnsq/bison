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

#अगर_अघोषित DERIVATION_H
# define DERIVATION_H

# include <gl_linked_list.h>
# include <gl_xlist.h>

# include "gram.h"

/* Derivations are trees of symbols such that each nonterminal's
   children are symbols that produce that nonterminal if they are
   relevant to the counterexample.  The leaves of a derivation form a
   counterexample when printed.  */

प्रकार gl_list_t derivation_list;
प्रकार काष्ठा derivation derivation;

अटल अंतरभूत derivation_list derivation_list_new (व्योम)
{
  वापस gl_list_create_empty (GL_LINKED_LIST, शून्य, शून्य, शून्य, true);
}

अटल अंतरभूत bool
derivation_list_अग्र (gl_list_iterator_t *it, derivation **d)
{
  स्थिर व्योम *p = शून्य;
  bool res = gl_list_iterator_अग्र (it, &p, शून्य);
  अगर (res)
    *d = (derivation *) p;
  अन्यथा
    gl_list_iterator_free (it);
  वापस res;
}

व्योम derivation_list_append (derivation_list dl, derivation *d);
व्योम derivation_list_prepend (derivation_list dl, derivation *d);
व्योम derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (प्रतीक_संख्या sym, derivation_list children,
                स्थिर rule *r);

अटल अंतरभूत derivation *derivation_new_leaf (प्रतीक_संख्या sym)
{
  वापस derivation_new (sym, शून्य, शून्य);
}

// Number of symbols.
माप_प्रकार derivation_आकार (स्थिर derivation *deriv);
व्योम derivation_print (स्थिर derivation *deriv, खाता *out, स्थिर अक्षर *prefix);
व्योम derivation_print_leaves (स्थिर derivation *deriv, खाता *out);
व्योम derivation_free (derivation *deriv);
व्योम derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (व्योम);

#पूर्ण_अगर /* DERIVATION_H */
