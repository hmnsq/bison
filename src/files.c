/* Open and close files for Bison.

   Copyright (C) 1984, 1986, 1989, 1992, 2000-2015, 2018-2022 Free
   Software Foundation, Inc.

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

#समावेश <config.h>
#समावेश "system.h"

#समावेश <configmake.h> /* PKGDATADIR */
#समावेश <dirname.h>
#समावेश <error.h>
#समावेश <get-errno.h>
#समावेश <gl_array_list.h>
#समावेश <gl_hash_map.h>
#समावेश <gl_xlist.h>
#समावेश <gl_xmap.h>
#समावेश <quote.h>
#समावेश <quotearg.h>
#समावेश <relocatable.h> /* relocate2 */
#समावेश <stdio-safer.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <unistd.h>
#समावेश <xstrndup.h>

#समावेश "complain.h"
#समावेश "files.h"
#समावेश "getargs.h"
#समावेश "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

अक्षर स्थिर *spec_outfile = शून्य;       /* for -o. */
अक्षर स्थिर *spec_file_prefix = शून्य;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
अक्षर स्थिर *spec_name_prefix = शून्य;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
अक्षर *spec_verbose_file = शून्य;  /* for --verbose. */
अक्षर *spec_graph_file = शून्य;    /* for -g. */
अक्षर *spec_html_file = शून्य;     /* for --html. */
अक्षर *spec_xml_file = शून्य;      /* for -x. */
अक्षर *spec_header_file = शून्य;   /* for --header. */
अक्षर *parser_file_name;

