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

প্রকারনির্দেশ সংরচনা
{
  location location;
  বর্ণ *fix;
} fixit;

gl_list_t fixits = রিক্ত;

স্থির fixit *
fixit_new (location ধ্রুব *loc, বর্ণ ধ্রুব* fix)
{
  fixit *res = xmalloc (পরিমাণ *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক
fixit_cmp (ধ্রুব  fixit *a, ধ্রুব fixit *b)
{
  প্রত্যাগম location_cmp (a->location, b->location);
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
fixit_print (fixit ধ্রুব *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.সারি, f->location.start.byte,
           f->location.end.সারি, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


শূন্য
fixits_register (location ধ্রুব *loc, বর্ণ ধ্রুব* fix)
{
  যদি (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ রিক্ত,
                                   /* hashcode */ রিক্ত,
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
  প্রত্যাগম !fixits;
}


শূন্য
fixits_run (শূন্য)
{
  যদি (!fixits)
    প্রত্যাগম;

  /* This is not unlike what is done in location_caret.  */
  uniqstr প্রবেশ = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  বর্ণ buf[256];
  size_t len = পরিমাণ (buf);
  বর্ণ *backup = asnprintf (buf, &len, "%s~", প্রবেশ);
  যদি (!backup)
    xalloc_die ();
  যদি (rename (প্রবেশ, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (প্রবেশ));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (প্রবেশ, "w");
  size_t সারি = 1;
  size_t offset = 1;
  শূন্য ধ্রুব *p = রিক্ত;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  যাবত্ (gl_list_iterator_next (&iter, &p, রিক্ত))
    {
      fixit ধ্রুব *f = p;
      /* Look for the correct line. */
      যাবত্ (সারি < f->location.start.সারি)
        {
          পূর্ণাঙ্ক c = getc (in);
          যদি (c == EOF)
            ভঙ্গ;
          যদি (c == '\n')
            {
              ++সারি;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      যাবত্ (offset < f->location.start.byte)
        {
          পূর্ণাঙ্ক c = getc (in);
          যদি (c == EOF)
            ভঙ্গ;
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
      যাবত্ (সারি < f->location.end.সারি)
        {
          পূর্ণাঙ্ক c = getc (in);
          যদি (c == EOF)
            ভঙ্গ;
          যদি (c == '\n')
            {
              ++সারি;
              offset = 1;
            }
        }
      যাবত্ (offset < f->location.end.byte)
        {
          পূর্ণাঙ্ক c = getc (in);
          যদি (c == EOF)
            ভঙ্গ;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      যদি (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          পূর্ণাঙ্ক c = getc (in);
          যদি (c == EOF)
            ভঙ্গ;
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
    পূর্ণাঙ্ক c;
    যাবত্ ((c = getc (in)) != EOF)
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
      fixits = রিক্ত;
    }
}
