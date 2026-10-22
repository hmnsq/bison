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

#که_نه_تعریف_شوی FILES_H_
# define FILES_H_

# include "location.h"
# include "uniqstr.h"

/* File name specified with -o for the output file, or 0 if no -o.  */
خارجي حرف ثابت *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
خارجي حرف *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
خارجي ثابت حرف *spec_name_مختاړی;
خارجي location spec_name_مختاړی_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
خارجي حرف ثابت *spec_file_مختاړی;
خارجي location spec_file_مختاړی_loc;

/* --verbose. */
خارجي حرف *spec_verbose_file;

/* File name specified for the output graph.  */
خارجي حرف *spec_ګراف_file;

/* File name specified for the HTML output.  */
خارجي حرف *spec_html_file;

/* File name specified for the XML output.  */
خارجي حرف *spec_xml_file;

/* File name specified with --header.  */
خارجي حرف *spec_سرلیک_file;

/* Directory prefix of output file names.  */
خارجي حرف *dir_مختاړی;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
خارجي uniqstr grammar_file;

/* The computed base for output file names.  */
خارجي حرف *all_but_ext;

/* Where our data files are installed.  */
حرف ثابت *pkgdatadir (فراغ);

/* Where the m4 program is installed.  */
حرف ثابت *m4path (فراغ);

فراغ compute_output_file_نومونه (فراغ);
فراغ output_file_نومونه_free (فراغ);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
فراغ output_file_name_check (حرف **file_name, bool سرچینه);

/** Remove all the generated source files. */
فراغ unlink_generated_sources (فراغ);

FILE *xfopen (ثابت حرف *name, حرف ثابت *ډېرپېښ);
فراغ xfclose (FILE *ptr);
FILE *xfdopen (صحيح fd, حرف ثابت *ډېرپېښ);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
ثابت حرف *map_file_name (حرف ثابت *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
فراغ جمع_کړه_مختاړی_map (حرف ثابت *oldprefix, حرف ثابت *newprefix);

#د_که_پای /* !FILES_H_ */
