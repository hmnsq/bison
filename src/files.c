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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <configmake.h> /* PKGDATADIR */
#ಅನ್ತರ್ಭಾವ <dirname.h>
#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <get-errno.h>
#ಅನ್ತರ್ಭಾವ <gl_array_list.h>
#ಅನ್ತರ್ಭಾವ <gl_hash_map.h>
#ಅನ್ತರ್ಭಾವ <gl_xlist.h>
#ಅನ್ತರ್ಭಾವ <gl_xmap.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <relocatable.h> /* relocate2 */
#ಅನ್ತರ್ಭಾವ <stdio-safer.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <xstrndup.h>

#ಅನ್ತರ್ಭಾವ "complain.h"
#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"
#ಅನ್ತರ್ಭಾವ "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

ವರ್ಣ ಧ್ರುವ *spec_outfile = ರಿಕ್ತ;       /* for -o. */
ವರ್ಣ ಧ್ರುವ *spec_file_prefix = ರಿಕ್ತ;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
ವರ್ಣ ಧ್ರುವ *spec_name_prefix = ರಿಕ್ತ;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
ವರ್ಣ *spec_verbose_file = ರಿಕ್ತ;  /* for --verbose. */
ವರ್ಣ *spec_graph_file = ರಿಕ್ತ;    /* for -g. */
ವರ್ಣ *spec_html_file = ರಿಕ್ತ;     /* for --html. */
ವರ್ಣ *spec_xml_file = ರಿಕ್ತ;      /* for -x. */
ವರ್ಣ *spec_header_file = ರಿಕ್ತ;   /* for --header. */
ವರ್ಣ *parser_file_name;

/* All computed output file names.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ generated_file
{
  /** File name.  */
  ವರ್ಣ *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
ಸ್ಥಿರ generated_file *generated_files = ರಿಕ್ತ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ generated_files_size = 0;

uniqstr grammar_file = ರಿಕ್ತ;

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

ವರ್ಣ *all_but_ext;
ಸ್ಥಿರ ವರ್ಣ *all_but_tab_ext;
ವರ್ಣ *dir_prefix;

/* C source file extension (the parser source).  */
ಸ್ಥಿರ ವರ್ಣ *src_extension = ರಿಕ್ತ;
/* Header file extension (if option '`-d'' is specified).  */
ಸ್ಥಿರ ವರ್ಣ *header_extension = ರಿಕ್ತ;

ಸಂರಚನಾ prefix_map
{
  ವರ್ಣ *oldprefix;
  ವರ್ಣ *newprefix;
};

ಸ್ಥಿರ gl_list_t prefix_maps = ರಿಕ್ತ;

