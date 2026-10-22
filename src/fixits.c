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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব "fixits.h"

#অন্তর্ভাব <error.h>
#অন্তর্ভাব <get-errno.h>
#অন্তর্ভাব <gl_array_list.h>
#অন্তর্ভাব <gl_xlist.h>
#অন্তর্ভাব <progname.h>
#অন্তর্ভাব <quote.h>
#অন্তর্ভাব <quotearg.h>
#অন্তর্ভাব <vasnprintf.h>

#অন্তর্ভাব "system.h"

#অন্তর্ভাব "files.h"
#অন্তর্ভাব "getargs.h"

প্রকারলেখ কাঠামো
{
  location location;
  অক্ষর *fix;
} fixit;

gl_list_t fixits = ফাঁকা;

স্থির fixit *
fixit_new (location ধ্রুবক *loc, অক্ষর ধ্রুবক* fix)
{
  fixit *res = xmalloc (আকার *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  ফেরত res;
}

স্থির পূর্ণসংখ্যা
fixit_cmp (ধ্রুবক  fixit *a, ধ্রুবক fixit *b)
{
  ফেরত location_cmp (a->location, b->location);
}

স্থির শূন্য
fixit_free (fixit *f)
{
  মুক্ত (f->fix);
  মুক্ত (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
স্থির শূন্য
fixit_print (fixit ধ্রুবক *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.সারি, f->location.start.byte,
           f->location.end.সারি, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


শূন্য
fixits_register (location ধ্রুবক *loc, অক্ষর ধ্রুবক* fix)
{
  যদি (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ ফাঁকা,
                                   /* hashcode */ ফাঁকা,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  যদি (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (শূন্য)
{
  ফেরত !fixits;
}


শূন্য
fixits_run (শূন্য)
{
  যদি (!fixits)
    ফেরত;

  /* This is not unlike what is done in location_caret.  */
  uniqstr প্রবেশ = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  অক্ষর buf[256];
  size_t len = আকার (buf);
  অক্ষর *backup = asnprintf (buf, &len, "%s~", প্রবেশ);
  যদি (!backup)
    xalloc_die ();
  যদি (rename (প্রবেশ, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (প্রবেশ));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (প্রবেশ, "w");
  size_t সারি = 1;
  size_t offset = 1;
  শূন্য ধ্রুবক *p = ফাঁকা;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  যখন (gl_list_iterator_পরবর্তী (&iter, &p, ফাঁকা))
    {
      fixit ধ্রুবক *f = p;
      /* Look for the correct line. */
      যখন (সারি < f->location.start.সারি)
        {
          পূর্ণসংখ্যা c = getc (in);
          যদি (c == EOF)
            বিরতি;
          যদি (c == '\n')
            {
              ++সারি;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      যখন (offset < f->location.start.byte)
        {
          পূর্ণসংখ্যা c = getc (in);
          যদি (c == EOF)
            বিরতি;
          ++offset;
          যদি (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      যদি (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      যখন (সারি < f->location.end.সারি)
        {
          পূর্ণসংখ্যা c = getc (in);
          যদি (c == EOF)
            বিরতি;
          যদি (c == '\n')
            {
              ++সারি;
              offset = 1;
            }
        }
      যখন (offset < f->location.end.byte)
        {
          পূর্ণসংখ্যা c = getc (in);
          যদি (c == EOF)
            বিরতি;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      যদি (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          পূর্ণসংখ্যা c = getc (in);
          যদি (c == EOF)
            বিরতি;
          অন্যথা যদি (c == '\n')
            {
              ++সারি;
              offset = 1;
            }
          অন্যথা
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    পূর্ণসংখ্যা c;
    যখন ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, প্রবেশ), quote_n (1, backup));
  যদি (backup != buf)
    মুক্ত (backup);
}


/* Free the registered fixits.  */
শূন্য fixits_free (শূন্য)
{
  যদি (fixits)
    {
      gl_list_free (fixits);
      fixits = ফাঁকা;
    }
}
