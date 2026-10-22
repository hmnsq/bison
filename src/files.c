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

#شامل <config.h>
#شامل "system.h"

#شامل <configmake.h> /* PKGDATADIR */
#شامل <dirname.h>
#شامل <error.h>
#شامل <get-errno.h>
#شامل <gl_array_list.h>
#شامل <gl_hash_map.h>
#شامل <gl_xlist.h>
#شامل <gl_xmap.h>
#شامل <quote.h>
#شامل <quotearg.h>
#شامل <relocatable.h> /* relocate2 */
#شامل <stdio-safer.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <unistd.h>
#شامل <xstrndup.h>

#شامل "complain.h"
#شامل "files.h"
#شامل "getargs.h"
#شامل "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

نویسه ثابت *spec_outfile = پوچ;       /* for -o. */
نویسه ثابت *spec_file_prefix = پوچ;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
نویسه ثابت *spec_name_prefix = پوچ;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
نویسه *spec_verbose_file = پوچ;  /* for --verbose. */
نویسه *spec_graph_file = پوچ;    /* for -g. */
نویسه *spec_html_file = پوچ;     /* for --html. */
نویسه *spec_xml_file = پوچ;      /* for -x. */
نویسه *spec_header_file = پوچ;   /* for --header. */
نویسه *parser_file_name;

