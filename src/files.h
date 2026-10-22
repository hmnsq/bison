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

#अगर_अघोषित FILES_H_
# define FILES_H_

# include "location.h"
# include "uniqstr.h"

/* File name specified with -o for the output file, or 0 if no -o.  */
बाह्य अक्षर स्थिर *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
बाह्य अक्षर *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
बाह्य स्थिर अक्षर *spec_name_prefix;
बाह्य location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
बाह्य अक्षर स्थिर *spec_file_prefix;
बाह्य location spec_file_prefix_loc;

/* --verbose. */
बाह्य अक्षर *spec_verbose_file;

/* File name specified for the output graph.  */
बाह्य अक्षर *spec_graph_file;

/* File name specified for the HTML output.  */
बाह्य अक्षर *spec_html_file;

/* File name specified for the XML output.  */
बाह्य अक्षर *spec_xml_file;

/* File name specified with --header.  */
बाह्य अक्षर *spec_header_file;

/* Directory prefix of output file names.  */
बाह्य अक्षर *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
बाह्य uniqstr grammar_file;

/* The computed base for output file names.  */
बाह्य अक्षर *all_but_ext;

/* Where our data files are installed.  */
अक्षर स्थिर *pkgdatadir (व्योम);

/* Where the m4 program is installed.  */
अक्षर स्थिर *m4path (व्योम);

व्योम compute_output_file_names (व्योम);
व्योम output_file_names_free (व्योम);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
व्योम output_file_name_check (अक्षर **file_name, bool source);

/** Remove all the generated source files. */
व्योम unlink_generated_sources (व्योम);

खाता *xfopen (स्थिर अक्षर *name, अक्षर स्थिर *mode);
व्योम xfclose (खाता *ptr);
खाता *xfdopen (पूर्णांक fd, अक्षर स्थिर *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
स्थिर अक्षर *map_file_name (अक्षर स्थिर *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
व्योम add_prefix_map (अक्षर स्थिर *oldprefix, अक्षर स्थिर *newprefix);

#पूर्ण_अगर /* !FILES_H_ */
