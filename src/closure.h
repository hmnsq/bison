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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಆವರಣ_H
# define ಆವರಣ_H

# include "gram.h"

/* Allocates the itemset and ruleset vectors, and precomputes useful
   data so that closure can be called.  n is the number of elements to
   allocate for itemset.  */

ಶೂನ್ಯ ಆವರಣ_new (ಪೂರ್ಣಾಂಕ n);


/* Given the kernel (aka core) of a state (a sorted vector of item indices
   ITEMS, of length N), set up RULESET and ITEMSET to indicate what
   rules could be run and which items could be accepted when those
   items are the active ones.  */

ಶೂನ್ಯ ಆವರಣ (ಅಂಶ_ಸೂಚ್ಯಂಕ ಸ್ಥಿರಾಂಕ *ವಸ್ತುಗಳು, size_t n);


/* Free ITEMSET, RULESET and internal data.  */

ಶೂನ್ಯ ಆವರಣ_free (ಶೂನ್ಯ);


/* ITEMSET is a sorted vector of item indices; NITEMSET is its size
   (actually, points to just beyond the end of the part of it that is
   significant).  CLOSURE places there the indices of all items which
   represent units of input that could arrive next.  */

ಬಾಹ್ಯ ಅಂಶ_ಸೂಚ್ಯಂಕ *itemset;
ಬಾಹ್ಯ size_t nitemset;

#ಆದರೆ_ಅಂತ್ಯ /* !CLOSURE_H_ */
