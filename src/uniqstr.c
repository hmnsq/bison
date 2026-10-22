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
#تعریف_کړه HT_INITIAL_ظرفیت 257

ساكن بنية hash_جدول *uniqstrs_جدول = عدم;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (حرف ثابت *str)
{
  uniqstr res = hash_lookup (uniqstrs_جدول, str);
  إذا (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_جدول, res);
    }
  أرجع res;
}

uniqstr
uniqstr_concat (صحيح nargs, ...)
{
  va_list دلیلونه;

  va_پیل (دلیلونه, nargs);
  size_t reslen = 0;
  لكل (صحيح i = 0; i < nargs; i++)
    reslen += مزي_اوږدوالی (va_arg (دلیلونه, حرف ثابت *));
  va_پای_ټکی (دلیلونه);

  حرف *str = xmalloc (reslen + 1);
  حرف *p = str;

  va_پیل (دلیلونه, nargs);
  لكل (صحيح i = 0; i < nargs; i++)
    {
      حرف ثابت *arg = va_arg (دلیلونه, حرف ثابت *);
      size_t arglen = مزي_اوږدوالی (arg);
      حافظه_نقل (p, arg, arglen);
      p += arglen;
    }
  va_پای_ټکی (دلیلونه);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_جدول, str);
  إذا (res != str)
    حرر (str);
  أرجع res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

فراغ
uniqstr_assert (حرف ثابت *str)
{
  uniqstr s = hash_lookup (uniqstrs_جدول, str);
  إذا (!s || s != str)
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
  أرجع true;
}

ساكن bool
uniqstr_print_processor (فراغ *ustr, فراغ *null MAYBE_ناکارول_شوی)
{
  أرجع uniqstr_print (ustr);
}

صحيح
uniqstr_cmp (uniqstr l, uniqstr r)
{
  أرجع (l == r ? 0
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
  أرجع STREQ (m1, m2);
}

ساكن size_t
hash_uniqstr (فراغ ثابت *m, size_t tablesize)
{
  أرجع hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

فراغ
uniqstrs_new (فراغ)
{
  uniqstrs_جدول = hash_xinitialize (HT_INITIAL_ظرفیت,
                                     عدم,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     حرر);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

ساكن فراغ
uniqstrs_do (Hash_processor processor, فراغ *processor_معلومات)
{
  hash_do_for_each (uniqstrs_جدول, processor, processor_معلومات);
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
  hash_free (uniqstrs_جدول);
}
