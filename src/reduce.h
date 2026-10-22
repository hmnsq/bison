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

#ifndef REDUCE_H_
# define REDUCE_H_

શૂન્ય reduce_grammar (શૂન્ય);
શૂન્ય reduce_output (FILE *out);
bool reduce_token_unused_in_grammar (પ્રતીક_સંખ્યા i);

/** Whether symbol \a i is useless in the grammar.
 * \pre  reduce_grammar was called before.
 */
bool reduce_nonterminal_useless_in_grammar (અચલ sym_content *sym);

શૂન્ય reduce_free (શૂન્ય);

/** Map initial nterm numbers to the new ones.  Built by
 * reduce_grammar.  Size nnterms + nuseless_nonterminals.  */
બાહ્ય પ્રતીક_સંખ્યા *nterm_map;

બાહ્ય પૂર્ણાંક nuseless_nonterminals;
બાહ્ય પૂર્ણાંક nuseless_productions;

#endif /* !REDUCE_H_ */