/* Map file names to prefix-mapped file names. */
ಸ್ಥಿರ gl_map_t mapped_files = ರಿಕ್ತ;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ ವರ್ಣ *
concat2 (ವರ್ಣ ಧ್ರುವ *str1, ವರ್ಣ ಧ್ರುವ *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  ವರ್ಣ *res = xmalloc (len + 1);
  ವರ್ಣ *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  ಪ್ರತ್ಯಾಗಮ res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *mode)
{
  FILE *res = fopen_safer (name, mode);
  ಯದಿ (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  ಪ್ರತ್ಯಾಗಮ res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

ಶೂನ್ಯ
xfclose (FILE *ptr)
{
  ಯದಿ (ptr == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  ಯದಿ (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *mode)
{
  FILE *res = fdopen (fd, mode);
  ಯದಿ (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  ಪ್ರತ್ಯಾಗಮ res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
ಸ್ಥಿರ ವರ್ಣ *
map_file_name_alloc (ವರ್ಣ ಧ್ರುವ *filename)
{
  ಸಂರಚನಾ prefix_map ಧ್ರುವ *p = ರಿಕ್ತ;
  ಪ್ರತಿಜ್ಞಾ (prefix_maps);
  {
    ಶೂನ್ಯ ಧ್ರುವ *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    ಯಾವತ್ (gl_list_iterator_next (&iter, &ptr, ರಿಕ್ತ))
      {
        p = ptr;
        ಯದಿ (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          ಭಙ್ಗ;
        p = ರಿಕ್ತ;
      }
    gl_list_iterator_free (&iter);
  }

  ಯದಿ (!p)
    ಪ್ರತ್ಯಾಗಮ xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  ವರ್ಣ *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  ವರ್ಣ *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ bool
string_equals (ಧ್ರುವ ಶೂನ್ಯ *x1, ಧ್ರುವ ಶೂನ್ಯ *x2)
{
  ಧ್ರುವ ವರ್ಣ *s1 = x1;
  ಧ್ರುವ ವರ್ಣ *s2 = x2;
  ಪ್ರತ್ಯಾಗಮ STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
ಸ್ಥಿರ size_t
string_hash (ಧ್ರುವ ಶೂನ್ಯ *x)
{
#define SIZE_BITS (ಪರಿಮಾಣ (size_t) * CHAR_BIT)

  ಧ್ರುವ ವರ್ಣ *s = x;
  size_t h = 0;

  ಪ್ರತಿ (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  ಪ್ರತ್ಯಾಗಮ h;
}

ಸ್ಥಿರ ಶೂನ್ಯ
string_free (ಧ್ರುವ ಶೂನ್ಯ *cp)
{
  ಶೂನ್ಯ *p = (ಶೂನ್ಯ*) cp;
  ಮುಕ್ತ (p);
}

ಧ್ರುವ ವರ್ಣ *
map_file_name (ವರ್ಣ ಧ್ರುವ *filename)
{
  ಯದಿ (!filename || !prefix_maps)
    ಪ್ರತ್ಯಾಗಮ filename;
  ಯದಿ (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ಧ್ರುವ ಶೂನ್ಯ *res = gl_map_get (mapped_files, filename);
  ಯದಿ (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
prefix_map_free (ಸಂರಚನಾ prefix_map *p)
{
  ಮುಕ್ತ (p->oldprefix);
  ಮುಕ್ತ (p->newprefix);
  ಮುಕ್ತ (p);
}

ಶೂನ್ಯ
add_prefix_map (ವರ್ಣ ಧ್ರುವ *oldprefix, ವರ್ಣ ಧ್ರುವ *newprefix)
{
  ಯದಿ (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ ರಿಕ್ತ,
                              /* hashcode */ ರಿಕ್ತ,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  ಸಂರಚನಾ prefix_map *p = xmalloc (ಪರಿಮಾಣ (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
ಸ್ಥಿರ ಶೂನ್ಯ
compute_exts_from_gf (ಧ್ರುವ ವರ್ಣ *ext)
{
  ಯದಿ (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  ಅನ್ಯಥಾ
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
ಸ್ಥಿರ ಶೂನ್ಯ
compute_exts_from_src (ಧ್ರುವ ವರ್ಣ *ext)
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

ಸ್ಥಿರ ಶೂನ್ಯ
file_name_split (ಧ್ರುವ ವರ್ಣ *file_name,
                 ಧ್ರುವ ವರ್ಣ **base, ಧ್ರುವ ವರ್ಣ **tab, ಧ್ರುವ ವರ್ಣ **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = ರಿಕ್ತ;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  ಯದಿ (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = ಪರಿಮಾಣ (TAB_EXT) - 1;
      ಯದಿ (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

ಸ್ಥಿರ ಶೂನ್ಯ
compute_file_name_parts (ಶೂನ್ಯ)
{
  ಯದಿ (spec_outfile)
    {
      ಧ್ರುವ ವರ್ಣ *base, *tab, *ext;
      file_name_split (spec_outfile, &base, &tab, &ext);
      dir_prefix = xstrndup (spec_outfile, base - spec_outfile);

      /* ALL_BUT_EXT goes up the EXT, excluding it. */
      all_but_ext =
        xstrndup (spec_outfile,
                  (strlen (spec_outfile) - (ext ? strlen (ext) : 0)));

      /* ALL_BUT_TAB_EXT goes up to TAB, excluding it.  */
      all_but_tab_ext =
        xstrndup (spec_outfile,
                  (strlen (spec_outfile)
                   - (tab ? strlen (tab) : (ext ? strlen (ext) : 0))));

      ಯದಿ (ext)
        compute_exts_from_src (ext);
    }
  ಅನ್ಯಥಾ
    {
      ಧ್ರುವ ವರ್ಣ *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      ಯದಿ (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      ಅನ್ಯಥಾ ಯದಿ (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      ಅನ್ಯಥಾ
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      ಯದಿ (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      ಅನ್ಯಥಾ
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      ಯದಿ (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

ಶೂನ್ಯ
compute_output_file_names (ಶೂನ್ಯ)
{
  compute_file_name_parts ();

  /* If not yet done. */
  ಯದಿ (!src_extension)
    src_extension = xstrdup (".c");
  ಯದಿ (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  ಯದಿ (header_flag)
    {
      ಯದಿ (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  ಯದಿ (graph_flag)
    {
      ಯದಿ (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  ಯದಿ (html_flag)
    {
      ಯದಿ (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  ಯದಿ (xml_flag)
    {
      ಯದಿ (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  ಯದಿ (report_flag)
    {
      ಯದಿ (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  ಮುಕ್ತ (all_but_tab_ext);
  ಮುಕ್ತ (src_extension);
  ಮುಕ್ತ (header_extension);
}

ಶೂನ್ಯ
output_file_name_check (ವರ್ಣ **file_name, bool source)
{
  bool conflict = false;
  ಯದಿ (STREQ (*file_name, grammar_file))
    {
      complain (ರಿಕ್ತ, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < generated_files_size; i++)
      ಯದಿ (STREQ (generated_files[i].name, *file_name))
        {
          complain (ರಿಕ್ತ, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  ಯದಿ (conflict)
    {
      ಮುಕ್ತ (*file_name);
      *file_name = strdup ("/dev/null");
    }
  ಅನ್ಯಥಾ
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   ಪರಿಮಾಣ *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

ಶೂನ್ಯ
unlink_generated_sources (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < generated_files_size; i++)
    ಯದಿ (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
ಸ್ಥಿರ ವರ್ಣ *relocate_buffer = ರಿಕ್ತ;

ವರ್ಣ ಧ್ರುವ *
pkgdatadir (ಶೂನ್ಯ)
{
  ಯದಿ (relocate_buffer)
    ಪ್ರತ್ಯಾಗಮ relocate_buffer;
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ ಧ್ರುವ *cp = getenv ("BISON_PKGDATADIR");
      ಪ್ರತ್ಯಾಗಮ cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

ವರ್ಣ ಧ್ರುವ *
m4path (ಶೂನ್ಯ)
{
  ವರ್ಣ ಧ್ರುವ *m4 = getenv ("M4");
  ಯದಿ (m4)
    ಪ್ರತ್ಯಾಗಮ m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  ವರ್ಣ ಧ್ರುವ *m4_relocated = relocate (M4);
  ಸಂರಚನಾ stat buf;
  ಯದಿ (stat (m4_relocated, &buf) == 0)
    ಪ್ರತ್ಯಾಗಮ m4_relocated;

  ಪ್ರತ್ಯಾಗಮ M4;
}

ಶೂನ್ಯ
output_file_names_free (ಶೂನ್ಯ)
{
  ಮುಕ್ತ (all_but_ext);
  ಮುಕ್ತ (spec_verbose_file);
  ಮುಕ್ತ (spec_graph_file);
  ಮುಕ್ತ (spec_html_file);
  ಮುಕ್ತ (spec_xml_file);
  ಮುಕ್ತ (spec_header_file);
  ಮುಕ್ತ (parser_file_name);
  ಮುಕ್ತ (dir_prefix);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < generated_files_size; i++)
    ಮುಕ್ತ (generated_files[i].name);
  ಮುಕ್ತ (generated_files);
  ಮುಕ್ತ (relocate_buffer);

  ಯದಿ (prefix_maps)
    gl_list_free (prefix_maps);
  ಯದಿ (mapped_files)
    gl_map_free (mapped_files);
}
