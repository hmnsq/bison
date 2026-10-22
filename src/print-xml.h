/* Output an xml of the generated parser, for Bison.

   Copyright (C) 2007, 2009-2015, 2018-2022 Free Software Foundation,
   Inc.

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

#ifndef PRINT_XML_H_
# define PRINT_XML_H_

# include <stdio.h>

ಶೂನ್ಯ xml_indent (FILE *out, ಪೂರ್ಣಾಙ್ಕ level);
ಶೂನ್ಯ xml_puts (FILE *, ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ ಧ್ರುವ *);
ಶೂನ್ಯ xml_printf (FILE *, ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ ಧ್ರುವ *, ...);
ವರ್ಣ ಧ್ರುವ *xml_escape_n (ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ ಧ್ರುವ *str);
ವರ್ಣ ಧ್ರುವ *xml_escape (ವರ್ಣ ಧ್ರುವ *str);
ಶೂನ್ಯ print_xml (ಶೂನ್ಯ);

/* Use xsltproc to generate HTML from XML output.  */
ಶೂನ್ಯ print_html (ಶೂನ್ಯ);

#endif /* !PRINT_XML_H_ */
