/* File names and variables for bison,

   Copyright (C) 1984, 1989, 2000-2002, 2006-2007, 2009-2015, 2018-2022
   Free Software Foundation, Inc.

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

#ifndef FILES_H_
# define FILES_H_

# include "location.h"
# include "uniqstr.h"

/* File name specified with -o for the output file, or 0 if no -o.  */
बाह्य वर्ण अचर *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
बाह्य वर्ण *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
बाह्य अचर वर्ण *spec_name_prefix;
बाह्य location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
बाह्य वर्ण अचर *spec_file_prefix;
बाह्य location spec_file_prefix_loc;

/* --verbose. */
बाह्य वर्ण *spec_verbose_file;

/* File name specified for the output graph.  */
बाह्य वर्ण *spec_graph_file;

/* File name specified for the HTML output.  */
बाह्य वर्ण *spec_html_file;

/* File name specified for the XML output.  */
बाह्य वर्ण *spec_xml_file;

/* File name specified with --header.  */
बाह्य वर्ण *spec_header_file;

/* Directory prefix of output file names.  */
बाह्य वर्ण *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
बाह्य uniqstr grammar_file;

/* The computed base for output file names.  */
बाह्य वर्ण *all_but_ext;

/* Where our data files are installed.  */
वर्ण अचर *pkgdatadir (शून्य);

/* Where the m4 program is installed.  */
वर्ण अचर *m4path (शून्य);

शून्य compute_output_file_names (शून्य);
शून्य output_file_names_free (शून्य);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
शून्य output_file_name_check (वर्ण **file_name, bool source);

/** Remove all the generated source files. */
शून्य unlink_generated_sources (शून्य);

खाता *xfopen (अचर वर्ण *name, वर्ण अचर *mode);
शून्य xfclose (खाता *ptr);
खाता *xfdopen (पूर्णाङ्क fd, वर्ण अचर *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
अचर वर्ण *map_file_name (वर्ण अचर *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
शून्य add_prefix_map (वर्ण अचर *oldprefix, वर्ण अचर *newprefix);

#endif /* !FILES_H_ */