/* All computed output file names.  */
تعریفنوع ساختار generated_file
{
  /** File name.  */
  نویسه *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
ایستا generated_file *generated_files = پوچ;
ایستا صحیح generated_files_size = 0;

uniqstr grammar_file = پوچ;

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

نویسه *all_but_ext;
ایستا نویسه *all_but_tab_ext;
نویسه *dir_prefix;

/* C source file extension (the parser source).  */
ایستا نویسه *src_extension = پوچ;
/* Header file extension (if option '`-d'' is specified).  */
ایستا نویسه *header_extension = پوچ;

ساختار prefix_map
{
  نویسه *oldprefix;
  نویسه *newprefix;
};

ایستا gl_list_t prefix_maps = پوچ;

/* Map file names to prefix-mapped file names. */
ایستا gl_map_t mapped_files = پوچ;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

ایستا نویسه *
concat2 (نویسه ثابت *str1, نویسه ثابت *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  نویسه *res = xmalloc (len + 1);
  نویسه *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  بازگشت res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ثابت نویسه *name, ثابت نویسه *mode)
{
  FILE *res = fopen_safer (name, mode);
  اگر (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  بازگشت res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

تهی
xfclose (FILE *ptr)
{
  اگر (ptr == پوچ)
    بازگشت;

  اگر (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  اگر (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (صحیح fd, نویسه ثابت *mode)
{
  FILE *res = fdopen (fd, mode);
  اگر (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  بازگشت res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
ایستا نویسه *
map_file_name_alloc (نویسه ثابت *filename)
{
  ساختار prefix_map ثابت *p = پوچ;
  أكد (prefix_maps);
  {
    تهی ثابت *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    وقتی (gl_list_iterator_next (&iter, &ptr, پوچ))
      {
        p = ptr;
        اگر (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          بشکن;
        p = پوچ;
      }
    gl_list_iterator_free (&iter);
  }

  اگر (!p)
    بازگشت xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  نویسه *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  نویسه *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  بازگشت res;
}

ایستا bool
string_equals (ثابت تهی *x1, ثابت تهی *x2)
{
  ثابت نویسه *s1 = x1;
  ثابت نویسه *s2 = x2;
  بازگشت STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
ایستا size_t
string_hash (ثابت تهی *x)
{
#define SIZE_BITS (اندازه (size_t) * CHAR_BIT)

  ثابت نویسه *s = x;
  size_t h = 0;

  برای (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  بازگشت h;
}

ایستا تهی
string_free (ثابت تهی *cp)
{
  تهی *p = (تهی*) cp;
  آزاد (p);
}

ثابت نویسه *
map_file_name (نویسه ثابت *filename)
{
  اگر (!filename || !prefix_maps)
    بازگشت filename;
  اگر (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ثابت تهی *res = gl_map_get (mapped_files, filename);
  اگر (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  بازگشت res;
}

ایستا تهی
prefix_map_free (ساختار prefix_map *p)
{
  آزاد (p->oldprefix);
  آزاد (p->newprefix);
  آزاد (p);
}

تهی
add_prefix_map (نویسه ثابت *oldprefix, نویسه ثابت *newprefix)
{
  اگر (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ پوچ,
                              /* hashcode */ پوچ,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  ساختار prefix_map *p = xmalloc (اندازه (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
ایستا تهی
compute_exts_from_gf (ثابت نویسه *ext)
{
  اگر (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  وگرنه
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
ایستا تهی
compute_exts_from_src (ثابت نویسه *ext)
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

ایستا تهی
file_name_split (ثابت نویسه *file_name,
                 ثابت نویسه **base, ثابت نویسه **tab, ثابت نویسه **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = پوچ;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  اگر (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = اندازه (TAB_EXT) - 1;
      اگر (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

ایستا تهی
compute_file_name_parts (تهی)
{
  اگر (spec_outfile)
    {
      ثابت نویسه *base, *tab, *ext;
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

      اگر (ext)
        compute_exts_from_src (ext);
    }
  وگرنه
    {
      ثابت نویسه *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      اگر (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      وگرنه اگر (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      وگرنه
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      اگر (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      وگرنه
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      اگر (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

تهی
compute_output_file_names (تهی)
{
  compute_file_name_parts ();

  /* If not yet done. */
  اگر (!src_extension)
    src_extension = xstrdup (".c");
  اگر (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  اگر (header_flag)
    {
      اگر (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  اگر (graph_flag)
    {
      اگر (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  اگر (html_flag)
    {
      اگر (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  اگر (xml_flag)
    {
      اگر (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  اگر (report_flag)
    {
      اگر (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  آزاد (all_but_tab_ext);
  آزاد (src_extension);
  آزاد (header_extension);
}

تهی
output_file_name_check (نویسه **file_name, bool source)
{
  bool conflict = false;
  اگر (STREQ (*file_name, grammar_file))
    {
      complain (پوچ, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  وگرنه
    برای (صحیح i = 0; i < generated_files_size; i++)
      اگر (STREQ (generated_files[i].name, *file_name))
        {
          complain (پوچ, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  اگر (conflict)
    {
      آزاد (*file_name);
      *file_name = strdup ("/dev/null");
    }
  وگرنه
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   اندازه *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

تهی
unlink_generated_sources (تهی)
{
  برای (صحیح i = 0; i < generated_files_size; i++)
    اگر (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
ایستا نویسه *relocate_buffer = پوچ;

نویسه ثابت *
pkgdatadir (تهی)
{
  اگر (relocate_buffer)
    بازگشت relocate_buffer;
  وگرنه
    {
      نویسه ثابت *cp = getenv ("BISON_PKGDATADIR");
      بازگشت cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

نویسه ثابت *
m4path (تهی)
{
  نویسه ثابت *m4 = getenv ("M4");
  اگر (m4)
    بازگشت m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  نویسه ثابت *m4_relocated = relocate (M4);
  ساختار stat buf;
  اگر (stat (m4_relocated, &buf) == 0)
    بازگشت m4_relocated;

  بازگشت M4;
}

تهی
output_file_names_free (تهی)
{
  آزاد (all_but_ext);
  آزاد (spec_verbose_file);
  آزاد (spec_graph_file);
  آزاد (spec_html_file);
  آزاد (spec_xml_file);
  آزاد (spec_header_file);
  آزاد (parser_file_name);
  آزاد (dir_prefix);
  برای (صحیح i = 0; i < generated_files_size; i++)
    آزاد (generated_files[i].name);
  آزاد (generated_files);
  آزاد (relocate_buffer);

  اگر (prefix_maps)
    gl_list_free (prefix_maps);
  اگر (mapped_files)
    gl_map_free (mapped_files);
}
