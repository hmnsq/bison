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

ଶୂନ୍ୟ xml_indent (FILE *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level);
ଶୂନ୍ୟ xml_puts (FILE *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର ଅଚଳ *);
ଶୂନ୍ୟ xml_printf (FILE *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର ଅଚଳ *, ...);
ଅକ୍ଷର ଅଚଳ *xml_escape_n (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର ଅଚଳ *str);
ଅକ୍ଷର ଅଚଳ *xml_escape (ଅକ୍ଷର ଅଚଳ *str);
ଶୂନ୍ୟ print_xml (ଶୂନ୍ୟ);

/* Use xsltproc to generate HTML from XML output.  */
ଶୂନ୍ୟ print_html (ଶୂନ୍ୟ);

#endif /* !PRINT_XML_H_ */
