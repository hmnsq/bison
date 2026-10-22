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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <configmake.h> /* PKGDATADIR */
#অন্তর্ভাব <dirname.h>
#অন্তর্ভাব <error.h>
#অন্তর্ভাব <get-errno.h>
#অন্তর্ভাব <gl_array_list.h>
#অন্তর্ভাব <gl_hash_map.h>
#অন্তর্ভাব <gl_xlist.h>
#অন্তর্ভাব <gl_xmap.h>
#অন্তর্ভাব <quote.h>
#অন্তর্ভাব <quotearg.h>
#অন্তর্ভাব <relocatable.h> /* relocate2 */
#অন্তর্ভাব <stdio-safer.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <xstrndup.h>

#অন্তর্ভাব "complain.h"
#অন্তর্ভাব "files.h"
#অন্তর্ভাব "getargs.h"
#অন্তর্ভাব "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

অক্ষর ধ্রুবক *spec_outfile = ফাঁকা;       /* for -o. */
অক্ষর ধ্রুবক *spec_file_prefix = ফাঁকা;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
অক্ষর ধ্রুবক *spec_name_prefix = ফাঁকা;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
অক্ষর *spec_verbose_file = ফাঁকা;  /* for --verbose. */
অক্ষর *spec_graph_file = ফাঁকা;    /* for -g. */
অক্ষর *spec_html_file = ফাঁকা;     /* for --html. */
অক্ষর *spec_xml_file = ফাঁকা;      /* for -x. */
অক্ষর *spec_header_file = ফাঁকা;   /* for --header. */
অক্ষর *parser_file_name;

