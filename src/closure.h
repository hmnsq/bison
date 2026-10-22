/* Subroutines for bison

   Copyright (C) 1984, 1989, 2000-2002, 2007, 2009-2015, 2018-2022 Free
   Software Foundation, Inc.

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

#ifndef CLOSURE_H_
# define CLOSURE_H_

# include "gram.h"

/* Allocates the itemset and ruleset vectors, and precomputes useful
   data so that closure can be called.  n is the number of elements to
   allocate for itemset.  */

ଶୂନ୍ୟ closure_new (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);


/* Given the kernel (aka core) of a state (a sorted vector of item indices
   ITEMS, of length N), set up RULESET and ITEMSET to indicate what
   rules could be run and which items could be accepted when those
   items are the active ones.  */

ଶୂନ୍ୟ closure (item_index ଅଚଳ *ବସ୍ତୁଗୁଡ଼ିକ, size_t n);


/* Free ITEMSET, RULESET and internal data.  */

ଶୂନ୍ୟ closure_free (ଶୂନ୍ୟ);


/* ITEMSET is a sorted vector of item indices; NITEMSET is its size
   (actually, points to just beyond the end of the part of it that is
   significant).  CLOSURE places there the indices of all items which
   represent units of input that could arrive next.  */

ବାହ୍ୟ item_index *itemset;
ବାହ୍ୟ size_t nitemset;

#endif /* !CLOSURE_H_ */
