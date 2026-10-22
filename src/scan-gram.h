/* Bison Grammar Scanner

   Copyright (C) 2006-2007, 2009-2015, 2018-2022 Free Software
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪರಿಶೀಲನೆ_GRAM_H
# define ಪರಿಶೀಲನೆ_GRAM_H

/* Initialize the scanner to read file GRAM. */
ಶೂನ್ಯ gram_scanner_open (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *gram);
/* Close the open files.  */
ಶೂನ್ಯ gram_scanner_close (ಶೂನ್ಯ);

/* Free all the memory allocated to the scanner. */
ಶೂನ್ಯ gram_scanner_free (ಶೂನ್ಯ);
ಶೂನ್ಯ gram_scanner_ಕೊನೆಯ_string_free (ಶೂನ್ಯ);

# define GRAM_LEX_DECL ಪೂರ್ಣಾಂಕ gram_lex (GRAM_STYPE *val, location *loc)
GRAM_LEX_DECL;

#ಆದರೆ_ಅಂತ್ಯ /* !SCAN_GRAM_H_ */
