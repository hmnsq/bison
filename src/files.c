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

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec_outfile = ಖಾಲಿ;       /* for -o. */
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;   /* for -b. */
location spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ_loc = ಖಾಲಿಗಳು_LOCATION_INIT;
ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;   /* for -p. */
location spec_name_ಪೂರ್ವಪ್ರತ್ಯಯ_loc = ಖಾಲಿಗಳು_LOCATION_INIT;
ಅಕ್ಷರ *spec_verbose_file = ಖಾಲಿ;  /* for --verbose. */
ಅಕ್ಷರ *spec_ನಕ್ಷೆ_file = ಖಾಲಿ;    /* for -g. */
ಅಕ್ಷರ *spec_html_file = ಖಾಲಿ;     /* for --html. */
ಅಕ್ಷರ *spec_xml_file = ಖಾಲಿ;      /* for -x. */
ಅಕ್ಷರ *spec_ಶೀರ್ಷಿಕೆ_file = ಖಾಲಿ;   /* for --header. */
ಅಕ್ಷರ *parser_file_name;

/* All computed output file names.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ generated_file
{
  /** File name.  */
  ಅಕ್ಷರ *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_ಮೂಲ;
} generated_file;
ಸ್ಥಿರ generated_file *generated_files = ಖಾಲಿ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ generated_files_ಗಾತ್ರ = 0;

uniqstr grammar_file = ಖಾಲಿ;

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

ಅಕ್ಷರ *all_but_ext;
ಸ್ಥಿರ ಅಕ್ಷರ *all_but_tab_ext;
ಅಕ್ಷರ *dir_ಪೂರ್ವಪ್ರತ್ಯಯ;

/* C source file extension (the parser source).  */
ಸ್ಥಿರ ಅಕ್ಷರ *src_extension = ಖಾಲಿ;
/* Header file extension (if option '`-d'' is specified).  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ_extension = ಖಾಲಿ;

ರಚನೆ ಪೂರ್ವಪ್ರತ್ಯಯ_map
{
  ಅಕ್ಷರ *oldprefix;
  ಅಕ್ಷರ *newprefix;
};

ಸ್ಥಿರ gl_list_t ಪೂರ್ವಪ್ರತ್ಯಯ_maps = ಖಾಲಿ;

/* Map file names to prefix-mapped file names. */
ಸ್ಥಿರ gl_map_t mapped_files = ಖಾಲಿ;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

