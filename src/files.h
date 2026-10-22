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
বাহ্য বর্ণ ধ্রুব *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
বাহ্য বর্ণ *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
বাহ্য ধ্রুব বর্ণ *spec_name_prefix;
বাহ্য location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
বাহ্য বর্ণ ধ্রুব *spec_file_prefix;
বাহ্য location spec_file_prefix_loc;

/* --verbose. */
বাহ্য বর্ণ *spec_verbose_file;

/* File name specified for the output graph.  */
বাহ্য বর্ণ *spec_graph_file;

/* File name specified for the HTML output.  */
বাহ্য বর্ণ *spec_html_file;

/* File name specified for the XML output.  */
বাহ্য বর্ণ *spec_xml_file;

/* File name specified with --header.  */
বাহ্য বর্ণ *spec_header_file;

/* Directory prefix of output file names.  */
বাহ্য বর্ণ *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
বাহ্য uniqstr grammar_file;

/* The computed base for output file names.  */
বাহ্য বর্ণ *all_but_ext;

/* Where our data files are installed.  */
বর্ণ ধ্রুব *pkgdatadir (শূন্য);

/* Where the m4 program is installed.  */
বর্ণ ধ্রুব *m4path (শূন্য);

শূন্য compute_output_file_names (শূন্য);
শূন্য output_file_names_free (শূন্য);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
শূন্য output_file_name_check (বর্ণ **file_name, bool source);

/** Remove all the generated source files. */
শূন্য unlink_generated_sources (শূন্য);

FILE *xfopen (ধ্রুব বর্ণ *name, বর্ণ ধ্রুব *mode);
শূন্য xfclose (FILE *ptr);
FILE *xfdopen (পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
ধ্রুব বর্ণ *map_file_name (বর্ণ ধ্রুব *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
শূন্য add_prefix_map (বর্ণ ধ্রুব *oldprefix, বর্ণ ধ্রুব *newprefix);

#endif /* !FILES_H_ */
