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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव <configmake.h> /* PKGDATADIR */
#अन्तर्भाव <dirname.h>
#अन्तर्भाव <error.h>
#अन्तर्भाव <get-errno.h>
#अन्तर्भाव <gl_array_list.h>
#अन्तर्भाव <gl_hash_map.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <gl_xmap.h>
#अन्तर्भाव <quote.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <relocatable.h> /* relocate2 */
#अन्तर्भाव <stdio-safer.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <xstrndup.h>

#अन्तर्भाव "complain.h"
#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"
#अन्तर्भाव "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

वर्ण ध्रुव *spec_outfile = रिक्त;       /* for -o. */
वर्ण ध्रुव *spec_file_उपसर्गः = रिक्त;   /* for -b. */
location spec_file_उपसर्गः_loc = रिक्तानि_LOCATION_INIT;
वर्ण ध्रुव *spec_name_उपसर्गः = रिक्त;   /* for -p. */
location spec_name_उपसर्गः_loc = रिक्तानि_LOCATION_INIT;
वर्ण *spec_verbose_file = रिक्त;  /* for --verbose. */
वर्ण *spec_graph_file = रिक्त;    /* for -g. */
वर्ण *spec_html_file = रिक्त;     /* for --html. */
वर्ण *spec_xml_file = रिक्त;      /* for -x. */
वर्ण *spec_शीर्षकम्_file = रिक्त;   /* for --header. */
वर्ण *parser_file_name;

