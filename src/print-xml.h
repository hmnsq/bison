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

ശൂന്യം xml_indent (FILE *out, പൂർണ്ണസംഖ്യ level);
ശൂന്യം xml_puts (FILE *, പൂർണ്ണസംഖ്യ, അക്ഷരം സ്ഥിരം *);
ശൂന്യം xml_printf (FILE *, പൂർണ്ണസംഖ്യ, അക്ഷരം സ്ഥിരം *, ...);
അക്ഷരം സ്ഥിരം *xml_escape_n (പൂർണ്ണസംഖ്യ n, അക്ഷരം സ്ഥിരം *str);
അക്ഷരം സ്ഥിരം *xml_escape (അക്ഷരം സ്ഥിരം *str);
ശൂന്യം print_xml (ശൂന്യം);

/* Use xsltproc to generate HTML from XML output.  */
ശൂന്യം print_html (ശൂന്യം);

#endif /* !PRINT_XML_H_ */
