/* Keep a unique copy of strings.

   Copyright (C) 2002-2005, 2009-2015, 2018-2022 Free Software
   Foundation, Inc.

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
#अन्तर्भाव "system.h"

#अन्तर्भाव <attribute.h>
#अन्तर्भाव <error.h>
#अन्तर्भाव <hash.h>
#अन्तर्भाव <quotearg.h>
#अन्तर्भाव <stdarg.h>

#अन्तर्भाव "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#परिभाषय HT_INITIAL_क्षमता 257

स्थिर संरचना hash_सारणी *uniqstrs_सारणी = रिक्त;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (वर्ण ध्रुव *str)
{
  uniqstr res = hash_lookup (uniqstrs_सारणी, str);
  यदि (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_सारणी, res);
    }
  प्रत्यागम res;
}

uniqstr
uniqstr_concat (पूर्णाङ्क nargs, ...)
{
  va_list तर्काः;

  va_start (तर्काः, nargs);
  size_t reslen = 0;
  प्रति (पूर्णाङ्क i = 0; i < nargs; i++)
    reslen += वाक्यदैर्घ्यम् (va_arg (तर्काः, वर्ण ध्रुव *));
  va_अन्तः (तर्काः);

  वर्ण *str = xmalloc (reslen + 1);
  वर्ण *p = str;

  va_start (तर्काः, nargs);
  प्रति (पूर्णाङ्क i = 0; i < nargs; i++)
    {
      वर्ण ध्रुव *arg = va_arg (तर्काः, वर्ण ध्रुव *);
      size_t arglen = वाक्यदैर्घ्यम् (arg);
      स्मृतिं_प्रतिलिख (p, arg, arglen);
      p += arglen;
    }
  va_अन्तः (तर्काः);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_सारणी, str);
  यदि (res != str)
    मुक्त (str);
  प्रत्यागम res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

शून्य
uniqstr_assert (वर्ण ध्रुव *str)
{
  uniqstr s = hash_lookup (uniqstrs_सारणी, str);
  यदि (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

स्थिर अन्तःस्थ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  प्रत्यागम true;
}

स्थिर bool
uniqstr_print_processor (शून्य *ustr, शून्य *null MAYBE_अप्रयुक्तम्)
{
  प्रत्यागम uniqstr_print (ustr);
}

पूर्णाङ्क
uniqstr_cmp (uniqstr l, uniqstr r)
{
  प्रत्यागम (l == r ? 0
          : !l ? -1
          : !r ? +1
          : वाक्यं_तुलय (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

स्थिर bool
hash_compare_uniqstr (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  प्रत्यागम STREQ (m1, m2);
}

स्थिर size_t
hash_uniqstr (शून्य ध्रुव *m, size_t tablesize)
{
  प्रत्यागम hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

शून्य
uniqstrs_new (शून्य)
{
  uniqstrs_सारणी = hash_xinitialize (HT_INITIAL_क्षमता,
                                     रिक्त,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     मुक्त);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

स्थिर शून्य
uniqstrs_do (Hash_processor processor, शून्य *processor_दत्त)
{
  hash_do_for_each (uniqstrs_सारणी, processor, processor_दत्त);
}


/*-----------------.
| Print them all.  |
`-----------------*/

शून्य
uniqstrs_print (शून्य)
{
  uniqstrs_do (uniqstr_print_processor, रिक्त);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

शून्य
uniqstrs_free (शून्य)
{
  hash_free (uniqstrs_सारणी);
}
