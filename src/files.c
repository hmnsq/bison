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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <configmake.h> /* PKGDATADIR */
#ଅନ୍ତର୍ଭାଵ <dirname.h>
#ଅନ୍ତର୍ଭାଵ <error.h>
#ଅନ୍ତର୍ଭାଵ <get-errno.h>
#ଅନ୍ତର୍ଭାଵ <gl_array_list.h>
#ଅନ୍ତର୍ଭାଵ <gl_hash_map.h>
#ଅନ୍ତର୍ଭାଵ <gl_xlist.h>
#ଅନ୍ତର୍ଭାଵ <gl_xmap.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>
#ଅନ୍ତର୍ଭାଵ <quotearg.h>
#ଅନ୍ତର୍ଭାଵ <relocatable.h> /* relocate2 */
#ଅନ୍ତର୍ଭାଵ <stdio-safer.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <xstrndup.h>

#ଅନ୍ତର୍ଭାଵ "complain.h"
#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"
#ଅନ୍ତର୍ଭାଵ "gram.h"

/* Initializing some values below (such SPEC_NAME_PREFIX to 'yy') is
   tempting, but don't do that: for the time being our handling of the
   %directive vs --option leaves precedence to the options by deciding
   that if a %directive sets a variable which is really set (i.e., not
   NULL), then the %directive is ignored.  As a result, %name-prefix,
   for instance, will not be honored.  */

ଅକ୍ଷର ଅଚଳ *spec_outfile = ଖାଲି;       /* for -o. */
ଅକ୍ଷର ଅଚଳ *spec_file_prefix = ଖାଲି;   /* for -b. */
location spec_file_prefix_loc = EMPTY_LOCATION_INIT;
ଅକ୍ଷର ଅଚଳ *spec_name_prefix = ଖାଲି;   /* for -p. */
location spec_name_prefix_loc = EMPTY_LOCATION_INIT;
ଅକ୍ଷର *spec_verbose_file = ଖାଲି;  /* for --verbose. */
ଅକ୍ଷର *spec_graph_file = ଖାଲି;    /* for -g. */
ଅକ୍ଷର *spec_html_file = ଖାଲି;     /* for --html. */
ଅକ୍ଷର *spec_xml_file = ଖାଲି;      /* for -x. */
ଅକ୍ଷର *spec_header_file = ଖାଲି;   /* for --header. */
ଅକ୍ଷର *parser_file_name;

/* All computed output file names.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା generated_file
{
  /** File name.  */
  ଅକ୍ଷର *name;
  /** Whether is a generated source file (e.g., *.c, *.java...), as
      opposed to the report file (e.g., *.output).  When late errors
      are detected, generated source files are removed.  */
  bool is_source;
} generated_file;
ସ୍ଥିର generated_file *generated_files = ଖାଲି;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା generated_files_size = 0;

uniqstr grammar_file = ଖାଲି;

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

ଅକ୍ଷର *all_but_ext;
ସ୍ଥିର ଅକ୍ଷର *all_but_tab_ext;
ଅକ୍ଷର *dir_prefix;

/* C source file extension (the parser source).  */
ସ୍ଥିର ଅକ୍ଷର *src_extension = ଖାଲି;
/* Header file extension (if option '`-d'' is specified).  */
ସ୍ଥିର ଅକ୍ଷର *header_extension = ଖାଲି;

ସଂରଚନା prefix_map
{
  ଅକ୍ଷର *oldprefix;
  ଅକ୍ଷର *newprefix;
};

ସ୍ଥିର gl_list_t prefix_maps = ଖାଲି;

/* Map file names to prefix-mapped file names. */
ସ୍ଥିର gl_map_t mapped_files = ଖାଲି;


/*-----------------------------------------------------------------.
| Return a newly allocated string composed of the concatenation of |
| STR1, and STR2.                                                  |
`-----------------------------------------------------------------*/

ସ୍ଥିର ଅକ୍ଷର *
concat2 (ଅକ୍ଷର ଅଚଳ *str1, ଅକ୍ଷର ଅଚଳ *str2)
{
  size_t len = strlen (str1) + strlen (str2);
  ଅକ୍ଷର *res = xmalloc (len + 1);
  ଅକ୍ଷର *cp;
  cp = stpcpy (res, str1);
  cp = stpcpy (cp, str2);
  ଫେରନ୍ତୁ res;
}

/*-----------------------------------------------------------------.
| Try to open file NAME with mode MODE, and print an error message |
| if fails.                                                        |
`-----------------------------------------------------------------*/

FILE *
xfopen (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *mode)
{
  FILE *res = fopen_safer (name, mode);
  ଯଦି (!res)
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot open"), quotearg_colon (name));

  ଫେରନ୍ତୁ res;
}

