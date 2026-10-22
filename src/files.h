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
بیرونی نویسه ثابت *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
بیرونی نویسه *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
بیرونی ثابت نویسه *spec_name_prefix;
بیرونی location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
بیرونی نویسه ثابت *spec_file_prefix;
بیرونی location spec_file_prefix_loc;

/* --verbose. */
بیرونی نویسه *spec_verbose_file;

/* File name specified for the output graph.  */
بیرونی نویسه *spec_graph_file;

/* File name specified for the HTML output.  */
بیرونی نویسه *spec_html_file;

/* File name specified for the XML output.  */
بیرونی نویسه *spec_xml_file;

/* File name specified with --header.  */
بیرونی نویسه *spec_header_file;

/* Directory prefix of output file names.  */
بیرونی نویسه *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
بیرونی uniqstr grammar_file;

/* The computed base for output file names.  */
بیرونی نویسه *all_but_ext;

/* Where our data files are installed.  */
نویسه ثابت *pkgdatadir (تهی);

/* Where the m4 program is installed.  */
نویسه ثابت *m4path (تهی);

تهی compute_output_file_names (تهی);
تهی output_file_names_free (تهی);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
تهی output_file_name_check (نویسه **file_name, bool source);

/** Remove all the generated source files. */
تهی unlink_generated_sources (تهی);

FILE *xfopen (ثابت نویسه *name, نویسه ثابت *mode);
تهی xfclose (FILE *ptr);
FILE *xfdopen (صحیح fd, نویسه ثابت *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
ثابت نویسه *map_file_name (نویسه ثابت *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
تهی add_prefix_map (نویسه ثابت *oldprefix, نویسه ثابت *newprefix);

#endif /* !FILES_H_ */
