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
வெளி எழுத்து மாறிலி *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
வெளி எழுத்து *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
வெளி மாறிலி எழுத்து *spec_name_prefix;
வெளி location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
வெளி எழுத்து மாறிலி *spec_file_prefix;
வெளி location spec_file_prefix_loc;

/* --verbose. */
வெளி எழுத்து *spec_verbose_file;

/* File name specified for the output graph.  */
வெளி எழுத்து *spec_graph_file;

/* File name specified for the HTML output.  */
வெளி எழுத்து *spec_html_file;

/* File name specified for the XML output.  */
வெளி எழுத்து *spec_xml_file;

/* File name specified with --header.  */
வெளி எழுத்து *spec_header_file;

/* Directory prefix of output file names.  */
வெளி எழுத்து *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
வெளி uniqstr grammar_file;

/* The computed base for output file names.  */
வெளி எழுத்து *all_but_ext;

/* Where our data files are installed.  */
எழுத்து மாறிலி *pkgdatadir (வெறுமை);

/* Where the m4 program is installed.  */
எழுத்து மாறிலி *m4path (வெறுமை);

வெறுமை compute_output_file_names (வெறுமை);
வெறுமை output_file_names_free (வெறுமை);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
வெறுமை output_file_name_check (எழுத்து **file_name, bool source);

/** Remove all the generated source files. */
வெறுமை unlink_generated_sources (வெறுமை);

FILE *xfopen (மாறிலி எழுத்து *name, எழுத்து மாறிலி *mode);
வெறுமை xfclose (FILE *ptr);
FILE *xfdopen (முழு fd, எழுத்து மாறிலி *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
மாறிலி எழுத்து *map_file_name (எழுத்து மாறிலி *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
வெறுமை add_prefix_map (எழுத்து மாறிலி *oldprefix, எழுத்து மாறிலி *newprefix);

#endif /* !FILES_H_ */