ಸ್ಥಿರ ಅಕ್ಷರ *
concat2 (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str1, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str2)
{
  size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str1) + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str2);
  ಅಕ್ಷರ *res = xmalloc (len + 1);
  ಅಕ್ಷರ *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  ಹಿಂತಿರುಗಿ res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಹುಲಕ)
{
  FILE *res = fopen_safer (name, ಬಹುಲಕ);
  ಆದರೆ (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  ಹಿಂತಿರುಗಿ res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

ಶೂನ್ಯ
xfclose (FILE *ptr)
{
  ಆದರೆ (ptr == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  ಆದರೆ (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಹುಲಕ)
{
  FILE *res = fdopen (fd, ಬಹುಲಕ);
  ಆದರೆ (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  ಹಿಂತಿರುಗಿ res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
map_file_name_alloc (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  ರಚನೆ ಪೂರ್ವಪ್ರತ್ಯಯ_map ಸ್ಥಿರಾಂಕ *p = ಖಾಲಿ;
  ಪ್ರತಿಜ್ಞಾ (ಪೂರ್ವಪ್ರತ್ಯಯ_maps);
  {
    ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ptr;
    gl_list_iterator_t iter = gl_list_iterator (ಪೂರ್ವಪ್ರತ್ಯಯ_maps);
    ಆಗ (gl_list_iterator_ಮುಂದಿನ (&iter, &ptr, ಖಾಲಿ))
      {
        p = ptr;
        ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (p->oldprefix, filename, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (p->oldprefix)) == 0)
          ನಿಲ್ಲಿಸು;
        p = ಖಾಲಿ;
      }
    gl_list_iterator_free (&iter);
  }

  ಆದರೆ (!p)
    ಹಿಂತಿರುಗಿ xstrdup (filename);

  size_t oldprefix_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (p->oldprefix);
  size_t newprefix_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (p->newprefix);
  ಅಕ್ಷರ *res = xmalloc (newprefix_len + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (filename) - oldprefix_len + 1);

  ಅಕ್ಷರ *ಕೊನೆ = stpcpy (res, p->newprefix);
  stpcpy (ಕೊನೆ, filename + oldprefix_len);

  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ bool
string_equals (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *x1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *x2)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1 = x1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2 = x2;
  ಹಿಂತಿರುಗಿ STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
ಸ್ಥಿರ size_t
string_hash (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *x)
{
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು (ಗಾತ್ರ (size_t) * CHAR_ಬಿಟ್)

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = x;
  size_t h = 0;

  ಗಾಗಿ (; *s; s++)
    h = *s + ((h << 9) | (h >> (ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು - 9)));

  ಹಿಂತಿರುಗಿ h;
}

ಸ್ಥಿರ ಶೂನ್ಯ
string_free (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *cp)
{
  ಶೂನ್ಯ *p = (ಶೂನ್ಯ*) cp;
  ಬಿಡುಗಡೆ (p);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
map_file_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  ಆದರೆ (!filename || !ಪೂರ್ವಪ್ರತ್ಯಯ_maps)
    ಹಿಂತಿರುಗಿ filename;
  ಆದರೆ (!mapped_files)
    mapped_files
      = gl_map_nx_create_ಖಾಲಿಗಳು (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *res = gl_map_get (mapped_files, filename);
  ಆದರೆ (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಪೂರ್ವಪ್ರತ್ಯಯ_map_free (ರಚನೆ ಪೂರ್ವಪ್ರತ್ಯಯ_map *p)
{
  ಬಿಡುಗಡೆ (p->oldprefix);
  ಬಿಡುಗಡೆ (p->newprefix);
  ಬಿಡುಗಡೆ (p);
}

ಶೂನ್ಯ
ಕೂಡಿಸು_ಪೂರ್ವಪ್ರತ್ಯಯ_map (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *oldprefix, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *newprefix)
{
  ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ_maps)
    ಪೂರ್ವಪ್ರತ್ಯಯ_maps
      = gl_list_create_ಖಾಲಿಗಳು (GL_ARRAY_LIST,
                              /* equals */ ಖಾಲಿ,
                              /* hashcode */ ಖಾಲಿ,
                              (gl_listelement_dispose_fn) ಪೂರ್ವಪ್ರತ್ಯಯ_map_free,
                              true);

  ರಚನೆ ಪೂರ್ವಪ್ರತ್ಯಯ_map *p = xmalloc (ಗಾತ್ರ (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_ಕೂಡಿಸು_ಕೊನೆಯ (ಪೂರ್ವಪ್ರತ್ಯಯ_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
ಸ್ಥಿರ ಶೂನ್ಯ
compute_exts_from_gf (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext)
{
  ಆದರೆ (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      ಶೀರ್ಷಿಕೆ_extension = xstrdup (language->ಶೀರ್ಷಿಕೆ_extension);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      src_extension = xstrdup (ext);
      ಶೀರ್ಷಿಕೆ_extension = xstrdup (ext);
      tr (src_extension, 'y', 'c');
      tr (src_extension, 'Y', 'C');
      tr (ಶೀರ್ಷಿಕೆ_extension, 'y', 'h');
      tr (ಶೀರ್ಷಿಕೆ_extension, 'Y', 'H');
    }
}

/* Compute extensions from the given c source file extension.  */
ಸ್ಥಿರ ಶೂನ್ಯ
compute_exts_from_src (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext)
{
  /* We use this function when the user specifies `-o' or `--output',
     so the extensions must be computed unconditionally from the file name
     given by this option.  */
  src_extension = xstrdup (ext);
  ಶೀರ್ಷಿಕೆ_extension = xstrdup (ext);
  tr (ಶೀರ್ಷಿಕೆ_extension, 'c', 'h');
  tr (ಶೀರ್ಷಿಕೆ_extension, 'C', 'H');
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
file_name_ವಿಭಜಿಸು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ತಳ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **tab, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ext)
{
  *ತಳ = ಕೊನೆಯ_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*ತಳ, '.');
  *tab = ಖಾಲಿ;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  ಆದರೆ (*ext)
    {
      size_t baselen = *ext - *ತಳ;
      size_t dottablen = ಗಾತ್ರ (TAB_EXT) - 1;
      ಆದರೆ (dottablen < baselen
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
  ಆದರೆ (spec_outfile)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ, *tab, *ext;
      file_name_ವಿಭಜಿಸು (spec_outfile, &ತಳ, &tab, &ext);
      dir_ಪೂರ್ವಪ್ರತ್ಯಯ = xstrndup (spec_outfile, ತಳ - spec_outfile);

      /* ALL_BUT_EXT goes up the EXT, excluding it. */
      all_but_ext =
        xstrndup (spec_outfile,
                  (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (spec_outfile) - (ext ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ext) : 0)));

      /* ALL_BUT_TAB_EXT goes up to TAB, excluding it.  */
      all_but_tab_ext =
        xstrndup (spec_outfile,
                  (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (spec_outfile)
                   - (tab ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (tab) : (ext ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ext) : 0))));

      ಆದರೆ (ext)
        compute_exts_from_src (ext);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ, *tab, *ext;
      file_name_ವಿಭಜಿಸು (grammar_file, &ತಳ, &tab, &ext);

      ಆದರೆ (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_ಪೂರ್ವಪ್ರತ್ಯಯ =
            xstrndup (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ,
                      ಕೊನೆಯ_component (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ) - spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ);
          all_but_tab_ext = xstrdup (spec_file_ಪೂರ್ವಪ್ರತ್ಯಯ);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! location_ಖಾಲಿಗಳು (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_ಪೂರ್ವಪ್ರತ್ಯಯ = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_ಪೂರ್ವಪ್ರತ್ಯಯ = xstrdup ("");
          all_but_tab_ext =
            xstrndup (ತಳ, (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ತಳ) - (ext ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ext) : 0)));
        }

      ಆದರೆ (language->ಕೂಡಿಸು_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      ಇಲ್ಲದಿದ್ದರೆ
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      ಆದರೆ (ext && location_ಖಾಲಿಗಳು (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

ಶೂನ್ಯ
compute_output_file_ಹೆಸರುಗಳು (ಶೂನ್ಯ)
{
  compute_file_name_parts ();

  /* If not yet done. */
  ಆದರೆ (!src_extension)
    src_extension = xstrdup (".c");
  ಆದರೆ (!ಶೀರ್ಷಿಕೆ_extension)
    ಶೀರ್ಷಿಕೆ_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  ಆದರೆ (ಶೀರ್ಷಿಕೆ_flag)
    {
      ಆದರೆ (! spec_ಶೀರ್ಷಿಕೆ_file)
        spec_ಶೀರ್ಷಿಕೆ_file = concat2 (all_but_ext, ಶೀರ್ಷಿಕೆ_extension);
    }

  ಆದರೆ (ನಕ್ಷೆ_flag)
    {
      ಆದರೆ (! spec_ನಕ್ಷೆ_file)
        spec_ನಕ್ಷೆ_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_ನಕ್ಷೆ_file, false);
    }

  ಆದರೆ (html_flag)
    {
      ಆದರೆ (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  ಆದರೆ (xml_flag)
    {
      ಆದರೆ (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  ಆದರೆ (report_flag)
    {
      ಆದರೆ (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  ಬಿಡುಗಡೆ (all_but_tab_ext);
  ಬಿಡುಗಡೆ (src_extension);
  ಬಿಡುಗಡೆ (ಶೀರ್ಷಿಕೆ_extension);
}

ಶೂನ್ಯ
output_file_name_check (ಅಕ್ಷರ **file_name, bool ಮೂಲ)
{
  bool conflict = false;
  ಆದರೆ (STREQ (*file_name, grammar_file))
    {
      complain (ಖಾಲಿ, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < generated_files_ಗಾತ್ರ; i++)
      ಆದರೆ (STREQ (generated_files[i].name, *file_name))
        {
          complain (ಖಾಲಿ, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  ಆದರೆ (conflict)
    {
      ಬಿಡುಗಡೆ (*file_name);
      *file_name = strdup ("/dev/null");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      generated_files = xnrealloc (generated_files, ++generated_files_ಗಾತ್ರ,
                                   ಗಾತ್ರ *generated_files);
      generated_files[generated_files_ಗಾತ್ರ-1].name = xstrdup (*file_name);
      generated_files[generated_files_ಗಾತ್ರ-1].is_ಮೂಲ = ಮೂಲ;
    }
}

ಶೂನ್ಯ
unlink_generated_sources (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < generated_files_ಗಾತ್ರ; i++)
    ಆದರೆ (generated_files[i].is_ಮೂಲ)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
ಸ್ಥಿರ ಅಕ್ಷರ *relocate_ಬಫರ್ = ಖಾಲಿ;

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
pkgdatadir (ಶೂನ್ಯ)
{
  ಆದರೆ (relocate_ಬಫರ್)
    ಹಿಂತಿರುಗಿ relocate_ಬಫರ್;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp = getenv ("BISON_PKGDATADIR");
      ಹಿಂತಿರುಗಿ cp ? cp : relocate2 (PKGDATADIR, &relocate_ಬಫರ್);
    }
}

ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
m4path (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *m4 = getenv ("M4");
  ಆದರೆ (m4)
    ಹಿಂತಿರುಗಿ m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *m4_relocated = relocate (M4);
  ರಚನೆ stat buf;
  ಆದರೆ (stat (m4_relocated, &buf) == 0)
    ಹಿಂತಿರುಗಿ m4_relocated;

  ಹಿಂತಿರುಗಿ M4;
}

ಶೂನ್ಯ
output_file_ಹೆಸರುಗಳು_free (ಶೂನ್ಯ)
{
  ಬಿಡುಗಡೆ (all_but_ext);
  ಬಿಡುಗಡೆ (spec_verbose_file);
  ಬಿಡುಗಡೆ (spec_ನಕ್ಷೆ_file);
  ಬಿಡುಗಡೆ (spec_html_file);
  ಬಿಡುಗಡೆ (spec_xml_file);
  ಬಿಡುಗಡೆ (spec_ಶೀರ್ಷಿಕೆ_file);
  ಬಿಡುಗಡೆ (parser_file_name);
  ಬಿಡುಗಡೆ (dir_ಪೂರ್ವಪ್ರತ್ಯಯ);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < generated_files_ಗಾತ್ರ; i++)
    ಬಿಡುಗಡೆ (generated_files[i].name);
  ಬಿಡುಗಡೆ (generated_files);
  ಬಿಡುಗಡೆ (relocate_ಬಫರ್);

  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ_maps)
    gl_list_free (ಪೂರ್ವಪ್ರತ್ಯಯ_maps);
  ಆದರೆ (mapped_files)
    gl_map_free (mapped_files);
}