/* All computed output file names.  */
प्रकारनिर्देश संरचना generated_file
{
  /** File name.  */
  वर्ण *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
स्थिर generated_file *generated_files = रिक्त;
स्थिर पूर्णाङ्क generated_files_आकार = 0;

uniqstr grammar_file = रिक्त;

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

वर्ण *all_but_ext;
स्थिर वर्ण *all_but_tab_ext;
वर्ण *dir_उपसर्गः;

/* C source file extension (the parser source).  */
स्थिर वर्ण *src_extension = रिक्त;
/* Header file extension (if option '`-d'' is specified).  */
स्थिर वर्ण *शीर्षकम्_extension = रिक्त;

संरचना उपसर्गः_map
{
  वर्ण *oldprefix;
  वर्ण *newprefix;
};

स्थिर gl_list_t उपसर्गः_maps = रिक्त;

/* Map file names to prefix-mapped file names. */
स्थिर gl_map_t mapped_files = रिक्त;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

स्थिर वर्ण *
concat2 (वर्ण ध्रुव *str1, वर्ण ध्रुव *str2)
{
  size_t len = वाक्यदैर्घ्यम् (str1) + वाक्यदैर्घ्यम् (str2);
  वर्ण *res = xmalloc (len + 1);
  वर्ण *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  प्रत्यागम res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ध्रुव वर्ण *name, ध्रुव वर्ण *बहुलकम्)
{
  FILE *res = fopen_safer (name, बहुलकम्);
  यदि (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  प्रत्यागम res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

शून्य
xfclose (FILE *ptr)
{
  यदि (ptr == रिक्त)
    प्रत्यागम;

  यदि (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  यदि (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (पूर्णाङ्क fd, वर्ण ध्रुव *बहुलकम्)
{
  FILE *res = fdopen (fd, बहुलकम्);
  यदि (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  प्रत्यागम res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
स्थिर वर्ण *
map_file_name_alloc (वर्ण ध्रुव *filename)
{
  संरचना उपसर्गः_map ध्रुव *p = रिक्त;
  प्रतिज्ञा (उपसर्गः_maps);
  {
    शून्य ध्रुव *ptr;
    gl_list_iterator_t iter = gl_list_iterator (उपसर्गः_maps);
    यावत् (gl_list_iterator_अग्र (&iter, &ptr, रिक्त))
      {
        p = ptr;
        यदि (वाक्ये_तुलय (p->oldprefix, filename, वाक्यदैर्घ्यम् (p->oldprefix)) == 0)
          भङ्ग;
        p = रिक्त;
      }
    gl_list_iterator_free (&iter);
  }

  यदि (!p)
    प्रत्यागम xstrdup (filename);

  size_t oldprefix_len = वाक्यदैर्घ्यम् (p->oldprefix);
  size_t newprefix_len = वाक्यदैर्घ्यम् (p->newprefix);
  वर्ण *res = xmalloc (newprefix_len + वाक्यदैर्घ्यम् (filename) - oldprefix_len + 1);

  वर्ण *अन्तः = stpcpy (res, p->newprefix);
  stpcpy (अन्तः, filename + oldprefix_len);

  प्रत्यागम res;
}

स्थिर bool
string_equals (ध्रुव शून्य *x1, ध्रुव शून्य *x2)
{
  ध्रुव वर्ण *s1 = x1;
  ध्रुव वर्ण *s2 = x2;
  प्रत्यागम STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
स्थिर size_t
string_hash (ध्रुव शून्य *x)
{
#परिभाषय आकार_द्वयङ्काः (परिमाण (size_t) * CHAR_BIT)

  ध्रुव वर्ण *s = x;
  size_t h = 0;

  प्रति (; *s; s++)
    h = *s + ((h << 9) | (h >> (आकार_द्वयङ्काः - 9)));

  प्रत्यागम h;
}

स्थिर शून्य
string_free (ध्रुव शून्य *cp)
{
  शून्य *p = (शून्य*) cp;
  मुक्त (p);
}

ध्रुव वर्ण *
map_file_name (वर्ण ध्रुव *filename)
{
  यदि (!filename || !उपसर्गः_maps)
    प्रत्यागम filename;
  यदि (!mapped_files)
    mapped_files
      = gl_map_nx_create_रिक्तानि (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ध्रुव शून्य *res = gl_map_get (mapped_files, filename);
  यदि (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  प्रत्यागम res;
}

स्थिर शून्य
उपसर्गः_map_free (संरचना उपसर्गः_map *p)
{
  मुक्त (p->oldprefix);
  मुक्त (p->newprefix);
  मुक्त (p);
}

शून्य
सङ्कलय_उपसर्गः_map (वर्ण ध्रुव *oldprefix, वर्ण ध्रुव *newprefix)
{
  यदि (!उपसर्गः_maps)
    उपसर्गः_maps
      = gl_list_create_रिक्तानि (GL_ARRAY_LIST,
                              /* equals */ रिक्त,
                              /* hashcode */ रिक्त,
                              (gl_listelement_dispose_fn) उपसर्गः_map_free,
                              true);

  संरचना उपसर्गः_map *p = xmalloc (परिमाण (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_सङ्कलय_चरमः (उपसर्गः_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
स्थिर शून्य
compute_exts_from_gf (ध्रुव वर्ण *ext)
{
  यदि (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      शीर्षकम्_extension = xstrdup (language->शीर्षकम्_extension);
    }
  अन्यथा
    {
      src_extension = xstrdup (ext);
      शीर्षकम्_extension = xstrdup (ext);
      tr (src_extension, 'y', 'c');
      tr (src_extension, 'Y', 'C');
      tr (शीर्षकम्_extension, 'y', 'h');
      tr (शीर्षकम्_extension, 'Y', 'H');
    }
}

/* Compute extensions from the given c source file extension.  */
स्थिर शून्य
compute_exts_from_src (ध्रुव वर्ण *ext)
{
  /* We use this function when the user specifies `-o' or `--output',
     so the extensions must be computed unconditionally from the file name
     given by this option.  */
  src_extension = xstrdup (ext);
  शीर्षकम्_extension = xstrdup (ext);
  tr (शीर्षकम्_extension, 'c', 'h');
  tr (शीर्षकम्_extension, 'C', 'H');
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

स्थिर शून्य
file_name_विभज (ध्रुव वर्ण *file_name,
                 ध्रुव वर्ण **आधारः, ध्रुव वर्ण **tab, ध्रुव वर्ण **ext)
{
  *आधारः = चरमः_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*आधारः, '.');
  *tab = रिक्त;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  यदि (*ext)
    {
      size_t baselen = *ext - *आधारः;
      size_t dottablen = परिमाण (TAB_EXT) - 1;
      यदि (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

स्थिर शून्य
compute_file_name_parts (शून्य)
{
  यदि (spec_outfile)
    {
      ध्रुव वर्ण *आधारः, *tab, *ext;
      file_name_विभज (spec_outfile, &आधारः, &tab, &ext);
      dir_उपसर्गः = xstrndup (spec_outfile, आधारः - spec_outfile);

      /* ALL_BUT_EXT goes up the EXT, excluding it. */
      all_but_ext =
        xstrndup (spec_outfile,
                  (वाक्यदैर्घ्यम् (spec_outfile) - (ext ? वाक्यदैर्घ्यम् (ext) : 0)));

      /* ALL_BUT_TAB_EXT goes up to TAB, excluding it.  */
      all_but_tab_ext =
        xstrndup (spec_outfile,
                  (वाक्यदैर्घ्यम् (spec_outfile)
                   - (tab ? वाक्यदैर्घ्यम् (tab) : (ext ? वाक्यदैर्घ्यम् (ext) : 0))));

      यदि (ext)
        compute_exts_from_src (ext);
    }
  अन्यथा
    {
      ध्रुव वर्ण *आधारः, *tab, *ext;
      file_name_विभज (grammar_file, &आधारः, &tab, &ext);

      यदि (spec_file_उपसर्गः)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_उपसर्गः =
            xstrndup (spec_file_उपसर्गः,
                      चरमः_component (spec_file_उपसर्गः) - spec_file_उपसर्गः);
          all_but_tab_ext = xstrdup (spec_file_उपसर्गः);
        }
      अन्यथा यदि (! location_रिक्तानि (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_उपसर्गः = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      अन्यथा
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_उपसर्गः = xstrdup ("");
          all_but_tab_ext =
            xstrndup (आधारः, (वाक्यदैर्घ्यम् (आधारः) - (ext ? वाक्यदैर्घ्यम् (ext) : 0)));
        }

      यदि (language->सङ्कलय_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      अन्यथा
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      यदि (ext && location_रिक्तानि (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

शून्य
compute_output_file_नामानि (शून्य)
{
  compute_file_name_parts ();

  /* If not yet done. */
  यदि (!src_extension)
    src_extension = xstrdup (".c");
  यदि (!शीर्षकम्_extension)
    शीर्षकम्_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  यदि (शीर्षकम्_flag)
    {
      यदि (! spec_शीर्षकम्_file)
        spec_शीर्षकम्_file = concat2 (all_but_ext, शीर्षकम्_extension);
    }

  यदि (graph_flag)
    {
      यदि (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  यदि (html_flag)
    {
      यदि (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  यदि (xml_flag)
    {
      यदि (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  यदि (report_flag)
    {
      यदि (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  मुक्त (all_but_tab_ext);
  मुक्त (src_extension);
  मुक्त (शीर्षकम्_extension);
}

शून्य
output_file_name_check (वर्ण **file_name, bool source)
{
  bool conflict = false;
  यदि (STREQ (*file_name, grammar_file))
    {
      complain (रिक्त, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  अन्यथा
    प्रति (पूर्णाङ्क i = 0; i < generated_files_आकार; i++)
      यदि (STREQ (generated_files[i].name, *file_name))
        {
          complain (रिक्त, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  यदि (conflict)
    {
      मुक्त (*file_name);
      *file_name = strdup ("/dev/null");
    }
  अन्यथा
    {
      generated_files = xnrealloc (generated_files, ++generated_files_आकार,
                                   परिमाण *generated_files);
      generated_files[generated_files_आकार-1].name = xstrdup (*file_name);
      generated_files[generated_files_आकार-1].is_source = source;
    }
}

शून्य
unlink_generated_sources (शून्य)
{
  प्रति (पूर्णाङ्क i = 0; i < generated_files_आकार; i++)
    यदि (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
स्थिर वर्ण *relocate_बफर = रिक्त;

वर्ण ध्रुव *
pkgdatadir (शून्य)
{
  यदि (relocate_बफर)
    प्रत्यागम relocate_बफर;
  अन्यथा
    {
      वर्ण ध्रुव *cp = getenv ("BISON_PKGDATADIR");
      प्रत्यागम cp ? cp : relocate2 (PKGDATADIR, &relocate_बफर);
    }
}

वर्ण ध्रुव *
m4path (शून्य)
{
  वर्ण ध्रुव *m4 = getenv ("M4");
  यदि (m4)
    प्रत्यागम m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  वर्ण ध्रुव *m4_relocated = relocate (M4);
  संरचना stat buf;
  यदि (stat (m4_relocated, &buf) == 0)
    प्रत्यागम m4_relocated;

  प्रत्यागम M4;
}

शून्य
output_file_नामानि_free (शून्य)
{
  मुक्त (all_but_ext);
  मुक्त (spec_verbose_file);
  मुक्त (spec_graph_file);
  मुक्त (spec_html_file);
  मुक्त (spec_xml_file);
  मुक्त (spec_शीर्षकम्_file);
  मुक्त (parser_file_name);
  मुक्त (dir_उपसर्गः);
  प्रति (पूर्णाङ्क i = 0; i < generated_files_आकार; i++)
    मुक्त (generated_files[i].name);
  मुक्त (generated_files);
  मुक्त (relocate_बफर);

  यदि (उपसर्गः_maps)
    gl_list_free (उपसर्गः_maps);
  यदि (mapped_files)
    gl_map_free (mapped_files);
}
