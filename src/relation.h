/* Binary relations.

   Copyright (C) 2002, 2004, 2009-2015, 2018-2022 Free Software
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


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಸಂಬಂಧ_H
# define ಸಂಬಂಧ_H

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define ಕೊನೆ_ಗಂಟು ((ಸಂಬಂಧ_ಗಂಟು) -1)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ size_t ಸಂಬಂಧ_ಗಂಟು;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸಂಬಂಧ_ಗಂಟು *ಸಂಬಂಧ_nodes;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸಂಬಂಧ_nodes *ಸಂಬಂಧ;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (ಸಂಬಂಧ_ಗಂಟು_print) (ಸಂಬಂಧ_ಗಂಟು ಗಂಟು, FILE* ಹೊರಹೋಗು);

/* Report a relation R that has SIZE vertices.  */
ಶೂನ್ಯ ಸಂಬಂಧ_print (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *title,
                     ಸಂಬಂಧ r, size_t ಗಾತ್ರ_2,
                     ಸಂಬಂಧ_ಗಂಟು_print print, FILE *ಹೊರಹೋಗು);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
ಶೂನ್ಯ ಸಂಬಂಧ_digraph (ಸ್ಥಿರಾಂಕ ಸಂಬಂಧ r, ಸಂಬಂಧ_ಗಂಟು ಗಾತ್ರ_2, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
ಶೂನ್ಯ ಸಂಬಂಧ_transpose (ಸಂಬಂಧ *R_arg, ಸಂಬಂಧ_ಗಂಟು ಗಾತ್ರ_2);

#ಆದರೆ_ಅಂತ್ಯ /* ! RELATION_H_ */
