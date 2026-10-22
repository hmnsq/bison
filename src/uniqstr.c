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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <attribute.h>
#ଅନ୍ତର୍ଭାଵ <error.h>
#ଅନ୍ତର୍ଭାଵ <hash.h>
#ଅନ୍ତର୍ଭାଵ <quotearg.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>

#ଅନ୍ତର୍ଭାଵ "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

ସ୍ଥିର ସଂରଚନା hash_table *uniqstrs_table = ଖାଲି;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (ଅକ୍ଷର ଅଚଳ *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  ଯଦି (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  ଫେରନ୍ତୁ res;
}

uniqstr
uniqstr_concat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, ଅକ୍ଷର ଅଚଳ *));
  va_end (args);

  ଅକ୍ଷର *str = xmalloc (reslen + 1);
  ଅକ୍ଷର *p = str;

  va_start (args, nargs);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nargs; i++)
    {
      ଅକ୍ଷର ଅଚଳ *arg = va_arg (args, ଅକ୍ଷର ଅଚଳ *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  ଯଦି (res != str)
    ମୁକ୍ତ (str);
  ଫେରନ୍ତୁ res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

ଶୂନ୍ୟ
uniqstr_assert (ଅକ୍ଷର ଅଚଳ *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  ଯଦି (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
uniqstr_print_processor (ଶୂନ୍ୟ *ustr, ଶୂନ୍ୟ *null MAYBE_UNUSED)
{
  ଫେରନ୍ତୁ uniqstr_print (ustr);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
uniqstr_cmp (uniqstr l, uniqstr r)
{
  ଫେରନ୍ତୁ (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

ସ୍ଥିର bool
hash_compare_uniqstr (ଶୂନ୍ୟ ଅଚଳ *m1, ଶୂନ୍ୟ ଅଚଳ *m2)
{
  ଫେରନ୍ତୁ STREQ (m1, m2);
}

ସ୍ଥିର size_t
hash_uniqstr (ଶୂନ୍ୟ ଅଚଳ *m, size_t tablesize)
{
  ଫେରନ୍ତୁ hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

ଶୂନ୍ୟ
uniqstrs_new (ଶୂନ୍ୟ)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     ଖାଲି,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     ମୁକ୍ତ);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ
uniqstrs_do (Hash_processor processor, ଶୂନ୍ୟ *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

ଶୂନ୍ୟ
uniqstrs_print (ଶୂନ୍ୟ)
{
  uniqstrs_do (uniqstr_print_processor, ଖାଲି);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

ଶୂନ୍ୟ
uniqstrs_free (ଶୂନ୍ୟ)
{
  hash_free (uniqstrs_table);
}
