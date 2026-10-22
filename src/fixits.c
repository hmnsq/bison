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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ "fixits.h"

#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <get-errno.h>
#ಅನ್ತರ್ಭಾವ <gl_array_list.h>
#ಅನ್ತರ್ಭಾವ <gl_xlist.h>
#ಅನ್ತರ್ಭಾವ <progname.h>
#ಅನ್ತರ್ಭಾವ <quote.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <vasnprintf.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "files.h"
#ಅನ್ತರ್ಭಾವ "getargs.h"

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  location location;
  ವರ್ಣ *fix;
} fixit;

gl_list_t fixits = ರಿಕ್ತ;

ಸ್ಥಿರ fixit *
fixit_new (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ* fix)
{
  fixit *res = xmalloc (ಪರಿಮಾಣ *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fixit_cmp (ಧ್ರುವ  fixit *a, ಧ್ರುವ fixit *b)
{
  ಪ್ರತ್ಯಾಗಮ location_cmp (a->location, b->location);
}

ಸ್ಥಿರ ಶೂನ್ಯ
fixit_free (fixit *f)
{
  ಮುಕ್ತ (f->fix);
  ಮುಕ್ತ (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
ಸ್ಥಿರ ಶೂನ್ಯ
fixit_print (fixit ಧ್ರುವ *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.ಸಾಲು, f->location.start.byte,
           f->location.end.ಸಾಲು, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


ಶೂನ್ಯ
fixits_register (location ಧ್ರುವ *loc, ವರ್ಣ ಧ್ರುವ* fix)
{
  ಯದಿ (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ ರಿಕ್ತ,
                                   /* hashcode */ ರಿಕ್ತ,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  ಯದಿ (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ !fixits;
}


ಶೂನ್ಯ
fixits_run (ಶೂನ್ಯ)
{
  ಯದಿ (!fixits)
    ಪ್ರತ್ಯಾಗಮ;

  /* This is not unlike what is done in location_caret.  */
  uniqstr ಒಳಹರಿವು = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  ವರ್ಣ buf[256];
  size_t len = ಪರಿಮಾಣ (buf);
  ವರ್ಣ *backup = asnprintf (buf, &len, "%s~", ಒಳಹರಿವು);
  ಯದಿ (!backup)
    xalloc_die ();
  ಯದಿ (rename (ಒಳಹರಿವು, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (ಒಳಹರಿವು));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (ಒಳಹರಿವು, "w");
  size_t ಸಾಲು = 1;
  size_t offset = 1;
  ಶೂನ್ಯ ಧ್ರುವ *p = ರಿಕ್ತ;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  ಯಾವತ್ (gl_list_iterator_next (&iter, &p, ರಿಕ್ತ))
    {
      fixit ಧ್ರುವ *f = p;
      /* Look for the correct line. */
      ಯಾವತ್ (ಸಾಲು < f->location.start.ಸಾಲು)
        {
          ಪೂರ್ಣಾಙ್ಕ c = getc (in);
          ಯದಿ (c == EOF)
            ಭಙ್ಗ;
          ಯದಿ (c == '\n')
            {
              ++ಸಾಲು;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      ಯಾವತ್ (offset < f->location.start.byte)
        {
          ಪೂರ್ಣಾಙ್ಕ c = getc (in);
          ಯದಿ (c == EOF)
            ಭಙ್ಗ;
          ++offset;
          ಯದಿ (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      ಯದಿ (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      ಯಾವತ್ (ಸಾಲು < f->location.end.ಸಾಲು)
        {
          ಪೂರ್ಣಾಙ್ಕ c = getc (in);
          ಯದಿ (c == EOF)
            ಭಙ್ಗ;
          ಯದಿ (c == '\n')
            {
              ++ಸಾಲು;
              offset = 1;
            }
        }
      ಯಾವತ್ (offset < f->location.end.byte)
        {
          ಪೂರ್ಣಾಙ್ಕ c = getc (in);
          ಯದಿ (c == EOF)
            ಭಙ್ಗ;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      ಯದಿ (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          ಪೂರ್ಣಾಙ್ಕ c = getc (in);
          ಯದಿ (c == EOF)
            ಭಙ್ಗ;
          ಅನ್ಯಥಾ ಯದಿ (c == '\n')
            {
              ++ಸಾಲು;
              offset = 1;
            }
          ಅನ್ಯಥಾ
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    ಪೂರ್ಣಾಙ್ಕ c;
    ಯಾವತ್ ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, ಒಳಹರಿವು), quote_n (1, backup));
  ಯದಿ (backup != buf)
    ಮುಕ್ತ (backup);
}


/* Free the registered fixits.  */
ಶೂನ್ಯ fixits_free (ಶೂನ್ಯ)
{
  ಯದಿ (fixits)
    {
      gl_list_free (fixits);
      fixits = ರಿಕ್ತ;
    }
}