/*-------------------------------------------------------------.
| Try to close file PTR, and print an error message if fails.  |
`-------------------------------------------------------------*/

ଶୂନ୍ୟ
xfclose (FILE *ptr)
{
  ଯଦି (ptr == ଖାଲି)
    ଫେରନ୍ତୁ;

  ଯଦି (ferror (ptr))
    error (EXIT_FAILURE, 0, _("input/output error"));

  ଯଦି (fclose (ptr) != 0)
    error (EXIT_FAILURE, get_errno (), _("cannot close file"));
}


FILE *
xfdopen (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *mode)
{
  FILE *res = fdopen (fd, mode);
  ଯଦି (! res)
    error (EXIT_FAILURE, get_errno (),
           /* On a separate line to please the "unmarked_diagnostics"
              syntax-check. */
           "fdopen");
  ଫେରନ୍ତୁ res;
}

/* The mapped name of FILENAME, allocated, if there are prefix maps.
   Otherwise NULL.  */
ସ୍ଥିର ଅକ୍ଷର *
map_file_name_alloc (ଅକ୍ଷର ଅଚଳ *filename)
{
  ସଂରଚନା prefix_map ଅଚଳ *p = ଖାଲି;
  ପ୍ରତିଜ୍ଞା (prefix_maps);
  {
    ଶୂନ୍ୟ ଅଚଳ *ptr;
    gl_list_iterator_t iter = gl_list_iterator (prefix_maps);
    ଯେତେବେଳେ (gl_list_iterator_next (&iter, &ptr, ଖାଲି))
      {
        p = ptr;
        ଯଦି (strncmp (p->oldprefix, filename, strlen (p->oldprefix)) == 0)
          ଭାଙ୍ଗ;
        p = ଖାଲି;
      }
    gl_list_iterator_free (&iter);
  }

  ଯଦି (!p)
    ଫେରନ୍ତୁ xstrdup (filename);

  size_t oldprefix_len = strlen (p->oldprefix);
  size_t newprefix_len = strlen (p->newprefix);
  ଅକ୍ଷର *res = xmalloc (newprefix_len + strlen (filename) - oldprefix_len + 1);

  ଅକ୍ଷର *end = stpcpy (res, p->newprefix);
  stpcpy (end, filename + oldprefix_len);

  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର bool
string_equals (ଅଚଳ ଶୂନ୍ୟ *x1, ଅଚଳ ଶୂନ୍ୟ *x2)
{
  ଅଚଳ ଅକ୍ଷର *s1 = x1;
  ଅଚଳ ଅକ୍ଷର *s2 = x2;
  ଫେରନ୍ତୁ STREQ (s1, s2);
}

/* A hash function for NUL-terminated char* strings using
   the method described by Bruno Haible.
   See https://www.haible.de/bruno/hashfunc.html.  */
ସ୍ଥିର size_t
string_hash (ଅଚଳ ଶୂନ୍ୟ *x)
{
#define SIZE_BITS (ଆକାର (size_t) * CHAR_BIT)

  ଅଚଳ ଅକ୍ଷର *s = x;
  size_t h = 0;

  ପାଇଁ (; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  ଫେରନ୍ତୁ h;
}

ସ୍ଥିର ଶୂନ୍ୟ
string_free (ଅଚଳ ଶୂନ୍ୟ *cp)
{
  ଶୂନ୍ୟ *p = (ଶୂନ୍ୟ*) cp;
  ମୁକ୍ତ (p);
}

ଅଚଳ ଅକ୍ଷର *
map_file_name (ଅକ୍ଷର ଅଚଳ *filename)
{
  ଯଦି (!filename || !prefix_maps)
    ଫେରନ୍ତୁ filename;
  ଯଦି (!mapped_files)
    mapped_files
      = gl_map_nx_create_empty (GL_HASH_MAP,
                                string_equals, string_hash,
                                string_free, string_free);
  ଅଚଳ ଶୂନ୍ୟ *res = gl_map_get (mapped_files, filename);
  ଯଦି (!res)
    {
      res = map_file_name_alloc (filename);
      gl_map_put (mapped_files, xstrdup (filename), res);
    }
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ
prefix_map_free (ସଂରଚନା prefix_map *p)
{
  ମୁକ୍ତ (p->oldprefix);
  ମୁକ୍ତ (p->newprefix);
  ମୁକ୍ତ (p);
}

ଶୂନ୍ୟ
add_prefix_map (ଅକ୍ଷର ଅଚଳ *oldprefix, ଅକ୍ଷର ଅଚଳ *newprefix)
{
  ଯଦି (!prefix_maps)
    prefix_maps
      = gl_list_create_empty (GL_ARRAY_LIST,
                              /* equals */ ଖାଲି,
                              /* hashcode */ ଖାଲି,
                              (gl_listelement_dispose_fn) prefix_map_free,
                              true);

  ସଂରଚନା prefix_map *p = xmalloc (ଆକାର (*p));
  p->oldprefix = xstrdup (oldprefix);
  p->newprefix = xstrdup (newprefix);

  gl_list_add_last (prefix_maps, p);
}

/*------------------------------------------------------------------.
| Compute ALL_BUT_EXT, ALL_BUT_TAB_EXT and output files extensions. |
`------------------------------------------------------------------*/

/* Compute extensions from the grammar file extension.  */
ସ୍ଥିର ଶୂନ୍ୟ
compute_exts_from_gf (ଅଚଳ ଅକ୍ଷର *ext)
{
  ଯଦି (STREQ (ext, ".y"))
    {
      src_extension = xstrdup (language->src_extension);
      header_extension = xstrdup (language->header_extension);
    }
  ଅନ୍ୟଥା
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
ସ୍ଥିର ଶୂନ୍ୟ
compute_exts_from_src (ଅଚଳ ଅକ୍ଷର *ext)
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

ସ୍ଥିର ଶୂନ୍ୟ
file_name_split (ଅଚଳ ଅକ୍ଷର *file_name,
                 ଅଚଳ ଅକ୍ଷର **base, ଅଚଳ ଅକ୍ଷର **tab, ଅଚଳ ଅକ୍ଷର **ext)
{
  *base = last_component (file_name);

  /* Look for the extension, i.e., look for the last dot. */
  *ext = strrchr (*base, '.');
  *tab = ଖାଲି;

  /* If there is an extension, check if there is a '.tab' part right
     before.  */
  ଯଦି (*ext)
    {
      size_t baselen = *ext - *base;
      size_t dottablen = ଆକାର (TAB_EXT) - 1;
      ଯଦି (dottablen < baselen
          && STRPREFIX_LIT (TAB_EXT, *ext - dottablen))
        *tab = *ext - dottablen;
    }
}

/* Compute ALL_BUT_EXT and ALL_BUT_TAB_EXT from SPEC_OUTFILE or
   GRAMMAR_FILE.

   The precise -o name will be used for FTABLE.  For other output
   files, remove the ".c" or ".tab.c" suffix.  */

ସ୍ଥିର ଶୂନ୍ୟ
compute_file_name_parts (ଶୂନ୍ୟ)
{
  ଯଦି (spec_outfile)
    {
      ଅଚଳ ଅକ୍ଷର *base, *tab, *ext;
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

      ଯଦି (ext)
        compute_exts_from_src (ext);
    }
  ଅନ୍ୟଥା
    {
      ଅଚଳ ଅକ୍ଷର *base, *tab, *ext;
      file_name_split (grammar_file, &base, &tab, &ext);

      ଯଦି (spec_file_prefix)
        {
          /* If --file-prefix=foo was specified, ALL_BUT_TAB_EXT = 'foo'.  */
          dir_prefix =
            xstrndup (spec_file_prefix,
                      last_component (spec_file_prefix) - spec_file_prefix);
          all_but_tab_ext = xstrdup (spec_file_prefix);
        }
      ଅନ୍ୟଥା ଯଦି (! location_empty (yacc_loc))
        {
          /* If --yacc, then the output is 'y.tab.c'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext = xstrdup ("y");
        }
      ଅନ୍ୟଥା
        {
          /* Otherwise, ALL_BUT_TAB_EXT is computed from the input
             grammar: 'foo/bar.yy' => 'bar'.  */
          dir_prefix = xstrdup ("");
          all_but_tab_ext =
            xstrndup (base, (strlen (base) - (ext ? strlen (ext) : 0)));
        }

      ଯଦି (language->add_tab)
        all_but_ext = concat2 (all_but_tab_ext, TAB_EXT);
      ଅନ୍ୟଥା
        all_but_ext = xstrdup (all_but_tab_ext);

      /* Compute the extensions from the grammar file name.  */
      ଯଦି (ext && location_empty (yacc_loc))
        compute_exts_from_gf (ext);
    }
}


/* Compute the output file names.  Warn if we detect conflicting
   outputs to the same file.  */

ଶୂନ୍ୟ
compute_output_file_names (ଶୂନ୍ୟ)
{
  compute_file_name_parts ();

  /* If not yet done. */
  ଯଦି (!src_extension)
    src_extension = xstrdup (".c");
  ଯଦି (!header_extension)
    header_extension = xstrdup (".h");

  parser_file_name =
    (spec_outfile
     ? xstrdup (spec_outfile)
     : concat2 (all_but_ext, src_extension));

  ଯଦି (header_flag)
    {
      ଯଦି (! spec_header_file)
        spec_header_file = concat2 (all_but_ext, header_extension);
    }

  ଯଦି (graph_flag)
    {
      ଯଦି (! spec_graph_file)
        spec_graph_file = concat2 (all_but_tab_ext, ".gv");
      output_file_name_check (&spec_graph_file, false);
    }

  ଯଦି (html_flag)
    {
      ଯଦି (! spec_html_file)
        spec_html_file = concat2 (all_but_tab_ext, ".html");
      output_file_name_check (&spec_html_file, false);
    }

  ଯଦି (xml_flag)
    {
      ଯଦି (! spec_xml_file)
        spec_xml_file = concat2 (all_but_tab_ext, ".xml");
      output_file_name_check (&spec_xml_file, false);
    }

  ଯଦି (report_flag)
    {
      ଯଦି (!spec_verbose_file)
        spec_verbose_file = concat2 (all_but_tab_ext, OUTPUT_EXT);
      output_file_name_check (&spec_verbose_file, false);
    }

  ମୁକ୍ତ (all_but_tab_ext);
  ମୁକ୍ତ (src_extension);
  ମୁକ୍ତ (header_extension);
}

ଶୂନ୍ୟ
output_file_name_check (ଅକ୍ଷର **file_name, bool source)
{
  bool conflict = false;
  ଯଦି (STREQ (*file_name, grammar_file))
    {
      complain (ଖାଲି, complaint, _("refusing to overwrite the input file %s"),
                quote (*file_name));
      conflict = true;
    }
  ଅନ୍ୟଥା
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < generated_files_size; i++)
      ଯଦି (STREQ (generated_files[i].name, *file_name))
        {
          complain (ଖାଲି, Wother, _("conflicting outputs to file %s"),
                    quote (generated_files[i].name));
          conflict = true;
        }
  ଯଦି (conflict)
    {
      ମୁକ୍ତ (*file_name);
      *file_name = strdup ("/dev/null");
    }
  ଅନ୍ୟଥା
    {
      generated_files = xnrealloc (generated_files, ++generated_files_size,
                                   ଆକାର *generated_files);
      generated_files[generated_files_size-1].name = xstrdup (*file_name);
      generated_files[generated_files_size-1].is_source = source;
    }
}

ଶୂନ୍ୟ
unlink_generated_sources (ଶୂନ୍ୟ)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < generated_files_size; i++)
    ଯଦି (generated_files[i].is_source)
      /* Ignore errors.  The file might not even exist.  */
      unlink (generated_files[i].name);
}

/* Memory allocated by relocate2, to free.  */
ସ୍ଥିର ଅକ୍ଷର *relocate_buffer = ଖାଲି;

ଅକ୍ଷର ଅଚଳ *
pkgdatadir (ଶୂନ୍ୟ)
{
  ଯଦି (relocate_buffer)
    ଫେରନ୍ତୁ relocate_buffer;
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର ଅଚଳ *cp = getenv ("BISON_PKGDATADIR");
      ଫେରନ୍ତୁ cp ? cp : relocate2 (PKGDATADIR, &relocate_buffer);
    }
}

