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


#अगर_अघोषित RELATION_H_
# define RELATION_H_

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define END_गाँठ ((relation_गाँठ) -1)

प्रकार माप_प्रकार relation_गाँठ;
प्रकार relation_गाँठ *relation_nodes;
प्रकार relation_nodes *relation;

प्रकार व्योम (relation_गाँठ_print) (relation_गाँठ गाँठ, खाता* out);

/* Report a relation R that has SIZE vertices.  */
व्योम relation_print (स्थिर अक्षर *title,
                     relation r, माप_प्रकार आकार,
                     relation_गाँठ_print print, खाता *out);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
व्योम relation_digraph (स्थिर relation r, relation_गाँठ आकार, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
व्योम relation_transpose (relation *R_arg, relation_गाँठ आकार);

#पूर्ण_अगर /* ! RELATION_H_ */
