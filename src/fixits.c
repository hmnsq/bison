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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ "fixits.h"

#അന്തര്ഭാവ <error.h>
#അന്തര്ഭാവ <get-errno.h>
#അന്തര്ഭാവ <gl_array_list.h>
#അന്തര്ഭാവ <gl_xlist.h>
#അന്തര്ഭാവ <progname.h>
#അന്തര്ഭാവ <quote.h>
#അന്തര്ഭാവ <quotearg.h>
#അന്തര്ഭാവ <vasnprintf.h>

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "files.h"
#അന്തര്ഭാവ "getargs.h"

തരനിർവചനം ഘടന
{
  location location;
  അക്ഷരം *fix;
} fixit;

gl_list_t fixits = ശൂന്യ;

സ്ഥിതം fixit *
fixit_new (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം* fix)
{
  fixit *res = xmalloc (വലുപ്പം *res);
  res->location = *loc;
  res->fix = xstrdup (fix);
  തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
fixit_cmp (സ്ഥിരം  fixit *a, സ്ഥിരം fixit *b)
{
  തിരികെ location_cmp (a->location, b->location);
}

സ്ഥിതം ശൂന്യം
fixit_free (fixit *f)
{
  സ്വതന്ത്രം (f->fix);
  സ്വതന്ത്രം (f);
}


/* GCC and Clang follow the same pattern.
   https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html
   https://clang.llvm.org/docs/UsersManual.html#cmdoption-fdiagnostics-parseable-fixits */
സ്ഥിതം ശൂന്യം
fixit_print (fixit സ്ഥിരം *f, FILE *out)
{
  fprintf (out, "fix-it:%s:{%d:%d-%d:%d}:%s\n",
           quotearg_n_style (1, c_quoting_style, f->location.start.file),
           f->location.start.വരി, f->location.start.byte,
           f->location.end.വരി, f->location.end.byte,
           quotearg_n_style (2, c_quoting_style, f->fix));
}


ശൂന്യം
fixits_register (location സ്ഥിരം *loc, അക്ഷരം സ്ഥിരം* fix)
{
  എങ്കിൽ (!fixits)
    fixits = gl_list_create_empty (GL_ARRAY_LIST,
                                   /* equals */ ശൂന്യ,
                                   /* hashcode */ ശൂന്യ,
                                   (gl_listelement_dispose_fn) fixit_free,
                                   true);
  fixit *f = fixit_new (loc, fix);
  gl_sortedlist_add (fixits, (gl_listelement_compar_fn) fixit_cmp, f);
  എങ്കിൽ (feature_flag & feature_fixit)
    fixit_print (f, stderr);
}


bool
fixits_empty (ശൂന്യം)
{
  തിരികെ !fixits;
}


ശൂന്യം
fixits_run (ശൂന്യം)
{
  എങ്കിൽ (!fixits)
    തിരികെ;

  /* This is not unlike what is done in location_caret.  */
  uniqstr നിവേശം = ((fixit *) gl_list_get_at (fixits, 0))->location.start.file;
  /* Backup the file. */
  അക്ഷരം buf[256];
  size_t len = വലുപ്പം (buf);
  അക്ഷരം *backup = asnprintf (buf, &len, "%s~", നിവേശം);
  എങ്കിൽ (!backup)
    xalloc_die ();
  എങ്കിൽ (rename (നിവേശം, backup))
    error (EXIT_FAILURE, get_errno (),
           _("%s: cannot backup"), quotearg_colon (നിവേശം));
  FILE *in = xfopen (backup, "r");
  FILE *out = xfopen (നിവേശം, "w");
  size_t വരി = 1;
  size_t offset = 1;
  ശൂന്യം സ്ഥിരം *p = ശൂന്യ;
  gl_list_iterator_t iter = gl_list_iterator (fixits);
  ആകുമ്പോൾ (gl_list_iterator_next (&iter, &p, ശൂന്യ))
    {
      fixit സ്ഥിരം *f = p;
      /* Look for the correct line. */
      ആകുമ്പോൾ (വരി < f->location.start.വരി)
        {
          പൂർണ്ണസംഖ്യ c = getc (in);
          എങ്കിൽ (c == EOF)
            നിർത്തുക;
          എങ്കിൽ (c == '\n')
            {
              ++വരി;
              offset = 1;
            }
          putc (c, out);
        }

      /* Look for the right offset. */
      bool need_eol = false;
      ആകുമ്പോൾ (offset < f->location.start.byte)
        {
          പൂർണ്ണസംഖ്യ c = getc (in);
          എങ്കിൽ (c == EOF)
            നിർത്തുക;
          ++offset;
          എങ്കിൽ (c == '\n')
            /* The position we are asked for is beyond the actual
               line: pad with spaces, and remember we need a \n.  */
            need_eol = true;
          putc (need_eol ? ' ' : c, out);
        }

      /* Paste the fix instead. */
      fputs (f->fix, out);

      /* Maybe install the eol afterwards.  */
      എങ്കിൽ (need_eol)
        putc ('\n', out);

      /* Skip the bad input. */
      ആകുമ്പോൾ (വരി < f->location.end.വരി)
        {
          പൂർണ്ണസംഖ്യ c = getc (in);
          എങ്കിൽ (c == EOF)
            നിർത്തുക;
          എങ്കിൽ (c == '\n')
            {
              ++വരി;
              offset = 1;
            }
        }
      ആകുമ്പോൾ (offset < f->location.end.byte)
        {
          പൂർണ്ണസംഖ്യ c = getc (in);
          എങ്കിൽ (c == EOF)
            നിർത്തുക;
          ++offset;
        }

      /* If erasing the content of a full line, also remove the
         end-of-line. */
      എങ്കിൽ (f->fix[0] == 0 && f->location.start.byte == 1)
        {
          പൂർണ്ണസംഖ്യ c = getc (in);
          എങ്കിൽ (c == EOF)
            നിർത്തുക;
          അല്ലെങ്കിൽ എങ്കിൽ (c == '\n')
            {
              ++വരി;
              offset = 1;
            }
          അല്ലെങ്കിൽ
            ungetc (c, in);
        }
    }
  /* Paste the rest of the file.  */
  {
    പൂർണ്ണസംഖ്യ c;
    ആകുമ്പോൾ ((c = getc (in)) != EOF)
      putc (c, out);
  }

  gl_list_iterator_free (&iter);
  xfclose (out);
  xfclose (in);
  fprintf (stderr, "%s: file %s was updated (backup: %s)\n",
           program_name, quote_n (0, നിവേശം), quote_n (1, backup));
  എങ്കിൽ (backup != buf)
    സ്വതന്ത്രം (backup);
}


/* Free the registered fixits.  */
ശൂന്യം fixits_free (ശൂന്യം)
{
  എങ്കിൽ (fixits)
    {
      gl_list_free (fixits);
      fixits = ശൂന്യ;
    }
}
