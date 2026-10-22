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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ "fixits.h"

#અન્તર્ભાવ <error.h>
#અન્તર્ભાવ <get-errno.h>
#અન્તર્ભાવ <gl_array_list.h>
#અન્તર્ભાવ <gl_xlist.h>
#અન્તર્ભાવ <progname.h>
#અન્તર્ભાવ <quote.h>
#અન્તર્ભાવ <quotearg.h>
#અન્તર્ભાવ <vasnprintf.h>

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "files.h"
#અન્તર્ભાવ "getargs.h"

પ્રકારવ્યાખ્યા સંરચના
{
  location location;
  અક્ષર *fix;
} fixit;

gl_list_t fixits = ખાલી;

સ્થિર fixit *
fixit_new (location અચલ *loc, અક્ષર અચલ* fix)
{
  fixit *res = xmalloc (કદ *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  પાછા res;
}

સ્થિર પૂર્ણાંક
fixit_cmp (અચલ  fixit *a, અચલ fixit *b)
{
  પાછા location_cmp (a->location, b->location);
}

સ્થિર શૂન્ય
fixit_free (fixit *f)
{
  મુક્ત (f->fix);
  મુક્ત (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
સ્થિર શૂન્ય
fixit_print (fixit અચલ *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.લીટી, f->location.start.byte,
           f->location.end.લીટી, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


શૂન્ય
fixits_register (location અચલ *loc, અક્ષર અચલ* fix)
{
  જો (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ ખાલી,
                                   /* hashcode */ ખાલી,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  જો (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (શૂન્ય)
{
  પાછા !fixits;
}


શૂન્ય
fixits_run (શૂન્ય)
{
  જો (!fixits)
    પાછા;

  /* This is not unlike what is done in location_caret.  */
  uniqstr આગત = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  અક્ષર buf[256];
  size_t len = કદ (buf);
  અક્ષર *backup = asnprintf (buf, &len, "%s~", આગત);
  જો (!backup)
    xalloc_die ();
  જો (rename (આગત, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (આગત));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (આગત, "w");
  size_t લીટી = 1;
  size_t offset = 1;
  શૂન્ય અચલ *p = ખાલી;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  જ્યારે (gl_list_iterator_next (&iter, &p, ખાલી))
    {
      fixit અચલ *f = p;
      /* Look for the correct line. */
      જ્યારે (લીટી < f->location.start.લીટી)
        {
          પૂર્ણાંક c = getc (in);
          જો (c == EOF)
            તોડો;
          જો (c == '\n')
            {
              ++લીટી;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      જ્યારે (offset < f->location.start.byte)
        {
          પૂર્ણાંક c = getc (in);
          જો (c == EOF)
            તોડો;
          ++offset;
          જો (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      જો (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      જ્યારે (લીટી < f->location.end.લીટી)
        {
          પૂર્ણાંક c = getc (in);
          જો (c == EOF)
            તોડો;
          જો (c == '\n')
            {
              ++લીટી;
              offset = 1;
            }
        }
      જ્યારે (offset < f->location.end.byte)
        {
          પૂર્ણાંક c = getc (in);
          જો (c == EOF)
            તોડો;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      જો (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          પૂર્ણાંક c = getc (in);
          જો (c == EOF)
            તોડો;
          નહીંતર જો (c == '\n')
            {
              ++લીટી;
              offset = 1;
            }
          નહીંતર
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    પૂર્ણાંક c;
    જ્યારે ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, આગત), quote_n (1, backup));
  જો (backup != buf)
    મુક્ત (backup);
}


/* Free the registered fixits.  */
શૂન્ય fixits_free (શૂન્ય)
{
  જો (fixits)
    {
      gl_list_free (fixits);
      fixits = ખાલી;
    }
}
