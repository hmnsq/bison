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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ <attribute.h>
#அந்தர்பாவ <error.h>
#அந்தர்பாவ <hash.h>
#அந்தர்பாவ <quotearg.h>
#அந்தர்பாவ <stdarg.h>

#அந்தர்பாவ "uniqstr.h"

/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

/* Initial capacity of uniqstr hash table.  */
#define HT_INITIAL_CAPACITY 257

நிலையான கட்டமைப்பு hash_table *uniqstrs_table = வெற்று;

/*-------------------------------------.
| Create the uniqstr for S if needed.  |
`-------------------------------------*/

uniqstr
uniqstr_new (எழுத்து மாறிலி *str)
{
  uniqstr res = hash_lookup (uniqstrs_table, str);
  எனில் (!res)
    {
      /* First insertion in the hash. */
      res = xstrdup (str);
      hash_xinsert (uniqstrs_table, res);
    }
  திரும்பு res;
}

uniqstr
uniqstr_concat (முழு nargs, ...)
{
  va_list args;

  va_start (args, nargs);
  size_t reslen = 0;
  க்கு (முழு i = 0; i < nargs; i++)
    reslen += strlen (va_arg (args, எழுத்து மாறிலி *));
  va_end (args);

  எழுத்து *str = xmalloc (reslen + 1);
  எழுத்து *p = str;

  va_start (args, nargs);
  க்கு (முழு i = 0; i < nargs; i++)
    {
      எழுத்து மாறிலி *arg = va_arg (args, எழுத்து மாறிலி *);
      size_t arglen = strlen (arg);
      memcpy (p, arg, arglen);
      p += arglen;
    }
  va_end (args);

  *p = '\0';
  uniqstr res = hash_xinsert (uniqstrs_table, str);
  எனில் (res != str)
    விடுவி (str);
  திரும்பு res;
}

/*------------------------------.
| Abort if S is not a uniqstr.  |
`------------------------------*/

வெறுமை
uniqstr_assert (எழுத்து மாறிலி *str)
{
  uniqstr s = hash_lookup (uniqstrs_table, str);
  எனில் (!s || s != str)
    {
      error (0, 0,
             "not a uniqstr: %s", quotearg (str));
      abort ();
    }
}


/*--------------------.
| Print the uniqstr.  |
`--------------------*/

நிலையான உள்வரி bool
uniqstr_print (uniqstr ustr)
{
  fprintf (stderr, "%s\n", ustr);
  திரும்பு true;
}

நிலையான bool
uniqstr_print_processor (வெறுமை *ustr, வெறுமை *null MAYBE_UNUSED)
{
  திரும்பு uniqstr_print (ustr);
}

முழு
uniqstr_cmp (uniqstr l, uniqstr r)
{
  திரும்பு (l == r ? 0
          : !l ? -1
          : !r ? +1
          : strcmp (l, r));
}


/*-----------------------.
| A uniqstr hash table.  |
`-----------------------*/

நிலையான bool
hash_compare_uniqstr (வெறுமை மாறிலி *m1, வெறுமை மாறிலி *m2)
{
  திரும்பு STREQ (m1, m2);
}

நிலையான size_t
hash_uniqstr (வெறுமை மாறிலி *m, size_t tablesize)
{
  திரும்பு hash_string (m, tablesize);
}


/*----------------------------.
| Create the uniqstrs table.  |
`----------------------------*/

வெறுமை
uniqstrs_new (வெறுமை)
{
  uniqstrs_table = hash_xinitialize (HT_INITIAL_CAPACITY,
                                     வெற்று,
                                     hash_uniqstr,
                                     hash_compare_uniqstr,
                                     விடுவி);
}


/*-------------------------------------.
| Perform a task on all the uniqstrs.  |
`-------------------------------------*/

நிலையான வெறுமை
uniqstrs_do (Hash_processor processor, வெறுமை *processor_data)
{
  hash_do_for_each (uniqstrs_table, processor, processor_data);
}


/*-----------------.
| Print them all.  |
`-----------------*/

வெறுமை
uniqstrs_print (வெறுமை)
{
  uniqstrs_do (uniqstr_print_processor, வெற்று);
}


/*--------------------.
| Free the uniqstrs.  |
`--------------------*/

வெறுமை
uniqstrs_free (வெறுமை)
{
  hash_free (uniqstrs_table);
}