/* All computed output file names.  */
प्रकार काष्ठा generated_file
{
  /** File name.  */
  अक्षर *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
अटल generated_file *generated_files = शून्य;
अटल पूर्णांक generated_files_आकार = 0;

uniqstr grammar_file = शून्य;

/* If --output=dir/foo.c was specified,
   DIR_PREFIX gis 'dir/' and ALL_BUT_EXT and ALL_BUT_TAB_EXT are 'dir/foo'.

   If --output=dir/foo.tab.c was specified, DIR_PREFIX is 'dir/',
   ALL_BUT_EXT is 'dir/foo.tab', and ALL_BUT_TAB_EXT is 'dir/foo'.

   If --output was not specified but --file-prefix=dir/foo was specified,
   ALL_BUT_EXT = 'foo.tab' and ALL_BUT_TAB_EXT = 'foo'.

   If neither --output nor --file was specified but the input grammar
   is name dir/foo.y, ALL_BUT_EXT and ALL_BUT_TAB_EXT are 'foo'.

   If neither --output nor --file was specified, DIR_PREFIX is the
   empty string (meaning the current directory); otherwise it is
   'dir/'.  */

अक्षर *all_but_ext;
अटल अक्षर *all_but_tab_ext;
अक्षर *dir_prefix;

/* C source file extension (the parser source).  */
अटल अक्षर *src_extension = शून्य;
/* Header file extension (if option '`-d'' is specified).  */
अटल अक्षर *header_extension = शून्य;

काष्ठा prefix_map
{
  अक्षर *oldprefix;
  अक्षर *newprefix;
};

अटल gl_list_t prefix_maps = शून्य;

/* Map file names to prefix-mapped file names. */
अटल gl_map_t mapped_files = शून्य;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

अटल अक्षर *
concat2 (अक्षर स्थिर *str1, अक्षर स्थिर *str2)
{
  माप_प्रकार len = म_माप (str1) + म_माप (str2);
  अक्षर *res = xmalloc (len + 1);
  अक्षर *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  वापस res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

खाता *
xfopen (स्थिर अक्षर *name, स्थिर अक्षर *mode)
{
  खाता *res = fopen_safer (name, mode);
  अगर (!res)
    error (निकास_त्रुटि, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  वापस res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

व्योम
xfclose (खाता *ptr)
{
  अगर (ptr == शून्य)
    वापस;

  अगर (ख_त्रुटि (ptr))
    error (निकास_त्रुटि, 0, _("input/output error"));

  अगर (ख_बंद (ptr) != 0)
    error (निकास_त्रुटि, get_errno (), _("cannot close file"));
}


खाता *
xfdopen (पूर्णांक fd, अक्षर स्थिर *mode)
{
  खाता *res = fdopen (fd, mode);
  अगर (! res)
    error (निकास_त्रुटि, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  वापस res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
अटल अक्षर *
map_file_name_alloc (अक्षर स्थिर *filename)
{
  काष्ठा prefix_map स्थिर *p = शून्य;
  निश्चित (prefix_maps);
  {
    व्योम स्थिर *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    जबतक (gl_list_iterator_अग्र (&iter, &ptr, शून्य))
      {
        p = ptr;
        अगर (म_भेदन (p->oldprefix, filename, म_माप (p->oldprefix)) == 0)
          अवरोध;
        p = शून्य;
      }
    gl_list_iterator_free (&iter);
  }

  अगर (!p)
    वापस xstrdup (filename);

  माप_प्रकार oldprefix_len = म_माप (p->oldprefix);
  माप_प्रकार newprefix_len = म_माप (p->newprefix);
  अक्षर *res = xmalloc (newprefix_len + म_माप (filename) - oldprefix_len + 1);

  अक्षर *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  वापस res;
}

अटल bool
string_equals (स्थिर व्योम *x1, स्थिर व्योम *x2)
{
  स्थिर अक्षर *s1 = x1;
  स्थिर अक्षर *s2 = x2;
  वापस STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
अटल माप_प्रकार
string_hash (स्थिर व्योम *x)
{
#घोषणा आकार_BITS (माप (माप_प्रकार) * अक्षर_बिट)

  स्थिर अक्षर *s = x;
  माप_प्रकार h = 0;

  क्रम (; *s; s++)
    h = *s + ((h << 9) | (h >> (आकार_BITS - 9)));

  वापस h;
}

अटल व्योम
string_free (स्थिर व्योम *cp)
{
  व्योम *p = (व्योम*) cp;
  मुक्त (p);
}

स्थिर अक्षर *
map_file_name (अक्षर स्थिर *filename)
{
  अगर (!filename || !prefix_maps)
    वापस filename;
  अगर (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  स्थिर व्योम *res = gl_map_get (mapped_files, filename);
  अगर (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  वापस res;
}

अटल व्योम
prefix_map_free (काष्ठा prefix_map *p)
{
  मुक्त (p->oldprefix);
  मुक्त (p->newprefix);
  मुक्त (p);
}

व्योम
add_prefix_map (अक्षर स्थिर *oldprefix, अक्षर स्थिर *newprefix)
{
  अगर (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ शून्य,
                              /* hashcode */ शून्य,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  काष्ठा prefix_map *p = xmalloc (माप (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
अटल व्योम
compute_exts_from_gf (स्थिर अक्षर *ext)
{
  अगर (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  अन्यथा
    {
      src_extension = xstrdup (ext);
      header_extension = xstrdup (ext);
      tr (src_extension, 'y', 'c');
      tr (src_extension, 'Y', 'C');
      tr (header_extension, 'y', 'h');
      tr (header_extension, 'Y', 'H');
    }
}

/* Compute extensions from the given c source file extension.  */
अटल व्योम
compute_exts_from_src (स्थिर अक्षर *ext)
{
  /* We use this function when the user specifies `-o' or `--output',
     so the extensions must be computed unconditionally from the file name
     given by this option.  */
  src_extension = xstrdup (ext);
  header_extension = xstrdup (ext);
  tr (header_extension, 'c', 'h');
  tr (header_extension, 'C', 'H');
}


/* Decompose FILE_NAME in four parts: *BASE, *TAB, and *EXT, the fourth
   part, (the directory) is ranging from FILE_NAME to the char before
   *BASE, so we don't need an additional parameter.

   *EXT points to the last period in the basename, or NULL if none.

   If there is no *EXT, *TAB is NULL.  Otherwise, *TAB points to
   '.tab' or '_tab' if present right before *EXT, or is NULL. *TAB
   cannot be equal to *BASE.

   None are allocated, they are simply pointers to parts of FILE_NAME.
   Examples:

   '/tmp/foo.tab.c' -> *BASE = 'foo.tab.c', *TAB = '.tab.c', *EXT =
   '.c'

   'foo.c' -> *BASE = 'foo.c', *TAB = NULL, *EXT = '.c'

   'tab.c' -> *BASE = 'tab.c', *TAB = NULL, *EXT = '.c'

   '.tab.c' -> *BASE = '.tab.c', *TAB = NULL, *EXT = '.c'

   'foo.tab' -> *BASE = 'foo.tab', *TAB = NULL, *EXT = '.tab'

   'foo_tab' -> *BASE = 'foo_tab', *TAB = NULL, *EXT = NULL

   'foo' -> *BASE = 'foo', *TAB = NULL, *EXT = NULL.  */

अटल व्योम
file_name_split (स्थिर अक्षर *file_name,
                 स्थिर अक्षर **base, स्थिर अक्षर **tab, स्थिर अक्षर **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = म_खोजप (*base, '.');
  *tab = शून्य;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  अगर (*ext)
    {
      माप_प्रकार baselen = *ext - *base;
      माप_प्रकार dottablen = माप (TAB_EXT) - 1;
      अगर (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

अटल व्योम
compute_file_name_parts (व्योम)
{
  अगर (spec_outfile)
    {
      स्थिर अक्षर *base, *tab, *ext;
      file_name_split (spec_outfile, &base, &tab, &ext);
      dir_prefix = xstrndup (spec_outfile, base - spec_outfile);

      /* ALL_BUT_EXT goes up the EXT, excluding it. */
      all_but_ext =
        xstrndup (spec_outfile,
                  (म_माप (spec_outfile) - (ext ? म_माप (ext) : 0)));

      /* ALL_BUT_TAB_EXT goes up to TAB, excluding it.  */
      all_but_tab_ext =
        xstrndup (spec_outfile,
                  (म_माप (spec_outfile)
                   - (tab ? म_माप (tab) : (ext ? म_माप (ext) : 0))));

      अगर (ext)
        compute_exts_from_src (ext);
    }
  अन्यथा
    {
      स्थिर अक्षर *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      अगर (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      अन्यथा अगर (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      अन्यथा
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (म_माप (base) - (ext ? म_माप (ext) : 0)));
        }

      अगर (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      अन्यथा
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      अगर (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

व्योम
compute_output_file_names (व्योम)
{
  compute_file_name_parts ();

  /* If not yet done. */
  अगर (!src_extension)
    src_extension = xstrdup (".c");
  अगर (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  अगर (header_flag)
    {
      अगर (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  अगर (graph_flag)
    {
      अगर (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  अगर (html_flag)
    {
      अगर (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  अगर (xml_flag)
    {
      अगर (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  अगर (report_flag)
    {
      अगर (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  मुक्त (all_but_tab_ext);
  मुक्त (src_extension);
  मुक्त (header_extension);
}

व्योम
output_file_name_check (अक्षर **file_name, bool source)
{
  bool conflict = false;
  अगर (STREQ (*file_name, grammar_file))
    {
      complain (शून्य, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  अन्यथा
    क्रम (पूर्णांक i = 0; i < generated_files_आकार; i++)
      अगर (STREQ (generated_files[i].name, *file_name))
        {
          complain (शून्य, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  अगर (conflict)
    {
      मुक्त (*file_name);
      *file_name = strdup ("/dev/null");
    }
  अन्यथा
    {
      generated_files = xnrealloc (generated_files, ++generated_files_आकार,
                                   माप *generated_files);
      generated_files[generated_files_आकार-1].name = xstrdup (*file_name);
      generated_files[generated_files_आकार-1].is_source = source;
    }
}

व्योम
unlink_generated_sources (व्योम)
{
  क्रम (पूर्णांक i = 0; i < generated_files_आकार; i++)
    अगर (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
अटल अक्षर *relocate_बफर = शून्य;

अक्षर स्थिर *
pkgdatadir (व्योम)
{
  अगर (relocate_बफर)
    वापस relocate_बफर;
  अन्यथा
    {
      अक्षर स्थिर *cp = दो_पर्या ("BISON_PKGDATADIR");
      वापस cp ? cp : relocate2 (PKGDATADIR, &relocate_बफर);
    }
}

अक्षर स्थिर *
m4path (व्योम)
{
  अक्षर स्थिर *m4 = दो_पर्या ("M4");
  अगर (m4)
    वापस m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  अक्षर स्थिर *m4_relocated = relocate (M4);
  काष्ठा stat buf;
  अगर (stat (m4_relocated, &buf) == 0)
    वापस m4_relocated;

  वापस M4;
}

व्योम
output_file_names_free (व्योम)
{
  मुक्त (all_but_ext);
  मुक्त (spec_verbose_file);
  मुक्त (spec_graph_file);
  मुक्त (spec_html_file);
  मुक्त (spec_xml_file);
  मुक्त (spec_header_file);
  मुक्त (parser_file_name);
  मुक्त (dir_prefix);
  क्रम (पूर्णांक i = 0; i < generated_files_आकार; i++)
    मुक्त (generated_files[i].name);
  मुक्त (generated_files);
  मुक्त (relocate_बफर);

  अगर (prefix_maps)
    gl_list_free (prefix_maps);
  अगर (mapped_files)
    gl_map_free (mapped_files);
}
