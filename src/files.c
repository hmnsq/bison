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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <configmake.h> /* PKGDATADIR */
#ضمن <dirname.h>
#ضمن <error.h>
#ضمن <get-errno.h>
#ضمن <gl_array_list.h>
#ضمن <gl_hash_map.h>
#ضمن <gl_xlist.h>
#ضمن <gl_xmap.h>
#ضمن <quote.h>
#ضمن <quotearg.h>
#ضمن <relocatable.h> /* relocate2 */
#ضمن <stdio-safer.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <unistd.h>
#ضمن <xstrndup.h>

#ضمن "complain.h"
#ضمن "files.h"
#ضمن "getargs.h"
#ضمن "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

حرف ثابت *spec_outfile = عدم;       /* for -o. */
حرف ثابت *spec_file_اگیتر = عدم;   /* for -b. */
location spec_file_اگیتر_loc = خالی_تھاواں_LOCATION_INIT;
حرف ثابت *spec_name_اگیتر = عدم;   /* for -p. */
location spec_name_اگیتر_loc = خالی_تھاواں_LOCATION_INIT;
حرف *spec_verbose_file = عدم;  /* for --verbose. */
حرف *spec_گراف_file = عدم;    /* for -g. */
حرف *spec_html_file = عدم;     /* for --html. */
حرف *spec_xml_file = عدم;      /* for -x. */
حرف *spec_سرخی_file = عدم;   /* for --header. */
حرف *parser_file_name;

/* All computed output file names.  */
تعريفنوع بنية generated_file
{
  /** File name.  */
  حرف *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_سروت;
} generated_file;
ساكن generated_file *generated_files = عدم;
ساكن صحيح generated_files_ناپ = 0;

uniqstr grammar_file = عدم;

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

حرف *all_but_ext;
ساكن حرف *all_but_tab_ext;
حرف *dir_اگیتر;

/* C source file extension (the parser source).  */
ساكن حرف *src_extension = عدم;
/* Header file extension (if option '`-d'' is specified).  */
ساكن حرف *سرخی_extension = عدم;

بنية اگیتر_map
{
  حرف *oldprefix;
  حرف *newprefix;
};

ساكن gl_list_t اگیتر_maps = عدم;

/* Map file names to prefix-mapped file names. */
ساكن gl_map_t mapped_files = عدم;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

