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

#समावेश <config.h>

#समावेश "fixits.h"

#समावेश <error.h>
#समावेश <get-errno.h>
#समावेश <gl_array_list.h>
#समावेश <gl_xlist.h>
#समावेश <progname.h>
#समावेश <quote.h>
#समावेश <quotearg.h>
#समावेश <vasnprintf.h>

#समावेश "system.h"

#समावेश "files.h"
#समावेश "getargs.h"

प्रकार काष्ठा
{
  location location;
  अक्षर *fix;
} fixit;

gl_list_t fixits = शून्य;

अटल fixit *
fixit_new (location स्थिर *loc, अक्षर स्थिर* fix)
{
  fixit *res = xmalloc (माप *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  वापस res;
}

अटल पूर्णांक
fixit_cmp (स्थिर  fixit *a, स्थिर fixit *b)
{
  वापस location_cmp (a->location, b->location);
}

अटल व्योम
fixit_free (fixit *f)
{
  मुक्त (f->fix);
  मुक्त (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
अटल व्योम
fixit_print (fixit स्थिर *f, खाता *out)
{
  ख_लिखो (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.पंक्ति, f->location.start.byte,
           f->location.end.पंक्ति, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


व्योम
fixits_register (location स्थिर *loc, अक्षर स्थिर* fix)
{
  अगर (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ शून्य,
                                   /* hashcode */ शून्य,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  अगर (feature_flag & feature_fixit)
    fixit_print (f, मानक_त्रुटि);
}


bool
fixits_empty (व्योम)
{
  वापस !fixits;
}


व्योम
fixits_run (व्योम)
{
  अगर (!fixits)
    वापस;

  /* This is not unlike what is done in location_caret.  */
  uniqstr निवेश = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  अक्षर buf[256];
  माप_प्रकार len = माप (buf);
  अक्षर *backup = asnprintf (buf, &len, "%s~", निवेश);
  अगर (!backup)
    xalloc_die ();
  अगर (नाम (निवेश, backup))
    error (निकास_त्रुटि, get_errno (),
           _("%s: cannot backup"), quotearg_colon (निवेश));
  खाता *in = xfopen (backup, "r");
  खाता *out = xfopen (निवेश, "w");
  माप_प्रकार पंक्ति = 1;
  माप_प्रकार offset = 1;
  व्योम स्थिर *p = शून्य;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  जबतक (gl_list_iterator_अग्र (&iter, &p, शून्य))
    {
      fixit स्थिर *f = p;
      /* Look for the correct line. */
      जबतक (पंक्ति < f->location.start.पंक्ति)
        {
          पूर्णांक c = अ_लो (in);
          अगर (c == खातापूर्ण)
            अवरोध;
          अगर (c == '\n')
            {
              ++पंक्ति;
              offset = 1;
            }
          अ_दो (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      जबतक (offset < f->location.start.byte)
        {
          पूर्णांक c = अ_लो (in);
          अगर (c == खातापूर्ण)
            अवरोध;
          ++offset;
          अगर (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          अ_दो (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      ख_माला_दो (f->fix, out);

      /* Maybe install the eol afterwards.  */
      अगर (need_eol)
        अ_दो ('\n', out);

      /* Skip the bad input. */
      जबतक (पंक्ति < f->location.end.पंक्ति)
        {
          पूर्णांक c = अ_लो (in);
          अगर (c == खातापूर्ण)
            अवरोध;
          अगर (c == '\n')
            {
              ++पंक्ति;
              offset = 1;
            }
        }
      जबतक (offset < f->location.end.byte)
        {
          पूर्णांक c = अ_लो (in);
          अगर (c == खातापूर्ण)
            अवरोध;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      अगर (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          पूर्णांक c = अ_लो (in);
          अगर (c == खातापूर्ण)
            अवरोध;
          अन्यथा अगर (c == '\n')
            {
              ++पंक्ति;
              offset = 1;
            }
          अन्यथा
            अक्षर_वापस (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    पूर्णांक c;
    जबतक ((c = अ_लो (in)) != खातापूर्ण)
      अ_दो (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  ख_लिखो (मानक_त्रुटि, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, निवेश), quote_n (1, backup));
  अगर (backup != buf)
    मुक्त (backup);
}


/* Free the registered fixits.  */
व्योम fixits_free (व्योम)
{
  अगर (fixits)
    {
      gl_list_free (fixits);
      fixits = शून्य;
    }
}