ଅକ୍ଷର ଅଚଳ *
m4path (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର ଅଚଳ *m4 = getenv ("M4");
  ଯଦି (m4)
    ଫେରନ୍ତୁ m4;

  /* We don't use relocate2() to store the temporary buffer and re-use
     it, because m4path() is only called once.  */
  ଅକ୍ଷର ଅଚଳ *m4_relocated = relocate (M4);
  ସଂରଚନା stat buf;
  ଯଦି (stat (m4_relocated, &buf) == 0)
    ଫେରନ୍ତୁ m4_relocated;

  ଫେରନ୍ତୁ M4;
}

ଶୂନ୍ୟ
output_file_names_free (ଶୂନ୍ୟ)
{
  ମୁକ୍ତ (all_but_ext);
  ମୁକ୍ତ (spec_verbose_file);
  ମୁକ୍ତ (spec_graph_file);
  ମୁକ୍ତ (spec_html_file);
  ମୁକ୍ତ (spec_xml_file);
  ମୁକ୍ତ (spec_header_file);
  ମୁକ୍ତ (parser_file_name);
  ମୁକ୍ତ (dir_prefix);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < generated_files_size; i++)
    ମୁକ୍ତ (generated_files[i].name);
  ମୁକ୍ତ (generated_files);
  ମୁକ୍ତ (relocate_buffer);

  ଯଦି (prefix_maps)
    gl_list_free (prefix_maps);
  ଯଦି (mapped_files)
    gl_map_free (mapped_files);
}
