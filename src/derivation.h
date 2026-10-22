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

തരനിർവചനം gl_list_t derivation_list;
തരനിർവചനം ഘടന derivation derivation;

സ്ഥിതം വരിയിൽ derivation_list derivation_list_new (ശൂന്യം)
{
  തിരികെ gl_list_create_empty (GL_LINKED_LIST, ശൂന്യ, ശൂന്യ, ശൂന്യ, true);
}

സ്ഥിതം വരിയിൽ bool
derivation_list_next (gl_list_iterator_t *it, derivation **d)
{
  സ്ഥിരം ശൂന്യം *p = ശൂന്യ;
  bool res = gl_list_iterator_next (it, &p, ശൂന്യ);
  എങ്കിൽ (res)
    *d = (derivation *) p;
  അല്ലെങ്കിൽ
    gl_list_iterator_free (it);
  തിരികെ res;
}

ശൂന്യം derivation_list_append (derivation_list dl, derivation *d);
ശൂന്യം derivation_list_prepend (derivation_list dl, derivation *d);
ശൂന്യം derivation_list_free (derivation_list dl);

// rule_num is the number of the rule SYM -> CHILDREN.
derivation *
derivation_new (ചിഹ്നം_സംഖ്യ sym, derivation_list children,
                സ്ഥിരം rule *r);

സ്ഥിതം വരിയിൽ derivation *derivation_new_leaf (ചിഹ്നം_സംഖ്യ sym)
{
  തിരികെ derivation_new (sym, ശൂന്യ, ശൂന്യ);
}

// Number of symbols.
size_t derivation_size (സ്ഥിരം derivation *deriv);
ശൂന്യം derivation_print (സ്ഥിരം derivation *deriv, FILE *out, സ്ഥിരം അക്ഷരം *prefix);
ശൂന്യം derivation_print_leaves (സ്ഥിരം derivation *deriv, FILE *out);
ശൂന്യം derivation_free (derivation *deriv);
ശൂന്യം derivation_retain (derivation *deriv);

// A derivation denoting the position of the dot.
derivation *derivation_dot (ശൂന്യം);

#endif /* DERIVATION_H */
