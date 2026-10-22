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

வெறுமை xml_indent (FILE *out, முழு level);
வெறுமை xml_puts (FILE *, முழு, எழுத்து மாறிலி *);
வெறுமை xml_printf (FILE *, முழு, எழுத்து மாறிலி *, ...);
எழுத்து மாறிலி *xml_escape_n (முழு n, எழுத்து மாறிலி *str);
எழுத்து மாறிலி *xml_escape (எழுத்து மாறிலி *str);
வெறுமை print_xml (வெறுமை);

/* Use xsltproc to generate HTML from XML output.  */
வெறுமை print_html (வெறுமை);

#endif /* !PRINT_XML_H_ */
