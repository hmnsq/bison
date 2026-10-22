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

#अन्तर्भाव <config.h>

#अन्तर्भाव "fixits.h"

#अन्तर्भाव <error.h>
#अन्तर्भाव <get-errno.h>
#अन्तर्भाव <gl_array_list.h>
#अन्तर्भाव <gl_xlist.h>
#अन्तर्भाव <progname.h>
#अन्तर्भाव <quote.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <vasnprintf.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "files.h"
#अन्तर्भाव "getargs.h"

प्रकारनिर्देश संरचना
{
  location location;
  वर्ण *fix;
} fixit;

gl_list_t fixits = रिक्त;

स्थिर fixit *
fixit_new (location ध्रुव *loc, वर्ण ध्रुव* fix)
{
  fixit *res = xmalloc (परिमाण *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  प्रत्यागम res;
}

स्थिर पूर्णाङ्क
fixit_cmp (ध्रुव  fixit *a, ध्रुव fixit *b)
{
  प्रत्यागम location_cmp (a->location, b->location);
}

स्थिर शून्य
fixit_free (fixit *f)
{
  मुक्त (f->fix);
  मुक्त (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
स्थिर शून्य
fixit_print (fixit ध्रुव *f, FILE *बहिर्गमनम्)
{
  fprintf (बहिर्गमनम्, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.पङ्क्तिः, f->location.start.byte,
           f->location.अन्तः.पङ्क्तिः, f->location.अन्तः.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


शून्य
fixits_पञ्जिकाकोशः (location ध्रुव *loc, वर्ण ध्रुव* fix)
{
  यदि (!fixits)
    fixits = gl_list_create_रिक्तानि (GL_ARRAY_LIST,
                                   /* equals */ रिक्त,
                                   /* hashcode */ रिक्त,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_सङ्कलय (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  यदि (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_रिक्तानि (शून्य)
{
  प्रत्यागम !fixits;
}


शून्य
fixits_सततम् (शून्य)
{
  यदि (!fixits)
    प्रत्यागम;

  /* This is not unlike what is done in location_caret.  */
  uniqstr निवेशः = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  वर्ण buf[256];
  size_t len = परिमाण (buf);
  वर्ण *backup = asnprintf (buf, &len, "%s~", निवेशः);
  यदि (!backup)
    xalloc_die ();
  यदि (rename (निवेशः, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (निवेशः));
  FILE *अन्तर् = xfopen (backup, "r");
  FILE *बहिर्गमनम् = xfopen (निवेशः, "w");
  size_t पङ्क्तिः = 1;
  size_t विस्थापनम् = 1;
  शून्य ध्रुव *p = रिक्त;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  यावत् (gl_list_iterator_अग्र (&iter, &p, रिक्त))
    {
      fixit ध्रुव *f = p;
      /* Look for the correct line. */
      यावत् (पङ्क्तिः < f->location.start.पङ्क्तिः)
        {
          पूर्णाङ्क c = getc (अन्तर्);
          यदि (c == EOF)
            भङ्ग;
          यदि (c == '\n')
            {
              ++पङ्क्तिः;
              विस्थापनम् = 1;
            }
          putc (c, बहिर्गमनम्);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      यावत् (विस्थापनम् < f->location.start.byte)
        {
          पूर्णाङ्क c = getc (अन्तर्);
          यदि (c == EOF)
            भङ्ग;
          ++विस्थापनम्;
          यदि (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, बहिर्गमनम्);
        }

      /* Paste the fix instead. */
      fputs (f->fix, बहिर्गमनम्);

      /* Maybe install the eol afterwards.  */
      यदि (need_eol)
        putc ('\n', बहिर्गमनम्);

      /* Skip the bad input. */
      यावत् (पङ्क्तिः < f->location.अन्तः.पङ्क्तिः)
        {
          पूर्णाङ्क c = getc (अन्तर्);
          यदि (c == EOF)
            भङ्ग;
          यदि (c == '\n')
            {
              ++पङ्क्तिः;
              विस्थापनम् = 1;
            }
        }
      यावत् (विस्थापनम् < f->location.अन्तः.byte)
        {
          पूर्णाङ्क c = getc (अन्तर्);
          यदि (c == EOF)
            भङ्ग;
          ++विस्थापनम्;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      यदि (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          पूर्णाङ्क c = getc (अन्तर्);
          यदि (c == EOF)
            भङ्ग;
          अन्यथा यदि (c == '\n')
            {
              ++पङ्क्तिः;
              विस्थापनम् = 1;
            }
          अन्यथा
            ungetc (c, अन्तर्);
        }
    }
  /* Paste the rest of the file.  */
  {
    पूर्णाङ्क c;
    यावत् ((c = getc (अन्तर्)) != EOF)
      putc (c, बहिर्गमनम्);
  }

  gl_list_iterator_free (&iter);
  xfclose (बहिर्गमनम्);
  xfclose (अन्तर्);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           क्रमादेशः_name, quote_n (0, निवेशः), quote_n (1, backup));
  यदि (backup != buf)
    मुक्त (backup);
}


/* Free the registered fixits.  */
शून्य fixits_free (शून्य)
{
  यदि (fixits)
    {
      gl_list_free (fixits);
      fixits = रिक्त;
    }
}
