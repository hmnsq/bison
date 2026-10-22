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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ "fixits.h"

#ଅନ୍ତର୍ଭାଵ <error.h>
#ଅନ୍ତର୍ଭାଵ <get-errno.h>
#ଅନ୍ତର୍ଭାଵ <gl_array_list.h>
#ଅନ୍ତର୍ଭାଵ <gl_xlist.h>
#ଅନ୍ତର୍ଭାଵ <progname.h>
#ଅନ୍ତର୍ଭାଵ <quote.h>
#ଅନ୍ତର୍ଭାଵ <quotearg.h>
#ଅନ୍ତର୍ଭାଵ <vasnprintf.h>

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "files.h"
#ଅନ୍ତର୍ଭାଵ "getargs.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  location location;
  ଅକ୍ଷର *fix;
} fixit;

gl_list_t fixits = ଖାଲି;

ସ୍ଥିର fixit *
fixit_new (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ* fix)
{
  fixit *res = xmalloc (ଆକାର *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
fixit_cmp (ଅଚଳ  fixit *a, ଅଚଳ fixit *b)
{
  ଫେରନ୍ତୁ location_cmp (a->location, b->location);
}

ସ୍ଥିର ଶୂନ୍ୟ
fixit_free (fixit *f)
{
  ମୁକ୍ତ (f->fix);
  ମୁକ୍ତ (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
ସ୍ଥିର ଶୂନ୍ୟ
fixit_print (fixit ଅଚଳ *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.ଧାଡ଼ି, f->location.start.byte,
           f->location.end.ଧାଡ଼ି, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


ଶୂନ୍ୟ
fixits_register (location ଅଚଳ *loc, ଅକ୍ଷର ଅଚଳ* fix)
{
  ଯଦି (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ ଖାଲି,
                                   /* hashcode */ ଖାଲି,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  ଯଦି (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ !fixits;
}


ଶୂନ୍ୟ
fixits_run (ଶୂନ୍ୟ)
{
  ଯଦି (!fixits)
    ଫେରନ୍ତୁ;

  /* This is not unlike what is done in location_caret.  */
  uniqstr ନିବେଶ = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  ଅକ୍ଷର buf[256];
  size_t len = ଆକାର (buf);
  ଅକ୍ଷର *backup = asnprintf (buf, &len, "%s~", ନିବେଶ);
  ଯଦି (!backup)
    xalloc_die ();
  ଯଦି (rename (ନିବେଶ, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (ନିବେଶ));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (ନିବେଶ, "w");
  size_t ଧାଡ଼ି = 1;
  size_t offset = 1;
  ଶୂନ୍ୟ ଅଚଳ *p = ଖାଲି;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  ଯେତେବେଳେ (gl_list_iterator_next (&iter, &p, ଖାଲି))
    {
      fixit ଅଚଳ *f = p;
      /* Look for the correct line. */
      ଯେତେବେଳେ (ଧାଡ଼ି < f->location.start.ଧାଡ଼ି)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getc (in);
          ଯଦି (c == EOF)
            ଭାଙ୍ଗ;
          ଯଦି (c == '\n')
            {
              ++ଧାଡ଼ି;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      ଯେତେବେଳେ (offset < f->location.start.byte)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getc (in);
          ଯଦି (c == EOF)
            ଭାଙ୍ଗ;
          ++offset;
          ଯଦି (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      ଯଦି (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      ଯେତେବେଳେ (ଧାଡ଼ି < f->location.end.ଧାଡ଼ି)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getc (in);
          ଯଦି (c == EOF)
            ଭାଙ୍ଗ;
          ଯଦି (c == '\n')
            {
              ++ଧାଡ଼ି;
              offset = 1;
            }
        }
      ଯେତେବେଳେ (offset < f->location.end.byte)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getc (in);
          ଯଦି (c == EOF)
            ଭାଙ୍ଗ;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      ଯଦି (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getc (in);
          ଯଦି (c == EOF)
            ଭାଙ୍ଗ;
          ଅନ୍ୟଥା ଯଦି (c == '\n')
            {
              ++ଧାଡ଼ି;
              offset = 1;
            }
          ଅନ୍ୟଥା
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
    ଯେତେବେଳେ ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, ନିବେଶ), quote_n (1, backup));
  ଯଦି (backup != buf)
    ମୁକ୍ତ (backup);
}


/* Free the registered fixits.  */
ଶୂନ୍ୟ fixits_free (ଶୂନ୍ୟ)
{
  ଯଦି (fixits)
    {
      gl_list_free (fixits);
      fixits = ଖାଲି;
    }
}
