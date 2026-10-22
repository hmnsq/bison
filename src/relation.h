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


#جيڪڏهن_وصف_نه_ٿيل لاڳاپو_H
# define لاڳاپو_H

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define پڇاڙي_ڳنڍ ((لاڳاپو_ڳنڍ) -1)

تعريفنوع size_t لاڳاپو_ڳنڍ;
تعريفنوع لاڳاپو_ڳنڍ *لاڳاپو_nodes;
تعريفنوع لاڳاپو_nodes *لاڳاپو;

تعريفنوع فراغ (لاڳاپو_ڳنڍ_print) (لاڳاپو_ڳنڍ ڳنڍ, FILE* ٻاهريون);

/* Report a relation R that has SIZE vertices.  */
فراغ لاڳاپو_print (ثابت حرف *title,
                     لاڳاپو r, size_t سائيز,
                     لاڳاپو_ڳنڍ_print print, FILE *ٻاهريون);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
فراغ لاڳاپو_digraph (ثابت لاڳاپو r, لاڳاپو_ڳنڍ سائيز, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
فراغ لاڳاپو_transpose (لاڳاپو *R_arg, لاڳاپو_ڳنڍ سائيز);

#جيڪڏهن_پورو /* ! RELATION_H_ */
