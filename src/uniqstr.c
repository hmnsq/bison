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

#شامل <config.h>
#شامل "system.h"

#شامل <attribute.h>
#شامل <error.h>
#شامل <hash.h>
#شامل <quotearg.h>
#شامل <stdarg.h>

#شامل "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

ایستا ساختار hash_table *uniqstrs_table = پوچ;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (نویسه ثابت *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  اگر (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  بازگشت res;
}

uniqstr
uniqstr_concat (صحیح nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  برای (صحیح i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, نویسه ثابت *));
  va_end (args);

  نویسه *str = xmalloc (reslen + 1);
  نویسه *p = str;

  va_start (args, nargs);
  برای (صحیح i = 0; i < nargs; i++)
    {
      نویسه ثابت *arg = va_arg (args, نویسه ثابت *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  اگر (res != str)
    آزاد (str);
  بازگشت res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

تهی
uniqstr_assert (نویسه ثابت *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  اگر (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

ایستا درونخطی bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  بازگشت true;
}

ایستا bool
uniqstr_print_processor (تهی *ustr, تهی *null MAYBE_UNUSED)
{
  بازگشت uniqstr_print (ustr);
}

صحیح
uniqstr_cmp (uniqstr l, uniqstr r)
{
  بازگشت (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

ایستا bool
hash_compare_uniqstr (تهی ثابت *m1, تهی ثابت *m2)
{
  بازگشت STREQ (m1, m2);
}

ایستا size_t
hash_uniqstr (تهی ثابت *m, size_t tablesize)
{
  بازگشت hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

تهی
uniqstrs_new (تهی)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     پوچ,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     آزاد);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

ایستا تهی
uniqstrs_do (Hash_processor processor, تهی *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

تهی
uniqstrs_print (تهی)
{
  uniqstrs_do (uniqstr_print_processor, پوچ);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

تهی
uniqstrs_free (تهی)
{
  hash_free (uniqstrs_table);
}
