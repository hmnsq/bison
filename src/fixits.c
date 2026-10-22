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

#شامل <config.h>

#شامل "fixits.h"

#شامل <error.h>
#شامل <get-errno.h>
#شامل <gl_array_list.h>
#شامل <gl_xlist.h>
#شامل <progname.h>
#شامل <quote.h>
#شامل <quotearg.h>
#شامل <vasnprintf.h>

#شامل "system.h"

#شامل "files.h"
#شامل "getargs.h"

تعریفنوع ساختار
{
  location location;
  نویسه *fix;
} fixit;

gl_list_t fixits = پوچ;

ایستا fixit *
fixit_new (location ثابت *loc, نویسه ثابت* fix)
{
  fixit *res = xmalloc (اندازه *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  بازگشت res;
}

ایستا صحیح
fixit_cmp (ثابت  fixit *a, ثابت fixit *b)
{
  بازگشت location_cmp (a->location, b->location);
}

ایستا تهی
fixit_free (fixit *f)
{
  آزاد (f->fix);
  آزاد (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
ایستا تهی
fixit_print (fixit ثابت *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.سطر, f->location.start.byte,
           f->location.end.سطر, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


تهی
fixits_register (location ثابت *loc, نویسه ثابت* fix)
{
  اگر (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ پوچ,
                                   /* hashcode */ پوچ,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  اگر (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (تهی)
{
  بازگشت !fixits;
}


تهی
fixits_run (تهی)
{
  اگر (!fixits)
    بازگشت;

  /* This is not unlike what is done in location_caret.  */
  uniqstr درون_داد = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  نویسه buf[256];
  size_t len = اندازه (buf);
  نویسه *backup = asnprintf (buf, &len, "%s~", درون_داد);
  اگر (!backup)
    xalloc_die ();
  اگر (rename (درون_داد, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (درون_داد));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (درون_داد, "w");
  size_t سطر = 1;
  size_t offset = 1;
  تهی ثابت *p = پوچ;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  وقتی (gl_list_iterator_next (&iter, &p, پوچ))
    {
      fixit ثابت *f = p;
      /* Look for the correct line. */
      وقتی (سطر < f->location.start.سطر)
        {
          صحیح c = getc (in);
          اگر (c == EOF)
            بشکن;
          اگر (c == '\n')
            {
              ++سطر;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      وقتی (offset < f->location.start.byte)
        {
          صحیح c = getc (in);
          اگر (c == EOF)
            بشکن;
          ++offset;
          اگر (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      اگر (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      وقتی (سطر < f->location.end.سطر)
        {
          صحیح c = getc (in);
          اگر (c == EOF)
            بشکن;
          اگر (c == '\n')
            {
              ++سطر;
              offset = 1;
            }
        }
      وقتی (offset < f->location.end.byte)
        {
          صحیح c = getc (in);
          اگر (c == EOF)
            بشکن;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      اگر (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          صحیح c = getc (in);
          اگر (c == EOF)
            بشکن;
          وگرنه اگر (c == '\n')
            {
              ++سطر;
              offset = 1;
            }
          وگرنه
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    صحیح c;
    وقتی ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, درون_داد), quote_n (1, backup));
  اگر (backup != buf)
    آزاد (backup);
}


/* Free the registered fixits.  */
تهی fixits_free (تهی)
{
  اگر (fixits)
    {
      gl_list_free (fixits);
      fixits = پوچ;
    }
}
