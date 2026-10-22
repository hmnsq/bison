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

# define END_عقدة ((relation_عقدة) -1)

تعريفنوع size_t relation_عقدة;
تعريفنوع relation_عقدة *relation_nodes;
تعريفنوع relation_nodes *relation;

تعريفنوع فراغ (relation_عقدة_print) (relation_عقدة عقدة, FILE* out);

/* Report a relation R that has SIZE vertices.  */
فراغ relation_print (ثابت حرف *title,
                     relation r, size_t size,
                     relation_عقدة_print print, FILE *out);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
فراغ relation_digraph (ثابت relation r, relation_عقدة size, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
فراغ relation_transpose (relation *R_arg, relation_عقدة size);

#endif /* ! RELATION_H_ */
