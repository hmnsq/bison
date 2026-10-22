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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <configmake.h> /* PKGDATADIR */
#அந்தர்பாவ <dirname.h>
#அந்தர்பாவ <error.h>
#அந்தர்பாவ <get-errno.h>
#அந்தர்பாவ <gl_array_list.h>
#அந்தர்பாவ <gl_hash_map.h>
#அந்தர்பாவ <gl_xlist.h>
#அந்தர்பாவ <gl_xmap.h>
#அந்தர்பாவ <quote.h>
#அந்தர்பாவ <quotearg.h>
#அந்தர்பாவ <relocatable.h> /* relocate2 */
#அந்தர்பாவ <stdio-safer.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <xstrndup.h>

#அந்தர்பாவ "complain.h"
#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"
#அந்தர்பாவ "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

எழுத்து மாறிலி *spec_outfile = வெற்று;       /* for -o. */
எழுத்து மாறிலி *spec_file_prefix = வெற்று;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
எழுத்து மாறிலி *spec_name_prefix = வெற்று;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
எழுத்து *spec_verbose_file = வெற்று;  /* for --verbose. */
எழுத்து *spec_graph_file = வெற்று;    /* for -g. */
எழுத்து *spec_html_file = வெற்று;     /* for --html. */
எழுத்து *spec_xml_file = வெற்று;      /* for -x. */
எழுத்து *spec_header_file = வெற்று;   /* for --header. */
எழுத்து *parser_file_name;

