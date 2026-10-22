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

प्रकारव्याख्या संरचना
{
  location location;
  अक्षर *fix;
} fixit;

gl_list_t fixits = रिक्त;

स्थिर fixit *
fixit_new (location स्थिरांक *loc, अक्षर स्थिरांक* fix)
{
  fixit *res = xmalloc (आकार *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  परत res;
}

स्थिर पूर्णांक
fixit_cmp (स्थिरांक  fixit *a, स्थिरांक fixit *b)
{
  परत location_cmp (a->location, b->location);
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
fixit_print (fixit स्थिरांक *f, खाता *out)
{
  ख_लिखो (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.ओळ, f->location.start.byte,
           f->location.end.ओळ, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


शून्य
fixits_register (location स्थिरांक *loc, अक्षर स्थिरांक* fix)
{
  जर (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ रिक्त,
                                   /* hashcode */ रिक्त,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  जर (feature_flag & feature_fixit)
    fixit_print (f, मानक_त्रुटि);
}


bool
fixits_empty (शून्य)
{
  परत !fixits;
}


शून्य
fixits_run (शून्य)
{
  जर (!fixits)
    परत;

  /* This is not unlike what is done in location_caret.  */
  uniqstr आदान = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  अक्षर buf[256];
  माप_प्रकार len = आकार (buf);
  अक्षर *backup = asnprintf (buf, &len, "%s~", आदान);
  जर (!backup)
    xalloc_die ();
  जर (नाम (आदान, backup))
    error (निकास_त्रुटि, get_errno (),
           _("%s: cannot backup"), quotearg_colon (आदान));
  खाता *in = xfopen (backup, "r");
  खाता *out = xfopen (आदान, "w");
  माप_प्रकार ओळ = 1;
  माप_प्रकार offset = 1;
  शून्य स्थिरांक *p = रिक्त;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  पर्यंत (gl_list_iterator_next (&iter, &p, रिक्त))
    {
      fixit स्थिरांक *f = p;
      /* Look for the correct line. */
      पर्यंत (ओळ < f->location.start.ओळ)
        {
          पूर्णांक c = अ_लो (in);
          जर (c == खातापूर्ण)
            थांबा;
          जर (c == '\n')
            {
              ++ओळ;
              offset = 1;
            }
          अ_दो (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      पर्यंत (offset < f->location.start.byte)
        {
          पूर्णांक c = अ_लो (in);
          जर (c == खातापूर्ण)
            थांबा;
          ++offset;
          जर (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          अ_दो (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      ख_माला_दो (f->fix, out);

      /* Maybe install the eol afterwards.  */
      जर (need_eol)
        अ_दो ('\n', out);

      /* Skip the bad input. */
      पर्यंत (ओळ < f->location.end.ओळ)
        {
          पूर्णांक c = अ_लो (in);
          जर (c == खातापूर्ण)
            थांबा;
          जर (c == '\n')
            {
              ++ओळ;
              offset = 1;
            }
        }
      पर्यंत (offset < f->location.end.byte)
        {
          पूर्णांक c = अ_लो (in);
          जर (c == खातापूर्ण)
            थांबा;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      जर (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          पूर्णांक c = अ_लो (in);
          जर (c == खातापूर्ण)
            थांबा;
          अन्यथा जर (c == '\n')
            {
              ++ओळ;
              offset = 1;
            }
          अन्यथा
            अक्षर_वापस (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    पूर्णांक c;
    पर्यंत ((c = अ_लो (in)) != खातापूर्ण)
      अ_दो (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  ख_लिखो (मानक_त्रुटि, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, आदान), quote_n (1, backup));
  जर (backup != buf)
    मुक्त (backup);
}


/* Free the registered fixits.  */
शून्य fixits_free (शून्य)
{
  जर (fixits)
    {
      gl_list_free (fixits);
      fixits = रिक्त;
    }
}
