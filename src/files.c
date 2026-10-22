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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <configmake.h> /* PKGDATADIR */
#અન્તર્ભાવ <dirname.h>
#અન્તર્ભાવ <error.h>
#અન્તર્ભાવ <get-errno.h>
#અન્તર્ભાવ <gl_array_list.h>
#અન્તર્ભાવ <gl_hash_map.h>
#અન્તર્ભાવ <gl_xlist.h>
#અન્તર્ભાવ <gl_xmap.h>
#અન્તર્ભાવ <quote.h>
#અન્તર્ભાવ <quotearg.h>
#અન્તર્ભાવ <relocatable.h> /* relocate2 */
#અન્તર્ભાવ <stdio-safer.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <xstrndup.h>

#અન્તર્ભાવ "complain.h"
#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"
#અન્તર્ભાવ "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

અક્ષર અચલ *spec_outfile = ખાલી;       /* for -o. */
અક્ષર અચલ *spec_file_prefix = ખાલી;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
અક્ષર અચલ *spec_name_prefix = ખાલી;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
અક્ષર *spec_verbose_file = ખાલી;  /* for --verbose. */
અક્ષર *spec_graph_file = ખાલી;    /* for -g. */
અક્ષર *spec_html_file = ખાલી;     /* for --html. */
અક્ષર *spec_xml_file = ખાલી;      /* for -x. */
અક્ષર *spec_header_file = ખાલી;   /* for --header. */
અક્ષર *parser_file_name;

