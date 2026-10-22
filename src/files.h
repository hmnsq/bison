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
ବାହ୍ୟ ଅକ୍ଷର ଅଚଳ *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
ବାହ୍ୟ ଅକ୍ଷର *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
ବାହ୍ୟ ଅଚଳ ଅକ୍ଷର *spec_name_prefix;
ବାହ୍ୟ location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
ବାହ୍ୟ ଅକ୍ଷର ଅଚଳ *spec_file_prefix;
ବାହ୍ୟ location spec_file_prefix_loc;

/* --verbose. */
ବାହ୍ୟ ଅକ୍ଷର *spec_verbose_file;

/* File name specified for the output graph.  */
ବାହ୍ୟ ଅକ୍ଷର *spec_graph_file;

/* File name specified for the HTML output.  */
ବାହ୍ୟ ଅକ୍ଷର *spec_html_file;

/* File name specified for the XML output.  */
ବାହ୍ୟ ଅକ୍ଷର *spec_xml_file;

/* File name specified with --header.  */
ବାହ୍ୟ ଅକ୍ଷର *spec_header_file;

/* Directory prefix of output file names.  */
ବାହ୍ୟ ଅକ୍ଷର *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
ବାହ୍ୟ uniqstr grammar_file;

/* The computed base for output file names.  */
ବାହ୍ୟ ଅକ୍ଷର *all_but_ext;

/* Where our data files are installed.  */
ଅକ୍ଷର ଅଚଳ *pkgdatadir (ଶୂନ୍ୟ);

/* Where the m4 program is installed.  */
ଅକ୍ଷର ଅଚଳ *m4path (ଶୂନ୍ୟ);

ଶୂନ୍ୟ compute_output_file_names (ଶୂନ୍ୟ);
ଶୂନ୍ୟ output_file_names_free (ଶୂନ୍ୟ);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
ଶୂନ୍ୟ output_file_name_check (ଅକ୍ଷର **file_name, bool source);

/** Remove all the generated source files. */
ଶୂନ୍ୟ unlink_generated_sources (ଶୂନ୍ୟ);

FILE *xfopen (ଅଚଳ ଅକ୍ଷର *name, ଅକ୍ଷର ଅଚଳ *mode);
ଶୂନ୍ୟ xfclose (FILE *ptr);
FILE *xfdopen (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
ଅଚଳ ଅକ୍ଷର *map_file_name (ଅକ୍ଷର ଅଚଳ *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
ଶୂନ୍ୟ add_prefix_map (ଅକ୍ଷର ଅଚଳ *oldprefix, ଅକ୍ଷର ଅଚଳ *newprefix);

#endif /* !FILES_H_ */
