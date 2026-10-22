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

শূন্য xml_indent (FILE *out, পূর্ণসংখ্যা level);
শূন্য xml_puts (FILE *, পূর্ণসংখ্যা, অক্ষর ধ্রুবক *);
শূন্য xml_printf (FILE *, পূর্ণসংখ্যা, অক্ষর ধ্রুবক *, ...);
অক্ষর ধ্রুবক *xml_escape_n (পূর্ণসংখ্যা n, অক্ষর ধ্রুবক *str);
অক্ষর ধ্রুবক *xml_escape (অক্ষর ধ্রুবক *str);
শূন্য print_xml (শূন্য);

/* Use xsltproc to generate HTML from XML output.  */
শূন্য print_html (শূন্য);

#endif /* !PRINT_XML_H_ */
