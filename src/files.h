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
ബാഹ്യ അക്ഷരം സ്ഥിരം *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
ബാഹ്യ അക്ഷരം *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
ബാഹ്യ സ്ഥിരം അക്ഷരം *spec_name_prefix;
ബാഹ്യ location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
ബാഹ്യ അക്ഷരം സ്ഥിരം *spec_file_prefix;
ബാഹ്യ location spec_file_prefix_loc;

/* --verbose. */
ബാഹ്യ അക്ഷരം *spec_verbose_file;

/* File name specified for the output graph.  */
ബാഹ്യ അക്ഷരം *spec_graph_file;

/* File name specified for the HTML output.  */
ബാഹ്യ അക്ഷരം *spec_html_file;

/* File name specified for the XML output.  */
ബാഹ്യ അക്ഷരം *spec_xml_file;

/* File name specified with --header.  */
ബാഹ്യ അക്ഷരം *spec_header_file;

/* Directory prefix of output file names.  */
ബാഹ്യ അക്ഷരം *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
ബാഹ്യ uniqstr grammar_file;

/* The computed base for output file names.  */
ബാഹ്യ അക്ഷരം *all_but_ext;

/* Where our data files are installed.  */
അക്ഷരം സ്ഥിരം *pkgdatadir (ശൂന്യം);

/* Where the m4 program is installed.  */
അക്ഷരം സ്ഥിരം *m4path (ശൂന്യം);

ശൂന്യം compute_output_file_names (ശൂന്യം);
ശൂന്യം output_file_names_free (ശൂന്യം);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
ശൂന്യം output_file_name_check (അക്ഷരം **file_name, bool source);

/** Remove all the generated source files. */
ശൂന്യം unlink_generated_sources (ശൂന്യം);

FILE *xfopen (സ്ഥിരം അക്ഷരം *name, അക്ഷരം സ്ഥിരം *mode);
ശൂന്യം xfclose (FILE *ptr);
FILE *xfdopen (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
സ്ഥിരം അക്ഷരം *map_file_name (അക്ഷരം സ്ഥിരം *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
ശൂന്യം add_prefix_map (അക്ഷരം സ്ഥിരം *oldprefix, അക്ഷരം സ്ഥിരം *newprefix);

#endif /* !FILES_H_ */
