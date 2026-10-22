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


#यदि_अपरिभाषितम् सम्बन्धः_H
# define सम्बन्धः_H

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define अन्तः_ग्रन्थिः ((सम्बन्धः_ग्रन्थिः) -1)

प्रकारनिर्देश size_t सम्बन्धः_ग्रन्थिः;
प्रकारनिर्देश सम्बन्धः_ग्रन्थिः *सम्बन्धः_nodes;
प्रकारनिर्देश सम्बन्धः_nodes *सम्बन्धः;

प्रकारनिर्देश शून्य (सम्बन्धः_ग्रन्थिः_print) (सम्बन्धः_ग्रन्थिः ग्रन्थिः, FILE* बहिर्गमनम्);

/* Report a relation R that has SIZE vertices.  */
शून्य सम्बन्धः_print (ध्रुव वर्ण *title,
                     सम्बन्धः r, size_t आकार,
                     सम्बन्धः_ग्रन्थिः_print print, FILE *बहिर्गमनम्);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
शून्य सम्बन्धः_digraph (ध्रुव सम्बन्धः r, सम्बन्धः_ग्रन्थिः आकार, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
शून्य सम्बन्धः_transpose (सम्बन्धः *R_arg, सम्बन्धः_ग्रन्थिः आकार);

#यदि_अन्तः /* ! RELATION_H_ */
