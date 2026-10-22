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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FILES_H_
# define FILES_H_

# include "location.h"
# include "uniqstr.h"

/* File name specified with -o for the output file, or 0 if no -o.  */
ಬಾಹ್ಯ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec_outfile;

/* File name for the parser (i.e., the one above, or its default.) */
ಬಾಹ್ಯ ಅಕ್ಷರ *parser_file_name;

/* Symbol prefix specified with -p, or 0 if no -p.  */
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ;
ಬಾಹ್ಯ location spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ_loc;

/* File name prefix specified with -b, or 0 if no -b.  */
ಬಾಹ್ಯ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ;
ಬಾಹ್ಯ location spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc;

/* --verbose. */
ಬಾಹ್ಯ ಅಕ್ಷರ *spec_verbose_file;

/* File name specified for the output graph.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *spec_ನಕ್ಷೆ_file;

/* File name specified for the HTML output.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *spec_html_file;

/* File name specified for the XML output.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *spec_xml_file;

/* File name specified with --header.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *spec_ಶೀರ್ಷಿಕೆ_file;

/* Directory prefix of output file names.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *dir_ಪೂರ್ವಪ್ರತ್ಯಯ;

/* The file name as given on the command line.
   Not named "input_file" because Flex uses this name for an argument,
   and therefore GCC warns about a name clash. */
ಬಾಹ್ಯ uniqstr grammar_file;

/* The computed base for output file names.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *all_but_ext;

/* Where our data files are installed.  */
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pkgdatadir (ಶೂನ್ಯ);

/* Where the m4 program is installed.  */
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *m4path (ಶೂನ್ಯ);

ಶೂನ್ಯ compute_output_file_ಹೆಸರುಗಳು (ಶೂನ್ಯ);
ಶೂನ್ಯ output_file_ಹೆಸರುಗಳು_free (ಶೂನ್ಯ);

/** Record that we generate a file.
 *
 *  \param file_name  the name of file being generated.
 *  \param source whether this is a source file (*c, *.java...)
 *                as opposed to a report (*.output, *.dot...).
 */
ಶೂನ್ಯ output_file_name_check (ಅಕ್ಷರ **file_name, bool ಮೂಲ);

/** Remove all the generated source files. */
ಶೂನ್ಯ unlink_generated_sources (ಶೂನ್ಯ);

FILE *xfopen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಹುಲಕ);
ಶೂನ್ಯ xfclose (FILE *ptr);
FILE *xfdopen (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಹುಲಕ);

/* Given an input file path, return a string that contains the path
   with the file prefix mapping rules applied, or NULL if the input
   was NULL.  Do not free the return value.  */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map_file_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename);

/* Add a new file prefix mapping. If a file path starts with
   oldprefix, it will be replaced with newprefix.  */
ಶೂನ್ಯ ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ_map (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *oldprefix, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *newprefix);

#ಆದರೆ_ಅಂತ್ಯ /* !FILES_H_ */
