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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ "fixits.h"

#அந்தர்பாவ <error.h>
#அந்தர்பாவ <get-errno.h>
#அந்தர்பாவ <gl_array_list.h>
#அந்தர்பாவ <gl_xlist.h>
#அந்தர்பாவ <progname.h>
#அந்தர்பாவ <quote.h>
#அந்தர்பாவ <quotearg.h>
#அந்தர்பாவ <vasnprintf.h>

#அந்தர்பாவ "system.h"

#அந்தர்பாவ "files.h"
#அந்தர்பாவ "getargs.h"

வகைவரை கட்டமைப்பு
{
  location location;
  எழுத்து *fix;
} fixit;

gl_list_t fixits = வெற்று;

நிலையான fixit *
fixit_new (location மாறிலி *loc, எழுத்து மாறிலி* fix)
{
  fixit *res = xmalloc (அளவு *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  திரும்பு res;
}

நிலையான முழு
fixit_cmp (மாறிலி  fixit *a, மாறிலி fixit *b)
{
  திரும்பு location_cmp (a->location, b->location);
}

நிலையான வெறுமை
fixit_free (fixit *f)
{
  விடுவி (f->fix);
  விடுவி (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
நிலையான வெறுமை
fixit_print (fixit மாறிலி *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.வரி, f->location.start.byte,
           f->location.end.வரி, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


வெறுமை
fixits_register (location மாறிலி *loc, எழுத்து மாறிலி* fix)
{
  எனில் (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ வெற்று,
                                   /* hashcode */ வெற்று,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  எனில் (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (வெறுமை)
{
  திரும்பு !fixits;
}


வெறுமை
fixits_run (வெறுமை)
{
  எனில் (!fixits)
    திரும்பு;

  /* This is not unlike what is done in location_caret.  */
  uniqstr உள்ளீடு = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  எழுத்து buf[256];
  size_t len = அளவு (buf);
  எழுத்து *backup = asnprintf (buf, &len, "%s~", உள்ளீடு);
  எனில் (!backup)
    xalloc_die ();
  எனில் (rename (உள்ளீடு, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (உள்ளீடு));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (உள்ளீடு, "w");
  size_t வரி = 1;
  size_t offset = 1;
  வெறுமை மாறிலி *p = வெற்று;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  போது (gl_list_iterator_next (&iter, &p, வெற்று))
    {
      fixit மாறிலி *f = p;
      /* Look for the correct line. */
      போது (வரி < f->location.start.வரி)
        {
          முழு c = getc (in);
          எனில் (c == EOF)
            நிறுத்து;
          எனில் (c == '\n')
            {
              ++வரி;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      போது (offset < f->location.start.byte)
        {
          முழு c = getc (in);
          எனில் (c == EOF)
            நிறுத்து;
          ++offset;
          எனில் (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      எனில் (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      போது (வரி < f->location.end.வரி)
        {
          முழு c = getc (in);
          எனில் (c == EOF)
            நிறுத்து;
          எனில் (c == '\n')
            {
              ++வரி;
              offset = 1;
            }
        }
      போது (offset < f->location.end.byte)
        {
          முழு c = getc (in);
          எனில் (c == EOF)
            நிறுத்து;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      எனில் (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          முழு c = getc (in);
          எனில் (c == EOF)
            நிறுத்து;
          இல்லையெனில் எனில் (c == '\n')
            {
              ++வரி;
              offset = 1;
            }
          இல்லையெனில்
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    முழு c;
    போது ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, உள்ளீடு), quote_n (1, backup));
  எனில் (backup != buf)
    விடுவி (backup);
}


/* Free the registered fixits.  */
வெறுமை fixits_free (வெறுமை)
{
  எனில் (fixits)
    {
      gl_list_free (fixits);
      fixits = வெற்று;
    }
}
