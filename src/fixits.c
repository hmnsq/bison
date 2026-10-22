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

#ضمن <config.h>

#ضمن "fixits.h"

#ضمن <error.h>
#ضمن <get-errno.h>
#ضمن <gl_array_list.h>
#ضمن <gl_xlist.h>
#ضمن <progname.h>
#ضمن <quote.h>
#ضمن <quotearg.h>
#ضمن <vasnprintf.h>

#ضمن "system.h"

#ضمن "files.h"
#ضمن "getargs.h"

تعريفنوع بنية
{
  location location;
  حرف *fix;
} fixit;

gl_list_t fixits = عدم;

ساكن fixit *
fixit_new (location ثابت *loc, حرف ثابت* fix)
{
  fixit *res = xmalloc (حجم *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  أرجع res;
}

ساكن صحيح
fixit_cmp (ثابت  fixit *a, ثابت fixit *b)
{
  أرجع location_cmp (a->location, b->location);
}

ساكن فراغ
fixit_free (fixit *f)
{
  حرر (f->fix);
  حرر (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
ساكن فراغ
fixit_print (fixit ثابت *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.باريس, f->location.start.byte,
           f->location.end.باريس, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


فراغ
fixits_register (location ثابت *loc, حرف ثابت* fix)
{
  إذا (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ عدم,
                                   /* hashcode */ عدم,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  إذا (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (فراغ)
{
  أرجع !fixits;
}


فراغ
fixits_run (فراغ)
{
  إذا (!fixits)
    أرجع;

  /* This is not unlike what is done in location_caret.  */
  uniqstr ماسوقن = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  حرف buf[256];
  size_t len = حجم (buf);
  حرف *backup = asnprintf (buf, &len, "%s~", ماسوقن);
  إذا (!backup)
    xalloc_die ();
  إذا (rename (ماسوقن, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (ماسوقن));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (ماسوقن, "w");
  size_t باريس = 1;
  size_t offset = 1;
  فراغ ثابت *p = عدم;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  طالما (gl_list_iterator_next (&iter, &p, عدم))
    {
      fixit ثابت *f = p;
      /* Look for the correct line. */
      طالما (باريس < f->location.start.باريس)
        {
          صحيح c = getc (in);
          إذا (c == EOF)
            اكسر;
          إذا (c == '\n')
            {
              ++باريس;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      طالما (offset < f->location.start.byte)
        {
          صحيح c = getc (in);
          إذا (c == EOF)
            اكسر;
          ++offset;
          إذا (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      إذا (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      طالما (باريس < f->location.end.باريس)
        {
          صحيح c = getc (in);
          إذا (c == EOF)
            اكسر;
          إذا (c == '\n')
            {
              ++باريس;
              offset = 1;
            }
        }
      طالما (offset < f->location.end.byte)
        {
          صحيح c = getc (in);
          إذا (c == EOF)
            اكسر;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      إذا (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          صحيح c = getc (in);
          إذا (c == EOF)
            اكسر;
          وإلا إذا (c == '\n')
            {
              ++باريس;
              offset = 1;
            }
          وإلا
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    صحيح c;
    طالما ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, ماسوقن), quote_n (1, backup));
  إذا (backup != buf)
    حرر (backup);
}


/* Free the registered fixits.  */
فراغ fixits_free (فراغ)
{
  إذا (fixits)
    {
      gl_list_free (fixits);
      fixits = عدم;
    }
}
