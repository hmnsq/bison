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

ശൂന്യം reduce_grammar (ശൂന്യം);
ശൂന്യം reduce_output (FILE *out);
bool reduce_token_unused_in_grammar (ചിഹ്നം_സംഖ്യ i);

/** Whether symbol \a i is useless in the grammar.
 * \pre  reduce_grammar was called before.
 */
bool reduce_nonterminal_useless_in_grammar (സ്ഥിരം sym_content *sym);

ശൂന്യം reduce_free (ശൂന്യം);

/** Map initial nterm numbers to the new ones.  Built by
 * reduce_grammar.  Size nnterms + nuseless_nonterminals.  */
ബാഹ്യ ചിഹ്നം_സംഖ്യ *nterm_map;

ബാഹ്യ പൂർണ്ണസംഖ്യ nuseless_nonterminals;
ബാഹ്യ പൂർണ്ണസംഖ്യ nuseless_productions;

#endif /* !REDUCE_H_ */
