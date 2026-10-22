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

शून्य xml_indent (FILE *out, पूर्णाङ्क level);
शून्य xml_puts (FILE *, पूर्णाङ्क, वर्ण ध्रुव *);
शून्य xml_printf (FILE *, पूर्णाङ्क, वर्ण ध्रुव *, ...);
वर्ण ध्रुव *xml_escape_n (पूर्णाङ्क n, वर्ण ध्रुव *str);
वर्ण ध्रुव *xml_escape (वर्ण ध्रुव *str);
शून्य print_xml (शून्य);

/* Use xsltproc to generate HTML from XML output.  */
शून्य print_html (शून्य);

#endif /* !PRINT_XML_H_ */
