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


#جے_بنا_تعریف سمبندھ_H
# define سمبندھ_H

/* Performing operations on graphs coded as list of adjacency.

   If GRAPH is a relation, then GRAPH[Node] is a list of adjacent
   nodes, ended with END_NODE.  */

# define سرا_گرہ ((سمبندھ_گرہ) -1)

تعريفنوع size_t سمبندھ_گرہ;
تعريفنوع سمبندھ_گرہ *سمبندھ_nodes;
تعريفنوع سمبندھ_nodes *سمبندھ;

تعريفنوع فراغ (سمبندھ_گرہ_print) (سمبندھ_گرہ گرہ, FILE* باہرلا);

/* Report a relation R that has SIZE vertices.  */
فراغ سمبندھ_print (ثابت حرف *title,
                     سمبندھ r, size_t ناپ,
                     سمبندھ_گرہ_print print, FILE *باہرلا);

/* Compute the transitive closure of the FUNCTION on the relation R
   with SIZE vertices.

   If R (NODE1, NODE2) then on exit FUNCTION[NODE1] was extended
   (unioned) with FUNCTION[NODE2].

   FUNCTION is in-out, R is read only.  */
فراغ سمبندھ_digraph (ثابت سمبندھ r, سمبندھ_گرہ ناپ, bitsetv function);

/* Destructively transpose *R_ARG, of size SIZE.  */
فراغ سمبندھ_transpose (سمبندھ *R_arg, سمبندھ_گرہ ناپ);

#جے_انت /* ! RELATION_H_ */
