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
બાહ્ય અક્ષર અચલ *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
બાહ્ય અક્ષર *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
બાહ્ય અચલ અક્ષર *spec_name_prefix;
બાહ્ય location spec_name_prefix_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
બાહ્ય અક્ષર અચલ *spec_file_prefix;
બાહ્ય location spec_file_prefix_loc;

/* --verbose. */
બાહ્ય અક્ષર *spec_verbose_file;

/* File name specified for the output graph.  */
બાહ્ય અક્ષર *spec_graph_file;

/* File name specified for the HTML output.  */
બાહ્ય અક્ષર *spec_html_file;

/* File name specified for the XML output.  */
બાહ્ય અક્ષર *spec_xml_file;

/* File name specified with --header.  */
બાહ્ય અક્ષર *spec_header_file;

/* Directory prefix of output file names.  */
બાહ્ય અક્ષર *dir_prefix;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
બાહ્ય uniqstr grammar_file;

/* The computed base for output file names.  */
બાહ્ય અક્ષર *all_but_ext;

/* Where our data files are installed.  */
અક્ષર અચલ *pkgdatadir (શૂન્ય);

/* Where the m4 program is installed.  */
અક્ષર અચલ *m4path (શૂન્ય);

શૂન્ય compute_output_file_names (શૂન્ય);
શૂન્ય output_file_names_free (શૂન્ય);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
શૂન્ય output_file_name_check (અક્ષર **file_name, bool source);

/** Remove all the generated source files. */
શૂન્ય unlink_generated_sources (શૂન્ય);

FILE *xfopen (અચલ અક્ષર *name, અક્ષર અચલ *mode);
શૂન્ય xfclose (FILE *ptr);
FILE *xfdopen (પૂર્ણાંક fd, અક્ષર અચલ *mode);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
અચલ અક્ષર *map_file_name (અક્ષર અચલ *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
શૂન્ય add_prefix_map (અક્ષર અચલ *oldprefix, અક્ષર અચલ *newprefix);

#endif /* !FILES_H_ */
