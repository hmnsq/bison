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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ REDUCE_H_
# define REDUCE_H_

ಶೂನ್ಯ reduce_grammar (ಶೂನ್ಯ);
ಶೂನ್ಯ reduce_output (FILE *ಹೊರಹೋಗು);
bool reduce_token_ಬಳಸದ_ಒಳ_grammar (ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ i);

/** Whether symbol \a i is useless in the grammar.
 * \pre  reduce_grammar was called before.
 */
bool reduce_nonterminal_useless_ಒಳ_grammar (ಸ್ಥಿರಾಂಕ sym_content *sym);

ಶೂನ್ಯ reduce_free (ಶೂನ್ಯ);

/** Map initial nterm numbers to the new ones.  Built by
 * reduce_grammar.  Size nnterms + nuseless_nonterminals.  */
ಬಾಹ್ಯ ಚಿಹ್ನೆ_ಸಂಖ್ಯೆ *nterm_map;

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ nuseless_nonterminals;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ nuseless_productions;

#ಆದರೆ_ಅಂತ್ಯ /* !REDUCE_H_ */
