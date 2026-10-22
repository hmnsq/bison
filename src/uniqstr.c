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
#define HT_INITIAL_CAPACITY 257

स्थिर संरचना hash_table *uniqstrs_table = रिक्त;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (वर्ण ध्रुव *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  सचे (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  पटिनिवत्त res;
}

uniqstr
uniqstr_concat (पूर्णाङ्क nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  अत्थाय (पूर्णाङ्क i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, वर्ण ध्रुव *));
  va_end (args);

  वर्ण *str = xmalloc (reslen + 1);
  वर्ण *p = str;

  va_start (args, nargs);
  अत्थाय (पूर्णाङ्क i = 0; i < nargs; i++)
    {
      वर्ण ध्रुव *arg = va_arg (args, वर्ण ध्रुव *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  सचे (res != str)
    मुक्त (str);
  पटिनिवत्त res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

शून्य
uniqstr_assert (वर्ण ध्रुव *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  सचे (!s || s != str)
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
  पटिनिवत्त true;
}

स्थिर bool
uniqstr_print_processor (शून्य *ustr, शून्य *null MAYBE_UNUSED)
{
  पटिनिवत्त uniqstr_print (ustr);
}

पूर्णाङ्क
uniqstr_cmp (uniqstr l, uniqstr r)
{
  पटिनिवत्त (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

स्थिर bool
hash_compare_uniqstr (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  पटिनिवत्त STREQ (m1, m2);
}

स्थिर size_t
hash_uniqstr (शून्य ध्रुव *m, size_t tablesize)
{
  पटिनिवत्त hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

शून्य
uniqstrs_new (शून्य)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     रिक्त,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     मुक्त);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

स्थिर शून्य
uniqstrs_do (Hash_processor processor, शून्य *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
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
  hash_free (uniqstrs_table);
}
