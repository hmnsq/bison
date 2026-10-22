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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <configmake.h> /* PKGDATADIR */
#അന്തര്ഭാവ <dirname.h>
#അന്തര്ഭാവ <error.h>
#അന്തര്ഭാവ <get-errno.h>
#അന്തര്ഭാവ <gl_array_list.h>
#അന്തര്ഭാവ <gl_hash_map.h>
#അന്തര്ഭാവ <gl_xlist.h>
#അന്തര്ഭാവ <gl_xmap.h>
#അന്തര്ഭാവ <quote.h>
#അന്തര്ഭാവ <quotearg.h>
#അന്തര്ഭാവ <relocatable.h> /* relocate2 */
#അന്തര്ഭാവ <stdio-safer.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <xstrndup.h>

#അന്തര്ഭാവ "complain.h"
#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"
#അന്തര്ഭാവ "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

അക്ഷരം സ്ഥിരം *spec_outfile = ശൂന്യ;       /* for -o. */
അക്ഷരം സ്ഥിരം *spec_file_prefix = ശൂന്യ;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
അക്ഷരം സ്ഥിരം *spec_name_prefix = ശൂന്യ;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
അക്ഷരം *spec_verbose_file = ശൂന്യ;  /* for --verbose. */
അക്ഷരം *spec_graph_file = ശൂന്യ;    /* for -g. */
അക്ഷരം *spec_html_file = ശൂന്യ;     /* for --html. */
അക്ഷരം *spec_xml_file = ശൂന്യ;      /* for -x. */
അക്ഷരം *spec_header_file = ശൂന്യ;   /* for --header. */
അക്ഷരം *parser_file_name;

