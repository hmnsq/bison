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


#ifndef RELATION_H_
# define RELATION_H_

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define END_ಗಂಟು ((relation_ಗಂಟು) -1)

ಪ್ರಕಾರನಿರ್ದೇಶ size_t relation_ಗಂಟು;
ಪ್ರಕಾರನಿರ್ದೇಶ relation_ಗಂಟು *relation_nodes;
ಪ್ರಕಾರನಿರ್ದೇಶ relation_nodes *relation;

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (relation_ಗಂಟು_print) (relation_ಗಂಟು ಗಂಟು, FILE* out);

/* Report a relation R that has SIZE vertices.  */
ಶೂನ್ಯ relation_print (ಧ್ರುವ ವರ್ಣ *title,
                     relation r, size_t size,
                     relation_ಗಂಟು_print print, FILE *out);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
ಶೂನ್ಯ relation_digraph (ಧ್ರುವ relation r, relation_ಗಂಟು size, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
ಶೂನ್ಯ relation_transpose (relation *R_arg, relation_ಗಂಟು size);

#endif /* ! RELATION_H_ */
