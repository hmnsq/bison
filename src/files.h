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
extern var_na const *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
extern var_na *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
extern const var_na *spec_name_prefix;
extern location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
extern var_na const *spec_file_prefix;
extern location spec_file_prefix_loc;

/* --verbose. */
extern var_na *spec_verbose_file;

/* File name specified for the output graph.  */
extern var_na *spec_graph_file;

/* File name specified for the HTML output.  */
extern var_na *spec_html_file;

/* File name specified for the XML output.  */
extern var_na *spec_xml_file;

/* File name specified with --header.  */
extern var_na *spec_header_file;

/* Directory prefix of output file names.  */
extern var_na *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
extern uniqstr grammar_file;

/* The computed base for output file names.  */
extern var_na *all_but_ext;

/* Where our data files are installed.  */
var_na const *pkgdatadir (void);

/* Where the m4 program is installed.  */
var_na const *m4path (void);

void compute_output_file_names (void);
void output_file_names_free (void);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
void output_file_name_check (var_na **file_name, bool source);

/** Remove all the generated source files. */
void unlink_generated_sources (void);

FILE *xfopen (const var_na *name, var_na const *mode);
void xfclose (FILE *ptr);
FILE *xfdopen (පූර්ණ_සංඛ්‍යා fd, var_na const *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
const var_na *map_file_name (var_na const *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
void add_prefix_map (var_na const *oldprefix, var_na const *newprefix);

#endif /* !FILES_H_ */
