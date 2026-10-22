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

#ضمن <config.h>

#ضمن "fixits.h"

#ضمن <error.h>
#ضمن <get-errno.h>
#ضمن <gl_array_list.h>
#ضمن <gl_xlist.h>
#ضمن <progname.h>
#ضمن <quote.h>
#ضمن <quotearg.h>
#ضمن <vasnprintf.h>

#ضمن "system.h"

#ضمن "files.h"
#ضمن "getargs.h"

تعريفنوع بنية
{
  location location;
  حرف *fix;
} fixit;

gl_list_t fixits = عدم;

ساكن fixit *
fixit_new (location ثابت *loc, حرف ثابت* fix)
{
  fixit *res = xmalloc (حجم *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  أرجع res;
}

ساكن صحيح
fixit_cmp (ثابت  fixit *a, ثابت fixit *b)
{
  أرجع location_cmp (a->location, b->location);
}

ساكن فراغ
fixit_free (fixit *f)
{
  حرر (f->fix);
  حرر (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
ساكن فراغ
fixit_print (fixit ثابت *f, FILE *بهر)
{
  fprintf (بهر, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.پیل.file),
           f->location.پیل.کرښه, f->location.پیل.byte,
           f->location.پای_ټکی.کرښه, f->location.پای_ټکی.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


فراغ
fixits_ثبتوونکی (location ثابت *loc, حرف ثابت* fix)
{
  إذا (!fixits)
    fixits = gl_list_create_تشه (GL_ARRAY_LIST,
                                   /* equals */ عدم,
                                   /* hashcode */ عدم,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_جمع_کړه (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  إذا (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_تشه (فراغ)
{
  أرجع !fixits;
}


فراغ
fixits_پرلپسې (فراغ)
{
  إذا (!fixits)
    أرجع;

  /* This is not unlike what is done in location_caret.  */
  uniqstr ننوت = ((fixit *) gl_list_get_at (fixits, 0))->location.پیل.file;
  /* Backup the file. */
  حرف buf[256];
  size_t len = حجم (buf);
  حرف *backup = asnprintf (buf, &len, "%s~", ننوت);
  إذا (!backup)
    xalloc_die ();
  إذا (rename (ننوت, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (ننوت));
  FILE *دننه = xfopen (backup, "r");
  FILE *بهر = xfopen (ننوت, "w");
  size_t کرښه = 1;
  size_t دننه_واټن = 1;
  فراغ ثابت *p = عدم;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  طالما (gl_list_iterator_بل (&iter, &p, عدم))
    {
      fixit ثابت *f = p;
      /* Look for the correct line. */
      طالما (کرښه < f->location.پیل.کرښه)
        {
          صحيح c = getc (دننه);
          إذا (c == EOF)
            اكسر;
          إذا (c == '\n')
            {
              ++کرښه;
              دننه_واټن = 1;
            }
          putc (c, بهر);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      طالما (دننه_واټن < f->location.پیل.byte)
        {
          صحيح c = getc (دننه);
          إذا (c == EOF)
            اكسر;
          ++دننه_واټن;
          إذا (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, بهر);
        }

      /* Paste the fix instead. */
      fputs (f->fix, بهر);

      /* Maybe install the eol afterwards.  */
      إذا (need_eol)
        putc ('\n', بهر);

      /* Skip the bad input. */
      طالما (کرښه < f->location.پای_ټکی.کرښه)
        {
          صحيح c = getc (دننه);
          إذا (c == EOF)
            اكسر;
          إذا (c == '\n')
            {
              ++کرښه;
              دننه_واټن = 1;
            }
        }
      طالما (دننه_واټن < f->location.پای_ټکی.byte)
        {
          صحيح c = getc (دننه);
          إذا (c == EOF)
            اكسر;
          ++دننه_واټن;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      إذا (f->fix[0] == 0 && f->location.پیل.byte == 1)
        {
          صحيح c = getc (دننه);
          إذا (c == EOF)
            اكسر;
          وإلا إذا (c == '\n')
            {
              ++کرښه;
              دننه_واټن = 1;
            }
          وإلا
            ungetc (c, دننه);
        }
    }
  /* Paste the rest of the file.  */
  {
    صحيح c;
    طالما ((c = getc (دننه)) != EOF)
      putc (c, بهر);
  }

  gl_list_iterator_free (&iter);
  xfclose (بهر);
  xfclose (دننه);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           پروګرام_name, quote_n (0, ننوت), quote_n (1, backup));
  إذا (backup != buf)
    حرر (backup);
}


/* Free the registered fixits.  */
فراغ fixits_free (فراغ)
{
  إذا (fixits)
    {
      gl_list_free (fixits);
      fixits = عدم;
    }
}
