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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  location location;
  ಅಕ್ಷರ *fix;
} fixit;

gl_list_t fixits = ಖಾಲಿ;

ಸ್ಥಿರ fixit *
fixit_new (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ* fix)
{
  fixit *res = xmalloc (ಗಾತ್ರ *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fixit_cmp (ಸ್ಥಿರಾಂಕ  fixit *a, ಸ್ಥಿರಾಂಕ fixit *b)
{
  ಹಿಂತಿರುಗಿ location_cmp (a->location, b->location);
}

ಸ್ಥಿರ ಶೂನ್ಯ
fixit_free (fixit *f)
{
  ಬಿಡುಗಡೆ (f->fix);
  ಬಿಡುಗಡೆ (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
ಸ್ಥಿರ ಶೂನ್ಯ
fixit_print (fixit ಸ್ಥಿರಾಂಕ *f, FILE *ಹೊರಹೋಗು)
{
  fprintf (ಹೊರಹೋಗು, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.ಆರಂಭ.file),
           f->location.ಆರಂಭ.ಸಾಲು, f->location.ಆರಂಭ.byte,
           f->location.ಕೊನೆ.ಸಾಲು, f->location.ಕೊನೆ.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


ಶೂನ್ಯ
fixits_ರಿಜಿಸ್ಟರ್ (location ಸ್ಥಿರಾಂಕ *loc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ* fix)
{
  ಆದರೆ (!fixits)
    fixits = gl_list_create_ಖಾಲಿಗಳು (GL_ARRAY_LIST,
                                   /* equals */ ಖಾಲಿ,
                                   /* hashcode */ ಖಾಲಿ,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_ಕೂಡಿಸು (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  ಆದರೆ (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_ಖಾಲಿಗಳು (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ !fixits;
}


ಶೂನ್ಯ
fixits_ಸತತ (ಶೂನ್ಯ)
{
  ಆದರೆ (!fixits)
    ಹಿಂತಿರುಗಿ;

  /* This is not unlike what is done in location_caret.  */
  uniqstr ಒಳಹರಿವು = ((fixit *) gl_list_get_at (fixits, 0))->location.ಆರಂಭ.file;
  /* Backup the file. */
  ಅಕ್ಷರ buf[256];
  size_t len = ಗಾತ್ರ (buf);
  ಅಕ್ಷರ *backup = asnprintf (buf, &len, "%s~", ಒಳಹರಿವು);
  ಆದರೆ (!backup)
    xalloc_die ();
  ಆದರೆ (rename (ಒಳಹರಿವು, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (ಒಳಹರಿವು));
  FILE *ಒಳ = xfopen (backup, "r");
  FILE *ಹೊರಹೋಗು = xfopen (ಒಳಹರಿವು, "w");
  size_t ಸಾಲು = 1;
  size_t ಸ್ಥಾನಾಂತರ = 1;
  ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *p = ಖಾಲಿ;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  ಆಗ (gl_list_iterator_ಮುಂದಿನ (&iter, &p, ಖಾಲಿ))
    {
      fixit ಸ್ಥಿರಾಂಕ *f = p;
      /* Look for the correct line. */
      ಆಗ (ಸಾಲು < f->location.ಆರಂಭ.ಸಾಲು)
        {
          ಪೂರ್ಣಾಂಕ c = getc (ಒಳ);
          ಆದರೆ (c == EOF)
            ನಿಲ್ಲಿಸು;
          ಆದರೆ (c == '\n')
            {
              ++ಸಾಲು;
              ಸ್ಥಾನಾಂತರ = 1;
            }
          putc (c, ಹೊರಹೋಗು);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      ಆಗ (ಸ್ಥಾನಾಂತರ < f->location.ಆರಂಭ.byte)
        {
          ಪೂರ್ಣಾಂಕ c = getc (ಒಳ);
          ಆದರೆ (c == EOF)
            ನಿಲ್ಲಿಸು;
          ++ಸ್ಥಾನಾಂತರ;
          ಆದರೆ (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, ಹೊರಹೋಗು);
        }

      /* Paste the fix instead. */
      fputs (f->fix, ಹೊರಹೋಗು);

      /* Maybe install the eol afterwards.  */
      ಆದರೆ (need_eol)
        putc ('\n', ಹೊರಹೋಗು);

      /* Skip the bad input. */
      ಆಗ (ಸಾಲು < f->location.ಕೊನೆ.ಸಾಲು)
        {
          ಪೂರ್ಣಾಂಕ c = getc (ಒಳ);
          ಆದರೆ (c == EOF)
            ನಿಲ್ಲಿಸು;
          ಆದರೆ (c == '\n')
            {
              ++ಸಾಲು;
              ಸ್ಥಾನಾಂತರ = 1;
            }
        }
      ಆಗ (ಸ್ಥಾನಾಂತರ < f->location.ಕೊನೆ.byte)
        {
          ಪೂರ್ಣಾಂಕ c = getc (ಒಳ);
          ಆದರೆ (c == EOF)
            ನಿಲ್ಲಿಸು;
          ++ಸ್ಥಾನಾಂತರ;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      ಆದರೆ (f->fix[0] == 0 && f->location.ಆರಂಭ.byte == 1)
        {
          ಪೂರ್ಣಾಂಕ c = getc (ಒಳ);
          ಆದರೆ (c == EOF)
            ನಿಲ್ಲಿಸು;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == '\n')
            {
              ++ಸಾಲು;
              ಸ್ಥಾನಾಂತರ = 1;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ungetc (c, ಒಳ);
        }
    }
  /* Paste the rest of the file.  */
  {
    ಪೂರ್ಣಾಂಕ c;
    ಆಗ ((c = getc (ಒಳ)) != EOF)
      putc (c, ಹೊರಹೋಗು);
  }

  gl_list_iterator_free (&iter);
  xfclose (ಹೊರಹೋಗು);
  xfclose (ಒಳ);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           ಕಾರ್ಯಕ್ರಮ_name, quote_n (0, ಒಳಹರಿವು), quote_n (1, backup));
  ಆದರೆ (backup != buf)
    ಬಿಡುಗಡೆ (backup);
}


/* Free the registered fixits.  */
ಶೂನ್ಯ fixits_free (ಶೂನ್ಯ)
{
  ಆದರೆ (fixits)
    {
      gl_list_free (fixits);
      fixits = ಖಾಲಿ;
    }
}
