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


#که_نه_تعریف_شوی اړیکه_H
# define اړیکه_H

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define پای_ټکی_غوټه ((اړیکه_غوټه) -1)

تعريفنوع size_t اړیکه_غوټه;
تعريفنوع اړیکه_غوټه *اړیکه_nodes;
تعريفنوع اړیکه_nodes *اړیکه;

تعريفنوع فراغ (اړیکه_غوټه_print) (اړیکه_غوټه غوټه, FILE* بهر);

/* Report a relation R that has SIZE vertices.  */
فراغ اړیکه_print (ثابت حرف *title,
                     اړیکه r, size_t اندازه,
                     اړیکه_غوټه_print print, FILE *بهر);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
فراغ اړیکه_digraph (ثابت اړیکه r, اړیکه_غوټه اندازه, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
فراغ اړیکه_transpose (اړیکه *R_arg, اړیکه_غوټه اندازه);

#د_که_پای /* ! RELATION_H_ */
