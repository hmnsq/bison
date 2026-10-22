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

#यदि_अपरिभाषितम् अवलोकनम्_GRAM_H
# define अवलोकनम्_GRAM_H

/* Initialize the scanner to read file GRAM. */
शून्य gram_scanner_open (ध्रुव वर्ण *gram);
/* Close the open files.  */
शून्य gram_scanner_close (शून्य);

/* Free all the memory allocated to the scanner. */
शून्य gram_scanner_free (शून्य);
शून्य gram_scanner_चरमः_string_free (शून्य);

# define GRAM_LEX_DECL पूर्णाङ्क gram_lex (GRAM_STYPE *val, location *loc)
GRAM_LEX_DECL;

#यदि_अन्तः /* !SCAN_GRAM_H_ */
