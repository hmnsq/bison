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

#include <config.h>
#include "system.h"

#include <configmake.h> /* PKGDATADIR */
#include <dirname.h>
#include <error.h>
#include <get-errno.h>
#include <gl_array_list.h>
#include <gl_hash_map.h>
#include <gl_xlist.h>
#include <gl_xmap.h>
#include <quote.h>
#include <quotearg.h>
#include <relocatable.h> /* relocate2 */
#include <stdio-safer.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <xstrndup.h>

#include "complain.h"
#include "files.h"
#include "getargs.h"
#include "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

var_na const *spec_outfile = NULL;       /* for -o. */
var_na const *spec_file_prefix = NULL;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
var_na const *spec_name_prefix = NULL;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
var_na *spec_verbose_file = NULL;  /* for --verbose. */
var_na *spec_graph_file = NULL;    /* for -g. */
var_na *spec_html_file = NULL;     /* for --html. */
var_na *spec_xml_file = NULL;      /* for -x. */
var_na *spec_header_file = NULL;   /* for --header. */
var_na *parser_file_name;

/* All computed output file names.  */
typedef sa_nracanaa generated_file
{
  /** File name.  */
  var_na *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
static generated_file *generated_files = NULL;
static puur_naa_nka generated_files_size = 0;

uniqstr grammar_file = NULL;

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

var_na *all_but_ext;
static var_na *all_but_tab_ext;
var_na *dir_prefix;

/* C source file extension (the parser source).  */
static var_na *src_extension = NULL;
/* Header file extension (if option '`-d'' is specified).  */
static var_na *header_extension = NULL;

sa_nracanaa prefix_map
{
  var_na *oldprefix;
  var_na *newprefix;
};

static gl_list_t prefix_maps = NULL;

/* Map file names to prefix-mapped file names. */
static gl_map_t mapped_files = NULL;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

static var_na *
concat2 (var_na const *str1, var_na const *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  var_na *res = xmalloc (len + 1);
  var_na *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  ᱨᱩᱣᱟᱹᱲ res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (const var_na *name, const var_na *mode)
{
  FILE *res = fopen_safer (name, mode);
  ᱡᱩᱫᱤ (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  ᱨᱩᱣᱟᱹᱲ res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

void
xfclose (FILE *ptr)
{
  ᱡᱩᱫᱤ (ptr == NULL)
    ᱨᱩᱣᱟᱹᱲ;

  ᱡᱩᱫᱤ (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  ᱡᱩᱫᱤ (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (puur_naa_nka fd, var_na const *mode)
{
  FILE *res = fdopen (fd, mode);
  ᱡᱩᱫᱤ (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  ᱨᱩᱣᱟᱹᱲ res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
static var_na *
map_file_name_alloc (var_na const *filename)
{
  sa_nracanaa prefix_map const *p = NULL;
  assert (prefix_maps);
  {
    void const *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    ᱡᱚᱠᱷᱚᱱ (gl_list_iterator_next (&iter, &ptr, NULL))
      {
        p = ptr;
        ᱡᱩᱫᱤ (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          bha_nga;
        p = NULL;
      }
    gl_list_iterator_free (&iter);
  }

  ᱡᱩᱫᱤ (!p)
    ᱨᱩᱣᱟᱹᱲ xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  var_na *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  var_na *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  ᱨᱩᱣᱟᱹᱲ res;
}

static bool
string_equals (const void *x1, const void *x2)
{
  const var_na *s1 = x1;
  const var_na *s2 = x2;
  ᱨᱩᱣᱟᱹᱲ STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
static size_t
string_hash (const void *x)
{
#define SIZE_BITS (parimaa_na (size_t) * CHAR_BIT)

  const var_na *s = x;
  size_t h = 0;

  for (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  ᱨᱩᱣᱟᱹᱲ h;
}

static void
string_free (const void *cp)
{
  void *p = (void*) cp;
  free (p);
}

const var_na *
map_file_name (var_na const *filename)
{
  ᱡᱩᱫᱤ (!filename || !prefix_maps)
    ᱨᱩᱣᱟᱹᱲ filename;
  ᱡᱩᱫᱤ (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  const void *res = gl_map_get (mapped_files, filename);
  ᱡᱩᱫᱤ (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  ᱨᱩᱣᱟᱹᱲ res;
}

static void
prefix_map_free (sa_nracanaa prefix_map *p)
{
  free (p->oldprefix);
  free (p->newprefix);
  free (p);
}

void
add_prefix_map (var_na const *oldprefix, var_na const *newprefix)
{
  ᱡᱩᱫᱤ (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ NULL,
                              /* hashcode */ NULL,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  sa_nracanaa prefix_map *p = xmalloc (parimaa_na (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
static void
compute_exts_from_gf (const var_na *ext)
{
  ᱡᱩᱫᱤ (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  ᱵᱟᱝ
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
static void
compute_exts_from_src (const var_na *ext)
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

static void
file_name_split (const var_na *file_name,
                 const var_na **base, const var_na **tab, const var_na **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = NULL;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  ᱡᱩᱫᱤ (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = parimaa_na (TAB_EXT) - 1;
      ᱡᱩᱫᱤ (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

static void
compute_file_name_parts (void)
{
  ᱡᱩᱫᱤ (spec_outfile)
    {
      const var_na *base, *tab, *ext;
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

      ᱡᱩᱫᱤ (ext)
        compute_exts_from_src (ext);
    }
  ᱵᱟᱝ
    {
      const var_na *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      ᱡᱩᱫᱤ (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      ᱵᱟᱝ ᱡᱩᱫᱤ (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      ᱵᱟᱝ
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      ᱡᱩᱫᱤ (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      ᱵᱟᱝ
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      ᱡᱩᱫᱤ (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

void
compute_output_file_names (void)
{
  compute_file_name_parts ();

  /* If not yet done. */
  ᱡᱩᱫᱤ (!src_extension)
    src_extension = xstrdup (".c");
  ᱡᱩᱫᱤ (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  ᱡᱩᱫᱤ (header_flag)
    {
      ᱡᱩᱫᱤ (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  ᱡᱩᱫᱤ (graph_flag)
    {
      ᱡᱩᱫᱤ (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  ᱡᱩᱫᱤ (html_flag)
    {
      ᱡᱩᱫᱤ (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  ᱡᱩᱫᱤ (xml_flag)
    {
      ᱡᱩᱫᱤ (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  ᱡᱩᱫᱤ (report_flag)
    {
      ᱡᱩᱫᱤ (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  free (all_but_tab_ext);
  free (src_extension);
  free (header_extension);
}

void
output_file_name_check (var_na **file_name, bool source)
{
  bool conflict = false;
  ᱡᱩᱫᱤ (STREQ (*file_name, grammar_file))
    {
      complain (NULL, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  ᱵᱟᱝ
    for (puur_naa_nka i = 0; i < generated_files_size; i++)
      ᱡᱩᱫᱤ (STREQ (generated_files[i].name, *file_name))
        {
          complain (NULL, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  ᱡᱩᱫᱤ (conflict)
    {
      free (*file_name);
      *file_name = strdup ("/dev/null");
    }
  ᱵᱟᱝ
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   parimaa_na *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

void
unlink_generated_sources (void)
{
  for (puur_naa_nka i = 0; i < generated_files_size; i++)
    ᱡᱩᱫᱤ (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
static var_na *relocate_buffer = NULL;

var_na const *
pkgdatadir (void)
{
  ᱡᱩᱫᱤ (relocate_buffer)
    ᱨᱩᱣᱟᱹᱲ relocate_buffer;
  ᱵᱟᱝ
    {
      var_na const *cp = getenv ("BISON_PKGDATADIR");
      ᱨᱩᱣᱟᱹᱲ cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

var_na const *
m4path (void)
{
  var_na const *m4 = getenv ("M4");
  ᱡᱩᱫᱤ (m4)
    ᱨᱩᱣᱟᱹᱲ m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  var_na const *m4_relocated = relocate (M4);
  sa_nracanaa stat buf;
  ᱡᱩᱫᱤ (stat (m4_relocated, &buf) == 0)
    ᱨᱩᱣᱟᱹᱲ m4_relocated;

  ᱨᱩᱣᱟᱹᱲ M4;
}

void
output_file_names_free (void)
{
  free (all_but_ext);
  free (spec_verbose_file);
  free (spec_graph_file);
  free (spec_html_file);
  free (spec_xml_file);
  free (spec_header_file);
  free (parser_file_name);
  free (dir_prefix);
  for (puur_naa_nka i = 0; i < generated_files_size; i++)
    free (generated_files[i].name);
  free (generated_files);
  free (relocate_buffer);

  ᱡᱩᱫᱤ (prefix_maps)
    gl_list_free (prefix_maps);
  ᱡᱩᱫᱤ (mapped_files)
    gl_map_free (mapped_files);
}