/* All computed output file names.  */
வகைவரை கட்டமைப்பு generated_file
{
  /** File name.  */
  எழுத்து *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
நிலையான generated_file *generated_files = வெற்று;
நிலையான முழு generated_files_size = 0;

uniqstr grammar_file = வெற்று;

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

எழுத்து *all_but_ext;
நிலையான எழுத்து *all_but_tab_ext;
எழுத்து *dir_prefix;

/* C source file extension (the parser source).  */
நிலையான எழுத்து *src_extension = வெற்று;
/* Header file extension (if option '`-d'' is specified).  */
நிலையான எழுத்து *header_extension = வெற்று;

கட்டமைப்பு prefix_map
{
  எழுத்து *oldprefix;
  எழுத்து *newprefix;
};

நிலையான gl_list_t prefix_maps = வெற்று;

/* Map file names to prefix-mapped file names. */
நிலையான gl_map_t mapped_files = வெற்று;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

நிலையான எழுத்து *
concat2 (எழுத்து மாறிலி *str1, எழுத்து மாறிலி *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  எழுத்து *res = xmalloc (len + 1);
  எழுத்து *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  திரும்பு res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (மாறிலி எழுத்து *name, மாறிலி எழுத்து *mode)
{
  FILE *res = fopen_safer (name, mode);
  எனில் (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  திரும்பு res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

வெறுமை
xfclose (FILE *ptr)
{
  எனில் (ptr == வெற்று)
    திரும்பு;

  எனில் (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  எனில் (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (முழு fd, எழுத்து மாறிலி *mode)
{
  FILE *res = fdopen (fd, mode);
  எனில் (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  திரும்பு res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
நிலையான எழுத்து *
map_file_name_alloc (எழுத்து மாறிலி *filename)
{
  கட்டமைப்பு prefix_map மாறிலி *p = வெற்று;
  ப்ரதிஜ்ஞா (prefix_maps);
  {
    வெறுமை மாறிலி *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    போது (gl_list_iterator_next (&iter, &ptr, வெற்று))
      {
        p = ptr;
        எனில் (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          நிறுத்து;
        p = வெற்று;
      }
    gl_list_iterator_free (&iter);
  }

  எனில் (!p)
    திரும்பு xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  எழுத்து *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  எழுத்து *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  திரும்பு res;
}

நிலையான bool
string_equals (மாறிலி வெறுமை *x1, மாறிலி வெறுமை *x2)
{
  மாறிலி எழுத்து *s1 = x1;
  மாறிலி எழுத்து *s2 = x2;
  திரும்பு STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
நிலையான size_t
string_hash (மாறிலி வெறுமை *x)
{
#define SIZE_BITS (அளவு (size_t) * CHAR_BIT)

  மாறிலி எழுத்து *s = x;
  size_t h = 0;

  க்கு (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  திரும்பு h;
}

நிலையான வெறுமை
string_free (மாறிலி வெறுமை *cp)
{
  வெறுமை *p = (வெறுமை*) cp;
  விடுவி (p);
}

மாறிலி எழுத்து *
map_file_name (எழுத்து மாறிலி *filename)
{
  எனில் (!filename || !prefix_maps)
    திரும்பு filename;
  எனில் (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  மாறிலி வெறுமை *res = gl_map_get (mapped_files, filename);
  எனில் (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  திரும்பு res;
}

நிலையான வெறுமை
prefix_map_free (கட்டமைப்பு prefix_map *p)
{
  விடுவி (p->oldprefix);
  விடுவி (p->newprefix);
  விடுவி (p);
}

வெறுமை
add_prefix_map (எழுத்து மாறிலி *oldprefix, எழுத்து மாறிலி *newprefix)
{
  எனில் (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ வெற்று,
                              /* hashcode */ வெற்று,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  கட்டமைப்பு prefix_map *p = xmalloc (அளவு (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
நிலையான வெறுமை
compute_exts_from_gf (மாறிலி எழுத்து *ext)
{
  எனில் (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  இல்லையெனில்
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
நிலையான வெறுமை
compute_exts_from_src (மாறிலி எழுத்து *ext)
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

நிலையான வெறுமை
file_name_split (மாறிலி எழுத்து *file_name,
                 மாறிலி எழுத்து **base, மாறிலி எழுத்து **tab, மாறிலி எழுத்து **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = வெற்று;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  எனில் (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = அளவு (TAB_EXT) - 1;
      எனில் (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

நிலையான வெறுமை
compute_file_name_parts (வெறுமை)
{
  எனில் (spec_outfile)
    {
      மாறிலி எழுத்து *base, *tab, *ext;
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

      எனில் (ext)
        compute_exts_from_src (ext);
    }
  இல்லையெனில்
    {
      மாறிலி எழுத்து *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      எனில் (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      இல்லையெனில் எனில் (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      இல்லையெனில்
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      எனில் (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      இல்லையெனில்
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      எனில் (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

வெறுமை
compute_output_file_names (வெறுமை)
{
  compute_file_name_parts ();

  /* If not yet done. */
  எனில் (!src_extension)
    src_extension = xstrdup (".c");
  எனில் (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  எனில் (header_flag)
    {
      எனில் (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  எனில் (graph_flag)
    {
      எனில் (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  எனில் (html_flag)
    {
      எனில் (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  எனில் (xml_flag)
    {
      எனில் (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  எனில் (report_flag)
    {
      எனில் (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  விடுவி (all_but_tab_ext);
  விடுவி (src_extension);
  விடுவி (header_extension);
}

வெறுமை
output_file_name_check (எழுத்து **file_name, bool source)
{
  bool conflict = false;
  எனில் (STREQ (*file_name, grammar_file))
    {
      complain (வெற்று, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  இல்லையெனில்
    க்கு (முழு i = 0; i < generated_files_size; i++)
      எனில் (STREQ (generated_files[i].name, *file_name))
        {
          complain (வெற்று, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  எனில் (conflict)
    {
      விடுவி (*file_name);
      *file_name = strdup ("/dev/null");
    }
  இல்லையெனில்
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   அளவு *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

வெறுமை
unlink_generated_sources (வெறுமை)
{
  க்கு (முழு i = 0; i < generated_files_size; i++)
    எனில் (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
நிலையான எழுத்து *relocate_buffer = வெற்று;

எழுத்து மாறிலி *
pkgdatadir (வெறுமை)
{
  எனில் (relocate_buffer)
    திரும்பு relocate_buffer;
  இல்லையெனில்
    {
      எழுத்து மாறிலி *cp = getenv ("BISON_PKGDATADIR");
      திரும்பு cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

எழுத்து மாறிலி *
m4path (வெறுமை)
{
  எழுத்து மாறிலி *m4 = getenv ("M4");
  எனில் (m4)
    திரும்பு m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  எழுத்து மாறிலி *m4_relocated = relocate (M4);
  கட்டமைப்பு stat buf;
  எனில் (stat (m4_relocated, &buf) == 0)
    திரும்பு m4_relocated;

  திரும்பு M4;
}

வெறுமை
output_file_names_free (வெறுமை)
{
  விடுவி (all_but_ext);
  விடுவி (spec_verbose_file);
  விடுவி (spec_graph_file);
  விடுவி (spec_html_file);
  விடுவி (spec_xml_file);
  விடுவி (spec_header_file);
  விடுவி (parser_file_name);
  விடுவி (dir_prefix);
  க்கு (முழு i = 0; i < generated_files_size; i++)
    விடுவி (generated_files[i].name);
  விடுவி (generated_files);
  விடுவி (relocate_buffer);

  எனில் (prefix_maps)
    gl_list_free (prefix_maps);
  எனில் (mapped_files)
    gl_map_free (mapped_files);
}
