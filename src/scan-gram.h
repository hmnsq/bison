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

#ifndef SCAN_GRAM_H_
# define SCAN_GRAM_H_

/* Initialize the scanner to read file GRAM. */
શૂન્ય gram_scanner_open (અચલ અક્ષર *gram);
/* Close the open files.  */
શૂન્ય gram_scanner_close (શૂન્ય);

/* Free all the memory allocated to the scanner. */
શૂન્ય gram_scanner_free (શૂન્ય);
શૂન્ય gram_scanner_last_string_free (શૂન્ય);

# define GRAM_LEX_DECL પૂર્ણાંક gram_lex (GRAM_STYPE *val, location *loc)
GRAM_LEX_DECL;

#endif /* !SCAN_GRAM_H_ */
