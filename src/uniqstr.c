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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <attribute.h>
#અન્તર્ભાવ <error.h>
#અન્તર્ભાવ <hash.h>
#અન્તર્ભાવ <quotearg.h>
#અન્તર્ભાવ <stdarg.h>

#અન્તર્ભાવ "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

સ્થિર સંરચના hash_table *uniqstrs_table = ખાલી;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (અક્ષર અચલ *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  જો (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  પાછા res;
}

uniqstr
uniqstr_concat (પૂર્ણાંક nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  માટે (પૂર્ણાંક i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, અક્ષર અચલ *));
  va_end (args);

  અક્ષર *str = xmalloc (reslen + 1);
  અક્ષર *p = str;

  va_start (args, nargs);
  માટે (પૂર્ણાંક i = 0; i < nargs; i++)
    {
      અક્ષર અચલ *arg = va_arg (args, અક્ષર અચલ *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  જો (res != str)
    મુક્ત (str);
  પાછા res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

શૂન્ય
uniqstr_assert (અક્ષર અચલ *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  જો (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

સ્થિર અંતઃરેખ bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  પાછા true;
}

સ્થિર bool
uniqstr_print_processor (શૂન્ય *ustr, શૂન્ય *null MAYBE_UNUSED)
{
  પાછા uniqstr_print (ustr);
}

પૂર્ણાંક
uniqstr_cmp (uniqstr l, uniqstr r)
{
  પાછા (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

સ્થિર bool
hash_compare_uniqstr (શૂન્ય અચલ *m1, શૂન્ય અચલ *m2)
{
  પાછા STREQ (m1, m2);
}

સ્થિર size_t
hash_uniqstr (શૂન્ય અચલ *m, size_t tablesize)
{
  પાછા hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

શૂન્ય
uniqstrs_new (શૂન્ય)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     ખાલી,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     મુક્ત);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

સ્થિર શૂન્ય
uniqstrs_do (Hash_processor processor, શૂન્ય *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

શૂન્ય
uniqstrs_print (શૂન્ય)
{
  uniqstrs_do (uniqstr_print_processor, ખાલી);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

શૂન્ય
uniqstrs_free (શૂન્ય)
{
  hash_free (uniqstrs_table);
}
