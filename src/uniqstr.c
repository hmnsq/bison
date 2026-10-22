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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব <attribute.h>
#অন্তর্ভাব <error.h>
#অন্তর্ভাব <hash.h>
#অন্তর্ভাব <quotearg.h>
#অন্তর্ভাব <stdarg.h>

#অন্তর্ভাব "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

স্থির কাঠামো hash_table *uniqstrs_table = ফাঁকা;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (অক্ষর ধ্রুবক *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  যদি (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  ফেরত res;
}

uniqstr
uniqstr_concat (পূর্ণসংখ্যা nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  জন্য (পূর্ণসংখ্যা i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, অক্ষর ধ্রুবক *));
  va_end (args);

  অক্ষর *str = xmalloc (reslen + 1);
  অক্ষর *p = str;

  va_start (args, nargs);
  জন্য (পূর্ণসংখ্যা i = 0; i < nargs; i++)
    {
      অক্ষর ধ্রুবক *arg = va_arg (args, অক্ষর ধ্রুবক *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  যদি (res != str)
    মুক্ত (str);
  ফেরত res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

শূন্য
uniqstr_assert (অক্ষর ধ্রুবক *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  যদি (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

স্থির অন্তঃরেখ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  ফেরত true;
}

স্থির bool
uniqstr_print_processor (শূন্য *ustr, শূন্য *null MAYBE_UNUSED)
{
  ফেরত uniqstr_print (ustr);
}

পূর্ণসংখ্যা
uniqstr_cmp (uniqstr l, uniqstr r)
{
  ফেরত (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

স্থির bool
hash_compare_uniqstr (শূন্য ধ্রুবক *m1, শূন্য ধ্রুবক *m2)
{
  ফেরত STREQ (m1, m2);
}

স্থির size_t
hash_uniqstr (শূন্য ধ্রুবক *m, size_t tablesize)
{
  ফেরত hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

শূন্য
uniqstrs_new (শূন্য)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     ফাঁকা,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     মুক্ত);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

স্থির শূন্য
uniqstrs_do (Hash_processor processor, শূন্য *processor_তথ্য)
{
  hash_do_for_each (uniqstrs_table, processor, processor_তথ্য);
}


/*-----------------.
| Print them all.  |
`-----------------*/

শূন্য
uniqstrs_print (শূন্য)
{
  uniqstrs_do (uniqstr_print_processor, ফাঁকা);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

শূন্য
uniqstrs_free (শূন্য)
{
  hash_free (uniqstrs_table);
}