/* All computed output file names.  */
തരനിർവചനം ഘടന generated_file
{
  /** File name.  */
  അക്ഷരം *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
സ്ഥിതം generated_file *generated_files = ശൂന്യ;
സ്ഥിതം പൂർണ്ണസംഖ്യ generated_files_size = 0;

uniqstr grammar_file = ശൂന്യ;

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

അക്ഷരം *all_but_ext;
സ്ഥിതം അക്ഷരം *all_but_tab_ext;
അക്ഷരം *dir_prefix;

/* C source file extension (the parser source).  */
സ്ഥിതം അക്ഷരം *src_extension = ശൂന്യ;
/* Header file extension (if option '`-d'' is specified).  */
സ്ഥിതം അക്ഷരം *header_extension = ശൂന്യ;

ഘടന prefix_map
{
  അക്ഷരം *oldprefix;
  അക്ഷരം *newprefix;
};

സ്ഥിതം gl_list_t prefix_maps = ശൂന്യ;

/* Map file names to prefix-mapped file names. */
സ്ഥിതം gl_map_t mapped_files = ശൂന്യ;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

സ്ഥിതം അക്ഷരം *
concat2 (അക്ഷരം സ്ഥിരം *str1, അക്ഷരം സ്ഥിരം *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  അക്ഷരം *res = xmalloc (len + 1);
  അക്ഷരം *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  തിരികെ res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *mode)
{
  FILE *res = fopen_safer (name, mode);
  എങ്കിൽ (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  തിരികെ res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

ശൂന്യം
xfclose (FILE *ptr)
{
  എങ്കിൽ (ptr == ശൂന്യ)
    തിരികെ;

  എങ്കിൽ (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  എങ്കിൽ (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *mode)
{
  FILE *res = fdopen (fd, mode);
  എങ്കിൽ (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  തിരികെ res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
സ്ഥിതം അക്ഷരം *
map_file_name_alloc (അക്ഷരം സ്ഥിരം *filename)
{
  ഘടന prefix_map സ്ഥിരം *p = ശൂന്യ;
  പ്രതിജ്ഞാ (prefix_maps);
  {
    ശൂന്യം സ്ഥിരം *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    ആകുമ്പോൾ (gl_list_iterator_next (&iter, &ptr, ശൂന്യ))
      {
        p = ptr;
        എങ്കിൽ (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          നിർത്തുക;
        p = ശൂന്യ;
      }
    gl_list_iterator_free (&iter);
  }

  എങ്കിൽ (!p)
    തിരികെ xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  അക്ഷരം *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  അക്ഷരം *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  തിരികെ res;
}

സ്ഥിതം bool
string_equals (സ്ഥിരം ശൂന്യം *x1, സ്ഥിരം ശൂന്യം *x2)
{
  സ്ഥിരം അക്ഷരം *s1 = x1;
  സ്ഥിരം അക്ഷരം *s2 = x2;
  തിരികെ STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
സ്ഥിതം size_t
string_hash (സ്ഥിരം ശൂന്യം *x)
{
#define SIZE_BITS (വലുപ്പം (size_t) * CHAR_BIT)

  സ്ഥിരം അക്ഷരം *s = x;
  size_t h = 0;

  വേണ്ടി (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  തിരികെ h;
}

സ്ഥിതം ശൂന്യം
string_free (സ്ഥിരം ശൂന്യം *cp)
{
  ശൂന്യം *p = (ശൂന്യം*) cp;
  സ്വതന്ത്രം (p);
}

സ്ഥിരം അക്ഷരം *
map_file_name (അക്ഷരം സ്ഥിരം *filename)
{
  എങ്കിൽ (!filename || !prefix_maps)
    തിരികെ filename;
  എങ്കിൽ (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  സ്ഥിരം ശൂന്യം *res = gl_map_get (mapped_files, filename);
  എങ്കിൽ (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  തിരികെ res;
}

സ്ഥിതം ശൂന്യം
prefix_map_free (ഘടന prefix_map *p)
{
  സ്വതന്ത്രം (p->oldprefix);
  സ്വതന്ത്രം (p->newprefix);
  സ്വതന്ത്രം (p);
}

ശൂന്യം
add_prefix_map (അക്ഷരം സ്ഥിരം *oldprefix, അക്ഷരം സ്ഥിരം *newprefix)
{
  എങ്കിൽ (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ ശൂന്യ,
                              /* hashcode */ ശൂന്യ,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  ഘടന prefix_map *p = xmalloc (വലുപ്പം (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
സ്ഥിതം ശൂന്യം
compute_exts_from_gf (സ്ഥിരം അക്ഷരം *ext)
{
  എങ്കിൽ (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  അല്ലെങ്കിൽ
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
സ്ഥിതം ശൂന്യം
compute_exts_from_src (സ്ഥിരം അക്ഷരം *ext)
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

സ്ഥിതം ശൂന്യം
file_name_split (സ്ഥിരം അക്ഷരം *file_name,
                 സ്ഥിരം അക്ഷരം **base, സ്ഥിരം അക്ഷരം **tab, സ്ഥിരം അക്ഷരം **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = ശൂന്യ;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  എങ്കിൽ (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = വലുപ്പം (TAB_EXT) - 1;
      എങ്കിൽ (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

സ്ഥിതം ശൂന്യം
compute_file_name_parts (ശൂന്യം)
{
  എങ്കിൽ (spec_outfile)
    {
      സ്ഥിരം അക്ഷരം *base, *tab, *ext;
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

      എങ്കിൽ (ext)
        compute_exts_from_src (ext);
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിരം അക്ഷരം *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      എങ്കിൽ (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      അല്ലെങ്കിൽ
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      എങ്കിൽ (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      അല്ലെങ്കിൽ
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      എങ്കിൽ (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

ശൂന്യം
compute_output_file_names (ശൂന്യം)
{
  compute_file_name_parts ();

  /* If not yet done. */
  എങ്കിൽ (!src_extension)
    src_extension = xstrdup (".c");
  എങ്കിൽ (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  എങ്കിൽ (header_flag)
    {
      എങ്കിൽ (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  എങ്കിൽ (graph_flag)
    {
      എങ്കിൽ (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  എങ്കിൽ (html_flag)
    {
      എങ്കിൽ (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  എങ്കിൽ (xml_flag)
    {
      എങ്കിൽ (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  എങ്കിൽ (report_flag)
    {
      എങ്കിൽ (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  സ്വതന്ത്രം (all_but_tab_ext);
  സ്വതന്ത്രം (src_extension);
  സ്വതന്ത്രം (header_extension);
}

ശൂന്യം
output_file_name_check (അക്ഷരം **file_name, bool source)
{
  bool conflict = false;
  എങ്കിൽ (STREQ (*file_name, grammar_file))
    {
      complain (ശൂന്യ, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  അല്ലെങ്കിൽ
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < generated_files_size; i++)
      എങ്കിൽ (STREQ (generated_files[i].name, *file_name))
        {
          complain (ശൂന്യ, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  എങ്കിൽ (conflict)
    {
      സ്വതന്ത്രം (*file_name);
      *file_name = strdup ("/dev/null");
    }
  അല്ലെങ്കിൽ
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   വലുപ്പം *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

ശൂന്യം
unlink_generated_sources (ശൂന്യം)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < generated_files_size; i++)
    എങ്കിൽ (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
സ്ഥിതം അക്ഷരം *relocate_buffer = ശൂന്യ;

അക്ഷരം സ്ഥിരം *
pkgdatadir (ശൂന്യം)
{
  എങ്കിൽ (relocate_buffer)
    തിരികെ relocate_buffer;
  അല്ലെങ്കിൽ
    {
      അക്ഷരം സ്ഥിരം *cp = getenv ("BISON_PKGDATADIR");
      തിരികെ cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

അക്ഷരം സ്ഥിരം *
m4path (ശൂന്യം)
{
  അക്ഷരം സ്ഥിരം *m4 = getenv ("M4");
  എങ്കിൽ (m4)
    തിരികെ m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  അക്ഷരം സ്ഥിരം *m4_relocated = relocate (M4);
  ഘടന stat buf;
  എങ്കിൽ (stat (m4_relocated, &buf) == 0)
    തിരികെ m4_relocated;

  തിരികെ M4;
}

ശൂന്യം
output_file_names_free (ശൂന്യം)
{
  സ്വതന്ത്രം (all_but_ext);
  സ്വതന്ത്രം (spec_verbose_file);
  സ്വതന്ത്രം (spec_graph_file);
  സ്വതന്ത്രം (spec_html_file);
  സ്വതന്ത്രം (spec_xml_file);
  സ്വതന്ത്രം (spec_header_file);
  സ്വതന്ത്രം (parser_file_name);
  സ്വതന്ത്രം (dir_prefix);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < generated_files_size; i++)
    സ്വതന്ത്രം (generated_files[i].name);
  സ്വതന്ത്രം (generated_files);
  സ്വതന്ത്രം (relocate_buffer);

  എങ്കിൽ (prefix_maps)
    gl_list_free (prefix_maps);
  എങ്കിൽ (mapped_files)
    gl_map_free (mapped_files);
}