/* All computed output file names.  */
প্রকারলেখ কাঠামো generated_file
{
  /** File name.  */
  অক্ষর *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
স্থির generated_file *generated_files = ফাঁকা;
স্থির পূর্ণসংখ্যা generated_files_আকার = 0;

uniqstr grammar_file = ফাঁকা;

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

অক্ষর *all_but_ext;
স্থির অক্ষর *all_but_tab_ext;
অক্ষর *dir_prefix;

/* C source file extension (the parser source).  */
স্থির অক্ষর *src_extension = ফাঁকা;
/* Header file extension (if option '`-d'' is specified).  */
স্থির অক্ষর *header_extension = ফাঁকা;

কাঠামো prefix_map
{
  অক্ষর *oldprefix;
  অক্ষর *newprefix;
};

স্থির gl_list_t prefix_maps = ফাঁকা;

/* Map file names to prefix-mapped file names. */
স্থির gl_map_t mapped_files = ফাঁকা;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

স্থির অক্ষর *
concat2 (অক্ষর ধ্রুবক *str1, অক্ষর ধ্রুবক *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  অক্ষর *res = xmalloc (len + 1);
  অক্ষর *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  ফেরত res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *mode)
{
  FILE *res = fopen_safer (name, mode);
  যদি (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  ফেরত res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

শূন্য
xfclose (FILE *ptr)
{
  যদি (ptr == ফাঁকা)
    ফেরত;

  যদি (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  যদি (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *mode)
{
  FILE *res = fdopen (fd, mode);
  যদি (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  ফেরত res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
স্থির অক্ষর *
map_file_name_alloc (অক্ষর ধ্রুবক *filename)
{
  কাঠামো prefix_map ধ্রুবক *p = ফাঁকা;
  প্রতিজ্ঞা (prefix_maps);
  {
    শূন্য ধ্রুবক *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    যখন (gl_list_iterator_পরবর্তী (&iter, &ptr, ফাঁকা))
      {
        p = ptr;
        যদি (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          বিরতি;
        p = ফাঁকা;
      }
    gl_list_iterator_free (&iter);
  }

  যদি (!p)
    ফেরত xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  অক্ষর *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  অক্ষর *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  ফেরত res;
}

স্থির bool
string_equals (ধ্রুবক শূন্য *x1, ধ্রুবক শূন্য *x2)
{
  ধ্রুবক অক্ষর *s1 = x1;
  ধ্রুবক অক্ষর *s2 = x2;
  ফেরত STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
স্থির size_t
string_hash (ধ্রুবক শূন্য *x)
{
#define আকার_BITS (আকার (size_t) * CHAR_BIT)

  ধ্রুবক অক্ষর *s = x;
  size_t h = 0;

  জন্য (; *s; s++)
    h = *s + ((h << 9) | (h >> (আকার_BITS - 9)));

  ফেরত h;
}

স্থির শূন্য
string_free (ধ্রুবক শূন্য *cp)
{
  শূন্য *p = (শূন্য*) cp;
  মুক্ত (p);
}

ধ্রুবক অক্ষর *
map_file_name (অক্ষর ধ্রুবক *filename)
{
  যদি (!filename || !prefix_maps)
    ফেরত filename;
  যদি (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ধ্রুবক শূন্য *res = gl_map_get (mapped_files, filename);
  যদি (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  ফেরত res;
}

স্থির শূন্য
prefix_map_free (কাঠামো prefix_map *p)
{
  মুক্ত (p->oldprefix);
  মুক্ত (p->newprefix);
  মুক্ত (p);
}

শূন্য
add_prefix_map (অক্ষর ধ্রুবক *oldprefix, অক্ষর ধ্রুবক *newprefix)
{
  যদি (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ ফাঁকা,
                              /* hashcode */ ফাঁকা,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  কাঠামো prefix_map *p = xmalloc (আকার (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
স্থির শূন্য
compute_exts_from_gf (ধ্রুবক অক্ষর *ext)
{
  যদি (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  অন্যথা
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
স্থির শূন্য
compute_exts_from_src (ধ্রুবক অক্ষর *ext)
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

স্থির শূন্য
file_name_split (ধ্রুবক অক্ষর *file_name,
                 ধ্রুবক অক্ষর **base, ধ্রুবক অক্ষর **tab, ধ্রুবক অক্ষর **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = ফাঁকা;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  যদি (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = আকার (TAB_EXT) - 1;
      যদি (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

স্থির শূন্য
compute_file_name_parts (শূন্য)
{
  যদি (spec_outfile)
    {
      ধ্রুবক অক্ষর *base, *tab, *ext;
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

      যদি (ext)
        compute_exts_from_src (ext);
    }
  অন্যথা
    {
      ধ্রুবক অক্ষর *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      যদি (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      অন্যথা যদি (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      অন্যথা
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      যদি (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      অন্যথা
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      যদি (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

শূন্য
compute_output_file_names (শূন্য)
{
  compute_file_name_parts ();

  /* If not yet done. */
  যদি (!src_extension)
    src_extension = xstrdup (".c");
  যদি (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  যদি (header_flag)
    {
      যদি (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  যদি (graph_flag)
    {
      যদি (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  যদি (html_flag)
    {
      যদি (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  যদি (xml_flag)
    {
      যদি (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  যদি (report_flag)
    {
      যদি (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  মুক্ত (all_but_tab_ext);
  মুক্ত (src_extension);
  মুক্ত (header_extension);
}

শূন্য
output_file_name_check (অক্ষর **file_name, bool source)
{
  bool conflict = false;
  যদি (STREQ (*file_name, grammar_file))
    {
      complain (ফাঁকা, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  অন্যথা
    জন্য (পূর্ণসংখ্যা i = 0; i < generated_files_আকার; i++)
      যদি (STREQ (generated_files[i].name, *file_name))
        {
          complain (ফাঁকা, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  যদি (conflict)
    {
      মুক্ত (*file_name);
      *file_name = strdup ("/dev/null");
    }
  অন্যথা
    {
      generated_files = xnrealloc (generated_files, ++generated_files_আকার,
                                   আকার *generated_files);
      generated_files[generated_files_আকার-1].name = xstrdup (*file_name);
      generated_files[generated_files_আকার-1].is_source = source;
    }
}

শূন্য
unlink_generated_sources (শূন্য)
{
  জন্য (পূর্ণসংখ্যা i = 0; i < generated_files_আকার; i++)
    যদি (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
স্থির অক্ষর *relocate_বাফার = ফাঁকা;

অক্ষর ধ্রুবক *
pkgdatadir (শূন্য)
{
  যদি (relocate_বাফার)
    ফেরত relocate_বাফার;
  অন্যথা
    {
      অক্ষর ধ্রুবক *cp = getenv ("BISON_PKGDATADIR");
      ফেরত cp ? cp : relocate2 (PKGDATADIR, &relocate_বাফার);
    }
}

অক্ষর ধ্রুবক *
m4path (শূন্য)
{
  অক্ষর ধ্রুবক *m4 = getenv ("M4");
  যদি (m4)
    ফেরত m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  অক্ষর ধ্রুবক *m4_relocated = relocate (M4);
  কাঠামো stat buf;
  যদি (stat (m4_relocated, &buf) == 0)
    ফেরত m4_relocated;

  ফেরত M4;
}

শূন্য
output_file_names_free (শূন্য)
{
  মুক্ত (all_but_ext);
  মুক্ত (spec_verbose_file);
  মুক্ত (spec_graph_file);
  মুক্ত (spec_html_file);
  মুক্ত (spec_xml_file);
  মুক্ত (spec_header_file);
  মুক্ত (parser_file_name);
  মুক্ত (dir_prefix);
  জন্য (পূর্ণসংখ্যা i = 0; i < generated_files_আকার; i++)
    মুক্ত (generated_files[i].name);
  মুক্ত (generated_files);
  মুক্ত (relocate_বাফার);

  যদি (prefix_maps)
    gl_list_free (prefix_maps);
  যদি (mapped_files)
    gl_map_free (mapped_files);
}
