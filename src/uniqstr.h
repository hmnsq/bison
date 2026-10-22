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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UNIQSTR_H_
# define UNIQSTR_H_

# include <stdio.h>

/*-----------------------------------------.
| Pointers to unique copies of C strings.  |
`-----------------------------------------*/

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *uniqstr;

/* Return the uniqstr for STR.  */
uniqstr uniqstr_new (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str);

/* Two uniqstr values have the same value iff they are the same.  */
# define UNIQSTR_EQ(Ustr1, Ustr2) (!!((Ustr1) == (Ustr2)))

/* Compare two uniqstr a la strcmp: negative for <, nul for =, and
   positive for >.  Undefined order, relies on addresses.  */
ಪೂರ್ಣಾಂಕ uniqstr_cmp (uniqstr u1, uniqstr u2);

/* Die if STR is not a uniqstr.  */
ಶೂನ್ಯ uniqstr_assert (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str);

/*----------------.
| Concatenation.  |
`----------------*/

/* Concatenate strings and return a uniqstr.  The goal of
   this macro is to make the caller's code a little more succinct.  */
# define UNIQSTR_CONCAT(...)                                            \
  uniqstr_concat (ARRAY_CARDINALITY (((ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *[]) {__VA_ARGS__})), \
                  __VA_ARGS__)
uniqstr uniqstr_concat (ಪೂರ್ಣಾಂಕ nargs, ...);

/*--------------------.
| Table of uniqstrs.  |
`--------------------*/

/* Create the string table.  */
ಶೂನ್ಯ uniqstrs_new (ಶೂನ್ಯ);

/* Free all the memory allocated for symbols.  */
ಶೂನ್ಯ uniqstrs_free (ಶೂನ್ಯ);

/* Report them all.  */
ಶೂನ್ಯ uniqstrs_print (ಶೂನ್ಯ);

#ಆದರೆ_ಅಂತ್ಯ /* ! defined UNIQSTR_H_ */
