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

#अगर_अघोषित PRINT_XML_H_
# define PRINT_XML_H_

# include <stdio.h>

व्योम xml_indent (खाता *out, पूर्णांक level);
व्योम xml_puts (खाता *, पूर्णांक, अक्षर स्थिर *);
व्योम xml_printf (खाता *, पूर्णांक, अक्षर स्थिर *, ...);
अक्षर स्थिर *xml_escape_n (पूर्णांक n, अक्षर स्थिर *str);
अक्षर स्थिर *xml_escape (अक्षर स्थिर *str);
व्योम print_xml (व्योम);

/* Use xsltproc to generate HTML from XML output.  */
व्योम print_html (व्योम);

#पूर्ण_अगर /* !PRINT_XML_H_ */