ساكن حرف *
concat2 (حرف ثابت *str1, حرف ثابت *str2)
{
  size_t len = لڑی_لمبائی (str1) + لڑی_لمبائی (str2);
  حرف *res = xmalloc (len + 1);
  حرف *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  أرجع res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ثابت حرف *name, ثابت حرف *منوال)
{
  FILE *res = fopen_safer (name, منوال);
  إذا (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  أرجع res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

فراغ
xfclose (FILE *ptr)
{
  إذا (ptr == عدم)
    أرجع;

  إذا (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  إذا (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (صحيح fd, حرف ثابت *منوال)
{
  FILE *res = fdopen (fd, منوال);
  إذا (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  أرجع res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
ساكن حرف *
map_file_name_alloc (حرف ثابت *filename)
{
  بنية اگیتر_map ثابت *p = عدم;
  أكد (اگیتر_maps);
  {
    فراغ ثابت *ptr;
    gl_list_iterator_t iter = gl_list_iterator (اگیتر_maps);
    طالما (gl_list_iterator_اگلا (&iter, &ptr, عدم))
      {
        p = ptr;
        إذا (لڑی_مقابلہ (p->oldprefix, filename, لڑی_لمبائی (p->oldprefix)) == 0)
          اكسر;
        p = عدم;
      }
    gl_list_iterator_free (&iter);
  }

  إذا (!p)
    أرجع xstrdup (filename);

  size_t oldprefix_len = لڑی_لمبائی (p->oldprefix);
  size_t newprefix_len = لڑی_لمبائی (p->newprefix);
  حرف *res = xmalloc (newprefix_len + لڑی_لمبائی (filename) - oldprefix_len + 1);

  حرف *سرا = stpcpy (res, p->newprefix);
  stpcpy (سرا, filename + oldprefix_len);

  أرجع res;
}

ساكن bool
string_equals (ثابت فراغ *x1, ثابت فراغ *x2)
{
  ثابت حرف *s1 = x1;
  ثابت حرف *s2 = x2;
  أرجع STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
ساكن size_t
string_hash (ثابت فراغ *x)
{
#تعریف_کرو ناپ_بٹ (حجم (size_t) * CHAR_ثنائی_ہندسہ)

  ثابت حرف *s = x;
  size_t h = 0;

  لكل (; *s; s++)
    h = *s + ((h << 9) | (h >> (ناپ_بٹ - 9)));

  أرجع h;
}

ساكن فراغ
string_free (ثابت فراغ *cp)
{
  فراغ *p = (فراغ*) cp;
  حرر (p);
}

ثابت حرف *
map_file_name (حرف ثابت *filename)
{
  إذا (!filename || !اگیتر_maps)
    أرجع filename;
  إذا (!mapped_files)
    mapped_files
      = gl_map_nx_create_خالی_تھاواں (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ثابت فراغ *res = gl_map_get (mapped_files, filename);
  إذا (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  أرجع res;
}

ساكن فراغ
اگیتر_map_free (بنية اگیتر_map *p)
{
  حرر (p->oldprefix);
  حرر (p->newprefix);
  حرر (p);
}

فراغ
جوڑو_اگیتر_map (حرف ثابت *oldprefix, حرف ثابت *newprefix)
{
  إذا (!اگیتر_maps)
    اگیتر_maps
      = gl_list_create_خالی_تھاواں (GL_ARRAY_LIST,
                              /* equals */ عدم,
                              /* hashcode */ عدم,
                              (gl_listelement_dispose_fn) اگیتر_map_free,
                              true);

  بنية اگیتر_map *p = xmalloc (حجم (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_جوڑو_آخری (اگیتر_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
ساكن فراغ
compute_exts_from_gf (ثابت حرف *ext)
{
  إذا (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      سرخی_extension = xstrdup (language->سرخی_extension);
    }
  وإلا
    {
      src_extension = xstrdup (ext);
      سرخی_extension = xstrdup (ext);
      tr (src_extension, 'y', 'c');
      tr (src_extension, 'Y', 'C');
      tr (سرخی_extension, 'y', 'h');
      tr (سرخی_extension, 'Y', 'H');
    }
}

/* Compute extensions from the given c source file extension.  */
ساكن فراغ
compute_exts_from_src (ثابت حرف *ext)
{
  /* We use this function when the user specifies `-o' or `--output',
     so the extensions must be computed unconditionally from the file name
     given by this option.  */
  src_extension = xstrdup (ext);
  سرخی_extension = xstrdup (ext);
  tr (سرخی_extension, 'c', 'h');
  tr (سرخی_extension, 'C', 'H');
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

ساكن فراغ
file_name_ونڈو (ثابت حرف *file_name,
                 ثابت حرف **ادھار, ثابت حرف **tab, ثابت حرف **ext)
{
  *ادھار = آخری_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*ادھار, '.');
  *tab = عدم;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  إذا (*ext)
    {
      size_t baselen = *ext - *ادھار;
      size_t dottablen = حجم (TAB_EXT) - 1;
      إذا (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

ساكن فراغ
compute_file_name_parts (فراغ)
{
  إذا (spec_outfile)
    {
      ثابت حرف *ادھار, *tab, *ext;
      file_name_ونڈو (spec_outfile, &ادھار, &tab, &ext);
      dir_اگیتر = xstrndup (spec_outfile, ادھار - spec_outfile);

      /* ALL_BUT_EXT goes up the EXT, excluding it. */
      all_but_ext =
        xstrndup (spec_outfile,
                  (لڑی_لمبائی (spec_outfile) - (ext ? لڑی_لمبائی (ext) : 0)));

      /* ALL_BUT_TAB_EXT goes up to TAB, excluding it.  */
      all_but_tab_ext =
        xstrndup (spec_outfile,
                  (لڑی_لمبائی (spec_outfile)
                   - (tab ? لڑی_لمبائی (tab) : (ext ? لڑی_لمبائی (ext) : 0))));

      إذا (ext)
        compute_exts_from_src (ext);
    }
  وإلا
    {
      ثابت حرف *ادھار, *tab, *ext;
      file_name_ونڈو (grammar_file, &ادھار, &tab, &ext);

      إذا (spec_file_اگیتر)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_اگیتر =
            xstrndup (spec_file_اگیتر,
                      آخری_component (spec_file_اگیتر) - spec_file_اگیتر);
          all_but_tab_ext = xstrdup (spec_file_اگیتر);
        }
      وإلا إذا (! location_خالی_تھاواں (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_اگیتر = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      وإلا
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_اگیتر = xstrdup ("");
          all_but_tab_ext =
            xstrndup (ادھار, (لڑی_لمبائی (ادھار) - (ext ? لڑی_لمبائی (ext) : 0)));
        }

      إذا (language->جوڑو_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      وإلا
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      إذا (ext && location_خالی_تھاواں (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

فراغ
compute_output_file_ناں (فراغ)
{
  compute_file_name_parts ();

  /* If not yet done. */
  إذا (!src_extension)
    src_extension = xstrdup (".c");
  إذا (!سرخی_extension)
    سرخی_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  إذا (سرخی_flag)
    {
      إذا (! spec_سرخی_file)
        spec_سرخی_file = concat2 (all_but_ext, سرخی_extension);
    }

  إذا (گراف_flag)
    {
      إذا (! spec_گراف_file)
        spec_گراف_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_گراف_file, false);
    }

  إذا (html_flag)
    {
      إذا (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  إذا (xml_flag)
    {
      إذا (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  إذا (report_flag)
    {
      إذا (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  حرر (all_but_tab_ext);
  حرر (src_extension);
  حرر (سرخی_extension);
}

فراغ
output_file_name_check (حرف **file_name, bool سروت)
{
  bool conflict = false;
  إذا (STREQ (*file_name, grammar_file))
    {
      complain (عدم, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  وإلا
    لكل (صحيح i = 0; i < generated_files_ناپ; i++)
      إذا (STREQ (generated_files[i].name, *file_name))
        {
          complain (عدم, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  إذا (conflict)
    {
      حرر (*file_name);
      *file_name = strdup ("/dev/null");
    }
  وإلا
    {
      generated_files = xnrealloc (generated_files, ++generated_files_ناپ,
                                   حجم *generated_files);
      generated_files[generated_files_ناپ-1].name = xstrdup (*file_name);
      generated_files[generated_files_ناپ-1].is_سروت = سروت;
    }
}

فراغ
unlink_generated_sources (فراغ)
{
  لكل (صحيح i = 0; i < generated_files_ناپ; i++)
    إذا (generated_files[i].is_سروت)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
ساكن حرف *relocate_بفر = عدم;

حرف ثابت *
pkgdatadir (فراغ)
{
  إذا (relocate_بفر)
    أرجع relocate_بفر;
  وإلا
    {
      حرف ثابت *cp = getenv ("BISON_PKGDATADIR");
      أرجع cp ? cp : relocate2 (PKGDATADIR, &relocate_بفر);
    }
}

حرف ثابت *
m4path (فراغ)
{
  حرف ثابت *m4 = getenv ("M4");
  إذا (m4)
    أرجع m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  حرف ثابت *m4_relocated = relocate (M4);
  بنية stat buf;
  إذا (stat (m4_relocated, &buf) == 0)
    أرجع m4_relocated;

  أرجع M4;
}

فراغ
output_file_ناں_free (فراغ)
{
  حرر (all_but_ext);
  حرر (spec_verbose_file);
  حرر (spec_گراف_file);
  حرر (spec_html_file);
  حرر (spec_xml_file);
  حرر (spec_سرخی_file);
  حرر (parser_file_name);
  حرر (dir_اگیتر);
  لكل (صحيح i = 0; i < generated_files_ناپ; i++)
    حرر (generated_files[i].name);
  حرر (generated_files);
  حرر (relocate_بفر);

  إذا (اگیتر_maps)
    gl_list_free (اگیتر_maps);
  إذا (mapped_files)
    gl_map_free (mapped_files);
}
