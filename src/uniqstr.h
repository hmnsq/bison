/* Keeping a unique copy of strings.

   Copyright (C) 2002-2003, 2008-2015, 2018-2022 Free Software
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

#ifndef UNIQSTR_H_
# define UNIQSTR_H_

# include <stdio.h>

/*-----------------------------------------.
| Pointers to unique copies of C strings.  |
`-----------------------------------------*/

प्रकारनिर्देश वर्ण अचर *uniqstr;

/* Return the uniqstr for STR.  */
uniqstr uniqstr_new (वर्ण अचर *str);

/* Two uniqstr values have the same value iff they are the same.  */
# define UNIQSTR_EQ(Ustr1, Ustr2) (!!((Ustr1) == (Ustr2)))

/* Compare two uniqstr a la strcmp: negative for <, nul for =, and
   positive for >.  Undefined order, relies on addresses.  */
पूर्णाङ्क uniqstr_cmp (uniqstr u1, uniqstr u2);

/* Die if STR is not a uniqstr.  */
शून्य uniqstr_assert (वर्ण अचर *str);

/*----------------.
| Concatenation.  |
`----------------*/

/* Concatenate strings and return a uniqstr.  The goal of
   this macro is to make the caller's code a little more succinct.  */
# define UNIQSTR_CONCAT(...)                                            \
  uniqstr_concat (ARRAY_CARDINALITY (((वर्ण अचर *[]) {__VA_ARGS__})), \
                  __VA_ARGS__)
uniqstr uniqstr_concat (पूर्णाङ्क nargs, ...);

/*--------------------.
| Table of uniqstrs.  |
`--------------------*/

/* Create the string table.  */
शून्य uniqstrs_new (शून्य);

/* Free all the memory allocated for symbols.  */
शून्य uniqstrs_free (शून्य);

/* Report them all.  */
शून्य uniqstrs_print (शून्य);

#endif /* ! defined UNIQSTR_H_ */