/* All computed output file names.  */
પ્રકારવ્યાખ્યા સંરચના generated_file
{
  /** File name.  */
  અક્ષર *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
સ્થિર generated_file *generated_files = ખાલી;
સ્થિર પૂર્ણાંક generated_files_size = 0;

uniqstr grammar_file = ખાલી;

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

અક્ષર *all_but_ext;
સ્થિર અક્ષર *all_but_tab_ext;
અક્ષર *dir_prefix;

/* C source file extension (the parser source).  */
સ્થિર અક્ષર *src_extension = ખાલી;
/* Header file extension (if option '`-d'' is specified).  */
સ્થિર અક્ષર *header_extension = ખાલી;

સંરચના prefix_map
{
  અક્ષર *oldprefix;
  અક્ષર *newprefix;
};

સ્થિર gl_list_t prefix_maps = ખાલી;

/* Map file names to prefix-mapped file names. */
સ્થિર gl_map_t mapped_files = ખાલી;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

સ્થિર અક્ષર *
concat2 (અક્ષર અચલ *str1, અક્ષર અચલ *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  અક્ષર *res = xmalloc (len + 1);
  અક્ષર *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  પાછા res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (અચલ અક્ષર *name, અચલ અક્ષર *mode)
{
  FILE *res = fopen_safer (name, mode);
  જો (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  પાછા res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

શૂન્ય
xfclose (FILE *ptr)
{
  જો (ptr == ખાલી)
    પાછા;

  જો (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  જો (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (પૂર્ણાંક fd, અક્ષર અચલ *mode)
{
  FILE *res = fdopen (fd, mode);
  જો (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  પાછા res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
સ્થિર અક્ષર *
map_file_name_alloc (અક્ષર અચલ *filename)
{
  સંરચના prefix_map અચલ *p = ખાલી;
  પ્રતિજ્ઞા (prefix_maps);
  {
    શૂન્ય અચલ *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    જ્યારે (gl_list_iterator_next (&iter, &ptr, ખાલી))
      {
        p = ptr;
        જો (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          તોડો;
        p = ખાલી;
      }
    gl_list_iterator_free (&iter);
  }

  જો (!p)
    પાછા xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  અક્ષર *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  અક્ષર *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  પાછા res;
}

સ્થિર bool
string_equals (અચલ શૂન્ય *x1, અચલ શૂન્ય *x2)
{
  અચલ અક્ષર *s1 = x1;
  અચલ અક્ષર *s2 = x2;
  પાછા STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
સ્થિર size_t
string_hash (અચલ શૂન્ય *x)
{
#define SIZE_BITS (કદ (size_t) * CHAR_BIT)

  અચલ અક્ષર *s = x;
  size_t h = 0;

  માટે (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  પાછા h;
}

સ્થિર શૂન્ય
string_free (અચલ શૂન્ય *cp)
{
  શૂન્ય *p = (શૂન્ય*) cp;
  મુક્ત (p);
}

અચલ અક્ષર *
map_file_name (અક્ષર અચલ *filename)
{
  જો (!filename || !prefix_maps)
    પાછા filename;
  જો (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  અચલ શૂન્ય *res = gl_map_get (mapped_files, filename);
  જો (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  પાછા res;
}

સ્થિર શૂન્ય
prefix_map_free (સંરચના prefix_map *p)
{
  મુક્ત (p->oldprefix);
  મુક્ત (p->newprefix);
  મુક્ત (p);
}

શૂન્ય
add_prefix_map (અક્ષર અચલ *oldprefix, અક્ષર અચલ *newprefix)
{
  જો (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ ખાલી,
                              /* hashcode */ ખાલી,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  સંરચના prefix_map *p = xmalloc (કદ (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
સ્થિર શૂન્ય
compute_exts_from_gf (અચલ અક્ષર *ext)
{
  જો (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  નહીંતર
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
સ્થિર શૂન્ય
compute_exts_from_src (અચલ અક્ષર *ext)
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

સ્થિર શૂન્ય
file_name_split (અચલ અક્ષર *file_name,
                 અચલ અક્ષર **base, અચલ અક્ષર **tab, અચલ અક્ષર **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = ખાલી;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  જો (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = કદ (TAB_EXT) - 1;
      જો (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

સ્થિર શૂન્ય
compute_file_name_parts (શૂન્ય)
{
  જો (spec_outfile)
    {
      અચલ અક્ષર *base, *tab, *ext;
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

      જો (ext)
        compute_exts_from_src (ext);
    }
  નહીંતર
    {
      અચલ અક્ષર *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      જો (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      નહીંતર જો (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      નહીંતર
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      જો (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      નહીંતર
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      જો (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

શૂન્ય
compute_output_file_names (શૂન્ય)
{
  compute_file_name_parts ();

  /* If not yet done. */
  જો (!src_extension)
    src_extension = xstrdup (".c");
  જો (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  જો (header_flag)
    {
      જો (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  જો (graph_flag)
    {
      જો (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  જો (html_flag)
    {
      જો (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  જો (xml_flag)
    {
      જો (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  જો (report_flag)
    {
      જો (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  મુક્ત (all_but_tab_ext);
  મુક્ત (src_extension);
  મુક્ત (header_extension);
}

શૂન્ય
output_file_name_check (અક્ષર **file_name, bool source)
{
  bool conflict = false;
  જો (STREQ (*file_name, grammar_file))
    {
      complain (ખાલી, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  નહીંતર
    માટે (પૂર્ણાંક i = 0; i < generated_files_size; i++)
      જો (STREQ (generated_files[i].name, *file_name))
        {
          complain (ખાલી, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  જો (conflict)
    {
      મુક્ત (*file_name);
      *file_name = strdup ("/dev/null");
    }
  નહીંતર
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   કદ *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

શૂન્ય
unlink_generated_sources (શૂન્ય)
{
  માટે (પૂર્ણાંક i = 0; i < generated_files_size; i++)
    જો (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
સ્થિર અક્ષર *relocate_buffer = ખાલી;

અક્ષર અચલ *
pkgdatadir (શૂન્ય)
{
  જો (relocate_buffer)
    પાછા relocate_buffer;
  નહીંતર
    {
      અક્ષર અચલ *cp = getenv ("BISON_PKGDATADIR");
      પાછા cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

અક્ષર અચલ *
m4path (શૂન્ય)
{
  અક્ષર અચલ *m4 = getenv ("M4");
  જો (m4)
    પાછા m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  અક્ષર અચલ *m4_relocated = relocate (M4);
  સંરચના stat buf;
  જો (stat (m4_relocated, &buf) == 0)
    પાછા m4_relocated;

  પાછા M4;
}

શૂન્ય
output_file_names_free (શૂન્ય)
{
  મુક્ત (all_but_ext);
  મુક્ત (spec_verbose_file);
  મુક્ત (spec_graph_file);
  મુક્ત (spec_html_file);
  મુક્ત (spec_xml_file);
  મુક્ત (spec_header_file);
  મુક્ત (parser_file_name);
  મુક્ત (dir_prefix);
  માટે (પૂર્ણાંક i = 0; i < generated_files_size; i++)
    મુક્ત (generated_files[i].name);
  મુક્ત (generated_files);
  મુક્ત (relocate_buffer);

  જો (prefix_maps)
    gl_list_free (prefix_maps);
  જો (mapped_files)
    gl_map_free (mapped_files);
}
