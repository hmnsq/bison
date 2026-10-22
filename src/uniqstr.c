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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <attribute.h>
#ಅನ್ತರ್ಭಾವ <error.h>
#ಅನ್ತರ್ಭಾವ <hash.h>
#ಅನ್ತರ್ಭಾವ <quotearg.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಅನ್ತರ್ಭಾವ "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

ಸ್ಥಿರ ಸಂರಚನಾ hash_table *uniqstrs_table = ರಿಕ್ತ;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (ವರ್ಣ ಧ್ರುವ *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  ಯದಿ (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  ಪ್ರತ್ಯಾಗಮ res;
}

uniqstr
uniqstr_concat (ಪೂರ್ಣಾಙ್ಕ nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, ವರ್ಣ ಧ್ರುವ *));
  va_end (args);

  ವರ್ಣ *str = xmalloc (reslen + 1);
  ವರ್ಣ *p = str;

  va_start (args, nargs);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nargs; i++)
    {
      ವರ್ಣ ಧ್ರುವ *arg = va_arg (args, ವರ್ಣ ಧ್ರುವ *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  ಯದಿ (res != str)
    ಮುಕ್ತ (str);
  ಪ್ರತ್ಯಾಗಮ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

ಶೂನ್ಯ
uniqstr_assert (ವರ್ಣ ಧ್ರುವ *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  ಯದಿ (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
uniqstr_print_processor (ಶೂನ್ಯ *ustr, ಶೂನ್ಯ *null MAYBE_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ uniqstr_print (ustr);
}

ಪೂರ್ಣಾಙ್ಕ
uniqstr_cmp (uniqstr l, uniqstr r)
{
  ಪ್ರತ್ಯಾಗಮ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

ಸ್ಥಿರ bool
hash_compare_uniqstr (ಶೂನ್ಯ ಧ್ರುವ *m1, ಶೂನ್ಯ ಧ್ರುವ *m2)
{
  ಪ್ರತ್ಯಾಗಮ STREQ (m1, m2);
}

ಸ್ಥಿರ size_t
hash_uniqstr (ಶೂನ್ಯ ಧ್ರುವ *m, size_t tablesize)
{
  ಪ್ರತ್ಯಾಗಮ hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

ಶೂನ್ಯ
uniqstrs_new (ಶೂನ್ಯ)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     ರಿಕ್ತ,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     ಮುಕ್ತ);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ
uniqstrs_do (Hash_processor processor, ಶೂನ್ಯ *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

ಶೂನ್ಯ
uniqstrs_print (ಶೂನ್ಯ)
{
  uniqstrs_do (uniqstr_print_processor, ರಿಕ್ತ);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

ಶೂನ್ಯ
uniqstrs_free (ಶೂನ್ಯ)
{
  hash_free (uniqstrs_table);
}
