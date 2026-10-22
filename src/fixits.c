/* Support for fixing grammar files.

   Copyright (C) 2019-2022 Free Software Foundation, Inc.

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

#include "fixits.h"

#include <error.h>
#include <get-errno.h>
#include <gl_array_list.h>
#include <gl_xlist.h>
#include <progname.h>
#include <quote.h>
#include <quotearg.h>
#include <vasnprintf.h>

#include "system.h"

#include "files.h"
#include "getargs.h"

typedef sa_nracanaa
{
  location location;
  var_na *fix;
} fixit;

gl_list_t fixits = NULL;

static fixit *
fixit_new (location const *loc, var_na const* fix)
{
  fixit *res = xmalloc (parimaa_na *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  ᱨᱩᱣᱟᱹᱲ res;
}

static puur_naa_nka
fixit_cmp (const  fixit *a, const fixit *b)
{
  ᱨᱩᱣᱟᱹᱲ location_cmp (a->location, b->location);
}

static void
fixit_free (fixit *f)
{
  free (f->fix);
  free (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
static void
fixit_print (fixit const *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.line, f->location.start.byte,
           f->location.end.line, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


void
fixits_register (location const *loc, var_na const* fix)
{
  ᱡᱩᱫᱤ (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ NULL,
                                   /* hashcode */ NULL,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  ᱡᱩᱫᱤ (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (void)
{
  ᱨᱩᱣᱟᱹᱲ !fixits;
}


void
fixits_run (void)
{
  ᱡᱩᱫᱤ (!fixits)
    ᱨᱩᱣᱟᱹᱲ;

  /* This is not unlike what is done in location_caret.  */
  uniqstr input = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  var_na buf[256];
  size_t len = parimaa_na (buf);
  var_na *backup = asnprintf (buf, &len, "%s~", input);
  ᱡᱩᱫᱤ (!backup)
    xalloc_die ();
  ᱡᱩᱫᱤ (rename (input, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (input));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (input, "w");
  size_t line = 1;
  size_t offset = 1;
  void const *p = NULL;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  ᱡᱚᱠᱷᱚᱱ (gl_list_iterator_next (&iter, &p, NULL))
    {
      fixit const *f = p;
      /* Look for the correct line. */
      ᱡᱚᱠᱷᱚᱱ (line < f->location.start.line)
        {
          puur_naa_nka c = getc (in);
          ᱡᱩᱫᱤ (c == EOF)
            bha_nga;
          ᱡᱩᱫᱤ (c == '\n')
            {
              ++line;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      ᱡᱚᱠᱷᱚᱱ (offset < f->location.start.byte)
        {
          puur_naa_nka c = getc (in);
          ᱡᱩᱫᱤ (c == EOF)
            bha_nga;
          ++offset;
          ᱡᱩᱫᱤ (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      ᱡᱩᱫᱤ (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      ᱡᱚᱠᱷᱚᱱ (line < f->location.end.line)
        {
          puur_naa_nka c = getc (in);
          ᱡᱩᱫᱤ (c == EOF)
            bha_nga;
          ᱡᱩᱫᱤ (c == '\n')
            {
              ++line;
              offset = 1;
            }
        }
      ᱡᱚᱠᱷᱚᱱ (offset < f->location.end.byte)
        {
          puur_naa_nka c = getc (in);
          ᱡᱩᱫᱤ (c == EOF)
            bha_nga;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      ᱡᱩᱫᱤ (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          puur_naa_nka c = getc (in);
          ᱡᱩᱫᱤ (c == EOF)
            bha_nga;
          ᱵᱟᱝ ᱡᱩᱫᱤ (c == '\n')
            {
              ++line;
              offset = 1;
            }
          ᱵᱟᱝ
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    puur_naa_nka c;
    ᱡᱚᱠᱷᱚᱱ ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, input), quote_n (1, backup));
  ᱡᱩᱫᱤ (backup != buf)
    free (backup);
}


/* Free the registered fixits.  */
void fixits_free (void)
{
  ᱡᱩᱫᱤ (fixits)
    {
      gl_list_free (fixits);
      fixits = NULL;
    }
}
