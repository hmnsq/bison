/* Grammar reduction for Bison.

   Copyright (C) 2000-2002, 2007, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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

#यदि_अपरिभाषितम् REDUCE_H_
# define REDUCE_H_

शून्य reduce_grammar (शून्य);
शून्य reduce_output (FILE *बहिर्गमनम्);
bool reduce_token_अप्रयुक्तम्_अन्तर्_grammar (चिह्नम्_सङ्ख्या i);

/** Whether symbol \a i is useless in the grammar.
 * \pre  reduce_grammar was called before.
 */
bool reduce_nonterminal_useless_अन्तर्_grammar (ध्रुव sym_content *sym);

शून्य reduce_free (शून्य);

/** Map initial nterm numbers to the new ones.  Built by
 * reduce_grammar.  Size nnterms + nuseless_nonterminals.  */
बाह्य चिह्नम्_सङ्ख्या *nterm_map;

बाह्य पूर्णाङ्क nuseless_nonterminals;
बाह्य पूर्णाङ्क nuseless_productions;

#यदि_अन्तः /* !REDUCE_H_ */
