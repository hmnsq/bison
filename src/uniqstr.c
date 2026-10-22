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

#ضمن <config.h>
#ضمن "system.h"

#ضمن <attribute.h>
#ضمن <error.h>
#ضمن <hash.h>
#ضمن <quotearg.h>
#ضمن <stdarg.h>

#ضمن "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

ساكن بنية hash_table *uniqstrs_table = عدم;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (حرف ثابت *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  ܐܢ (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  ܦܢܐ res;
}

uniqstr
uniqstr_concat (صحيح nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  لكل (صحيح i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, حرف ثابت *));
  va_end (args);

  حرف *str = xmalloc (reslen + 1);
  حرف *p = str;

  va_start (args, nargs);
  لكل (صحيح i = 0; i < nargs; i++)
    {
      حرف ثابت *arg = va_arg (args, حرف ثابت *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  ܐܢ (res != str)
    حرر (str);
  ܦܢܐ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

فراغ
uniqstr_assert (حرف ثابت *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  ܐܢ (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

ساكن مضمن bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  ܦܢܐ true;
}

ساكن bool
uniqstr_print_processor (فراغ *ustr, فراغ *null MAYBE_UNUSED)
{
  ܦܢܐ uniqstr_print (ustr);
}

صحيح
uniqstr_cmp (uniqstr l, uniqstr r)
{
  ܦܢܐ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

ساكن bool
hash_compare_uniqstr (فراغ ثابت *m1, فراغ ثابت *m2)
{
  ܦܢܐ STREQ (m1, m2);
}

ساكن size_t
hash_uniqstr (فراغ ثابت *m, size_t tablesize)
{
  ܦܢܐ hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

فراغ
uniqstrs_new (فراغ)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     عدم,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     حرر);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

ساكن فراغ
uniqstrs_do (Hash_processor processor, فراغ *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

فراغ
uniqstrs_print (فراغ)
{
  uniqstrs_do (uniqstr_print_processor, عدم);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

فراغ
uniqstrs_free (فراغ)
{
  hash_free (uniqstrs_table);
}
